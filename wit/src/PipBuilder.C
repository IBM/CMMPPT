//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "PipBuilder.C"
//
// Contains the implementation of the following classes and templates:
//
//    PipBuilder
//    PipIncPgg <RcpComp>, with explicit instantiation
//------------------------------------------------------------------------------

#include <PipBuilder.h>
#include <PipIncPgg.h>
#include <PipReqMgr.h>
#include <PipPartReqMgr.h>
#include <PipReqBasis.h>
#include <PipReqClient.h>
#include <PipMgr.h>
#include <PipPgg.h>
#include <PipAttPgg.h>
#include <PipRcpPgg.h>
#include <ExecPerSch.h>
#include <GlobalComp.h>
#include <Material.h>
#include <Operation.h>
#include <BomEntry.h>
#include <SubEntry.h>
#include <BopEntry.h>
#include <Demand.h>
#include <Post.h>
#include <Mapping.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class PipBuilder.
//------------------------------------------------------------------------------

WitPipBuilder::WitPipBuilder (WitPipPgg * thePipPgg):

      WitProbAssoc    (thePipPgg),
      myPipPgg_       (thePipPgg),
      myReqMgr_       (NULL),
      myPartReqMgr_   (NULL),
      myExecPerSched_ (NULL),
      sideVol_        (myProblem (), 0.0),
      myIncPggToDems_ (NULL),
      myIncPggToOpns_ (NULL)
   {
   myReqMgr_       = new WitPipReqMgr                   (myProblem ());
   myPartReqMgr_   = new WitPipPartReqMgr               (myReqMgr_);
   myExecPerSched_ = new WitExecPerSched <WitBopEntry>  (myProblem ());
   myIncPggToDems_ = new WitPipIncPgg    <WitDemand>    (myProblem ());
   myIncPggToOpns_ = new WitPipIncPgg    <WitOperation> (myProblem ());

   compSideVols (sideVol_);
   }

//------------------------------------------------------------------------------

WitPipBuilder::~WitPipBuilder ()
   {
   delete myIncPggToOpns_;
   delete myIncPggToDems_;
   delete myExecPerSched_;
   delete myPartReqMgr_;
   delete myReqMgr_;
   }

//------------------------------------------------------------------------------

void WitPipBuilder::buildPegging ()
   {
   validateData ();

   placeShipmentReqs ();

   explodeAllReqs ();

   if (devMode ())
      myReqMgr_->prtSizes ();

   if (myGlobalComp ()->prtPipMaxRatio ())
      dispMaxPegRatio ();

   myPipPgg_->consolidatePeggings ();

   myPartReqMgr_->prtInfeas ();
   }

//------------------------------------------------------------------------------

void WitPipBuilder::validateData ()
   {
   WitMaterial *  theMat;
   WitOperation * theOpn;

   if (not myGlobalComp ()->newPipPggOrder ())
      {
      forEachMaterial (theMat, myProblem ())
         if (theMat->asapPipOrder ())
            myMsgFac () ("asapPipOrderOpoSmsg", theMat->partName ());

      forEachOperation (theOpn, myProblem ())
         if (theOpn->pipEnabled ())
            myMsgFac () ("pipEnabledOpoSmsg", theOpn->operationName ());
      }
   }

//------------------------------------------------------------------------------

void WitPipBuilder::compSideVols (WitSchedule <WitPart, double> & sideVolSched)
   {
   compBopSideVols (sideVolSched);
   compBomSideVols (sideVolSched);

   if (selPrintLevelAt (3))
      prtSideVols (sideVolSched);
   }

//------------------------------------------------------------------------------

