//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "HeurAtor.C"
//
// Contains the implementation of class HeurAtor.
//------------------------------------------------------------------------------

#include <HeurAtor.h>
#include <HeurAllMgr.h>
#include <AvailSched.h>
#include <ReqSched.h>
#include <HeurCrit.h>
#include <PclBldr.h>
#include <HeurModifier.h>
#include <RtMgr.h>
#include <RtCand.h>
#include <Selector.h>
#include <MrMgr.h>
#include <PrMgr.h>
#include <BaMgr.h>
#include <MeMgr.h>
#include <SsrMgr.h>
#include <PenExMgr.h>
#include <SglSrcMgr.h>
#include <Pegger.h>
#include <PipMgr.h>
#include <Material.h>
#include <BopEntry.h>
#include <BoundSet.h>
#include <Post.h>
#include <FeasChkr.h>
#include <Variant.h>
#include <MsgFac.h>
#include <DataRepos.h>
#include <FixedPer.h>

//------------------------------------------------------------------------------
// Implementation of class HeurAtor
//------------------------------------------------------------------------------

WitHeurAtor::WitHeurAtor (
         WitProblem * theProblem,
         WitPegger *  thePegger,
         bool         forOpt):

      WitProbAssoc      (theProblem),

      myAvailSched_     (NULL),
      myReqSched_       (NULL),
      myHeurCritList_   (NULL),
      myPclBldr_        (NULL),
      myHeurModifier_   (NULL),
      mySelector_       (NULL),
      myMrMgr_          (NULL),
      myPrMgr_          (NULL),
      myBaMgr_          (NULL),
      myMeMgr_          (NULL),
      mySsrMgr_         (NULL),
      myPenExMgr_       (NULL),
      mySglSrcMgr_      (NULL),
      myPegger_         (NULL),
      tempCommRepos_    (NULL),
      multiRoute_       (false),
      buildAhead_       (false),
      multiExec_        (false),
      penExec_          (false),
      selSplit_         (false),
      sglSrc_           (false),
      perfPegging_      (false),
      userHeurStart_    (false),
      finiteExecBounds_ (false),
      leadTimeBounds_   (false),
      ltbMinPer_        (0),
      tempCommActive_   (false),
      permCommActive_   (false),
      blocked_          (),
      hasSubsToNet_     (myProblem (), false),
      searchInc_        (1.0),
      topPart_          (NULL),
      topPer_           (-1),
      nMaxCommits_      (0),
      nSelCommits_      (0),
      nBsearches_       (0),
      nTempComms_       (0),
      nPermComms_       (0),
      nComms_           (0),
      failedOn1_        (false)
   {
   WitOperation * theOpn;
   WitSubEntry *  theSub;

   if (not forOpt)
      userHeurStart_ = myGlobalComp ()->userHeurStart ();

   if (userHeurStart_)
      myProblem ()->myPostprocessor ()->myFeasChkr ()->verifyUhsFeas ();

   if (not forOpt)
      if (myGlobalComp ()->computeCriticalList ())
         myHeurCritList_ = new WitHeurCritList (myProblem ());

   if (not forOpt)
      if (myGlobalComp ()->pgdCritListMode ())
         myPclBldr_ = new WitPclBldr (myProblem ());

   myAvailSched_ = new WitAvailSched (this);
   myReqSched_   = new WitReqSched   (myProblem ());

   if (myGlobalComp ()->modHeurAlloc ())
      myHeurModifier_ = new WitHeurModifier (myAvailSched_);

   if (myGlobalComp ()->skipFailures ())
      initBlocked ();

   forEachSubEntry (theSub, myProblem ())
      if (theSub->netAllowed ())
         hasSubsToNet_ (theSub->myBomEnt ()) = true;

   setUpSelection ();

   if (myGlobalComp ()->penExec ())
      if (not multiRoute_)
         myMsgFac () ("penExecWoMultiRouteWmsg");

   if (WitSglSrcMgr::sglSrcReq (myProblem ()))
      if (not multiRoute_)
         myMsgFac () ("sglSrcWoMultiRouteWmsg");

   if (not userHeurStart_)
      initSoln ();

   forEachOperation (theOpn, myProblem ())
      if (theOpn->execBounds ()->hardUB () != -1.0)
         {
         finiteExecBounds_ = true;

         break;
         }

   compLeadTimeBounds ();

   if (leadTimeBounds_)
      setUpLeadTimeBounds ();

   if (not forOpt)
      if (myGlobalComp ()->perfPegging ())
         {
         witAssert (thePegger != NULL);

         perfPegging_ = true;

         myPegger_    = thePegger;

         myPegger_->setUpPeggedHeurAlloc (this);
         }

   if (myGlobalComp ()->pipSeqFromHeur ())
      myProblem ()->myPipMgr ()->clearShipSeq ();

   tempCommRepos_ = new WitDataRepos (theProblem);

   myMsgFac () ("heurAllActiveMsg");
   }

//------------------------------------------------------------------------------

void WitHeurAtor::deleteInstance (WitHeurAtor * theHeurAtor)
   {
   theHeurAtor->shutDown ();
      
   delete theHeurAtor;
   }

//------------------------------------------------------------------------------

double WitHeurAtor::incHeurAlloc (
      WitDemand * theDemand, 
      WitPeriod   thePer, 
      double      desIncVol,
      bool        asapMultiExec)
   {
   double demandedIncVol;
   double netQty;
   double incVol;

   witAssert (desIncVol >= 0.0);
   witAssert (thePer >= 0);
   witAssert (thePer < nPeriods ());

   if (myGlobalComp ()->modHeurAlloc ())
      myHeurModifier ()->checkFeasibility ();

   if (desIncVol <= NET_TOL)
      return 0.0;

   demandedIncVol = compDemandedIncVol (theDemand, thePer, desIncVol);

   if (demandedIncVol <= NET_TOL)
      return 0.0;

   preIncHeurAlloc (theDemand, thePer, asapMultiExec);

   netQty = demandedIncVol;

   if (theDemand->buildAheadUB ()[thePer] == 0)
      commitMaxQty (theDemand->demandedPart (), thePer, netQty);
   else
      doBaByDemand (theDemand,                  thePer, netQty);

   incVol = demandedIncVol - positivePart (netQty);

   postIncHeurAlloc (theDemand, thePer, desIncVol, incVol);

   return incVol;
   }

//------------------------------------------------------------------------------

void WitHeurAtor::enforceStockSLBs ()
   {
   WitPart * thePart;

   WitObjStack <WitPart> reversedParts (myProblem ());

   myMsgFac () ("enfStockSLBsMsg");

   if (myGlobalComp ()->prefHighStockSLBs ())
      {
      revCopyInto (reversedParts, myCompMgr ()->allParts ());

      while (reversedParts.pop (thePart))
         enforceStockSLBs (thePart);
      }
   else
      forEachPart (thePart, myProblem ())
         enforceStockSLBs (thePart);
   }

