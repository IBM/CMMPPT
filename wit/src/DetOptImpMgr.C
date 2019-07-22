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
// Source file: "DetOptImpMgr.C"
//
// Contains the implementation of class DetOptImpMgr.
//------------------------------------------------------------------------------

#include <DetOptImpMgr.h>
#include <ExtOptMgr.h>
#include <StochImpMgr.h>
#include <MultiObjMgr.h>
#include <Objective.h>
#include <OptStarter.h>
#include <DetImpOP.h>
#include <DetVars.h>
#include <DetCons.h>
#include <HeurImp.h>
#include <wit/src/Variant.h>
#include <Pre.h>
#include <Post.h>
#include <Part.h>
#include <Demand.h>
#include <Operation.h>
#include <SubEntry.h>
#include <BopEntry.h>
#include <GlobalComp.h>
#include <OptComp.h>
#include <BoundSet.h>
#include <wit.h>
#include <Timing.h>
#include <MsgFrag.h>
#include <MsgFac.h>

typedef struct std::pair <WitPart *, WitPeriod> WitPartPer;

//------------------------------------------------------------------------------
// Implementation of class DetOptImpMgr.
//------------------------------------------------------------------------------

WitDetOptImpMgr::WitDetOptImpMgr (WitProblem * theProblem):

      WitProbAssoc   (theProblem),

      myDetImpOP_    (NULL),
      extOptActive_  (false),
      myExtOptMgr_   (NULL),
      accelerated_   (false),
      clearingSoln_  (false),
      uploadingSoln_ (false),
      revCritList_   (myProblem ()),
      potential_     ()
   {                
   }

//------------------------------------------------------------------------------

WitDetOptImpMgr::~WitDetOptImpMgr ()
   {
   shutDown ();

   delete myExtOptMgr_;
   }

//------------------------------------------------------------------------------

void WitDetOptImpMgr::shutDown ()
   {
   if (myDetImpOP_ == NULL)
      return;

   if (accelerated ())
      {
      accelerated_ =
         myMsgFac ().displayStateChange (
            "unaccStateMsg",
            accelerated (),
            false);

      myOptComp ()->crashOptStarter ()->beChosen ();
      }

   delete myDetImpOP_;

   myDetImpOP_ = NULL;
   }

//------------------------------------------------------------------------------

void WitDetOptImpMgr::implode ()
   {
   WitTimer::enterSection ("opt-prob");

   myMsgFac () ("optImpMsg");

   myOptComp ()->requireCplex ();

   if (myOptComp ()->multiObjMode ())
      myOptComp ()->myMultiObjMgr ()->validateData ();

   startOptImp  ();

   myDetImpOP ()->solve ();

   myProblem ()->optVariant ()->becomesCurrent ();

   finishOptImp ();

   WitTimer::leaveSection ("opt-prob");
   }

//------------------------------------------------------------------------------

void WitDetOptImpMgr::evalObjectives ()
   {
   WitTimer::enterSection ("opt-prob");

   myProblem ()->inputVariant ()->becomesCurrent ();

   myPreprocessor ()->preprocess ();

   setUpOptProblem ();

   if (myOptComp ()->printOptProblem ())
      myDetImpOP ()->print ();

   myPostprocessor ()->postprocess ();

   if (not myGlobalComp ()->feasible ())
      myMsgFac () ("infeasObjWmsg");

   myDetImpOP ()->calcInitSoln ();

   compObjectives ();

   WitTimer::leaveSection ("opt-prob");
   }

//------------------------------------------------------------------------------

bool WitDetOptImpMgr::boundsObjUsed () const
   {
   return myDetImpOP ()->slbvVarsExist ();
   }

//------------------------------------------------------------------------------

