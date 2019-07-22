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
// Source file: "FeasChkr.C"
//
// Contains the implementation of the following class:
//
//    FeasChkr
//------------------------------------------------------------------------------

#include <FeasChkr.h>
#include <Post.h>
#include <OptComp.h>
#include <GlobalComp.h>
#include <Material.h>
#include <Operation.h>
#include <BomEntry.h>
#include <SubEntry.h>
#include <Demand.h>
#include <BoundSet.h>
#include <CompMgr.h>
#include <wit/src/Variant.h>
#include <MsgFrag.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class FeasChkr.
//------------------------------------------------------------------------------

WitFeasChkr::WitFeasChkr (WitProblem * theProblem):

      WitProbAssoc  (theProblem),

      maxViolation_ (0.0)
   {
   }

//------------------------------------------------------------------------------

WitFeasChkr::~WitFeasChkr ()
   {
   }

//------------------------------------------------------------------------------

void WitFeasChkr::compFeasible ()
   {
   maxViolation_ = 0.0;

   checkFeasReq ();

   if (myProblem ()->currentVariant ()->respectsOptCons ())
      checkFeasAllBounds ();
   else
      checkFeasLimitedBounds ();

   if (myProblem ()->currentVariant ()->respectsLotSizes ())
      checkFeasLotSizes ();

   if (myProblem ()->currentVariant ()->respectsOptCons ())
      {
      checkDisScrap ();

      checkIntegrality ();
      }

   if (devMode ())
      myMsgFac () ("maxViolationMsg", maxViolation ());

   myGlobalComp ()->storePostFeasible (maxViolation_ <= FEAS_TOL);

   if (not myGlobalComp ()->feasible ())
      {
      if (myProblem ()->extOptVariant ()->isCurrent ())
         myMsgFac () ("infeasExtOptWmsg", maxViolation ());

      else if (devMode ())
         if (myProblem ()->currentVariant ()->internalSoln ())
            myMsgFac () (
               "infeasWmsg",
               myMsgFac ().myFrag (
                  myProblem ()->currentVariant ()->myFragID ()),
                  maxViolation ());
      }
   }

//------------------------------------------------------------------------------

void WitFeasChkr::unpostprocess ()
   {
   maxViolation_ = 0.0;
   }

//------------------------------------------------------------------------------

void WitFeasChkr::verifyUhsFeas ()
   {
   stronglyAssert (myGlobalComp ()->userHeurStart ());

   verifyUhsNonExecCon   ();
   verifyUhsSubCon       ();
   verifyUhsCumDemandCon ();
   verifyUhsExecHUBCon   ();
   verifyUhsLotSizeCon   ();
   }

//------------------------------------------------------------------------------

void WitFeasChkr::checkFeasReq ()
   {
   checkScrapVolNonneg   ();
   checkStockVolNonneg   ();
   checkExecVolNonneg    ();
   checkSubVolNonneg     ();
   checkShipVolNonneg    ();
   checkCumShipVolNonneg ();

   checkStockVols        ();
   checkExecVols         ();
   checkNonSubVols       ();
   checkShipVols         ();
   checkCumShipVols      ();
   }

//------------------------------------------------------------------------------

void WitFeasChkr::checkScrapVolNonneg ()
   {
   WitPart * thePart;
   WitPeriod thePer;
   double    theViol;
   bool      reportNeeded;

   forEachPart (thePart, myProblem ())
      forEachPeriod (thePer, myProblem ())
         {
         theViol = - thePart->scrapVol ()[thePer];

         recordViolation (theViol, reportNeeded);

         if (reportNeeded)
            {
            myMsgFac () ("nonnegConstraintViolMsg", "scrapVol");

            myMsgFac () ("maxViolObjectMsg", "Part:", thePart->partName ());

            myMsgFac () ("maxViolPeriodMsg", thePer);
            }
         }
   }

//------------------------------------------------------------------------------

void WitFeasChkr::checkStockVolNonneg ()
   {
   WitMaterial * theMat;
   WitPeriod     thePer;
   double        theViol;
   bool          reportNeeded;

   forEachMaterial (theMat, myProblem ())
      forEachPeriod (thePer, myProblem ())
         {
         theViol = - theMat->stockVol ()[thePer];

         recordViolation (theViol, reportNeeded);

         if (reportNeeded)
            {
            myMsgFac () ("nonnegConstraintViolMsg", "stockVol");

            myMsgFac () ("maxViolObjectMsg", "Part:", theMat->partName ());

            myMsgFac () ("maxViolPeriodMsg", thePer);
            }
         }
   }