void WitPipBuilder::compBopSideVols (
      WitSchedule <WitPart, double> & sideVolSched)
   {
   WitBopEntry * theBopEnt;
   WitPart *     thePart;
   WitPeriod     execPer;
   double        execVolVal;
   WitPeriod     prodPer;

   forEachBopEntry (theBopEnt, myProblem ())
      {
      thePart = theBopEnt->myPart ();

      forEachPeriod (execPer, myProblem ())
         {
         if (not theBopEnt->inEffect (execPer))
            continue;

         if (theBopEnt->expAllowed ())
            if (theBopEnt->execPerOKForExp (execPer))
               continue;

         execVolVal = theBopEnt->myOperation ()->execVol ()[execPer];

         if (execVolVal <= 0.0)
            continue;

         prodPer = theBopEnt->impactPeriod ()[execPer];

         sideVolSched (thePart)[prodPer] +=
            execVolVal * theBopEnt->effProdRate ()[execPer];
         }
      }
   }

//------------------------------------------------------------------------------

void WitPipBuilder::compBomSideVols (
      WitSchedule <WitPart, double> & sideVolSched)
   {
   WitBomEntry * theBomEnt;
   WitPeriod     execPer;
   double        effConsRateVal;
   double        execVolVal;
   WitPart *     thePart;
   WitPeriod     consPer;
   double        incConsVol;

   forEachBomEntry (theBomEnt, myProblem ())
      forEachPeriod (execPer, myProblem ())
         {
         if (not theBomEnt->inEffect (execPer))
            continue;

         effConsRateVal = theBomEnt->effConsRate ()[execPer];

         if (effConsRateVal >= 0.0)
            continue;

         execVolVal = theBomEnt->myOperation ()->execVol ()[execPer];

         if (execVolVal <= 0.0)
            continue;

         thePart    = theBomEnt->myPart ();

         consPer    = theBomEnt->impactPeriod ()[execPer];

         incConsVol = execVolVal * effConsRateVal;

         sideVolSched (thePart)[consPer] -= incConsVol;
         }
   }

//------------------------------------------------------------------------------

void WitPipBuilder::prtSideVols (
      const WitSchedule <WitPart, double> & sideVolSched)
   {
   WitPart * thePart;
   WitPeriod thePer;
   double    sideVolVal;

   fprintf (msgFile (),
      "\n"
      "Side Effect Volumes:\n"
      "\n"
      "   Part Per SideVol\n");

   forEachPart (thePart, myProblem ())
      forEachPeriod (thePer, myProblem ())
         {
         sideVolVal = sideVolSched (thePart)[thePer];

         if (sideVolVal == 0.0)
            continue;

         fprintf (msgFile (),
            "   %4s %3d %7.2f\n",
            thePart->partName ().myCstring (),
            thePer,
            sideVolVal);
         }

   fprintf (msgFile (), "\n");
   }

//------------------------------------------------------------------------------

void WitPipBuilder::placeShipmentReqs ()
   {
   WitDemTripStack  theShipSeq (myProblem ());
   int              theSeqNo;
   WitDemand *      theDemand;
   WitPeriod        shipPer;
   double           incShipVol;
   WitPipReqBasis * theBasis;
   WitPart *        thePart;

   myProblem ()->myPipMgr ()->getShipSeq (theShipSeq);

   theSeqNo = -1;

   while (theShipSeq.pop (theDemand, shipPer, incShipVol))
      {
      theSeqNo ++;

      theBasis = myReqMgr_->newBasisForSeqNo (theSeqNo);

      thePart  = theDemand->demandedPart ();

      myReqMgr_->addReq (thePart, shipPer, theBasis, incShipVol);
      }
   }

//------------------------------------------------------------------------------

void WitPipBuilder::explodeAllReqs ()
   {
   WitObjStack <WitPart>          unexpParts (myProblem ());
   WitMapping <WitOperation, int> nUnexpBopEnts;
   WitPart *                      thePart;
   WitBopEntry *                  theBopEnt;
   WitOperation *                 theOpn;

   revCopyInto (unexpParts, myCompMgr ()->allParts ());

   nUnexpBopEnts.allocate (myProblem (), 0);

   forEachBopEntry (theBopEnt, myProblem ())
      if (theBopEnt->expAllowed ())
         nUnexpBopEnts (theBopEnt->myOperation ()) ++;

   while (unexpParts.pop (thePart))
      {
      fillPartReqs (thePart);

      forEachEl (theBopEnt, thePart->producingBopEntries ())
         if (theBopEnt->expAllowed ())
            {
            theOpn = theBopEnt->myOperation ();

            nUnexpBopEnts (theOpn) --;

            if (nUnexpBopEnts (theOpn) == 0)
               fillOpnReqs (theOpn);
            }
      }
   }

