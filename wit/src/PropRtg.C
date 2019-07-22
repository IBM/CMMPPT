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
// Source file: "PropRtg.C"
//
// Contains the implementation of the following classes:
//
//    PrMgr.
//    PrCoord.
//    PrSelMgr.
//    PprSelMgr.
//    CprSelMgr.
//    PrSelPt.
//    PrAltPt.
//    PrCand.
//------------------------------------------------------------------------------

#include <PrMgr.h>
#include <PrCoord.h>
#include <PrSelMgr.h>
#include <PrSelPt.h>
#include <PrAltPt.h>
#include <PrCand.h>
#include <RtMgr.h>
#include <RtSite.h>
#include <RtCand.h>
#include <Selector.h>
#include <SplitPt.h>
#include <SglSrcMgr.h>
#include <HeurAtor.h>
#include <GlobalComp.h>
#include <Part.h>
#include <Operation.h>
#include <BopEntry.h>
#include <CompMgr.h>
#include <PairStack.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class PrMgr.
//------------------------------------------------------------------------------

WitPrMgr::WitPrMgr (WitSelector * theSelector):

      WitProbAssoc (theSelector),
      myPrCoord_   (NULL),
      myRtMgr_     (theSelector->myRtMgr ())
   {
   myPrCoord_ = new WitPrCoord (theSelector);
   }

//------------------------------------------------------------------------------

WitPrMgr::~WitPrMgr ()
   {
   delete myPrCoord_;
   }

//------------------------------------------------------------------------------

bool WitPrMgr::instNeededFor (WitProblem * theProblem)
   {
   WitPart *     thePart;
   WitBomEntry * theBomEnt;

   forEachPart (thePart, theProblem)
      if (thePart->propRtg () != false)
         return true;

   forEachBomEntry (theBomEnt, theProblem)
      if (theBomEnt->propRtg () != false)
         return true;

   return false;
   }

//------------------------------------------------------------------------------

void WitPrMgr::print ()
   {
   myPrCoord_->print ();
   }

//------------------------------------------------------------------------------

void WitPrMgr::definePropExp (
      WitRtSite *         theRtSite, 
      WitPeriod           thePer,
      double              expVol,
      WitRtCandDblStack & theExpPairs)
   {
   myPrCoord_->definePropExp (theRtSite, thePer, expVol, theExpPairs);
   }

//------------------------------------------------------------------------------

void WitPrMgr::definePropExp (
      WitPart *           thePart, 
      WitPeriod           expPer,
      double              expVol,
      WitRtCandDblStack & theExpPairs)
   {
   WitRtSite * theRtSite;

   theRtSite = myRtMgr_->myRtSiteFor (thePart);

   myPrCoord_->definePropExp (theRtSite, expPer, expVol, theExpPairs);
   }

//------------------------------------------------------------------------------

void WitPrMgr::definePropExp (
      WitBomEntry *       theBomEnt, 
      WitPeriod           execPer,
      double              expVol,
      WitRtCandDblStack & theExpPairs)
   {
   WitRtSite * theRtSite;

   theRtSite = myRtMgr_->myRtSiteFor (theBomEnt);

   myPrCoord_->definePropExp (theRtSite, execPer, expVol, theExpPairs);
   }

//------------------------------------------------------------------------------

bool WitPrMgr::active (WitRtCand * theRtCand, WitPeriod thePer)
   {
   return myPrCoord_->active (theRtCand, thePer);
   }

//------------------------------------------------------------------------------

void WitPrMgr::setActive (
      WitRtCand * theRtCand,
      WitPeriod   thePer,
      bool        theValue)
   {
   myPrCoord_->setActive (theRtCand, thePer, theValue);
   }

//------------------------------------------------------------------------------

bool WitPrMgr::allowed (WitRtCand * theRtCand, WitPeriod thePer)
   {
   return myPrCoord_->allowed (theRtCand, thePer);
   }

//------------------------------------------------------------------------------

WitSelPt * WitPrMgr::mySelPtFor (WitRtSite * theRtSite, WitPeriod thePer)
   {
   return myPrCoord_->mySelPtFor (theRtSite, thePer);
   }

//------------------------------------------------------------------------------

