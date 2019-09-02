//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "Problem.C"
//
// Contains the implementation of class Problem.
//------------------------------------------------------------------------------

#include <HeurImp.h>
#include <EqAll.h>
#include <HeurAllMgr.h>
#include <DetOptImpMgr.h>
#include <StochImpMgr.h>
#include <OrigMrp.h>
#include <PipMgr.h>
#include <DataRead.h>
#include <DataWrit.h>
#include <SolnWrit.h>
#include <Timing.h>
#include <Pre.h>
#include <Post.h>
#include <FeasChkr.h>
#include <FSS.h>
#include <LinkMgr.h>
#include <WitRun.h>
#include <ApiMgr.h>
#include <wit/src/Variant.h>
#include <GlobalComp.h>
#include <OptComp.h>
#include <Material.h>
#include <Demand.h>
#include <Operation.h>
#include <SubEntry.h>
#include <BopEntry.h>
#include <BoundSet.h>
#include <CpxParSpecMgr.h>
#include <FlexVec.h>
#include <TempMsgFile.h>
#include <MsgFrag.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class Problem.
//------------------------------------------------------------------------------

WitProblem::WitProblem (WitSession * theSession):
      mySession_            (theSession),

      myCompMgr_            (NULL),
      myPreprocessor_       (NULL),
      myPostprocessor_      (NULL),
      myHeurImploder_       (NULL),
      myEqAllocator_        (NULL),
      myHeurAllMgr_         (NULL),
      myDetOptImpMgr_       (NULL),
      myStochImpMgr_        (NULL),
      myPipMgr_             (NULL),
      myOrigMrpExp_         (NULL),
      myFSS_                (NULL),
      myDataReader_         (NULL),
      myDataWriter_         (NULL),
      mySolnWriter_         (NULL),

      myMsgFac_             (theSession->myMsgFac ()),
      myWitRun_             (theSession->myWitRun ()),
      myApiMgr_             (theSession->myApiMgr ()),
      solnFile_             (NULL),
      nPeriods_             (WitGlobalComp::defNPeriods ()),
      lastPeriod_           (nPeriods_ - 1),
      nPeriodsWasSet_       (false),
      myObjLinkMgr_         (NULL),
      myIntPairLinkMgr_     (NULL),
      myBoolPairLinkMgr_    (NULL),
      myDblPairLinkMgr_     (NULL),
      myVoidPtrPairLinkMgr_ (NULL),
      myTripLinkMgr_        (NULL),
      optVariant_           (NULL),
      extOptVariant_        (NULL),
      heurVariant_          (NULL),
      heurOptVariant_       (NULL),
      inputVariant_         (NULL),
      allVariants_          (),
      currentVariant_       (NULL),
      scalarFlexVAsst_      (NULL),
      vectorFlexVAsst_      (NULL),
      dispDoubleVector_     (),
      dispIntVector_        (),
      dispBoolVector_       ()
   {
   scalarFlexVAsst_      = new WitFlexVAsst (this, false);
   vectorFlexVAsst_      = new WitFlexVAsst (this, true);

   myObjLinkMgr_         = new WitObjLinkMgr           (this);
   myIntPairLinkMgr_     = new WitPairLinkMgr <int>    (this);
   myBoolPairLinkMgr_    = new WitPairLinkMgr <bool>   (this);
   myDblPairLinkMgr_     = new WitPairLinkMgr <double> (this);
   myVoidPtrPairLinkMgr_ = new WitPairLinkMgr <void *> (this);
   myTripLinkMgr_        = new WitTripLinkMgr          (this);

   myPreprocessor_       = new WitPreprocessor  (this);
   myPostprocessor_      = new WitPostprocessor (this);
   myHeurImploder_       = new WitHeurImploder  (this);
   myEqAllocator_        = new WitEqAllocator   (this);
   myHeurAllMgr_         = new WitHeurAllMgr    (this);
   myDetOptImpMgr_       = new WitDetOptImpMgr  (this);
   myStochImpMgr_        = new WitStochImpMgr   (this);
   myPipMgr_             = new WitPipMgr        (this);
   myOrigMrpExp_         = new WitOrigMrpExp    (this);
   myFSS_                = new WitFSS           (this);
   myDataReader_         = new WitDataReader    (this);
   myDataWriter_         = new WitDataWriter    (this);
   mySolnWriter_         = new WitSolnWriter    (this);

   setUpVariants ();

   myCompMgr_            = new WitCompMgr       (this);

   myGlobalComp_         = new WitGlobalComp    (myCompMgr ());
   myOptComp_            = new WitOptComp       (myCompMgr ());
   }