//------------------------------------------------------------------------------

void WitPipBuilder::fillPartReqs (WitPart * thePart)
   {
   myPartReqMgr_->setUpPartReqs (thePart);

   if (myGlobalComp ()->newPipPggOrder ())
      pegToAllAttsNpo (thePart);
   else
      pegToAllAttsOpo (thePart);

   myPartReqMgr_->recordInfeas (thePart);
   }

//------------------------------------------------------------------------------

void WitPipBuilder::pegToAllAttsNpo (WitPart * thePart)
   {
   if (thePart->supplyVol () != 0.0)
      pegToAtt (thePart, & WitPipBuilder::netWithSupply);

   if (thePart->prodVol () != 0.0)
      pegToAtt (thePart, & WitPipBuilder::explodeToProd);

   if (not (sideVol_ (thePart) == 0.0))
      pegToAtt (thePart, & WitPipBuilder::netWithSideVol);
   }

//------------------------------------------------------------------------------

void WitPipBuilder::pegToAtt (
      WitPart *              thePart,
      void (WitPipBuilder::* thePegFunc) (WitPart *, WitPeriod))
   {
   WitPeriod thePer;

   for (thePer = lastPeriod (); thePer >= 0; thePer --)
      {
      myPartReqMgr_->mergeCurPartReqsIntoWorkSeq (thePer);

      if (not asapPggOrder (thePart))
         (this->*thePegFunc) (thePart, thePer);

      if (thePer > 0)
         myPartReqMgr_->rollReqsBack (thePer);
      }

   forEachPeriod (thePer, myProblem ())
      {
      if (thePer > 0)
         myPartReqMgr_->rollReqsFwd (thePer);

      if (asapPggOrder (thePart))
         (this->*thePegFunc) (thePart, thePer);

      myPartReqMgr_->restoreUnfilledCurPartReqs (thePer);
      }
   }

//------------------------------------------------------------------------------

bool WitPipBuilder::asapPggOrder (WitPart * thePart)
   {
   WitMaterial * theMat;

   theMat = thePart->thisMat ();

   if (theMat != NULL)
      if (theMat->asapPipOrder ())
         return true;

   return false;
   }

//------------------------------------------------------------------------------

void WitPipBuilder::pegToAllAttsOpo (WitPart * thePart)
   {
   WitPeriod thePer;

   for (thePer = lastPeriod (); thePer >= 0; thePer --)
      {
      myPartReqMgr_->mergeCurPartReqsIntoWorkSeq (thePer);

      netWithSupply (thePart, thePer);
      explodeToProd (thePart, thePer);

      if (thePer > 0)
         myPartReqMgr_->rollReqsBack (thePer);
      }

   forEachPeriod (thePer, myProblem ())
      {
      if (thePer > 0)
         myPartReqMgr_->rollReqsFwd (thePer);

      netWithSideVol (thePart, thePer);

      myPartReqMgr_->restoreUnfilledCurPartReqs (thePer);
      }
   }

//------------------------------------------------------------------------------

void WitPipBuilder::netWithSupply (WitPart * thePart, WitPeriod thePer)
   {
   double nonExcSuppVol;

   if (myPartReqMgr_->workSeqIsEmpty ())
      return;

   if (myGlobalComp ()->printDebug ())
      myPartReqMgr_->prtWorkReqSeq ("SupplyVol", thePart, thePer);

   nonExcSuppVol =
        thePart->supplyVol ()[thePer]
      - myProblem ()->myPostprocessor ()->compExcessVol (thePart, thePer);

   pegAttToWorkReqs (
      nonExcSuppVol, 
      myPipPgg_->supplyVolPgg (),
      thePart,
      thePer,
      NULL);
   }

