
////////////////////////////////////////////////////////////////////////////////
// WIT-M Source File OneToOne.java.
////////////////////////////////////////////////////////////////////////////////

package com.ibm.witm;

import static com.ibm.witm.Utility.*;

import java.io.PrintStream;

/*******************************************************************************
 * A OneToOne {@literal<E, V>} is a Property {@literal<V>} that's one-to-one on
 * non-null values. Specifically, it's a Property that maintains the following
 * invariant:<br>
 * Let OTO be a OneToOne {@literal<E, V>}.<br>
 * Let theValue be a non-null value of type V.<br>
 * Let theEmb be an Embodiment.<br>
 * Then there will always be at most one Entity, theEnt in theEmb such that
 * theEnt.get (OTO).equals (theValue).<br>
 * To maintain this one-to-one invariant, any time the value of a OneToOne for a
 * particular Entity is set to a value that's already held by a different Entity
 * in the same Embodiment, the value of the OneToOne for the other Entity is set
 * to null.<br><br>
 * The factory method for creating instances of this generic class is<br>
 * EntityCategory.newOneToOne (theName, theValueType)
 * 
 * @param <E> The type of Entity to which this OneToOne applies
 * @param <V> The type of value to be associated with Entities by this OneToOne
 *******************************************************************************
 */
public final class OneToOne <E extends Entity, V> extends Property <V>
{
////////////////////////////////////////////////////////////////////////////////
// Private Non-Static Fields
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * The index of this OneToOne.<br>
 * The # of OneToOnes in the Scheme created before this OneToOne.
 *******************************************************************************
 */
   private final int myOTOIndex;
   
////////////////////////////////////////////////////////////////////////////////
// Package Internal Constructor
////////////////////////////////////////////////////////////////////////////////
   
   OneToOne (
         String             theName,
         Class <V>          theValueType,
         EntityCategory <E> theEntCat)
      {
      super (theName, theValueType, theEntCat);
      
      myOTOIndex = theEntCat.getScheme ().getNOneToOnes ();
      }

////////////////////////////////////////////////////////////////////////////////
// Package Internal Non-Static Methods
////////////////////////////////////////////////////////////////////////////////
   
/*******************************************************************************
 * Prints this OneToOne.
 *******************************************************************************
 */    @Override
   void print (PrintStream theStream)
      {
      theStream.printf (
         "   OneToOne    %s:%n"
       + "   Value Type: %s%n%n",
         getName (),
         getValueType ().getSimpleName ());
      }

/*******************************************************************************
 * @return The index of this OneToOne.
 *******************************************************************************
 */
   int getOTOIndex ()
      {
      return myOTOIndex;
      }
   
/*******************************************************************************
 * {@inheritDoc}
 * <br>
 * Maintains the one-to-one invariant and revises the relevant inverse Map, as
 * needed.
 *******************************************************************************
 */    @Override
   void setValueFor (Entity theEnt, V theValue)
      {
      theEnt.getEmbodiment ().setValue (this, theEnt, theValue);
      }

////////////////////////////////////////////////////////////////////////////////
// Public Non-Static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Retrieves the unique Entity in the current Embodiment whose value for this
 * OneToOne is equal to a specified value.
 * 
 * @param theValue The specified value
 * @return         The unique Entity (cast as type E), if any; otherwise null.
 * 
 * @throws NullPointerException  If theValue is null
 * @throws IllegalStateException If the current Embodiment is null
 *******************************************************************************
 */
   public E getEntity (V theValue)
      {
      banNull   (theValue, "theValue");
      reqCurEmb ();

      return curEmb ().getEntity (this, theValue);
      }

////////////////////////////////////////////////////////////////////////////////
}
