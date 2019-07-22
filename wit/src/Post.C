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
// Source file: "Post.C"
//
// Contains the implementation of class Postprocessor.
//------------------------------------------------------------------------------

#include <Post.h>
#include <FeasChkr.h>
#include <Pre.h>
#include <CompMgr.h>
#include <BoundSet.h>
#include <PipMgr.h>
#include <Timing.h>
#include <HeurAllMgr.h>
#include <FSS.h>
#include <wit/src/Variant.h>
#include <GlobalComp.h>
#include <OptComp.h>
#include <Material.h>
#include <Demand.h>
#include <Operation.h>
#include <BomEntry.h>
#include <SubEntry.h>
#include <BopEntry.h>
#include <MsgFrag.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class Postprocessor.
//------------------------------------------------------------------------------

WitPostprocessor::WitPostprocessor (WitProblem * theProblem):

      WitProbAssoc    (theProblem),

      postprocessing_ (false),
      postprocessed_  (false),
      myFeasChkr_     (NULL)
   {
   myFeasChkr_ = new WitFeasChkr (myProblem ());
   }

//------------------------------------------------------------------------------

WitPostprocessor::~WitPostprocessor ()
   {
   delete myFeasChkr_;
   }

//------------------------------------------------------------------------------

void WitPostprocessor::postprocess ()
   {
   WitDemand * theDemand;

   if (postprocessed_)
      return;

   myProblem ()->myHeurAllMgr ()->reset ();

   myPreprocessor ()->preprocess ();

   WitTimer::enterSection ("postproc");

   postprocessing_ = true;

   forEachDemand (theDemand, myProblem ())
      theDemand->initFssShipVolForPost ();

   compNonSubVols ();

   compConsVols ();
   compProdVols ();

   if (not myProblem ()->currentVariant ()->compsSecScheds ())
      {
      compCapacityScrapVols      ();
      compMaterialStockScrapVols ();
      }
 
   compCumShipVols  ();
   compCapResidVols ();

   if (myGlobalComp ()->nstnResidual ())
      compMatResidVolsNstn ();
   else
      compMatResidVolsAsap ();

   myFeasChkr_->compFeasible ();

   postprocessing_ = false;

   postprocessed_  =
      myMsgFac ().displayStateChange (
        "postStateMsg",
         postprocessed_,
         true);

   WitTimer::leaveSection ("postproc");
   }

//------------------------------------------------------------------------------

void WitPostprocessor::unpostprocess ()
   {
   if (not postprocessed_)
      return;

   myFeasChkr_    ->unpostprocess ();
   myOptComp    ()->unpostprocess ();
   myGlobalComp ()->unpostprocess ();

   myProblem ()->myPipMgr ()->clearPegging ();

   myProblem ()->myFSS ()->uncompute ();

   postprocessed_ =
      myMsgFac ().displayStateChange (
        "unpostStateMsg",
         postprocessed_,
         false);
   }

//------------------------------------------------------------------------------

void WitPostprocessor::compNonSubVol (
      WitBomEntry *      theBomEnt,
      WitTVec <double> & nonSubVolVec)
   {
   WitSubEntry * theSub;
   WitPeriod     execPer;

   nonSubVolVec = theBomEnt->myOperation ()->execVol ();

   forEachPeriod (execPer, myProblem ())
      forEachEl (theSub, theBomEnt->mySubEntries ())
         if (theSub->inEffect (execPer))
            nonSubVolVec[execPer] -= theSub->subVol ()[execPer];
   }

//------------------------------------------------------------------------------

void WitPostprocessor::compProdVol (
      WitPart *          thePart,
      WitTVec <double> & prodVolVec)
   {
   WitBopEntry * theBopEnt;
   WitPeriod     execPer;

   prodVolVec = 0.0;

   forEachEl (theBopEnt, thePart->producingBopEntries ())
      forEachPeriod (execPer, myProblem ())
         if (theBopEnt->inEffect (execPer))
            prodVolVec[theBopEnt->impactPeriod ()[execPer]] +=
               theBopEnt->effProdRate ()[execPer] *
               theBopEnt->myOperation ()->execVol ()[execPer];
   }

