
////////////////////////////////////////////////////////////////////////////////
// WIT-M/POK Source File Builder.java.
////////////////////////////////////////////////////////////////////////////////

package com.ibm.witm.pok;

import static com.ibm.witm.pok.SchemeKeeper.*;
import static com.ibm.witm.Embodiment.*;

import com.ibm.witm.InputRow;
import com.ibm.witm.InputTable;

/*******************************************************************************
 * Responsible for building up the Embodiment of the POK model, based on data
 * from CSV files. This class works with the current Embodiment, which is
 * assumed to belong to Scheme POK.<br>
 * This is a non-instantiatable class: it is to be used statically.
 *******************************************************************************
 */
final class Builder
{
////////////////////////////////////////////////////////////////////////////////
// Private Constructors
////////////////////////////////////////////////////////////////////////////////
   
/*******************************************************************************
 * (Not called)
 *******************************************************************************
 */
   private Builder ()
      {
      }
   
////////////////////////////////////////////////////////////////////////////////
// Private Static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Reads global.csv and populates from its data.
 *******************************************************************************
 */
   private static void readGlobalFile ()
      {
      InputTable theTable;
      InputRow   theRow;
      int        nPers;

      theTable = InputTable.newInputTable ("global.csv", ",");

      theRow   = theTable.getRow (2);

      theRow.requireNCells (20);

      nPers    = theRow.getCell (2).asInt ("# Periods");

      currentGlobalItem ().set (N_PERS, nPers);
      }

////////////////////////////////////////////////////////////////////////////////
// Package Internal Static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Builds up the current Embodiment, based on data from CSV files.
 *******************************************************************************
 */
   static void buildEmbodiment ()
      {
      readGlobalFile ();
      }

////////////////////////////////////////////////////////////////////////////////
}
