//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "HeurImp.C"
//
// Contains the implementation of the following classes:
//
//    HeurImploder
//    HeurImpPerf
//------------------------------------------------------------------------------

#include <HeurImp.h>
#include <HeurImpP.h>
#include <EqAll.h>
#include <HeurAllMgr.h>
#include <Pre.h>
#include <GlobalComp.h>
#include <OptComp.h>
#include <Material.h>
#include <Demand.h>
#include <Timing.h>
#include <DataWrit.h>
#include <DetOptImpMgr.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class HeurImploder.
//------------------------------------------------------------------------------

WitHeurImploder::WitHeurImploder (WitProblem * theProblem):

      WitProbAssoc   (theProblem),
      myHeurImpPerf_ (NULL),
      priorityPer_   (-1)
   {
   }

//------------------------------------------------------------------------------

WitHeurImploder::~WitHeurImploder ()
   {
   }

//------------------------------------------------------------------------------

void WitHeurImploder::implode (bool forOpt)
   {
   myHeurImpPerf_ = new WitHeurImpPerf (myProblem (), forOpt);

   myHeurImpPerf_->implode ();

   delete myHeurImpPerf_;

   myHeurImpPerf_ = NULL;
   }

//------------------------------------------------------------------------------

void WitHeurImploder::genPriorities (WitSchedule <WitDemand, int> & newPriority)
   {
   int         theIdx;
   WitDemand * theDemand;

   WitPtrVec    <WitDemand> theDemVec;
   WitPtrVecItr <WitDemand> theItr;

   theDemVec.resize (myCompMgr ()->nDemands ());

   theIdx = -1;

   forEachDemand (theDemand, myProblem ())
      {
      theIdx ++;

      theDemVec[theIdx] = theDemand;
      }

   forEachPeriod (priorityPer_, myProblem ())
      {
      theDemVec.sort (compareObj);

      theDemVec.attachItr (theItr);

      while (theItr.advance (theDemand))
         newPriority (theDemand)[priorityPer_] = theItr.myIdx () + 1;
      }
   }

//------------------------------------------------------------------------------

bool WitHeurImploder::compareObj (
      WitDemand * theDemand1,
      WitDemand * theDemand2)
   {
   int           priPer;
   double        theVal1;
   double        theVal2;
   WitMaterial * theMat1;
   WitMaterial * theMat2;
   int           theIdx1;
   int           theIdx2;

   priPer  = theDemand1->myProblem ()->myHeurImploder ()->priorityPer_;

   theVal1 = theDemand1->shipReward ()[priPer];
   theVal2 = theDemand2->shipReward ()[priPer];

   if (theVal1 > theVal2)
      return true;

   if (theVal1 < theVal2)
      return false;

   theVal1 = theDemand1->cumShipReward ()[priPer];
   theVal2 = theDemand2->cumShipReward ()[priPer];

   if (theVal1 > theVal2)
      return true;

   if (theVal1 < theVal2)
      return false;

   theMat1 = theDemand1->demandedPart ()->thisMat ();
   theMat2 = theDemand2->demandedPart ()->thisMat ();

   if (theMat1 != NULL)
      if (theMat2 != NULL)
         {
         theVal1 = theMat1->stockCost ()[priPer];
         theVal2 = theMat2->stockCost ()[priPer];

         if (theVal1 < theVal2)
            return true;

         if (theVal1 > theVal2)
            return false;
         }

   theIdx1 = theDemand1->mappingIndex ();
   theIdx2 = theDemand2->mappingIndex ();

   return (theIdx1 < theIdx2);
   }

//------------------------------------------------------------------------------
// Implementation of class HeurImpPerf.
//------------------------------------------------------------------------------

WitHeurImpPerf::WitHeurImpPerf (WitProblem * theProblem, bool forOptVal):
      WitProbAssoc    (theProblem),

      forOpt_         (forOptVal),
      tempPriority_   (),
      sortedDemVec_   (),
      modPriority_    (theProblem, 0),
      curDemPer_      (-1),
      unmetDemandVol_ (theProblem, 0.0),
      selDemPers_     (theProblem)
   {
   sortedDemVec_ .resize (theProblem->myCompMgr ()->nDemands ());
   }

