//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "SaeMgr.C"
//
// Contains the implementation of class SaeMgr.
//------------------------------------------------------------------------------

#include <SaeMgr.h>
#include <ParamMgr.h>
#include <Timing.h>
#include <DetOptImpMgr.h>
#include <StochImpMgr.h>
#include <PipMgr.h>
#include <Variant.h>
#include <OptStarter.h>
#include <FSS.h>
#include <DataWrit.h>
#include <Pre.h>
#include <Post.h>
#include <GlobalComp.h>
#include <OptComp.h>
#include <Material.h>
#include <Demand.h>
#include <Operation.h>
#include <SubEntry.h>
#include <BopEntry.h>
#include <Mapping.h>
#include <ObjStack.h>
#include <IVRealArg.h>
#include <WitRun.h>
#include <ApiMgr.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class WitSaeMgr.
//------------------------------------------------------------------------------

bool WitSaeMgr::standAloneMode_ = false;

//------------------------------------------------------------------------------

inline const WitString & WitSaeMgr::stringParamVal (const char * paramName)
   {
   return myParamMgr_->stringParamVal (paramName);
   }

//------------------------------------------------------------------------------

inline const char * WitSaeMgr::cStringParamVal (const char * paramName)
   {
   return stringParamVal (paramName).myCstring ();
   }

//------------------------------------------------------------------------------

inline int WitSaeMgr::intParamVal (const char * paramName)
   {
   return myParamMgr_->intParamVal (paramName);
   }

//------------------------------------------------------------------------------

inline const WitString & WitSaeMgr::choiceParamVal (const char * paramName)
   {
   return myParamMgr_->choiceParamVal (paramName);
   }

//------------------------------------------------------------------------------

inline bool WitSaeMgr::boolParamVal (const char * paramName)
   {
   return myParamMgr_->boolParamVal (paramName);
   }

//------------------------------------------------------------------------------

void WitSaeMgr::execute (WitRun * theWitRun, int argc, char * argv[])
   {
   stronglyAssert (not standAloneMode_);

   standAloneMode_ = true;

   WitSaeMgr theSaeMgr (theWitRun, argc, argv);

   theSaeMgr.execute ();

   stronglyAssert (standAloneMode_);

   standAloneMode_ = false;
   }

//------------------------------------------------------------------------------

WitSaeMgr::WitSaeMgr (WitRun * theWitRun, int argc, char * argv[]):

      myWitRun_   (theWitRun),
      mySession_  (theWitRun->mySession ()),
      myMsgFac_   (theWitRun->mySession ()->myMsgFac ()),
      myTimer_    (NULL),
      myParamMgr_ (NULL),
      myProblem_  (NULL)
   {
   myTimer_    = new WitTimer (myMsgFac_);

   myParamMgr_ = new WitParamMgr (myWitRun_, argc, argv);
   }

//------------------------------------------------------------------------------

WitSaeMgr::~WitSaeMgr ()
   {
   delete myParamMgr_;
   delete myTimer_;
   }

//------------------------------------------------------------------------------

void WitSaeMgr::execute ()
   {
   setUp ();

   if (WitMsgFacility::stopRunningWasSet ())
      myMsgFac_ ("standAloneAfterStopSmsg");

   WitTimer::enterSection ("other");

   readInput ();

   preprocess ();

   performAction ();

   if (choiceParamVal ("action") == "mrp")
      printMrp ();
   else
      printImplosion ();

   WitTimer::enterSection ("freeMemory");

   myProblem_ = NULL;

   mySession_->deactivate ();

   WitTimer::leaveSection ("freeMemory");

   WitTimer::leaveSection ("other");

   myTimer_->display ();

   myMsgFac_ ("normalTermMsg");
   }

//------------------------------------------------------------------------------

void WitSaeMgr::setUp ()
   {
   setUpMsg ();

   mySession_->activate ();

   myProblem_ = mySession_->myProblem ();
   }

//------------------------------------------------------------------------------

