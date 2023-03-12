package com.cpre550.project2.restaurant.server;

import java.util.Random;

public class ManagerThread  implements Runnable{

	public ManagerThread(double timeout){
		this.thread = null;
		this.timeout = timeout;
		this.userToken = "";
		this.lastPing = System.currentTimeMillis();
	}

	public void ping(String token){
		if(userToken.equals("") || !userToken.equals(token)) return;
		lastPing = System.currentTimeMillis();
	}

	public String addManager(){
		if(managerRegistered()) return "";

		Random r = new Random();
		StringBuilder token = new StringBuilder();
		for(int i = 0; i < 8; i++) token.append(r.nextInt(26) + 'A');
		lastPing = System.currentTimeMillis();
		userToken = token.toString();
		System.out.println("Manager connected.");
		
		return userToken;
	}

	public boolean managerRegistered(){
		return !userToken.equals("");
	}

	public void start(){
		if(thread != null) return;
		thread = new Thread(this);
		thread.start();
	}

	@Override
	public void run() {
		while(true){
			if(userToken.equals("")) continue;
			if((System.currentTimeMillis() - lastPing) > timeout * 1000){
				System.out.println("Manager has been disconnected.");
				userToken = "";
			}
		}
	}
	
	private Thread thread;
	private double timeout;
	private volatile String userToken;
	private volatile long lastPing;
}