//------------------------------------------------------------------------------

WitHeurImpPerf::~WitHeurImpPerf ()
   {
   }

//------------------------------------------------------------------------------

void WitHeurImpPerf::implode ()
   {
   WitTimer::enterSection ("heuristic");

   myPreprocessor ()->preprocess ();

   myMsgFac () ("heuristicMsg");

   genPriorities ();

   init ();

   if (forOpt_)
      myProblem ()->myHeurAllMgr ()->startForOpt ();
   else
      myProblem ()->myHeurAllMgr ()->start ();

   myEqAllocator ()->activate ();

   mainLoop ();

   if (myGlobalComp ()->inputPauses ())
      myProblem ()->pauseForInput ("at the conclusion of heur implosion");

   myProblem ()->myHeurAllMgr ()->finish ();

   WitTimer::leaveSection ("heuristic");
   }

//------------------------------------------------------------------------------

void WitHeurImpPerf::genPriorities ()
   {
   if (forOpt_)
      {
      tempPriority_.allocate (myProblem (), 0);

      myProblem ()->myHeurImploder ()->genPriorities (tempPriority_);
      }
   else
      {
      if (myGlobalComp ()->autoPriority ())
         WitDemand::genPriorities (myProblem ());
      }
   }

//------------------------------------------------------------------------------

void WitHeurImpPerf::init ()
   {
   WitDemand * theDemand;
   WitPeriod   thePer;
   int         thePri;
   int         maxPri;
   int         theIdx;

   maxPri = 1;

   forEachDemand (theDemand, myProblem ())
       forEachPeriod (thePer, myProblem ())
          setToMax (maxPri, origPriority (theDemand, thePer));

   forEachDemand (theDemand, myProblem ())
      forEachPeriod (thePer, myProblem ())
         {
         thePri = origPriority (theDemand, thePer);

         modPriority_ (theDemand)[thePer] = 
            (thePri > 0)?
               thePri - 1:
               maxPri;
         }

   theIdx = -1;

   forEachDemand (theDemand, myProblem ())
      {
      theIdx ++;

      sortedDemVec_[theIdx]       = theDemand;

      unmetDemandVol_ (theDemand) = theDemand->demandVol ();
      }

   if (not forOpt_)
      if (myGlobalComp ()->userHeurStart ())
         allocShipVols ();
   }

//------------------------------------------------------------------------------

int WitHeurImpPerf::origPriority (WitDemand * theDemand, WitPeriod thePer) const
   {
   return 
      forOpt_?
         tempPriority_ (theDemand)[thePer]:
         theDemand->priority ()   [thePer];
   }

//------------------------------------------------------------------------------

void WitHeurImpPerf::allocShipVols ()
   {
   WitDemand * theDemand;
   WitPeriod   demPer;
   double      unallocShipVol;

   forEachDemand (theDemand, myProblem ())
      {
      unallocShipVol = 0.0;

      for (demPer = lastPeriod (); demPer >= 0; demPer --)
         {
         unallocShipVol += theDemand->shipVol ()[demPer];

         net (unmetDemandVol_ (theDemand)[demPer], unallocShipVol);
         }
      }
   }

//------------------------------------------------------------------------------

void WitHeurImpPerf::mainLoop ()
   {
   WitPeriod demPer;
   WitPeriod shipPer;

   forEachPeriod (demPer, myProblem ())
      {
      if (prioritiesAreDifferent (demPer))
         {
         curDemPer_ = demPer;

         sortedDemVec_.sort (comparePriorities);
         }

      for (shipPer = demPer; shipPer < nPeriods (); ++ shipPer)
         allocSortedDemands (demPer, shipPer);
      }
   }

//------------------------------------------------------------------------------

bool WitHeurImpPerf::prioritiesAreDifferent (WitPeriod demPer)
   {
   WitDemand * theDemand;

   if (demPer == 0)
      return true;

   forEachDemand (theDemand, myProblem ())
      if (modPriority_ (theDemand)[demPer] !=
          modPriority_ (theDemand)[demPer - 1])
         return true;

   return false;
   }

//------------------------------------------------------------------------------

