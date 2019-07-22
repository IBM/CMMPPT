
////////////////////////////////////////////////////////////////////////////////
// WIT-M Source File EntityCategory.java.
////////////////////////////////////////////////////////////////////////////////

package com.ibm.witm;

import static com.ibm.witm.Utility.*;

import java.io.PrintStream;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/*******************************************************************************
 * An EntityCategory {@literal<E>} represents a conceptual grouping of similar
 * Entities of the I class. Each Entity belongs to a particular EntityCategory
 * and all Entities that belong to a given EntityCategory are treated uniformly
 * by WIT-M. Each EntityCategory belongs to a specific Scheme and is
 * considered to be a defining aspect of the Scheme.
 * 
 * @param <E> The type of Entity that will belong to this
 *            EntityCategory {@literal<E>}: either Item or Tuple
 *******************************************************************************
 */
public abstract class EntityCategory <E extends Entity>
{
////////////////////////////////////////////////////////////////////////////////
// Private Non-Static Fields
////////////////////////////////////////////////////////////////////////////////
   
/*******************************************************************************
 * The Scheme of the WIT model to which this EntityCategory belongs
 *******************************************************************************
 */
   private final Scheme myScheme;
   
/*******************************************************************************
 * The name of this EntityCategory
 *******************************************************************************
 */
   private final String myName;
   
/*******************************************************************************
 * A list of all Properties that belong to this EntityCategory.
 *******************************************************************************
 */
   private final ArrayList <Property <?>> myProperties;

/*******************************************************************************
 * An unmodifiable view of myProperties.
 *******************************************************************************
 */
   private final List <Property <?>> unmodProperties;

/*******************************************************************************
 * A unique index for this EntityCategory: the # of Categories in the Scheme
 * created before this one.
 *******************************************************************************
 */
   private final int myIndex;

////////////////////////////////////////////////////////////////////////////////
// Private Non-Static Methods
////////////////////////////////////////////////////////////////////////////////
   
/*******************************************************************************
 * Error checking for newProperty and new OneToOne.
 *******************************************************************************
 */
   private void checkForNewProp (
         String    theName,
         Class <?> theValueType,
         String    signature)
      {
      banNull             (theName,      "theName");
      banNull             (theValueType, "theValueType");
      myScheme.banDupName (theName,      signature);
      banFrozenScheme                   (signature);
      }

////////////////////////////////////////////////////////////////////////////////
// Package Internal Constructor
////////////////////////////////////////////////////////////////////////////////
   
