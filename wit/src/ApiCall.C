//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "ApiCall.C"
//
// Contains the implementation of the following classes:
//
//    ApiCall
//    GlobalCompApiCall
//    OptCompApiCall
//    StochApiCall
//    PartApiCall
//    MaterialApiCall
//    DemandApiCall
//    OperationApiCall
//    BomEntryApiCall
//    SubEntryApiCall
//    BopEntryApiCall
//------------------------------------------------------------------------------

#include <ApiCall.h>
#include <ApiMgr.h>
#include <InputID.h>
#include <WitRun.h>
#include <IVRealArg.h>
#include <DetOptImpMgr.h>
#include <CompMgr.h>
#include <GlobalComp.h>
#include <OptComp.h>
#include <Material.h>
#include <Operation.h>
#include <BomEntry.h>
#include <SubEntry.h>
#include <BopEntry.h>
#include <Demand.h>
#include <BoundSet.h>
#include <MultiObjMgr.h>
#include <StochImpMgr.h>
#include <PipMgr.h>
#include <MsgFrag.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class ApiCall.
//------------------------------------------------------------------------------

inline WitInputID * WitApiCall::myInputID ()
   {
   return myApiMgr ()->myInputID ();
   }

//------------------------------------------------------------------------------

void WitApiCall::prepBool (
      const char * theAttName,
      bool         oldValue,
      WitBoolean   theValue)
   {
   prepSetAttr (theAttName);

   if (myMsgFac ().mayIssue ("setBooleanAttrMsg"))
      myMsgFac () ("setBooleanAttrMsg", theAttName, oldValue, theValue);
   }

//------------------------------------------------------------------------------

void WitApiCall::prepInt (
      const char * theAttName,
      int          oldValue,
      int          theValue)
   {
   prepSetAttr (theAttName);

   if (myMsgFac ().mayIssue ("setIntAttrMsg"))
      myMsgFac () ("setIntAttrMsg", theAttName, oldValue, theValue);
   }

//------------------------------------------------------------------------------

void WitApiCall::prepNonNegInt (
      const char * theAttName,
      int          oldValue,
      int          theValue)
   {
   prepSetAttr (theAttName);

   if (theValue < 0)
      {
      errorFound ();

      myMsgFac () ("intLowEqSmsg", theValue, 0);
      }
   else
      if (myMsgFac ().mayIssue ("setIntAttrMsg"))
         myMsgFac () ("setIntAttrMsg", theAttName, oldValue, theValue);
   }

//------------------------------------------------------------------------------

void WitApiCall::prepImmPosInt (
      const char * theAttName,
      int          oldValue,
      int          theValue)
   {
   prepSetAttr (theAttName);

   if (theValue < 1)
      myMsgFac () ("intLowEqImmSmsg", theValue, 1);

   if (myMsgFac ().mayIssue ("setIntAttrMsg"))
      myMsgFac () ("setIntAttrMsg", theAttName, oldValue, theValue);
   }

//------------------------------------------------------------------------------

void WitApiCall::prepRangedInt (
      const char * theAttName,
      int          oldValue,
      int          theValue,
      int          lb,
      int          ub)
   {
   prepSetAttr (theAttName);

   if ((theValue < lb) or (theValue > ub))
      {
      errorFound ();

      myMsgFac () ("intLowEqUpEqSmsg", theValue, lb, ub);
      }
   else
      if (myMsgFac ().mayIssue ("setIntAttrMsg"))
         myMsgFac () ("setIntAttrMsg", theAttName, oldValue, theValue);
   }

//------------------------------------------------------------------------------

void WitApiCall::prepImmRangedInt (
      const char * theAttName,
      int          oldValue,
      int          theValue,
      int          lb,
      int          ub)
   {
   prepSetAttr (theAttName);

   if ((theValue < lb) or (theValue > ub))
      myMsgFac () ("intLowEqUpEqImmSmsg", theValue, lb, ub);
   else
      if (myMsgFac ().mayIssue ("setIntAttrMsg"))
         myMsgFac () ("setIntAttrMsg", theAttName, oldValue, theValue);
   }

//------------------------------------------------------------------------------

void WitApiCall::prepDouble (
      const char * theAttName,
      double       oldValue,
      double       theValue)
   {
   prepSetAttr (theAttName);

   if (myMsgFac ().mayIssue ("setDblAttrMsg"))
      myMsgFac () ("setDblAttrMsg", theAttName, oldValue, theValue);
   }

//------------------------------------------------------------------------------

void WitApiCall::prepNonNegDouble (
      const char * theAttName, 
      double       oldValue,
      double       theValue)
   {
   prepLBDouble (theAttName, oldValue, theValue, 0.0, 0.0);
   }

//------------------------------------------------------------------------------