//------------------------------------------------------------------------------

void WitFeasChkr::checkExecVolNonneg ()
   {
   WitOperation * theOpn;
   WitPeriod      thePer;
   double         theViol;
   bool           reportNeeded;

   forEachOperation (theOpn, myProblem ())
      forEachPeriod (thePer, myProblem ())
         {
         theViol = - theOpn->execVol ()[thePer];

         recordViolation (theViol, reportNeeded);

         if (reportNeeded)
            {
            myMsgFac () ("nonnegConstraintViolMsg", "execVol");

            myMsgFac () ("maxViolObjectMsg",
               "Operation:",
               theOpn->operationName ());

            myMsgFac () ("maxViolPeriodMsg", thePer);
            }
         }
   }

//------------------------------------------------------------------------------

void WitFeasChkr::checkSubVolNonneg ()
   {
   WitSubEntry * theSub;
   WitPeriod     thePer;
   double        theViol;
   bool          reportNeeded;

   forEachSubEntry (theSub, myProblem ())
      forEachPeriod (thePer, myProblem ())
         {
         theViol = - theSub->subVol()[thePer];

         recordViolation (theViol, reportNeeded);

         if (reportNeeded)
            {
            myMsgFac () ("nonnegConstraintViolMsg", "subVol");

            myMsgFac () ("maxViolObjectMsg",
               "Operation:",
               theSub->myOperationName ());

            myMsgFac () ("maxViolIndexMsg",
               "BOM Entry #:",
               theSub->myBomEnt ()->localIndex ());

            myMsgFac () ("maxViolIndexMsg",
               "Sub Entry #:",
               theSub->localIndex ());

            myMsgFac () ("maxViolPeriodMsg", thePer);
            }
         }
   }

//------------------------------------------------------------------------------

void WitFeasChkr::checkShipVolNonneg ()
   {
   WitDemand * theDemand;
   WitPeriod   shipPer;
   double      theViol;
   bool        reportNeeded;

   forEachDemand (theDemand, myProblem ())
      forEachPeriod (shipPer, myProblem ())
         {
         theViol = - theDemand->shipVol ()[shipPer];

         recordViolation (theViol, reportNeeded);

         if (reportNeeded)
            {
            myMsgFac () ("nonnegConstraintViolMsg", "shipVol");

            myMsgFac () ("maxViolObjectMsg",
               "Part:",
               theDemand->demandedPartName ());

            myMsgFac () ("maxViolObjectMsg",
               "Demand:",
               theDemand->demandName ());

            myMsgFac () ("maxViolPeriodMsg", shipPer);
            }
         }
   }

//------------------------------------------------------------------------------

void WitFeasChkr::checkCumShipVolNonneg ()
   {
   WitDemand * theDemand;
   WitPeriod   shipPer;
   double      theViol;
   bool        reportNeeded;

   forEachDemand (theDemand, myProblem ())
      forEachPeriod (shipPer, myProblem ())
         {
         theViol = - theDemand->cumShipVol ()[shipPer];

         recordViolation (theViol, reportNeeded);

         if (reportNeeded)
            {
            myMsgFac () ("nonnegConstraintViolMsg", "cumShipVol");

            myMsgFac () ("maxViolObjectMsg",
               "Part:",
               theDemand->demandedPartName ());

            myMsgFac () ("maxViolObjectMsg",
               "Demand:",
               theDemand->demandName ());

            myMsgFac () ("maxViolPeriodMsg", shipPer);
            }
         }
   }

//------------------------------------------------------------------------------

void WitFeasChkr::checkStockVols ()
   {
   WitMaterial * theMat;
   WitPeriod     thePer;
   double        theViol;
   bool          reportNeeded;

   forEachMaterial (theMat, myProblem ())
      forEachPeriod (thePer, myProblem ())
         if (not theMat->canStock (thePer))
            {
            theViol = theMat->stockVol ()[thePer];

            recordViolation (theViol, reportNeeded);

            if (reportNeeded)
               {
               myMsgFac () ("maxViolConstraintMsg",
                  myMsgFac ().myFrag ("maxViolMandECFrag"));

               myMsgFac () ("maxViolObjectMsg", "Part:", theMat->partName ());

               myMsgFac () ("maxViolPeriodMsg", thePer);
               }
            }
   }

//------------------------------------------------------------------------------