//------------------------------------------------------------------------------

double WitPostprocessor::compExcessVol (WitPart * thePart, WitPeriod thePer)
   {
   double        excVol;
   WitMaterial * theMat;

   excVol = thePart->residualVol ()[thePer];

   if (myGlobalComp ()->minimalExcess ())
      {
      excVol -= thePart->prodVol ()[thePer];

      excVol -= positivePart (- thePart->consVol ()[thePer]);

      theMat  = thePart->thisMat ();

      if (theMat != NULL)
         if (thePer > 0)
            excVol -= stockLB (theMat, thePer - 1);

      excVol  = positivePart (excVol);
      }

   setToMin (excVol, thePart->supplyVol ()[thePer]);

   return excVol;
   }

//------------------------------------------------------------------------------

void WitPostprocessor::compNonSubVols ()
   {
   WitTVec <double> nonSubVolVec;
   WitBomEntry *    theBomEnt;

   nonSubVolVec.allocate  (myProblem (), 0.0);

   forEachBomEntry (theBomEnt, myProblem ())
      if (not theBomEnt->mySubEntries ().isEmpty ())
         {
         compNonSubVol (theBomEnt, nonSubVolVec);

         theBomEnt->storePostNonSubVol (nonSubVolVec);
         }
   }

//------------------------------------------------------------------------------

void WitPostprocessor::compConsVols ()
   {
   WitPart *        thePart;
   WitBomEntry *    theBomEnt;
   WitSubEntry *    theSub;
   WitTVec <double> consVolVec (myProblem (), 0.0);
   WitTVec <double> execVolVec (myProblem (), 0.0);

   forEachPart (thePart, myProblem ())
      {
      consVolVec = 0.0;

      forEachEl (theBomEnt, thePart->consumingBomEntries ())
         {
         execVolVec = theBomEnt->nonSubVol ();

         theBomEnt->incConsVol (consVolVec, execVolVec);
         }

      forEachEl (theSub, thePart->consumingSubEntries ())
         {
         execVolVec = theSub->subVol ();

         theSub->incConsVol (consVolVec, execVolVec);
         }

      thePart->storePostConsVol (consVolVec);
      }
   }

//------------------------------------------------------------------------------

void WitPostprocessor::compProdVols ()
   {
   WitPart *        thePart;
   WitTVec <double> prodVolVec (myProblem (), 0.0);

   forEachPart (thePart, myProblem ())
      {
      compProdVol (thePart, prodVolVec);

      thePart->storePostProdVol (prodVolVec);
      }
   }

//------------------------------------------------------------------------------

void WitPostprocessor::compCapacityScrapVols ()
   {
   WitPart *        thePart;
   WitPeriod        thePer;
   WitTVec <double> scrapVolVec (myProblem (), 0.0);

   forEachPart (thePart, this)
      if (thePart->thisCap () != NULL)
         {
         forEachPeriod (thePer, myProblem ())
            scrapVolVec[thePer] = unstockedScrapVol (thePart, thePer);

         thePart->storePostScrapVol (scrapVolVec);
         }
   }

//------------------------------------------------------------------------------
// compMaterialStockScrapVols.
// Computes scrapVol and stockVol.
// This is done by rolling the inventory forward in time, allocating as much of
// it as possible to stock and scrapping the rest. Then rollBackScrap is
// applied. Lower bounds on stock are respected to the extent possible given
// the production and shipment schedules. Upper bounds on stock are respected
// if appropriate for the currentVariant. If the solution implies a negative
// stockVol or scrapVol (due to a numerical problem or a genuine constraint
// violation), the negative value is assigned to the scrapVol rather than the
// stockVol.
//------------------------------------------------------------------------------