//------------------------------------------------------------------------------

void WitHeurAtor::commitMaxQtySel (double & netQty)
   {
   double maxQty;

   maxQty = findMaxQty (netQty);

   if (maxQty >= NET_TOL)
      {
      permCommit (maxQty);

      netQty -= maxQty;

      witAssert (netQty >= 0.0);

      if (penExec_)
         mySelector ()->recCommitVol (maxQty);
      }

   if (critListMode ())
      myHeurCritList ()->postCmqs ();

   if (pclMode ())
      myPclBldr ()->     postCmqs ();
   }

//------------------------------------------------------------------------------

bool WitHeurAtor::tempCommit (double qty)
   {
   bool success;

   witAssert (not permCommActive_);
   witAssert (not tempCommActive_);

   nTempComms_ ++;

   tempCommActive_ = true;

   success         = commit (qty);

   if (not success)
      failedOn1_ = (qty == 1.0);

   if (multiSel ())
      mySelector ()->postTempCommit (success);

   tempCommActive_ = false;

   return success;
   }

//------------------------------------------------------------------------------

void WitHeurAtor::permCommit1SS ()
   {
   witAssert (selSplit_);

   permCommit (1.0);

   myAvailSched_->addToIncAvailVol (topPart (), topPer (), 1.0);
   }

//------------------------------------------------------------------------------

bool WitHeurAtor::incAvailPos (WitPart * thePart, WitPeriod thePer)
   {
   return (myAvailSched_->incAvailVol (thePart, thePer) > NET_TOL);
   }

//------------------------------------------------------------------------------

bool WitHeurAtor::selectiveSR (WitMaterial * theMat)
   {
   return myAvailSched_->selectiveSR (theMat);
   }

//------------------------------------------------------------------------------

void WitHeurAtor::saveVal (int & theVar)
   {
   if (tempCommActive_)
      tempCommRepos_->saveVal (theVar);

   else if (sglSrc_)
      mySglSrcMgr ()->saveAllocVal (theVar);
   }

//------------------------------------------------------------------------------

void WitHeurAtor::saveVal (double & theVar)
   {
   if (tempCommActive_)
      tempCommRepos_->saveVal (theVar);

   else if (sglSrc_)
      mySglSrcMgr ()->saveAllocVal (theVar);
   }

//------------------------------------------------------------------------------

void WitHeurAtor::saveExecVol (WitOperation * theOpn, WitPeriod thePer)
   {
   if (tempCommActive_)
      tempCommRepos_->saveExecVol (theOpn, thePer);

   else if (sglSrc_)
      mySglSrcMgr ()->saveExecVol (theOpn, thePer);
   }

//------------------------------------------------------------------------------

void WitHeurAtor::saveSubVol (WitSubEntry * theSub, WitPeriod thePer)
   {
   if (tempCommActive_)
      tempCommRepos_->saveSubVol (theSub, thePer);

   else if (sglSrc_)
      mySglSrcMgr ()->saveSubVol (theSub, thePer);
   }

//------------------------------------------------------------------------------

void WitHeurAtor::printAvailSched ()
   {
   myAvailSched_->print ();
   }

//------------------------------------------------------------------------------

WitHeurAtor::~WitHeurAtor ()
   {
   }

//------------------------------------------------------------------------------

void WitHeurAtor::initBlocked ()
   {
   WitDemand * theDemand;
   WitPart *   thePart;

   blocked_.allocate1D (myProblem ());

   forEachDemand (theDemand, myProblem ())
      {
      thePart = theDemand->demandedPart ();

      if (blocked_ (thePart).isAllocated ())
         continue;

      if (theDemand->demandVol () == 0.0)
         continue;

      blocked_ (thePart).allocate (myProblem (), false);
      }
   }

//------------------------------------------------------------------------------

void WitHeurAtor::setUpSelection ()
   {
   if (not WitSelector::instNeededFor (this))
      return;

   mySelector_ = new WitSelector (this);

   multiRoute_ = mySelector ()->multiRoute ();
   buildAhead_ = mySelector ()->buildAhead ();
   multiExec_  = mySelector ()->multiExec  ();
   penExec_    = mySelector ()->penExec    ();
   selSplit_   = mySelector ()->selSplit   ();
   sglSrc_     = mySelector ()->sglSrc     ();

   if (multiRoute_)
      myMrMgr_ = mySelector ()->myMrMgr ();

   if (mySelector ()->propRtg ())
      myPrMgr_ = mySelector ()->myPrMgr ();

   if (buildAhead_)
      myBaMgr_  = mySelector ()->myBaMgr ();

   if (multiExec_)
      myMeMgr_  = mySelector ()->myMeMgr ();

   if (mySelector ()->selectiveSR ())
      {
      mySsrMgr_ = mySelector ()->mySsrMgr ();

      myAvailSched_->attachSsrMgr ();
      }

   if (penExec_)
      myPenExMgr_ = mySelector ()->myPenExMgr ();

   if (sglSrc_)
      mySglSrcMgr_ = mySelector ()->mySglSrcMgr ();
   }

//------------------------------------------------------------------------------

void WitHeurAtor::initSoln ()
   {
   WitDemand *    theDemand;
   WitOperation * theOpn;
   WitSubEntry *  theSub;

   if (critListMode ())
      myHeurCritList ()->initSoln ();

   if (pclMode ())
      myPclBldr ()->initSoln ();

   forEachDemand (theDemand, myProblem ())
      theDemand->clearShipVolForHeur ();

   forEachOperation (theOpn, myProblem ())
      theOpn->clearExecVolForHeur ();

   forEachSubEntry (theSub, myProblem ())
      theSub->clearSubVolForHeur ();
   }

//------------------------------------------------------------------------------

void WitHeurAtor::compLeadTimeBounds ()
   {
   WitPart * thePart;

   leadTimeBounds_ = false;

   forEachPart (thePart, myProblem ())
      {
      if (thePart->boundedLeadTimes ())
         {
         leadTimeBounds_ = true;

         return;
         }
      }
   }

//------------------------------------------------------------------------------

void WitHeurAtor::setUpLeadTimeBounds ()
   {
   myMsgFac () ("leadTimeBoundsMsg");

   if (myGlobalComp ()->skipFailures ())
      {
      myMsgFac () ("boolAttReqForLtbSmsg", "skipFailures", false);
      }

   if (not myGlobalComp ()->selectionRecovery ())
      {
      myMsgFac () ("boolAttReqForLtbSmsg", "selectionRecovery", true);
      }
   }