void WitApiCall::prepLBDouble (
      const char * theAttName, 
      double       oldValue, 
      double       theValue,
      double       lb,
      double       msgLB)
   {
   prepSetAttr (theAttName);

   if (theValue < lb)
      {
      errorFound ();

      myMsgFac () ("dblLowEqSmsg", theValue, msgLB);
      }
   else
      if (myMsgFac ().mayIssue ("setDblAttrMsg"))
         myMsgFac () ("setDblAttrMsg", theAttName, oldValue, theValue);
   }

//------------------------------------------------------------------------------

void WitApiCall::prepNonNegUBDouble (
      const char * theAttName, 
      double       oldValue, 
      double       theValue,
      double       ub,
      double       msgUB)
   {
   prepSetAttr (theAttName);

   if (theValue < 0.0)
      myMsgFac () ("dblLowEqUpEqImmSmsg", theValue, 0.0, msgUB);

   if (theValue > ub)
      myMsgFac () ("dblLowEqUpEqImmSmsg", theValue, 0.0, msgUB);

   if (myMsgFac ().mayIssue ("setDblAttrMsg"))
      myMsgFac () ("setDblAttrMsg", theAttName, oldValue, theValue);
   }

//------------------------------------------------------------------------------

void WitApiCall::prepPosDouble (
      const char * theAttName,
      double       oldValue,
      double       theValue)
   {
   prepSetAttr (theAttName);

   if (theValue <= 0.0)
      {
      errorFound ();

      myMsgFac () ("dblLowSmsg", theValue, 0.0);
      }
   else
      if (myMsgFac ().mayIssue ("setDblAttrMsg"))
         myMsgFac () ("setDblAttrMsg", theAttName, oldValue, theValue);
   }

//------------------------------------------------------------------------------

void WitApiCall::prepBoolVec (
      const char *       theAttName,
      const WitBoolean * theValue)
   {
   prepSetAttr (theAttName);

   forbidNullSetArg (theValue);

   if (myMsgFac ().mayIssue ("setVecBoolAttrMsg"))
      {
      WitMsgArg theMsgArg (theValue, nPeriods ());

      myMsgFac () ("setVecBoolAttrMsg", theAttName, theMsgArg);
      }
   }

//------------------------------------------------------------------------------

void WitApiCall::prepIntVec (
      const char * theAttName,
      const int *  theValue)
   {
   prepSetAttr (theAttName);

   forbidNullSetArg (theValue);

   if (myMsgFac ().mayIssue ("setVecIntAttrMsg"))
      {
      WitMsgArg theMsgArg (theValue, nPeriods ());

      myMsgFac () ("setVecIntAttrMsg", theAttName, theMsgArg);
      }
   }

//------------------------------------------------------------------------------

void WitApiCall::prepDblVec (
      const char *         theAttName,
      const WitIVRealArg & theValue)
   {
   prepSetAttr (theAttName);

   forbidNullSetArg (theValue);

   theValue.issueSetDblVecMsg (theAttName, * this);
   }

//------------------------------------------------------------------------------

void WitApiCall::prepNonNegDblVec (
      const char *         theAttName,
      const WitIVRealArg & theValue)
   {
   WitPeriod thePer;

   prepSetAttr (theAttName);

   forbidNullSetArg (theValue);

   forEachPeriod (thePer, myProblem ())
      if (theValue[thePer] < 0.0)
         {
         errorFound (thePer);

         myMsgFac () ("dblLowEqSmsg", theValue[thePer], 0.0);
         }

   if (argsOK_)
      theValue.issueSetDblVecMsg (theAttName, * this);
   }

//------------------------------------------------------------------------------

void WitApiCall::prepBuildAheadUB (const int * theValue)
   {
   WitPeriod thePer;

   prepSetAttr ("buildAheadUB");

   checkPeriodIntVec (theValue);

   for (thePer = 0; thePer <= nPeriods () - 2; thePer ++)
      {
      if (theValue[thePer + 1] > theValue[thePer] + 1)
         {
         errorFound (thePer);

         myMsgFac () ("selfConsistSmsg",
            "buildAheadUB[t+1] <= buildAheadUB[t] + 1",
            "buildAheadUB",
            thePer,
            theValue[thePer],
            thePer + 1,
            theValue[thePer + 1]);
         }
      }

   if (argsOK_)
      if (myMsgFac ().mayIssue ("setVecIntAttrMsg"))
         {
         WitMsgArg theMsgArg (theValue, nPeriods ());

         myMsgFac () ("setVecIntAttrMsg", "buildAheadUB", theMsgArg);
         }
   }

//------------------------------------------------------------------------------

