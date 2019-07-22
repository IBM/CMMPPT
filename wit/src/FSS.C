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
// Source file: "FSS.C"
//
// Contains the implemention of the following classes:
//
// FSS.
// FssMrpExp.
//------------------------------------------------------------------------------

#include <FSS.h>
#include <FssMrp.h>
#include <Post.h>
#include <GlobalComp.h>
#include <Material.h>
#include <Demand.h>
#include <Operation.h>
#include <SubEntry.h>
#include <BopEntry.h>
#include <BoundSet.h>
#include <Timing.h>
#include <Variant.h>
#include <MsgFac.h>
#include <ObjStack.h>

//------------------------------------------------------------------------------
// Implementation of class FSS.
//
// Class FSS is responsible for computing the Focussed Shortage Schedule.
// This is done by the compute member function.
//
// The focus data consists of either:
//    the focus horizons (theDemand->focusHorizon (), for each Demand), or
//    the FSS shipment schedule (theDemand->fssShipVol () for each Demand).
// If myGlobalComp ()->useFocusHorizons () is true, then, as a preliminary step,
// compute computes the FSS shipment schedule automatically, based on
// the focus horizons. Otherwise, the FSS shipment schedule is assumed to be
// user-specified.
//
// The Focussed Shortage Schedule is computed by defining a certain MRP
// problem, invoking WIT-MRP on it and taking the resulting requirements
// schedule as the Focussed Shortage Schedule. The input for the MRP is defined
// by the incremental availability schedule for the MRP: mrpIncAvail. Positive
// values of mrpIncAvail (thePart, thePer) are interpreted as supply for
// thePart;
// negative values are interpreted as demand for it. An instance of class
// WitFssMrpExp is created and its mrp function is called, causing it to
// perform MRP, using mrpIncAvail for its supply and demand and storing the
// resulting requirement schedule back in FSS.
//
// Note: for purposes of defining the MRP problem for FSS, there is no need
// to distinguish between multiple demands for the same part, so things like
// demand schedules and shipment schedules are defined for Parts instead of
// Demands.
//
// To define the MRP problem, an "unimplosion" problem is defined, an
// unimplosion is performed on it and then the MRP problem is defined from the
// result. (Unimplosion is defined below.) The data for the unimplosion problem
// is the same as the data from the original implosion problem and its
// solution, except that the shipment schedule is replaced by the "achieved FSS
// shipment schedule", defined for any Part in any period as:
//
//    achFssShipVol[thePer] = min (fssShipVol[thePer], shipVol[thePer])
//
// This is the amount of the fssShipVol that was achieved by the implosion
// shipment schedule. The rest of the shipment schedule is initially
// discarded as scrap. This scrap constitutes over-production in the implosion
// solution relative to the FSS shipment schedule. The task of unimplosion is
// to undo this over-production wherever possible, so as to free up the
// resources that were consumed for over-production and make them available to
// the FSS MRP.
//
// The unimplosion algorithm is a heuristic for doing this. It works roughly
// like an MRP explosion with negative demand. In the spirit of MRP, JIT, the
// WIT implosion heuristic, etc., the idea is to produce as little as possible
// and as late as possible, while still meeting demands (in this case, the
// achieved FSS shipment schedule). For unimplosion, this implies reducing
// production ("unimploding") as much as possible and as early as possible.
// The Operations are examined in reverse order. A Operation, uiOpn will
// be unimploded only if:
//
//    It's non-coproducing
//    Its BOP contains a Part (uiPart) with significantly positive productRate.
//       (thePart will be unique.)
//
// If uiPart is a Material, the scrap for thePart in all periods is rolled back
// as early in time as possible to make it available for early unimplosion. The
// execution periods, execPer, for uiOpn are then considered in order. If
// uiPart is a Material, the scrap from any previous period is rolled forward to
// the impactPeriod
// for execPer. Then deltaExecVol is computed, the maximum possible reduction in
// uiOpn's execVol such that uiPart's scrapVol will remain
// nonnegative and there will be no increased violation of uiOpn's
// execution lower bound. (FSS is allowed to violate lower bounds on execution,
// but by no more than the amount (if any) by which the implosion solution does
// so. Thus the lower bound for FSS purposes is:
// min (exec softLB[thePer], execVol[thePer]).) The scrapVols of uiPart and
// Parts in the BOM are updated accordingly. If a BomEntry has substitutes, the
// reduction in execution must be allocated among them. The reduction volume is
// allocated to the subs for theBomEnt in reverse preference order, so that
// the higher preference subs tend to keep their subVols.
//
// After the unimplosion is performed, several schedules can be computed,
// leading up to the computation of mrpIncAvail, which defines the FSS MRP.
// First is the non-supply incremental availability schedule, nonSupIncAvail.
// For any Material, nonSupIncAvail[thePer] is the change in availability of the
// Material in thePer, excluding supply. It is computed as:
//
// nonSupIncAvail   [thePer] =
//      scrapVol    [thePer]
//    + (stockVol   [thePer]     - fssStockLB[thePer])
//    - (stockVol   [thePer - 1] - fssStockLB[thePer - 1])
//    - supplyVol ()[thePer]
//
// If we ignore the "fssStockLB" terms and the supplyVol term, we just have
// the normal formula for incremental availability. The supplyVol term is the
// necessary adjustment to exclude supply.
//
// The two "fssStockLB" terms reflect the fact that FSS will not violate stock
// lower bounds any more than the implosion solution did. This implies a stock
// lower bound for FSS purposes of:
//
//    fssStockLB[thePer] = min (stock softLB[thePer], stockVol[thePer])
//
// Thus in thePer, only (stockVol[thePer] - fssStockLB[thePer]) units of stock
// are actually available, and this explains the two fssStockLB terms.
//
// nonSupIncAvail is used to compute the minimum supply availability schedule,
// minSupAvail. For any Material, minSupAvail[thePer] is the availability of the
// Material in thePer, using the minumum possible supply to insure
// availability is non-negative. It is computed as:
//
// minSupAvail[-1] = 0
//
// For each period, t:
//
//   minSupAvail[thePer] = 
//      positive part of (minSupAvail[thePer - 1] + nonSupIncAvail[thePer])
//
// minSupAvail is used to compute the supply consumption schedule,
// supplyConsVol. For any Material, supplyConsVol[thePer] is the amount of
// supply of the Material that is consumed in thePer. It is computed as:
//
//   supplyConsVol[thePer] = 
//      negative part of (minSupAvail[thePer - 1] + nonSupIncAvail[thePer])
//
// In other words, any time (minSupAvail[thePer - 1] + nonSupIncAvail[thePer]) 
// is
// negative, - (minSupAvail[thePer - 1] + nonSupIncAvail[thePer]) units of
// supply are
// being consumed in order to keep availability (minSupAvail) from going 
// negative.
//
// Finally, once supplyConsVol has been computed, mrpIncAvail is computed as
// follows:
//
// mrpIncAvail[thePer] =
//      supplyVol ()[thePer]
//    - supplyConsVol[thePer]
//    - (fssShipVol[thePer] - achFssShipVol)
//
// In other words, the original supply is nominally available to the FSS MRP,
// but any time supply is consumed by the (unimploded) implosion solution, that
// consumption is accounted for in the FSS MRP, either by reducing supply
// (when mrpIncAvail[thePer] >= 0) or increasing demand (when 
// mrpIncAvail[thePer] < 0).
// The other demand term, (fssShipVol[thePer] - achFssShipVol) (which is always
// >= 0), is the part of fssShipVol that is not achieved by the implosion
// solution.
//
// Note that by putting supply into the FSS MRP in the period in which it is
// actually supplied (with supplyVol) and then taking it back out in the period
// in which it is consumed (with supplyConsVol), the FSS MRP is being given
// access to the invervening stocks of supply. This allows FSS to report
// shortages in later periods than would otherwise be possible, while still
// satisfying the "feasibility" requirement of FSS.
//
// The above describes how mrpIncAvail is computed in principle; in practice,
// the computation is streamlined in a number of ways:
//
// * shipVol and fssShipVol for Materials (rather than Demands) are not
//   explicitly maintained.
// * achFssShipVol is not explicitly maintained.
// * unimplosion only maintains a scrap schedule and a stock schedule,
//   called uiScrapVol_ and uiStockVol_. The production schedule itself doesn't
//   need to be maintained.
// * nonSupIncAvail is not explicitly stored.
// * minSupAvail and supplyConsVol are only computed for the current Material
//   and period and are not stored away as schedules.
//------------------------------------------------------------------------------