//------------------------------------------------------------------------------

double WitHeurAtor::compDemandedIncVol (
      WitDemand * theDemand,
      WitPeriod   thePer,
      double      desIncVol)
   {
   double incVolUB;

   if (theDemand->shipLateAllowed ())
      incVolUB = minUnmetCumDem (theDemand, thePer);
   else
      incVolUB =
           theDemand->demandVol ()[thePer]
         - theDemand->shipVol   ()[thePer];

   return min (desIncVol, incVolUB);
   }

//------------------------------------------------------------------------------

double WitHeurAtor::minUnmetCumDem (WitDemand * theDemand, WitPeriod thePer)
   {
   double    unmetCD;
   WitPeriod othPer;
   double    minUnmetCD;

   unmetCD = theDemand->cumDemandVol ()[thePer];

   for (othPer = 0; othPer <= thePer; othPer ++)
      unmetCD -= theDemand->shipVol ()[othPer];

   minUnmetCD = unmetCD;

   for (othPer = thePer + 1; othPer < nPeriods (); othPer ++)
      {
      unmetCD +=
           theDemand->demandVol ()[othPer] 
         - theDemand->shipVol   ()[othPer];

      setToMin (minUnmetCD, unmetCD);
      }

   return minUnmetCD;
   }

//------------------------------------------------------------------------------

void WitHeurAtor::preIncHeurAlloc (
      WitDemand * theDemand, 
      WitPeriod   thePer, 
      bool        asapMultiExec)
   {
   if (perfPegging_)
      myPegger_->setUpPeggedIncAlloc (theDemand, thePer);

   if (pclMode ())
      myPclBldr ()->preIncAlloc (theDemand, thePer);

   if (myGlobalComp ()->twoWayMultiExec ())
      myMeMgr ()->setInitDirIsAsap (asapMultiExec);
   else
      witAssert (not asapMultiExec);

   if (leadTimeBounds_)
      ltbMinPer_ =
         thePer - theDemand->leadTimeUB ()[thePer];

   searchInc_ = theDemand->searchInc ();
   }

//------------------------------------------------------------------------------

void WitHeurAtor::doBaByDemand (
      WitDemand * theDemand, 
      WitPeriod   lastPer,
      double &    netQty)
   {
   WitPart * thePart;
   WitPeriod firstPer;
   WitPeriod thePer;

   thePart  = theDemand->demandedPart ();

   firstPer = max (0, lastPer - theDemand->buildAheadUB ()[lastPer]);

   for (thePer = firstPer; thePer < lastPer; thePer ++)
      if (not thePart->canStock (thePer))
         firstPer = thePer + 1;

   if (theDemand->prefBuildAhead ())
      for (thePer = firstPer; thePer <= lastPer; thePer ++)
         {
         commitMaxQty (thePart, thePer, netQty);

         if (netQty <= NET_TOL)
            break;
         }
   else
      for (thePer = lastPer; thePer >= firstPer; thePer --)
         {
         commitMaxQty (thePart, thePer, netQty);

         if (netQty <= NET_TOL)
            break;
         }
   }

//------------------------------------------------------------------------------

void WitHeurAtor::postIncHeurAlloc (
      WitDemand * theDemand, 
      WitPeriod   thePer, 
      double      desIncVol,
      double      incVol)
   {
   if (incVol >= NET_TOL)
      theDemand->incShipVolForHeur (thePer, incVol);

   if (myGlobalComp ()->printIncAlloc ())
      if (myMsgFac ().mayIssue ("incAllocMsg"))
         myMsgFac () ("incAllocMsg",
            theDemand->demandedPartName (),
            theDemand->demandName (),
            thePer,
            desIncVol,
            incVol);

   searchInc_ = 1.0;

   if (leadTimeBounds_)
      ltbMinPer_ = 0;

   if (penExec_)
      myPenExMgr ()->replenishPsSups ();

   if (multiSel ())
      if (myGlobalComp ()->selectionRecovery ())
         mySelector ()->recoverInitState ();

   if (perfPegging_)
      myPegger_->shutDownPeggedIncAlloc ();

   if (myGlobalComp ()->pipSeqFromHeur ())
      if (myProblem ()->heurVariant ()->isCurrent ())
         if (incVol > NET_TOL)
            myProblem ()->
               myPipMgr ()->
                  appendToShipSeq (theDemand, thePer, incVol);
   }

//------------------------------------------------------------------------------

void WitHeurAtor::enforceStockSLBs (WitPart * thePart)
   {
   WitMaterial * theMat;
   WitPeriod     thePer;

   theMat = thePart->thisMat ();

   if (theMat != NULL)
      if (theMat->stockBounds ()->softLB () != 0.0)
         forEachPeriod (thePer, myProblem ())
            if (not theMat->mandEC ()[thePer])
               if (theMat->stockBounds ()->softLB ()[thePer] >= NET_TOL)
                  enforceStockSLB (theMat, thePer);
   }

//------------------------------------------------------------------------------

void WitHeurAtor::enforceStockSLB (WitMaterial * theMat, WitPeriod thePer)
   {
   double unachVol;
   double netVol;
   double commitVol;

   myAvailSched_->reserveAchStSLB (theMat, thePer, unachVol);

   if (unachVol < NET_TOL)
      return;

   netVol = unachVol;

   commitMaxQty (theMat, thePer, netVol);

   commitVol = unachVol - netVol;

   if (commitVol >= NET_TOL)
      if (thePer < lastPeriod ())
         myAvailSched_->addToIncAvailVol (theMat, thePer + 1, commitVol);
   }

//------------------------------------------------------------------------------

void WitHeurAtor::commitMaxQty (
      WitPart * thePart, 
      WitPeriod thePer,
      double &  netQty)
   {
   if (myGlobalComp ()->skipFailures ())
      if (blocked_ (thePart).isAllocated ())
         if (blocked_ (thePart)[thePer])
            return;

   if (devMode ())
      nMaxCommits_ ++;

   witAssert (topPart_ == NULL);

   topPart_ = thePart;
   topPer_  = thePer;

   if (sglSrc_)
      mySglSrcMgr ()->commitMaxQty (netQty);

   else if (multiSel ())
      mySelector ()->commitMaxQty (netQty);

   else
      commitMaxQtySel (netQty);

   topPer_  = -1;
   topPart_ = NULL;

   if (myGlobalComp ()->skipFailures ())
      if (blocked_ (thePart).isAllocated ())
         if (netQty > SEARCH_TOL * searchInc ())
            blocked_ (thePart)[thePer] = true;
   }

//------------------------------------------------------------------------------

