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
// Source file: "SglSrcMgr.C"
//
// Contains the implementation of class SglSrcMgr.
//------------------------------------------------------------------------------

#include <SglSrcMgr.h>
#include <SglSrcSite.h>
#include <Selector.h>
#include <SelPt.h>
#include <RtSite.h>
#include <RtCand.h>
#include <RtMgr.h>
#include <MrMgr.h>
#include <HeurAtor.h>
#include <Part.h>
#include <Operation.h>
#include <BopEntry.h>
#include <SubEntry.h>
#include <Demand.h>
#include <GlobalComp.h>
#include <CompMgr.h>
#include <DataRepos.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------

WitSglSrcMgr::WitSglSrcMgr (WitSelector * theSelector):

      WitProbAssoc       (theSelector),
      mySelector_        (theSelector),
      myRtMgr_           (theSelector->myRtMgr ()),
      mySglSrcSiteFor_   (myProblem ()),
      alteredSites_      (myProblem ()),
      cmqMode_           (false),
      curPermCommIdx_    (0),
      curVisitSites_     (myProblem ()),
      incumbVisitSites_  (myProblem ()),
      pureMsMode_        (true),
      initAllocRepos_    (NULL),
      initConfigRepos_   (NULL),
      incumbAllocRepos_  (NULL),
      incumbConfigRepos_ (NULL),

      nCmq_              (0),
      nCmqFixed_         (0),
      nFullAlloc_        (0),
      nNonFullAlloc_     (0),
      nOpps_             (0),
      nDiscovered_       (0),
      nTested_           (0),
      nAccepted_         (0),
      nRejected_         (0)
   {
   WitRtSite * theRtSite;

   verifyRestrictions ();

   myMsgFac () ("sglSrcMsg");

   initAllocRepos_    = new WitDataRepos (myProblem ());
   initConfigRepos_   = new WitDataRepos (myProblem ());
   incumbAllocRepos_  = new WitDataRepos (myProblem ());
   incumbConfigRepos_ = new WitDataRepos (myProblem ());

   forEachEl (theRtSite, myRtMgr_->myRtSites ())
      if (theRtSite->myRtCands ().nElements () > 1)
         if (theRtSite->singleSource ())
            if (mySelector_->myMrMgr ()->multiChoice (theRtSite))
               mySglSrcSiteFor_ (theRtSite) = new WitSglSrcSite (theRtSite);
   }

//------------------------------------------------------------------------------

WitSglSrcMgr::~WitSglSrcMgr ()
   {
   WitRtSite * theRtSite;

   if (devMode ())
      myMsgFac () ("sglSrcCountsMsg",
         nCmq_,
         nCmqFixed_,
         nFullAlloc_,
         nNonFullAlloc_,
         nOpps_,
         nDiscovered_,
         nTested_,
         nAccepted_,
         nRejected_);

   forEachEl (theRtSite, myRtMgr_->myRtSites ())
      delete mySglSrcSiteFor_ (theRtSite);

   delete incumbConfigRepos_;
   delete incumbAllocRepos_;
   delete initConfigRepos_;
   delete initAllocRepos_;
   }

//------------------------------------------------------------------------------

bool WitSglSrcMgr::sglSrcReq (WitProblem * theProblem)
   {
   WitPart *     thePart;
   WitBomEntry * theBomEnt;

   forEachPart (thePart, theProblem)
      if (thePart->singleSource ())
         return true;

   forEachBomEntry (theBomEnt, theProblem)
      if (theBomEnt->singleSource ())
         return true;

   return false;
   }

//------------------------------------------------------------------------------

void WitSglSrcMgr::commitMaxQty (double & netQty)
   {
   bool            fullAlloc;
   WitSglSrcSite * oppSite;

   stronglyAssert (not cmqMode_);

   cmqMode_ = true;

   nCmq_ ++;

   initMsAlloc (netQty, fullAlloc);

   if (not fullAlloc)
      {
      cmqMode_ = false;

      return;
      }

   while (true)
      {
      oppSite = nextOpp ();

      if (oppSite == NULL)
         break;

      nOpps_ ++;

      if (oppSite->nextCandIsSglSrc ())
         selectDiscovery (oppSite);
      else
         tryNextCand (netQty, oppSite);
      }

   preserveIncumb ();

   netQty = 0.0;

   cmqMode_ = false;
   }

//------------------------------------------------------------------------------

void WitSglSrcMgr::recPermCommit ()
   {
   curPermCommIdx_ ++;

   if (selPrintLevelAt (2))
      fprintf (msgFile (),
         "\n"
         "Perm-Commit #%d\n",
         curPermCommIdx_);
   }

//------------------------------------------------------------------------------

