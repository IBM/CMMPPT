//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "EqAll.C"
//
// Contains the implementation of class EqAllocator.
//------------------------------------------------------------------------------

#include <EqAll.h>
#include <HeurAllMgr.h>
#include <HeurModifier.h>
#include <DataWrit.h>
#include <GlobalComp.h>
#include <Demand.h>
#include <PairStack.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class EqAllocator
//------------------------------------------------------------------------------

WitEqAllocator::WitEqAllocator (WitProblem * theProblem):
      WitProbAssoc   (theProblem),

      active_        (false),
      desIncVol_     (),
      incVol_        (),
      prevDesVol_    (),
      firstIndex_    (),
      asapMultiExec_ ()
   {
   }

//------------------------------------------------------------------------------

WitEqAllocator::~WitEqAllocator ()
   {
   deactivate ();
   }

//------------------------------------------------------------------------------

void WitEqAllocator::setAsapMultiExec (
      WitDemand * theDemand, 
      WitPeriod   shipPer,
      bool        theValue)
   {
   witAssert (myGlobalComp ()->twoWayMultiExec ());

   asapMultiExec_ (theDemand)[shipPer] = theValue;
   }

//------------------------------------------------------------------------------

bool WitEqAllocator::containsDup (
      const WitDemPerStack & theDemPers,
      int &                  firstDupIndex,
      int &                  secondDupIndex)
   {
   bool         dupFound;
   int          targetIndex;
   WitDemPerItr theItr;
   WitDemand *  theDemand;
   WitPeriod    thePer;

   witAssert (active_);

   dupFound = false;

   if (firstIndex_.domainSize () == 0)
      firstIndex_.allocate (myProblem (), -1);

   targetIndex = -1;

   theDemPers.attachItr (theItr);

   while (theItr.advance (theDemand, thePer))
      {
      ++ targetIndex;

      if (firstIndex_ (theDemand)[thePer] >= 0)
         {
         dupFound       = true;
         firstDupIndex  = firstIndex_ (theDemand)[thePer];
         secondDupIndex = targetIndex;

         break;
         }

      firstIndex_ (theDemand)[thePer] = targetIndex;
      }

   theDemPers.attachItr (theItr);

   while (theItr.advance (theDemand, thePer))
      firstIndex_ (theDemand)[thePer] = -1;

   return dupFound;
   }

//------------------------------------------------------------------------------

void WitEqAllocator::activate ()
   {
   if (active_)
      return;

   witAssert (myProblem ()->myHeurAllMgr ()->active ());

   desIncVol_ .allocate (myProblem (), 0.0);
   incVol_    .allocate (myProblem (), 0.0);
   prevDesVol_.allocate (myProblem (), 0.0);

   if (myGlobalComp ()->twoWayMultiExec ())
      asapMultiExec_.allocate (myProblem (), false);

   active_ = true;
   }

//------------------------------------------------------------------------------

void WitEqAllocator::heurAllocIsReset ()
   {
   deactivate ();
   }

//------------------------------------------------------------------------------

void WitEqAllocator::eqAlloc (const WitDemPerStack & theDemPers)
   {
   WitDemPerItr theItr;
   WitDemand *  theDemand;
   WitPeriod    thePer;
   int          firstIndex;
   int          secondIndex;

   witAssert (active_);
   witAssert (not theDemPers.isEmpty ());

   witAssert (not containsDup (theDemPers, firstIndex, secondIndex));

   if (myGlobalComp ()->modHeurAlloc ())
      {
      myProblem ()->myHeurModifier ()->checkFeasibility ();
      }

   if (myGlobalComp ()->forcedMultiEq () or multiSigDDs (theDemPers))
      eqAllocMulti (theDemPers);
   else
      {
      theDemPers.attachItr (theItr);

      while (theItr.advance (theDemand, thePer))
         incVol_ (theDemand)[thePer] =
            myProblem ()->myHeurAllMgr ()->incHeurAlloc (
               theDemand,
               thePer,
               desIncVol_ (theDemand)[thePer],
               asapMultiExec (theDemand, thePer));
      }
   }

//------------------------------------------------------------------------------