void WitFeasChkr::checkExecVols ()
   {
   WitOperation * theOpn;
   WitPeriod      thePer;
   double         theViol;
   bool           reportNeeded;

   forEachOperation (theOpn, myProblem ())
      forEachPeriod (thePer, myProblem ())
         if (not theOpn->executable ()[thePer])
            {
            theViol = theOpn->execVol ()[thePer];

            recordViolation (theViol, reportNeeded);

            if (reportNeeded)
               {
               myMsgFac () ("maxViolConstraintMsg",
                  myMsgFac ().myFrag ("maxViolExecutableFrag"));

               myMsgFac () ("maxViolObjectMsg",
                  "Operation:",
                  theOpn->operationName ());

               myMsgFac () ("maxViolPeriodMsg", thePer);
               }
            }
   }

//------------------------------------------------------------------------------

void WitFeasChkr::checkNonSubVols ()
   {
   WitBomEntry * theBomEnt;
   WitPeriod     thePer;
   double        theViol;
   bool          reportNeeded;

   forEachBomEntry (theBomEnt, myProblem ())
      if (not theBomEnt->mySubEntries ().isEmpty ())
         forEachPeriod (thePer, myProblem ())
            {
            theViol = - theBomEnt->nonSubVol()[thePer];

            recordViolation (theViol, reportNeeded);

            if (reportNeeded)
               {
               myMsgFac () ("maxViolConstraintMsg",
                  myMsgFac ().myFrag ("maxViolTotSubVolFrag"));

               myMsgFac () ("maxViolObjectMsg",
                  "Operation:",
                  theBomEnt->myOperationName ());

               myMsgFac () ("maxViolIndexMsg",
                  "BOM Entry #:",
                  theBomEnt->localIndex ());

               myMsgFac () ("maxViolPeriodMsg", thePer);
               }
            }
   }

//------------------------------------------------------------------------------

void WitFeasChkr::checkShipVols ()
   {
   WitDemand * theDemand;
   WitPeriod   shipPer;
   double      theViol;
   bool        reportNeeded;

   forEachDemand (theDemand, myProblem ())
      if (not theDemand->shipLateAllowed ())
         forEachPeriod (shipPer, myProblem ())
            {
            theViol = 
                 theDemand->shipVol   ()[shipPer]
               - theDemand->demandVol ()[shipPer];

            recordViolation (theViol, reportNeeded);

            if (reportNeeded)
               {
               myMsgFac () ("maxViolConstraintMsg",
                  myMsgFac ().myFrag ("maxViolShipVolFrag"));

               myMsgFac () ("maxViolObjectMsg",
                  "Part:",
                  theDemand->demandedPartName ());

               myMsgFac () ("maxViolObjectMsg",
                  "Demand:",
                  theDemand->demandName ());

               myMsgFac () ("maxViolPeriodMsg", shipPer);
               }
            }
   }

//------------------------------------------------------------------------------

void WitFeasChkr::checkCumShipVols ()
   {
   WitDemand * theDemand;
   WitPeriod   shipPer;
   double      theViol;
   bool        reportNeeded;

   forEachDemand (theDemand, myProblem ())
      if (theDemand->shipLateAllowed ())
         forEachPeriod (shipPer, myProblem ())
            {
            theViol = 
                 theDemand->cumShipVol   ()[shipPer]
               - theDemand->cumDemandVol ()[shipPer];

            recordViolation (theViol, reportNeeded);

            if (reportNeeded)
               {
               myMsgFac () ("maxViolConstraintMsg",
                  myMsgFac ().myFrag ("maxViolCumShipVolFrag"));

               myMsgFac () ("maxViolObjectMsg",
                  "Part:",
                  theDemand->demandedPartName ());

               myMsgFac () ("maxViolObjectMsg",
                  "Demand:",
                  theDemand->demandName ());

               myMsgFac () ("maxViolPeriodMsg", shipPer);
               }
            }
   }

//------------------------------------------------------------------------------

void WitFeasChkr::checkFeasAllBounds ()
   {
   WitComponent * theComp;
   WitBoundSet *  theBoundSet;

   forEachEl (theComp, myCompMgr ()->allComponents ())
      {
      theBoundSet = theComp->myBoundSet ();

      if (theBoundSet != NULL)
         {
         checkFeasHardLB (theBoundSet);
         checkFeasHardUB (theBoundSet);
         }
      }
   }

//------------------------------------------------------------------------------

