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

import java.io.IOException;
import java.io.OutputStream;
import java.net.Socket;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;

import com.easyndk.classes.AndroidNDKHelper;

public class PFCVersion2 extends Cocos2dxActivity{
	
    protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
        AndroidNDKHelper.SetNDKReciever(this);
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
    
    public void SampleSelectorWithData(JSONObject prms) throws JSONException
    {
    	String textToToast = "no funciona :(";
		textToToast = prms.getString("text_to_toast");
		Log.i("TEXT TO TOAST", textToToast);
		
    	Toast.makeText(this, textToToast, Toast.LENGTH_SHORT).show();
    }
    
    public void SendColors(JSONObject prms) throws JSONException, IOException
    {
    	Log.i("PRMS:", "Entro en send colors atentos!");
    	Log.i("PRMS:", prms.toString());
    	int nLeds = prms.getInt("nLeds");
    	JSONArray colores = prms.getJSONArray("colores");
    	Log.i("JSON:", "nLeds:"+nLeds+"; colores:"+colores.toString());

    	Connection.getInstance().writeSocket(1);
    	Connection.getInstance().writeSocket(nLeds);
    	Connection.getInstance().writeSocket(2);
    	
    	// for (int i = 0; i < colores.length(); i++)
    	for (int i = 0; i < nLeds; i++)
    	{
    		JSONArray colores_aux = colores.getJSONArray(i);
    		for (int j = 0; j < 3; j++)
    		{
    			// Enviamos cada uno de los red, green, blue de cada led
    			Connection.getInstance().writeSocket(colores_aux.getInt(j));
    		}
    	}
    	
    	Connection.getInstance().writeSocket(3);
    }
    
//    void sendColor(int color) throws IOException {
//		int red 	= Color.red(color);
//		int green 	= Color.green(color);
//		int blue 	= Color.blue(color);
//    	Log.v("COLOR", "R:"+red+" G:"+green+" B:"+blue);
//		writeSocket(1);
//		writeSocket(red);
//		writeSocket(2);
//		writeSocket(green);
//		writeSocket(3);
//		writeSocket(blue);
//		writeSocket(4);
//	}
    
    public void SelectorConnect(JSONObject prms) throws JSONException
    {
    	String ip = prms.getString("ip");
    	String port = prms.getString("port");
    	new ConnectSocket(PFCVersion2.this).execute(ip, port);
    }
    
    public void ConnectionOk()
    {
    	Toast.makeText(this, "BIEN!", Toast.LENGTH_SHORT).show();
    	this.runOnGLThread(new Runnable()
    	{
    		@Override
    		public void run()
    		{
    			// TODO Auto-generated method stub
    			AndroidNDKHelper.SendMessageWithParameters("connectionOk", null);
    		}
		});
    }
    
    public void ConnectionError()
    {
    	Toast.makeText(this, "MIERDA :/ pero bien :D", Toast.LENGTH_SHORT).show();
    	this.runOnGLThread(new Runnable()
    	{
    		@Override
    		public void run()
    		{
    			// TODO Auto-generated method stub
    			AndroidNDKHelper.SendMessageWithParameters("connectionError", null);
    		}
		});
    }
    
//    public void ChangeSomethingInCocos()
//    {
//        // If you want to change anything that cocos handles, please run it on GLThread
//        // Because cocos is a non threaded environment, it is required to queue stuff there
//        // Every call on NDK opens up a new thread, hence making inconsistency in cocos and NDK
//        
//    	this.runOnGLThread(new Runnable()
//                           {
//			@Override
//			public void run()
//			{
//				// TODO Auto-generated method stub
//				AndroidNDKHelper.SendMessageWithParameters("ChangeLabelSelector", null);
//			}
//		});
//    }

}