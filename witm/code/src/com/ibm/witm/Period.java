
////////////////////////////////////////////////////////////////////////////////
// WIT-M Source File Period.java.
////////////////////////////////////////////////////////////////////////////////

package com.ibm.witm;

import static com.ibm.witm.Utility.concatenation;

/*******************************************************************************
 * A Period is an Item that belongs to the Period Category of the Scheme.
 * A Period is intended to represent a time period that will correspond to a WIT
 * period.<br><br>
 * The factory method for this class is ItemCategory.newItem (theName).
 *******************************************************************************
 */
final class Period extends Item
{
////////////////////////////////////////////////////////////////////////////////
// Private Non-static Fields
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * The period index of this Period, which is the number of Periods that were
 * created before it in the Embodiment.
 *******************************************************************************
 */
   private final int myPeriodIndex;

////////////////////////////////////////////////////////////////////////////////
// Package Internal Constructor
////////////////////////////////////////////////////////////////////////////////
   
   Period (ItemCategory theItemCat, String theName, Embodiment theEmb)
      {
      super (theItemCat, theName, theEmb);
      
      assert (theItemCat.isPeriodCategory ());

      myPeriodIndex = theEmb.getPeriodMgr ().getAll ().size () - 1;
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
      return
         concatenation (
            "'",
            getName (),
            "' (#",
            Integer.toString (myPeriodIndex),
            ")");
      }

////////////////////////////////////////////////////////////////////////////////
// Public Non-static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * @return The period index of this Period, which is the number of Periods that
 *         were created before it in the Embodiment.
 *******************************************************************************
 */          @Override
   public int getPeriodIndex ()
      {
      return myPeriodIndex;
      }

////////////////////////////////////////////////////////////////////////////////
}