WitFSS::WitFSS (WitProblem * theProblem):
      WitProbAssoc   (theProblem),

      computed_      (false),
      uploadingSoln_ (false),
      fssExecVol_    (),
      fssSubVol_     ()
   {
   }

//------------------------------------------------------------------------------

WitFSS::~WitFSS ()
   {
   }

//------------------------------------------------------------------------------
// compute.
// Computes the Focussed Shortage Schedule.
// The following approach is used:
// New supply and demand schedules are computed, based on the implosion
//    solution and the focus.
// MRP is invoked on the new supply and demand schedules.
// The resulting requirements schedule is stored as the Focussed Shortage
// Schedule.
//------------------------------------------------------------------------------

void WitFSS::compute ()
   {
   WitDemand * theDemand;

   WitTimer::enterSection ("fss");

   if (not myPostprocessor ()->postprocessed ())
      myMsgFac () ("unpostprocessedFssSmsg");

   if (not myProblem ()->currentVariant ()->internalSoln ())
      myMsgFac () ("fssWithUserSolnSmsg");

   myMsgFac () ("compFSSMsg");

   if (myGlobalComp ()->useFocusHorizons ())
      forEachDemand (theDemand, myProblem ())
         theDemand->compFssShipVol ();

   initSoln ();

   defineMrp ();

   doMrp ();

   fssExecVol_.clear ();
   fssSubVol_ .clear ();

   computed_ = true;

   WitTimer::leaveSection ("fss");
   }

