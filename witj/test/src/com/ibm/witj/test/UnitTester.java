
//==============================================================================
// WIT-J/Test Source File UnitTester.java.
//==============================================================================

package       com.ibm.witj.test;

import        com.ibm.witj.*;
import static com.ibm.witj.Attribute.*;
import static com.ibm.witj.Part.Category.*;

import                java.util.*;

//==============================================================================
// Class UnitTester
//
// Responsible for performing a unit test on WIT-J.
//==============================================================================

final class UnitTester
{
//==============================================================================
// Package Internal Methods
//==============================================================================

//------------------------------------------------------------------------------
// performTest ()
//
// Performs the unit test.
//------------------------------------------------------------------------------

   static void performTest ()
      {
      UnitTester theUnitTester;

      theUnitTester = new UnitTester ();

      theUnitTester.perfTest ();
      }

//==============================================================================
// Private Constructor
//==============================================================================

   private UnitTester ()
      {
      allComponentClasses = new ArrayList <Class <? extends Component>> (7);

      allComponentClasses.add (Problem   .class);
      allComponentClasses.add (Part      .class);
      allComponentClasses.add (Demand    .class);
      allComponentClasses.add (Operation .class);
      allComponentClasses.add (BomEntry  .class);
      allComponentClasses.add (Substitute.class);
      allComponentClasses.add (BopEntry  .class);

      undisplayedAttributes =
         new HashSet <Attribute <?>> (getAttributes ().size () * 2);

      for (Attribute <?> theAtt: getAttributes ())
         {
         undisplayedAttributes.add (theAtt);
         }
      }

//==============================================================================
// Private Methods
//==============================================================================

//------------------------------------------------------------------------------
// perfTest ().
//
// Internal implementation of performTest ().
//------------------------------------------------------------------------------

   private void perfTest ()
      {
      boolean cplexEmb;

      println (
         "\n"
       + "Unit test of WIT-J.");

      assert (! TerminalException.witjHasTerminated ());

      myProblem = Problem.newInstance ();

      testMessageMgr               ();
      setUpNPeriods                ();
      myProblem.performAction      ("self-test");
      buildComponents              ();
      testClearData                ();
      printPublicObjects           ();
      testAssocMethods             ();
      displayAttributes            ();
      checkForNonAppAttributes     ();
      verifyDefaultValues          ();
      testIsValidFor               ();
      testSetGet                   ();
      testSpecAtts                 ();
      testOptInitMethod            ();
      setAttributes                ();
      testCopyData                 ();
      testCopyCompData             ();
      testExpCycle                 ();
      testBelowList                ();
      testHeurAlloc                ();
      testEqHeurAlloc              ();
      myProblem.heurImplode        ();
      testPip                      ();
      testCPeg                     ();
      testCriticalList             ();
      testPgdCritList              ();
      testWriteMethods             ();
      testMultiObjMode             ();
      displayNonDefGlobalBoolAtts  (myProblem);

      myProblem.generatePriorities ();
      myProblem.postprocess        ();
      myProblem.evalObjectives     ();
      testCplexParSpecs            ();

      cplexEmb = myProblem.get     (CPLEX_EMBEDDED);

      if (cplexEmb)
         myProblem.optImplode      ();

      myProblem.deactivate         ();

      assert (! myProblem.isActive ());

      testSortedParts              ();
      testSortedOperations         ();

      testReadData                 ();

      testUnicode                  ();

      PurgeTester.performTest      ();

      if (cplexEmb)
         NewsVendor.performTest    ();

      println (
         "\n"
       + "Normal termination of unit test.");
      }

//------------------------------------------------------------------------------
// testMessageMgr ()
//
// Test of the MessageMgr.
//------------------------------------------------------------------------------

   private void testMessageMgr ()
      {
      boolean quietVal;
      String  theName;
      String  theMode;
      int     theInt;

      myMessageMgr = myProblem   .getMessageMgr ();
      quietVal     = myMessageMgr.isQuiet       ();

      assert (quietVal);

      myMessageMgr.setMesgFileName ("UnitTester.output");
      myMessageMgr.setQuiet        (false);

      quietVal = myMessageMgr.isQuiet ();

      assert (! quietVal);

      myMessageMgr.setMesgFileName ("stdout");

      theName = myMessageMgr.getMesgFileName ();

      assert (theName.equals ("stdout"));

      myMessageMgr.setMesgFileName ("UnitTester.output");

      theName = myMessageMgr.getMesgFileName ();

      assert (theName.equals ("UnitTester.output"));

      myMessageMgr.setMesgFileAccessMode ("a");

      theMode = myMessageMgr.getMesgFileAccessMode ();

      assert (theMode.equals ("a"));

      myMessageMgr.setMesgFileAccessMode ("w");

      myMessageMgr.setMesgTimesPrint (101, 50);

      theInt = myMessageMgr.getMesgTimesPrint (101);

      assert (theInt == 50);

      myMessageMgr.setMesgTimesPrint (MessageGroup.INFORMATIONAL, -7);

      theInt = myMessageMgr.getMesgTimesPrint (101);

      assert (theInt == -1);
      }

//------------------------------------------------------------------------------
// setUpNPeriods ().
//
// Sets up N_PERIODS.
//------------------------------------------------------------------------------

   private void setUpNPeriods ()
      {
      int nPeriodsVal;

      myProblem.set (N_PERIODS, 3);

      nPeriodsVal = myProblem.get (N_PERIODS);

      assert (nPeriodsVal == 3);
      }

//------------------------------------------------------------------------------
// buildComponents ().
//
// Builds the Components for the unit test.
//------------------------------------------------------------------------------

   private void buildComponents ()
      {
      myPartA     = Part      .newInstance (myProblem,  "A", CAPACITY);
      myPartB     = Part      .newInstance (myProblem,  "B", CAPACITY);
      myPartC     = Part      .newInstance (myProblem,  "C", MATERIAL);
      myPartD     = Part      .newInstance (myProblem,  "D", MATERIAL);
      myPartE     = Part      .newInstance (myProblem,  "E", MATERIAL);
      myOpnF      = Operation .newInstance (myProblem,  "F");
      myOpnG      = Operation .newInstance (myProblem,  "G");
      myBomEntFA  = BomEntry  .newInstance (myOpnF,     myPartA);
      myBomEntFB1 = BomEntry  .newInstance (myOpnF,     myPartB);
      myBomEntFB2 = BomEntry  .newInstance (myOpnF,     myPartB);
                    BomEntry  .newInstance (myOpnG,     myPartC);
      mySubFAB    = Substitute.newInstance (myBomEntFA, myPartB);
      myBopEntFC  = BopEntry  .newInstance (myOpnF,     myPartC);
      myBopEntFG  = BopEntry  .newInstance (myOpnF,     myPartD);
                    BopEntry  .newInstance (myOpnG,     myPartE);
      myDemandH   = Demand    .newInstance (myPartE,    "H");
      myDemandI   = Demand    .newInstance (myPartB,    "I");
      myDemandJ   = Demand    .newInstance (myPartD,    "J");

      myMessageMgr.flushFile ();
      }

//------------------------------------------------------------------------------
// printPublicObjects ().
//
// Prints the Problem and the public objects that it owns.
//------------------------------------------------------------------------------

   private void printPublicObjects ()
      {
      println ("");
      println (myProblem);
      println (myMessageMgr);
      println (myPartA);
      println (myPartB);
      println (myPartC);
      println (myOpnF);
      println (myBomEntFA);
      println (myBomEntFB1);
      println (myBomEntFB2);
      println (mySubFAB);
      println (myBopEntFC);
      println (myDemandH);
      println (myDemandI);

      println (myOpnF + ": " + myOpnF.getThread ());
      }

//------------------------------------------------------------------------------
// testAssocMethods ()
//
// Tests methods that return Objects associated with a particular Object.
//------------------------------------------------------------------------------

   private void testAssocMethods ()
      {
      testProblemAssocMethods    ();
      testPartAssocMethods       ();
      testDemandAssocMethods     ();
      testOperationAssocMethods  ();
      testBomEntryAssocMethods   ();
      testSubstituteAssocMethods ();
      testBopEntryAssocMethods   ();
      }

//------------------------------------------------------------------------------
// testProblemAssocMethods ()
//
// Tests methods that return Objects associated with a Problem
//------------------------------------------------------------------------------

   private void testProblemAssocMethods ()
      {
      println ("");

      println ("myProblem.getPart      (\"B\"): "
              + myProblem.getPart      ( "B" ));
      println ("myProblem.getOperation (\"F\"): "
              + myProblem.getOperation ( "F" ));

      printList (
          myProblem.getComponents (),
         "myProblem.getComponents ()");

      printList (
          myProblem.getParts (),
         "myProblem.getParts ()");

      printList (
          myProblem.getDemands (),
         "myProblem.getDemands ()");

      printList (
          myProblem.getOperations (),
         "myProblem.getOperations ()");

      printList (
          myProblem.getBomEntries (),
         "myProblem.getBomEntries ()");

      printList (
          myProblem.getSubstitutes (),
         "myProblem.getSubstitutes ()");

      printList (
          myProblem.getBopEntries (),
         "myProblem.getBopEntries ()");
      }

//------------------------------------------------------------------------------
// testPartAssocMethods ()
//
// Tests methods that return Objects associated with a Part
//------------------------------------------------------------------------------

   private void testPartAssocMethods ()
      {
      println ("");

      println ("myPartE.getProblem  ():    "
              + myPartE.getProblem  ());
      println ("myPartE.getDemand   (\"H\"): "
              + myPartE.getDemand   ( "H" ));
      println ("myPartE.getCategory ():    "
              + myPartE.getCategory ());

      printList (
          myPartE.getDemands (),
         "myPartE.getDemands ()");

      printList (
          myPartB.getBomEntries (),
         "myPartB.getBomEntries ()");

      printList (
          myPartB.getSubstitutes (),
         "myPartB.getSubstitutes ()");

      printList (
          myPartE.getBopEntries (),
         "myPartE.getBopEntries ()");
      }

//------------------------------------------------------------------------------
// testDemandAssocMethods ()
//
// Tests methods that return Objects associated with a Demand
//------------------------------------------------------------------------------