//------------------------------------------------------------------------------

WitProblem::~WitProblem ()
   {
   myStochImpMgr_->setStochMode (false);

   myPreprocessor_->unpreprocess ();

   while (not allVariants_.isEmpty ())
      delete allVariants_.get ();

   delete myFSS_;
   delete mySolnWriter_;
   delete myDataWriter_;
   delete myDataReader_;
   delete myOrigMrpExp_;
   delete myPipMgr_;
   delete myStochImpMgr_;
   delete myDetOptImpMgr_;
   delete myHeurAllMgr_;
   delete myEqAllocator_;
   delete myHeurImploder_;
   delete myPostprocessor_;
   delete myPreprocessor_;

   myOptComp_    = NULL;
   myGlobalComp_ = NULL;

   delete myCompMgr_;

   WitAbsLinkMgr::deleteInstance (myTripLinkMgr_);
   WitAbsLinkMgr::deleteInstance (myVoidPtrPairLinkMgr_);
   WitAbsLinkMgr::deleteInstance (myDblPairLinkMgr_);
   WitAbsLinkMgr::deleteInstance (myBoolPairLinkMgr_);
   WitAbsLinkMgr::deleteInstance (myIntPairLinkMgr_);
   WitAbsLinkMgr::deleteInstance (myObjLinkMgr_);

   delete scalarFlexVAsst_;
   delete vectorFlexVAsst_;
   }

//------------------------------------------------------------------------------

bool WitProblem::wit34Compatible ()
   {
   return myCompMgr ()->myGlobalComp ()->wit34Compatible ();
   }

//------------------------------------------------------------------------------

WitHeurModifier * WitProblem::myHeurModifier ()
   {
   return myHeurAllMgr_->myHeurModifier ();
   }

//------------------------------------------------------------------------------

void WitProblem::store (WitVariant * theVariant)
   {
   allVariants_.append (theVariant);
   }

//------------------------------------------------------------------------------

void WitProblem::setUpVariants ()
   {
   optVariant_     = new WitVariant (this);
   extOptVariant_  = new WitVariant (this);
   heurVariant_    = new WitVariant (this);
   heurOptVariant_ = new WitVariant (this);
   inputVariant_   = new WitVariant (this);

   WitVariant * theVariant;

   forEachEl (theVariant, allVariants_)
      theVariant->configure ();

   currentVariant_ = inputVariant ();
   }

//------------------------------------------------------------------------------

void WitProblem::makeCurrent (WitVariant * theVariant)
   {
   resetSoln ();

   currentVariant_ = theVariant;
   }

//------------------------------------------------------------------------------

void WitProblem::allocTVecs ()
   {
   if (dispDoubleVector_.isAllocated ())
      return;

   dispDoubleVector_.allocate (this, 0.0);
   dispIntVector_   .allocate (this, 0);
   dispBoolVector_  .allocate (this, false);
   }

//------------------------------------------------------------------------------

void WitProblem::clearTVecs ()
   {
   if (not dispDoubleVector_.isAllocated ())
      return;

   dispDoubleVector_.clear ();
   dispIntVector_   .clear ();
   dispBoolVector_  .clear ();
   }

//------------------------------------------------------------------------------

void WitProblem::updateNPeriods ()
   {
   nPeriods_       = myGlobalComp ()->nPeriods ();

   lastPeriod_     = nPeriods_ - 1;

   nPeriodsWasSet_ = true;

   scalarFlexVAsst_->updateNPeriods ();
   vectorFlexVAsst_->updateNPeriods ();
   }

//------------------------------------------------------------------------------

void WitProblem::displayData (const char * filename)
   {
   WitTimer::enterSection ("extra");

   myMsgFac () ("witFileParam",
      (filename == NULL)?
         myMsgFac ().fileName ().myCstring ():
         filename);

   WitTempMsgFile theTemp (myMsgFac (), filename);

   mySession ()->printHeading ();

   myMsgFac () ("fileHeadingDdMsg");

   displayGlobalData ();

   if (myStochImpMgr_->stochMode ())
      myStochImpMgr_->displayDataStochMode ();
   else
      displayNonGlobalData ();

   myPipMgr_->displayShipSeq ();

   WitTimer::leaveSection ("extra");
   }

//------------------------------------------------------------------------------

