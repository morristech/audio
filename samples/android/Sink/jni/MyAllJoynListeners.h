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

#include <alljoyn/AllJoynStd.h>
#include <alljoyn/BusListener.h>
#include <alljoyn/SessionPortListener.h>
#include <alljoyn/SessionListener.h>
#include <alljoyn/BusAttachment.h>
#include <cstdio>

#ifndef _MY_ALLJOYN_LISTENERS_
#define _MY_ALLJOYN_LISTENERS_

using namespace ajn;

class MyAllJoynListeners : public BusListener, public SessionPortListener, public SessionListener {
  public:
    MyAllJoynListeners(BusAttachment* bus) : mBusAttachment(bus) { };

    virtual ~MyAllJoynListeners() {
        mBusAttachment = NULL;
    };

    /* From SessionPortListener */
    virtual bool AcceptSessionJoiner(SessionPort sessionPort, const char* joiner, const SessionOpts& opts);

    virtual void SessionJoined(SessionPort sessionPort, SessionId id, const char* joiner);

  private:
    BusAttachment* mBusAttachment;
};

#endif //_MY_ALLJOYN_LISTENERS_
