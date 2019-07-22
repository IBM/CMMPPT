//------------------------------------------------------------------------------
// WIT-J C++ Source File Coordinator.C.
//
// Contains the implementation of class Coordinator.
//------------------------------------------------------------------------------

#include      <Coordinator.h>
#include          <Problem.h>
#include        <CaseToken.h>
#include             <Part.h>
#include           <Demand.h>
#include        <Operation.h>
#include         <BomEntry.h>
#include       <Substitute.h>
#include         <BopEntry.h>
#include       <PggHandler.h>
#include      <CTVecSupply.h>
#include     <ComponentJOR.h>
#include     <ArrayListJOR.h>
#include <PreJavaException.h>

#include              <wit.h>

//------------------------------------------------------------------------------

WitJ::Coordinator::Coordinator ():

      JavaAccObj          (this),
      myWitRun_           (NULL),
      myProblem_          (NULL),
      myPggHandler_       (NULL),
      nPartsOpns_         (0),
      myCTVecSupply_      (NULL),
      extractInProgress_  (false),
      extractForReadData_ (false)
   {
   witNewRun (& myWitRun_);

   witSetMesgThrowErrorExc (myWitRun_, WitTRUE, WitSEVERE_MESSAGES, WitTRUE);
   witSetMesgThrowErrorExc (myWitRun_, WitTRUE,  WitFATAL_MESSAGES, WitTRUE);

   witSetMesgTimesPrint    (myWitRun_, WitTRUE,                 97, 0);
   witSetMesgTimesPrint    (myWitRun_, WitTRUE,                 98, 0);
   witSetMesgTimesPrint    (myWitRun_, WitTRUE,                183, 0);

   witInitialize           (myWitRun_);
   witSetWit34Allowed      (myWitRun_,                              WitFALSE);

   witSetMesgTimesPrint    (myWitRun_, WitTRUE,                183, UCHAR_MAX);
   witSetMesgTimesPrint    (myWitRun_, WitTRUE,                 98, UCHAR_MAX);
   witSetMesgTimesPrint    (myWitRun_, WitTRUE,                 97, UCHAR_MAX);

   myPggHandler_ = new PggHandler (this);
   }

//------------------------------------------------------------------------------

WitJ::Coordinator::~Coordinator ()
   {
   delete myCTVecSupply_;
   delete myPggHandler_;

   witDeleteRun (myWitRun_);
   }

//------------------------------------------------------------------------------

WitJ::Coordinator * WitJ::Coordinator::asCoordinator ()
   {
   return this;
   }

//------------------------------------------------------------------------------

void WitJ::Coordinator::store (Problem * theProblem)
   {
   EXCEPTION_ASSERT ( myProblem_ == NULL);
   EXCEPTION_ASSERT (theProblem  != NULL);

   EXCEPTION_ASSERT (theProblem->getCoordinator () == this);

   myProblem_ = theProblem;
   }

//------------------------------------------------------------------------------

void WitJ::Coordinator::mapNameToCaseToken (
      const char * theName,
      CaseToken *  theCaseToken)
   {
   EXCEPTION_ASSERT (myCaseTokenMap_.count (theName) == 0);

   myCaseTokenMap_[theName] = theCaseToken;
   }

//------------------------------------------------------------------------------