void WitHeurAtor::permCommit (double qty)
   {
   bool success;

   witAssert (not tempCommActive_);
   witAssert (not permCommActive_);

   permCommActive_ = true;

   nPermComms_ ++;

   if (sglSrc_)
      mySglSrcMgr ()->recPermCommit ();

   success = commit (qty);

   permCommActive_ = false;

   if (not success)
      myMsgFac () ("permCommitFailedFmsg", 
         topPart ()->partName (), 
         topPer (), 
         qty);

   if (selPrintLevelAt (3))
      fprintf (msgFile (),
         "\n"
         "Permanent Commit: Part %s, Period %d, Qty: %.3f\n",
         topPart_->partName ().myCstring (),
         topPer_,
         qty);
   }

//------------------------------------------------------------------------------

double WitHeurAtor::findMaxQty (double desQty)
   {
   bool success;

   failedOn1_ = false;

   if (devMode ())
      if (multiSel ())
         {
         nSelCommits_ ++;

         if (selPrintLevelAt (3))
            mySelector ()->printCommitMaxQtySel ();
         }

   if (desQty <=  SEARCH_TOL * searchInc ())
      return 0.0;

   if (desQty <= (1.0 + SEARCH_TOL) * searchInc ())
      {
      success = tempCommit (desQty);

      return
         success?
            desQty:
            0.0;
      }

   success = tempCommit (searchInc ());

   if (not success)
      return 0.0;

   success = tempCommit (desQty);

   if (success)
      return desQty;

   return binarySearch (desQty);
   }

//------------------------------------------------------------------------------
// HeurAtor::binarySearch (...).
//
// Local variables:
//    failMult: the smallest quantity for which tempCommit is known (or assumed)
//              to return failure.
//    okMult:   the largest quantity for which tempCommit is known (or assumed)
//              to return success.
//    curMult:  the current quantity to be tried in tempCommit.
//
// failMult, okMult, and curMult are all expressed as multiples of searchInc.
//------------------------------------------------------------------------------

double WitHeurAtor::binarySearch (double desQty)
   {
   double failMult;
   double okMult;
   double curMult;
   bool   success;

   if (devMode ())
      nBsearches_ ++;

   failMult = ceil (desQty / searchInc () - SEARCH_TOL);

   okMult   = 1.0;

   while (failMult > okMult + 1.0 + SEARCH_TOL)
      {
      curMult = floor (((okMult + failMult) / 2.0) + SEARCH_TOL);

      success = tempCommit (curMult * searchInc ());

      if (success)
         okMult   = curMult;
      else
         failMult = curMult;
      }

   return (okMult * searchInc ());
   }

//------------------------------------------------------------------------------

bool WitHeurAtor::commit (double qty)
   {
   bool success;

   witAssert (tempCommActive_ or permCommActive_);

   nComms_ ++;

   if (selSplit_)
      mySelector ()->preCommitSS ();

   success = innerCommit (qty);

   myReqSched_->clear ();

   tempCommRepos_->restore ();

   if (multiSel ())
      mySelector ()->postCommit ();

   return success;
   }

//------------------------------------------------------------------------------

bool WitHeurAtor::innerCommit (double qty)
   {
   WitPtrVecItr <WitPart> theItr;
   WitPart *              fillPart;
   WitPeriod              fillPer;
   double                 theReqVol;
   bool                   fillOK;

   if (qty <= NET_TOL)
      return true;

   myReqSched_->addTo (topPart (), topPer (), qty);

   topPart ()->belowList ().attachItr (theItr);

   while (theItr.advance (fillPart))
      for (fillPer =  myReqSched_->firstPer (fillPart); 
           fillPer <= myReqSched_->lastPer  (fillPart);
           fillPer ++)
         {
         theReqVol = myReqSched_->reqVol (fillPart, fillPer);

         if (theReqVol <= NET_TOL)
            continue;

         fillOK = fillReq (fillPart, fillPer, theReqVol);

         if (not fillOK)
            return false;
         }

   return true;
   }

//------------------------------------------------------------------------------

bool WitHeurAtor::fillReq (
      WitPart * thePart, 
      WitPeriod fillPer, 
      double    theReqVol)
   {
   double        netReqVol;
   WitMaterial * theMat;
   bool          bddSsrSplit;

   witAssert (theReqVol > NET_TOL);

   netReqVol   = theReqVol;

   theMat      = thePart->thisMat ();

   bddSsrSplit = false;

   if (theMat == NULL)
      myAvailSched_->netWithIncAvail (thePart, fillPer, netReqVol);

   else if (myAvailSched_->eitherSR (theMat))
      netAndRollReq                 (theMat,  fillPer, netReqVol, bddSsrSplit);

   else
      myAvailSched_->netMatWoSR     (theMat,  fillPer, netReqVol);

   if (multiSel ())
      if (netReqVol < theReqVol)
         if (not bddSsrSplit)
            mySelector ()->recFillByCons (thePart, fillPer);

   if (netReqVol <= NET_TOL)
      return true;

   if (multiSel ())
      mySelector ()->recFillNetRec (thePart, fillPer);

   return fillNetReq (thePart, fillPer, netReqVol);
   }

//------------------------------------------------------------------------------

bool WitHeurAtor::fillNetReq (
      WitPart * thePart, 
      WitPeriod fillPer, 
      double    netReqVol)
   {
   WitPeriod expPer;

   witAssert (netReqVol > NET_TOL);

   if (selSplit_)
      if (buildAhead_)
         if (myBaMgr ()->selIsSplit (thePart, fillPer))
            return fillNetReqBASS (thePart, fillPer, netReqVol);

   if (penExec_)
      myPenExMgr ()->recExpBeyondNet (thePart, fillPer, false);

   if (buildAhead_)
      expPer = myBaMgr ()->selExpPer (thePart, fillPer);
   else
      expPer = thePart->explosionPeriod (fillPer);

   if (not thePart->explodeable (expPer))
      {
      if (multiSel ())
         mySelector ()->recFundShortage (thePart, fillPer);

      recClShortage (thePart, fillPer);

      return false;
      }

   if (thePart->boundedLeadTimes ())
      if (expPer < ltbMinPer_)
         {
         if (multiSel ())
            mySelector ()->recLTBoundShortage  (thePart, fillPer);

         recClShortage (thePart, fillPer);

         return false;
         }

   if (sglSrc_)
      if (mySglSrcMgr ()->expProhibited (thePart, expPer))
         {
         mySelector ()->recFundShortage (thePart, fillPer);

         return false;
         }

   return explodePart (thePart, expPer, netReqVol, fillPer);
   }

//------------------------------------------------------------------------------

