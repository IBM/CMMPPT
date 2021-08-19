
////////////////////////////////////////////////////////////////////////////////
// WIT-M Source File EntityMgr.java.
////////////////////////////////////////////////////////////////////////////////

package com.ibm.witm;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/*******************************************************************************
 * "Entity Manager"<br>
 * An EntityMgr {@literal<E>} is responsible for maintaining the Entities of a
 * particular EntityCategory that belong to a particular Embodiment.
 * 
 * @param <E> The type of Entity being maintained: either Item or Tuple
 *******************************************************************************
 */
abstract class EntityMgr <E extends Entity>
{
////////////////////////////////////////////////////////////////////////////////
// Private Non-Static Fields
////////////////////////////////////////////////////////////////////////////////
   
/*******************************************************************************
 * The List of all Entities maintained by this EntityMgr, listed in the order
 * in which they were created.
 *******************************************************************************
 */
   private final ArrayList <E> myEntities;
   
/*******************************************************************************
 * An unmodifiable view of myEntities.
 *******************************************************************************
 */
   private final List <E> myEntitiesView;
   
////////////////////////////////////////////////////////////////////////////////
// Package Internal Constructor
////////////////////////////////////////////////////////////////////////////////
   
   EntityMgr ()
      {
      myEntities     = new ArrayList <E> ();
      
      myEntitiesView = Collections.unmodifiableList (myEntities);
      }
   
////////////////////////////////////////////////////////////////////////////////
// Package Internal Non-Static Methods
////////////////////////////////////////////////////////////////////////////////
   
/*******************************************************************************
 * Stores theI in this EntityMgr.
 *******************************************************************************
 */
   void storeEntity (E theI)
      {
      myEntities.add (theI);
      }
   
/*******************************************************************************
 * @return An unmodifiable view of an ArrayList of all I instances maintained by
 *         this EntityMgr, listed in the order in which they were created.
 *******************************************************************************
 */
   List <E> getAll ()
      {
      return myEntitiesView;
      }
   
////////////////////////////////////////////////////////////////////////////////
}
