//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "StochOpt.C"
//
// Contains the implementation of the following classes:
//
//    StochOptMgr
//    StochImpOP
//    ScenGroup
//    StochLoc
//    StochVar
//    StochCon
//------------------------------------------------------------------------------

#include <StochOptMgr.h>
#include <StochLoc.h>
#include <StVars.h>
#include <StCons.h>
#include <Stage.h>
#include <StageMgr.h>
#include <ScenMgr.h>
#include <Scenario.h>
#include <ScenAttMgr.h>
#include <ScenInputAtt.h>
#include <StochImpOP.h>
#include <StochImpMgr.h>
#include <OptStarter.h>
#include <Pre.h>
#include <Post.h>
#include <wit/src/Variant.h>
#include <OptComp.h>
#include <Material.h>
#include <Operation.h>
#include <Demand.h>
#include <BomEntry.h>
#include <SubEntry.h>
#include <BoundSet.h>
#include <ExecPerSch.h>
#include <PtrMap.h>
#include <Timing.h>
#include <MsgFrag.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class StochOptMgr.
//------------------------------------------------------------------------------

WitStochOptMgr::WitStochOptMgr (WitStochModeMgr * theStochModeMgr):

      WitStochAssoc  (theStochModeMgr),
      myStochImpOP_  (NULL),
      uploadingSoln_ (false)
   {
   }

//------------------------------------------------------------------------------

WitStochOptMgr::~WitStochOptMgr ()
   {
   }

//------------------------------------------------------------------------------

void WitStochOptMgr::implode ()
   {
   WitTimer::enterSection ("opt-prob");

   startStochImp ();

   myStochImpOP_->solve ();

   finishStochImp ();

   WitTimer::leaveSection ("opt-prob");
   }

//------------------------------------------------------------------------------

void WitStochOptMgr::startStochImp ()
   {
   stronglyAssert (not stochSolnMode ());

   myMsgFac () ("stochImpMsg");

   myOptComp    ()->requireCplex  ();
   myScenAttMgr ()->downloadInput ();
   myStageMgr   ()->validateData  ();
   myScenMgr    ()->validateData  ();
   myScenAttMgr ()->validateData  ();

   myProblem      ()->resetSoln ();

   myPreprocessor ()->preprocess ();

   myOptComp      ()->optInitMethod ()->display ();

   myMsgFac () ("genLpMipProbMsg", myMsgFac ().myFrag ("stochLpFrag"));

   myStochImpOP_ = new WitStochImpOP (myStochModeMgr ());

   myStochImpOP_->generateFixed ();

   myStochImpOP_->generateFlexible ();
   }

//------------------------------------------------------------------------------

void WitStochOptMgr::finishStochImp ()
   {
   myProblem ()->optVariant ()->becomesCurrent ();

   uploadObjValues ();

   uploadScenSoln ();

   delete myStochImpOP_;

   myStochImpOP_ = NULL;

   myStochModeMgr ()->uploadSoln ();

   myPostprocessor ()->postprocess ();
   }

//------------------------------------------------------------------------------

void WitStochOptMgr::uploadObjValues ()
   {
   double        objValue;
   double        boundsValue;
   WitOptVar *   theOptVar;
   WitStochVar * theStochVar;

   uploadingSoln_ = true;

   objValue       = myStochImpOP_->compObjValue ();

   myStochModeMgr ()->passObjValue (objValue);

   boundsValue    = 0.0;

   forEachEl (theOptVar, myStochImpOP_->myOptVars ())
      {
      theStochVar = theOptVar->mandStochVar ();

      if (theStochVar->isStSlbvVar ())
         {
         boundsValue +=
            theStochVar->primalValue () *
            theStochVar->totalProb   ();
         }
      }

   myStochModeMgr ()->passBoundsValue (boundsValue);

   uploadingSoln_ = false;

   myOptComp ()->writeObjValue ();
   }

//------------------------------------------------------------------------------

void WitStochOptMgr::uploadScenSoln ()
   {
   WitOptVar *   theOptVar;
   WitStochVar * theStochVar;
   WitScenario * theScenario;

   uploadingSoln_ = true;

   forEachEl (theOptVar, myStochImpOP_->myOptVars ())
      {
      theStochVar = theOptVar->mandStochVar ();

      forEachEl (theScenario, theStochVar->myScenGroup ()->myScenarios ())
         theStochVar->uploadSolnTo (theScenario);
      }

   uploadingSoln_ = false;
   }

//------------------------------------------------------------------------------
// Implementation of class StochImpOP.
//------------------------------------------------------------------------------

