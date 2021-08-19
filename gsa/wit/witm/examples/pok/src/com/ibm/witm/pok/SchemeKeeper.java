
////////////////////////////////////////////////////////////////////////////////
// WIT-M/POK Source File SchemeKeeper.java.
////////////////////////////////////////////////////////////////////////////////

package com.ibm.witm.pok;

import com.ibm.witm.ItemCategory;
import com.ibm.witm.Property;
import com.ibm.witm.Scheme;

import static com.ibm.witm.Scheme.*;

/*******************************************************************************
 * Responsible for owning and providing access to  all of the schematic objects
 * of the model.<br>
 * This is a non-instantiatable class: it is to be used statically.
 *******************************************************************************
 */
final class SchemeKeeper
{
////////////////////////////////////////////////////////////////////////////////
// Private Constructors
////////////////////////////////////////////////////////////////////////////////
   
/*******************************************************************************
 * (Not called)
 *******************************************************************************
 */
   private SchemeKeeper ()
      {
      }
   
////////////////////////////////////////////////////////////////////////////////
// Package Internal Static Fields
//
// Declaration of the POK Scheme and its elements.
// The POK Scheme is the Scheme for the POK model.
////////////////////////////////////////////////////////////////////////////////

   static final Scheme             POK;
   
   static final ItemCategory       GLOBAL;
   static final Property <Integer> N_PERS;
   static final Property <Integer> N_PERS2;
   
////////////////////////////////////////////////////////////////////////////////
// Static Initialization Block
//
// Constructs the POK Scheme.
// Populates  the POK Scheme.
// Freezes    the POK Scheme.
////////////////////////////////////////////////////////////////////////////////

static
   {
   POK     = Scheme.newScheme ("POK");
   
   GLOBAL  = currentGlobalCategory ();
   
   N_PERS  = GLOBAL.newProperty ("N_PERS",  Integer.class);
   N_PERS2 = GLOBAL.newProperty ("N_PERS2", Integer.class);

   freezeCurrentScheme ();
   }

////////////////////////////////////////////////////////////////////////////////
}
