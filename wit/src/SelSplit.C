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
// Source file: "SelSplit.C"
//
// Contains the implementation of the following classes:
//
//    Splitter
//    SplitPt
//    SelCand
//    FlowMonitor
//    SplitCntr
//------------------------------------------------------------------------------

#include <SplitCntr.h>
#include <SplitPt.h>
#include <FlowMon.h>
#include <Splitter.h>
#include <Selector.h>
#include <SelMgr.h>
#include <SelPt.h>
#include <AltPt.h>
#include <SelCand.h>
#include <RtCand.h>
#include <PenExMgr.h>
#include <MrMgr.h>
#include <HeurAtor.h>
#include <BopEntry.h>
#include <Part.h>
#include <GlobalComp.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class Splitter.
//------------------------------------------------------------------------------

WitSplitter::WitSplitter (WitSelector * theSelector):

      WitProbAssoc     (theSelector),
      mySelector_      (theSelector),
      myFlowMonitor_   (NULL),
      mySplitCntr_     (NULL),
      myPenExMgr_      (NULL),
      penExec_         (false),
      splitRes_        (myGlobalComp ()->splitRes ()),
      zeroPts_         (myProblem ()),
      splitCommActive_ (false),
      mySplitPts_      (myProblem ())
   {
   WitDemand * theDemand;
   WitSelMgr * theSelMgr;

   myMsgFac () ("selSplitMsg");

   if (myGlobalComp ()->computeCriticalList ())
      myMsgFac () ("critListSelSplitSmsg");

   if (myGlobalComp ()->pgdCritListMode ())
      myMsgFac () ("pgdCritListSelSplitSmsg");

   forEachDemand (theDemand, myProblem ())
      if (theDemand->searchInc () != 1.0)
         myMsgFac () ("searchIncSelSplitSmsg",
            theDemand->demandedPartName (),
            theDemand->demandName (),
            theDemand->searchInc ());

   forEachEl (theSelMgr, mySelector_->mySelMgrs ())
      theSelMgr->setUpSelSplit ();

   myFlowMonitor_ = new WitFlowMonitor (mySelector_->myHeurAtor ());

   mySplitCntr_   = new WitSplitCntr (mySelector_);

   penExec_       = mySelector_->penExec ();

   if (penExec_)
      myPenExMgr_ = theSelector->myPenExMgr ();
   }

//------------------------------------------------------------------------------

WitSplitter::~WitSplitter ()
   {
   delete mySplitCntr_;
   delete myFlowMonitor_;
   }

//------------------------------------------------------------------------------

void WitSplitter::commitMaxQty (double & netQty)
   {
   bool scNeeded;
   bool unresFound;

   while (true)
      {
      mySelector_->prepCmqSel ();

      myFlowMonitor_->activate ();

      mySelector_->myHeurAtor ()->commitMaxQtySel (netQty);

      if ((netQty <= 0.0) or not resAltOrRest ())
         {
         myFlowMonitor_->deactivate ();

         break;
         }

      scNeeded = splitCommitNeeded ();
      
      myFlowMonitor_->deactivate ();

      if (scNeeded)
         if (not mySelector_->myHeurAtor ()->failedOn1 ())
            if (mySelector_->myHeurAtor ()->tempCommit (1.0))
               scNeeded = false;

      if (scNeeded)
         {
         splitCommit (unresFound);

         if (unresFound)
            break;
         }
      else
         {
         if (penExec_)
            myPenExMgr ()->updatePsSup ();

         mySelector_->alterSelSS ();
         }
      }

   if (penExec_)
      myPenExMgr ()->updatePsSup ();
   }

//------------------------------------------------------------------------------

void WitSplitter::preCommit ()
   {
   WitObjStItr <WitSplitPt> theItr;
   WitSplitPt *             theSplitPt;

   mySplitPts_.attachItr (theItr);

   while (theItr.advance (theSplitPt))
      theSplitPt->initNetSplitList ();
   }

//------------------------------------------------------------------------------

void WitSplitter::postTempCommit (bool success)
   {
   myFlowMonitor_->postTempCommit (success);
   }

//------------------------------------------------------------------------------

