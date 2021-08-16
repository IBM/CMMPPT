
////////////////////////////////////////////////////////////////////////////////
// WIT-M/Test Source File ErrorTester.java.
////////////////////////////////////////////////////////////////////////////////

package com.ibm.witm.test;

import com.ibm.witj.Part;
import com.ibm.witm.InputRow;
import com.ibm.witm.InputTable;
import com.ibm.witm.Embodiment;
import com.ibm.witm.Item;
import com.ibm.witm.ItemCategory;
import com.ibm.witm.Property;
import com.ibm.witm.Scheme;
import com.ibm.witm.Tuple;
import com.ibm.witm.TupleCategory;

import static com.ibm.witm.Scheme.*;

import static com.ibm.witj.Part.Category.*;

/*******************************************************************************
 * Responsible for performing a series of tests on WIT-M each of which
 * generates an error condition that causes an Exception to be thrown.
 *******************************************************************************
 */
final class ErrorTester
{
////////////////////////////////////////////////////////////////////////////////
// Private Non-Static Fields
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * The index of the error test cased to be performed.
 *******************************************************************************
 */
   private final int curCaseIdx;
   
/*******************************************************************************
 * The main Scheme to be used for an error test.
 *******************************************************************************
 */
   private final Scheme ERROR_TEST;

////////////////////////////////////////////////////////////////////////////////
// Private Constructor
////////////////////////////////////////////////////////////////////////////////

