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

#include <alljoyn/BusAttachment.h>
#include <alljoyn/ProxyBusObject.h>
#include <alljoyn/DBusStd.h>
#include <alljoyn/AllJoynStd.h>
#include <qcc/String.h>
#include "Constants.h"
#include <alljoyn/audio/StreamObject.h>
#include <alljoyn/audio/AudioDevice.h>
#include "MyAllJoynListeners.h"

#ifndef _MY_ALLJOYN_CODE_
#define _MY_ALLJOYN_CODE_

class MyAllJoynCode {
  public:
    MyAllJoynCode()
        : listener(NULL), mBusAttachment(NULL), mAboutProps(NULL), mStreamObj(NULL) { };

    ~MyAllJoynCode() {
        shutdown();
        if (listener)
            delete listener;
        listener = NULL;
    };

    void initialize(const char* packageName);

    void shutdown();

  private:
    MyAllJoynListeners* listener;
    BusAttachment* mBusAttachment;
    qcc::String mAdvertisedName;
    services::PropertyStore* mAboutProps;
    services::StreamObject* mStreamObj;
    services::AudioDevice* mAudioDevice;
};

#endif //_MY_ALLJOYN_CODE_
