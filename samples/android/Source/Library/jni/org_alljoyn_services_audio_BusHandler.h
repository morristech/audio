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

/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class org_alljoyn_services_audio_AllJoynAudioStreamingMediaPlayer */

#ifndef _Included_org_alljoyn_services_audio_AllJoynAudioStreamingMediaPlayer
#define _Included_org_alljoyn_services_audio_AllJoynAudioStreamingMediaPlayer
#ifdef __cplusplus
extern "C" {
#endif
#undef org_alljoyn_services_audio_AllJoynAudioStreamingMediaPlayer_PREPARE
#define org_alljoyn_services_audio_AllJoynAudioStreamingMediaPlayer_PREPARE 0L
#undef org_alljoyn_services_audio_AllJoynAudioStreamingMediaPlayer_SETDATASOURCE
#define org_alljoyn_services_audio_AllJoynAudioStreamingMediaPlayer_SETDATASOURCE 1L
#undef org_alljoyn_services_audio_AllJoynAudioStreamingMediaPlayer_START
#define org_alljoyn_services_audio_AllJoynAudioStreamingMediaPlayer_START 2L
#undef org_alljoyn_services_audio_AllJoynAudioStreamingMediaPlayer_PAUSE
#define org_alljoyn_services_audio_AllJoynAudioStreamingMediaPlayer_PAUSE 3L
#undef org_alljoyn_services_audio_AllJoynAudioStreamingMediaPlayer_STOP
#define org_alljoyn_services_audio_AllJoynAudioStreamingMediaPlayer_STOP 4L
#undef org_alljoyn_services_audio_AllJoynAudioStreamingMediaPlayer_CHANGEVOLUME
#define org_alljoyn_services_audio_AllJoynAudioStreamingMediaPlayer_CHANGEVOLUME 5L
#undef org_alljoyn_services_audio_AllJoynAudioStreamingMediaPlayer_MUTE
#define org_alljoyn_services_audio_AllJoynAudioStreamingMediaPlayer_MUTE 6L
#undef org_alljoyn_services_audio_AllJoynAudioStreamingMediaPlayer_RELEASE
#define org_alljoyn_services_audio_AllJoynAudioStreamingMediaPlayer_RELEASE 7L
/*
 * Class:     org_alljoyn_services_audio_AllJoynAudioStreamingMediaPlayer
 * Method:    Prepare
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_alljoyn_services_audio_AllJoynAudioStreamingMediaPlayer_Prepare
    (JNIEnv *, jobject, jstring);

/*
 * Class:     org_alljoyn_services_audio_AllJoynAudioStreamingMediaPlayer
 * Method:    SetDataSource
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_alljoyn_services_audio_AllJoynAudioStreamingMediaPlayer_SetDataSource
    (JNIEnv *, jobject, jstring);

/*
 * Class:     org_alljoyn_services_audio_AllJoynAudioStreamingMediaPlayer
 * Method:    AddSink
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_alljoyn_services_audio_AllJoynAudioStreamingMediaPlayer_AddSink
    (JNIEnv *, jobject, jstring, jstring, jshort);

/*
 * Class:     org_alljoyn_services_audio_AllJoynAudioStreamingMediaPlayer
 * Method:    RemoveSink
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_alljoyn_services_audio_AllJoynAudioStreamingMediaPlayer_RemoveSink
    (JNIEnv *, jobject, jstring);

/*
 * Class:     org_alljoyn_services_audio_AllJoynAudioStreamingMediaPlayer
 * Method:    Start
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_alljoyn_services_audio_AllJoynAudioStreamingMediaPlayer_Start
    (JNIEnv *, jobject);

/*
 * Class:     org_alljoyn_services_audio_AllJoynAudioStreamingMediaPlayer
 * Method:    Pause
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_alljoyn_services_audio_AllJoynAudioStreamingMediaPlayer_Pause
    (JNIEnv *, jobject);

/*
 * Class:     org_alljoyn_services_audio_AllJoynAudioStreamingMediaPlayer
 * Method:    Stop
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_alljoyn_services_audio_AllJoynAudioStreamingMediaPlayer_Stop
    (JNIEnv *, jobject);

/*
 * Class:     org_alljoyn_services_audio_AllJoynAudioStreamingMediaPlayer
 * Method:    initialize
 * Signature: (F)V
 */
JNIEXPORT void JNICALL Java_org_alljoyn_services_audio_AllJoynAudioStreamingMediaPlayer_ChangeVolume
    (JNIEnv *, jobject, jfloat);

/*
 * Class:     org_alljoyn_services_audio_AllJoynAudioStreamingMediaPlayer
 * Method:    Mute
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_alljoyn_services_audio_AllJoynAudioStreamingMediaPlayer_Mute
    (JNIEnv *, jobject);

/*
 * Class:     org_alljoyn_services_audio_AllJoynAudioStreamingMediaPlayer
 * Method:    Release
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_alljoyn_services_audio_AllJoynAudioStreamingMediaPlayer_Relase
    (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
