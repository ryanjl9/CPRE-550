package com.cpre550.project2.test_proj;

/**
* com/cpre550/project2/test_proj/TestInterfaceHolder.java .
* Generated by the IDL-to-Java compiler (portable), version "3.2"
* from test_app.idl
* Saturday, March 4, 2023 2:01:24 PM CST
*/

public final class TestInterfaceHolder implements org.omg.CORBA.portable.Streamable
{
  public com.cpre550.project2.test_proj.TestInterface value = null;

  public TestInterfaceHolder ()
  {
  }

  public TestInterfaceHolder (com.cpre550.project2.test_proj.TestInterface initialValue)
  {
    value = initialValue;
  }

  public void _read (org.omg.CORBA.portable.InputStream i)
  {
    value = com.cpre550.project2.test_proj.TestInterfaceHelper.read (i);
  }

  public void _write (org.omg.CORBA.portable.OutputStream o)
  {
    com.cpre550.project2.test_proj.TestInterfaceHelper.write (o, value);
  }

  public org.omg.CORBA.TypeCode _type ()
  {
    return com.cpre550.project2.test_proj.TestInterfaceHelper.type ();
  }

}
