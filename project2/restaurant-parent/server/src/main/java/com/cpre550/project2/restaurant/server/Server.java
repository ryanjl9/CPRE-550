package com.cpre550.project2.restaurant.server;

import java.util.ArrayList;

import org.omg.CORBA.ORB;
import org.omg.CosNaming.NameComponent;
import org.omg.CosNaming.NamingContextExt;
import org.omg.CosNaming.NamingContextExtHelper;
import org.omg.PortableServer.POA;
import org.omg.PortableServer.POAHelper;

import com.cpre550.project2.restaurant.common.*;

class CustomerImplementation extends CustomerPOA{
	public CustomerImplementation(ArrayList<Order> orders){
		super();
		menu = new Menu();
		menu.addItem("Fried Chicken", 5.00);
		menu.addItem("Cola", 1.00);

		this.orders = orders;
	}

	private boolean userAlreadyOrdered(String user){
		for(Order order: orders){
			if(order.getName().equals(user)) return true;
		}
		return false;
	}

	public ArrayList<Order> getOrders(){ return orders; }
	
	@Override
	public String view_menu(){
		return menu.toString();
	}
	
	@Override
	public String place_order(String customer_order){
		String[] order_parts = customer_order.split(" ");
		if(userAlreadyOrdered(order_parts[0])) return String.format("%s already ordered.", order_parts[0]);
		if(order_parts.length < menu.items.size() + 1) return "Invalid number of arguments.";

		Order order = new Order(order_parts[0]);

		for(int i = 0; i < menu.items.size(); i++){
			try{
				order.addItem(menu.items.get(i), Integer.parseInt(order_parts[1 + i]));
			}catch(NumberFormatException e){
				return String.format("Invalid count for %s. - %s", menu.items.get(i).name, order_parts[i+1]);
			}catch(IllegalArgumentException e){
				return String.format("Provided count %s is negative", order_parts[1 + i]);
			}
		}
		orders.add(order);
		return "The order is received.";
	}
	
	@Override
	public String check_order_status(String username){
		for(Order order: orders){
			if(order.getName().equals(username)) return order.toString();
		}
		return String.format("ERROR: %s has not placed an order yet.", username);
	}

	private Menu menu;
	private ArrayList<Order> orders;
}

class ManagerImplementation extends ManagerPOA{
	public ManagerImplementation(ArrayList<Order> orders){
		super();
		this.orders = orders;
		mt = new ManagerThread(5.0);
		mt.start();
	}
	
	@Override
	public String connect() {
		if(mt.managerRegistered()) return "";
		return mt.addManager();
	}
	
	@Override
	public void keepalive(String token) {
		if(!mt.managerRegistered()) return;
		mt.ping(token);
	}
	
	@Override
	public String view_current_orders() {
		if(orders.size() == 0) return "There are no orders.";

		StringBuilder retval = new StringBuilder();
		retval.append("******************* ORDERS *******************\n");
		for(Order order: orders) retval.append(order.toString() + "\n");
		retval.append("**********************************************");
		
		return retval.toString();
	}

	private static ManagerThread mt;
	private ArrayList<Order> orders;
}

/**
 * Hello world!
 *
 */
public class Server 
{
    public static void main( String[] args )
    {
		orders = new ArrayList<Order>();

		try{
			ORB orb = ORB.init(serverArgs, null);
			POA rootpoa = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
			rootpoa.the_POAManager().activate();
	
			CustomerImplementation customer = new CustomerImplementation(orders);
			ManagerImplementation manager = new ManagerImplementation(orders);
	
			org.omg.CORBA.Object cObj = rootpoa.servant_to_reference(customer);
			org.omg.CORBA.Object mObj = rootpoa.servant_to_reference(manager);

			Customer cRef = CustomerHelper.narrow(cObj);
			Manager mRef = ManagerHelper.narrow(mObj);

			org.omg.CORBA.Object objRef = orb.resolve_initial_references("NameService");
			NamingContextExt ncRef = NamingContextExtHelper.narrow(objRef);

			NameComponent[] cPath = ncRef.to_name("Customer");
			NameComponent[] mPath = ncRef.to_name("Manager");

			ncRef.rebind(cPath, cRef);
			ncRef.rebind(mPath, mRef);

			System.out.println("Starting Server");
			orb.run();
		} catch(Exception e) {}
    }

	private static ArrayList<Order> orders;
	private static String[] serverArgs = new String[] {
		"-ORBInitialPort", "1050",
		"-ORBInitialHost", "localhost"
	};
}