   EntityCategory (Scheme theScheme, String theName)
      {
      assert (theScheme != null);
      assert (theName   != null);
      
      myScheme        = theScheme;
      myName          = theName;
      
      myProperties    = new ArrayList <Property    <?>> ();
      
      unmodProperties = Collections.unmodifiableList (myProperties);
      
      myIndex         = theScheme.getEntityCategories ().size ();
      
      theScheme.store     (this);
      theScheme.storeName (theName);
      }

////////////////////////////////////////////////////////////////////////////////
// Package Internal Non-Static Methods
////////////////////////////////////////////////////////////////////////////////
   
/*******************************************************************************
 * Performs actions to be taken by this EntityCategory when the Scheme is being
 * frozen.
 *******************************************************************************
 */
   void freeze ()
      {
      }
   
/*******************************************************************************
 * Verifies that myScheme is not frozen.
 *******************************************************************************
 */
   void banFrozenScheme (String signature)
      {
      if (myScheme.isFrozen ())
         throw
            new IllegalStateException (
               "\n\n"
             + "   Call to " + signature + ",\n"
             + "   on an EntityCategory whose Scheme is frozen.\n"
             + "\n"
             + "      EntityCategory: " + myName              + "\n"
             + "      Scheme:         " + myScheme.getName () + "\n");
      }

/*******************************************************************************
 * Verifies that the Scheme of this EntityCategory matches the Scheme of the
 * current Embodiment.
 *******************************************************************************
 */
   void reqSchemeMatchesCurEmb ()
      {
      if (myScheme != curEmb ().getScheme ())
         throw
            new IllegalStateException (
               "\n\n"
             + "   A method was called on an EntityCategory that belongs to a\n"
             + "   Scheme that does not match the Scheme of the current\n"
             + "   Embodiment.\n"
             + "\n"
             + "      EntityCategory:     " +               myName    + "\n"
             + "         Scheme:          " + myScheme.    getName () + "\n"
             + "      Current Embodiment: " + curEmb ().   getName () + "\n"
             + "         Scheme:          " + curEmb ().
                                              getScheme ().getName () + "\n");
      }

/*******************************************************************************
 * Prints this EntityCategory to theStream.
 *******************************************************************************
 */
   final void print (PrintStream theStream)
      {
      printConcrete (theStream);
      
      theStream.printf ("%n");
      
      for (Property <?> theProp: myProperties)
         theProp.print (theStream);
      }

/*******************************************************************************
 * Prints the subclass-specific aspects of this EntityCategory to theStream.
 *******************************************************************************
 */
   abstract void printConcrete (PrintStream theStream);

/*******************************************************************************
 * @return An unmodifiable view of a List of the Properties that belong to this
 *         EntityCategory.
 *******************************************************************************
 */
   final List <Property <?>> getProperties ()
      {
      return unmodProperties;
      }

/*******************************************************************************
 * @return The unique index for this EntityCategory.
********************************************************************************
 */
   final int getIndex ()
      {
      return myIndex;
      }

/*******************************************************************************
 * @return true, iff this EntityCategory is the Period Category.
********************************************************************************
 */
   final boolean isPeriodCategory ()
      {
      return (this == myScheme.getPeriodCategory ());
      }

/*******************************************************************************
 * Creates a new EntityMgr for this EntityCategory in theEmb.
 * 
 * @return The new EntityMgr
********************************************************************************
 */
   abstract EntityMgr <?> newEntityMgr (Embodiment theEmb);

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
 * @return The name of this EntityCategory
 *******************************************************************************
 */
   public final String getName ()
      {
      return myName;
      }

/*******************************************************************************
 * @return The Scheme to which this EntityCategory belongs
 *******************************************************************************
 */
   public final Scheme getScheme ()
      {
      return myScheme;
      }

/*******************************************************************************
 * Factory method:<br>
 * Constructs a new Property {@literal<V>} that will belong to this
 * EntityCategory.
 * 
 * @param <V>          The value type of the Property (compile time)
 * @param theName      The name of the Property
 * @param theValueType The value type of the Property (run time)
 * @return             The new Property {@literal<V>}
 * 
 * @throws NullPointerException     If any argument is null
 * @throws IllegalArgumentException If theName duplicates the name of an 
 *                                  existing schematic object in the Scheme of
 *                                  this EntityCategory
 * @throws IllegalStateException    If the Scheme of this EntityCategory is
 *                                  frozen
 *******************************************************************************
 */
   public <V> Property <V> newProperty (String theName, Class <V> theValueType)
      {
      Property <V> theProp;
      
      checkForNewProp (
         theName,
         theValueType,
         "EntityCategory.newProperty (theName, theValueType)");
      
      theProp = new Property <V> (theName, theValueType, this);

      myProperties.add (theProp);
      
      myScheme.storeName (theName);

      return theProp;
      }

/*******************************************************************************
 * Factory method:<br>
 * Constructs a new OneToOne {@literal<E, V>} that will belong to this
 * EntityCategory.
 * 
 * @param <V>          The value type of the OneToOne (compile time)
 * @param theName      The name of the OneToOne
 * @param theValueType The value type of the OneToOne (run time)
 * @return             The new OneToOne {@literal<V>}
 * 
 * @throws NullPointerException     If any argument is null
 * @throws IllegalArgumentException If theName duplicates the name of an 
 *                                  existing schematic object in the Scheme of
 *                                  this EntityCategory
 * @throws IllegalStateException    If the Scheme of this EntityCategory is
 *                                  frozen
 *******************************************************************************
 */
   public <V> OneToOne <E, V> newOneToOne (
         String    theName,
         Class <V> theValueType)
      {
      OneToOne <E, V> theOTO;
      
      checkForNewProp (
         theName,
         theValueType,
         "EntityCategory.newOneToOne (theName, theValueType)");
      
      theOTO = new OneToOne <E, V> (theName, theValueType, this);

      myProperties.add (theOTO);
      
      myScheme.storeName (theName);

      myScheme.recordNewOneToOne ();

      return theOTO;
      }

/*******************************************************************************
 * Entity list retrieval method.
 * 
 * @return  An unmodifiable view of an ArrayList of all of the I instances of
 *          this EntityCategory in in current Embodiment, listed in the order
 *          in which they were created.
 * 
 * @throws IllegalStateException    If the current Embodiment is null
 * @throws IllegalArgumentException If the Scheme of this EntityCategory does
 *                                  not match the Scheme of the current
 *                                  Embodiment
 *******************************************************************************
 */
   public List <E> getAll ()
      {
      reqCurEmb              ();
      reqSchemeMatchesCurEmb ();
      
      return curEmb ().getEntityMgr (this).getAll ();
      }

////////////////////////////////////////////////////////////////////////////////
}
