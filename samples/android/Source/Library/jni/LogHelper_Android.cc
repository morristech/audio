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
#include "LogHelper_Android.h"
#include "Constants.h"
#include <cstdio>

void LogHelper_Android::LogString(const char* logMsg, ...) {
	va_list args;
	char theMsg[1000];
	va_start(args, logMsg);
	vsprintf(theMsg, logMsg, args);
	va_end(args);

	JNIEnv* env;
	jint jret = vm->GetEnv((void**)&env, JNI_VERSION_1_2);
	if (JNI_EDETACHED == jret) {
	    vm->AttachCurrentThread(&env, NULL);
	}

	jclass jcls = env->GetObjectClass(jobj);
	jmethodID mid = env->GetMethodID(jcls, "LogToUI", "(Ljava/lang/String;)V");
	if (mid == 0) {
		LOGE("Failed to get Java LogToUI");
	} else {
		jstring jMsg = env->NewStringUTF(theMsg);
		env->CallVoidMethod(jobj, mid, jMsg);
		env->DeleteLocalRef(jMsg);
	}
	if (JNI_EDETACHED == jret) {
		vm->DetachCurrentThread();
	}
}
