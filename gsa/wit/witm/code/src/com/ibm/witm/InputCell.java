
////////////////////////////////////////////////////////////////////////////////
// WIT-M Source File InputCell.java.
////////////////////////////////////////////////////////////////////////////////

package com.ibm.witm;

import static com.ibm.witm.Utility.concatenation;
import static com.ibm.witm.Utility.curEmb;
import static com.ibm.witm.Utility.reqCurEmb;

/*******************************************************************************
 * An InputCell represents a cell in an InputTable.<br>
 * Immutable class
 * @see InputTable
 * @see InputRow
 *******************************************************************************
 */
public final class InputCell
{
////////////////////////////////////////////////////////////////////////////////
// Private Non-Static Fields
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * The InputRow to which this InputCell belongs.
 *******************************************************************************
 */
   private final InputRow myRow;

/*******************************************************************************
 * The column index for the cell represented by this InputCell.
 *******************************************************************************
 */
   private final int myColIndex;

/*******************************************************************************
 * The content of the cell represented by this InputCell.
 *******************************************************************************
 */
   private final String myContent;

////////////////////////////////////////////////////////////////////////////////
// Private Non-Static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * @return The name of the file from which this InputCell was read.
 *******************************************************************************
 */
   private String getFileName ()
      {
      return myRow.getTable ().getFileName ();
      }
   
/*******************************************************************************
 * Creates and returns a FileFormatException for a cell in this InputRow.
 * 
 * @param  message The first part of the detail message for the
 *                 FileFormatException.
 * @param  colIdx  The column index of the cell.
 * @param  meaning The meaning of the cell.
 *                 This will be used in the detail message.
 * @return         The FileFormatException.
 *******************************************************************************
 */
   private FileFormatException newFileFormatException (
         String message,
         String meaning)
      {
      return
         new FileFormatException (
            "\n\n"
          + message
          + "\n"
          + "      File:          "   + getFileName ()    + "\n" 
          + "      Row #:         "   + myRow.getIndex () + "\n"
          + "      Column #:      "   + myColIndex        + "\n"
          + "      Cell Meaning:  "   + meaning           + "\n"
          + "      Cell Contents: \"" + myContent         + "\"\n");
      }

////////////////////////////////////////////////////////////////////////////////
// Package-Internal Constructor
////////////////////////////////////////////////////////////////////////////////

   InputCell (
         InputRow theRow,
         int      colIdx,
         String   theContent)
      {
      myRow      = theRow;
      myColIndex = colIdx;
      myContent  = theContent;
      }

////////////////////////////////////////////////////////////////////////////////
// Public Non-Static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Returns the content of this cell as a String (i.e., its original form).
 * 
 * @return (See above)
 *******************************************************************************
 */
   public String asString ()
      {
      return myContent;
      }
   
/*******************************************************************************
 * {@inheritDoc}
 *******************************************************************************
 */             @Override
   public String toString ()
      {
      return
         concatenation (
            "InputCell for column #",
            String.valueOf (myColIndex),
            " of ",
            myRow.toString (),
            ": \"",
            myContent,
            "\"");
      }
   
/*******************************************************************************
 * Verifies that the content of this InputCell does not match the name of any
 * existing Item of a specified ItemCategory.
 * 
 * @param theItemCat The ItemCategory
 * 
 * @throws FileFormatException If the content of this InputCell matches the name
 *                             of an exiting Item of theItemCat in the current
 *                             Embodiment.
 *******************************************************************************
 */
   public void forbidItemNameMatch (ItemCategory theItemCat)
      {
      if (theItemCat.get (myContent) != null)
         throw
            new FileFormatException (
               "\n\n"
             + "   The content of a cell in an input data file matched the\n"
             + "   name of an existing Item of Category "
             +     theItemCat.getName () + ",\n"
             + "   where this is not allowed.\n"
             + "\n"
             + "      File:         " + getFileName ()    + "\n" 
             + "      Row #:        " + myRow.getIndex () + "\n"
             + "      Column #:     " + myColIndex        + "\n"
             + "      Cell Content: " + myContent         + "\n");
      }

/*******************************************************************************
 * Returns The content of this InputCell, converted to an int.
 * 
 * @param  meaning             The meaning of this InputCell, to be used if a
 *                             FileFormatException is thrown.
 * @return                     The content, as an int.
 *
 * @throws FileFormatException If the content cannot be converted to an int.
 *******************************************************************************
 */
   public int asInt (String meaning)
      {
      int theInt;
      
      try
         {
         theInt = Integer.parseInt (myContent);
         }
      
      catch (NumberFormatException theExc)
         {
         throw
            newFileFormatException (
               "   A cell (in an input data file) that is to hold an integer\n"
             + "   value was not in the required format.\n",
               meaning);
         }
      
      return theInt;
      }

/*******************************************************************************
 * Returns The content of this InputCell, converted to a double.
 * 
 * @param  meaning             The meaning of this InputCell, to be used if a
 *                             FileFormatException is thrown.
 * @return                     The content, as a double.
 *
 * @throws FileFormatException If the content cannot be converted to a double.
 *******************************************************************************
 */
   public double asDouble (String meaning)
      {
      double theDbl;
      
      try
         {
         theDbl = Double.parseDouble (myContent);
         }
      
      catch (NumberFormatException theExc)
         {
         throw
            newFileFormatException (
               "   A cell (in an input data file) that is to hold a double\n"
             + "   precision numerical value was not in the required format.\n",
               meaning);
         }
      
      return theDbl;
      }

/*******************************************************************************
 * Returns The content of this InputCell, converted to an Item.
 * 
 * @param  theItemCat                The ItemCategory to which the 
 *                                   Item is to belong.
 * @return                           The Item of the current Embodiment
 *                                   that belongs to theItemCat and whose name
 *                                   matches the content of this InputCell.
 *
 * @throws IllegalStateException     If the current Embodiment is null.
 * @throws IllegalArgumentException  If the Scheme of theItemCat does not match
 *                                   the Scheme of the current Embodiment.
 * @throws FileFormatException       If the content of this InputCell does not
 *                                   match the name of an Item in the current
 *                                   Embodiment that belongs to theItemCat.
 *******************************************************************************
 */
   public Item asItem (ItemCategory theItemCat)
      {
      Item theItem;
      
      reqCurEmb                         ();
      theItemCat.reqSchemeMatchesCurEmb ();
      
      theItem = curEmb ().getItemMgr (theItemCat).getItem (myContent);
      
      if (theItem == null)
         throw
            newFileFormatException (
               "   A cell (in an input data file) that is to hold\n"
             + "   the name of an Item of Category "
             +     theItemCat.getName () + "\n"
             + "   does not match the name of any Item of Category "
             +     theItemCat.getName () + ".\n",
               theItemCat.getName ());
      
      return theItem;
      }

////////////////////////////////////////////////////////////////////////////////
}
