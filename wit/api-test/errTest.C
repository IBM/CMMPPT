//------------------------------------------------------------------------------
// Source File: "errTest.C".
//
// A WIT application program whose purpose is to test WIT's handling of errors
// that cannot be tested in stand-alone mode.
//------------------------------------------------------------------------------

#include <wit.h>

#include <string>
#include <iostream>

#include <assert.h>
#include <math.h>

//------------------------------------------------------------------------------

void testCase1   ();
void testCase2   ();
void testCase3   ();
void testCase4   ();
void doWitCalls4 ();
void testCase5   ();
void doWitCalls5 ();
void testCase6   ();
void testCase7   ();
void testCase8   ();
void testCase9   ();
void testCase10  ();
void testCase11  ();
void testCase12  ();
void testCase13  ();
void testCase14  ();
void testCase15  ();
void testCase16  ();
void testCase17  ();
void testCase18  ();
void testCase19  ();
void testCase20  ();
void testCase21  ();
void testCase22  ();
void testCase23  ();
void testCase24  ();
void testCase25  ();
void testCase26  ();
void testCase27  ();
void testCase28  ();
void testCase29  ();
void testCase30  ();
void testCase31  ();
void testCase32  ();
void testCase33  ();
void testCase34  ();
void testCase35  ();
void testCase36  ();
void testCase37  ();
void testCase38  ();
void testCase39  ();
void testCase40  ();
void testCase41  ();
void testCase42  ();
void testCase43  ();
void testCase44  ();
void testCase45  ();
void testCase46  ();
void testCase47  ();
void testCase48  ();
void testCase49  ();
void testCase50  ();
void testCase51  ();
void testCase52  ();
void testCase53  ();
void testCase54  ();
void testCase55  ();
void testCase56  ();
void testCase57  ();
void testCase58  ();
void testCase59  ();
void testCase60  ();
void testCase61  ();
void testCase62  ();
void testCase63  ();
void testCase64  ();
void testCase65  ();
void testCase66  ();
void testCase67  ();
void testCase68  ();
void testCase69  ();
void testCase70  ();
void testCase71  ();

//------------------------------------------------------------------------------

const float * floatVec (float theFlt0, float theFlt1);
   //
   // Returns a static float vector of length 2, populated with the arguments.

//------------------------------------------------------------------------------

void terminationExpected (const char * theFuncName);
   //
   // Issues an error message indicating that the call to the function whose
   // name is given by theFuncName should have terminated the program.
   // Terminates the program.

//------------------------------------------------------------------------------

void handleErrorExc (WitErrorExc & theErrorExc);
   //
   // Handles theErrorExc.

//------------------------------------------------------------------------------

void handleUnknownExc ();
   //
   // Handles an unknown exception.

//------------------------------------------------------------------------------

void makeNewWitRun (WitRun * & theWitRun);
   //
   // Creates a new WitRun and sets theWitRun to it.

//------------------------------------------------------------------------------

int main (int argc, char * argv[])
   {
   std::string theArg;

   if (argc != 2)
      {
      std::cerr
         << "ERROR: Exactly one argument must be passed to errTest."
         << std::endl
         << std::endl;

      exit (1);
      }

   theArg = argv[1];

   std::cout
      << "----------------------------------------"
      << "----------------------------------------"
      << std::endl
      << "Error Test, Case "
      << theArg
      << "."
      << std::endl
      << "----------------------------------------"
      << "----------------------------------------"
      << std::endl
      << std::endl;

   if      (theArg == "1")
      testCase1 ();

   else if (theArg == "2")
      testCase2 ();

   else if (theArg == "3")
      testCase3 ();

   else if (theArg == "4")
      testCase4 ();

   else if (theArg == "5")
      testCase5 ();

   else if (theArg == "6")
      testCase6 ();

   else if (theArg == "7")
      testCase7 ();

   else if (theArg == "8")
      testCase8 ();

   else if (theArg == "9")
      testCase9 ();

   else if (theArg == "10")
      testCase10 ();

   else if (theArg == "11")
      testCase11 ();

   else if (theArg == "12")
      testCase12 ();

   else if (theArg == "13")
      testCase13 ();

   else if (theArg == "14")
      testCase14 ();

   else if (theArg == "15")
      testCase15 ();

   else if (theArg == "16")
      testCase16 ();

   else if (theArg == "17")
      testCase17 ();

   else if (theArg == "18")
      testCase18 ();

   else if (theArg == "19")
      testCase19 ();

   else if (theArg == "20")
      testCase20 ();

   else if (theArg == "21")
      testCase21 ();

   else if (theArg == "22")
      testCase22 ();

   else if (theArg == "23")
      testCase23 ();

   else if (theArg == "24")
      testCase24 ();

   else if (theArg == "25")
      testCase25 ();

   else if (theArg == "26")
      testCase26 ();

   else if (theArg == "27")
      testCase27 ();

   else if (theArg == "28")
      testCase28 ();

   else if (theArg == "29")
      testCase29 ();

   else if (theArg == "30")
      testCase30 ();

   else if (theArg == "31")
      testCase31 ();

   else if (theArg == "32")
      testCase32 ();

   else if (theArg == "33")
      testCase33 ();

   else if (theArg == "34")
      testCase34 ();

   else if (theArg == "35")
      testCase35 ();

   else if (theArg == "36")
      testCase36 ();

   else if (theArg == "37")
      testCase37 ();

   else if (theArg == "38")
      testCase38 ();

   else if (theArg == "39")
      testCase39 ();

   else if (theArg == "40")
      testCase40 ();

   else if (theArg == "41")
      testCase41 ();

   else if (theArg == "42")
      testCase42 ();

   else if (theArg == "43")
      testCase43 ();

   else if (theArg == "44")
      testCase44 ();

   else if (theArg == "45")
      testCase45 ();

   else if (theArg == "46")
      testCase46 ();

   else if (theArg == "47")
      testCase47 ();

   else if (theArg == "48")
      testCase48 ();

   else if (theArg == "49")
      testCase49 ();

   else if (theArg == "50")
      testCase50 ();

   else if (theArg == "51")
      testCase51 ();

   else if (theArg == "52")
      testCase52 ();

   else if (theArg == "53")
      testCase53 ();

   else if (theArg == "54")
      testCase54 ();

   else if (theArg == "55")
      testCase55 ();

   else if (theArg == "56")
      testCase56 ();

   else if (theArg == "57")
      testCase57 ();

   else if (theArg == "58")
      testCase58 ();

   else if (theArg == "59")
      testCase59 ();

   else if (theArg == "60")
      testCase60 ();

   else if (theArg == "61")
      testCase61 ();

   else if (theArg == "62")
      testCase62 ();

   else if (theArg == "63")
      testCase63 ();

   else if (theArg == "64")
      testCase64 ();

   else if (theArg == "65")
      testCase65 ();

   else if (theArg == "66")
      testCase66 ();

   else if (theArg == "67")
      testCase67 ();

   else if (theArg == "68")
      testCase68 ();

   else if (theArg == "69")
      testCase69 ();

   else if (theArg == "70")
      testCase70 ();

   else if (theArg == "71")
      testCase71 ();

   else
      {
      std::cerr
         << "ERROR: The argument value must be an integer from 1 to 71."
         << std::endl
         << std::endl;

      exit (1);
      }

   std::cout
      << "Normal termination of errTest, Case " << theArg << "."
      << std::endl
      << std::endl;

   exit (0);
   }

//------------------------------------------------------------------------------

void testCase1 ()
   {
   WitRun * theWitRun;

   std::cout
      << "Test of error handling by program termination."
      << std::endl;

   makeNewWitRun  (theWitRun);
   witInitialize  (theWitRun);
   witAddBomEntry (theWitRun, "Opn1", "Part1");

   terminationExpected ("witAddBomEntry");
   }

//------------------------------------------------------------------------------

void testCase2 ()
   {
   WitRun *      theWitRun;
   witReturnCode theRC;

   std::cout
      << "Test of error handling by returning with an error code."
      << std::endl;

   makeNewWitRun (theWitRun);

   witSetMesgStopRunning (
      theWitRun,
      WitFALSE,
      521,
      WitFALSE);

   witInitialize (theWitRun);

   theRC = witAddBomEntry (theWitRun, "Opn2", "Part2");

   std::cout
      << std::endl
      << "The return code for the call to witAddBomEntry was "
      << theRC
      << "."
      << std::endl
      << std::endl;
   }

