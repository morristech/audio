/**
 * @file
 * PCM encoders and decoders.
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

#ifndef _RAWCODEC_H
#define _RAWCODEC_H

#ifndef __cplusplus
#error Only include RawCodec.h in C++ code.
#endif

#include <alljoyn/audio/AudioCodec.h>

namespace ajn {
namespace services {

/**
 * A PCM decoder used by AudioSinkObject.
 *
 * The decode is just a pass-through.
 */
class RawDecoder : public AudioDecoder {
  public:
    RawDecoder();
    ~RawDecoder();

    /**
     * Gets the capability of this raw decoder.  This is suitable for
     * use in the Capabilities property of a Port object.
     *
     * @param[out] capability this (and each parameters member) should be
     *                        delete[]'d by the caller.
     */
    static void GetCapability(Capability* capability);

    QStatus Configure(Capability* capability);
    uint32_t GetFrameSize() const { return FRAMES_PER_PACKET; }
    void Decode(uint8_t** buffer, uint32_t* numBytes);
};

/**
 * A PCM encoder used by SinkPlayer.
 *
 * The encode is just a pass-through.
 */
class RawEncoder : public AudioEncoder {
  public:
    RawEncoder();
    ~RawEncoder();

    QStatus Configure(DataSource* dataSource);
    uint32_t GetFrameSize() const { return FRAMES_PER_PACKET; }
    void GetConfiguration(Capability* configuration);
    void Encode(uint8_t** buffer, uint32_t* numBytes);

  private:
    uint32_t mChannelsPerFrame;
    double mSampleRate;
};

}
}

#endif /* _RAW_H */