//------------------------------------------------------------------------------
// compFssShipVol ()
//
// Local Variables:
//
// cumShipVolThePer:      Cumulative shipment volume in thePer.
// cDFHorizon:            Cumulative demand volume at the focus horizon of
//                        theDemand
// cumFssShipVolT:        Cumulative of fssShipVol ()[thePer].
// cumFssShipVolThePer_1: Cumulative of fssShipVol ()[thePer - 1].
//------------------------------------------------------------------------------

void WitFSS::compFssShipVol (
      WitDemand *     theDemand,
      WitDblFlexVec & fssShipVolRef)
   {
   double      cumShipVolThePer;
   double      cDFHorizon;
   WitPeriod   thePer;
   double      cumFssShipVolThePer;
   double      cumFssShipVolThePer_1;

   witAssert (myGlobalComp ()->useFocusHorizons ());

   cumShipVolThePer = 0.0;

   cDFHorizon =
      (theDemand->focusHorizon () >= 0)?
         theDemand->cumDemandVol ()[theDemand->focusHorizon ()]:
         0.0;

   cumFssShipVolThePer = 0.0;

   forEachPeriod (thePer, myProblem ())
      {
      cumShipVolThePer += theDemand->shipVol ()[thePer];

      cumFssShipVolThePer_1 = cumFssShipVolThePer;

      cumFssShipVolThePer =
         (thePer <= theDemand->focusHorizon ())?
            theDemand->cumDemandVol ()[thePer]:
            max (cDFHorizon, cumShipVolThePer);

      fssShipVolRef.elemRef (thePer) =
         cumFssShipVolThePer - cumFssShipVolThePer_1;
      }
   }

//------------------------------------------------------------------------------

