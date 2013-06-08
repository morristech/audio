/**
 * @file
 * Definitions common to both service and client.
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

#ifndef _AUDIO_H
#define _AUDIO_H

#ifndef __cplusplus
#error Only include Audio.h in C++ code.
#endif

#include <alljoyn/MsgArg.h>

namespace ajn {
namespace services {

extern const char* MIMETYPE_AUDIO_RAW; /**< The media type of a raw (PCM) audio port capability. */
extern const char* MIMETYPE_AUDIO_ALAC; /**< The media type of an ALAC encoded audio port capability. */

extern const char* MIMETYPE_IMAGE_JPEG; /**< The media type of a JPEG image port capability. */
extern const char* MIMETYPE_IMAGE_PNG; /**< The media type of a PNG image port capability. */

extern const char* MIMETYPE_METADATA; /**< The media type of a metadata port capability. */

/**
 * A capability of a port.
 */
struct Capability {
    qcc::String type; /**< The media type of this capability. */
    ajn::MsgArg* parameters;  /**< An array of parameters of the media type. */
    size_t numParameters; /**< The number of parameters in the array. */
    Capability() : parameters(NULL), numParameters(0) { }
};

}
}

#endif /* _AUDIO_H */
