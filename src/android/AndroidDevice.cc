/******************************************************************************
 * Copyright 2013, Qualcomm Innovation Center, Inc.
 *
 *    All rights reserved.
 *    This file is licensed under the 3-clause BSD license in the NOTICE.txt
 *    file for this project. A copy of the 3-clause BSD license is found at:
 *
 *        http://opensource.org/licenses/BSD-3-Clause.
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the license is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the license for the specific language governing permissions and
 *    limitations under the license.
 ******************************************************************************/

#include <alljoyn/audio/android/AndroidDevice.h>

#include "../Sink.h"
#include "../Clock.h"
#include <qcc/Debug.h>
#include <malloc.h>
#include <math.h>
#include <stdlib.h>

#define QCC_MODULE "ALLJOYN_AUDIO"

#define NUM_BUFFERS 2
#define BYTES_PER_FRAME 4096

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

namespace ajn {
namespace services {

AndroidDevice::AndroidDevice() :
    mMute(false), mVolumeValue(100), mSLEngineObject(NULL), mOutputMixObject(NULL), mBufferQueuePlayerObject(NULL),
    mBufferQueue(NULL), mNumberOfFrames(0), mBuffersAvailable(2), mBufferIndex(0)
{
    mBufferMutex = new qcc::Mutex();
    mAudioBuffers = new uint8_t *[NUM_BUFFERS];
    for (int i = 0; i < NUM_BUFFERS; i++) {
        mAudioBuffers[i] = NULL;
    }
}

void AndroidDevice::bqPlayerCallback(SLAndroidSimpleBufferQueueItf bq, void* context)
{
    AndroidDevice* androidDevice = (AndroidDevice*)context;
    androidDevice->mBufferMutex->Lock();
    androidDevice->mBuffersAvailable++;
    androidDevice->mBufferMutex->Unlock();
}

bool AndroidDevice::Open(const char*format, uint32_t sampleRate, uint32_t numChannels, uint32_t& bufferSize)
{

    SLresult result;
    //Step 1 crate OpenSL audio engine
    result = slCreateEngine(&mSLEngineObject, 0, NULL, 0, NULL, NULL);
    if (SL_RESULT_SUCCESS != result) {
        return false;
    }

    //Now realize the engine so we can use it
    result = (*mSLEngineObject)->Realize(mSLEngineObject, SL_BOOLEAN_FALSE);
    if (SL_RESULT_SUCCESS != result) {
        return false;
    }

    //Get the interface for the EngineObject so we can use it to setup everything else
    result = (*mSLEngineObject)->GetInterface(mSLEngineObject, SL_IID_ENGINE, &mSLEngine);
    if (SL_RESULT_SUCCESS != result) {
        return false;
    }

    //Create an OutputMix so that we can write audio
    result = (*mSLEngine)->CreateOutputMix(mSLEngine, &mOutputMixObject, 0, NULL, NULL);
    if (SL_RESULT_SUCCESS != result) {
        return false;
    }

    //Realize the OutputMix so it is active
    result = (*mOutputMixObject)->Realize(mOutputMixObject, SL_BOOLEAN_FALSE);
    if (SL_RESULT_SUCCESS != result) {
        return false;
    }

    //Setup the buffer to be able to stream the PCM audio data
    SLDataLocator_AndroidSimpleBufferQueue androidBuffers = { SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, NUM_BUFFERS };
    //setup the PCM audio format
    SLDataFormat_PCM pcmDataFormat = { SL_DATAFORMAT_PCM, numChannels, sampleRate * 1000,
                                       SL_PCMSAMPLEFORMAT_FIXED_16, SL_PCMSAMPLEFORMAT_FIXED_16,
                                       numChannels == 1 ? SL_SPEAKER_FRONT_CENTER
                                       : SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,
                                       SL_BYTEORDER_LITTLEENDIAN };
    SLDataSource dataSource = { &androidBuffers, &pcmDataFormat };

    // configure audio sink
    SLDataLocator_OutputMix dataLocatorOutputMix = { SL_DATALOCATOR_OUTPUTMIX, mOutputMixObject };
    SLDataSink dataSink = { &dataLocatorOutputMix, NULL };

    // create audio player
    const SLInterfaceID interfaceIds[] = { SL_IID_PLAY, SL_IID_VOLUME, SL_IID_BUFFERQUEUE };
    const SLboolean requiredInterfaces[] = { SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE };
    //Construct the AudioPlayer
    result = (*mSLEngine)->CreateAudioPlayer(mSLEngine, &mBufferQueuePlayerObject, &dataSource, &dataSink, ARRAY_SIZE(interfaceIds), interfaceIds, requiredInterfaces);
    if (SL_RESULT_SUCCESS != result) {
        return false;
    }

    //Realize the created AudioPlayer
    result = (*mBufferQueuePlayerObject)->Realize(mBufferQueuePlayerObject, SL_BOOLEAN_FALSE);
    if (SL_RESULT_SUCCESS != result) {
        return false;
    }

    //Get the Play interface so we can play/pause the driver
    result = (*mBufferQueuePlayerObject)->GetInterface(mBufferQueuePlayerObject, SL_IID_PLAY, &mPlay);
    if (SL_RESULT_SUCCESS != result) {
        return false;
    }

    //Get the Volume interface so we can control/set the volume level and mute the device.
    result = (*mBufferQueuePlayerObject)->GetInterface(mBufferQueuePlayerObject, SL_IID_VOLUME, &mVolume);
    if (SL_RESULT_SUCCESS != result) {
        return false;
    }

    //Get the Buffer interface so we can queue streaming PCM audio data
    result = (*mBufferQueuePlayerObject)->GetInterface(mBufferQueuePlayerObject, SL_IID_BUFFERQUEUE, &mBufferQueue);
    if (SL_RESULT_SUCCESS != result) {
        return false;
    }

    //Register callback function to trigger upon completion of PCM audio data
    result = (*mBufferQueue)->RegisterCallback(mBufferQueue, bqPlayerCallback, this);
    if (SL_RESULT_SUCCESS != result) {
        return false;
    }

    //Start playing so when we first write PCM audio data to the buffer it will play
    result = (*mPlay)->SetPlayState(mPlay, SL_PLAYSTATE_PLAYING);
    if (SL_RESULT_SUCCESS != result) {
        return false;
    }

    //Compute the frame size and return how many bytes we want the AllJoyn Audio Streaming Engine to buffer
    mNumberOfFrames = (16 >> 3) * numChannels;
    bufferSize = BYTES_PER_FRAME * mNumberOfFrames;
    for (int i = 0; i < NUM_BUFFERS; i++) {
        if (mAudioBuffers[i] == NULL)
            mAudioBuffers[i] = (uint8_t*)malloc(bufferSize * sizeof(uint8_t));
    }
    mBuffersAvailable = 2;
    mBufferIndex = 0;

    return true;
}

void AndroidDevice::Close()
{
    if (mBufferQueue) {
        (*mBufferQueue)->Clear(mBufferQueue);
        mBuffersAvailable = 2;
        mBufferIndex = 0;
    }
}

AndroidDevice::~AndroidDevice()
{
    if (mBufferMutex) {
        delete mBufferMutex;
        mBufferMutex = NULL;
    }

    if (mPlay) {
        (*mPlay)->SetPlayState(mPlay, SL_PLAYSTATE_STOPPED);
        mPlay = NULL;
    }

    if (mBufferQueuePlayerObject != NULL) {
        (*mBufferQueuePlayerObject)->Destroy(mBufferQueuePlayerObject);
        mBufferQueuePlayerObject = NULL;
        mBufferQueue = NULL;
        mVolume = NULL;
    }

    if (mOutputMixObject != NULL) {
        (*mOutputMixObject)->Destroy(mOutputMixObject);
        mOutputMixObject = NULL;
    }

    if (mSLEngineObject != NULL) {
        (*mSLEngineObject)->Destroy(mSLEngineObject);
        mSLEngineObject = NULL;
        mSLEngine = NULL;
    }

    for (int i = 0; i < NUM_BUFFERS; i++) {
        if (mAudioBuffers[i]) {
            delete mAudioBuffers[i];
            mAudioBuffers[i] = NULL;
        }
    }
    delete[] mAudioBuffers;
    mAudioBuffers = NULL;
}

bool AndroidDevice::Pause()
{
    // set the player's state to playing
    SLresult result = (*mPlay)->SetPlayState(mPlay, SL_PLAYSTATE_PAUSED);
    if (SL_RESULT_SUCCESS != result) {
        return false;
    }
    return true;
}

bool AndroidDevice::Play()
{
    // set the player's state to playing
    SLresult result = (*mPlay)->SetPlayState(mPlay, SL_PLAYSTATE_PLAYING);
    if (SL_RESULT_SUCCESS != result) {
        return false;
    }
    return true;
}

bool AndroidDevice::Recover()
{
    return true;
}

uint32_t AndroidDevice::GetDelay()
{
    return 120000;
}

uint32_t AndroidDevice::GetFramesWanted()
{
    return 1;
}

bool AndroidDevice::Write(const uint8_t*buffer, uint32_t bufferSizeInFrames)
{
    while (mBuffersAvailable == 0) {
        //block and wait for a buffer to free up
        SleepNanos(1000000);         // 1/1000th of a second
    }
    mBufferMutex->Lock();
    bufferSizeInFrames *= mNumberOfFrames;
    SLresult result;
    memset(mAudioBuffers[mBufferIndex], 0, bufferSizeInFrames);
    memcpy(mAudioBuffers[mBufferIndex], buffer, bufferSizeInFrames);
    result = (*mBufferQueue)->Enqueue(mBufferQueue, mAudioBuffers[mBufferIndex], bufferSizeInFrames);
    if (SL_RESULT_SUCCESS != result) {
        mBufferMutex->Unlock();
        return false;
    }
    mBuffersAvailable--;
    mBufferIndex = !mBufferIndex;
    mBufferMutex->Unlock();
    return true;
}

bool AndroidDevice::GetMute(bool& mute)
{
    if (!mVolume)
        return false;
    SLboolean value;
    SLresult result = (*mVolume)->GetMute(mVolume, &value);
    mute = value == SL_BOOLEAN_TRUE;
    return result == SL_RESULT_SUCCESS;
}

bool AndroidDevice::SetMute(bool mute)
{
    if (!mVolume)
        return false;
    SLresult result = (*mVolume)->SetMute(mVolume, mute ? SL_BOOLEAN_TRUE : SL_BOOLEAN_FALSE);
    return result == SL_RESULT_SUCCESS;
}

bool AndroidDevice::GetVolumeRange(int16_t& low, int16_t& high, int16_t& step) {
    // TODO
    return false;
}


bool AndroidDevice::GetVolume(int16_t& volume)
{
    if (!mVolume)
        return false;
    SLmillibel mCurrentVolume;
    SLresult result = (*mVolume)->GetVolumeLevel(mVolume, &mCurrentVolume);
    if (SL_RESULT_SUCCESS == result) {
        volume = mCurrentVolume / -50;
        return true;
    }
    return false;
}

bool AndroidDevice::SetVolume(int16_t newVolume)
{
    if (!mVolume)
        return false;
    SLresult result;
    SLmillibel mCurrentVolume;
    SLmillibel mNewVolume = newVolume * -50;
    result = (*mVolume)->SetVolumeLevel(mVolume, mNewVolume);
    result = (*mVolume)->GetVolumeLevel(mVolume, &mCurrentVolume);
    if (SL_RESULT_SUCCESS == result) {
        return true;
    }
    return false;
}

void AndroidDevice::AddListener(AudioDeviceListener* listener)
{
    mListenersMutex.Lock();
    mListeners.insert(listener);
    mListenersMutex.Unlock();
}

void AndroidDevice::RemoveListener(AudioDeviceListener* listener)
{
    mListenersMutex.Lock();
    Listeners::iterator it = mListeners.find(listener);
    if (it != mListeners.end())
        mListeners.erase(it);
    mListenersMutex.Unlock();
}

}
}
