//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "ApiMgr.C"
//
// Contains the implementation of the following classes:
//
//    ApiMgr.
//    InputID.
//------------------------------------------------------------------------------

#include <ApiMgr.h>
#include <InputID.h>
#include <AbortApiExc.h>
#include <IssueMsgExc.h>
#include <SaeMgr.h>
#include <WitRun.h>
#include <Demand.h>
#include <Part.h>
#include <Operation.h>
#include <SubEntry.h>
#include <BomEntry.h>
#include <BopEntry.h>
#include <DetOptImpMgr.h>
#include <RealArg.h>
#include <MsgFac.h>

#include <stdlib.h>

//------------------------------------------------------------------------------
// Implementation of class ApiMgr.
//------------------------------------------------------------------------------

bool WitApiMgr::newHandlerSet_ = false;

//------------------------------------------------------------------------------

inline WitMsgFacility & WitApiMgr::myMsgFac ()
   {
   return myWitRun_->myMsgFac ();
   }

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------

WitApiMgr::WitApiMgr (WitRun * theWitRun):
      myWitRun_         (theWitRun),
      myInputID_        (NULL),
      myWitRunIsLocked_ (false),
      apiFuncName_      (NULL),
      startInnerNeeded_ (false),
      mustSkipCallMsg_  (false),
      preInitAllowed_   (false),
      extOptActAllowed_ (false),
      prevApiAbort_     (false),
      myApiMgr2_        (NULL),
      tempIntVec_       (),
      tempDblVec_       (),
      tempVecsReserved_ (false)
   {
   setUpNewFailure ();

   myInputID_ = new WitInputID (this);
   }

//------------------------------------------------------------------------------

WitApiMgr::~WitApiMgr ()
   {
   delete myInputID_;
   }

//------------------------------------------------------------------------------

void WitApiMgr::startApiFunc (WitRun * theWitRun, const char * theFuncName)
   {
   WitApiMgr * theApiMgr;

   WitRun::validateWitRun (theWitRun, theFuncName);

   theApiMgr = theWitRun->myApiMgr ();

   theApiMgr->lockMyWitRun (theFuncName);

   theApiMgr->apiRc_ = WitINFORMATIONAL_RC;

   if (theApiMgr->prevApiAbort_)
      theApiMgr->myMsgFac () ("postErrApiCallSameWRSmsg",
         theApiMgr->apiFuncName_);

   theApiMgr->startInnerNeeded_ = true;
   theApiMgr->mustSkipCallMsg_  = false;
   theApiMgr->preInitAllowed_   = false;
   theApiMgr->extOptActAllowed_ = false;
   }

//------------------------------------------------------------------------------

void WitApiMgr::finishApiFunc ()
   {
   nonMsgAssert (not startInnerNeeded_);

   if (myApiMgr2_ != NULL)
      {
      setToMax (apiRc_, myApiMgr2_->apiRc_);

      myApiMgr2_->unlockMyWitRun ();

      myApiMgr2_ = NULL;
      }

   unlockMyWitRun ();
   }

//------------------------------------------------------------------------------

void WitApiMgr::handleAnyExc ()
   {
   try
      {
      try
         {
         throw;
         }

      catch (WitAbortApiExc &)
         {
         throw;
         }

      catch (WitIssueMsgExc & theIssueMsgExc)
         {
         handleIssueMsgExc (theIssueMsgExc);
         }

      catch (std::bad_alloc &)
         {
         myMsgFac () ("memoryErrorSmsg");
         }

      catch (std::exception & theStdExc)
         {
         myMsgFac () ("stdExcFmsg", theStdExc.what ());
         }

      catch (...)
         {
         myMsgFac () ("unknownExcFmsg");
         }
      }

   catch (WitAbortApiExc & theAbortApiExc)
      {
      handleAbortApiExc (theAbortApiExc);
      }
   }

//------------------------------------------------------------------------------

