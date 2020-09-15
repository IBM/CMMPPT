//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "RtAn.C"
//
// Contains the implementation of class RtAnalyzer.
//------------------------------------------------------------------------------

#include <RtAn.h>
#include <ExpRest.h>
#include <MrMgr.h>
#include <PrMgr.h>
#include <RtMgr.h>
#include <RtSite.h>
#include <RtCand.h>
#include <Selector.h>
#include <HeurAtor.h>
#include <BopEntry.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class RtAnalyzer.
//------------------------------------------------------------------------------

WitRtAnalyzer::WitRtAnalyzer (
         WitSelector * theSelector,
         WitExpRest *  theExpRest):

      WitProbAssoc    (theSelector),
      mySelector_     (theSelector),
      myExpRest_      (theExpRest),
      myMrMgr_        (theSelector->myMrMgr ()),
      myPrMgr_        (NULL),
      myRtMgr_        (theSelector->myRtMgr ()),
      curMrMods_      (myProblem ()),
      curPrActMods_   (myProblem ()),
      curPrDeactMods_ (myProblem ()),
      toPreAn_        (myProblem ()),
      dependents_     (myProblem ()),
      nPrereqsToAna_  (myProblem (), 0),
      toAnalyze_      (myProblem ()),
      minPen_         (myProblem (), 0.0),
      minPenValid_    (),
      printMode_      (selPrintLevelAt (1)),
      curPen_         (0.0),
      totCommitVol_   (0.0),
      weightedTotPen_ (0.0),
      inTheModPts_    ()
   {
   WitPart *   thePart;
   WitPeriod   thePer;
   WitRtCand * theRtCand;
   WitRtSite * theRtSite;

   if (mySelector_->propRtg ())
      myPrMgr_ = mySelector_->myPrMgr ();

   checkInput ();

   forEachPart (thePart, myProblem ())
      forEachPeriod (thePer, myProblem ())
         dependents_.myPtrAt (thePart, thePer) = 
            new WitPartPerStack (myProblem ());

   if (DEBUG_MODE)
      minPenValid_.allocate (myProblem (), false);

   if (mySelector_->propRtg ())
      {
      if (myGlobalComp ()->tieBreakPropRt ())
         myMsgFac () ("tieBreakPropRtMsg");
      else
         myMsgFac () ("highPriPropRtgMsg");
      }
   }

//------------------------------------------------------------------------------

WitRtAnalyzer::~WitRtAnalyzer ()
   {
   WitPart * thePart;
   WitPeriod thePer;
   double    aveExecPen;

   forEachPart (thePart, myProblem ())
      forEachPeriod (thePer, myProblem ())
         delete dependents_.myPtrAt (thePart, thePer);

   aveExecPen =
      (totCommitVol_ > FLOAT_EPSILON)?
         weightedTotPen_ / totCommitVol_:
         0.0;

   myMsgFac () ("aveExecPenMsg", aveExecPen);
   }

//------------------------------------------------------------------------------

void WitRtAnalyzer::modifyRouting ()
   {
   WitRtCand * theRtCand;
   WitPeriod   thePer;
   bool        actStat;

   findRoutingMods ();

   if (selPrintLevelAt (3))
      printMrMods ();

   while (curMrMods_.pop (theRtCand, thePer))
      {
      myMrMgr_->select (theRtCand, thePer);

      if (myGlobalComp ()->selectionRecovery ())
         myMrMgr_->storeRecoveryPair (theRtCand->myRtSite (), thePer);
      }

   if (mySelector_->tbprMode ())
      {
      if (selPrintLevelAt (3))
         printLpprMods ();

      while (curPrActMods_.pop (theRtCand, thePer))
         myPrMgr_->setActive (theRtCand, thePer, true);

      while (curPrDeactMods_.pop (theRtCand, thePer))
         {
         myPrMgr_->setActive (theRtCand, thePer, false);

         if (myGlobalComp ()->selectionRecovery ())
            myPrMgr_->storeRecoveryPair (theRtCand, thePer);
         }
      }
   }

//------------------------------------------------------------------------------

