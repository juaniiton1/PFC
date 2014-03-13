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
package com.joanpfc.race;

import java.io.IOException;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;

import com.easyndk.classes.AndroidNDKHelper;

public class PFCRace extends Cocos2dxActivity{
	
    protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
        AndroidNDKHelper.SetNDKReciever(this);
	}

    public Cocos2dxGLSurfaceView onCreateView() {
    	Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
    	// PFCRace should create stencil buffer
    	glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
    	glSurfaceView.setKeepScreenOn(true);
    	
    	return glSurfaceView;
    }

    static {
        System.loadLibrary("cocos2dcpp");
    }
    
    public void SetNLeds(JSONObject prms) throws JSONException, IOException
    {
    	Log.i("PRMS:", prms.toString());
    	int nLeds = prms.getInt("nLeds");
    	Connection.getInstance().writeSocket(1);
    	Connection.getInstance().writeSocket(nLeds);
    }
    
    public void JugadorEntra(JSONObject prms) throws JSONException, IOException
    {
    	Log.i("PRMS:", prms.toString());
    	int iColor = prms.getInt("color");
    	Connection.getInstance().writeSocket(2);
    	Connection.getInstance().writeSocket(iColor);
    }
    
    public void EmpiezaCarrera(JSONObject prms) throws JSONException, IOException
    {
    	Log.i("PRMS:", prms.toString());
    	int iColor = prms.getInt("color");
    	Connection.getInstance().writeSocket(3);
    	Connection.getInstance().writeSocket(iColor);
    }
    
    public void JugadorAvanza(JSONObject prms) throws JSONException, IOException
    {
    	Log.i("PRMS:", prms.toString());
    	int iColor = prms.getInt("color");
    	Connection.getInstance().writeSocket(4);
    	Connection.getInstance().writeSocket(iColor);
    }
    
    public void JugadorTermina(JSONObject prms) throws JSONException, IOException
    {
    	Log.i("PRMS:", prms.toString());
    	int iColor = prms.getInt("color");
    	Connection.getInstance().writeSocket(5);
    	Connection.getInstance().writeSocket(iColor);
    }
    
    public void JugadorSale(JSONObject prms) throws JSONException, IOException
    {
    	Log.i("PRMS:", prms.toString());
    	int iColor = prms.getInt("color");
    	Connection.getInstance().writeSocket(6);
    	Connection.getInstance().writeSocket(iColor);
    }
    
//    public void SendColors(JSONObject prms) throws JSONException, IOException
//    {
//    	Log.i("PRMS:", "Entro en send colors atentos!");
//    	Log.i("PRMS:", prms.toString());
//    	int nLeds = prms.getInt("nLeds");
//    	JSONArray colores = prms.getJSONArray("colores");
//    	Log.i("JSON:", "nLeds:"+nLeds+"; colores:"+colores.toString());
//
//    	Connection.getInstance().writeSocket(1);
//    	Connection.getInstance().writeSocket(nLeds);
//    	Connection.getInstance().writeSocket(2);
//    	
//    	for (int i = 1; i <= nLeds; i++)
//    	{
//    		// Mandamos del ultimo al primero
//    		int rgb 	= colores.getInt(nLeds-i);
//    		int red 	= Color.red(rgb);
//    		int green 	= Color.green(rgb);
//    		int blue 	= Color.blue(rgb);
//    		Connection.getInstance().writeSocket(red);
//    		Connection.getInstance().writeSocket(green);
//    		Connection.getInstance().writeSocket(blue);
//    	}
//    	
//    	Connection.getInstance().writeSocket(3);
//    }
    
    public void SelectorConnect(JSONObject prms) throws JSONException
    {
    	String ip = prms.getString("ip");
    	String port = prms.getString("port");
    	Log.i("CONEXION", "HELLO??? "+ip+" "+port);
    	new ConnectSocket(PFCRace.this).execute(ip, port);
    }
    
    public void ConnectionOk()
    {
    	Log.i("CONEXION", "CONNECTION OK!!!");
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
    	Log.i("CONEXION", "CONNECTION ERROR! :(");
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
}