bool WitHeurAtor::fillNetReqBASS (
      WitPart * thePart, 
      WitPeriod fillPer, 
      double    netReqVol)
   {
   bool           expOK;
   WitPerDblStack theSplitPairs (myProblem ());
   double         unbddVol;
   WitFixedPer *  theFixedPer;
   double         expVol;
   WitPeriod      expPer;

   witAssert (selSplit_);

   witAssert (netReqVol > NET_TOL);

   witAssert (myBaMgr ()->selIsSplit (thePart, fillPer));

   if (penExec_)
      myPenExMgr ()->recExpBeyondNet (thePart, fillPer, false);

   expOK = true;

   myBaMgr ()->
      defineSplit (thePart, fillPer, netReqVol, theSplitPairs, unbddVol);

   myBaMgr ()->setBoundedSplit (true);

   while (theSplitPairs.pop (theFixedPer, expVol))
      {
      expPer = theFixedPer->myPer ();

      expOK  = explodePartSS (thePart, expPer, expVol, fillPer);

      if (not expOK)
         break;
      }

   myBaMgr ()->setBoundedSplit (false);

   if (expOK)
      if (unbddVol > 0.0)
         {
         expPer = myBaMgr ()->selExpPer (thePart, fillPer);

         expOK  = explodePartSS (thePart, expPer, unbddVol, fillPer);
         }

   return expOK;
   }

//------------------------------------------------------------------------------

bool WitHeurAtor::explodePart (
      WitPart * thePart, 
      WitPeriod expPer,
      double    expVol,
      WitPeriod fillPer)
   {
   WitBopEntry * theBopEnt;
   WitPeriod     execPer;

   if (selSplit_)
      return explodePartSS (thePart, expPer, expVol, fillPer);

   if (thePart->propRtg ()[expPer])
      return explodePartPR (thePart, expPer, expVol, fillPer);

   theBopEnt = expBopEntry (thePart,   expPer);
   execPer   = execPerFor  (theBopEnt, expPer);

   return explodeBopEnt (theBopEnt, execPer, expVol, fillPer);
   }

//------------------------------------------------------------------------------

bool WitHeurAtor::explodePartSS (
      WitPart * thePart, 
      WitPeriod expPer,
      double    expVol,
      WitPeriod fillPer)
   {
   WitBopEntry * theBopEnt;

   witAssert (selSplit_);
   witAssert (expVol > NET_TOL);

   if (buildAhead_)
      myBaMgr ()->recExpMatSS (thePart, fillPer, expVol);

   if (thePart->boundedLeadTimes ())
      if (expPer < ltbMinPer_)
         {
         mySelector ()->recLTBoundShortage  (thePart, fillPer);

         recClShortage (thePart, fillPer);

         return false;
         }

   if (thePart->propRtg ()[expPer])
      {
      if (myPrMgr ()->selIsSplit (thePart, expPer))
         return explodePartPRSS (thePart, expPer, expVol, fillPer);

      return    explodePartPR   (thePart, expPer, expVol, fillPer);
      }

   if (multiRoute_)
      if (myMrMgr ()->selIsSplit (thePart, expPer))
         return explodePartMRSS (thePart, expPer, expVol, fillPer);

   theBopEnt = expBopEntry (thePart, expPer);

   return explodeBopEntSS (theBopEnt, expPer, expVol, fillPer);
   }

//------------------------------------------------------------------------------

bool WitHeurAtor::explodePartPRSS (
      WitPart * thePart, 
      WitPeriod expPer,
      double    expVol,
      WitPeriod fillPer)
   {
   WitRtCandDblStack theExpPairs (myProblem ());
   double            unbddVol;
   WitRtCand *       theRtCand;
   double            propExpVol;
   WitBopEntry *     theBopEnt;
   bool              expOK;

   myPrMgr ()->defineSplit (thePart, expPer, expVol, theExpPairs, unbddVol);

   while (theExpPairs.pop (theRtCand, propExpVol))
      {
      theBopEnt = theRtCand->myBopEnt ();

      myPrMgr ()->mySelMgrForProd ()->setBoundedSplit (true);

      expOK = explodeBopEntSS (theBopEnt, expPer, propExpVol, fillPer);

      myPrMgr ()->mySelMgrForProd ()->setBoundedSplit (false);

      if (not expOK)
         return false;
      }

   if (unbddVol <= NET_TOL)
      return true;

   return explodePartPR (thePart, expPer, unbddVol, fillPer);
   }

//------------------------------------------------------------------------------

bool WitHeurAtor::explodePartMRSS (
      WitPart * thePart, 
      WitPeriod expPer,
      double    expVol,
      WitPeriod fillPer)
   {
   bool              expOK;
   WitRtCandDblStack theSplitPairs (myProblem ());
   double            unbddVol;
   WitRtCand *       theRtCand;
   double            splitExpVol;
   WitBopEntry *     theBopEnt;

   witAssert (expVol > NET_TOL);

   expOK = true;

   myMrMgr ()->defineSplit (thePart, expPer, expVol, theSplitPairs, unbddVol);

   myMrMgr ()->mySelMgrForProd ()->setBoundedSplit (true);

   while (theSplitPairs.pop (theRtCand, splitExpVol))
      {
      theBopEnt = theRtCand->myBopEnt ();

      expOK     = explodeBopEntSS (theBopEnt, expPer, splitExpVol, fillPer);

      if (not expOK)
         break;
      }

   myMrMgr ()->mySelMgrForProd ()->setBoundedSplit (false);

   if (expOK)
      if (unbddVol > 0.0)
         {
         theBopEnt = expBopEntry (thePart, expPer);

         expOK     = explodeBopEntSS (theBopEnt, expPer, unbddVol, fillPer);
         }

   return expOK;
   }

//------------------------------------------------------------------------------

bool WitHeurAtor::explodePartPR (
      WitPart * thePart, 
      WitPeriod expPer,
      double    expVol,
      WitPeriod fillPer)
   {
   WitRtCandDblStack theExpPairs (myProblem ());
   WitRtCand *       theRtCand;
   double            propExpVol;
   WitBopEntry *     theBopEnt;
   WitPeriod         execPer;
   bool              expOK;

   if (selSplit_)
      myPrMgr ()->recordFlowSS (thePart, expPer, expVol);

   myPrMgr ()->definePropExp (thePart, expPer, expVol, theExpPairs);

   while (theExpPairs.pop (theRtCand, propExpVol))
      {
      theBopEnt = theRtCand->myBopEnt ();

      if (selSplit_)
         expOK = explodeBopEntSS (theBopEnt, expPer, propExpVol, fillPer);
      else
         {
         execPer = execPerFor (theBopEnt, expPer);

         expOK   = explodeBopEnt (theBopEnt, execPer, propExpVol, fillPer);
         }

      if (! expOK)
         return false;
      }

   return true;
   }

