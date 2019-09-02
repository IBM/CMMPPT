//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "StochImpMgr.C"
//
// Contains the implementation of class StochImpMgr.
//------------------------------------------------------------------------------

#include <StochImpMgr.h>
#include <StochAttMgr.h>
#include <StochAtt.h>
#include <Stage.h>
#include <StageMgr.h>
#include <ScenMgr.h>
#include <Scenario.h>
#include <ScenAttMgr.h>
#include <StochOptMgr.h>
#include <DataWrit.h>
#include <OptComp.h>
#include <Material.h>
#include <Demand.h>
#include <Operation.h>
#include <SubEntry.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class StochImpMgr.
//------------------------------------------------------------------------------

WitStochImpMgr::WitStochImpMgr (WitProblem * theProblem):

      WitProbAssoc    (theProblem),

      stageByObject_  (defStageByObject ()),
      nScenarios_     (defNScenarios ()),
      stochMode_      (defStochMode ()),
      myStochModeMgr_ (NULL)
   {
   }

//------------------------------------------------------------------------------

WitStochImpMgr::~WitStochImpMgr ()
   {
   delete myStochModeMgr_;
   }

//------------------------------------------------------------------------------

void WitStochImpMgr::copyAttrsFrom (WitStochImpMgr * theStochImpMgr)
   {
   stageByObject_ = theStochImpMgr->stageByObject_;
   nScenarios_    = theStochImpMgr->nScenarios_;

   if (stochMode_ != theStochImpMgr->stochMode_)
      setStochMode (theStochImpMgr->stochMode_);

   if (stochMode_)
      myStochModeMgr ()->copyAttrsFrom (theStochImpMgr->myStochModeMgr ());
   }

//------------------------------------------------------------------------------

void WitStochImpMgr::writeData ()
   {
   if (not writeDataNeeded ())
      return;

   if (stochMode_)
      myStochModeMgr ()->myScenMgr  ()->writeUndiffData ();

   myMsgFac () ("stochImpPreCommentWdMsg");

   writeDataID ();

   writeDataAttrs ();

   myDataWriter ()->writeEndCommand ();

   if (stochMode_)
      {
      myStochModeMgr ()->myStageMgr ()->writeData     ();
      myStochModeMgr ()->myScenMgr  ()->writeDiffData ();
      }

   myMsgFac () ("stochImpPostCommentWdMsg");
   }

//------------------------------------------------------------------------------

bool WitStochImpMgr::skipWriteData (
      const char *   theAttName,
      WitComponent * theComp)
   {
   if (stochMode_)
      if (isScenSpec (theAttName))
         if (not myStochModeMgr ()->myScenMgr ()->isWritingData ())
            return true;

   return false;
   }

//------------------------------------------------------------------------------

void WitStochImpMgr::displayGlobalData ()
   {
   myMsgFac () ("stochMgrDdMsg",
      stochMode_,
      stageByObject_,
      nScenarios_);

   if (stochMode_)
      myStochModeMgr ()->myScenMgr ()->displayGlobalData ();
   }

//------------------------------------------------------------------------------

void WitStochImpMgr::setStageByObject (bool theValue)
   {
   myProblem ()->prepSetUnpostAttr ();

   stageByObject_ = theValue;
   }

//------------------------------------------------------------------------------

void WitStochImpMgr::setNScenarios (int theValue)
   {
   stronglyAssert (theValue >= 1);

   myProblem ()->prepSetUnpostAttr ();

   nScenarios_ = theValue;
   }

//------------------------------------------------------------------------------

void WitStochImpMgr::setStochMode (bool theValue)
   {
   myProblem ()->prepSetStochModeAttr ("stochMode");

   if (theValue == stochMode_)
      return;

   if (theValue)
      {
      stronglyAssert (not myOptComp ()->multiObjMode ());

      myStochModeMgr_ = new WitStochModeMgr (myProblem ());

      myStochModeMgr ()->clearCompSoln ();
      }
   else
      {
      delete myStochModeMgr_;

      myStochModeMgr_ = NULL;
      }

   stochMode_ = theValue;
   }

//------------------------------------------------------------------------------

void WitStochImpMgr::prepSetStochModeAttr (const char * theAttName)
   {
   myStochModeMgr ()->myStochAttMgr ()->prepSetStochModeAttr (theAttName);
   }

//------------------------------------------------------------------------------

void WitStochImpMgr::prepSetScenSpecAttr (
      const char * theAttName,
      WitDelComp * theDelComp)
   {
   myStochModeMgr ()->
      myStochAttMgr ()->
         prepSetScenSpecAttr (theAttName, theDelComp);
   }

//------------------------------------------------------------------------------

void WitStochImpMgr::prepSetBoundSet (WitBoundSet * theBoundSet)
   {
   myStochModeMgr ()->myStochAttMgr ()->prepSetBoundSet (theBoundSet);
   }

//------------------------------------------------------------------------------

