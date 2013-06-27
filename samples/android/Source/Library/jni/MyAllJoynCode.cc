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
#include <unistd.h>
#include <alljoyn/audio/WavDataSource.h>
#include <alljoyn/audio/Audio.h>
#include <math.h>

using namespace ajn;
using namespace services;

void MyAllJoynCode::Prepare(const char* packageName) {
	QStatus status = ER_OK;

	/* Initialize AllJoyn only once */
	if (!mBusAttachment) {
		mBusAttachment = new ajn::BusAttachment(packageName, true);
		/* Start the msg bus */
		if (ER_OK == status) {
			status = mBusAttachment->Start();
		} else {
			LOGD("BusAttachment::Start failed");
		}
		/* Connect to the daemon */
		if (ER_OK == status) {
			status = mBusAttachment->Connect();
			if (ER_OK != status) {
				LOGD("BusAttachment Connect failed.");
			}
		}
		LOGD("Created BusAttachment and connected");

		/* Register the IoE AllJoyn Audio Service Service */
		mSinkPlayer = new SinkPlayer(mBusAttachment);
		mSinkPlayer->AddListener(this);
		mSinkPlayer->SetPreferredFormat(MIMETYPE_AUDIO_RAW);
		Register(mBusAttachment);
	}
}

void MyAllJoynCode::SetDataSource(const char* dataSource)
{
	if(mDataSourcePath != NULL)
		delete mDataSourcePath;
	mDataSourcePath = strdup(dataSource);
	SetDataSourceHelper();
}

void MyAllJoynCode::SetDataSourceHelper()
{
	uint8_t strLenSource = strlen(mDataSourcePath);
	bool isWavFile = strLenSource > 4 && 0 == strncmp(mDataSourcePath + strLenSource - 4, ".wav",4);
	if(mCurrentDataSource != NULL) {
		if(isWavFile) {
			((WavDataSource*)mCurrentDataSource)->Close();
		}
	}
	if(isWavFile) {
		if(mCurrentDataSource == NULL)
			mCurrentDataSource = new WavDataSource();
		((WavDataSource*)mCurrentDataSource)->Open(mDataSourcePath);
	}
	if(mCurrentDataSource != NULL) {
		mSinkPlayer->SetDataSource(mCurrentDataSource);
		if(wasStopped)
			mSinkPlayer->OpenAllSinks();
		wasStopped = false;
	}
}

void MyAllJoynCode::AddSink(const char *name, const char *path, uint16_t port)
{
	//LOGD("Name: %s, Path: %s, port:%d", name, path, port);
	if( !mSinkPlayer->HasSink(name) )
		mSinkPlayer->AddSink(name, port, path);
}

void MyAllJoynCode::RemoveSink(const char *name)
{
	if( mSinkPlayer->HasSink(name) )
		mSinkPlayer->RemoveSink(name);
}

void MyAllJoynCode::Start()
{
	if(wasStopped)
		mSinkPlayer->OpenAllSinks();
	wasStopped = false;
	if( !mSinkPlayer->IsPlaying() )
		mSinkPlayer->Play();
}

void MyAllJoynCode::Pause()
{
	if( mSinkPlayer->IsPlaying() )
		mSinkPlayer->Pause();
}

void MyAllJoynCode::Stop()
{
	if( mSinkPlayer->IsPlaying() )
	{
		mSinkPlayer->CloseAllSinks();
		int8_t strLenSource = strlen(mDataSourcePath);
		if(strLenSource > 4 && 0 == strncmp(mDataSourcePath + strLenSource - 4, ".wav",4)) {
			((WavDataSource*)mCurrentDataSource)->Close();
			((WavDataSource*)mCurrentDataSource)->Open(mDataSourcePath);
		}
		wasStopped = true;
	}
}

void MyAllJoynCode::ChangeVolume(float value)
{
	int16_t low;
	int16_t high;
	int16_t step;
	int16_t newVolume;
	for (std::list<qcc::String>::iterator it = mSinkNames.begin(); it != mSinkNames.end(); ++it) {
		mSinkPlayer->GetVolumeRange(it->c_str(), low, high, step);
		//value is from 0 to 1 for Android
		newVolume = ((high-low)*value) + low;
		mSinkPlayer->SetVolume(it->c_str(), newVolume);
	}
}

void MyAllJoynCode::Mute()
{
	isMuted = !isMuted;
	for (std::list<qcc::String>::iterator it = mSinkNames.begin(); it != mSinkNames.end(); ++it) {
		LOGD("Muting device: %s", it->c_str());
		mSinkPlayer->SetMute(it->c_str(), isMuted);
	}
}

void MyAllJoynCode::Release()
{
	Unregister();
	mSinkPlayer->RemoveAllSinks();
	while( mSinkPlayer->GetSinkCount() > 0 )
	        usleep(100 * 1000);
	delete mSinkPlayer;
	mSinkPlayer = NULL;
	/* Deallocate the BusAttachment */
	if (mBusAttachment) {
		delete mBusAttachment;
		mBusAttachment = NULL;
	}
}

