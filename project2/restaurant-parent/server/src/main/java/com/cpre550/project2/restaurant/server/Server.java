package com.cpre550.project2.restaurant.server;

import java.util.ArrayList;
import com.cpre550.project2.restaurant.common.*;

class Menu {
	class MenuItem {
		private String name;
		private Double price;
		public MenuItem(String name, Double price){
			this.name = name;
			this.price = price;
		}

		@Override
		public String toString(){
			return String.format("%s - $%0.2f", name, price);
		}
	}

	private ArrayList<MenuItem> items;

	public Menu(){
		items = new ArrayList<MenuItem>();
	}

	public void addItem(String name, Double price){
		items.add(new MenuItem(name, price));
	}
}

class CustomerImplementation extends CustomerPOA{
	@Override
	public String view_menu(){
		return "";
	}

	@Override
	public String place_order(String order){
		return "";
	}

	@Override
	public String check_order_status(String username){
		return "";
	}
}

/**
 * Hello world!
 *
 */
public class Server 
{
    public static void main( String[] args )
    {
        System.out.println( "Hello World!" );
    }
}
