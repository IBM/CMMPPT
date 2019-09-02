//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "Pegging.C"
//
// Contains the implementation of the following classes:
//
//    Pegger
//    PegEl
//    FixedPegEl
//------------------------------------------------------------------------------

#include <Pegger.h>
#include <PegEl.h>
#include <HeurAllMgr.h>
#include <CompMgr.h>
#include <Demand.h>
#include <Part.h>
#include <Operation.h>
#include <SubEntry.h>
#include <HeurAtor.h>
#include <Selector.h>
#include <SglSrcMgr.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class Pegger.
//------------------------------------------------------------------------------

WitPegger::WitPegger (WitProblem * theProblem):
      WitProbAssoc     (theProblem),

      myHeurAtor_      (NULL),
      curDemand_       (NULL),
      curShipPer_      (-1),
      myPegElStack_    (theProblem),
      curOpnPegEls_    (),
      curSubPegEls_    (),
      nonEmptyDemPers_ (theProblem)
   {
   }

//------------------------------------------------------------------------------

WitPegger::~WitPegger ()
   {
   clear ();
   }

//------------------------------------------------------------------------------

void WitPegger::setUpPeggedHeurAlloc (WitHeurAtor * theHeurAtor)
   {
   WitDemand * theDemand;

   witAssert (myHeurAtor_ == NULL);
   witAssert (curDemand_  == NULL);

   clear ();

   myHeurAtor_ = theHeurAtor;

   curOpnPegEls_.allocate (myProblem ());
   curSubPegEls_.allocate (myProblem ());

   myMsgFac () ("concPeggingMsg");
   }

//------------------------------------------------------------------------------

void WitPegger::setUpPeggedIncAlloc (WitDemand * theDemand, WitPeriod shipPer)
   {
   WitObjStItr <WitPegEl> theItr;
   WitPegEl *             thePegEl;

   witAssert (myHeurAtor_ != NULL);
   witAssert (curDemand_  == NULL);

   curDemand_  = theDemand;
   curShipPer_ = shipPer;

   curDemand_->extractPegEls (curShipPer_, myPegElStack_);

   myPegElStack_.attachItr (theItr);

   while (theItr.advance (thePegEl))
      curPegElsPtrFor (thePegEl) = thePegEl;
   }

//------------------------------------------------------------------------------

void WitPegger::pegDeltaVol (
      WitOperation * theOpn,
      WitPeriod      execPer,
      double         deltaVol)
   {
   WitPegEl * thePegEl;

   if (curDemand_ == NULL)
      return;

   thePegEl = curOpnPegEls_.myPtrAt (theOpn, execPer);
   
   pegDeltaVol (theOpn, execPer, thePegEl, deltaVol);
   }

//------------------------------------------------------------------------------

void WitPegger::pegDeltaVol (
      WitSubEntry * theSub,
      WitPeriod     execPer,
      double        deltaVol)
   {
   WitPegEl * thePegEl;

   if (curDemand_ == NULL)
      return;

   thePegEl = curSubPegEls_.myPtrAt (theSub, execPer);
   
   pegDeltaVol (theSub, execPer, thePegEl, deltaVol);
   }

//------------------------------------------------------------------------------

void WitPegger::shutDownPeggedIncAlloc ()
   {
   WitObjStItr <WitPegEl> theItr;
   WitPegEl *             thePegEl;

   witAssert (myHeurAtor_ != NULL);
   witAssert (curDemand_  != NULL);

   myPegElStack_.attachItr (theItr);

   while (theItr.advance (thePegEl))
      curPegElsPtrFor (thePegEl) = NULL;

   if (myHeurAtor_->multiSel ())
      if (myHeurAtor_->mySelector ()->sglSrc ())
         deleteZeroPegEls ();

   curDemand_->receivePegEls (curShipPer_, myPegElStack_);

   curDemand_  = NULL;
   curShipPer_ = -1;
   }

//------------------------------------------------------------------------------

void WitPegger::shutDownPeggedHeurAlloc ()
   {
   witAssert (myHeurAtor_ != NULL);
   witAssert (curDemand_  == NULL);

   curSubPegEls_.clear ();
   curOpnPegEls_.clear ();

   myHeurAtor_ = NULL;
   }

//------------------------------------------------------------------------------

void WitPegger::clear ()
   {
   WitDemand * theDemand;
   WitPeriod   shipPer;

   while (nonEmptyDemPers_.pop (theDemand, shipPer))
      theDemand->clearPegging (shipPer);
   }

//------------------------------------------------------------------------------

void WitPegger::print ()
   {
   WitDemand *                 theDemand;
   WitPeriod                   shipPer;
   WitObjStack <WitFixedPegEl> theFixedPegEls (myProblem ());

   fprintf (myProblem ()->solnFile (),
      "\n\n"
      "Concurrent Pegging:\n");

   forEachDemand (theDemand, myProblem ())
      forEachPeriod (shipPer, myProblem ())
         {
         theDemand->getMyFixedPegEls (shipPer, theFixedPegEls);

         if (theFixedPegEls.isEmpty ())
            continue;

         fprintf (myProblem ()->solnFile (),
            "\n\n"
            "Part %s, Demand %s, Ship Per %d:\n",
            theDemand->demandedPartName ().myCstring (),
            theDemand->demandName ().myCstring (),
            shipPer);

         printOpnPegEls (theFixedPegEls);
         printSubPegEls (theFixedPegEls);
         }

   fprintf (myProblem ()->solnFile (), "\n");
   }

//------------------------------------------------------------------------------

