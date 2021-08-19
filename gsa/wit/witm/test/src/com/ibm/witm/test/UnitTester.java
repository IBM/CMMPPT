
////////////////////////////////////////////////////////////////////////////////
// WIT-M/Test Source File UnitTester.java.
////////////////////////////////////////////////////////////////////////////////

package com.ibm.witm.test;

import com.ibm.witj.BomEntry;
import com.ibm.witj.BopEntry;
import com.ibm.witj.Demand;
import com.ibm.witj.Operation;
import com.ibm.witj.Part;
import com.ibm.witj.Problem;
import com.ibm.witj.Substitute;

import com.ibm.witm.InputRow;
import com.ibm.witm.InputTable;
import com.ibm.witm.Embodiment;
import com.ibm.witm.OneToOne;
import com.ibm.witm.Tuple;
import com.ibm.witm.TupleCategory;
import com.ibm.witm.Entity;
import com.ibm.witm.Item;
import com.ibm.witm.ItemCategory;
import com.ibm.witm.Property;
import com.ibm.witm.Scheme;

import static com.ibm.witm.Scheme.*;
import static com.ibm.witm.Embodiment.*;

import static com.ibm.witj.Part.Category.*;

/*******************************************************************************
 * Responsible for performing a unit test of WIT-M.
 *******************************************************************************
 */
final class UnitTester
{
////////////////////////////////////////////////////////////////////////////////
// Private Static Fields
//
// Declaration of the UNIT_TEST Scheme and its elements.
// UNIT_TEST is the main Scheme for the unit test.
////////////////////////////////////////////////////////////////////////////////

   private static final Scheme                    UNIT_TEST;
   
   private static final ItemCategory              GLOBAL;
   private static final Property <Double>         GLOBAL_DBL;
   
   private static final ItemCategory              A;
   private static final Property <Integer>        A_INT;
   private static final OneToOne <Item,Part>      A_MAT;
   private static final Property <Part>           A_CAP;
   private static final OneToOne <Item, String>   A_STR;
   
   private static final ItemCategory              B;
   private static final Property <Operation>      B_OPN;
   private static final Property <Demand>         B_DEM;
   
   private static final TupleCategory             A_B_A;
   private static final Property <String>         A_B_A_STR;
   private static final Property <Part>           A_B_A_MAT;
   private static final Property <BomEntry>       A_B_A_BOM;
   private static final Property <BopEntry>       A_B_A_BOP;
   private static final Property <Substitute>     A_B_A_SUB;
   
   private static final TupleCategory             B_ABA;
   
   private static final ItemCategory              PERIOD;

   private static final TupleCategory             A_PER;
   private static final Property <Double>         A_PER_DBL;
   private static final OneToOne <Tuple, Integer> A_PER_INT;
   private static final Property <Boolean>        A_PER_BOOL;

////////////////////////////////////////////////////////////////////////////////
// Static Initialization Block
//
// Construction of the UNIT_TEST Scheme and its elements.
////////////////////////////////////////////////////////////////////////////////
   
