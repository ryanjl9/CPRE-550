package com.cpre550.project2.restaurant.common;

/**
* com/cpre550/project2/restaurant/common/ManagerHolder.java .
* Generated by the IDL-to-Java compiler (portable), version "3.2"
* from restaurant.idl
* Saturday, March 11, 2023 1:00:57 PM CST
*/

public final class ManagerHolder implements org.omg.CORBA.portable.Streamable
{
  public com.cpre550.project2.restaurant.common.Manager value = null;

  public ManagerHolder ()
  {
  }

  public ManagerHolder (com.cpre550.project2.restaurant.common.Manager initialValue)
  {
    value = initialValue;
  }

  public void _read (org.omg.CORBA.portable.InputStream i)
  {
    value = com.cpre550.project2.restaurant.common.ManagerHelper.read (i);
  }

  public void _write (org.omg.CORBA.portable.OutputStream o)
  {
    com.cpre550.project2.restaurant.common.ManagerHelper.write (o, value);
  }

  public org.omg.CORBA.TypeCode _type ()
  {
    return com.cpre550.project2.restaurant.common.ManagerHelper.type ();
  }

}