void WitFeasChkr::checkFeasLimitedBounds ()
   {
   WitOperation * theOpn;

   forEachOperation (theOpn, myProblem ())
      checkFeasHardUB (theOpn->execBounds ());
   }

//------------------------------------------------------------------------------

void WitFeasChkr::checkFeasHardLB (WitBoundSet * theBoundSet)
   {
   WitPeriod thePer;
   double    theViol;
   bool      reportNeeded;

   forEachPeriod (thePer, myProblem ())
      {
      theViol = 
           theBoundSet->hardLB ()  [thePer]
         - theBoundSet->boundedVal (thePer);

      recordViolation (theViol, reportNeeded);

      if (reportNeeded)
         {
         myMsgFac () ("maxViolConstraintMsg",
            myMsgFac ().myFrag ("maxViolHardLBFrag"));

         myMsgFac () ("maxViolIndexMsg",
            "Boundset #:",
            theBoundSet->mappingIndex ());

         myMsgFac () ("maxViolPeriodMsg", thePer);
         }
      }
   }

//------------------------------------------------------------------------------

void WitFeasChkr::checkFeasHardUB (WitBoundSet * theBoundSet)
   {
   WitPeriod thePer;
   double    theViol;
   bool      reportNeeded;

   forEachPeriod (thePer, myProblem ())
      if (theBoundSet->hardUBIsFinite (thePer))
         {
         theViol = 
              theBoundSet->boundedVal (thePer)
            - theBoundSet->hardUB ()  [thePer];

         recordViolation (theViol, reportNeeded);

         if (reportNeeded)
            {
            myMsgFac () ("maxViolConstraintMsg",
               myMsgFac ().myFrag ("maxViolHardUBFrag"));

            myMsgFac () ("maxViolIndexMsg",
               "Boundset #:",
               theBoundSet->mappingIndex ());

            myMsgFac () ("maxViolPeriodMsg", thePer);
            }
         }
   }

//------------------------------------------------------------------------------

void WitFeasChkr::checkFeasLotSizes ()
   {
   WitOperation * theOpn;
   WitPeriod      execPer;
   double         theViol;
   bool           reportNeeded;

   forEachOperation (theOpn, myProblem ())
      forEachPeriod (execPer, myProblem ())
         {
         theViol = lotSizeViol (theOpn, execPer);

         recordViolation (theViol, reportNeeded);

         if (reportNeeded)
            {
            myMsgFac () ("maxViolConstraintMsg",
               myMsgFac ().myFrag ("maxViolLotSizeFrag"));

            myMsgFac () ("maxViolObjectMsg",
               "Operation:",
               theOpn->operationName ());

            myMsgFac () ("maxViolPeriodMsg", execPer);
            }
         }
   }

//------------------------------------------------------------------------------

void WitFeasChkr::checkDisScrap ()
   {
   WitPart * thePart;
   WitPeriod thePer;
   double    theViol;
   bool      reportNeeded;

   forEachPart (thePart, myProblem ())
      if (not thePart->scrapAllowed ())
         forEachPeriod (thePer, myProblem ())
            {
            theViol = thePart->scrapVol ()[thePer];
            
            recordViolation (theViol, reportNeeded);

            if (reportNeeded)
               {
               myMsgFac () ("disScrapConstraintViolMsg");

               myMsgFac () ("maxViolObjectMsg",
                  "Part:",
                  thePart->partName ());

               myMsgFac () ("maxViolPeriodMsg", thePer);
               }
            }
   }

//------------------------------------------------------------------------------

void WitFeasChkr::checkIntegrality ()
   {
   if (not myOptComp ()->mipMode ())
      return;

   checkExecVolInt ();
   checkSubVolInt  ();
   checkShipVolInt ();
   }

//------------------------------------------------------------------------------

void WitFeasChkr::checkExecVolInt ()
   {
   WitOperation * theOpn;
   WitPeriod      execPer;
   double         theViol;
   bool           reportNeeded;

   forEachOperation (theOpn, myProblem ())
      if (theOpn->intExecVols ())
         forEachPeriod (execPer, myProblem ())
            {
            theViol = integralityViol (theOpn->execVol ()[execPer]);

            recordViolation (theViol, reportNeeded);

            if (reportNeeded)
               {
               myMsgFac () ("intConstraintViolMsg", "execVol");

               myMsgFac () ("maxViolObjectMsg",
                  "Operation:",
                  theOpn->operationName ());

               myMsgFac () ("maxViolPeriodMsg", execPer);
               }
            }
   }