   private void testDemandAssocMethods ()
      {
      println ("");

      println ("myDemandH.getPart (): "
              + myDemandH.getPart ());
      }

//------------------------------------------------------------------------------
// testOperationAssocMethods ()
//
// Tests methods that return Objects associated with an Operation
//------------------------------------------------------------------------------

   private void testOperationAssocMethods ()
      {
      printList (
          myOpnF.getBomEntries (),
         "myOpnF.getBomEntries ()");

      printList (
          myOpnF.getBomEntriesTo (myPartB),
         "myOpnF.getBomEntriesTo (myPartB)");

      printList (
          myOpnF.getBomEntriesTo (myPartD),
         "myOpnF.getBomEntriesTo (myPartD)");

      println (
         "\n"
       + "myOpnF.getUniqueBomEntryTo (myPartA):\n"
       + "   "
       +  myOpnF.getUniqueBomEntryTo (myPartA));

      println (
         "\n"
       + "myOpnF.getUniqueBomEntryTo (myPartD):\n"
       + "   "
       +  myOpnF.getUniqueBomEntryTo (myPartD));

      printList (
          myOpnF.getBopEntries (),
         "myOpnF.getBopEntries ()");

      printList (
          myOpnF.getBopEntriesTo (myPartD),
         "myOpnF.getBopEntriesTo (myPartD)");

      println (
         "\n"
       + "myOpnF.getUniqueBopEntryTo (myPartC):\n"
       + "   "
       +  myOpnF.getUniqueBopEntryTo (myPartC));
      }

//------------------------------------------------------------------------------
// testBomEntryAssocMethods ().
//
// Tests methods that return Objects associated with a BomEntry.
//------------------------------------------------------------------------------

   private void testBomEntryAssocMethods ()
      {
      println ("");

      println ("myBomEntFB1.getOperation (): "
              + myBomEntFB1.getOperation ());
      println ("myBomEntFB1.getPart      (): "
              + myBomEntFB1.getPart      ());

      printList (
          myBomEntFA.getSubstitutes (),
         "myBomEntFA.getSubstitutes ()");

      printList (
          myBomEntFA.getSubstitutesTo (myPartB),
         "myBomEntFA.getSubstitutesTo (myPartB)");

      println (
         "\n"
       + "myBomEntFA.getUniqueSubstituteTo (myPartB):\n"
       + "   "
       +  myBomEntFA.getUniqueSubstituteTo (myPartB));

      }

//------------------------------------------------------------------------------
// testSubstituteAssocMethods ().
//
// Tests methods that return Objects associated with a Substitute.
//------------------------------------------------------------------------------

   private void testSubstituteAssocMethods ()
      {
      println ("");

      println ("mySubFAB.getBomEntry (): "
              + mySubFAB.getBomEntry ());
      println ("mySubFAB.getPart     (): "
              + mySubFAB.getPart     ());
      }

//------------------------------------------------------------------------------
// testBopEntryAssocMethods ().
//
// Tests methods that return Objects associated with a BopEntry.
//------------------------------------------------------------------------------

   private void testBopEntryAssocMethods ()
      {
      println ("");

      println ("myBopEntFC .getOperation (): "
              + myBopEntFC .getOperation ());
      println ("myBopEntFC .getPart      (): "
              + myBopEntFC .getPart      ());
      }

//------------------------------------------------------------------------------
// displayAttributes ()
//------------------------------------------------------------------------------

   private void displayAttributes ()
      {
      println (
           "\n"
         + "Attributes:");

      display (ACCELERATED);
      display (ACC_AFTER_OPT_IMP);
      display (ACC_AFTER_SOFT_L_B);
      display (ASAP_PIP_ORDER);
      display (AUTO_PRIORITY);
      display (BOM_INDEX);
      display (BOP_INDEX);
      display (BOUNDED_LEAD_TIMES);
      display (BOUNDS_VALUE);
      display (BUILD_AHEAD_U_B);
      display (BUILD_ASAP);
      display (BUILD_NSTN);
      display (COMPUTE_CRITICAL_LIST);
      display (COMP_PRICES);
      display (CONS_RATE);
      display (CONS_VOL);
      display (CPLEX_EMBEDDED);
      display (CPLEX_MIP_BOUND);
      display (CPLEX_MIP_REL_GAP);
      display (CPLEX_PAR_SPEC_DBL_VAL);
      display (CPLEX_PAR_SPEC_INT_VAL);
      display (CPLEX_PAR_SPEC_NAME);
      display (CPLEX_STATUS_CODE);
      display (CPLEX_STATUS_TEXT);
      display (CUM_SHIP_BOUNDS);
      display (CUM_SHIP_REWARD);
      display (CURRENT_OBJECTIVE);
      display (CURRENT_SCENARIO);
      display (DEMAND_NAME);
      display (DEMAND_VOL);
      display (EARLIEST_PERIOD);
      display (EQUITABILITY);
      display (EXCESS_VOL);
      display (EXECUTABLE);
      display (EXEC_BOUNDS);
      display (EXEC_COST);
      display (EXEC_EMPTY_BOM);
      display (EXEC_PENALTY);
      display (EXEC_VOL);
      display (EXP_ALLOWED);
      display (EXP_AVERSION);
      display (EXP_CUTOFF);
      display (EXP_NET_AVERSION);
      display (EXT_OPT_ACTIVE);
      display (FALLOUT_RATE);
      display (FEASIBLE);
      display (FOCUS_HORIZON);
      display (FOCUS_SHORTAGE_VOL);
      display (FORCED_MULTI_EQ);
      display (FSS_EXEC_VOL);
      display (FSS_SHIP_VOL);
      display (FSS_SUB_VOL);
      display (HEUR_ALLOC_ACTIVE);
      display (HIGH_PRECISION_W_D);
      display (IMPACT_PERIOD);
      display (INC_LOT_SIZE);
      display (INC_LOT_SIZE2);
      display (INDEPENDENT_OFFSETS);
      display (INT_EXEC_VOLS);
      display (INT_SHIP_VOLS);
      display (INT_SUB_VOLS);
      display (LATEST_PERIOD);
      display (LEAD_TIME_U_B);
      display (LOT_SIZE2_THRESH);
      display (LOT_SIZE_TOL);
      display (MAND_E_C);
      display (MINIMAL_EXCESS);
      display (MIN_LOT_SIZE);
      display (MIN_LOT_SIZE2);
      display (MIP_MODE);
      display (MOD_HEUR_ALLOC);
      display (MRP_CONS_VOL);
      display (MRP_EXCESS_VOL);
      display (MRP_EXEC_VOL);
      display (MRP_NET_ALLOWED);
      display (MRP_RESIDUAL_VOL);
      display (MRP_SUB_VOL);
      display (MULTI_EXEC);
      display (MULTI_OBJ_MODE);
      display (MULTI_OBJ_TOL);
      display (MULTI_ROUTE);
      display (NET_ALLOWED);
      display (NSTN_RESIDUAL);
      display (N_PERIODS);
      display (N_SCENARIOS);
      display (OBJECTIVE_LIST_SPEC);
      display (OBJECTIVE_SEQ_NO);
      display (OBJECT_STAGE);
      display (OBJ_VALUE);
      display (OFFSET);
      display (OPERATION_NAME);
      display (PART_NAME);
      display (PEN_EXEC);
      display (PERF_PEGGING);
      display (PERIOD_STAGE);
      display (PGD_CRIT_LIST_MODE);
      display (PIP_ENABLED);
      display (PIP_EXISTS);
      display (PIP_RANK);
      display (PIP_SEQ_FROM_HEUR);
      display (POSTPROCESSED);
      display (PREF_HIGH_STOCK_S_L_BS);
      display (PREPROCESSED);
      display (PRIORITY);
      display (PROBABILITY);
      display (PRODUCT_RATE);
      display (PROD_VOL);
      display (PROP_RTG);
      display (REQ_VOL);
      display (RESIDUAL_VOL);
      display (RESPECT_STOCK_S_L_BS);
      display (ROUND_REQ_VOLS);
      display (ROUTING_SHARE);
      display (SCRAP_ALLOWED);
      display (SCRAP_COST);
      display (SCRAP_VOL);
      display (SEARCH_INC);
      display (SELECTION_RECOVERY);
      display (SEL_FOR_DEL);
      display (SEL_SPLIT);
      display (SHADOW_PRICE);
      display (SHIP_LATE_ALLOWED);
      display (SHIP_LATE_U_B);
      display (SHIP_REWARD);
      display (SHIP_VOL);
      display (SINGLE_SOURCE);
      display (SKIP_FAILURES);
      display (SOLVER_LOG_FILE_NAME);
      display (STAGE_BY_OBJECT);
      display (STOCH_MODE);
      display (STOCH_SOLN_MODE);
      display (STOCK_BOUNDS);
      display (STOCK_COST);
      display (STOCK_REALLOCATION);
      display (STOCK_VOL);
      display (SUB_COST);
      display (SUB_INDEX);
      display (SUB_VOL);
      display (SUPPLY_VOL);
      display (TIE_BREAK_PROP_RT);
      display (TITLE);
      display (TRUNC_OFFSETS);
      display (TWO_LEVEL_LOT_SIZES);
      display (TWO_WAY_MULTI_EXEC);
      display (USER_HEUR_START);
      display (USE_FOCUS_HORIZONS);
      display (WBOUNDS);
      display (YIELD_RATE);

      if (undisplayedAttributes.isEmpty ())
         {
         println (
              "\n"
            + "All Attributes have been displayed.");
         }
      else
         {
         println (
            "\n"
          + "The following Attributes were missing from the above listing:\n");

         for (Attribute <?> theAtt: undisplayedAttributes)
            {
            println ("   " + theAtt);
            }
         }

      println (
         "\n"
       + "Total # of Attributes: " + getAttributes ().size ());
      }

//------------------------------------------------------------------------------
// display (theAtt)
//
// Displays information about theAtt.
// To be invoked exactly once for each Attribute.
//------------------------------------------------------------------------------