void WitRtAnalyzer::findHypoModPts (WitObjStack <WitSelPt> & theModPts)
   {
   WitRtCand * theRtCand;
   WitPeriod   thePer;
   WitSelPt *  theSelPt;
   WitRtSite * theRtSite;

   witAssert (mySelector_->selSplit ());

   myExpRest_->startTempMode ();

   myExpRest_->updatePsSup ();

   findRoutingMods ();

   myExpRest_->finishTempMode ();

   theModPts.clear ();

   while (curMrMods_.pop (theRtCand, thePer))
      {
      theRtSite = theRtCand->myRtSite ();

      theSelPt  = myMrMgr_->mySelPtFor (theRtSite, thePer);

      theModPts.push (theSelPt);
      }

   if (mySelector_->tbprMode ())
      addHypoPrModPts (theModPts);
   }

//------------------------------------------------------------------------------

void WitRtAnalyzer::recCommitVol (double commitVol)
   {
   totCommitVol_   += commitVol;

   weightedTotPen_ += commitVol * curPen_;
   }

//------------------------------------------------------------------------------

void WitRtAnalyzer::checkInput ()
   {
   WitBomEntry * theBomEnt;
   WitSubEntry * theSub;
   bool          hasPenalty;

   forEachBomEntry (theBomEnt, myProblem ())
      {
      hasPenalty = (theBomEnt->execPenalty () > 0.0);

      if (not hasPenalty)
         forEachEl (theSub, theBomEnt->mySubEntries ())
            if (theSub->execPenalty () > 0.0)
               {
               hasPenalty = true;

               break;
               }

      if (not hasPenalty)
         continue;
      
      forEachEl (theSub, theBomEnt->mySubEntries ())
         if (theSub->netAllowed ())
            myMsgFac () ("netExecPenaltySmsg",
               theBomEnt->myOperationName (),
               theBomEnt->localIndex ());
      }
   }

//------------------------------------------------------------------------------

void WitRtAnalyzer::findRoutingMods ()
   {
   WitPart *       thePart;
   WitPeriod       fillPer;
   WitPartPerStack validPartPers (myProblem ());
      //
      // The set of pairs (thePart, thePer) for which
      // minPenValid_ (thePart)[thePer] is true.

   witAssert (inInitState ());

   if (selPrintLevelAt (3))
      printAnaHead ();

   toPreAn_.push (
      mySelector_->myHeurAtor ()->topPart (),
      mySelector_->myHeurAtor ()->topPer ());

   while (toPreAn_.pop (thePart, fillPer))
      preAnalyze (thePart, fillPer);

   while (toAnalyze_.pop (thePart, fillPer))
      {
      minPen_ (thePart)[fillPer] = analyze (thePart, fillPer);

      if (DEBUG_MODE)
         {
         minPenValid_ (thePart)[fillPer] = true;

         validPartPers.push (thePart, fillPer);
         }

      postAnalyze (thePart, fillPer);
      }

   curPen_ = 
      minPen_ (mySelector_->myHeurAtor ()->topPart ())
              [mySelector_->myHeurAtor ()->topPer ()];

   if (DEBUG_MODE)
      while (validPartPers.pop (thePart, fillPer))
         minPenValid_ (thePart)[fillPer] = false;
   }

//------------------------------------------------------------------------------

bool WitRtAnalyzer::inInitState ()
   {
   WitPart * thePart;
   WitPeriod thePer;

   if (curMrMods_.isNonEmpty ())
      return false;

   if (mySelector_->tbprMode ())
      {
      if (curPrActMods_.isNonEmpty ())
         return false;

      if (curPrDeactMods_.isNonEmpty ())
         return false;
      }

   if (toPreAn_.isNonEmpty ())
      return false;

   if (toAnalyze_.isNonEmpty ())
      return false;

   forEachPart (thePart, myProblem ())
      forEachPeriod (thePer, myProblem ())
         {
         if (nPrereqsToAna_ (thePart)[thePer] != 0)
            return false;

         if (dependents_.myElemAt (thePart, thePer).isNonEmpty ())
            return false;
         }

   return true;
   }

//------------------------------------------------------------------------------

