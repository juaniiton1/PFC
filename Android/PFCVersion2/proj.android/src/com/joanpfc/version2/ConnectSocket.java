package com.joanpfc.version2;

import java.io.IOException;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.Socket;

import android.content.Context;
import android.os.AsyncTask;
import android.util.Log;

public class ConnectSocket extends AsyncTask<String, Void, Boolean> {

	//private Context mContext;
	private Socket mSocket;
	private OutputStream mOutput;
	private PFCVersion2 myActivity;
	
	public ConnectSocket(Context context) {
		//mContext = context;
		myActivity = (PFCVersion2) context;
	}
	
	@Override
	protected Boolean doInBackground(String... params) {

		String ip 	= params[0];
		int port 	= Integer.parseInt(params[1]);

		try {
			InetAddress address = InetAddress.getByName(ip);
			mSocket 			= new Socket(address,port);
			mOutput 			= mSocket.getOutputStream();
		} catch (IOException e) {
			e.printStackTrace();
		}
    	
		return true;
	}

    protected void onPostExecute(Boolean result) {

    	if ((mSocket != null) && (mOutput != null) && mSocket.isConnected()) {
    		
    		Log.v("Connection","Connected to Server!");
    		Connection.getInstance().setSocket(mSocket);
    		Connection.getInstance().setOutput(mOutput);
    		myActivity.ConnectionOk();
    	
    	} else {
    		Log.v("Connection","Could not connect to Server!");
    		myActivity.ConnectionError();
    	}
    	
    }
	
}
