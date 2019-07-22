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
// Source file: "HeurAll.C"
//
// Contains the implementation of the following classes:
//
//    HeurAllMgr
//    ReqSched
//    HeurCritList
//    PclBldr
//    PclEl
//------------------------------------------------------------------------------

#include <HeurAllMgr.h>
#include <HeurAtor.h>
#include <EqAll.h>
#include <PclBldr.h>
#include <PclEl.h>
#include <HeurModifier.h>
#include <wit/src/Variant.h>
#include <Pre.h>
#include <Post.h>
#include <Pegger.h>
#include <ReqSched.h>
#include <HeurCrit.h>
#include <PipMgr.h>
#include <DataRepos.h>
#include <MsgFrag.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class HeurAllMgr
//------------------------------------------------------------------------------

WitHeurAllMgr::WitHeurAllMgr (WitProblem * theProblem):

      WitProbAssoc  (theProblem),

      myHeurAtor_   (NULL),
      myPegger_     (NULL),
      updatingSoln_ (false)
   {
   }

//------------------------------------------------------------------------------

WitHeurAllMgr::~WitHeurAllMgr ()
   {
   reset ();

   delete myPegger_;
   }

//------------------------------------------------------------------------------

void WitHeurAllMgr::setUpPegging ()
   {
   WitDemand * theDemand;

   stronglyAssert (myGlobalComp ()->perfPegging ());

   stronglyAssert (myPegger_ == NULL);

   forEachDemand (theDemand, myProblem ())
      theDemand->setUpPegging ();

   myPegger_ = new WitPegger (myProblem ());
   }

//------------------------------------------------------------------------------

void WitHeurAllMgr::clearPegging ()
   {
   witAssert (myPegger_ != NULL);

   myPegger_->clear ();
   }

//------------------------------------------------------------------------------

void WitHeurAllMgr::shutDownPegging ()
   {
   WitDemand * theDemand;

   stronglyAssert (not myGlobalComp ()->perfPegging ());

   stronglyAssert (myPegger_ != NULL);

   delete myPegger_;

   myPegger_ = NULL;

   forEachDemand (theDemand, myProblem ())
      theDemand->shutDownPegging ();
   }

//------------------------------------------------------------------------------

void WitHeurAllMgr::start ()
   {
   start (false);
   }

//------------------------------------------------------------------------------

void WitHeurAllMgr::startForOpt ()
   {
   start (true);
   }

//------------------------------------------------------------------------------

double WitHeurAllMgr::incHeurAlloc (
      WitDemand * theDemand, 
      WitPeriod   thePer, 
      double      desIncVol,
      bool        asapMultiExec)
   {
   double incVol;

   updatingSoln_ = true;

   incVol        =
      myHeurAtor ()->incHeurAlloc (theDemand, thePer, desIncVol, asapMultiExec);

   updatingSoln_ = false;

   return incVol;
   }

//------------------------------------------------------------------------------

void WitHeurAllMgr::reset ()
   {
   if (not active ())
      return;

   delete myHeurAtor_;

   myHeurAtor_ = NULL;

   myProblem ()->myEqAllocator ()->heurAllocIsReset ();
   }

//------------------------------------------------------------------------------

void WitHeurAllMgr::finish ()
   {
   if (myGlobalComp ()->respectStockSLBs ())
      {
      updatingSoln_ = true;

      myHeurAtor ()->enforceStockSLBs ();

      updatingSoln_ = false;
      }

   myPostprocessor ()->postprocess ();
   }

//------------------------------------------------------------------------------

void WitHeurAllMgr::printMeDir (bool asapMultiExec)
   {
   WitMsgFragID theMeDirFragID;

   theMeDirFragID = asapMultiExec? "asapFrag": "nstnFrag";

   if (myMsgFac ().mayIssue ("incHeurMeDirMsg"))
      myMsgFac () ("incHeurMeDirMsg", myMsgFac ().myFrag (theMeDirFragID));
   }

//------------------------------------------------------------------------------

void WitHeurAllMgr::printPegging ()
   {
   witAssert (myGlobalComp ()->perfPegging ());

   myPegger_->print ();
   }

//------------------------------------------------------------------------------

void WitHeurAllMgr::printAvailSched ()
   {
   myHeurAtor ()->printAvailSched ();
   }

//------------------------------------------------------------------------------

bool WitHeurAllMgr::modifiable ()
   {
   if (active ())
      if (myGlobalComp ()->modHeurAlloc ())
         return true;
         
   return false;
   }

//------------------------------------------------------------------------------

WitHeurModifier * WitHeurAllMgr::myHeurModifier ()
   {
   return myHeurAtor ()->myHeurModifier ();
   }

//------------------------------------------------------------------------------

void WitHeurAllMgr::start (bool forOpt)
   {
   myProblem ()->resetSoln ();

   if (forOpt)
      myProblem ()->heurOptVariant ()->becomesCurrent ();
   else
      myProblem ()->heurVariant    ()->becomesCurrent ();

   myPreprocessor ()->preprocess ();

   updatingSoln_ = true;

   myHeurAtor_   = new WitHeurAtor (myProblem (), myPegger_, forOpt);

   updatingSoln_ = false;
   }

//------------------------------------------------------------------------------
// Implementation of class ReqSched.
//------------------------------------------------------------------------------

