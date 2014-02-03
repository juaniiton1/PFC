package com.joan.pfc.version1;

import java.io.IOException;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.Socket;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Color;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.TextView;

public class MainActivity extends Activity {
	
    public final static String EXTRA_MESSAGE = "com.joan.pfc.version1.MESSAGE";

	private Button button_connect;
	private EditText edittext_ip;
	private EditText edittext_port;
	private Spinner spinner_color;
	
	private int value 	= 0;
	private int[] colors = {Color.BLACK, Color.BLUE, Color.GREEN, Color.CYAN,
							Color.RED, Color.MAGENTA, Color.YELLOW, Color.WHITE};
	
	private OutputStream out; 
	private Socket client;
	private String ip 	= "192.168.1.40";
	private String port = "2000";

    @Override
    public void onCreate(Bundle savedInstanceState) {
    	Log.v("onCreate","onCreate");
    	
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
//        spinner_color = (Spinner) findViewById(R.id.spinner_color_led);
//        ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(this, 
//        		R.array.array_colors_led, android.R.layout.simple_spinner_item);
//        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
//        spinner_color.setAdapter(adapter);
//        spinner_color.setOnItemSelectedListener(new OnItemSelectedListener() {
//
//			@Override
//			public void onItemSelected(AdapterView<?> arg0, View arg1,
//					int arg2, long arg3) {
//				// TODO Auto-generated method stub
//				try {
//					writeSocket(a);
//				} catch (IOException e) {
//					e.printStackTrace();
//				}
//			}
//
//			@Override
//			public void onNothingSelected(AdapterView<?> arg0) {
//				// TODO Auto-generated method stub
//			}
//        	
//        });
        
        button_connect = (Button) findViewById(R.id.button_connect);
        button_connect.setOnClickListener(new View.OnClickListener() {	
        	
        	@Override
			public void onClick(View v) {      		
        		
				edittext_ip 	= (EditText) findViewById(R.id.editText_ip);
				edittext_port 	= (EditText) findViewById(R.id.editText_port);
				ip 				= edittext_ip.getText().toString();
				port			= edittext_port.getText().toString(); 
				
				new ConnectSocket(MainActivity.this).execute(ip, port);
			}
        });

    }

    @Override
    protected void onRestart(){
//    	Log.v("onRestart","onRestart");
//		new ConnectSocket(MainActivity.this).execute(ip, port);
    	super.onResume();
    }

    @Override
    protected void onStop(){
//    	Log.v("onStop","onStop");
//    	try {
//			closeSocket();
//		} catch (IOException e) {
//			e.printStackTrace();
//		}
    	super.onStop();
    }

}