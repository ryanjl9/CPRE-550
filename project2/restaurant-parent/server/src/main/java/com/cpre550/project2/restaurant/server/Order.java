package com.cpre550.project2.restaurant.server;

public class Order {
	public Order(String name){
		this.name = name;
		this.totalCost = 0;
		order = new StringBuilder();
		order.append(String.format("Name: %s:\n", name));
	}

	public void addItem(Menu.MenuItem item, int count) throws IllegalArgumentException{
		if(count < 0) throw new IllegalArgumentException();
		order.append(
			String.format(
				"- %s: %d * %.2f = $%.2f\n", 
				item.name, 
				count, 
				item.price, 
				count * item.price
			)
		);
		totalCost += count * item.price;
	}

	public String getName(){ return name; }

	@Override
	public String toString(){
		StringBuilder retval = new StringBuilder(order.toString());
		retval.append(String.format("- Total: $%.2f\n", totalCost));
		return retval.toString();
	}

	private String name;
	private StringBuilder order;
	private double totalCost;
}
