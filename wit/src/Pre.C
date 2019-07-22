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
// Source file: "Pre.C"
//
// Contains the implementation of class Preprocessor.
//------------------------------------------------------------------------------

#include <Pre.h>
#include <Post.h>
#include <NodeSorter.h>
#include <OffsetProc.h>
#include <Below.h>
#include <AvSorter.h>
#include <GlobalComp.h>
#include <OptComp.h>
#include <Material.h>
#include <Operation.h>
#include <BomEntry.h>
#include <SubEntry.h>
#include <BopEntry.h>
#include <Demand.h>
#include <BoundSet.h>
#include <StochImpMgr.h>
#include <Timing.h>
#include <DetOptImpMgr.h>
#include <OptStarter.h>
#include <MsgFac.h>
#include <PtrTVec.h>

//------------------------------------------------------------------------------
// Implementation of class Preprocessor.
//------------------------------------------------------------------------------

WitPreprocessor::WitPreprocessor (WitProblem * theProblem):

      WitProbAssoc   (theProblem),
      preprocessing_ (false),
      preprocessed_  (false),
      myNodeSorter_  (NULL),
      myOffsetProc_  (NULL),
      myBelowLister_ (NULL)
   {
   }

//------------------------------------------------------------------------------

WitPreprocessor::~WitPreprocessor ()
   {
   }

//------------------------------------------------------------------------------

void WitPreprocessor::preprocess ()
   {
   WitBomEntry * theBomEnt;

   if (preprocessed_)
      return;

   myMsgFac () ("preProcMsg");

   setUp ();

   checkMaxNameLength ();

   myNodeSorter ()->sortNodesIfNeeded ();

   myOffsetProc ()->roundOffsets ();

   findMandECs ();

   validateBLA ();

   procLotSizes ();

   compHasSubsInEffect ();

   compEffConsRates ();

   forEachBomEntry (theBomEnt, myProblem ())
      checkMinLotSize (theBomEnt);

   compEffProdRates ();

   chooseExpExecPeriods ();

   checkSmallEffProdRates ();

   sortBopEntries ();
   sortSubEntries ();

   chooseExpBopEntries ();

   findExpPerGaps ();

   myBelowLister ()->buildBelow ();

   shutDown ();
   }

//------------------------------------------------------------------------------

void WitPreprocessor::getExpCycle (
      WitObjStack <WitPart> &      theParts,
      WitObjStack <WitOperation> & theOpns)
   {
   theParts.clear ();
   theOpns .clear ();

   if (preprocessed_)
      return;

   setUp ();

   myNodeSorter ()->getExpCycle (theParts, theOpns);

   shutDown ();
   }

//------------------------------------------------------------------------------

void WitPreprocessor::unpreprocess ()
   {
   WitComponent * theComp;

   stronglyAssert (not myStochImpMgr ()->stochMode ());

   if (not preprocessed_)
      return;

   myProblem ()->resetSoln ();

   myDetOptImpMgr ()->shutDown ();

   myOptComp ()->crashOptStarter ()->beChosen ();

   preprocessed_ = false;

   forEachEl (theComp, myCompMgr ()->allComponents ())
      theComp->unpreprocess ();
   }

//------------------------------------------------------------------------------

void WitPreprocessor::setUp ()
   {
   myMsgFac ().abortIfErrors ();

   WitTimer::enterSection ("preproc");

   preprocessing_ = true;

   myNodeSorter_  = new WitNodeSorter  (myProblem ());
   myOffsetProc_  = new WitOffsetProc  (myProblem ());
   myBelowLister_ = new WitBelowLister (myProblem ());
   }

//------------------------------------------------------------------------------

void WitPreprocessor::shutDown ()
   {
   delete myBelowLister_;
   delete myOffsetProc_;
   delete myNodeSorter_;

   myBelowLister_ = NULL;
   myOffsetProc_  = NULL;
   myNodeSorter_  = NULL;

   preprocessing_ = false;
   preprocessed_  = true;

   WitTimer::leaveSection ("preproc");
   }

//------------------------------------------------------------------------------