double WitFSS::mrpSupplyVol (WitPart * thePart, WitPeriod thePer) const
   {
   return positivePart (mrpIncAvail_ (thePart)[thePer]);
   }

//------------------------------------------------------------------------------

double WitFSS::mrpDemandVol (WitPart * thePart, WitPeriod thePer) const
   {
   return positivePart (- mrpIncAvail_ (thePart)[thePer]);
   }

//------------------------------------------------------------------------------

void WitFSS::uncompute ()
   {
   computed_ = false;
   }

//------------------------------------------------------------------------------

void WitFSS::initSoln ()
   {
   WitOperation * theOpn;
   WitSubEntry *  theSub;

   fssExecVol_.allocate (myProblem (), 0.0);
   fssSubVol_ .allocate (myProblem (), 0.0);

   forEachOperation (theOpn, myProblem ())
      fssExecVol_ (theOpn) = theOpn->execVol ();

   forEachSubEntry (theSub, myProblem ())
      fssSubVol_  (theSub) = theSub->subVol ();
   }

//------------------------------------------------------------------------------

void WitFSS::defineMrp ()
   {
   uiScrapVol_.allocate (myProblem (), 0.0);
   uiStockVol_.allocate (myProblem (), 0.0);

   defineUnimplode ();

   unimplode ();

   compMrpIncAvail ();

   uiScrapVol_.clear ();
   uiStockVol_.clear ();
   }

//------------------------------------------------------------------------------

void WitFSS::defineUnimplode ()
   {
   WitPart *     thePart;
   WitMaterial * theMat;
   WitPeriod     thePer;
   int           maxNSubs;
   WitBomEntry * theBomEnt;

   forEachPart (thePart, myProblem ())
      forEachPeriod (thePer, myProblem ())
         {
         uiScrapVol_ (thePart)[thePer] = thePart->scrapVol ()[thePer] +
            positivePart (- deltaFssShipVol (thePart, thePer));
               //
               // shipVol - achFssShipVol
               // Scrapping any shipVol that's in excess of achFssShipVol.
               // This causes unimplosion to be performed relative to
               // achFssShipVol.

         if (uiScrapVol_ (thePart)[thePer] < 0.0)
             uiScrapVol_ (thePart)[thePer] = 0.0;
             //
             // A negative scrapVol is a constraint violation that is
             // presumed to be an insignificant numerical error and therefore
             // ignored.
         }

   forEachMaterial (theMat, myProblem ())
      uiStockVol_ (theMat) = theMat->stockVol ();

   maxNSubs = 0;

   forEachBomEntry (theBomEnt, myProblem ())
      setToMax (maxNSubs, theBomEnt->mySubEntries ().nElements ());
   }

//------------------------------------------------------------------------------

double WitFSS::deltaFssShipVol (WitPart * thePart, WitPeriod thePer)
   {
   WitDemand * theDemand;
   double      delta;

   delta = 0.0;

   forEachEl (theDemand, thePart->myDemands ())
      delta +=
         theDemand->fssShipVol ()[thePer] -
         theDemand->shipVol    ()[thePer];

   return delta;
   }

//------------------------------------------------------------------------------
// unimplode.
//
// deltaExecVol is the amount by which an execVol will be reduced.
//------------------------------------------------------------------------------