void WitApiMgr::lockMyWitRun (const char * theFuncName)
   {
   if (myWitRunIsLocked_)
      {
      startNonMsgSevereError ();

      fprintf (stderr,
         "WIT Thread-safety error:\n"
         "\n"
         "   An API function was called while the previous API function call\n"
         "   with the same WitRun argument was still in progress.\n"
         "   Overlapping API function calls with the same WitRun argument\n"
         "   are not allowed.\n"
         "\n"
         "      Previous Function: %s\n"
         "      Current  Function: %s\n",
         apiFuncName_,
         theFuncName);

      finishNonMsgSevereError ();
      }

   myWitRunIsLocked_ = true;

   apiFuncName_      = theFuncName;
   }

//------------------------------------------------------------------------------

void WitApiMgr::unlockMyWitRun ()
   {
   nonMsgAssert (myWitRunIsLocked_);

   apiFuncName_      = NULL;
   myWitRunIsLocked_ = false;
   }

//------------------------------------------------------------------------------

void WitApiMgr::skipCallMsg ()
   {
   if (not startInnerNeeded_)
      return;

   nonMsgAssert (not mustSkipCallMsg_);

   mustSkipCallMsg_ = true;
   }

//------------------------------------------------------------------------------

void WitApiMgr::allowPreInit ()
   {
   if (not startInnerNeeded_)
      return;

   nonMsgAssert (not preInitAllowed_);

   preInitAllowed_ = true;
   }

//------------------------------------------------------------------------------

void WitApiMgr::allowExtOptActive ()
   {
   if (not startInnerNeeded_)
      return;

   nonMsgAssert (not extOptActAllowed_);

   extOptActAllowed_ = true;
   }

//------------------------------------------------------------------------------

void WitApiMgr::startInnerFunc ()
   {
   if (not startInnerNeeded_)
      return;

   startInnerNeeded_ = false;

   if (not mustSkipCallMsg_)
      if (myMsgFac ().mayIssue ("witFuncCalled"))
         myMsgFac () ("witFuncCalled", apiFuncName_);

   if (not preInitAllowed_)
      if (not myWitRun_->mySession ()->active ())
         myMsgFac () ("callBeforeInitSMsg", apiFuncName_);

   if (strncmp (apiFuncName_ + 3, "Get", 3) == 0)
      {
      nonMsgAssert (not extOptActAllowed_);

      extOptActAllowed_ = true;
      }

   if (not extOptActAllowed_)
      if (myWitRun_->myDetOptImpMgr ()->extOptActive ())
         myMsgFac () ("extOptActiveSmsg", apiFuncName_);

   mustSkipCallMsg_  = false;
   preInitAllowed_   = false;
   extOptActAllowed_ = false;
   }

//------------------------------------------------------------------------------

void WitApiMgr::start2WitRunFunc (WitApiMgr * theApiMgr2)
   {
   if (theApiMgr2 == this)
      return;

   nonMsgAssert (myApiMgr2_ == NULL);

   myApiMgr2_ = theApiMgr2;

   myApiMgr2_->lockMyWitRun (apiFuncName_);

   if (myApiMgr2_->prevApiAbort_)
      myMsgFac () ("postErrApiCallSameWRSmsg", apiFuncName_);

   myApiMgr2_->apiRc_ = WitINFORMATIONAL_RC;
   }

//------------------------------------------------------------------------------

void WitApiMgr::updateApiRc (WitReturnCode theRc)
   {
   setToMax (apiRc_, theRc);
   }

//------------------------------------------------------------------------------

#if defined(_MSC_VER)

#include <new.h>

void WitApiMgr::setUpNewFailure ()
   {
   if (newHandlerSet_)
      return;

   _set_new_handler (throwBadAlloc);

   newHandlerSet_ = true;
   }

#else

void WitApiMgr::setUpNewFailure ()
   {
   }

#endif

//------------------------------------------------------------------------------

int WitApiMgr::throwBadAlloc (size_t)
   {
   throw std::bad_alloc ();
   }

//------------------------------------------------------------------------------