void WitSaeMgr::setUpMsg ()
   {
   myMsgFac_.setAccessMode ("w+");
      //
      // Overwrite existing files

   myMsgFac_.setDisplayExternalNumberByLevel (WitINFORMATIONAL_MESSAGES, false);

   myMsgFac_.setMaxTimesIssued ("witFuncCalled",     0);
   myMsgFac_.setMaxTimesIssued ("setStringAttrMsg",  0);
   myMsgFac_.setMaxTimesIssued ("witFileParam",      0);
   myMsgFac_.setMaxTimesIssued ("setIntAttrMsg",     0);
   myMsgFac_.setMaxTimesIssued ("setDblAttrMsg",     0);
   myMsgFac_.setMaxTimesIssued ("getAttrMsg",        0);
   myMsgFac_.setMaxTimesIssued ("setBooleanAttrMsg", 0);
   myMsgFac_.setMaxTimesIssued ("fileFormatMsg",     0);
   }

//------------------------------------------------------------------------------

void WitSaeMgr::readInput ()
   {
   WitOptStarter * theOptStarter;

   myParamMgr_->procParams ();

   myMsgFac_ ("readDataMsg");

   myWitRun_->witReadData (cStringParamVal ("data_ifname"));

   if (myProblem ()->myGlobalComp ()->inputPauses ())
      myProblem ()->pauseForInput ("just after reading the data");

   if (stringParamVal ("data_ifname2") != "")
      {
      myMsgFac_ ("genericMsg", "Reading the second input data file.");

      myWitRun_->witReadData (cStringParamVal ("data_ifname2"));
      }

   if (boolParamVal ("makeVolServ"))
      makeVolServObj ();

   if (boolParamVal ("print_echo"))
      {
      myMsgFac_ ("writeEchoMsg");

      myWitRun_->witDisplayData (cStringParamVal ("echo_ofname"));
      }

   if (myProblem ()->myStochImpMgr ()->stochMode ())
      if (     (choiceParamVal ("action") == "heur")
            or (choiceParamVal ("action") == "opt")
            or (choiceParamVal ("action") == "eval"))
         {
         myMsgFac_ ("actionInStochModeSmsg", choiceParamVal ("action"));
         }

   if (choiceParamVal ("optInitMethod") != "none")
      {
      theOptStarter =
         myProblem ()->
            myOptComp ()->
               allOptStarters ().
                  find (
                   & WitOptStarter::paramValue,
                     choiceParamVal ("optInitMethod").myCstring ());

      theOptStarter->beChosen ();
      }
   }

//------------------------------------------------------------------------------

void WitSaeMgr::preprocess ()
   {
   myWitRun_->witPreprocess ();

   if (   (choiceParamVal ("action") == "preproc")
       or (choiceParamVal ("action") == "opt")
       or (choiceParamVal ("action") == "stoch")
       or (choiceParamVal ("action") == "eval"))

      myProblem ()->myPreprocessor ()->preprocess ();

   if (boolParamVal ("pipEnableAll"))
      {
      pipEnableAllOpns ();

      displayDepth ();
      }

   if (boolParamVal ("writeData"))
      {
      myMsgFac_ ("genericMsg", "Writing the input data file.");

      myWitRun_->witWriteData (cStringParamVal ("data_ofname"));
      }

   if (boolParamVal ("print_pre"))
      {
      if (myProblem ()->myCompMgr ()->myGlobalComp ()->autoPriority ())
         WitDemand::genPriorities (myProblem ());

      myMsgFac_ ("writePreprocMsg");

      myWitRun_->witDisplayData (cStringParamVal ("pre_ofname"));
      }

   if (boolParamVal ("print_global"))
      {
      myMsgFac_ ("writeGlobalMsg");

      myProblem ()->displayGlobalData ();
      }
   }

//------------------------------------------------------------------------------

