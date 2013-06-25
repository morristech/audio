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

import java.util.ArrayList;

import org.alljoyn.services.audio.android.SinkSelectDialog.SinkSelectDialogListener;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.View.OnClickListener;
import android.widget.ArrayAdapter;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.TextView;

public class SinkListAdapter extends ArrayAdapter<Object> {

	private Activity mActivity;
	private static ArrayList<Object> mList = new ArrayList<Object>(); 

	class SessionInfoHolder{
		public String name;
		public String path;
		public short port;
		public boolean isChecked;

		public SessionInfoHolder(String name, String path, short port){
			this.name = name;
			this.path = path;
			this.port = port;
			isChecked = false;
		}
	}
	
	public SinkListAdapter(Activity activity, int textViewResourceId) {
		super(activity, textViewResourceId, mList);
		mActivity = activity;
	}
	
	private int foundLoc(String name) {
		int i = -1;
		for(Object info:mList) {
			i++;
			if(((SessionInfoHolder)info).name.equals(name))
				return i;
		}
		return -1;
	}
	
	public void add(String name, String path, short port) {
		if(-1 == foundLoc(name)) {
			mList.add(new SessionInfoHolder(name, path, port));
			if(mActivity != null) {
				mActivity.runOnUiThread(new Runnable() {
					public void run() {
						notifyDataSetChanged();	
					}
				});
			}
		}
	}
	
	public void remove(String name) {
		int foundLoc = foundLoc(name);
		if(-1 != foundLoc) {
			SessionInfoHolder rem = (SessionInfoHolder)mList.remove(foundLoc);
			if(mActivity != null) {
				mActivity.runOnUiThread(new Runnable() {
					public void run() {
						notifyDataSetChanged();	
					}
				});
			}
		}
	}
	
	public void clear() {
		mList.clear();
		notifyDataSetChanged();
	}
	
	@Override
	public int getCount() {
		return mList.size();
	}

	@Override
	public Object getItem(int pos) {
		return mList.get(pos);
	}
	
	public String getName(int pos) {
		return ((SessionInfoHolder)mList.get(pos)).name;
	}
	
	public String getPath(int pos) {
		return ((SessionInfoHolder)mList.get(pos)).path;
	}
	
	public short getPort(int pos) {
		return ((SessionInfoHolder)mList.get(pos)).port;
	}
	
	public void toggleSelected(int pos) {
		((SessionInfoHolder)mList.get(pos)).isChecked = true;
		if(mActivity != null) {
			mActivity.runOnUiThread(new Runnable() {
				public void run() {
					notifyDataSetChanged();	
				}
			});
		}
	}

	@Override
	public long getItemId(int id) {
		return id;
	}
	
	@Override
	public boolean areAllItemsEnabled()
    {
            return true;
    }

	@Override
    public boolean isEnabled(int post)
    {
            return true;
    } 

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		// Inflate a view template
		SessionInfoHolder info = (SessionInfoHolder)mList.get(position);
		//user == null implies my chat message
		if(mActivity != null) {
			LayoutInflater inflater = (LayoutInflater)mActivity.getSystemService (Context.LAYOUT_INFLATER_SERVICE);
			convertView = inflater.inflate(R.layout.genericitem, parent, false);
	
			final CheckBox cb = (CheckBox) convertView.findViewById(R.id.itemCheckbox);
			cb.setChecked(info.isChecked);
			final int loc = position;
			cb.setOnClickListener(new OnClickListener() {
				public void onClick(View arg0) {
					((SessionInfoHolder)mList.get(loc)).isChecked = !((SessionInfoHolder)mList.get(loc)).isChecked;
					try {
						if(((SessionInfoHolder)mList.get(loc)).isChecked) {
							((SinkSelectDialogListener)mActivity).onDialogSinkEnable(getName(loc), getPath(loc), getPort(loc));
						} else {
							((SinkSelectDialogListener)mActivity).onDialogSinkDisable(getName(loc));
						}
					}catch(Exception e) {
						((CheckBox)arg0).setChecked(false);
						((SessionInfoHolder)mList.get(loc)).isChecked = false;
						AlertDialog.Builder alertBuilder = new AlertDialog.Builder(mActivity);
			    		alertBuilder.setMessage(e.getMessage());
			    		alertBuilder.create().show();
					}
				}
			});
			cb.setText(info.name); 
		}		
		return convertView;
	}
}