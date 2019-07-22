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
// Source file: "MultiRoute.C"
//
// Contains the implementation of the following classes:
//
//    MrMgr
//    MrCoord
//    MrSelMgr
//    PmrSelMgr
//    CmrSelMgr
//    MrSite
//    MrPt
//    MrCand
//------------------------------------------------------------------------------

#include <MrMgr.h>
#include <MrCoord.h>
#include <MrSelMgr.h>
#include <MrCand.h>
#include <MrPt.h>
#include <Selector.h>
#include <MrSite.h>
#include <DetAltPt.h>
#include <RtMgr.h>
#include <RtSite.h>
#include <RtCand.h>
#include <SplitPt.h>
#include <SglSrcMgr.h>
#include <GlobalComp.h>
#include <Part.h>
#include <Operation.h>
#include <SubEntry.h>
#include <BopEntry.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class MrMgr.
//------------------------------------------------------------------------------

WitMrMgr::WitMrMgr (WitSelector * theSelector):

      WitProbAssoc (theSelector),
      myMrCoord_   (NULL),
      myRtMgr_     (theSelector->myRtMgr ())
   {
   myMsgFac () ("multiRouteMsg");

   myMrCoord_ = new WitMrCoord (theSelector);
   }

//------------------------------------------------------------------------------

WitMrMgr::~WitMrMgr ()
   {
   delete myMrCoord_;
   }

//------------------------------------------------------------------------------

bool WitMrMgr::instNeededFor (WitProblem * theProblem)
   {
   return theProblem->myGlobalComp ()->multiRoute ();
   }

//------------------------------------------------------------------------------

WitBopEntry * WitMrMgr::selBopEnt (WitPart * thePart, WitPeriod expPer)
   {
   WitRtSite * theRtSite;
   WitRtCand * theRtCand;

   theRtSite = myRtMgr_->myRtSiteFor (thePart);

   theRtCand = myMrCoord_->selRtCand (theRtSite, expPer);

   return theRtCand->myBopEnt ();
   }

//------------------------------------------------------------------------------

WitConsEntry * WitMrMgr::selConsEnt (WitBomEntry * theBomEnt, WitPeriod execPer)
   {
   WitRtSite * theRtSite;
   WitRtCand * theRtCand;

   theRtSite = myRtMgr_->myRtSiteFor (theBomEnt);

   theRtCand = myMrCoord_->selRtCand (theRtSite, execPer);

   return theRtCand->myConsEnt ();
   }

//------------------------------------------------------------------------------

WitRtCand * WitMrMgr::selRtCand (WitRtSite * theRtSite, WitPeriod thePer)
   {
   return myMrCoord_->selRtCand (theRtSite, thePer);
   }

//------------------------------------------------------------------------------

bool WitMrMgr::activeForPE (WitRtCand * theRtCand, WitPeriod thePer)
   {
   WitMrSite * theMrSite;

   if (not theRtCand->isEligible (thePer))
      return false;

   if (not theRtCand->usedByMr ())
      return false;

   if (not myMrCoord_->prevSel (theRtCand, thePer))
      return true;

   theMrSite = myMrCoord_->myMrSiteFor (theRtCand->myRtSite ());

   if (theMrSite->myMrPt ()[thePer]->selRtCand () == NULL)
      if (theRtCand == theRtCand->myRtSite ()->mrDefaultRtCand (thePer))
         return true;

   return false;
   }

//------------------------------------------------------------------------------

bool WitMrMgr::multiChoice (WitRtSite * theRtSite)
   {
   return myMrCoord_->myMrSiteFor (theRtSite)->multiChoice ();
   }

//------------------------------------------------------------------------------

bool WitMrMgr::valid (WitRtCand * theRtCand, WitPeriod thePer)
   {
   if (theRtCand->isEligible (thePer))
      if (not myMrCoord_->prevSel (theRtCand, thePer))
         return true;

   return false;
   }

