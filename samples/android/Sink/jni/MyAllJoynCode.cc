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
#include <alljoyn/version.h>

using namespace qcc;
using namespace ajn;
using namespace services;

class AboutStore : public ajn::services::PropertyStore {
  public:
    AboutStore(const char* friendlyName) {
        mFriendlyName = strdup(friendlyName);
    }
    ~AboutStore() {
        if (mFriendlyName != NULL)
            free((void*)mFriendlyName);
    }
    QStatus ReadAll(const char* languageTag, PropertyStore::Filter filter, MsgArg& all) {
        if (languageTag && strcmp(languageTag, "en") != 0) {
            return ER_LANGUAGE_NOT_SUPPORTED;
        }
        if (PropertyStore::WRITE == filter) {
            return ER_NOT_IMPLEMENTED;
        }

        /*
         * The properties of these values should be customized for the application.
         */
        size_t numProps = (PropertyStore::READ == filter) ? 11 : 7;
        MsgArg* props = new MsgArg[numProps];
        static const uint8_t appId[] = { 0xc8, 0x74, 0xb3, 0xa9, 0x6d, 0x56, 0x47, 0xe1, 0xb8, 0x1f, 0x47, 0x05, 0x48, 0xc2, 0x01, 0x5e };
        props[0].Set("{sv}", "AppId", new MsgArg("ay", 16, appId));
        props[1].Set("{sv}", "DefaultLanguage", new MsgArg("s", "en"));
        props[2].Set("{sv}", "DeviceName", new MsgArg("s", mFriendlyName));
        props[3].Set("{sv}", "DeviceId", new MsgArg("s", "Android"));
        props[4].Set("{sv}", "AppName", new MsgArg("s", "SinkService"));
        props[5].Set("{sv}", "Manufacturer", new MsgArg("s", "AllJoyn"));
        props[6].Set("{sv}", "ModelNumber", new MsgArg("s", "1"));
        if (PropertyStore::READ == filter) {
            static const char* supportedLanguages[] = { "en" };
            props[7].Set("{sv}", "SupportedLanguages", new MsgArg("as", 1, supportedLanguages));
            props[8].Set("{sv}", "Description", new MsgArg("s", "AllJoyn Audio Sink"));
            props[9].Set("{sv}", "SoftwareVersion", new MsgArg("s", "v0.0.1"));
            props[10].Set("{sv}", "AJSoftwareVersion", new MsgArg("s", ajn::GetVersion()));
        }

        all.Set("a{sv}", numProps, props);
        all.SetOwnershipFlags(MsgArg::OwnsArgs, true);
        return ER_OK;
    }
  private:
    const char* mFriendlyName;
};

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

        mAboutProps = new AboutStore("AndroidDevice");
        mStreamObj = new StreamObject(mBusAttachment, "/Speaker/In", mAudioDevice,
                                      port, mAboutProps);
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