void WitRtAnalyzer::preAnalyze (WitPart * fillPart, WitPeriod fillPer)
   {
   WitPeriod     expPer;
   WitBopEntry * theBopEnt;
   WitRtCand *   theRtCand;
   WitPeriod     execPer;
   WitBomEntry * theBomEnt;

   if (selPrintLevelAt (3))
      printPreAn (fillPart, fillPer);

   if (needToAnalyze (fillPart, fillPer, expPer))
      forEachEl (theBopEnt, fillPart->producingBopEntries ())
         {
         theRtCand = myRtMgr_->myRtCandFor (theBopEnt);

         if (not activeForPreAn (theRtCand, expPer))
            continue;

         execPer = mySelector_->modelessExecPer (theBopEnt, expPer);

         forEachEl (theBomEnt, theBopEnt->myOperation ()->bom ())
            recBomEntPrereq (fillPart, fillPer, theBomEnt, execPer);
         }

   if (nPrereqsToAna_ (fillPart)[fillPer] == 0)
      toAnalyze_.push (fillPart, fillPer);

   if (selPrintLevelAt (3))
      printGap ();
   }

//------------------------------------------------------------------------------

void WitRtAnalyzer::recBomEntPrereq (
      WitPart *     fillPart, 
      WitPeriod     fillPer,
      WitBomEntry * theBomEnt,
      WitPeriod     execPer)
   {
   WitSubEntry * theSub;

   if (not theBomEnt->inEffect (execPer))
      return;

   if (theBomEnt->hasSubsInEffect ()[execPer])
      {
      if (canPseudoNet (theBomEnt, execPer))
         return;

      recConsEntPrereq (fillPart, fillPer, theBomEnt, execPer);

      forEachEl (theSub, theBomEnt->mySubEntries ())
         recConsEntPrereq (fillPart, fillPer, theSub, execPer);
      }
   else
      recConsEntPrereq (fillPart, fillPer, theBomEnt, execPer);
   }

//------------------------------------------------------------------------------

void WitRtAnalyzer::recConsEntPrereq (
      WitPart *      fillPart, 
      WitPeriod      fillPer,
      WitConsEntry * theConsEnt,
      WitPeriod      execPer)
   {
   WitRtCand * theRtCand;

   theRtCand = myRtMgr_->myRtCandFor (theConsEnt);

   if (activeForPreAn (theRtCand, execPer))
      if (theConsEnt->effConsRate ()[execPer] > 0.0)
         recordPrereq (
            fillPart, 
            fillPer, 
            theConsEnt->myPart (),
            theConsEnt->impactPeriod ()[execPer]);
   }

//------------------------------------------------------------------------------

void WitRtAnalyzer::recordPrereq (
      WitPart * depPart, 
      WitPeriod depPer,
      WitPart * prereqPart, 
      WitPeriod prereqPer)
   {
   if (dependents_.myElemAt (prereqPart, prereqPer).isEmpty ())
      toPreAn_.push (prereqPart, prereqPer);

   dependents_.myElemAt (prereqPart, prereqPer).push (depPart, depPer);

   nPrereqsToAna_ (depPart)[depPer] ++;

   if (selPrintLevelAt (3))
      printPrereq (prereqPart, prereqPer);
   }

//------------------------------------------------------------------------------

bool WitRtAnalyzer::activeForPreAn (WitRtCand * theRtCand, WitPeriod thePer)
   {
   if (not theRtCand->myRtSite ()->propRtg ()[thePer])
      return myMrMgr_->activeForPE (theRtCand, thePer);

   else
      return myPrMgr_->allowed     (theRtCand, thePer);
   }

//------------------------------------------------------------------------------

double WitRtAnalyzer::analyze (WitPart * fillPart, WitPeriod fillPer)
   {
   WitPeriod   expPer;
   WitRtSite * theRtSite;

   if (not needToAnalyze (fillPart, fillPer, expPer))
      return 0.0;

   theRtSite = myRtMgr_->myRtSiteFor (fillPart);

   if (not fillPart->propRtg ()[expPer])
      return penThruRtSiteMR   (theRtSite, expPer);

   else if (not myGlobalComp ()->tieBreakPropRt ())
      return penThruRtSiteOPR  (theRtSite, expPer);

   else
      return penThruRtSiteTBPR (theRtSite, expPer);
   }

//------------------------------------------------------------------------------

bool WitRtAnalyzer::needToAnalyze (
      WitPart *   fillPart, 
      WitPeriod   fillPer, 
      WitPeriod & expPer)
   {
   if (fillPart->producingBopEntries ().isEmpty ())
      return false;

   if (myExpRest_->hasPseudoSup (fillPart, fillPer))
      return false;

   expPer = mySelector_->modelessExpPer (fillPart, fillPer);

   if (not fillPart->explodeable (expPer))
      return false;

   if (fillPart->boundedLeadTimes ())
      if (expPer < mySelector_->myHeurAtor ()->ltbMinPer ())
         return false;

   return true;
   }  