//------------------------------------------------------------------------------

void testCase3 ()
   {
   WitRun *      theWitRun;
   witReturnCode theRC;

   std::cout
      << "Test of error handling by returning with an error code."
      << std::endl
      << "Specified by message level."
      << std::endl;

   makeNewWitRun (theWitRun);

   witSetMesgStopRunning (
      theWitRun,
      WitFALSE,
      WitSEVERE_MESSAGES,
      WitFALSE);

   witInitialize (theWitRun);

   theRC = witAddBomEntry (theWitRun, "Opn3", "Part3");

   std::cout
      << std::endl
      << "The return code for the call to witAddBomEntry was "
      << theRC
      << "."
      << std::endl
      << std::endl;
   }

//------------------------------------------------------------------------------

void testCase4 ()
   {
   std::cout
      << "Test of error handling by throwing an exception."
      << std::endl;

   try
      {
      doWitCalls4 ();
      }

   catch (WitErrorExc & theErrorExc)
      {
      handleErrorExc (theErrorExc);
      }

   catch (...)
      {
      handleUnknownExc ();
      }
   }

//------------------------------------------------------------------------------

void doWitCalls4 ()
   {
   WitRun * theWitRun;

   makeNewWitRun (theWitRun);

   witSetMesgThrowErrorExc (
      theWitRun,
      WitFALSE,
      521,
      WitTRUE);

   witInitialize  (theWitRun);
   witAddBomEntry (theWitRun, "Opn4", "Part4");
   witDeleteRun   (theWitRun);
   }

//------------------------------------------------------------------------------

void testCase5 ()
   {
   std::cout
      << "Test of error handling by throwing an exception."
      << std::endl
      << "Specified by message level."
      << std::endl
      << "The exception is caught by value."
      << std::endl;

   try
      {
      doWitCalls5 ();
      }

   catch (WitErrorExc theErrorExc)
      {
      handleErrorExc (theErrorExc);
      }

   catch (...)
      {
      handleUnknownExc ();
      }
   }

//------------------------------------------------------------------------------

void doWitCalls5 ()
   {
   WitRun * theWitRun;

   makeNewWitRun (theWitRun);

   witSetMesgThrowErrorExc (
      theWitRun,
      WitFALSE,
      WitSEVERE_MESSAGES,
      WitTRUE);

   witInitialize  (theWitRun);
   witAddBomEntry (theWitRun, "Opn5", "Part5");
   witDeleteRun   (theWitRun);
   }

//------------------------------------------------------------------------------

void testCase6 ()
   {
   std::cerr
      << std::endl
      << "ERROR: errTest case 6 is no longer available."
      << std::endl;

   exit (1);
   }

//------------------------------------------------------------------------------

void testCase7 ()
   {
   WitRun *      theWitRun;
   witReturnCode theRC;

   std::cout
      << "Test of WIT's behavior when it runs out of memory."
      << std::endl
      << "Function return case."
      << std::endl;

   makeNewWitRun (theWitRun);
   witInitialize (theWitRun);

   witSetMesgStopRunning (
      theWitRun,
      WitFALSE,
      404,
      WitFALSE);

   theRC = witExhaustMemory (theWitRun);

   std::cout
      << std::endl
      << "The return code for the call to witExhaustMemory was "
      << theRC
      << "."
      << std::endl
      << std::endl;
   }

//------------------------------------------------------------------------------

void testCase8 ()
   {
   WitRun * theWitRun;

   std::cout
      << "Test of WIT's behavior when it runs out of memory."
      << std::endl
      << "Exception throw case."
      << std::endl;

   makeNewWitRun (theWitRun);
   witInitialize (theWitRun);

   witSetMesgThrowErrorExc (
      theWitRun,
      WitFALSE,
      404,
      WitTRUE);

   try
      {
      witExhaustMemory (theWitRun);
      }

   catch (WitErrorExc & theErrorExc)
      {
      handleErrorExc (theErrorExc);
      }

   catch (...)
      {
      handleUnknownExc ();
      }
   }

//------------------------------------------------------------------------------

void testCase9 ()
   {
   WitRun * theWitRun;

   std::cout
      << "Test of WIT's behavior when witInterpretDevCmd is called "
      << std::endl
      << "with an invalid command."
      << std::endl;

   makeNewWitRun      (theWitRun);
   witInitialize      (theWitRun);
   witInterpretDevCmd (theWitRun, "This is an invalid command.");

   terminationExpected ("witInterpretDevCmd");
   }

//------------------------------------------------------------------------------

void testCase10 ()
   {
   WitRun * theWitRun;

   std::cout
      << "Test of WIT's response to an unknown exception."
      << std::endl;

   makeNewWitRun (theWitRun);
   witInitialize (theWitRun);

   witSetMesgThrowErrorExc (
      theWitRun,
      WitFALSE,
      849,
      WitTRUE);

   try
      {
      witInterpretDevCmd (theWitRun, "Throw an exception of unknown type.");
      }

   catch (WitErrorExc & theErrorExc)
      {
      handleErrorExc (theErrorExc);
      }

   catch (...)
      {
      handleUnknownExc ();
      }
   }

//------------------------------------------------------------------------------

void testCase11 ()
   {
   WitRun * origWitRun;
   WitRun *  dupWitRun;

   std::cout
      << "Test of WIT's behavior when witCopyData is called "
      << std::endl
      << "and the API had previously aborted from origWitRun."
      << std::endl;

   makeNewWitRun         (origWitRun);
   witInitialize         (origWitRun);
   witSetMesgStopRunning (origWitRun, WitFALSE, 521, WitFALSE);
   witAddBomEntry        (origWitRun, "Opn11",  "Part11");

   makeNewWitRun         (dupWitRun);
   witCopyData           (dupWitRun,  origWitRun);

   terminationExpected ("witCopyData");
   }

//------------------------------------------------------------------------------

void testCase12 ()
   {
   WitRun * origWitRun;
   WitRun *  dupWitRun;

   std::cout
      << "Test of WIT's behavior when an API function is called with "
      << std::endl
      << "origWitRun and the API had previously aborted from witCopyData."
      << std::endl;

   makeNewWitRun         (origWitRun);
   witSetMesgStopRunning (origWitRun, WitFALSE, 74, WitFALSE);
   witInitialize         (origWitRun); 
   witSetWbounds         (origWitRun, -1.0);
                                      
   makeNewWitRun         (dupWitRun);
   witCopyData           (dupWitRun, origWitRun);

   witAddOperation       (origWitRun, "Opn12");

   terminationExpected ("witAddOperation");
   }

//------------------------------------------------------------------------------

void testCase13 ()
   {
   WitRun *      origWitRun;
   WitRun *       dupWitRun;
   witReturnCode theRC;

   std::cout
      << "Test of WIT's behavior when witCopyData is called"
      << std::endl
      << "and the API had previously aborted from origWitRun."
      << std::endl
      << "Function return case."
      << std::endl;

   makeNewWitRun         (origWitRun);
   witInitialize         (origWitRun);
   witSetMesgStopRunning (origWitRun, WitFALSE, 521, WitFALSE);
   witAddBomEntry        (origWitRun, "Opn11", "Part11");

   makeNewWitRun         (dupWitRun);
   witSetMesgStopRunning (dupWitRun, WitFALSE, 873, WitFALSE);

   theRC = witCopyData   (dupWitRun, origWitRun);

   std::cout
      << std::endl
      << "The return code for the call to witCopyData was "
      << theRC
      << "."
      << std::endl
      << std::endl;
   }

//------------------------------------------------------------------------------

void testCase14 ()
   {
   WitRun *      theWitRunA;
   WitRun *      theWitRunB;
   witReturnCode theRC;

   std::cout
      << "Test of WIT's behavior when witReadData is called "
      << std::endl
      << "and the API had previously aborted from witReadData."
      << std::endl;

   makeNewWitRun         (theWitRunA);
   witInitialize         (theWitRunA);
   witSetMesgStopRunning (theWitRunA, WitFALSE, 631, WitFALSE);

   theRC = witReadData   (theWitRunA, "diner.params");

   std::cout
      << std::endl
      << "The return code for the call to witReadData was "
      << theRC
      << "."
      << std::endl
      << std::endl;

   makeNewWitRun         (theWitRunB);
   witInitialize         (theWitRunB);
   witReadData           (theWitRunB, "diner.data");

   terminationExpected ("witReadData");
   }

