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

#include <alljoyn/audio/SinkSearcher.h>
#include <alljoyn/AllJoynStd.h>

#include "Sink.h"
#include <qcc/Debug.h>

#define QCC_MODULE "ALLJOYN_AUDIO"

using namespace ajn;
using namespace qcc;
using namespace std;

namespace ajn {
namespace services {

SinkSearcher::SinkSearcher() : mBus(NULL) {

}

void SinkSearcher::ListenerRegistered(BusAttachment* bus) {
    mBus = bus;
}

void SinkSearcher::OnAnnounce(const InterfaceDescription::Member* member, const char* srcPath, Message& message) {
    Service service;
    bool serviceAnnounced = false;

    /*
     * Filter for the interfaces we're interested in.
     */
    const char* serviceName = message->GetSender();

    size_t numObjectDescriptions = 0;
    MsgArg* objectDescriptions = NULL;
    QStatus status = message->GetArg(2)->Get("a(oas)", &numObjectDescriptions, &objectDescriptions);
    if (status != ER_OK)
        QCC_LogError(status, ("Failed to get objectDescriptions"));
    for (size_t i = 0; i < numObjectDescriptions; ++i) {
        char* objectPath = NULL;
        size_t numInterfaces = 0;
        MsgArg* interfaces = NULL;
        status = objectDescriptions[i].Get("(oas)", &objectPath, &numInterfaces, &interfaces);
        if (status != ER_OK)
            QCC_LogError(status, ("Failed to get interfaces"));
        for (size_t j = 0; j < numInterfaces; ++j) {
            char* intf;
            status = interfaces[j].Get("s", &intf);
            if (status != ER_OK)
                QCC_LogError(status, ("Failed to get interface"));

            if (!strcmp(intf, AUDIO_SINK_INTERFACE)) {
                serviceAnnounced = true;
            } else if (!strcmp(intf, STREAM_INTERFACE)) {
                service.path = objectPath;
            }
        }
    }

    if (!serviceAnnounced || service.path.empty())
        return;

    /*
     * Extract extra information from the service metadata.
     */
    status = message->GetArg(1)->Get("q", &service.port);
    if (status != ER_OK)
        QCC_LogError(status, ("Failed to get SessionPort"));
    const char* deviceName = NULL;
    status = message->GetArg(3)->GetElement("{ss}", ANNOUNCE_DEVICE_NAME, &deviceName);
    if (status != ER_OK) {
        QCC_LogError(status, ("Failed to get DeviceName"));
    } else {
        service.friendlyName = deviceName;
    }

    /*
     * Now we've got all the info we need to join a session and begin doing useful work so cache it.
     */
    service.name = serviceName;
    mServices.erase(serviceName);
    mServices.insert(pair<String, Service>(serviceName, service));
    QCC_DbgHLPrintf(("OnAnnounce: \"%s\" %s %s", service.friendlyName.c_str(), serviceName, service.path.c_str()));

    /*
     * There are a number of strategies for tracking device presence.
     *
     * One is to leave outstanding find requests for each device of interest.  This is what is
     * below.  However, this can lead to false negatives if multicast traffic does not make it
     * through, and may take up to 40 seconds to find the device again after a false negative.
     *
     * Another is to force a refresh by cancelling any finds and re-issuing them.  The found
     * response should occur relative quickly after re-issue if the device is present.  See
     * Refresh below.
     *
     * Note also that we have to be careful here when calling FindAdvertisedName.  If it blocks
     * then we could run out of "concurrency" threads if we get flooded with OnAnnounce calls 
     * before FindAdvertisedName returns.  Call it asynchronously to avoid this problem.
     */
    status = FindAdvertisedNameAsync(serviceName);
    if (status != ER_OK)
        QCC_LogError(status, ("Failed to call FindAdvertisedName"));
}

QStatus SinkSearcher::FindAdvertisedNameAsync(const char* namePrefix) {
    MsgArg arg("s", namePrefix);
    const ProxyBusObject& alljoynObj = mBus->GetAllJoynProxyObj();
    return alljoynObj.MethodCallAsync(org::alljoyn::Bus::InterfaceName, "FindAdvertisedName", 
                                      this, static_cast<MessageReceiver::ReplyHandler>(&SinkSearcher::FindAdvertisedNameAsyncCB),
                                      &arg, 1);
}

void SinkSearcher::FindAdvertisedNameAsyncCB(Message& message, void* context) {
    QStatus status = ER_FAIL;
    if (message->GetType() == MESSAGE_METHOD_RET) {
        uint32_t disposition;
        status = message->GetArgs("u", &disposition);
        if (ER_OK == status) {
            switch (disposition) {
            case ALLJOYN_FINDADVERTISEDNAME_REPLY_SUCCESS:
                break;

            default:
                QCC_LogError(ER_BUS_UNEXPECTED_DISPOSITION, ("%s.FindAdvertisedName returned %d", 
                                                             org::alljoyn::Bus::InterfaceName, disposition));
                break;
            }
        }
    } else if (message->GetType() == MESSAGE_ERROR) {
        QCC_LogError(status, ("%s.FindAdvertisedName returned ERROR_MESSAGE (error=%s)", 
                              org::alljoyn::Bus::InterfaceName, message->GetErrorDescription().c_str()));
    }
}

QStatus SinkSearcher::Register(BusAttachment* bus) {
    bus->RegisterBusListener(*this);

    QStatus status = bus->CreateInterfacesFromXml(ABOUT_XML);
    if (status != ER_OK)
        QCC_LogError(status, ("CreateInterfacesFromXml failed"));

    if (status == ER_OK) {
        status = bus->RegisterSignalHandler(this,
                                            static_cast<MessageReceiver::SignalHandler>(&SinkSearcher::OnAnnounce),
                                            bus->GetInterface(ABOUT_INTERFACE)->GetMember("Announce"), 0);
        if (status != ER_OK)
            QCC_LogError(status, ("RegisterSignalHandler failed"));
    }

    if (status == ER_OK) {
        status = bus->AddMatch(ANNOUNCE_MATCH_RULE);
        if (status != ER_OK)
            QCC_LogError(status, ("AddMatch failed"));
    }

    return status;
}

void SinkSearcher::Unregister() {
    if (mBus == NULL)
        return;

    QStatus status = mBus->RemoveMatch(ANNOUNCE_MATCH_RULE);
    if (status != ER_OK)
        QCC_LogError(status, ("RemoveMatch failed"));

    status = mBus->UnregisterAllHandlers(this);
    if (status != ER_OK)
        QCC_LogError(status, ("UnregisterAllHandlers failed"));

    mBus->UnregisterBusListener(*this);
}

void SinkSearcher::Refresh() {
    for (map<String, Service>::iterator it = mServices.begin(); it != mServices.end(); ++it) {
        mBus->CancelFindAdvertisedName(it->first.c_str());
        mBus->FindAdvertisedName(it->first.c_str());
    }
}

void SinkSearcher::FoundAdvertisedName(const char* name, TransportMask transport, const char* namePrefix) {
    QCC_DbgHLPrintf(("FoundAdvertisedName: %s", name));
    map<String, Service>::iterator it = mServices.find(name);
    if (it != mServices.end()) {
        it->second.found = true;
        SinkFound(&it->second);
    }
}

void SinkSearcher::LostAdvertisedName(const char* name, TransportMask transport, const char* namePrefix) {
    QCC_DbgHLPrintf(("LostAdvertisedName: %s", name));
    map<String, Service>::iterator it = mServices.find(name);
    if (it != mServices.end()) {
        it->second.found = false;
        SinkLost(&it->second);
    }
}

}
}