   private void display (Attribute <?> theAtt)
      {
      assert (undisplayedAttributes.contains (theAtt));

      println (
           "\n"
         + "   " + theAtt + ":");

      println ("      WIT Name:   " + theAtt.getWitName ());

      for (Class <? extends Component> theClass: allComponentClasses)
         {
         if (theAtt.appliesTo (theClass))
            {
            println ("      Applies to: " + theClass.getSimpleName ());
            }
         }

      println (
           "      Value Type: "
         + theAtt.getValueType ().getSimpleName ());

      println ("      Modifiable: " + theAtt.isModifiable ());

      println ("      Default:    " + defaultValText (theAtt));

      undisplayedAttributes.remove (theAtt);
      }

//------------------------------------------------------------------------------
// defaultValText (theAtt)
//
// Returns the default value for theAtt, converted to a String, or "None", if
// theAtt does not have a default value.
//------------------------------------------------------------------------------

   private String defaultValText (Attribute <?> theAtt)
      {
      Class <?> theValueType;

      theValueType = theAtt.getValueType ();

      if (! theAtt.hasDefaultValue ())
         {
         return "None";
         }

      if   ((theValueType == Boolean.class)
         || (theValueType == Integer.class)
         || (theValueType ==  Double.class))
         {
         return theAtt.getDefaultValue (myProblem).toString ();
         }

      if (theValueType == String.class)
         {
         return "\"" + theAtt.getDefaultValue (myProblem) + "\"";
         }

      if (theValueType == boolean[].class)
         {
         return defaultValTextBV (theAtt.asAttOfType (boolean[].class));
         }

      if (theValueType == int[].class)
         {
         return defaultValTextIV (theAtt.asAttOfType (int[].class));
         }

      if (theValueType == double[].class)
         {
         return defaultValTextDV (theAtt.asAttOfType (double[].class));
         }

      if (theValueType == BoundSet.class)
         {
         return defaultValTextBS (theAtt.asAttOfType (BoundSet.class));
         }

      throw new AssertionError ();
      }

//------------------------------------------------------------------------------
// defaultValTextBV (theBoolVecAtt)
//
// Returns the default value for theBoolVecAtt, converted to a String.
//------------------------------------------------------------------------------

   private String defaultValTextBV (Attribute <boolean[]> theBoolVecAtt)
      {
      boolean[] theBoolVec;

      theBoolVec = theBoolVecAtt.getDefaultValue (myProblem);

      return
         Boolean.toString (theBoolVec[0])
       + " "
       + Boolean.toString (theBoolVec[1])
       + " "
       + Boolean.toString (theBoolVec[2]);
      }

//------------------------------------------------------------------------------
// defaultValTextIV (theIntVecAtt)
//
// Returns the default value for theIntVecAtt, converted to a String.
//------------------------------------------------------------------------------

   private String defaultValTextIV (Attribute <int[]> theIntVecAtt)
      {
      int[] theIntVec;

      theIntVec = theIntVecAtt.getDefaultValue (myProblem);

      return
         Integer.toString (theIntVec[0])
       + " "
       + Integer.toString (theIntVec[1])
       + " "
       + Integer.toString (theIntVec[2]);
      }

//------------------------------------------------------------------------------
// defaultValTextDV (theDblVecAtt)
//
// Returns the default value for theDblVecAtt, converted to a String.
//------------------------------------------------------------------------------

   private String defaultValTextDV (Attribute <double[]> theDblVecAtt)
      {
      double[] theDblVec;

      theDblVec = theDblVecAtt.getDefaultValue (myProblem);

      return
         Double.toString (theDblVec[0])
       + " "
       + Double.toString (theDblVec[1])
       + " "
       + Double.toString (theDblVec[2]);
      }

//------------------------------------------------------------------------------
// defaultValTextBS (theBSetAtt)
//
// Returns the default value for theBSetAtt, converted to a String.
//------------------------------------------------------------------------------

   private String defaultValTextBS (Attribute <BoundSet> theBSetAtt)
      {
      double[] hardLB = new double[3];
      double[] softLB = new double[3];
      double[] hardUB = new double[3];

      theBSetAtt.getDefaultBoundSet (myProblem, hardLB, softLB, hardUB);

      return
         "\n"
       + "         hardLB: "
       + " " + Double.toString (hardLB[0])
       + " " + Double.toString (hardLB[1])
       + " " + Double.toString (hardLB[2])
       + "\n"
       + "         softLB: "
       + " " + Double.toString (softLB[0])
       + " " + Double.toString (softLB[1])
       + " " + Double.toString (softLB[2])
       + "\n"
       + "         hardUB: "
       + " " + Double.toString (hardUB[0])
       + " " + Double.toString (hardUB[1])
       + " " + Double.toString (hardUB[2]);
      }

//------------------------------------------------------------------------------
// checkForNonAppAttributes ()
//
// Displays all Attributes that don't apply to any Component class.
//------------------------------------------------------------------------------

   private void checkForNonAppAttributes ()
      {
      boolean nonAppFound;
      boolean appFound;

      nonAppFound = false;

      for (Attribute <?> theAtt: getAttributes ())
         {
         appFound = false;

         for (Class <? extends Component> theClass: allComponentClasses)
            {
            if (theAtt.appliesTo (theClass))
               appFound = true;
            }

         if (! appFound)
            {
            if (! nonAppFound)
               println (
                  "\n"
                + "The following Attributes do not apply to any Component "
                + "class:\n");

            nonAppFound = true;

            println ("   " + theAtt);
            }
         }

      if (! nonAppFound)
         println (
            "\n"
          + "All Attributes apply to at least one Component class.");
      }

//------------------------------------------------------------------------------
// verifyDefaultValues ()
//
// Verifies that WIT-J's default value for each Attribute matches that in WIT.
//------------------------------------------------------------------------------

   private void verifyDefaultValues ()
      {
      Attribute <?>[]         theAttsToSkipArr;
      HashSet <Attribute <?>> theAttsToSkip;

      println (
         "\n"
       + "Verifying Attribute default values:");

      theAttsToSkipArr =
         new Attribute <?>[]
            {
            CURRENT_SCENARIO,
            MULTI_OBJ_TOL,
            N_PERIODS,
            OBJECT_STAGE,
            OBJECTIVE_LIST_SPEC,
            PERIOD_STAGE,
            PROBABILITY,
            };

      theAttsToSkip =
         new HashSet <Attribute <?>> (Arrays.asList (theAttsToSkipArr));

      for (Attribute <?> theAtt: getAttributes ())
         {
         if (! theAtt.hasDefaultValue ())
            continue;

         if (theAttsToSkip.contains (theAtt))
            continue;

         println ("   " + theAtt);

         verifyDefaultValue (theAtt, myProblem);
         verifyDefaultValue (theAtt, myPartC);
         verifyDefaultValue (theAtt, myOpnF);
         verifyDefaultValue (theAtt, myBomEntFA);
         verifyDefaultValue (theAtt, mySubFAB);
         verifyDefaultValue (theAtt, myBopEntFC);
         verifyDefaultValue (theAtt, myDemandH);
         }

      println ("");
      }

//------------------------------------------------------------------------------
// verifyDefaultValue (theAtt, theComp)
//
// If theAtt appliesTo theComp, this method verifies that WIT-J's default value
// for theAtt matches that in WIT.
//------------------------------------------------------------------------------

   private void verifyDefaultValue (Attribute <?> theAtt, Component theComp)
      {
      if (! theAtt.appliesTo (theComp.getClass ()))
         return;

      verifyBoolDefault     (theAtt, theComp);
      verifyIntDefault      (theAtt, theComp);
      verifyDblDefault      (theAtt, theComp);
      verifyStringDefault   (theAtt, theComp);
      verifyBoolVecDefault  (theAtt, theComp);
      verifyIntVecDefault   (theAtt, theComp);
      verifyDblVecDefault   (theAtt, theComp);
      verifyBoundSetDefault (theAtt, theComp);
      }

//------------------------------------------------------------------------------
// verify{ValueType}Default methods
//
// Each of these methods checks that theAtt has the indicated value type and,
// if so, verifies that WIT-J's default value for theAtt matches that in WIT.
//------------------------------------------------------------------------------

   private void verifyBoolDefault (Attribute <?> theAtt, Component theComp)
      {
      Attribute <Boolean> theBoolAtt;
      Boolean             witjValue;
      Boolean             witValue;
      
      if (theAtt.getValueType () != Boolean.class)
         return;

      theBoolAtt = theAtt.asAttOfType (Boolean.class);

      witjValue  = theBoolAtt.getDefaultValue (myProblem);

      witValue   = theComp.get (theBoolAtt);

      assert (witjValue == witValue);
      }

   private void verifyIntDefault (Attribute <?> theAtt, Component theComp)
      {
      Attribute <Integer> theIntAtt;
      Integer             witjValue;
      Integer             witValue;
      
      if (theAtt.getValueType () != Integer.class)
         return;

      theIntAtt = theAtt.asAttOfType (Integer.class);

      witjValue = theIntAtt.getDefaultValue (myProblem);

      witValue  = theComp.get (theIntAtt);

      assert (witjValue == witValue);
      }

   private void verifyDblDefault (Attribute <?> theAtt, Component theComp)
      {
      Attribute <Double> theDblAtt;
      Double             witjValue;
      Double             witValue;
      
      if (theAtt.getValueType () != Double.class)
         return;

      theDblAtt = theAtt.asAttOfType (Double.class);

      witjValue = theDblAtt.getDefaultValue (myProblem);

      witValue  = theComp.get (theDblAtt);

      assertApproxEq (witjValue, witValue);
      }