//------------------------------------------------------------------------------

double WitRtAnalyzer::penThruBopEnt (WitBopEntry * theBopEnt, WitPeriod expPer)
   {
   WitOperation * theOpn;
   WitPeriod      execPer;
   double         penThruOpn;
   WitBomEntry *  theBomEnt;

   theOpn     = theBopEnt  ->myOperation ();
   execPer    = mySelector_->modelessExecPer (theBopEnt, expPer);
   penThruOpn = theOpn     ->execPenalty ();

   forEachEl (theBomEnt, theOpn->bom ())
      if (theBomEnt->inEffect (execPer))
         penThruOpn += penThruBomEnt (theBomEnt, execPer);

   return penThruOpn;
   }

//------------------------------------------------------------------------------

double WitRtAnalyzer::penThruBomEnt (WitBomEntry * theBomEnt, WitPeriod execPer)
   {
   WitRtSite * theRtSite;

   if (not theBomEnt->hasSubsInEffect ()[execPer])
      return    penThruConsEnt    (theBomEnt, execPer);

   if (not theBomEnt->propRtg ()[execPer])
      return    penThruBomEntMR   (theBomEnt, execPer);
   else
      {
      theRtSite = myRtMgr_->myRtSiteFor (theBomEnt);

      if (not myGlobalComp ()->tieBreakPropRt ())
         return penThruRtSiteOPR  (theRtSite, execPer);
      else
         return penThruRtSiteTBPR (theRtSite, execPer);
      }
   }

//------------------------------------------------------------------------------

double WitRtAnalyzer::penThruBomEntMR (
      WitBomEntry * theBomEnt,
      WitPeriod     execPer)
   {
   WitRtSite * theRtSite;

   if (canPseudoNet (theBomEnt, execPer))
      return 0.0;

   theRtSite = myRtMgr_->myRtSiteFor (theBomEnt);

   return penThruRtSiteMR (theRtSite, execPer);
   }

//------------------------------------------------------------------------------

double WitRtAnalyzer::penThruRtSiteMR (WitRtSite * theRtSite, WitPeriod thePer)
   {
   WitRtCand * minRtCand;
   double      minPen;
   WitRtCand * theRtCand;
   double      thePen;

   minRtCand = NULL;
   minPen    = 0.0;

   forEachEl (theRtCand, theRtSite->myRtCands ())
      if (myMrMgr_->activeForPE (theRtCand, thePer))
         {
         thePen = penThruRtCand (theRtCand, thePer);

         if ((minRtCand == NULL) or (thePen < minPen))
            {
            minRtCand = theRtCand;
            minPen    = thePen;
            }
         }

   witAssert (minRtCand != NULL);

   if (minRtCand != myMrMgr_->selRtCand (theRtSite, thePer))
      curMrMods_.push (minRtCand, thePer);

   return minPen;
   }

//------------------------------------------------------------------------------

double WitRtAnalyzer::penThruRtSiteOPR (
      WitRtSite * theRtSite,
      WitPeriod   thePer)
   {
   double            totPen;
   WitRtCandDblStack theExpPairs (myProblem ());
   WitRtCand *       theRtCand;
   double            normShare;

   totPen = 0.0;

   myPrMgr_->definePropExp (theRtSite, thePer, 1.0, theExpPairs);

   while (theExpPairs.pop (theRtCand, normShare))
      totPen += normShare * penThruRtCand (theRtCand, thePer);

   return totPen;
   }

//------------------------------------------------------------------------------

double WitRtAnalyzer::penThruRtSiteTBPR (
      WitRtSite * theRtSite,
      WitPeriod   thePer)
   {
   WitRtCand * theRtCand;
   double      thePen;
   double      minPen;
   bool        isMinimal;

   WitPairStack <WitRtCand, double> theCandPens (myProblem ());

   forEachEl (theRtCand, theRtSite->myRtCands ())
      if (myPrMgr_->allowed (theRtCand, thePer))
         {
         thePen = penThruRtCand (theRtCand, thePer);

         if (theCandPens.isEmpty ())
            minPen = thePen;

         setToMin (minPen, thePen);

         theCandPens.push (theRtCand, thePen);
         }

   witAssert (theCandPens.isNonEmpty ());

   while (theCandPens.pop (theRtCand, thePen))
      {
      isMinimal = (thePen <= minPen + FLOAT_EPSILON);

      if (isMinimal)
         if (not myPrMgr_->active (theRtCand, thePer))
            curPrActMods_.push (theRtCand, thePer);

      if (not isMinimal)
         if (myPrMgr_->active (theRtCand, thePer))
            curPrDeactMods_.push (theRtCand, thePer);
      }

   return minPen;
   }