WitStochImpOP::WitStochImpOP (WitStochModeMgr * theStochModeMgr):

      WitOptProblem         (theStochModeMgr->myProblem ()),
      myStochModeMgr_       (theStochModeMgr),
      myScenGroups_         (),
      myScenGroupForStage0_ (NULL),
      myExecPerSched_       (NULL)
   {
   buildScenGroups ();

   myExecPerSched_ = new WitExecPerSched <WitBillEntry> (myProblem ());
   }

//------------------------------------------------------------------------------

WitStochImpOP::~WitStochImpOP ()
   {
   delete myExecPerSched_;

   myScenGroupForStage0_ = NULL;

   while (not myScenGroups_.isEmpty ())
      delete myScenGroups_.get ();
   }

//------------------------------------------------------------------------------

bool WitStochImpOP::needDual ()
   {
   return false;
   }

//------------------------------------------------------------------------------

bool WitStochImpOP::reSolveMode ()
   {
   return false;
   }

//------------------------------------------------------------------------------

void WitStochImpOP::printItem (WitNode * theNode)
   {
   fprintf (optProbFile (),
      " %-11s                        ",
      theNode->nodeName ().myCstring ());
   }

//------------------------------------------------------------------------------

void WitStochImpOP::printItem (WitDemand * theDemand)
   {
   fprintf (optProbFile (),
      " %-11s %-11s            ", 
      theDemand->demandedPartName ().myCstring (),
      theDemand->demandName ()      .myCstring ());
   }

//------------------------------------------------------------------------------

void WitStochImpOP::printItem (WitBomEntry * theBomEnt)
   {
   fprintf (optProbFile (),
      " %-11s %-11d            ", 
      theBomEnt->myOperationName ().myCstring (),
      theBomEnt->localIndex ());
   }

//------------------------------------------------------------------------------

void WitStochImpOP::printItem (WitSubEntry * theSub)
   {
   fprintf (optProbFile (),
      " %-11s %-11d %-11d", 
      theSub->myOperationName ().myCstring (),
      theSub->myBomEntIndex (),
      theSub->localIndex ());
   }

//------------------------------------------------------------------------------

void WitStochImpOP::printItem (WitStBddVar * theStBddVar)
   {
   fprintf (optProbFile (),
      " Var #%-6d BS #%-7d            ",
      theStBddVar->index (),
      theStBddVar->myBoundSet ()->mappingIndex ());
   }

//------------------------------------------------------------------------------

void WitStochImpOP::generateVariables ()
   {
   WitScenGroup * theScenGroup;
   WitStochLoc *  theStochLoc;
   WitOptVar *    theOptVar;

   forEachEl (theScenGroup, myScenGroups_)
      forEachEl (theStochLoc, theScenGroup->myStochLocs ())
         theStochLoc->generateVariables ();

   forEachEl (theOptVar, myOptVars ())
      theOptVar->mandStochVar ()->genStSlbvVarIfNeeded ();
   }

//------------------------------------------------------------------------------

void WitStochImpOP::generateConstraints ()
   {
   WitScenGroup * theScenGroup;
   WitStochLoc *  theStochLoc;
   WitOptVar *    theOptVar;

   forEachEl (theScenGroup, myScenGroups_)
      forEachEl (theStochLoc, theScenGroup->myStochLocs ())
         theStochLoc->generateConstraints ();

   forEachEl (theOptVar, myOptVars ())
      theOptVar->mandStochVar ()->genStSlbConIfNeeded ();
   }

//------------------------------------------------------------------------------

void WitStochImpOP::postGenFixed ()
   {
   WitScenGroup * theScenGroup;
   WitStochLoc *  theStochLoc;

   forEachEl (theScenGroup, myScenGroups_)
      forEachEl (theStochLoc, theScenGroup->myStochLocs ())
         theStochLoc->clearStochVars ();

   delete myExecPerSched_;

   myExecPerSched_ = NULL;
   }

//------------------------------------------------------------------------------

void WitStochImpOP::postGenFlexible ()
   {
   WitScenGroup * theScenGroup;

   forEachEl (theScenGroup, myScenGroups_)
      theScenGroup->postGenFlexible ();
   }

//------------------------------------------------------------------------------

void WitStochImpOP::printOptVCHeading ()
   {
   fprintf (optProbFile (),
      "Index    Class        Item        Item        Item        Per Scn");
   }

//------------------------------------------------------------------------------

void WitStochImpOP::doScreening ()
   {
   if (myOptComp ()->printOptProblem ())
      saveBounds ();
   }

