//==============================================================================
// Message Class Library
//
// (C) Copyright IBM Corp. 1997, 2005  All Rights Reserved
//==============================================================================

//-----------------------------------------------------------------------------
// File unitTest.C
//
// Contains main program for unitTest
//
// Contains the implementation of the following classes:
//
// Tester
// TestOutputDisplayer
// TestMsg
//
//-----------------------------------------------------------------------------

#include <mcl/src/unitTest.h>
#include <mcl/src/arg.h>
#include <mcl/src/facility.h>
#include <mcl/src/level.h>
#include <mcl/src/set.h>

//-----------------------------------------------------------------------------
// Turn on assert.
//-----------------------------------------------------------------------------

//#ifdef NDEBUG
//#undef NDEBUG
//#endif
#include <assert.h>

#include <iostream>


//-----------------------------------------------------------------------------
// Main program.
//-----------------------------------------------------------------------------

int main ()
   {
   MclTester theTester;

   theTester.unitTest ();

   return 0;
   }

//-----------------------------------------------------------------------------
// Implementation of class Tester.
//-----------------------------------------------------------------------------

MclTester::MclTester ()
   {
   }

//-----------------------------------------------------------------------------

MclTester::~MclTester ()
   {
   }

//-----------------------------------------------------------------------------

void MclTester::unitTest ()
   {
   testArg             ();
   testStringArg       ();
   testArgList         ();
   testMsgUnit         ();
   testMsg             ();
   testMsgFrag         ();
   testLevel           ();
   testDictEl      ();
   testMsgUnitSet      ();
   testOutputDisplayer ();
   testMsgUnitItr      ();
   testFacility        ();

   testingMessage ("All tests completed successfully.\n");
   }

//-----------------------------------------------------------------------------

void MclTester::testArg ()
   {
   testingMessage ("Testing MclArg\n");

  bool         theBoolVec[] = {false, true, true, false};
  int          iv[]         = {1,2,3,4,5};
  MclBoolean   bv[]         = {TRUE, FALSE, TRUE};
  float        fv[]         = {0.3F, 3.5F, 4.21F};
  double       dv[]         = {1.0000000000001,
                               1.0000000000002,
                               1.0000000000003};
  const char * sv[]         = {"Zero", "One", "Two", "Three"};

  //--------------------------------------------------------------------------
  // Set up testing environment.
  //--------------------------------------------------------------------------

  MclTestOutputDisplayer theDisp;
  MclFacility            theFacility ("TST", theDisp);

  MclTestMsg * theInfoMsg = new MclTestMsg (
     theFacility,
     "setStringAttrMsg",
     MclLevel::info (),
     "%1$s changed from \"%2$s\" to \"%3$s\".",
     99);

  MclMsgFrag * theFrag =
     new MclMsgFrag (theFacility, "hardLowerBoundFrag", "Hard lower bound");

  //--------------------------------------------------------------------------

  //Test constructors
  MclStringArg s("This is a test");
  MclBoolArg theBoolArg (true);
  MclIntArg i(123);
  MclIntArg b(FALSE);
  MclFloatArg f(1.2345f);
  MclDoubleArg theDblArg (1.23456789012345);
  MclMsgFragArg theMsgFragArg (theFrag);
  MclBoolVectorArg theBoolVectorArg(theBoolVec, 4);
  MclIntVectorArg theIntVectorArg(iv, 5);
  MclIntVectorArg theBoolIntVectorArg(bv, 3);
  MclFloatVectorArg theFloatVectorArg(fv, 3);
  MclDoubleVectorArg theDoubleVectorArg (dv, 3);
  MclStringVectorArg theStringVectorArg(sv, 4);

  // Test method clone()
  MclArg * a = s.clone();
  assert (valueText (* a, theInfoMsg) == "This is a test");
  delete a;

  // Test valueText

  assert (valueText (theIntVectorArg, theInfoMsg) == "\n1 2 3 4 5");

  // Test format

  std::string formattedArg;

  s.format (formattedArg, "%-20s", FALSE, 1, theInfoMsg);

  assert (formattedArg == "This is a test      ");

  theBoolArg.format (formattedArg, "%-6b", FALSE, 1, theInfoMsg);

  assert (formattedArg == "TRUE  ");

  i.format (formattedArg, "%-6d", FALSE, 1, theInfoMsg);

  assert (formattedArg == "123   ");

  b.format (formattedArg, "%-6b", FALSE, 1, theInfoMsg);

  assert (formattedArg == "FALSE ");

  f.format (formattedArg, "%7.3f", FALSE, 1, theInfoMsg);

  assert (formattedArg == "  1.235");

  theDblArg.format (formattedArg, "%15.12f", FALSE, 1, theInfoMsg);

  assert (formattedArg == " 1.234567890123");

  theMsgFragArg.format (formattedArg, "%s", FALSE, 1, theInfoMsg);

  assert (formattedArg == "Hard lower bound");

  theMsgFragArg.format (formattedArg, "%-20m", FALSE, 1, theInfoMsg);

  assert (formattedArg == "Hard lower bound    ");

  theBoolVectorArg.format (formattedArg, "%-5b", TRUE, 1, theInfoMsg);

  assert (formattedArg == "\nFALSE TRUE  TRUE  FALSE");

  theIntVectorArg.format (formattedArg, "%2d", TRUE, 1, theInfoMsg);

  assert (formattedArg == "\n 1  2  3  4  5");

  theBoolIntVectorArg.format (formattedArg, "%b", TRUE, 1, theInfoMsg);

  assert (formattedArg == "\nTRUE FALSE TRUE");

  theFloatVectorArg.format (formattedArg, "%.3f", TRUE, 1, theInfoMsg);

  assert (formattedArg == "\n0.300 3.500 4.210");

  theDoubleVectorArg.format (formattedArg, "%.13f", TRUE, 1, theInfoMsg);

  assert (formattedArg == "\n1.0000000000001 1.0000000000002 1.0000000000003");

  theStringVectorArg.format (formattedArg, "%-6s", TRUE, 1, theInfoMsg);

  assert (formattedArg == "\nZero   One    Two    Three ");

  // Test MclMakeVec functions.

  assert (
        valueText (MclMakeVec (theBoolVec, 4), theInfoMsg)
     == "\nFALSE TRUE TRUE FALSE");

  assert (valueText (MclMakeVec (iv, 5), theInfoMsg) == "\n1 2 3 4 5");
  assert (valueText (MclMakeVec (fv, 3), theInfoMsg) == "\n0.3 3.5 4.21");

  assert (valueText (MclMakeVec (sv, 4), theInfoMsg) == "\nZero One Two Three");
   }