void WitSaeMgr::performAction ()
   {
   const WitString & actionString = choiceParamVal ("action");

   if      (actionString == "preproc")
      {
      if (boolParamVal ("print_soln"))
         myProblem ()->myPostprocessor ()->postprocess ();
      }

   else if (actionString == "mrp")
      myWitRun_->witMrp ();

   else if (actionString == "heur")
      myWitRun_->witHeurImplode ();

   else if (actionString == "opt")
      myProblem ()->myDetOptImpMgr ()->implode ();

   else if (actionString == "stoch")
      {
      if (not myProblem ()->myStochImpMgr ()->stochMode ())
         myMsgFac_ ("stochImpWoStochModeSmsg");

      myProblem ()->myStochImpMgr ()->implode ();
      }

   else if (actionString == "eval")
      {
      myProblem ()->myDetOptImpMgr ()->evalObjectives ();

      myProblem ()->myOptComp ()->writeObjValue ();
      }

   if (boolParamVal ("invokePip"))
      {
      if (boolParamVal ("print_echo"))
         myWitRun_->witDisplayData ("pipd.out");

      if (myProblem ()->myGlobalComp ()->inputPauses ())
         myProblem ()->pauseForInput ("just before building PIP");

      myProblem ()->myPipMgr ()->buildPegging ();

      if (myProblem ()->myGlobalComp ()->inputPauses ())
         myProblem ()->pauseForInput ("just after building PIP");
      }
   }

//------------------------------------------------------------------------------

void WitSaeMgr::printMrp ()
   {
   myMsgFac_ ("writeReqSchedMsg");

   myWitRun_->witWriteReqSched (cStringParamVal ("mrpsup_ofname"), WitBSV);

   if (boolParamVal ("print_soln"))
      {
      myMsgFac_ ("writeMrpSolnMsg");

      myProblem ()->writeMrpSoln (cStringParamVal ("soln_ofname"));
      }
   }

//------------------------------------------------------------------------------

void WitSaeMgr::printImplosion ()
   {
   if (boolParamVal ("compVolServ"))
      compVolServ ();

   if (boolParamVal ("print_exec"))
      {
      myMsgFac_ ("writeExecSchedMsg");

      myWitRun_->witWriteExecSched (cStringParamVal ("exec_ofname"), WitBSV);
      }

   if (boolParamVal ("print_ship"))
      {
      myMsgFac_ ("writeShipSchedMsg");

      myWitRun_->witWriteShipSched (cStringParamVal ("ship_ofname"), WitBSV);
      }

   if (boolParamVal ("print_soln"))
      {
      if (myProblem ()->currentVariant ()->internalSoln ())
         if (not myProblem ()->myStochImpMgr ()->stochMode ())
            doFss ();

      myMsgFac_ ("writeSolnMsg");

      myProblem ()->writeSoln (cStringParamVal ("soln_ofname"));
      }

   if (intParamVal ("n_critical") != 0)
      {
      myMsgFac_ ("writeCriticalListMsg");

      myWitRun_->
         witWriteCriticalList (
            cStringParamVal ("critical_ofname"),
            WitBSV,
            intParamVal ("n_critical"));
      }

   if (boolParamVal ("writeSolnData"))
      {
      myMsgFac_ ("genericMsg", "Writing the solution data file.");

      myProblem ()->
         myDataWriter ()->
            writeSolnData (cStringParamVal ("sdata_ofname"));
      }
   }

//------------------------------------------------------------------------------

void WitSaeMgr::doFss ()
   {
   WitDemand * theDemand;

   forEachDemand (theDemand, myProblem ())
      theDemand->setFocusHorizon (myProblem ()->lastPeriod ());

   myProblem ()->myFSS ()->compute ();
   }

//------------------------------------------------------------------------------

void WitSaeMgr::pipEnableAllOpns ()
   {
   WitOperation * theOpn;

   forEachOperation (theOpn, myProblem ())
      theOpn->setPipEnabled (true);
   }

//------------------------------------------------------------------------------