//------------------------------------------------------------------------------

void WitFeasChkr::checkSubVolInt ()
   {
   WitSubEntry * theSub;
   WitPeriod     thePer;
   double        theViol;
   bool          reportNeeded;

   forEachSubEntry (theSub, myProblem ())
      if (theSub->intSubVols ())
         forEachPeriod (thePer, myProblem ())
            {
            theViol = integralityViol (theSub->subVol()[thePer]);

            recordViolation (theViol, reportNeeded);

            if (reportNeeded)
               {
               myMsgFac () ("intConstraintViolMsg", "subVol");

               myMsgFac () ("maxViolObjectMsg",
                  "Operation:",
                  theSub->myOperationName ());

               myMsgFac () ("maxViolIndexMsg",
                  "BOM Entry #:",
                  theSub->myBomEnt ()->localIndex ());

               myMsgFac () ("maxViolIndexMsg",
                  "Sub Entry #:",
                  theSub->localIndex ());

               myMsgFac () ("maxViolPeriodMsg", thePer);
               }
            }
   }

//------------------------------------------------------------------------------

void WitFeasChkr::checkShipVolInt ()
   {
   WitDemand * theDemand;
   WitPeriod   shipPer;
   double      theViol;
   bool        reportNeeded;

   forEachDemand (theDemand, myProblem ())
      if (theDemand->intShipVols ())
         forEachPeriod (shipPer, myProblem ())
            {
            theViol = integralityViol (theDemand->shipVol ()[shipPer]);

            recordViolation (theViol, reportNeeded);

            if (reportNeeded)
               {
               myMsgFac () ("intConstraintViolMsg", "shipVol");

               myMsgFac () ("maxViolObjectMsg",
                  "Part:",
                  theDemand->demandedPartName ());

               myMsgFac () ("maxViolObjectMsg",
                  "Demand:",
                  theDemand->demandName ());

               myMsgFac () ("maxViolPeriodMsg", shipPer);
               }
            }
   }

//------------------------------------------------------------------------------

void WitFeasChkr::recordViolation (
      double theViolation,
      bool & reportNeeded)
   {
   reportNeeded = false;

   if (theViolation > maxViolation_)
      {
      maxViolation_ = theViolation;

      if (theViolation > FEAS_TOL)
         {
         myMsgFac () ("maxViolHeadingMsg", FEAS_TOL);
         myMsgFac () ("maxViolViolMsg",    theViolation);

         reportNeeded = true;
         }
      }
   }

//------------------------------------------------------------------------------

void WitFeasChkr::verifyUhsNonExecCon ()
   {
   WitOperation * theOpn;
   WitPeriod      thePer;

   forEachOperation (theOpn, myProblem ())
      forEachPeriod (thePer, myProblem ())
         if (not theOpn->executable ()[thePer])
            if (theOpn->execVol ()[thePer] > 0.0)
               myMsgFac () ("uhsNonExecConViolSmsg", 
                  theOpn->operationName (),
                  thePer,
                  theOpn->execVol ()[thePer]);
   }

//------------------------------------------------------------------------------

void WitFeasChkr::verifyUhsSubCon ()
   {
   WitTVec <double> nonSubVolVec;
   WitBomEntry *    theBomEnt;
   WitPeriod        thePer;
   double           maxViol;
   WitBomEntry *    mvBomEnt;
   WitPeriod        mvPer;

   nonSubVolVec.allocate (myProblem (), 0.0);

   maxViol  = FEAS_TOL;
   mvBomEnt = NULL;
   mvPer    = -1;

   forEachBomEntry (theBomEnt, myProblem ())
      if (not theBomEnt->mySubEntries ().isEmpty ())
         {
         myPostprocessor ()->compNonSubVol (theBomEnt, nonSubVolVec);

         forEachPeriod (thePer, myProblem ())
            if (- nonSubVolVec[thePer] > maxViol)
               {
               maxViol  = - nonSubVolVec[thePer];
               mvBomEnt = theBomEnt;
               mvPer    = thePer;
               }
         }

   if (mvBomEnt != NULL)
      {
      myMsgFac () ("uhsConViolIntroWmsg");

      myMsgFac () ("uhsSubConViolWmsg",
         maxViol,
         mvBomEnt->myOperationName (),
         mvBomEnt->localIndex (),
         mvPer,
         mvBomEnt->myOperation ()->execVol ()[mvPer],
         mvBomEnt->myOperation ()->execVol ()[mvPer] + maxViol);
      }
   }