//-----------------------------------------------------------------------------

void MclTester::testStringArg ()
   {
   testingMessage ("Testing MclStringArg\n");

  //--------------------------------------------------------------------------
  // Set up testing environment.
  //--------------------------------------------------------------------------

  MclTestOutputDisplayer theDisp;
  MclFacility            theFacility ("TST", theDisp);

  MclTestMsg * theInfoMsg = new MclTestMsg (
     theFacility,
     "setStringAttrMsg",
     MclLevel::info (),
     "%1$s changed from \"%2$s\" to \"%3$s\".",
     99);

  //--------------------------------------------------------------------------

  //Test constructors
  MclStringArg s("This is a test");

  // Test method clone()
  MclArg * a = s.clone();
  assert (valueText (* a, theInfoMsg) == "This is a test");

  // Test destructor
  delete a;
   }

//-----------------------------------------------------------------------------

void MclTester::testArgList ()
   {
   testingMessage ("Testing MclArgList\n");


   //--------------------------------------------------------------------------
   // Set up testing environment.
   //--------------------------------------------------------------------------

   MclTestOutputDisplayer theDisp;
   MclFacility            theFacility ("TST", theDisp);

  MclTestMsg * theInfoMsg = new MclTestMsg (
     theFacility,
     "setStringAttrMsg",
     MclLevel::info (),
     "%1$s changed from \"%2$s\" to \"%3$s\".",
     99);

   //--------------------------------------------------------------------------

   // Test constructor
   MclArgList argList;

   // Test append methods
   MclStringArg arg("1st arg");
   argList << arg<<"2nd arg"<<true<<123<<1.23456f;

   MclMsgFrag * theFrag =
      new MclMsgFrag (theFacility, "materialFrag", "Material");

   argList << theFrag;
   bool theBoolVec[] = {false,false,true};
   argList << MclBoolVectorArg(theBoolVec, 3);
   int iv[] = {1,2,3,4,5,6,7,8,9,10,11,12};
   argList << MclIntVectorArg(iv, 12);
   float fv[] = {1.1F,2.2F,3.3F};
   argList << MclFloatVectorArg(fv, 3);
   double dv[] = {1.0000000000001, 1.0000000000026};
   argList << MclDoubleVectorArg (dv, 2);

  // Test operator[]
   assert (valueText (argList[1],  theInfoMsg) == "1st arg");
   assert (valueText (argList[2],  theInfoMsg) == "2nd arg");
   assert (valueText (argList[3],  theInfoMsg) == "TRUE");
   assert (valueText (argList[4],  theInfoMsg) == "123");
   assert (valueText (argList[5],  theInfoMsg) == "1.23456");
   assert (valueText (argList[6],  theInfoMsg) == "Material");
   assert (valueText (argList[7],  theInfoMsg) == "\nFALSE FALSE TRUE");
   assert (valueText (argList[8],  theInfoMsg) ==
                                   "\n1 2 3 4 5 6 7 8 9 10 11 12");
   assert (valueText (argList[9],  theInfoMsg) == "\n1.1 2.2 3.3");
   assert (valueText (argList[10], theInfoMsg) ==
                                   "\n1.0000000000001 1.0000000000026");
   // Test length

   assert (argList.length () == 10);
   }

