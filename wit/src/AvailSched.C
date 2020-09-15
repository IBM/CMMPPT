//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "AvailSched.C"
//
// Contains the implementation of class AvailSched.
//------------------------------------------------------------------------------

#include <AvailSched.h>
#include <HeurAtor.h>
#include <SsrMgr.h>
#include <GlobalComp.h>
#include <Material.h>
#include <BoundSet.h>
#include <Post.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class AvailSched.
//------------------------------------------------------------------------------

WitAvailSched::WitAvailSched (WitHeurAtor * theHeurAtor):

      WitProbAssoc (theHeurAtor),

      myHeurAtor_  (theHeurAtor),
      mySsrMgr_    (NULL),
      incAvailVol_ (myProblem (), 0.0),
      lastDecPer_  (myProblem (), -1),
      uncondSR_    (myProblem (), false),
      selectiveSR_ (myProblem (), false)
   {
   WitPart * thePart;

   if (myHeurAtor_->userHeurStart ())
      {
      myMsgFac () ("userHeurStartMsg");

      initForUHS ();

      verifyUhsNNScrapCon ();
      }
   else
      forEachPart (thePart, myProblem ())
         incAvailVol_ (thePart) = thePart->supplyVol ();

   initStockRealloc ();
   }

//------------------------------------------------------------------------------

WitAvailSched::~WitAvailSched ()
   {
   }

//------------------------------------------------------------------------------

void WitAvailSched::attachSsrMgr ()
   {
   mySsrMgr_ = myHeurAtor_->mySsrMgr ();
   }

//------------------------------------------------------------------------------

void WitAvailSched::netWoSR (
      WitPart * thePart,
      WitPeriod reqPer,
      double &  theReqVol)
   {
   WitMaterial * theMat;

   witAssert (theReqVol >= NET_TOL);

   theMat = thePart->thisMat ();

   if (theMat != NULL)
      netMatWoSR      (theMat,  reqPer, theReqVol);
   else
      netWithIncAvail (thePart, reqPer, theReqVol);
   }

//------------------------------------------------------------------------------

void WitAvailSched::netWithIncAvail (
      WitPart * thePart,
      WitPeriod thePer,
      double &  theReqVol)
   {
   double deltaVol;

   deltaVol = min (theReqVol, incAvailVol_ (thePart)[thePer]);

   if (deltaVol < NET_TOL)
      return;

   theReqVol -= deltaVol;

   addToIncAvailVol (thePart, thePer, - deltaVol);
   }

//------------------------------------------------------------------------------

void WitAvailSched::netMatWoSR (
      WitMaterial * theMat,
      WitPeriod     reqPer,
      double &      theReqVol)
   {
   double allVol;
   double totReqVol;
   double availVol;
   double deltaVol;

   allVol    = allocedVol (theMat, reqPer, false);

   totReqVol = theReqVol + allVol;

   availVol  = availToNet (theMat, reqPer, totReqVol, 0.0);

   deltaVol  = 
      (availVol == totReqVol)?
         theReqVol:
         availVol - allVol;

   if (deltaVol < NET_TOL)
      return;

   theReqVol -= deltaVol;

   addToIncAvailVol (theMat, reqPer, - deltaVol);
   }

//------------------------------------------------------------------------------

WitPeriod WitAvailSched::netAndRoll (
      WitMaterial * theMat, 
      WitPeriod     reqPer,
      double &      theReqVol,
      double &      rollVol,
      bool &        bddSsrSplit)
   {
   double    allVol;
   WitPeriod rollPer;

   witAssert (eitherSR (theMat));

   witAssert (theReqVol >= NET_TOL);

   if (selectiveSR_ (theMat))
      allVol = allocedVol (theMat, reqPer, true);
   else
      allVol = 0.0;

   rollVol = availToNet (theMat, reqPer, theReqVol, allVol);

   if (rollVol < NET_TOL)
      return -1;

   theReqVol -= rollVol;

   rollPer    = rollPeriod (theMat, reqPer);

   if (selectiveSR_ (theMat))
      {
      mySsrMgr_->adjReqAndRollForSS (
         theMat,
         rollPer,
         theReqVol,
         rollVol,
         bddSsrSplit);

      if (rollVol < NET_TOL)
         return -1;
      }

   addToIncAvailVol (theMat, reqPer, - rollVol);

   if (rollPer >= 0)
      addToIncAvailVol (theMat, rollPer, rollVol);

   return rollPer;
   }

//------------------------------------------------------------------------------

