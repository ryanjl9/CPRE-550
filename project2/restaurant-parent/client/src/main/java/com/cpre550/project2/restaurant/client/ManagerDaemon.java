package com.cpre550.project2.restaurant.client;

import com.cpre550.project2.restaurant.common.Manager;

public class ManagerDaemon implements Runnable{
	public ManagerDaemon(Manager manager, double pingRate){
		this.thread = null;
		this.manager = manager;
		this.pingRate = pingRate;
		this.token = manager.connect();
		this.start();
	}

	public boolean isRunning(){
		return !this.token.equals("");
	}

	public void kill(){
		this.token = "";
	}

	private void start(){
		if(thread != null) return;
		thread = new Thread(this);
		thread.start();
	}

	@Override
	public void run() {
		lastPing = System.currentTimeMillis();
		while(!this.token.equals("")){
			if((System.currentTimeMillis() - lastPing) >= pingRate * 1000){
				try{  // In the event that the manager is cleaned up before this while loop exists
					manager.keepalive(token);
				}catch(NullPointerException e){ return; }
				lastPing = System.currentTimeMillis();
			}
		}
	}
	
	private Thread thread;
	private double pingRate;
	private volatile String token;
	private long lastPing;
	private Manager manager;
}
