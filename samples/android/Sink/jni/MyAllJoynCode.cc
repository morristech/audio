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

#include "MyAllJoynCode.h"
#include <sys/system_properties.h>
#include "Constants.h"
#include <alljoyn/audio/android/AndroidDevice.h>

using namespace qcc;
using namespace ajn;
using namespace services;

void MyAllJoynCode::initialize(const char* packageName) {
    QStatus status = ER_OK;

    /* Initialize AllJoyn only once */
    if (!mBusAttachment) {
        mBusAttachment = new BusAttachment(packageName, true);
        /* Start the msg bus */
        if (ER_OK == status) {
            status = mBusAttachment->Start();
        } else {
            LOGE("BusAttachment::Start failed");
        }
        /* Connect to the daemon */
        if (ER_OK == status) {
            status = mBusAttachment->Connect();
            if (ER_OK != status) {
                LOGE("BusAttachment Connect failed.");
            }
        }
        LOGE("Created BusAttachment and connected");
        if (!listener) {
            listener = new MyAllJoynListeners(mBusAttachment);
        }
        mBusAttachment->RegisterBusListener(*listener);

        char deviceFriendlyName[PROP_VALUE_MAX];
        __system_property_get("ro.product.board", deviceFriendlyName);
        LOGE("device friendly name: %s", deviceFriendlyName);
        mAdvertisedName = mBusAttachment->GetUniqueName();        //String("org.example.Speaker-") + mBusAttachment->GetGlobalGUIDString();
//		QStatus status = mBusAttachment->RequestName(mAdvertisedName.c_str(), DBUS_NAME_FLAG_DO_NOT_QUEUE);
//		if (ER_OK != status) {
//			LOGE("RequestName() failed");
//			status = (status == ER_OK) ? ER_FAIL : status;
//		} else {
//			LOGE("Request Name was successful");
//		}

        mAudioDevice = new AndroidDevice();
        SessionOpts opts(SessionOpts::TRAFFIC_MESSAGES, true, SessionOpts::PROXIMITY_ANY, TRANSPORT_ANY);
        SessionPort port = SESSION_PORT_ANY;
        status = mBusAttachment->BindSessionPort(port, opts, *listener);
        if (ER_OK != status) {
            LOGE("BindSessionPort failed");
        } else {
            LOGE("Bind Session Port to was successful ");
        }
        /* Advertise the name */
        status = mBusAttachment->AdvertiseName(mAdvertisedName.c_str(), opts.transports);
        if (status != ER_OK) {
            LOGE("Failed to advertise name");
        } else {
            LOGE("Advertisement was successfully advertised");
        }

        mStreamObj = new StreamObject(mBusAttachment, "/org/example/Speaker/In", mAudioDevice,
                                      mAdvertisedName.c_str(), port, "AndroidDevice");
        //mBusAttachment->RegisterBusObject(*mStreamObj);
        mStreamObj->Register(mBusAttachment);
    }
}

void MyAllJoynCode::shutdown()
{
    /* Deallocate the BusAttachment */
    if (mBusAttachment) {
        delete mBusAttachment;
        mBusAttachment = NULL;
    }
    if (mAudioDevice) {
        delete mAudioDevice;
        mAudioDevice = NULL;
    }
}