void WitAvailSched::reserveAchStSLB (
      WitMaterial * theMat, 
      WitPeriod     thePer,
      double &      unachVol)
   {
   double resVol;

   witAssert (myGlobalComp ()->respectStockSLBs ());

   witAssert (not theMat->mandEC ()[thePer]);

   unachVol = theMat->stockBounds ()->softLB ()[thePer];

   witAssert (unachVol >= NET_TOL);

   resVol   = availToNet (theMat, thePer, unachVol, 0.0);

   if (resVol <= NET_TOL)
      return;

   addToIncAvailVol    (theMat, thePer,     - resVol);

   if (thePer < lastPeriod ())
      addToIncAvailVol (theMat, thePer + 1, + resVol);

   unachVol -= resVol;
   }

//------------------------------------------------------------------------------

void WitAvailSched::addToIncAvailVol (
      WitPart * thePart, 
      WitPeriod thePer, 
      double    deltaAvailVol)
   {
   WitMaterial * theMat;

   double & incAvailVolRef = incAvailVol_ (thePart)[thePer];

   myHeurAtor_->saveVal (incAvailVolRef);

   incAvailVolRef += deltaAvailVol;

   if (incAvailVolRef >= - NET_TOL)
      return;

   theMat = thePart->thisMat ();

   if (theMat != NULL)
      if (thePer > lastDecPer_ (theMat))
         {
         myHeurAtor_->saveVal (lastDecPer_ (theMat));

         lastDecPer_ (theMat) = thePer;
         }
   }

//------------------------------------------------------------------------------

void WitAvailSched::print ()
   {
   WitPart * thePart;
   WitPeriod thePer;

   fprintf (msgFile (),
      "\n"
      "----------------------\n"
      "Availability Schedule:\n"
      "----------------------\n"
      "\n");

   forEachPart (thePart, myProblem ())
      {
      fprintf (msgFile (),
         "Part \"%s\":\n",
         thePart->partName ().myCstring ());

      forEachPeriod (thePer, myProblem ())
         {
         fprintf (msgFile (),
            "   Period %d: IncAvail: %3.0f\n",
            thePer,
            incAvailVol_ (thePart)[thePer]);
         }

      fprintf (msgFile (),"\n");
      }
   }

//------------------------------------------------------------------------------

void WitAvailSched::initForUHS ()
   {
   WitTVec <double> prodVolVec;
   WitTVec <double> consVolVec;
   WitTVec <double> nonSubVolVec;
   WitTVec <double> subVolVec;
   WitPart *        thePart;
   WitBomEntry *    theBomEnt;
   WitSubEntry *    theSub;
   WitPeriod        thePer;
   double           incAvVol;
   WitDemand *      theDemand;

   prodVolVec  .allocate (myProblem (), 0.0);
   consVolVec  .allocate (myProblem (), 0.0);
   nonSubVolVec.allocate (myProblem (), 0.0);
   subVolVec   .allocate (myProblem (), 0.0);

   forEachPart (thePart, myProblem ())
      {
      myProblem ()->myPostprocessor ()->compProdVol (thePart, prodVolVec);

      consVolVec = 0.0;

      forEachEl (theBomEnt, thePart->consumingBomEntries ())
         {
         myProblem ()->
            myPostprocessor ()->
               compNonSubVol (theBomEnt, nonSubVolVec);

         theBomEnt->incConsVol (consVolVec, nonSubVolVec);
         }

      forEachEl (theSub, thePart->consumingSubEntries ())
         {
         subVolVec = theSub->subVol ();

         theSub->incConsVol (consVolVec, subVolVec);
         }

      forEachPeriod (thePer, myProblem ())
         {
         incAvVol =
              thePart->supplyVol ()[thePer]
            + prodVolVec           [thePer]
            - consVolVec           [thePer];

         forEachEl (theDemand, thePart->myDemands ())
            incAvVol -= theDemand->shipVol ()[thePer];

         addToIncAvailVol (thePart, thePer, incAvVol);
         }
      }
   }

//------------------------------------------------------------------------------