void WitEqAllocator::print (const WitDemPerStack & theDemPers)
   {
   int          theIndex;
   WitDemPerItr theItr;
   WitDemand *  theDemand;
   WitPeriod    shipPer;

   theIndex = -1;

   theDemPers.attachItr (theItr);

   while (theItr.advance (theDemand, shipPer))
      {
      myMsgFac () ("targetNumMsg", ++ theIndex);

      myMsgFac () ("partNameMsg", theDemand->demandedPartName ());
      myMsgFac () ("demandNameMsg", theDemand->demandName ());

      if (myGlobalComp ()->twoWayMultiExec ())
         myProblem ()->myHeurAllMgr ()->
            printMeDir (asapMultiExec_ (theDemand)[shipPer]);

      myMsgFac () ("incHeurDataMsg",
         shipPer,
         desIncVol_ (theDemand)[shipPer],
         incVol_    (theDemand)[shipPer]);
      }
   }

//------------------------------------------------------------------------------

void WitEqAllocator::setDesIncVol (
      WitDemand * theDemand, 
      WitPeriod   shipPer, 
      double       theValue)
   {
   witAssert (theValue >= 0.0);

   desIncVol_ (theDemand)[shipPer] = theValue;
   }

//------------------------------------------------------------------------------

double WitEqAllocator::incVol (WitDemand * theDemand, WitPeriod shipPer)
   {
   return incVol_ (theDemand)[shipPer];
   }

//------------------------------------------------------------------------------

void WitEqAllocator::deactivate ()
   {
   if (not active_)
      return;

   firstIndex_   .clear ();
   prevDesVol_   .clear ();
   incVol_       .clear ();
   desIncVol_    .clear ();
   asapMultiExec_.clear ();

   active_ = false;
   }

//------------------------------------------------------------------------------

bool WitEqAllocator::multiSigDDs (const WitDemPerStack & theDemPers)
   {
   WitDemPerItr theItr;
   WitDemand *  theDemand;
   WitPeriod    thePer;
   int          nSigPairs;

   nSigPairs = 0;

   theDemPers.attachItr (theItr);

   while (theItr.advance (theDemand, thePer))
      if (desIncVol_   (theDemand)[thePer] > NET_TOL)
         if (++ nSigPairs > 1)
            return true;

   return false;
   }

//------------------------------------------------------------------------------

void WitEqAllocator::eqAllocMulti (const WitDemPerStack & theDemPers)
   {
   int          equit;
   double       curSearchInc;
   WitDemPerItr theItr;
   WitDemand *  theDemand;
   WitPeriod    thePer;
   int          itNo;
   double       fraction;
   double       totDesVol;
   double       newDesVol;

   equit = myGlobalComp ()->equitability ();

   theDemPers.attachItr (theItr);

   while (theItr.advance (theDemand, thePer))
      incVol_ (theDemand)[thePer] = 0.0;

   for (itNo = 1; itNo <= equit; itNo ++)
      {
      fraction =
           static_cast <double> (itNo)
         / static_cast <double> (equit);

      theDemPers.attachItr (theItr);

      while (theItr.advance (theDemand, thePer))
         {
         curSearchInc = theDemand->searchInc ();

         totDesVol    = desIncVol_ (theDemand)[thePer];

         newDesVol    =
              ceil (fraction * totDesVol / curSearchInc - .001)
            * curSearchInc;

         setToMin (newDesVol, totDesVol);

         incVol_ (theDemand)[thePer] +=
            myProblem ()->myHeurAllMgr ()->incHeurAlloc (
               theDemand,
               thePer,
               newDesVol - prevDesVol_ (theDemand)[thePer],
               asapMultiExec (theDemand, thePer));

         prevDesVol_ (theDemand)[thePer] = newDesVol;
         }
      }

   while (theItr.advance (theDemand, thePer))
      prevDesVol_ (theDemand)[thePer] = 0.0;
   }

//------------------------------------------------------------------------------

bool WitEqAllocator::asapMultiExec (WitDemand * theDemand, WitPeriod shipPer)
   {
   return
      myGlobalComp ()->twoWayMultiExec ()?
         asapMultiExec_ (theDemand)[shipPer]:
         false;
   }
