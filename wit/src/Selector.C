//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "Selector.C"
//
// Contains the implementation of class Selector.
//------------------------------------------------------------------------------

#include <Selector.h>
#include <SelPt.h>
#include <AltPt.h>
#include <ReqPt.h>
#include <ReqPtMgr.h>
#include <RtMgr.h>
#include <SglSrcMgr.h>
#include <MrMgr.h>
#include <MrCoord.h>
#include <PrCoord.h>
#include <PenExMgr.h>
#include <BaMgr.h>
#include <MeMgr.h>
#include <SsrMgr.h>
#include <PrMgr.h>
#include <Splitter.h>
#include <HeurAtor.h>
#include <Mapping.h>
#include <FixedPer.h>
#include <GlobalComp.h>
#include <Material.h>
#include <BopEntry.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class Selector.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Inlines.
//------------------------------------------------------------------------------

inline WitReqPt * WitSelector::myReqPt (WitPart * thePart, WitPeriod thePer)
   {
   return myReqPtMgr_->myReqPt (thePart, thePer);
   }

//------------------------------------------------------------------------------
// Non-inlines.
//------------------------------------------------------------------------------

WitSelector::WitSelector (WitHeurAtor * theHeurAtor):

      WitProbAssoc       (theHeurAtor),
      myHeurAtor_        (theHeurAtor),
      myRtMgr_           (NULL),
      myMrMgr_           (NULL),
      myPrMgr_           (NULL),
      myPenExMgr_        (NULL),
      mySglSrcMgr_       (NULL),
      myReqPtMgr_        (NULL),
      mySplitter_        (NULL),
      myBaMgr_           (NULL),
      myMeMgr_           (NULL),
      mySsrMgr_          (NULL),
      mySelMgrs_         (),
      pendingAlts_       (myProblem ()),
      altCounts_         (),
      curResAlt_         (false),
      failResAlt_        (false),
      lastConsResAltPer_ (* new WitMapping <WitMaterial, WitPeriod>),
      nonNegLCRAPMats_   (myProblem ()),
      multiRoute_        (false),
      penExec_           (false),
      tbprMode_          (false),
      nAlts_             (0),
      myFixedPer_        (myProblem ())
   {
   WitPeriod thePer;

   if (devMode ())
      myMsgFac () ("multiSelMsg");

   lastConsResAltPer_.allocate (myProblem (), -1);

   if (  WitMrMgr::instNeededFor (myProblem ()) or
         WitPrMgr::instNeededFor (myProblem ()))
      myRtMgr_ = new WitRtMgr (this);

   buildSelMgrs ();

   collectSelMgrs ();

   myReqPtMgr_ = new WitReqPtMgr (myProblem ());

   if (myGlobalComp ()->multiRoute ())
      if (myGlobalComp ()->penExec ())
         if (WitPrMgr::instNeededFor (myProblem ()))
            if (myGlobalComp ()->tieBreakPropRt ())
               tbprMode_ = true;

   if (multiRoute_)
      penExec_ = myGlobalComp ()->penExec ();

   if (penExec_)
      myPenExMgr_ = new WitPenExMgr (this);

   if (selSplitNeeded ())
      mySplitter_ = new WitSplitter (this);

   if (multiRoute_)
      if (WitSglSrcMgr::sglSrcReq (myProblem ()))
         mySglSrcMgr_ = new WitSglSrcMgr (this);

   if (myGlobalComp ()->selectionRecovery ())
      myMsgFac () ("selectionRecoveryMsg");

   if (selPrintLevelAt (3))
      printInit ();

   forEachPeriod (thePer, myProblem ())
      myFixedPer_[thePer] = new WitFixedPer (thePer);
   }

//------------------------------------------------------------------------------