void WitPrMgr::storeRecoveryPair (WitRtCand * theRtCand, WitPeriod thePer)
   {
   myPrCoord_->storeRecoveryPair (theRtCand, thePer);
   }

//------------------------------------------------------------------------------

void WitPrMgr::recoverInitState ()
   {
   myPrCoord_->recoverInitState ();
   }

//------------------------------------------------------------------------------

bool WitPrMgr::selIsSplit (WitPart * thePart, WitPeriod expPer)
   {
   WitRtSite * theRtSite;

   theRtSite = myRtMgr_->myRtSiteFor (thePart);

   return myPrCoord_->selIsSplit (theRtSite, expPer);
   }

//------------------------------------------------------------------------------

bool WitPrMgr::selIsSplit (WitBomEntry * theBomEnt, WitPeriod execPer)
   {
   WitRtSite * theRtSite;

   theRtSite = myRtMgr_->myRtSiteFor (theBomEnt);

   return myPrCoord_->selIsSplit (theRtSite, execPer);
   }

//------------------------------------------------------------------------------

void WitPrMgr::defineSplit (
      WitPart *           thePart, 
      WitPeriod           expPer,
      double              expVol,
      WitRtCandDblStack & theExpPairs,
      double &            unbddVol)
   {
   WitRtSite * theRtSite;

   theRtSite = myRtMgr_->myRtSiteFor (thePart);

   myPrCoord_->defineSplit (
      theRtSite,
      expPer,
      expVol,
      theExpPairs,
      unbddVol);
   }

//------------------------------------------------------------------------------

void WitPrMgr::defineSplit (
      WitBomEntry *       theBomEnt, 
      WitPeriod           execPer,
      double              expVol,
      WitRtCandDblStack & theExpPairs,
      double &            unbddVol)
   {
   WitRtSite * theRtSite;

   theRtSite = myRtMgr_->myRtSiteFor (theBomEnt);

   myPrCoord_->defineSplit (
      theRtSite,
      execPer,
      expVol,
      theExpPairs,
      unbddVol);
   }

//------------------------------------------------------------------------------

void WitPrMgr::recordFlowSS (
      WitPart * thePart,
      WitPeriod expPer,
      double    incFlowVol)
   {
   WitRtSite * theRtSite;

   theRtSite = myRtMgr_->myRtSiteFor (thePart);

   myPrCoord_->recordFlowSS (theRtSite, expPer, incFlowVol);
   }

//------------------------------------------------------------------------------

void WitPrMgr::recordFlowSS (
      WitBomEntry * theBomEnt,
      WitPeriod     execPer,
      double        incFlowVol)
   {
   WitRtSite * theRtSite;

   theRtSite = myRtMgr_->myRtSiteFor (theBomEnt);

   myPrCoord_->recordFlowSS (theRtSite, execPer, incFlowVol);
   }

//------------------------------------------------------------------------------

WitSelMgr * WitPrMgr::mySelMgrForProd ()
   {
   return myPrCoord_->myPrSelMgrForProd ();
   }

//------------------------------------------------------------------------------

WitSelMgr * WitPrMgr::mySelMgrForCons ()
   {
   return myPrCoord_->myPrSelMgrForCons ();
   }

//------------------------------------------------------------------------------
// Implementation of class PrCoord.
//------------------------------------------------------------------------------

WitPrCoord::WitPrCoord (WitSelector * theSelector):

      WitProbAssoc       (theSelector),
      mySelector_        (theSelector),
      myRtMgr_           (theSelector->myRtMgr ()),
      myPrSelMgrForProd_ (NULL),
      myPrSelMgrForCons_ (NULL),
      myPrSelPts_        (),
      myPrAltPts_        (),
      myRecoveryPairs_   (myProblem ())
   {
   myMsgFac () ("propRtgMsg");

   myPrSelMgrForProd_ = new WitPprSelMgr (this);
   myPrSelMgrForCons_ = new WitCprSelMgr (this);

   buildPrSelPts ();
   buildPrAltPts ();
   }

//------------------------------------------------------------------------------

