/**
 * @file
 * ALAC encoders and decoders.
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

#ifndef _ALACCODEC_H
#define _ALACCODEC_H

#ifndef __cplusplus
#error Only include AlacCodec.h in C++ code.
#endif

#include "ALACAudioTypes.h"
#include "ALACDecoder.h"
#include "ALACEncoder.h"
#include <alljoyn/audio/AudioCodec.h>

namespace ajn {
namespace services {

/**
 * An ALAC decoder used by AudioSinkObject.
 */
class AlacDecoder : public AudioDecoder {
  public:
    AlacDecoder();
    ~AlacDecoder();

    /**
     * Gets the capability of this ALAC decoder.  This is suitable for
     * use in the Capabilities property of a Port object.
     *
     * @param[out] capability this (and each parameters member) should be
     *                        delete[]'d by the caller.
     */
    static void GetCapability(Capability* capability);

    QStatus Configure(Capability* configuration);
    uint32_t GetFrameSize() const { return mFramesPerPacket; }
    void Decode(uint8_t** buffer, uint32_t* numBytes);

  private:
    ALACDecoder* mDecoder;
    uint8_t* mDecodeBuffer;
    uint32_t mChannelsPerFrame;
    uint32_t mBytesPerFrame;
    uint32_t mFramesPerPacket;
};

/**
 * An ALAC encoder used by SinkPlayer.
 */
class AlacEncoder : public AudioEncoder {
  public:
    AlacEncoder();
    ~AlacEncoder();

    QStatus Configure(DataSource* dataSource);
    uint32_t GetFrameSize() const { return mOutputFormat.mFramesPerPacket; }
    void GetConfiguration(Capability* configuration);
    void Encode(uint8_t** buffer, uint32_t* numBytes);

  private:
    ALACEncoder* mEncoder;
    AudioFormatDescription mInputFormat;
    AudioFormatDescription mOutputFormat;
    uint8_t* mEncodeBuffer;
};

}
}

#endif /* _ALACCODEC_H */
