//==============================================================================
// IBM Confidential
//
// OCO Source Materials
//
// Watson Implosion Technology
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//
// The Source code for this program is not published or otherwise divested of
// its trade secrets, irrespective of what has been deposited with the U. S.
// Copyright office.
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "SelStRe.C"
//
// Contains the implementation of the following classes:
//
//    SsrMgr.
//    SsrPt.
//    SsrCand.
//------------------------------------------------------------------------------

#include <SsrMgr.h>
#include <SsrCand.h>
#include <SsrPt.h>
#include <DetAltPt.h>
#include <SplitPt.h>
#include <SglSrcMgr.h>
#include <Selector.h>
#include <HeurAtor.h>
#include <GlobalComp.h>
#include <Material.h>
#include <BopEntry.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class SsrMgr.
//------------------------------------------------------------------------------

WitSsrMgr::WitSsrMgr (WitSelector * theSelector):

      WitSelMgr        (theSelector),
      mySsrPts_        (),
      srWasDonePts_    (myProblem ()),
      srSelSplit_      (false),
      myRecoveryPairs_ (myProblem ())
   {
   WitMaterial * theMat;
   WitPeriod     fromPer;
   WitPeriod     expPer;

   if (devMode ())
      myMsgFac () ("selSRMsg");

   mySsrPts_.allocate1D (myProblem ());

   forEachMaterial (theMat, myProblem ())
      {
      if (not mySelector ()->myHeurAtor ()->selectiveSR (theMat))
         continue;

      mySsrPts_.myPtrTVecAt (theMat).allocate (myProblem ());

      for (fromPer = 1; fromPer < nPeriods (); fromPer ++)
         {
         expPer = theMat->explosionPeriod (fromPer);

         if (theMat->explodeable (expPer))
            mySsrPts_.myPtrAt (theMat, fromPer) =
               new WitSsrPt (theMat, fromPer, this);
         }
      }

   srSelSplit_ =
         myGlobalComp ()->selSplit      ()
      or myGlobalComp ()->nonMrSelSplit () 
      or myGlobalComp ()->srSelSplit    ();
   }

//------------------------------------------------------------------------------

WitSsrMgr::~WitSsrMgr ()
   {
   WitMaterial * theMat;
   WitPeriod     fromPer;

   forEachMaterial (theMat, myProblem ())
      if (mySsrPts_.myPtrTVecAt (theMat).isAllocated ())
         forEachPeriod (fromPer, myProblem ())
            delete mySsrPts_.myPtrAt (theMat, fromPer);
   }

//------------------------------------------------------------------------------

bool WitSsrMgr::instNeededFor (WitHeurAtor * theHeurAtor)
   {
   WitMaterial * theMat;

   forEachMaterial (theMat, theHeurAtor->myProblem ())
      if (theHeurAtor->selectiveSR (theMat))
         return true;

   return false;
   }

//------------------------------------------------------------------------------

bool WitSsrMgr::srAllowedFrom (WitMaterial * theMat, WitPeriod thePer)
   {
   WitSsrPt * theSsrPt;

   if (not mySsrPts_.myPtrTVecAt (theMat).isAllocated ())
      return false;

   theSsrPt = mySsrPts_.myPtrAt (theMat, thePer);

   if (theSsrPt == NULL)
      return false;

   if (theSsrPt->isSplit ())
      return true;

   return theSsrPt->srAllowed ();
   }

//------------------------------------------------------------------------------

void WitSsrMgr::recordSR (WitMaterial * theMat, WitPeriod thePer, double srVol)
   {
   WitSsrPt * theSsrPt;

   theSsrPt = mySsrPts_.myPtrAt (theMat, thePer);

   witAssert (theSsrPt != NULL);

   witAssert (theSsrPt->srAllowed () or theSsrPt->isSplit ());

   if (srSelSplit_)
      theSsrPt->recordFlowSS (srVol);

   if (theSsrPt->srWasDone ())
      return;

   theSsrPt->setSrWasDone (true);

   srWasDonePts_.push (theSsrPt);
   }

//------------------------------------------------------------------------------

void WitSsrMgr::postTempCommit ()
   {
   WitSsrPt * theSsrPt;

   while (srWasDonePts_.pop (theSsrPt))
      theSsrPt->setSrWasDone (false);
   }

//------------------------------------------------------------------------------

void WitSsrMgr::print (WitMaterial * theMat)
   {
   WitPeriod  fromPer;
   WitSsrPt * theSsrPt;

   fprintf (msgFile (),
      "\n"
      "Part %s:\n\n"
      "   Per   S.R. Allowed?\n",
      theMat->partName ().myCstring ());

   forEachPeriod (fromPer, myProblem ())
      {
      theSsrPt = mySsrPts_.myPtrAt (theMat, fromPer);

      if (theSsrPt != NULL)
         fprintf (msgFile (), "   %3d   %s\n",
            fromPer,
            theSsrPt->srAllowed ()?
               "Yes":
               "No");
      }
   }

