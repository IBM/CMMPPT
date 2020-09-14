//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "WitRun.C"
//
// Contains some of the implementation of class WitRun.
// The full implementation of class WitRun is in this file plus the source files
// whose names end in "Api.C", e.g., partApi.C.
//------------------------------------------------------------------------------

#include <WitRun.h>
#include <ApiMgr.h>
#include <InputID.h>
#include <ApiCall.h>
#include <Pre.h>
#include <Post.h>
#include <DelCompItr.h>
#include <GlobalComp.h>
#include <OptComp.h>
#include <DelComp.h>
#include <DetOptImpMgr.h>
#include <ExtOptMgr.h>
#include <StochImpMgr.h>
#include <OSRealArg.h>
#include <OVRealArg.h>
#include <HeurImp.h>
#include <HeurAllMgr.h>
#include <HeurModifier.h>
#include <OrigMrp.h>
#include <DataRead.h>
#include <DataWrit.h>
#include <SolnWrit.h>
#include <Part.h>
#include <Demand.h>
#include <Operation.h>
#include <BoundSet.h>
#include <PipMgr.h>
#include <FlexVec.h>
#include <SaeMgr.h>
#include <MsgFrag.h>
#include <MsgFac.h>

#include <stdlib.h>

//------------------------------------------------------------------------------
// VALID_CERTIFICATE
//
// The value of the certificate_ data member of any valid WitRun.
//------------------------------------------------------------------------------

#define VALID_CERTIFICATE 1234567890

//------------------------------------------------------------------------------

WitRun::~WitRun ()
   {
   delete mySession_;
   delete myApiMgr_;

   certificate_ = 0;
   }

//------------------------------------------------------------------------------
// witAdvanceObjItr
// API function to advance object iteration.
//------------------------------------------------------------------------------

void WitRun::witAdvanceObjItr ()
   {
   myApiMgr_->allowExtOptActive ();
   myApiMgr_->startInnerFunc    ();

   myCompMgr ()->myDelCompItr ()->advance ();
   }

//------------------------------------------------------------------------------

void WitRun::witBuildPip ()
   {
   myApiMgr_->startInnerFunc ();

   myProblem ()->myPipMgr ()->buildPegging ();
   }

//------------------------------------------------------------------------------

void WitRun::witClearPegging ()
   {
   myApiMgr_->startInnerFunc ();

   if (not myGlobalComp ()->perfPegging ())
      myMsgFac () ("reqPerfPeggingSmsg");
      
   myProblem ()->myHeurAllMgr ()->clearPegging ();
   }

//------------------------------------------------------------------------------

void WitRun::witCopyData (WitRun * origWitRun)
   {
   myApiMgr_->allowPreInit      ();
   myApiMgr_->allowExtOptActive ();
   myApiMgr_->startInnerFunc    ();

   if (mySession_->active ())
      if (myDetOptImpMgr ()->extOptActive ())
         myMsgFac () ("extOptActiveWR2Smsg", "witCopyData", "dupWitRun");

   forbidNullArg (origWitRun, "origWitRun");

   myApiMgr_->start2WitRunFunc (origWitRun->myApiMgr_);

   if (not origWitRun->mySession_->active ())
      myMsgFac () ("uninitWitRun2Smsg", "witCopyData", "origWitRun");

   if (origWitRun->myDetOptImpMgr ()->extOptActive ())
      myMsgFac () ("extOptActiveWR2Smsg", "witCopyData", "origWitRun");

   mySession ()->copyData (origWitRun->mySession_);
   }

//------------------------------------------------------------------------------
// witDeleteRun
//------------------------------------------------------------------------------

void WitRun::witDeleteRun ()
   {
   myApiMgr_->skipCallMsg       ();
   myApiMgr_->allowPreInit      ();
   myApiMgr_->allowExtOptActive ();
   myApiMgr_->startInnerFunc    ();

   mySession_->deactivate ();
   }

//------------------------------------------------------------------------------
// witGetDevMode ()
//------------------------------------------------------------------------------

void WitRun::witGetDevMode (WitBoolean * devMode)
   {
   myApiMgr_->allowPreInit   ();
   myApiMgr_->startInnerFunc ();

   apiGet (devMode, "devMode", WitNonClass::devMode ());
   }

//------------------------------------------------------------------------------
// witDisplayData
// API callable routine to print a formatted output of the input data.
//------------------------------------------------------------------------------

void WitRun::witDisplayData (const char * filename)
   {
   myApiMgr_->startInnerFunc ();

   myProblem ()->displayData (filename);
   }

//------------------------------------------------------------------------------
// witEvalObjectives ()
// API callable routine to evaluate objective function values
//------------------------------------------------------------------------------