//------------------------------------------------------------------------------

void testCase15 ()
   {
   WitRun * theWitRunA;
   WitRun * theWitRunB;

   std::cout
      << "Test of WIT's behavior when witExecStandAlone is called"
      << std::endl
      << "and the value of the stopRunning attribute has been set."
      << std::endl;

   makeNewWitRun         (theWitRunA);
   witSetMesgStopRunning (theWitRunA, WitFALSE, 262, WitFALSE);
   witDeleteRun          (theWitRunA);

   makeNewWitRun         (theWitRunB);
   witExecStandAlone     (theWitRunB, "");

   terminationExpected ("witExecStandAlone");
   }

//------------------------------------------------------------------------------

void testCase16 ()
   {
   WitRun * theWitRun;

   std::cout
      << "Test of WIT's behavior when an inappropriate function is called"
      << std::endl
      << "before witInitialize."
      << std::endl;

   makeNewWitRun  (theWitRun);

   witSetNPeriods (theWitRun, 2);

   terminationExpected ("witSetNPeriods");
   }

//------------------------------------------------------------------------------

void testCase17 ()
   {
   WitRun * theWitRun;

   std::cout
      << "Test of WIT's behavior when an inappropriate function is called while"
      << std::endl
      << "external optimizing implosion is active."
      << std::endl;

   makeNewWitRun   (theWitRun);
   witInitialize   (theWitRun);
   witAddPart      (theWitRun, "A", WitMATERIAL);
   witStartExtOpt  (theWitRun);
   witAddOperation (theWitRun, "B");

   terminationExpected ("witAddOperation");
   }

//------------------------------------------------------------------------------

void testCase18 ()
   {
   WitRun * theWitRun;

   std::cout
      << "Test of WIT's behavior when an inappropriate function is called while"
      << std::endl
      << "external optimizing implosion is inactive."
      << std::endl;

   makeNewWitRun   (theWitRun);
   witInitialize   (theWitRun);
   witFinishExtOpt (theWitRun);

   terminationExpected ("witFinishExtOpt");
   }

//------------------------------------------------------------------------------

void testCase19 ()
   {
   std::cout
      << "Test of WIT's behavior when a function is called with a NULL WitRun "
         "pointer."
      << std::endl;

   witInitialize (NULL);

   terminationExpected ("witInitialize");
   }

//------------------------------------------------------------------------------

void testCase20 ()
   {
   WitRun * theWitRun;

   std::cout
      << "Test of WIT's behavior when a function is called with a deleted "
         "WitRun."
      << std::endl;

   makeNewWitRun   (theWitRun);
   witInitialize   (theWitRun);
   witDeleteRun    (theWitRun);
   witAddOperation (theWitRun, "A");

   terminationExpected ("witAddOperation");
   }

//------------------------------------------------------------------------------

void testCase21 ()
   {
   std::cerr
      << std::endl
      << "ERROR: errTest case 21 is no longer available."
      << std::endl;

   exit (1);
   }

//------------------------------------------------------------------------------

void testCase22 ()
   {
   WitRun * theWitRun;

   std::cout
      << "Test of WIT's behavior when witStartExtOpt is called while "
         "accAfterOptImp\n"
         "is TRUE."
      << std::endl;

   makeNewWitRun        (theWitRun);
   witInitialize        (theWitRun);
   witSetAccAfterOptImp (theWitRun, WitTRUE);
   witStartExtOpt       (theWitRun);

   terminationExpected ("witStartExtOpt");
   }

//------------------------------------------------------------------------------

void testCase23 ()
   {
   WitRun * theWitRun;
   int      stockVarIndex;

   std::cout
      << "Test of WIT's behavior when witGetPartStockVarIndex is called with "
         "an invalid\n"
         "period."
      << std::endl;

   makeNewWitRun           (theWitRun);
   witInitialize           (theWitRun);
   witAddPart              (theWitRun, "A", WitMATERIAL);
   witStartExtOpt          (theWitRun);
   witGetPartStockVarIndex (theWitRun, "A", 30, & stockVarIndex);

   terminationExpected ("witGetPartStockVarIndex");
   }

//------------------------------------------------------------------------------

void testCase24 ()
   {
   WitRun *  dupWitRun;
   WitRun * origWitRun;

   std::cout
      << "Test of WIT's behavior when witCopyData is called and\n"
         "ext opt implosion is active for dupWitRun."
      << std::endl;

   makeNewWitRun  (origWitRun);
   witInitialize  (origWitRun);

   makeNewWitRun  (dupWitRun);
   witInitialize  (dupWitRun);
   witAddPart     (dupWitRun, "A", WitMATERIAL);
   witStartExtOpt (dupWitRun);

   witCopyData    (dupWitRun, origWitRun);

   terminationExpected ("witCopyData");
   }

//------------------------------------------------------------------------------

void testCase25 ()
   {
   WitRun * origWitRun;
   WitRun *  dupWitRun;

   std::cout
      << "Test of WIT's behavior when witCopyData is called and\n"
         "ext opt implosion is active for origWitRun."
      << std::endl;

   makeNewWitRun  (origWitRun);
   witInitialize  (origWitRun);
   witAddPart     (origWitRun, "A", WitMATERIAL);
   witStartExtOpt (origWitRun);

   makeNewWitRun  (dupWitRun);
   witCopyData    (dupWitRun, origWitRun);

   terminationExpected ("witCopyData");
   }

//------------------------------------------------------------------------------

void testCase26 ()
   {
   WitRun * origWitRun;
   WitRun *  dupWitRun;

   std::cout
      << "Test of WIT's behavior when witCopyData is called and origWitRun is "
      << std::endl
      << "uninitialized."
      << std::endl;

   makeNewWitRun (origWitRun);

   makeNewWitRun (dupWitRun);
   witInitialize (dupWitRun);

   witCopyData   (dupWitRun, origWitRun);

   terminationExpected ("witCopyData");
   }

//------------------------------------------------------------------------------

void testCase27 ()
   {
   WitRun * theWitRun;

   std::cout
      << "Test of witCopyPartData with the same original and duplicate parts."
      << std::endl;

   makeNewWitRun   (theWitRun);
   witInitialize   (theWitRun);
   witAddPart      (theWitRun, "A", WitCAPACITY);

   witCopyPartData (theWitRun, "A", theWitRun, "A");

   terminationExpected ("witCopyPartData");
   }

//------------------------------------------------------------------------------

void testCase28 ()
   {
   WitRun * theWitRun;

   std::cout
      << "Test of witCopyPartData with "
         "a material duplicate and a capacity original."
      << std::endl;

   makeNewWitRun   (theWitRun);
   witInitialize   (theWitRun);
   witAddPart      (theWitRun, "A", WitMATERIAL);
   witAddPart      (theWitRun, "B", WitCAPACITY);

   witCopyPartData (theWitRun, "A", theWitRun, "B");

   terminationExpected ("witCopyPartData");
   }

//------------------------------------------------------------------------------

void testCase29 ()
   {
   WitRun * origWitRun;
   WitRun * dupWitRun;

   std::cout
      << "Test of witCopyPartData where origWitRun is not initialized."
      << std::endl;

   makeNewWitRun   (origWitRun);

   makeNewWitRun   (dupWitRun);
   witInitialize   (dupWitRun);

   witCopyPartData (dupWitRun, "A", origWitRun, "A");

   terminationExpected ("witCopyPartData");
   }

//------------------------------------------------------------------------------

void testCase30 ()
   {
   WitRun * origWitRun;
   WitRun * dupWitRun;

   std::cout
      << "Test of witCopyPartData where origWitRun issues a severe error, "
      << std::endl
      << "but does not terminate execution of the program."
      << std::endl;

   makeNewWitRun         (origWitRun);
   witInitialize         (origWitRun);
   witSetMesgStopRunning (origWitRun, WitFALSE, 178, WitFALSE);

   makeNewWitRun         (dupWitRun);
   witInitialize         (dupWitRun);
   witAddPart            (dupWitRun,  "A", WitCAPACITY);

   witCopyPartData       (dupWitRun,  "A", origWitRun, "B");

   witSetNPeriods        (origWitRun,  1);

   terminationExpected ("witSetNPeriods");
   }

//------------------------------------------------------------------------------

