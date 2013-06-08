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

#include "MyAllJoynListeners.h"
#include "Constants.h"

/* From SessionPortListener */
bool MyAllJoynListeners::AcceptSessionJoiner(SessionPort sessionPort, const char* joiner, const SessionOpts& opts)
{
    LOGD("Returning true to accept session join");
    return true;
}

void MyAllJoynListeners::SessionJoined(SessionPort sessionPort, SessionId id, const char* joiner)
{
    LOGD("SessionJoined....");
    if (mBusAttachment != NULL) {
        mBusAttachment->EnableConcurrentCallbacks();
        uint32_t timeout = 20;
        QStatus status = mBusAttachment->SetLinkTimeout(id, timeout);
    }
}