void WitApiCall::prepLeadTimeUB (const int * theValue)
   {
   WitPeriod thePer;

   prepSetAttr ("leadTimeUB");

   checkPeriodIntVec (theValue);

   for (thePer = 0; thePer <= nPeriods () - 2; thePer ++)
      {
      if (theValue[thePer + 1] > theValue[thePer] + 1)
         {
         errorFound (thePer);

         myMsgFac () ("selfConsistSmsg",
            "leadTimeUB[t+1] <= leadTimeUB[t] + 1",
            "leadTimeUB",
            thePer,
            theValue[thePer],
            thePer + 1,
            theValue[thePer + 1]);
         }
      }

   if (argsOK_)
      if (myMsgFac ().mayIssue ("setVecIntAttrMsg"))
         {
         WitMsgArg theMsgArg (theValue, nPeriods ());

         myMsgFac () ("setVecIntAttrMsg", "leadTimeUB", theMsgArg);
         }
   }

//------------------------------------------------------------------------------

void WitApiCall::prepShipLateUB (const int * theValue)
   {
   WitPeriod thePer;

   prepSetAttr ("shipLateUB");

   checkPeriodIntVec (theValue);

   for (thePer = 0; thePer <= nPeriods () - 2; thePer ++)
      {
      if (theValue[thePer + 1] < theValue[thePer] - 1)
         {
         errorFound (thePer);

         myMsgFac () ("selfConsistSmsg",
            "shipLateUB[t+1] >= shipLateUB[t] - 1",
            "shipLateUB",
            thePer,
            theValue[thePer],
            thePer + 1,
            theValue[thePer + 1]);
         }
      }

   if (argsOK_)
      if (myMsgFac ().mayIssue ("setVecIntAttrMsg"))
         {
         WitMsgArg theMsgArg (theValue, nPeriods ());

         myMsgFac () ("setVecIntAttrMsg", "shipLateUB", theMsgArg);
         }
   }

//------------------------------------------------------------------------------

void WitApiCall::prepString (
      const char *      theAttName,
      const WitString & oldValue,
      const char *      theValue)
   {
   prepSetAttr (theAttName);

   forbidNullSetArg (theValue);

   if (myMsgFac ().mayIssue ("setStringAttrMsg"))
      myMsgFac () ("setStringAttrMsg", theAttName, oldValue, theValue);
   }

//------------------------------------------------------------------------------

void WitApiCall::setAppData (void * appData)
   {
   prepSetAttr ("appData");

   if (argsOK ())
      myComponent ()->setAppData (appData);
   }

//------------------------------------------------------------------------------

void WitApiCall::setSelForDel (WitBoolean theValue)
   {
   prepBool ("selForDel", myDelComp ()->selForDel (), theValue);

   if (argsOK ())
      myDelComp ()->setSelForDel (asaBool (theValue));
   }

//------------------------------------------------------------------------------

void WitApiCall::setEarliestPeriod (int theValue)
   {
   prepRangedInt (
      "earliestPeriod",
      myBillEntry ()->earliestPeriod (),
      theValue,
      0,
      nPeriods ());

   if (argsOK ())
      myBillEntry ()->setEarliestPeriod (theValue);
   }

//------------------------------------------------------------------------------

void WitApiCall::setLatestPeriod (int theValue)
   {
   prepRangedInt (
      "latestPeriod",
      myBillEntry ()->latestPeriod (),
      theValue,
      0,
      lastPeriod ());

   if (argsOK ())
      myBillEntry ()->setLatestPeriod (theValue);
   }

//------------------------------------------------------------------------------

void WitApiCall::setRoutingShare (const WitIVRealArg & theValue)
   {
   WitPeriod thePer;

   prepSetAttr ("routingShare");

   forbidNullSetArg (theValue);

   forEachPeriod (thePer, myProblem ())
      if (theValue[thePer] < 1.0 - FLOAT_EPSILON)
         {
         errorFound (thePer);

         myMsgFac () ("routingShareRangeSmsg", theValue[thePer]);
         }

   if (argsOK ())
      {
      theValue.issueSetDblVecMsg ("routingShare", * this);

      myBillEntry ()->setRoutingShare (theValue);
      }
   }

//------------------------------------------------------------------------------

void WitApiCall::setOffset (const WitIVRealArg & theValue)
   {
   prepSetAttr ("offset");

   if (myBillEntry ()->storesOffset ())
      prepDblVec ("offset", theValue);
   else
      {
      errorFound ();

      myMsgFac () ("setSubOffsetSmsg");
      }

   if (argsOK ())
      myBillEntry ()->setOffset (theValue);
   }

//------------------------------------------------------------------------------