void WitJ::Coordinator::setUpCaseTokens ()
   {
   myCaseTokenMap_["BuildPip"          ]->setSimpleWitFunc (witBuildPip);
   myCaseTokenMap_["ClearCplexParSpecs"]->setSimpleWitFunc (
                 witClearCplexParSpecs);
   myCaseTokenMap_["ClearPegging"      ]->setSimpleWitFunc (witClearPegging);
   myCaseTokenMap_["ClearPipSeq"       ]->setSimpleWitFunc (witClearPipSeq);
   myCaseTokenMap_["ClearStochSoln"    ]->setSimpleWitFunc (witClearStochSoln);
   myCaseTokenMap_["EvalObjectives"    ]->setSimpleWitFunc (witEvalObjectives);
   myCaseTokenMap_["FinishExtOpt"      ]->setSimpleWitFunc (witFinishExtOpt);
   myCaseTokenMap_["FinishHeurAlloc"   ]->setSimpleWitFunc (witFinishHeurAlloc);
   myCaseTokenMap_["GeneratePriorities"]->setSimpleWitFunc (
                 witGeneratePriorities);
   myCaseTokenMap_["HeurImplode"       ]->setSimpleWitFunc (witHeurImplode);
   myCaseTokenMap_["Mrp"               ]->setSimpleWitFunc (witMrp);
   myCaseTokenMap_["OptImplode"        ]->setSimpleWitFunc (witOptImplode);
   myCaseTokenMap_["Preprocess"        ]->setSimpleWitFunc (witPreprocess);
   myCaseTokenMap_["Postprocess"       ]->setSimpleWitFunc (witPostprocess);
   myCaseTokenMap_["PurgeData"         ]->setSimpleWitFunc (witPurgeData);
   myCaseTokenMap_["ShutDownExtOpt"    ]->setSimpleWitFunc (witShutDownExtOpt);
   myCaseTokenMap_["ShutDownHeurAlloc" ]->setSimpleWitFunc (
                 witShutDownHeurAlloc);
   myCaseTokenMap_["StartExtOpt"       ]->setSimpleWitFunc (witStartExtOpt);
   myCaseTokenMap_["StartHeurAlloc"    ]->setSimpleWitFunc (witStartHeurAlloc);
   myCaseTokenMap_["StochImplode"      ]->setSimpleWitFunc (witStochImplode);

   myCaseTokenMap_["CAPACITY"]   ->setWitAttr (WitCAPACITY);
   myCaseTokenMap_["MATERIAL"]   ->setWitAttr (WitMATERIAL);
   myCaseTokenMap_["ACCELERATED"]->setWitAttr (  WitACC_OPT_INIT_METHOD);
   myCaseTokenMap_["HEURISTIC"]  ->setWitAttr ( WitHEUR_OPT_INIT_METHOD);
   myCaseTokenMap_["SCHEDULE"]   ->setWitAttr (WitSCHED_OPT_INIT_METHOD);
   myCaseTokenMap_["CRASH"]      ->setWitAttr (WitCRASH_OPT_INIT_METHOD);

   myCaseTokenMap_["BSV"]->setWitFileFormat (WitBSV);
   myCaseTokenMap_["CSV"]->setWitFileFormat (WitCSV);

   myCaseTokenMap_["INFORMATIONAL"]->setMsgGroupNo (WitINFORMATIONAL_MESSAGES);
   myCaseTokenMap_["WARNING"]      ->setMsgGroupNo (      WitWARNING_MESSAGES);

   myCaseTokenMap_["EXEC_SCHED"]->setWriteSchedFunc (witWriteExecSched);
   myCaseTokenMap_["REQ_SCHED" ]->setWriteSchedFunc (witWriteReqSched);
   myCaseTokenMap_["SHIP_SCHED"]->setWriteSchedFunc (witWriteShipSched);

   setDemPggFunc ("DEM_CONS_PIP",    & PggHandler::getConsVolPip);
   setDemPggFunc ("DEM_CO_EXEC_PIP", & PggHandler::getCoExecVolPip);
   setDemPggFunc ("DEM_EXEC_PIP",    & PggHandler::getExecVolPip);
   setDemPggFunc ("DEM_PROD_PIP",    & PggHandler::getProdVolPip);
   setDemPggFunc ("DEM_SIDE_PIP",    & PggHandler::getSideVolPip);
   setDemPggFunc ("DEM_SUB_PIP",     & PggHandler::getSubVolPip);
   setDemPggFunc ("DEM_SUPPLY_PIP",  & PggHandler::getSupplyVolPip);
   setDemPggFunc ("DEM_EXEC_CP",     & PggHandler::getExecVolCp);
   setDemPggFunc ("DEM_SUB_CP",      & PggHandler::getSubVolCp);

   setOpnPggFunc ("OPN_CONS_PIP",    & PggHandler::getConsVolPip);
   setOpnPggFunc ("OPN_CO_EXEC_PIP", & PggHandler::getCoExecVolPip);
   setOpnPggFunc ("OPN_EXEC_PIP",    & PggHandler::getExecVolPip);
   setOpnPggFunc ("OPN_PROD_PIP",    & PggHandler::getProdVolPip);
   setOpnPggFunc ("OPN_SIDE_PIP",    & PggHandler::getSideVolPip);
   setOpnPggFunc ("OPN_SUB_PIP",     & PggHandler::getSubVolPip);
   setOpnPggFunc ("OPN_SUPPLY_PIP",  & PggHandler::getSupplyVolPip);

   myCaseTokenMap_.clear ();
   }