void WitDetOptImpMgr::startExtOpt ()
   {
   WitTimer::enterSection ("opt-prob");

   stronglyAssert (not myGlobalComp ()->computeCriticalList ());
   stronglyAssert (not myOptComp    ()->compPrices          ());
   stronglyAssert (not myOptComp    ()->accAfterOptImp      ());
   stronglyAssert (not myOptComp    ()->multiObjMode        ());
   stronglyAssert (not extOptActive_);

   myMsgFac () ("startExtOptMsg");

   extOptActive_ = true;

   startOptImp ();

   myExtOptMgr_ = new WitExtOptMgr (myDetImpOP ());

   WitTimer::leaveSection ("opt-prob");
   }

//------------------------------------------------------------------------------

void WitDetOptImpMgr::finishExtOpt ()
   {
   WitTimer::enterSection ("opt-prob");

   stronglyAssert (extOptActive_);

   stronglyAssert (myExtOptMgr_->solnProvided ());

   myMsgFac () ("finishExtOptMsg");

   delete myExtOptMgr_;

   myExtOptMgr_ = NULL;

   myProblem ()->extOptVariant ()->becomesCurrent ();

   finishOptImp ();

   extOptActive_ = false;

   WitTimer::leaveSection ("opt-prob");
   }

//------------------------------------------------------------------------------

void WitDetOptImpMgr::shutDownExtOpt ()
   {
   WitTimer::enterSection ("opt-prob");

   stronglyAssert (extOptActive_);

   myMsgFac () ("shutDownExtOptMsg");

   delete myExtOptMgr_;

   myExtOptMgr_ = NULL;

   shutDown ();

   WitTimer::leaveSection ("opt-prob");
   }

//------------------------------------------------------------------------------

void WitDetOptImpMgr::startOptImp ()
   {
   WitComponent * theComp;

   myProblem ()->resetSoln ();

   if (myCompMgr ()->nParts () == 0)
      myMsgFac () ("noPartsSmsg");

   if (myOptComp ()->mipMode ())
      validateMipMode ();

   myPreprocessor ()->preprocess ();

   if (not extOptActive_)
      compInitSoln ();

   setUpOptProblem ();

   if (not extOptActive_)
      if (myOptComp ()->optInitMethod ()->external ())
         compInitOptSoln ();

   clearingSoln_ = true;

   forEachEl (theComp, myCompMgr ()->allComponents ())
      theComp->clearOptSoln ();

   clearingSoln_ = false;
   }

//------------------------------------------------------------------------------

void WitDetOptImpMgr::finishOptImp ()
   {
   if (myOptComp ()->printOptSoln ())
      myDetImpOP ()->writeSoln ("opSoln.out");

   buildCritList ();

   uploadSoln ();

   compObjectives ();

   myOptComp ()->writeObjValue ();

   if (myGlobalComp ()->inputPauses ())
      myProblem ()->pauseForInput ("at the conclusion of opt implosion");

   if (myOptComp ()->accAfterOptImp ())
      {
      accelerated_ =
         myMsgFac ().displayStateChange (
            "accStateMsg",
            accelerated (),
            true);

      myOptComp ()->accOptStarter ()->beChosen ();
      }
   else
      shutDown ();

   myPostprocessor ()->postprocess ();
   }

//------------------------------------------------------------------------------

void WitDetOptImpMgr::validateMipMode ()
   {
   if (myGlobalComp ()->computeCriticalList ())
      myMsgFac () ("invalidAttrForMipModeSmsg", "computeCriticalList");

   if (myOptComp ()->compPrices ())
      myMsgFac () ("invalidAttrForMipModeSmsg", "compPrices");

   if (myOptComp ()->accAfterOptImp ())
      myMsgFac () ("invalidAttrForMipModeSmsg", "accAfterOptImp");
   }

//------------------------------------------------------------------------------

void WitDetOptImpMgr::compInitSoln ()
   {
   myOptComp ()->optInitMethod ()->display ();

   if (myOptComp ()->heurOptStarter ()->isChosen ())
      {
      if (myOptComp ()->multiObjMode ())
         heurImpOnFirstObj ();
      else
         myProblem ()->myHeurImploder ()->implode (true);
      }
   else if (myOptComp ()->accOptStarter ()->isChosen ())
      {
      witAssert (accelerated ());
      }
   else if (myOptComp ()->schedOptStarter ()->isChosen ())
      {
      myPostprocessor ()->postprocess ();
      }
   }