//------------------------------------------------------------------------------

void WitMrMgr::select (WitRtCand * theRtCand, WitPeriod thePer)
   {
   myMrCoord_->
      myMrSiteFor (theRtCand->myRtSite ())->
         myMrPt ()[thePer]->
            select (theRtCand);
   }

//------------------------------------------------------------------------------

WitSelPt * WitMrMgr::mySelPtFor (WitRtSite * theRtSite, WitPeriod thePer)
   {
   return myMrCoord_->myMrSiteFor (theRtSite)->myMrPt ()[thePer];
   }

//------------------------------------------------------------------------------

void WitMrMgr::print ()
   {
   WitRtSite * theRtSite;

   fprintf (msgFile (),
      "\n"
      "\n"
      "Multi-Route Selection Points with Alternative(s):\n\n");

   forEachEl (theRtSite, myMrCoord_->mySelector ()->myRtMgr ()->myRtSites ())
      myMrCoord_->myMrSiteFor (theRtSite)->printHasAlt ();

   fprintf (msgFile (), "\n");
   }

//------------------------------------------------------------------------------

void WitMrMgr::storeRecoveryPair (WitRtSite * theRtSite, WitPeriod thePer)
   {
   myMrCoord_->storeRecoveryPair (theRtSite, thePer);
   }

//------------------------------------------------------------------------------

void WitMrMgr::recoverInitState ()
   {
   myMrCoord_->recoverInitState ();
   }

//------------------------------------------------------------------------------

bool WitMrMgr::selIsSplit (WitPart * thePart, WitPeriod expPer)
   {
   WitRtSite * theRtSite;

   theRtSite = myRtMgr_->myRtSiteFor (thePart);

   return myMrCoord_->selIsSplit (theRtSite, expPer);
   }

//------------------------------------------------------------------------------

bool WitMrMgr::selIsSplit (WitBomEntry * theBomEnt, WitPeriod execPer)
   {
   WitRtSite * theRtSite;

   theRtSite = myRtMgr_->myRtSiteFor (theBomEnt);

   return myMrCoord_->selIsSplit (theRtSite, execPer);
   }

//------------------------------------------------------------------------------

void WitMrMgr::defineSplit (
      WitPart *           thePart,
      WitPeriod           expPer,
      double              expVol,
      WitRtCandDblStack & theSplitPairs,
      double &            unbddVol)
   {
   WitRtSite * theRtSite;

   theRtSite = myRtMgr_->myRtSiteFor (thePart);

   myMrCoord_->defineSplit (theRtSite, expPer, expVol, theSplitPairs, unbddVol);
   }

//------------------------------------------------------------------------------

void WitMrMgr::defineSplit (
      WitBomEntry *       theBomEnt,
      WitPeriod           execPer,
      double              expVol,
      WitRtCandDblStack & theSplitPairs,
      double &            unbddVol)
   {
   WitRtSite * theRtSite;

   theRtSite = myRtMgr_->myRtSiteFor (theBomEnt);

   myMrCoord_->
      defineSplit (theRtSite, execPer, expVol, theSplitPairs, unbddVol);
   }

//------------------------------------------------------------------------------

void WitMrMgr::recordFlowSS (
      WitBillEntry * theBillEnt,
      WitPeriod      thePer,
      double         incFlowVol)
   {
   WitRtSite * theRtSite;
   WitMrSite * theMrSite;

   theRtSite = myRtMgr_->myRtCandFor (theBillEnt)->myRtSite ();

   theMrSite = myMrCoord_->myMrSiteFor (theRtSite);

   if (theMrSite->multiChoice ())
      theMrSite->myMrPt ()[thePer]->recordFlowSS (incFlowVol);
   }

//------------------------------------------------------------------------------

WitSelMgr * WitMrMgr::mySelMgrForProd ()
   {
   return myMrCoord_->myMrSelMgrForProd ();
   }

