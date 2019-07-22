
////////////////////////////////////////////////////////////////////////////////
// WIT-M Source File Property.java.
////////////////////////////////////////////////////////////////////////////////

package com.ibm.witm;

import java.io.PrintStream;

/*******************************************************************************
 * A Property {@literal<V>} is associated with an EntityCategory <?> and
 * specifies that a value of type V may be associated with each Entity of the
 * EntityCategory.
 * <br><br>
 * The factory method for creating instances of this generic class is
 * EntityCategory.newProperty (theName, theValueType)
 * 
 * @param <V> The type of value to be associated with the Entities
 *******************************************************************************
 */
public class Property <V>
{
////////////////////////////////////////////////////////////////////////////////
// Private Non-Static Fields
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * The name of this Property.
 *******************************************************************************
 */
   private final String myName;
   
/*******************************************************************************
 * The value type of this Property.
 *******************************************************************************
 */
   private final Class <V> myValueType;
   
/*******************************************************************************
 * The EntityCategory <?> to which this Property applies.
 *******************************************************************************
 */
   private final EntityCategory <?> myEntityCategory;
   
/*******************************************************************************
 * The index of this Property.<br>
 * The # of Properties for myEntityCategory created before this Property.
 *******************************************************************************
 */
   private final int myIndex;
   
////////////////////////////////////////////////////////////////////////////////
// Package Internal Constructor
////////////////////////////////////////////////////////////////////////////////
   
   Property (
         String             theName,
         Class <V>          theValueType,
         EntityCategory <?> theEntCat)
      {
      myName           = theName;
      myValueType      = theValueType;
      myEntityCategory = theEntCat;
      myIndex          = theEntCat.getProperties ().size ();
      }

////////////////////////////////////////////////////////////////////////////////
// Package Internal Non-Static Methods
////////////////////////////////////////////////////////////////////////////////
   
/*******************************************************************************
 * Prints this Property.
 *******************************************************************************
 */
   void print (PrintStream theStream)
      {
      theStream.printf (
         "   Property    %s:%n"
       + "   Value Type: %s%n%n",
         myName,
         myValueType.getSimpleName ());
      }

/*******************************************************************************
 * @return The index of the Property
 *******************************************************************************
 */
   final int getIndex ()
      {
      return myIndex;
      }

/*******************************************************************************
 * Sets the value of a the Property for theEnt to theValue.
 *******************************************************************************
 */
   void setValueFor (Entity theEnt, V theValue)
      {
      theEnt.setValue (this, theValue);
      }

////////////////////////////////////////////////////////////////////////////////
// Public Non-Static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * {@inheritDoc}
 *******************************************************************************
 */                   @Override
   public final String toString ()
      {
      return myName;
      }
   
/*******************************************************************************
 * @return The EntityCategory <?> to which this Property applies
 *******************************************************************************
 */
   public final EntityCategory <?> getEntityCategory ()
      {
      return myEntityCategory;
      }

/*******************************************************************************
 * @return The value type of this Property
 *******************************************************************************
 */
   public final Class <V> getValueType ()
      {
      return myValueType;
      }

/*******************************************************************************
 * @return The name of this Property
 *******************************************************************************
 */
   public final String getName ()
      {
      return myName;
      }

////////////////////////////////////////////////////////////////////////////////
}
