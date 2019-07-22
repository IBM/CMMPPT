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
// Source file: "BoundSet.C"
//
// Contains the implementation of the following classes:
//
//    BoundSet
//    StockBoundSet
//    CumShipBoundSet
//    ExecBoundSet
//------------------------------------------------------------------------------

#include <StockBS.h>
#include <CShipBS.h>
#include <ExecBS.h>
#include <Material.h>
#include <Demand.h>
#include <Operation.h>
#include <OptComp.h>
#include <DetOptImpMgr.h>
#include <StochImpMgr.h>
#include <DataWrit.h>
#include <Pre.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class BoundSet
//------------------------------------------------------------------------------

WitBoundSet::WitBoundSet (WitProblem * theProblem):
      WitProbAssoc  (theProblem),

      mappingIndex_ (myCompMgr ()->nBoundSets ()),
      hardLB_       (myProblem (), defHardLB ()),
      softLB_       (myProblem (), defSoftLB ()),
      hardUB_       (myProblem (), defHardUB ())
   {
   }

//------------------------------------------------------------------------------

WitBoundSet::~WitBoundSet ()
   {
   }

//------------------------------------------------------------------------------

int WitBoundSet::nInstances (WitProblem * theProblem)
   {
   return theProblem->myCompMgr ()->nBoundSets ();
   }

//------------------------------------------------------------------------------

bool WitBoundSet::isAtDefault ()
   {
   if (hardLB_ == defHardLB ())
      if (softLB_ == defSoftLB ())
         if (hardUB_ == defHardUB ())
            return true;

   return false;
   }

//------------------------------------------------------------------------------

bool WitBoundSet::hardUBIsFinite (WitPeriod thePer)
   {
   return (hardUB_[thePer] >= 0.0);
   }

//------------------------------------------------------------------------------

bool WitBoundSet::hardUBisSatisfied (WitPeriod thePer, double theValue) 
   {
   return
      hardUBIsFinite (thePer)?
         (theValue <= hardUB_[thePer]):
         true;
   }

//------------------------------------------------------------------------------

void WitBoundSet::set (
      const WitDblFlexVec & hardLBArg,
      const WitDblFlexVec & softLBArg,
      const WitDblFlexVec & hardUBArg)
   {
   WitPeriod thePer;

   if (DEBUG_MODE)
      forEachPeriod (thePer, myProblem ())
         witAssert (not seqError (hardLBArg, softLBArg, hardUBArg, thePer));

   if (myStochImpMgr ()->stochMode ())
      myStochImpMgr ()->prepSetBoundSet (this);

   myProblem ()->resetSoln ();

   if (unaccNeeded (hardLBArg, softLBArg))
      myDetOptImpMgr ()->shutDown ();

   hardLB_ = hardLBArg;
   softLB_ = softLBArg;
   hardUB_ = hardUBArg;
   }

//------------------------------------------------------------------------------

void WitBoundSet::storeStochHardLB (const WitDblFlexVec & theValue)
   {
   witAssert (myStochImpMgr ()->uploadingData ());

   hardLB_ = theValue;
   }

//------------------------------------------------------------------------------

void WitBoundSet::storeStochSoftLB (const WitDblFlexVec & theValue)
   {
   witAssert (myStochImpMgr ()->uploadingData ());

   softLB_ = theValue;
   }

//------------------------------------------------------------------------------

void WitBoundSet::storeStochHardUB (const WitDblFlexVec & theValue)
   {
   witAssert (myStochImpMgr ()->uploadingData ());

   hardUB_ = theValue;
   }

//------------------------------------------------------------------------------

void WitBoundSet::copyAttrsFrom (WitBoundSet * theBoundSet)
   {
   hardLB_ = theBoundSet->hardLB_;
   softLB_ = theBoundSet->softLB_;
   hardUB_ = theBoundSet->hardUB_;
   }

//------------------------------------------------------------------------------

bool WitBoundSet::seqError (
      const WitDblFlexVec & hardLBArg,
      const WitDblFlexVec & softLBArg,
      const WitDblFlexVec & hardUBArg,
      WitPeriod             thePer)
   {
   if (hardLBArg[thePer] < 0.0)
      return true;

   if (hardLBArg[thePer] > softLBArg[thePer])
      return true;

   if (hardUBArg[thePer] >= 0.0)
      if (softLBArg[thePer] > hardUBArg[thePer])
         return true;

   return false;
   }

//------------------------------------------------------------------------------

bool WitBoundSet::hasPositiveHardLB ()
   {
   WitPeriod thePer;

   forEachPeriod (thePer, myProblem ())
      if (hardLB_[thePer] > 0.0)
         return true;

   return false;
   }

//------------------------------------------------------------------------------