void WitProblem::displayGlobalData ()
   {
   WitPart *      thePart;
   int            nCapacities;
   WitOperation * theOpn;
   int            nPosPens;

   myMsgFac () ("globalHeadingDdMsg");

   myMsgFac () ("titleMsg", myCompMgr ()->myGlobalComp ()->title ());
   myMsgFac () ("dimenHeadingDdMsg");

   nCapacities = 0;

   forEachPart (thePart, this)
      if (thePart->thisCap () != NULL)
         ++ nCapacities;

   myMsgFac () ("categorySizeDdMsg",
      myMsgFac ().myFrag ("capacityFrag"),
      nCapacities);

   myMsgFac () ("categorySizeDdMsg",
      myMsgFac ().myFrag ("materialFrag"),
      myCompMgr ()->nMaterials ());

   myMsgFac () ("dimenDataDdMsg",
      myCompMgr ()->nParts (),
      myCompMgr ()->nOperations (),
      myCompMgr ()->nDemands (),
                    nPeriods (),
      myCompMgr ()->nBoundSetsSpecified (),
      myCompMgr ()->nBoundSets (),
      myCompMgr ()->nBomEntries (),
      myCompMgr ()->nSubEntries (),
      myCompMgr ()->nBopEntries (),
      myCompMgr ()->nDelComps ());

   nPosPens = 0;

   forEachOperation (theOpn, this)
      if (theOpn->execPenalty () > 0.0)
         nPosPens ++;

   myMsgFac () ("globalDataDdMsg",
                                     wit34Compatible     (),
      myCompMgr ()->myGlobalComp ()->independentOffsets  (),
      myCompMgr ()->myGlobalComp ()->execEmptyBom        (),
      myCompMgr ()->myGlobalComp ()->computeCriticalList (),
      myCompMgr ()->myGlobalComp ()->pgdCritListMode     (),
      myCompMgr ()->myGlobalComp ()->hashTableSize       (),
      myCompMgr ()->myGlobalComp ()->roundReqVols        (),
      myCompMgr ()->myGlobalComp ()->outputPrecision     (),
      myCompMgr ()->myGlobalComp ()->useFocusHorizons    (),
                                     nPosPens,
      myPreprocessor ()->            preprocessed        ());

   if (myPreprocessor_->preprocessed ())
      myMsgFac () ("preprocDataDdMsg",
         myCompMgr ()->myGlobalComp ()->lotSizesExist ());

   myCompMgr ()->myGlobalComp ()->display ();
   myCompMgr ()->myOptComp    ()->display ();

   myStochImpMgr_->displayGlobalData ();

   myCompMgr ()->myOptComp ()->myCpxParSpecMgr ()->display ();
   }

//------------------------------------------------------------------------------

void WitProblem::displayNonGlobalData ()
   {
   WitPart *      thePart;
   WitOperation * theOpn;
   WitBomEntry *  theBomEnt;
   WitSubEntry *  theSub;
   WitBopEntry *  theBopEnt;
   WitDemand *    theDemand;

   myMsgFac () ("partHeadingDdMsg");

   forEachPart (thePart, this)
      thePart->display ();

   myMsgFac () ("demandHeadingDdMsg");

   forEachDemand (theDemand, this)
      theDemand->display ();

   myMsgFac () ("operationHeadingDdMsg");

   forEachOperation (theOpn, this)
      theOpn->display ();

   myMsgFac () ("bomEntryHeadingDdMsg");

   forEachBomEntry (theBomEnt, this)
      theBomEnt->display ();

   myMsgFac () ("subHeadingDdMsg");

   forEachSubEntry (theSub, this)
      theSub->display ();

   myMsgFac () ("bopEntryHeadingDdMsg");

   forEachBopEntry (theBopEnt, this)
      theBopEnt->display ();
   }

//------------------------------------------------------------------------------

void WitProblem::display (
      WitMsgID              theMsgID,
      const WitDblFlexVec & theFlexVec)
   {
   dispDoubleVector_ = theFlexVec;

   myMsgFac () (theMsgID, dispDoubleVector_);
   }

//------------------------------------------------------------------------------

void WitProblem::display (
      WitMsgID                 theMsgID,
      const WitFlexVec <int> & theFlexVec)
   {
   dispIntVector_ = theFlexVec;
                    
   myMsgFac () (theMsgID, dispIntVector_);
   }

//------------------------------------------------------------------------------

void WitProblem::display (
      WitMsgID                  theMsgID,
      const WitFlexVec <bool> & theFlexVec)
   {
   dispBoolVector_ = theFlexVec;

   myMsgFac () (theMsgID, dispBoolVector_);
   }