//------------------------------------------------------------------------------

void WitJ::Coordinator::recordCreatePartOpn ()
   {
   if (nPartsOpns_ == 0)
      {
      myCTVecSupply_ = new CTVecSupply (myProblem_->getNPeriods ());
      }

   nPartsOpns_ ++;
   }

//------------------------------------------------------------------------------

void WitJ::Coordinator::recordDeletePartOpn ()
   {
   nPartsOpns_ --;

   if (nPartsOpns_ == 0)
      {
      delete myCTVecSupply_;

      myCTVecSupply_ = NULL;
      }
   }

//------------------------------------------------------------------------------

void WitJ::Coordinator::extract (bool forReadData)
   {
   EXCEPTION_ASSERT (! extractInProgress_);

   extractInProgress_  = true;
   extractForReadData_ = forReadData;

   if (forReadData)
      verifyGlobalUTF8 ();

   extractGlobalData     ();
   extractNonGlobalComps ();

   extractForReadData_ = false;
   extractInProgress_  = false;
   }

//------------------------------------------------------------------------------

void WitJ::Coordinator::restoreAfterPurge ()
   {
   witAttr theState;

   witGetObjItrState (myWitRun_, & theState);

   EXCEPTION_ASSERT (theState == WitINACTIVE);

   while (true)
      {
      witAdvanceObjItr (myWitRun_);

      witGetObjItrState (myWitRun_, & theState);

           if (theState == WitINACTIVE)     break;
      else if (theState == WitAT_PART)      restorePart       ();
      else if (theState == WitAT_DEMAND)    restoreDemand     ();
      else if (theState == WitAT_OPERATION) restoreOperation  ();
      else if (theState == WitAT_BOM_ENTRY) restoreBomEntry   ();
      else if (theState == WitAT_SUB_ENTRY) restoreSubstitute ();
      else if (theState == WitAT_BOP_ENTRY) restoreBopEntry   ();
      else                                  EXCEPTION_ASSERT  (false);
      }
   }

//------------------------------------------------------------------------------

void WitJ::Coordinator::clearData ()
   {
   nPartsOpns_   = 0;

   delete myCTVecSupply_;

   myCTVecSupply_ = NULL;

   myProblem_->clearData ();

   witInitialize      (myWitRun_);
   witSetWit34Allowed (myWitRun_, WitFALSE);
   }

//------------------------------------------------------------------------------

void WitJ::Coordinator::populate (
      CompALJOR <Part> &   thePartALJOR,
      const char * const * partNameCVec,
      int                  theLength)
   {
   int    theIdx;
   Part * thePart;

   thePartALJOR.clearAndAllocate (theLength);

   for (theIdx = 0; theIdx < theLength; theIdx ++)
      {
      thePart = findPart (partNameCVec[theIdx]);

      thePartALJOR.add (thePart);
      }
   }

//------------------------------------------------------------------------------

void WitJ::Coordinator::populate (
      CompALJOR <Demand> & theDemandALJOR,
      const char * const * partNameCVec,
      const char * const * demandNameCVec,
      int                  theLength)
   {
   int      theIdx;
   Demand * theDemand;

   theDemandALJOR.clearAndAllocate (theLength);

   for (theIdx = 0; theIdx < theLength; theIdx ++)
      {
      theDemand = findDemand (partNameCVec[theIdx], demandNameCVec[theIdx]);

      theDemandALJOR.add (theDemand);
      }
   }

//------------------------------------------------------------------------------