void MyAllJoynCode::SinkFound( Service *sink ) {
	const char *name = sink->name.c_str();
	const char *path = sink->path.c_str();
	LOGD("Found %s objectPath=%s, sessionPort=%d\n", name, path, sink->port);
	JNIEnv* env;
	jint jret = vm->GetEnv((void**)&env, JNI_VERSION_1_2);
	if (JNI_EDETACHED == jret) {
		vm->AttachCurrentThread(&env, NULL);
	}

	jclass jcls = env->GetObjectClass(jobj);
	jmethodID mid = env->GetMethodID(jcls, "SinkFound", "(Ljava/lang/String;Ljava/lang/String;S)V");
	if (mid == 0) {
		LOGD("Failed to get Java SinkFound");
	} else {
		jstring jName = env->NewStringUTF(name);
		jstring jPath = env->NewStringUTF(path);
		env->CallVoidMethod(jobj, mid, jName, jPath, sink->port);
		env->DeleteLocalRef(jName);
		env->DeleteLocalRef(jPath);
	}
	if (JNI_EDETACHED == jret) {
		vm->DetachCurrentThread();
	}
}

void MyAllJoynCode::SinkLost( Service *sink ) {
	const char *name = sink->name.c_str();
	LOGD("Lost %s\n", name);
	JNIEnv* env;
	jint jret = vm->GetEnv((void**)&env, JNI_VERSION_1_2);
	if (JNI_EDETACHED == jret) {
		vm->AttachCurrentThread(&env, NULL);
	}

	jclass jcls = env->GetObjectClass(jobj);
	jmethodID mid = env->GetMethodID(jcls, "SinkLost", "(Ljava/lang/String;)V");
	if (mid == 0) {
		LOGD("Failed to get Java SinkLost");
	} else {
		jstring jName = env->NewStringUTF(name);
		env->CallVoidMethod(jobj, mid, jName);
		env->DeleteLocalRef(jName);
	}
	if (JNI_EDETACHED == jret) {
		vm->DetachCurrentThread();
	}
}

void MyAllJoynCode::SinkAdded( const char *name ) {
	mSinkNames.push_back(name);
	LOGD("SinkAdded: %s\n", name);

	mSinkPlayer->OpenSink(name);
	LOGD("SinkOpened: %s\n", name);
	JNIEnv* env;
	jint jret = vm->GetEnv((void**)&env, JNI_VERSION_1_2);
	if (JNI_EDETACHED == jret) {
		vm->AttachCurrentThread(&env, NULL);
	}

	jclass jcls = env->GetObjectClass(jobj);
	jmethodID mid = env->GetMethodID(jcls, "SinkAdded", "(Ljava/lang/String;)V");
	if (mid == 0) {
		LOGD("Failed to get Java SinkAdded");
	} else {
		jstring jName = env->NewStringUTF(name);
		env->CallVoidMethod(jobj, mid, jName);
		env->DeleteLocalRef(jName);
	}
	if (JNI_EDETACHED == jret) {
		vm->DetachCurrentThread();
	}
}

void MyAllJoynCode::SinkAddFailed( const char *name ) {
	LOGD("SinkAddFailed: %s\n", name);
	JNIEnv* env;
	jint jret = vm->GetEnv((void**)&env, JNI_VERSION_1_2);
	if (JNI_EDETACHED == jret) {
		vm->AttachCurrentThread(&env, NULL);
	}

	jclass jcls = env->GetObjectClass(jobj);
	jmethodID mid = env->GetMethodID(jcls, "SinkAddFailed", "(Ljava/lang/String;)V");
	if (mid == 0) {
		LOGD("Failed to get Java SinkAddFailed");
	} else {
		jstring jName = env->NewStringUTF(name);
		env->CallVoidMethod(jobj, mid, jName);
		env->DeleteLocalRef(jName);
	}
	if (JNI_EDETACHED == jret) {
		vm->DetachCurrentThread();
	}
}

void MyAllJoynCode::SinkRemoved( const char *name, bool lost ) {
	mSinkNames.remove(qcc::String(name));
	LOGD("SinkRemoved: %s lost=%d\n", name, lost);
	JNIEnv* env;
	jint jret = vm->GetEnv((void**)&env, JNI_VERSION_1_2);
	if (JNI_EDETACHED == jret) {
		vm->AttachCurrentThread(&env, NULL);
	}

	jclass jcls = env->GetObjectClass(jobj);
	jmethodID mid = env->GetMethodID(jcls, "SinkRemoved", "(Ljava/lang/String;Z)V");
	if (mid == 0) {
		LOGD("Failed to get Java SinkRemoved");
	} else {
		jstring jName = env->NewStringUTF(name);
		env->CallVoidMethod(jobj, mid, jName, lost);
		env->DeleteLocalRef(jName);
	}
	if (JNI_EDETACHED == jret) {
		vm->DetachCurrentThread();
	}
}