void testCase31 ()
   {
   WitRun * origWitRun;
   WitRun * dupWitRun;

   std::cout
      << "Test of witCopyPartData with two WitRuns with different values of "
         "nPeriods."
      << std::endl;

   makeNewWitRun   (origWitRun);
   witInitialize   (origWitRun);
   witSetNPeriods  (origWitRun,  4);
   witAddPart      (origWitRun, "A", WitCAPACITY);

   makeNewWitRun   (dupWitRun);
   witInitialize   (dupWitRun);
   witAddPart      (dupWitRun,  "A", WitCAPACITY);

   witCopyPartData (dupWitRun,  "A", origWitRun, "A");

   terminationExpected ("witCopyPartData");
   }

//------------------------------------------------------------------------------

void testCase32 ()
   {
   WitRun * theWitRun;

   std::cout
      << "Test of witPurgeData during stochastic mode."
      << std::endl;

   makeNewWitRun            (theWitRun);
   witInitialize            (theWitRun);
   witAddOperation          (theWitRun, "A");
   witAddPart               (theWitRun, "B", WitMATERIAL);
   witSetOperationSelForDel (theWitRun, "A", WitTRUE);
   witSetStageByObject      (theWitRun,      WitTRUE);
   witSetStochMode          (theWitRun,      WitTRUE);
   witPurgeData             (theWitRun);

   terminationExpected ("witPurgeData");
   }

//------------------------------------------------------------------------------

void testCase33 ()
   {
   WitRun * theWitRun;

   std::cout
      << "Test of witStochImplode without stochastic mode."
      << std::endl;

   makeNewWitRun   (theWitRun);
   witInitialize   (theWitRun);
   witStochImplode (theWitRun);

   terminationExpected ("witStochImplode");
   }

//------------------------------------------------------------------------------

void testCase34 ()
   {
   WitRun * theWitRun;

   std::cout
      << "Test of witStochImplode in stochastic solution mode."
      << std::endl;

   makeNewWitRun         (theWitRun);
   witInitialize         (theWitRun);
   witSetPreferCoin      (theWitRun,      WitTRUE);
   witAddPart            (theWitRun, "A", WitMATERIAL);
   witSetStageByObject   (theWitRun,      WitTRUE);
   witSetStochMode       (theWitRun,      WitTRUE);
   witSetPartObjectStage (theWitRun, "A", 1);
   witStochImplode       (theWitRun);
   witStochImplode       (theWitRun);

   terminationExpected ("witStochImplode");
   }

//------------------------------------------------------------------------------

void testCase35 ()
   {
   WitRun * theWitRun;

   std::cout
      << "Test of witClearStochSoln when not in stochastic solution mode."
      << std::endl;

   makeNewWitRun       (theWitRun);
   witInitialize       (theWitRun);
   witAddPart          (theWitRun, "A", WitMATERIAL);
   witSetStageByObject (theWitRun, WitTRUE);
   witSetStochMode     (theWitRun, WitTRUE);
   witClearStochSoln   (theWitRun);

   terminationExpected ("witClearStochSoln");
   }

//------------------------------------------------------------------------------

void testCase36 ()
   {
   WitRun * theWitRun;
   float    demandVolB[] = {100.0};

   std::cout
      << "Test of witSetDemandDemandVol in stochastic solution mode."
      << std::endl;

   makeNewWitRun         (theWitRun);
   witInitialize         (theWitRun);
   witSetPreferCoin      (theWitRun,           WitTRUE);
   witSetNPeriods        (theWitRun,           1);
   witAddPart            (theWitRun, "A",      WitMATERIAL);
   witAddDemand          (theWitRun, "A", "B");
   witSetStageByObject   (theWitRun,           WitTRUE);
   witSetStochMode       (theWitRun,           WitTRUE);
   witSetPartObjectStage (theWitRun, "A",      1);
   witStochImplode       (theWitRun);
   witSetDemandDemandVol (theWitRun, "A", "B", demandVolB);

   terminationExpected ("witSetDemandDemandVol");
   }

//------------------------------------------------------------------------------

void testCase37 ()
   {
   WitRun * origWitRun;
   WitRun * dupWitRun;

   std::cout
      << "Test of witCopyOperationData where origWitRun is in stochastic mode."
      << std::endl;

   makeNewWitRun        (origWitRun);
   witInitialize        (origWitRun);
   witAddOperation      (origWitRun, "A");
   witAddPart           (origWitRun, "B", WitMATERIAL);
   witSetStageByObject  (origWitRun, WitTRUE);
   witSetStochMode      (origWitRun, WitTRUE);

   makeNewWitRun        (dupWitRun);
   witInitialize        (dupWitRun);
   witAddOperation      (dupWitRun,  "A");

   witCopyOperationData (dupWitRun,  "A", origWitRun, "A");

   terminationExpected ("witCopyOperationData");
   }

//------------------------------------------------------------------------------

void testCase38 ()
   {
   WitRun * theWitRun;
   float    demandVolB[] = {100.0};

   std::cout
      << "Test of witSetPartObjectStage in stochastic solution mode."
      << std::endl;

   makeNewWitRun         (theWitRun);
   witInitialize         (theWitRun);
   witSetPreferCoin      (theWitRun,           WitTRUE);
   witSetNPeriods        (theWitRun,           1);
   witAddPart            (theWitRun, "A",      WitMATERIAL);
   witAddDemand          (theWitRun, "A", "B");
   witSetStageByObject   (theWitRun,           WitTRUE);
   witSetStochMode       (theWitRun,           WitTRUE);
   witSetPartObjectStage (theWitRun, "A",      1);
   witStochImplode       (theWitRun);
   witSetPartObjectStage (theWitRun, "A",      0);

   terminationExpected ("witSetPartObjectStage");
   }

//------------------------------------------------------------------------------

void testCase39 ()
   {
   WitRun * theWitRun;
   int      stageIdx;

   std::cout
      << "Test of witGetPartObjectStage without stochastic mode."
      << std::endl;

   makeNewWitRun         (theWitRun);
   witInitialize         (theWitRun);
   witAddPart            (theWitRun, "A",   WitMATERIAL);
   witGetPartObjectStage (theWitRun, "A", & stageIdx);

   terminationExpected ("witGetPartObjectStage");
   }

//------------------------------------------------------------------------------

void testCase40 ()
   {
   WitRun * theWitRun;
   int      stageIdx;

   std::cout
      << "Test of witGetOperationObjectStage while stageByObject is FALSE."
      << std::endl;

   makeNewWitRun              (theWitRun);
   witInitialize              (theWitRun);
   witAddPart                 (theWitRun, "A",   WitMATERIAL);
   witAddOperation            (theWitRun, "B");
   witSetStochMode            (theWitRun,        WitTRUE);
   witGetOperationObjectStage (theWitRun, "B", & stageIdx);

   terminationExpected ("witGetOperationObjectStage");
   }

//------------------------------------------------------------------------------

void testCase41 ()
   {
   WitRun * theWitRun;
   int *    periodStage;

   std::cout
      << "Test of witGetPeriodStage while stageByObject is TRUE."
      << std::endl;

   makeNewWitRun       (theWitRun);
   witInitialize       (theWitRun);
   witAddPart          (theWitRun, "A", WitMATERIAL);
   witSetStageByObject (theWitRun,      WitTRUE);
   witSetStochMode     (theWitRun,      WitTRUE);
   witGetPeriodStage   (theWitRun,    & periodStage);

   terminationExpected ("witGetPeriodStage");
   }

//------------------------------------------------------------------------------