//------------------------------------------------------------------------------

bool WitHeurAtor::explodeBopEntSS (
      WitBopEntry * theBopEnt,
      WitPeriod     expPer,
      double        expVol,
      WitPeriod     fillPer)
   {
   WitPeriod execPer;

   witAssert (selSplit_);

   if (multiRoute_)
      myMrMgr ()->recordFlowSS (theBopEnt, expPer, expVol);

   if (multiExec_)
      if (myMeMgr ()->selIsSplit (theBopEnt, expPer))
         return explodeBopEntMESS (theBopEnt, expPer, expVol, fillPer);

   execPer = execPerFor (theBopEnt, expPer);

   return explodeBopEnt (theBopEnt, execPer, expVol, fillPer);
   }

//------------------------------------------------------------------------------

bool WitHeurAtor::explodeBopEntMESS (
      WitBopEntry * theBopEnt,
      WitPeriod     expPer,
      double        expVol,
      WitPeriod     fillPer)
   {
   bool           expOK;
   WitPerDblStack theSplitPairs (myProblem ());
   double         unbddVol;
   WitFixedPer *  theFixedPer;
   double         splitExpVol;
   WitPeriod      execPer;

   witAssert (myMeMgr ()->selIsSplit (theBopEnt, expPer));

   expOK = true;

   myMeMgr ()->
      defineSplit (theBopEnt, expPer, expVol, theSplitPairs, unbddVol);

   myMeMgr ()->setBoundedSplit (true);

   while (theSplitPairs.pop (theFixedPer, splitExpVol))
      {
      execPer = theFixedPer->myPer ();

      expOK   = explodeBopEnt (theBopEnt, execPer, splitExpVol, fillPer);

      if (not expOK)
         break;
      }

   myMeMgr ()->setBoundedSplit (false);

   if (expOK)
      if (unbddVol > 0.0)
         {
         execPer = execPerFor (theBopEnt, expPer);

         expOK   = explodeBopEnt (theBopEnt, execPer, unbddVol, fillPer);
         }

   return expOK;
   }

//------------------------------------------------------------------------------

bool WitHeurAtor::explodeBopEnt (
      WitBopEntry * theBopEnt,
      WitPeriod     execPer,
      double        qty,
      WitPeriod     fillPer)
   {
   WitOperation * theOpn;
   double         oldExecVol;
   double         deltaExecVol;
   bool           execBoundOK;

   witAssert (qty > NET_TOL);

   if (selSplit_)
      if (multiExec_)
         myMeMgr ()->recBopEntExecPerSS (theBopEnt, execPer, qty);

   if (sglSrc_)
      mySglSrcMgr ()->recExp (theBopEnt);

   theOpn       = theBopEnt->myOperation ();

   oldExecVol   = theOpn->execVol ()[execPer];

   deltaExecVol = qty / theBopEnt->effProdRate ()[execPer];

   deltaExecVol = theOpn->lotSizeDelta (execPer, oldExecVol, deltaExecVol);

   if (deltaExecVol < NET_TOL)
      return true;

   execBoundOK =
      checkExecBound (theBopEnt, execPer, oldExecVol, deltaExecVol, fillPer);

   if (not execBoundOK)
      return false;

   if (penExec_)
      if (not myPenExMgr ()->restExpAllowed (theBopEnt, fillPer))
         {
         mySelector ()->recShortage ();

         if (critListMode ())
            myHeurCritList ()->recRestShortage ();

         if (pclMode ())
            myPclBldr ()->recRestShortage ();

         return false;
         }

   if (sglSrc_)
      if (mySglSrcMgr ()->expProhibited (theOpn, execPer))
         {
         mySelector ()->recFundShortage (theBopEnt->myPart (), fillPer);

         return false;
         }

   innerExplodeBopEnt (theBopEnt, execPer, qty, deltaExecVol, fillPer);

   return true;
   }

//------------------------------------------------------------------------------

bool WitHeurAtor::checkExecBound (
      WitBopEntry * theBopEnt,
      WitPeriod     execPer,
      double        oldExecVol,
      double        deltaExecVol,
      WitPeriod     fillPer)
   {
   bool execBoundOK;

   if (not finiteExecBounds_)
      return true;

   execBoundOK =
      theBopEnt->
         myOperation ()->
            execBounds ()->
               hardUBisSatisfied (execPer, oldExecVol + deltaExecVol);

   if (not execBoundOK)
      {
      if (multiSel ())
         mySelector ()->recExecBoundShortage (theBopEnt, execPer, fillPer);

      recClShortage (theBopEnt->myPart (), fillPer);
      }

   return execBoundOK;
   }

//------------------------------------------------------------------------------

void WitHeurAtor::recClShortage (WitPart * thePart, WitPeriod fillPer)
   {
   if (critListMode ())
      myHeurCritList ()->recFundShortage (thePart, fillPer);

   if (pclMode ())
      myPclBldr ()->     recFundShortage (thePart, fillPer);
   }

//------------------------------------------------------------------------------

void WitHeurAtor::innerExplodeBopEnt (
      WitBopEntry * theBopEnt,
      WitPeriod     execPer,
      double        qty,
      double        deltaExecVol,
      WitPeriod     fillPer)
   {
   WitOperation * theOpn;
   double         execVolVal;
   WitBomEntry *  theBomEnt;

   witAssert (deltaExecVol >= NET_TOL);

   theOpn = theBopEnt->myOperation ();

   saveExecVol (theOpn, execPer);

   execVolVal = theOpn->execVol ()[execPer] + deltaExecVol;

   theOpn->storeHeurExecVol (execPer, execVolVal);

   if (perfPegging_)
      if (permCommActive_)
         myPegger_->pegDeltaVol (theOpn, execPer, deltaExecVol);

   forEachEl (theBomEnt, theOpn->bom ())
      {
      explodeBomEntry (
         theBomEnt, 
         execPer, 
         deltaExecVol, 
         fillPer, 
         theBopEnt);

      if (penExec_)
         myPenExMgr ()->postExpBomEnt ();
      }

   explodeBopEntries (theBopEnt, execPer, deltaExecVol);

   myAvailSched_->addToIncAvailVol (theBopEnt->myPart (), fillPer, - qty);
   }

//------------------------------------------------------------------------------

