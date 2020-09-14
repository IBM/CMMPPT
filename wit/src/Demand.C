//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "Demand.C"
//
// Contains the implementation of class Demand.
//------------------------------------------------------------------------------

#include <Demand.h>
#include <CShipBS.h>
#include <OptComp.h>
#include <Timing.h>
#include <DataWrit.h>
#include <Post.h>
#include <FSS.h>
#include <OptStarter.h>
#include <PegEl.h>
#include <HeurImp.h>
#include <HeurAllMgr.h>
#include <HeurModifier.h>
#include <DetOptImpMgr.h>
#include <StochImpMgr.h>
#include <GlobalComp.h>
#include <Part.h>
#include <ObjStack.h>
#include <wit.h>
#include <IVRealArg.h>
#include <MsgFrag.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class Demand.
//------------------------------------------------------------------------------

WitDemand::WitDemand (const char * theName, WitPart * thePart):

      WitDelComp       (thePart->myCompMgr ()),
      mappingIndex_    (myCompMgr ()->nDemands ()),
      demandName_      (),
      demandedPart_    (thePart),
      demandVol_       (myProblem (), defDemandVol ()),
      priority_        (myProblem (), defPriority ()),
      searchInc_       (defSearchInc ()),
      buildAheadUB_    (myProblem (), defBuildAheadUB ()),
      leadTimeUB_      (myProblem (), defLeadTimeUB (myProblem ())),
      shipLateUB_      (myProblem (), defShipLateUB (myProblem ())),
      shipLateAllowed_ (defShipLateAllowed ()),
      prefBuildAhead_  (defPrefBuildAhead ()),
      intShipVols_     (defIntShipVols ()),
      cumShipBounds_   (NULL),
      shipReward_      (myProblem (), defShipReward ()),
      cumShipReward_   (myProblem (), defCumShipReward ()),
      shipVol_         (myProblem (), 0.0),
      focusHorizon_    (defFocusHorizon ()),
      fssShipVol_      (myProblem (), 0.0),
      cumDemandVol_    (myProblem (), 0.0),
      cumShipVol_      (myProblem (), 0.0),
      myPegElsFor_     ()
   {
   setDemandName (theName);

   cumShipBounds_ = new WitCumShipBoundSet (this);

   if (myGlobalComp ()->perfPegging ())
      buildPegEls ();

   myCompMgr    ()->insert (this);
   demandedPart ()->insert (this);
   }

//------------------------------------------------------------------------------

WitDemand::~WitDemand ()
   {
   deleteContents (myPegElsFor_);

   delete cumShipBounds_;
   }

//------------------------------------------------------------------------------

WitPeriod WitDemand::defLeadTimeUB (WitProblem * theProblem)
   {
   return theProblem->lastPeriod ();
   }

//------------------------------------------------------------------------------

WitPeriod WitDemand::defShipLateUB (WitProblem * theProblem)
   {
   return theProblem->lastPeriod ();
   }

//------------------------------------------------------------------------------
// "Set" functions.
//------------------------------------------------------------------------------

void WitDemand::setDemandName (const char * theName)
   {
   WitDemand * priorDemand;

   witAssert (theName != NULL);

   witAssert (not isNullCstring (theName));

   prepSetNonUnpostAttr ();

   priorDemand = myCompMgr ()->findDemand (demandedPart_, theName);

   witAssert (priorDemand == NULL);

   demandName_ = theName;
   }

//------------------------------------------------------------------------------

void WitDemand::setDemandVol (const WitIVRealArg & theValue)
   {
   witAssert (theValue != NULL);

   witAssert (theValue.isNonNeg (nPeriods ()));

   prepSetScenSpecAttr ("demandVol");

   demandVol_ = theValue;

   compCumDemandVol ();
   }

//------------------------------------------------------------------------------

void WitDemand::setPriority (const int * theValue)
   {
   witAssert (theValue != NULL);

   prepSetUnpostAttr ();

   priority_ = theValue;
   }