bool WitHeurImpPerf::comparePriorities (
      WitDemand * theDemand1,
      WitDemand * theDemand2)
   {
   WitHeurImpPerf * thePerf;
   int              thePri1;
   int              thePri2;
   int              theIdx1;
   int              theIdx2;

   thePerf = theDemand1->myProblem ()->myHeurImploder ()->myHeurImpPerf ();

   thePri1 = thePerf->modPriority_ (theDemand1)[thePerf->curDemPer_];
   thePri2 = thePerf->modPriority_ (theDemand2)[thePerf->curDemPer_];

   if (thePri1 < thePri2)
      return true;

   if (thePri2 < thePri1)
      return false;

   theIdx1 = theDemand1->mappingIndex ();
   theIdx2 = theDemand2->mappingIndex ();

   return (theIdx1 < theIdx2);
   }

//------------------------------------------------------------------------------

void WitHeurImpPerf::allocSortedDemands (WitPeriod demPer, WitPeriod shipPer)
   {
   bool        eqAllNeeded;
   WitDemand * theDemand;

   WitPtrVecItr <WitDemand> theItr;

   eqAllNeeded = (myGlobalComp ()->equitability () > 1);

   sortedDemVec_.attachItr (theItr);

   while (theItr.advance (theDemand))
      {
      if (unmetDemandVol_ (theDemand)[demPer] < NET_TOL)
         continue;

      if (not theDemand->shipLateAllowed ())
         if (shipPer > demPer)
            continue;

      if (shipPer - demPer > theDemand->shipLateUB ()[demPer])
         continue;

      if (eqAllNeeded)
         selectForEqAlloc (theDemand, demPer, shipPer);
      else
         incAlloc         (theDemand, demPer, shipPer);
      }

   if (eqAllNeeded)
      if (not selDemPers_.isEmpty ())
         eqAlloc (demPer);
   }

//------------------------------------------------------------------------------

void WitHeurImpPerf::selectForEqAlloc (
      WitDemand * theDemand,
      WitPeriod   demPer,
      WitPeriod   shipPer)
   {
   if (not selDemPers_.isEmpty ())
      if (modPriority_ (theDemand)[demPer]
          !=
          modPriority_ (selDemPers_.firstObject ())[demPer])

         eqAlloc (demPer);

   selDemPers_.push (theDemand, shipPer);

   myEqAllocator ()->setDesIncVol (
      theDemand, 
      shipPer,
      unmetDemandVol_ (theDemand)[demPer]);

   if (myGlobalComp ()->twoWayMultiExec ())
      myEqAllocator ()->setAsapMultiExec (theDemand, shipPer, true);
   }

//------------------------------------------------------------------------------

void WitHeurImpPerf::eqAlloc (WitPeriod demPer)
   {
   WitDemand * theDemand;
   WitPeriod   thePer;
   double      incVolVal;

   selDemPers_.reverse ();

   myEqAllocator ()->eqAlloc (selDemPers_);

   if (myGlobalComp ()->printEqAlloc ())
      myEqAllocator ()->print (selDemPers_);

   while (selDemPers_.pop (theDemand, thePer))
      {
      incVolVal = myEqAllocator ()->incVol (theDemand, thePer);

      if (incVolVal >= NET_TOL)
         unmetDemandVol_ (theDemand)[demPer] -= incVolVal;
      }
   }

//------------------------------------------------------------------------------

void WitHeurImpPerf::incAlloc (
      WitDemand * theDemand,
      WitPeriod   demPer,
      WitPeriod   shipPer)
   {
   bool   asapMultiExec;
   double incVolVal;

   asapMultiExec = false;

   if (myGlobalComp ()->twoWayMultiExec ())
      asapMultiExec =
         myGlobalComp ()->twmeByDemand ()?
            theDemand->selForDel ():
            true;

   incVolVal =
      myProblem ()->myHeurAllMgr ()->incHeurAlloc (
         theDemand,
         shipPer,
         unmetDemandVol_ (theDemand)[demPer],
         asapMultiExec);

   if (incVolVal >= NET_TOL)
      unmetDemandVol_ (theDemand)[demPer] -= incVolVal;
   }