void WitFSS::unimplode ()
   {
   WitOperation * uiOpn;
   bool           isCoprod;
   WitBopEntry *  uiBopEnt;
   WitPart *      uiPart;
   WitMaterial *  uiMat;
   WitPeriod      execPer;
   WitPeriod      scrapPer;
   WitBopEntry *  theBopEnt;

   WitObjStack <WitOperation> reversedOpns (myProblem ());

   revCopyInto (reversedOpns, myCompMgr ()->allOperations ());

   while (reversedOpns.pop (uiOpn))
      {
      isCoprod = false;

      forEachPeriod (execPer, myProblem ())
         if (uiOpn->isCoproducing (execPer))
            isCoprod = true;

      if (isCoprod)
         continue;

      uiBopEnt = NULL;

      forEachEl (theBopEnt, uiOpn->bop ())
         forEachPeriod (execPer, myProblem ())
            if (theBopEnt->productRate ()[execPer] > 0.0)
               {
               uiBopEnt = theBopEnt;

               break;
               }

      if (uiBopEnt == NULL)
         continue;

      uiPart = uiBopEnt->myPart ();

      uiMat  = uiPart->thisMat ();

      if (uiMat != NULL)
         uiMat->rollBackScrap (uiStockVol_ (uiMat), uiScrapVol_ (uiMat));

      scrapPer = 0;

      forEachPeriod (execPer, myProblem ())
         unimplode (uiOpn, uiBopEnt, uiPart, execPer, scrapPer);
      }
   }

//------------------------------------------------------------------------------

void WitFSS::unimplode (
      WitOperation * uiOpn,
      WitBopEntry *  uiBopEnt,
      WitPart *      uiPart,
      WitPeriod      execPer,
      WitPeriod &    scrapPer)
   {
   WitPeriod     prodPer;
   WitMaterial * uiMat;
   double        deltaExecVol;
   double        oldExecVol;

   if (uiBopEnt->effProdRate ()[execPer] < .01)
      return;

   deltaExecVol =
      positivePart (
         + fssExecVol_ (uiOpn)[execPer]
         - uiOpn->execBounds ()->softLB ()[execPer]);

   if (deltaExecVol < FLOAT_EPSILON)
      return;

   prodPer = uiBopEnt->impactPeriod ()[execPer];

   uiMat   = uiPart->thisMat ();

   if (uiMat != NULL)
      rollOverScrap (uiMat, scrapPer, prodPer);

   setToMin (
      deltaExecVol,
      uiScrapVol_ (uiPart)[prodPer] / uiBopEnt->effProdRate ()[execPer]);

   if (deltaExecVol < FLOAT_EPSILON)
      return;

   oldExecVol   = fssExecVol_ (uiOpn)[execPer];

   deltaExecVol = - uiOpn->lotSizeDelta (execPer, oldExecVol, - deltaExecVol);

   if (deltaExecVol < FLOAT_EPSILON)
      return;

   fssExecVol_ (uiOpn)[execPer] -= deltaExecVol;

   if (uiBopEnt->inEffect (execPer))
      uiScrapVol_ (uiPart)[prodPer] -=
         deltaExecVol * uiBopEnt->effProdRate ()[execPer];

   unimplodeBom (uiOpn, execPer, deltaExecVol);
   }

//------------------------------------------------------------------------------

void WitFSS::rollOverScrap (
      WitMaterial * theMat,
      WitPeriod &   scrapPer,
      WitPeriod     prodPer)
   {
   double carryOver;
      //
      // Scrap volume carried over as stock from thePer - 1 to thePer.

   double maxStockInc;
      //
      // The amount by which stockVol can be increased without violating its
      // hardUB.

   while (scrapPer < prodPer)
      {
      ++ scrapPer;

      if (not theMat->canStock (scrapPer - 1))
         continue;

      carryOver = uiScrapVol_ (theMat)[scrapPer - 1];

      if (theMat->stockBounds ()->hardUBIsFinite (scrapPer - 1))
         {
         maxStockInc =
            positivePart (
               + theMat->stockBounds ()->hardUB ()[scrapPer - 1]
               - uiStockVol_ (theMat)[scrapPer - 1]);

         carryOver = min (carryOver, maxStockInc);
         }

      if (carryOver <= FLOAT_EPSILON)
         continue;

      uiScrapVol_ (theMat)[scrapPer - 1] -= carryOver;
      uiStockVol_ (theMat)[scrapPer - 1] += carryOver;
      uiScrapVol_ (theMat)[scrapPer]     += carryOver;
      }
   }

//------------------------------------------------------------------------------