   static
      {
      UNIT_TEST  =        newScheme         ("UNIT_TEST");
      
      GLOBAL     =        currentGlobalCategory ();

      GLOBAL_DBL = GLOBAL.newProperty       ("GLOBAL_DBL", Double.class);
      
      A          =        newItemCategory   ("A");
      A_INT      = A.     newProperty       ("A_INT",      Integer.class);
      A_MAT      = A.     newOneToOne       ("A_MAT",         Part.class);
      A_CAP      = A.     newProperty       ("A_CAP",         Part.class);
      A_STR      = A.     newOneToOne       ("A_STR",      String.class);
      
      B          =        newItemCategory   ("B");
      B_OPN      = B.     newProperty       ("B_OPN",      Operation.class);
      B_DEM      = B.     newProperty       ("B_DEM",         Demand.class);
      
      A_B_A      =        newTupleCategory  ("A_B_A",      A, B, A);
      A_B_A_STR  = A_B_A. newProperty       ("A_B_A_STR",      String.class);
      A_B_A_MAT  = A_B_A. newProperty       ("A_B_A_MAT",        Part.class);
      A_B_A_BOM  = A_B_A. newProperty       ("A_B_A_BOM",    BomEntry.class);
      A_B_A_BOP  = A_B_A. newProperty       ("A_B_A_BOP",    BopEntry.class);
      A_B_A_SUB  = A_B_A. newProperty       ("A_B_A_SUB",  Substitute.class);
      
      B_ABA      =        newTupleCategory  ("B_ABA",      B, A_B_A);

      PERIOD     =        currentPeriodCategory ();

      A_PER      =        newTupleCategory  ("A_PER",      A, PERIOD);
      A_PER_DBL  = A_PER. newProperty       ("A_PER_DBL",   Double.class);
      A_PER_INT  = A_PER. newOneToOne       ("A_PER_INT",  Integer.class);
      A_PER_BOOL = A_PER. newProperty       ("A_PER_BOOL", Boolean.class);
      }

////////////////////////////////////////////////////////////////////////////////
// Private Static Fields
//
// Declaration of the POK Scheme and its elements.
// POK is the Scheme for tests of classes InputTable and InputRow.
////////////////////////////////////////////////////////////////////////////////
   
   private static final Scheme             POK;
   
   private static final ItemCategory       POK_PERIOD;
   private static final ItemCategory       AREA;
   private static final Property <Integer> N999;

////////////////////////////////////////////////////////////////////////////////
// Static Initialization Block
//
// Construction of the POK Scheme and its elements.
////////////////////////////////////////////////////////////////////////////////
   
   static
      {
      POK        = newScheme ("POK", "POK_GLOBAL", "POK_PERIOD");

      POK_PERIOD = currentPeriodCategory ();

      AREA       = newItemCategory  ("AREA");
      N999       = AREA.newProperty ("N999", Integer.class);
      
      freezeCurrentScheme ();
      }
   
////////////////////////////////////////////////////////////////////////////////
// Private Non-Static Fields
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * The main Embodiment for the unit test.
 *******************************************************************************
 */
   private Embodiment myEmbodiment;
   
////////////////////////////////////////////////////////////////////////////////
// Private Constructor
////////////////////////////////////////////////////////////////////////////////