//------------------------------------------------------------------------------

void WitFeasChkr::verifyUhsCumDemandCon ()
   {
   WitDemand * theDemand;
   WitPeriod   thePer;
   double      cumDemandVolVal;
   double      cumShipVolVal;
   double      maxViol;
   WitDemand * mvDemand;
   WitPeriod   mvPer;
   double      mvCumDemVol;

   maxViol     = FEAS_TOL;
   mvDemand    = NULL;
   mvPer       = -1;
   mvCumDemVol = 0.0;

   forEachDemand (theDemand, myProblem ())
      {
      cumDemandVolVal = 0.0;
      cumShipVolVal   = 0.0;

      forEachPeriod (thePer, myProblem ())
         {
         cumDemandVolVal += theDemand->demandVol ()[thePer];
         cumShipVolVal   += theDemand->shipVol ()[thePer];

         if (cumShipVolVal - cumDemandVolVal > maxViol)
            {
            maxViol     = cumShipVolVal - cumDemandVolVal;
            mvDemand    = theDemand;
            mvPer       = thePer;
            mvCumDemVol = cumDemandVolVal;
            }
         }
      }

   if (mvDemand != NULL)
      {
      myMsgFac () ("uhsConViolIntroWmsg");

      myMsgFac () ("uhsCumDemandConViolWmsg",
         maxViol,
         mvDemand->demandedPartName (),
         mvDemand->demandName (),
         mvPer,
         mvCumDemVol + maxViol,
         mvCumDemVol);
      }
   }

//------------------------------------------------------------------------------

void WitFeasChkr::verifyUhsExecHUBCon ()
   {
   WitOperation * theOpn;
   WitPeriod      thePer;
   double         hardUBVal;
   double         maxViol;
   WitOperation * mvOpn;
   WitPeriod      mvPer;

   maxViol = FEAS_TOL;
   mvOpn   = NULL;
   mvPer   = -1;

   forEachOperation (theOpn, myProblem ())
      forEachPeriod (thePer, myProblem ())
         if (theOpn->execBounds ()->hardUBIsFinite (thePer))
            {
            hardUBVal = theOpn->execBounds ()->hardUB ()[thePer];

            if (theOpn->execVol ()[thePer] - hardUBVal > maxViol)
               {
               maxViol = theOpn->execVol ()[thePer] - hardUBVal;
               mvOpn   = theOpn;
               mvPer   = thePer;
               }
            }

   if (mvOpn != NULL)
      {
      myMsgFac () ("uhsConViolIntroWmsg");

      myMsgFac () ("uhsExecHUBConViolWmsg",
         maxViol,
         mvOpn->operationName (),
         mvPer,
         mvOpn->execVol ()[mvPer],
         mvOpn->execBounds ()->hardUB ()[mvPer]);
      }
   }

//------------------------------------------------------------------------------

void WitFeasChkr::verifyUhsLotSizeCon ()
   {
   WitOperation * theOpn;
   WitPeriod      execPer;
   double         maxViol;
   WitOperation * mvOpn;
   WitPeriod      mvPer;

   maxViol = FEAS_TOL;
   mvOpn   = NULL;
   mvPer   = -1;

   forEachOperation (theOpn, myProblem ())
      forEachPeriod (execPer, myProblem ())
         if (lotSizeViol (theOpn, execPer) > maxViol)
            {
            maxViol = lotSizeViol (theOpn, execPer);
            mvOpn   = theOpn;
            mvPer   = execPer;
            }

   if (mvOpn != NULL)
      {
      myMsgFac () ("uhsConViolIntroWmsg");

      myMsgFac () ("uhsLotSizeConViolWmsg",
         maxViol,
         mvOpn->operationName (),
         mvPer,
         mvOpn->execVol ()[mvPer]);
      }
   }

//------------------------------------------------------------------------------

double WitFeasChkr::lotSizeViol (WitOperation * theOpn, WitPeriod execPer)
   {
   double theExecVol;
   double lsDelta;

   theExecVol = theOpn->execVol ()[execPer];

   lsDelta    = theOpn->lotSizeDelta (execPer, theExecVol, 0.0);

   return positivePart (lsDelta);
   }

//------------------------------------------------------------------------------

double WitFeasChkr::integralityViol (double theValue)
   {
   return
      min (      theValue  - floor (theValue),
           ceil (theValue) -        theValue);
   }