WitSelector::~WitSelector ()
   {
   WitPeriod thePer;

   if (selPrintLevelAt (1))
      if (not selSplit ())
         countAlts ();

   delete & lastConsResAltPer_;
   delete   mySplitter_;
   delete   myReqPtMgr_;
   delete   mySglSrcMgr_;

   if (penExec_)
      delete myPenExMgr_;

   mySelMgrs_.clear ();

   delete mySsrMgr_;
   delete myMeMgr_;
   delete myBaMgr_;
   delete myPrMgr_;
   delete myMrMgr_;
   delete myRtMgr_;

   forEachPeriod (thePer, myProblem ())
      delete myFixedPer_[thePer];
   }

//------------------------------------------------------------------------------

WitPeriod WitSelector::modelessExpPer (WitPart * thePart, WitPeriod fillPer)
   {
   return
      buildAhead ()?
         myBaMgr ()->selExpPer (thePart, fillPer):
         thePart->explosionPeriod       (fillPer);
   }

//------------------------------------------------------------------------------

WitPeriod WitSelector::modelessExecPer (
      WitBopEntry * theBopEnt, 
      WitPeriod     expPer)
   {
   return
      multiExec ()?
         myMeMgr ()->selExecPer   (theBopEnt, expPer):
         theBopEnt->expExecPeriod ()         [expPer];
   }

//------------------------------------------------------------------------------

bool WitSelector::instNeededFor (WitHeurAtor * theHeurAtor)
   {
   WitProblem * theProblem;

   theProblem = theHeurAtor->myProblem ();

   if (WitPrMgr::instNeededFor (theProblem))
      return true;

   return
          WitMrMgr::instNeededFor (theProblem)
      or  WitBaMgr::instNeededFor (theProblem)
      or  WitMeMgr::instNeededFor (theProblem)
      or WitSsrMgr::instNeededFor (theHeurAtor);
   }

//------------------------------------------------------------------------------

void WitSelector::commitMaxQty (double & netQty)
   {
   bool restShortOnly;

   if (selSplit ())
      {
      mySplitter ()->commitMaxQty (netQty);

      return;
      }

   while (true)
      {
      prepCmqSel ();

      myHeurAtor_->commitMaxQtySel (netQty);

      if (penExec_)
         {
         restShortOnly = myPenExMgr ()->failRestShortOnly ();

         myPenExMgr ()->updatePsSup ();

         if (restShortOnly)
            continue;
         }

      alterSelections ();

      if (not failResAlt_)
         return;
      }
   }

//------------------------------------------------------------------------------

void WitSelector::printCommitMaxQtySel ()
   {
   fprintf (msgFile (),
      "\n"
      "Commit Max Qty Selected: Part %s, Period %d\n",
      myHeurAtor_->topPart ()->partName ().myCstring (),
      myHeurAtor_->topPer ());
   }

//------------------------------------------------------------------------------

void WitSelector::preCommitSS ()
   {
   mySplitter ()->preCommit ();
   }

//------------------------------------------------------------------------------

void WitSelector::postCommit ()
   {
   WitMaterial * theMat;

   while (nonNegLCRAPMats_.pop (theMat))
      lastConsResAltPer_ (theMat) = -1;
   }

//------------------------------------------------------------------------------

void WitSelector::postTempCommit (bool success)
   {
   if (not success)
      failResAlt_ = curResAlt_;

   curResAlt_ = false;

   myReqPtMgr_->reset ();

   if (selectiveSR ())
      mySsrMgr ()->postTempCommit ();

   if (penExec_)
      myPenExMgr ()->postTempCommit (success);

   if (selSplit ())
      mySplitter ()->postTempCommit (success);
   }

//------------------------------------------------------------------------------

void WitSelector::alterSelections ()
   {
   WitAltPt * theAltPt;

   stronglyAssert (not selSplit ());

   if (pendingAlts_.isEmpty ())
      return;

   if (selPrintLevelAt (1))
      printPreAlt ();

   while (getPendingAlt (theAltPt))
      alterSelection (theAltPt);
   }

//------------------------------------------------------------------------------