//------------------------------------------------------------------------------

WitSelMgr * WitMrMgr::mySelMgrForCons ()
   {
   return myMrCoord_->myMrSelMgrForCons ();
   }

//------------------------------------------------------------------------------
// Implementation of class MrCoord.
//------------------------------------------------------------------------------

WitMrCoord::WitMrCoord (WitSelector * theSelector):

      WitProbAssoc       (theSelector),
      myMrSelMgrForProd_ (NULL),
      myMrSelMgrForCons_ (NULL),
      myMrSiteFor_       (myProblem ()),
      mySelector_        (theSelector),
      prevSel_           (),
      savedRtCand_       (NULL),
      myRtSiteRecPairs_  (myProblem ()),
      myRtCandRecPairs_  (myProblem ())
   {
   WitRtSite * theRtSite;

   myMrSelMgrForProd_ = new WitPmrSelMgr (this);
   myMrSelMgrForCons_ = new WitCmrSelMgr (this);

   prevSel_.allocate1D (myProblem ());

   forEachEl (theRtSite, mySelector_->myRtMgr ()->myRtSites ())
      myMrSiteFor_ (theRtSite) = new WitMrSite (theRtSite, this);
   }

//------------------------------------------------------------------------------

WitMrCoord::~WitMrCoord ()
   {
   WitRtSite * theRtSite;

   forEachEl (theRtSite, mySelector_->myRtMgr ()->myRtSites ())
      delete myMrSiteFor_ (theRtSite);

   delete myMrSelMgrForCons_;
   delete myMrSelMgrForProd_;
   }

//------------------------------------------------------------------------------

WitRtCand * WitMrCoord::selRtCand (WitRtSite * theRtSite, WitPeriod thePer)
   {
   return myMrSiteFor_ (theRtSite)->selRtCand (thePer);
   }

//------------------------------------------------------------------------------

WitAltPt * WitMrCoord::myAltPtFor (WitRtCand * theRtCand, WitPeriod thePer)
   {
   WitMrSite * theMrSite;
   WitMrPt *   theMrPt;

   theMrSite = myMrSiteFor_ (theRtCand->myRtSite ());

   theMrPt   = theMrSite->myMrPtWithAlt (thePer);

   return
      (theMrPt != NULL)?
         theMrPt->myDetAltPt ():
         NULL;
   }

//------------------------------------------------------------------------------

bool WitMrCoord::selIsSplit (WitRtSite * theRtSite, WitPeriod thePer)
   {
   WitMrSite * theMrSite;

   if (not mySelector_->selSplit ())
      return false;

   theMrSite = myMrSiteFor_ (theRtSite);

   if (not theMrSite->multiChoice ())
      return false;

   return theMrSite->myMrPt ()[thePer]->isSplit ();
   }

//------------------------------------------------------------------------------

void WitMrCoord::defineSplit (
      WitRtSite *         theRtSite, 
      WitPeriod           thePer,
      double              expVol,
      WitRtCandDblStack & theSplitPairs,
      double &            unbddVol)
   {
   WitSelCandDblStack theAbsSplitPairs (myProblem ());
   WitSelCand *       theSelCand;
   double             splitVol;
   WitRtCand *        theRtCand;

   witAssert (selIsSplit (theRtSite, thePer));

   myMrSiteFor (theRtSite)->
      myMrPt ()[thePer]->
         mySplitPt ()->
            defineSplit (expVol, theAbsSplitPairs, unbddVol);

   theSplitPairs.clear ();

   while (theAbsSplitPairs.pop (theSelCand, splitVol))
      {
      theSelCand->getMrData (theRtCand);

      theSplitPairs.push (theRtCand, splitVol);
      }
   }

//------------------------------------------------------------------------------