void WitRun::witEvalObjectives ()
   {
   myApiMgr_->startInnerFunc ();

   forbidStochMode ();

   myDetOptImpMgr ()->evalObjectives ();
   }

//------------------------------------------------------------------------------
// witFinishExtOpt
// WIT API to conclude external optimizing implosion.
//------------------------------------------------------------------------------

void WitRun::witFinishExtOpt ()
   {
   myApiMgr_->allowExtOptActive ();
   myApiMgr_->startInnerFunc    ();

   if (not myDetOptImpMgr ()->extOptActive ())
      myMsgFac () ("extOptInactiveSmsg", apiFuncName ());

   if (not myDetOptImpMgr ()->myExtOptMgr ()->solnProvided ())
      myMsgFac () ("finishExtOptWoSolnSmsg");

   myDetOptImpMgr ()->finishExtOpt ();
   }

//------------------------------------------------------------------------------
// witFinishHeurAlloc
// API function to conclude heuristic allocation.
//------------------------------------------------------------------------------

void WitRun::witFinishHeurAlloc ()
   {
   myApiMgr_->startInnerFunc ();

   if (not myProblem ()->myHeurAllMgr ()->active ())
      myMsgFac () ("inactiveHeurAllocSmsg");

   myProblem ()->myHeurAllMgr ()->finish ();
   }

//------------------------------------------------------------------------------
// witGeneratePriorities ()
// API callable routine to generate priorities
//------------------------------------------------------------------------------