WitPrCoord::~WitPrCoord ()
   {
   WitRtCand * theRtCand;
   WitRtSite * theRtSite;
   WitPeriod   thePer;

   forEachEl (theRtCand, myRtMgr_->myRtCands ())
      if (myPrAltPts_.myPtrTVecAt (theRtCand).isAllocated ())
         forEachPeriod (thePer, myProblem ())
            delete myPrAltPts_.myPtrAt (theRtCand, thePer);

   forEachEl (theRtSite, myRtMgr_->myRtSites ())
      if (myPrSelPts_.myPtrTVecAt (theRtSite).isAllocated ())
         forEachPeriod (thePer, myProblem ())
            delete myPrSelPtFor (theRtSite, thePer);

   delete myPrSelMgrForCons_;
   delete myPrSelMgrForProd_;
   }

//------------------------------------------------------------------------------

void WitPrCoord::print ()
   {
   WitRtSite *  theRtSite;
   WitPeriod    thePer;
   WitPrSelPt * thePrSelPt;

   fprintf (msgFile (),
      "\n"
      "Proportionate Routing Status:\n");

   forEachEl (theRtSite, myRtMgr_->myRtSites ())
      forEachPeriod (thePer, myProblem ())
         {
         thePrSelPt = myPrSelPtFor (theRtSite, thePer);

         if (thePrSelPt == NULL)
            continue;

         thePrSelPt->print ();

         thePrSelPt->printAllowedRtCands ();
         }

   fprintf (msgFile (), "\n");
   }

//------------------------------------------------------------------------------

void WitPrCoord::definePropExp (
      WitRtSite *         theRtSite, 
      WitPeriod           thePer,
      double              expVol,
      WitRtCandDblStack & theExpPairs)
   {
   WitRtCandStack theRtCands (myProblem ());
   WitRtCand *    theRtCand;

   witAssert (theRtSite->propRtg ()[thePer]);

   theExpPairs.clear ();

   if (theRtSite->myRtCands ().nElements () > 1)
      {
      findActiveRtCands (theRtSite, thePer, theRtCands);

      defineExpPairs (theRtCands, thePer, expVol, theExpPairs);
      }
   else
      {
      witAssert (theRtSite->myRtCands ().nElements () == 1);

      theRtCand = theRtSite->myRtCands ().first ();

      witAssert (theRtCand->isEligible (thePer));

      theExpPairs.push (theRtCand, expVol);
      }

   if (selPrintLevelAt (3))
      printPropExp (theRtSite, thePer, expVol, theExpPairs, false);
   }

//------------------------------------------------------------------------------

bool WitPrCoord::active (WitRtCand * theRtCand, WitPeriod thePer)
   {
   WitPrAltPt * thePrAltPt;

   witAssert (theRtCand->myRtSite ()->propRtg ()[thePer]);

   if (myPrAltPts_.myPtrTVecAt (theRtCand).isAllocated ())
      {
      thePrAltPt = myPrAltPts_.myPtrAt (theRtCand, thePer);

      if (thePrAltPt != NULL)
         return thePrAltPt->active ();
      }

   if (theRtCand->myRtSite ()->myRtCands ().nElements () == 1)
      if (theRtCand->isEligible (thePer))
         return true;

   return false;
   }

//------------------------------------------------------------------------------

void WitPrCoord::setActive (
      WitRtCand * theRtCand,
      WitPeriod   thePer,
      bool        theValue)
   {
   WitPrAltPt * thePrAltPt;

   witAssert (mySelector_->tbprMode ());

   witAssert (theRtCand->myRtSite ()->propRtg ()[thePer]);

   thePrAltPt = myPrAltPts_.myPtrAt (theRtCand, thePer);

   witAssert (thePrAltPt != NULL);

   witAssert (thePrAltPt->allowed ());

   thePrAltPt->setStatus (theValue, true);
   }

//------------------------------------------------------------------------------

bool WitPrCoord::allowed (WitRtCand * theRtCand, WitPeriod thePer)
   {
   WitPrAltPt * thePrAltPt;

   witAssert (theRtCand->myRtSite ()->propRtg ()[thePer]);

   if (myPrAltPts_.myPtrTVecAt (theRtCand).isAllocated ())
      {
      thePrAltPt = myPrAltPts_.myPtrAt (theRtCand, thePer);

      if (thePrAltPt != NULL)
         return thePrAltPt->allowed ();
      }

   if (theRtCand->myRtSite ()->myRtCands ().nElements () == 1)
      if (theRtCand->isEligible (thePer))
         return true;

   return false;
   }

