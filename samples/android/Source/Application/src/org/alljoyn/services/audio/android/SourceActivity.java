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

import android.os.Bundle;
import android.app.Activity;
import android.app.Dialog;
import android.support.v4.app.FragmentActivity;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;

public class SourceActivity extends FragmentActivity implements SinkSelectDialog.SinkSelectDialogListener {
    private UIHelper mUIHelper;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.player);
        mUIHelper = new UIHelper(this);
        mUIHelper.init();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.quit_menu, menu);
        return true;
    }
    
    @Override
	public boolean onOptionsItemSelected(MenuItem item) {
	    // Handle item selection
	    switch (item.getItemId()) {
	    case R.id.quit_menu:
	    	finish();
	    	android.os.Process.killProcess(android.os.Process.myPid());
	        return true;
	    default:
	        return super.onOptionsItemSelected(item);
	    }
	}
    
    @Override
    protected void onDestroy() {
        super.onDestroy();
        mUIHelper.onDestroy();
    }
    
    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
    	return mUIHelper.onKeyDown(keyCode, event);
    }
    
    @Override
    public void onDialogSinkEnable(String speakerName, String speakerPath, short speakerPort) {
    	mUIHelper.onDialogSinkEnable(speakerName, speakerPath, speakerPort);
    }
    
    public void onDialogSinkDisable(String speakerName) {
    	mUIHelper.onDialogSinkDisable(speakerName);
    }
}