void WitPreprocessor::checkMaxNameLength ()
   {
   int       maxNameLength;
      //
      // Maximum length of theNode->nodeName () and
      // theDemand->demandName () over all Nodes and Demands.

   WitString theLongestName;
      //
      // A name whose length is maxNameLength.

   int       nominalNameLength;
      //
      // The max name length allowed without a warning.

   WitPart *      thePart;
   WitOperation * theOpn;
   WitDemand *    theDemand;

   maxNameLength  = 0;
   theLongestName = "";

   forEachPart (thePart, myProblem ())
      updateLongestName (
         thePart->partName (),
         maxNameLength,
         theLongestName);

   forEachOperation (theOpn, myProblem ())
      updateLongestName (
         theOpn->operationName (),
         maxNameLength,
         theLongestName);

   forEachDemand (theDemand, myProblem ())
      updateLongestName (
         theDemand->demandName (),
         maxNameLength,
         theLongestName);

   nominalNameLength = 12;

   if (maxNameLength > nominalNameLength)
      myMsgFac () ("longNameWmsg", nominalNameLength, theLongestName);
   }

//------------------------------------------------------------------------------

void WitPreprocessor::updateLongestName (
      const WitString & theName,
      int &             maxNameLength,
      WitString &       theLongestName)
   {
   if (theName.length () > maxNameLength)
      {
      maxNameLength  = theName.length ();
      theLongestName = theName;
      }
   }

//------------------------------------------------------------------------------

void WitPreprocessor::validateBLA ()
   {
   WitDemand * theDemand;
   WitPeriod   thePer;

   forEachDemand (theDemand, myProblem ())
      if (not theDemand->shipLateAllowed ())
         {
         if (theDemand->cumShipReward () != 0.0)
            myMsgFac () ("cumShipRewardWoShipLateSmsg",
               theDemand->demandedPartName (),
               theDemand->demandName ());

         if (theDemand->cumShipBounds ()->softLB () != 0.0)
            myMsgFac () ("cumShipSLBWoShipLateSmsg",
               theDemand->demandedPartName (),
               theDemand->demandName ());

         forEachPeriod (thePer, myProblem ())
            if (theDemand->cumShipBounds ()->hardUBIsFinite (thePer))
               myMsgFac () ("cumShipHUBWoShipLateSmsg",
                  theDemand->demandedPartName (),
                  theDemand->demandName ());
         }
   }

//------------------------------------------------------------------------------

void WitPreprocessor::procLotSizes ()
   {
   bool           lotSizesExistVal;
   WitOperation * theOpn;
   WitPeriod      thePer;

   lotSizesExistVal = false;

   forEachOperation (theOpn, myProblem ())
      forEachPeriod (thePer, myProblem ())
         if (theOpn->incLotSize ()[thePer] > 0.0)
            lotSizesExistVal = true;

   myGlobalComp ()->storePreLotSizesExist (lotSizesExistVal);

   forEachOperation (theOpn, myProblem ())
      if (theOpn->twoLevelLotSizes ())
         forEachPeriod (thePer, myProblem ())
            {
            if (theOpn->incLotSize ()[thePer] < 0.9)
               myMsgFac () ("incLotSizeLT1Smsg",
                  theOpn->operationName ().myCstring (),
                  thePer,
                  theOpn->incLotSize ()[thePer]);

            if (theOpn->incLotSize2 ()[thePer] < 0.9)
               myMsgFac () ("incLotSize2LT1Smsg",
                  theOpn->operationName ().myCstring (),
                  thePer,
                  theOpn->incLotSize2 ()[thePer]);
            }
   }

//------------------------------------------------------------------------------

void WitPreprocessor::compHasSubsInEffect ()
   {
   WitBomEntry *  theBomEnt;
   WitPeriod      thePer;
   WitSubEntry *  theSub;
   WitTVec <bool> hasSubsInEffectVec (myProblem (), false);

   forEachBomEntry (theBomEnt, myProblem ())
      {
      hasSubsInEffectVec = false;

      if (theBomEnt->mySubEntries ().isEmpty ())
         continue;

      forEachPeriod (thePer, myProblem ())
         forEachEl (theSub, theBomEnt->mySubEntries ())
            if (theSub->inEffect (thePer))
               {
               hasSubsInEffectVec[thePer] = true;

               break;
               }

      theBomEnt->storePreHasSubsInEffect (hasSubsInEffectVec);
      }
   }

//------------------------------------------------------------------------------

void WitPreprocessor::compEffConsRates ()
   {
   WitTVec <double> effConsRateVec;
   WitBomEntry *    theBomEnt;
   WitSubEntry *    theSub;

   effConsRateVec.allocate (myProblem (), 0.0);

   forEachBomEntry (theBomEnt, myProblem ())
      compEffConsRate (theBomEnt, effConsRateVec);

   forEachSubEntry (theSub, myProblem ())
      compEffConsRate (theSub, effConsRateVec);
   }

