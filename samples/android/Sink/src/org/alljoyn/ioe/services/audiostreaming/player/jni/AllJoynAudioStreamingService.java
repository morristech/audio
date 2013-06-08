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

import android.app.Service;
import android.content.Intent;
import android.os.HandlerThread;
import android.os.IBinder;

public class AllJoynAudioStreamingService extends Service{
	private static final String TAG = "IoEAudioStreamingPlayer";
	
	private BusHandler mBusHandler;
	
	@Override
	public IBinder onBind(Intent arg0) {
		// TODO Auto-generated method stub
		return null;
	}
	
	@Override
	public void onCreate() {
		super.onCreate();
		setupService();
	}
		
	private void setupService() {	
		if(mBusHandler != null)
			return;
		/* Make all AllJoyn calls through a separate handler thread to prevent blocking the UI. */
        HandlerThread busThread = new HandlerThread("BusHandler");
        busThread.start();
    	mBusHandler = new BusHandler(busThread.getLooper(), this);
        mBusHandler.sendEmptyMessage(BusHandler.INITIALIZE);
	}
	 
	@Override
	public void onDestroy() {
		mBusHandler.handleShutdown();
	}
	
	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		setupService();
		return START_STICKY;
	}
}
