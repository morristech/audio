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

package org.alljoyn.services.audio.android;

import android.app.Activity;
import android.app.DialogFragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.view.Window;
import android.widget.Button;
import android.widget.ListView;

public class SinkSelectDialog extends DialogFragment {
	
	public interface SinkSelectDialogListener {
        void onDialogSinkEnable(String speakerName, String speakerPath, short speakerPort);
        void onDialogSinkDisable(String speakerName);
    }
	
	private SinkListAdapter mSinkListAdapter;
	
	public SinkSelectDialog() {
		
	}
	
	public void SetActivity(Activity activity) {
		mSinkListAdapter = new SinkListAdapter(activity, R.layout.genericitem);
	}
	
	public void AddSink(String speakerName, String speakerPath, short speakerPort) {
		mSinkListAdapter.add(speakerName, speakerPath, speakerPort);
	}
	
	public void RemoveSink(String speakerName) {
		mSinkListAdapter.remove(speakerName);
	}
	
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
		getDialog().getWindow().requestFeature(Window.FEATURE_NO_TITLE);
		View view = inflater.inflate(R.layout.selectspeakerdialog, container);
		ListView speakerList = (ListView) view.findViewById(R.id.speakerList);
    	speakerList.setAdapter(mSinkListAdapter);
//    	speakerList.setOnItemClickListener(new OnItemClickListener() {
//			public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
//				((SinkSelectDialogListener) getActivity()).onFinishSinkSelectDialog(mSinkListAdapter.getName(position),
//						mSinkListAdapter.getPath(position), mSinkListAdapter.getPort(position));
//				mSinkListAdapter.toggleSelected(position);
//			}
//	    });
    	Button closeButton = (Button) view.findViewById(R.id.closePopup);
    	closeButton.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View arg0) {
				getDialog().dismiss();
			}
    	});
		return view;
	}
	
	
}