void WitApiMgr::handleIssueMsgExc (WitIssueMsgExc & theIssueMsgExc)
   {
   myMsgFac () (theIssueMsgExc.myMsgID (),
      theIssueMsgExc.myMsgArg1 (),
      theIssueMsgExc.myMsgArg2 (),
      theIssueMsgExc.myMsgArg3 ());

   myMsgFac () ("invIssueByExcFmsg");
   }

//------------------------------------------------------------------------------

void WitApiMgr::handleAbortApiExc (WitAbortApiExc & theAbortApiExc)
   {
   WitString theFuncName;

   theFuncName   = apiFuncName_;

   if (myApiMgr2_ != NULL)
      myApiMgr2_->prevApiAbort_ = true;

   prevApiAbort_ = true;

   finishApiFunc ();

   abortApiFunc (theAbortApiExc, theFuncName);
   }

//------------------------------------------------------------------------------

void WitApiMgr::abortApiFunc (
      WitAbortApiExc &  theAbortApiExc,
      const WitString & theFuncName)
   {
   if      (theAbortApiExc.stopRunning ())
      {
      myMsgFac () ("witStopsSmsg", apiRc_);

      fflush (stdout);

      exit (apiRc_);
      }
   else if (theAbortApiExc.throwErrorExc ())
      {
      WitErrorExc theErrorExc (
         theFuncName   .myCstring (),
         theAbortApiExc.myMsgNum (),
         apiRc_);

      myMsgFac () ("throwWitExcSmsg", apiRc_);

      throw theErrorExc;
      }
   else
      myMsgFac () ("returnOnErrorSmsg", apiRc_);
   }

//------------------------------------------------------------------------------

WitTVec <WitBoolean> & WitApiMgr::reserveTempBooleanVec ()
   {
   return reserveTempIntVec ();
   }

//------------------------------------------------------------------------------

WitTVec <int> & WitApiMgr::reserveTempIntVec ()
   {
   int nPeriodsVal;

   stronglyAssert (not tempVecsReserved_);

   nPeriodsVal       = myWitRun_->mySession ()->myProblem ()->nPeriods ();

   tempVecsReserved_ = true;

   if (tempIntVec_.length () != nPeriodsVal)
      tempIntVec_.allocate (myWitRun_->mySession ()->myProblem (), 0);

   return tempIntVec_;
   }

//------------------------------------------------------------------------------

WitTVec <double> & WitApiMgr::reserveTempDblVec ()
   {
   int nPeriodsVal;

   stronglyAssert (not tempVecsReserved_);

   tempVecsReserved_ = true;

   nPeriodsVal       = myWitRun_->mySession ()->myProblem ()->nPeriods ();

   if (tempDblVec_.length () != nPeriodsVal)
      tempDblVec_.allocate (myWitRun_->mySession ()->myProblem (), 0);

   return tempDblVec_;
   }

//------------------------------------------------------------------------------

void WitApiMgr::releaseTempVecs ()
   {
   stronglyAssert (tempVecsReserved_);

   tempVecsReserved_ = false;
   }

//------------------------------------------------------------------------------

void WitApiMgr::resetInputID ()
   {
   delete myInputID ();

   myInputID_ = new WitInputID (this);
   }

//------------------------------------------------------------------------------
// Implementation of class InputID.
//------------------------------------------------------------------------------

WitInputID::WitInputID (WitApiMgr * theApiMgr):

      myApiMgr_       (theApiMgr),
      myAttName_      (NULL),
      myPart_         (NULL),
      myOperation_    (NULL),
      myDemand_       (NULL),
      myBomEnt_       (NULL),
      mySub_          (NULL),
      myBopEnt_       (NULL),
      argName_        (NULL),
      argIndex_       (-1),
      dataFileName_   (NULL),
      dataFileLineNo_ (0),
      errorPeriod_    (-1)
   {
   }

//------------------------------------------------------------------------------

WitInputID::~WitInputID ()
   {
   }

//------------------------------------------------------------------------------

void WitInputID::setMyAttName (const char * theAttName)
   {
   myAttName_ = theAttName;
   }

//------------------------------------------------------------------------------

