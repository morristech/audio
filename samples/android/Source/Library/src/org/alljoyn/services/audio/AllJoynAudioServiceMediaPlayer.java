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

package org.alljoyn.services.audio;

import java.io.IOException;

import android.media.AudioManager;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;
import android.os.Message;
import android.util.Log;

public class AllJoynAudioServiceMediaPlayer extends MediaPlayer {
	private static final String TAG = "AllJoynAudioMediaPlayer";
	
	/* Handler used to make calls to AllJoyn methods. See onCreate(). */
    private Handler mBusHandler;
    private AllJoynAudioServiceListener mListener;
    private boolean mStreamOverAllJoyn = false;
    private boolean isPlayingOverStream = false;
    private int numEnabledSinks = 0;
    private String mDataSourcePath;
    
    /* These are the messages sent to the BusHandler to avoid blocking the UI. */
    private static final int PREPARE = 0;
    private static final int SET_DATA_SOURCE = 1;
    private static final int SET_ALLJOYN_SPEAKER = 2;
    private static final int REMOVE_ALLJOYN_SPEAKER = 3;
	private static final int START_STREAMING = 4;
	private static final int PAUSE_STREAMING = 5;
	private static final int STOP_STREAMING = 6;
	private static final int CHANGE_VOLUME = 7;
	private static final int MUTE_VOLUME = 8;
	private static final int RELEASE = -1;

	/*
	 * Native JNI methods
	 */
	static {
		System.loadLibrary("easy_alljoyn_audio");
	}

	/**  */
	private native void Prepare(String packageName);

	/**  */
	private native void SetDataSource(String dataSource);
	
	/**  */
	private native void SendAudioPacket(byte[] audioData);
	
	/**  */
	private native void AddSink(String name, String path, short port);
	
	/**  */
	private native void RemoveSink(String name);
	
	/**  */
	private native void Start();
	
	/**  */
	private native void Pause();
	
	/**  */
	private native void Stop();
	
	/**  */
	private native void ChangeVolume(float volume);
	
	/**  */
	private native void Mute();

	/**  */
	private native void Release();
    
    public AllJoynAudioServiceMediaPlayer() {
    	/* Make all AllJoyn calls through a separate handler thread to prevent blocking the UI. */
        HandlerThread busThread = new HandlerThread("BusHandler");
        busThread.start();

    	mBusHandler = new BusHandler(busThread.getLooper());
        mBusHandler.sendEmptyMessage(PREPARE);
        numEnabledSinks = 0;
    }
    
    public void setAllJoynAudioServiceListener(AllJoynAudioServiceListener listener) {
    	mListener = listener;
    }
        
    public void setDataSource(String path) throws IllegalStateException, IOException, IllegalArgumentException, SecurityException {
    	//Should stream over alljoyn
    	//Check if the path is for a wav file, if so pass it on down and let it stream as is
    	mDataSourcePath = path;
    	if(path.endsWith(".wav"))
    		mBusHandler.sendMessage(mBusHandler.obtainMessage(SET_DATA_SOURCE, path));
    	try{
    		super.setDataSource(path);
    	}catch(IllegalStateException e) {
    		throw e;
    	}catch(IOException e) {
    		throw e;
    	}catch(IllegalArgumentException e) {
    		throw e;
    	}catch(SecurityException e) {
    		throw e;
    	}
    }
    
    public void addAllJoynSink(String speakerName, String speakerPath, short speakerPort) {
    	if(mDataSourcePath == null || mDataSourcePath.length() == 0) {
    		throw new IllegalStateException("Call setDataSource prior to calling add");
    	}
    	mStreamOverAllJoyn = true;
    	numEnabledSinks++;
    	Message msg = mBusHandler.obtainMessage(SET_ALLJOYN_SPEAKER);
    	Bundle data = new Bundle();
    	data.putString("name", speakerName);
    	data.putString("path", speakerPath);
    	data.putShort("port", speakerPort);
    	msg.setData(data);
    	mBusHandler.sendMessage(msg);
    }
    
