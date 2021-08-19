
////////////////////////////////////////////////////////////////////////////////
// WIT-M Source File Scheme.java.
////////////////////////////////////////////////////////////////////////////////

package com.ibm.witm;

import static com.ibm.witm.Utility.*;

import java.io.PrintStream;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashSet;
import java.util.List;

/*******************************************************************************
 * A Scheme is the central object representing the static structure or "scheme"
 * of a WIT model, i.e., the data-independent aspects of the model.<br><br>
 * 
 * The factory method for creating instances of this class is
 * Scheme.newScheme (theName).
 *******************************************************************************
 */
public final class Scheme
{
////////////////////////////////////////////////////////////////////////////////
// Private Static Fields
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * current.get () returns the "current Scheme": This is a designated
 * Scheme that will be used by various WIT-M methods in the current Thread
 * if any; otherwise null.
 *******************************************************************************
 */
   private static ThreadLocal <Scheme> current;
   
////////////////////////////////////////////////////////////////////////////////
// Static Initialization Block
////////////////////////////////////////////////////////////////////////////////
   
   static
      {
      current = new ThreadLocal <Scheme> ();
      }

////////////////////////////////////////////////////////////////////////////////
// Private Non-Static Fields
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * The name of the model.
 *******************************************************************************
 */
   private final String myName;
   
/*******************************************************************************
 * True, iff this Scheme is "frozen".
 *******************************************************************************
 */
   private boolean frozen = false;

/*******************************************************************************
 * The set of the names of all of schematic objects that belong to this Scheme.
 *******************************************************************************
 */
   private final HashSet <String> allNames;

/*******************************************************************************
 * A list of all Categories that belong to this Scheme.
 *******************************************************************************
 */
   private final ArrayList <EntityCategory <?>> myEntityCategories;

/*******************************************************************************
 * The Global Category for this Scheme.
 *******************************************************************************
 */
   private final ItemCategory myGlobalCategory;

/*******************************************************************************
 * The Period Category for this Scheme.
 *******************************************************************************
 */
   private final ItemCategory myPeriodCategory;

/*******************************************************************************
 * The # of OneToOnes in this Scheme.
 *******************************************************************************
 */
   private int nOneToOnes;

////////////////////////////////////////////////////////////////////////////////
// Private Constructor
////////////////////////////////////////////////////////////////////////////////

