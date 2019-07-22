
////////////////////////////////////////////////////////////////////////////////
// WIT-M Source File Embodiment.java.
////////////////////////////////////////////////////////////////////////////////

package com.ibm.witm;

import static com.ibm.witm.Utility.*;

import java.util.ArrayList;
import java.util.HashMap;

import com.ibm.witj.Problem;

/*******************************************************************************
 * An Embodiment is the central object representing a concrete embodiment of a
 * WIT model, i.e., a WIT model as populated with specific data.<br><br>
 *
 * The factory method for this class is Scheme.newEmbodiment (theName).
 *******************************************************************************
 */
public final class Embodiment
{
////////////////////////////////////////////////////////////////////////////////
// Private Static Fields
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * current.get () returns the "current Embodiment": This is a designated
 * Embodiment that will be used by various WIT-M methods in the current Thread
 * if any; otherwise null.
 *******************************************************************************
 */
   private static ThreadLocal <Embodiment> current;
   
////////////////////////////////////////////////////////////////////////////////
// Static Initialization Block
////////////////////////////////////////////////////////////////////////////////
   
   static
      {
      current = new ThreadLocal <Embodiment> ();
      }

////////////////////////////////////////////////////////////////////////////////
// Private Non-Static Fields
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * The Scheme implemented by this Embodiment
 *******************************************************************************
 */
   private final Scheme myScheme;
   
/*******************************************************************************
 * The name of this Embodiment
 *******************************************************************************
 */
   private final String myName;
   
/*******************************************************************************
 * myEntityMgrArr[theEntCat.getIndex ()] is the EntityMgr for theEntCat in this
 * Embodiment.
 *******************************************************************************
 */
   private final EntityMgr <?> [] myEntityMgrArr;

/*******************************************************************************
 * The global Item for this Embodiment.
 *******************************************************************************
 */
   private final Item myGlobalItem;
   
/*******************************************************************************
 * The ItemMgr for the Period Category of myScheme, if there is one;
 * otherwise null.
 *******************************************************************************
 */
   private final ItemMgr myPeriodMgr;
   
/*******************************************************************************
 * The list of inverse Maps for each OneToOne in myScheme.<br><br>
 * 
 * For each OneToOne, theOTO, in myScheme:
 * let theInvMap = myInverseMaps.get (theOTO.getOTOIndex ()).<br>
 * For each Entity, theEnt, in myEntityCategory in the Embodiment,
 * let theValue = theEnt.get (theOTO).<br>
 * If theValue != null, then theInvMap.get (theValue) == theEnt.
 *******************************************************************************
 */
   private final ArrayList <HashMap <Object, Entity>> myInverseMaps;

/*******************************************************************************
 * The WIT-J Problem currently associated with this Embodiment, if any;
 * otherwise null.
 *******************************************************************************
 */
   private Problem myProblem;
   
/*******************************************************************************
 * True, iff short names are to be used for Parts, Operations, and Demands.
 *******************************************************************************
 */
   private boolean useShortNames;
   
////////////////////////////////////////////////////////////////////////////////
// Package Internal Constructor
////////////////////////////////////////////////////////////////////////////////

