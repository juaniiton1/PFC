package com.joan.pfc.version1;

import java.io.IOException;
import java.io.OutputStream;
import java.net.Socket;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

import com.larswerkman.holocolorpicker.ColorPicker;
import com.larswerkman.holocolorpicker.ColorPicker.OnColorChangedListener;

public class ColorPickerActivity extends Activity {

	private TextView textView_color_old;
	private TextView textView_color_new;
	private ColorPicker picker;
	
	private OutputStream mOutput;
	private Socket mSocket;
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_color_picker);
		
		mOutput = Connection.getInstance().getOutput();
		mSocket = Connection.getInstance().getSocket();
		
		textView_color_old 	= (TextView) findViewById(R.id.color_old);
		textView_color_new 	= (TextView) findViewById(R.id.color_new);
		picker 				= (ColorPicker) findViewById(R.id.picker);

		//To get the color
		picker.getColor();

		//To set the old selected color u can do it like this
		picker.setOldCenterColor(picker.getColor());
		
		// adds listener to the colorpicker which is implemented in the activity
		picker.setOnColorChangedListener(new OnColorChangedListener() {

			@Override
			public void onColorChanged(int color) {
				// TODO Auto-generated method stub
				textView_color_new.setText(
						"R:"+Color.red(color)+" G:"+Color.green(color)+" B:"+Color.blue(color));
				textView_color_new.setTextColor(picker.getColor());
				
			}
			
		});
		
		Button button_change_color = (Button) findViewById(R.id.button_set_color);
		button_change_color.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				String string_color_new = (String) textView_color_new.getText();
				textView_color_old.setText(string_color_new);
				textView_color_old.setTextColor(picker.getColor());
				picker.setOldCenterColor(picker.getColor());
				
				/////////////////////
				// mandarlo al led //
				/////////////////////
				try {
					sendColor(picker.getColor());
				} catch (IOException e) {
					e.printStackTrace();
				}
				
			}
			
		});
		
	}
	
	void sendColor(int color) throws IOException {
		int red 	= Color.red(color);
		int green 	= Color.green(color);
		int blue 	= Color.blue(color);
    	Log.v("COLOR", "R:"+red+" G:"+green+" B:"+blue);
		writeSocket(1);
		writeSocket(red);
		writeSocket(2);
		writeSocket(green);
		writeSocket(3);
		writeSocket(blue);
		writeSocket(4);
	}
	
    void writeSocket(int value) throws IOException
    {
    	Log.v("WRITESOCKET", ""+value);
    	if(mOutput != null) {
	    	mOutput.write(value);
	    	mOutput.flush();
    	}
    }

    void closeSocket() throws IOException
    {
    	if(mOutput != null && mSocket != null) {
    		mOutput.close();
	    	mSocket.close();
    		Connection.getInstance().setSocket(null);
    		Connection.getInstance().setOutput(null);
    	}
    }

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.color_picker, menu);
		return true;
	}

//    @Override
//    protected void onRestart(){
//    	Log.v("onRestart","onRestart");
//		new ConnectSocket(MainActivity.this).execute(ip, port);
//    	super.onResume();
//    }
//
    @Override
    protected void onStop(){
    	Log.v("onStop", "onStop");
    	try {
			closeSocket();
		} catch (IOException e) {
			e.printStackTrace();
		}
    	super.onStop();
    }

}