   private void verifyStringDefault (Attribute <?> theAtt, Component theComp)
      {
      Attribute <String> theStringAtt;
      String             witjValue;
      String             witValue;
      
      if (theAtt.getValueType () != String.class)
         return;

      theStringAtt = theAtt.asAttOfType (String.class);

      witjValue = theStringAtt.getDefaultValue (myProblem);

      witValue  = theComp.get (theStringAtt);

      assert (witjValue.equals (witValue));
      }

   private void verifyBoolVecDefault (Attribute <?> theAtt, Component theComp)
      {
      Attribute <boolean[]> theBoolVecAtt;
      boolean[]             witjValue;
      boolean[]             witValue;
      
      if (theAtt.getValueType () != boolean[].class)
         return;

      theBoolVecAtt = theAtt.asAttOfType (boolean[].class);

      witjValue     = theBoolVecAtt.getDefaultValue (myProblem);

      witValue      = theComp.get (theBoolVecAtt);

      assertEq (witjValue, witValue);
      }

   private void verifyIntVecDefault (Attribute <?> theAtt, Component theComp)
      {
      Attribute <int[]> theIntVecAtt;
      int[]             witjValue;
      int[]             witValue;
      
      if (theAtt.getValueType () != int[].class)
         return;

      theIntVecAtt = theAtt.asAttOfType (int[].class);

      witjValue    = theIntVecAtt.getDefaultValue (myProblem);

      witValue     = theComp.get (theIntVecAtt);

      assertEq (witjValue, witValue);
      }

   private void verifyDblVecDefault (Attribute <?> theAtt, Component theComp)
      {
      Attribute <double[]> theDblVecAtt;
      double[]             witjValue;
      double[]             witValue;
      
      if (theAtt.getValueType () != double[].class)
         return;

      theDblVecAtt = theAtt.asAttOfType (double[].class);

      witjValue    = theDblVecAtt.getDefaultValue (myProblem);

      witValue     = theComp.get (theDblVecAtt);

      assertApproxEq (witjValue, witValue);
      }

   private void verifyBoundSetDefault (Attribute <?> theAtt, Component theComp)
      {
      Attribute <BoundSet> theBSetAtt;
      double[]             witjHardLB;
      double[]             witjSoftLB;
      double[]             witjHardUB;
      double[]             witHardLB;
      double[]             witSoftLB;
      double[]             witHardUB;
      
      if (theAtt.getValueType () != BoundSet.class)
         return;

      theBSetAtt = theAtt.asAttOfType (BoundSet.class);

      witjHardLB = new double[3];
      witjSoftLB = new double[3];
      witjHardUB = new double[3];
      witHardLB  = new double[3];
      witSoftLB  = new double[3];
      witHardUB  = new double[3];

      theBSetAtt.
         getDefaultBoundSet (myProblem, witjHardLB, witjSoftLB, witjHardUB);

      theComp.getBoundSet   (theBSetAtt, witHardLB,  witSoftLB,  witHardUB);

      assertApproxEq (witjHardLB, witHardLB);
      assertApproxEq (witjSoftLB, witSoftLB);
      assertApproxEq (witjHardUB, witHardUB);
      }

//------------------------------------------------------------------------------
// testIsValidFor ()
//
// Tests the isValidFor method of class Attribute <V>.
//------------------------------------------------------------------------------

   private void testIsValidFor ()
      {
      boolean isValid;

      //------------------------------------------------------------------------

      isValid = DEMAND_NAME.isValidFor (myOpnF);

      assert (! isValid);
         //
         // DEMAND_NAME does not apply to class Operation.

      //------------------------------------------------------------------------

      isValid = BUILD_ASAP.isValidFor (myPartA);

      assert (! isValid);
         //
         // BUILD_ASAP requires a material Part and myPartA is a capacity part.

      //------------------------------------------------------------------------

      isValid = PROBABILITY.isValidFor (myPartA);

      assert (! isValid);
         //
         // PROBABILITY requires stochastic mode.

      //------------------------------------------------------------------------

      isValid = OBJECTIVE_LIST_SPEC.isValidFor (myProblem);

      assert (! isValid);
         //
         // OBJECTIVE_LIST_SPEC requires multiple objectives mode.

      //------------------------------------------------------------------------

      myProblem.set (STAGE_BY_OBJECT, true);
      myProblem.set (STOCH_MODE,      true);

      isValid = PERIOD_STAGE.isValidFor (myProblem);

      assert (! isValid);

      isValid = OBJECT_STAGE.isValidFor (myPartA);

      assert (isValid);

      myProblem.set (STOCH_MODE,      false);
      myProblem.set (STAGE_BY_OBJECT, false);
      myProblem.set (STOCH_MODE,      true);

      isValid = PERIOD_STAGE.isValidFor (myProblem);

      assert (isValid);

      isValid = OBJECT_STAGE.isValidFor (myPartA);

      assert (! isValid);

      myProblem.set (STOCH_MODE, false);
      }

//------------------------------------------------------------------------------
// testSetGet ()
//
// Tests Attribute set... and get... methods of class Component.
//------------------------------------------------------------------------------

   private void testSetGet ()
      {
      testSetGetBool    ();
      testSetGetInt     ();
      testSetGetDbl     ();
      testSetGetBoolVec ();
      testSetGetIntVec  ();
      testSetGetDblVec  ();
      testSetGetStr     ();
      }

//------------------------------------------------------------------------------
// testSetGetBool ()
//
// Tests Attribute set... and get... methods for Attributes of value type
// Boolean.
//------------------------------------------------------------------------------

   private void testSetGetBool ()
      {
      boolean multiRouteOut;

      myProblem.set (MULTI_ROUTE, true);

      multiRouteOut = myProblem.get (MULTI_ROUTE);

      assert (multiRouteOut);

      myProblem.set (MULTI_ROUTE, false);
      }

//------------------------------------------------------------------------------
// testSetGetInt ()
//
// Tests Attribute set... and get... methods for Attributes of value type
// Integer.
//------------------------------------------------------------------------------

   private void testSetGetInt ()
      {
      int pipRankOut;

      myOpnF.set (PIP_RANK, 7);

      pipRankOut = myOpnF.get (PIP_RANK);

      assert (pipRankOut == 7);

      myOpnF.set (PIP_RANK, 0);
      }

//------------------------------------------------------------------------------
// testSetGetDbl ()
//
// Tests Attribute set... and get... methods for Attributes of value type
// Double.
//------------------------------------------------------------------------------

   private void testSetGetDbl ()
      {
      double expAversionOut;

      myBopEntFC.set (EXP_AVERSION, 2.3);
      
      expAversionOut = myBopEntFC.get (EXP_AVERSION);

      assertApproxEq (expAversionOut, 2.3);

      myBopEntFC.set (EXP_AVERSION, 0.0);
      }

//------------------------------------------------------------------------------
// testSetGetBoolVec ()
//
// Tests Attribute set... and get... methods for Attributes of value type
// boolean[].
//------------------------------------------------------------------------------

   private void testSetGetBoolVec ()
      {
      boolean[] propRtgIn;
      boolean[] propRtgOut;

      //------------------------------------------------------------------------

      propRtgIn  = new boolean[] {true, false, true};

      myPartB.set (PROP_RTG, propRtgIn);

      propRtgOut = myPartB.get (PROP_RTG);

      assertEq (propRtgOut, propRtgIn);

      //------------------------------------------------------------------------

      myPartB.setVectorToScalar (PROP_RTG, true);

      propRtgIn  = new boolean[] {true, true, true};

      propRtgOut = new boolean[3];

      myPartB.getVector (PROP_RTG, propRtgOut);

      assertEq (propRtgOut, propRtgIn);

      //------------------------------------------------------------------------

      myPartB.setVectorToScalar (PROP_RTG, false);
      }

//------------------------------------------------------------------------------
// testSetGetIntVec ()
//
// Tests Attribute set... and get... methods for Attributes of value type int[].
//------------------------------------------------------------------------------

   private void testSetGetIntVec ()
      {
      int[] priorityIn;
      int[] priorityOut;

      //------------------------------------------------------------------------

      priorityIn  = new int[] {2, 7, 3};

      myDemandH.set (PRIORITY, priorityIn);

      priorityOut = myDemandH.get (PRIORITY);

      assertEq (priorityOut, priorityIn);

      //------------------------------------------------------------------------

      myDemandH.setVectorToScalar (PRIORITY, 17);

      priorityIn  = new int[] {17, 17, 17};

      priorityOut = new int[3];

      myDemandH.getVector (PRIORITY, priorityOut);

      assertEq (priorityOut, priorityIn);

      //------------------------------------------------------------------------

      myDemandH.setVectorToScalar (PRIORITY, 0);
      }

//------------------------------------------------------------------------------
// testSetGetDblVec ()
//
// Tests Attribute set... and get... methods for Attributes of value type
// double[].
//------------------------------------------------------------------------------

   private void testSetGetDblVec ()
      {
      double[] supplyVolIn;
      double[] supplyVolOut;

      //------------------------------------------------------------------------

      supplyVolIn  = new double[] {0, 30, 10};

      myPartB.set (SUPPLY_VOL, supplyVolIn);

      supplyVolOut = myPartB.get (SUPPLY_VOL);

      assertApproxEq (supplyVolOut, supplyVolIn);

      //------------------------------------------------------------------------

      myPartB.setVectorToScalar (SUPPLY_VOL, 25);

      supplyVolIn  = new double[] {25, 25, 25};

      supplyVolOut = new double[3];

      myPartB.getVector (SUPPLY_VOL, supplyVolOut);
      
      assertApproxEq (supplyVolOut, supplyVolIn);

      //------------------------------------------------------------------------

      myPartB.setVectorToScalar (SUPPLY_VOL, 0.0);
      }

//------------------------------------------------------------------------------
// testSetGetStr ()
//
// Tests Attribute set... and get... methods for Attributes of value type
// String.
//------------------------------------------------------------------------------