   Embodiment (Scheme theScheme, String theName)
      {
      int                      nCats;
      int                      theIdx;
      HashMap <Object, Entity> theInvMap;
      
      myScheme       = theScheme;
      myName         = theName;
      
      nCats          = myScheme.getEntityCategories ().size ();
      
      myEntityMgrArr = new EntityMgr <?>[nCats];

      for (EntityCategory <?> theEntCat: myScheme.getEntityCategories ())
         {
         theIdx                 = theEntCat.getIndex ();
         
         myEntityMgrArr[theIdx] = theEntCat.newEntityMgr (this);
         }

      myGlobalItem = new Item (theScheme.getGlobalCategory (), "Global", this);
      
      myPeriodMgr  = getItemMgr (myScheme.getPeriodCategory ());
   
      myInverseMaps   =
         new ArrayList <HashMap <Object, Entity>> (myScheme.getNOneToOnes ());

      for (theIdx = 0; theIdx < myScheme.getNOneToOnes (); theIdx ++)
         {
         theInvMap = new HashMap <Object, Entity> ();
            
         myInverseMaps.add (theInvMap);
         }
      
      myProblem     = null;
      useShortNames = false;

      current.set (this);
      }

////////////////////////////////////////////////////////////////////////////////
// Package Internal Non-Static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * @return the EntityMgr for theEntCat in this Embodiment.
 *******************************************************************************
 */                                @SuppressWarnings("unchecked")
   <E extends Entity> EntityMgr <E> getEntityMgr (EntityCategory <E> theEntCat)
      {
      EntityMgr <?> theEntMgr;
      
      assert (theEntCat.getScheme () == myScheme);
         
      theEntMgr = myEntityMgrArr[theEntCat.getIndex ()];
      
      return (EntityMgr <E>) theEntMgr;
      }
  
/*******************************************************************************
 * @return the ItemMgr for theItemCat in this Embodiment.
 *******************************************************************************
 */
   ItemMgr getItemMgr (ItemCategory theItemCat)
      {
      return (ItemMgr) getEntityMgr (theItemCat);
      }
   
/*******************************************************************************
 * @return the TupleMgr for theTupleCat in this Embodiment.
 *******************************************************************************
 */
   TupleMgr getTupleMgr (TupleCategory theTupleCat)
      {
      return (TupleMgr) getEntityMgr (theTupleCat);
      }
   
/*******************************************************************************
 * @return the ItemMgr for the Period Category in this Embodiment, or null if
 *         there is no Period Category.
 *******************************************************************************
 */
   ItemMgr getPeriodMgr ()
      {
      return myPeriodMgr;
      }
   
/*******************************************************************************
 * Sets the value of theOTO for theEnt to theValue.<br>
 * Maintains the one-to-one invariant of theOTO and revising the corresponding
 * inverse Map, as appropriate.
 *******************************************************************************
 */
   <V> void setValue (OneToOne <?, V> theOTO, Entity theEnt, V theValue)
      {
      HashMap <Object, Entity> theInvMap;
      V                        oldValue;
      Entity                   oldEnt;

      theInvMap = myInverseMaps.get (theOTO.getOTOIndex ());

      oldValue  = theEnt.getValue (theOTO);

      if (oldValue != null)
         theInvMap.remove (oldValue);
      
      if (theValue != null)
         {
         oldEnt = theInvMap.get (theValue);
         
         if (oldEnt != null)
            oldEnt.setValue (theOTO, null);
         }
      
      theEnt.setValue (theOTO, theValue);
      
      theInvMap.put (theValue, theEnt);
      }

/*******************************************************************************
 * @returns The unique Entity in this Embodiment whose value for theOTO is equal
 *          to theValue, if any; otherwise null.
 *******************************************************************************
 */                       @SuppressWarnings("unchecked")
   <E extends Entity, V> E getEntity (OneToOne <E, V> theOTO, V theValue)
      {
      HashMap <Object, Entity> theInvMap;

      theInvMap = myInverseMaps.get (theOTO.getOTOIndex ());

      return (E) theInvMap.get (theValue);
      }

////////////////////////////////////////////////////////////////////////////////
// Public Static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Sets the current Embodiment. This is a designated Embodiment that will be
 * used by various WIT-M methods in the current Thread if any; otherwise null.
 * <br>
 * Note: The current Embodiment should eventually be set to null, in order to
 * allow the Embodiment to be garbage collected.
 * 
 * @param theEmb The Embodiment which is to become current. (Null is allowed.)
 *******************************************************************************
 */
   public static void setCurrentEmbodiment (Embodiment theEmb)
      {
      current.set (theEmb);
      }

/*******************************************************************************
 * Returns The current Embodiment if any; otherwise null.
 * @see    setCurrentEmbodiment
 *******************************************************************************
 */
   public static Embodiment currentEmbodiment ()
      {
      return current.get ();
      }

/*******************************************************************************
 * Returns The global Item for the current Embodiment
 *
 * @throws IllegalStateException If the current Embodiment is null
 *******************************************************************************
 */
   public static Item currentGlobalItem ()
      {
      reqCurEmb ();
      
      return current.get ().myGlobalItem;
      }

/*******************************************************************************
 * Returns the Problem associated with the current Embodiment, if any;
 * otherwise null
 *
 * @throws IllegalStateException If the current Embodiment is null
 *******************************************************************************
 */
   public static Problem currentProblem ()
      {
      reqCurEmb ();
      
      return current.get ().myProblem;
      }

////////////////////////////////////////////////////////////////////////////////
// Public Non-Static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * @return The name of this Embodiment
 *******************************************************************************
 */
   public String getName ()
      {
      return myName;
      }

/*******************************************************************************
 * @return The Scheme of this Embodiment
 *******************************************************************************
 */
   public Scheme getScheme ()
      {
      return myScheme;
      }

/*******************************************************************************
 * Associates a WIT-J Problem with this Embodiment.
 * 
 * @param theProblem The Problem to be associated with this Embodiment.
 *                   Null is allowed.
 *******************************************************************************
 */
   public void setProblem (Problem theProblem)
      {
      myProblem = theProblem;
      }

/*******************************************************************************
 * @return The WIT-J Problem currently associated with this Embodiment, if any;
 *         otherwise null.
 *******************************************************************************
 */
   public Problem getProblem ()
      {
      return myProblem;
      }

/*******************************************************************************
 * Sets the value of the useShortNames variable for this Embodiment to a
 * specified value.  When an Entity creates a new Part, Operation, or Demand, it
 * will use a short name, iff the useShortNames variable for its embodiment is
 * true.
 * 
 * @param theValue The value to which the useShortNames variable will be set.
 *******************************************************************************
 */
   public void setUseShortNames (boolean theValue)
      {
      useShortNames = theValue;
      }

/*******************************************************************************
 * @return The value of the useShortNames for this Embodiment.
 *******************************************************************************
 */
   public boolean getUseShortNames ()
      {
      return useShortNames;
      }

/*******************************************************************************
 * {@inheritDoc}
 *******************************************************************************
 */             @Override
   public String toString ()
      {
      return myName;
      }

////////////////////////////////////////////////////////////////////////////////
}