void WitJ::Coordinator::populate (
      CompALJOR <Operation> & theOpnALJOR,
      const char * const *    opnNameCVec,
      int                     theLength)
   {
   int         theIdx;
   Operation * theOpn;

   theOpnALJOR.clearAndAllocate (theLength);

   for (theIdx = 0; theIdx < theLength; theIdx ++)
      {
      theOpn = findOperation (opnNameCVec[theIdx]);

      theOpnALJOR.add (theOpn);
      }
   }

//------------------------------------------------------------------------------

WitJ::Part * WitJ::Coordinator::findPart (const char * thePartName)
   {
   void * theAppData;

   witGetPartAppData (getWitRun (), thePartName, & theAppData);

   return static_cast <Part *> (theAppData);
   }

//------------------------------------------------------------------------------

WitJ::Demand * WitJ::Coordinator::findDemand (
      const char * thePartName,
      const char * theDemandName)
   {
   void * theAppData;

   witGetDemandAppData (getWitRun (), thePartName, theDemandName, & theAppData);

   return static_cast <Demand *> (theAppData);
   }

//------------------------------------------------------------------------------

WitJ::Operation * WitJ::Coordinator::findOperation (const char * theOpnName)
   {
   void * theAppData;

   witGetOperationAppData (getWitRun (), theOpnName, & theAppData);

   return static_cast <Operation *> (theAppData);
   }

//------------------------------------------------------------------------------

WitJ::BomEntry * WitJ::Coordinator::findBomEntry (
      const char * theOpnName,
      int          theBomIndex)
   {
   void * theAppData;

   witGetBomEntryAppData (getWitRun (), theOpnName, theBomIndex, & theAppData);

   return static_cast <BomEntry *> (theAppData);
   }

//------------------------------------------------------------------------------

WitJ::Substitute * WitJ::Coordinator::findSubstitute (
      const char * theOpnName,
      int          theBomIndex,
      int          theSubIndex)
   {
   void * theAppData;

   witGetSubsBomEntryAppData (
      getWitRun (),
      theOpnName,
      theBomIndex,
      theSubIndex,
    & theAppData);

   return static_cast <Substitute *> (theAppData);
   }

//------------------------------------------------------------------------------

WitJ::BopEntry * WitJ::Coordinator::findBopEntry (
      const char * theOpnName,
      int          theBopIndex)
   {
   void * theAppData;

   witGetBopEntryAppData (getWitRun (), theOpnName, theBopIndex, & theAppData);

   return static_cast <BopEntry *> (theAppData);
   }

//------------------------------------------------------------------------------

void WitJ::Coordinator::setDemPggFunc (
      const char * theTokenName,
      DemPggFunc   theFunc)
   {
   myCaseTokenMap_[theTokenName]->setDemPggFunc (theFunc);
   }

//------------------------------------------------------------------------------

void WitJ::Coordinator::setOpnPggFunc (
      const char * theTokenName,
      OpnPggFunc   theFunc)
   {
   myCaseTokenMap_[theTokenName]->setOpnPggFunc (theFunc);
   }

//------------------------------------------------------------------------------

void WitJ::Coordinator::verifyGlobalUTF8 ()
   {
   char *     theCString;
   witBoolean multiObjModeVal;

   witGetTitle                (getWitRun (), & theCString);
   verifyUTF8                                 (theCString);
   witFree                                    (theCString);

   witGetSolverLogFileName    (getWitRun (), & theCString);
   verifyUTF8                                 (theCString);
   witFree                                    (theCString);

   witGetMultiObjMode         (getWitRun (), & multiObjModeVal);

   if (multiObjModeVal)
      {
      witGetObjectiveListSpec (getWitRun (), & theCString);
      verifyUTF8                              (theCString);
      witFree                                 (theCString);
      }
   }

//------------------------------------------------------------------------------

void WitJ::Coordinator::extractGlobalData ()
   {
   int nPeriodsVal;

   witGetNPeriods (myWitRun_, & nPeriodsVal);
   
   ComponentJOR::setJavaNPeriods (myProblem_, nPeriodsVal);
   }

//------------------------------------------------------------------------------

