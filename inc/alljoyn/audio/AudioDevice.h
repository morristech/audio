/**
 * @file
 * Access to an audio device / sound card.
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

#ifndef _AUDIODEVICE_H
#define _AUDIODEVICE_H

#ifndef __cplusplus
#error Only include AudioDevice.h in C++ code.
#endif

#include <stdint.h>

namespace ajn {
namespace services {

/**
 * Pure virtual class for volume and mute events.
 */
class AudioDeviceListener {
  public:
    virtual ~AudioDeviceListener() { }

    /**
     * Called when the mute state changes.
     *
     * @param[in] mute true if mute
     */
    virtual void MuteChanged(bool mute) = 0;

    /**
     * Called when the volume changes.
     *
     * @param[in] volume the volume.
     */
    virtual void VolumeChanged(int16_t volume) = 0;
};

/**
 * A pure virtual class for implementing access to an audio device /
 * sound card.
 */
class AudioDevice {
  public:
    virtual ~AudioDevice() { }

    /**
     * Opens and configures the audio device.
     *
     * @param[in] format the format to configure.
     * @param[in] sampleRate the sample rate to configure.
     * @param[in] numChannels the number of channels to configure.
     * @param[out] bufferSize the audio device's buffer size (in
     *                        frames).
     *
     * @return true if the audio device was successfully opened and
     *         configured.
     */
    virtual bool Open(const char* format, uint32_t sampleRate, uint32_t numChannels, uint32_t& bufferSize) = 0;

    /**
     * Closes the audio device.
     *
     * @see Open()
     */
    virtual void Close() = 0;

    /**
     * Pauses the audio device playback.
     *
     * @return true if playback was paused.
     */
    virtual bool Pause() = 0;

    /**
     * Starts the audio device playback.
     *
     * @return true if playback was started.
     */
    virtual bool Play() = 0;

    /**
     * Recovers from an underrun if one has occurred.
     *
     * @return true if an underrun occurred.
     */
    virtual bool Recover() = 0;

    /**
     * Gets the audio device delay (time until new samples become
     * audible).
     *
     * @return the time (in frames) that a frame that is written to
     *         the audio device shortly after this call will take to be
     *         audible.
     */
    virtual uint32_t GetDelay() = 0;

    /**
     * Gets the number of frames the audio device wants.
     *
     * @return the number of frames that can be written to the audio
     *         device without blocking.
     */
    virtual uint32_t GetFramesWanted() = 0;

    /**
     * Writes samples to the audio device.  This call blocks until
     * samples are written to the audio device.
     *
     * @param[in] buffer the buffer of audio samples to write.
     * @param[in] bufferSizeInFrames the size of buffer in frames.
     *
     * @return true on successful write.
     */
    virtual bool Write(const uint8_t* buffer, uint32_t bufferSizeInFrames) = 0;

    /**
     * Gets the audio device mute state.
     *
     * @param[out] mute true if mute
     *
     * @return true on successful get.
     */
    virtual bool GetMute(bool& mute) = 0;

    /**
     * Sets the audio device mute state.
     *
     * This should call any listeners MutedChanged methods if the
     * muted state has changed.
     *
     * @param[in] mute true if mute
     *
     * @return true on successful set.
     */
    virtual bool SetMute(bool mute) = 0;

    /**
     * Gets the audio device volume range.
     *
     * @param[out] low the minimum volume.
     * @param[out] high the maximum volume.
     * @param[out] step the volume step.
     *
     * @return true on successful get.
     */
    virtual bool GetVolumeRange(int16_t& low, int16_t& high, int16_t& step) = 0;

    /**
     * Gets the audio device volume.
     *
     * @param[out] volume the volume.
     *
     * @return true on successful get.
     */
    virtual bool GetVolume(int16_t& volume) = 0;

    /**
     * Sets the audio device volume.
     *
     * This should call any listeners VolumeChanged methods if the
     * volume state has changed.
     *
     * @param[in] volume the volume.
     *
     * @return true on successful set.
     */
    virtual bool SetVolume(int16_t volume) = 0;

    /**
     * Adds a listener for volume and mute events.
     *
     * @param[in] listener the listener to add.
     */
    virtual void AddListener(AudioDeviceListener* listener) = 0;

    /**
     * Removes a listener for volume and mute events.
     *
     * @param[in] listener the listener to remove.
     *
     * @see AddListener()
     */
    virtual void RemoveListener(AudioDeviceListener* listener) = 0;
};

}
}

#endif /* _AUDIODEVICE_H */