//------------------------------------------------------------------------------

void WitPreprocessor::compEffConsRate (
      WitConsEntry *     theConsEnt,
      WitTVec <double> & effConsRateVec)
   {
   WitPeriod execPer;
   double    effConsRateVal;

   if (theConsEnt->consRate () == theConsEnt->consRate ()[0])
      {
      effConsRateVal =
           theConsEnt->consRate ()[0]
         / (1.0 - theConsEnt->falloutRate ());

      theConsEnt->storePreEffConsRate (effConsRateVal);
      }
   else
      {
      forEachPeriod (execPer, myProblem ())
         effConsRateVec[execPer] =
              theConsEnt->consRate ()[execPer] 
            / (1.0 - theConsEnt->falloutRate ());

      theConsEnt->storePreEffConsRate (effConsRateVec);
      }
   }

//------------------------------------------------------------------------------

void WitPreprocessor::checkMinLotSize (WitBomEntry * theBomEnt)
   {
   WitPart *      thePart;
   WitOperation * theOpn;
   WitPeriod      execPer; // Execution period.
   WitPeriod      tCons;   // Consumption period.
   double         avail;   // Availability.
   double         minCons; // Consumption for minLotSize.

   if (not myGlobalComp ()->lotSizesExist ())
      return;

   thePart = theBomEnt->myPart ();
   theOpn  = theBomEnt->myOperation ();

   if (thePart->thisCap () == NULL)
      return;

   forEachPeriod (execPer, myProblem ())
      {
      if (not theBomEnt->inEffect (execPer))
         continue;

      if (theOpn->minLotSize ()[execPer] == 0.0)
         continue;

      tCons = theBomEnt->impactPeriod ()[execPer];

      avail = thePart->supplyVol ()[tCons];

      if (avail == 0.0)
         continue;

      minCons = 
           theOpn   ->minLotSize  ()[execPer]
         * theBomEnt->effConsRate ()[execPer];

      if (minCons > avail + FLOAT_EPSILON)
         {
         myMsgFac () ("minLotSizeTooLargeWmsg",
            theOpn->operationName (),
            execPer,
            minCons,
            thePart->partName (),
            tCons,
            avail);

         return;
         }
      }
   }

//------------------------------------------------------------------------------

void WitPreprocessor::compEffProdRates ()
   {
   WitBopEntry *    theBopEnt;
   WitPeriod        execPer;
   WitTVec <double> effProdRateVec;

   effProdRateVec.allocate (myProblem (), 0.0);

   forEachBopEntry (theBopEnt, myProblem ())
      {
      forEachPeriod (execPer, myProblem ())
         effProdRateVec[execPer] =
            theBopEnt->inEffect (execPer)?
                 theBopEnt->productRate ()[execPer]
               * theBopEnt->myOperation ()->yieldRate ()[execPer]:
               0.0;

      theBopEnt->storePreEffProdRate (effProdRateVec);
      }
   }

//------------------------------------------------------------------------------

void WitPreprocessor::chooseExpExecPeriods ()
   {
   WitTVec <WitPeriod> expExecPeriodVec;
   WitBopEntry *       theBopEnt;
   WitPeriod           execPer;
   double              effProd;
   WitPeriod           prodPer;

   expExecPeriodVec.allocate (myProblem (), -1);

   forEachBopEntry (theBopEnt, myProblem ())
      {
      if (not theBopEnt->expAllowed ())
         continue;

      expExecPeriodVec = -1;

      forEachPeriod (execPer, myProblem ())
         {
         effProd = theBopEnt->effProdRate ()[execPer];

         if (effProd >= myGlobalComp ()->expCutoff ())
            {
            prodPer                   = theBopEnt->impactPeriod ()[execPer];

            expExecPeriodVec[prodPer] = execPer;
            }

         theBopEnt->storePreExpExecPeriod (expExecPeriodVec);
         }
      }
   }

//------------------------------------------------------------------------------