void WitBoundSet::writeData ()
   {
   if (isAtDefault ())
      return;

   if (myStochImpMgr ()->stochMode ())
      return;

   myDataWriter ()->writeFirstKeyWord (myAttName ());

   myDataWriter ()->writeVector ("hardLB", hardLB_, defHardLB ());
   myDataWriter ()->writeVector ("softLB", softLB_, defSoftLB ());
   myDataWriter ()->writeVector ("hardUB", hardUB_, defHardUB ());

   myDataWriter ()->writeFirstKeyWord ("endBounds");
   }

//------------------------------------------------------------------------------

void WitBoundSet::writeSetClause ()
   {
   myDelComp ()->writeSetClause ();

   myDataWriter ()->writeFirstKeyWord (myAttName ());
   }

//------------------------------------------------------------------------------

void WitBoundSet::display ()
   {
   if (isAtDefault ())
      return;

   myMsgFac () (headingMsgID ());

   myMsgFac () ("boundHeadingDdMsg", mappingIndex_);

   myProblem ()->displayBoundVec ("hardLBFrag", hardLB_);
   myProblem ()->displayBoundVec ("softLBFrag", softLB_);
   myProblem ()->displayBoundVec ("hardUBFrag", hardUB_);
   }

//------------------------------------------------------------------------------

void WitBoundSet::writeSoln ()
   {
   if (isAtDefault ())
      return;

   WitPeriod thePer;

   fprintf (solnFile (),
      "\n\nSolution for bounds on the %s of ", boundedVectorName ());

   idObjectForSoln ();

   fprintf (solnFile (),
      "\n%6s  %11s  %11s  %11s  %11s  %11s  %11s  %11s\n",
      "Period", boundedVectorName (),
      "Hard LB", "Soft LB", "Hard UB",
      "Hard LB", "Soft LB", "Hard UB");

   fprintf (solnFile (),
      "%6s  %11s  %11s  %11s  %11s  %11s  %11s  %11s\n",
      "", "", "", "", "", "Violation", "Violation", "Violation");

   forEachPeriod (thePer, myProblem ())
      {
      fprintf (solnFile (), "%6d", thePer);

      writeSolnValue (boundedVal (thePer));

      writeSolnValue (hardLB_[thePer]);
      writeSolnValue (softLB_[thePer]);

      if (hardUBIsFinite (thePer))
         writeSolnValue (hardUB_[thePer]);
      else
         fprintf (solnFile (), "    +Infinity");

      writeSolnValue (positivePart (hardLB_[thePer] - boundedVal (thePer)));

      writeSolnValue (positivePart (softLB_[thePer] - boundedVal (thePer)));

      writeSolnValue (
         hardUBIsFinite (thePer)?
            positivePart (boundedVal (thePer) - hardUB_[thePer]):
            0.0);

      fprintf (solnFile (), "\n");
      }
   }

//------------------------------------------------------------------------------

void WitBoundSet::reindex ()
   {
   witAssert (myCompMgr ()->isReindexing (this));

   mappingIndex_ = myCompMgr ()->nBoundSets ();
   }

//------------------------------------------------------------------------------

bool WitBoundSet::unaccNeeded (
      const WitDblFlexVec & hardLBArg,
      const WitDblFlexVec & softLBArg)
   {
   WitPeriod thePer;

   if (not myDetOptImpMgr ()->accelerated ())
      return false;

   if (myOptComp ()->accAfterSoftLB ())
      return false;

   forEachPeriod (thePer, myProblem ())
      if (softLB_[thePer] == hardLB_[thePer])
         if (softLBArg[thePer] > hardLBArg[thePer])
            return true;

   return false;
   }

//------------------------------------------------------------------------------
// Implementation of class StockBoundSet.
//------------------------------------------------------------------------------

WitStockBoundSet::WitStockBoundSet (WitMaterial * theMat):
      WitBoundSet (theMat->myProblem ()),
      myMaterial_ (theMat)
   {
   }

//------------------------------------------------------------------------------

WitStockBoundSet::~WitStockBoundSet ()
   {
   }

//------------------------------------------------------------------------------

double WitStockBoundSet::boundedVal (WitPeriod thePer)
   {
   return myMaterial_->stockVol ()[thePer];
   }

//------------------------------------------------------------------------------

const char * WitStockBoundSet::myAttName ()
   {
   return "stockBounds";
   }

//------------------------------------------------------------------------------

WitDelComp * WitStockBoundSet::myDelComp ()
   {
   return myMaterial_;
   }

//------------------------------------------------------------------------------

WitBoundSet * WitStockBoundSet::myMatchIn (WitCompMgr * theCompMgr)
   {
   WitPart *     thePart;
   WitMaterial * theMat;

   thePart = theCompMgr->myMatchFor (myMaterial_);

   theMat  = thePart   ->thisMat  ();

   witAssert (theMat != NULL);

   return theMat->stockBounds ();
   }

//------------------------------------------------------------------------------

WitNode * WitStockBoundSet::stageNode ()
   {
   return myMaterial_;
   }

//------------------------------------------------------------------------------