void WitApiCall::setObjectStage (int theValue)
   {
   WitNode * theNode;

   theNode = myComponent ()->thisNode ();

   witAssert (theNode != NULL);

   if (not myStochImpMgr ()->stochMode ())
      myMsgFac () ("objNeedsStochModeSmsg",
         "objectStage",
         theNode->objTypeMsgFrag ());

   if (not myStochImpMgr ()->stageByObject ())
      myMsgFac () ("objectStageWoStageByObjectSmsg",
         theNode->objTypeMsgFrag ());

   prepImmRangedInt (
                       "objectStage", 
      myStochImpMgr ()->objectStageIdx (theNode),
                        theValue,
                        0,
                        1);

   if (argsOK ())
      myStochImpMgr ()->setObjectStageIdx (theNode, theValue);
   }

//------------------------------------------------------------------------------

void WitApiCall::getObjectStage (int * objectStage)
   {
   WitNode * theNode;

   theNode = myComponent ()->thisNode ();

   witAssert (theNode != NULL);

   if (not myStochImpMgr ()->stochMode ())
      myMsgFac () ("objNeedsStochModeSmsg",
         "objectStage",
         theNode->objTypeMsgFrag ());

   if (not myStochImpMgr ()->stageByObject ())
      myMsgFac () ("objectStageWoStageByObjectSmsg",
         theNode->objTypeMsgFrag ());

   myWitRun ()->
      apiGet (             objectStage,
                          "objectStage",
         myStochImpMgr ()->objectStageIdx (theNode));
   }

//------------------------------------------------------------------------------

void WitApiCall::setBoundSet (
      WitBoundSet *        theBoundSet,
      const WitIVRealArg & hardLBArg,
      const WitIVRealArg & softLBArg,
      const WitIVRealArg & hardUBArg)
   {
   WitPeriod thePer;

   WitDblFlexVec hardLBMrg (myProblem (), 0.0);
   WitDblFlexVec softLBMrg (myProblem (), 0.0);
   WitDblFlexVec hardUBMrg (myProblem (), 0.0);
      //
      // The merged values to which theBoundSet's vectors are to be set.

   prepSetAttr (theBoundSet->myAttName ());

   mergeBoundVecs (hardLBMrg, theBoundSet->hardLB (), hardLBArg);
   mergeBoundVecs (softLBMrg, theBoundSet->softLB (), softLBArg);
   mergeBoundVecs (hardUBMrg, theBoundSet->hardUB (), hardUBArg);

      // Check for errors.
      //
   forEachPeriod (thePer, myProblem ())
      if (WitBoundSet::seqError (hardLBMrg, softLBMrg, hardUBMrg, thePer))
         {
         errorFound (thePer);

         myMsgFac () ("boundSetRangeSmsg", 
            hardLBMrg[thePer],
            softLBMrg[thePer],
            hardUBMrg[thePer]);
         }

   if (not argsOK ())
      return;

   if (myMsgFac ().mayIssue ("setBoundVecAttrMsg"))
      {
      hardLBArg.issueBoundVecMsg (theBoundSet, "hardLowerBoundFrag", * this);
      softLBArg.issueBoundVecMsg (theBoundSet, "softLowerBoundFrag", * this);
      hardUBArg.issueBoundVecMsg (theBoundSet, "hardUpperBoundFrag", * this);
      }

   theBoundSet->set (hardLBMrg, softLBMrg, hardUBMrg);
   }

//------------------------------------------------------------------------------

void WitApiCall::reqNoPartsOrOpns (const char * theAttName)
   {
   if (myCompMgr ()->nNodes () == 0)
      return;

   myInputID ()->setMyAttName (theAttName);

   myMsgFac () ("noPartsOrOpnsRequiredSmsg",
      theAttName,
      myCompMgr ()->nParts (),
      myCompMgr ()->nOperations ());
   }

//------------------------------------------------------------------------------

void WitApiCall::issueSetDblVecMsg (
      const char *  theAttName,
      const float * theFltCVec)
   {
   if (myMsgFac ().mayIssue ("setVecDblAttrMsg"))
      {
      WitMsgArg theMsgArg (theFltCVec, nPeriods ());

      myMsgFac () ("setVecDblAttrMsg", theAttName, theMsgArg);
      }
   }

//------------------------------------------------------------------------------

void WitApiCall::issueSetDblVecMsg (
      const char *   theAttName,
      const double * theDblCVec)
   {
   if (myMsgFac ().mayIssue ("setVecDblAttrMsg"))
      {
      WitMsgArg theMsgArg (theDblCVec, nPeriods ());

      myMsgFac () ("setVecDblAttrMsg", theAttName, theMsgArg);
      }
   }

//------------------------------------------------------------------------------