WitSelMgr * WitSplitter::lowestBddSelMgr ()
   {
   WitSelMgr * lbSelMgr;
   WitSelMgr * theSelMgr;

   lbSelMgr = NULL;

   forEachEl (theSelMgr, mySelector_->mySelMgrs ())
      if (theSelMgr->boundedSplit ())
         lbSelMgr = theSelMgr;

   return lbSelMgr;
   }

//------------------------------------------------------------------------------

void WitSplitter::recordFlow (WitSelPt * theSelPt, double incFlowVol)
   {
   myFlowMonitor_->recordFlow (theSelPt, incFlowVol);
   }

//------------------------------------------------------------------------------

bool WitSplitter::splitCommitNeeded ()
   {
   WitSelPt * theSelPt;
   double     flowGap;

   WitObjStack <WitSelPt> chgSelPts (myProblem ());

   findHypoChanges (chgSelPts);

   while (chgSelPts.pop (theSelPt))
      if (myFlowMonitor_->splitIsNeeded (theSelPt, true, flowGap))
         return true;

   return false;
   }

//------------------------------------------------------------------------------

void WitSplitter::splitCommit (bool & unresFound)
   {
   int         nItrs;
   bool        tcOK;
   bool        splitFound;
   WitSelMgr * theSelMgr;

   splitCommActive_ = true;
   unresFound       = false;
   nItrs            = 0;
   tcOK             = false;

   mySplitCntr_->initCurSplits ();

   while (not tcOK)
      {
      nItrs ++;

      if (nItrs > myGlobalComp ()->splitItrUB ())
         break;

      buildZeroPts (unresFound, splitFound);

      if (unresFound)
         break;

      if (splitFound)
         splitSels ();

      clearSplitItr ();

      stronglyAssert (resAltOrRest ());

      mySelector_->alterSelSS ();

      if (penExec_)
         {
         myPenExMgr ()->updatePsSup   ();
         myPenExMgr ()->modifyRouting ();
         }

      tcOK = mySelector_->myHeurAtor ()->tempCommit (1.0);
      }

   if (tcOK)
      mySelector_->myHeurAtor ()->permCommit1SS ();

   if (selPrintLevelAt (1))
      mySplitCntr_->countSplits (tcOK);

   forEachEl (theSelMgr, mySelector_->mySelMgrs ())
      theSelMgr->postSplitCommit ();

   deleteContents (mySplitPts_);

   splitCommActive_ = false;
   }

//------------------------------------------------------------------------------

void WitSplitter::buildZeroPts (bool & unresFound, bool & splitFound)
   {
   bool addedZeroPts;

   stronglyAssert (zeroPts_.isEmpty ());

   unresFound = false;
   splitFound = false;

   while (not splitFound)
      {
      unresFound = not resAltOrRest ();

      if (unresFound)
         {
         clearSplitItr ();

         return;
         }

      addedZeroPts = addChgPtsToZeroPts ();

      if (not addedZeroPts)
         return;

      splitFound = tempCommDiscZeroPts ();
      }
   }

//------------------------------------------------------------------------------

void WitSplitter::clearSplitItr ()
   {
   WitObjStack <WitSplitPt> revSplitPts (myProblem ());
   WitSplitPt *             theSplitPt;

   zeroPts_.clear ();

   revSplitPts.revCopyFrom (mySplitPts_);

   mySplitPts_.clear ();

   while (revSplitPts.pop (theSplitPt))
      if (theSplitPt->hasSplits ())
         mySplitPts_.push (theSplitPt);
      else
         delete theSplitPt;
   }

//------------------------------------------------------------------------------

bool WitSplitter::addChgPtsToZeroPts ()
   {
   bool       addedZeroPts;
   WitSelPt * theSelPt;

   WitObjStack <WitSelPt> chgSelPts (myProblem ());

   addedZeroPts = false;

   findHypoChanges (chgSelPts);

   while (chgSelPts.pop (theSelPt))
      {
      if (not theSelPt->isSplit ())
         mySplitPts_.push (new WitSplitPt (this, theSelPt));

      else if (zeroPts_.contains (theSelPt->mySplitPt ()))
         continue;

      zeroPts_.push (theSelPt->mySplitPt ());

      addedZeroPts = true;
      }

   return addedZeroPts;
   }

//------------------------------------------------------------------------------