//------------------------------------------------------------------------------

void WitPrCoord::storeRecoveryPair (WitRtCand * theRtCand, WitPeriod thePer)
   {
   myRecoveryPairs_.push (theRtCand, thePer);
   }

//------------------------------------------------------------------------------

void WitPrCoord::recoverInitState ()
   {
   WitRtCand * theRtCand;
   WitPeriod   thePer;

   while (myRecoveryPairs_.pop (theRtCand, thePer))
      {
      myPrAltPts_.myPtrAt (theRtCand, thePer)->recoverInitState ();
      }
   }

//------------------------------------------------------------------------------

WitSelPt * WitPrCoord::mySelPtFor (WitRtSite * theRtSite, WitPeriod thePer)
   {
   return myPrSelPtFor (theRtSite, thePer);
   }

//------------------------------------------------------------------------------

WitAltPt * WitPrCoord::myAltPtFor (WitRtCand * theRtCand, WitPeriod thePer)
   {
   WitPrAltPt * thePrAltPt;

   thePrAltPt = myPrAltPtFor (theRtCand, thePer);

   if (thePrAltPt == NULL)
      return NULL;

   witAssert (thePrAltPt->active ());

   if (thePrAltPt->myPrSelPt ()->nAllowedRtCands () == 1)
      return NULL;

   return thePrAltPt;
   }

//------------------------------------------------------------------------------

WitPrAltPt * WitPrCoord::myPrAltPtFor (WitRtCand * theRtCand, WitPeriod thePer)
   {
   if (not myPrAltPts_.myPtrTVecAt (theRtCand).isAllocated ())
      return NULL;

   return myPrAltPts_.myPtrAt (theRtCand, thePer);
   }

//------------------------------------------------------------------------------

bool WitPrCoord::selIsSplit (WitRtSite * theRtSite, WitPeriod thePer)
   {
   WitPrSelPt * thePrSelPt;

   if (not mySelector_->selSplit ())
      return false;

   thePrSelPt = myPrSelPtFor (theRtSite, thePer);

   if (thePrSelPt == NULL)
      return false;

   return thePrSelPt->isSplit ();
   }

//------------------------------------------------------------------------------

void WitPrCoord::defineSplit (
      WitRtSite *         theRtSite, 
      WitPeriod           thePer,
      double              expVol,
      WitRtCandDblStack & theExpPairs,
      double &            unbddVol)
   {
   WitSelCand * theSelCand;
   double       splitVol;
   WitRtCand *  theRtCand;
   double       propExpVol;

   WitSelCandDblStack theSplitPairs (myProblem ());
   WitRtCandStack     theRtCands    (myProblem ());
   WitRtCandDblStack  localExpPairs (myProblem ());

   witAssert (selIsSplit (theRtSite, thePer));

   witAssert (expVol >= NET_TOL);

   theExpPairs.clear ();

   myPrSelPtFor (theRtSite, thePer)->
      mySplitPt ()->
         defineSplit (expVol, theSplitPairs, unbddVol);

   while (theSplitPairs.pop (theSelCand, splitVol))
      {
      theSelCand->getPrData (theRtCands);

      defineExpPairs (
         theRtCands,
         thePer,
         splitVol,
         localExpPairs);

      localExpPairs.reverse ();

      while (localExpPairs.pop (theRtCand, propExpVol))
         theExpPairs.push (theRtCand, propExpVol);
      }

   if (selPrintLevelAt (3))
      printPropExp (theRtSite, thePer, expVol - unbddVol, theExpPairs, true);
   }

//------------------------------------------------------------------------------

void WitPrCoord::recordFlowSS (
      WitRtSite * theRtSite,
      WitPeriod   thePer,
      double      incFlowVol)
   {
   WitPrSelPt * thePrSelPt;

   thePrSelPt = myPrSelPtFor (theRtSite, thePer);

   if (thePrSelPt == NULL)
      return;

   thePrSelPt->recordFlowSS (incFlowVol);
   }

//------------------------------------------------------------------------------