void WitApiCall::issueBoundVecMsg (
      WitBoundSet * theBoundSet,
      const float * theFltCVec,
      WitMsgFragID  theFragID)
   {
   witAssert (myMsgFac ().mayIssue ("setBoundVecAttrMsg"));

   if (theFltCVec == NULL)
      return;

   WitMsgArg theMsgArg (theFltCVec, nPeriods ());

   myMsgFac () (
      "setBoundVecAttrMsg",
      theBoundSet->myAttName (),
      myMsgFac ().myFrag (theFragID),
      theMsgArg);
   }

//------------------------------------------------------------------------------

void WitApiCall::issueBoundVecMsg (
      WitBoundSet *  theBoundSet,
      const double * theDblCVec,
      WitMsgFragID   theFragID)
   {
   witAssert (myMsgFac ().mayIssue ("setBoundVecAttrMsg"));

   if (theDblCVec == NULL)
      return;

   WitMsgArg theMsgArg (theDblCVec, nPeriods ());

   myMsgFac () (
      "setBoundVecAttrMsg",
      theBoundSet->myAttName (),
      myMsgFac ().myFrag (theFragID),
      theMsgArg);
   }

//------------------------------------------------------------------------------

WitApiCall::WitApiCall (WitRun * theWitRun):

      WitProbAssoc   (theProblemFor (theWitRun)),
      argsOK_        (true),
      prepSetCalled_ (false)
   {
   }

//------------------------------------------------------------------------------

WitApiCall::~WitApiCall ()
   {
   myInputID ()->setMyAttName   (NULL);
   myInputID ()->setErrorPeriod (-1);
   }

//------------------------------------------------------------------------------

WitDelComp * WitApiCall::myDelComp ()
   {
   stronglyAssert (false);

   return NULL;
   }

//------------------------------------------------------------------------------

WitBillEntry * WitApiCall::myBillEntry ()
   {
   stronglyAssert (false);

   return NULL;
   }

//------------------------------------------------------------------------------

WitConsEntry * WitApiCall::myConsEntry ()
   {
   stronglyAssert (false);

   return NULL;
   }

//------------------------------------------------------------------------------

void WitApiCall::forbidNullSetArg (const void * theArg)
   {
   if (theArg == NULL)
      myMsgFac () ("nullSetArgSmsg");
   }

//------------------------------------------------------------------------------

void WitApiCall::errorFound (WitPeriod thePer)
   {
   errorFound ();

   myInputID ()->setErrorPeriod (thePer);
   }

//------------------------------------------------------------------------------

void WitApiCall::errorFound ()
   {
   argsOK_ = false;
   }

//------------------------------------------------------------------------------

void WitApiCall::prepSetAttr (const char * theAttName)
   {
   if (DEBUG_MODE)
      prepSetCalled_ = true;

   myInputID ()->setMyAttName (theAttName);

   if (myStochImpMgr ()->stochMode ())
      prepSetAttrStochMode (theAttName);
   }

//------------------------------------------------------------------------------

void WitApiCall::prepGetPip (int rcpPeriod, const char * theArgName)
   {
   if (not ((rcpPeriod >= 0) and (rcpPeriod < nPeriods ())))
      {
      myInputID ()->setArgName (theArgName);

      myMsgFac () ("argRangeIntLeValLtIntSmsg", rcpPeriod, 0, nPeriods ());
      }

   if (not myProblem ()->myPipMgr ()->peggingExists ())
      myMsgFac () ("needPipExistsSmsg");
   }

//------------------------------------------------------------------------------

WitProblem * WitApiCall::theProblemFor (WitRun * theWitRun)
   {
   theWitRun->myApiMgr ()->startInnerFunc ();

   return theWitRun->myProblem ();
   }

//------------------------------------------------------------------------------

void WitApiCall::checkPeriodIntVec (const int * theValue)
   {
   WitPeriod thePer;

   forbidNullSetArg (theValue);

   forEachPeriod (thePer, myProblem ())
      if ((theValue[thePer] < 0) or (theValue[thePer] > lastPeriod ()))
         {
         errorFound (thePer);

         myMsgFac () ("intLowEqUpEqSmsg", theValue[thePer], 0, lastPeriod ());
         }
   }

//------------------------------------------------------------------------------

void WitApiCall::mergeBoundVecs (
      WitDblFlexVec &       mergedVec,
      const WitDblFlexVec & oldVec,
      const WitIVRealArg &  theIVRealArg)
   {
   if (theIVRealArg != NULL)
      theIVRealArg.copyInto (mergedVec);
   else
      mergedVec = oldVec;
   }

//------------------------------------------------------------------------------