//------------------------------------------------------------------------------

void WitDemand::setSearchInc (double theValue)
   {
   witAssert (theValue >= 0.000999);

   prepSetUnpostAttr ();

   searchInc_ = theValue;
   }

//------------------------------------------------------------------------------

void WitDemand::setBuildAheadUB (const int * theValue)
   {
   WitPeriod thePer;

   witAssert (theValue != NULL);

   if (DEBUG_MODE)
      forEachPeriod (thePer, myProblem ())
         {
         stronglyAssert    (theValue[thePer] >= 0);
         stronglyAssert    (theValue[thePer] <  nPeriods ());

         if (thePer <= nPeriods () - 2)
            stronglyAssert (theValue[thePer + 1] <= theValue[thePer] + 1);
         }

   prepSetUnpostAttr ();

   buildAheadUB_ = theValue;
   }

//------------------------------------------------------------------------------

void WitDemand::setLeadTimeUB (const int * theValue)
   {
   WitPeriod thePer;

   witAssert (theValue != NULL);

   if (DEBUG_MODE)
      forEachPeriod (thePer, myProblem ())
         {
         stronglyAssert (theValue[thePer] >= 0);
         stronglyAssert (theValue[thePer] <  nPeriods ());

         if (thePer <= nPeriods () - 2)
            stronglyAssert (theValue[thePer + 1] <= theValue[thePer] + 1);
         }

   prepSetUnpostAttr ();

   leadTimeUB_ = theValue;
   }

//------------------------------------------------------------------------------

void WitDemand::setShipLateUB (const int * theValue)
   {
   WitPeriod thePer;

   witAssert (theValue != NULL);

   if (DEBUG_MODE)
      forEachPeriod (thePer, myProblem ())
         {
         stronglyAssert (theValue[thePer] >= 0);
         stronglyAssert (theValue[thePer] <  nPeriods ());

         if (thePer <= nPeriods () - 2)
            stronglyAssert (theValue[thePer + 1] >= theValue[thePer] - 1);
         }

   prepSetUnpostAttr ();

   shipLateUB_ = theValue;
   }

//------------------------------------------------------------------------------

void WitDemand::setShipLateAllowed (bool theValue)
   {
   prepSetUnpreAttr ();

   shipLateAllowed_ = theValue;
   }

//------------------------------------------------------------------------------

void WitDemand::setPrefBuildAhead (bool theValue)
   {
   prepSetUnpostAttr ();

   prefBuildAhead_ = theValue;
   }

//------------------------------------------------------------------------------

void WitDemand::setIntShipVols (bool theValue)
   {
   prepSetUnpreAttr ();

   intShipVols_ = theValue;
   }

//------------------------------------------------------------------------------

void WitDemand::setShipReward (const WitIVRealArg & theValue)
   {
   witAssert (theValue != NULL);

   prepSetScenSpecAttr ("shipReward");

   shipReward_.curDblFlexVec () = theValue;
   }

//------------------------------------------------------------------------------

void WitDemand::setCumShipReward (const WitIVRealArg & theValue)
   {
   witAssert (theValue != NULL);

   prepSetScenSpecAttr ("cumShipReward");

   cumShipReward_.curDblFlexVec () = theValue;
   }

//------------------------------------------------------------------------------

void WitDemand::setShipVol (const WitIVRealArg & theValue)
   {
   witAssert (theValue != NULL);

   witAssert (theValue.isNonNeg (nPeriods ()));

   if (myProblem ()->myHeurAllMgr ()->modifiable ())
      {
      prepSetNonUnpostAttr ();

      myProblem ()->myHeurModifier ()->modifyForShipVol (this, theValue);
      }
   else
      {
      myDetOptImpMgr ()->shutDown ();

      myOptComp ()->schedOptStarter ()->beChosen ();

      prepSetUnpostAttr ();
      }

   shipVol_ = theValue;
   }

//------------------------------------------------------------------------------