void testCase42 ()
   {
   WitRun * theWitRun;
   float    yieldRate []  = {1,   0};
   float    execCost  []  = {0.6, 0.6};
   float    shipReward[]  = {1.0, 1.0};
   int      periodStage[] = {0,   1};
   float    demandVol []  = {0,   0};
   WitRun * cpyWitRun;

   std::cout
      << "Test of witCopyData in stage-by-period mode."
      << std::endl;

   //---------------------------------------------------------------------------
   // Core Problem
   //---------------------------------------------------------------------------

   makeNewWitRun            (theWitRun);
   witInitialize            (theWitRun);
   witSetNPeriods           (theWitRun, 2);

   witAddPart               (theWitRun, "Hold",         WitMATERIAL);
   witAddOperation          (theWitRun, "Buy");
   witAddBopEntry           (theWitRun, "Buy", "Hold");
   witAddDemand             (theWitRun, "Hold","Sell");

   witSetOperationYieldRate (theWitRun, "Buy",          yieldRate);
   witSetOperationExecCost  (theWitRun, "Buy",          execCost);

   witSetDemandShipReward   (theWitRun, "Hold", "Sell", shipReward);

   //---------------------------------------------------------------------------
   // Stochastic Data
   //---------------------------------------------------------------------------

   witSetNScenarios         (theWitRun,                 3);
   witSetStochMode          (theWitRun,                 WitTRUE);
   witSetPeriodStage        (theWitRun,                 periodStage);

   witSetCurrentScenario    (theWitRun,                 0);
   witSetProbability        (theWitRun,                 0.3);
   demandVol[1] =                                       200;
   witSetDemandDemandVol    (theWitRun, "Hold", "Sell", demandVol);

   witSetCurrentScenario    (theWitRun,                 1);
   witSetProbability        (theWitRun,                 0.4);
   demandVol[1] =                                       300;
   witSetDemandDemandVol    (theWitRun, "Hold", "Sell", demandVol);

   witSetCurrentScenario    (theWitRun,                 2);
   witSetProbability        (theWitRun,                 0.3);
   demandVol[1] =                                       400;
   witSetDemandDemandVol    (theWitRun, "Hold", "Sell", demandVol);

   //---------------------------------------------------------------------------
   // The copy WitRun.
   //---------------------------------------------------------------------------

   makeNewWitRun            (cpyWitRun);
   witCopyData              (cpyWitRun, theWitRun);
   witDisplayData           (cpyWitRun, WitSTDOUT);
   witDeleteRun             (cpyWitRun);

   witDeleteRun             (theWitRun);

   exit (0);
   }

//------------------------------------------------------------------------------

void testCase43 ()
   {
   WitRun * theWitRun;

   std::cout
      << "Test of witSetPartPartName with an empty partName."
      << std::endl;

   makeNewWitRun      (theWitRun);
   witInitialize      (theWitRun);
   witAddPart         (theWitRun, "A", WitMATERIAL);
   witSetPartPartName (theWitRun, "A", "");
   
   terminationExpected ("witSetPartPartName");
   }

//------------------------------------------------------------------------------

void testCase44 ()
   {
   WitRun * theWitRun;

   std::cout
      << "Test of witSetOperationOperationName with a operationName that "
      << std::endl
      << "matches an existing operation."
      << std::endl;

   makeNewWitRun                (theWitRun);
   witInitialize                (theWitRun);
   witAddOperation              (theWitRun, "A");
   witAddOperation              (theWitRun, "B");
   witSetOperationOperationName (theWitRun, "A", "B");
   
   terminationExpected ("witSetOperationOperationName");
   }

//------------------------------------------------------------------------------

void testCase45 ()
   {
   WitRun * theWitRun;

   std::cout
      << "Test of witSetDemandDemandName with a demandName that "
      << std::endl
      << "matches an existing demand for the same part."
      << std::endl;

   makeNewWitRun          (theWitRun);
   witInitialize          (theWitRun);
   witAddPart             (theWitRun, "A", WitMATERIAL);
   witAddDemand           (theWitRun, "A", "B");
   witAddDemand           (theWitRun, "A", "C");
   witSetDemandDemandName (theWitRun, "A", "B", "C");
   
   terminationExpected ("witSetDemandDemandName");
   }

//------------------------------------------------------------------------------

void testCase46 ()
   {
   WitRun * theWitRun;

   std::cout
      << "Test of witIssueFatalMesg."
      << std::endl;

   makeNewWitRun     (theWitRun);
   witInitialize     (theWitRun);
   witIssueFatalMesg (theWitRun);

   terminationExpected ("witIssueFatalMesg");
   }

//------------------------------------------------------------------------------

void testCase47 ()
   {
   WitRun * theWitRun;

   std::cout
      << "Test of witSet34Compatible when not allowed."
      << std::endl;

   makeNewWitRun         (theWitRun);
   witInitialize         (theWitRun);
   witSetWit34Allowed    (theWitRun, WitFALSE);
   witSetWit34Compatible (theWitRun, WitTRUE);

   terminationExpected ("witSetWit34Compatible");
   }

//------------------------------------------------------------------------------

void testCase48 ()
   {
   WitRun * theWitRun;

   std::cout
      << "Test of witSetCurrentObjective without multiple objectives mode."
      << std::endl;

   makeNewWitRun (theWitRun);
   witInitialize (theWitRun);

   witSetCurrentObjective (theWitRun, "First"); 

   terminationExpected ("witSetCurrentObjective");
  }

//------------------------------------------------------------------------------

void testCase49 ()
   {
   WitRun * theWitRun;

   std::cout
      << "Test of witStartExtOpt in multiple objectives mode."
      << std::endl;

   makeNewWitRun      (theWitRun);
   witInitialize      (theWitRun);
   witSetMultiObjMode (theWitRun, WitTRUE);
   witStartExtOpt     (theWitRun);

   terminationExpected ("witStartExtOpt");
   }

//------------------------------------------------------------------------------

void testCase50 ()
   {
   WitRun *     theWitRun;
   const char * theObjList[] = {"High|Low"};

   std::cout
      << "Test of a vertical bar in an objective name."
      << std::endl;

   makeNewWitRun       (theWitRun);
   witInitialize       (theWitRun);
   witSetMultiObjMode  (theWitRun, WitTRUE);
   witSetObjectiveList (theWitRun, 1,       theObjList);

   terminationExpected ("witSetObjectiveList");
   }

//------------------------------------------------------------------------------

void testCase51 ()
   {
   WitRun *     theWitRun;
   const char * theObjList[] = {"High"};

   std::cout
      << "Test of witSetObjectiveList with a zero-length list."
      << std::endl;

   makeNewWitRun       (theWitRun);
   witInitialize       (theWitRun);
   witSetMultiObjMode  (theWitRun, WitTRUE);
   witSetObjectiveList (theWitRun, 0,       theObjList);

   terminationExpected ("witSetObjectiveList");
   }

//------------------------------------------------------------------------------

void testCase52 ()
   {
   WitRun *     theWitRun;
   const char * theObjList[] = {"High", NULL, "Low"};

   std::cout
      << "Test of witSetObjectiveList with a NULL element in the list."
      << std::endl;

   makeNewWitRun       (theWitRun);
   witInitialize       (theWitRun);
   witSetMultiObjMode  (theWitRun, WitTRUE);
   witSetObjectiveList (theWitRun, 3,       theObjList);

   terminationExpected ("witSetObjectiveList");
   }

//------------------------------------------------------------------------------

void testCase53 ()
   {
   WitRun *     origWitRun;
   WitRun *     dupWitRun;
   const char * theObjList[] = {"High", "Medium", "Low"};

   std::cout
      << "Test of witCopyOperationData where origWitRun is in "
      << "multiple objectives mode."
      << std::endl;

   makeNewWitRun        (origWitRun);
   witInitialize        (origWitRun);
   witSetMultiObjMode   (origWitRun,     WitTRUE);
   witSetObjectiveList  (origWitRun,     3, theObjList);
   witAddOperation      (origWitRun, "A");

   makeNewWitRun        (dupWitRun);
   witInitialize        (dupWitRun);
   witAddOperation      (dupWitRun,  "B");

   witCopyOperationData (dupWitRun,  "B", origWitRun, "A");

   terminationExpected ("witCopyOperationData");
   }

//------------------------------------------------------------------------------

void testCase54 ()
   {
   WitRun *     origWitRun;
   WitRun *     dupWitRun;
   const char * theObjList[] = {"High", "Medium", "Low"};

   std::cout
      << "Test of witCopyPartData where dupWitRun is in "
      << "multiple objectives mode."
      << std::endl;

   makeNewWitRun       (origWitRun);
   witInitialize       (origWitRun);
   witAddPart          (origWitRun, "A", WitCAPACITY);

   makeNewWitRun       (dupWitRun);
   witInitialize       (dupWitRun);
   witSetMultiObjMode  (dupWitRun,       WitTRUE);
   witAddPart          (dupWitRun,  "B", WitCAPACITY);

   witCopyPartData     (dupWitRun,  "B", origWitRun, "A");

   terminationExpected ("witCopyPartData");
   }

//------------------------------------------------------------------------------