void WitMrCoord::setPrevSel (
      WitRtCand * theRtCand, 
      WitPeriod   thePer, 
      bool        theVal)
   {
   if (not prevSel_ (theRtCand).isAllocated ())
      prevSel_ (theRtCand).allocate (myProblem (), false);

   if (mySelector_->sglSrc ())
      mySelector_->mySglSrcMgr ()->saveConfigVal (prevSel_ (theRtCand)[thePer]);

   prevSel_ (theRtCand)[thePer] = theVal;

   if (myGlobalComp ()->selectionRecovery ())
      myRtCandRecPairs_.push (theRtCand, thePer);
   }


//------------------------------------------------------------------------------

void WitMrCoord::saveRtCand (WitRtCand * theRtCand)
   {
   witAssert (savedRtCand_ == NULL);
   witAssert (theRtCand    != NULL);

   savedRtCand_ = theRtCand;
   }

//------------------------------------------------------------------------------

void WitMrCoord::retrieveRtCand (WitRtCand * & theRtCand)
   {
   witAssert (savedRtCand_ != NULL);

   theRtCand    = savedRtCand_;
   savedRtCand_ = NULL;
   }

//------------------------------------------------------------------------------

void WitMrCoord::storeRecoveryPair (WitRtSite * theRtSite, WitPeriod thePer)
   {
   myRtSiteRecPairs_.push (theRtSite, thePer);
   }

//------------------------------------------------------------------------------

void WitMrCoord::recoverInitState ()
   {
   WitRtCand * theRtCand;
   WitRtSite * theRtSite;
   WitPeriod   thePer;

   while (myRtCandRecPairs_.pop (theRtCand, thePer))
      {
      prevSel_ (theRtCand)[thePer] = false;

      if (selPrintLevelAt (3))
         prtRecover (theRtCand, thePer);
      }

   while (myRtSiteRecPairs_.pop (theRtSite, thePer))
      {
      myMrSiteFor_ (theRtSite)->myMrPt ()[thePer]->recoverInitState ();
      }
   }

//------------------------------------------------------------------------------

void WitMrCoord::prtRecover (WitRtCand * theRtCand, WitPeriod thePer)
   {
   fprintf (msgFile (), "   Multi-Route Candidate Recovered:\n");

   theRtCand->myRtSite ()->printID          (7, 22);
   theRtCand->myRtSite ()->printPer (thePer, 7, 22);
   theRtCand             ->printID          (7, 22);

   fprintf (msgFile (), "\n");
   }

//------------------------------------------------------------------------------
// Implementation of class MrSelMgr.
//------------------------------------------------------------------------------

WitMrSelMgr::~WitMrSelMgr ()
   {
   }

//------------------------------------------------------------------------------

WitMrSelMgr::WitMrSelMgr (WitMrCoord * theMrCoord):

      WitSelMgr  (theMrCoord->mySelector ()),
      myMrCoord_ (theMrCoord)
   {
   }

//------------------------------------------------------------------------------

bool WitMrSelMgr::selSplitNeeded ()
   {
   return (myGlobalComp ()->selSplit ());
   }

//------------------------------------------------------------------------------
// Implementation of class PmrSelMgr.
//------------------------------------------------------------------------------

WitPmrSelMgr::WitPmrSelMgr (WitMrCoord * theMrCoord):

      WitMrSelMgr (theMrCoord)
   {
   }

//------------------------------------------------------------------------------

WitPmrSelMgr::~WitPmrSelMgr ()
   {
   }

//------------------------------------------------------------------------------

WitAltPt * WitPmrSelMgr::myAltPtFor (
      WitBopEntry * theBopEnt,
      WitConsEntry *,
      WitPeriod     execPer,
      WitPeriod)
   {
   WitRtCand * theRtCand;
   WitPeriod   expPer;

   theRtCand = mySelector ()->myRtMgr ()->myRtCandFor (theBopEnt);

   expPer    = theBopEnt->impactPeriod ()[execPer];

   return myMrCoord ()->myAltPtFor (theRtCand, expPer);
   }

//------------------------------------------------------------------------------