void WitApiCall::prepSetAttrStochMode (const char * theAttName)
   {
   WitDelComp * theDelComp;

   stronglyAssert (myStochImpMgr ()->stochMode ());

   theDelComp = myComponent ()->thisDelComp ();

   if (not myStochImpMgr ()->canSetInStochMode (theAttName))
      {
      if (theDelComp != NULL)
         myMsgFac () ("setObjStochModeSmsg",
            theAttName,
            theDelComp->objTypeMsgFrag ());
      else
         myMsgFac () ("setGlobStochModeSmsg", theAttName);
      }

   if (myStochImpMgr ()->stochSolnMode ())
      if (not myStochImpMgr ()->canSetInStochSolnMode (theAttName))
         {
         if (theDelComp != NULL)
            myMsgFac () ("setObjStochSolnSmsg",
               theAttName,
               theDelComp->objTypeMsgFrag ());
         else
            myMsgFac () ("setGlobStochSolnSmsg", theAttName);
         }
   }

//------------------------------------------------------------------------------
// Implementation of class GlobalCompApiCall.
//------------------------------------------------------------------------------

WitGlobalCompApiCall::WitGlobalCompApiCall (WitRun * theWitRun):
      WitApiCall (theWitRun)
   {
   }

//------------------------------------------------------------------------------

WitGlobalCompApiCall::~WitGlobalCompApiCall ()
   {
   }

//------------------------------------------------------------------------------

WitComponent * WitGlobalCompApiCall::myComponent ()
   {
   return myGlobalComp ();
   }

//------------------------------------------------------------------------------
// Implementation of class OptCompApiCall.
//------------------------------------------------------------------------------

WitOptCompApiCall::WitOptCompApiCall (WitRun * theWitRun):
      WitApiCall (theWitRun)
   {
   }

//------------------------------------------------------------------------------

WitOptCompApiCall::~WitOptCompApiCall ()
   {
   }

//------------------------------------------------------------------------------

void WitOptCompApiCall::reqMultiObjMode (const char * theAttName)
   {
   if (not myOptComp ()->multiObjMode ())
      myMsgFac () ("globNeedsMultiObjModeSmsg", theAttName);
   }

//------------------------------------------------------------------------------

WitMultiObjMgr * WitOptCompApiCall::myMultiObjMgr ()
   {
   return myOptComp ()->myMultiObjMgr ();
   }

//------------------------------------------------------------------------------

WitObjective * WitOptCompApiCall::currentObjective ()
   {
   return myMultiObjMgr ()->currentObjective ();
   }

//------------------------------------------------------------------------------

WitCpxParSpecMgr * WitOptCompApiCall::myCpxParSpecMgr ()
   {
   return myOptComp ()->myCpxParSpecMgr ();
   }

//------------------------------------------------------------------------------

WitComponent * WitOptCompApiCall::myComponent ()
   {
   return myOptComp ();
   }

//------------------------------------------------------------------------------
// Implementation of class StochApiCall.
//------------------------------------------------------------------------------

WitStochApiCall::WitStochApiCall (WitRun * theWitRun):
      WitApiCall (theWitRun)
   {
   }

//------------------------------------------------------------------------------

WitStochApiCall::~WitStochApiCall ()
   {
   }

//------------------------------------------------------------------------------

void WitStochApiCall::setPeriodStage (const int * periodStage)
   {
   WitPeriod thePer;

   prepSetAttr ("periodStage");

   if (not myStochImpMgr ()->stochMode ())
      myMsgFac () ("globNeedsStochModeSmsg", "periodStage");

   if (myStochImpMgr ()->stageByObject ())
      myMsgFac () ("periodStageWStageByObjectSmsg");

   forbidNullSetArg (periodStage);

   forEachPeriod (thePer, myProblem ())
      if    (periodStage[thePer] != 0)
         if (periodStage[thePer] != 1)
            {
            myInputID ()->setErrorPeriod (thePer);

            myMsgFac () ("intLowEqUpEqImmSmsg", periodStage[thePer], 0, 1);
            }

   for (thePer = 1; thePer < nPeriods (); thePer ++)
      if    (periodStage[thePer - 1] == 1)
         if (periodStage[thePer    ] == 0)
            myMsgFac () ("perStageConflictSmsg", thePer - 1, thePer);

   if (argsOK ())
      {
      if (myMsgFac ().mayIssue ("setVecIntAttrMsg"))
         {
         WitMsgArg theMsgArg (periodStage, nPeriods ());

         myMsgFac () ("setVecIntAttrMsg", "periodStage", theMsgArg);
         }

      forEachPeriod (thePer, myProblem ())
         myStochImpMgr ()->setPeriodStageIdx (thePer, periodStage[thePer]);
      }
   }

//------------------------------------------------------------------------------

WitComponent * WitStochApiCall::myComponent ()
   {
   return myGlobalComp ();
   }

//------------------------------------------------------------------------------
// Implementation of class PartApiCall.
//------------------------------------------------------------------------------