   private UnitTester ()
      {
      myEmbodiment = null;
      }

////////////////////////////////////////////////////////////////////////////////
// Private Non-Static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Tests class Scheme.
 *******************************************************************************
 */
   private void testScheme ()
      {
      assert (currentScheme () == null);
      
      setCurrentScheme (UNIT_TEST);
      
      System.out.printf (
         "%n"
       + "Scheme %s:%n"
       + "   Name:            %s%n"
       + "   Frozen:          %s%n"
       + "   Global Category: %s%n"
       + "   Period Category: %s%n",
         UNIT_TEST,
         UNIT_TEST.getName (),
         UNIT_TEST.isFrozen (),
         UNIT_TEST.getGlobalCategory ().getName (),
         UNIT_TEST.getPeriodCategory ().getName ());
      
      System.out.printf (
         "%n"
       + "----------------------------------------"
       + "----------------------------------------%n%n");
      
      UNIT_TEST.print    (System.out);
      
      System.out.printf (
         "----------------------------------------"
       + "----------------------------------------%n");
      
      freezeCurrentScheme ();
      }

/*******************************************************************************
 * Tests a specific ItemCategory.
 *******************************************************************************
 */
   private void testItemCategory (ItemCategory theItemCat)
      {
      System.out.printf (
         "%n"
       + "ItemCategory %s:%n"
       + "   Scheme:  \"%s\"%n"
       + "   Name:    \"%s\"%n",
         theItemCat,
         theItemCat.getScheme ().getName (),
         theItemCat             .getName ());
      }

/*******************************************************************************
 * Tests class ItemCategory.
 *******************************************************************************
 */
   private void testItemCategory ()
      {
      testItemCategory (A);
      testItemCategory (PERIOD);
      }

/*******************************************************************************
 * Tests class TupleCategory.
 *******************************************************************************
 */
   private void testTupleCategory ()
      {
      System.out.printf (
         "%n"
       + "TupleCategory %s:%n"
       + "   Scheme: \"%s\"%n"
       + "   Name:   \"%s\"%n",
         A_B_A,
         A_B_A.getScheme ().getName (),
         A_B_A             .getName ());
      }

/*******************************************************************************
 * Tests class Property.
 *******************************************************************************
 */
   private void testProperty ()
      {
      System.out.printf (
         "%n"
       + "Property %s:%n"
       + "   Name:           %s%n"
       + "   Value Type:     %s%n"
       + "   EntityCategory: %s%n",
         A_INT,
         A_INT                     .getName       (),
         A_INT.getValueType      ().getSimpleName (),
         A_INT.getEntityCategory ().getName       ());
      }

/*******************************************************************************
 * Tests class Embodiment.
 *******************************************************************************
 */
   private void testEmbodiment ()
      {
      assert (currentEmbodiment () == null);
      
      myEmbodiment = UNIT_TEST.newEmbodiment ("Unit-Test");
      
      System.out.printf (
         "%n"
       + "Embodiment \"%s\":%n"
       + "   Scheme: \"%s\"%n"
       + "   Name:   \"%s\"%n",
         myEmbodiment,
         myEmbodiment.getScheme ().getName (),
         myEmbodiment             .getName ());
      
      UNIT_TEST.newEmbodiment ("Embodiment #2");

      System.out.printf (
         "%n"
       + "Current Embodiment: \"%s\"%n",
         currentEmbodiment ().getName ());
      
      setCurrentEmbodiment (myEmbodiment);
      
      System.out.printf (
         "%n"
       + "Current Embodiment: \"%s\"%n",
         currentEmbodiment ().getName ());
      }

/*******************************************************************************
 * Tests class Item.
 *******************************************************************************
 */
   private void testItem ()
      {
      Item theA;
      Item foundItem;
      
      theA = A.newItem ("1");
             B.newItem ("2");
             A.newItem ("3");

      System.out.printf (
         "%n"
       + "%s:%n"
       + "   Embodiment:     \"%s\"%n"
       + "   EntityCategory: \"%s\"%n"
       + "   ItemCategory:   \"%s\"%n"
       + "   Name:           \"%s\"%n",
         theA,
         theA.getEmbodiment     ().getName (),
         theA.getEntityCategory ().getName (),
         theA.getItemCategory   ().getName (),
         theA                     .getName ());
      
      foundItem = A.get ("1");

      assert (foundItem == theA);

      System.out.printf (
         "%n"
       + "A Items:%n");
      
      for (Item theItem: A.getAll ())
         {
         System.out.printf ("   %s%n", theItem);
         }
      
      System.out.printf (
         "%n"
       + "Global Item: %s%n",
         currentGlobalItem ());
      }
   
/*******************************************************************************
 * Tests class Tuple.
 *******************************************************************************
 */
   private void testTuple ()
      {
      Item   theA1;
      Item   theB2;
      Item   theA3;
      Tuple  theA1_B2_A3;
      Tuple  theA_B_A;
      Tuple  theB_ABA;
      Item   theItem;
      String theText;

      theA1       = A.get ("1");
      theB2       = B.get ("2");
      theA3       = A.get ("3");
      
      theA1_B2_A3 = A_B_A.newTuple (theA1, theB2, theA3);
      
      A_B_A.newTuple (theA1, theB2, theA1);
      
      assert (theA1_B2_A3.getTupleCategory () == A_B_A);
      
      System.out.printf (
         "%n"
       + "%s:%n",
         theA1_B2_A3);
      
      System.out.printf (
         "%n"
       + "   B Coordinate: %s%n",
         theA1_B2_A3.get (B));
      
      System.out.printf (
         "%n"
       + "All Tuples of type A_B_A:%n");
      
      for (Tuple locA_B_A: A_B_A.getAll ())
         {
         System.out.printf (
            "   %s%n",
            locA_B_A);
         }
      
      theA_B_A = A_B_A.get (theA3, theB2, theA1);
      
      assert (theA_B_A == null);
      
      theA_B_A = A_B_A.get (theA1, theB2, theA3);
      
      assert (theA_B_A == theA1_B2_A3);
      
      theA_B_A.set (A_B_A_STR, "ABA Text");
      
      theText = theA_B_A.get (A_B_A_STR);
      
      assert (theText.equals ("ABA Text"));
      
      theB_ABA = B_ABA.newTuple (theB2, theA1_B2_A3);
      
      System.out.printf (
         "%n"
       + "The B_ABA:%n"
       + "   %s%n",
         theB_ABA);
         
      theItem  = theB_ABA.get (A_B_A).get (B);
      
      assert (theItem == theB2);
      
      theItem = theA1_B2_A3.get (A, 2);
      
      assert (theItem == theA3);
      }
   
/*******************************************************************************
 * Tests getSlice (...) on thePattern.
 *******************************************************************************
 */
   private void testSlice (String thePatternText, Entity...  thePattern)
      {
      System.out.printf (
         "%n"
         + "Slice (" + thePatternText + "):%n");
      
      for (Tuple theTuple: A_B_A.getSlice (thePattern))
         System.out.printf ("   %s%n", theTuple);
      }
  
/*******************************************************************************
 * Tests getSlice (...), buildSlice (...), and buildAll (). 
 *******************************************************************************
 */
   private void testSlice ()
      {
      Item A1;
      Item A2;
      Item A3;
      Item B1;
      Item B2;
      
      UNIT_TEST.newEmbodiment ("Slice Test");
      
      A1 = A.newItem ("1");
      A2 = A.newItem ("2");
      A3 = A.newItem ("3");
      B1 = B.newItem ("1");
      B2 = B.newItem ("2");
      
      A_B_A.newTuple (A3, B2, A3);
      A_B_A.newTuple (A3, B2, A1);
      A_B_A.newTuple (A3, B1, A2);
      A_B_A.newTuple (A2, B2, A3);
      A_B_A.newTuple (A2, B2, A1);
      A_B_A.newTuple (A2, B1, A2);
      A_B_A.newTuple (A1, B2, A2);
      A_B_A.newTuple (A1, B1, A3);
      A_B_A.newTuple (A1, B1, A1);
      
      System.out.printf (
         "%n"
         + "Tuples for the Slice Test:%n");
      
      for (Tuple theTuple: A_B_A.getAll ())
         System.out.printf ("   %s%n", theTuple);
      
      testSlice ("A2, B2, A3",         A2,   B2,   A3);
      testSlice ("A2, B2, A2",         A2,   B2,   A2);
      testSlice ("Null, B1, A2",     null,   B1,   A2);
      testSlice ("A2, Null, Null",     A2, null, null);
      testSlice ("Null, Null, Null", null, null, null);
      
      A_B_A.buildSlice (A2, null, null);
      
      System.out.printf (
         "%n"
         + "Tuples after buildSlice (A2, null, null):%n");
      
      for (Tuple theTuple: A_B_A.getAll ())
         System.out.printf ("   %s%n", theTuple);
      
      A_B_A.buildSlice (null, B1, A3);
      
      System.out.printf (
         "%n"
         + "Tuples after buildSlice (null, B1, A3):%n");
      
      for (Tuple theTuple: A_B_A.getAll ())
         System.out.printf ("   %s%n", theTuple);
      
      A_B_A.buildAll ();
      
      System.out.printf (
         "%n"
         + "Tuples after buildAll ():%n");
      
      for (Tuple theTuple: A_B_A.getAll ())
         System.out.printf ("   %s%n", theTuple);
      
      setCurrentEmbodiment (myEmbodiment);
      }
   
/*******************************************************************************
 * Tests the set and get methods for Properties.
 *******************************************************************************
 */
   private void testSetGet ()
      {
      Entity  theA;
      Integer theInteger;
      Double  theDouble;
      
      theA = A.get ("1");
      
      theInteger = theA.get (A_INT);
      
      assert (theInteger == null);
      
      theA.set (A_INT, 17);
      
      theInteger = theA.get (A_INT);
      
      assert (theInteger == 17);
      
      theA.set (A_INT, null);
      
      theInteger = theA.get (A_INT);
      
      assert (theInteger == null);
      
      currentGlobalItem ().set (GLOBAL_DBL, 29.7);
      
      theDouble = currentGlobalItem ().get (GLOBAL_DBL);
      
      assert (theDouble == 29.7);
      }

/*******************************************************************************
 * Tests the setByPeriod and getByPeriod methods.
 *******************************************************************************
 */
   private void testSetGetByPeriod ()
      {
      Item      theA1;
      Item      mon;
      Item      wed;
      Tuple     theA1wed;
      double[]  theDblArr;
      int[]     theIntArr;
      boolean[] theBoolArr;
      int       theIdx;
      
      UNIT_TEST.newEmbodiment ("Set/Get ByPeriod Test");
      
              A     .newItem ("A0");
      theA1 = A     .newItem ("A1");
      mon   = PERIOD.newItem ("mon");
              PERIOD.newItem ("tue");
      wed   = PERIOD.newItem ("wed");

                 A_PER.newTuple (theA1, mon);
      theA1wed = A_PER.newTuple (theA1, wed);
      
      theA1wed.set (A_PER_DBL,  3.0);
      theA1wed.set (A_PER_INT,  4);
      theA1wed.set (A_PER_BOOL, true);

      theDblArr  = new double [3];
      theIntArr  = new int    [3];
      theBoolArr = new boolean[3];
      
      theA1.getByPeriod (A_PER_DBL,  theDblArr,  7.0);
      theA1.getByPeriod (A_PER_INT,  theIntArr,  9);
      theA1.getByPeriod (A_PER_BOOL, theBoolArr, false);
      
      System.out.printf ("%n");
      
      for (theIdx = 0; theIdx < 3; theIdx ++)
         System.out.printf (
            "Idx = %d, Dbl = %.0f, Int = %d, Bool = %s%n",
            theIdx,
            theDblArr [theIdx],
            theIntArr [theIdx],
            theBoolArr[theIdx]);
      
      theDblArr  = new double []{10.0, 11.0,  12.0};
      theIntArr  = new int    []{13,   13,    13};
      theBoolArr = new boolean[]{true, false, false};
      
      theA1.setByPeriod (A_PER_DBL,  theDblArr);
      theA1.setByPeriod (A_PER_INT,  theIntArr);
      theA1.setByPeriod (A_PER_BOOL, theBoolArr);
      
      for (Tuple theA_PER: A_PER.getAll ())
         {
         System.out.printf (
            "%n"
          + "%s:%n"
          + "   A_PER_DBL:  %.0f%n"
          + "   A_PER_INT:  %d%n"
          + "   A_PER_BOOL: %s%n",
            theA_PER,
            theA_PER.get (A_PER_DBL),
            theA_PER.get (A_PER_INT),
            theA_PER.get (A_PER_BOOL));
         }
      
      setCurrentEmbodiment (myEmbodiment);
      }

/*******************************************************************************
 * Test some WIT-J-related methods.
 *******************************************************************************
 */
   private void testWitJRelated ()
      {
      Problem theProblem;
      
      theProblem = Problem.newInstance ();
      
      myEmbodiment.setProblem (theProblem);
      
      assert (myEmbodiment.getProblem () == theProblem);

      assert (currentProblem () == theProblem);

      assert (! myEmbodiment.getUseShortNames ());
      
      myEmbodiment.setUseShortNames (true);
      
      assert   (myEmbodiment.getUseShortNames ());
      }

/*******************************************************************************
 * Tests Entity.newPart, Entity.newOperation, etc.
 *******************************************************************************
 */
   private void testNewComp ()
      {
      Item  theA1;
      Item  theB2;
      Item  theA3;
      Tuple theA1_B2_A3;
      Part  theCap1;
      Part  theMat1;
      Part  theMat3;
      
      theA1 = A.get ("1");
      theB2 = B.get ("2");
      theA3 = A.get ("3");
      
      theA1_B2_A3 = A_B_A.get (theA1, theB2, theA3);
      
      theCap1 = theA1      .newPart      (A_CAP,     CAPACITY);
      
      myEmbodiment.setUseShortNames (false);

      theMat1 = theA1      .newPart      (A_MAT,     MATERIAL);
      theMat3 = theA3      .newPart      (A_MAT,     MATERIAL);
                theA1_B2_A3.newPart      (A_B_A_MAT, MATERIAL);
                theB2      .newOperation (B_OPN);
                theB2      .newDemand    (B_DEM,     theA1.get (A_MAT));

      theA1_B2_A3.
         newBomEntry  (
            A_B_A_BOM,
            theA1_B2_A3.get (B)   .get (B_OPN),
            theA1_B2_A3.get (A, 0).get (A_MAT));

      theA1_B2_A3.
         newBopEntry  (
            A_B_A_BOP,
            theA1_B2_A3.get (B)   .get (B_OPN),
            theA1_B2_A3.get (A, 2).get (A_MAT));

      theA1_B2_A3.
         newSubstitute (
            A_B_A_SUB,
            theA1_B2_A3           .get (A_B_A_BOM),
            theA1_B2_A3.get (A, 2).get (A_MAT));

      System.out.printf (
         "%n"
         + "A1    .A_CAP:        %s%n"
         + "A1    .A_MAT:        %s%n"
         + "A3    .A_CAP:        %s%n"
         + "A3    .A_MAT:        %s%n"
         + "A1B2A3.A_B_A_MAT:    %s%n"
         + "B2    .B_OPN:        %s%n"
         + "B2    .B_DEM:        %s%n"
         + "A1B2A3.A_B_A_BOM:    %s%n"
         + "A1B2A3.A_B_A_BOP:    %s%n"
         + "A1B2A3.A_B_A_SUB:    %s%n"
         + "A_MAT.Entity (Cap1): %s%n"
         + "A_MAT.Entity (Mat1): %s%n"
         + "A_MAT.Entity (Mat3): %s%n",  
         theA1      .get (A_CAP),
         theA1      .get (A_MAT),
         theA3      .get (A_CAP),
         theA3      .get (A_MAT),
         theA1_B2_A3.get (A_B_A_MAT),
         theB2      .get (B_OPN),
         theB2      .get (B_DEM),
         theA1_B2_A3.get (A_B_A_BOM),
         theA1_B2_A3.get (A_B_A_BOP),
         theA1_B2_A3.get (A_B_A_SUB),
         A_MAT.getEntity (theCap1),
         A_MAT.getEntity (theMat1),
         A_MAT.getEntity (theMat3));
      }

/*******************************************************************************
 * Tests class OneToOne.
 *******************************************************************************
 */
   private void testOneToOne ()
      {
      Item   theA1;
      Item   theA3;
      String three;
      
      three = new String ("Three");
      
      theA1 = A.get ("1");
      theA3 = A.get ("3");
      
      theA1.set (A_STR, "One");
      theA3.set (A_STR, "Three");

      assert (theA1.get (A_STR).equals ("One"));
      assert (theA3.get (A_STR).equals (three));

      assert (A_STR.getEntity ("One") == theA1);
      assert (A_STR.getEntity (three) == theA3);

      theA1.set (A_STR, "Three");

      assert (theA1.get (A_STR).equals ("Three"));
      
      assert (theA3.get (A_STR) == null);

      assert (A_STR.getEntity ("One")   == null);
      assert (A_STR.getEntity ("Three") == theA1);
      }

/*******************************************************************************
 * Tests class InputTable by processing the file period.csv.
 *******************************************************************************
 */
   private void procPeriodCSV ()
      {
      InputTable theTable;
      
      POK.newEmbodiment ("Poughkeepsie");
      
      theTable = InputTable.newInputTable ("POK/period.csv", ",");
      
      System.out.printf (
         "%n"
       + "%s%n",
         theTable);
         
      for (InputRow theRow: theTable.getRows ())
         if (theRow.getIndex () > 1)
            POK_PERIOD.newItem (theRow.getCell (1).asString ());
      
      System.out.printf (
         "%n"
       + "Periods for POK Problem:%n");
         
      for (Item thePer: POK_PERIOD.getAll ())
         System.out.printf ("   %s%n", thePer.getName ());
      }
 
/*******************************************************************************
 * Tests class InputTable by processing the file area.dotsv.
 * Tests the reading of a file whose delimiter is dot (period) instead of comma.
 * area.dotsv is the same as area.csv, except that all commas have been replaced
 * with dots.
 *******************************************************************************
 */
   private void procAreaDotSV ()
      {
      InputTable theTable;
      Item       newArea;
      int        theN999;
      
      theTable = InputTable.newInputTable ("POK/area.dotsv", ".");
      
      for (InputRow theRow: theTable.getRows ())
         {
         if (theRow.getIndex () == 1)
            continue;

         theRow.getCell (1).forbidItemNameMatch (AREA);
         
         newArea = AREA.newItem (theRow.getCell (1).asString ());
            
         theN999 = theRow.getCell (6).asInt ("#999");
            
         newArea.set (N999, theN999);
         }
      
      System.out.printf (
         "%n"
       + "Areas for POK Problem:%n");
         
      for (Item theArea: AREA.getAll ())
         System.out.printf (
            "   %-31s #999: %d%n",
            theArea.getName (),
            theArea.get     (N999));
      }
 
/*******************************************************************************
 * Tests class InputRow by processing the file area-shift-period.csv.
 *******************************************************************************
 */
   private void procAreaShiftPeriodCSV ()
      {
      InputTable theTable;
      InputRow   theRow65;
      
      theTable = InputTable.newInputTable ("POK/area-shift-period.csv", ",");
      
      theRow65 = theTable.getRow (65);
      
      theRow65.requireNCells (5);
      
      System.out.printf (
         "%n"
       + "%s:%n%n"
       + "   Line #:      %d%n"
       + "   # of Cells:  %d%n"
       + "   Area:        %s%n"
       + "   Shift:       %d%n"
       + "   Period:      %s%n"
       + "   Period #:    %d%n"
       + "   Min Workers: %s%n"
       + "   Max Workers: %.1f%n",
         theRow65,
         theRow65.getIndex (),
         theRow65.nCells   (),
         theRow65.getCell  (1).asItem   (AREA)      .getName (),
         theRow65.getCell  (2).asInt    ("Shift"),
         theRow65.getCell  (3).asItem   (POK_PERIOD).getName (),
         theRow65.getCell  (3).asItem   (POK_PERIOD).getPeriodIndex (),
         theRow65.getCell  (4).asString (),
         theRow65.getCell  (5).asDouble ("Max Workers"));
      
      System.out.printf (
         "%n"
       + "%s%n%n",
         theRow65.getCell (3));
      }
 
/*******************************************************************************
 * Releases / deactivates various objects associated with the test.
 *******************************************************************************
 */
   private void shutDown ()
      {
      myEmbodiment.getProblem ().deactivate ();
      
      setCurrentEmbodiment (null);
      }

/*******************************************************************************
 * Internal implementation of the unit test.
 *******************************************************************************
 */
   private void perfTest ()
      {
      testScheme             ();
      testItemCategory       ();
      testTupleCategory      ();
      testProperty           ();
      testEmbodiment         ();
      testItem               ();
      testTuple              ();
      testSlice              ();
      testSetGet             ();
      testSetGetByPeriod     ();
      testWitJRelated        ();
      testNewComp            ();
      testOneToOne           ();
      procPeriodCSV          ();
      procAreaDotSV          ();
      procAreaShiftPeriodCSV ();
      shutDown               ();
      }

////////////////////////////////////////////////////////////////////////////////
// Package Internal Static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Performs a unit test of WIT-M: Invokes all public methods of WIT-M.
 *******************************************************************************
 */
   static void performTest ()
      {
      try
         {
         UnitTester theUnitTester;
         
         theUnitTester = new UnitTester ();

         theUnitTester.perfTest ();
         }

      catch (Exception theExc)
         {
         theExc.printStackTrace ();

         System.exit (4);
         }

      System.out.printf (
         "\n"
       + "Normal termination of unit test.%n");
      }

////////////////////////////////////////////////////////////////////////////////
}
