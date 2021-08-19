
////////////////////////////////////////////////////////////////////////////////
// WIT-M Source File DsvFileReader.java.
////////////////////////////////////////////////////////////////////////////////

package com.ibm.witm;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Pattern;

/*******************************************************************************
 * Responsible for reading a DSV (delimiter separated value) file.
 *******************************************************************************
 */
final class DsvFileReader
{
////////////////////////////////////////////////////////////////////////////////
// Private Non-Static Fields
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * The name of the DSV file to be read.
 *******************************************************************************
 */
   private final String myFileName;
   
/*******************************************************************************
 * The test mode for reading the file:<br>
 * 1: Throw an IOException when reading the file.<br>
 * 2: Throw an IOException when closing the file.<br>
 * Anything else: Take no test action.
 *******************************************************************************
 */
   private final int myTestMode;
   
/*******************************************************************************
 * The BufferedReader for reading the file.
 *******************************************************************************
 */
   private BufferedReader myBuffReader;
      
/*******************************************************************************
 * The delimiter, compiled into a Pattern.
 *******************************************************************************
 */
   private final Pattern myDelimPattern;
      
////////////////////////////////////////////////////////////////////////////////
// Private Non-Static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Opens the BufferedReader for reading the file.
 *******************************************************************************
 */
   private void openBufferedReader ()
      {
      FileReader theFileReader;
      
      try
         {
         theFileReader = new FileReader (myFileName);
         }
      
      catch (FileNotFoundException theFNFExc)
         {
         throw
            new FileFormatException (
               "\n\n"
             + "   Input data file not found.\n"
             + "\n"
             + "      File Name: " + myFileName + "\n");
         }
      
      myBuffReader = new BufferedReader (theFileReader);
      }

/*******************************************************************************
 * Closes the BufferedReader for the DSV file.
 *******************************************************************************
 */
   private void closeBufferedReader ()
      {
      try
         {
         if (myTestMode == 2)
            {
            throw new IOException ("(Artificial error for test purposes)");
            }
      
         myBuffReader.close ();
         }
      
      catch (IOException theIOExc)
         {
         FileFormatException theFFExc;
         
         theFFExc =
            new FileFormatException (
               "\n\n"
             + "   An I/O error occurred when closing an input data file.\n"
             + "\n"
             + "      File Name: " + myFileName             + "\n"
             + "      I/O Error: " + theIOExc.getMessage () + "\n");
         
         theFFExc.initCause (theIOExc);
                  
         throw theFFExc;
         }
      }

/*******************************************************************************
 * Reads and returns the next line from the DSV file.
 *******************************************************************************
 */
   private String readLine ()
      {
      try
         {
         return myBuffReader.readLine ();
         }
      
      catch (IOException theIOExc)
         {
         FileFormatException theFFExc;
         
         try
            {
            myBuffReader.close ();
            }
         
         catch (IOException theIOExc2)
            {
            }
         
         theFFExc =
            new FileFormatException (
               "\n\n"
             + "   An I/O error occurred when reading an input data file.\n"
             + "\n"
             + "      File Name: " + myFileName             + "\n"
             + "      I/O Error: " + theIOExc.getMessage () + "\n");
         
         theFFExc.initCause (theIOExc);
                  
         throw theFFExc;
         }
      }

////////////////////////////////////////////////////////////////////////////////
// Package-Internal Constructor
////////////////////////////////////////////////////////////////////////////////

   DsvFileReader (String theFileName, String theDelim, int theTestMode)
      {
      myFileName     = theFileName;
      myTestMode     = theTestMode;
      myBuffReader   = null;

      myDelimPattern = Pattern.compile (theDelim, Pattern.LITERAL);
      }

////////////////////////////////////////////////////////////////////////////////
// Package-Internal Non-Static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Reads the DSV file.
 * 
 * @return The cells given the DSV file, one array of cells for each line in the
 *         file.
 *******************************************************************************
 */
   List <String[]> readFile ()
      {
      ArrayList <String[]> theCellArrList;
      String               theLine;
      String[]             theCellArr;
      
      openBufferedReader ();

      if (myTestMode == 1)
         closeBufferedReader ();

      theCellArrList = new ArrayList <String[]> ();
      
      while (true)
         {
         theLine = readLine ();

         if (theLine == null)
            break;

         theCellArr = myDelimPattern.split (theLine, -1);

         theCellArrList.add (theCellArr);
         }

      closeBufferedReader ();

      return theCellArrList;
      }

////////////////////////////////////////////////////////////////////////////////
}
