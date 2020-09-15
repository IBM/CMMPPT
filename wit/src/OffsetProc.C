//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "OffsetProc.C"
//
// Contains the implementation of class OffsetProc.
//------------------------------------------------------------------------------

#include <OffsetProc.h>
#include <BopEntry.h>
#include <SubEntry.h>
#include <GlobalComp.h>
#include <Part.h>
#include <Operation.h>

//------------------------------------------------------------------------------
// Implementation of class OffsetProc.
//------------------------------------------------------------------------------

WitOffsetProc::WitOffsetProc (WitProblem * theProblem):

      WitProbAssoc (theProblem),
      truncOffset_ (),
      dblCumLead_  ()
   {
   }

//------------------------------------------------------------------------------

WitOffsetProc::~WitOffsetProc ()
   {
   }

//------------------------------------------------------------------------------

void WitOffsetProc::roundOffsets ()
   {
   WitOperation * theOpn;
   WitPeriod      thePer;
   bool           executableVal;

   WitTVec <double> leadToImpact (myProblem (), 0.0);
   WitTVec <bool>   hasImpact    (myProblem (), false);
      //
      // See updateProdCumLeads.

   if (myGlobalComp ()->truncOffsets ())
      truncateOffsets ();

   dblCumLead_.allocate (myProblem (), 0.0);

   forEachOperation (theOpn, myProblem ())
      {
      forEachPeriod (thePer, myProblem ())
         {
         executableVal = roundOffsets (theOpn, thePer);

         theOpn->storePreExecutable (thePer, executableVal);

         if (not executableVal)
            resetExec (theOpn, thePer);
         }

      updateProdCumLeads (theOpn, leadToImpact, hasImpact);
      }

   truncOffset_.clear ();
   dblCumLead_ .clear ();
   }

//------------------------------------------------------------------------------

void WitOffsetProc::truncateOffsets ()
   {
   WitBomEntry * theBomEnt;
   WitSubEntry * theSub;
   WitBopEntry * theBopEnt;

   truncOffset_.allocate (myProblem (), 0.0);

   forEachBomEntry (theBomEnt, myProblem ())
      {
      truncateOffset (theBomEnt);

      forEachEl (theSub, theBomEnt->mySubEntries ())
         truncateOffset (theSub);
      }

   forEachBopEntry (theBopEnt, myProblem ())
      truncateOffset (theBopEnt);
   }

//------------------------------------------------------------------------------

void WitOffsetProc::truncateOffset (WitBillEntry * theBillEnt)
   {
   WitPeriod thePer;

   forEachPeriod (thePer, myProblem ())
      truncOffset_ (theBillEnt)[thePer] =
         min (static_cast <double> (thePer), theBillEnt->offset ()[thePer]);
   }

//------------------------------------------------------------------------------
// roundOffsets
//
// Computes impactPeriods for the BomEntries and BopEntries of theOpn in thePer.
// Also checks for the various conditions under which execution of
// theOpn is to be forbidden in thePer.
// If any such condition occurs, computation is terminated and false
// is returned, indicating forbidden execution.
// Otherwise true is returned.
//
// The conditions for forbidding execution are:
//
//   yieldRate         == 0.0
//   cycle time        < 0 (3.4 mode only)
//   release date      < 0 (3.4 mode only)
//   an impactPeriod   < 0, or >= nPeriods
//   execEmptyBom () is false and no BomEntries are in effect in period t.
//------------------------------------------------------------------------------

bool WitOffsetProc::roundOffsets (WitOperation * theOpn, WitPeriod thePer)
   {
   WitBomEntry * theBomEnt;
   WitSubEntry * theSub;
   WitBopEntry * theBopEnt;
   bool          emptyBom;

   if (theOpn->yieldRate ()[thePer] == 0.0)
      return false;

   if (wit34Compatible ())
      if (theOpn->cycleTime34PreventsExec (thePer))
         return false;

   forEachEl (theBomEnt, theOpn->bom ())
      {
      if (not compImpactPeriod (theBomEnt, thePer))
         return false;

      forEachEl (theSub, theBomEnt->mySubEntries ())
         if (myGlobalComp ()->independentOffsets ())
            compImpactPeriod (theSub, thePer);
         else
            copyImpactPeriod (theSub, thePer);
      }

   forEachEl (theBopEnt, theOpn->bop ())
      if (not compImpactPeriod (theBopEnt, thePer))
         return false;

   if (not myGlobalComp ()->execEmptyBom ())
      {
      emptyBom = true;

      forEachEl (theBomEnt, theOpn->bom ())
         if (theBomEnt->inEffect (thePer))
            emptyBom = false;

      if (emptyBom)
         return false;
      }

   return true;
   }

//------------------------------------------------------------------------------
// compImpactPeriod.
// Computes theConsEnt->impactPeriod_[thePer].
// If the resulting impactPeriod falls outside the time horizon, the
// computation is aborted and the function returns false, indicating that
// execution of theConsEnt->myOperation () should be forbidden in thePer.
// Otherwise returns true.
//
// An initial value for the impactPeriod is computed using a simple integer
// round down of the double workOffset ()[thePer]. The final value is computed
// by rounding the double lead time either up or down so as to have the right
// effect on cumulative lead time.
//
// Variables:
//
//    initImpactPeriod:   Initial value for impactPeriod
//    consDCLead:         The double cum lead time for the consumed Part.
//    propDCLead:         The proposed double cum lead time for the consuming
//                        Operation. The proposed value is taken iff it is
//                        larger than the current value.
//    leadTime:           The int lead time
//    finalImpactPeriod:  Final value for ImpactPeriod
//------------------------------------------------------------------------------