//------------------------------------------------------------------------------

void WitSsrMgr::storeRecoveryPair (WitMaterial * theMat, WitPeriod fromPer)
   {
   myRecoveryPairs_.push (theMat, fromPer);
   }

//------------------------------------------------------------------------------

void WitSsrMgr::recoverInitState ()
   {
   WitMaterial * theMat;
   WitPeriod     fromPer;

   while (myRecoveryPairs_.pop (theMat, fromPer))
      {
      mySsrPts_.myPtrAt (theMat, fromPer)->recoverInitState ();
      }
   }

//------------------------------------------------------------------------------

bool WitSsrMgr::selSplitNeeded ()
   {
   return srSelSplit_;
   }

//------------------------------------------------------------------------------

void WitSsrMgr::adjReqAndRollForSS (
      WitMaterial * theMat,
      WitPeriod     rollPer,
      double &      theReqVol,
      double &      rollVol,
      bool &        bddSplit)
   {
   WitSsrPt * theSsrPt;
   double     srAllVol;
   double     srNotAllVol;

   witAssert (rollVol >= NET_TOL);

   if (not srSelSplit_)
      return;

   if (rollPer < 0)
      return;
   
   theSsrPt = mySsrPts_.myPtrAt (theMat, rollPer);

   if (theSsrPt == NULL)
      return;

   if (not theSsrPt->isSplit ())
      return;

   theSsrPt->defineSplit (rollVol, srAllVol, srNotAllVol);

   if (srAllVol == 0.0)
      {
      rollVol = 0.0;

      return;
      }

   bddSplit = true;

   if (srAllVol >= rollVol)
      return;

   rollVol    = srAllVol;

   theReqVol += srNotAllVol;
   }

//------------------------------------------------------------------------------

WitAltPt * WitSsrMgr::myAltPtFor (
      WitBopEntry * theBopEnt,
      WitConsEntry *,
      WitPeriod,
      WitPeriod     fillPer)
   {
   return myAltPtFor (theBopEnt->myPart (), fillPer);
   }

//------------------------------------------------------------------------------

WitAltPt * WitSsrMgr::myAltPtFor (WitPart * thePart, WitPeriod fillPer)
   {
   WitMaterial * theMat;
   WitSsrPt *    theSsrPt;

   theMat = thePart->thisMat ();

   if (theMat != NULL)
      if (hasanSsrPtFor (theMat, fillPer))
         {
         theSsrPt = mySsrPts_.myPtrAt (theMat, fillPer);

         if (theSsrPt->srAllowed ())
            return theSsrPt->myDetAltPt ();
         }

   return NULL;
   }

//------------------------------------------------------------------------------

bool WitSsrMgr::validForNetting ()
   {
   return true;
   }

//------------------------------------------------------------------------------

bool WitSsrMgr::validForExecBounds ()
   {
   return true;
   }

//------------------------------------------------------------------------------

bool WitSsrMgr::validForLTBounds ()
   {
   return true;
   }

//------------------------------------------------------------------------------

const char * WitSsrMgr::selCaseName ()
   {
   return "Stock Reallocation";
   }

//------------------------------------------------------------------------------

bool WitSsrMgr::hasanSsrPtFor (WitMaterial * theMat, WitPeriod fromPer)
   {
   return
      mySsrPts_.myPtrTVecAt (theMat).isAllocated ()?
         (mySsrPts_.myPtrAt (theMat, fromPer) != NULL):
         false;
   }

//------------------------------------------------------------------------------
// Implementation of class SsrPt.
//------------------------------------------------------------------------------

WitSsrPt::WitSsrPt (
         WitMaterial * theMat,
         WitPeriod     fromPer,
         WitSsrMgr *   theSsrMgr):

      WitDetSelPt (),

      myMat_      (theMat),
      myFromPer_  (fromPer),
      mySsrMgr_   (theSsrMgr),
      srAllowed_  (true),
      srWasDone_  (false)
   {
   }

//------------------------------------------------------------------------------

WitSsrPt::~WitSsrPt ()
   {
   }

//------------------------------------------------------------------------------