//------------------------------------------------------------------------------

void WitDetOptImpMgr::heurImpOnFirstObj ()
   {
   WitObjective * savedObj;
   WitObjective * firstObj;

   savedObj = myOptComp ()->myMultiObjMgr ()->currentObjective ();
   firstObj = myOptComp ()->myMultiObjMgr ()->myObjectives     ().first ();

   myOptComp ()->myMultiObjMgr ()->setCurrentObjective (firstObj);

   myProblem ()->myHeurImploder ()->implode (true);

   myOptComp ()->myMultiObjMgr ()->setCurrentObjective (savedObj);
   }

//------------------------------------------------------------------------------

void WitDetOptImpMgr::setUpOptProblem ()
   {
   if (myDetImpOP_ == NULL)
      {
      myMsgFac () ("genLpMipProbMsg",
         myMsgFac ().myFrag (myOptComp ()->mipMode ()? "mipFrag": "lpFrag"));

      myDetImpOP_ = new WitDetImpOP (myProblem ());

      myDetImpOP ()->generateFixed ();
      }
   else
      myMsgFac () ("updateOptProblemMsg");

   myDetImpOP ()->generateFlexible ();
   }

//------------------------------------------------------------------------------

void WitDetOptImpMgr::compInitOptSoln ()
   {
   myDetImpOP ()->calcInitSoln ();

   if (devMode ())
      {
      compObjectives ();

      myMsgFac () ("initObjMsg");

      myOptComp ()->writeObjValue ();
      }

   if (myOptComp ()->printInitSoln ())
      myDetImpOP ()->writeSoln ("initSoln.out");
   }

//------------------------------------------------------------------------------

void WitDetOptImpMgr::compObjectives ()
   {
   if (myOptComp ()->multiObjMode ())
      {
      compMultiObj ();
      }
   else
      {
      compSglObj ();
      }
   }

//------------------------------------------------------------------------------

void WitDetOptImpMgr::compSglObj ()
   {
   double      objValue;
   double      boundsValue;
   WitOptVar * theOptVar;

   witAssert (not myOptComp ()->multiObjMode ());

   uploadingSoln_ = true;

   objValue       = myDetImpOP ()->compObjValue ();

   myOptComp ()->storeOptObjValue (objValue);

   if (myDetImpOP ()->slbvVarsExist ())
      {
      boundsValue = 0.0;

      forEachEl (theOptVar, myDetImpOP ()->myOptVars ())
         {
         if (theOptVar->mandDetVar ()->isSlbvVar ())
            boundsValue += theOptVar->primalValue ();
         }

      myOptComp ()->storeOptBoundsValue (boundsValue);
      }

   uploadingSoln_ = false;
   }

//------------------------------------------------------------------------------

void WitDetOptImpMgr::compMultiObj ()
   {
   WitObjective * theObj;
   double         objValue;
   double         boundsValue;

   witAssert (myOptComp ()->multiObjMode ());

   uploadingSoln_ = true;

   forEachEl (theObj, myOptComp ()->myMultiObjMgr ()->myObjectives ())
      {
      objValue = myDetImpOP ()->myMultiObjVar (theObj)->primalValue ();

      theObj->storeOptObjValue (objValue);
      }

   if (myDetImpOP ()->slbvVarsExist ())
      {
      boundsValue = - myDetImpOP ()->myNtbvVar ()->primalValue ();

      myOptComp ()->storeOptBoundsValue (boundsValue);
      }

   uploadingSoln_ = false;
   }

//------------------------------------------------------------------------------

