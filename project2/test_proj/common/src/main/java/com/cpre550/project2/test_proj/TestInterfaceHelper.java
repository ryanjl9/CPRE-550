package com.cpre550.project2.test_proj;


/**
* com/cpre550/project2/test_proj/TestInterfaceHelper.java .
* Generated by the IDL-to-Java compiler (portable), version "3.2"
* from test_app.idl
* Saturday, March 4, 2023 2:01:24 PM CST
*/

abstract public class TestInterfaceHelper
{
  private static String  _id = "IDL:com/cpre550/project2/test_proj/TestInterface:1.0";

  public static void insert (org.omg.CORBA.Any a, com.cpre550.project2.test_proj.TestInterface that)
  {
    org.omg.CORBA.portable.OutputStream out = a.create_output_stream ();
    a.type (type ());
    write (out, that);
    a.read_value (out.create_input_stream (), type ());
  }

  public static com.cpre550.project2.test_proj.TestInterface extract (org.omg.CORBA.Any a)
  {
    return read (a.create_input_stream ());
  }

  private static org.omg.CORBA.TypeCode __typeCode = null;
  synchronized public static org.omg.CORBA.TypeCode type ()
  {
    if (__typeCode == null)
    {
      __typeCode = org.omg.CORBA.ORB.init ().create_interface_tc (com.cpre550.project2.test_proj.TestInterfaceHelper.id (), "TestInterface");
    }
    return __typeCode;
  }

  public static String id ()
  {
    return _id;
  }

  public static com.cpre550.project2.test_proj.TestInterface read (org.omg.CORBA.portable.InputStream istream)
  {
    return narrow (istream.read_Object (_TestInterfaceStub.class));
  }

  public static void write (org.omg.CORBA.portable.OutputStream ostream, com.cpre550.project2.test_proj.TestInterface value)
  {
    ostream.write_Object ((org.omg.CORBA.Object) value);
  }

  public static com.cpre550.project2.test_proj.TestInterface narrow (org.omg.CORBA.Object obj)
  {
    if (obj == null)
      return null;
    else if (obj instanceof com.cpre550.project2.test_proj.TestInterface)
      return (com.cpre550.project2.test_proj.TestInterface)obj;
    else if (!obj._is_a (id ()))
      throw new org.omg.CORBA.BAD_PARAM ();
    else
    {
      org.omg.CORBA.portable.Delegate delegate = ((org.omg.CORBA.portable.ObjectImpl)obj)._get_delegate ();
      com.cpre550.project2.test_proj._TestInterfaceStub stub = new com.cpre550.project2.test_proj._TestInterfaceStub ();
      stub._set_delegate(delegate);
      return stub;
    }
  }

  public static com.cpre550.project2.test_proj.TestInterface unchecked_narrow (org.omg.CORBA.Object obj)
  {
    if (obj == null)
      return null;
    else if (obj instanceof com.cpre550.project2.test_proj.TestInterface)
      return (com.cpre550.project2.test_proj.TestInterface)obj;
    else
    {
      org.omg.CORBA.portable.Delegate delegate = ((org.omg.CORBA.portable.ObjectImpl)obj)._get_delegate ();
      com.cpre550.project2.test_proj._TestInterfaceStub stub = new com.cpre550.project2.test_proj._TestInterfaceStub ();
      stub._set_delegate(delegate);
      return stub;
    }
  }

}