void WitSelector::alterSelSS ()
   {
   WitAltPt * theAltPt;

   stronglyAssert (selSplit ());

   if (getPendingAlt (theAltPt))
      {
      witAssert (pendingAlts_.isEmpty ());

      alterSelection (theAltPt);
      }
   }

//------------------------------------------------------------------------------

void WitSelector::alterSelection (WitAltPt * theAltPt)
   {
   theAltPt->alterSelection ();

   if (selPrintLevelAt (1))
      nAlts_ ++;

   if (selPrintLevelAt (2))
      theAltPt->printAlteration ();

   if (myGlobalComp ()->selectionRecovery ())
      theAltPt->storeRecoveryPt ();
   }

//------------------------------------------------------------------------------

void WitSelector::recFillByCons (WitPart * thePart, WitPeriod fillPer)
   {
   WitMaterial * theMat;

   theMat = thePart->thisMat ();

   if (theMat == NULL)
      return;

   stronglyAssert (fillPer > lastConsResAltPer_ (theMat));

   if (not myReqPt (thePart, fillPer)->hasResAlt ())
      return;

   if (lastConsResAltPer_ (theMat) == -1)
      nonNegLCRAPMats_.push (theMat);

   lastConsResAltPer_ (theMat) = fillPer;
   }

//------------------------------------------------------------------------------

void WitSelector::recFillNetRec (WitPart * thePart, WitPeriod fillPer)
   {
   WitMaterial * theMat;
   WitReqPt *    fillReqPt;
   WitPeriod     srcPer;
   WitPeriod     stockPer;

   theMat = thePart->thisMat ();

   if (theMat == NULL)
      return;

   if (fillPer == 0)
      return;

   srcPer = lastConsResAltPer_ (theMat);

   if (srcPer < 0)
      return;

   fillReqPt = myReqPt (thePart, fillPer);

   if (fillReqPt->hasResAlt ())
      return;

   stronglyAssert (srcPer < fillPer);

   if (theMat->mandEC () != false)
      for (stockPer = srcPer; stockPer < fillPer; stockPer ++)
         if (theMat->mandEC ()[stockPer])
            return;

   fillReqPt->setSource (myReqPt (thePart, srcPer));
   }

//------------------------------------------------------------------------------

void WitSelector::rollReq (
      WitMaterial * theMat,
      WitPeriod     fromPer,
      WitPeriod     toPer,
      double        rollVol)
   {
   WitReqPt * toReqPt;

   if (penExec_)
      myPenExMgr ()->placeReqs (theMat, fromPer, theMat, toPer);

   toReqPt = myReqPt (theMat, toPer);

   if (not toReqPt->hasResAlt ())
      if (myHeurAtor_->selectiveSR (theMat))
         mySsrMgr ()->recordSR (theMat, toPer, rollVol);
   }

//------------------------------------------------------------------------------

void WitSelector::placeReqs (
      WitBopEntry *  theBopEnt,
      WitConsEntry * theConsEnt,
      WitPeriod      execPer,
      WitPeriod      fillPer,
      bool           forNetting)
   {  
   WitPart *  consPart;
   WitPeriod  consPer;
   WitReqPt * theReqPt;

   consPart = theConsEnt->myPart ();
   consPer  = theConsEnt->impactPeriod ()[execPer];

   if (penExec_)
      myPenExMgr ()->placeReqs (
         theBopEnt->myPart (),
         fillPer,
         consPart,
         consPer);

   theReqPt = myReqPt (consPart, consPer);

   innerPlaceReqs (
      theBopEnt,
      theConsEnt,
      execPer,
      fillPer, 
      theReqPt,
      forNetting);
   }

//------------------------------------------------------------------------------