WitReqSched::WitReqSched (WitProblem * theProblem):

      WitProbAssoc (theProblem),

      myDataRepos_ (NULL),
      reqVol_      (myProblem (), 0.0),
      firstPer_    (myProblem (), nPeriods ()),
      lastPer_     (myProblem (), -1)
   {
   myDataRepos_ = new WitDataRepos (myProblem ());
   }

//------------------------------------------------------------------------------

WitReqSched::~WitReqSched ()
   {
   delete myDataRepos_;
   }

//------------------------------------------------------------------------------

void WitReqSched::addTo (WitPart * thePart, WitPeriod thePer, double qty)
   {
   witAssert (qty > 0.0);

   myDataRepos_->saveVal (reqVol_ (thePart)[thePer]);

   reqVol_ (thePart)[thePer] += qty;

   if (thePer < firstPer_ (thePart))
      {
      myDataRepos_->saveVal (firstPer_ (thePart));

      firstPer_ (thePart) = thePer;
      }

   if (thePer > lastPer_ (thePart))
      {
      myDataRepos_->saveVal (lastPer_ (thePart));

      lastPer_ (thePart) = thePer;
      }
   }

//------------------------------------------------------------------------------

void WitReqSched::clear ()
   {
   myDataRepos_->restore ();
   }

//------------------------------------------------------------------------------
// Implementation of class HeurCritList.
//------------------------------------------------------------------------------

WitHeurCritList::WitHeurCritList (WitProblem * theProblem):

      WitProbAssoc   (theProblem),

      culpritPart_   (NULL),
      culpritPeriod_ (0)
   {
   listed_.allocate (myProblem (), false);
   }

//------------------------------------------------------------------------------

WitHeurCritList::~WitHeurCritList ()
   {
   }

//------------------------------------------------------------------------------

void WitHeurCritList::initSoln ()
   {
   myGlobalComp ()->clearRevCritListForHeur ();
   }

//------------------------------------------------------------------------------

void WitHeurCritList::recFundShortage (WitPart * thePart, WitPeriod thePer)
   {
   witAssert (thePart != NULL);

   culpritPart_   = thePart;
   culpritPeriod_ = thePer;
   }

//------------------------------------------------------------------------------

void WitHeurCritList::recRestShortage ()
   {
   culpritPart_   = NULL;
   culpritPeriod_ = -1;
   }

//------------------------------------------------------------------------------

void WitHeurCritList::postCmqs ()
   {
   if (culpritPart_ == NULL)
      return;

   if (listed_ (culpritPart_)[culpritPeriod_])
      return;

   myGlobalComp ()->addToRevCritListForHeur (culpritPart_, culpritPeriod_);

   listed_ (culpritPart_)[culpritPeriod_] = true;

   culpritPart_   = NULL;
   culpritPeriod_ = -1;
   }

//------------------------------------------------------------------------------
// Implementation of class PclBldr.
//------------------------------------------------------------------------------

WitPclBldr::WitPclBldr (WitProblem * theProblem):

      WitProbAssoc (theProblem),

      curDemand_   (NULL),
      curShipPer_  (-1),
      shortPart_   (NULL),
      shortPer_    (-1)
   {
   myMsgFac () ("pgdCritListMsg");
   }

//------------------------------------------------------------------------------

WitPclBldr::~WitPclBldr ()
   {
   }

//------------------------------------------------------------------------------

void WitPclBldr::initSoln ()
   {
   curDemand_  = NULL;
   curShipPer_ = -1;
   shortPart_  = NULL;
   shortPer_   = -1;

   myGlobalComp ()->clearPgdCritList ();
   }

//------------------------------------------------------------------------------

void WitPclBldr::preIncAlloc (WitDemand * theDemand, WitPeriod shipPer)
   {
   witAssert (theDemand  != NULL);
   witAssert (shipPer    != -1);
   witAssert (shortPart_ == NULL);
   witAssert (shortPer_  == -1);

   curDemand_  = theDemand;
   curShipPer_ = shipPer;
   }

//------------------------------------------------------------------------------

void WitPclBldr::recFundShortage (WitPart * thePart, WitPeriod thePer)
   {
   witAssert (thePart != NULL);
   witAssert (thePer  != -1);

   shortPart_ = thePart;
   shortPer_  = thePer;
   }

//------------------------------------------------------------------------------

void WitPclBldr::recRestShortage ()
   {
   shortPart_ = NULL;
   shortPer_  = -1;
   }

//------------------------------------------------------------------------------

void WitPclBldr::postCmqs ()
   {
   WitPclEl * thePclEl;

   if (shortPart_ == NULL)
      return;

   thePclEl   = new WitPclEl (shortPart_, shortPer_, curDemand_, curShipPer_);

   myGlobalComp ()->appendToPgdCritListForHeur (thePclEl);

   shortPart_ = NULL;
   shortPer_  = -1;
   }

//------------------------------------------------------------------------------
// Implementation of class WitPclEl.
//------------------------------------------------------------------------------

WitPclEl::WitPclEl (
         WitPart *   thePart,
         WitPeriod   shortPer,
         WitDemand * theDemand,
         WitPeriod   shipPer):

      critPart_  (thePart),
      critPer_   (shortPer),
      myDemand_  (theDemand),
      myShipPer_ (shipPer)
   {
   witAssert (thePart   != NULL);
   witAssert (shortPer  != -1);
   witAssert (theDemand != NULL);
   witAssert (shipPer   != -1);
   }

//------------------------------------------------------------------------------

WitPclEl::~WitPclEl ()
   {
   }