void WitHeurAtor::explodeBopEntries (
      WitBopEntry * theBopEnt,
      WitPeriod     execPer,
      double        deltaExecVol)
   {
   WitOperation * theOpn;
   WitBopEntry *  prodBopEnt;
   double         deltaProdVol;
   WitPart *      prodPart;
   WitPeriod      prodPer;

   theOpn = theBopEnt->myOperation ();

   forEachEl (prodBopEnt, theOpn->bop ())
      {
      deltaProdVol = deltaExecVol * prodBopEnt->effProdRate ()[execPer];

      if (deltaProdVol <= NET_TOL)
         continue;

      prodPart = prodBopEnt->myPart ();
      prodPer  = prodBopEnt->impactPeriod ()[execPer];

      myAvailSched_->addToIncAvailVol (prodPart, prodPer, deltaProdVol);

      if (penExec_)
         if (prodBopEnt != theBopEnt)
            if (permCommActive_)
               myPenExMgr ()->reqPsSupRep (prodPart, prodPer);
      }
   }

//------------------------------------------------------------------------------

void WitHeurAtor::netAndRollReq (
      WitMaterial * theMat,
      WitPeriod     thePer,
      double &      netReqVol,
      bool &        bddSsrSplit)
   {
   WitPeriod rollPer;
   double    rollVol;

   witAssert (myAvailSched_->eitherSR (theMat));

   rollPer =
      myAvailSched_->netAndRoll (
         theMat,
         thePer,
         netReqVol,
         rollVol,
         bddSsrSplit);

   if (rollPer < 0)
      return;

   myReqSched_->addTo (theMat, rollPer, rollVol);

   if (multiSel ())
      if (tempCommActive_)
         mySelector ()->rollReq (theMat, thePer, rollPer, rollVol);
   }

//------------------------------------------------------------------------------

void WitHeurAtor::explodeBomEntry (
      WitBomEntry * theBomEnt,
      WitPeriod     execPer,
      double        deltaExecVol,
      WitPeriod     fillPer,
      WitBopEntry * theBopEnt)
   {
   WitConsEntry * theConsEnt;

   witAssert (deltaExecVol >= NET_TOL);

   if (not theBomEnt->inEffect (execPer))
      return;

   netConsEntries (theBomEnt, execPer, deltaExecVol, fillPer, theBopEnt);

   if (deltaExecVol < NET_TOL )
      return;

   if (theBomEnt->propRtg ()[execPer])
      {
      if (myPrMgr ()->selIsSplit (theBomEnt, execPer))
         explodeBomEntryPRSS (
            theBomEnt, execPer, deltaExecVol, fillPer, theBopEnt);
      else
         explodeBomEntryPR (
            theBomEnt, execPer, deltaExecVol, fillPer, theBopEnt);

      return;
      }

   if (multiRoute_)
      if (myMrMgr ()->selIsSplit (theBomEnt, execPer))
         {
         explodeBomEntryMRSS (
            theBomEnt,
            execPer,
            deltaExecVol,
            fillPer,
            theBopEnt); 

         return;
         }

   theConsEnt = 
      multiRoute_?
         myMrMgr ()->selConsEnt (theBomEnt, execPer):
         theBomEnt;

   explodeConsEntry (
      theConsEnt,
      execPer, 
      deltaExecVol,
      fillPer, 
      theBopEnt);
   }

//------------------------------------------------------------------------------

void WitHeurAtor::explodeBomEntryMRSS (
      WitBomEntry * theBomEnt,
      WitPeriod     execPer,
      double        deltaExecVol,
      WitPeriod     fillPer,
      WitBopEntry * theBopEnt)
   {
   WitRtCandDblStack theSplitPairs (myProblem ());
   double            unbddVol;
   WitRtCand *       theRtCand;
   double            splitExecVol;
   WitConsEntry *    theConsEnt;

   witAssert (deltaExecVol >= NET_TOL);

   myMrMgr ()->
      defineSplit (theBomEnt, execPer, deltaExecVol, theSplitPairs, unbddVol);

   myMrMgr ()->mySelMgrForCons ()->setBoundedSplit (true);

   while (theSplitPairs.pop (theRtCand, splitExecVol))
      {
      theConsEnt = theRtCand->myConsEnt ();

      explodeConsEntry (theConsEnt, execPer, splitExecVol, fillPer, theBopEnt);
      }

   myMrMgr ()->mySelMgrForCons ()->setBoundedSplit (false);

   if (unbddVol > 0.0)
      {
      theConsEnt = myMrMgr ()->selConsEnt (theBomEnt, execPer);

      explodeConsEntry (theConsEnt, execPer, unbddVol, fillPer, theBopEnt);
      }
   }

//------------------------------------------------------------------------------

void WitHeurAtor::netConsEntries (
      WitBomEntry * theBomEnt,
      WitPeriod     execPer,
      double &      netExecQty,
      WitPeriod     fillPer,
      WitBopEntry * theBopEnt)
   {
   WitSubEntry * theSub;

   if (not hasSubsToNet_ (theBomEnt))
      return;

   if (sglSrc_)
      if (theBomEnt->singleSource ())
         return;

   if (theBomEnt->propRtg ()[execPer])
      return;

   netConsEntry (theBomEnt, execPer, netExecQty, fillPer, theBopEnt);
      //
      // First net possible usage of prime from supply.

   if (netExecQty < NET_TOL)
      return;

   forEachEl (theSub, theBomEnt->mySubEntries ())
      {
      netSubEntry (theSub, execPer, netExecQty, fillPer, theBopEnt);

      if (netExecQty < NET_TOL)
         return;
      }
   }

//------------------------------------------------------------------------------

void WitHeurAtor::netSubEntry (
      WitSubEntry * theSub,
      WitPeriod     execPer,
      double &      netExecQty,
      WitPeriod     fillPer,
      WitBopEntry * theBopEnt)
   {
   double oldExecQty;
   double deltaVol;
   double subVolVal;

   if (not theSub->netAllowed ())
      return;

   if (not theSub->inEffect (execPer))
      return;

   oldExecQty = netExecQty;

   netConsEntry (theSub, execPer, netExecQty, fillPer, theBopEnt);

   if (permCommActive_)
      {
      deltaVol = positivePart (oldExecQty - netExecQty);

      if (deltaVol > NET_TOL)
         {
         saveSubVol (theSub, execPer);

         subVolVal = theSub->subVol ()[execPer] + deltaVol;

         theSub->storeHeurSubVol (execPer, subVolVal);

         if (perfPegging_)
            myPegger_->pegDeltaVol (theSub, execPer, deltaVol);
         }
      }
   }

//------------------------------------------------------------------------------