//------------------------------------------------------------------------------

void WitStochImpOP::buildScenGroups ()
   {
   WitScenario *         theScenario;
   WitList <WitScenario> theScenarioList;
   WitScenGroup *        theScenGroup;

   myScenGroupForStage0_ =
      new WitScenGroup (
         this,
         myStochModeMgr_->myStageMgr ()->myStageFor (0),
         myStochModeMgr_->myScenMgr  ()->myScenarios ());

   myScenGroups_.append (myScenGroupForStage0_);

   forEachEl (theScenario, myStochModeMgr_->myScenMgr ()->myScenarios ())
      {
      theScenarioList.clear ();

      theScenarioList.append (theScenario);

      theScenGroup =
         new WitScenGroup (
            this,
            myStochModeMgr_->myStageMgr ()->myStageFor (1),
            theScenarioList);

      myScenGroups_.append (theScenGroup);
      }
   }

//------------------------------------------------------------------------------

bool WitStochImpOP::positiveHardLBsExist ()
   {
   WitScenInputAtt <WitBoundSet> * hardLBAtt;
   WitBoundSet *                   theBoundSet;
   WitScenario *                   theScenario;

   hardLBAtt = myStochModeMgr_->myScenAttMgr ()->hardLB ();

   forEachEl (theBoundSet, myStochModeMgr_->myScenAttMgr ()->allBoundSets ())
      forEachEl (theScenario, myStochModeMgr_->myScenMgr ()->myScenarios ())
         if (hardLBAtt->myValueFor (theBoundSet, theScenario) != 0.0)
            return true;

   return false;
   }

//------------------------------------------------------------------------------

bool WitStochImpOP::disallowedScrapExists ()
   {
   WitPart * thePart;

   forEachPart (thePart, myProblem ())
      if (not thePart->scrapAllowed ())
         return true;

   return false;
   }

//------------------------------------------------------------------------------
// Implementation of class ScenGroup.
//------------------------------------------------------------------------------

WitScenGroup::WitScenGroup (
         WitStochImpOP *               theStochImpOP,
         WitStage *                    theStage,
         const WitList <WitScenario> & theScenarios):

      WitStochAssoc  (theStochImpOP->myStochModeMgr ()),
      myStochImpOP_  (theStochImpOP),
      myStage_       (theStage),
      myScenarios_   (),
      myStochLocVec_ (myProblem ()),
      myStochLocs_   (),
      cumDemandVol_  (myProblem (), 0.0)
   {
   WitScenario * theScenario;
   WitPeriod     thePer;
   WitStochLoc * theStochLoc;

   myScenarios_ = theScenarios;

   totalProb_   = 0.0;

   forEachEl (theScenario, myScenarios_)
      totalProb_ += theScenario->probability ();

   forEachPeriod (thePer, myProblem ())
      if (myStage_->includes (thePer))
         {
         theStochLoc = new WitStochLoc (this, thePer);

         myStochLocVec_[thePer] = theStochLoc;

         myStochLocs_.append (theStochLoc);
         }

   compCumDemandVol ();
   }

//------------------------------------------------------------------------------

WitScenGroup::~WitScenGroup ()
   {
   myStochLocs_.clear ();

   deleteContents (myStochLocVec_);
   }

//------------------------------------------------------------------------------

void WitScenGroup::postGenFlexible ()
   {
   cumDemandVol_.clear ();
   }

//------------------------------------------------------------------------------

void WitScenGroup::print ()
   {
   if (myStage_->isStage0 ())
      fprintf (myStochImpOP_->optProbFile (),
         " All");
   else
      fprintf (myStochImpOP_->optProbFile (),
         " %3d",
         firstScenario ()->myIndex ());
   }

//------------------------------------------------------------------------------

void WitScenGroup::compCumDemandVol ()
   {
   WitDemand * theDemand;
   double      cumSum;
   WitPeriod   thePer;

   forEachDemand (theDemand, myProblem ())
      {
      cumSum = 0.0;

      forEachPeriod (thePer, myProblem ())
         {
         cumSum +=
            myScenAttMgr ()->
               demandVol ()->
                  myValueFor (theDemand, firstScenario ())[thePer];

         cumDemandVol_ (theDemand)[thePer] = cumSum;
         }
      }
   }

//------------------------------------------------------------------------------
// Implementation of class StochLoc.
//------------------------------------------------------------------------------