bool WitPmrSelMgr::validForNetting ()
   {
   return true;
   }

//------------------------------------------------------------------------------

bool WitPmrSelMgr::validForExecBounds ()
   {
   return true;
   }

//------------------------------------------------------------------------------

bool WitPmrSelMgr::validForLTBounds ()
   {
   return false;
   }

//------------------------------------------------------------------------------

const char * WitPmrSelMgr::selCaseName ()
   {
   return "Production Multi-Route";
   }

//------------------------------------------------------------------------------
// Implementation of class CmrSelMgr.
//------------------------------------------------------------------------------

WitCmrSelMgr::WitCmrSelMgr (WitMrCoord * theMrCoord):

      WitMrSelMgr (theMrCoord)
   {
   }

//------------------------------------------------------------------------------

WitCmrSelMgr::~WitCmrSelMgr ()
   {
   }

//------------------------------------------------------------------------------

WitAltPt * WitCmrSelMgr::myAltPtFor (
      WitBopEntry *,
      WitConsEntry * theConsEnt,
      WitPeriod      execPer,
      WitPeriod)
   {
   WitRtCand * theRtCand;

   theRtCand = mySelector ()->myRtMgr ()->myRtCandFor (theConsEnt);

   return myMrCoord ()->myAltPtFor (theRtCand, execPer);
   }

//------------------------------------------------------------------------------

bool WitCmrSelMgr::validForNetting ()
   {
   return false;
   }

//------------------------------------------------------------------------------

bool WitCmrSelMgr::validForExecBounds ()
   {
   return false;
   }

//------------------------------------------------------------------------------

bool WitCmrSelMgr::validForLTBounds ()
   {
   return false;
   }

//------------------------------------------------------------------------------

const char * WitCmrSelMgr::selCaseName ()
   {
   return "Consumption Multi-Route";
   }

//------------------------------------------------------------------------------
// Implementation of class MrSite.
//------------------------------------------------------------------------------

WitMrSite::WitMrSite (WitRtSite * theRtSite, WitMrCoord * theMrCoord):

      WitProbAssoc (theMrCoord),
      myRtSite_    (theRtSite),
      myMrCoord_   (theMrCoord),
      myMrPt_      (),
      multiChoice_ (false)
   {
   multiChoice_ = compMultiChoice ();

   buildMrPts ();
   }

//------------------------------------------------------------------------------

WitMrSite::~WitMrSite ()
   {
   deleteContents (myMrPt_);
   }

//------------------------------------------------------------------------------

WitRtCand * WitMrSite::selRtCand (WitPeriod thePer)
   {
   WitRtCand * theRtCand;

   if (multiChoice_)
      {
      theRtCand = myMrPt_[thePer]->selRtCand ();

      if (theRtCand != NULL)
         return theRtCand;
      }

   return myRtSite_->mrDefaultRtCand (thePer);
   }

//------------------------------------------------------------------------------

void WitMrSite::printHasAlt ()
   {
   WitPeriod thePer;
   WitMrPt * theMrPt;

   if (not multiChoice_)
      return;

   forEachPeriod (thePer, myProblem ())
      {
      theMrPt = myMrPt_[thePer];

      if (theMrPt != NULL)
         if (theMrPt->selRtCand () != NULL)
            {
            myRtSite_->printID          (4, 14);
            myRtSite_->printPer (thePer, 4, 14);

            fprintf (msgFile (), "\n");
            }
      }
   }

//------------------------------------------------------------------------------

WitSelMgr * WitMrSite::mySelMgr ()
   {
   return
      myRtSite_->isForProd ()?
         myMrCoord_->myMrSelMgrForProd ():
         myMrCoord_->myMrSelMgrForCons ();
   }

//------------------------------------------------------------------------------

