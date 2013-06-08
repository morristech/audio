/**
 * @file
 * Audio device access using the Android NDK API.
 */
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

#ifndef _ANDROIDDEVICE_H
#define _ANDROIDDEVICE_H

#ifndef __cplusplus
#error Only include AndroidDevice.h in C++ code.
#endif

#include <alljoyn/audio/AudioDevice.h>
#include <qcc/Mutex.h>
#include <set>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

namespace ajn {
namespace services {

/**
 * A subclass of AudioDevice that implements access to an audio device using the Android NDK API.
 */
class AndroidDevice : public AudioDevice {
  public:
    AndroidDevice();
    ~AndroidDevice();

    bool Open(const char*format, uint32_t sampleRate, uint32_t numChannels, uint32_t& bufferSize);
    void Close();
    bool Pause();
    bool Play();
    bool Recover();
    uint32_t GetDelay();
    uint32_t GetFramesWanted();
    bool Write(const uint8_t*buffer, uint32_t bufferSizeInFrames);

    bool GetMute(bool& mute);
    bool SetMute(bool mute);
    bool GetVolumeRange(int16_t& low, int16_t& high, int16_t& step);
    bool GetVolume(int16_t& volume);
    bool SetVolume(int16_t volume);

    void AddListener(AudioDeviceListener* listener);
    void RemoveListener(AudioDeviceListener* listener);

  private:
    typedef std::set<AudioDeviceListener*> Listeners;

    static void bqPlayerCallback(SLAndroidSimpleBufferQueueItf bq, void* context);

    bool mMute;
    int16_t mVolumeValue;
    SLObjectItf mSLEngineObject;
    SLEngineItf mSLEngine;
    SLObjectItf mOutputMixObject;
    SLObjectItf mBufferQueuePlayerObject;
    SLPlayItf mPlay;
    SLVolumeItf mVolume;
    SLAndroidSimpleBufferQueueItf mBufferQueue;

    uint32_t mNumberOfFrames;
    qcc::Mutex* mBufferMutex;
    uint8_t mBuffersAvailable;
    uint8_t** mAudioBuffers;
    uint8_t mBufferIndex;

    qcc::Mutex mListenersMutex;
    Listeners mListeners;
};

}
}

#endif /* _ANDROIDDEVICE_H */
