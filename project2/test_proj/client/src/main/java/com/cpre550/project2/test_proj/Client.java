package com.cpre550.project2.test_proj;

import org.omg.CORBA.ORB;
import org.omg.CosNaming.NamingContextExt;
import org.omg.CosNaming.NamingContextExtHelper;

/**
 * Hello world!
 *
 */
public class Client 
{
    public static void main(String[] args)
    {
		TestInterface svr = Client.setupServer(args);

		if(svr == null){
			System.out.println("TestInterface server not running.");
			System.exit(1);
		}

		for(int i = 0; i < TestInterface.SIZE; i++){
			svr.set_index((short)i, i + 0x0FE0);
		}

		for(int i = 0; i < TestInterface.SIZE; i++){
			assert svr.get_index((short)i) == 0x0FE0 + i;
		}

		for(int i = 0; i < TestInterface.SIZE + 5; i++){
			svr.set_index((short)i, i+1);
		}

		for(int i = 0; i < TestInterface.SIZE + 5; i++){
			System.out.println(String.format("Index: %d - %d", i, svr.get_index((short)i)));
		}

    }

	private static TestInterface setupServer(String[] params){
		try {
			ORB org = ORB.init(params, null);

			org.omg.CORBA.Object objRef = org.resolve_initial_references("NameService");

			NamingContextExt ncRef = NamingContextExtHelper.narrow(objRef);

			return TestInterfaceHelper.narrow(ncRef.resolve_str("TestInterface"));

		} catch(Exception e) {}
		return null;
	}
}
