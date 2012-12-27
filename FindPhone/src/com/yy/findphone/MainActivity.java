package com.yy.findphone;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;

import android.net.wifi.WifiManager;
import android.net.wifi.WifiManager.MulticastLock;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;
import android.os.RemoteException;
import android.R.integer;
import android.app.Activity;
import android.app.Service;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.view.Menu;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

public class MainActivity extends Activity {

	private EditText editText;
	
	private Button setNameButton = null;
	
	private int count = 0;
	
//	private WakeLock wakeLock;
	
	DatagramSocket socket = null;
	
	private byte[] recvBuffer = new byte[1024];
	
	private Handler handler = new Handler() {
		public void handleMessage(Message msg) {
            // TODO Auto-generated method stub
			System.out.println("receive broadcast");
			count++;
			editText.setText(count+new String(recvBuffer));
        }
	};
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		/*
		PowerManager pm = (PowerManager)getSystemService(Context.POWER_SERVICE);  
		wakeLock = pm.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, "MyWakeLock");  
		*/
		
		/*
		WifiManager manager = (WifiManager) getSystemService(Context.WIFI_SERVICE);
		MulticastLock lock = manager.createMulticastLock("lock name");
		lock.setReferenceCounted(true);
		lock.acquire();
		*/
		
		editText = (EditText)this.findViewById(R.id.edit_text);
		setNameButton = (Button)this.findViewById(R.id.set_name);
		
		setNameButton.setOnClickListener(new Button.OnClickListener() {

			@Override
			public void onClick(View arg0) {
				// TODO Auto-generated method stub
				String nString = editText.getText().toString();
				try {
					findService.setName(nString);
				} catch (RemoteException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
			
		});
		
		//new Thread(new TestRun()).start();
		Intent intent = new Intent(MainActivity.this, FindPhoneService.class); 
		startService(intent); 
        
        bindService(intent, serviceConnection, Service.BIND_AUTO_CREATE);  
	}
	
	class TestRun implements Runnable {

		@Override
		public void run() {
			// TODO Auto-generated method stub
			try {
				socket = new DatagramSocket(33333);
				socket.setBroadcast(true);	
				
				while(true) {
					DatagramPacket packet = new DatagramPacket(recvBuffer,recvBuffer.length);
					socket.receive(packet);
					handler.sendEmptyMessage(0);
				}
			} catch (SocketException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
		}
		
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.activity_main, menu);
		return true;
	}
	
	
	
	public void onDesotroy() {
		/*
		if(socket != null) {
			socket.close();
		}
		wakeLock.release();
		*/ 
        
        this.unbindService(serviceConnection);  
		super.onDestroy();
	}
	
	private IFindPhoneService findService = null;
	
	private ServiceConnection serviceConnection = new ServiceConnection() { 


		@Override
		public void onServiceConnected(ComponentName arg0, IBinder arg1) {
			// TODO Auto-generated method stub
			findService = (IFindPhoneService) arg1; 
			System.out.println("OKK****************");
			try {
				editText.setText(findService.getName());
			} catch (RemoteException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}

		@Override
		public void onServiceDisconnected(ComponentName arg0) {
			// TODO Auto-generated method stub
			findService = null;
		} 

	}; 

}