void WitDemand::setFocusHorizon (int theValue)
   {
   witAssert (theValue >= -1);
   witAssert (theValue <= lastPeriod ());

   prepSetNonUnpostAttr ();

   myProblem ()->myFSS ()->uncompute ();

   focusHorizon_ = theValue;
   }

//------------------------------------------------------------------------------

void WitDemand::setFssShipVol (const WitIVRealArg & theValue)
   {
   witAssert (theValue != NULL);

   witAssert (theValue.isNonNeg (nPeriods ()));

   prepSetNonUnpostAttr ();

   myProblem ()->myFSS ()->uncompute ();

   fssShipVol_ = theValue;
   }

//------------------------------------------------------------------------------
// Other functions.
//------------------------------------------------------------------------------

void WitDemand::storePostCumShipVol (const WitTVec <double> & theTVec)
   {
   witAssert (myPostprocessor ()->postprocessing ());

   cumShipVol_ = theTVec;
   }

//------------------------------------------------------------------------------

void WitDemand::storeOptShipVol (WitPeriod thePer, double theValue)
   {
   witAssert (myDetOptImpMgr ()->uploadingSoln ());

   shipVol_.elemRef (thePer) = theValue;
   }

//------------------------------------------------------------------------------

void WitDemand::storeOptCumShipVol (WitPeriod thePer, double theValue)
   {
   witAssert (myDetOptImpMgr ()->uploadingSoln ());

   cumShipVol_.elemRef (thePer) = theValue;
   }

//------------------------------------------------------------------------------

void WitDemand::storeStochDemandVol (const WitDblFlexVec & theFlexVec)
   {
   witAssert (myStochImpMgr ()->uploadingData ());

   demandVol_ = theFlexVec;

   compCumDemandVol ();
   }

//------------------------------------------------------------------------------

void WitDemand::storeStochShipReward (const WitDblFlexVec & theFlexVec)
   {
   witAssert (myStochImpMgr ()->uploadingData ());

   shipReward_.curDblFlexVec () = theFlexVec;
   }

//------------------------------------------------------------------------------

void WitDemand::storeStochCumShipReward (const WitDblFlexVec & theFlexVec)
   {
   witAssert (myStochImpMgr ()->uploadingData ());

   cumShipReward_.curDblFlexVec () = theFlexVec;
   }

//------------------------------------------------------------------------------

void WitDemand::storeStochShipVol (const WitDblFlexVec & theFlexVec)
   {
   witAssert (myStochImpMgr ()->uploadingData ());

   shipVol_ = theFlexVec;
   }

//------------------------------------------------------------------------------

void WitDemand::storeStochCumShipVol (const WitDblFlexVec & theFlexVec)
   {
   witAssert (myStochImpMgr ()->uploadingData ());

   cumShipVol_ = theFlexVec;
   }

//------------------------------------------------------------------------------

bool WitDemand::inserted ()
   {
   return (demandedPart ()->myDemands ().contains (this));
   }

//------------------------------------------------------------------------------

void WitDemand::writeMultiObjVecs (WitObjective * theObj)
   {
   shipReward_   .writeDataMulti ("shipReward",    this, theObj);
   cumShipReward_.writeDataMulti ("cumShipReward", this, theObj);
   }

//------------------------------------------------------------------------------

void WitDemand::identifyInstance ()
   {
   if (myMsgFac ().mayIssue ("demandInstIDMsg"))
      myMsgFac () ("demandInstIDMsg", demandedPartName (), demandName ());
   }

//------------------------------------------------------------------------------

void WitDemand::writeSetClause ()
   {
   myDataWriter ()->writeFirstKeyWord ("set");
   myDataWriter ()->writeKeyWord      ("demand");
   myDataWriter ()->writeStringValue  (demandedPartName ());
   myDataWriter ()->writeStringValue  (demandName ());
   }

//------------------------------------------------------------------------------

void WitDemand::copySelfInto (WitCompMgr * theCompMgr)
   {
   WitPart *   thePart;
   WitDemand * theDemand;

   thePart   = theCompMgr->myMatchFor (demandedPart ());

   theDemand = new WitDemand (demandName ().myCstring (), thePart);

   theDemand->copyAttrsFrom (this);
   }