void WitSplitter::splitSels ()
   {
   bool         success;
   WitSplitPt * searchPt;
   bool         splitNeeded;
   double       flowGap;

   myFlowMonitor_->activate ();

   success = true;

   while (success)
      {
      stronglyAssert (zeroPts_.isNonEmpty ());

      searchPt = findLowestZeroPt ();

      zeroPts_.remove (searchPt);

      success = tempCommDiscZeroPts ();
      }

   splitNeeded =
      myFlowMonitor_->splitIsNeeded (searchPt->mySelPt (), false, flowGap);

   if (splitNeeded)
      searchForFlow (searchPt, flowGap);

   if (resAltOrRest ())
      splitChgPts ();
   else
      {
      success = mySelector_->myHeurAtor ()->tempCommit (1.0);

      stronglyAssert (not success);
      }

   myFlowMonitor_->deactivate ();
   }

//------------------------------------------------------------------------------

bool WitSplitter::resAltOrRest ()
   {
   if (mySelector_->failResAlt ())
      return true;

   else if (penExec_)
      return myPenExMgr ()->failRestShortOnly ();

   else
      return false;
   }

//------------------------------------------------------------------------------

WitSplitPt * WitSplitter::findLowestZeroPt ()
   {
   WitSplitPt * lowZeroPt;
   WitSplitPt * theZeroPt;

   WitObjStItr <WitSplitPt> theItr;

   stronglyAssert (zeroPts_.isNonEmpty ());

   zeroPts_.attachItr (theItr);

   theItr.advance (lowZeroPt);

   while (theItr.advance (theZeroPt))
      if (theZeroPt->isLowerThan (lowZeroPt))
         lowZeroPt = theZeroPt;

   return lowZeroPt;
   }

//------------------------------------------------------------------------------

void WitSplitter::searchForFlow (WitSplitPt * searchPt, double theFlowVol)
   {
   bool success;
   int  upperGP;
   int  lowerGP;
   int  midGP;

   success = tempCommForSearch (searchPt, splitRes_);

   mySplitCntr_->countBS (success);

   if (not success)
      return;

   lowerGP = 1;
   upperGP = static_cast <int> (ceil (theFlowVol / splitRes_));

   stronglyAssert (upperGP >= 2);

   while (upperGP - lowerGP > 1)
      {
      midGP   = (lowerGP + upperGP) / 2;

      success = tempCommForSearch (searchPt, midGP * splitRes_);

      if (success)
         lowerGP = midGP;
      else
         upperGP = midGP;
      }
   }

//------------------------------------------------------------------------------

bool WitSplitter::tempCommForSearch (WitSplitPt * searchPt, double searchVol)
   {
   bool success;

   searchPt->setNetSearchVol (searchVol);

   success = tempCommDiscZeroPts ();

   searchPt->clearNetSearchVol ();

   return success;
   }

//------------------------------------------------------------------------------

bool WitSplitter::tempCommDiscZeroPts ()
   {
   WitObjStItr <WitSplitPt> theItr;
   WitSplitPt *             theZeroPt;
   bool                     success;

   zeroPts_.attachItr (theItr);

   while (theItr.advance (theZeroPt))
      theZeroPt->setNetSearchVol (0.0);

   success = mySelector_->myHeurAtor ()->tempCommit (1.0);

   zeroPts_.attachItr (theItr);

   while (theItr.advance (theZeroPt))
      theZeroPt->clearNetSearchVol ();

   return success;
   }

//------------------------------------------------------------------------------

void WitSplitter::splitChgPts ()
   {
   WitSplitPt * theSplitPt;

   WitObjStack <WitSplitPt> chgSplitPts (myProblem ());
   WitObjStItr <WitSplitPt> theItr;

   mySplitCntr_->countBatch ();

   findChgSplitPts (chgSplitPts);

   chgSplitPts.attachItr (theItr);

   while (theItr.advance (theSplitPt))
      theSplitPt->activateTotFlowVol ();

   myFlowMonitor_->setTotFlowVols ();

   chgSplitPts.attachItr (theItr);

   while (theItr.advance (theSplitPt))
      {
      theSplitPt->splitSelAsNeeded ();

      theSplitPt->deactivateTotFlowVol ();
      }
   }

//------------------------------------------------------------------------------