void WitPostprocessor::compMaterialStockScrapVols ()
   {
   WitMaterial * theMat;
   WitPeriod     thePer;
   double        rollingInv;  // Rolling inventory in thePer.
   double        stockVolVal; // stockVol[thePer].

   WitTVec <double> stockVolVec (myProblem (), 0.0);
   WitTVec <double> scrapVolVec (myProblem (), 0.0);

   forEachMaterial (theMat, myProblem ())
      {
      forEachPeriod (thePer, myProblem ())
         {
         rollingInv = unstockedScrapVol (theMat, thePer);

         if (thePer > 0)
            rollingInv += stockVolVec[thePer - 1];

         stockVolVal = rollingInv;

         if (not theMat->canStock (thePer))
            stockVolVal = 0.0;

         if (myProblem ()->currentVariant ()->respectsOptCons ())
            if (theMat->stockBounds ()->hardUBIsFinite (thePer))
               setToMin (
                  stockVolVal,
                  theMat->stockBounds ()->hardUB ()[thePer]);

         if (stockVolVal < 0.0)
             stockVolVal = 0.0;

         stockVolVec[thePer] = stockVolVal;
         scrapVolVec[thePer] = rollingInv - stockVolVal;
         }

      theMat->rollBackScrap (stockVolVec, scrapVolVec);

      theMat->storePostStockVol (stockVolVec);
      theMat->storePostScrapVol (scrapVolVec);
      }
   }

//------------------------------------------------------------------------------

double WitPostprocessor::unstockedScrapVol (WitPart * thePart, WitPeriod thePer)
   {
   WitDemand * theDemand;
   double      usVol;

   usVol =
        thePart->supplyVol ()[thePer]
      + thePart->prodVol   ()[thePer]
      - thePart->consVol   ()[thePer];

   forEachEl (theDemand, thePart->myDemands ())
      usVol -= theDemand->shipVol ()[thePer];

   return usVol;
   }

//------------------------------------------------------------------------------

void WitPostprocessor::compCumShipVols ()
   {
   WitTVec <double> cumShipVolVec;
   WitDemand *      theDemand;
   double           cumSum;
   WitPeriod        thePer;

   cumShipVolVec.allocate (myProblem (), 0.0);

   forEachDemand (theDemand, myProblem ())
      {
      if (myProblem ()->currentVariant ()->compsSecScheds ())
         if (theDemand->shipLateAllowed ())
            continue;

      if (theDemand->shipVol () == 0.0)
         cumShipVolVec = 0.0;
      else
         {
         cumSum = 0.0;

         forEachPeriod (thePer, myProblem ())
            {
            cumSum               += theDemand->shipVol ()[thePer];

            cumShipVolVec[thePer] = cumSum;
            }
         }

      theDemand->storePostCumShipVol (cumShipVolVec);
      }
   }

//------------------------------------------------------------------------------

void WitPostprocessor::compCapResidVols ()
   {
   WitTVec <double> resVolVec;
   WitPart *        thePart;
   WitPeriod        thePer;

   resVolVec.allocate (myProblem (), 0.0);

   forEachPart (thePart, myProblem ())
      if (thePart->thisCap () != NULL)
         {
         resVolVec = thePart->scrapVol ();

         forEachPeriod (thePer, myProblem ())
            if (resVolVec[thePer] <= NET_TOL)
               resVolVec[thePer] = 0.0;

         thePart->storePostResidualVol (resVolVec);
         }
   }

//------------------------------------------------------------------------------
// compMatResidVolsAsap.
// Computes the residualVols as follows:
// Create a temporary usedStockVol, the portion of stockVol that is used by the
//    solution and let residualVol be the corresponding scrapVol.
// Let usedStockVol and residualVol reflect the following scrapping policy:
// * Any stock in the last period in excess of the softLB on stock is scrapped.
// * All scrapping is done in the earliest possible period.
//------------------------------------------------------------------------------