   private void testSetGetStr ()
      {
      String titleOut;

      myProblem.set (TITLE, "testSetGetStr");

      titleOut = myProblem.get (TITLE);

      assert (titleOut.equals ("testSetGetStr"));

      myProblem.set (TITLE, "Untitled");
      }

//------------------------------------------------------------------------------
// testSpecAtts ()
//
// Tests special Attributes.
//------------------------------------------------------------------------------

   private void testSpecAtts ()
      {
      testPartName      ();
      testDemandName    ();
      testOperationName ();
      testBomIndex      ();
      testSubIndex      ();
      testBopIndex      ();
      testStockBounds   ();
      }

//------------------------------------------------------------------------------
// testPartName ()
//
// Tests the PART_NAME Attribute.
//------------------------------------------------------------------------------

   private void testPartName ()
      {
      String theName;
                                        
      myPartC.set (PART_NAME, "Temporary Part Name");

      theName = myPartC.get (PART_NAME);

      assert (theName.equals ("Temporary Part Name"));

      myPartC.set (PART_NAME, "C");
      }

//------------------------------------------------------------------------------
// testDemandName ()
//
// Tests the DEMAND_NAME Attribute.
//------------------------------------------------------------------------------

   private void testDemandName ()
      {
      String theName;
                                        
      myDemandJ.set (DEMAND_NAME, "Temporary Demand Name");

      theName = myDemandJ.get (DEMAND_NAME);

      assert (theName.equals ("Temporary Demand Name"));

      myDemandJ.set (DEMAND_NAME, "J");
      }

//------------------------------------------------------------------------------
// testOperationName ()
//
// Tests the OPERATION_NAME Attribute.
//------------------------------------------------------------------------------

   private void testOperationName ()
      {
      String theName;
                                        
      myOpnG.set (OPERATION_NAME, "Temporary Operation Name");

      theName = myOpnG.get (OPERATION_NAME);

      assert (theName.equals ("Temporary Operation Name"));

      myOpnG.set (OPERATION_NAME, "G");
      }

//------------------------------------------------------------------------------
// testBomIndex ()
//
// Tests the BOM_INDEX Attribute.
//------------------------------------------------------------------------------

   private void testBomIndex ()
      {
      int theBomIndex;

      theBomIndex = myBomEntFB2.get (BOM_INDEX);

      assert (theBomIndex == 2);
      }

//------------------------------------------------------------------------------
// testSubIndex ()
//
// Tests the SUB_INDEX Attribute.
//------------------------------------------------------------------------------

   private void testSubIndex ()
      {
      int theSubIndex;

      theSubIndex = mySubFAB.get (SUB_INDEX);

      assert (theSubIndex == 0);
      }

//------------------------------------------------------------------------------
// testBopIndex ()
//
// Tests the BOP_INDEX Attribute.
//------------------------------------------------------------------------------

   private void testBopIndex ()
      {
      int theBopIndex;

      theBopIndex = myBopEntFG.get (BOP_INDEX);

      assert (theBopIndex == 1);
      }

//------------------------------------------------------------------------------
// testStockBounds ()
//
// Tests the STOCK_BOUNDS Attribute.
//------------------------------------------------------------------------------

   private void testStockBounds ()
      {
      double[] zeroArr;
      double[] neg1Arr;
      double[] softLBin;
      double[] hardUBin;
      double[] hardLBout;
      double[] softLBout;
      double[] hardUBout;

      zeroArr   = new double[] {  0.0,   0.0,   0.0};
      neg1Arr   = new double[] { -1.0,  -1.0,  -1.0};
      softLBin  = new double[] {140.0, 150.0, 160.0};
      hardUBin  = new double[] {170.0, 180.0, 190.0};

      hardLBout = new double[3];
      softLBout = new double[3];
      hardUBout = new double[3];

      myPartC.setBoundSet (STOCK_BOUNDS, null,      softLBin,  hardUBin);
      myPartC.getBoundSet (STOCK_BOUNDS, hardLBout, softLBout, hardUBout);

      assertApproxEq (hardLBout, zeroArr);
      assertApproxEq (softLBout, softLBin);
      assertApproxEq (hardUBout, hardUBin);

      myPartC.setBoundSetToScalars (STOCK_BOUNDS, 0, 0, -1);

      myPartC.getBoundSet (STOCK_BOUNDS, hardLBout, softLBout, hardUBout);

      assertApproxEq (hardLBout, zeroArr);
      assertApproxEq (softLBout, zeroArr);
      assertApproxEq (hardUBout, neg1Arr);
      }

//------------------------------------------------------------------------------
// testOptInitMethod ()
//
// Tests OptInitMethod
//------------------------------------------------------------------------------

   private void testOptInitMethod ()
      {
      OptInitMethod theMethod;

      myProblem.setOptInitMethod (OptInitMethod.CRASH);

      theMethod = myProblem.getOptInitMethod ();

      assert (theMethod.equals (OptInitMethod.CRASH));

      myProblem.setOptInitMethod (OptInitMethod.HEURISTIC);
      }

//------------------------------------------------------------------------------
// testClearData ()
//
// Tests theProblem.clearData ().
//------------------------------------------------------------------------------

   private void testClearData ()
      {
      myProblem.clearData ();

      myProblem.set (N_PERIODS, 3);

      buildComponents ();
      }

//------------------------------------------------------------------------------
// testCopyData ()
//
// Tests theProblem.copyData (...).
//------------------------------------------------------------------------------

   private void testCopyData ()
      {
      Problem cpyProblem;

      myProblem.copyData (myProblem);

      cpyProblem = Problem.newInstance ();

      cpyProblem.getMessageMgr ().setMesgFileName ("copyData.output");

      cpyProblem.copyData (myProblem);

      println ("Components for the copied Problem:");

      for (Component theComp: cpyProblem.getComponents ())
         {
         println ("   " + theComp);
         }

      println ("");

      cpyProblem.deactivate ();
      }

//------------------------------------------------------------------------------
// testCopyCompData ()
//
// Tests theComp.copyComponentData (...).
//------------------------------------------------------------------------------

   private void testCopyCompData ()
      {
      Problem    theProblem;

      Part       thePartA1;
      Part       thePartA2;
      Demand     theDemandD1;
      Demand     theDemandD2;
      Operation  theOpnF1;
      Operation  theOpnF2;
      BomEntry   theBomEntFA1;
      BomEntry   theBomEntFA2;
      Substitute theSubFAA1;
      Substitute theSubFAA2;
      BopEntry   theBopEntFA1;
      BopEntry   theBopEntFA2;

      theProblem   = Problem   .newInstance ();

      theProblem.set (N_PERIODS, 3);

      thePartA1    = Part      .newInstance (theProblem,  "A1", CAPACITY);
      thePartA2    = Part      .newInstance (theProblem,  "A2", CAPACITY);
      theDemandD1  = Demand    .newInstance (thePartA1,   "D1");
      theDemandD2  = Demand    .newInstance (thePartA2,   "D2");
      theOpnF1     = Operation .newInstance (theProblem,  "F1");
      theOpnF2     = Operation .newInstance (theProblem,  "F2");
      theBomEntFA1 = BomEntry  .newInstance (theOpnF1,     thePartA1);
      theBomEntFA2 = BomEntry  .newInstance (theOpnF2,     thePartA2);
      theSubFAA1   = Substitute.newInstance (theBomEntFA1, thePartA1);
      theSubFAA2   = Substitute.newInstance (theBomEntFA2, thePartA2);
      theBopEntFA1 = BopEntry  .newInstance (theOpnF1,     thePartA1);
      theBopEntFA2 = BopEntry  .newInstance (theOpnF2,     thePartA2);

      thePartA1   .copyComponentData  (myPartA);
      thePartA2   .copyComponentData (thePartA1);

      theDemandD1 .copyComponentData  (myDemandH);
      theDemandD2 .copyComponentData (theDemandD1);

      theOpnF1    .copyComponentData  (myOpnF);
      theOpnF2    .copyComponentData (theOpnF1);

      theBomEntFA1.copyComponentData  (myBomEntFA);
      theBomEntFA2.copyComponentData (theBomEntFA1);

      theSubFAA1  .copyComponentData  (mySubFAB);
      theSubFAA2  .copyComponentData (theSubFAA1);

      theBopEntFA1.copyComponentData  (myBopEntFC);
      theBopEntFA2.copyComponentData (theBopEntFA1);

      theProblem.displayData ("");

      theProblem.deactivate ();
      }

//------------------------------------------------------------------------------
// testExpCycle ()
//
// Tests theProblem.getExpCycle ().
//------------------------------------------------------------------------------

   private void testExpCycle ()
      {
      Operation theOpnI;
      ArrayList <Part>      thePartList;
      ArrayList <Operation> theOpnList;
      int                   theIdx;

      theOpnI = Operation.newInstance (myProblem,  "I");

      BomEntry.newInstance (theOpnI, myPartC);
      BopEntry.newInstance (theOpnI, myPartA);

      thePartList = new ArrayList <Part>      (0);
      theOpnList  = new ArrayList <Operation> (0);

      myProblem.getExpCycle (thePartList, theOpnList);

      println ("Explodable Cycle:\n");

      for (theIdx = 0; theIdx < thePartList.size (); theIdx ++)
         {
         println ("   " + thePartList.get (theIdx));
         println ("   " + theOpnList .get (theIdx));
         }

      println ("");

      theOpnI.set (SEL_FOR_DEL, true);

      myProblem.purgeData ();
      }

//------------------------------------------------------------------------------
// setAttributes ()
//
// Sets various Attributes of the Components of myProblem in order to form an
// implosion problem appropriate for the unit test.
//------------------------------------------------------------------------------

