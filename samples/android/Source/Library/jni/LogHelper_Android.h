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

#include <jni.h>
#include "LogHelper.h"
#include "Constants.h"

#ifndef _LOG_HELPER_ANDROID_
#define _LOG_HELPER_ANDROID_

class LogHelper_Android : public LogHelper {
  public:
    LogHelper_Android(JavaVM* vm, jobject jobj) : vm(vm), jobj(jobj) { };

    ~LogHelper_Android() {
        vm = NULL;
    }

    void LogString(const char* logMsg, ...);

  private:
    JavaVM* vm;
    jobject jobj;
};

#endif //_LOG_HELPER_ANDROID_