void testCase55 ()
   {
   WitRun *   theWitRun;
   witBoolean modHeurAlloc;
   float      incVol;

   std::cout
      << "Test of modifiable heuristic allocation."
      << std::endl;

   //...........................................................................

   makeNewWitRun            (theWitRun);
   witInitialize            (theWitRun);
   witSetNPeriods           (theWitRun, 2);
   witSetSelectionRecovery  (theWitRun, WitTRUE);
   witSetSkipFailures       (theWitRun, WitFALSE);
   witSetIndependentOffsets (theWitRun, WitTRUE);
   witSetModHeurAlloc       (theWitRun, WitTRUE);

   witGetModHeurAlloc       (theWitRun, & modHeurAlloc);

   assert                                (modHeurAlloc);

   //...........................................................................

   witAddPart               (theWitRun, "A", WitMATERIAL);
   witAddPart               (theWitRun, "B", WitMATERIAL);
   witAddPart               (theWitRun, "C", WitMATERIAL);
   witAddPart               (theWitRun, "D", WitMATERIAL);

   witAddOperation          (theWitRun, "E");
   witAddOperation          (theWitRun, "F");

   witAddBomEntry           (theWitRun, "E", "A");
   witAddBomEntry           (theWitRun, "F", "A");
   witAddBomEntry           (theWitRun, "F", "B");

   witAddSubsBomEntry       (theWitRun, "E", 0,   "B");

   witAddBopEntry           (theWitRun, "E", "C");
   witAddBopEntry           (theWitRun, "E", "D");
   witAddBopEntry           (theWitRun, "F", "D");

   witAddDemand             (theWitRun, "C", "G");
   witAddDemand             (theWitRun, "D", "H");

   //...........................................................................

   witSetBomEntryOffset       (theWitRun, "F", 0,    floatVec (-1, -1));
   witSetSubsBomEntryOffset   (theWitRun, "E", 0, 0, floatVec ( 1,  1));
   witSetBopEntryOffset       (theWitRun, "E", 1,    floatVec ( 1,  1));

   witSetBomEntryConsRate     (theWitRun, "F", 0,    floatVec ( 4,  4));
   witSetBomEntryConsRate     (theWitRun, "F", 1,    floatVec ( 3,  3));
   witSetSubsBomEntryConsRate (theWitRun, "E", 0, 0, floatVec ( 2,  2));
   witSetBopEntryProductRate  (theWitRun, "E", 1,    floatVec ( 2,  2));
   witSetBopEntryProductRate  (theWitRun, "F", 0,    floatVec ( 3,  3));

   witSetPartSupplyVol        (theWitRun, "A",       floatVec (500, 0));
   witSetPartSupplyVol        (theWitRun, "B",       floatVec (500, 0));

   witSetDemandDemandVol      (theWitRun, "C", "G",  floatVec (100, 0));
   witSetDemandDemandVol      (theWitRun, "D", "H",  floatVec (100, 0));

   //...........................................................................

   witStartHeurAlloc        (theWitRun);

   witIncHeurAlloc          (theWitRun, "D", "H", 0, 30, & incVol);

   witInterpretDevCmd       (theWitRun, "Print availability schedule.");

   witSetSubsBomEntrySubVol (theWitRun, "E",  0,  0, floatVec (0, 30));

   witSetDemandShipVol      (theWitRun, "C", "G",    floatVec (0, 30));

   witSetOperationExecVol   (theWitRun, "E", floatVec (0, 40));
   witSetOperationExecVol   (theWitRun, "F", floatVec (20, 0));

   witInterpretDevCmd       (theWitRun, "Print availability schedule.");

   witIncHeurAlloc          (theWitRun, "D", "H", 1, 40, & incVol);

   witInterpretDevCmd       (theWitRun, "Print availability schedule.");

   witFinishHeurAlloc       (theWitRun);

   //...........................................................................

   witDeleteRun             (theWitRun);
   }

//------------------------------------------------------------------------------

void testCase56 ()
   {
   WitRun * theWitRun;

   std::cout
      << "Test of modifiable heuristic allocation when selectionRecovery is "
         "FALSE."
      << std::endl;

   makeNewWitRun       (theWitRun);
   witInitialize       (theWitRun);
   witSetSkipFailures  (theWitRun, WitFALSE);
   witSetModHeurAlloc  (theWitRun, WitTRUE);

   witStartHeurAlloc   (theWitRun);

   terminationExpected ("witStartHeurAlloc");
   }

//------------------------------------------------------------------------------

void testCase57 ()
   {
   WitRun * theWitRun;
   float    incVol;

   std::cout
      << "Test of setting modHeurAlloc during heuristic allocation."
      << std::endl;

   makeNewWitRun           (theWitRun);
   witInitialize           (theWitRun);
   witSetSelectionRecovery (theWitRun, WitTRUE);
   witSetSkipFailures      (theWitRun, WitFALSE);
   witSetNPeriods          (theWitRun, 2);

   witAddPart              (theWitRun, "A", WitMATERIAL);

   witAddDemand            (theWitRun, "A", "B");

   witSetPartSupplyVol     (theWitRun, "A",       floatVec (100, 100));

   witSetDemandDemandVol   (theWitRun, "A", "B",  floatVec (100, 100));

   witStartHeurAlloc       (theWitRun);

   witSetModHeurAlloc      (theWitRun, WitTRUE);

   witIncHeurAlloc         (theWitRun, "A", "B", 1, 30, & incVol);

   terminationExpected     ("witIncHeurAlloc");
   }

//------------------------------------------------------------------------------

void testCase58 ()
   {
   WitRun * theWitRun;
   float    incVol;

   std::cout
      << "Test of violation of the non-executable constraint during "
      << std::endl
      << "modifiable heuristic allocation."
      << std::endl;

   makeNewWitRun           (theWitRun);
   witInitialize           (theWitRun);
   witSetSelectionRecovery (theWitRun, WitTRUE);
   witSetSkipFailures      (theWitRun, WitFALSE);
   witSetExecEmptyBom      (theWitRun, WitFALSE);
   witSetNPeriods          (theWitRun, 2);
   witSetModHeurAlloc      (theWitRun, WitTRUE);

   witAddPart              (theWitRun, "A", WitMATERIAL);

   witAddOperation         (theWitRun, "B");

   witAddDemand            (theWitRun, "A", "C");

   witSetDemandDemandVol   (theWitRun, "A", "C", floatVec (10, 10));

   witStartHeurAlloc       (theWitRun);

   witSetOperationExecVol  (theWitRun, "B",      floatVec (0, 0.1));

   witIncHeurAlloc         (theWitRun, "A", "C", 1, 7, & incVol);

   terminationExpected     ("witIncHeurAlloc");
   }

//------------------------------------------------------------------------------

void testCase59 ()
   {
   WitRun * theWitRun;
   const char * thePartNames  [] = {"A"};
   const char * theDemandNames[] = {"C"};
   int      shipPers          [] = {1};
   float    desIncVols        [] = {7};
   float *  incVols;

   std::cout
      << "Test of violation of the execVol hard upper bound constraint during "
      << std::endl
      << "modifiable heuristic allocation."
      << std::endl;

   makeNewWitRun             (theWitRun);
   witInitialize             (theWitRun);
   witSetSelectionRecovery   (theWitRun, WitTRUE);
   witSetSkipFailures        (theWitRun, WitFALSE);
   witSetNPeriods            (theWitRun, 2);
   witSetModHeurAlloc        (theWitRun, WitTRUE);

   witAddPart                (theWitRun, "A", WitMATERIAL);

   witAddOperation           (theWitRun, "B");

   witAddDemand              (theWitRun, "A", "C");

   witSetDemandDemandVol     (theWitRun, "A", "C", floatVec (10, 10));

   witSetOperationExecBounds (theWitRun, "B", NULL, NULL, floatVec (10, 10));

   witStartHeurAlloc         (theWitRun);

   witSetOperationExecVol    (theWitRun, "B", floatVec (10, 11));

   witEqHeurAlloc            (theWitRun,
                              1,
                              thePartNames,
                              theDemandNames,
                              shipPers,
                              desIncVols,
                            & incVols);

   terminationExpected       ("witEqHeurAlloc");
   }

//------------------------------------------------------------------------------

