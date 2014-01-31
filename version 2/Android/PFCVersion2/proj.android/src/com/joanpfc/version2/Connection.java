package com.joanpfc.version2;

import java.io.OutputStream;
import java.net.Socket;

public class Connection {

	private static Connection mInstance = null;
	
	private Socket mSocket;
	private OutputStream mOutput;
	
	private Connection() {
		
	}
	
	public static Connection getInstance(){
		if (mInstance == null) {
			mInstance = new Connection();
		}
		return mInstance;
	}
	
	public void setSocket(Socket value) {
		mSocket = value;
	}
	
	public Socket getSocket() {
		return this.mSocket;
	}
	
	public void setOutput(OutputStream value) {
		mOutput = value;
	}
	
	public OutputStream getOutput() {
		return this.mOutput;
	}
	
}