void WitPreprocessor::checkSmallEffProdRates ()
   {
   double        minAccEffProd;
   WitBopEntry * minAccBopEnt;
   WitBopEntry * theBopEnt;
   WitPeriod     execPer;
   double        effProd;
   WitPeriod     minAccPer;
   double        maxRejEffProd;
   WitBopEntry * maxRejBopEnt;
   WitPeriod     maxRejPer;

   minAccEffProd = WitGlobalComp::defExpCutoff ();
   maxRejEffProd = 0.0;

   forEachBopEntry (theBopEnt, myProblem ())
      if (theBopEnt->expAllowed ())
         forEachPeriod (execPer, myProblem ())
            {
            effProd = theBopEnt->effProdRate ()[execPer];

            if (effProd >= myGlobalComp ()->expCutoff ())
               {
               if (effProd < minAccEffProd)
                  {
                  minAccEffProd = effProd;
                  minAccBopEnt  = theBopEnt;
                  minAccPer     = execPer;
                  }
               }
            else 
               if (effProd > maxRejEffProd)
                  {
                  maxRejEffProd = effProd;
                  maxRejBopEnt  = theBopEnt;
                  maxRejPer     = execPer;
                  }
            }

   if (maxRejEffProd > 0.0)
      myMsgFac () ("rejBopEntWmsg",
         myGlobalComp ()->expCutoff (),
         maxRejEffProd,
         maxRejBopEnt->myOperationName (),
         maxRejBopEnt->localIndex (),
         maxRejPer);

   if (minAccEffProd < WitGlobalComp::defExpCutoff () - FLOAT_EPSILON)
      myMsgFac () ("smallEffProdWmsg",
         myGlobalComp ()->expCutoff (),
         WitGlobalComp::defExpCutoff (),
         minAccEffProd,
         minAccBopEnt->myOperationName (),
         minAccBopEnt->localIndex (),
         minAccPer);
   }

//------------------------------------------------------------------------------

void WitPreprocessor::sortBopEntries ()
   {
   WitAvSorter <WitBopEntry> theSorter (myCompMgr ()->nBopEntries ());
   WitPart *                 thePart;
   WitList <WitBopEntry>     theBopEntries;

   forEachPart (thePart, myProblem ())
      {
      theBopEntries = thePart->producingBopEntries ();

      theSorter.sort (theBopEntries);

      thePart->storePreProducingBopEntries (theBopEntries);
      }
   }

//------------------------------------------------------------------------------

void WitPreprocessor::sortSubEntries ()
   {
   WitAvSorter <WitSubEntry> theSorter (myCompMgr ()->nSubEntries ());
   WitBomEntry *             theBomEnt;
   WitList <WitSubEntry>     theSubs;

   forEachBomEntry (theBomEnt, myProblem ())
      {
      theSubs = theBomEnt->mySubEntries ();

      theSorter.sort (theSubs);

      theBomEnt->storePreMySubEntries (theSubs);
      }
   }

//------------------------------------------------------------------------------

void WitPreprocessor::chooseExpBopEntries ()
   {
   WitPtrTVec <WitBopEntry> expBopEntryVec;
   WitPart *                thePart;
   WitBopEntry *            theBopEnt;
   WitPeriod                thePer;

   expBopEntryVec.allocate (myProblem ());

   forEachPart (thePart, myProblem ())
      {
      if (thePart->producingBopEntries ().isEmpty ())
         continue;

      expBopEntryVec.setToNull ();

      forEachPeriod (thePer, myProblem ())
         forEachEl (theBopEnt, thePart->producingBopEntries ())
            if (theBopEnt->expExecPeriod ()[thePer] >= 0)
               {
               expBopEntryVec[thePer] = theBopEnt;

               break;
               }

      thePart->storePreExpBopEntry (expBopEntryVec);
      }
   }

//------------------------------------------------------------------------------

void WitPreprocessor::findExpPerGaps ()
   {
   bool          expPerGapsVal;
   WitPart *     thePart;
   WitMaterial * theMat;
   WitPeriod     thePer;

   forEachPart (thePart, myProblem ())
      {
      expPerGapsVal = false;

      theMat        = thePart->thisMat ();

      if (theMat != NULL)
         if (not theMat->producingBopEntries ().isEmpty ())
            for (thePer = 1; thePer < nPeriods (); thePer ++)
               if (not theMat->explodeable (thePer))
                  if (theMat->explodeable (thePer - 1))
                     if (not theMat->mandEC ()[thePer - 1])
                        {
                        expPerGapsVal = true;

                        break;
                        }

      thePart->storePreExpPerGaps (expPerGapsVal);
      }  
   }