//-----------------------------------------------------------------------------

void MclTester::testMsgUnit ()
   {
   testingMessage ("Testing MclMsgUnit\n");

   //--------------------------------------------------------------------------
   // Set up testing environment.
   //--------------------------------------------------------------------------

   MclTestOutputDisplayer theDisp;
   MclFacility            theFacility ("TST", theDisp);

   //--------------------------------------------------------------------------
   // Test constructor.
   //--------------------------------------------------------------------------

   MclMsgUnit * theMsg = new MclMsg (
      theFacility,
      "witFuncCalled",
      MclLevel::info (),
      "PRM function %1$s entered.",
      98);

   MclMsgUnit * theFrag =
      new MclMsgFrag (theFacility, "capacityFrag", "Capacity");

   //--------------------------------------------------------------------------
   // Test access functions.
   //--------------------------------------------------------------------------

   assert (& (theMsg ->myFacility ()) == & theFacility);

   match  (   theMsg ->id (), "witFuncCalled");

   match  (   theFrag->text (), "Capacity");

   assert (!  theFrag->isInserting ());

   //--------------------------------------------------------------------------
   // Test type conversion.
   //--------------------------------------------------------------------------

   assert (theMsg ->asaMsg     () == theMsg);
   assert (theMsg ->asaMsgFrag () == NULL);
   assert (theFrag->asaMsg     () == NULL);
   assert (theFrag->asaMsgFrag () == theFrag);
   }

//-----------------------------------------------------------------------------

void MclTester::testMsg ()
   {
   MclArgList emptyArgList;

   testingMessage ("Testing MclMsg\n");

   //--------------------------------------------------------------------------
   // Set up testing environment.
   //--------------------------------------------------------------------------

   MclTestOutputDisplayer theDisp;
   MclFacility            theFacility ("TST", theDisp);

   //--------------------------------------------------------------------------
   // Test constructor.
   //--------------------------------------------------------------------------

   MclMsg * const theWarningMsg = new MclMsg (
      theFacility,
      "compatible34WarningMsg",
      MclLevel::warning (),
      "wit34Compatible is TRUE.\n"
      "Your application should be updated to work with PRM Release 4.0.",
      538);

   MclMsg * const theInfoMsg = new MclMsg (
      theFacility,
      "witFuncCalled",
      MclLevel::info (),
      "PRM function %1$s entered.",
      98);

   match (theWarningMsg->id (), "compatible34WarningMsg");
   match (theInfoMsg   ->id (), "witFuncCalled");

   //--------------------------------------------------------------------------
   // Test issue.
   //--------------------------------------------------------------------------

   theWarningMsg->issue (emptyArgList);
      //
      // The local variable emptyArgList is being passed to the issue function.
      // The natural alternative to this would be to create an empty ArgList
      // with an explicit call to the constructor and pass the resulting
      // temporary object to the issue function. But this causes some compilers
      // to invoke the copy ctor for class MclArgList (or at least require that
      // it be accessible), and this causes a syntax error, because the copy
      // ctor has been declared private in order to prevent unintentional copy
      // constructing. (See "Effective C++", Item 27.)

   theDisp.reqText (
      "TST0538W wit34Compatible is TRUE.\n"
      "         Your application should be updated "
               "to work with PRM Release 4.0.\n",
      TRUE);

   theInfoMsg ->issue (MclArgList () << "witInitialize");

   theDisp.reqText ("TST0098I PRM function witInitialize entered.\n", FALSE);

   //--------------------------------------------------------------------------
   // Test data setting and access functions.
   //--------------------------------------------------------------------------

   theInfoMsg                      ->
      maxTimesIssued            (2)->
      preceedingLineFeeds       (1)->
      trailingLineFeeds         (3)->
      displayExternalNumber (FALSE)->
      allowedToChange       (FALSE);

   assert (theInfoMsg->maxTimesIssued ()        == 2);
   assert (theInfoMsg->preceedingLineFeeds ()   == 1);
   assert (theInfoMsg->trailingLineFeeds ()     == 3);

   assert (theInfoMsg->displayExternalNumber () == FALSE);
   assert (theInfoMsg->allowedToChange ()       == FALSE);

   theInfoMsg ->issue (MclArgList () << "<#2>");

   theDisp.reqText ("\nPRM function <#2> entered.\n\n\n", FALSE);

   theInfoMsg ->issue (MclArgList () << "<#3>");

   theDisp.reqText ("\nPRM function <#2> entered.\n\n\n", FALSE);

   assert (theInfoMsg->timesIssued () == 3);

   //---------------------------------------------------------------------------
   // Test vectorIndent.
   //---------------------------------------------------------------------------

   MclMsg * const theVecMsg = new MclMsg (
      theFacility,
      "extSupplyVolDdMsg",
      MclLevel::info (),
      "   External supply volumes:%1v$8.0f",
      131);

   float theVec [] = {
       0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
      10, 11, 12, 13, 14, 15, 16, 17, 18, 19};

   theVecMsg->vectorIndent (6);

   assert (theVecMsg->vectorIndent () == 6);

   theVecMsg->issue (MclArgList () << MclMakeVec (theVec, 20));

   theDisp.reqText (
"TST0131I    External supply volumes:\n"
"                      0        1        2        3        4        5        6"
"\n"
"                      7        8        9       10       11       12       13"
"\n"
"                     14       15       16       17       18       19\n",
      FALSE);

   theVecMsg->displayExternalNumber (FALSE);

   theVecMsg->issue (MclArgList () << MclMakeVec (theVec, 20));


   theDisp.reqText (
"   External supply volumes:\n"
"             0        1        2        3        4        5        6        7"
"\n"
"             8        9       10       11       12       13       14       15"
"\n"
"            16       17       18       19\n",
      FALSE);
   }

