package com.cpre550.project2.test_proj;


/**
* com/cpre550/project2/test_proj/TestInterfacePOA.java .
* Generated by the IDL-to-Java compiler (portable), version "3.2"
* from test_app.idl
* Saturday, March 4, 2023 2:01:24 PM CST
*/

public abstract class TestInterfacePOA extends org.omg.PortableServer.Servant
 implements com.cpre550.project2.test_proj.TestInterfaceOperations, org.omg.CORBA.portable.InvokeHandler
{

  // Constructors

  private static java.util.Hashtable _methods = new java.util.Hashtable ();
  static
  {
    _methods.put ("get_index", new java.lang.Integer (0));
    _methods.put ("set_index", new java.lang.Integer (1));
  }

  public org.omg.CORBA.portable.OutputStream _invoke (String $method,
                                org.omg.CORBA.portable.InputStream in,
                                org.omg.CORBA.portable.ResponseHandler $rh)
  {
    org.omg.CORBA.portable.OutputStream out = null;
    java.lang.Integer __method = (java.lang.Integer)_methods.get ($method);
    if (__method == null)
      throw new org.omg.CORBA.BAD_OPERATION (0, org.omg.CORBA.CompletionStatus.COMPLETED_MAYBE);

    switch (__method.intValue ())
    {
       case 0:  // com/cpre550/project2/test_proj/TestInterface/get_index
       {
         short index = in.read_ushort ();
         int $result = (int)0;
         $result = this.get_index (index);
         out = $rh.createReply();
         out.write_ulong ($result);
         break;
       }

       case 1:  // com/cpre550/project2/test_proj/TestInterface/set_index
       {
         short index = in.read_ushort ();
         int val = in.read_ulong ();
         this.set_index (index, val);
         out = $rh.createReply();
         break;
       }

       default:
         throw new org.omg.CORBA.BAD_OPERATION (0, org.omg.CORBA.CompletionStatus.COMPLETED_MAYBE);
    }

    return out;
  } // _invoke

  // Type-specific CORBA::Object operations
  private static String[] __ids = {
    "IDL:com/cpre550/project2/test_proj/TestInterface:1.0"};

  public String[] _all_interfaces (org.omg.PortableServer.POA poa, byte[] objectId)
  {
    return (String[])__ids.clone ();
  }

  public TestInterface _this() 
  {
    return TestInterfaceHelper.narrow(
    super._this_object());
  }

  public TestInterface _this(org.omg.CORBA.ORB orb) 
  {
    return TestInterfaceHelper.narrow(
    super._this_object(orb));
  }


} // class TestInterfacePOA