void WitRun::witGeneratePriorities ()
   {
   myApiMgr_->startInnerFunc ();

   forbidStochMode ();

   WitDemand::genPriorities (myProblem ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetExpCycle (
      int *      lenLists,
      char * * * partNameList,
      char * * * operationNameList)
   {
   WitObjStack <WitPart>      theParts (myProblem ());
   WitObjStack <WitOperation> theOpns  (myProblem ());
   int                        theIdx;
   WitPart *                  thePart;
   WitOperation *             theOpn;
   
   myApiMgr_->startInnerFunc ();

   myProblem ()->myPreprocessor ()->getExpCycle (theParts, theOpns);

   apiGet (lenLists, "lenLists", theOpns.nElements ());

   forbidNullArg (partNameList,      "partNameList");
   forbidNullArg (operationNameList, "operationNameList");

   apiAlloc (partNameList,      theParts.nElements ());
   apiAlloc (operationNameList, theOpns .nElements ());

   theIdx = -1;

   while (theParts.pop (thePart))
      {
      theIdx ++;

      (* partNameList)[theIdx] = thePart->partName ().apiDupCstring ();
      }

   theIdx = -1;

   while (theOpns.pop (theOpn))
      {
      theIdx ++;

      (* operationNameList)[theIdx] = theOpn->operationName ().apiDupCstring ();
      }
   }

//------------------------------------------------------------------------------

void WitRun::witGetExtOptIntVarIndices (int * * indices, int * len)
   {
   WitVector <int> indicesLoc;

   myApiMgr_->startInnerFunc ();

   if (not myDetOptImpMgr ()->extOptActive ())
      myMsgFac () ("extOptInactiveSmsg", apiFuncName ());

   myDetOptImpMgr ()->
      myExtOptMgr ()->
         getIntVarIndices (indicesLoc);

   apiGet (indices, "indices", indicesLoc);
   apiGet (len,     "len",     indicesLoc.length ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetExtOptLpProb (
      int *                numcols,
      int *                numrows,
      int * *              start,
      int * *              index,
      const WitOVRealArg & value,
      const WitOVRealArg & collb,
      const WitOVRealArg & colub,
      const WitOVRealArg & obj,
      const WitOVRealArg & rowlb,
      const WitOVRealArg & rowub)
   {
   WitVector <int>    startLoc;
   WitVector <int>    indexLoc;
   WitVector <double> valueLoc;
   WitVector <double> collbLoc;
   WitVector <double> colubLoc;
   WitVector <double> objLoc;
   WitVector <double> rowlbLoc;
   WitVector <double> rowubLoc;

   myApiMgr_->startInnerFunc ();

   if (not myDetOptImpMgr ()->extOptActive ())
      myMsgFac () ("extOptInactiveSmsg", apiFuncName ());

   myDetOptImpMgr ()->
      myExtOptMgr ()->
         getLpProb (
            startLoc,
            indexLoc,
            valueLoc,
            collbLoc,
            colubLoc,
            objLoc,
            rowlbLoc,
            rowubLoc);

   apiGet (numcols, "numcols", collbLoc.length ());
   apiGet (numrows, "numrows", rowlbLoc.length ());
   apiGet (start,   "start",   startLoc);
   apiGet (index,   "index",   indexLoc);
   apiGet (value,   "value",   valueLoc);
   apiGet (collb,   "collb",   collbLoc);
   apiGet (colub,   "colub",   colubLoc);
   apiGet (obj,     "obj",     objLoc);
   apiGet (rowlb,   "rowlb",   rowlbLoc);
   apiGet (rowub,   "rowub",   rowubLoc);
   }

//------------------------------------------------------------------------------
// witGetObjItrState
// API function retrieve the object type of data object at which object
// iteration is currently located.
//------------------------------------------------------------------------------

void WitRun::witGetObjItrState (WitAttr * objItrState)
   {
   myApiMgr_->startInnerFunc ();

   apiGet (
      objItrState, 
      "objItrState", 
      myCompMgr ()->myDelCompItr ()->apiObjItrState ());
   }

//------------------------------------------------------------------------------
// WIT API for heuristic implosion
//------------------------------------------------------------------------------

void WitRun::witHeurImplode ()
   {
   myApiMgr_->startInnerFunc ();

   forbidStochMode ();

   myProblem ()->myHeurImploder ()->implode (false);
   }

//------------------------------------------------------------------------------
// witInitialize
// API function to put this WitRun into its initial state, excluding the
// message facility.
//------------------------------------------------------------------------------

void WitRun::witInitialize ()
   {
   myApiMgr_->allowPreInit      ();
   myApiMgr_->allowExtOptActive ();
   myApiMgr_->startInnerFunc    ();

   mySession ()->activate ();
   }

//------------------------------------------------------------------------------
// witIssueFatalMesg
//
// Issues a fatal message.
// To be used for testing.
//------------------------------------------------------------------------------

void WitRun::witIssueFatalMesg ()
   {
   myApiMgr_->startInnerFunc ();

   stronglyAssert (false);
   }

//------------------------------------------------------------------------------
// API entry point for WIT-MRP.
//------------------------------------------------------------------------------

void WitRun::witMrp ()
   {
   myApiMgr_->startInnerFunc ();

   forbidStochMode ();

   myProblem ()->myOrigMrpExp ()->mrp ();
   }

//------------------------------------------------------------------------------
// witNewRun
//
// This function is just a place-holder to be called by the expansion of the
// STANDARD_API_IMPL macro in api.C.
//------------------------------------------------------------------------------

void WitRun::witNewRun ()
   {
   myApiMgr_->skipCallMsg       ();
   myApiMgr_->allowPreInit      ();
   myApiMgr_->allowExtOptActive ();
   myApiMgr_->startInnerFunc    ();
   }

//------------------------------------------------------------------------------
// witOptImplode
// WIT API to perform implosion by optimization.
//------------------------------------------------------------------------------

void WitRun::witOptImplode ()
   {
   myApiMgr_->startInnerFunc ();

   forbidStochMode ();

   myDetOptImpMgr ()->implode ();
   }

//------------------------------------------------------------------------------
// witOptPreprocess ()
// Same as witPreprocess.
// No longer documented.
//------------------------------------------------------------------------------

void WitRun::witOptPreprocess ()
   {
   myApiMgr_->startInnerFunc ();

   myProblem ()->myPreprocessor ()->preprocess ();
   }

//------------------------------------------------------------------------------
// witPostprocess ()
// API callable routine to perform general postprocess of data.
//------------------------------------------------------------------------------

void WitRun::witPostprocess ()
   {
   myApiMgr_->startInnerFunc ();

   myProblem ()->myPostprocessor ()->postprocess ();
   }

//------------------------------------------------------------------------------
// witPreprocess ()
// API callable routine to perform general preprocessing of data.
//------------------------------------------------------------------------------

void WitRun::witPreprocess ()
   {
   myApiMgr_->startInnerFunc ();

   myProblem ()->myPreprocessor ()->preprocess ();
   }

//------------------------------------------------------------------------------

void WitRun::witPurgeData ()
   {
   myApiMgr_->startInnerFunc ();

   forbidStochMode ();

   myCompMgr ()->purgeData ();
   }

//------------------------------------------------------------------------------
// witReadData
// API callable routine to read the input data file.
//------------------------------------------------------------------------------

void WitRun::witReadData (const char * fileName)
   {
   myApiMgr_->startInnerFunc ();

   forbidNullArg (fileName, "fileName");

   myProblem ()->myDataReader ()->readData (fileName);
   }

//------------------------------------------------------------------------------
// witResetObjItr
// API function to reset object iteration.
//------------------------------------------------------------------------------

void WitRun::witResetObjItr ()
   {
   myApiMgr_->allowExtOptActive ();
   myApiMgr_->startInnerFunc    ();

   myCompMgr ()->myDelCompItr ()->reset ();
   }

//------------------------------------------------------------------------------
// witSetExtOptSoln
// Sets the primal solution in ext opt implosion mode.
//------------------------------------------------------------------------------

void WitRun::witSetExtOptSoln (const WitIVRealArg & colsol)
   {
   myApiMgr_->allowExtOptActive ();
   myApiMgr_->startInnerFunc    ();

   if (not myDetOptImpMgr ()->extOptActive ())
      myMsgFac () ("extOptInactiveSmsg", apiFuncName ());

   myDetOptImpMgr ()->myExtOptMgr ()->setSoln (colsol);
   }

//------------------------------------------------------------------------------
// witShutDownExtOpt
// WIT API to conclude external optimizing implosion.
//------------------------------------------------------------------------------

void WitRun::witShutDownExtOpt ()
   {
   myApiMgr_->allowExtOptActive ();
   myApiMgr_->startInnerFunc    ();

   if (not myDetOptImpMgr ()->extOptActive ())
      myMsgFac () ("extOptInactiveSmsg", apiFuncName ());

   myDetOptImpMgr ()->shutDownExtOpt ();
   }

//------------------------------------------------------------------------------
// witShutDownHeurAlloc
// API function to shut down heuristic allocation without post-processing.
//------------------------------------------------------------------------------

void WitRun::witShutDownHeurAlloc ()
   {
   myApiMgr_->startInnerFunc ();

   if (not myProblem ()->myHeurAllMgr ()->active ())
      myMsgFac () ("inactiveHeurAllocSmsg");

   myProblem ()->myHeurAllMgr ()->reset ();
   }

//------------------------------------------------------------------------------
// witStartExtOpt
// WIT API to initiate external optimizing implosion.
//------------------------------------------------------------------------------

void WitRun::witStartExtOpt ()
   {
   myApiMgr_->startInnerFunc ();

   if (myGlobalComp ()->computeCriticalList ())
      myMsgFac () ("invalidAttrForStartExtSmsg", "computeCriticalList");

   if (myOptComp ()->compPrices ())
      myMsgFac () ("invalidAttrForStartExtSmsg", "compPrices");

   if (myOptComp ()->accAfterOptImp ())
      myMsgFac () ("invalidAttrForStartExtSmsg", "accAfterOptImp");

   if (myOptComp ()->multiObjMode ())
      myMsgFac () ("startExtOptMultiObjSmsg");

   forbidStochMode ();

   myDetOptImpMgr ()->startExtOpt ();
   }

//------------------------------------------------------------------------------
// witStartHeurAlloc
// API function to initiate heuristic allocation.
//------------------------------------------------------------------------------

void WitRun::witStartHeurAlloc ()
   {
   myApiMgr_->startInnerFunc ();

   forbidStochMode ();

   myProblem ()->myHeurAllMgr ()->start ();
   }

//------------------------------------------------------------------------------
// witWriteCriticalList: Writes the critical part list.
//------------------------------------------------------------------------------

void WitRun::witWriteCriticalList (
   const char *  filename,
   WitFileFormat fileFormat,
   int           maxListLength)
   {
   myApiMgr_->startInnerFunc ();

   myProblem ()->
      mySolnWriter ()->
         writeCriticalList (
            filename,
            fileFormat,
            maxListLength);
   }

//------------------------------------------------------------------------------
// witWriteData
// API callable routine to write the input data file.
//------------------------------------------------------------------------------

void WitRun::witWriteData (const char * filename)
   {
   myApiMgr_->startInnerFunc ();

   myProblem ()->myDataWriter ()->writeData (filename);
   }

//------------------------------------------------------------------------------
// witWriteExecSched: Prints execution schedule.
//------------------------------------------------------------------------------

void WitRun::witWriteExecSched (
      const char *  filename,
      WitFileFormat fileFormat)
   {
   myApiMgr_->startInnerFunc ();

   myProblem ()->mySolnWriter ()->writeExecSched (filename, fileFormat);
   }

//------------------------------------------------------------------------------
// witWriteProdSched: Prints production schedule.
//------------------------------------------------------------------------------

void WitRun::witWriteProdSched (
   const char * filename,
   WitFileFormat fileFormat )
   {
   myApiMgr_->startInnerFunc ();

   if (wit34Compatible ())
      {
      myMsgFac () ("witWriteProdSchedWithCompatibility");

      witWriteExecSched (filename, fileFormat);
      }

   else
      myMsgFac () ("witWriteProdSchedWithoutCompatibility");
   }

//------------------------------------------------------------------------------
// witWriteReqSched: Prints supply requirements schedule.
//------------------------------------------------------------------------------

void WitRun::witWriteReqSched (
      const char * filename,
      WitFileFormat fileFormat)
   {
   myApiMgr_->startInnerFunc ();

   myProblem ()->mySolnWriter ()->writeReqSched (filename, fileFormat);
   }

//------------------------------------------------------------------------------
// witWriteShipSched: Prints the shipment schedule.
//------------------------------------------------------------------------------

void WitRun::witWriteShipSched (
   const char * filename,
   WitFileFormat fileFormat )
   {
   myApiMgr_->startInnerFunc ();

   myProblem ()->mySolnWriter ()->writeShipSched (filename, fileFormat);
   }

//------------------------------------------------------------------------------

void WitRun::witNoDependantWitRunVeneersIsOne (int * isOne)
   {
   myApiMgr_->skipCallMsg       ();
   myApiMgr_->allowPreInit      ();
   myApiMgr_->allowExtOptActive ();
   myApiMgr_->startInnerFunc    ();

   apiGet (
      isOne,
      "isOne",
      (nDepVeneers_ == 1)? 1: 0);
   }

//------------------------------------------------------------------------------

void WitRun::witNoDependantWitRunVeneersDecr ()
   {
   myApiMgr_->skipCallMsg       ();
   myApiMgr_->allowPreInit      ();
   myApiMgr_->allowExtOptActive ();
   myApiMgr_->startInnerFunc    ();

   -- nDepVeneers_;
   }

//------------------------------------------------------------------------------

void WitRun::witNoDependantWitRunVeneersIncr ()
   {
   myApiMgr_->skipCallMsg       ();
   myApiMgr_->allowPreInit      ();
   myApiMgr_->allowExtOptActive ();
   myApiMgr_->startInnerFunc    ();

   ++ nDepVeneers_;
   }

//------------------------------------------------------------------------------

void WitRun::witExhaustMemory ()
   {
   int    totDiscarded;
   char * theMem;

   myApiMgr_->startInnerFunc ();

   for (totDiscarded = 100; alwaysTrue (); totDiscarded += 100)
      {
      theMem = NULL;

      theMem = new char[104857600];

      if (theMem == NULL)
         myMsgFac () ("newRetNullFmsg");

      myMsgFac () ("discardMemoryMsg", totDiscarded);
      }
   }

//------------------------------------------------------------------------------

void WitRun::witExecStandAlone (const char * theFileName)
   {
   myApiMgr_->skipCallMsg       ();
   myApiMgr_->allowPreInit      ();
   myApiMgr_->allowExtOptActive ();
   myApiMgr_->startInnerFunc    ();

   if (WitSaeMgr::standAloneMode ())
      myMsgFac () ("overlapStandAloneSmsg");

   if (mySession_->active ())
      forbidStochMode ();

   forbidNullArg (theFileName, "theFileName");

   WitSaeMgr::execute (this, theFileName);
   }

//------------------------------------------------------------------------------

void WitRun::witInterpretDevCmd (const char * theDevCmd)
   {
   WitString theDevCmdString;

   myApiMgr_->startInnerFunc ();

   if (not devMode ())
      myMsgFac () ("intDevCmdWoDevModeSmsg");

   theDevCmdString = theDevCmd;

   myMsgFac () ("intDevCmdMsg", theDevCmdString);

   if (theDevCmdString      == "Throw an exception of unknown type.")
      throw bool ();

   else if (theDevCmdString == "Print availability schedule.")
      myProblem ()->
         myHeurAllMgr ()->
            printAvailSched ();

   else
      myMsgFac () ("invalidDevCmdSmsg");
   }

//------------------------------------------------------------------------------

WitInputID * WitRun::myInputID () const
   {
   return myApiMgr ()->myInputID ();
   }

//------------------------------------------------------------------------------

WitRun * WitRun::newInstance ()
   {
   WitRun * theWitRun;

   void (* appNewHandler) ();

   try
      {
      theWitRun = new WitRun;
      }

   catch (std::bad_alloc)
      {
      handleBadAllocForCtor ();
      }

   return theWitRun;
   }

//------------------------------------------------------------------------------

void WitRun::validateWitRun (WitRun * theWitRun, const char * theFuncName)
   {
   if (theWitRun == NULL)
      {
      startNonMsgSevereError ();

      fprintf (stderr,
         "   A WIT function was called with a NULL pointer for the WitRun "
            "argument.\n"
         "\n"
         "      Function: %s\n",
         theFuncName);

      finishNonMsgSevereError ();
      }

   if (theWitRun->certificate_ != VALID_CERTIFICATE)
      {
      startNonMsgSevereError ();

      fprintf (stderr,
         "   A WIT function was called with an invalid (possibly deleted) "
            "WitRun argument.\n"
         "\n"
         "      Function: %s\n",
         theFuncName);

      finishNonMsgSevereError ();
      }
   }

//------------------------------------------------------------------------------

void WitRun::forbidNullArg (const void * theArg, const char * argName)
   {
   witAssert (argName != NULL);

   if (theArg == NULL)
      {
      myInputID ()->setArgName (argName);

      myMsgFac () ("nullPointerSmsg");
      }
   }

//------------------------------------------------------------------------------

void WitRun::apiVoidAlloc (void * * theCVecPtr, int nElems, size_t elSize)
   {
   * theCVecPtr = NULL;

   if (nElems == 0)
      return;

   * theCVecPtr = calloc (nElems, elSize);

   if (* theCVecPtr == NULL)
      WitMsgFacility::issueByException ("callocSmsg", nElems * elSize);
   }

//------------------------------------------------------------------------------

void WitRun::apiGet (
      int *        theVarPtr,
      const char * argName,
      int          theValue)
   {
   forbidNullArg (theVarPtr, argName);

   * theVarPtr = theValue;
   }

//------------------------------------------------------------------------------

void WitRun::apiGet (
      WitBoolean * theVarPtr,
      const char * argName,
      bool         theValue)
   {
   forbidNullArg (theVarPtr, argName);

   * theVarPtr = theValue;
   }

//------------------------------------------------------------------------------

void WitRun::apiGet (
      const WitOSRealArg & theArg,
      const char *         argName,
      double               theValue)
   {
   forbidNullArg (theArg, argName);

   theArg = theValue;
   }

//------------------------------------------------------------------------------

void WitRun::apiGet (
      WitAttr *    theVarPtr,
      const char * argName,
      WitAttr      theValue)
   {
   forbidNullArg (theVarPtr, argName);

   * theVarPtr = theValue;
   }

//------------------------------------------------------------------------------

void WitRun::apiGet (
      FILE * *     theVarPtr,
      const char * argName,
      FILE *       theValue)
   {
   forbidNullArg (theVarPtr, argName);

   * theVarPtr = theValue;
   }

//------------------------------------------------------------------------------

void WitRun::apiGet (
      void * *     theVarPtr,
      const char * argName,
      void *       theValue)
   {
   forbidNullArg (theVarPtr, argName);

   * theVarPtr = theValue;
   }

//------------------------------------------------------------------------------

void WitRun::apiGet (
      char * *          theVarPtr,
      const char *      argName,
      const WitString & theValue)
   {
   forbidNullArg (theVarPtr, argName);

   * theVarPtr = theValue.apiDupCstring ();
   }

//------------------------------------------------------------------------------

void WitRun::apiGet (
      const WitOVRealArg &       theArg,
      const char *               argName,
      const WitVector <double> & theValue)
   {
   forbidNullArg (theArg, argName);

   apiAlloc (theArg, theValue.length ());

   theArg = theValue;
   }

//------------------------------------------------------------------------------

void WitRun::apiGet (
      int * *                 theVarPtr,
      const char *            argName,
      const WitVector <int> & theValue)
   {
   forbidNullArg (theVarPtr, argName);

   apiAlloc (theVarPtr, theValue.length ());

   theValue.copyInto (* theVarPtr);
   }

//------------------------------------------------------------------------------

void WitRun::apiGet (
      WitBoolean * *           theVarPtr,
      const char *             argName,
      const WitVector <bool> & theValue)
   {
   WitPeriod thePer;

   forbidNullArg (theVarPtr, argName);

   apiAlloc (theVarPtr, theValue.length ());

   forEachPeriod (thePer, this)
      (* theVarPtr)[thePer] = theValue[thePer];
   }

//------------------------------------------------------------------------------

void WitRun::apiGet (
      const WitOVRealArg &  theArg,
      const char *          argName,
      const WitDblFlexVec & theValue)
   {
   forbidNullArg (theArg, argName);

   apiAlloc (theArg, nPeriods ());

   theArg = theValue;
   }

//------------------------------------------------------------------------------

void WitRun::apiGet (
      WitBoolean * *            theVarPtr,
      const char *              argName,
      const WitFlexVec <bool> & theValue)
   {
   forbidNullArg (theVarPtr, argName);

   apiAlloc (theVarPtr, nPeriods ());

   copy (* theVarPtr, theValue);
   }

//------------------------------------------------------------------------------

void WitRun::apiGet (
      int * *                  theVarPtr,
      const char *             argName,
      const WitFlexVec <int> & theValue)
   {
   forbidNullArg (theVarPtr, argName);

   apiAlloc (theVarPtr, nPeriods ());

   theValue.copyInto (* theVarPtr);
   }

//------------------------------------------------------------------------------

void WitRun::apiGet (
      const WitOVRealArg & hardLBVec,
      const WitOVRealArg & softLBVec,
      const WitOVRealArg & hardUBVec,
      WitBoundSet *        theValue)
   {
   apiGet (hardLBVec, "hardLB", theValue->hardLB ());
   apiGet (softLBVec, "softLB", theValue->softLB ());
   apiGet (hardUBVec, "hardUB", theValue->hardUB ());
   }

//------------------------------------------------------------------------------

WitRun::WitRun ():
      certificate_ (VALID_CERTIFICATE),
      myApiMgr_    (NULL),
      mySession_   (NULL),
      nDepVeneers_ (0)
   {
   myApiMgr_  = new WitApiMgr (this);
   mySession_ = new WitSession (this);
   }

//------------------------------------------------------------------------------

WitDelComp * WitRun::curItrDelCompForGet (
      WitAttr      objItrStateVal,
      WitMsgFragID theObjTypeFragID)
   {
   WitDelComp * theDelComp;

   if (not myCompMgr ()->myDelCompItr ()->active ())
      myMsgFac () ("getObjWhileItrInactiveSmsg");

   theDelComp = myCompMgr ()->myDelCompItr ()->curDelComp ();

   if (objItrStateVal != theDelComp->apiObjItrState ())
      myMsgFac () ("getObjObjTypeMismatchSmsg",
         myMsgFac ().myFrag (theObjTypeFragID),
         theDelComp->objTypeMsgFrag ());

   return theDelComp;
   }

//------------------------------------------------------------------------------

void WitRun::handleBadAllocForCtor ()
   {
   startNonMsgSevereError ();

   fprintf (stderr,
      "Memory allocation error.\n"
      "There is not enough memory available for WIT to create a new WitRun.\n"
      "\n"
      "   Function           : witNewRun\n");

   finishNonMsgSevereError ();
   }

//------------------------------------------------------------------------------

void WitRun::startCopyObjData (WitRun * origWitRun)
   {
   myApiMgr_->allowPreInit      ();
   myApiMgr_->allowExtOptActive ();
   myApiMgr_->startInnerFunc    ();

   forbidNullArg (origWitRun, "origWitRun");

   myApiMgr_->start2WitRunFunc (origWitRun->myApiMgr_);


   if (not mySession_->active ())
      myMsgFac () ("uninitWitRun2Smsg",   apiFuncName (), "dupWitRun");

   if (not origWitRun->mySession_->active ())
      myMsgFac () ("uninitWitRun2Smsg",   apiFuncName (), "origWitRun");


   if (myDetOptImpMgr ()->extOptActive ())
      myMsgFac () ("extOptActiveWR2Smsg", apiFuncName (), "dupWitRun");

   if (origWitRun->myDetOptImpMgr ()->extOptActive ())
      myMsgFac () ("extOptActiveWR2Smsg", apiFuncName (), "origWitRun");


   if (myStochImpMgr ()->stochMode ())
      myMsgFac () ("funcInStochModeWR2Smsg", apiFuncName (), "dupWitRun");

   if (origWitRun->myStochImpMgr ()->stochMode ())
      myMsgFac () ("funcInStochModeWR2Smsg", apiFuncName (), "origWitRun");


   if (myOptComp ()->multiObjMode ())
      myMsgFac () ("funcInMultiObjModeWR2Smsg", apiFuncName (), "dupWitRun");

   if (origWitRun->myOptComp ()->multiObjMode ())
      myMsgFac () ("funcInMultiObjModeWR2Smsg", apiFuncName (), "origWitRun");


   checkGlobAttsForCopy (origWitRun);

   if (this == origWitRun)
      myMsgFac () ("sameWitRunMsg");
   else
      myMsgFac () ("distinctWitRunsMsg");
   }

//------------------------------------------------------------------------------

void WitRun::checkGlobAttsForCopy (WitRun * origWitRun)
   {
   WitGlobalComp *  dupGlobal;
   WitGlobalComp * origGlobal;

    dupGlobal =             myGlobalComp ();
   origGlobal = origWitRun->myGlobalComp ();

   if (dupGlobal->nPeriods () != origGlobal->nPeriods ())
      {
      myMsgFac () ("copyObjDiffNPeriodsSmsg",
         apiFuncName (),
          dupGlobal->nPeriods (),
         origGlobal->nPeriods ());
      }
 
   if (dupGlobal->wit34Compatible () != origGlobal->wit34Compatible ())
      {
      myMsgFac () ("copyObjDiffBoolAttSmsg",
         apiFuncName (),
                    "wit34Compatible",
          dupGlobal->wit34Compatible (),
         origGlobal->wit34Compatible ());
      }

   if (dupGlobal->independentOffsets () != origGlobal->independentOffsets ())
      {
      myMsgFac () ("copyObjDiffBoolAttSmsg",
         apiFuncName (),
                    "independentOffsets",
          dupGlobal->independentOffsets (),
         origGlobal->independentOffsets ());
      }
   }

//------------------------------------------------------------------------------

void WitRun::prepSet34Attr (
      const char * the34Object,
      const char * the34Attribute,
      const char * the40Object,
      const char * the40Attribute)
   {
   if (wit34Compatible ())
      myMsgFac () ("setting34AttributeWithCompatibilityMode",
         apiFuncName (),
         the34Object,
         the34Attribute,
         the40Object,
         the40Attribute);
   else
      myMsgFac () ("setting34AttributeWithoutCompatibilityMode",
         apiFuncName (),
         the34Object,
         the34Attribute,
         the40Object,
         the40Attribute);
   }

//------------------------------------------------------------------------------

void WitRun::prepGet34Attr (
      const char * the34Object,
      const char * the34Attribute,
      const char * the40Object,
      const char * the40Attribute)
   {
   if (wit34Compatible ())
      myMsgFac () ("getting34AttributeWithCompatibilityMode",
         apiFuncName (),
         the34Object,
         the34Attribute,
         the40Object,
         the40Attribute);
   else
      myMsgFac () ("getting34AttributeWithoutCompatibilityMode",
         apiFuncName (),
         the34Object,
         the34Attribute,
         the40Object,
         the40Attribute);
   }

//------------------------------------------------------------------------------

void WitRun::prepGetIndex (int thePer)
   {
   if (myMsgFac ().mayIssue ("periodMsg"))
      myMsgFac () ("periodMsg", thePer);

   if (not ((thePer >= 0) and (thePer < nPeriods ())))
      {
      myInputID ()->setArgName ("thePer");

      myMsgFac () ("argRangeIntLeValLtIntSmsg", thePer, 0, nPeriods ());
      }

   if (not myDetOptImpMgr ()->extOptActive ())
      myMsgFac () ("extOptInactiveSmsg", apiFuncName ());
   }

//------------------------------------------------------------------------------

void WitRun::checkNPeriodsWasSet ()
   {
   if (myCompMgr ()->nNodes () == 0)
      if (not myProblem ()->nPeriodsWasSet ())
         myMsgFac () ("nPeriodsNotSetWmsg", nPeriods ());
   }

//------------------------------------------------------------------------------

void WitRun::addingDelComp (WitMsgFragID theObjTypeFragID)
   {
   if (myStochImpMgr ()->stochMode ())
      myMsgFac () ("addStochModeSmsg", myMsgFac ().myFrag (theObjTypeFragID));
   }

//------------------------------------------------------------------------------

void WitRun::forbidStochMode ()
   {
   if (myStochImpMgr ()->stochMode ())
      myMsgFac () ("funcInStochModeSmsg", apiFuncName ());
   }

//------------------------------------------------------------------------------

const char * WitRun::apiFuncName ()
   {
   return myApiMgr ()->apiFuncName ();
   }

//------------------------------------------------------------------------------

char * WitNonClass::dupCString (const char * theCString)
   {
   char * theDup;

   apiAlloc (& theDup, strlen (theCString) + 1);

   strcpy (theDup, theCString);

   return theDup;
   }

//------------------------------------------------------------------------------
// Implementation and explicit instantiation of function template
// NonClass::apiAlloc.
//------------------------------------------------------------------------------

template <typename Elem>
      void WitNonClass::apiAlloc (Elem * * theCVecPtr, int nElems)
   {
   WitRun::apiVoidAlloc (
      reinterpret_cast <void * *> (theCVecPtr),
      nElems,
      sizeof (Elem));
   }

//------------------------------------------------------------------------------

template void WitNonClass::apiAlloc <int>      (int    * *,   int);
template void WitNonClass::apiAlloc <float>    (float  * *,   int);
template void WitNonClass::apiAlloc <double>   (double * *,   int);
template void WitNonClass::apiAlloc <char>     (char   * *,   int);
template void WitNonClass::apiAlloc <float  *> (float  * * *, int);
template void WitNonClass::apiAlloc <double *> (double * * *, int);
template void WitNonClass::apiAlloc <char   *> (char   * * *, int);

//------------------------------------------------------------------------------

void WitNonClass::apiAlloc (const WitOVRealArg & theOVRealArg, int nElems)
   {
   theOVRealArg.apiAlloc (nElems);
   }
