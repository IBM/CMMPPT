
////////////////////////////////////////////////////////////////////////////////
// WIT-M Source File InputRow.java.
////////////////////////////////////////////////////////////////////////////////

package com.ibm.witm;

import static com.ibm.witm.Utility.*;

/*******************************************************************************
 * An InputRow represents a row in an InputTable.<br>
 * Immutable class
 * @see InputTable
 *******************************************************************************
 */
public final class InputRow
{
////////////////////////////////////////////////////////////////////////////////
// Private Non-Static Fields
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * The InputTable to which this InputRow belongs.
 *******************************************************************************
 */
   private final InputTable myTable;

/*******************************************************************************
 * The row index for the row represented by this InputRow
 *******************************************************************************
 */
   private final int myIndex;

/*******************************************************************************
 * {@code myCellArr[colIdx-1]} is the InputCell for this InputRow and
 * column #{@code colIdx}.
 *******************************************************************************
 */
   private final InputCell[] myCellArr;

////////////////////////////////////////////////////////////////////////////////
// Private Non-Static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * @return The name of the file from which this InputRow was read.
 *******************************************************************************
 */
   private String getFileName ()
      {
      return myTable.getFileName ();
      }
   
/*******************************************************************************
 * Verifies that the column index passed to getCell is within range.
 *******************************************************************************
 */
   private void banIndexOutOfBounds (int colIdx)
      {
      if ((colIdx < 1) || (colIdx > nCells ()))
         throw
            new IndexOutOfBoundsException (
               "\n\n"
             + "   Call to InputRow.getCell (colIdx) with an index that's\n"
             + "   outside of the range: 1 <= colIdx <= # of cells.\n"
             + "\n"
             + "      ColIdx:     " + colIdx         + "\n"
             + "      # of Cells: " + nCells ()      + "\n"
             + "      File:       " + getFileName () + "\n" 
             + "      Row #       " + myIndex        + "\n");
      }

////////////////////////////////////////////////////////////////////////////////
// Package-Internal Constructor
////////////////////////////////////////////////////////////////////////////////

   InputRow (InputTable theTable, int theIdx, String[] theCellArr)
      {
      int colIdx;
      
      myTable   = theTable;
   
      myIndex   = theIdx;
   
      myCellArr = new InputCell[theCellArr.length];
      
      for (colIdx = 1; colIdx <= theCellArr.length; colIdx ++)
         myCellArr[colIdx - 1] =
            new InputCell (this, colIdx, theCellArr[colIdx - 1]);
      }

////////////////////////////////////////////////////////////////////////////////
// Package Internal Non-Static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * @return The InputTable to which this InputRow belongs.
 *******************************************************************************
 */
   InputTable getTable ()
      {
      return myTable;
      }
   
////////////////////////////////////////////////////////////////////////////////
// Public Non-Static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Accessor.
 * 
 * @return The row index for the row represented by this InputRow
 *******************************************************************************
 */ 
   public int getIndex ()
      {
      return myIndex;
      }

/*******************************************************************************
 * @return The # of Cells in the InputRow. 
 *******************************************************************************
 */ 
   public int nCells ()
      {
      return myCellArr.length;
      }

/*******************************************************************************
 * {@inheritDoc}
 *******************************************************************************
 */             @Override
   public String toString ()
      {
      return
         concatenation (
            "InputRow #",
            String.valueOf (myIndex),
            " of ",
            myTable.toString ());
      }
   
/*******************************************************************************
 * Verifies that this InputRow contains the required number of cells.
 * 
 * @param  nCellsReq           The required # of cells.
 *
 * @throws FileFormatException If the # of cells in this InputRow does not match
 *                             nCellsReq. 
 *******************************************************************************
 */ 
   public void requireNCells (int nCellsReq)
      {
      if (nCells () != nCellsReq)
         throw
            new FileFormatException (
               "\n\n"
             + "   A row in an input data file contained the wrong number of\n"
             + "   cells.\n"
             + "\n"
             + "      File:             " + getFileName () + "\n" 
             + "      Row #:            " + myIndex        + "\n"
             + "      # Cells in Row:   " + nCells ()      + "\n"
             + "      # Cells Required: " + nCellsReq      + "\n");
      }

/*******************************************************************************
 * Returns The InputCell of this InputRow for a column index.
 * 
 * @param  colIdx                    The column index
 * @return                           The InputCell
 *
 * @throws IndexOutOfBoundsException If colIdx < 1 or colIdx > nCells ()
 *******************************************************************************
 */ 
   public InputCell getCell (int colIdx)
      {
      banIndexOutOfBounds (colIdx);
      
      return myCellArr[colIdx - 1];
      }

////////////////////////////////////////////////////////////////////////////////
}