void WitSplitter::findChgSplitPts (WitObjStack <WitSplitPt> & chgSplitPts)
   {
   WitSelPt * theSelPt;

   WitObjStack <WitSelPt> chgSelPts (myProblem ());

   findHypoChanges (chgSelPts);

   while (chgSelPts.pop (theSelPt))
      {
      if (not theSelPt->isSplit ())
         mySplitPts_.push (new WitSplitPt (this, theSelPt));

      chgSplitPts.push (theSelPt->mySplitPt ());
      }
   }

//------------------------------------------------------------------------------

void WitSplitter::findHypoChanges (WitObjStack <WitSelPt> & chgSelPts)
   {
   WitSelPt * theSelPt;

   if (penExec_)
      findHypoChangesPE (chgSelPts);
   else
      {
      chgSelPts.clear ();

      if (mySelector_->pendingAlts ().isNonEmpty ())
         {
         theSelPt = mySelector_->pendingAlts ().firstObject ()->mySelPt ();

         chgSelPts.push (theSelPt);
         }
      }
   }

//------------------------------------------------------------------------------

void WitSplitter::findHypoChangesPE (WitObjStack <WitSelPt> & chgSelPts)
   {
   WitAltPt *             theAltPt;
   WitObjStack <WitSelPt> theModPts (myProblem ());
   WitSelPt *             altSelPt;
   WitSelPt *             modSelPt;

   stronglyAssert (penExec_);

   chgSelPts.clear ();

   if (mySelector_->pendingAlts ().isNonEmpty ())
      {
      theAltPt = mySelector_->pendingAlts ().firstObject ();

      theAltPt->tempAlterSel ();
      }
   else
      theAltPt = NULL;

   myPenExMgr ()->findHypoModPts (theModPts);

   if (theAltPt != NULL)
      {
      theAltPt->cancelTempAlt ();

      altSelPt = theAltPt->mySelPt ();

      chgSelPts.push (altSelPt);
      }
   else
      altSelPt = NULL;

   while (theModPts.pop (modSelPt))
      if (modSelPt != altSelPt)
         chgSelPts.push (modSelPt);
   }

//------------------------------------------------------------------------------
// Implementation of class SplitPt.
//------------------------------------------------------------------------------

WitSplitPt::WitSplitPt (WitSplitter * theSplitter, WitSelPt * theSelPt):

      WitProbAssoc  (theSplitter),
      mySplitter_   (theSplitter),
      mySelPt_      (theSelPt),
      netSearchVol_ (-1.0),
      revSplitList_ (myProblem ()),
      netSplitList_ (myProblem ()),
      totFlowVol_   (-1.0)
   {
   mySelPt_->attachSplitPt (this);
   }

//------------------------------------------------------------------------------

WitSplitPt::~WitSplitPt ()
   {
   WitSelCand * theSelCand;
   double       splitBound;

   while (revSplitList_.pop (theSelCand, splitBound))
      {
      delete theSelCand;

      theSelCand = NULL;
      }

   mySelPt_->detachSplitPt ();
   }

//------------------------------------------------------------------------------

bool WitSplitPt::isLowerThan (WitSplitPt * theSplitPt)
   {
   WitPart *   thePart1;
   WitPeriod   thePer1;
   WitSelMgr * theSelMgr1;

   WitPart *   thePart2;
   WitPeriod   thePer2;
   WitSelMgr * theSelMgr2;

   witAssert (theSplitPt != NULL);

               mySelPt_->getSortData (thePart1, thePer1);
   theSplitPt->mySelPt_->getSortData (thePart2, thePer2);

   if (thePart1->sortedPartIndex () < thePart2->sortedPartIndex ())
      return true;

   if (thePart1->sortedPartIndex () > thePart2->sortedPartIndex ())
      return false;

   theSelMgr1 =             mySelPt_->mySelMgr ();
   theSelMgr2 = theSplitPt->mySelPt_->mySelMgr ();

   if (theSelMgr1->isLowerThan (theSelMgr2))
      return true;

   if (theSelMgr2->isLowerThan (theSelMgr1))
      return false;

   if (thePer1 > thePer2)
      return true;

   if (thePer1 < thePer2)
      return false;

   return false;
   }

//------------------------------------------------------------------------------

void WitSplitPt::setNetSearchVol (double theVal)
   {
   witAssert (theVal        >= 0.0);
   witAssert (netSearchVol_ < -0.5);

   netSearchVol_ = theVal;
   }