void WitSelector::recExecBoundShortage (
      WitBopEntry * theBopEnt,
      WitPeriod     execPer,
      WitPeriod     fillPer)
   {
   WitPart *   prodPart;
   WitReqPt *  theReqPt;
   WitSelMgr * theSelMgr;
   WitAltPt *  theAltPt;

   prodPart = theBopEnt->myPart ();

   theReqPt = myReqPt (prodPart, fillPer);

   forEachEl (theSelMgr, mySelMgrs_)
      {
      if (not theSelMgr->validForExecBounds ())
         continue;

      theAltPt = theSelMgr->myAltPtFor (theBopEnt, NULL, execPer, fillPer);

      if (theAltPt == NULL)
         continue;

      if (selSplit ())
         if (not theAltPt->hasResAlt ())
            continue;

      if (sglSrc ())
         if (theAltPt->mySelPt ()->sglSrcMode ())
            continue;

      theReqPt->putDirectAlt (theAltPt);
      }

   recFundShortage (prodPart, fillPer);
   }

//------------------------------------------------------------------------------

void WitSelector::recLTBoundShortage (WitPart * thePart, WitPeriod fillPer)
   {
   WitReqPt *  theReqPt;
   WitSelMgr * theSelMgr;
   WitAltPt *  theAltPt;

   theReqPt = myReqPt (thePart, fillPer);

   forEachEl (theSelMgr, mySelMgrs_)
      {
      if (not theSelMgr->validForLTBounds ())
         continue;

      theAltPt = theSelMgr->myAltPtFor (thePart, fillPer);

      if (theAltPt == NULL)
         continue;

      if (selSplit ())
         if (not theAltPt->hasResAlt ())
            continue;

      if (sglSrc ())
         if (theAltPt->mySelPt ()->sglSrcMode ())
            continue;

      theReqPt->putDirectAlt (theAltPt);
      }

   recFundShortage (thePart, fillPer);
   }

//------------------------------------------------------------------------------

void WitSelector::recFundShortage (WitPart * thePart, WitPeriod fillPer)
   {
   WitReqPt * theReqPt;
   WitAltPt * theAltPt;

   if (penExec_)
      myPenExMgr ()->recordFundShortage ();

   recShortage ();

   theReqPt   = myReqPt (thePart, fillPer);

   curResAlt_ = theReqPt->hasResAlt ();

   while (theReqPt != NULL)
      {
      while (theReqPt->getDirectAlt (theAltPt))
         if (not theAltPt->pending ())
            {  
            pendingAlts_.push (theAltPt);

            theAltPt->setPending (true);
            }

      theReqPt = theReqPt->source ();
      }

   if (pendingAlts_.isEmpty ())
      stronglyAssert (not curResAlt_);
   }

//------------------------------------------------------------------------------

void WitSelector::recShortage ()
   {
   WitAltPt * theAltPt;

   while (getPendingAlt (theAltPt));

   curResAlt_ = false;
   }

//------------------------------------------------------------------------------

void WitSelector::recCommitVol (double commitVol)
   {
   myPenExMgr ()->recCommitVol (commitVol);
   }

//------------------------------------------------------------------------------

void WitSelector::prepCmqSel ()
   {
   failResAlt_ = false;

   if (penExec_)
      myPenExMgr ()->modifyRouting ();
   }

//------------------------------------------------------------------------------

void WitSelector::recordFlowSS (WitSelPt * theSelPt, double incFlowVol)
   {
   if (myHeurAtor_->tempCommActive ())
      mySplitter ()->recordFlow (theSelPt, incFlowVol);
   }

//------------------------------------------------------------------------------

bool WitSelector::splitCommActive ()
   {
   return mySplitter ()->splitCommActive ();
   }

//------------------------------------------------------------------------------

void WitSelector::recoverInitState ()
   {
   witAssert (myGlobalComp ()->selectionRecovery ());

   if (selPrintLevelAt (3))
      fprintf (msgFile (),
         "\n"
         "Recovering the initial selection configuration:\n\n");

   if (multiRoute_)
      myMrMgr  ()->recoverInitState ();

   if (buildAhead ())
      myBaMgr  ()->recoverInitState ();

   if (multiExec ())
      myMeMgr  ()->recoverInitState ();

   if (selectiveSR ())
      mySsrMgr ()->recoverInitState ();

   if (propRtg ())
      myPrMgr  ()->recoverInitState ();
   }

