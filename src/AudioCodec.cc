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

#include <alljoyn/audio/AudioCodec.h>

#include "RawCodec.h"
#ifdef WITH_ALAC
#include "alac/AlacCodec.h"
#endif
#include <qcc/Debug.h>

#define QCC_MODULE "ALLJOYN_AUDIO"

namespace ajn {
namespace services {

const char* MIMETYPE_AUDIO_RAW = "audio/x-raw";
const char* MIMETYPE_AUDIO_ALAC = "audio/x-alac";

const char* MIMETYPE_IMAGE_JPEG = "image/jpeg";
const char* MIMETYPE_IMAGE_PNG = "image/png";

const char* MIMETYPE_METADATA = "application/x-metadata";

void AudioDecoder::GetCapabilities(Capability** capabilities, size_t* numCapabilities) {
    *numCapabilities = 1;
#ifdef WITH_ALAC
    bool alacEnabled = true;
    char* e = getenv("DISABLE_ALAC");
    if (e != NULL && strcmp(e, "1") == 0)
        alacEnabled = false;
    if (alacEnabled)
        ++*numCapabilities;
#endif

    int i = 0;
    *capabilities = new Capability[*numCapabilities];
    RawDecoder::GetCapability(&(*capabilities)[i++]);
#ifdef WITH_ALAC
    if (alacEnabled)
        AlacDecoder::GetCapability(&(*capabilities)[i++]);
#endif
}

AudioDecoder* AudioDecoder::Create(const char* type) {
    if (strcmp(type, MIMETYPE_AUDIO_RAW) == 0)
        return new RawDecoder();
#ifdef WITH_ALAC
    else if (strcmp(type, MIMETYPE_AUDIO_ALAC) == 0)
        return new AlacDecoder();
#endif
    return NULL;
}

bool AudioEncoder::CanCreate(const char* type) {
    if (strcmp(type, MIMETYPE_AUDIO_RAW) == 0)
        return true;
#ifdef WITH_ALAC
    else if (strcmp(type, MIMETYPE_AUDIO_ALAC) == 0)
        return true;
#endif
    return false;
}

AudioEncoder* AudioEncoder::Create(const char* type) {
    if (strcmp(type, MIMETYPE_AUDIO_RAW) == 0)
        return new RawEncoder();
#ifdef WITH_ALAC
    else if (strcmp(type, MIMETYPE_AUDIO_ALAC) == 0)
        return new AlacEncoder();
#endif
    return NULL;
}

}
}