bool WitOffsetProc::compImpactPeriod (
      WitConsEntry * theConsEnt,
      WitPeriod      thePer)
   {
   WitPeriod initImpactPeriod;
   double    consDCLead;
   double    propDCLead;
   int       leadTime;
   WitPeriod finalImpactPeriod;

   theConsEnt->storePreImpactPeriod (thePer, -1);

   if (not theConsEnt->withinInterval (thePer))
      return true;

   initImpactPeriod = thePer - ltRoundDown (workOffset (theConsEnt, thePer));

   if (initImpactPeriod < 0)
      return false;

   if (initImpactPeriod >= nPeriods ())
      return false;

   consDCLead = dblCumLead_ (theConsEnt->myPart ())[initImpactPeriod];

   propDCLead = consDCLead + workOffset (theConsEnt, thePer);

   leadTime =
        ltRoundDown (propDCLead)
      - ltRoundDown (consDCLead);

   finalImpactPeriod = thePer - leadTime;

   if (finalImpactPeriod < 0)
      return false;

   theConsEnt->storePreImpactPeriod (thePer, finalImpactPeriod);

   setToMax (dblCumLead_ (theConsEnt->myOperation ())[thePer], propDCLead);

   return true;
   }

//------------------------------------------------------------------------------

void WitOffsetProc::copyImpactPeriod (WitSubEntry * theSub, WitPeriod thePer)
   {
   WitPeriod impactPer;

   theSub->storePreImpactPeriod (thePer, -1);

   if (theSub->myBomEnt ()->inEffect (thePer))
      if (theSub->withinInterval (thePer))
         {
         impactPer = theSub->myBomEnt ()->impactPeriod () [thePer];

         theSub->storePreImpactPeriod (thePer, impactPer);
         }
   }

//------------------------------------------------------------------------------
// compImpactPeriod for a BopEntry.
//
// Variables:
//
//    opnDCLead:        The double cum lead time for the producing Operation:
//    propDCLead:       The proposed double cum lead time for the produced
//                      Part. The proposed value is taken iff it is larger
//                      than the current value.
//    leadTime:         The int lead time
//    theImpactPeriod:  The impactPeriod
//------------------------------------------------------------------------------

bool WitOffsetProc::compImpactPeriod (
      WitBopEntry * theBopEnt,
      WitPeriod     thePer)
   {
   double    opnDCLead;
   double    propDCLead;
   int       leadTime;
   WitPeriod impactPer;

   theBopEnt->storePreImpactPeriod (thePer, -1);

   if (not theBopEnt->withinInterval (thePer))
      return true;

   opnDCLead  = dblCumLead_ (theBopEnt->myOperation ())[thePer];

   propDCLead = opnDCLead - workOffset (theBopEnt, thePer);

   leadTime =
        ltRoundDown (opnDCLead)
      - ltRoundDown (propDCLead);

   impactPer = thePer - leadTime;

   if (impactPer < 0)
      return false;

   if (impactPer >= nPeriods ())
      return false;

   theBopEnt->storePreImpactPeriod (thePer, impactPer);

   return true;
   }

//------------------------------------------------------------------------------

int WitOffsetProc::ltRoundDown (double leadTime)
   {
   return static_cast <int> (floor (leadTime + 1.0E-3));
   }

//------------------------------------------------------------------------------

void WitOffsetProc::resetExec (WitOperation * theOpn, WitPeriod thePer)
   {
   WitBomEntry * theBomEnt;
   WitSubEntry * theSub;
   WitBopEntry * theBopEnt;

   forEachEl (theBomEnt, theOpn->bom ())
      {
      theBomEnt->storePreImpactPeriod (thePer, -1);

      forEachEl (theSub, theBomEnt->mySubEntries ())
         theSub->storePreImpactPeriod (thePer, -1);
      }

   forEachEl (theBopEnt, theOpn->bop ())
      theBopEnt->storePreImpactPeriod (thePer, -1);

   dblCumLead_ (theOpn)[thePer] = 0.0;
   }

//------------------------------------------------------------------------------

void WitOffsetProc::updateProdCumLeads (
      WitOperation *     theOpn,
      WitTVec <double> & leadToImpact,
      WitTVec <bool> &   hasImpact)
   {
   WitBopEntry * theBopEnt;
   WitPeriod     thePer;
   WitPeriod     theImpactPer;

   forEachEl (theBopEnt, theOpn->bop ())
      {
      hasImpact = false;

      forEachPeriod (thePer, myProblem ())
         if (theBopEnt->inEffect (thePer))
            {
            theImpactPer = theBopEnt->impactPeriod ()[thePer];

            hasImpact[theImpactPer]    = true;

            leadToImpact[theImpactPer] = 
               dblCumLead_ (theOpn)[thePer] - workOffset (theBopEnt, thePer);
            }

      forEachPeriod (theImpactPer, myProblem ())
         if (hasImpact[theImpactPer])
            setToMax (
               dblCumLead_ (theBopEnt->myPart ())[theImpactPer],
               leadToImpact[theImpactPer]);
      }
   }

//------------------------------------------------------------------------------

double WitOffsetProc::workOffset (WitBillEntry * theBillEnt, WitPeriod thePer)
   {
   return
      myGlobalComp ()->truncOffsets ()?
         truncOffset_ (theBillEnt)[thePer]:
         theBillEnt->offset ()[thePer];
   }
