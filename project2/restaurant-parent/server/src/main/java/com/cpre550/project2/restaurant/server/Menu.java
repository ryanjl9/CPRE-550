package com.cpre550.project2.restaurant.server;

import java.util.ArrayList;

public class Menu {
	class MenuItem {
		public String name;
		public Double price;
		public MenuItem(String name, Double price){
			this.name = name;
			this.price = price;
		}

		@Override
		public String toString(){
			return String.format("%s - $%.2f", name, price);
		}
	}

	public Menu(){
		items = new ArrayList<MenuItem>();
	}
	
	public void addItem(String name, Double price){
		items.add(new MenuItem(name, price));
	}

	@Override
	public String toString(){
		StringBuilder retval = new StringBuilder();
		retval.append("Menu Items:\n");
		for (MenuItem item : items) {
			retval.append(String.format("- %s\n", item.toString()));
		}
		return retval.toString();
	}

	public ArrayList<MenuItem> items;
}