bool WitSglSrcMgr::expProhibited (WitPart * thePart, WitPeriod expPer)
   {
   WitRtSite *     theRtSite;
   WitSglSrcSite * theSite;

   theRtSite = myRtMgr_->myRtSiteFor (thePart);

   theSite   = mySglSrcSiteFor_ (theRtSite);

   return expProhibited (theSite, expPer);
   }

//------------------------------------------------------------------------------

bool WitSglSrcMgr::expProhibited (WitOperation * theOpn, WitPeriod execPer)
   {
   WitBomEntry *   theBomEnt;
   WitRtSite *     theRtSite;
   WitSglSrcSite * theSite;

   forEachEl (theBomEnt, theOpn->bom ())
      if (theBomEnt->inEffect (execPer))
         {
         theRtSite = myRtMgr_->myRtSiteFor (theBomEnt);

         theSite   = mySglSrcSiteFor_ (theRtSite);

         if (expProhibited (theSite, execPer))
            return true;
         }

   return false;
   }

//------------------------------------------------------------------------------

void WitSglSrcMgr::recExp (WitBillEntry * theBillEnt)
   {
   WitRtCand *     theRtCand;
   WitSglSrcSite * theSite;

   theRtCand = myRtMgr_->myRtCandFor (theBillEnt);

   theSite   = mySglSrcSiteFor_ (theRtCand->myRtSite ());

   if (theSite == NULL)
      return;

   if (mySelector_->myHeurAtor ()->permCommActive ())
      if (theSite->curPcIdx () == -1)
         curVisitSites_.push (theSite);

   theSite->recordVisit (theRtCand, curPermCommIdx_);
   }

//------------------------------------------------------------------------------

bool WitSglSrcMgr::sglSrcMode (WitRtSite * theRtSite)
   {
   WitSglSrcSite * theSite;

   theSite = mySglSrcSiteFor_ (theRtSite);

   if (theSite != NULL)
      if (theSite->sglSrcMode ())
         return true;

   return false;
   }

//------------------------------------------------------------------------------

void WitSglSrcMgr::saveAllocVal (int & theVar)
   {
   if (cmqMode_)
      initAllocRepos_->saveVal (theVar);
   }

//------------------------------------------------------------------------------

void WitSglSrcMgr::saveAllocVal (double & theVar)
   {
   if (cmqMode_)
      initAllocRepos_->saveVal (theVar);
   }

//------------------------------------------------------------------------------

void WitSglSrcMgr::saveExecVol (WitOperation * theOpn, WitPeriod thePer)
   {
   if (cmqMode_)
      initAllocRepos_->saveExecVol (theOpn, thePer);
   }

//------------------------------------------------------------------------------

void WitSglSrcMgr::saveSubVol (WitSubEntry * theSub, WitPeriod thePer)
   {
   if (cmqMode_)
      initAllocRepos_->saveSubVol (theSub, thePer);
   }

//------------------------------------------------------------------------------

void WitSglSrcMgr::saveConfigVal (bool & theVar)
   {                                       
   if (cmqMode_)
      initConfigRepos_->saveVal (theVar);
   }

//------------------------------------------------------------------------------

void WitSglSrcMgr::saveConfigVal (int & theVar)
   {                                       
   if (cmqMode_)
      initConfigRepos_->saveVal (theVar);
   }

//------------------------------------------------------------------------------

void WitSglSrcMgr::saveConfigObjByVPVal (void * & theVar)
   {
   if (cmqMode_)
      initConfigRepos_->saveVal (theVar);
   }

//------------------------------------------------------------------------------

void WitSglSrcMgr::verifyRestrictions ()
   {
   WitDemand * theDemand;

   stronglyAssert (sglSrcReq (myProblem ()));

   if (mySelector_->penExec ())
      myMsgFac () ("sglSrcAndPenExecSmsg");

   if (myGlobalComp ()->computeCriticalList ())
      myMsgFac () ("sglSrcAndCritListSmsg");

   if (myGlobalComp ()->pgdCritListMode ())
      myMsgFac () ("pgdCritListSglSrcSmsg");

   if (myGlobalComp ()->equitability () > 1)
      myMsgFac () ("sglSrcAndEqAllSmsg");

   forEachDemand (theDemand, myProblem ())
      if (theDemand->buildAheadUB () != 0)
         myMsgFac () ("sglSrcAndBaByDemSmsg");
   }

//------------------------------------------------------------------------------

bool WitSglSrcMgr::expProhibited (WitSglSrcSite * theSite, WitPeriod thePer)
   {
   if (theSite != NULL)
      if (theSite->expProhibited (thePer))
         return true;

   return false;
   }