void WitPrCoord::printPropExp (
      WitRtSite *               theRtSite, 
      WitPeriod                 thePer,
      double                    expVol,
      const WitRtCandDblStack & theExpPairs,
      bool                      bddSplit)
   {
   WitPairStItr <WitRtCand, double> theItr;
   WitRtCand *                      theRtCand;
   double                           propExpVol;

   if (not mySelector_->myHeurAtor ()->permCommActive ())
      return;

   fprintf (msgFile (),
      "\n"
      "Proportionate Explosion%s:\n\n",
      bddSplit? " (Bounded Split)": "");

   theRtSite->printID (4, 22);

   fprintf (msgFile (),
      "   Period:           %d\n"
      "   Exp Vol:          %.3f\n",
      thePer,
      expVol);

   theExpPairs.attachItr (theItr);

   while (theItr.advance (theRtCand, propExpVol))
      {
      fprintf (msgFile (), "\n");

      theRtCand->printID (7, 22);

      fprintf (msgFile (), "      Prop Vol:      %.3f\n", propExpVol);
      }
   }

//------------------------------------------------------------------------------

void WitPrCoord::buildPrSelPts ()
   {
   WitRtSite * theRtSite;
   WitPeriod   thePer;

   myPrSelPts_.allocate1D (myProblem ());

   forEachEl (theRtSite, myRtMgr_->myRtSites ())
      if (theRtSite->myRtCands ().nElements () > 1)
         if (theRtSite->propRtg () != false)
            {
            myPrSelPts_.myPtrTVecAt (theRtSite).allocate (myProblem ());

            forEachPeriod (thePer, myProblem ())
               if (theRtSite->explodeable (thePer))
                  if (theRtSite->propRtg ()[thePer])
                     myPrSelPts_.myPtrAt (theRtSite, thePer) =
                        new WitPrSelPt   (theRtSite, thePer, this);
            }
   }

//------------------------------------------------------------------------------

void WitPrCoord::buildPrAltPts ()
   {
   WitRtCand *  theRtCand;
   WitRtSite *  theRtSite;
   WitPeriod    thePer;
   WitPrSelPt * thePrSelPt;

   myPrAltPts_.allocate1D (myProblem ());

   forEachEl (theRtCand, myRtMgr_->myRtCands ())
      {
      theRtSite = theRtCand->myRtSite ();

      if (not myPrSelPts_.myPtrTVecAt (theRtSite).isAllocated ())
         continue;

      myPrAltPts_.myPtrTVecAt (theRtCand).allocate (myProblem ());

      forEachPeriod (thePer, myProblem ())
         {
         if (not theRtCand->isEligible (thePer))
            continue;

         thePrSelPt = myPrSelPtFor (theRtSite, thePer);

         if (thePrSelPt == NULL)
            continue;

         myPrAltPts_.myPtrAt (theRtCand, thePer) =
            new WitPrAltPt (thePrSelPt, theRtCand, this);
         }
      }
   }

//------------------------------------------------------------------------------

void WitPrCoord::findActiveRtCands (
      WitRtSite *      theRtSite,
      WitPeriod        thePer,
      WitRtCandStack & theRtCands)
   {
   WitRtCand *  theRtCand;
   WitPrAltPt * thePrAltPt;

   witAssert (myPrSelPtFor (theRtSite, thePer) != NULL);

   theRtCands.clear ();

   forEachEl (theRtCand, theRtSite->myRtCands ())
      {
      thePrAltPt = myPrAltPts_.myPtrAt (theRtCand, thePer);

      if (thePrAltPt != NULL)
         if (thePrAltPt->active ())
            theRtCands.push (theRtCand);
      }

   theRtCands.reverse ();
   }

//------------------------------------------------------------------------------

void WitPrCoord::defineExpPairs (
      const WitRtCandStack & theRtCands,
      WitPeriod              thePer,
      double                 expVol,
      WitRtCandDblStack &    theExpPairs)
   {
   double                  totShare;
   WitObjStItr <WitRtCand> theItr;
   WitRtCand *             theRtCand;
   double                  expVolPerShare;
   double                  propExpVol;

   witAssert (theRtCands.isNonEmpty ());

   totShare = 0.0;

   theRtCands.attachItr (theItr);

   while (theItr.advance (theRtCand))
      totShare += theRtCand->myBillEnt ()->routingShare ()[thePer];

   expVolPerShare = expVol / totShare;

   theExpPairs.clear ();

   theRtCands.attachItr (theItr);

   while (theItr.advance (theRtCand))
      {
      propExpVol =
         expVolPerShare * theRtCand->myBillEnt ()->routingShare ()[thePer];

      theExpPairs.push (theRtCand, propExpVol);
      }

   witAssert (theExpPairs.isNonEmpty ());

   theExpPairs.reverse ();
   }

