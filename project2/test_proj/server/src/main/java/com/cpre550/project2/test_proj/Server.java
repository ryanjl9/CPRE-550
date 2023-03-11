package com.cpre550.project2.test_proj;

import org.omg.CORBA.ORB;
import org.omg.CosNaming.NameComponent;
import org.omg.CosNaming.NamingContextExt;
import org.omg.CosNaming.NamingContextExtHelper;
import org.omg.PortableServer.POA;
import org.omg.PortableServer.POAHelper;

class ServerImplementation extends TestInterfacePOA{
	private int[] registers;

	public ServerImplementation(){
		super();
		registers = new int[TestInterface.SIZE];
	}

	@Override
	public int get_index(short index) {
		if(index < TestInterface.SIZE) return registers[index];
		return -1;
	}

	@Override
	public void set_index(short index, int val) {
		if(index < TestInterface.SIZE) registers[index] = val;
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
		try{
			ORB orb = ORB.init(args, null);
			POA rootpoa = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
			rootpoa.the_POAManager().activate();
	
			ServerImplementation deligate = new ServerImplementation();
	
			org.omg.CORBA.Object ref = rootpoa.servant_to_reference(deligate);
			TestInterface tref = TestInterfaceHelper.narrow(ref);

			org.omg.CORBA.Object objRef = orb.resolve_initial_references("NameService");
			NamingContextExt ncRef = NamingContextExtHelper.narrow(objRef);

			NameComponent[] path = ncRef.to_name("TestInterface");
			ncRef.rebind(path, tref);

			System.out.println("Starting Server");
			orb.run();
		} catch(Exception e) {}
    }
}