void WitJ::Coordinator::extractNonGlobalComps ()
   {
   witAttr theState;

   witGetObjItrState (myWitRun_, & theState);

   EXCEPTION_ASSERT (theState == WitINACTIVE);

   while (true)
      {
      witAdvanceObjItr (myWitRun_);

      witGetObjItrState (myWitRun_, & theState);

           if (theState == WitINACTIVE)     break;
      else if (theState == WitAT_PART)      extractPart       ();
      else if (theState == WitAT_DEMAND)    extractDemand     ();
      else if (theState == WitAT_OPERATION) extractOperation  ();
      else if (theState == WitAT_BOM_ENTRY) extractBomEntry   ();
      else if (theState == WitAT_SUB_ENTRY) extractSubstitute ();
      else if (theState == WitAT_BOP_ENTRY) extractBopEntry   ();
      else                                  EXCEPTION_ASSERT  (false);
      }
   }

//------------------------------------------------------------------------------

void WitJ::Coordinator::extractPart ()
   {
   char *  thePartNameCStr;
   witAttr thePartCatAttr;
   bool    isMaterial;

   witGetObjItrPart (getWitRun (), & thePartNameCStr);

   if (extractForReadData_)
      verifyUTF8 (thePartNameCStr);

   witGetPartCategory (getWitRun (), thePartNameCStr, & thePartCatAttr);

   isMaterial = (thePartCatAttr == WitMATERIAL);

   ComponentJOR::makeJavaPart (myProblem_, thePartNameCStr, isMaterial);

   witFree (thePartNameCStr);
   }

//------------------------------------------------------------------------------

void WitJ::Coordinator::extractDemand ()
   {
   char * thePartNameCStr;
   char * theDemandNameCStr;
   Part * thePart;

   witGetObjItrDemand (getWitRun (), & thePartNameCStr, & theDemandNameCStr);

   thePart = findPart (thePartNameCStr);

   if (extractForReadData_)
      verifyUTF8 (theDemandNameCStr);

   ComponentJOR::makeJavaDemand (thePart, theDemandNameCStr);

   witFree (theDemandNameCStr);
   witFree (thePartNameCStr);
   }

//------------------------------------------------------------------------------

void WitJ::Coordinator::extractOperation ()
   {
   char * theOpnNameCStr;

   witGetObjItrOperation (getWitRun (), & theOpnNameCStr);

   if (extractForReadData_)
      verifyUTF8 (theOpnNameCStr);

   ComponentJOR::makeJavaOperation (myProblem_, theOpnNameCStr);

   witFree (theOpnNameCStr);
   }

//------------------------------------------------------------------------------

void WitJ::Coordinator::extractBomEntry ()
   {
   char *      theOpnNameCStr;
   int         theBomIndex;
   char *      thePartNameCStr;
   Operation * theOpn;
   Part *      thePart;

   witGetObjItrBomEntry (getWitRun (), & theOpnNameCStr, & theBomIndex);

   witGetBomEntryConsumedPart (
      getWitRun (),
      theOpnNameCStr,
      theBomIndex,
    & thePartNameCStr);

   theOpn  = findOperation (theOpnNameCStr);
   thePart = findPart      (thePartNameCStr);

   ComponentJOR::makeJavaBomEntry (theOpn, thePart);

   witFree (thePartNameCStr);
   witFree (theOpnNameCStr);
   }

//------------------------------------------------------------------------------

void WitJ::Coordinator::extractSubstitute ()
   {
   char *     theOpnNameCStr;
   int        theBomIndex;
   int        theSubIndex;
   char *     thePartNameCStr;
   BomEntry * theBomEnt;
   Part *     thePart;

   witGetObjItrSubsBomEntry (
      getWitRun (),
    & theOpnNameCStr,
    & theBomIndex,
    & theSubIndex);

   witGetSubsBomEntryConsumedPart (
      getWitRun (),
      theOpnNameCStr,
      theBomIndex,
      theSubIndex,
    & thePartNameCStr);

   theBomEnt = findBomEntry (theOpnNameCStr, theBomIndex);
   thePart   = findPart     (thePartNameCStr);

   ComponentJOR::makeJavaSubstitute (theBomEnt, thePart);

   witFree (thePartNameCStr);
   witFree (theOpnNameCStr);
   }