//------------------------------------------------------------------------------

void WitSplitPt::clearNetSearchVol ()
   {
   witAssert (netSearchVol_ >= 0.0);

   netSearchVol_ = -1.0;
   }

//------------------------------------------------------------------------------

void WitSplitPt::initNetSplitList ()
   {
   netSplitList_.revCopyFrom (revSplitList_);
   }

//------------------------------------------------------------------------------

void WitSplitPt::splitSelAsNeeded ()
   {
   WitSelCand * theSelCand;

   if (totFlowVol_ <= NET_TOL)
      return;

   theSelCand = mySelPt_->newSelCand ();

   revSplitList_.push (theSelCand, totFlowVol_);

   mySplitter_->mySplitCntr ()->countCurSplits ();

   if (selPrintLevelAt (3))
      prtSplitting (theSelCand, totFlowVol_);
   }

//------------------------------------------------------------------------------

void WitSplitPt::defineSplit (
      double               totFlowVol,
      WitSelCandDblStack & theSplitPairs,
      double &             unbddVol)
   {
   double netFlowVol;

   witAssert (totFlowVol > 0.0);

   netFlowVol = totFlowVol;

   unbddVol = 0.0;

   theSplitPairs.clear ();

   while (netSplitList_.isNonEmpty ())
      {
      addNextPair (netFlowVol, theSplitPairs);

      if (netFlowVol == 0.0)
         return;
      }

   unbddVol = compUnbddVol (netFlowVol);
   }

//------------------------------------------------------------------------------

void WitSplitPt::activateTotFlowVol ()
   {
   stronglyAssert (totFlowVol_ < -0.5);

   totFlowVol_ = 0.0;
   }

//------------------------------------------------------------------------------

void WitSplitPt::incTotFlowVol (double incFlowVol)
   {
   if (totFlowVol_ > -0.5)
       totFlowVol_ += incFlowVol;
   }

//------------------------------------------------------------------------------

void WitSplitPt::deactivateTotFlowVol ()
   {
   stronglyAssert (totFlowVol_ >= 0.0);

   totFlowVol_ = -1.0;
   }

//------------------------------------------------------------------------------

bool WitSplitPt::hasSplits ()
   {
   return (revSplitList_.isNonEmpty ());
   }

//------------------------------------------------------------------------------

void WitSplitPt::prtSplitting (WitSelCand * theSelCand, double splitBound)
   {
   fprintf (msgFile (),
      "\n"
      "Splitting the %s Selection:\n",
      mySelPt_->mySelMgr ()->selCaseName ());

   mySelPt_->prtID ();

   theSelCand->prtID ();
               
   fprintf (msgFile (), "   Split Bound:      %.3f\n", splitBound);
   }

//------------------------------------------------------------------------------

void WitSplitPt::addNextPair (
      double &             netFlowVol,
      WitSelCandDblStack & theSplitPairs)
   {
   WitSelCand * theSelCand;
   double       splitBound;
   double       splitVol;

   witAssert (netSplitList_.isNonEmpty ());

   netSplitList_.pop (theSelCand, splitBound);

   splitVol = min (splitBound, netFlowVol);
   
   if (selPrintLevelAt (3))
      prtSplitExp (theSelCand, splitVol, splitBound, true);

   netFlowVol -= splitVol;
   splitBound -= splitVol;

   if (netFlowVol <= NET_TOL)
      netFlowVol = 0.0;
   
   if (splitBound > NET_TOL)
      netSplitList_.push (theSelCand, splitBound);

   theSplitPairs.push (theSelCand, splitVol);
   }

//------------------------------------------------------------------------------

double WitSplitPt::compUnbddVol (double netFlowVol)
   {
   double unbddVol;

   witAssert (netFlowVol > 0.0);

   unbddVol = netFlowVol;

   if (netSearchVol_ >= 0.0)
      {
      if (netSearchVol_ < unbddVol)
         {
         unbddVol      = netSearchVol_;

         netSearchVol_ = 0.0;

         if (unbddVol <= NET_TOL)
            return 0.0;
         }
      else
         netSearchVol_ -= unbddVol;
      }

   if (selPrintLevelAt (3))
      prtUnbddSplitExp (unbddVol);

   return unbddVol;
   }

//------------------------------------------------------------------------------