//-----------------------------------------------------------------------------

void MclTester::testMsgFrag ()
   {
   testingMessage ("Testing MclMsgFrag\n");

   //--------------------------------------------------------------------------
   // Set up testing environment.
   //--------------------------------------------------------------------------

   MclTestOutputDisplayer theDisp;
   MclFacility            theFacility ("TST", theDisp);

   //--------------------------------------------------------------------------
   // Test constructor.
   //--------------------------------------------------------------------------

   MclMsgFrag * theFrag =
      new MclMsgFrag (theFacility, "hardLowerBoundFrag", "Hard lower bound");
   }

//-----------------------------------------------------------------------------

void MclTester::testLevel ()
   {
   testingMessage ("Testing MclLevel and its derived classes\n");

   //--------------------------------------------------------------------------
   // Set up testing environment.
   //--------------------------------------------------------------------------

   MclTestOutputDisplayer theDisp;
   MclFacility            theFacility ("TST", theDisp);

   //--------------------------------------------------------------------------
   // (Test of Level instance access functions is implied by the other tests.)
   //--------------------------------------------------------------------------

   //--------------------------------------------------------------------------
   // Test Level access functions.
   //--------------------------------------------------------------------------

   assert (MclLevel::warning ().flag     () == 'W');
   assert (MclLevel::error   ().severity () ==  2 );

   //--------------------------------------------------------------------------
   // Test comparison operators.
   //--------------------------------------------------------------------------

   assert (! (MclLevel::info    () == MclLevel::warning ()));
   assert    (MclLevel::warning () != MclLevel::error   ());
   assert    (MclLevel::error   () <= MclLevel::severe  ());
   assert (! (MclLevel::severe  () >= MclLevel::fatal   ()));
   assert (! (MclLevel::severe  () <  MclLevel::severe  ()));
   assert    (MclLevel::fatal   () >  MclLevel::warning ());
   }

//-----------------------------------------------------------------------------

void MclTester::testDictEl ()
   {
   testingMessage ("Testing MclDictEl\n");

   //--------------------------------------------------------------------------
   // Set up testing environment.
   //--------------------------------------------------------------------------

   MclTestOutputDisplayer theDisp;
   MclFacility            theFacility ("TST", theDisp);

   MclMsg * const theInfoMsg =
      new MclTestMsg (
         theFacility,
         "titleMsg",
         MclLevel::info (),
         "Problem Title:"
         "%1$s",
         101);

   //--------------------------------------------------------------------------
   // Test main ctor.
   //--------------------------------------------------------------------------

/*   MclDictEl theEl (theInfoMsg);

   //--------------------------------------------------------------------------
   // Test myMsg.
   //--------------------------------------------------------------------------

   assert (theEl.myMsgUnit () == theInfoMsg);*/
   }

//-----------------------------------------------------------------------------

void MclTester::testMsgUnitSet ()
   {
   testingMessage ("Testing MclMsgUnitSet\n");

     // Set up testing environment.
     // Implicitly test MsgUnitSet's ctor.
     //
  MclTestOutputDisplayer theDisp;
  MclFacility            theFacility ("TST", theDisp);

  MclMsgUnitSet & theMsgUnitSet = theFacility.myMsgUnitSet_;

  // Implicitly test insertKeyAndValue

  MclMsg * theMsg =
     new MclTestMsg (
        theFacility,
        "testMsgId",
        MclLevel::fatal (),
        "A message to test class MsgUnitSet.",
        53);

  //test method findValue
  const MclMsgUnit * msg2 = theMsgUnitSet.findValue ("testMsgId");

  match (msg2->id (), "testMsgId");
  }