   private ErrorTester (int theCaseIdx)
      {
      curCaseIdx = theCaseIdx;
      
      ERROR_TEST = newScheme ("ERROR_TEST");
      }

////////////////////////////////////////////////////////////////////////////////
// Private Non-Static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Prints a heading for the error test case indexed by theCaseIdx.
 * 
 * @param text Text describing the error test case.
 *******************************************************************************
 */
   private void printHeading (String text)
      {
      System.out.printf (
         "\n"
       + "----------------------------------------"
       + "----------------------------------------\n"
       + "WIT-M Error Test Case %d:\n"
       + "\n"
       + "%s\n"
       + "----------------------------------------"
       + "----------------------------------------\n"
       + "\n",
         curCaseIdx,
         text);
      }

/*******************************************************************************
 * Creates the Embodiment for the error test.
 *******************************************************************************
 */
   private void makeEmbodiment ()
      {
      setCurrentScheme (ERROR_TEST);
      
      freezeCurrentScheme ();
      
      ERROR_TEST.newEmbodiment ("Test Case #" + curCaseIdx);
      }

/*******************************************************************************
 * Performs error test case 1.
 *******************************************************************************
 */
   private void testCase1 ()
      {
      printHeading ("Passing a null argument to newScheme (*, *, *).");

      newScheme ("SCHEME1", null, "PERIOD");
      }

/*******************************************************************************
 * Performs error test case 2.
 *******************************************************************************
 */
   private void testCase2 ()
      {
      printHeading ("Creating an ItemCategory with a duplicate name.");

      newItemCategory ("A");
      newItemCategory ("A");
      }

/*******************************************************************************
 * Performs error test case 3.
 *******************************************************************************
 */
   private void testCase3 ()
      {
      ItemCategory A;

      printHeading (
         "Call to ItemCategory.newItem (...) with a Scheme mismatch.");
      
      newScheme ("SCHEME_B");
      
      A = newItemCategory ("A");
      
      makeEmbodiment ();
      
      A.newItem ("1");
      }
   
/*******************************************************************************
 * Performs error test case 4.
 *******************************************************************************
 */
   private void testCase4 ()
      {
      printHeading (
         "Call to newItemCategory when there is no current Scheme.");
      
      setCurrentScheme (null);
      
      newItemCategory ("A");
      }

/*******************************************************************************
 * Performs error test case 5.
 *******************************************************************************
 */
   private void testCase5 ()
      {
      printHeading (
         "Call to Scheme.newEmbodiment (...) on an unfrozen Scheme.");

      ERROR_TEST.newEmbodiment ("Test Case #6");
      }

/*******************************************************************************
 * Performs error test case 6.
 *******************************************************************************
 */
   private void testCase6 ()
      {
      ItemCategory A;

      printHeading (
         "Call to ItemCategory.newItem (...) with a name collision.");
      
      A = newItemCategory ("A");
      
      makeEmbodiment ();
      
      A.newItem ("A1");
      A.newItem ("A1");
      }

/*******************************************************************************
 * Performs error test case 7.
 *******************************************************************************
 */
   private void testCase7 ()
      {
      ItemCategory A;

      printHeading (
         "Call to a modifying method on the result of\n"
       + "EntityCategory.getAll ().");
      
      A = newItemCategory ("A");
      
      makeEmbodiment ();
      
      A.getAll ().clear ();
      }

/*******************************************************************************
 * Performs error test case 8.
 *******************************************************************************
 */
   private void testCase8 ()
      {
      ItemCategory      A; 
      ItemCategory      B; 
      Property <String> C;
      Item              theB;
      
      printHeading (
         "Call to theEnt.set (...) with an EntityCategory mismatch.");
      
      A = newItemCategory ("A");
      B = newItemCategory ("B");
      
      C = A.newProperty ("C", String.class);
      
      makeEmbodiment ();
      
      theB = B.newItem ("1");
      
      theB.set (C, "Text");
      }

/*******************************************************************************
 * Performs error test case 9.
 *******************************************************************************
 */
   private void testCase9 ()
      {
      ItemCategory      A; 
      ItemCategory      B; 
      Property <String> C;
      Item              theB;
      
      printHeading (
         "Call to theEnt.get (...) with an EntityCategory mismatch.");
      
      A = newItemCategory ("A");
      B = newItemCategory ("B");
      
      C = A.newProperty ("C", String.class);
      
      makeEmbodiment ();
      
      theB = B.newItem ("1");
      
      theB.get (C);
      }

/*******************************************************************************
 * Performs error test case 10.
 *******************************************************************************
 */
   private void testCase10 ()
      {
      printHeading (
         "Call to newTupleCategory with a null EntityCategory reference.");
      
      newTupleCategory ("A", null, null);
      }

/*******************************************************************************
 * Performs error test case 11.
 *******************************************************************************
 */
   private void testCase11 ()
      {
      printHeading ("Call to newTupleCategory with no Categories.");
      
      newTupleCategory ("A");
      }

/*******************************************************************************
 * Performs error test case 12.
 *******************************************************************************
 */
   private void testCase12 ()
      {
      ItemCategory A;
      ItemCategory B;

      printHeading ("Call to newTupleCategory with a Scheme mismatch.");
      
      A = newItemCategory ("A");
      
      newScheme ("C");
      
      B = newItemCategory ("B");
      
      newTupleCategory ("D", A, B);
      }

/*******************************************************************************
 * Performs error test case 13.
 *******************************************************************************
 */
   private void testCase13 ()
      {
      ItemCategory A;
      
      printHeading ("Call to newTupleCategory with a duplicate name.");
      
      A = newItemCategory ("A");
      
      newTupleCategory ("A", A, A);
      }

/*******************************************************************************
 * Performs error test case 14.
 *******************************************************************************
 */
   private void testCase14 ()
      {
      ItemCategory  A;
      TupleCategory B;
      Item          theA;
      Tuple         theB;
      
      printHeading (
         "Call to TupleCategory.get (Entity...) with a coordinate of the\n"
       + "wrong EntityCategory.");
      
      A = newItemCategory  ("A");
      B = newTupleCategory ("B", A);
      
      makeEmbodiment ();
      
      theA = A.newItem  ("1");
      theB = B.newTuple (theA);
      
      B.get (theB);
      }

/*******************************************************************************
 * Performs error test case 15.
 *******************************************************************************
 */
   private void testCase15 ()
      {
      ItemCategory  A;
      TupleCategory B;
      Item          theA2;
      Item          theA1;
      
      printHeading (
         "Call to newTuple with a coordinate from a non-current Embodiment.");
      
      A = newItemCategory  ("A");
      B = newTupleCategory ("B", A, A);
      
      makeEmbodiment ();
      
      theA2 = A.newItem ("2");
      
      ERROR_TEST.newEmbodiment ("theEmb_17_B");
      
      theA1 = A.newItem ("1");
      
      B.newTuple (theA1, theA2);
      }

/*******************************************************************************
 * Performs error test case 16.
 *******************************************************************************
 */
   private void testCase16 ()
      {
      ItemCategory A;
      
      printHeading (
         "Call to Embodiment.newItem (...) when the current Embodiment is\n"
       + "null.");
      
      A = newItemCategory ("A");
      
      A.newItem ("1");
      }

/*******************************************************************************
 * Performs error test case 17.
 *******************************************************************************
 */
   private void testCase17 ()
      {
      ItemCategory  A;
      TupleCategory B;
      Item          theA;
      
      printHeading (
         "Call to getSlice, where the number of coordinates in the pattern\n"
       + "does not match the number of dimensions in the TupleCategory.");
      
      A = newItemCategory  ("A");
      B = newTupleCategory ("B", A, A);
      
      makeEmbodiment ();
      
      theA = A.newItem ("1");

      B.getSlice (theA);
      }

/*******************************************************************************
 * Performs error test case 18.
 *******************************************************************************
 */
   private void testCase18 ()
      {
      printHeading ("Call to setCurrentScheme with a frozen Scheme.");
      
      freezeCurrentScheme ();

      setCurrentScheme (ERROR_TEST);
      }

/*******************************************************************************
 * Performs error test case 19.
 *******************************************************************************
 */
   private void testCase19 ()
      {
      ItemCategory  A;
      TupleCategory B;
      Item          theA;
      Tuple         theB;
      ItemCategory  C;
      
      printHeading (
         "Call to Tuple.get (EntityCategory <E>) with an EntityCategory and\n"
       + "Tuple whose Schemes do not match.");
      
      A = newItemCategory  ("A");
      B = newTupleCategory ("B", A);
      
      makeEmbodiment ();
      
      theA = A.newItem  ("1");
      theB = B.newTuple (theA);
      
      newScheme ("SCHEME19");
      
      C = newItemCategory ("C");
      
      theB.get (C);
      }

/*******************************************************************************
 * Performs error test case 20.
 *******************************************************************************
 */
   private void testCase20 ()
      {
      ItemCategory  A;
      ItemCategory  B;
      TupleCategory C;
      Item          theA;
      Tuple         theC;
      
      printHeading (
         "Call to Tuple.get (EntityCategory <E>) with an EntityCategory that\n"
       + "is not a dimension of the TupleCategory of the Tuple.");
      
      A = newItemCategory  ("A");
      B = newItemCategory  ("B");
      C = newTupleCategory ("C", A, A);
      
      makeEmbodiment ();
      
      theA = A.newItem  ("1");
      theC = C.newTuple (theA, theA);
      
      theC.get (B);
      }

/*******************************************************************************
 * Performs error test case 21.
 *******************************************************************************
 */
   private void testCase21 ()
      {
      ItemCategory  A;
      TupleCategory B;
      Item          theA;
      Tuple         theB;
      
      printHeading (
         "Call to Tuple.get (EntityCategory <E>) with an EntityCategory that\n"
       + "occurs more than once as a dimension of the TupleCategory of the\n"
       + "Tuple.\n");
      
      A = newItemCategory  ("A");
      B = newTupleCategory ("B", A, A);
      
      makeEmbodiment ();
      
      theA = A.newItem  ("1");
      theB = B.newTuple (theA, theA);
      
      theB.get (A);
      }

/*******************************************************************************
 * Performs error test case 22.
 *******************************************************************************
 */
   private void testCase22 ()
      {
      ItemCategory  A;
      TupleCategory B;
      Item          theA;
      
      printHeading (
         "Call to TupleCategory.newTuple (Entity...) with a coordinate\n"
       + "list that duplicates that of an existing Tuple.");
      
      A = newItemCategory  ("A");
      B = newTupleCategory ("B", A, A);
      
      makeEmbodiment ();
      
      theA = A.newItem ("1");
      
      B.newTuple (theA, theA);
      B.newTuple (theA, theA);
      }

/*******************************************************************************
 * Performs error test case 23.
 *******************************************************************************
 */
   private void testCase23 ()
      {
      ItemCategory theGlobalCat;
      
      printHeading (
         "Call to ItemCategory.newItem (...) on the Global Category.");
      
      theGlobalCat = currentGlobalCategory ();
      
      makeEmbodiment ();

      theGlobalCat.newItem ("My Global");
      }
   
/*******************************************************************************
 * Performs error test case 24.
 *******************************************************************************
 */
   private void testCase24 ()
      {
      ItemCategory    A;
      Property <Part> A_CAP;
      Item            theA;
      
      printHeading (
         "Call to Entity.newPart (...) on an Entity whose Embodiment is not\n"
       + "associated with a WIT-J Problem.");
      
      A     = newItemCategory ("A");
      
      A_CAP = A.newProperty ("A_CAP", Part.class);
      
      makeEmbodiment ();
      
      theA = A.newItem ("1");
      
      theA.newPart (A_CAP, CAPACITY);
      }

/*******************************************************************************
 * Performs error test case 25.
 *******************************************************************************
 */
   private void testCase25 ()
      {
      ItemCategory A;
      
      printHeading (
         "Call to EntityCategory.newProperty on an EntityCategory whose\n"
       + "Scheme is frozen.");
      
      A = newItemCategory ("A");
      
      makeEmbodiment ();
      
      A.newProperty ("INT_PROP", Integer.class);
      }

/*******************************************************************************
 * Performs error test case 26.
 *******************************************************************************
 */
   private void testCase26 ()
      {
      ItemCategory  A;
      ItemCategory  B;
      TupleCategory C;
      Item          theA;
      Item          theB;
      Tuple         theC;
      
      printHeading (
         "Call to Tuple.get (theEntCat, dimIdx), where dimIdx is out of\n"
       + "range.");
      
      A = newItemCategory  ("A");
      B = newItemCategory  ("B");
      C = newTupleCategory ("C", A, B);
      
      makeEmbodiment ();
      
      theA = A.newItem  ("1");
      theB = B.newItem  ("2");
      theC = C.newTuple (theA, theB);
      
      theC.get (A, 2);
      }

/*******************************************************************************
 * Performs error test case 27.
 *******************************************************************************
 */
   private void testCase27 ()
      {
      ItemCategory  A;
      ItemCategory  B;
      TupleCategory C;
      Item          theA;
      Item          theB;
      Tuple         theC;
      
      printHeading (
         "Call to Tuple.get (theEntCat, dimIdx), where theEntCat does not\n"
       + "match the indexed dimension.");
      
      A = newItemCategory  ("A");
      B = newItemCategory  ("B");
      C = newTupleCategory ("C", A, B);
      
      makeEmbodiment ();
      
      theA = A.newItem  ("1");
      theB = B.newItem  ("2");
      theC = C.newTuple (theA, theB);
      
      theC.get (A, 1);
      }

/*******************************************************************************
 * Performs error test case 28.
 *******************************************************************************
 */
   private void testCase28 ()
      {
      printHeading (
         "Call to InputTable.newInputTable (...) with a file name for which\n"
       + "there is no such file.");

      InputTable.newInputTable ("No-Such.csv", ",");
      }

/*******************************************************************************
 * Performs error test case 29.
 *******************************************************************************
 */
   private void testCase29 ()
      {
      printHeading ("I/O exception while reading the file for an InputTable.");
      
      InputTable.newInputTableForTest ("POK/period.csv", ",", 1);
      }

/*******************************************************************************
 * Performs error test case 30.
 *******************************************************************************
 */
   private void testCase30 ()
      {
      printHeading ("I/O exception while closing the file for an InputTable.");
      
      InputTable.newInputTableForTest ("POK/skillgroup.csv", ",", 2);
      }

/*******************************************************************************
 * Performs error test case 31.
 *******************************************************************************
 */
   private void testCase31 ()
      {
      InputTable theTable;
      
      printHeading (
         "Call to theInputRow.requireNCells (...) on an InputRow with the\n"
       + "wrong # of cells.");
      
      theTable = InputTable.newInputTable ("POK/period.csv", ",");
      
      theTable.getRow (7).requireNCells (17);
      }

/*******************************************************************************
 * Performs error test case 32.
 *******************************************************************************
 */
   private void testCase32 ()
      {
      InputTable theTable;
      
      printHeading (
         "Call to theInputRow.getInputCell (...) with a column index that's \n"
       + "out of bounds.");
      
      theTable = InputTable.newInputTable ("POK/period.csv", ",");
      
      theTable.getRow (3).getCell (5);
      }

/*******************************************************************************
 * Performs error test case 33.
 *******************************************************************************
 */
   private void testCase33 ()
      {
      InputTable theTable;
      
      printHeading (
         "Call to theInputCell.getDouble (...) on an InputCell whose content\n"
       + "cannot be converted to a double.");
      
      theTable = InputTable.newInputTable ("POK/period.csv", ",");
      
      theTable.getRow (3).getCell (1).asDouble ("Max Workers");
      }

/*******************************************************************************
 * Performs error test case 34.
 *******************************************************************************
 */
   private void testCase34 ()
      {
      InputTable theTable;
      
      printHeading (
         "Call to theInputCell.getInt (...) on an InputCell whose content\n"
       + "cannot be converted to an int.");
      
      theTable = InputTable.newInputTable ("POK/skillgroup.csv", ",");
      
      theTable.getRow (8).getCell (2).asInt ("Shift");
      }

/*******************************************************************************
 * Performs error test case 35.
 *******************************************************************************
 */
   private void testCase35 ()
      {
      InputTable theTable;
      
      printHeading (
         "Call to theInputTable.getInputRow (...) with a row index that's out\n"
       + "of bounds.");
      
      theTable = InputTable.newInputTable ("POK/area-shift-period.csv", ",");
      
      theTable.getRow (200);
      }

/*******************************************************************************
 * Performs error test case 36.
 *******************************************************************************
 */
   private void testCase36 ()
      {
      ItemCategory AREA;
      InputTable   theTable;
      
      printHeading (
         "Call to theInputCell.asItem (...) on a cell whose content does not\n"
       + "match the name of an Item of the specified ItemCategory.");
      
      AREA = newItemCategory ("AREA");
      
      makeEmbodiment ();
      
      theTable = InputTable.newInputTable ("POK/area-shift-period.csv", ",");
      
      theTable.getRow (7).getCell (1).asItem (AREA);
      }

/*******************************************************************************
 * Performs error test case 37.
 *******************************************************************************
 */
   private void testCase37 ()
      {
      ItemCategory      A;
      Property <Double> A_DBL;
      Item              theA;
      double[]          theArr;
      
      printHeading (
         "Call to Entity.setByPeriod (...) with a Property whose\n"
       + "EntityCategory is not a TupleCategory.");

      A      = newItemCategory ("A");
      
      A_DBL  = A.newProperty ("A_DBL", Double.class);
      
      makeEmbodiment ();
      
      theA   = A.newItem ("1");

      theArr = new double[3];
      
      theA.setByPeriod (A_DBL, theArr);
      }

/*******************************************************************************
 * Performs error test case 38.
 *******************************************************************************
 */
   private void testCase38 ()
      {
      ItemCategory      A;
      TupleCategory     B;
      Property <Double> B_DBL;
      Item              theA;
      double[]          theArr;
      
      printHeading (
         "Call to Entity.getByPeriod (...) with a Property of a TupleCategory\n"
       + "with only one dimension.");

      A      = newItemCategory  ("A");
      B      = newTupleCategory ("B", A);
      
      B_DBL  = B.newProperty ("B_DBL", Double.class);
      
      makeEmbodiment ();
      
      theA   = A.newItem ("1");
      
      theArr = new double[3];
      
      theA.getByPeriod (B_DBL, theArr, 1.0);
      }

/*******************************************************************************
 * Performs error test case 39.
 *******************************************************************************
 */
   private void testCase39 ()
      {
      ItemCategory       A;
      ItemCategory       B;
      TupleCategory      C;
      Property <Integer> C_INT;
      Item               theB;
      int[]              theArr;
      
      printHeading (
         "Call to Entity.setByPeriod (...) with a Property of a TupleCategory\n"
       + "whose dimension #0 is not the EntityCategory of the Entity.");

      A      = newItemCategory  ("A");
      B      = newItemCategory  ("B");
      C      = newTupleCategory ("C", A, B);
      
      C_INT  = C.newProperty ("C_INT", Integer.class);
      
      makeEmbodiment ();
      
      theB   = B.newItem ("1");

      theArr = new int[1];
      
      theB.setByPeriod (C_INT, theArr);
      }

/*******************************************************************************
 * Performs error test case 40.
 *******************************************************************************
 */
   private void testCase40 ()
      {
      ItemCategory       PERIOD;
      TupleCategory      B;
      Property <Integer> B_INT;
      Item               theA;
      int[]              theArr;
      
      printHeading (
         "Call to Entity.getByPeriod (...) with an array whose length does\n"
       + "not match the number of Periods in the Embodiment.");

      PERIOD = currentPeriodCategory ();
      
      B      = newTupleCategory ("B", PERIOD, PERIOD);
      
      B_INT  = B.newProperty ("B_INT", Integer.class);
      
      makeEmbodiment ();
      
      theA   = PERIOD.newItem ("1");

      theArr = new int[2];
      
      theA.getByPeriod (B_INT, theArr, 2);
      }

/*******************************************************************************
 * Performs error test case 41.
 *******************************************************************************
 */
   private void testCase41 ()
      {
      ItemCategory       A;
      TupleCategory      B;
      Property <Boolean> B_BOOL;
      Item               theA;
      boolean[]          theArr;
      
      printHeading (
         "Call to Entity.getByPeriod (...) with a Property of a TupleCategory\n"
       + "whose dimension #1 is not the Period Category.");

      A      = newItemCategory  ("A");
      B      = newTupleCategory ("B", A, A);
      
      B_BOOL = B.newProperty ("B_BOOL", Boolean.class);
      
      makeEmbodiment ();
      
      theA   = A.newItem ("1");

      theArr = new boolean[1];
      
      theA.getByPeriod (B_BOOL, theArr, true);
      }

/*******************************************************************************
 * Performs error test case 42.
 *******************************************************************************
 */
   private void testCase42 ()
      {
      ItemCategory A;
      Item         theA;

      printHeading (
         "Call to Item.getPeriodIndex () on an Item that does not belong to\n"
       + "the Period Category.");

      A    = newItemCategory ("A");
      
      makeEmbodiment ();
      
      theA = A.newItem ("A0");
      
      theA.getPeriodIndex ();
      }

/*******************************************************************************
 * Performs error test case 43.
 *******************************************************************************
 */
   private void testCase43 ()
      {
      ItemCategory AREA;
      InputTable   theTable;
      
      printHeading (
         "Call to theInputCell.forbidItemNameMatch (...) on an InputCell\n"
       + "whose content matches the name of an Item of the specified Category."
         );
      
      AREA = newItemCategory ("AREA");
      
      makeEmbodiment ();
      
      AREA.newItem ("LPAT_Pack_Ship");
      
      theTable = InputTable.newInputTable ("POK/area.csv", ",");
      
      for (InputRow theRow: theTable.getRows ())
         {
         if (theRow.getIndex () == 1)
            continue;
         
         theRow.getCell (1).forbidItemNameMatch (AREA);
         }
      }

/*******************************************************************************
 * Performs error test case 44.
 *******************************************************************************
 */
   private void testCase44 ()
      {
      printHeading (
         "Call to Scheme.newScheme (*, *, *) with duplicate names.");

      newScheme ("TEST44", "GLOBAL", "GLOBAL");
      }

/*******************************************************************************
 * Performs the error test indexed by curCaseIdx.
 *******************************************************************************
 */
   private void innerTestCurCase ()
      {
      switch (curCaseIdx)
         {
         case 1:
            testCase1 ();
            break;

         case 2:
            testCase2 ();
            break;

         case 3:
            testCase3 ();
            break;

         case 4:
            testCase4 ();
            break;

         case 5:
            testCase5 ();
            break;

         case 6:
            testCase6 ();
            break;

         case 7:
            testCase7 ();
            break;

         case 8:
            testCase8 ();
            break;

         case 9:
            testCase9 ();
            break;

         case 10:
            testCase10 ();
            break;

         case 11:
            testCase11 ();
            break;

         case 12:
            testCase12 ();
            break;

         case 13:
            testCase13 ();
            break;

         case 14:
            testCase14 ();
            break;

         case 15:
            testCase15 ();
            break;

         case 16:
            testCase16 ();
            break;

         case 17:
            testCase17 ();
            break;

         case 18:
            testCase18 ();
            break;

         case 19:
            testCase19 ();
            break;

         case 20:
            testCase20 ();
            break;

         case 21:
            testCase21 ();
            break;

         case 22:
            testCase22 ();
            break;

         case 23:
            testCase23 ();
            break;

         case 24:
            testCase24 ();
            break;

         case 25:
            testCase25 ();
            break;

         case 26:
            testCase26 ();
            break;

         case 27:
            testCase27 ();
            break;

         case 28:
            testCase28 ();
            break;

         case 29:
            testCase29 ();
            break;

         case 30:
            testCase30 ();
            break;

         case 31:
            testCase31 ();
            break;

         case 32:
            testCase32 ();
            break;

         case 33:
            testCase33 ();
            break;

         case 34:
            testCase34 ();
            break;

         case 35:
            testCase35 ();
            break;

         case 36:
            testCase36 ();
            break;

         case 37:
            testCase37 ();
            break;

         case 38:
            testCase38 ();
            break;

         case 39:
            testCase39 ();
            break;

         case 40:
            testCase40 ();
            break;

         case 41:
            testCase41 ();
            break;

         case 42:
            testCase42 ();
            break;

         case 43:
            testCase43 ();
            break;

         case 44:
            testCase44 ();
            break;

         default:
            throw new AssertionError ();
         }
      }

/*******************************************************************************
 * Performs the error test indexed by curCaseIdx.
 *******************************************************************************
 */
   private void testCurCase ()
      {
      try
         {
         innerTestCurCase ();
 
         throw
            new AssertionError (
               "\n\n"
             + "   Test Case " + curCaseIdx + " completed without throwing "
             +    "an exception.\n");
         }

      catch (Exception theExc)
         {
         System.out.flush ();
         
         theExc.printStackTrace ();
                  
         System.out.flush ();
         }
      
      Embodiment.setCurrentEmbodiment (null);
      
      System.out.printf (
         "\n"
       + "Error Test Case %d now concludes.\n",
         curCaseIdx);
      }

////////////////////////////////////////////////////////////////////////////////
// Package-Internal Static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Performs an error test.
 *******************************************************************************
 */
   static void performTest ()
      {
      int         theCaseIdx;
      ErrorTester theErrTester;
      
      for (theCaseIdx = 1; theCaseIdx <= 44; theCaseIdx ++)
         {
         theErrTester = new ErrorTester (theCaseIdx);
      
         theErrTester.testCurCase ();
         }

      System.out.println (
         "\n"
       + "Normal termination of error test.");
      }

////////////////////////////////////////////////////////////////////////////////
}