//------------------------------------------------------------------------------

void WitPipBuilder::explodeToProd (WitPart * thePart, WitPeriod thePer)
   {
   WitBopEntry * theBopEnt;

   if (myPartReqMgr_->workSeqIsEmpty ())
      return;

   if (thePart->prodVol ()[thePer] <= 0.0)
      return;

   if (not thePart->explodeable (thePer))
      return;

   if (myGlobalComp ()->printDebug ())
      myPartReqMgr_->prtWorkReqSeq ("ProdVol", thePart, thePer);

   forEachEl (theBopEnt, thePart->producingBopEntries ())
      if (theBopEnt->expEligible (thePer))
         {
         explodeBopEntry (theBopEnt, thePer);

         if (myPartReqMgr_->workSeqIsEmpty ())
            return;
         }
   }

//------------------------------------------------------------------------------

void WitPipBuilder::explodeBopEntry (WitBopEntry * theBopEnt, WitPeriod thePer)
   {
   WitPeriod execPerFirst;
   WitPeriod execPerLast;
   WitPeriod execPer;

   witAssert (theBopEnt->expEligible (thePer));

   witAssert (not myPartReqMgr_->workSeqIsEmpty ());

   myExecPerSched_->
      getExecPerRange (theBopEnt, thePer, execPerFirst, execPerLast);

   for (execPer = execPerLast; execPer >= execPerFirst; execPer --)
      if (theBopEnt->execPerOKForExp (execPer))
         if (theBopEnt->impactPeriod ()[execPer] == thePer)
            {
            explodeToExecPer (theBopEnt, execPer);

            if (myPartReqMgr_->workSeqIsEmpty ())
               return;
            }
   }

//------------------------------------------------------------------------------

void WitPipBuilder::explodeToExecPer (
      WitBopEntry * theBopEnt,
      WitPeriod     execPer)
   {
   double           theProdVol;
   WitPipReqList    pgdReqList (myProblem ());
   WitPipReqBasis * theBasis;
   double           incProdVol;
   double           incExecVol;

   witAssert (not myPartReqMgr_->workSeqIsEmpty ());

   theProdVol =
        theBopEnt->myOperation ()->execVol ()[execPer]
      * theBopEnt->effProdRate ()            [execPer];

   pegAttToWorkReqs (
        theProdVol,
        myPipPgg_->prodVolPgg (),
        theBopEnt->myPart (),
        theBopEnt->impactPeriod ()[execPer],
      & pgdReqList);

   pgdReqList.reverse ();

   while (pgdReqList.pop (theBasis, incProdVol))
      {
      incExecVol = incProdVol / theBopEnt->effProdRate ()[execPer];

      addToIncPegVol (incExecVol, theBasis);

      myReqMgr_->addReq (theBopEnt, execPer, theBasis, incExecVol);
      }
   
   myPipPgg_->coExecVolPgg ()->appendIncPgg (theBopEnt, execPer);
   }

//------------------------------------------------------------------------------

void WitPipBuilder::netWithSideVol (WitPart * thePart, WitPeriod thePer)
   {
   if (myPartReqMgr_->workSeqIsEmpty ())
      return;

   if (myGlobalComp ()->printDebug ())
      myPartReqMgr_->prtWorkReqSeq ("SideVol", thePart, thePer);

   pegAttToWorkReqs (
      sideVol_ (thePart)[thePer],
      myPipPgg_->sideVolPgg (),
      thePart,
      thePer,
      NULL);
   }

//------------------------------------------------------------------------------

