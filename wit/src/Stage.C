//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "Stage.C"
//
// Contains the implementation of the following classes:
//
//    Stage
//    StageMgr
//    ObjStageMgr
//    PerStageMgr
//------------------------------------------------------------------------------

#include <Stage.h>
#include <ObjStageMgr.h>
#include <PerStageMgr.h>
#include <StochImpMgr.h>
#include <StochModeMgr.h>
#include <DataWrit.h>
#include <GlobalComp.h>
#include <Material.h>
#include <Demand.h>
#include <Operation.h>
#include <SubEntry.h>
#include <BopEntry.h>
#include <BoundSet.h>
#include <CompMgr.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class Stage.
//------------------------------------------------------------------------------

WitStage::WitStage (int theIdx, WitStochModeMgr * theStochModeMgr):

      WitStochAssoc (theStochModeMgr),
      myIndex_      (theIdx)
   {
   }

//------------------------------------------------------------------------------

WitStage::~WitStage ()
   {
   }

//------------------------------------------------------------------------------

bool WitStage::includes (WitNode * theNode)
   {
   return myStageMgr ()->includesNode (this, theNode);
   }

//------------------------------------------------------------------------------

bool WitStage::includes (WitPeriod thePer)
   {
   return myStageMgr ()->includesPeriod (this, thePer);
   }

//------------------------------------------------------------------------------
// Implementation of class StageMgr.
//------------------------------------------------------------------------------

WitStageMgr::~WitStageMgr ()
   {
   delete myStageVec_[1];
   delete myStageVec_[0];
   }

//------------------------------------------------------------------------------

WitStageMgr * WitStageMgr::newStageMgr (WitStochModeMgr * theStochModeMgr)
   {
   if (theStochModeMgr->myStochImpMgr ()->stageByObject ())
      return new WitObjStageMgr (theStochModeMgr);
   else
      return new WitPerStageMgr (theStochModeMgr);
   }

//------------------------------------------------------------------------------

WitStageMgr::WitStageMgr (WitStochModeMgr * theStochModeMgr):

      WitStochAssoc (theStochModeMgr),
      myStageVec_   (2)
   {
   myStageVec_[0] = new WitStage (0, myStochModeMgr ());
   myStageVec_[1] = new WitStage (1, myStochModeMgr ());
   }

//------------------------------------------------------------------------------
// Implementation of class ObjStageMgr.
//------------------------------------------------------------------------------

WitObjStageMgr::WitObjStageMgr (WitStochModeMgr * theStochModeMgr):

      WitStageMgr  (theStochModeMgr),
      objectStage_ (myProblem ())
   {
   WitPart *      thePart;
   WitOperation * theOpn;

   stronglyAssert (myStochImpMgr ()->stageByObject ());

   forEachPart (thePart, myProblem ())
      objectStage_ (thePart) = defStage ();

   forEachOperation (theOpn, myProblem ())
      objectStage_ (theOpn)  = defStage ();
   }

//------------------------------------------------------------------------------

WitObjStageMgr::~WitObjStageMgr ()
   {
   }

//------------------------------------------------------------------------------

void WitObjStageMgr::setObjectStageIdx (WitNode * theNode, int theStageIdx)
   {
   myProblem ()->prepSetStochModeAttr ("objectStage");

   objectStage_ (theNode) = myStageFor (theStageIdx);
   }

//------------------------------------------------------------------------------

void WitObjStageMgr::setPeriodStageIdx (WitPeriod thePer, int theStageIdx)
   {
   stronglyAssert (false);
   }

//------------------------------------------------------------------------------

WitStage * WitObjStageMgr::objectStage (WitNode * theNode)
   {
   return objectStage_ (theNode);
   }

//------------------------------------------------------------------------------

WitStage * WitObjStageMgr::periodStage (WitPeriod)
   {
   stronglyAssert (false);

   return NULL;
   }

//------------------------------------------------------------------------------

WitStage * WitObjStageMgr::myStageOf (WitNode * theNode, WitPeriod)
   {
   return objectStage_ (theNode);
   }