//------------------------------------------------------------------------------

WitPrSelPt * WitPrCoord::myPrSelPtFor (WitRtSite * theRtSite, WitPeriod thePer)
   {
   if (not myPrSelPts_.myPtrTVecAt (theRtSite).isAllocated ())
      return NULL;

   return myPrSelPts_.myPtrAt (theRtSite, thePer);
   }

//------------------------------------------------------------------------------
// Implementation of class PrSelMgr.
//------------------------------------------------------------------------------

WitPrSelMgr::~WitPrSelMgr ()
   {
   }

//------------------------------------------------------------------------------

WitPrSelMgr::WitPrSelMgr (WitPrCoord * thePrCoord):

      WitSelMgr  (thePrCoord->mySelector ()),
      myPrCoord_ (thePrCoord)
   {
   }

//------------------------------------------------------------------------------

bool WitPrSelMgr::selSplitNeeded ()
   {
   return myGlobalComp ()->selSplit ();
   }

//------------------------------------------------------------------------------
// Implementation of class PprSelMgr.
//------------------------------------------------------------------------------

WitPprSelMgr::WitPprSelMgr (WitPrCoord * thePrCoord):

      WitPrSelMgr (thePrCoord)
   {
   }

//------------------------------------------------------------------------------

WitPprSelMgr::~WitPprSelMgr ()
   {
   }

//------------------------------------------------------------------------------

WitAltPt * WitPprSelMgr::myAltPtFor (
      WitBopEntry * theBopEnt,
      WitConsEntry *,
      WitPeriod     execPer,
      WitPeriod)
   {
   WitRtCand * theRtCand;
   WitPeriod   expPer;

   theRtCand = mySelector ()->myRtMgr ()->myRtCandFor (theBopEnt);

   expPer    = theBopEnt->impactPeriod ()[execPer];

   return myPrCoord ()->myAltPtFor (theRtCand, expPer);
   }

//------------------------------------------------------------------------------

bool WitPprSelMgr::validForNetting ()
   {
   return true;
   }

//------------------------------------------------------------------------------

bool WitPprSelMgr::validForExecBounds ()
   {
   return true;
   }

//------------------------------------------------------------------------------

bool WitPprSelMgr::validForLTBounds ()
   {
   return false;
   }

//------------------------------------------------------------------------------

const char * WitPprSelMgr::selCaseName ()
   {
   return "Production Proportionate Routing";
   }

//------------------------------------------------------------------------------
// Implementation of class CprSelMgr.
//------------------------------------------------------------------------------

WitCprSelMgr::WitCprSelMgr (WitPrCoord * thePrCoord):

      WitPrSelMgr (thePrCoord)
   {
   }

//------------------------------------------------------------------------------

WitCprSelMgr::~WitCprSelMgr ()
   {
   }

//------------------------------------------------------------------------------

WitAltPt * WitCprSelMgr::myAltPtFor (
      WitBopEntry *,
      WitConsEntry * theConsEnt,
      WitPeriod      execPer,
      WitPeriod)
   {
   WitRtCand * theRtCand;

   theRtCand = mySelector ()->myRtMgr ()->myRtCandFor (theConsEnt);

   return myPrCoord ()->myAltPtFor (theRtCand, execPer);
   }

//------------------------------------------------------------------------------

bool WitCprSelMgr::validForNetting ()
   {
   return false;
   }

//------------------------------------------------------------------------------

bool WitCprSelMgr::validForExecBounds ()
   {
   return false;
   }

//------------------------------------------------------------------------------

bool WitCprSelMgr::validForLTBounds ()
   {
   return false;
   }

//------------------------------------------------------------------------------

const char * WitCprSelMgr::selCaseName ()
   {
   return "Consumption Proportionate Routing";
   }

//------------------------------------------------------------------------------
// Implementation of class PrSelPt.
//------------------------------------------------------------------------------