void WitHeurAtor::netConsEntry (
      WitConsEntry * theConsEnt,
      WitPeriod      execPer,
      double &       netExecQty,
      WitPeriod      fillPer,
      WitBopEntry *  theBopEnt)
   {
   double    reqConsQty;
   WitPart * consPart;
   WitPeriod consPer;
   double    netConsQty;

   witAssert (netExecQty > 0.0);

   reqConsQty = theConsEnt->effConsRate ()[execPer] * netExecQty;
   consPart   = theConsEnt->myPart ();
   consPer    = theConsEnt->impactPeriod ()[execPer];

   if (theConsEnt->effConsRate ()[execPer] >= FLOAT_EPSILON)
      {
      netConsQty = reqConsQty;

      myAvailSched_->netWoSR (consPart, consPer, netConsQty);

      netExecQty = netConsQty / theConsEnt->effConsRate ()[execPer];

      if (netExecQty <= NET_TOL)
         netExecQty = 0.0;
      else if (penExec_)
         myPenExMgr ()->recExpBeyondNet (consPart, consPer, true);

      if (multiSel ())
         if (tempCommActive_)
            if (reqConsQty - netConsQty > NET_TOL)
               mySelector ()->placeReqs (
                  theBopEnt,
                  theConsEnt,
                  execPer,
                  fillPer,
                  true);
      }
   else
      {
      if (theConsEnt->effConsRate ()[execPer] <= - FLOAT_EPSILON )
         myAvailSched_->addToIncAvailVol (consPart, consPer, - reqConsQty);

      netExecQty = 0.0;
      }
   }

//------------------------------------------------------------------------------

void WitHeurAtor::explodeBomEntryPRSS (
      WitBomEntry * theBomEnt,
      WitPeriod     execPer,
      double        deltaExecVol,
      WitPeriod     fillPer,
      WitBopEntry * theBopEnt)
   {
   WitRtCandDblStack theExpPairs (myProblem ());
   double            unbddVol;
   WitRtCand *       theRtCand;
   double            propDeltaVol;

   myPrMgr ()->
      defineSplit (theBomEnt, execPer, deltaExecVol, theExpPairs, unbddVol);

   myPrMgr ()->mySelMgrForCons ()->setBoundedSplit (true);

   while (theExpPairs.pop (theRtCand, propDeltaVol))
      explodeConsEntry (
         theRtCand->myConsEnt (),
         execPer, 
         propDeltaVol,
         fillPer, 
         theBopEnt);

   myPrMgr ()->mySelMgrForCons ()->setBoundedSplit (false);

   if (unbddVol <= NET_TOL)
      return;

   explodeBomEntryPR (theBomEnt, execPer, unbddVol, fillPer, theBopEnt);
   }

//------------------------------------------------------------------------------

void WitHeurAtor::explodeBomEntryPR (
      WitBomEntry * theBomEnt,
      WitPeriod     execPer,
      double        deltaExecVol,
      WitPeriod     fillPer,
      WitBopEntry * theBopEnt)
   {
   WitRtCandDblStack theExpPairs (myProblem ());
   WitRtCand *       theRtCand;
   double            propDeltaVol;

   if (selSplit_)
      myPrMgr ()->recordFlowSS (theBomEnt, execPer, deltaExecVol);

   myPrMgr ()->definePropExp (theBomEnt, execPer, deltaExecVol, theExpPairs);
   
   while (theExpPairs.pop (theRtCand, propDeltaVol))
      explodeConsEntry (
         theRtCand->myConsEnt (),
         execPer, 
         propDeltaVol,
         fillPer, 
         theBopEnt);
   }

//------------------------------------------------------------------------------

void WitHeurAtor::explodeConsEntry (
      WitConsEntry * theConsEnt,
      WitPeriod      execPer,
      double         netExecQty,
      WitPeriod      fillPer,
      WitBopEntry *  theBopEnt)
   {
   WitPeriod consPer;
   double    consumedQty;
   WitPart * consPart;

   witAssert (netExecQty > 0.0);

   if (sglSrc_)
      mySglSrcMgr ()->recExp (theConsEnt);

   consPer     = theConsEnt->impactPeriod ()[execPer];
   consumedQty = theConsEnt->effConsRate ()[execPer] * netExecQty;
   consPart    = theConsEnt->myPart ();

   if (consumedQty > NET_TOL)
      {
      myReqSched_->addTo (consPart, consPer, consumedQty);

      if (multiSel ())
         if (tempCommActive_)
            mySelector ()->placeReqs (
               theBopEnt,
               theConsEnt,
               execPer,
               fillPer,
               false);
      }

   if (consumedQty < - NET_TOL)
      myAvailSched_->addToIncAvailVol (consPart, consPer, - consumedQty);

   explodeSub (theConsEnt, execPer, netExecQty);

   if (selSplit_)
      if (multiRoute_)
         myMrMgr ()->recordFlowSS (theConsEnt, execPer, netExecQty);
   }

//------------------------------------------------------------------------------

void WitHeurAtor::explodeSub (
      WitConsEntry * theConsEnt,
      WitPeriod      execPer,
      double         netExecQty)
   {
   WitSubEntry * theSub;
   double        subVolVal;

   theSub = theConsEnt->thisSub ();

   if (theSub == NULL)
      return;

   if (not permCommActive_)
      return;

   saveSubVol (theSub, execPer);

   subVolVal = theSub->subVol ()[execPer] + netExecQty;

   theSub->storeHeurSubVol (execPer, subVolVal);

   if (perfPegging_)
      myPegger_->pegDeltaVol (theSub, execPer, netExecQty);
   }

//------------------------------------------------------------------------------

WitBopEntry * WitHeurAtor::expBopEntry (WitPart * thePart, WitPeriod expPer)
   {
   return
      multiRoute_? 
         myMrMgr ()->selBopEnt (thePart, expPer):
         thePart->expBopEntry (expPer);
   }

//------------------------------------------------------------------------------

WitPeriod WitHeurAtor::execPerFor (WitBopEntry * theBopEnt, WitPeriod expPer)
   {
   return
      multiExec_?
         myMeMgr ()->selExecPer (theBopEnt, expPer):
         theBopEnt ->expExecPeriod ()[expPer];
   }

//------------------------------------------------------------------------------

void WitHeurAtor::shutDown ()
   {
   if (myGlobalComp ()->modHeurAlloc ())
      myHeurModifier ()->checkFeasibility ();

   if (devMode ())
      {
      myMsgFac ()    ("nMaxCommitsMsg", nMaxCommits_);
      
      if (multiSel ())
         myMsgFac () ("nSelCommitsMsg", nSelCommits_);

      myMsgFac () ("nCommsMsg",
         nBsearches_,
         nTempComms_,
         nPermComms_,
         nComms_);
      }

   if (perfPegging_)
      myPegger_->shutDownPeggedHeurAlloc ();

   delete tempCommRepos_;
   delete mySelector_;
   delete myReqSched_;
   delete myHeurModifier_;
   delete myPclBldr_;
   delete myHeurCritList_;
   delete myAvailSched_;

   myMsgFac () ("heurAllInactiveMsg");
   }
