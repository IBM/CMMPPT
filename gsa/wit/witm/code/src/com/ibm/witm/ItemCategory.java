
////////////////////////////////////////////////////////////////////////////////
// WIT-M Source File ItemCategory.java.
////////////////////////////////////////////////////////////////////////////////

package com.ibm.witm;

import java.io.PrintStream;

import static com.ibm.witm.Utility.*;

/*******************************************************************************
 * An ItemCategory is an EntityCategory of Items.<br><br>
 * 
 * The factory method for creating instances of this class is
 * Scheme.newItemCategory (theName).
 *******************************************************************************
 */
public final class ItemCategory extends EntityCategory <Item>
{
////////////////////////////////////////////////////////////////////////////////
// Private Non-Static Methods
////////////////////////////////////////////////////////////////////////////////
   
/*******************************************************************************
 * Verifies that this ItemCategory is not the GLOBAL EntityCategory.
 *******************************************************************************
 */
   private void banGlobalCat ()
      {
      if (this == getScheme ().getGlobalCategory ())
         throw
            new IllegalStateException (
               "\n\n"
             + "   Call to ItemCategory.newItem (theName) on the GLOBAL\n"
             + "   ItemCategory.\n");
      }
   
/*******************************************************************************
 * Verifies that theName does not match the name of an Item of this
 * ItemCategory in the current Embodiment.
 *******************************************************************************
 */
   private void banDupName (String theName)
      {
      Item theItem;
      
      theItem = curEmb ().getItemMgr (this).getItem (theName);
      
      if (theItem != null)
         throw
            new IllegalArgumentException (
               "\n\n"
             + "   Call to ItemCategory.newItem (theName), where\n"
             + "   theName matches the name of an existing Item of\n"
             + "   the ItemCategory in the current Embodiment.\n"
             + "\n"
             + "      ItemCategory: "   + getName () + "\n"
             + "      Item Name:    \"" + theName    + "\"\n");
      }

////////////////////////////////////////////////////////////////////////////////
// Package Internal Constructor
////////////////////////////////////////////////////////////////////////////////
   
   ItemCategory (Scheme theScheme, String theName)
      {
      super (theScheme, theName);
      }

////////////////////////////////////////////////////////////////////////////////
// Package Internal Non-Static Methods
////////////////////////////////////////////////////////////////////////////////
   
/*******************************************************************************
 * {@inheritDoc}
 *******************************************************************************
 */    @Override
   void printConcrete (PrintStream theStream)
      {
      theStream.printf (
         "%n"
      +  "ItemCategory %s:%n",
         getName ());
      }

/*******************************************************************************
 * {@inheritDoc}
********************************************************************************
 */       @Override 
   ItemMgr newEntityMgr (Embodiment theEmb)
      {
      return new ItemMgr ();
      }

////////////////////////////////////////////////////////////////////////////////
// Public Non-Static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Factory method:<br>
 * Constructs a new Item for this ItemCategory in the current Embodiment.
 *   
 * @param  theName                  The name for the Item 
 * @return                          The new Item
 * @throws NullPointerException     If any argument is null
 * @throws IllegalStateException    If the current Embodiment is null
 * @throws IllegalStateException    If the Scheme of this ItemCategory does not
 *                                  match the Scheme of the current Embodiment
 * @throws IllegalStateException    If this ItemCategory is the global
 *                                  EntityCategory
 * @throws IllegalArgumentException If theName matches the name of an existing
 *                                  Item of this ItemCategory in the current
 *                                  Embodiment
 *******************************************************************************
 */
   public Item newItem (String theName)
      {
      banNull                (theName, "theName");
      reqCurEmb              ();
      reqSchemeMatchesCurEmb ();
      banGlobalCat           ();
      banDupName             (theName);

      if (isPeriodCategory ())
         return new Period (this, theName, curEmb ());
      else
         return new Item   (this, theName, curEmb ());
      }

/*******************************************************************************
 * Item look-up method.
 * 
 * @param  theName The name of the Item to be retrieved
 * @return         The Item of this ItemCategory in the current Embodiment whose
 *                 name matches theName, if any; otherwise null
 *         
 * @throws NullPointerException     If any argument is null
 * @throws IllegalStateException    If the current Embodiment is null
 * @throws IllegalArgumentException If the Scheme of this ItemCategory does not
 *                                  match the Scheme of the current Embodiment
 *******************************************************************************
 */
   public Item get (String theName)
      {
      banNull                (theName, "theName");
      reqCurEmb              ();
      reqSchemeMatchesCurEmb ();
      
      return curEmb ().getItemMgr (this).getItem (theName);
      }
   
////////////////////////////////////////////////////////////////////////////////
}