WitPrSelPt::WitPrSelPt (
         WitRtSite *  theRtSite,
         WitPeriod    thePer,
         WitPrCoord * thePrCoord):

      WitSelPt         (),
      myRtSite_        (theRtSite),
      myPer_           (thePer),
      myPrCoord_       (thePrCoord),
      nAllowedRtCands_ (0)
   {
   }

//------------------------------------------------------------------------------

WitPrSelPt::~WitPrSelPt ()
   {
   }

//------------------------------------------------------------------------------

void WitPrSelPt::print ()
   {
   fprintf (myPrCoord_->msgFile (),
      "\n"
      "Proportionate Routing Point:\n"
      "\n");

   myRtSite_->printID          (4, 22);
   myRtSite_->printPer (myPer_, 4, 22);
   }

//------------------------------------------------------------------------------

void WitPrSelPt::incAllowedRtCands (int delta)
   {
   if (myPrCoord_->mySelector ()->sglSrc ())
      myPrCoord_->
         mySelector ()->
            mySglSrcMgr ()->
               saveConfigVal (nAllowedRtCands_);

   nAllowedRtCands_ += delta;

   witAssert (nAllowedRtCands_ >= 1);
   }

//------------------------------------------------------------------------------

void WitPrSelPt::printAllowedRtCands ()
   {
   WitRtCand *  theRtCand;
   WitPrAltPt * thePrAltPt;

   fprintf (myPrCoord_->msgFile (),
      "\n"
      "   Allowed Routing Candidates:\n");

   forEachEl (theRtCand, myRtSite_->myRtCands ())
      {
      thePrAltPt = myPrCoord_->myPrAltPtFor (theRtCand, myPer_);

      if (thePrAltPt != NULL)
         if (thePrAltPt->allowed ())
            theRtCand->printID (7, 22);
      }
   }

//------------------------------------------------------------------------------

WitSelMgr * WitPrSelPt::mySelMgr ()
   {
   return
      myRtSite_->isForProd ()?
         myPrCoord_->myPrSelMgrForProd ():
         myPrCoord_->myPrSelMgrForCons ();
   }

//------------------------------------------------------------------------------

void WitPrSelPt::prtID () 
   {
   myRtSite_->printID (4, 22);

   fprintf (myRtSite_->msgFile (), "   Period:           %d\n", myPer_);
   }

//------------------------------------------------------------------------------

bool WitPrSelPt::splittable () 
   {
   if (myPrCoord_->myGlobalComp ()->selSplit ())
      return true;

   if (myPrCoord_->myGlobalComp ()->nonMrSelSplit ())
      return true;

   return false;
   }

//------------------------------------------------------------------------------

void WitPrSelPt::getSortData (WitPart * & thePart, WitPeriod & thePer)
   {
   thePart = myRtSite_->mrSortPart ();

   thePer  = myPer_;
   }

//------------------------------------------------------------------------------

WitSelCand * WitPrSelPt::newSelCand ()
   {
   return new WitPrCand (myPrCoord_, myRtSite_, myPer_);
   }

//------------------------------------------------------------------------------
// Implementation of class PrAltPt.
//------------------------------------------------------------------------------

WitPrAltPt::WitPrAltPt (
         WitPrSelPt * thePrSelPt,
         WitRtCand *  theRtCand,
         WitPrCoord * thePrCoord):

      WitAltPt   (),
      myPrSelPt_ (thePrSelPt),
      myRtCand_  (theRtCand),
      myPrCoord_ (thePrCoord),
      active_    (true),
      allowed_   (true)
   {  
   witAssert (myPrSelPt_ != NULL);

   myPrSelPt_->incAllowedRtCands (+1);
   }

//------------------------------------------------------------------------------

WitPrAltPt::~WitPrAltPt ()
   {
   }

//------------------------------------------------------------------------------

void WitPrAltPt::setStatus (bool activeVal, bool allowedVal)
   {
   if (DEBUG_MODE)
      {
      if (activeVal)
         witAssert (allowedVal);

      if (not myPrCoord_->mySelector ()->tbprMode ())
         witAssert (activeVal == allowedVal);
      }

   if (myPrCoord_->mySelector ()->sglSrc ())
      {
      myPrCoord_->mySelector ()->mySglSrcMgr ()->saveConfigVal (allowed_);
      myPrCoord_->mySelector ()->mySglSrcMgr ()->saveConfigVal (active_);
      }

   if (allowedVal != allowed_)
      myPrSelPt_->incAllowedRtCands (allowedVal? +1: -1);

   allowed_ = allowedVal;
   active_  = activeVal;
   }