WitStochLoc::WitStochLoc (WitScenGroup * theScenGroup, WitPeriod thePer):

      WitStochAssoc    (theScenGroup),
      myScenGroup_     (theScenGroup),
      myPer_           (thePer),
      myScrapVarFor_   (myProblem ()),
      myStockVarFor_   (myProblem ()),
      myExecVarFor_    (myProblem ()),
      myShipVarFor_    (myProblem ()),
      myCumShipVarFor_ (myProblem ()),
      myNonSubVarFor_  (myProblem ()),
      mySubVarFor_     (myProblem ())
   {                     
   }

//------------------------------------------------------------------------------

WitStochLoc::~WitStochLoc ()
   {
   }

//------------------------------------------------------------------------------

void WitStochLoc::generateVariables ()
   {
   genScrapVars   ();
   genStockVars   ();
   genShipVars    ();
   genCumShipVars ();
   genExecVars    ();
   genNonSubVars  ();
   genSubVars     ();
   }

//------------------------------------------------------------------------------

void WitStochLoc::generateConstraints ()
   {
   genResCons  ();
   genShipCons ();
   genSubCons  ();
   }

//------------------------------------------------------------------------------

void WitStochLoc::clearStochVars ()
   {
   myScrapVarFor_  .clear ();
   myStockVarFor_  .clear ();
   myExecVarFor_   .clear ();
   myShipVarFor_   .clear ();
   myCumShipVarFor_.clear ();
   myNonSubVarFor_ .clear ();
   mySubVarFor_    .clear ();
   }

//------------------------------------------------------------------------------

void WitStochLoc::print ()
   {
   fprintf (myStochImpOP ()->optProbFile (), " %3d", myPer_);

   myScenGroup_->print ();
   }

//------------------------------------------------------------------------------

WitStScrapVar * WitStochLoc::myScrapVarFor (WitPart * thePart)
   {
   return myScrapVarFor_ (thePart);
   }

//------------------------------------------------------------------------------

WitStStockVar * WitStochLoc::myStockVarFor (WitMaterial * theMat)
   {
   return myStockVarFor_ (theMat);
   }

//------------------------------------------------------------------------------

WitStExecVar * WitStochLoc::myExecVarFor (WitOperation * theOpn)
   {
   return myExecVarFor_ (theOpn);
   }

//------------------------------------------------------------------------------

WitStShipVar * WitStochLoc::myShipVarFor (WitDemand * theDemand)
   {
   return myShipVarFor_ (theDemand);
   }

//------------------------------------------------------------------------------

WitStCumShipVar * WitStochLoc::myCumShipVarFor (WitDemand * theDemand)
   {
   return myCumShipVarFor_ (theDemand);
   }

//------------------------------------------------------------------------------

WitStNonSubVar * WitStochLoc::myNonSubVarFor (WitBomEntry * theBomEnt)
   {
   return myNonSubVarFor_ (theBomEnt);
   }

//------------------------------------------------------------------------------

WitStSubVar * WitStochLoc::mySubVarFor (WitSubEntry * theSub)
   {
   return mySubVarFor_ (theSub);
   }

//------------------------------------------------------------------------------

void WitStochLoc::genScrapVars ()
   {
   WitPart * thePart;

   forEachPart (thePart, myProblem ())
      if (myStage ()->includes (thePart))
         if (thePart->scrapAllowed ())
            myScrapVarFor_ (thePart) = new WitStScrapVar (thePart, this);
   }

//------------------------------------------------------------------------------

void WitStochLoc::genStockVars ()
   {
   WitMaterial * theMat;

   forEachMaterial (theMat, myProblem ())
      if (myStage ()->includes (theMat))
         myStockVarFor_ (theMat) = new WitStStockVar (theMat, this);
   }

//------------------------------------------------------------------------------

void WitStochLoc::genShipVars ()
   {
   WitDemand * theDemand;

   forEachDemand (theDemand, myProblem ())
      if (myStage ()->includes (theDemand->demandedPart ()))
         myShipVarFor_ (theDemand) = new WitStShipVar (theDemand, this);
   }

//------------------------------------------------------------------------------

void WitStochLoc::genCumShipVars ()
   {
   WitDemand * theDemand;

   forEachDemand (theDemand, myProblem ())
      if (myStage ()->includes (theDemand->demandedPart ()))
         if (theDemand->shipLateAllowed ())
            myCumShipVarFor_       (theDemand) =
               new WitStCumShipVar (theDemand, this);
   }

//------------------------------------------------------------------------------

void WitStochLoc::genExecVars ()
   {
   WitOperation * theOpn;

   forEachOperation (theOpn, myProblem ())
      if (myStage ()->includes (theOpn))
         if (theOpn->executable ()[myPer_])
            myExecVarFor_ (theOpn) = new WitStExecVar (theOpn, this);
   }