//------------------------------------------------------------------------------

bool WitObjStageMgr::includesNode (WitStage * theStage, WitNode * theNode)
   {
   return (theStage == objectStage_ (theNode));
   }

//------------------------------------------------------------------------------

bool WitObjStageMgr::includesPeriod (WitStage *, WitPeriod)
   {
   return true;
   }

//------------------------------------------------------------------------------

void WitObjStageMgr::writeData ()
   {
   WitPart *      thePart;
   WitOperation * theOpn;

   forEachPart (thePart, myProblem ())
      writeObjectStage (thePart, "part");

   forEachOperation (theOpn, myProblem ())
      writeObjectStage (theOpn, "operation");
   }

//------------------------------------------------------------------------------

void WitObjStageMgr::displayData ()
   {
   WitPart *      thePart;
   WitOperation * theOpn;

   myMsgFac () ("stageHeadingDdMsg");

   myMsgFac () ("partStageHeadingDdMsg");

   forEachPart (thePart, myProblem ())
      myMsgFac () ("objectStageDdMsg",
         thePart->partName (),
         objectStage_ (thePart)->myIndex ());

   myMsgFac () ("opnStageHeadingDdMsg");

   forEachOperation (theOpn, myProblem ())
      myMsgFac () ("objectStageDdMsg",
         theOpn->operationName (),
         objectStage_ (thePart)->myIndex ());
   }

//------------------------------------------------------------------------------

void WitObjStageMgr::copyAttrsFrom (WitStageMgr * origStageMgr)
   {
   WitCompMgr *   origCompMgr;
   WitPart *      thePart;
   WitPart *      origPart;
   int            theIdx;
   WitOperation * theOpn;
   WitOperation * origOpn;
   
   origCompMgr = origStageMgr->myCompMgr ();

   forEachPart (thePart, myProblem ())
      {
      origPart = origCompMgr->findPart (thePart->partName ().myCstring ());

      theIdx   = origStageMgr->objectStage (origPart)->myIndex ();

      objectStage_ (thePart) = myStageFor (theIdx);
      }

   forEachOperation (theOpn, myProblem ())
      {
      origOpn =
         origCompMgr->findOperation (theOpn->operationName ().myCstring ());

      theIdx  = origStageMgr->objectStage (origOpn)->myIndex ();

      objectStage_ (theOpn) = myStageFor (theIdx);
      }
   }

//------------------------------------------------------------------------------

void WitObjStageMgr::validateData ()
   {
   validateBomEnts ();
   validateSubs    ();
   validateBopEnts ();
   }

//------------------------------------------------------------------------------

void WitObjStageMgr::issueAttMisMatchHeadMsg ()
   {
   myMsgFac () ("stage0ObjAttMisMatchSmsg");
   }

//------------------------------------------------------------------------------

void WitObjStageMgr::writeObjectStage (
      WitNode *    theNode,
      const char * nodeTypeName)
   {
   if (objectStage_ (theNode) == defStage ())
      return;

   myDataWriter ()->writeFirstKeyWord ("set");
   myDataWriter ()->writeKeyWord      (nodeTypeName);
   myDataWriter ()->writeStringValue  (theNode->nodeName ());

   myDataWriter ()->writeInt (
        "objectStage",
         objectStage_ (theNode)->myIndex (),
            defStage  ()       ->myIndex ());

   myDataWriter ()->writeEndCommand ();
   }

//------------------------------------------------------------------------------

void WitObjStageMgr::validateBomEnts ()
   {
   WitBomEntry * theBomEnt;
   WitStage *    theOpnStage;
   WitStage *    thePartStage;

   forEachBomEntry (theBomEnt, myProblem ())
      {
      theOpnStage  = objectStage_ (theBomEnt->myOperation ());
      thePartStage = objectStage_ (theBomEnt->myPart      ());

      if    (theOpnStage ->isStage1 ())
         if (thePartStage->isStage0 ())
            myMsgFac () ("bomEntStageConflictSmsg",
               theBomEnt->myOperationName (),
               theBomEnt->localIndex (),
               theBomEnt->myPartName ());
      }
   }