//------------------------------------------------------------------------------

void WitProblem::display (WitMsgID theMsgID, const WitObjVec & theObjVec)
   {
   theObjVec.display (theMsgID, this);
   }

//------------------------------------------------------------------------------

void WitProblem::displayBoundVec (
      WitMsgFragID          theFragID,
      const WitDblFlexVec & theFlexVec)
   {
   dispDoubleVector_ = theFlexVec;

   myMsgFac () ("boundDataDdMsg",
      myMsgFac ().myFrag (theFragID), dispDoubleVector_);
   }

//------------------------------------------------------------------------------

void WitProblem::copyProblemInto (WitProblem * theProblem)
   {
   WitComponent * theComp;

   myPreprocessor_->preprocess ();

   forEachEl (theComp, myCompMgr ()->allComponents ())
      theComp->copySelfInto (theProblem->myCompMgr ());

   theProblem->myStochImpMgr_->copyAttrsFrom (myStochImpMgr_);
   }

//------------------------------------------------------------------------------

void WitProblem::prepSetUnpreAttr ()
   {
   stronglyAssert (not myStochImpMgr ()->stochMode ());

   myPreprocessor_->unpreprocess ();
   }

//------------------------------------------------------------------------------

void WitProblem::prepSetUnpostAttr ()
   {
   stronglyAssert (not myStochImpMgr ()->stochMode ());

   resetSoln ();
   }

//------------------------------------------------------------------------------

void WitProblem::prepSetNonUnpostAttr ()
   {
   stronglyAssert (not myStochImpMgr ()->stochMode ());
   }

//------------------------------------------------------------------------------

void WitProblem::prepSetStochModeAttr (const char * theAttName)
   {
   if (myStochImpMgr ()->stochMode ())
      myStochImpMgr ()->prepSetStochModeAttr (theAttName);

   resetSoln ();
   }

//------------------------------------------------------------------------------

void WitProblem::resetSoln ()
   {
   currentVariant_ = inputVariant ();

   myPostprocessor_->unpostprocess ();

   myHeurAllMgr ()->reset ();
   }

//------------------------------------------------------------------------------

void WitProblem::writeSoln (const char * fileName)
   {
   myPostprocessor_->postprocess ();

   WitTimer::enterSection ("extra");

   solnFile_ = mySession_->openFile (fileName, "w");

   mySession ()->writeHeading (solnFile ());

   fprintf (solnFile (), "Comprehensive Implosion Solution.\n\n");

   fprintf (solnFile (),
      "Solution provided by: %s.\n\n",
      myMsgFac ().fragText (currentVariant ()->myFragID ()));

   if (    (myStochImpMgr_->stochMode ())
       and (myStochImpMgr_->stochSolnMode ()))
      writeStochSoln ();
   else
      writeSolnInt ();

   fclose (solnFile ());

   solnFile_ = NULL;

   WitTimer::leaveSection ("extra");
   }

//------------------------------------------------------------------------------

#define SOLN_EPSILON  0.0005
   //
   // Values must exceed this amount in order to appear in the comprehensive
   // solution output.

//------------------------------------------------------------------------------

void WitProblem::writeSolnValue (double val)
   {
   if (fabs (val) >= SOLN_EPSILON)
      fprintf (solnFile (), "  %11.3f", val);
   else
      fprintf (solnFile (), "             ");
   }

//------------------------------------------------------------------------------

void WitProblem::writeMrpSoln (const char * fileName)
   {
   WitPart *      thePart;
   WitOperation * theOpn;
   WitBomEntry  * theBomEnt;

   WitTimer::enterSection ("extra");

   solnFile_ = mySession_->openFile (fileName, "w");

   mySession ()->writeHeading (solnFile ());

   fprintf (solnFile (), "Comprehensive MRP Solution.\n\n");

   fprintf (solnFile (), "\n\nComprehensive MRP Solution for Parts:\n");

   forEachPart (thePart, this)
      thePart->writeMrpSoln ();

   fprintf (solnFile (), "\n\nComprehensive MRP Solution for Operations:\n");

   forEachOperation (theOpn, this)
      theOpn->writeMrpSoln ();

   if (myCompMgr ()->hasSubEntries ())
      {
      fprintf (solnFile (),
         "\n\nComprehensive MRP Solution for Substitutes:\n");

      forEachBomEntry (theBomEnt, this)
         theBomEnt->writeMrpSubSoln ();
      }

   fclose (solnFile ());

   solnFile_ = NULL;

   WitTimer::leaveSection ("extra");
   }