//------------------------------------------------------------------------------

void WitStochLoc::genNonSubVars ()
   {
   WitBomEntry * theBomEnt;

   forEachBomEntry (theBomEnt, myProblem ())
      if (myStage ()->includes (theBomEnt->myOperation ()))
         if (theBomEnt->hasSubsInEffect ()[myPer_])
            myNonSubVarFor_       (theBomEnt) =
               new WitStNonSubVar (theBomEnt, this);
   }

//------------------------------------------------------------------------------

void WitStochLoc::genSubVars ()
   {
   WitSubEntry * theSub;

   forEachSubEntry (theSub, myProblem ())
      if (myStage ()->includes (theSub->myOperation ()))
         if (theSub->inEffect (myPer_))
            mySubVarFor_ (theSub) = new WitStSubVar (theSub, this);
   }

//------------------------------------------------------------------------------

void WitStochLoc::genResCons ()
   {
   WitPart * thePart;

   forEachPart (thePart, myProblem ())
      if (myStage ()->includes (thePart))
         new WitStResCon (thePart, this);
   }

//------------------------------------------------------------------------------

void WitStochLoc::genShipCons ()
   {
   WitDemand * theDemand;

   forEachDemand (theDemand, myProblem ())
      if (myStage ()->includes (theDemand->demandedPart ()))
         new WitStShipCon (theDemand, this);
   }

//------------------------------------------------------------------------------

void WitStochLoc::genSubCons ()
   {
   WitBomEntry * theBomEnt;

   forEachBomEntry (theBomEnt, myProblem ())
      if (myStage ()->includes (theBomEnt->myOperation ()))
         if (theBomEnt->hasSubsInEffect ()[myPer_])
            new WitStSubCon (theBomEnt, this);
   }

//------------------------------------------------------------------------------
// Implementation of class StochVar.
//------------------------------------------------------------------------------

WitStochVar::WitStochVar (WitStochLoc * theStochLoc):

      WitOptVar   (theStochLoc->myScenGroup ()->myStochImpOP ()),
      myStochLoc_ (theStochLoc)
   {
   }

//------------------------------------------------------------------------------

WitStochVar::~WitStochVar ()
   {
   }

//------------------------------------------------------------------------------

WitOptProblem * WitStochVar::myOptProblem ()
   {
   return myStochImpOP ();
   }

//------------------------------------------------------------------------------

WitStochVar * WitStochVar::mandStochVar ()
   {
   return this;
   }

//------------------------------------------------------------------------------

void WitStochVar::genStSlbvVarIfNeeded ()
   {
   }

//------------------------------------------------------------------------------

void WitStochVar::genStSlbConIfNeeded ()
   {
   }

//------------------------------------------------------------------------------

double WitStochVar::stBoundsCoeffVal ()
   {
   return 0.0;
   }

//------------------------------------------------------------------------------

bool WitStochVar::isStSlbvVar ()
   {
   return false;
   }

//------------------------------------------------------------------------------

void WitStochVar::printSource ()
   {
   printItem ();

   myStochLoc_->print ();
   }

//------------------------------------------------------------------------------
// Implementation of class StochCon.
//------------------------------------------------------------------------------

WitStochCon::WitStochCon (WitStochLoc * theStochLoc):

      WitOptCon   (theStochLoc->myScenGroup ()->myStochImpOP ()),
      myStochLoc_ (theStochLoc)
   {
   }

//------------------------------------------------------------------------------

WitStochCon::~WitStochCon ()
   {
   }

//------------------------------------------------------------------------------

WitOptProblem * WitStochCon::myOptProblem ()
   {
   return myStochImpOP ();
   }

//------------------------------------------------------------------------------

WitStochLoc * WitStochCon::findVarStochLocFor (
      WitNode * theNode,
      WitPeriod thePer)
   {
   WitStage *     theStage;
   WitScenGroup * theScenGroup;

   theStage = myStochLoc_->myStageMgr ()->myStageOf (theNode, thePer);

   if (theStage == myStochLoc_->myStage ())
      theScenGroup = myScenGroup ();
   else
      {
      stronglyAssert (theStage->isStage0 ());

      theScenGroup = myStochImpOP ()->myScenGroupForStage0 ();
      }

   return theScenGroup->myStochLocFor (thePer);
   }

//------------------------------------------------------------------------------

void WitStochCon::printSource ()
   {
   printItem ();

   myStochLoc_->print ();
   }