//------------------------------------------------------------------------------

void WitObjStageMgr::validateSubs ()
   {
   WitSubEntry * theSub;
   WitStage *    theOpnStage;
   WitStage *    thePartStage;

   forEachSubEntry (theSub, myProblem ())
      {
      theOpnStage  = objectStage_ (theSub->myOperation ());
      thePartStage = objectStage_ (theSub->myPart      ());

      if    (theOpnStage ->isStage1 ())
         if (thePartStage->isStage0 ())
            myMsgFac () ("subStageConflictSmsg",
               theSub->myOperationName (),
               theSub->myBomEntIndex (),
               theSub->localIndex (),
               theSub->myPartName ());
      }
   }

//------------------------------------------------------------------------------

void WitObjStageMgr::validateBopEnts ()
   {
   WitBopEntry * theBopEnt;
   WitStage *    theOpnStage;
   WitStage *    thePartStage;

   forEachBopEntry (theBopEnt, myProblem ())
      {
      theOpnStage  = objectStage_ (theBopEnt->myOperation ());
      thePartStage = objectStage_ (theBopEnt->myPart      ());

      if    (theOpnStage ->isStage1 ())
         if (thePartStage->isStage0 ())
            myMsgFac () ("bopEntStageConflictSmsg",
               theBopEnt->myOperationName (),
               theBopEnt->localIndex (),
               theBopEnt->myPartName ());
      }
   }

//------------------------------------------------------------------------------
// Implementation of class PerStageMgr.
//------------------------------------------------------------------------------

WitPerStageMgr::WitPerStageMgr (WitStochModeMgr * theStochModeMgr):

      WitStageMgr  (theStochModeMgr),
      periodStage_ (myProblem ())
   {
   WitPeriod thePer;

   stronglyAssert (not myStochImpMgr ()->stageByObject ());

   forEachPeriod (thePer, myProblem ())
      periodStage_[thePer] = defStage ();

   validateCoreData ();
   }

//------------------------------------------------------------------------------

WitPerStageMgr::~WitPerStageMgr ()
   {
   }

//------------------------------------------------------------------------------

void WitPerStageMgr::setObjectStageIdx (WitNode * theNode, int theStageIdx)
   {
   stronglyAssert (false);
   }

//------------------------------------------------------------------------------

void WitPerStageMgr::setPeriodStageIdx (WitPeriod thePer, int theStageIdx)
   {
   myProblem ()->prepSetStochModeAttr ("periodStage");

   periodStage_[thePer] = myStageFor (theStageIdx);
   }

//------------------------------------------------------------------------------

WitStage * WitPerStageMgr::objectStage (WitNode *)
   {
   stronglyAssert (false);

   return NULL;
   }

//------------------------------------------------------------------------------

WitStage * WitPerStageMgr::periodStage (WitPeriod thePer)
   {
   return periodStage_[thePer];
   }

//------------------------------------------------------------------------------

WitStage * WitPerStageMgr::myStageOf (WitNode *, WitPeriod thePer)
   {
   return periodStage_[thePer];
   }

//------------------------------------------------------------------------------

bool WitPerStageMgr::includesNode (WitStage *, WitNode *)
   {
   return true;
   }

//------------------------------------------------------------------------------

bool WitPerStageMgr::includesPeriod (WitStage * theStage, WitPeriod thePer)
   {
   return (theStage == periodStage_[thePer]);
   }

//------------------------------------------------------------------------------

void WitPerStageMgr::writeData ()
   {
   WitTVec <int> theIdxVec;
   WitPeriod     thePer;

   theIdxVec.allocate (myProblem (), -1);

   forEachPeriod (thePer, myProblem ())
      theIdxVec[thePer] = periodStage_[thePer]->myIndex ();

   myDataWriter ()->writeFirstKeyWord ("set");
   myDataWriter ()->writeKeyWord      ("problem");

   myDataWriter ()->writeVector (
      "periodStage",
       theIdxVec,
       defStage ()->myIndex ());

   myDataWriter ()->writeEndCommand ();
   }