void WitInputID::setMyPart (WitPart * thePart)
   {
   myPart_ = thePart;
   }

//------------------------------------------------------------------------------

void WitInputID::setMyOperation (WitOperation * theOpn)
   {
   myOperation_ = theOpn;
   }

//------------------------------------------------------------------------------

void WitInputID::setMyDemand (WitDemand * theDemand)
   {
   myDemand_ = theDemand;
   }

//------------------------------------------------------------------------------

void WitInputID::setMyBomEntry (WitBomEntry * theBomEnt)
   {
   myBomEnt_ = theBomEnt;
   }

//------------------------------------------------------------------------------

void WitInputID::setMySubEntry (WitSubEntry * theSub)
   {
   mySub_ = theSub;
   }

//------------------------------------------------------------------------------

void WitInputID::setMyBopEntry (WitBopEntry * theBopEnt)
   {
   myBopEnt_ = theBopEnt;
   }

//------------------------------------------------------------------------------

void WitInputID::setArgName (const char * theName)
   {
   argName_ = theName;
   }

//------------------------------------------------------------------------------

void WitInputID::setArgIndex (int theIndex)
   {
   argIndex_ = theIndex;
   }

//------------------------------------------------------------------------------

void WitInputID::setDataFileName (const char * theName)
   {
   dataFileName_ = theName;
   }

//------------------------------------------------------------------------------

void WitInputID::setDataFileLineNo (int theValue)
   {
   dataFileLineNo_ = theValue;
   }

//------------------------------------------------------------------------------

void WitInputID::setErrorPeriod (WitPeriod t)
   {
   errorPeriod_ = t;
   }

//------------------------------------------------------------------------------

void WitInputID::idInput ()
   {
   WitMsgFacility & theMsgFac = myApiMgr_->myWitRun ()->myMsgFac ();

   if (myAttName_ != NULL)
      theMsgFac ("inputAttribSmsg", myAttName_);

   if (argName_ != NULL)
      {
      if (argIndex_ >= 0)
         theMsgFac ("inputArgElSmsg", argName_, argIndex_);
      else
         theMsgFac ("inputArgumentSmsg", argName_);
      }

   if (errorPeriod_ >= 0)
      theMsgFac ("inputPeriodSmsg", errorPeriod_);

   if (myPart_ != NULL)
      theMsgFac ("inputPartNameSmsg", myPart_->partName ());

   if (myOperation_ != NULL)
      theMsgFac ("inputOperationNameSmsg", myOperation_->operationName ());

   if (myDemand_ != NULL)
      theMsgFac ("inputDemandNameSmsg", myDemand_->demandName ());

   if      (mySub_ != NULL)
      theMsgFac ("inputConsumedPartNameSmsg", mySub_->myPartName ());
   else if (myBomEnt_ != NULL)
      theMsgFac ("inputConsumedPartNameSmsg", myBomEnt_->myPartName ());

   if (myBopEnt_ != NULL)
      theMsgFac ("inputProducedPartNameSmsg", myBopEnt_->myPartName ());

   if (myBomEnt_ != NULL)
      theMsgFac ("inputBomIndexSmsg", myBomEnt_->localIndex ());

   if (mySub_ != NULL)
      theMsgFac ("inputSubsBomEntryIndexSmsg", mySub_->localIndex ());

   if (myBopEnt_ != NULL)
      theMsgFac ("inputBopIndexSmsg", myBopEnt_->localIndex ());

   if (dataFileName_ != NULL)
      theMsgFac ("inputFileNameSmsg", dataFileName_);

   if (dataFileLineNo_ != 0)
      theMsgFac ("inputFileLineNumberSmsg", dataFileLineNo_);

   if (not WitSaeMgr::standAloneMode ())
      if (myApiMgr_->apiFuncName () != NULL)
         theMsgFac ("apiFunctionSmsg", myApiMgr_->apiFuncName ());
   }

//------------------------------------------------------------------------------

bool WitNonClass::asaBool (WitBoolean theBoolean)
   {
   return
      theBoolean?
         true:
         false;
   }