void WitAvailSched::verifyUhsNNScrapCon ()
   {
   WitPart * thePart;
   WitPeriod thePer;
   double    availVol;
   double    maxViol;
   WitPart * mvPart;
   WitPeriod mvPer;

   maxViol = FEAS_TOL;
   mvPart  = NULL;
   mvPer   = -1;

   forEachPart (thePart, myProblem ())
      {
      availVol = 0.0;

      forEachPeriod (thePer, myProblem ())
         {
         availVol += incAvailVol_ (thePart)[thePer];

         if (- availVol > maxViol)
            {
            maxViol = - availVol;
            mvPart  =   thePart;
            mvPer   =   thePer;
            }

         setToMax (availVol, 0.0);

         if (not thePart->canStock (thePer))
            availVol = 0.0;
         }
      }

   if (mvPart != NULL)
      {
      myMsgFac () ("uhsConViolIntroWmsg");

      myMsgFac () ("uhsNNScrapConViolWmsg",
         maxViol,
         mvPart->partName (),
         mvPer,
         - maxViol);
      }
   }

//------------------------------------------------------------------------------

void WitAvailSched::initStockRealloc ()
   {
   WitMaterial * theMat;

   if (myGlobalComp ()->oldSR ())
      {
      myMsgFac () ("oldSRMsg");

      forEachMaterial (theMat, myProblem ())
         if (not theMat->producingBopEntries ().isEmpty ())
            uncondSR_ (theMat) = true; 
      }
   else if (myGlobalComp ()->stockReallocation ())
      {
      myMsgFac () ("stockReMsg");

      forEachMaterial (theMat, myProblem ())
         {
         if (theMat->producingBopEntries ().isEmpty ())
            continue;

         if (unlimitedBuildAhead (theMat))
            uncondSR_    (theMat) = true; 
         else
            selectiveSR_ (theMat) = true;
         }
      }
   }

//------------------------------------------------------------------------------

bool WitAvailSched::unlimitedBuildAhead (WitMaterial * theMat)
   {
   WitPeriod thePer;

   if    (not theMat->buildNstn ())
      if (not theMat->buildAsap ())
         return false;

   forEachPeriod (thePer, myProblem ())
      if (theMat->buildAheadUB ()[thePer] < thePer)
         return false;

   return true;
   }

//------------------------------------------------------------------------------
// allocedVol
//
// Note that mandECs can be ignored: When one occurs, the allocated volume will
// have to be zero anyway.
//------------------------------------------------------------------------------

double WitAvailSched::allocedVol (
      WitMaterial * theMat,
      WitPeriod     reqPer,
      bool          forSR)
   {
   WitPeriod thePer;
   double    allVol;
   double    incAvVol;

   allVol = 0.0;

   for (thePer = lastDecPer_ (theMat); thePer > reqPer; thePer --)
      {
      incAvVol = incAvailVol_ (theMat)[thePer];

      if (incAvVol < - NET_TOL)
         {
         if (forSR)
            if (srAllowedFrom (theMat, thePer))
               continue;

         allVol += - incAvVol;
         }
      else if (incAvVol > NET_TOL)
         {
         allVol -= incAvVol;

         setToMax (allVol, 0.0);
         }
      }

   return allVol;
   }

//------------------------------------------------------------------------------

double WitAvailSched::availToNet (
      WitMaterial * theMat, 
      WitPeriod     reqPer,
      double        theReqVol,
      double        allVol)
   {
   double    availVol;
   WitPeriod thePer;
   double    theIncAvailVol;

   witAssert (theReqVol >= NET_TOL);

   availVol = - allVol;

   for (thePer = reqPer; thePer >= 0; thePer --)
      {
      if (thePer < reqPer)
         if (theMat->mandEC ()[thePer])
            return availVol;

      theIncAvailVol = incAvailVol_ (theMat)[thePer];

      if (theIncAvailVol == 0.0)
         continue;

      availVol += theIncAvailVol;

      if (availVol >= theReqVol - NET_TOL)
         return theReqVol;
      }

   return availVol;
   }

//------------------------------------------------------------------------------

WitPeriod WitAvailSched::rollPeriod (WitMaterial * theMat, WitPeriod fromPer)
   {
   WitPeriod thePer;

   if (fromPer < lastDecPer_ (theMat))
      for (thePer = fromPer + 1; thePer < nPeriods (); thePer ++)
         {
         if (theMat->mandEC ()[thePer - 1])
            break;

         if (srAllowedFrom (theMat, thePer))
            if (incAvailVol_ (theMat)[thePer] < - NET_TOL)
               return thePer;
         }

   return -1;
   }

//------------------------------------------------------------------------------

bool WitAvailSched::srAllowedFrom (WitMaterial * theMat, WitPeriod thePer)
   {
   return
      uncondSR_    (theMat)? true:
      selectiveSR_ (theMat)? mySsrMgr_->srAllowedFrom (theMat, thePer):
                             false;
   }
