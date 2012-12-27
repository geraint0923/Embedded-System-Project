package com.yy.findphone;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;

import android.R.integer;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.media.MediaPlayer;
import android.os.Binder;
import android.os.IBinder;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;
import android.os.RemoteException;



public class FindPhoneService extends Service {

	//private boolean isListening = false;
	
	private int packetCount = 0;
	
	private Thread listenThread = null;
	
	private WakeLock wakeLock;
	
	private String name = "newbie";
	
	private byte[] recvBuffer = new byte[1024];
	
	private final String configPath = "/sdcard/findphone/name";
	
	private final String musicPath = "/sdcard/rainbow.mp3";
	
	private MediaPlayer mediaPlayer = null;
	
	DatagramSocket socket = null;
	
	private final IFindPhoneService.Stub binderStub = new IFindPhoneService.Stub() {

		@Override
		public void setName(String n) throws RemoteException {
			// TODO Auto-generated method stub
			name = n;
		}

		@Override
		public String getName() throws RemoteException {
			// TODO Auto-generated method stub
			return name;
		}

		@Override
		public void startFind() throws RemoteException {
			// TODO Auto-generated method stub
			/*
			isListening = true;
			listenThread = new Thread(new BroadcastListener());
			listenThread.start();
			*/
		}

		@Override
		public void stopFind() throws RemoteException {
			// TODO Auto-generated method stub
			/*
			isListening = false;
			if(listenThread != null) {
				listenThread.interrupt();
				listenThread = null;
			}
			*/
		}
		
	};
	
	class BroadcastListener implements Runnable {

		@Override
		public void run() {
			
			// TODO Auto-generated method stub
			try {
				socket = new DatagramSocket(33333);
//				socket.setBroadcast(true);
					
				while(true) {
					DatagramPacket packet = new DatagramPacket(recvBuffer,recvBuffer.length);
					socket.receive(packet);
					String str = new String(recvBuffer, 0, packet.getLength());
					System.out.println(packetCount+ " "+packet.getLength()+" "+":::" + str);
					packetCount++;
					String[] spl = str.split(":");
					if(spl.length == 2) {
						if(spl[1].equals(name)) {
							if(mediaPlayer == null) {
								new Thread(new Runnable() {
									
									@Override
									public void run() {
										// TODO Auto-generated method stub
										mediaPlayer = new MediaPlayer();
										try {
											mediaPlayer.setDataSource(musicPath);
											mediaPlayer.prepare();
											mediaPlayer.start();
											Thread.sleep(10000);
											System.out.print("play");
											mediaPlayer.stop();
											mediaPlayer.release();
											mediaPlayer = null;
										} catch (IllegalArgumentException e) {
											// TODO Auto-generated catch block
											e.printStackTrace();
										} catch (SecurityException e) {
											// TODO Auto-generated catch block
											e.printStackTrace();
										} catch (IllegalStateException e) {
											// TODO Auto-generated catch block
											e.printStackTrace();
										} catch (IOException e) {
											// TODO Auto-generated catch block
											e.printStackTrace();
										} catch (InterruptedException e) {
											// TODO Auto-generated catch block
											e.printStackTrace();
										}
									}
								}).start();

							}
						}
					}
					//handler.sendEmptyMessage(0);
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
		PowerManager pm = (PowerManager)getSystemService(Context.POWER_SERVICE);  
		wakeLock = pm.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, "MyWakeLock");  
		wakeLock.acquire();
		
		//isListening = true;
		listenThread = new Thread(new BroadcastListener());
		listenThread.start();
		
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
		if(listenThread != null) {
			listenThread.interrupt();
			listenThread = null;
		}
		wakeLock.release();  
		super.onDestroy();
	}
	
}