void WitFSS::unimplodeBom (
      WitOperation * uiOpn,
      WitPeriod      execPer,
      double         deltaExecVol)
   {
   WitBomEntry * theBomEnt;
   WitPeriod     consPer;

   forEachEl (theBomEnt, uiOpn->bom ())
      if (theBomEnt->inEffect (execPer))
         {
         consPer = theBomEnt->impactPeriod ()[execPer];

         uiScrapVol_ (theBomEnt->myPart ())[consPer] +=
            deltaExecVol * theBomEnt->effConsRate ()[execPer];

         unimplodeSubs (theBomEnt, execPer, deltaExecVol);
         }
   }

//------------------------------------------------------------------------------
// unimplodeSubs.
// Reduces the subs for theBomEnt in thePer for a reduction of
// deltaExecVol in the execVol of theBomEnt->myOperation ().
//
// The reduction volume is allocated to the subs for theBomEnt in decreasing
// aversion order, so that the highest aversion subs are unimploded first.
//------------------------------------------------------------------------------

void WitFSS::unimplodeSubs (
      WitBomEntry * theBomEnt,
      WitPeriod     execPer,
      double        deltaExecVol)
   {
   WitSubEntry * theSub;      // SubEntry being unimploded.
   double        unallocVol;  // The amount of deltaExecVol that has not yet
                              // been allocated to any Sub for a particular
                              // bomEntry.
   double        deltaSubVol; // The amount of deltaExecVol allocated to a
                              // particular sub.
   WitPeriod     consPer;     // Consumption period.

   unallocVol = deltaExecVol;
   consPer    = theBomEnt->impactPeriod ()[execPer];


   WitObjStack <WitSubEntry> reversedSubEntries (myProblem ());
      //
      // The Subs for theBomEnt, in reverse order.

   revCopyInto (reversedSubEntries, theBomEnt->mySubEntries ());

   while (reversedSubEntries.pop (theSub))
      {
      if (not theSub->inEffect (execPer))
         continue;

      if (fssSubVol_ (theSub)[execPer] < FLOAT_EPSILON)
         continue;

      if (fssSubVol_ (theSub)[execPer] < unallocVol)
         {
         deltaSubVol = fssSubVol_ (theSub)[execPer];

         fssSubVol_ (theSub)[execPer] = 0.0;

         unallocVol -= deltaSubVol;
         }
      else
         {
         deltaSubVol = unallocVol;

         fssSubVol_ (theSub)[execPer] -= deltaSubVol;

         unallocVol = 0.0;
         }

      uiScrapVol_ (theSub->myPart ())[consPer] +=
         deltaSubVol * theSub->effConsRate ()[execPer];

      if (unallocVol < FLOAT_EPSILON)
         break;
      }
   }

//------------------------------------------------------------------------------