void WitPipBuilder::pegAttToWorkReqs (
      double                   theAttVol,
      WitPipAttPgg <WitPart> * theAttPgg,
      WitPart *                thePart,
      WitPeriod                thePer,
      WitPipReqList *          pgdReqListPtr)
   {
   double           netAttVol;
   WitPipReqBasis * theBasis;
   double           incPegVol;

   witAssert (not myPartReqMgr_->workSeqIsEmpty ());

   netAttVol = theAttVol;

   if (pgdReqListPtr != NULL)
      pgdReqListPtr->clear ();

   while ((not myPartReqMgr_->workSeqIsEmpty ()) and (netAttVol > NET_TOL))
      {
      myPartReqMgr_->getNextReq (netAttVol, theBasis, incPegVol);

      addToIncPegVol (incPegVol, theBasis);

      if (pgdReqListPtr != NULL)
         pgdReqListPtr->push (theBasis, incPegVol);
      }

   theAttPgg->appendIncPgg (thePart, thePer);
   }

//------------------------------------------------------------------------------

void WitPipBuilder::fillOpnReqs (WitOperation * theOpn)
   {
   WitPeriod        execPer;
   WitPipReqBasis * theBasis;
   double           incExecVol;
   WitBomEntry *    theBomEnt;

   for (execPer = lastPeriod (); execPer >= 0; execPer --)
      {
      if (not myReqMgr_->hasReqsFor (theOpn, execPer))
         continue;

      myReqMgr_->loadCurReqList (theOpn, execPer);

      myReqMgr_->initReqListItr ();

      while (not myReqMgr_->finishedReqListItr ())
         {
         myReqMgr_->advanceReqListItr (theBasis, incExecVol);

         addToIncPegVol (incExecVol, theBasis);
         }

      myPipPgg_->execVolPgg ()->appendIncPgg (theOpn, execPer);

      if (theOpn->pipEnabled ())
         myReqMgr_->addOpnClientToCurReqs (theOpn, execPer);

      forEachEl (theBomEnt, theOpn->bom ())
         explodeBomEntry (theBomEnt, execPer);

      myReqMgr_->clearCurReqList ();
      }
   }

//------------------------------------------------------------------------------

void WitPipBuilder::explodeBomEntry (
      WitBomEntry * theBomEnt,
      WitPeriod     execPer)
   {
   WitSubEntry * theSub;

   if (not theBomEnt->inEffect (execPer))
      return;

   pegSubVols (theBomEnt, execPer);

   myReqMgr_->initReqListItr ();

   explodeConsEntry (
      theBomEnt,
      execPer,
      theBomEnt->nonSubVol ()[execPer]);

   forEachEl (theSub, theBomEnt->mySubEntries ())
      {
      if (myReqMgr_->finishedReqListItr ())
         return;

      explodeConsEntry (
         theSub,
         execPer,
         theSub->subVol ()[execPer]);
      }
   }

//------------------------------------------------------------------------------

void WitPipBuilder::pegSubVols (WitBomEntry * theBomEnt, WitPeriod execPer)
   {
   double           netNonSubVol;
   WitPipReqBasis * theBasis;
   double           incNonSubVol;
   WitSubEntry *    theSub;
   double           netSubVol;
   double           incSubVol;

   if (not theBomEnt->hasSubsInEffect ()[execPer])
      return;

   myReqMgr_->initReqListItr ();

   netNonSubVol = theBomEnt->nonSubVol ()[execPer];

   while (netNonSubVol > NET_TOL)
      {
      if (myReqMgr_->finishedReqListItr ())
         return;

      myReqMgr_->advanceReqListItr (netNonSubVol, theBasis, incNonSubVol);
      }

   forEachEl (theSub, theBomEnt->mySubEntries ())
      {
      netSubVol = theSub->subVol ()[execPer];

      while (netSubVol > NET_TOL)
         {
         if (myReqMgr_->finishedReqListItr ())
            break;

         myReqMgr_->advanceReqListItr (netSubVol, theBasis, incSubVol);

         addToIncPegVol (incSubVol, theBasis);
         }

      myPipPgg_->subVolPgg ()->appendIncPgg (theSub, execPer);

      if (myReqMgr_->finishedReqListItr ())
         return;
      }
   }