WitPartApiCall::WitPartApiCall (WitRun * theWitRun, const char * partName):
      WitApiCall (theWitRun),

      myPart_    (theWitRun->setGetPartInit (partName))
   {
   }

//------------------------------------------------------------------------------

WitPartApiCall::~WitPartApiCall ()
   {
   myInputID ()->setMyPart (NULL);
   }

//------------------------------------------------------------------------------

WitComponent * WitPartApiCall::myComponent ()
   {
   return myPart_;
   }

//------------------------------------------------------------------------------

WitDelComp * WitPartApiCall::myDelComp ()
   {
   return myPart_;
   }

//------------------------------------------------------------------------------
// Implementation of class MaterialApiCall.
//------------------------------------------------------------------------------

WitMaterialApiCall::WitMaterialApiCall (
         WitRun *     theWitRun, 
         const char * partName,
         const char * theAttName):

      WitApiCall  (theWitRun),

      myMaterial_ (theWitRun->setGetMaterialInit (partName, theAttName))
   {
   }

//------------------------------------------------------------------------------

WitMaterialApiCall::~WitMaterialApiCall ()
   {
   myInputID ()->setMyPart (NULL);
   }

//------------------------------------------------------------------------------

void WitMaterialApiCall::prepBa (
      const char * theAttName,
      bool         oldValue,
      WitBoolean   theValue,
      const char * otherAttr)
   {
   prepBool (theAttName, oldValue, theValue);

   if (not oldValue)
      if (theValue)
         if (myMaterial_->buildNstn () or myMaterial_->buildAsap ())
            if (myMsgFac ().mayIssue ("setBooleanAttrMsg"))
               myMsgFac () ("setBooleanAttrMsg", otherAttr, true, false);
   }

//------------------------------------------------------------------------------

WitComponent * WitMaterialApiCall::myComponent ()
   {
   return myMaterial_;
   }

//------------------------------------------------------------------------------

WitDelComp * WitMaterialApiCall::myDelComp ()
   {
   return myMaterial_;
   }

//------------------------------------------------------------------------------
// Implementation of class DemandApiCall.
//------------------------------------------------------------------------------

WitDemandApiCall::WitDemandApiCall (
         WitRun *     theWitRun,
         const char * demandedPartName,
         const char * demandName):

      WitApiCall (theWitRun),

      myDemand_  (theWitRun->setGetDemandInit (demandedPartName, demandName))
   {
   }

//------------------------------------------------------------------------------

WitDemandApiCall::~WitDemandApiCall ()
   {
   myInputID ()->setMyPart   (NULL);
   myInputID ()->setMyDemand (NULL);
   }

//------------------------------------------------------------------------------

void WitDemandApiCall::prepGetPip (int shipPeriod)
   {
   if (myMsgFac ().mayIssue ("shipPerMsg"))
      myMsgFac () ("shipPerMsg", shipPeriod);

   WitApiCall::prepGetPip (shipPeriod, "shipPeriod");
   }

//------------------------------------------------------------------------------

WitComponent * WitDemandApiCall::myComponent ()
   {
   return myDemand_;
   }

//------------------------------------------------------------------------------

WitDelComp * WitDemandApiCall::myDelComp ()
   {
   return myDemand_;
   }

//------------------------------------------------------------------------------
// Implementation of class OperationApiCall.
//------------------------------------------------------------------------------

WitOperationApiCall::WitOperationApiCall (
         WitRun *     theWitRun,
         const char * operationName):

      WitApiCall   (theWitRun),

      myOperation_ (theWitRun->setGetOperationInit (operationName))
   {
   }

//------------------------------------------------------------------------------

WitOperationApiCall::~WitOperationApiCall ()
   {
   myInputID ()->setMyOperation (NULL);
   }

//------------------------------------------------------------------------------

void WitOperationApiCall::setYieldRate (const WitIVRealArg & theValue)
   {
   WitPeriod thePer;

   prepSetAttr ("yieldRate");

   forbidNullSetArg (theValue);

   forEachPeriod (thePer, myProblem ())
      if (not WitOperation::validYieldRate (theValue[thePer]))
         {
         errorFound (thePer);

         myMsgFac () ("yieldRateRangeSmsg", theValue[thePer]);
         }

   if (argsOK ())
      {
      theValue.issueSetDblVecMsg ("yieldRate", * this);

      myOperation_->setYieldRate (theValue);
      }
   }

//------------------------------------------------------------------------------

void WitOperationApiCall::setCycleTime34 (const WitIVRealArg & theValue)
   {
   prepSetAttr ("cycleTime34");

   forbidNullSetArg (theValue);

   if (not wit34Compatible ())
      myMsgFac () ("settingCycleTime34WithoutCompatibility",
         myApiMgr ()->apiFuncName ());

   if (argsOK ())
      myOperation_->setCycleTime34 (theValue);
   }