//-----------------------------------------------------------------------------

void MclTester::testOutputDisplayer ()
   {
   testingMessage ("Testing MclOutputDisplayer and its derived classes\n");

   //test constructor
   MclPrintfOutputDisplayer out("mclOut.tmp", "w");
   out.output("This is a test\n");

   MclTestOutputDisplayer tester;

   tester.output ("Test output.\n");

   tester.reqText ("Test output.\n", FALSE);

   tester.outputError ("Test error output.\n");

   assert (tester.outputErrorText_ == "Test error output.\n");
   }

//-----------------------------------------------------------------------------

void MclTester::testMsgUnitItr ()
   {
   testingMessage ("Testing MclMsgUnitItr");

   //--------------------------------------------------------------------------
   // Set up testing environment.
   //--------------------------------------------------------------------------

   MclTestOutputDisplayer theDisp;
   MclFacility            theFacility ("TST", theDisp);

   new MclMsgFrag (theFacility, "softLowerBoundFrag", "Soft lower bound");

   new MclTestMsg (
      theFacility,
      "timingMismatchFmsg",
      MclLevel::fatal (),
      "Timing section mismatch.",
      192);

   new MclMsgFrag (theFacility, "capacityFrag", "Capacity");

   new MclTestMsg (
      theFacility,
      "witFuncCalled",
      MclLevel::info (),
      "PRM function %1$s entered.",
      198);

   (new MclTestMsg (
      theFacility,
      "witFileParam",
      MclLevel::info (),
      "The file \"%1$s\" will be accessed.",
	  100))->
         allowedToChange (FALSE);

   //--------------------------------------------------------------------------
   // Test ctor, nextMsgUnit.
   //--------------------------------------------------------------------------

   MclMsgUnitItr theItr (theFacility);

   match  (theItr.nextMsgUnit ()->id (), "softLowerBoundFrag");  // 1st Frag inserted
   match  (theItr.nextMsgUnit ()->id (), "capacityFrag");  // 2nd Frag inserted
   match  (theItr.nextMsgUnit ()->id (), "witFileParam"); // lowest ext msg no
   match  (theItr.nextMsgUnit ()->id (), "timingMismatchFmsg"); // middle ext msg no
   match  (theItr.nextMsgUnit ()->id (), "witFuncCalled"); // highest external msg no

   assert (theItr.nextMsgUnit () == NULL);

   //--------------------------------------------------------------------------
   // Test reset, nextMsg.
   //--------------------------------------------------------------------------

   theItr.reset ();

   match  (theItr.nextMsg ()->id (), "witFileParam");
   match  (theItr.nextMsg ()->id (), "timingMismatchFmsg");
   match  (theItr.nextMsg ()->id (), "witFuncCalled");

   assert (theItr.nextMsg () == NULL);

   //--------------------------------------------------------------------------
   // Test reset, nextMsgForChange.
   //--------------------------------------------------------------------------

   theItr.reset ();

   match (theItr.nextMsgForChange (MclLevel::info ())->id (), "witFuncCalled");

   assert (theItr.nextMsgForChange (MclLevel::info ()) == NULL);

   //--------------------------------------------------------------------------
   // Test nextMsgFrag.
   //--------------------------------------------------------------------------

   theItr.reset ();

   match  (theItr.nextMsgFrag ()->id (), "softLowerBoundFrag");
   match  (theItr.nextMsgFrag ()->id (), "capacityFrag");

   assert (theItr.nextMsgFrag () == NULL);
   }

//-----------------------------------------------------------------------------