//------------------------------------------------------------------------------

void WitPipBuilder::explodeConsEntry (
      WitConsEntry * theConsEnt,
      WitPeriod      execPer,
      double         totExecVol)
   {
   WitPart *        consPart;
   WitPeriod        consPer;
   double           netExecVol;
   WitPipReqBasis * theBasis;
   double           incExecVol;
   double           incConsVol;

   if (totExecVol <= NET_TOL)
      return;

   consPart   = theConsEnt->myPart ();

   consPer    = theConsEnt->impactPeriod ()[execPer];

   netExecVol = totExecVol;

   while (netExecVol > NET_TOL)
      {
      if (myReqMgr_->finishedReqListItr ())
         break;

      myReqMgr_->advanceReqListItr (netExecVol, theBasis, incExecVol);

      incConsVol = incExecVol * theConsEnt->effConsRate ()[execPer];

      if (incConsVol <= NET_TOL)
         continue;

      addToIncPegVol (incConsVol, theBasis);

      myReqMgr_->addReq (consPart, consPer, theBasis, incConsVol);
      }

   myPipPgg_->consVolPgg ()->appendIncPgg (consPart, consPer);
   }

//------------------------------------------------------------------------------

void WitPipBuilder::addToIncPegVol (double pegVol, WitPipReqBasis * theBasis)
   {
   myIncPggToDems_->addToIncPegVol (pegVol, theBasis);
   myIncPggToOpns_->addToIncPegVol (pegVol, theBasis);
   }

//------------------------------------------------------------------------------

void WitPipBuilder::dispMaxPegRatio ()
   {
   WitSchedule <WitOperation, double> totPgdExecVol;
   WitDemand *                        theDemand;
   WitPeriod                          shipPer;
   WitTripStack <WitOperation>        theOpnTrips (myProblem ());
   WitOperation *                     theOpn;
   WitPeriod                          execPer;
   double                             pgdExecVol;
   double                             ratio;
   double                             maxRatio;
   WitOperation *                     maxOpn;
   WitPeriod                          maxExecPer;

   totPgdExecVol.allocate (myProblem (), 0.0);

   forEachDemand (theDemand, myProblem ())
      forEachPeriod (shipPer, myProblem ())
         {
         myPipPgg_->
            execVolPgg ()->
               myPggToDems ()->
                  getPegTrips (theDemand, shipPer, theOpnTrips);

         while (theOpnTrips.pop (theOpn, execPer, pgdExecVol))
            totPgdExecVol (theOpn)[execPer] += pgdExecVol;
         }

   maxRatio = 0.0;

   maxOpn   = NULL;

   forEachOperation (theOpn, myProblem ())
      forEachPeriod (execPer, myProblem ())
         if (theOpn->execVol ()[execPer] > NET_TOL)
            {
            ratio =
                 totPgdExecVol (theOpn)[execPer]
               / theOpn->execVol ()    [execPer];

            if (ratio > maxRatio)
               {
               maxRatio   = ratio;
               maxOpn     = theOpn;
               maxExecPer = execPer;
               }
            }

   prtMaxPegRatio (maxRatio, maxOpn, maxExecPer);
   }

//------------------------------------------------------------------------------