   private void setAttributes ()
      {
      myProblem  .set (COMPUTE_CRITICAL_LIST, true);
      myProblem  .set (PGD_CRIT_LIST_MODE,    true);
      myProblem  .set (PERF_PEGGING,          true);
      myProblem  .set (SKIP_FAILURES,         false);
      myPartE    .set (BUILD_NSTN,            true);
      myPartA    .set (SUPPLY_VOL,            new double[] {10, 10, 10});
      myPartB    .set (SUPPLY_VOL,            new double[] {60,  0, 50});
      myOpnG     .set (PIP_ENABLED,           true);
      myBomEntFB2.set (CONS_RATE,             new double[] { 0,  0,  0});
      myBopEntFG .set (EXP_ALLOWED,           false);
      myDemandH  .set (DEMAND_VOL,            new double[] {20,  0, 30});
      myDemandI  .set (DEMAND_VOL,            new double[] {20, 10, 10});
      myDemandJ  .set (DEMAND_VOL,            new double[] {25,  0, 35});
      }

//------------------------------------------------------------------------------
// testBelowList ()
//
// Tests thePart.getBelowList (...).
//------------------------------------------------------------------------------

   private void testBelowList ()
      {
      ArrayList <Part> theBelowList;

      theBelowList = myPartC.getBelowList ();

      println ("Below List for Part C:\n");

      for (Part thePart: theBelowList)
         println ("   " + thePart);

      println ("");
      }

//------------------------------------------------------------------------------
// testHeurAlloc ()
//
// Tests heuristic allocation.
//------------------------------------------------------------------------------

   private void testHeurAlloc ()
      {
      boolean activeVal;
      double  incVol;

      myProblem.startHeurAlloc ();

      activeVal = myProblem.get (HEUR_ALLOC_ACTIVE);

      assert (activeVal);

      incVol = myDemandH.incHeurAlloc (2, 90);

      myProblem.shutDownHeurAlloc ();

      activeVal = myProblem.get (HEUR_ALLOC_ACTIVE);

      assert (! activeVal);

      println ("incVol = " + incVol);
      println ();
      }

//------------------------------------------------------------------------------
// testEqHeurAlloc ()
//
// Tests equitable heuristic allocation.
//------------------------------------------------------------------------------

   private void testEqHeurAlloc ()
      {
      ArrayList <Demand>  theDemandList;
      ArrayList <Integer> shipPeriodList;
      ArrayList <Double>  desIncVolList;
      ArrayList <Double>  incVolList;

      theDemandList  = new ArrayList <Demand>  (2);
      shipPeriodList = new ArrayList <Integer> (2);
      desIncVolList  = new ArrayList <Double>  (2);
      incVolList     = new ArrayList <Double>  (0);

      theDemandList .add (myDemandH);
      shipPeriodList.add (2);
      desIncVolList .add (50.0);

      theDemandList .add (myDemandI);
      shipPeriodList.add (2);
      desIncVolList .add (40.0);

      myProblem.set (EQUITABILITY, 100);

      myProblem.startHeurAlloc ();

      myProblem.
         eqHeurAlloc (
            theDemandList,
            shipPeriodList,
            desIncVolList,
            incVolList);

      myProblem.finishHeurAlloc ();

      myProblem.set (EQUITABILITY, 1);

      println ("incVolList.get (0) = " + incVolList.get (0));
      println ("incVolList.get (1) = " + incVolList.get (1));
      println ();
      }

//------------------------------------------------------------------------------
// testPip ()
//
// Tests PIP.
//------------------------------------------------------------------------------

   private void testPip ()
      {
      testPipShipSeq ();

      myProblem.buildPip ();

      testConsVolPip   ();
      testCoExecVolPip ();
      testExecVolPip   ();
      testProdVolPip   ();
      testSideVolPip   ();
      testSubVolPip    ();
      testSupplyVolPip ();
      }

//------------------------------------------------------------------------------
// testPipShipSeq ()
//
// Tests the methods for storing and retrieving the PIP shipment sequence.
//------------------------------------------------------------------------------

   private void testPipShipSeq ()
      {
      ArrayList <PeggingTriple <Demand>> theTripleList;

      theTripleList = new ArrayList <PeggingTriple <Demand>> (0);

      myDemandH.appendToPipSeq (1, 14);

      myProblem.clearPipSeq ();

      myDemandH.appendToPipSeq (0, 20);
      myDemandI.appendToPipSeq (0, 20);
      myDemandH.appendToPipSeq (2, 30);
      myDemandI.appendToPipSeq (2, 10);
      myDemandJ.appendToPipSeq (0, 20);
      myDemandJ.appendToPipSeq (2, 30);

      theTripleList = myProblem.getPipSeq ();

      println ("Shipment Sequence:\n");

      for (PeggingTriple <Demand> theTriple: theTripleList)
         {
         println (
           "   "
          + theTriple.getRoot   ()
          + ", Ship Period = "
          + theTriple.getPeriod ()
          + ", Inc ShipVol = "
          + theTriple.getVolume ());
         }

      println ();
      }

//------------------------------------------------------------------------------
// testConsVolPip ()
//
// Tests Demand   .getConsVolPip.
// Tests Operation.getConsVolPip.
//------------------------------------------------------------------------------

   private void testConsVolPip ()
      {
      ArrayList <PeggingTriple <Part>> theTripleList;

      theTripleList = myDemandH.getConsVolPip (2);

      printPegging ("ConsVol PIP", myDemandH, 2, theTripleList);

      theTripleList = myOpnG   .getConsVolPip (2);

      printPegging ("ConsVol PIP", myOpnG,    2, theTripleList);
      }

//------------------------------------------------------------------------------
// testCoExecVolPip ()
//
// Tests Demand   .getCoExecVolPip.
// Tests Operation.getCoExecVolPip.
//------------------------------------------------------------------------------

   private void testCoExecVolPip ()
      {
      ArrayList <PeggingTriple <BopEntry>> theTripleList;

      theTripleList = myDemandH.getCoExecVolPip (2);

      printPegging ("CoExecVol PIP", myDemandH, 2, theTripleList);

      theTripleList = myOpnG   .getCoExecVolPip (2);

      printPegging ("CoExecVol PIP", myOpnG,    2, theTripleList);
      }

//------------------------------------------------------------------------------
// testExecVolPip ()
//
// Tests Demand   .getExecVolPip.
// Tests Operation.getExecVolPip.
//------------------------------------------------------------------------------

   private void testExecVolPip ()
      {
      ArrayList <PeggingTriple <Operation>> theTripleList;

      theTripleList = myDemandH.getExecVolPip (2);

      printPegging ("ExecVol PIP", myDemandH, 2, theTripleList);

      theTripleList = myOpnG   .getExecVolPip (2);

      printPegging ("ExecVol PIP", myOpnG,    2, theTripleList);
      }

//------------------------------------------------------------------------------
// testProdVolPip ()
//
// Tests Demand   .getProdVolPip.
// Tests Operation.getProdVolPip.
//------------------------------------------------------------------------------

   private void testProdVolPip ()
      {
      ArrayList <PeggingTriple <Part>> theTripleList;

      theTripleList = myDemandH.getProdVolPip (2);

      printPegging ("ProdVol PIP", myDemandH, 2, theTripleList);

      theTripleList = myOpnG   .getProdVolPip (2);

      printPegging ("ProdVol PIP", myOpnG,    2, theTripleList);
      }

//------------------------------------------------------------------------------
// testSideVolPip ()
//
// Tests Demand   .getSideVolPip.
// Tests Operation.getSideVolPip.
//------------------------------------------------------------------------------

   private void testSideVolPip ()
      {
      ArrayList <PeggingTriple <Part>> theTripleList;

      theTripleList = myDemandJ.getSideVolPip (2);

      printPegging ("SideVol PIP", myDemandJ, 2, theTripleList);

      theTripleList = myOpnG   .getSideVolPip (2);

      printPegging ("SideVol PIP", myOpnG,    2, theTripleList);
      }

//------------------------------------------------------------------------------
// testSubVolPip ()
//
// Tests Demand   .getSubVolPip.
// Tests Operation.getSubVolPip.
//------------------------------------------------------------------------------

   private void testSubVolPip ()
      {
      ArrayList <PeggingTriple <Substitute>> theTripleList;

      theTripleList = myDemandH.getSubVolPip (2);

      printPegging ("SubVol PIP", myDemandH, 2, theTripleList);

      theTripleList = myOpnG   .getSubVolPip (2);

      printPegging ("SubVol PIP", myOpnG,    2, theTripleList);
      }

//------------------------------------------------------------------------------
// testSupplyVolPip ()
//
// Tests Demand   .getSupplyVolPip.
// Tests Operation.getSupplyVolPip.
//------------------------------------------------------------------------------

   private void testSupplyVolPip ()
      {
      ArrayList <PeggingTriple <Part>> theTripleList;
      double                           supVolPip;

      theTripleList = myDemandH.getSupplyVolPip (2);

      printPegging ("SupplyVol PIP", myDemandH, 2, theTripleList);

      supVolPip = getSpecificSupplyVolPip (myPartB, 2, myDemandH, 2);

      assertApproxEq (supVolPip, 40.0);

      supVolPip = getSpecificSupplyVolPip (myPartB, 1, myDemandH, 2);

      assertApproxEq (supVolPip,  0.0);

      theTripleList = myOpnG   .getSupplyVolPip (2);

      printPegging ("SupplyVol PIP", myOpnG,    2, theTripleList);
      }

//------------------------------------------------------------------------------
// testCPeg ()
//
// Tests concurrent pegging.
//------------------------------------------------------------------------------

   private void testCPeg ()
      {
      testExecVolCPeg ();
      testSubVolCPeg  ();

      myProblem.clearPegging ();

      testExecVolCPeg ();
      }

//------------------------------------------------------------------------------
// testExecVolCPeg ()
//
// Tests Demand.getExecVolPegging.
//------------------------------------------------------------------------------

   private void testExecVolCPeg ()
      {
      ArrayList <PeggingTriple <Operation>> theTripleList;

      theTripleList = myDemandH.getExecVolPegging (2);

      printPegging ("ExecVol Concurrent Pegging", myDemandH, 2, theTripleList);
      }

//------------------------------------------------------------------------------
// testSubVolCPeg ()
//
// Tests Demand.getSubVolPegging.
//------------------------------------------------------------------------------