void WitSaeMgr::displayDepth ()
   {
   int                            maxDepth;
   WitMapping <WitOperation, int> opnDepth (myProblem (), 1);
   WitObjStack <WitPart>          theParts (myProblem ());
   WitOperation *                 theOpn;
   WitBomEntry *                  theBomEnt;
   WitSubEntry *                  theSub;
   WitPart *                      thePart;
   WitBopEntry *                  theBopEnt;
   int                            branchDepth;

   maxDepth = 0;

   forEachOperation (theOpn, myProblem ())
      {
      forEachEl (theBomEnt, theOpn->bom ())
         {
         theParts.push (theBomEnt->myPart ());

         forEachEl (theSub, theBomEnt->mySubEntries ())
            theParts.push (theSub->myPart ());
         }

      while (theParts.pop (thePart))
         forEachEl (theBopEnt, thePart->producingBopEntries ())
            {
            if (not theBopEnt->expAllowed ())
               continue;

            branchDepth = opnDepth (theBopEnt->myOperation ()) + 1;

            setToMax (opnDepth (theOpn), branchDepth);
            }

      setToMax (maxDepth, opnDepth (theOpn));
      }

   fprintf (myMsgFac_.myFile (),
      "\n"
      "Operation Depth: %d\n\n",
      maxDepth);
   }

//------------------------------------------------------------------------------

void WitSaeMgr::makeVolServObj ()
   {
   WitDemand *      theDemand;
   WitPeriod        thePer;
   double           totCumDemandVol;
   WitTVec <double> theCumShipReward (myProblem (), 0.0);

   clearObj ();

   totCumDemandVol = 0;

   forEachDemand (theDemand, myProblem ())
      forEachPeriod (thePer, myProblem ())
         totCumDemandVol += theDemand->cumDemandVol ()[thePer];

   stronglyAssert (totCumDemandVol >= .001);

   theCumShipReward = (100.0 / totCumDemandVol);

   forEachDemand (theDemand, myProblem ())
      theDemand->setCumShipReward (theCumShipReward.myCVec ());
   }

//------------------------------------------------------------------------------

void WitSaeMgr::clearObj ()
   {
   WitPart *        thePart;
   WitMaterial *    theMat;
   WitOperation *   theOpn;
   WitSubEntry *    theSub;
   WitDemand *      theDemand;
   WitTVec <double> the0Vector (myProblem (),  0.0);

   forEachPart (thePart, myProblem ())
      thePart->setScrapCost (the0Vector.myCVec ());

   forEachMaterial (theMat, myProblem ())
      theMat->setStockCost (the0Vector.myCVec ());

   forEachOperation (theOpn, myProblem ())
      theOpn->setExecCost (the0Vector.myCVec ());

   forEachSubEntry (theSub, myProblem ())
      theSub->setSubCost (the0Vector.myCVec ());

   forEachDemand (theDemand, myProblem ())
      theDemand->setShipReward (the0Vector.myCVec ());

   forEachDemand (theDemand, myProblem ())
      theDemand->setCumShipReward (the0Vector.myCVec ());
   }

//------------------------------------------------------------------------------

void WitSaeMgr::compVolServ ()
   {
   WitDemand * theDemand;
   WitPeriod   thePer;
   double      totCumDemandVol;
   double      totCumShipVol;

   totCumDemandVol = 0;
   totCumShipVol   = 0;

   forEachDemand (theDemand, myProblem ())
      forEachPeriod (thePer, myProblem ())
         {
         totCumDemandVol += theDemand->cumDemandVol ()[thePer];
         totCumShipVol   += theDemand->cumShipVol   ()[thePer];
         }

   if (totCumDemandVol < .001)
      fprintf (myMsgFac_.myFile (),
         "\n"
         "Volume Serviceability: undefined.\n\n");
   else
      fprintf (myMsgFac_.myFile (),
         "\n"
         "Volume Serviceability: %9.6f%%\n\n",
         100.0 * (totCumShipVol / totCumDemandVol));
   }

//------------------------------------------------------------------------------

bool WitSaeMgr::tempParIsSet (int index)
   {
   return myProblem ()->myGlobalComp ()->tempParIsSet (index);
   }