//------------------------------------------------------------------------------

void WitDemand::recount ()
   {
   mappingIndex_ = myCompMgr ()->nDemands ();

   myCompMgr ()->recountDemand (this);
   }

//------------------------------------------------------------------------------

void WitDemand::clearOptSoln  ()
   {                              
   witAssert (myDetOptImpMgr ()->clearingSoln ());

   shipVol_    = 0.0;
   cumShipVol_ = 0.0;
   }

//------------------------------------------------------------------------------

void WitDemand::clearStochSoln ()
   {
   WitComponent::clearStochSoln ();

   shipVol_ = 0.0;
   }

//------------------------------------------------------------------------------

WitDemand * WitDemand::thisDemand ()
   {
   return this;
   }

//------------------------------------------------------------------------------

WitMsgFrag WitDemand::objTypeMsgFrag ()
   {
   return myMsgFac ().myFrag ("demandFrag");
   }

//------------------------------------------------------------------------------

WitAttr WitDemand::apiObjItrState ()
   {
   return WitAT_DEMAND;
   }

//------------------------------------------------------------------------------

void WitDemand::copyAttrsFrom (WitDemand * theDemand)
   {
   WitDelComp::copyAttrsFrom (theDemand);

   demandVol_       = theDemand->demandVol_;
   priority_        = theDemand->priority_;
   searchInc_       = theDemand->searchInc_;
   buildAheadUB_    = theDemand->buildAheadUB_;
   leadTimeUB_      = theDemand->leadTimeUB_;
   shipLateUB_      = theDemand->shipLateUB_;
   shipLateAllowed_ = theDemand->shipLateAllowed_;
   prefBuildAhead_  = theDemand->prefBuildAhead_;
   intShipVols_     = theDemand->intShipVols_;
   shipReward_      = theDemand->shipReward_;
   cumShipReward_   = theDemand->cumShipReward_;
   focusHorizon_    = theDemand->focusHorizon_;
   cumDemandVol_    = theDemand->cumDemandVol_;
   cumShipBounds_->
      copyAttrsFrom  (theDemand->cumShipBounds_);
   }

//------------------------------------------------------------------------------

const WitString & WitDemand::demandName (const WitDemand * theDemand)
   {
   return theDemand->demandName ();
   }

//------------------------------------------------------------------------------

void WitDemand::setUpPegging ()
   {
   witAssert (myGlobalComp ()->perfPegging ());

   witAssert (not myPegElsFor_.isAllocated ());

   buildPegEls ();
   }

//------------------------------------------------------------------------------

void WitDemand::shutDownPegging ()
   {
   witAssert (not myGlobalComp ()->perfPegging ());

   witAssert (myPegElsFor_.isAllocated ());

   deleteContents (myPegElsFor_);
   }

//------------------------------------------------------------------------------

void WitDemand::extractPegEls (
      WitPeriod                shipPer,
      WitObjStack <WitPegEl> & thePegElStack)
   {
   witAssert (myHeurAllMgr ()->updatingSoln ());

   witAssert (thePegElStack.isEmpty ());

   thePegElStack.takeContentsFrom (myPegElsFor_ (shipPer));
   }

//------------------------------------------------------------------------------

void WitDemand::receivePegEls (
      WitPeriod                shipPer,
      WitObjStack <WitPegEl> & thePegElStack)
   {
   witAssert (myHeurAllMgr ()->updatingSoln ());

   witAssert (myPegElsFor_ (shipPer).isEmpty ());

   myPegElsFor_ (shipPer).takeContentsFrom (thePegElStack);
   }

//------------------------------------------------------------------------------