void WitSsrPt::defineSplit (
      double   rollVol,
      double & srAllVol,
      double & srNotAllVol)
   {
   WitSelCandDblStack theSplitPairs (mySsrMgr_->myProblem ());
   double             unbddVol;
   bool               splitFound;
   WitSelCand *       theSelCand;
   double             splitVol;
   bool               srAllowedVal;

   witAssert (rollVol >= NET_TOL);

   witAssert (isSplit ());

   srAllVol    = 0.0;
   srNotAllVol = 0.0;

   mySplitPt ()->defineSplit (rollVol, theSplitPairs, unbddVol);

   theSplitPairs.reverse ();

   splitFound = theSplitPairs.pop (theSelCand, splitVol);

   if (not splitFound)
      {
      srAllVol = unbddVol;

      return;
      }

   if (DEBUG_MODE)
      {
      theSelCand->getSsrData (srAllowedVal);

      witAssert (srAllowedVal);
      }

   srAllVol   = splitVol;

   splitFound = theSplitPairs.pop (theSelCand, splitVol);

   if (not splitFound)
      {
      srNotAllVol = unbddVol;

      return;
      }

   if (DEBUG_MODE)
      {
      theSelCand->getSsrData (srAllowedVal);

      witAssert (not srAllowedVal);
      }

   srNotAllVol = splitVol;

   witAssert (theSplitPairs.isEmpty ());
   }

//------------------------------------------------------------------------------

void WitSsrPt::setSrWasDone (bool theValue)
   {
   witAssert (theValue != srWasDone_);

   srWasDone_ = theValue;
   }

//------------------------------------------------------------------------------

void WitSsrPt::recoverInitState ()
   {
   srAllowed_ = true;

   if (mySelMgr ()->selPrintLevelAt (3))
      printRecovery ();
   }

//------------------------------------------------------------------------------

WitSelMgr * WitSsrPt::mySelMgr ()
   {
   return mySsrMgr_;
   }

//------------------------------------------------------------------------------

void WitSsrPt::prtID ()        
   {
   fprintf (mySsrMgr_->msgFile (),
      "   Part:             %s\n"
      "   From Per:         %d\n",
      myMat_->partName ().myCstring (),
      myFromPer_);
   }

//------------------------------------------------------------------------------

bool WitSsrPt::splittable ()   
   {
   return mySsrMgr_->srSelSplit ();
   }

//------------------------------------------------------------------------------

void WitSsrPt::getSortData (WitPart * & thePart, WitPeriod & thePer)
   {
   thePart = myMat_;
   thePer  = myFromPer_;
   }

//------------------------------------------------------------------------------

WitSelCand * WitSsrPt::newSelCand ()
   {
   return new WitSsrCand (srAllowed_, mySsrMgr_->myProblem ());
   }

//------------------------------------------------------------------------------

bool WitSsrPt::hasResAlt ()
   {
   witAssert (mySsrMgr_->mySelector ()->myHeurAtor ()->tempCommActive ());

   if (srAllowed ())
      if (srWasDone_)
         return true;

   return false;
   }

//------------------------------------------------------------------------------

void WitSsrPt::alterSelection ()
   {
   witAssert (srAllowed_);

   setSrAllowed (false);
   }

//------------------------------------------------------------------------------

void WitSsrPt::storeRecoveryPt ()
   {
   mySsrMgr_->storeRecoveryPair (myMat_, myFromPer_);
   }

//------------------------------------------------------------------------------

void WitSsrPt::tempAlterSel ()
   {
   witAssert (srAllowed_);

   setSrAllowed (false);
   }

//------------------------------------------------------------------------------

void WitSsrPt::cancelTempAlt ()
   {
   witAssert (not srAllowed_);

   setSrAllowed (true);
   }

//------------------------------------------------------------------------------

void WitSsrPt::printAlteration ()
   {
   mySsrMgr_->print (myMat_);
   }

//------------------------------------------------------------------------------

void WitSsrPt::setSrAllowed (bool srAllowedVal)
   {
   if (mySelMgr ()->mySelector ()->sglSrc ())
      mySelMgr ()->mySelector ()->mySglSrcMgr ()->saveConfigVal (srAllowed_);

   srAllowed_ = srAllowedVal;
   }

//------------------------------------------------------------------------------

void WitSsrPt::printRecovery ()
   {
   fprintf (mySsrMgr_->msgFile (),
      "   Selective Stock Reallocation Period Recovered:\n"
      "      Part:     %s\n"
      "      From Per: %d\n\n",
      myMat_->partName ().myCstring (),
      myFromPer_);
   }

//------------------------------------------------------------------------------
// Implementation of class SsrCand.
//------------------------------------------------------------------------------

WitSsrCand::WitSsrCand (bool srAllowedVal, WitProblem * theProblem):

      WitSelCand (theProblem),
      srAllowed_ (srAllowedVal)
   {
   }

//------------------------------------------------------------------------------

WitSsrCand::~WitSsrCand ()
   {
   }

//------------------------------------------------------------------------------

void WitSsrCand::prtID ()
   {
   fprintf (msgFile (), "      SR Allowed?    %s\n", srAllowed_? "Yes": "No");
   }

//------------------------------------------------------------------------------

void WitSsrCand::getSsrData (bool & srAllowedVal)
   {
   srAllowedVal = srAllowed_;
   }