//------------------------------------------------------------------------------

void WitPrAltPt::recoverInitState ()
   {
   if (not allowed_)
      myPrSelPt_->incAllowedRtCands (+1);

   allowed_ = true;
   active_  = true;

   if (myPrCoord_->selPrintLevelAt (3))
      printRecovery ();
   }

//------------------------------------------------------------------------------

bool WitPrAltPt::hasResAlt ()
   {
   return (myPrSelPt_->nAllowedRtCands () > 1);
   }

//------------------------------------------------------------------------------

void WitPrAltPt::alterSelection ()
   {
   witAssert (myPrSelPt_->nAllowedRtCands () > 1);

   setStatus (false, false);
   }

//------------------------------------------------------------------------------

void WitPrAltPt::storeRecoveryPt ()
   {
   myPrCoord_->storeRecoveryPair (myRtCand_, myPrSelPt_->myPer ());
   }

//------------------------------------------------------------------------------

void WitPrAltPt::tempAlterSel ()
   {
   witAssert (active_);
   witAssert (allowed_);

   witAssert (myPrSelPt_->nAllowedRtCands () > 1);

   setStatus (false, false);
   }

//------------------------------------------------------------------------------

void WitPrAltPt::cancelTempAlt ()
   {
   witAssert (not active_);
   witAssert (not allowed_);

   setStatus (true, true);
   }

//------------------------------------------------------------------------------

WitSelPt * WitPrAltPt::mySelPt ()
   {
   return myPrSelPt_;
   }

//------------------------------------------------------------------------------

void WitPrAltPt::printAlteration ()
   {
   if (not myRtCand_->selPrintLevelAt (3))
      return;

   fprintf (myRtCand_->msgFile (),
      "\n"
      "Proportionate Routing Candidate Disallowance:\n\n");

   myRtCand_->myRtSite ()->printID                        (4, 22);
   myRtCand_->myRtSite ()->printPer (myPrSelPt_->myPer (), 4, 22);
   myRtCand_->             printID                        (7, 22);

   myPrSelPt_->printAllowedRtCands ();
   }

//------------------------------------------------------------------------------

void WitPrAltPt::printRecovery ()
   {
   fprintf (myRtCand_->msgFile (),
      "   Proportionate Routing Candidate Recovered:\n");

   myRtCand_->myRtSite ()->printID                        (4, 22);
   myRtCand_->myRtSite ()->printPer (myPrSelPt_->myPer (), 4, 22);
   myRtCand_->             printID                        (7, 22);

   fprintf (myRtCand_->msgFile (), "\n");
   }

//------------------------------------------------------------------------------
// Implementation of class PrCand.
//------------------------------------------------------------------------------

WitPrCand::WitPrCand (
         WitPrCoord * thePrCoord,
         WitRtSite *  theRtSite,
         WitPeriod    thePer):

      WitSelCand  (thePrCoord->myProblem ()),
      actRtCands_ (myProblem ())
   {
   WitRtCand * theRtCand;

   forEachEl (theRtCand, theRtSite->myRtCands ())
      if (thePrCoord->active (theRtCand, thePer))
         actRtCands_.push (theRtCand);

   actRtCands_.reverse ();

   witAssert (actRtCands_.isNonEmpty ());
   }

//------------------------------------------------------------------------------

WitPrCand::~WitPrCand ()
   {
   }

//------------------------------------------------------------------------------

void WitPrCand::prtID ()
   {
   WitObjStItr <WitRtCand> theItr;
   WitRtCand *             theRtCand;

   actRtCands_.attachItr (theItr);

   while (theItr.advance (theRtCand))
      theRtCand->printID (7, 22);
   }

//------------------------------------------------------------------------------

void WitPrCand::getPrData (WitRtCandStack & theRtCands)
   {
   theRtCands.revCopyFrom (actRtCands_);

   theRtCands.reverse ();
   }