void WitPostprocessor::compMatResidVolsAsap ()
   {
   WitTVec <double> resVolVec;
   WitTVec <double> usedStockVol;
   WitMaterial *    theMat;
   double           unusedFinalStock;
   WitPeriod        lastPer;
   WitPeriod        thePer;

   resVolVec   .allocate (myProblem (), 0.0);
   usedStockVol.allocate (myProblem (), 0.0);

   lastPer = lastPeriod ();

   forEachMaterial (theMat, myProblem ())
      {
      usedStockVol = theMat->stockVol ();
      resVolVec    = theMat->scrapVol ();

      unusedFinalStock =
         usedStockVol[lastPer] - theMat->stockBounds ()->softLB ()[lastPer];

      if (unusedFinalStock > FLOAT_EPSILON)
         {
         usedStockVol[lastPer] -= unusedFinalStock;
         resVolVec   [lastPer] += unusedFinalStock;
         }

      theMat->rollBackScrap (usedStockVol, resVolVec);

      forEachPeriod (thePer, myProblem ())
         setToMax (resVolVec[thePer], 0.0);

      theMat->storePostResidualVol (resVolVec);
      }
   }

//------------------------------------------------------------------------------

void WitPostprocessor::compMatResidVolsNstn ()
   {
   WitTVec <double> resVolVec;
   WitTVec <double> unstockConsVol;
   WitMaterial *    theMat;
   double           netConsVol;
   WitPeriod        thePer;
   double           resVol;

   resVolVec     .allocate (myProblem (), 0.0);
   unstockConsVol.allocate (myProblem (), 0.0);

   forEachMaterial (theMat, myProblem ())
      {
      netConsVol = compCumConsVol (theMat, unstockConsVol);

      resVolVec  = 0.0;

      forEachPeriod (thePer, myProblem ())
         {
         if (thePer > 0)
            netConsVol += unstockConsVol[thePer];

         resVol     =                 theMat->supplyVol ()[thePer];

         resVol    +=                 theMat->prodVol   ()[thePer];

         resVol    += positivePart (- theMat->consVol   ()[thePer]);

         if (thePer > 0)
            resVol += stockLB (theMat, thePer - 1);

         net (resVol, netConsVol);

         if (resVol > NET_TOL)
            resVolVec[thePer] = resVol;
         }

      theMat->storePostResidualVol (resVolVec);
      }
   }

//------------------------------------------------------------------------------

double WitPostprocessor::compCumConsVol (
      WitMaterial *      theMat, 
      WitTVec <double> & unstockConsVol)
   {
   double      cumConsVol;
   WitPeriod   thePer;
   WitDemand * theDemand;
   double      boundGap;
   bool        gapIsFinite;

   cumConsVol     = 0.0;

   unstockConsVol = 0.0;

   for (thePer = lastPeriod (); thePer >= 0; thePer --)
      {
      forEachEl (theDemand, theMat->myDemands ())
         cumConsVol += theDemand->shipVol ()[thePer];

      cumConsVol    += positivePart (theMat->consVol ()[thePer]);

      cumConsVol    += stockLB (theMat, thePer);

      if (thePer > 0)
         {
         boundGap = stockBndGap (theMat, thePer - 1, gapIsFinite);

         if (gapIsFinite)
            if (boundGap < cumConsVol - NET_TOL)
               {
               unstockConsVol[thePer] = cumConsVol - boundGap;

               cumConsVol             = boundGap;
               }
         }
      }

   return cumConsVol;
   }

//------------------------------------------------------------------------------

double WitPostprocessor::stockBndGap (
      WitMaterial * theMat,
      WitPeriod     thePer,
      bool &        gapIsFinite)
   {
   gapIsFinite = true;

   if (not theMat->canStock (thePer))
      return 0.0;

   if (not theMat->stockBounds ()->hardUBIsFinite (thePer))
      {
      gapIsFinite = false;

      return 0.0;
      }

   return
        theMat->stockBounds ()->hardUB ()[thePer]
      - stockLB (theMat, thePer);
   }

//------------------------------------------------------------------------------

double WitPostprocessor::stockLB (WitMaterial * theMat, WitPeriod thePer)
   {
   return
      min (
         theMat->stockBounds ()->softLB ()[thePer],
         theMat->stockVol    ()           [thePer]);
   }
