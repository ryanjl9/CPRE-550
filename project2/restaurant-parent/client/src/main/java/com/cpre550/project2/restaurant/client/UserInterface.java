package com.cpre550.project2.restaurant.client;

import com.cpre550.project2.restaurant.common.*;

public class UserInterface {
	public UserInterface(Customer customer, Manager manager){
		this.customer = customer;
		this.manager = manager;
		this.quit = false;
	}

	public void parse_cmd(String cmd){
		String[] cmd_parts = cmd.split(" ", 2);
		
		switch(cmd_parts[0]){
			case "help":
				viewHelp();
				break;
			case "view_menu":
				view_menu();
				break;
			case "place_order":
				if(cmd_parts.length < 2) return;
				place_order(cmd_parts[1]);
				break;
			case "order_status":
				if(cmd_parts.length < 2) return;
				check_order_status(cmd_parts[1]);
				break;
			case "view_orders":
				view_orders();
				break;
			case "quit":
				quit = true;
				break;
			default:
				System.out.println("Invalid command.");
		}
	}

	private void viewHelp(){
		StringBuilder helpMenu = new StringBuilder();
		helpMenu.append("Commands: \nhelp: prints this menu\n");
		if(customer != null){
			helpMenu.append("view_menu: Prints the menu for the restaurant\n");
			helpMenu.append("place_order: Places an order. Format: NAME COUNT_OF_ITEM1, COUNT_OF_ITEM2\n");
			helpMenu.append("\tEXAMPLE: place_order Ryan 4 6\n");
			helpMenu.append("order_status: Shows the order status for the provided user.\n");
			helpMenu.append("\tEXAMPLE: order_status Ryan\n");
		}
		
		if(manager != null){
			helpMenu.append("view_orders: Returns the current orders in the system.\n");
		}
		
		System.out.println(helpMenu.toString());
	}

	private void view_menu(){
		if(customer == null) return;
		System.out.println(customer.view_menu());
	}

	private void place_order(String order){
		if(customer == null) return;
		System.out.println(customer.place_order(order));
	}

	private void check_order_status(String name){
		if(customer == null) return;
		System.out.println(customer.check_order_status(name));
	}

	private void view_orders(){
		if(manager == null){
			return;
		}
		System.out.println(manager.view_current_orders());
	}

	private Customer customer;
	private Manager manager;
	public boolean quit;
}
