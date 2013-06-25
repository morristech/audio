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
#include "LogHelper_Android.h"
#include "MyAllJoynCode.h"

/* Static data */
static MyAllJoynCode* myAllJoynCode = NULL;
static LogHelper* logger;

#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     org_alljoyn_services_audio_AllJoynAudioServiceMediaPlayer
 * Method:    Prepare
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_alljoyn_services_audio_AllJoynAudioServiceMediaPlayer_Prepare
  (JNIEnv* env, jobject jobj, jstring packageNameStrObj) {
	if(myAllJoynCode == NULL)
	{
		JavaVM* vm;
		env->GetJavaVM(&vm);
		jobject gjobj = env->NewGlobalRef(jobj);
		logger = new LogHelper_Android(vm, gjobj);
		myAllJoynCode = new MyAllJoynCode(logger, vm, gjobj);
	}
	jboolean iscopy;
	const char* packageNameStr = env->GetStringUTFChars(packageNameStrObj, &iscopy);
	myAllJoynCode->Prepare(packageNameStr);
	env->ReleaseStringUTFChars(packageNameStrObj, packageNameStr);
}

/*
 * Class:     org_alljoyn_services_audio_AllJoynAudioServiceMediaPlayer
 * Method:    SetDataSource
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_alljoyn_services_audio_AllJoynAudioServiceMediaPlayer_SetDataSource
(JNIEnv* env, jobject jobj, jstring jPath) {
	jboolean iscopy;
	const char* path = env->GetStringUTFChars(jPath, &iscopy);
	if(myAllJoynCode != NULL) myAllJoynCode->SetDataSource(path);
	env->ReleaseStringUTFChars(jPath, path);
}

/*
 * Class:     org_alljoyn_services_audio_AllJoynAudioServiceMediaPlayer
 * Method:    AddSink
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_alljoyn_services_audio_AllJoynAudioServiceMediaPlayer_AddSink
(JNIEnv* env, jobject jobj, jstring jName, jstring jPath, jshort port) {
	jboolean iscopy;
	const char* name = env->GetStringUTFChars(jName, &iscopy);
	const char* path = env->GetStringUTFChars(jPath, &iscopy);
	if(myAllJoynCode != NULL) myAllJoynCode->AddSink(name, path, port);
	env->ReleaseStringUTFChars(jName, name);
	env->ReleaseStringUTFChars(jPath, path);
}

/*
 * Class:     org_alljoyn_services_audio_AllJoynAudioServiceMediaPlayer
 * Method:    RemoveSink
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_alljoyn_services_audio_AllJoynAudioServiceMediaPlayer_RemoveSink
(JNIEnv* env, jobject jobj, jstring jName) {
	jboolean iscopy;
	const char* name = env->GetStringUTFChars(jName, &iscopy);
	if(myAllJoynCode != NULL) myAllJoynCode->RemoveSink(name);
	env->ReleaseStringUTFChars(jName, name);
}

/*
 * Class:     org_alljoyn_services_audio_AllJoynAudioServiceMediaPlayer
 * Method:    Start
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_alljoyn_services_audio_AllJoynAudioServiceMediaPlayer_Start
(JNIEnv* env, jobject jobj) {
	if(myAllJoynCode != NULL) myAllJoynCode->Start();
}

/*
 * Class:     org_alljoyn_services_audio_AllJoynAudioServiceMediaPlayer
 * Method:    Pause
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_alljoyn_services_audio_AllJoynAudioServiceMediaPlayer_Pause
(JNIEnv* env, jobject jobj) {
	if(myAllJoynCode != NULL) myAllJoynCode->Pause();
}

/*
 * Class:     org_alljoyn_services_audio_AllJoynAudioServiceMediaPlayer
 * Method:    Stop
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_alljoyn_services_audio_AllJoynAudioServiceMediaPlayer_Stop
(JNIEnv* env, jobject jobj) {
	if(myAllJoynCode != NULL) myAllJoynCode->Stop();
}
/*
 * Class:     org_alljoyn_services_audio_AllJoynAudioServiceMediaPlayer
 * Method:    ChangeVolume
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_alljoyn_services_audio_AllJoynAudioServiceMediaPlayer_ChangeVolume
(JNIEnv* env, jobject jobj, jfloat value) {
	if(myAllJoynCode != NULL) myAllJoynCode->ChangeVolume(value);
}

/*
 * Class:     org_alljoyn_services_audio_AllJoynAudioServiceMediaPlayer
 * Method:    Mute
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_alljoyn_services_audio_AllJoynAudioServiceMediaPlayer_Mute
(JNIEnv* env, jobject jobj) {
	if(myAllJoynCode != NULL) myAllJoynCode->Mute();
}

/*
 * Class:     org_alljoyn_services_audio_AllJoynAudioServiceMediaPlayer
 * Method:    Release
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_alljoyn_services_audio_AllJoynAudioServiceMediaPlayer_Release
  (JNIEnv* env, jobject jobj) {
	myAllJoynCode->Release();
	delete myAllJoynCode;
	myAllJoynCode = NULL;
}

#ifdef __cplusplus
}
#endif