void WitPegger::pegDeltaVol (
      WitDelComp * theDelComp,
      WitPeriod    execPer,
      WitPegEl *   thePegEl,
      double       deltaVol)
   {
   witAssert (curDemand_ != NULL);

   witAssert (deltaVol >= NET_TOL);

   if (thePegEl == NULL)
      {
      thePegEl = new WitPegEl (theDelComp, execPer);

      if (myPegElStack_.isEmpty ())
         nonEmptyDemPers_.push (curDemand_, curShipPer_);

      myPegElStack_.push (thePegEl);

      curPegElsPtrFor (thePegEl) = thePegEl;
      }

   if (myHeurAtor_->multiSel ())
      if (myHeurAtor_->mySelector ()->sglSrc ())
         thePegEl->saveStateIn (myHeurAtor_->mySelector ()->mySglSrcMgr ());

   thePegEl->pegDeltaVol (deltaVol);
   }

//------------------------------------------------------------------------------

void WitPegger::deleteZeroPegEls ()
   {
   WitObjStack <WitPegEl> tempPegElStack (myProblem ());
   WitPegEl *             thePegEl;

   while (myPegElStack_.pop (thePegEl))
      if (thePegEl->peggedVol () == 0.0)
         delete thePegEl;
      else
         tempPegElStack.push (thePegEl);

   myPegElStack_.revCopyFrom (tempPegElStack);
   }

//------------------------------------------------------------------------------

void WitPegger::printOpnPegEls (
      const WitObjStack <WitFixedPegEl> & theFixedPegEls)
   {
   WitObjStItr <WitFixedPegEl> theItr;
   WitFixedPegEl *             theFixedPegEl;
   WitOperation *              theOpn;

   fprintf (myProblem ()->solnFile (), "\n   Opn  Per   ExecVol\n");

   theFixedPegEls.attachItr (theItr);

   while (theItr.advance (theFixedPegEl))
      {
      theOpn = theFixedPegEl->myOpn ();

      if (theOpn != NULL)
         fprintf (myProblem ()->solnFile (),
            "   %-3s  %-3d %9.3f\n",
            theOpn  ->operationName ().myCstring (),
            theFixedPegEl->myExecPer (),
            theFixedPegEl->peggedVol ());
      }
   }

//------------------------------------------------------------------------------

void WitPegger::printSubPegEls (
      const WitObjStack <WitFixedPegEl> & theFixedPegEls)
   {
   WitObjStItr <WitFixedPegEl> theItr;
   WitFixedPegEl *             theFixedPegEl;
   WitSubEntry *               theSub;

   fprintf (
      myProblem ()->solnFile (), 
      "\n   Opn  B-E  Sub  Part  Per   ExecVol\n");

   theFixedPegEls.attachItr (theItr);

   while (theItr.advance (theFixedPegEl))
      {
      theSub = theFixedPegEl->mySub ();

      if (theSub != NULL)
         fprintf (myProblem ()->solnFile (),
            "   %-3s  %-3d  %-3d  %-4s  %-3d %9.3f\n",
            theSub  ->myOperationName ().myCstring (),
            theSub  ->myBomEntIndex (),
            theSub  ->localIndex (),
            theSub  ->myPart ()->partName ().myCstring (),
            theFixedPegEl->myExecPer (),
            theFixedPegEl->peggedVol ());
      }
   }

//------------------------------------------------------------------------------

WitPegEl * & WitPegger::curPegElsPtrFor (WitPegEl * thePegEl)
   {
   WitOperation * theOpn;
   WitSubEntry *  theSub;

   theOpn = thePegEl->myOpn ();

   if (theOpn != NULL)
      return curOpnPegEls_.myPtrAt (theOpn, thePegEl->myExecPer ());

   theSub = thePegEl->mySub ();

   witAssert (theSub != NULL);

   return    curSubPegEls_.myPtrAt (theSub, thePegEl->myExecPer ());
   }

//------------------------------------------------------------------------------
// Implementation of class PegEl.
//------------------------------------------------------------------------------

WitPegEl::WitPegEl (WitDelComp * theDelComp, WitPeriod execPer):
      myDelComp_    (theDelComp),
      myExecPer_    (execPer),
      peggedVol_    (0.0),
      myFixedPegEl_ (NULL)
   {
   witAssert ((myOpn () != NULL) or (mySub () != NULL));

   myFixedPegEl_ = new WitFixedPegEl (this);
   }

//------------------------------------------------------------------------------

WitPegEl::~WitPegEl ()
   {
   delete myFixedPegEl_;
   }

//------------------------------------------------------------------------------

void WitPegEl::saveStateIn (WitSglSrcMgr * theSglSrcMgr)
   {
   theSglSrcMgr->saveAllocVal (peggedVol_);
   }

//------------------------------------------------------------------------------

void WitPegEl::pegDeltaVol (double deltaVol)
   {
   peggedVol_ += deltaVol;
   }

//------------------------------------------------------------------------------

WitOperation * WitPegEl::myOpn () const
   {
   return myDelComp_->thisOpn ();
   }

//------------------------------------------------------------------------------

WitSubEntry * WitPegEl::mySub () const
   {
   return myDelComp_->thisSub ();
   }

//------------------------------------------------------------------------------
// Implementation of class FixedPegEl.
//------------------------------------------------------------------------------

WitFixedPegEl::WitFixedPegEl (WitPegEl * thePegEl):
      myPegEl_ (thePegEl)
   {
   }

//------------------------------------------------------------------------------

WitFixedPegEl::~WitFixedPegEl ()
   {
   }