void testCase60 ()
   {
   WitRun * theWitRun;

   std::cout
      << "Test of violation of the lot-size constraint during"
      << std::endl
      << "modifiable heuristic allocation."
      << std::endl;

   makeNewWitRun             (theWitRun);
   witInitialize             (theWitRun);
   witSetSelectionRecovery   (theWitRun, WitTRUE);
   witSetSkipFailures        (theWitRun, WitFALSE);
   witSetNPeriods            (theWitRun, 2);
   witSetModHeurAlloc        (theWitRun, WitTRUE);

   witAddOperation           (theWitRun, "A");

   witSetOperationIncLotSize (theWitRun, "A", floatVec (10, 10));
                                              
   witStartHeurAlloc         (theWitRun);

   witSetOperationExecVol    (theWitRun, "A", floatVec (10, 11));

   witFinishHeurAlloc        (theWitRun);

   terminationExpected       ("witFinishHeurAlloc");
   }

//------------------------------------------------------------------------------

void testCase61 ()
   {
   WitRun * theWitRun;

   std::cout
      << "Test of violation of the cumulative shipment constraint during"
      << std::endl
      << "modifiable heuristic allocation."
      << std::endl;

   makeNewWitRun           (theWitRun);
   witInitialize           (theWitRun);
   witSetSelectionRecovery (theWitRun, WitTRUE);
   witSetSkipFailures      (theWitRun, WitFALSE);
   witSetNPeriods          (theWitRun, 2);
   witSetModHeurAlloc      (theWitRun, WitTRUE);

   witAddPart              (theWitRun, "A", WitMATERIAL);
   witAddDemand            (theWitRun, "A", "B");

   witSetPartSupplyVol     (theWitRun, "A",      floatVec (30,  0));

   witSetDemandDemandVol   (theWitRun, "A", "B", floatVec (10, 10));

   witStartHeurAlloc       (theWitRun);

   witSetDemandShipVol     (theWitRun, "A", "B", floatVec ( 7, 15));

   witFinishHeurAlloc      (theWitRun);

   terminationExpected     ("witFinishHeurAlloc");
   }

//------------------------------------------------------------------------------

void testCase62 ()
   {
   WitRun * theWitRun;
   float    incVol;

   std::cout
      << "Test of violation of the substitution constraint during"
      << std::endl
      << "modifiable heuristic allocation:"
      << std::endl
      << std::endl
      << "   SubVol case."
      << std::endl;

   makeNewWitRun            (theWitRun);
   witInitialize            (theWitRun);
   witSetSelectionRecovery  (theWitRun, WitTRUE);
   witSetSkipFailures       (theWitRun, WitFALSE);
   witSetUserHeurStart      (theWitRun, WitTRUE);
   witSetNPeriods           (theWitRun, 2);
   witSetModHeurAlloc       (theWitRun, WitTRUE);

   witAddPart               (theWitRun, "A", WitMATERIAL);

   witAddOperation          (theWitRun, "B");

   witAddBomEntry           (theWitRun, "B", "A");
   witAddBomEntry           (theWitRun, "B", "A");
   witAddBomEntry           (theWitRun, "B", "A");

   witAddSubsBomEntry       (theWitRun, "B",  2, "A");
   witAddSubsBomEntry       (theWitRun, "B",  2, "A");

   witAddDemand             (theWitRun, "A", "C");

   witSetPartSupplyVol      (theWitRun, "A",       floatVec (100, 100));

   witSetDemandDemandVol    (theWitRun, "A", "C",  floatVec (100, 100));

   witSetOperationExecVol   (theWitRun, "B",       floatVec  (10,  10));

   witStartHeurAlloc        (theWitRun);

   witSetSubsBomEntrySubVol (theWitRun, "B", 2, 1, floatVec (5, 17));

   witIncHeurAlloc          (theWitRun, "A", "C", 1, 30, & incVol);

   terminationExpected      ("witIncHeurAlloc");
   }

//------------------------------------------------------------------------------

void testCase63 ()
   {
   WitRun * theWitRun;
   float    incVol;

   std::cout
      << "Test of violation of the substitution constraint during"
      << std::endl
      << "modifiable heuristic allocation:"
      << std::endl
      << std::endl
      << "   ExecVol case."
      << std::endl;

   makeNewWitRun            (theWitRun);
   witInitialize            (theWitRun);
   witSetSelectionRecovery  (theWitRun, WitTRUE);
   witSetSkipFailures       (theWitRun, WitFALSE);
   witSetUserHeurStart      (theWitRun, WitTRUE);
   witSetNPeriods           (theWitRun, 2);
   witSetModHeurAlloc       (theWitRun, WitTRUE);

   witAddPart               (theWitRun, "A", WitMATERIAL);

   witAddOperation          (theWitRun, "B");

   witAddBomEntry           (theWitRun, "B", "A");
   witAddBomEntry           (theWitRun, "B", "A");
   witAddBomEntry           (theWitRun, "B", "A");

   witAddSubsBomEntry       (theWitRun, "B",  2, "A");
   witAddSubsBomEntry       (theWitRun, "B",  2, "A");

   witAddDemand             (theWitRun, "A", "C");

   witSetPartSupplyVol      (theWitRun, "A",       floatVec (100, 100));

   witSetDemandDemandVol    (theWitRun, "A", "C",  floatVec (100, 100));

   witSetOperationExecVol   (theWitRun, "B",       floatVec  (10,  10));

   witSetSubsBomEntrySubVol (theWitRun, "B", 2, 0, floatVec   (5,   3));
   witSetSubsBomEntrySubVol (theWitRun, "B", 2, 1, floatVec   (5,   4));

   witStartHeurAlloc        (theWitRun);

   witSetOperationExecVol   (theWitRun, "B",       floatVec  (10,   5));

   witIncHeurAlloc          (theWitRun, "A", "C", 1, 30, & incVol);

   terminationExpected      ("witIncHeurAlloc");
   }

//------------------------------------------------------------------------------

void testCase64 ()
   {
   WitRun * theWitRun;
   float    incVol;

   std::cout
      << "Test of violation of the scrapVol constraint during"
      << std::endl
      << "modifiable heuristic allocation:"
      << std::endl
      << std::endl
      << "   ShipVol case."
      << std::endl;

   makeNewWitRun           (theWitRun);
   witInitialize           (theWitRun);
   witSetSelectionRecovery (theWitRun, WitTRUE);
   witSetSkipFailures      (theWitRun, WitFALSE);
   witSetNPeriods          (theWitRun, 2);
   witSetModHeurAlloc      (theWitRun, WitTRUE);

   witAddPart              (theWitRun, "A", WitCAPACITY);

   witAddDemand            (theWitRun, "A", "B");

   witSetPartSupplyVol     (theWitRun, "A",       floatVec (10, 10));

   witSetDemandDemandVol   (theWitRun, "A", "B",  floatVec ( 0, 50));

   witStartHeurAlloc       (theWitRun);

   witSetDemandShipVol     (theWitRun, "A", "B",  floatVec ( 0, 10.0012));

   witIncHeurAlloc         (theWitRun, "A", "B", 0, 2, & incVol);

   terminationExpected     ("witIncHeurAlloc");
   }

//------------------------------------------------------------------------------

void testCase65 ()
   {
   WitRun * theWitRun;
   float    incVol;

   std::cout
      << "Test of violation of the scrapVol constraint during"
      << std::endl
      << "modifiable heuristic allocation:"
      << std::endl
      << std::endl
      << "   ExecVol, BOM entry & material case."
      << std::endl;

   makeNewWitRun           (theWitRun);
   witInitialize           (theWitRun);
   witSetSelectionRecovery (theWitRun, WitTRUE);
   witSetSkipFailures      (theWitRun, WitFALSE);
   witSetNPeriods          (theWitRun, 2);

   witSetModHeurAlloc      (theWitRun, WitTRUE);

   witAddPart              (theWitRun, "A", WitMATERIAL);

   witAddOperation         (theWitRun, "B");

   witAddBomEntry          (theWitRun, "B", "A");

   witAddDemand            (theWitRun, "A", "C");

   witSetPartSupplyVol     (theWitRun, "A",       floatVec (10,  0));

   witSetDemandDemandVol   (theWitRun, "A", "C",  floatVec ( 0, 10));

   witStartHeurAlloc       (theWitRun);

   witIncHeurAlloc         (theWitRun, "A", "C", 1, 7, & incVol);

   witSetOperationExecVol  (theWitRun, "B",       floatVec ( 7,  0));

   witIncHeurAlloc         (theWitRun, "A", "C", 1, 3, & incVol);

   terminationExpected     ("witIncHeurAlloc");
   }

//------------------------------------------------------------------------------

