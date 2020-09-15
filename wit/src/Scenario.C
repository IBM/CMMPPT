//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "Scenario.C"
//
// Contains the implementation of the following classes:
//
//    ScenMgr
//    Scenario
//------------------------------------------------------------------------------

#include <ScenMgr.h>
#include <Scenario.h>
#include <ScenAttMgr.h>
#include <StochImpMgr.h>
#include <DataWrit.h>
#include <Post.h>
#include <PipMgr.h>
#include <wit/src/Variant.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class ScenMgr.
//------------------------------------------------------------------------------

WitScenMgr::WitScenMgr (WitStochModeMgr * theStochModeMgr):

      WitStochAssoc   (theStochModeMgr),
      myStochModeMgr_ (theStochModeMgr),
      nScenarios_     (myStochImpMgr ()->nScenarios ()),
      myScenarios_    (),
      myScenarioFor_  (myStochImpMgr ()->nScenarios ()),
      curScenario_    (NULL),
      isWritingData_  (false)
   {
   int           theIdx;
   WitScenario * theScenario;

   for (theIdx = 0; theIdx < nScenarios_; theIdx ++)
      {
      theScenario = new WitScenario (myProblem (), theIdx);

      myScenarios_.append (theScenario);

      myScenarioFor_[theIdx] = theScenario;
      }

   curScenario_ = myScenarioFor_[0];
   }

//------------------------------------------------------------------------------

WitScenMgr::~WitScenMgr ()
   {
   int theIdx;

   curScenario_ = NULL;

   myScenarioFor_.clear ();

   while (not myScenarios_.isEmpty ())
      delete myScenarios_.get ();
   }

//------------------------------------------------------------------------------

void WitScenMgr::copyAttrsFrom (WitScenMgr * theScenMgr)
   {
   int           theIdx;
   WitScenario * dupScenario;
   WitScenario * origScenario;

   theIdx        = theScenMgr->curScenario_->myIndex ();

   curScenario_  = myScenarioFor_[theIdx];

   forEachEl (dupScenario, myScenarios_)
      {
      theIdx       = dupScenario->myIndex ();

      origScenario = theScenMgr->myScenarioFor_[theIdx];

      dupScenario->copyAttrsFrom (origScenario);
      }
   }

//------------------------------------------------------------------------------

void WitScenMgr::writeUndiffData ()
   {
   if (not stochSolnMode ())
      myScenAttMgr ()->downloadInput ();

   myMsgFac () ("undiffScenSpecWdMsg");

   isWritingData_ = true;

   myScenAttMgr ()->writeCompData     (curScenario_, false);
   myScenAttMgr ()->writeBoundSetData (curScenario_, false);

   isWritingData_ = false;
   }

//------------------------------------------------------------------------------

void WitScenMgr::writeDiffData ()
   {
   WitScenario * theScenario;

   isWritingData_ = true;

   forEachEl (theScenario, myScenarios_)
      {
      theScenario->writeData ();

      myScenAttMgr ()->writeCompData     (theScenario, true);
      myScenAttMgr ()->writeBoundSetData (theScenario, true);
      }

   myMsgFac () ("finalScenIdxWdMsg");

   curScenario_->writeMyIndex ();

   isWritingData_ = false;
   }

//------------------------------------------------------------------------------

void WitScenMgr::displayGlobalData ()
   {
   myMsgFac () ("stochImploderDdMsg", curScenario_->myIndex ());
   }

//------------------------------------------------------------------------------

void WitScenMgr::displayData ()
   {
   WitScenario * origScenario;

   if (not stochSolnMode ())
      myScenAttMgr ()->downloadInput ();

   origScenario = curScenario_;

   forEachEl (curScenario_, myScenarios_)
      {
      myStochModeMgr ()->uploadInput ();

      myMsgFac () ("scenarioDdMsg",
         curScenario_->myIndex (),
         curScenario_->probability ());

      myProblem ()->displayNonGlobalData ();
      }

   curScenario_ = origScenario;

   myStochModeMgr ()->uploadInput ();
   }

//------------------------------------------------------------------------------

void WitScenMgr::validateData ()
   {
   double        totProb;
   WitScenario * theScenario;

   totProb = 0.0;

   forEachEl (theScenario, myScenarios_)
      totProb += theScenario->probability ();

   if (fabs (totProb - 1.0) > 0.001)
      myMsgFac () ("totScenProbNeq1Smsg", totProb);
   }

//------------------------------------------------------------------------------

void WitScenMgr::setCurScenario (WitScenario * theScenario)
   {
   witAssert (theScenario != NULL);

   if (not stochSolnMode ())
      myScenAttMgr ()->downloadInput ();

   myProblem ()->myPipMgr ()->clearShipSeq ();

   curScenario_ = theScenario;

   myStochModeMgr ()->uploadInput ();

   if (stochSolnMode ())
      {
      myStochModeMgr ()->uploadSoln ();

      myProblem ()->optVariant ()->becomesCurrent ();

      myPostprocessor ()->postprocess ();
      }
   }

//------------------------------------------------------------------------------
// Implementation of class Scenario.
//------------------------------------------------------------------------------

WitScenario::WitScenario (WitProblem * theProblem, int theIdx):

      WitProbAssoc (theProblem),
      myIndex_     (theIdx),
      probability_ (defProbability ())
   {
   }

//------------------------------------------------------------------------------

WitScenario::~WitScenario ()
   {
   }

//------------------------------------------------------------------------------

void WitScenario::copyAttrsFrom (WitScenario * thatScenario)
   {
   probability_ = thatScenario->probability_;
   }

//------------------------------------------------------------------------------

void WitScenario::writeData ()
   {
   myMsgFac () ("scenarioHeadingWdMsg", myIndex_);

   writeMyIndex ();

   writeProbability ();
   }

//------------------------------------------------------------------------------

void WitScenario::writeMyIndex ()
   {
   myDataWriter ()->writeFirstKeyWord ("set");
   myDataWriter ()->writeKeyWord      ("problem");

   myDataWriter ()->writeInt ("currentScenario", myIndex_);

   myDataWriter ()->writeEndCommand ();
   }

//------------------------------------------------------------------------------

double WitScenario::defProbability ()
   {
   return 1.0 / static_cast <double> (myStochImpMgr ()->nScenarios ());
   }

//------------------------------------------------------------------------------

void WitScenario::setProbability (double theValue)
   {
   stronglyAssert (theValue >= 0.0);
   stronglyAssert (theValue <= 1.0 + FEAS_TOL);

   myProblem ()->prepSetStochModeAttr ("probability");

   probability_ = theValue;
   }

//------------------------------------------------------------------------------

int WitScenario::nInstances (WitProblem * theProblem)
   {
   return theProblem->myStochImpMgr ()->nScenarios ();
   }

//------------------------------------------------------------------------------

void WitScenario::writeProbability ()
   {
   if (probability_ == defProbability ())
      return;

   myDataWriter ()->writeFirstKeyWord ("set");
   myDataWriter ()->writeKeyWord      ("problem");

   myDataWriter ()->writeDouble (
      "probability",
       probability_,
       defProbability ());

   myDataWriter ()->writeEndCommand ();
   }
