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

#include "gtest/gtest.h"
#include <alljoyn/BusAttachment.h>

using namespace qcc;
using namespace ajn;

class TestClientListener;

class AudioTest : public testing::Environment {
  public:

    AudioTest();
    virtual ~AudioTest();
    virtual void SetUp();
    virtual void TearDown();

    static char* sServiceName;
    static char* sStreamObjectPath;
    static uint16_t sSessionPort;
    static bool sReady;
    static uint32_t sTimeout;

  private:
    TestClientListener* mListener;
    BusAttachment* mMsgBus;
};
