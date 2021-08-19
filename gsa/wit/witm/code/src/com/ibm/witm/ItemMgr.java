
////////////////////////////////////////////////////////////////////////////////
// WIT-M Source File ItemMgr.java.
////////////////////////////////////////////////////////////////////////////////

package com.ibm.witm;

import java.util.HashMap;

/*******************************************************************************
 * "Item Manager"<br>
 * An ItemMgr is responsible for maintaining the Items of a particular
 * ItemCategory that belong to a particular Embodiment.
 *******************************************************************************
 */
final class ItemMgr extends EntityMgr <Item>
{
////////////////////////////////////////////////////////////////////////////////
// Private Non-Static Fields
////////////////////////////////////////////////////////////////////////////////
   
/*******************************************************************************
 * myItemMap.get (theName) is the Item maintained by this ItemMgr whose name
 * matches theName.
 *******************************************************************************
 */
   private final HashMap <String, Item> myItemMap;
   
////////////////////////////////////////////////////////////////////////////////
// Package Internal Constructor
////////////////////////////////////////////////////////////////////////////////
   
   ItemMgr ()
      {
      myItemMap = new HashMap <String, Item> ();
      }
   
////////////////////////////////////////////////////////////////////////////////
// Package Internal Non-Static Methods
////////////////////////////////////////////////////////////////////////////////
   
/*******************************************************************************
 * Stores theItem in this ItemMgr.
 *******************************************************************************
 */
   void storeItem (Item theItem)
      {
      storeEntity (theItem);
      
      myItemMap.put (theItem.getName (), theItem);
      }
   
/*******************************************************************************
 * @return The Item maintained by this ItemMgr whose name matches
 * theName, if any; otherwise null.
 *******************************************************************************
 */
   Item getItem (String theName)
      {
      return myItemMap.get (theName);
      }
   
////////////////////////////////////////////////////////////////////////////////
}
