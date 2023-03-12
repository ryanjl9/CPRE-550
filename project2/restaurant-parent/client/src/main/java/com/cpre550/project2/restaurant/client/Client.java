package com.cpre550.project2.restaurant.client;

import com.cpre550.project2.restaurant.common.*;

import java.util.Scanner;

import org.omg.CORBA.ORB;
import org.omg.CosNaming.NamingContextExt;
import org.omg.CosNaming.NamingContextExtHelper;


/**
 * Hello world!
 *
 */
public class Client{
	enum ClientType {CUSTOMER, MANAGER}
	private static ClientType type;
	private static String[] serverArgs = new String[] {
		"-ORBInitialPort", "1050",
		"-ORBInitialHost", "localhost"
	};

	private static Customer customer = null;
	private static Manager manager = null;
	private static ManagerDaemon daemon = null;

    public static void main(String[] args){
		if(args.length > 0 && args[0].equals("--manager")) type = ClientType.MANAGER;
		else type = ClientType.CUSTOMER;
		setupServer(serverArgs);

		Scanner scanner = new Scanner(System.in);
		UserInterface ui = new UserInterface(customer, manager);
		if(type == ClientType.MANAGER) daemon = new ManagerDaemon(manager, 3.0);

		if(daemon != null && !daemon.isRunning()){
			System.out.println("Manager already registered.");
			return;
		}

		while(!ui.quit){
			System.out.print("> ");
			ui.parse_cmd(scanner.nextLine());
		}
		if(daemon != null) daemon.kill();
		scanner.close();
    }

	private static void setupServer(String[] params){
		try {
			ORB org = ORB.init(params, null);
			org.omg.CORBA.Object objRef = org.resolve_initial_references("NameService");
			NamingContextExt ncRef = NamingContextExtHelper.narrow(objRef);

			if(type == ClientType.CUSTOMER) customer = CustomerHelper.narrow(ncRef.resolve_str("Customer"));
			else if(type == ClientType.MANAGER) manager = ManagerHelper.narrow(ncRef.resolve_str("Manager"));
		} catch(Exception e) {}
	}
}