void MclTester::testFacility ()
   {
   MclArgList emptyArgList;

   testingMessage ("Testing MclFacility");

  // test constructor and methods insert() and operator()

  // Simple messages for testing

const char * msgId[] = {
  "paramReadErrorInt",
  "callostd::cerror",
  "invalidAttr",
  "intRangeError",
  "floatRangeError",
  "IntegerVector",
  "FloatVector",
  "StringVector",
  "setBooleanAttrMsg",
  "hasSubsInEffectDdMsg",
  "categorySizeDdMsg",
  "floatRangeErrorG",
  "formatedStringWithAnyWidth",
  "formatedIntWithAnyWidth",
};

const char * formatList[] = {
   "Test_a_sequence_of_non-blanks_longer_than_lineLengthxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx%1$s.",
  "Memory allocation error when attempting to obtain %1$d bytes of memory.",
  "Unrecognized attribute passed as parameter.",
   "Value specified was %2$d. Attribute %1$s is out of range.",
  "Float Value was %0002$-2.3f. Attribute %1$s is out of range.",
  "Integer $ Vector in print format %%1v$d:%1v$d",
  "2%%NOTFmt$10 Float Vector in the format %%1v$2.1f:%1v$2.1f",
  "String v Vector:$%1v$s",
  "%1$s changed from %2$b to %3$b.",
  "Has Substitute Bom Entries In Effect?%1v$-5b",
  "Number of %1$-8m Parts:                 %2$5d",
  "Float Value was %2$9.3g. Attribute %1$s is out of range.",
   "Value specified was %2$d. Attribute %1$-50s is out of range.",
   "Number of Parts:%1$9d %2$s",
};

const char * finalTextList[] =
   {
     "\n\nTST0000E Test_a_sequence_of_non-blanks_longer_than_lineLengthxxxxxxxxxxxxxxxxxxx\n"
     "         xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"
     "         Parameter.txt.\n\n\n",    
     "TST0001W Memory allocation error "
            "when attempting to obtain 123 bytes of memory.\n",
   "TST0002I Unrecognized attribute passed as parameter.\n",
      "TST0003S Value specified was 123. "
          "Attribute IntegerAttribute is out of range.\n",
    "TST0004F Float Value was 1.235. "
            "Attribute FloatAttribute is out of range.\n",
   "TST0005F Integer $ Vector in print format %1v$d:\n         1 2 3 4 5\n",
   "TST0006F 2%NOTFmt$10 Float Vector in the format %1v$2.1f:\n"
   "         1.1 2.2 3.3 4.4 5.5\n",
   "TST0007F String v Vector:$\n         str1 st%r2 ~!@#$%^&*()_+`0-=[]{}\\|:;'\"<>,./? %1$d str5%\n",
   "TST0183I execEmptyBom changed from FALSE to TRUE.\n",
   "TST0544I Has Substitute Bom Entries In Effect?\n"
   "         FALSE TRUE  FALSE\n",
   "TST0098I Number of Capacity Parts:                   412\n",
   "TST0073S Float Value was  3.46e+03. "
            "Attribute FloatAttribute is out of range.\n",
     "TST0012S Value specified was 123. "
     "Attribute This is a test       is out of"
     "              range.\n",
   };


MclTestOutputDisplayer theDisp;
  MclFacility            msgFacil ("TST", theDisp);

  //---------------------------------------------------------------------------
  // Implictly test insert by constructing Msgs and MsgFrags.
  //---------------------------------------------------------------------------

  new MclTestMsg (msgFacil, msgId[4], MclLevel::fatal   (), formatList[4], 4);
  new MclTestMsg (msgFacil, msgId[3], MclLevel::severe  (), formatList[3], 3);
  new MclTestMsg (msgFacil, msgId[0], MclLevel::error   (), formatList[0], 0);
  new MclTestMsg (msgFacil, msgId[1], MclLevel::warning (), formatList[1], 1);
  new MclTestMsg (msgFacil, msgId[2], MclLevel::info    (), formatList[2], 2);
  new MclTestMsg (msgFacil, msgId[5], MclLevel::fatal   (), formatList[5], 5);
  new MclTestMsg (msgFacil, msgId[6], MclLevel::fatal   (), formatList[6], 6);
  new MclTestMsg (msgFacil, msgId[7], MclLevel::fatal   (), formatList[7], 7);
  new MclTestMsg (msgFacil, msgId[8], MclLevel::info    (), formatList[8],183);
  new MclTestMsg (msgFacil, msgId[9], MclLevel::info    (), formatList[9],544);
  new MclTestMsg (msgFacil, msgId[10],MclLevel::info    (), formatList[10],98);
  new MclTestMsg (msgFacil, msgId[11],MclLevel::severe  (), formatList[11],73);
  new MclTestMsg (msgFacil, msgId[12], MclLevel::severe  (), formatList[12], 12);
  new MclTestMsg (msgFacil, msgId[13], MclLevel::severe  (), formatList[13], 13);
  
  new MclMsgFrag (msgFacil, "capacityFrag", "Capacity");
  
  //---------------------------------------------------------------------------
  // Test operator ()
  //---------------------------------------------------------------------------

  msgFacil.findMsg (msgId[0])->
     preceedingLineFeeds (2) ->
     trailingLineFeeds   (3);

  msgFacil(msgId[0], MclArgList() << "Parameter.txt");

  theDisp.reqText (finalTextList[0], TRUE);

  msgFacil(msgId[1], MclArgList() << 123);

  theDisp.reqText (finalTextList[1], TRUE);


   msgFacil(msgId[2], emptyArgList);

   theDisp.reqText (finalTextList[2], FALSE);


   msgFacil(msgId[2]);

   theDisp.reqText (finalTextList[2], FALSE);


  msgFacil(msgId[3], MclArgList() << "IntegerAttribute" << 123);
 
   theDisp.reqText (finalTextList[3], TRUE);


  msgFacil(msgId[4], MclArgList() << "FloatAttribute" << 1.23456f);

  theDisp.reqText (finalTextList[4], TRUE);


  int iv[] = {1,2,3,4,5};
  msgFacil(msgId[5], MclArgList() << MclMakeVec (iv, 5));

  theDisp.reqText (finalTextList[5], TRUE);

  float fv[] = {1.1F,2.2F,3.3F,4.4F,5.5F};
  msgFacil(msgId[6], MclArgList() << MclMakeVec (fv, 5));

  theDisp.reqText (finalTextList[6], TRUE);


  const char* sv[] = {"str1", "st%r2", "~!@#$%^&*()_+`0-=[]{}\\|:;'\"<>,./?", "%1$d", "str5%"};
  msgFacil(msgId[7], MclArgList() << MclMakeVec (sv, 5));

  theDisp.reqText (finalTextList[7], TRUE);

  msgFacil(msgId[8], MclArgList() << "execEmptyBom" << FALSE << TRUE);

  theDisp.reqText (finalTextList[8], FALSE);

  bool bv[] = {false, true, false};

  msgFacil(msgId[9], MclArgList() << MclMakeVec (bv, 3));

  theDisp.reqText (finalTextList[9], FALSE);

  msgFacil (
     msgId[10],
     MclArgList() << msgFacil.findMsgFrag ("capacityFrag") << 412);

  theDisp.reqText (finalTextList[10], FALSE);

  msgFacil(msgId[11], MclArgList() << "FloatAttribute" << 3456.789f);

  theDisp.reqText (finalTextList[11], TRUE);
  
  // Test string,int format with any width, such as %-20s

  msgFacil(msgId[12], MclArgList() << "This is a test" << 123);

  //---------------------------------------------------------------------------
  // Test findMsgUnit, findMsg, and findMsgFrag functions
  //---------------------------------------------------------------------------

  match (msgFacil.findMsgUnit (msgId[3])->id (), msgId[3]);
  match (msgFacil.findMsg     (msgId[4])->id (), msgId[4]);
  match (msgFacil.findMsg     (5)       ->id (), msgId[5]);

  match (
     msgFacil.findMsgFrag ("capacityFrag")->text (),
     "Capacity");

  //---------------------------------------------------------------------------
  // Test attribute set-by-Level functions.
  //---------------------------------------------------------------------------

  msgFacil.findMsg (msgId[8])->allowedToChange (FALSE);

  msgFacil.maxTimesIssued        (MclLevel::info (), 73);

  msgFacil.displayExternalNumber (MclLevel::info (), FALSE);

  msgFacil(msgId[1], MclArgList() << 123);

  theDisp.reqText (finalTextList[1], TRUE);

  msgFacil(msgId[2], emptyArgList);

  theDisp.reqText ("Unrecognized attribute passed as parameter.\n", FALSE);

  msgFacil(msgId[8], MclArgList() << "execEmptyBom" << FALSE << TRUE);

  theDisp.reqText (finalTextList[8], FALSE);

  //---------------------------------------------------------------------------
  // Test boolean functions.
  //---------------------------------------------------------------------------

  new MclMsgFrag (msgFacil, "yesFrag", "Yes");
  new MclMsgFrag (msgFacil,  "noFrag", "No");

  msgFacil.booleanMsgFrags (
     msgFacil.findMsgFrag ("yesFrag"),
     msgFacil.findMsgFrag ( "noFrag"));

  match (msgFacil.booleanText (TRUE), "Yes");

  msgFacil ("setBooleanAttrMsg", MclArgList () << "mandEC" << TRUE << FALSE);

  theDisp.reqText ("TST0183I mandEC changed from Yes to No.\n", FALSE);

  //---------------------------------------------------------------------------
  // Test lineLength
  //---------------------------------------------------------------------------

  msgFacil.lineLength (24);

  assert (msgFacil.lineLength () == 24);

   msgFacil (msgId[3], MclArgList () << "latestPeriod" << 5432);

   theDisp.reqText (
      "TST0003S Value\n"
      "         specified was\n"
      "         5432. Attribute\n"
      "         latestPeriod is\n"
      "         out of range.\n",
      TRUE);

   msgFacil.findMsg (3)->displayExternalNumber (FALSE);

   msgFacil (msgId[3], MclArgList () << "latestPeriod" << 5432);

   theDisp.reqText (
                    "Value specified was\n"
                    "5432. Attribute\n"
                    "latestPeriod is out of\n"
                    "range.\n",
                    TRUE);

  msgFacil.lineLength (80);

  //---------------------------------------------------------------------------
  // Test deletingMsgUnits.
  //---------------------------------------------------------------------------

  assert (! msgFacil.deletingMsgUnits ());

  //---------------------------------------------------------------------------
  // Test co-existence of Facilities.
  //---------------------------------------------------------------------------

  MclFacility mfb ("MFB", theDisp);

  new MclTestMsg (mfb, msgId[4], MclLevel::severe (), formatList[1], 27);

  mfb (msgId[4], MclArgList() << 8765);

  theDisp.reqText (
     "MFB0027S Memory allocation error "
              "when attempting to obtain 8765 bytes of memory.\n",
     TRUE);

  //---------------------------------------------------------------------------
  // Test minErrOutLevel.
  //---------------------------------------------------------------------------

  msgFacil.minErrOutLevel (MclLevel::severe ());

  assert (msgFacil.minErrOutLevel () == MclLevel::severe ());

  msgFacil(msgId[0], MclArgList() << "Parameter.txt");

  theDisp.reqText (finalTextList[0], FALSE);

  msgFacil(msgId[1], MclArgList() << 123);

  theDisp.reqText (finalTextList[1], FALSE);

  msgFacil.findMsg (2)->displayExternalNumber (TRUE);

  msgFacil(msgId[2], emptyArgList);

  theDisp.reqText (finalTextList[2], FALSE);

  msgFacil.findMsg (3)->displayExternalNumber (TRUE);

   msgFacil(msgId[3], MclArgList() << "IntegerAttribute" << 123);

  theDisp.reqText (finalTextList[3], TRUE);

  msgFacil(msgId[4], MclArgList() << "FloatAttribute" << 1.23456f);

  theDisp.reqText (finalTextList[4], TRUE);
  }

