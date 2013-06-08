/**
 * @file
 * Audio device access using the Linux ALSA API.
 */

/******************************************************************************
 * Copyright 2013, doubleTwist Corporation and Qualcomm Innovation Center, Inc.
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

#ifndef _ALSADEVICE_H
#define _ALSADEVICE_H

#ifndef __cplusplus
#error Only include ALSADevice.h in C++ code.
#endif

#include <alljoyn/audio/AudioDevice.h>
#include <qcc/Thread.h>
#include <alsa/asoundlib.h>

namespace ajn {
namespace services {

/**
 * A subclass of AudioDevice that implements access to an audio device using the Linux ALSA API.
 */
class ALSADevice : public AudioDevice {
  public:
    ALSADevice();

    bool Open(const char* format, uint32_t sampleRate, uint32_t numChannels, uint32_t& bufferSize);
    void Close();
    bool Pause();
    bool Play();
    bool Recover();
    uint32_t GetDelay();
    uint32_t GetFramesWanted();
    bool Write(const uint8_t* buffer, uint32_t bufferSizeInFrames);

    bool GetMute(bool& mute);
    bool SetMute(bool mute);
    bool GetVolumeRange(int16_t& low, int16_t& high, int16_t& step);
    bool GetVolume(int16_t& volume);
    bool SetVolume(int16_t volume);
    void AddListener(AudioDeviceListener* listener);
    void RemoveListener(AudioDeviceListener* listener);

  private:
    bool SetVolumeMax(snd_mixer_elem_t* elem);
    void StartAudioMixerThread();
    void StopAudioMixerThread();
    static qcc::ThreadReturn AudioMixerThread(void* arg);
    static int AudioMixerEvent(snd_mixer_elem_t* elem, unsigned int mask);

  private:
    typedef std::set<AudioDeviceListener*> Listeners;

    bool mMute;
    int16_t mVolume;
    snd_pcm_t* mAudioDeviceHandle;
    snd_mixer_t* mAudioMixerHandle;
    snd_mixer_elem_t* mAudioMixerElementMaster;
    snd_mixer_elem_t* mAudioMixerElementPCM;
    bool mHardwareCanPause;
    long mMinVolume;
    long mMaxVolume;
    qcc::Thread* mAudioMixerThread;
    qcc::Mutex mListenersMutex;
    Listeners mListeners;
};

}
}

#endif /* _ALSADEVICE_H */