bool WitStochImpMgr::isScenSpec (const char * theAttName)
   {
   return
      myStochModeMgr ()->
         myStochAttMgr ()->
            valueOf (& WitStochAtt::isScenSpec, theAttName);
   }

//------------------------------------------------------------------------------

bool WitStochImpMgr::canSetInStochMode (const char * theAttName)
   {
   return
      myStochModeMgr ()->
         myStochAttMgr ()->
            valueOf (& WitStochAtt::canSetInStochMode, theAttName);
   }

//------------------------------------------------------------------------------

bool WitStochImpMgr::canSetInStochSolnMode (const char * theAttName)
   {
   return
      myStochModeMgr ()->
         myStochAttMgr ()->
            valueOf (& WitStochAtt::canSetInSolnMode, theAttName);
   }

//------------------------------------------------------------------------------

bool WitStochImpMgr::uploadingData ()
   {
   return myStochModeMgr ()->uploadingData ();
   }

//------------------------------------------------------------------------------

void WitStochImpMgr::displayDataStochMode ()
   {
   myStochModeMgr ()->myScenMgr  ()->displayData ();
   myStochModeMgr ()->myStageMgr ()->displayData ();
   }

//------------------------------------------------------------------------------

void WitStochImpMgr::implode ()
   {
   myStochModeMgr ()->implode ();
   }

//------------------------------------------------------------------------------

void WitStochImpMgr::clearSoln ()
   {
   myStochModeMgr ()->clearSoln ();
   }

//------------------------------------------------------------------------------

void WitStochImpMgr::setCurrentScenario (int theIdx)
   {
   WitScenario * theScenario;

   myProblem ()->prepSetStochModeAttr ("currentScenario");

   theScenario = myStochModeMgr ()->myScenMgr ()->myScenarioFor (theIdx);

   myStochModeMgr ()->myScenMgr ()->setCurScenario (theScenario);
   }

//------------------------------------------------------------------------------

void WitStochImpMgr::setProbability (double theValue)
   {
   myStochModeMgr ()->myScenMgr ()->curScenario ()->setProbability (theValue);
   }

//------------------------------------------------------------------------------

void WitStochImpMgr::setObjectStageIdx (WitNode * theNode, int theStageIdx)
   {
   myStochModeMgr ()->myStageMgr ()->setObjectStageIdx (theNode, theStageIdx);
   }

//------------------------------------------------------------------------------

void WitStochImpMgr::setPeriodStageIdx (WitPeriod thePer, int theStageIdx)
   {
   myStochModeMgr ()->myStageMgr ()->setPeriodStageIdx (thePer, theStageIdx);
   }

//------------------------------------------------------------------------------

int WitStochImpMgr::currentScenario ()
   {
   return myStochModeMgr ()->myScenMgr ()->curScenario ()->myIndex ();
   }

//------------------------------------------------------------------------------

double WitStochImpMgr::probability ()
   {
   return myStochModeMgr ()->myScenMgr ()->curScenario ()->probability ();
   }

//------------------------------------------------------------------------------

int WitStochImpMgr::objectStageIdx (WitNode * theNode)
   {
   return myStochModeMgr ()->myStageMgr ()->objectStage (theNode)->myIndex ();
   }

//------------------------------------------------------------------------------

int WitStochImpMgr::periodStageIdx (WitPeriod thePer)
   {
   return myStochModeMgr ()->myStageMgr ()->periodStage (thePer)->myIndex ();
   }

//------------------------------------------------------------------------------

bool WitStochImpMgr::stochSolnMode ()
   {
   return myStochModeMgr ()->stochSolnMode ();
   }

//------------------------------------------------------------------------------

bool WitStochImpMgr::clearingSoln ()
   {
   return myStochModeMgr ()->clearingSoln ();
   }

//------------------------------------------------------------------------------

bool WitStochImpMgr::writeDataNeeded ()
   {
   if (stageByObject_ != defStageByObject ())
      return true;

   if (nScenarios_ != defNScenarios ())
      return true;

   if (stochMode_)
      return true;

   return false;
   }

//------------------------------------------------------------------------------

void WitStochImpMgr::writeDataID ()
   {
   myDataWriter ()->writeFirstKeyWord ("set");
   myDataWriter ()->writeKeyWord      ("problem");
   }

//------------------------------------------------------------------------------

void WitStochImpMgr::writeDataAttrs ()
   {
   myDataWriter ()->writeBool (
        "stageByObject",
         stageByObject_,
      defStageByObject ());

   myDataWriter ()->writeInt (
        "nScenarios",
         nScenarios_,
      defNScenarios ());

   myDataWriter ()->writeBool (
        "stochMode",
         stochMode_,
      defStochMode ());
   }

//------------------------------------------------------------------------------

void WitStochImpMgr::setSolnAttr (
            WitDblFlexVec &    theAttr,
      const WitTVec <double> & theTVec)
   {
   if (theTVec.isAllocated ())
      theAttr = theTVec;
   else
      theAttr = 0.0;
   }