//-----------------------------------------------------------------------------

void MclTester::testingMessage (const char * msg)
   {
    std::cout <<std::endl <<"************************************************"
        <<std::endl <<msg <<std::endl;
   }

//-----------------------------------------------------------------------------

std::string MclTester::valueText (
      const MclArg & theArg,
      const MclMsg * theMsg)
      const
   {
   std::string theText;

   theArg.getValueText (theText, theMsg);

   return theText;
   }

//-----------------------------------------------------------------------------

void MclTester::match (const char * charStar1, const char * charStar2)
   {
   assert (std::string (charStar1) == charStar2);
   }

//-----------------------------------------------------------------------------
// Implementation of class TestOutputDisplayer
//-----------------------------------------------------------------------------

MclTestOutputDisplayer::MclTestOutputDisplayer ():
      MclOutputDisplayer (),

      outputText_        (),
      outputErrorText_   ()
   {
   }

//-----------------------------------------------------------------------------

MclTestOutputDisplayer::~MclTestOutputDisplayer ()
   {
   }

//-----------------------------------------------------------------------------

void MclTestOutputDisplayer::output (const char * finalText)
   {
   std::cout <<
      "\n"
      "------\n"
      "Output\n"
      "------\n"
      "" << finalText << std::flush;

   outputText_      = finalText;
   outputErrorText_ = "";
   }