//------------------------------------------------------------------------------

double WitRtAnalyzer::penThruRtCand (WitRtCand * theRtCand, WitPeriod thePer)
   {
   return
      theRtCand->myRtSite ()->isForProd ()?
         penThruBopEnt  (theRtCand->myBopEnt  (), thePer):
         penThruConsEnt (theRtCand->myConsEnt (), thePer);
   }

//------------------------------------------------------------------------------

double WitRtAnalyzer::penThruConsEnt (
      WitConsEntry * theConsEnt, 
      WitPeriod      execPer)
   {
   WitPart * consPart;
   WitPeriod consPer;

   if (theConsEnt->effConsRate ()[execPer] <= 0.0)
      return theConsEnt->execPenalty ();

   consPart = theConsEnt->myPart ();
   consPer  = theConsEnt->impactPeriod ()[execPer];

   witAssert (minPenValid_ (consPart)[consPer]);

   return theConsEnt->execPenalty () + minPen_ (consPart)[consPer];
   }

//------------------------------------------------------------------------------

bool WitRtAnalyzer::canPseudoNet (WitBomEntry * theBomEnt, WitPeriod execPer)
   {
   WitSubEntry * theSub;

   if (theBomEnt->propRtg ()[execPer])
      return false;

   if (not mySelector_->myHeurAtor ()->hasSubsToNet () (theBomEnt))
      return false;

   if (canDirPseudoNet (theBomEnt, execPer))
      return true;

   forEachEl (theSub, theBomEnt->mySubEntries ())
      if (theSub->inEffect (execPer))
         if (theSub->netAllowed ())
            if (canDirPseudoNet (theSub, execPer))
               return true;

   return false;
   }

//------------------------------------------------------------------------------

bool WitRtAnalyzer::canDirPseudoNet (
      WitConsEntry * theConsEnt, 
      WitPeriod      execPer)
   {
   WitPart * consPart;
   WitPeriod consPer;

   witAssert (theConsEnt->inEffect (execPer));

   consPart = theConsEnt->myPart ();
   consPer  = theConsEnt->impactPeriod ()[execPer];

   return myExpRest_->hasPseudoSup (consPart, consPer);
   }

//------------------------------------------------------------------------------

void WitRtAnalyzer::postAnalyze (WitPart * anaPart, WitPeriod anaPer)
   {
   WitPart * depPart;
   WitPeriod depPer;

   if (selPrintLevelAt (3))
      printAnalysis (anaPart, anaPer);

   while (dependents_.myElemAt (anaPart, anaPer).pop (depPart, depPer))
      {
      nPrereqsToAna_ (depPart)[depPer] --;

      if (nPrereqsToAna_ (depPart)[depPer] == 0)
         toAnalyze_.push (depPart, depPer);
      }
   }

//------------------------------------------------------------------------------

void WitRtAnalyzer::addHypoPrModPts (WitObjStack <WitSelPt> & theModPts)
   {
   WitRtCand * theRtCand;
   WitPeriod   thePer;
   WitRtSite * theRtSite;
   WitSelPt *  theSelPt;

   WitPairStack <WitRtSite, WitPeriod> thePrModPairs (myProblem ());
   WitPairStItr <WitRtSite, WitPeriod> theItr;

   witAssert (mySelector_->tbprMode ());

   if (inTheModPts_.domainSize () == 0)
      {
      inTheModPts_.allocate1D (myProblem ());

      forEachEl (theRtSite, myRtMgr_->myRtSites ())
         if (theRtSite->propRtg () != false)
            inTheModPts_ (theRtSite).allocate (myProblem (), false);
      }

   while (curPrActMods_.pop (theRtCand, thePer))
      thePrModPairs.push (theRtCand->myRtSite (), thePer);

   while (curPrDeactMods_.pop (theRtCand, thePer))
      thePrModPairs.push (theRtCand->myRtSite (), thePer);

   thePrModPairs.attachItr (theItr);

   while (theItr.advance (theRtSite, thePer))
      {
      if (inTheModPts_ (theRtSite)[thePer])
         continue;

      theSelPt = myPrMgr_->mySelPtFor (theRtSite, thePer);

      theModPts.push (theSelPt);

      inTheModPts_ (theRtSite)[thePer] = true;
      }

   while (thePrModPairs.pop (theRtSite, thePer))
      inTheModPts_ (theRtSite)[thePer] = false;
   }

