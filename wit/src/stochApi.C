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
// Source file: "stochApi.C"
//
// Inner API functions relating to stochastic implosion.
//------------------------------------------------------------------------------

#include <WitRun.h>
#include <ApiMgr.h>
#include <InputID.h>
#include <ApiCall.h>
#include <StochImpMgr.h>
#include <OptComp.h>
#include <ISRealArg.h>
#include <OSRealArg.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------

void WitRun::witClearStochSoln ()
   {
   WitStochApiCall theCall (this);

   if (not myStochImpMgr ()->stochMode ())
      myMsgFac () ("funcNeedsStochModeSmsg", "witClearStochSoln");

   if (not myStochImpMgr ()->stochSolnMode ())
      myMsgFac () ("funcWoStochSolnSmsg", "witClearStochSoln");

   myStochImpMgr ()->clearSoln ();
   }

//------------------------------------------------------------------------------

void WitRun::witGetCurrentScenario (int * currentScenario)
   {
   WitStochApiCall theCall (this);

   if (not myStochImpMgr ()->stochMode ())
      myMsgFac () ("globNeedsStochModeSmsg", "currentScenario");

   apiGet (
                                currentScenario,
                               "currentScenario",
      theCall.myStochImpMgr ()->currentScenario ());

   issueGetAttrMsg ("currentScenario");
   }

//------------------------------------------------------------------------------

void WitRun::witGetNScenarios (int * nScenarios)
   {
   WitStochApiCall theCall (this);

   apiGet (             nScenarios, 
                       "nScenarios", 
      myStochImpMgr ()->nScenarios ());

   issueGetAttrMsg ("nScenarios");
   }

//------------------------------------------------------------------------------

void WitRun::witGetPeriodStage (int * * periodStage)
   {
   WitStochApiCall theCall (this);
   WitPeriod       thePer;

   if (not myStochImpMgr ()->stochMode ())
      myMsgFac () ("globNeedsStochModeSmsg", "periodStage");

   if (myStochImpMgr ()->stageByObject ())
      myMsgFac () ("periodStageWStageByObjectSmsg");

   forbidNullArg (periodStage, "periodStage");

   apiAlloc (periodStage, myProblem ()->nPeriods ());

   forEachPeriod (thePer, myProblem ())
      (* periodStage)[thePer] = myStochImpMgr ()->periodStageIdx (thePer);

   issueGetAttrMsg ("periodStage");
   }

//------------------------------------------------------------------------------

void WitRun::witGetProbability (const WitOSRealArg & probability)
   {
   WitStochApiCall theCall (this);

   if (not myStochImpMgr ()->stochMode ())
      myMsgFac () ("globNeedsStochModeSmsg", "probability");

   apiGet (                     probability,
                               "probability",
      theCall.myStochImpMgr ()->probability ());

   issueGetAttrMsg ("probability");
   }

//------------------------------------------------------------------------------

void WitRun::witGetStageByObject (WitBoolean * stageByObject)
   {
   WitStochApiCall theCall (this);

   apiGet (             stageByObject, 
                       "stageByObject", 
      myStochImpMgr ()->stageByObject ());

   issueGetAttrMsg ("stageByObject");
   }

//------------------------------------------------------------------------------

void WitRun::witGetStochMode (WitBoolean * stochMode)
   {
   WitStochApiCall theCall (this);

   apiGet (             stochMode, 
                       "stochMode", 
      myStochImpMgr ()->stochMode ());

   issueGetAttrMsg ("stochMode");
   }

//------------------------------------------------------------------------------

void WitRun::witGetStochSolnMode (WitBoolean * stochSolnMode)
   {
   WitStochApiCall theCall (this);

   if (not myStochImpMgr ()->stochMode ())
      myMsgFac () ("globNeedsStochModeSmsg", "stochSolnMode");

   apiGet (             stochSolnMode, 
                       "stochSolnMode", 
      myStochImpMgr ()->stochSolnMode ());

   issueGetAttrMsg ("stochSolnMode");
   }

//------------------------------------------------------------------------------

void WitRun::witSetCurrentScenario (int currentScenario)
   {
   WitStochApiCall theCall (this);

   if (not myStochImpMgr ()->stochMode ())
      myMsgFac () ("globNeedsStochModeSmsg", "currentScenario");

   theCall.prepImmRangedInt (
                               "currentScenario",
      theCall.myStochImpMgr ()->currentScenario (),
                                currentScenario,
      0,
      myStochImpMgr ()->nScenarios () - 1);

   if (theCall.argsOK ())
      theCall.myStochImpMgr ()->setCurrentScenario (currentScenario);
   }

//------------------------------------------------------------------------------

void WitRun::witSetNScenarios (int nScenarios)
   {
   WitStochApiCall theCall (this);

   theCall.prepImmPosInt (
                       "nScenarios", 
      myStochImpMgr ()->nScenarios (), 
                        nScenarios);

   if (theCall.argsOK ())
      myStochImpMgr ()->setNScenarios (nScenarios);
   }

//------------------------------------------------------------------------------

void WitRun::witSetPeriodStage (const int * periodStage)
   {
   WitStochApiCall theCall (this);

   theCall.setPeriodStage (periodStage);
   }

//------------------------------------------------------------------------------

void WitRun::witSetProbability (const WitISRealArg & probability)
   {
   WitStochApiCall theCall (this);

   if (not myStochImpMgr ()->stochMode ())
      myMsgFac () ("globNeedsStochModeSmsg", "probability");

   theCall.prepNonNegUBDouble (
                               "probability",
      theCall.myStochImpMgr ()->probability (),
                                probability,
                                1. + FEAS_TOL,
                                1.);

   if (theCall.argsOK ())
      theCall.myStochImpMgr ()->setProbability (probability);
   }

//------------------------------------------------------------------------------

void WitRun::witSetStageByObject (WitBoolean boolval)
   {
   WitStochApiCall theCall (this);

   theCall.prepBool (
                       "stageByObject", 
      myStochImpMgr ()->stageByObject (), 
      boolval);

   if (theCall.argsOK ())
      myStochImpMgr ()->setStageByObject (asaBool (boolval));
   }

//------------------------------------------------------------------------------

void WitRun::witSetStochMode (WitBoolean boolval)
   {
   WitStochApiCall theCall (this);

   if (boolval)
      if (theCall.myOptComp ()->multiObjMode ())
         myMsgFac () ("stochModeInMultiObjSmsg");

   theCall.prepBool (  
                       "stochMode", 
      myStochImpMgr ()->stochMode (), 
      boolval);

   if (theCall.argsOK ())
      myStochImpMgr ()->setStochMode (asaBool (boolval));
   }

//------------------------------------------------------------------------------

void WitRun::witStochImplode ()
   {
   WitStochApiCall theCall (this);

   if (not myStochImpMgr ()->stochMode ())
      myMsgFac () ("funcNeedsStochModeSmsg", "witStochImplode");

   if (myStochImpMgr ()->stochSolnMode ())
      myMsgFac () ("funcWStochSolnSmsg", "witStochImplode");

   myStochImpMgr ()->implode ();
   }