//------------------------------------------------------------------------------
// findMandECs.
// Determines mandEC for each Material in theProblem.
//
// A Material theMat will have a mandEC in thePer, iff
// * There is a BopEntry producing theMat in thePer, and
// * The producing Operation incurs a mandEC in the corresponding exec period.
//
// An Operation, theOpn, can incur a mandEC for either of two reasons:
//
// Direct case:
//    A BomEntry, theBomEnt, in its BOM has a mandEC. In this case,
//    theOpn incurs a mandEC in periods
//    theBomEnt->earliestPeriod () - 1  and theBomEnt->latestPeriod (),
//    but not in lastPeriod ().
//
// Propagated case:
//    theOpn has a ConsEntry, theConsEnt, whose consumed Part is a
//    Material, theMat, that incurs a mandEC. In this case, let tImp be a
//    period in which theMat incurs a mandEC. theOpn incurs its
//    mandEC in the last period execPer, such that
//    theConsEnt->impactPeriod ()[execPer] <= tImp.
//    In other words, the mandEC occurs in the last period in which one can
//    execute theOpn by consuming theMat before or during its
//    mandEC. The mandEC periods, execPer, are found by iterating backwards on
//    tImp and then scanning backwards for execPer, starting with the last 
//    execPer found. This works, because the execPer value for a given tImp is
//    an upper bound on the execPer value of any lower tImp.
//
//    The purpose of the propagated case is to prevent obsolete material from
//    being implicitly stocked by building a product early. The propagation
//    logic implictly assumes that the impact periods of the BopEntries are a
//    non-decreasing function of the exec periods, i.e.,
//    t1 <= t2 ==> impactPeriod ()[t1] <= impactPeriod ()[t2]
//    If this assumption fails to hold, the mandECs generate might not
//    prevent obsolete material from being implicitly stocked, but this is a
//    pathological case.
//
// Note that the mandECs for Operations are not stored: As soon as one is
// found, the corresponding Material mandECs are generated.
//------------------------------------------------------------------------------

void WitPreprocessor::findMandECs ()
   {
   WitBomEntry * theBomEnt;
   WitSubEntry * theSub;

   forEachBomEntry (theBomEnt, myProblem ())
      setDirectMandECs (theBomEnt);

   forEachBomEntry (theBomEnt, myProblem ())
      propMandECs (theBomEnt);

   forEachSubEntry (theSub, myProblem ())
      propMandECs (theSub);
   }

//------------------------------------------------------------------------------

void WitPreprocessor::setDirectMandECs (WitBomEntry * theBomEnt)
   {
   if (not theBomEnt->mandEC ())
      return;

   if (theBomEnt->earliestPeriod () > 0)
      setMandECs (
         theBomEnt->myOperation (),
         theBomEnt->earliestPeriod () - 1);

   if (theBomEnt->latestPeriod () < lastPeriod ())
      setMandECs (
         theBomEnt->myOperation (),
         theBomEnt->latestPeriod ());
   }

//------------------------------------------------------------------------------

void WitPreprocessor::propMandECs (WitConsEntry * theConsEnt)
   {
   WitMaterial * theMat;
   WitPeriod     execPer;
   WitPeriod     thePer;

   theMat = theConsEnt->myPart ()->thisMat ();

   if (theMat == NULL)
      return;

   execPer = lastPeriod ();

   for (thePer = lastPeriod (); thePer >= 0; -- thePer)
      if (theMat->mandEC ()[thePer])
         for (; execPer >= 0; -- execPer)
            if (theConsEnt->inEffect (execPer))
               if (theConsEnt->impactPeriod ()[execPer] <= thePer)
                  {
                  setMandECs (theConsEnt->myOperation (), execPer);

                  break;
                  }
   }

//------------------------------------------------------------------------------

void WitPreprocessor::setMandECs (WitOperation * theOpn, WitPeriod thePer)
   {
   WitMaterial * theMat;
   WitBopEntry * theBopEnt;
   WitPeriod     prodPer;

   forEachEl (theBopEnt, theOpn->bop ())
      {
      theMat = theBopEnt->myPart ()->thisMat ();

      if (theMat != NULL)
         if (theBopEnt->inEffect (thePer))
            {
            prodPer = theBopEnt->impactPeriod ()[thePer];

            theMat->storePreMandEC (prodPer, true);
            }
      }
   }