//-----------------------------------------------------------------------------

void MclTestOutputDisplayer::outputError (const char * finalText)
   {
   std::cerr <<
      "\n"
      "------------\n"
      "Error Output\n"
      "------------\n"
      "" << finalText << std::flush;

   outputErrorText_ = finalText;
   }

//-----------------------------------------------------------------------------

void MclTestOutputDisplayer::reqText (
      const char * theText,
      MclBoolean   isErrorText)
   {
   testText ("outputText_", outputText_, theText);

   testText ("outputErrorText_", outputErrorText_, (isErrorText? theText: ""));
   }

//-----------------------------------------------------------------------------

void MclTestOutputDisplayer::testText (
      const char *      textName,
      const std::string & actualText,
      const char *      expectedText)
   {
   if (actualText != expectedText)
      {
      std::cerr <<
         "\n\n"
         "MCL UNIT TEST ERROR:\n\n"
         "   Unexpected value in " << textName << ".\n\n"
         "   " << textName << " actual contents:\n"
         "----->\n" << actualText.c_str() << "<-----\n\n"
         "   " << textName << " expected contents:\n"
         "----->\n" << expectedText << "<-----\n";

      MclFacility::abortMcl ();
      }
   }

//-----------------------------------------------------------------------------
// Implementation of class TestMsg
//-----------------------------------------------------------------------------

MclTestMsg::MclTestMsg (
         MclFacility &    theFacility,
         const char *     msgId,
         const MclLevel & theLevel,
         const char *     format,
         int              externalNumber):

      MclMsg (theFacility, msgId, theLevel, format, externalNumber)
   {
   }

//-----------------------------------------------------------------------------

MclTestMsg::~MclTestMsg ()
   {
   }

//-----------------------------------------------------------------------------

void MclTestMsg::postIssue ()
   {
   if (myLevel () < MclLevel::error ())
      return;

   std::cerr <<
      "\n\n"
      "The above message was of severity level \"error\" or higher.\n"
      "However, since this is only an MCL unit test, "
         "the program will continue.\n\n";
   }