   private void testSubVolCPeg ()
      {
      ArrayList <PeggingTriple <Substitute>> theTripleList;

      theTripleList = myDemandH.getSubVolPegging (2);

      printPegging ("SubVol Concurrent Pegging", myDemandH, 2, theTripleList);
      }

//------------------------------------------------------------------------------
// printPegging (pggTypeName, theComp, shipPer, theTripleList)
//
// Prints the pegging for theComp in shipPer given in theTripleList
// thePggCaseName indicates the pegging case.
//------------------------------------------------------------------------------

   private <C extends Component> void printPegging (
         String                        thePggCaseName,
         Component                     theComp,
         int                           shipPer,
         ArrayList <PeggingTriple <C>> theTripleList)
      {
      println (
         "\n"
       + thePggCaseName
       + " for "
       + theComp
       + " in Period "
       +  shipPer
       + ":\n");

      for (PeggingTriple <C> theTriple: theTripleList)
         println ("   " + theTriple);
      }

//------------------------------------------------------------------------------
// testCriticalList ()
//
// Tests Problem.getCriticalList (...).
//------------------------------------------------------------------------------

   private void testCriticalList ()
      {
      ArrayList <Part>    critPartList;
      ArrayList <Integer> critPeriodList;
      int                 theIdx;

      critPartList   = new ArrayList <Part>    (0);
      critPeriodList = new ArrayList <Integer> (0);

      critPartList  .add (myPartC);
      critPeriodList.add (17);
      critPeriodList.add (92);
         //
         // These should be cleared by getCriticalList.

      myProblem.getCriticalList (critPartList, critPeriodList);

      println ("Critical Parts List:");

      for (theIdx = 0; theIdx < critPartList.size (); theIdx ++)
         println (
            "   "          + critPartList  .get (theIdx)
          + " in Period "  + critPeriodList.get (theIdx));
      }

//------------------------------------------------------------------------------
// testPgdCritList ()
//
// Tests Problem.getPgdCritList (...).
//------------------------------------------------------------------------------

   private void testPgdCritList ()
      {
      ArrayList <Part>    critPartList;
      ArrayList <Integer> critPeriodList;
      ArrayList <Demand>  theDemandList;
      ArrayList <Integer> shipPeriodList;
      int                 theIdx;

      critPartList   = new ArrayList <Part>    (0);
      critPeriodList = new ArrayList <Integer> (0);
      theDemandList  = new ArrayList <Demand>  (0);
      shipPeriodList = new ArrayList <Integer> (0);

      critPartList  .add (myPartB);
      critPeriodList.add (9);
      theDemandList .add (myDemandI);
      shipPeriodList.add (21);

      critPartList  .add (myPartA);
      critPeriodList.add (12);
      theDemandList .add (myDemandJ);
      shipPeriodList.add (14);
         //
         // These should be cleared by getPgdCritList.

      myProblem.
         getPgdCritList (
            critPartList,
            critPeriodList,
            theDemandList,
            shipPeriodList);

      println ();
      println ("Pegged Critical List:");

      for (theIdx = 0; theIdx < critPartList.size (); theIdx ++)
         println (
            "   "         + critPartList  .get (theIdx)
          + " in Period " + critPeriodList.get (theIdx)
          + " blocked "   + theDemandList .get (theIdx)
          + " in Period " + shipPeriodList.get (theIdx)
          + ".");
      }

//------------------------------------------------------------------------------
// testWriteMethods ()
//
// Tests the "write" methods.
//------------------------------------------------------------------------------

   private void testWriteMethods ()
      {
      myProblem.writeData         ("unit-test.data");
      myProblem.preprocess        ();
      myProblem.displayData       ("");
      myProblem.writeExecSched    ("",               FileFormat.BSV);
      myProblem.writeExecSched    ("unit-test.exec", FileFormat.BSV);
      myProblem.writeShipSched    ("unit-test.ship", FileFormat.CSV);
      myProblem.writeCriticalList ("unit-test.crit", FileFormat.BSV, 100);
      myProblem.mrp               ();
      myProblem.writeReqSched     ("unit-test.req",  FileFormat.BSV);
      }

//------------------------------------------------------------------------------
// testMultiObjMode ()
//
// Tests WIT-J in multiple objectives mode.
//------------------------------------------------------------------------------

   private void testMultiObjMode ()
      {
      Problem theProblem;

      theProblem = Problem.newInstance ();

      theProblem.set (MULTI_OBJ_MODE, true);

      testObjectiveListSpec (theProblem);
      testObjectiveList     (theProblem);
      testCurObjObjSeqNo    (theProblem);

      theProblem.deactivate ();
      }

//------------------------------------------------------------------------------
// testObjectiveListSpec (theProblem)
//------------------------------------------------------------------------------

   private void testObjectiveListSpec (Problem theProblem)
      {
      String             theSpec;
      ArrayList <String> theObjNameList;

      theProblem.set (OBJECTIVE_LIST_SPEC, "First|Second|Third|Fourth");

      theSpec = theProblem.get (OBJECTIVE_LIST_SPEC);

      assert (theSpec.equals ("First|Second|Third|Fourth"));

      theObjNameList = theProblem.getObjectiveList ();

      assert (theObjNameList.size () == 4);

      assert (theObjNameList.get (0).equals ("First" ));
      assert (theObjNameList.get (1).equals ("Second"));
      assert (theObjNameList.get (2).equals ("Third" ));
      assert (theObjNameList.get (3).equals ("Fourth"));
      }

//------------------------------------------------------------------------------
// testObjectiveList (theProblem)
//------------------------------------------------------------------------------

   private void testObjectiveList (Problem theProblem)
      {
      String             theSpec;
      ArrayList <String> theObjNameList;

      theProblem.setObjectiveList ("High", "Medium", "Low");

      theSpec = theProblem.get (OBJECTIVE_LIST_SPEC);

      assert (theSpec.equals ("High|Medium|Low"));

      theObjNameList = theProblem.getObjectiveList ();

      assert (theObjNameList.size () == 3);

      assert (theObjNameList.get (0).equals ("High"  ));
      assert (theObjNameList.get (1).equals ("Medium"));
      assert (theObjNameList.get (2).equals ("Low"   ));

      //------------------------------------------------------------------------

      theObjNameList.clear ();

      theObjNameList.add ("A");
      theObjNameList.add ("B");
      theObjNameList.add ("C");

      theProblem.setObjectiveList (theObjNameList);

      theSpec = theProblem.get (OBJECTIVE_LIST_SPEC);

      assert (theSpec.equals ("A|B|C"));
      }

//------------------------------------------------------------------------------
// testCurObjObjSeqNo (theProblem)
//
// Tests CURRENT_OBJECTIVE and OBJECTIVE_SEQ_NO.
//------------------------------------------------------------------------------

   private void testCurObjObjSeqNo (Problem theProblem)
      {
      theProblem.set (CURRENT_OBJECTIVE, "B");
      theProblem.set (OBJECTIVE_SEQ_NO,   3);
      theProblem.set (CURRENT_OBJECTIVE, "C");
      theProblem.set (OBJECTIVE_SEQ_NO,   2);

      verifyObjSeqNo (theProblem, "A", 1);
      verifyObjSeqNo (theProblem, "B", 3);
      verifyObjSeqNo (theProblem, "C", 2);
      }

//------------------------------------------------------------------------------
// verifyObjSeqNo (theProblem, theObjName, theSeqNo)
//
// Verifies that the objective indicated by theObjName has OBJECTIVE_SEQ_NO
// equal to theSeqNo.
//------------------------------------------------------------------------------

   private void verifyObjSeqNo (
         Problem theProblem,
         String  theObjName,
         int     theSeqNo)
      {
      int curSeqNo;

      theProblem.set (CURRENT_OBJECTIVE, theObjName);

      curSeqNo = theProblem.get (OBJECTIVE_SEQ_NO);

      assert (curSeqNo == theSeqNo);
      }

//------------------------------------------------------------------------------
// displayNonDefGlobalBoolAtts (theProblem)
//
// For each global boolean Attribute, if the Attribute is currently valid but
// not at its default value in theProblem, this method displays the Attribute,
// its WIT name, its default value, and its current value.
//
// This method is used as an example in the WIT-J Guide.
//------------------------------------------------------------------------------

   private static void displayNonDefGlobalBoolAtts (Problem theProblem)
      {
      Attribute <Boolean> theBoolAtt;
      boolean             defValue;
      boolean             theValue;

      for (Attribute <?> theAtt: getAttributes ())
         {
         if (theAtt.isValidFor (theProblem))
            {
            if (theAtt.getValueType () == Boolean.class)
               {
               theBoolAtt = theAtt.asAttOfType (Boolean.class);

               if (theBoolAtt.hasDefaultValue ())
                  {
                  defValue = theBoolAtt.getDefaultValue (theProblem);

                  theValue = theProblem.get (theBoolAtt);

                  if (theValue != defValue)
                     {
                     System.out.printf (
                        "%n"
                      + "Non-default global boolean Attribute found.%n"
                      + "%n"
                      + "   Attribute:     %s%n"
                      + "   WIT Name:      %s%n"
                      + "   Default Value: %s%n"
                      + "   Current Value: %s%n",
                        theBoolAtt.toString (),
                        theBoolAtt.getWitName (),
                        defValue,
                        theValue);
                     }
                  }
               }
            }
         }
      }

//------------------------------------------------------------------------------
// getSpecificSupplyVolPip (thePart, supPer, theDemand, shipPer)
//
// Returns the amount of supplyVol of the supplyVol of thePart in supPer that
// has been pegged by PIP to theDemand in shipPer.
//
// This method is used as an example in the WIT-J Guide.
//------------------------------------------------------------------------------