//------------------------------------------------------------------------------

void WitSglSrcMgr::initMsAlloc (double & netQty, bool & fullAlloc)
   {
   double newNetQty;

   pureMsMode_ = true;

   cmqFixed (netQty, newNetQty);

   fullAlloc =
      (newNetQty <= SEARCH_TOL * mySelector_->myHeurAtor ()->searchInc ());

   if (fullAlloc)
      {
      nFullAlloc_ ++;

      saveCurAsIncumb ();
      }
   else
      {
      nNonFullAlloc_ ++;

      initAllocRepos_ ->clear ();
      initConfigRepos_->clear ();

      netQty = newNetQty;

      if (selPrintLevelAt (2))
         prtNonFull ();
      }
   }

//------------------------------------------------------------------------------

WitSglSrcSite * WitSglSrcMgr::nextOpp ()
   {
   WitObjStItr <WitSglSrcSite> theItr;
   WitSglSrcSite *             theSite;
   WitSglSrcSite *             oppSite;

   oppSite = NULL;

   incumbVisitSites_.attachItr (theItr);

   while (theItr.advance (theSite))
      if (not theSite->sglSrcMode ())
         if (theSite->hasUntried ())
            if (theSite->isPreferredTo (oppSite))
               oppSite = theSite;

   return oppSite;
   }

//------------------------------------------------------------------------------

void WitSglSrcMgr::selectDiscovery (WitSglSrcSite * theSite)
   {
   nDiscovered_ ++;

   if (not theSite->altered ())
      alteredSites_.push (theSite);

   theSite->selectNext ();

   pureMsMode_ = false;

   if (selPrintLevelAt (2))
      theSite->prtSel ("Discovered");
   }

//------------------------------------------------------------------------------

void WitSglSrcMgr::tryNextCand (double netQty, WitSglSrcSite * theSite)
   {
   double newNetQty;

   nTested_ ++;

   if (not theSite->altered ())
      alteredSites_.push (theSite);

   theSite->selectNext ();

   if (selPrintLevelAt (2))
      theSite->prtSel ("Tested");

   theSite->applySel ();

   cmqFixed (netQty, newNetQty);

   if (newNetQty <= SEARCH_TOL * mySelector_->myHeurAtor ()->searchInc ())
      {
      nAccepted_ ++;

      saveCurAsIncumb ();

      pureMsMode_ = false;

      if (selPrintLevelAt (2))
         theSite->prtSel ("Accepted");
      }
   else
      {
      nRejected_ ++;

      initAllocRepos_ ->restore ();
      initConfigRepos_->restore ();

      if (selPrintLevelAt (2))
         theSite->prtSel ("Rejected");

      theSite->resumeMS ();
      }
   }

//------------------------------------------------------------------------------

void WitSglSrcMgr::saveCurAsIncumb ()
   {
   WitObjStItr <WitSglSrcSite> theItr;
   WitSglSrcSite *             theSite;

   initAllocRepos_ ->restoreAndSave (incumbAllocRepos_);
   initConfigRepos_->restoreAndSave (incumbConfigRepos_);

   while (incumbVisitSites_.pop (theSite))
      theSite->resetIncumb ();

   curVisitSites_.attachItr (theItr);

   while (theItr.advance (theSite))
      {
      incumbVisitSites_.push (theSite);

      theSite->saveCurAsIncumb ();
      }
   }

//------------------------------------------------------------------------------

void WitSglSrcMgr::preserveIncumb ()
   {
   WitSglSrcSite * theSite;

   if (pureMsMode_)
      incumbConfigRepos_->restore ();
   else
      incumbConfigRepos_->clear ();

   incumbAllocRepos_->restore ();

   while (alteredSites_.pop (theSite))
      theSite->reset ();
   }

//------------------------------------------------------------------------------

void WitSglSrcMgr::prtNonFull ()
   {
   fprintf (msgFile (),
      "\n"
      "------------------------------------\n"
      "-> Full Allocation Was Not Achieved.\n"
      "-> Multi-Source Allocation Used.    \n"
      "------------------------------------\n"
      "\n");
   }

//------------------------------------------------------------------------------

void WitSglSrcMgr::cmqFixed (double netQty, double & newNetQty)
   {
   WitSglSrcSite *             theSite;
   WitObjStItr <WitSglSrcSite> theItr;

   nCmqFixed_ ++;

   while (curVisitSites_.pop (theSite))
      theSite->initCmqFixed ();

   curPermCommIdx_ = 0;
   newNetQty       = netQty;

   alteredSites_.attachItr (theItr);

   while (theItr.advance (theSite))
      theSite->applySel ();

   mySelector_->commitMaxQty (newNetQty);
   }
