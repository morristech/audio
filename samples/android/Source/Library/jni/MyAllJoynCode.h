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

#include <alljoyn/BusAttachment.h>
#include <alljoyn/ProxyBusObject.h>
#include <alljoyn/DBusStd.h>
#include <alljoyn/AllJoynStd.h>
#include "Constants.h"
#include "LogHelper.h"
#include <alljoyn/audio/SinkPlayer.h>
#include <alljoyn/audio/SinkSearcher.h>
#include <alljoyn/audio/DataSource.h>

#ifndef _MY_ALLJOYN_CODE_
#define _MY_ALLJOYN_CODE_

class MyAllJoynCode : public ajn::services::SinkSearcher, public ajn::services::SinkListener  {
public:
	MyAllJoynCode(LogHelper* logger, JavaVM* vm, jobject jobj) : vm(vm), jobj(jobj),
		logger(logger), mBusAttachment(NULL), mSinkPlayer(NULL), mCurrentDataSource(NULL), isMuted(false)  {};

	~MyAllJoynCode() {
		Release();
		if(logger)
			delete logger;
		logger = NULL;
		if(mSinkPlayer)
			delete mSinkPlayer;
		mSinkPlayer = NULL;
	};

	void Prepare(const char* packageName);

	void SetDataSource(const char* dataSource);

	void AddSink(const char *name, const char *path, uint16_t port);

	void RemoveSink(const char *name);

	void Start();

	void Pause();

	void Stop();

	void ChangeVolume(float value);

	void Mute();

	void Release();

	/* SinkSearcher */
	virtual void SinkFound( Service *sink );

	virtual void SinkLost( Service *sink );

	/* SinkListener */
    void SinkAdded( const char *name );

    void SinkAddFailed( const char *name );

    void SinkRemoved( const char *name, bool lost );

private:
    void SetDataSourceHelper(const char* dataSource);

private:
    JavaVM* vm;
	jobject jobj;
	LogHelper* logger;
	char * mDataSourcePath;
	/* Static data */
	qcc::String wellKnownName;
	ajn::BusAttachment* mBusAttachment;
	ajn::services::SinkPlayer* mSinkPlayer;
	ajn::services::DataSource* mCurrentDataSource;
	std::list<qcc::String> mSinkNames;
	bool isMuted;
};

#endif //_MY_ALLJOYN_CODE_