   private static double getSpecificSupplyVolPip (
         Part   thePart,
         int    supPer,
         Demand theDemand,
         int    shipPer)
      {
      ArrayList <PeggingTriple <Part>> theTripleList;

      theTripleList = theDemand.getSupplyVolPip (shipPer);

      for (PeggingTriple <Part> theTriple: theTripleList)
         {
         if    (theTriple.getRoot   () == thePart)
            {
            if (theTriple.getPeriod () == supPer)
               {
               return theTriple.getVolume ();
               }
            }
         }

      return 0.0;
         //
         // If no PeggingTriple is found matching thePart and supPer, the amount
         // pegged is 0.
      }

//------------------------------------------------------------------------------
// testCplexParSpecs ()
//
// Tests methods for working with CPLEX parameters specifications.
//------------------------------------------------------------------------------

   private void testCplexParSpecs ()
      {
      Integer simDisplaySpec;
      Double  tilimSpec;

      simDisplaySpec = myProblem.getIntCplexParSpec ("SIMDISPLAY");
      tilimSpec      = myProblem.getDblCplexParSpec ("TILIM");

      assert (simDisplaySpec == null);
      assert (tilimSpec      == null);

      myProblem.addIntCplexParSpec ("SIMDISPLAY", 2);
      myProblem.addDblCplexParSpec ("TILIM",      10.0);

      simDisplaySpec = myProblem.getIntCplexParSpec ("SIMDISPLAY");
      tilimSpec      = myProblem.getDblCplexParSpec ("TILIM");

      assert (simDisplaySpec.   intValue () ==  2);
      assert (tilimSpec     .doubleValue () == 10.0);

      myProblem.clearCplexParSpecs ();

      simDisplaySpec = myProblem.getIntCplexParSpec ("SIMDISPLAY");
      tilimSpec      = myProblem.getDblCplexParSpec ("TILIM");

      assert (simDisplaySpec == null);
      assert (tilimSpec      == null);
      }

//------------------------------------------------------------------------------
// testSortedParts ()
//
// Tests Problem.getSortedParts ().
//
// Builds a separate Problem for this purpose.
//------------------------------------------------------------------------------

   private void testSortedParts ()
      {
      Problem          theProblem;
      List <Part>      unsortedPartList;
      ArrayList <Part> sortedPartList;

      theProblem       = Problem.newInstance ();

      buildProblemForSortTest (theProblem);

      unsortedPartList = theProblem.getParts       ();
      sortedPartList   = theProblem.getSortedParts ();

      println ("");

      println ("Unsorted Parts:\n");

      for (Part thePart: unsortedPartList)
         println ("   " + thePart);

      println ("");

      println ("Sorted Parts:\n");

      for (Part thePart: sortedPartList)
         println ("   " + thePart);

      println ("");

      theProblem.deactivate ();
      }

//------------------------------------------------------------------------------
// testSortedOperations ()
//
// Tests Problem.getSortedOperations ().
//
// Builds a separate Problem for this purpose.
//------------------------------------------------------------------------------

   private void testSortedOperations ()
      {
      Problem               theProblem;
      List <Operation>      unsortedOpnList;
      ArrayList <Operation> sortedOpnList;

      theProblem      = Problem.newInstance ();

      buildProblemForSortTest (theProblem);

      unsortedOpnList = theProblem.getOperations       ();
      sortedOpnList   = theProblem.getSortedOperations ();

      println ("");

      println ("Unsorted Operations:\n");

      for (Operation theOpn: unsortedOpnList)
         println ("   " + theOpn);

      println ("");

      println ("Sorted Operations:\n");

      for (Operation theOpn: sortedOpnList)
         println ("   " + theOpn);

      println ("");

      theProblem.deactivate ();
      }

//------------------------------------------------------------------------------
// buildProblemForSortTest (theProblem)
//
// Populates theProblem for testSortedParts ().
//------------------------------------------------------------------------------

   private void buildProblemForSortTest (Problem theProblem)
      {
      Part      thePartA;
      Part      thePartB;
      Part      thePartC;
      Operation theOpnD;
      Operation theOpnE;

      thePartA = Part     .newInstance (theProblem, "A", MATERIAL);
      thePartB = Part     .newInstance (theProblem, "B", MATERIAL);
      thePartC = Part     .newInstance (theProblem, "C", MATERIAL);

      theOpnD  = Operation.newInstance (theProblem, "H");
      theOpnE  = Operation.newInstance (theProblem, "I");

      BomEntry.newInstance (theOpnD, thePartB);
      BomEntry.newInstance (theOpnE, thePartC);

      BopEntry.newInstance (theOpnD, thePartA);
      BopEntry.newInstance (theOpnE, thePartB);
      }

//------------------------------------------------------------------------------
// testReadData ()
//
// Tests Problem.readData (...).
//------------------------------------------------------------------------------

   private void testReadData ()
      {
      Problem theProblem;

      theProblem = Problem.newInstance ();

      theProblem.getMessageMgr ().setMesgFileAccessMode ("a");

      theProblem.getMessageMgr ().setMesgFileName ("UnitTester.output");

      theProblem.readData ("diner-utf8.data");

      assert (theProblem.get (N_PERIODS) == 2);

      println (
         "\n"
       + "The following Components were read in from file "
       + "\"diner-utf8.data\":\n");

      for (Component theComp: theProblem.getComponents ())
         {
         Tester.printlnUTF8 ("   " + theComp);
         }

      theProblem.deactivate ();
      }

//------------------------------------------------------------------------------
// testUnicode ()
//
// Tests WIT-J with an Operation name that has a non-ASCII Unicode character.
//------------------------------------------------------------------------------

   private void testUnicode ()
      {
      Problem   theProblem;
      Operation theOpn;

      theProblem = Problem.newInstance ();

      Operation.newInstance (theProblem, "S\u00e3o Paulo");

      theProblem.writeData ("sao-paulo.data");

      theProblem.deactivate ();

      theProblem = Problem.newInstance ();

      theProblem.readData ("sao-paulo.data");

      theProblem.getMessageMgr ().flushFile ();

      theOpn = theProblem.getOperation ("S\u00e3o Paulo");

      println ();

      Tester.printlnUTF8 ("theOpn.toString (): " + theOpn.toString () + "\n");

      theProblem.deactivate ();
      }

//------------------------------------------------------------------------------
// assertEq (theVec1, theVec2) methods
//
// Each of these methods asserts that the elements of theVec1 and theVec2 are
// equal.
//------------------------------------------------------------------------------

   private static void assertEq (
         boolean[] theVec1,
         boolean[] theVec2)
      {
      int theIdx;

      assert (theVec1.length == theVec2.length);

      for (theIdx = 0; theIdx < theVec1.length; theIdx ++)
         assert (theVec1[theIdx] == theVec2[theIdx]);
      }

//------------------------------------------------------------------------------

   private static void assertEq (
         int[] theVec1,
         int[] theVec2)
      {
      int theIdx;

      assert (theVec1.length == theVec2.length);

      for (theIdx = 0; theIdx < theVec1.length; theIdx ++)
         assert (theVec1[theIdx] == theVec2[theIdx]);
      }

//------------------------------------------------------------------------------
// assertApproxEq (theDblVec1, theDblVec2)
//
// Asserts that the elements of theDblVec1 and theDblVec2 are nearly equal.
//------------------------------------------------------------------------------

   private static void assertApproxEq (
         double[] theDblVec1,
         double[] theDblVec2)
      {
      int theIdx;

      assert (theDblVec1.length == theDblVec2.length);

      for (theIdx = 0; theIdx < theDblVec1.length; theIdx ++)
         {
         assertApproxEq (theDblVec1[theIdx], theDblVec2[theIdx]);
         }
      }

//------------------------------------------------------------------------------
// assertApproxEq (theDbl1, theDbl2)
//
// Asserts that theDbl1 and theDbl2 are (nearly) equal.
//------------------------------------------------------------------------------

   private static void assertApproxEq (
         double theDbl1,
         double theDbl2)
      {
      assert (StrictMath.abs (theDbl1 - theDbl2) < .0001);
      }

//------------------------------------------------------------------------------
// printList (theList, theListName)
//
// Prints the contents of theList, whose name is given by theListName.
//------------------------------------------------------------------------------

   private static void printList (List <?> theList, String theListName)
      {
      println (
         "\n"
       + theListName + ":");

      for (Object theObject: theList)
         println ("   " + theObject);
      }

//------------------------------------------------------------------------------
// println ()
//
// Wrapper method.
//------------------------------------------------------------------------------

   private static void println ()
      {
      println ("");
      }

//------------------------------------------------------------------------------
// println (theObject)
//
// Wrapper method.
//------------------------------------------------------------------------------

   private static void println (Object theObject)
      {
      System.out.println (theObject);
      }

//------------------------------------------------------------------------------
// asString (theObject)
//------------------------------------------------------------------------------

   private static String asString (Object theObject)
      {
      return theObject.toString ();
      }

//==============================================================================
// Private Fields
//==============================================================================

//------------------------------------------------------------------------------
// WIT-J objects for the unit test.
//------------------------------------------------------------------------------

   private Problem    myProblem;
   private MessageMgr myMessageMgr;
   private Part       myPartA;
   private Part       myPartB;
   private Part       myPartC;
   private Part       myPartD;
   private Part       myPartE;
   private Operation  myOpnF;
   private Operation  myOpnG;
   private BomEntry   myBomEntFA;
   private BomEntry   myBomEntFB1;
   private BomEntry   myBomEntFB2;
   private Substitute mySubFAB;
   private BopEntry   myBopEntFC;
   private BopEntry   myBopEntFG;
   private Demand     myDemandH;
   private Demand     myDemandI;
   private Demand     myDemandJ;

//------------------------------------------------------------------------------
// allComponentClasses
//
// Contains the Class object for each subclass of class Component.
//------------------------------------------------------------------------------

   private final List <Class <? extends Component>> allComponentClasses;

//------------------------------------------------------------------------------
// undisplayedAttributes
//
// undisplayedAttributes.contains (theAtt) is true, iff display has not
// been invoked on theAtt.
//------------------------------------------------------------------------------

   private final HashSet <Attribute <?>> undisplayedAttributes;

//------------------------------------------------------------------------------
}
