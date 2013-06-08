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
#include <alljoyn/BusAttachment.h>
#include <alljoyn/ProxyBusObject.h>
#include <alljoyn/DBusStd.h>
#include <alljoyn/AllJoynStd.h>
#include "Constants.h"
#include "MyAllJoynCode.h"

/* Static data */
static MyAllJoynCode* myAllJoynCode = NULL;

#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     Java_org_alljoyn_ioe_services_audiostreaming_player_jni_BusHandler_initialize
 * Method:    initialize
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_alljoyn_ioe_services_audiostreaming_player_jni_BusHandler_initialize(JNIEnv* env, jobject jobj, jstring packageNameStrObj) {
    if (myAllJoynCode == NULL) {
        JavaVM* vm;
        env->GetJavaVM(&vm);
        jobject gjobj = env->NewGlobalRef(jobj);
        myAllJoynCode = new MyAllJoynCode();
    }
    jboolean iscopy;
    const char* packageNameStr = env->GetStringUTFChars(packageNameStrObj, &iscopy);
    myAllJoynCode->initialize(packageNameStr);
    env->ReleaseStringUTFChars(packageNameStrObj, packageNameStr);
}

/*
 * Class:     Java_org_alljoyn_ioe_services_audiostreaming_player_jni_BusHandler_shutdown
 * Method:    shutdown
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_alljoyn_ioe_services_audiostreaming_player_jni_BusHandler_shutdown(JNIEnv* env, jobject jobj) {
    myAllJoynCode->shutdown();
    delete myAllJoynCode;
    myAllJoynCode = NULL;
}

#ifdef __cplusplus
}
#endif