//------------------------------------------------------------------------------

void WitPerStageMgr::displayData ()
   {
   WitPeriod thePer;

   myMsgFac () ("stageHeadingDdMsg");
   myMsgFac () ("periodStageHeadingDdMsg");

   forEachPeriod (thePer, myProblem ())
      myMsgFac () ("periodStageDdMsg",
         thePer,
         periodStage_[thePer]->myIndex ());
   }

//------------------------------------------------------------------------------

void WitPerStageMgr::copyAttrsFrom (WitStageMgr * origStageMgr)
   {
   WitPeriod thePer;
   int       theIdx;

   forEachPeriod (thePer, myProblem ())
      {
      theIdx = origStageMgr->periodStage (thePer)->myIndex ();

      periodStage_[thePer] = myStageFor (theIdx);
      }
   }

//------------------------------------------------------------------------------

void WitPerStageMgr::validateData ()
   {
   WitPeriod thePer;

   if (DEBUG_MODE)
      forEachPeriod (thePer, myProblem ())
         if (thePer > 0)
            if           (periodStage_[thePer    ]->isStage0 ())
               witAssert (periodStage_[thePer - 1]->isStage0 ());
   }

//------------------------------------------------------------------------------

void WitPerStageMgr::issueAttMisMatchHeadMsg ()
   {
   myMsgFac () ("stage0PerAttMisMatchSmsg");
   }

//------------------------------------------------------------------------------

void WitPerStageMgr::validateCoreData ()
   {
   validateBomEnts ();
   validateSubs    ();
   validateBopEnts ();
   }

//------------------------------------------------------------------------------

void WitPerStageMgr::validateBomEnts ()
   {
   WitBomEntry * theBomEnt;
   WitPeriod     execPer;

   forEachBomEntry (theBomEnt, myProblem ())
      forEachPeriod (execPer, myProblem ())
         {
         if (theBomEnt->offset ()[execPer] > 0.0)
            myMsgFac () ("posBomOffsetInSBPSmsg",
               theBomEnt->myOperationName (),
               theBomEnt->localIndex (),
               theBomEnt->myPartName (),
               execPer,
               theBomEnt->offset ()[execPer]);

         checkImpactPeriod (theBomEnt, execPer);
         }
   }

//------------------------------------------------------------------------------

void WitPerStageMgr::validateSubs ()
   {
   WitSubEntry * theSub;
   WitPeriod     execPer;

   if (not myGlobalComp ()->independentOffsets ())
      return;

   forEachSubEntry (theSub, myProblem ())
      forEachPeriod (execPer, myProblem ())
         {
         if (theSub->offset ()[execPer] > 0.0)
            myMsgFac () ("posSubOffsetInSBPSmsg",
               theSub->myOperationName (),
               theSub->myBomEntIndex (),
               theSub->localIndex (),
               theSub->myPartName (),
               execPer,
               theSub->offset ()[execPer]);

         checkImpactPeriod (theSub, execPer);
         }
   }

//------------------------------------------------------------------------------

void WitPerStageMgr::validateBopEnts ()
   {
   WitBopEntry * theBopEnt;
   WitPeriod     execPer;

   forEachBopEntry (theBopEnt, myProblem ())
      forEachPeriod (execPer, myProblem ())
         {
         if (theBopEnt->offset ()[execPer] > 0.0)
            myMsgFac () ("posBopOffsetInSBPSmsg",
               theBopEnt->myOperationName (),
               theBopEnt->localIndex (),
               theBopEnt->myPartName (),
               execPer,
               theBopEnt->offset ()[execPer]);

         checkImpactPeriod (theBopEnt, execPer);
         }
   }

//------------------------------------------------------------------------------

void WitPerStageMgr::checkImpactPeriod (
      WitBillEntry * theBillEnt,
      WitPeriod      execPer)
   {
   if (theBillEnt->inEffect (execPer))
      stronglyAssert (theBillEnt->impactPeriod ()[execPer] >= execPer);
   }
