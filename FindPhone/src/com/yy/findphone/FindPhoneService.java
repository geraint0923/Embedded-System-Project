package com.yy.findphone;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.RemoteException;



public class FindPhoneService extends Service {

	private boolean isListening = false;
	
	private Thread listenThread = null;
	
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
		
		super.onCreate();
	}
	
	public void onDestroy() {
		
		super.onDestroy();
	}
	
}