    public void removeAllJoynSink(String speakerName) {
    	numEnabledSinks--;
    	if(numEnabledSinks == 0)
    		mStreamOverAllJoyn = false;
    	Message msg = mBusHandler.obtainMessage(REMOVE_ALLJOYN_SPEAKER);
    	Bundle data = new Bundle();
    	data.putString("name", speakerName);
    	msg.setData(data);
    	mBusHandler.sendMessage(msg);
    }
    
    public void stopAllJoynSinks() {
    	
    }
    
    public void stopAllJoynSink(String AllJoynSinkName) {
    	
    }
    
    @Override
    public void start() {
    	if(!mStreamOverAllJoyn) {
    		super.start();
    		return;
    	}
    	isPlayingOverStream = true;
    	mBusHandler.sendEmptyMessage(START_STREAMING);
    }
    
    @Override
    public void pause() {
    	if(!mStreamOverAllJoyn) {
    		super.pause();
    		return;
    	}
    	isPlayingOverStream = false;
    	mBusHandler.sendEmptyMessage(PAUSE_STREAMING);
    }
    
    @Override
    public void stop() {
    	if(!mStreamOverAllJoyn) {
    		super.stop();
    		return;
    	}
    	isPlayingOverStream = false;
    	mBusHandler.sendEmptyMessage(STOP_STREAMING);
    }
    
    @Override
    public boolean isPlaying() {
    	if(!mStreamOverAllJoyn)
    		return super.isPlaying();
    	return isPlayingOverStream;
    }
    
    @Override
    public void setVolume(float leftVolume, float rightVolume) {
    	if(!mStreamOverAllJoyn) {
    		super.setVolume(leftVolume, rightVolume);
    		return;
    	}
    	Message msg = mBusHandler.obtainMessage(leftVolume == 0 ? MUTE_VOLUME : CHANGE_VOLUME);
    	Bundle data = new Bundle();
    	data.putFloat("volume", leftVolume);
    	msg.setData(data);
    	mBusHandler.sendMessage(msg);
    }
    
    public void release() {
    	super.release();
    	mBusHandler.sendEmptyMessage(RELEASE);
    }
    
    
    private class BusHandler extends Handler {
    	public BusHandler(Looper looper) {
    		super(looper);
    	}

    	@Override
    	public void handleMessage(Message msg) {
    		Bundle data = msg.getData();
    		switch (msg.what) {
    		/* Connect to the bus and start our service. */
    		case PREPARE:
    			Prepare("alljoyn.audio.streaming.IoE");
    			break;
    		case SET_DATA_SOURCE:
    			SetDataSource((String)msg.obj);
    			break;
    		case SET_ALLJOYN_SPEAKER:
    			AddSink(data.getString("name"), data.getString("path"), data.getShort("port"));
    			break;
    		case REMOVE_ALLJOYN_SPEAKER:
    			RemoveSink(data.getString("name"));
    			break;
    		case START_STREAMING:
    			Start();
    			break;
    		case PAUSE_STREAMING:
    			Pause();
    			break;
    		case STOP_STREAMING:
    			Stop();
    			break;
    		case CHANGE_VOLUME:
    			ChangeVolume(data.getFloat("volume"));
    			break;
    		case MUTE_VOLUME:
    			Mute();
    			break;
    		/* Release all resources acquired in connect. */
    		case RELEASE:
    			Release();
    			break;
    		default:
    			LogToUI("DEFAULT CASE STATEMENT IN BUSHANDLER");
    			break;
    		}
    	}
    }
    
    private void LogToUI(String msg) {
		Log.d(TAG, msg);
	}

	private void SinkFound( String name, String path, short port ) {
		if(mListener != null)
			mListener.SinkFound(name, path, port);
	}

	private void SinkLost( String name ) {
		if(mListener != null)
			mListener.SinkLost(name);
	}

	private void SinkAdded( String name ) {
		if(mListener != null)
			mListener.SinkReady(name);
	}

	private void SinkAddFailed( String name ) {
		if(mListener != null)
			mListener.SinkError(name);
	}

	private void SinkRemoved( String name, boolean lost ) {
		if(mListener != null)
			mListener.SinkRemoved(name);
	}
}