void WitSplitPt::prtUnbddSplitExp (double unbddVol)
   {
   WitSelCand * theSelCand;

   theSelCand = mySelPt_->newSelCand ();

   if (selPrintLevelAt (3))
      prtSplitExp (theSelCand, unbddVol, 0.0, false);

   delete theSelCand;
   }

//------------------------------------------------------------------------------

void WitSplitPt::prtSplitExp (
      WitSelCand * theSelCand, 
      double       splitVol, 
      double       splitBound,
      bool         boundedSplit)
   {
   WitSelMgr * theSelMgr;

   if (not mySplitter_->mySelector ()->myHeurAtor ()->permCommActive ())
      return;

   theSelMgr = mySelPt_->mySelMgr ();

   fprintf (msgFile (),
      "\n"
      "Split Selection for %s:\n",
      theSelMgr->selCaseName ());

   mySelPt_->prtID ();

   theSelCand->prtID ();
               
   fprintf    (msgFile (), "   SplitVol:         %.3f\n", splitVol);
               
   if (boundedSplit)
      fprintf (msgFile (), "   Split Bound:      %.3f\n", splitBound);
   else
      fprintf (msgFile (), "   Split Bound:      None\n");
   }

//------------------------------------------------------------------------------
// Implementation of class SelCand.
//------------------------------------------------------------------------------

WitSelCand::~WitSelCand ()
   {
   }

//------------------------------------------------------------------------------

void WitSelCand::getMrData (WitRtCand * &)
   {
   stronglyAssert (false);
   }

//------------------------------------------------------------------------------

void WitSelCand::getBaData (WitPeriod &)
   {
   stronglyAssert (false);
   }

//------------------------------------------------------------------------------

void WitSelCand::getMeData (WitPeriod &)
   {
   stronglyAssert (false);
   }

//------------------------------------------------------------------------------

void WitSelCand::getSsrData (bool &)
   {
   stronglyAssert (false);
   }

//------------------------------------------------------------------------------

void WitSelCand::getPoData (bool &, bool &)
   {
   stronglyAssert (false);
   }

//------------------------------------------------------------------------------

void WitSelCand::getPrData (WitRtCandStack & theRtCands)
   {
   stronglyAssert (false);
   }

//------------------------------------------------------------------------------

WitSelCand::WitSelCand (WitProblem * theProblem):
      WitProbAssoc (theProblem)
   {
   }

//------------------------------------------------------------------------------
// Implementation of class FlowMonitor.
//------------------------------------------------------------------------------

WitFlowMonitor::WitFlowMonitor (WitHeurAtor * theHeurAtor):

      WitProbAssoc   (theHeurAtor),
      myHeurAtor_    (theHeurAtor),
      active_        (false),
       curFlowPairs_ (myProblem ()),
        okFlowPairs_ (myProblem ()),
      failFlowPairs_ (myProblem ())
   {
   }

//------------------------------------------------------------------------------

WitFlowMonitor::~WitFlowMonitor ()
   {
   }

//------------------------------------------------------------------------------

void WitFlowMonitor::activate ()
   {
   stronglyAssert (not active_);

   active_ = true;
   }

//------------------------------------------------------------------------------

void WitFlowMonitor::deactivate ()
   {
   stronglyAssert (active_);

    curFlowPairs_.clear ();
     okFlowPairs_.clear ();
   failFlowPairs_.clear ();

   active_ = false;
   }

//------------------------------------------------------------------------------

void WitFlowMonitor::recordFlow (WitSelPt * theSelPt, double incFlowVol)
   {
   witAssert (myHeurAtor_->tempCommActive ());

   if (not active_)
      return;

   if (not theSelPt->splittable ())
      return;

   if (theSelPt->mySelMgr ()->boundedSplit ())
      return;

   curFlowPairs_.push (theSelPt, incFlowVol);
   }

//------------------------------------------------------------------------------

void WitFlowMonitor::postTempCommit (bool success)
   {
   if (not active_)
      return;

   if (success)
      {
      okFlowPairs_.clear ();

      okFlowPairs_.takeContentsFrom (curFlowPairs_);
      }
   else
      {
      failFlowPairs_.clear ();

      failFlowPairs_.takeContentsFrom (curFlowPairs_);
      }
   }

//------------------------------------------------------------------------------

