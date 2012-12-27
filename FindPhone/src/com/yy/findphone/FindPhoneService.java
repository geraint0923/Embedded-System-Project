package com.yy.findphone;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.RemoteException;



public class FindPhoneService extends Service {

	private boolean isListening = false;
	
	private Thread listenThread = null;
	
	private String name = "newbie";
	
	private final String configPath = "/sdcard/findphone/name";
	
	private final IFindPhoneService.Stub binderStub = new IFindPhoneService.Stub() {

		@Override
		public void setName(String name) throws RemoteException {
			// TODO Auto-generated method stub
			
		}

		@Override
		public String getName() throws RemoteException {
			// TODO Auto-generated method stub
			return null;
		}

		@Override
		public void startFind() throws RemoteException {
			// TODO Auto-generated method stub
			isListening = true;
			listenThread = new Thread(new BroadcastListener());
			listenThread.start();
		}

		@Override
		public void stopFind() throws RemoteException {
			// TODO Auto-generated method stub
			isListening = false;
			if(listenThread != null) {
				listenThread.interrupt();
				listenThread = null;
			}
		}
		
	};
	
	class BroadcastListener implements Runnable {

		@Override
		public void run() {
			
			// TODO Auto-generated method stub
			try {
				while(isListening) {
					Thread.sleep(1000);
				}
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		
	}
	
	@Override
	public IBinder onBind(Intent intent) {
		// TODO Auto-generated method stub
		return binderStub;
	}
	
	public void onCreate() {
		File file = new File(configPath);
		if(file.exists()) {
			try {
				FileReader fr = new FileReader(file);
				BufferedReader br = new BufferedReader(fr);
				name = br.readLine();
				br.close();
			} catch (FileNotFoundException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
		}
		super.onCreate();
	}
	
	public void onDestroy() {
		File file = new File(configPath);
		if(file.exists()) {
			file.delete();
		}
		try {
			file.createNewFile();
			FileOutputStream fos = new FileOutputStream(file);
			fos.write(name.getBytes());
			fos.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		super.onDestroy();
	}
	
}
