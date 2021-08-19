
////////////////////////////////////////////////////////////////////////////////
// WIT-M Source File InputTable.java.
////////////////////////////////////////////////////////////////////////////////

package com.ibm.witm;

import static com.ibm.witm.Utility.*;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/*******************************************************************************
 * An InputTable represents a table of Strings that was read in from a "DSV"
 * file, i.e., a file a values separated by occurrences of a specified
 * delimiter (a delimiter-separated-value file).
 *******************************************************************************
 */
public final class InputTable
{
////////////////////////////////////////////////////////////////////////////////
// Private Non-Static Fields
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * The name of the file from which this InputTable was read.
 *******************************************************************************
 */
   private final String myFileName;

/*******************************************************************************
 * The delimiter used in the DSV file.
 *******************************************************************************
 */
   private final String myDelimiter;

/*******************************************************************************
 * The list InputRows that make up this InputTable.
 *******************************************************************************
 */
   private final ArrayList <InputRow> myRows;

/*******************************************************************************
 * An unmodifiable view of the list InputRows that make up this InputTable.
 *******************************************************************************
 */
   private final List <InputRow> unmodRows;

////////////////////////////////////////////////////////////////////////////////
// Private Constructor
////////////////////////////////////////////////////////////////////////////////
   
   private InputTable (String theFileName, String theDelim)
      {
      myFileName  = theFileName;
      
      myDelimiter = theDelim;
   
      myRows      = new ArrayList <InputRow> (0);
      
      unmodRows   = Collections.unmodifiableList (myRows);
      }

////////////////////////////////////////////////////////////////////////////////
// Private Non-Static Methods
////////////////////////////////////////////////////////////////////////////////
   
/*******************************************************************************
 * Verifies that the row index passed to getInputRow is within range.
 *******************************************************************************
 */
   private void banIndexOutOfBounds (int rowIdx)
      {
      if ((rowIdx < 1) || (rowIdx > myRows.size ()))
         throw
            new IndexOutOfBoundsException (
               "\n\n"
             + "   Call to InputTable.getInputRow (rowIdx) with an index\n"
             + "   that's outside of the range 1 <= rowIdx <= # of InputRows.\n"
             + "\n"
             + "      RowIdx:         " + rowIdx         + "\n"
             + "      # of InputRows: " + myRows.size () + "\n"
             + "      File:           " + getFileName () + "\n");
      }

/*******************************************************************************
 * Builds up this InputTable: Reads the DSV file and creates the InputRows.
 *******************************************************************************
 */
   private void buildUp (int theTestMode)
      {
      DsvFileReader   theDsvFileReader;
      List <String[]> theCellArrList;
      int             theIdx;
      InputRow        theInputRow;

      theDsvFileReader =
         new DsvFileReader (myFileName, myDelimiter, theTestMode);
      
      theCellArrList   = theDsvFileReader.readFile ();
      
      theIdx           = 0;

      myRows.ensureCapacity (theCellArrList.size ());
      
      for (String[] theCellArr: theCellArrList)
         {
         theIdx ++;
         
         theInputRow = new InputRow (this, theIdx, theCellArr);

         myRows.add (theInputRow);
         }
      }

////////////////////////////////////////////////////////////////////////////////
// Package-Internal Non-Static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * @return The name of the file from which this InputTable was read.
 *******************************************************************************
 */
   String getFileName ()
      {
      return myFileName;
      }
   
////////////////////////////////////////////////////////////////////////////////
// Public Static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Factory method: Constructs a new InputTable that stores the data given in a
 * DSV file.
 * 
 * @param  theFileName          The name of the DSV file.
 * @param  theDelim             The delimiter that separates the values in the
 *                              file (normally, a single character).
 * @return                      The new InputTable
 * 
 * @throws NullPointerException If any argument is null
 * @throws IOException          If thrown when reading the file
 *******************************************************************************
 */
   public static InputTable newInputTable (String theFileName, String theDelim)
      {
      InputTable theInputTable;

      banNull (theFileName, "theFileName");
      banNull (theDelim,    "theDelim");

      theInputTable = new InputTable (theFileName, theDelim);
      
      theInputTable.buildUp (0);

      return theInputTable;
      }

/*******************************************************************************
 * Factory method: Constructs a new InputTable that stores the data given in a
 * DSV file.<br>
 * Version to be used for testing purposes.
 * 
 * @param  theFileName          The name of the DSV file.
 * @param  theDelim             The delimiter that separates the values in the
 *                              file.
 * @param  theTestMode          Test mode (see source code)
 * @return                      The new InputTable
 * 
 * @throws NullPointerException If any argument is null
 * @throws IOException          If thrown when reading the file
 *******************************************************************************
 */
   public static InputTable newInputTableForTest (
         String theFileName,
         String theDelim,
         int    theTestMode)
      {
      InputTable theInputTable;

      banNull (theFileName, "theFileName");
      banNull (theDelim,    "theDelim");

      theInputTable = new InputTable (theFileName, theDelim);
      
      theInputTable.buildUp (theTestMode);

      return theInputTable;
      }

////////////////////////////////////////////////////////////////////////////////
// Public Non-Static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * {@inheritDoc}
 *******************************************************************************
 */             @Override
   public String toString ()
      {
      return concatenation ("InputTable from file \"", myFileName, "\"");
      }
   
/*******************************************************************************
 * @return The List of InputRows for this InputTable, as an unmodifiable view of
 *         an ArrayList
 *******************************************************************************
 */
   public List <InputRow> getRows ()
      {
      return unmodRows;
      }

/*******************************************************************************
 * Returns An InputRow of this InputTable.
 * 
 * @param  rowIdx                    A row index into this InputTable.
 * @return                           The InputRow for the InputTable whose row
 *                                   index is rowIdx.
 * @throws IndexOutOfBoundsException If rowIdx < 1 or rowIdx > # InputRows in
 *                                   InputTable.
 *******************************************************************************
 */
   public InputRow getRow (int rowIdx)
      {
      banIndexOutOfBounds (rowIdx);
      
      return myRows.get (rowIdx - 1);
      }

////////////////////////////////////////////////////////////////////////////////
}