void WitDetOptImpMgr::uploadSoln ()
   {
   WitOptVar *      theOptVar;
   WitPart *        thePart;
   WitPeriod        thePer;
   WitResourceCon * theResCon;

   uploadingSoln_ = true;

   forEachEl (theOptVar, myDetImpOP ()->myOptVars ())
      {
      if (not extOptActive_)
         if (theOptVar->primalValue () <= 0.0)
            continue;

      theOptVar->mandDetVar ()->uploadSoln ();
      }

   if (myOptComp ()->compPrices ())
      forEachPart (thePart, myProblem ())
         forEachPeriod (thePer, myProblem ())
            {
            theResCon = myDetImpOP ()->myResourceCon (thePart, thePer);

            if (theResCon != NULL)
               thePart->storeOptShadowPrice (thePer, theResCon->dualValue ());
            }

   myGlobalComp ()->receiveOptRevCritList (revCritList_);

   uploadingSoln_ = false;
   }

//------------------------------------------------------------------------------

double WitDetOptImpMgr::solnValue (const WitOptVar * theOptVar)
   {
   if (theOptVar == NULL)
      return 0.0;

   else if (extOptActive_)
      return theOptVar->primalValue ();

   else
      return positivePart (theOptVar->primalValue ());
   }

//------------------------------------------------------------------------------

void WitDetOptImpMgr::buildCritList ()
   {
   double                    criticalEpsilon;
   WitPart *                 thePart;
   WitTVec <bool>            producible (myProblem (), false);
   WitPeriod                 execPer;
   WitPeriod                 thePer;
   WitBopEntry *             theBopEnt;
   int                       theIdx;
   WitPartPer *              thePartPer;
   WitPtrVec    <WitPartPer> criticalVec;
   WitPtrVecItr <WitPartPer> theItr;

   stronglyAssert (revCritList_.isEmpty ());

   if (not myGlobalComp ()->computeCriticalList ())
      return;

   myMsgFac () ("formCriticalListMsg");

   criticalEpsilon = .001;

   potential_.allocate (myProblem (), 0.0);

   forEachPart (thePart, myProblem ())
      {
      producible = false;

      forEachEl (theBopEnt, thePart->producingBopEntries ())
         forEachPeriod (execPer, myProblem ())
            if (theBopEnt->inEffect (execPer))
               producible[theBopEnt->impactPeriod ()[execPer]] = true;

      forEachPeriod (thePer, myProblem ())
         {
         if (producible[thePer])
            continue;

         potential_ (thePart)[thePer] =
            myDetImpOP ()->myResourceCon (thePart, thePer)->dualValue ();

         if (potential_ (thePart)[thePer] <= criticalEpsilon)
            continue;

         revCritList_.push (thePart, thePer);
         }
      }

   criticalVec.resize (revCritList_.nElements ());

   theIdx = -1;

   while (revCritList_.pop (thePart, thePer))
      {
      theIdx ++;

      criticalVec[theIdx] = new WitPartPer (thePart, thePer);
      }

   criticalVec.sort (comparePotential);

   criticalVec.attachItr (theItr);

   while (theItr.advance (thePartPer))
      {
      revCritList_.push (thePartPer->first, thePartPer->second);

      delete thePartPer;
      }

   criticalVec.clear ();
   potential_ .clear ();
   }

//------------------------------------------------------------------------------

bool WitDetOptImpMgr::comparePotential (
      WitPartPer * thePartPer1,
      WitPartPer * thePartPer2)
   {
   WitPart *         thePart1;
   WitPart *         thePart2;
   WitPeriod         thePer1;
   WitPeriod         thePer2;
   WitDetOptImpMgr * theDetOptImpMgr;
   double            thePot1;
   double            thePot2;

   thePart1        = thePartPer1->first;
   thePart2        = thePartPer2->first;

   thePer1         = thePartPer1->second;
   thePer2         = thePartPer2->second;

   theDetOptImpMgr = thePart1->myProblem ()->myDetOptImpMgr ();

   thePot1         = theDetOptImpMgr->potential_ (thePart1)[thePer1];
   thePot2         = theDetOptImpMgr->potential_ (thePart2)[thePer2];

   return (thePot1 > thePot2);
   }