bool WitFlowMonitor::splitIsNeeded (
      WitSelPt * theSelPt, 
      bool       compToSuccess,
      double &   flowGap)
   {
   stronglyAssert (active_);

   if (not theSelPt->splittable ())
      return false;

   flowGap = flowThruSelPt (theSelPt, failFlowPairs_);

   if (compToSuccess)
      flowGap -= flowThruSelPt (theSelPt, okFlowPairs_);

   return (flowGap > myGlobalComp ()->splitTol () + SEARCH_TOL);
   }

//------------------------------------------------------------------------------

void WitFlowMonitor::setTotFlowVols ()
   {
   WitSelPt * theSelPt;
   double     incFlowVol;

   WitPairStItr <WitSelPt, double> theItr;

   stronglyAssert (active_);

   okFlowPairs_.attachItr (theItr);

   while (theItr.advance (theSelPt, incFlowVol))
      if (theSelPt->isSplit ())
         theSelPt->mySplitPt ()->incTotFlowVol (incFlowVol);
   }

//------------------------------------------------------------------------------

double WitFlowMonitor::flowThruSelPt (
      WitSelPt *               theSelPt,
      const WitSelPtDblStack & theFlowPairs)
   {
   double     flowVol;
   WitSelPt * curSelPt;
   double     incFlowVol;

   WitPairStItr <WitSelPt, double> theItr;

   stronglyAssert (theSelPt != NULL);

   flowVol = 0.0;

   theFlowPairs.attachItr (theItr);

   while (theItr.advance (curSelPt, incFlowVol))
      if (curSelPt == theSelPt)
         flowVol += incFlowVol;

   return flowVol;
   }

//------------------------------------------------------------------------------
// Implementation of class SplitCntr.
//------------------------------------------------------------------------------

WitSplitCntr::WitSplitCntr (WitSelector * theSelector):

      WitProbAssoc       (theSelector),
      mySelector_        (theSelector),
      nCurSplits_        (0),
      nSplitsDone_       (0),
      nSplitsDiscarded_  (0),
      nSplitSplitComs_   (0),
      nUnsplitSplitComs_ (0),
      nBSsDone_          (0),
      nBSsSkipped_       (0),
      nBatches_          (0)
   {
   }

//------------------------------------------------------------------------------

WitSplitCntr::~WitSplitCntr ()
   {
   if (selPrintLevelAt (1))
      print ();
   }

//------------------------------------------------------------------------------

void WitSplitCntr::initCurSplits ()
   {
   if (selPrintLevelAt (1))
      nCurSplits_ = 0;
   }

//------------------------------------------------------------------------------

void WitSplitCntr::countCurSplits ()
   {
   if (selPrintLevelAt (1))
      nCurSplits_ ++;
   }

//------------------------------------------------------------------------------

void WitSplitCntr::countSplits (bool tempCommOK)
   {
   if (tempCommOK)
      {
      nSplitsDone_          += nCurSplits_;

      if (nCurSplits_ > 0)
         nSplitSplitComs_   ++;
      else
         nUnsplitSplitComs_ ++;
      }
   else
      {
      nSplitsDiscarded_     += nCurSplits_;

      nUnsplitSplitComs_    ++;
      }
   }

//------------------------------------------------------------------------------

void WitSplitCntr::countBS (bool bsDone)
   {
   if (bsDone)
      nBSsDone_ ++;
   else
      nBSsSkipped_ ++;
   }

//------------------------------------------------------------------------------

void WitSplitCntr::countBatch ()
   {
   nBatches_ ++;
   }

//------------------------------------------------------------------------------

void WitSplitCntr::print ()
   {
   fprintf (msgFile (), 
      "# SS BS's Done:         %7d\n"
      "# SS BS's Skipped:      %7d\n"
      "# Split Batches:        %7d\n"
      "# Splits Performed:     %7d\n"
      "# Splits Discarded:     %7d\n"
      "# Alts, Total:          %7d\n\n"
      "#   Split Split-Coms:   %7d\n"
      "# Unsplit Split-Coms:   %7d\n\n\n",

      nBSsDone_,
      nBSsSkipped_,
      nBatches_,
      nSplitsDone_,
      nSplitsDiscarded_,
      mySelector_->nAlts (),
      nSplitSplitComs_,
      nUnsplitSplitComs_);
   }