//------------------------------------------------------------------------------

void WitJ::Coordinator::extractBopEntry ()
   {
   char *      theOpnNameCStr;
   int         theBopIndex;
   char *      thePartNameCStr;
   Operation * theOpn;
   Part *      thePart;

   witGetObjItrBopEntry (getWitRun (), & theOpnNameCStr, & theBopIndex);

   witGetBopEntryProducedPart (
      getWitRun (),
      theOpnNameCStr,
      theBopIndex,
    & thePartNameCStr);

   theOpn  = findOperation (theOpnNameCStr);
   thePart = findPart      (thePartNameCStr);

   ComponentJOR::makeJavaBopEntry (theOpn, thePart);

   witFree (thePartNameCStr);
   witFree (theOpnNameCStr);
   }

//------------------------------------------------------------------------------

void WitJ::Coordinator::restorePart ()
   {
   char * thePartNameCStr;
   Part * thePart;

   witGetObjItrPart (getWitRun (), & thePartNameCStr);

   thePart = findPart (thePartNameCStr);

   thePart->setWasDeletedInWit (false);

   witFree (thePartNameCStr);
   }

//------------------------------------------------------------------------------

void WitJ::Coordinator::restoreDemand ()
   {
   char *   thePartNameCStr;
   char *   theDemandNameCStr;
   Demand * theDemand;

   witGetObjItrDemand (getWitRun (), & thePartNameCStr, & theDemandNameCStr);

   theDemand = findDemand (thePartNameCStr, theDemandNameCStr);

   theDemand->setWasDeletedInWit (false);

   witFree (theDemandNameCStr);
   witFree (thePartNameCStr);
   }

//------------------------------------------------------------------------------

void WitJ::Coordinator::restoreOperation ()
   {
   char *      theOpnNameCStr;
   Operation * theOpn;

   witGetObjItrOperation (getWitRun (), & theOpnNameCStr);

   theOpn = findOperation (theOpnNameCStr);

   theOpn->setWasDeletedInWit (false);

   witFree (theOpnNameCStr);
   }

//------------------------------------------------------------------------------

void WitJ::Coordinator::restoreBomEntry ()
   {
   char *     theOpnNameCStr;
   int        theBomIndex;
   BomEntry * theBomEnt;

   witGetObjItrBomEntry (getWitRun (), & theOpnNameCStr, & theBomIndex);

   theBomEnt = findBomEntry (theOpnNameCStr, theBomIndex);

   theBomEnt->setBomIndex        (theBomIndex);
   theBomEnt->setWasDeletedInWit (false);

   witFree (theOpnNameCStr);
   }

//------------------------------------------------------------------------------

void WitJ::Coordinator::restoreSubstitute ()
   {
   char *       theOpnNameCStr;
   int          theBomIndex;
   int          theSubIndex;
   Substitute * theSub;

   witGetObjItrSubsBomEntry (
      getWitRun (),
    & theOpnNameCStr,
    & theBomIndex,
    & theSubIndex);

   theSub = findSubstitute (theOpnNameCStr, theBomIndex, theSubIndex);

   theSub->setSubIndex        (theSubIndex);
   theSub->setWasDeletedInWit (false);

   witFree (theOpnNameCStr);
   }

//------------------------------------------------------------------------------

void WitJ::Coordinator::restoreBopEntry ()
   {
   char *     theOpnNameCStr;
   int        theBopIndex;
   BopEntry * theBopEnt;

   witGetObjItrBopEntry (getWitRun (), & theOpnNameCStr, & theBopIndex);

   theBopEnt = findBopEntry (theOpnNameCStr, theBopIndex);

   theBopEnt->setBopIndex        (theBopIndex);
   theBopEnt->setWasDeletedInWit (false);

   witFree (theOpnNameCStr);
   }

//------------------------------------------------------------------------------

void WitJ::Coordinator::verifyUTF8 (const char * theCString)
   {
   if (! myProblem_->getComponentJOR ()->isaUTF8String (theCString))
      throw PreJavaException ("BadCharacterException", theCString);
   }
