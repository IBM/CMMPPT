
////////////////////////////////////////////////////////////////////////////////
// WIT-M/POK Source File Reporter.java.
////////////////////////////////////////////////////////////////////////////////

package com.ibm.witm.pok;

import static com.ibm.witm.Embodiment.currentGlobalItem;
import static com.ibm.witm.pok.SchemeKeeper.N_PERS2;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;

/*******************************************************************************
 * Responsible for writing (to CSV files) the solution to the POK model
 * instance. This class works with the current Embodiment, which is
 * assumed to belong to Scheme POK.<br>
 * This is a non-instantiatable class: it is to be used statically.
 *******************************************************************************
 */
final class Reporter
{
////////////////////////////////////////////////////////////////////////////////
// Private Constructors
////////////////////////////////////////////////////////////////////////////////
   
/*******************************************************************************
 * (Not called)
 *******************************************************************************
 */
   private Reporter ()
      {
      }
   
////////////////////////////////////////////////////////////////////////////////
// Private Static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Opens a PrintWriter.
 * 
 * @param fileName The name of the file
 * @return         The PrintWriter
 *******************************************************************************
 */
   private static PrintWriter openPrintWriter (String fileName)
      {
      FileWriter     theFileWriter;
      BufferedWriter theBufferedWriter;

      try
         {
         theFileWriter = new FileWriter (fileName);
         }

      catch (IOException theIOExc)
         {
         System.err.printf (
            "%n"
          + "The following Exception was thrown when attempting to open the "
          + "file \"%s\":%n"
          + "%n"
          + "   %s%n"
          + "%n"
          + "The WIT-M POK example terminates.%n"
          + "%n",
            fileName,
            theIOExc);
         
         System.exit (3);
         
         return null;
         }

      theBufferedWriter = new BufferedWriter (theFileWriter);

      return new PrintWriter (theBufferedWriter);
      }

/*******************************************************************************
 * Closes a PrintWriter and terminates with an error message if there was an
 * error.
 * 
 * @param thePrintWriter The PrintWriter
 * @param fileName       The name of the file.
 *******************************************************************************
 */
   private static void closePrintWriter (
         PrintWriter thePrintWriter,
         String      fileName)
      {
      thePrintWriter.close ();
      
      if (thePrintWriter.checkError ())
         {
         System.err.printf (
            "%n"
          + "An error occurred when writing to or closing the file \"%s\".%n"
          + "%n"
          + "The WIT-M POK example terminates.%n"
          + "%n",
            fileName);
         
         System.exit (3);
         }
      }

/*******************************************************************************
 * Writes out-global.csv from the solution.
 *******************************************************************************
 */
   private static void writeGlobalFile ()
      {
      PrintWriter thePrintWriter;

      thePrintWriter = openPrintWriter ("output/out-global.csv");

      thePrintWriter.printf ("# Periods * 2%n");

      thePrintWriter.printf (
         "%d%n",
         currentGlobalItem ().get (N_PERS2));

      closePrintWriter (thePrintWriter, "output/out-global.csv");
      }

////////////////////////////////////////////////////////////////////////////////
// Package Internal Static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Writes the solution to the POK model instance.
 *******************************************************************************
 */
   static void writeSolution ()
      {
      writeGlobalFile ();
      }

////////////////////////////////////////////////////////////////////////////////
}