//------------------------------------------------------------------------------

void WitRtAnalyzer::printAnaHead ()
   {
   if (myExpRest_->tempMode ())
      return;

   fprintf (msgFile (),
      "\n"
      "Analyzing routings for part %s in period %d.\n\n",
      mySelector_->myHeurAtor ()->topPart ()->partName ().myCstring (),
      mySelector_->myHeurAtor ()->topPer ());
   }

//------------------------------------------------------------------------------

void WitRtAnalyzer::printPreAn (WitPart * thePart, WitPeriod thePer)
   {
   if (myExpRest_->tempMode ())
      return;

   fprintf (msgFile (),
      "   Pre-analyzing Part %s in Period %d.\n",
      thePart->partName ().myCstring (),
      thePer);
   }

//------------------------------------------------------------------------------

void WitRtAnalyzer::printPrereq (WitPart * thePart, WitPeriod thePer)
   {
   if (myExpRest_->tempMode ())
      return;

   fprintf (msgFile (),
      "      Prereq:    Part %s in Period %d.\n",
      thePart->partName ().myCstring (),
      thePer);
   }

//------------------------------------------------------------------------------

void WitRtAnalyzer::printGap ()
   {
   if (myExpRest_->tempMode ())
      return;

   fprintf (msgFile (), "\n");
   }

//------------------------------------------------------------------------------

void WitRtAnalyzer::printAnalysis (WitPart * thePart, WitPeriod thePer)
   {
   if (myExpRest_->tempMode ())
      return;

   fprintf (msgFile (), 
      "   Part %s, Period %d: Min Pen: %.0f\n", 
      thePart->partName ().myCstring (),
      thePer,
      minPen_ (thePart)[thePer]);
   }

//------------------------------------------------------------------------------

void WitRtAnalyzer::printMrMods ()
   {
   WitPairStItr <WitRtCand, WitPeriod> theItr;
   WitRtCand *                         theRtCand;
   WitPeriod                           thePer;

   if (myExpRest_->tempMode ())
      return;

   fprintf (msgFile (), "\n" "Multi-Route Modifications:\n");

   if (curMrMods_.isEmpty ())
      {
      fprintf (msgFile (),
         "\n"
         "   None\n");

      return;
      }

   curMrMods_.attachItr (theItr);

   while (theItr.advance (theRtCand, thePer))
      {
      fprintf (msgFile (), "\n");

      theRtCand->myRtSite ()->printID          (4, 22);
      theRtCand->myRtSite ()->printPer (thePer, 4, 22);
      theRtCand->             printID          (7, 22);
      }
   }

//------------------------------------------------------------------------------

void WitRtAnalyzer::printLpprMods ()
   {
   if (not myExpRest_->tempMode ())
      {
      printLpprMods (curPrActMods_,   "Activations");
      printLpprMods (curPrDeactMods_, "Deactivations");
      }
   }

//------------------------------------------------------------------------------

void WitRtAnalyzer::printLpprMods (
      const WitRtCandPerStack & theRtCandPers,
      const char *              statText)
   {
   WitPairStItr <WitRtCand, WitPeriod> theItr;
   WitRtCand *                         theRtCand;
   WitPeriod                           thePer;

   witAssert (printMode_);

   fprintf (msgFile (),
      "\n"
      "Proportionate Routing %s:\n",
      statText);

   if (theRtCandPers.isEmpty ())
      {
      fprintf (msgFile (),"\n   None\n");

      return;
      }

   theRtCandPers.attachItr (theItr);

   while (theItr.advance (theRtCand, thePer))
      {
      fprintf (msgFile (),"\n");

      theRtCand->myRtSite ()->printID          (4, 22);
      theRtCand->myRtSite ()->printPer (thePer, 4, 22);
      theRtCand->             printID          (7, 22);
      }
   }