//------------------------------------------------------------------------------

void WitSelector::buildSelMgrs ()
   {
   if (WitMrMgr::instNeededFor (myProblem ()))
      {
      myMrMgr_    = new WitMrMgr (this);

      multiRoute_ = true;
      }

   if (WitBaMgr::instNeededFor (myProblem ()))
      myBaMgr_ = new WitBaMgr (this);

   if (WitMeMgr::instNeededFor (myProblem ()))
      myMeMgr_ = new WitMeMgr (this);

   if (WitSsrMgr::instNeededFor (myHeurAtor_))
      mySsrMgr_ = new WitSsrMgr (this);

   if (WitPrMgr::instNeededFor (myProblem ()))
      myPrMgr_ = new WitPrMgr (this);
   }

//------------------------------------------------------------------------------

void WitSelector::collectSelMgrs ()
   {
   collect (mySsrMgr_);
   collect (myBaMgr_);

   if (multiRoute_)
      collect (myMrMgr ()->mySelMgrForProd ());

   if (propRtg ())
      collect (myPrMgr ()->mySelMgrForProd ());

   collect (myMeMgr_);

   if (multiRoute_)
      collect (myMrMgr ()->mySelMgrForCons ());

   if (propRtg ())
      collect (myPrMgr ()->mySelMgrForCons ());
   }

//------------------------------------------------------------------------------

void WitSelector::collect (WitSelMgr * theSelMgr)
   {
   if (theSelMgr == NULL)
      return;

   theSelMgr->setIndex ();

   mySelMgrs_.append (theSelMgr);
   }

//------------------------------------------------------------------------------

bool WitSelector::selSplitNeeded ()
   {
   WitSelMgr * theSelMgr;

   forEachEl (theSelMgr, mySelMgrs_)
      if (theSelMgr->selSplitNeeded ())
         return true;

   return false;
   }

//------------------------------------------------------------------------------

void WitSelector::innerPlaceReqs (
      WitBopEntry *  theBopEnt,
      WitConsEntry * theConsEnt,
      WitPeriod      execPer,
      WitPeriod      fillPer,
      WitReqPt *     theReqPt,
      bool           forNetting)
   {
   bool        priorAlt;
   WitSelMgr * lbSelMgr;
   WitSelMgr * theSelMgr;
   WitReqPt *  theSource;
   WitAltPt *  theAltPt;

   priorAlt = theReqPt->hasAlt ();

   if (selSplit ())
      lbSelMgr = mySplitter ()->lowestBddSelMgr ();

   if ((not selSplit ()) or (lbSelMgr == NULL))
      {
      theSource = myReqPt (theBopEnt->myPart (), fillPer);

      if (theSource->hasResAlt ())
         priorAlt = false;

      if (not priorAlt)
         theReqPt->setSource (theSource);
      }

   forEachEl (theSelMgr, mySelMgrs_)
      {
      if (selSplit ())
         if (lbSelMgr != NULL)
            if (not theSelMgr->isLowerThan (lbSelMgr))
               continue;

      if (forNetting)
         if (not theSelMgr->validForNetting ())
            continue;

      theAltPt = 
         theSelMgr->myAltPtFor (theBopEnt, theConsEnt, execPer, fillPer);

      if (theAltPt == NULL)
         continue;

      if (selSplit ())
         if (not theAltPt->hasResAlt ())
            continue;

      if (sglSrc ())
         if (theAltPt->mySelPt ()->sglSrcMode ())
            continue;

      if (theAltPt->hasResAlt ())
         priorAlt = false;

      if (not priorAlt)
         theReqPt->putDirectAlt (theAltPt);
      }

   getResAltFromNet (theConsEnt->myBomEnt (), execPer, theReqPt);
   }

//------------------------------------------------------------------------------