void WitFSS::compMrpIncAvail ()
   {
   WitPart *     thePart;
   WitMaterial * theMat;
   WitPeriod     thePer;

   double uiStockVolThePer_1;
      //
      // uiStockVol_[thePer - 1], or 0.0, if thePer == 0.

   double fssStockLBThePer_1;
      //
      // Lower bound on stockVol for a material in thePer - 1 for FSS purposes.

   double fssStockLBThePer;
      //
      // Lower bound on stockVol for a material in thePer for FSS purposes.

   double minSupAvail;
      //
      // Minimum supply availability.
      // The availability of theMat in thePer, using the minumum
      // necessary supply. When this becomes negative, supply is consumed to
      // compensate make it zero.

   double supplyConsVol;
      //
      // Supply Consumption volume for theMat in thePer.

   //--------------------------------------------------------------------------

   mrpIncAvail_.allocate (myProblem (), 0.0);

   forEachPart (thePart, this)
      if (thePart->thisCap () != NULL)
         mrpIncAvail_ (thePart) = uiScrapVol_ (thePart);

   forEachMaterial (theMat, myProblem ())
      {
      fssStockLBThePer = 0.0;
      minSupAvail      = 0.0;

      forEachPeriod (thePer, myProblem ())
         {
         uiStockVolThePer_1 =
            (thePer > 0)?
               uiStockVol_ (theMat)[thePer - 1]:
               0.0;

         fssStockLBThePer_1 = fssStockLBThePer;

         fssStockLBThePer =
            min (
               theMat->stockBounds ()->softLB ()[thePer],
               theMat->stockVol ()[thePer]);
                  //
                  // Note that the actual stockVol (and not uiStockVol_) is
                  // appropriate for defining FSS Stock LB.

         minSupAvail +=
            + uiScrapVol_ (theMat)[thePer]
            + uiStockVol_ (theMat)[thePer]
            - fssStockLBThePer
            - uiStockVolThePer_1
            + fssStockLBThePer_1
            - theMat->supplyVol ()[thePer];

         if (minSupAvail < - FLOAT_EPSILON)
            {
            supplyConsVol = - minSupAvail;
            minSupAvail   = 0.0;
            }
         else supplyConsVol = 0.0;

         mrpIncAvail_ (theMat)[thePer] =
            theMat->supplyVol ()[thePer] - supplyConsVol;
         }
      }

   forEachPart (thePart, myProblem ())
      forEachPeriod (thePer, myProblem ())
         mrpIncAvail_ (thePart)[thePer] -=
            positivePart (deltaFssShipVol (thePart, thePer));
               //
               // fssShipVol - achFssShipVol
   }

//------------------------------------------------------------------------------

void WitFSS::doMrp ()
   {
   WitFssMrpExp
      theFssMrpExp (
         myProblem (),
         this,
         uploadingSoln_,
         fssExecVol_,
         fssSubVol_);

   theFssMrpExp.mrp ();

   mrpIncAvail_.clear ();
   }

//------------------------------------------------------------------------------
// Implementation of class FssMrpExp.
//------------------------------------------------------------------------------

WitFssMrpExp::WitFssMrpExp (
         WitProblem *                         theProblem,
         WitFSS *                             theFSS,
         bool &                               uploadingSolnArg,
         WitSchedule <WitOperation, double> & fssExecVolArg,
         WitSchedule <WitSubEntry,  double> & fssSubVolArg):

      WitMrpExp      (theProblem),

      myFSS_         (theFSS),
      uploadingSoln_ (uploadingSolnArg),
      fssExecVol_    (fssExecVolArg),
      fssSubVol_     (fssSubVolArg)
   {
   }

//------------------------------------------------------------------------------

WitFssMrpExp::~WitFssMrpExp ()
   {
   }

//------------------------------------------------------------------------------

double WitFssMrpExp::supplyVol (WitPart * thePart, WitPeriod thePer) 
   {
   return myFSS_->mrpSupplyVol (thePart, thePer);
   }

//------------------------------------------------------------------------------

double WitFssMrpExp::demandVol (WitPart * thePart, WitPeriod thePer)
   {
   return myFSS_->mrpDemandVol (thePart, thePer);
   }

//------------------------------------------------------------------------------

double & WitFssMrpExp::execVol (WitOperation * theOpn, WitPeriod thePer)
   {
   return fssExecVol_ (theOpn)[thePer];
   }

//------------------------------------------------------------------------------

double & WitFssMrpExp::subVol (WitSubEntry * theSub, WitPeriod thePer)
   {
   return fssSubVol_ (theSub)[thePer];
   }

//------------------------------------------------------------------------------

void WitFssMrpExp::finishDerived ()
   {
   WitPart *      thePart;
   WitOperation * theOpn;
   WitSubEntry *  theSub;

   uploadingSoln_ = true;

   forEachPart (thePart, myProblem ())
      thePart->storeFssFocShortageVol (myReqVol () (thePart));

   forEachOperation (theOpn, myProblem ())
      theOpn->storeFssFssExecVol      (fssExecVol_ (theOpn));

   forEachSubEntry (theSub, myProblem ())
      theSub->storeFssFssSubVol       (fssSubVol_  (theSub));

   uploadingSoln_ = false;
   }