bool WitMrSite::compMultiChoice ()
   {
   WitRtCand * theRtCand;
   bool        foundCand;

   foundCand = false;

   forEachEl (theRtCand, myRtSite_->myRtCands ())
      if (theRtCand->usedByMr ())
         {
         if (foundCand)
            return true;
         else
            foundCand = true;
         }

   return false;
   }

//------------------------------------------------------------------------------

void WitMrSite::buildMrPts ()
   {
   WitPeriod thePer;

   if (not multiChoice_)
      return;

   myMrPt_.allocate (myProblem ());

   forEachPeriod (thePer, myProblem ())
      if (myRtSite_->mrDefaultRtCand (thePer) != NULL)
         myMrPt_[thePer] = new WitMrPt (this, thePer);
   }

//------------------------------------------------------------------------------

WitMrPt * WitMrSite::multiChoiceMrPtWithAlt (WitPeriod thePer)
   {
   WitMrPt * theMrPt;

   witAssert (multiChoice_);

   if (myRtSite_->propRtg ()[thePer])
      return NULL;

   theMrPt = myMrPt_[thePer];

   witAssert (theMrPt != NULL);

   return
      (theMrPt->selRtCand () != NULL)?
         theMrPt:
         NULL;
   }

//------------------------------------------------------------------------------
// Implementation of class template MrPt.
//------------------------------------------------------------------------------

inline WitSelector * WitMrPt::mySelector ()
   {
   return myMrSite_->myMrCoord ()->mySelector ();
   }

//------------------------------------------------------------------------------

WitMrPt::WitMrPt (WitMrSite * theMrSite, WitPeriod thePer):

      WitDetSelPt (),

      myMrSite_   (theMrSite),
      myPer_      (thePer),
      selRtCand_  (NULL),
      hasResAlt_  (false)
   {
   selRtCand_ = myMrSite_->myRtSite ()->mrDefaultRtCand (myPer_);

   refreshHasResAlt ();
   }

//------------------------------------------------------------------------------

WitMrPt::~WitMrPt ()
   {
   }

//------------------------------------------------------------------------------

void WitMrPt::select (WitRtCand * theRtCand)
   {
   if (mySelector ()->sglSrc ())
      mySelector ()->mySglSrcMgr ()->saveConfigVal (selRtCand_);

   selRtCand_ = theRtCand;
   }

//------------------------------------------------------------------------------

void WitMrPt::recoverInitState ()
   {
   selRtCand_ = myMrSite_->myRtSite ()->mrDefaultRtCand (myPer_);

   refreshHasResAlt ();

   if (mySelMgr ()->selPrintLevelAt (3))
      printRecovery ();
   }

//------------------------------------------------------------------------------

WitSelMgr * WitMrPt::mySelMgr ()
   {
   return myMrSite_->mySelMgr ();
   }

//------------------------------------------------------------------------------

void WitMrPt::prtID ()
   {
   myMrSite_->myRtSite ()->printID          (4, 22);
   myMrSite_->myRtSite ()->printPer (myPer_, 4, 22);
   }

//------------------------------------------------------------------------------

bool WitMrPt::splittable ()
   {
   if (mySelector ()->sglSrc ())
      if (sglSrcMode ())
         return false;

   return (myMrSite_->myGlobalComp ()->selSplit ());
   }

//------------------------------------------------------------------------------

void WitMrPt::getSortData (WitPart * & thePart, WitPeriod & thePer)
   {
   thePart = myMrSite_->myRtSite ()->mrSortPart ();

   thePer  = myPer_;
   }

//------------------------------------------------------------------------------

WitSelCand * WitMrPt::newSelCand ()
   {
   return new WitMrCand (selRtCand_);
   }

//------------------------------------------------------------------------------

bool WitMrPt::sglSrcMode ()
   {
   witAssert (mySelector ()->sglSrc ());

   return mySelector ()->mySglSrcMgr ()->sglSrcMode (myMrSite_->myRtSite ());
   }

//------------------------------------------------------------------------------