//------------------------------------------------------------------------------

void WitOperationApiCall::prepGetPip (int execPeriod)
   {
   if (myMsgFac ().mayIssue ("execPerMsg"))
      myMsgFac () ("execPerMsg", execPeriod);

   if (not myOperation_->pipEnabled ())
      myMsgFac () ("pipNotEnabledSmsg");

   WitApiCall::prepGetPip (execPeriod, "execPeriod");
   }

//------------------------------------------------------------------------------

WitComponent * WitOperationApiCall::myComponent ()
   {
   return myOperation_;
   }

//------------------------------------------------------------------------------

WitDelComp * WitOperationApiCall::myDelComp ()
   {
   return myOperation_;
   }

//------------------------------------------------------------------------------
// Implementation of class BomEntryApiCall.
//------------------------------------------------------------------------------

WitBomEntryApiCall::WitBomEntryApiCall (
         WitRun *     theWitRun,
         const char * consumingOperationName,
         int          bomEntryIndex):

      WitApiCall (theWitRun),

      myBomEnt_  (NULL)
   {
   myBomEnt_ =
      theWitRun->setGetBomEntryInit (consumingOperationName, bomEntryIndex);
   }

//------------------------------------------------------------------------------

WitBomEntryApiCall::~WitBomEntryApiCall ()
   {
   myInputID ()->setMyOperation (NULL);
   myInputID ()->setMyBomEntry  (NULL);
   }

//------------------------------------------------------------------------------

WitComponent * WitBomEntryApiCall::myComponent ()
   {
   return myBomEnt_;
   }

//------------------------------------------------------------------------------

WitDelComp * WitBomEntryApiCall::myDelComp ()
   {
   return myBomEnt_;
   }

//------------------------------------------------------------------------------

WitBillEntry * WitBomEntryApiCall::myBillEntry ()
   {
   return myBomEnt_;
   }

//------------------------------------------------------------------------------

WitConsEntry * WitBomEntryApiCall::myConsEntry ()
   {
   return myBomEnt_;
   }

//------------------------------------------------------------------------------
// Implementation of class SubEntryApiCall.
//------------------------------------------------------------------------------

WitSubEntryApiCall::WitSubEntryApiCall (
         WitRun *     theWitRun,
         const char * consumingOperationName,
         int          bomEntryIndex,
         int          subEntryIndex):

      WitApiCall (theWitRun),

      mySub_     (NULL)
   {
   mySub_ =
      theWitRun->setGetSubBomEntryInit (
         consumingOperationName,
         bomEntryIndex,
         subEntryIndex);
   }

//------------------------------------------------------------------------------

WitSubEntryApiCall::~WitSubEntryApiCall ()
   {
   myInputID ()->setMyOperation (NULL);
   myInputID ()->setMyBomEntry  (NULL);
   myInputID ()->setMySubEntry  (NULL);
   }

//------------------------------------------------------------------------------

WitComponent * WitSubEntryApiCall::myComponent ()
   {
   return mySub_;
   }

//------------------------------------------------------------------------------

WitDelComp * WitSubEntryApiCall::myDelComp ()
   {
   return mySub_;
   }

//------------------------------------------------------------------------------

WitBillEntry * WitSubEntryApiCall::myBillEntry ()
   {
   return mySub_;
   }

//------------------------------------------------------------------------------

WitConsEntry * WitSubEntryApiCall::myConsEntry ()
   {
   return mySub_;
   }

//------------------------------------------------------------------------------
// Implementation of class BopEntryApiCall.
//------------------------------------------------------------------------------

WitBopEntryApiCall::WitBopEntryApiCall (
         WitRun *     theWitRun,
         const char * producingOperationName,
         int          bopEntryIndex):

      WitApiCall (theWitRun),

      myBopEnt_  (NULL)
   {
   myBopEnt_ =
      theWitRun->setGetBopEntryInit (producingOperationName, bopEntryIndex);
   }

//------------------------------------------------------------------------------

WitBopEntryApiCall::~WitBopEntryApiCall ()
   {
   myInputID ()->setMyOperation (NULL);
   myInputID ()->setMyBopEntry  (NULL);
   }

//------------------------------------------------------------------------------

WitComponent * WitBopEntryApiCall::myComponent ()
   {
   return myBopEnt_;
   }

//------------------------------------------------------------------------------

WitDelComp * WitBopEntryApiCall::myDelComp ()
   {
   return myBopEnt_;
   }

//------------------------------------------------------------------------------

WitBillEntry * WitBopEntryApiCall::myBillEntry ()
   {
   return myBopEnt_;
   }
