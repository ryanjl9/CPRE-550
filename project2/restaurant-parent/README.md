# Restaurant Application

## How to build

This project uses maven as a build system to make the two jar files that make up this project. This uses Java 1.8 for it's runtime. To build the project, in the restaurant-parent folder, run the following command: `./mvnw clean package`. This will create two jar files, `server.jar` and `client.jar` in the `server/target` and `client/target` folders respectively.

## How to run the server

The server needs to be ran before the client. However, before the server can be ran, the ORB daemon needs to running on the host machine. Run the following commands: `orbd -ORBInitialPort 1050&`. Once this service has started, the server can be executed by running the following command from within the restaurant-parent directory: `java -jar server/target/server-0.1.0-jar-with-dependencies.jar`. Depending on the version of the jar, this name may be different.

## How to run the client

The client binary supports two modes of operation: Customer and Manager. By default, the client appliation will launch in Customer mode, but it can be switched to Manager by providing the optional `--manager` flag as a commandline argument. 

### In Customer mode

The client can be ran in Customer mode by running the following command: `java -jar client/target/client-0.1.0-jar-with-dependencies.jar`. It should be noted that if the server is not running, the client will not work.

### In Manager mode

The client can be ran in Manager mode by running the following command: `java -jar client/target/client-0.1.0-jar-with-dependencies.jar --manager`. It should be noted that if the server is not running, the client will not work.

## How to use the Client App

While the server app may not have any sort interactable component, the client app does. The available commands varies between Customer mode and Manager mode, but both support the help command. The list of available commands are provided below:

* Both:
	* help: Prints out the help menu.
	* quit: Closes the application.

* Customer Mode:
	* view_menu: Prints out the menu items and their prices.
	* place_order: Allows the user to place an order. Each person can place one order and each order must specify an amount for each items. Example 1: `place_order Ryan 1 1` selects 1 fried chicken and 1 cola. Example 2: `place_order Ryan 0 1` selects 0 fried chicken and 1 cola.
	* order_status: Allows the user to view the status or an order given the user name. Example: `order_status Ryan` will return the order information for `Ryan` **IF** an order has been placed for Ryan.

* Manager Mode:
	* view_orders: Returns a list of all orders which have been placed.