void testCase66 ()
   {
   WitRun * theWitRun;
   float    incVol;

   std::cout
      << "Test of violation of the scrapVol constraint during"
      << std::endl
      << "modifiable heuristic allocation:"
      << std::endl
      << std::endl
      << "   ExecVol & BOP entry case."
      << std::endl;

   makeNewWitRun           (theWitRun);
   witInitialize           (theWitRun);
   witSetSelectionRecovery (theWitRun, WitTRUE);
   witSetSkipFailures      (theWitRun, WitFALSE);
   witSetNPeriods          (theWitRun, 2);

   witSetModHeurAlloc      (theWitRun, WitTRUE);

   witAddPart              (theWitRun, "A", WitCAPACITY);

   witAddOperation         (theWitRun, "B");
   witAddOperation         (theWitRun, "C");

   witAddBomEntry          (theWitRun, "C", "A");

   witAddBopEntry          (theWitRun, "B", "A");

   witAddDemand            (theWitRun, "A", "C");

   witSetBopEntryOffset    (theWitRun, "B",  0,   floatVec (-1, -1)); 

   witSetPartSupplyVol     (theWitRun, "A",       floatVec (10, 10));

   witSetDemandDemandVol   (theWitRun, "A", "C",  floatVec ( 0, 20));

   witStartHeurAlloc       (theWitRun);

   witIncHeurAlloc         (theWitRun, "A", "C", 1, 17, & incVol);

   witSetOperationExecVol  (theWitRun, "B",       floatVec ( 4,  0));

   witSetPartSupplyVol     (theWitRun, "A",       floatVec (20, 20));

   terminationExpected     ("witSetPartSupplyVol");
   }

//------------------------------------------------------------------------------

void testCase67 ()
   {
   WitRun * theWitRun;
   float    incVol;

   std::cout
      << "Test of violation of the scrapVol constraint during"
      << std::endl
      << "modifiable heuristic allocation:"
      << std::endl
      << std::endl
      << "   SubVol & Substitute case."
      << std::endl;

   makeNewWitRun            (theWitRun);
   witInitialize            (theWitRun);
   witSetSelectionRecovery  (theWitRun, WitTRUE);
   witSetSkipFailures       (theWitRun, WitFALSE);
   witSetNPeriods           (theWitRun, 2);

   witSetModHeurAlloc       (theWitRun, WitTRUE);

   witAddPart               (theWitRun, "A", WitCAPACITY);
   witAddPart               (theWitRun, "B", WitCAPACITY);

   witAddOperation          (theWitRun, "C");

   witAddBomEntry           (theWitRun, "C", "A");

   witAddSubsBomEntry       (theWitRun, "C",  0, "B");

   witSetPartSupplyVol      (theWitRun, "A",         floatVec (10, 10));
   witSetPartSupplyVol      (theWitRun, "B",         floatVec (10, 10));

   witStartHeurAlloc        (theWitRun);

   witSetOperationExecVol   (theWitRun, "C",         floatVec ( 0, 27));

   witSetSubsBomEntrySubVol (theWitRun, "C",  0,  0, floatVec ( 0, 19));

   witFinishHeurAlloc       (theWitRun);

   terminationExpected      ("witFinishHeurAlloc");
   }

//------------------------------------------------------------------------------

void testCase68 ()
   {
   WitRun * theWitRun;
   float    incVol;

   std::cout
      << "Test of violation of the scrapVol constraint during"
      << std::endl
      << "modifiable heuristic allocation:"
      << std::endl
      << std::endl
      << "   SubVol & BOM entry case."
      << std::endl;

   makeNewWitRun            (theWitRun);
   witInitialize            (theWitRun);
   witSetSelectionRecovery  (theWitRun, WitTRUE);
   witSetSkipFailures       (theWitRun, WitFALSE);
   witSetUserHeurStart      (theWitRun, WitTRUE);
   witSetNPeriods           (theWitRun, 2);

   witSetModHeurAlloc       (theWitRun, WitTRUE);

   witAddPart               (theWitRun, "A", WitCAPACITY);
   witAddPart               (theWitRun, "B", WitCAPACITY);

   witAddOperation          (theWitRun, "C");

   witAddBomEntry           (theWitRun, "C", "A");

   witAddSubsBomEntry       (theWitRun, "C",  0, "B");

   witSetPartSupplyVol      (theWitRun, "A",         floatVec (10, 10));
   witSetPartSupplyVol      (theWitRun, "B",         floatVec (10, 10));

   witSetOperationExecVol   (theWitRun, "C",         floatVec ( 0, 17));

   witSetSubsBomEntrySubVol (theWitRun, "C",  0,  0, floatVec ( 0,  9));

   witStartHeurAlloc        (theWitRun);

   witSetSubsBomEntrySubVol (theWitRun, "C",  0,  0, floatVec ( 0,  0));

   witFinishHeurAlloc       (theWitRun);

   terminationExpected      ("witFinishHeurAlloc");
   }

//------------------------------------------------------------------------------

void testCase69 ()
   {
   WitRun * theWitRun;

   std::cout
      << "Test of adding a CPLEX parameter specification with the name: "
      << "\"NO_PARAM\".";

   makeNewWitRun         (theWitRun);
   witInitialize         (theWitRun);
   witAddIntCplexParSpec (theWitRun, "NO_PARAM", 17);

   terminationExpected   ("witAddIntCplexParSpec");
   }

//------------------------------------------------------------------------------

void testCase70 ()
   {
   WitRun * theWitRun;
   int      lenLists;
   char * * partNameList;
   int *    periodList;
   float *  peggedSupplyVolList;

   std::cout
      << "Test of PIP to Operations on a non-PIP-enabled operation.";

   makeNewWitRun   (theWitRun);
   witInitialize   (theWitRun);

   witAddOperation (theWitRun, "A");
   witHeurImplode  (theWitRun);
   witBuildPip     (theWitRun);

   witGetOperationSupplyVolPip (
        theWitRun,
        "A",
        3,
      & lenLists,
      & partNameList,
      & periodList,
      & peggedSupplyVolList);

   terminationExpected ("witGetOperationSupplyVolPip");
   }

//------------------------------------------------------------------------------

void testCase71 ()
   {
   WitRun * theWitRun;

   witBoolean devMode;
   int        lenLists;
   char * *   partNameList;
   int *      periodList;
   float *    peggedSupplyVolList;

   std::cout
      << "Test of PIP retrieval when the PIP has not been computed.";

   makeNewWitRun         (theWitRun);
   witInitialize         (theWitRun);

   witAddPart            (theWitRun, "A", WitMATERIAL);
   witAddDemand          (theWitRun, "A", "B");

   witGetDemandSupplyVolPip (
        theWitRun,
        "A",
        "B",
        2,
      & lenLists,
      & partNameList,
      & periodList,
      & peggedSupplyVolList);

   terminationExpected ("witGetDemandSupplyVolPip");
   }

//------------------------------------------------------------------------------

const float * floatVec (float theFlt0, float theFlt1)
   {
   static float theVec[2];

   theVec[0] = theFlt0;
   theVec[1] = theFlt1;

   return theVec;
   }

//------------------------------------------------------------------------------

void terminationExpected (const char * theFuncName)
   {
   std::cerr
      << std::endl
      << "ERROR: errTest should have terminated during the call to "
      << theFuncName
      << "."
      << std::endl
      << std::endl
      << "       errTest now terminates."
      << std::endl;

   exit (1);
   }

//------------------------------------------------------------------------------

void handleErrorExc (WitErrorExc & theErrorExc)
   {
   std::cerr
      << std::endl
      << "WIT has thrown an exception of type WitErrorExc."
      << std::endl
      << "The API function was "
      << theErrorExc.funcName ()
      << "."
      << std::endl
      << "The message number was "
      << theErrorExc.mesgNum ()
      << "."
      << std::endl
      << "The return code was "
      << theErrorExc.retCode ()
      << "."
      << std::endl
      << std::endl;
   }

//------------------------------------------------------------------------------

void handleUnknownExc ()
   {
   std::cerr
      << std::endl
      << "ERROR: WIT has thrown an exception of unknown type."
      << std::endl
      << std::endl;

   exit (1);
   }

//------------------------------------------------------------------------------

void makeNewWitRun (WitRun * & theWitRun)
   {
   witNewRun          (& theWitRun);

   witSetMesgTimesPrint (theWitRun, WitTRUE, 1060, 0); 
   }