   private Scheme (
         String theSchemeName,
         String theGlobalCatName,
         String thePeriodCatName)
      {
      myName             = theSchemeName;
      
      allNames           = new HashSet <String> ();
      
      myEntityCategories = new ArrayList <EntityCategory <?>> ();
      
      myGlobalCategory   = new ItemCategory (this, theGlobalCatName);

      myPeriodCategory   = new ItemCategory (this, thePeriodCatName);

      nOneToOnes         = 0;

      current.set (this);
      
      storeName (theSchemeName);
      }

////////////////////////////////////////////////////////////////////////////////
// Private Static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Verifies that the two names are distinct.<br>
 * To be called by newScheme (*, *, *).
 *******************************************************************************
 */
   private static void banSameName (
         String theName1,
         String theName2,
         String argName1,
         String argName2)
      {
      if (theName2.equals (theName1))
         throw
            new IllegalArgumentException (
               "\n\n"
             + "   Call to Scheme newScheme (\n"
             + "      theSchemeName, theGlobalCatName, thePeriodCatName),\n"
             + "   where " + argName1 + " and " + argName2 + " are\n"
             + "   identical Strings.\n"
             + "\n"
             + "      Duplicated String: " + theName1 + "\n");
      }

/*******************************************************************************
 * Verifies that the current Scheme is not null.
 *******************************************************************************
 */
   private static void reqCurScheme ()
      {
      if (current.get () == null)
         throw
            new IllegalStateException (
               "\n\n"
             + "   Call to a method that requires a current Scheme when the\n"
             + "   current Scheme is null.\n");
      }

/*******************************************************************************
 * Verifies that newTupleCategory (...) was called with at least one
 * EntityCategory.<br>
 * To be called by newTupleCategory.
 *******************************************************************************
 */
   private static void reqAtLeast1Cat (
         String               theName,
         EntityCategory <?>[] theDimenArr)
      {
      if (theDimenArr.length == 0)
         {
         throw
            new IllegalArgumentException (
               "\n\n"
             + "   Call to Scheme.newTupleCategory (theName, theDimens) with\n"
             + "   no Categories.\n"
             + "\n"
             + "      theName: \"" + theName + "\"\n");
         }
      }
   
/*******************************************************************************
 * Verifies that the Scheme of each EntityCategory in theDimenArr is the current
 * Scheme.<br>
 * To be called by newTupleCategory.
 *******************************************************************************
 */
   private static void reqSchemesBeCur (
         String               theName,
         EntityCategory <?>[] theDimenArr)
      {
      for (EntityCategory <?> theEntCat: theDimenArr)
         {
         if (theEntCat.getScheme () != current.get ())
            throw
               new IllegalArgumentException (
                  "\n\n"
                + "   Call to\n"
                + "   Scheme.newTupleCategory (theName, theDimens) with at\n"
                + "   least one EntityCategory whose Scheme is not the\n" 
                + "   current Scheme.\n"
                + "\n"
                + "      theName:        " +                theName    + "\n"
                + "      EntityCategory: " + theEntCat.     getName () + "\n"
                + "         Scheme:      " + theEntCat.getScheme ().
                                                            getName () + "\n"
                + "      Current Scheme: " + current.get ().getName () + "\n");
         }
      }
   
////////////////////////////////////////////////////////////////////////////////
// Private Non-Static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Verifies that this Scheme is not frozen.
 *******************************************************************************
 */
   private void banFrozen ()
      {
      if (frozen)
         throw
            new IllegalArgumentException (
               "\n\n"
             + "   Call to setCurrentScheme (theScheme) with a frozen Scheme.\n"
             + "\n"
             + "      Scheme: " + myName + "\n");
      }

/*******************************************************************************
 * Verifies that this Scheme is frozen.
 *******************************************************************************
 */
   private void reqFrozen (String theName)
      {
      if (! frozen)
         throw
            new IllegalStateException (
               "\n\n"
             + "   Call to Scheme.newEmbodiment (theName), when the Scheme\n"
             + "   was not frozen:\n"
             + "\n"
             + "      Scheme:  " + myName  + "\n"
             + "      theName: " + theName + "\n");
      }
   
////////////////////////////////////////////////////////////////////////////////
// Package Internal Non-Static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Verifies that theName does not duplicate the name of an existing schematic
 * object in this Scheme.
 *******************************************************************************
 */
   void banDupName (String theName, String signature)
      {
      if (allNames.contains (theName))
         throw
            new IllegalArgumentException (
               "\n\n"
             + "   Call to " + signature + "\n"
             + "   with a name that duplicates the name of an existing\n"
             + "   schematic object in the Scheme.\n"
             + "\n"
             + "      Duplicate Name: \"" + theName + "\"\n");
      }

/*******************************************************************************
 * Stores theName as the name of a schematic object that belongs to this Scheme.
 *******************************************************************************
 */
   void storeName (String theName)
      {
      allNames.add (theName);
      }

/*******************************************************************************
 * Stores theEntCat (a newly constructed EntityCategory) in this Scheme.
 *******************************************************************************
 */
   void store (EntityCategory <?> theEntCat)
      {
      myEntityCategories.add (theEntCat);
      }

/*******************************************************************************
 * @return An unmodifiable view of a list of all Categories that belong to this
 *         Scheme.
 *******************************************************************************
 */
   List <EntityCategory <?>> getEntityCategories ()
      {
      return Collections.unmodifiableList (myEntityCategories);
      }

/*******************************************************************************
 * Records the fact that a OneToOne was created in this Scheme.
 *******************************************************************************
 */
   void recordNewOneToOne ()
      {
      nOneToOnes ++;
      }

/*******************************************************************************
 * @return The # of OneToOnes in this Scheme.
 *******************************************************************************
 */
   int getNOneToOnes ()
      {
      return nOneToOnes;
      }

////////////////////////////////////////////////////////////////////////////////
// Public Static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Sets the current Scheme. The current Scheme is a designated Scheme that will
 * be used by various WIT-M methods in the current Thread.
 * 
 * @param theScheme The Scheme which is to become current. (Null is allowed.)
 * 
 * @throws IllegalArgumentException If theScheme is frozen
 *******************************************************************************
 */
   public static void setCurrentScheme (Scheme theScheme)
      {
      if (theScheme != null)
         theScheme.banFrozen ();

      current.set (theScheme);
      }

/*******************************************************************************
 * Returns the current Scheme, if any; otherwise null.
 * 
 * @see setCurrentScheme
 *******************************************************************************
 */
   public static Scheme currentScheme ()
      {
      return current.get ();
      }

/*******************************************************************************
 * Puts the current Scheme into a frozen state.<br>
 * When a Scheme is frozen, its state and the state of the Categories and
 * Properties that belong to it cannot be changed.<br>
 * This method also sets the current Scheme reference to null.<br>
 * In general, a frozen Scheme is never allowed to be the current Scheme.
 * 
 * @throws IllegalStateException If the current Scheme is null
 *******************************************************************************
 */
   public static void freezeCurrentScheme ()
      {
      Scheme theScheme;
      
      reqCurScheme ();
      
      theScheme = current.get ();
      
      current.remove ();
      
      for (EntityCategory <?> theEntCat: theScheme.myEntityCategories)
         theEntCat.freeze ();
      
      theScheme.frozen = true;
      }

/*******************************************************************************
 * Returns the Global Category for the current Scheme.
 * 
 * @throws IllegalStateException If the current Scheme is null
 *******************************************************************************
 */
   public static ItemCategory currentGlobalCategory ()
      {
      reqCurScheme ();
      
      return current.get ().myGlobalCategory;
      }

/*******************************************************************************
 * Returns the Period Category for the current Scheme.
 * 
 * @throws IllegalStateException If the current Scheme is null
 *******************************************************************************
 */
   public static ItemCategory currentPeriodCategory ()
      {
      reqCurScheme ();
      
      return current.get ().myPeriodCategory;
      }

/*******************************************************************************
 * Factory method:<br>
 * Constructs a new Scheme.<br>
 * Sets the current Scheme to the new Scheme.<br>
 * The name of the Global Category for the Scheme will be "GLOBAL".<br>
 * The name of the Period Category for the Scheme will be "PERIOD".
 *
 * @param  theSchemeName The name of the new Scheme
 * @return               The new Scheme
 * 
 * @throws NullPointerException  If any argument is null
 *******************************************************************************
 */
   public static Scheme newScheme (String theSchemeName)
      {
      banNull (theSchemeName, "theSchemeName");
      
      return new Scheme (theSchemeName, "GLOBAL", "PERIOD");
      }

/*******************************************************************************
 * Factory method:<br>
 * Constructs a new Scheme.<br>
 * Sets the current Scheme to the new Scheme.<br>
 *
 * @param  theSchemeName    The name of the new Scheme
 * @param  theGlobalCatName The name of the Global Category for the new Scheme
 * @param  thePeriodCatName The name of the Period Category for the new Scheme
 * @return                  The new Scheme
 * 
 * @throws NullPointerException     If any argument is null
 * @throws IllegalArgumentException If any two of the argument names are
 *                                  identical
 *******************************************************************************
 */
   public static Scheme newScheme (
         String theSchemeName,
         String theGlobalCatName,
         String thePeriodCatName)
      {
      banNull (theSchemeName,    "theSchemeName");
      banNull (theGlobalCatName, "theGlobalCatName");
      banNull (thePeriodCatName, "thePeriodCatName");

      banSameName (theSchemeName,      theGlobalCatName,
                  "theSchemeName",    "theGlobalCatName");
      banSameName (theSchemeName,      thePeriodCatName,
                  "theSchemeName",    "thePeriodCatName");
      banSameName (theGlobalCatName,   thePeriodCatName,
                  "theGlobalCatName", "thePeriodCatName");
      
      return new Scheme (theSchemeName, theGlobalCatName, thePeriodCatName);
      }

/*******************************************************************************
 * Factory method:<br>
 * Constructs a new ItemCategory that will belong to the current Scheme.
 * 
 * @param theName                   The name of the ItemCategory to be
 *                                  constructed
 * @return                          The new ItemCategory
 * @throws NullPointerException     If theName is null
 * @throws IllegalStateException    If the current Scheme is null
 * @throws IllegalArgumentException If theName duplicates the name of an
 *                                  existing schematic object in the current
 *                                  Scheme
 *******************************************************************************
 */
   public static ItemCategory newItemCategory (String theName)
      {
      banNull                   (theName, "theName");
      reqCurScheme              ();
      current.get ().banDupName (theName, "Scheme.newItemCategory (theName)");
      
      return new ItemCategory (current.get (), theName);
      }

/*******************************************************************************
 * Factory method:<br>
 * Constructs a new TupleCategory for the current Scheme.
 * 
 * @param  theName                  The name of the TupleCategory to be
 *                                  constructed
 * @param  theDimens                The Categories that are to be the
 *                                  dimensions of the TupleCategory    
 * @return                          The new TupleCategory
 * @throws NullPointerException     If any argument is null
 * @throws IllegalStateException    If the current Scheme is null
 * @throws IllegalArgumentException If no Categories are given
 * @throws IllegalArgumentException The Categories do not all belong to the
 *                                  current Scheme
 * @throws IllegalArgumentException If theName duplicates the name of an 
 *                                  existing schematic object in the current
 *                                  Scheme
 *******************************************************************************
 */
   public static TupleCategory newTupleCategory (
         String                theName,
         EntityCategory <?>... theDimens)
      {
      banNull         (theName,   "theName");
      banNullArray    (theDimens, "theDimens");
      reqCurScheme    ();
      reqAtLeast1Cat  (theName,   theDimens);
      reqSchemesBeCur (theName,   theDimens);
      current.get ().
         banDupName   (theName,   "Scheme.newTupleCategory (theName)");
      
      return new TupleCategory (current.get (), theName, theDimens);
      }

////////////////////////////////////////////////////////////////////////////////
// Public Non-Static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * @return The name of the WIT model
 *******************************************************************************
 */
   public String getName ()
      {
      return myName;
      }

/*******************************************************************************
 * @return True, iff this Scheme is "frozen".<br>
 * When a Scheme is frozen, its state cannot be changed.
 *******************************************************************************
 */
   public boolean isFrozen ()
      {
      return frozen;
      }

/*******************************************************************************
 * Return The Global Category for this Scheme.<br>
 * The Global Category is an ItemCategory whose Properties store global data
 * for an Embodiment. Each Scheme has exactly one Global Category. Each
 * Embodiment has exactly one Item that belongs to the Global Category of its
 * Scheme, called that Global Item of the Embodiment.
 *******************************************************************************
 */
   public ItemCategory getGlobalCategory ()
      {
      return myGlobalCategory;
      }

/*******************************************************************************
 * Returns the Period Category for this Scheme.<br>
 * The Period Category is an ItemCategory whose items are intended to represent
 * time periods that will correspond to WIT periods. Each Scheme has exactly
 * one Period Category. The Items of the Period Category are called Periods.
 *******************************************************************************
 */
   public ItemCategory getPeriodCategory ()
      {
      return myPeriodCategory;
      }

/*******************************************************************************
 * {@inheritDoc}
 *******************************************************************************
 */             @Override
   public String toString ()
      {
      return myName; 
      }
   
/*******************************************************************************
 * Prints this Scheme.
 * 
 * @param theStream the PrintStream to which this Scheme is to be printed.
 *******************************************************************************
 */
   public void print (PrintStream theStream)
      {
      theStream.printf (
         "Detailed Print of Scheme %s:%n"
       + "%n"
       + "Frozen Scheme? %s%n",
         myName,
         frozen);
      
      for (EntityCategory <?> theEntCat: myEntityCategories)
         theEntCat.print (theStream);
      }

/*******************************************************************************
 * Factory method: Constructs a new Embodiment for this Scheme.<br>
 * The current Embodiment is set to the new Embodiment.<br>
 *
 * @param  theName               The name of the new Embodiment
 * @return                       The new Embodiment
 * @throws NullPointerException  If any argument is null
 * @throws IllegalStateException If this Scheme is not frozen
 *******************************************************************************
 */
   public Embodiment newEmbodiment (String theName)
      {
      banNull   (theName, "theName");
      reqFrozen (theName);
      
      return new Embodiment (this, theName);
      }
      
////////////////////////////////////////////////////////////////////////////////
}
