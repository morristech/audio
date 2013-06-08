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

package org.alljoyn.ioe.services.audiostreaming.player.jni;

import android.content.Context;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;

/*
 * We are using a Handler because ...
 */
public class BusHandler extends Handler {
	public static String TAG = "IoEAudioStreamingPlayer";
	
	private Context mContext;

	/* These are the messages sent to the BusHandler from the UI. */
	public static final int INITIALIZE = 0;
	public static final int SHUTDOWN = 10;

	/*
	 * Native JNI methods
	 */
	static {
		System.loadLibrary("alljoyn_audio_service");
	}

	/**  */
	private native void initialize(String packageName);

	/**  */
	private native void shutdown();

	public BusHandler(Looper looper, Context context) {
		super(looper);
		this.mContext = context;
	}
	
	public void handleShutdown() {
		shutdown();
	}

	@Override
	public void handleMessage(Message msg) {
		switch (msg.what) {
		/* Connect to the bus and start our service. */
		case INITIALIZE:
		{ 
			initialize(mContext.getPackageName());
			break;
		}
		/* Release all resources acquired in connect. */
		case SHUTDOWN: {
			shutdown();
			break;   
		}
		default:
			break;
		}
	}
//
//	private void LogMessage(String msg) {
//		Log.d(TAG, msg);
//	}
}