void WitPipBuilder::prtMaxPegRatio (
      double         ratio,
      WitOperation * theOpn,
      WitPeriod      execPer)
   {
   WitDemand *                 theDemand;
   WitPeriod                   shipPer;
   WitTripStack <WitOperation> thePegTrips (myProblem ());
   WitOperation *              pgdOpn;
   WitPeriod                   pgdExecPer;
   double                      pgdExecVol;

   fprintf (msgFile (),
      "Max pegging ratio: %.3f\n",
      ratio);

   if (theOpn == NULL)
      return;

   fprintf (msgFile (),
      "\n"
      "Occurred at:\n"
      "   Operation:     %s\n"
      "   Exec Per:      %d\n"
      "   ExecVol:       %.3f\n\n",
      theOpn->operationName ().myCstring (),
      execPer,
      theOpn->execVol ()[execPer]);

   forEachDemand (theDemand, myProblem ())
      forEachPeriod (shipPer, myProblem ())
         {
         myPipPgg_->
            execVolPgg ()->
               myPggToDems ()->
                  getPegTrips (theDemand, shipPer, thePegTrips);

         while (thePegTrips.pop (pgdOpn, pgdExecPer, pgdExecVol))
            if    (pgdOpn     == theOpn)
               if (pgdExecPer == execPer)
                  {
                  fprintf (msgFile (),
                     "      Demand:     %s\n"
                     "      ShipPer:    %d\n"
                     "      PgdExecVol: %.3f\n\n",
                     theDemand->demandName ().myCstring (),
                     shipPer,
                     pgdExecVol);
                  }
         }
   }

//------------------------------------------------------------------------------
// Implementation of class template PipIncPgg <RcpComp>.
//------------------------------------------------------------------------------

template <typename RcpComp> 
      WitPipIncPgg <RcpComp>::WitPipIncPgg (WitProblem * theProblem):

         WitProbAssoc (theProblem),
         incPegVol_   (myProblem (), 0.0),
         addedTo_     (myProblem (), false),
         posRcpPairs_ (myProblem ())
   {
   }

//------------------------------------------------------------------------------

template <typename RcpComp> 
      WitPipIncPgg <RcpComp>::~WitPipIncPgg ()
   {
   }

//------------------------------------------------------------------------------

template <typename RcpComp> 
      void WitPipIncPgg <RcpComp>::addToIncPegVol (
         double           pegVol,
         WitPipReqBasis * theBasis)
   {
   WitPtrVecItr <WitPipReqClient <RcpComp> > theClientItr;
   RcpComp *                                 theRcpComp;
   WitPeriod                                 rcpPer;

   witAssert (pegVol > NET_TOL);

   theBasis->attachClientItr (theClientItr);

   while (advanceClientItr (theClientItr, theRcpComp, rcpPer))
      {
      if (addedTo_ (theRcpComp)[rcpPer])
         continue;

      if (incPegVol_ (theRcpComp)[rcpPer] == 0.0)
         posRcpPairs_.push (theRcpComp, rcpPer);

      incPegVol_ (theRcpComp)[rcpPer] += pegVol;
      addedTo_   (theRcpComp)[rcpPer]  = true;
      }

   theBasis->attachClientItr (theClientItr);

   while (advanceClientItr (theClientItr, theRcpComp, rcpPer))
      {
      addedTo_ (theRcpComp)[rcpPer] = false;
      }
   }

//------------------------------------------------------------------------------

template <typename RcpComp> 
      bool WitPipIncPgg <RcpComp>::getIncPggTrip (
         RcpComp * & theRcpComp,
         WitPeriod & rcpPer,
         double &    pegVol)
   {
   if (posRcpPairs_.pop (theRcpComp, rcpPer))
      {
      pegVol = incPegVol_ (theRcpComp)[rcpPer];

      incPegVol_ (theRcpComp)[rcpPer] = 0.0;

      return true;
      }
   else
      return false;
   }

//------------------------------------------------------------------------------

template <typename RcpComp> 
      bool WitPipIncPgg <RcpComp>::advanceClientItr (
         WitPtrVecItr <WitPipReqClient <RcpComp> > & theClientItr,
         RcpComp * &                                 theRcpComp,
         WitPeriod &                                 rcpPer)
   {
   WitPipReqClient <RcpComp> * theClient;

   if (theClientItr.advance (theClient))
      {
      theClient->getData (theRcpComp, rcpPer);

      return true;
      }

   return false;
   }

//------------------------------------------------------------------------------
// Explicit instantiation of class template PipIncPgg <RcpComp>.
//------------------------------------------------------------------------------

template class WitPipIncPgg <WitDemand>;
template class WitPipIncPgg <WitOperation>;