void WitSelector::getResAltFromNet (
      WitBomEntry * theBomEnt,
      WitPeriod     execPer,
      WitReqPt *    theReqPt)
   {
   WitSubEntry * theSub;
   bool          resFound;

   if (theReqPt->hasResAlt ())
      return;

   if (not theBomEnt->hasSubsInEffect ()[execPer])
      return;

   resFound = copyResAlt (theBomEnt, execPer, theReqPt);

   if (resFound)
      return;

   forEachEl (theSub, theBomEnt->mySubEntries ())
      if (theSub->netAllowed ())
         if (theSub->inEffect (execPer))
            {
            resFound = copyResAlt (theSub, execPer, theReqPt);

            if (resFound)
               return;
            }
   }

//------------------------------------------------------------------------------

bool WitSelector::copyResAlt (
      WitConsEntry * theConsEnt,
      WitPeriod      execPer,
      WitReqPt *     theReqPt)
   {
   WitPart *  consPart;
   WitPeriod  consPer;
   WitReqPt * consReqPt;

   if (theConsEnt->effConsRate ()[execPer] < FLOAT_EPSILON)
      return false;

   consPart  = theConsEnt->myPart ();
   consPer   = theConsEnt->impactPeriod ()[execPer];

   consReqPt = myReqPt (consPart, consPer);

   if (not consReqPt->hasResAlt ())
      return false;

   theReqPt->copyStateOf (consReqPt);

   return true;
   }

//------------------------------------------------------------------------------

bool WitSelector::getPendingAlt (WitAltPt * & theAltPt)
   {
   if (pendingAlts_.pop (theAltPt))
      {
      theAltPt->setPending (false);

      return true;
      }
   else
      return false;
   }

//------------------------------------------------------------------------------

void WitSelector::printPreAlt ()
   {
   if (pendingAlts_.isEmpty ())
      return;

   if (selPrintLevelAt (2))
      fprintf (msgFile (),
         "\n"
         "------------------------------------\n"
         "Altering the Selection Configuration\n"
         "------------------------------------\n\n"
         "Alteration potentially resolves shortage? %s\n",
         myMsgFac ().boolText (failResAlt_));

   altCounts_.append (new int (pendingAlts_.nElements ()));
   }

//------------------------------------------------------------------------------

void WitSelector::printInit ()
   {
   if (multiRoute_)
      myMrMgr    ()->print ();

   if (buildAhead ())
      myBaMgr    ()->print ();
   
   if (multiExec ())
      myMeMgr    ()->print ();

   if (propRtg ())
      myPrMgr    ()->print ();

   if (penExec_)
      myPenExMgr ()->print ();
   }

//------------------------------------------------------------------------------

void WitSelector::countAlts ()
   {
   int    maxAltCount;
   int *  altCountPtr;
   int    altCount;
   int    totAlts;
   int    totBatches;
   double altsPerBatch;

   WitVector <int> nBatches;

   maxAltCount = 0;

   forEachEl (altCountPtr, altCounts_)
      setToMax (maxAltCount, * altCountPtr);

   nBatches.resize (maxAltCount + 1, 0);
   
   totAlts    = 0;
   totBatches = 0;

   while (not altCounts_.isEmpty ())
      {
      altCountPtr = altCounts_.get ();

      nBatches[* altCountPtr] ++;

      totAlts    += * altCountPtr;
      totBatches ++;

      delete altCountPtr;
      }
   
   fprintf (msgFile (), "\n");

   for (altCount = 0; altCount <= maxAltCount; altCount ++)
      if (nBatches[altCount] > 0)
         fprintf (msgFile (),
            "# Alts: %3d  # Batches: %7d\n",
            altCount,
            nBatches[altCount]);

   altsPerBatch = 
      (totBatches > 0)?
         static_cast <double> (totAlts) / static_cast <double> (totBatches):
         0;

   fprintf (msgFile (),
      "\n"
      "# Alterations:    %7d\n"
      "# Batches:        %7d\n"
      "# Alts Per Batch: %10.2f\n\n",
      totAlts,
      totBatches,
      altsPerBatch);
   }