bool WitMrPt::hasResAlt ()
   {
   witAssert (selRtCand_ != NULL);

   return hasResAlt_;
   }

//------------------------------------------------------------------------------

void WitMrPt::alterSelection ()
   {
   witAssert (selRtCand_ != NULL);

   if (mySelector ()->sglSrc ())
      mySelector ()->mySglSrcMgr ()->saveConfigVal (hasResAlt_);

   myMrSite_->myMrCoord ()->setPrevSel (selRtCand_, myPer_, true);

   select (findNextRtCand ());

   refreshHasResAlt ();
   }

//------------------------------------------------------------------------------

void WitMrPt::storeRecoveryPt ()
   {
   myMrSite_->
      myMrCoord ()->
         storeRecoveryPair (myMrSite_->myRtSite (), myPer_);
   }

//------------------------------------------------------------------------------

void WitMrPt::tempAlterSel ()
   {
   myMrSite_->myMrCoord ()->saveRtCand (selRtCand_);

   alterSelection ();
   }

//------------------------------------------------------------------------------

void WitMrPt::cancelTempAlt ()
   {
   WitRtCand * theRtCand;

   myMrSite_->myMrCoord ()->retrieveRtCand (theRtCand);

   witAssert (myMrSite_->myMrCoord ()->prevSel (theRtCand, myPer_));

   myMrSite_->myMrCoord ()->setPrevSel (theRtCand, myPer_, false);

   select (theRtCand);

   refreshHasResAlt ();
   }

//------------------------------------------------------------------------------

void WitMrPt::printAlteration ()
   {
   fprintf (msgFile (),
      "\n"
      "Multi-Route Alteration:\n");

   myMrSite_->myRtSite ()->printID          (4, 22);
   myMrSite_->myRtSite ()->printPer (myPer_, 4, 22);

   fprintf (msgFile (), "   Selection:\n");

   if (selRtCand_ != NULL)
      selRtCand_->printID (7, 22);
   else
      fprintf (msgFile (), "      None\n");
   }

//------------------------------------------------------------------------------

void WitMrPt::refreshHasResAlt ()
   {
   hasResAlt_ = (findNextRtCand () != NULL);
   }

//------------------------------------------------------------------------------

WitRtCand * WitMrPt::findNextRtCand ()
   {
   WitRtCand * theRtCand;

   forEachEl (theRtCand, myMrSite_->myRtSite ()->myRtCands ())
      if (theRtCand->usedByMr ())
         if (theRtCand->isEligible (myPer_))
            if (not myMrSite_->myMrCoord ()->prevSel (theRtCand, myPer_))
               if (theRtCand != selRtCand_)
                  return theRtCand;

   return NULL;
   }

//------------------------------------------------------------------------------

void WitMrPt::printRecovery ()
   {
   fprintf (msgFile (),
      "   Initial Multi-Route Selection Recovered:\n");

   myMrSite_->myRtSite ()->printID          (7, 22);
   myMrSite_->myRtSite ()->printPer (myPer_, 7, 22);
   selRtCand_            ->printID          (7, 22);

   fprintf (msgFile (), "\n");
   }

//------------------------------------------------------------------------------

FILE * WitMrPt::msgFile ()
   {
   return myMrSite_->msgFile ();
   }

//------------------------------------------------------------------------------
// Implementation of class MrCand.
//------------------------------------------------------------------------------

WitMrCand::WitMrCand (WitRtCand * theRtCand):

      WitSelCand (theRtCand->myProblem ()),
      myRtCand_  (theRtCand)
   {
   }

//------------------------------------------------------------------------------

WitMrCand::~WitMrCand ()
   {
   }

//------------------------------------------------------------------------------

void WitMrCand::prtID ()
   {
   myRtCand_->printID (7, 22);
   }

//------------------------------------------------------------------------------

void WitMrCand::getMrData (WitRtCand * & theRtCand)
   {
   theRtCand = myRtCand_;
   }