void WitStockBoundSet::issueStage0MMCompMsg ()
   {
   myMsgFac () ("stage0AttMisMatchPartSmsg", myMaterial_->partName ());
   }

//------------------------------------------------------------------------------

WitMsgID WitStockBoundSet::headingMsgID ()
   {
   return "stockBoundsHeadingDdMsg";
   }

//------------------------------------------------------------------------------

const char * WitStockBoundSet::boundedVectorName ()
   {
   return "StockVol";
   }

//------------------------------------------------------------------------------

void WitStockBoundSet::idObjectForSoln ()
   {
   fprintf (myProblem ()->solnFile (),
      "material part \"%s\":\n",
      myMaterial_->partName ().myCstring ());
   }

//------------------------------------------------------------------------------
// Implementation of class CumShipBoundSet.
//------------------------------------------------------------------------------

WitCumShipBoundSet::WitCumShipBoundSet (WitDemand * theDemand):

      WitBoundSet (theDemand->myProblem ()),
      myDemand_   (theDemand)
   {
   }

//------------------------------------------------------------------------------

WitCumShipBoundSet::~WitCumShipBoundSet ()
   {
   }

//------------------------------------------------------------------------------

double WitCumShipBoundSet::boundedVal (WitPeriod thePer)
   {
   return myDemand_->cumShipVol ()[thePer];
   }

//------------------------------------------------------------------------------

const char * WitCumShipBoundSet::myAttName ()
   {
   return "cumShipBounds";
   }

//------------------------------------------------------------------------------

WitDelComp * WitCumShipBoundSet::myDelComp ()
   {
   return myDemand_;
   }

//------------------------------------------------------------------------------

WitBoundSet * WitCumShipBoundSet::myMatchIn (WitCompMgr * theCompMgr)
   {
   WitDemand * theDemand;

   theDemand = theCompMgr->myMatchFor (myDemand_);

   return theDemand->cumShipBounds ();
   }

//------------------------------------------------------------------------------

WitNode * WitCumShipBoundSet::stageNode ()
   {
   return myDemand_->demandedPart ();
   }

//------------------------------------------------------------------------------

void WitCumShipBoundSet::issueStage0MMCompMsg ()
   {
   myMsgFac () ("stage0AttMisMatchDemandSmsg",
      myDemand_->demandedPartName (),
      myDemand_->demandName       ());
   }

//------------------------------------------------------------------------------

WitMsgID WitCumShipBoundSet::headingMsgID ()
   {
   return "cumShipBoundsHeadingDdMsg";
   }

//------------------------------------------------------------------------------

const char * WitCumShipBoundSet::boundedVectorName ()
   {
   return "CumShipVol";
   }

//------------------------------------------------------------------------------

void WitCumShipBoundSet::idObjectForSoln ()
   {
   fprintf (myProblem ()->solnFile (),
      "demand \"%s\" for part \"%s\":\n",
      myDemand_->demandName ().myCstring (),
      myDemand_->demandedPartName ().myCstring ());
   }

//------------------------------------------------------------------------------
// Implementation of class ExecBoundSet.
//------------------------------------------------------------------------------

WitExecBoundSet::WitExecBoundSet (WitOperation * theOpn):
      WitBoundSet  (theOpn->myProblem ()),

      myOperation_ (theOpn)
   {
   }

//------------------------------------------------------------------------------

WitExecBoundSet::~WitExecBoundSet ()
   {
   }

//------------------------------------------------------------------------------

double WitExecBoundSet::boundedVal (WitPeriod thePer)
   {
   return myOperation_->execVol ()[thePer];
   }

//------------------------------------------------------------------------------

const char * WitExecBoundSet::myAttName ()
   {
   return "execBounds";
   }

//------------------------------------------------------------------------------

WitDelComp * WitExecBoundSet::myDelComp ()
   {
   return myOperation_;
   }

//------------------------------------------------------------------------------

WitBoundSet * WitExecBoundSet::myMatchIn (WitCompMgr * theCompMgr)
   {
   WitOperation * theOpn;

   theOpn = theCompMgr->myMatchFor (myOperation_);

   return theOpn->execBounds ();
   }

//------------------------------------------------------------------------------

WitNode * WitExecBoundSet::stageNode ()
   {
   return myOperation_;
   }

//------------------------------------------------------------------------------

void WitExecBoundSet::issueStage0MMCompMsg ()
   {
   myMsgFac () ("stage0AttMisMatchOpnSmsg", myOperation_->operationName ());
   }

//------------------------------------------------------------------------------

WitMsgID WitExecBoundSet::headingMsgID ()
   {
   return "execBoundsHeadingDdMsg";
   }

//------------------------------------------------------------------------------

const char * WitExecBoundSet::boundedVectorName ()
   {
   return "ExecVol";
   }

//------------------------------------------------------------------------------

void WitExecBoundSet::idObjectForSoln ()
   {
   fprintf (myProblem ()->solnFile (),
      "operation \"%s\":\n",
      myOperation_->operationName ().myCstring ());
   }
