
////////////////////////////////////////////////////////////////////////////////
// WIT-M Source File Item.java.
////////////////////////////////////////////////////////////////////////////////

package com.ibm.witm;

import static com.ibm.witm.Utility.*;

/*******************************************************************************
 * An Item is an Entity that's considered to be "primitive", i.e., not defined
 * in terms of other Entities.
 * Each Item has a name and belongs to a particular ItemCategory and a
 * particular Embodiment.<br><br>
 * 
 * The factory method for this class is ItemCategory.newItem (theName).
 *******************************************************************************
 */
public class Item extends Entity
{
////////////////////////////////////////////////////////////////////////////////
// Private Non-static Fields
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * The name of this Item
 *******************************************************************************
 */
   private final String myName;

////////////////////////////////////////////////////////////////////////////////
// Package Internal Constructor
////////////////////////////////////////////////////////////////////////////////
   
   Item (ItemCategory theItemCat, String theName, Embodiment theEmb)
      {
      super (theItemCat, theEmb);
      
      myName = theName;
      
      theEmb.getItemMgr (theItemCat).storeItem (this);
      }
   
////////////////////////////////////////////////////////////////////////////////
// Package Internal Non-static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * {@inheritDoc}
 *******************************************************************************
 */      @Override
   String getID ()
      {
      return concatenation ("'", myName, "'");
      }

////////////////////////////////////////////////////////////////////////////////
// Public Non-static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * @return The name of this Item
 *******************************************************************************
 */
   public final String getName ()
      {
      return myName;
      }

/*******************************************************************************
 * @return The ItemCategory of this Item
 *******************************************************************************
 */
   public final ItemCategory getItemCategory ()
      {
      return (ItemCategory) getEntityCategory ();
      }

/*******************************************************************************
 * @return The period index of this Item, which is the number of Periods that
 *         were created before it in the Embodiment
 * 
 * @throws IllegalStateException If this Item does not belong to the Period
 *                               ItemCategory.
 *******************************************************************************
 */
   public int getPeriodIndex ()
      {
      throw
         new IllegalStateException (
            "\n\n"
          + "   Call to Item.getPeriodIndex () on an Item that does not\n"
          + "   belong to the Period Category.\n"
          + "\n"
          + "      Item: " + this + "\n");
      }

////////////////////////////////////////////////////////////////////////////////
}