//------------------------------------------------------------------------------

void WitProblem::computeFssIfNeeded ()
   {
   if (not myFSS ()->computed ())
      myFSS ()->compute ();
   }

//------------------------------------------------------------------------------

void WitProblem::pauseForInput (const char * theText)
   {
   int  retVal;
   char theCstring[2];

   printf (
      "\n"
      "------------------------------------------------------\n"
      "Pausing execution %s.\n"
      "------------------------------------------------------\n"
      "\n"
      "To continue,\n"
      "   type one non-blank character,\n"
      "   followed by \"enter\".\n"
      "\n",
      theText);

   retVal = scanf ("%1s", theCstring);
      //
      // The return value is stored in retVal to avoid a compiler warning on
      // some platforms.

   printf (
      "\n"
      "------------------------------------------------------\n"
      "Resuming execution.\n"
      "------------------------------------------------------\n"
      "\n");
   }

//------------------------------------------------------------------------------

void WitProblem::writeStochSoln ()
   {
   int origScenIdx;
   int theIdx;

   stronglyAssert (myStochImpMgr_->stochSolnMode ());

   origScenIdx = myStochImpMgr_->currentScenario ();

   for (theIdx = 0; theIdx < myStochImpMgr_->nScenarios (); theIdx ++)
      {
      myStochImpMgr_->setCurrentScenario (theIdx);

      fprintf (solnFile (),
         "\n"
         "-------------------------------------------------"
         "------------------------------------------------\n"
         "Comprehensive Solution for Scenario #%d:\n"
         "-------------------------------------------------"
         "------------------------------------------------\n\n",
         theIdx);

      writeSolnInt ();
      }
      
   myStochImpMgr_->setCurrentScenario (origScenIdx);
   }

//------------------------------------------------------------------------------

void WitProblem::writeSolnInt ()
   {
   WitPart *      thePart;
   WitDemand *    theDemand;
   WitOperation * theOpn;
   WitBomEntry *  theBomEnt;

   fprintf (solnFile (),
      "Solution is feasible?         %s\n",
      myMsgFac ().boolText (myCompMgr ()->myGlobalComp ()->feasible ()));

   fprintf (solnFile (),
      "Maximum constraint violation: %g\n",
      myPostprocessor ()->myFeasChkr ()->maxViolation ());

   fprintf (solnFile (), "\n\nComprehensive Solution for Parts:\n");

   forEachPart (thePart, this)
      thePart->writeSoln ();

   fprintf (solnFile (), "\n\nComprehensive Solution for Demands:\n");

   forEachDemand (theDemand, this)
      theDemand->writeSoln ();

   fprintf (solnFile (), "\n\nComprehensive Solution for Operations:\n");

   forEachOperation (theOpn, this)
      theOpn->writeSoln ();

   if (myCompMgr ()->hasSubEntries ())
      {
      fprintf (solnFile (), "\n\nComprehensive Solution for Substitutes:\n");

      forEachBomEntry (theBomEnt, this)
         theBomEnt->writeSubSoln ();
      }

   writeBoundsSoln  ();

   if (myOptComp ()->compPrices ())
      {
      fprintf (solnFile (), "\n\nShadow Prices:\n");

      forEachPart (thePart, this)
         thePart->writeDualSoln ();
      }

   if (devMode ())
      {
      if (myGlobalComp ()->perfPegging ())
         myHeurAllMgr_->printPegging ();

      myPipMgr_->printPegging ();

      if (myGlobalComp ()->pgdCritListMode ())
         myGlobalComp ()->writePdgCritList ();
      }
   }

//------------------------------------------------------------------------------

void WitProblem::writeBoundsSoln ()
   {
   WitMaterial *  theMat;
   WitDemand *    theDemand;
   WitOperation * theOpn;

   fprintf (solnFile (), "\n\nComprehensive Solution for Bounds:\n");

   if (myCompMgr ()->nBoundSetsSpecified () == 0)
      {
      fprintf (solnFile (),
         "\n\nNo bound sets specified for this implosion problem.\n");

      return;
      }

   forEachMaterial (theMat, this)
      theMat->stockBounds ()->writeSoln ();

   forEachDemand (theDemand, this)
      theDemand->cumShipBounds ()->writeSoln ();

   forEachOperation (theOpn, this)
      theOpn->execBounds ()->writeSoln ();
   }