void WitDemand::getMyFixedPegEls (
      WitPeriod                     shipPer,
      WitObjStack <WitFixedPegEl> & theFixedPegEls)
   {
   WitObjStItr <WitPegEl> theItr;
   WitPegEl *             thePegEl;

   theFixedPegEls.clear ();

   myPegElsFor_ (shipPer).attachItr (theItr);

   while (theItr.advance (thePegEl))
      theFixedPegEls.push (thePegEl->myFixedPegEl ()); 
  }

//------------------------------------------------------------------------------

void WitDemand::clearPegging (WitPeriod shipPer)
   {
   deleteContents (myPegElsFor_ (shipPer));
   }

//------------------------------------------------------------------------------

double WitDemand::netRev () const
   {
   return 0.0;
   }

//------------------------------------------------------------------------------

void WitDemand::writeSolnData ()
   {
   if (shipVol () == defShipVol ())
      return;

   myDataWriter ()->writeFirstKeyWord ("set");
   myDataWriter ()->writeKeyWord      ("demand");
   myDataWriter ()->writeStringValue  (demandedPartName ());
   myDataWriter ()->writeStringValue  (demandName ());

   myDataWriter ()->writeVector       ("shipVol", shipVol (), defShipVol ());

   myDataWriter ()->writeEndCommand ();
   }

//------------------------------------------------------------------------------

void WitDemand::display ()
   {
   myMsgFac () ("demandIdDdMsg",
      demandedPartName (),
      demandName_);

   myMsgFac () ("demandScalarsDdMsg",
      mappingIndex_,
      searchInc_,
      shipLateAllowed_,
      prefBuildAhead_,
      intShipVols_);

   myMsgFac ()           ("focusHorizonDdMsg",     focusHorizon_);

   myProblem ()->display ("demVolDdMsg",           demandVol_);
   myProblem ()->display ("demPriDdMsg",           priority_);
   myProblem ()->display ("buildAheadUBDdMsg",     buildAheadUB_);
   myProblem ()->display ("leadTimeUBDdMsg",       leadTimeUB_);
   myProblem ()->display ("shipLateUBDdMsg",       shipLateUB_);
   myProblem ()->display ("demShipRewardDdMsg",    shipReward_);
   myProblem ()->display ("demCumShipRewardDdMsg", cumShipReward_);

   cumShipBounds_->display ();

   WitDelComp::display ();
   }

//------------------------------------------------------------------------------

void WitDemand::genPriorities (WitProblem * theProblem)
   {
   WitDemand * theDemand;

   WitTimer::enterSection ("preproc");

   WitSchedule <WitDemand, int> newPriority (theProblem, 0);

   theProblem->prepSetUnpostAttr ();

   theProblem->myHeurImploder ()->genPriorities (newPriority);

   forEachDemand (theDemand, theProblem)
      theDemand->priority_ = newPriority (theDemand);

   WitTimer::leaveSection ("preproc");
   }

//------------------------------------------------------------------------------

void WitDemand::clearShipVolForHeur ()
   {
   witAssert (myHeurAllMgr ()->updatingSoln ());

   shipVol_ = 0.0;
   }

//------------------------------------------------------------------------------

void WitDemand::incShipVolForHeur (WitPeriod thePer, double incVol)
   {
   witAssert (myHeurAllMgr ()->updatingSoln ());

   shipVol_.elemRef (thePer) += incVol;
   }

//------------------------------------------------------------------------------

void WitDemand::initFssShipVolForPost ()
   {
   witAssert (myPostprocessor ()->postprocessing ());

   fssShipVol_ = shipVol_;
   }

//------------------------------------------------------------------------------

void WitDemand::compFssShipVol ()
   {
   myFSS ()->compFssShipVol (this, fssShipVol_);
   }

//------------------------------------------------------------------------------

