/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package com.joanpfc.version2;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.json.JSONException;
import org.json.JSONObject;

import com.easyndk.classes.AndroidNDKHelper;

import android.app.AlertDialog;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.view.View.OnClickListener;
import android.view.ViewGroup.LayoutParams;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.Toast;

public class PFCVersion2 extends Cocos2dxActivity{
	

	private FrameLayout rootView = null;
	
	/* Helper method to get the hold of Cocos2dx Changable View,
	 * You can add others views using this view
	 */
	private FrameLayout GetRootView()
	{
		if (this.rootView == null)
		{
			this.rootView = (FrameLayout)this.getWindow().getDecorView().findViewById(android.R.id.content);
		}
		return this.rootView;
	}
	
    protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
        AndroidNDKHelper.SetNDKReciever(this);
        //this.AddButton();	
	}

    public Cocos2dxGLSurfaceView onCreateView() {
    	Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
    	// PFCVersion2 should create stencil buffer
    	glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
    	
    	return glSurfaceView;
    }

    static {
        System.loadLibrary("cocos2dcpp");
    }
    
    public void LoadInterstitial(JSONObject prms)
    {
	    // Show an interstitial
    	Toast.makeText(this, "Hello hello!!", Toast.LENGTH_SHORT).show();
    }
    
    public void ChangeSomethingInCocos()
    {
        // If you want to change anything that cocos handles, please run it on GLThread
        // Because cocos is a non threaded environment, it is required to queue stuff there
        // Every call on NDK opens up a new thread, hence making inconsistency in cocos and NDK
        
    	this.runOnGLThread(new Runnable()
                           {
			@Override
			public void run()
			{
				// TODO Auto-generated method stub
				AndroidNDKHelper.SendMessageWithParameters("ChangeLabelSelector", null);
			}
		});
    }
    
    public void SampleSelector(JSONObject prms)
    {
    	Log.v("SampleSelector", "purchase something called");
    	Log.v("SampleSelector", "Passed params are : " + prms.toString());
    	
    	String CPPFunctionToBeCalled = null;
		try
		{
			CPPFunctionToBeCalled = prms.getString("to_be_called");
		}
		catch (JSONException e)
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    	
    	AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setMessage("This is a sample popup on Android").
        setTitle("Hello World!").
        setNeutralButton("OK", null).show();
    	
    	AndroidNDKHelper.SendMessageWithParameters(CPPFunctionToBeCalled, null);
    }
    
    private void AddButton()
    {
        Button tapButton = new Button(this);
        tapButton.setText("Tap to change text");
        tapButton.setLayoutParams(new LayoutParams(
                                                   ViewGroup.LayoutParams.WRAP_CONTENT,
                                                   ViewGroup.LayoutParams.WRAP_CONTENT)
                                  );
        
        tapButton.setOnClickListener(new OnClickListener()
        {
			@Override
			public void onClick(View v)
			{
				// TODO Auto-generated method stub
				PFCVersion2.this.ChangeSomethingInCocos();
			}
		});
        
        this.GetRootView().addView(tapButton);
    }
}