void WitDemand::writeSoln ()
   {
   WitPeriod thePer;

   fprintf (solnFile (),
      "\nPart %s,  Demand %s:\n",
      demandedPartName ().myCstring (),
      demandName ().myCstring ());

   fprintf (solnFile (), "Period");

   fprintf (solnFile (),
       "    DemandVol"
       " CumDemandVol"
       "      ShipVol"
       "   CumShipVol");

   fprintf (solnFile (), "\n");

   forEachPeriod (thePer, myProblem ())
      {
      fprintf (solnFile (), "%6d", thePer);

      writeSolnValue (demandVol ()[thePer]);
      writeSolnValue (cumDemandVol ()[thePer]);
      writeSolnValue (shipVol ()[thePer]);
      writeSolnValue (cumShipVol ()[thePer]);

      fprintf (solnFile (), "\n");
      }
   }

//------------------------------------------------------------------------------

const WitString & WitDemand::demandedPartName ()
   {
   return demandedPart ()->partName ();
   }

//------------------------------------------------------------------------------

int WitDemand::nInstances (WitProblem * theProblem)
   {
   return theProblem->myCompMgr ()->nDemands ();
   }

//------------------------------------------------------------------------------

void WitDemand::compCumDemandVol ()
   {
   double    cumSum;
   WitPeriod thePer;

   if (demandVol_ == 0.0)
      cumDemandVol_ = 0.0;
   else
      {
      cumSum = 0.0;

      forEachPeriod (thePer, myProblem ())
         {
         cumSum += demandVol_[thePer];

         cumDemandVol_.elemRef (thePer) = cumSum;
         }
      }
   }

//------------------------------------------------------------------------------

WitBoundSet * WitDemand::myBoundSet ()
   {
   return cumShipBounds_;
   }

//------------------------------------------------------------------------------

void WitDemand::writeDataID ()
   {
   myDataWriter ()->writeFirstKeyWord ("add");
   myDataWriter ()->writeKeyWord      ("demand");
   myDataWriter ()->writeStringValue  (demandedPartName ());
   myDataWriter ()->writeStringValue  (demandName ());
   }

//------------------------------------------------------------------------------

void WitDemand::writeDataAttrs ()
   {
   WitDelComp::writeDataAttrs ();

   myDataWriter ()->writeVector (
      "demandVol",
      demandVol (),
      defDemandVol ());

   myDataWriter ()->writeVector (
      "priority",
      priority (),
      defPriority ());

   myDataWriter ()->writeDouble (
      "searchInc",
      searchInc (),
      defSearchInc ());

   myDataWriter ()->writeVector (
        "buildAheadUB",
         buildAheadUB (),
      defBuildAheadUB ());

   myDataWriter ()->writeVector (
        "leadTimeUB",
         leadTimeUB (),
      defLeadTimeUB (myProblem ()));

   myDataWriter ()->writeVector (
        "shipLateUB",
         shipLateUB (),
      defShipLateUB (myProblem ()));

   myDataWriter ()->writeBool (
        "shipLateAllowed",
         shipLateAllowed (),
      defShipLateAllowed ());

   myDataWriter ()->writeBool (
        "prefBuildAhead",
         prefBuildAhead (),
      defPrefBuildAhead ());

   myDataWriter ()->writeBool (
        "intShipVols",
         intShipVols (),
      defIntShipVols ());

   cumShipBounds_->writeData ();

   myDataWriter ()->writeObjVec (
        "shipReward",
         shipReward_,
      defShipReward ());

   myDataWriter ()->writeObjVec (
        "cumShipReward",
         cumShipReward_,
      defCumShipReward ());

   myDataWriter ()->writeInt (
      "focusHorizon",
      focusHorizon (),
      defFocusHorizon ());
   }

//------------------------------------------------------------------------------

void WitDemand::findPrereqs (WitDelComp * & prereq1, WitDelComp * & prereq2)
   {
   prereq1 = demandedPart ();
   prereq2 = NULL;
   }

//------------------------------------------------------------------------------

void WitDemand::buildPegEls ()
   {
   WitPeriod shipPer;

   witAssert (myGlobalComp ()->perfPegging ());

   myPegElsFor_.allocate (myProblem ());

   forEachPeriod (shipPer, myProblem ())
      myPegElsFor_[shipPer] = new WitObjStack <WitPegEl> (myProblem ());
   }
