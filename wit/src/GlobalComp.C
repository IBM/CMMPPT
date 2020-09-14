//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "GlobalComp.C"
//
// Contains the implementation of class GlobalComp.
//------------------------------------------------------------------------------

#include <GlobalComp.h>
#include <CompMgr.h>
#include <DataWrit.h>
#include <Pre.h>
#include <Post.h>
#include <FSS.h>
#include <PipMgr.h>
#include <HeurAllMgr.h>
#include <PclEl.h>
#include <DetOptImpMgr.h>
#include <Part.h>
#include <Demand.h>
#include <Operation.h>
#include <MsgFac.h>
#include <MsgFrag.h>
#include <PtrVec.h>

//------------------------------------------------------------------------------

WitGlobalComp::WitGlobalComp (WitCompMgr * theCompMgr):

      WitComponent         (theCompMgr),

      title_               (defTitle               ()),
      nPeriods_            (defNPeriods            ()),
      independentOffsets_  (defIndependentOffsets  ()),
      execEmptyBom_        (defExecEmptyBom        ()),
      hashTableSize_       (defHashTableSize       ()),
      roundReqVols_        (defRoundReqVols        ()),
      outputPrecision_     (defOutputPrecision     ()),
      computeCriticalList_ (defComputeCriticalList ()),
      pgdCritListMode_     (defPgdCritListMode     ()),
      wit34Compatible_     (defWit34Compatible     ()),
      wit34Allowed_        (defWit34Allowed        ()),
      useFocusHorizons_    (defUseFocusHorizons    ()),
      localBuildAhead_     (defLocalBuildAhead     ()),
      multiExec_           (defMultiExec           ()),
      twoWayMultiExec_     (defTwoWayMultiExec     ()),
      localMultiExec_      (defLocalMultiExec      ()),
      equitability_        (defEquitability        ()),
      forcedMultiEq_       (defForcedMultiEq       ()),
      autoPriority_        (defAutoPriority        ()),
      skipFailures_        (defSkipFailures        ()),
      multiRoute_          (defMultiRoute          ()),
      truncOffsets_        (defTruncOffsets        ()),
      lotSizeTol_          (defLotSizeTol          ()),
      expCutoff_           (defExpCutoff           ()),
      penExec_             (defPenExec             ()),
      tieBreakPropRt_      (defTieBreakPropRt      ()),
      stockRealloc_        (defStockRealloc        ()),
      stockReallocation_   (defStockReallocation   ()),
      srSelSplit_          (defSrSelSplit          ()),
      respectStockSLBs_    (defRespectStockSLBs    ()),
      prefHighStockSLBs_   (defPrefHighStockSLBs   ()),
      perfPegging_         (defPerfPegging         ()),
      selSplit_            (defSelSplit            ()),
      nonMrSelSplit_       (defNonMrSelSplit       ()),
      selectionRecovery_   (defSelectionRecovery   ()),
      userHeurStart_       (defUserHeurStart       ()),
      modHeurAlloc_        (defModHeurAlloc        ()),
      pipSeqFromHeur_      (defPipSeqFromHeur      ()),
      newPipPggOrder_      (defNewPipPggOrder      ()),
      highPrecisionWD_     (defHighPrecisionWD     ()),
      nstnResidual_        (defNstnResidual        ()),
      minimalExcess_       (defMinimalExcess       ()),

      readDataMsgs_        (false),
      printIncAlloc_       (false),
      printEqAlloc_        (false),
      selPrintLevel_       (0),
      twmeByDemand_        (false),
      splitTol_            (1.0),
      splitRes_            (1.0),
      splitItrUB_          (20),
      inputPauses_         (false),
      prtPipMaxRatio_      (false),
      printDebug_          (false),
      printExcessVol_      (false),
      tempPar_             (* new WitPtrVec <WitString>),

      lotSizesExist_       (false),
      revCritList_         (myProblem ()),
      pgdCritList_         (),
      feasible_            (false)
   {
      // The following statement normally sets nTempPars to 1, but it can be
      // altered to a larger number for testing purposes:
      //
   setNTempPars (1);

   myCompMgr ()->insert (this);
   }

//------------------------------------------------------------------------------

WitGlobalComp::~WitGlobalComp ()
   {
   clearPgdCritList ();

   setNTempPars (0);

   delete & tempPar_;
   }

//------------------------------------------------------------------------------

const WitString & WitGlobalComp::tempPar (int index) const
   {
   checkTempParIndex (index);

   return tempPar_.myElemAt (index);
   }

//------------------------------------------------------------------------------

int WitGlobalComp::nTempPars () const
   {
   return tempPar_.length () - 1;
   }

//------------------------------------------------------------------------------
// "Set" functions.
//------------------------------------------------------------------------------

void WitGlobalComp::setTitle (const char * theValue)
   {
   witAssert (theValue != NULL);

   prepSetNonUnpostAttr ();

   title_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setNPeriods (int theValue)
   {
   witAssert (myCompMgr ()->nNodes () == 0);

   witAssert (theValue >= 1);

   prepSetUnpreAttr ();

   nPeriods_ = theValue;

   myProblem ()->updateNPeriods ();
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setIndependentOffsets (bool theValue)
   {
   witAssert (myCompMgr ()->nNodes () == 0);

   prepSetUnpreAttr ();

   independentOffsets_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setExecEmptyBom (bool theValue)
   {
   prepSetUnpreAttr ();

   execEmptyBom_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setHashTableSize (int theValue)
   {
   witAssert (theValue >= 0);

   prepSetNonUnpostAttr ();

   hashTableSize_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setRoundReqVols (bool theValue)
   {
   prepSetUnpostAttr ();

   roundReqVols_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setOutputPrecision (int theValue)
   {
   witAssert (theValue >= 0);

   prepSetNonUnpostAttr ();

   outputPrecision_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setComputeCriticalList (bool theValue)
   {
   prepSetUnpostAttr ();

   computeCriticalList_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setPgdCritListMode (bool theValue)
   {
   if (pgdCritListMode_)
      if (not theValue)
         clearPgdCritList ();

   prepSetUnpostAttr ();

   pgdCritListMode_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setWit34Compatible (bool theValue)
   {
   WitOperation * theOpn;

   prepSetUnpreAttr ();

   if (myCompMgr ()->nOperations () > 0)
      myMsgFac () ("setWit34CompatibleCpuWmsg");

   forEachOperation (theOpn, myProblem ())
      theOpn->setWit34Compatible (theValue);

   wit34Compatible_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setWit34Allowed (bool theValue)
   {
   prepSetUnpreAttr ();

   wit34Allowed_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setUseFocusHorizons (bool theValue)
   {
   prepSetNonUnpostAttr ();

   myProblem ()->myFSS ()->uncompute ();

   useFocusHorizons_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setLocalBuildAhead (bool theValue)
   {
   prepSetUnpostAttr ();

   localBuildAhead_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setMultiExec (bool theValue)
   {
   if (theValue)
      setLocalMultiExec  (false);

   if (not theValue)
      setTwoWayMultiExec (false);

   prepSetUnpreAttr ();

   multiExec_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setTwoWayMultiExec (bool theValue)
   {
   if (theValue)
      setMultiExec (true);

   prepSetUnpreAttr ();

   twoWayMultiExec_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setLocalMultiExec (bool theValue)
   {
   if (theValue)
      setMultiExec (false);

   prepSetUnpreAttr ();

   localMultiExec_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setEquitability (int theValue)
   {
   witAssert (theValue >=   1);
   witAssert (theValue <= 100);

   prepSetUnpostAttr ();

   equitability_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setForcedMultiEq (bool theValue)
   {
   prepSetUnpostAttr ();

   forcedMultiEq_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setAutoPriority (bool theValue)
   {
   prepSetUnpostAttr ();

   autoPriority_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setSkipFailures (bool theValue)
   {
   prepSetUnpostAttr ();

   skipFailures_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setMultiRoute (bool theValue)
   {
   prepSetUnpreAttr ();

   multiRoute_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setTruncOffsets (bool theValue)
   {
   prepSetUnpreAttr ();

   truncOffsets_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setLotSizeTol (double theValue)
   {
   witAssert (theValue >= 0.0);

   prepSetUnpostAttr ();

   lotSizeTol_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setExpCutoff (double theValue)
   {
   witAssert (theValue > 0.999E-6);

   prepSetUnpreAttr ();

   expCutoff_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setPenExec (bool theValue)
   {
   prepSetUnpostAttr ();

   penExec_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setTieBreakPropRt (bool theValue)
   {
   prepSetUnpostAttr ();

   tieBreakPropRt_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setStockRealloc (bool theValue)
   {
   if (theValue)
      witAssert (not stockReallocation_);

   prepSetUnpostAttr ();

   stockRealloc_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setStockReallocation (bool theValue)
   {
   if (theValue)
      witAssert (not stockRealloc_);

   prepSetUnpostAttr ();

   stockReallocation_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setSrSelSplit (bool theValue)
   {
   prepSetUnpostAttr ();

   srSelSplit_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setRespectStockSLBs (bool theValue)
   {
   prepSetUnpostAttr ();

   respectStockSLBs_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setPrefHighStockSLBs (bool theValue)
   {
   prepSetUnpostAttr ();

   prefHighStockSLBs_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setPerfPegging (bool theValue)
   {
   prepSetUnpostAttr ();

   if (perfPegging_ == theValue)
      return;

   perfPegging_ = theValue;

   if (theValue)
      myProblem ()->myHeurAllMgr ()->setUpPegging ();
   else
      myProblem ()->myHeurAllMgr ()->shutDownPegging ();
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setSelSplit (bool theValue)
   {
   prepSetUnpostAttr ();

   if (theValue)
      nonMrSelSplit_ = false;

   selSplit_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setNonMrSelSplit (bool theValue)
   {
   prepSetUnpostAttr ();

   if (theValue)
      selSplit_ = false;

   nonMrSelSplit_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setSelectionRecovery (bool theValue)
   {
   prepSetUnpostAttr ();

   selectionRecovery_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setUserHeurStart (bool theValue)
   {
   prepSetUnpostAttr ();

   userHeurStart_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setModHeurAlloc (bool theValue)
   {
   prepSetUnpostAttr ();

   modHeurAlloc_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setPipSeqFromHeur (bool theValue)
   {
   prepSetUnpostAttr ();

   pipSeqFromHeur_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setNewPipPggOrder (bool theValue)
   {
   prepSetNonUnpostAttr ();

   myProblem ()->myPipMgr ()->clearPegging ();

   newPipPggOrder_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setHighPrecisionWD (bool theValue)
   {
   prepSetUnpostAttr ();

   highPrecisionWD_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setNstnResidual (bool theValue)
   {
   prepSetUnpostAttr ();

   nstnResidual_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setMinimalExcess (bool theValue)
   {
   prepSetUnpostAttr ();

   minimalExcess_ = theValue;
   }

//------------------------------------------------------------------------------
// Other functions.
//------------------------------------------------------------------------------

void WitGlobalComp::setReadDataMsgs (bool theValue)
   {
   readDataMsgs_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setPrintIncAlloc (bool theValue)
   {
   printIncAlloc_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setPrintEqAlloc (bool theValue)
   {
   printEqAlloc_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setSelPrintLevel (int theValue)
   {
   selPrintLevel_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setTwmeByDemand (bool theValue)
   {
   twmeByDemand_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setSplitTol (double theValue)
   {
   splitTol_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setSplitRes (double theValue)
   {
   splitRes_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setSplitItrUB (int theValue)
   {
   splitItrUB_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setInputPauses (bool theValue)
   {
   inputPauses_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setPrtPipMaxRatio (bool theValue)
   {
   prtPipMaxRatio_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setPrintDebug (bool theValue)
   {
   printDebug_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setPrintExcessVol (bool theValue)
   {
   printExcessVol_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setTempPar (int index, const WitString & theValue)
   {
   checkTempParIndex (index);

   tempPar_.myElemAt (index) = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::storePreLotSizesExist (bool theValue)
   {
   witAssert (myPreprocessor ()->preprocessing ());

   lotSizesExist_ = theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::storePostFeasible (bool theValue)
   {
   witAssert (myPostprocessor ()->postprocessing ());

   feasible_ = theValue;
   }

//------------------------------------------------------------------------------

bool WitGlobalComp::inserted ()
   {
   return (myCompMgr ()->myGlobalComp () == this);
   }

//------------------------------------------------------------------------------

void WitGlobalComp::copySelfInto (WitCompMgr * theCompMgr)
   {
   theCompMgr->myGlobalComp ()->copyAttrsFrom (this);
   }

//------------------------------------------------------------------------------

void WitGlobalComp::recount ()
   {
   myCompMgr ()->recountGlobalComp (this);
   }

//------------------------------------------------------------------------------

void WitGlobalComp::unpreprocess ()
   {
   lotSizesExist_ = false;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::clearOptSoln  ()
   {                              
   witAssert (myDetOptImpMgr ()->clearingSoln ());

   revCritList_.clear ();
   }

//------------------------------------------------------------------------------

void WitGlobalComp::clearStochSoln ()
   {
   WitComponent::clearStochSoln ();

   feasible_ = false;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::display ()
   {
   myMsgFac () ("globalComp1DdMsg",
      multiExecNeeded (),
      twoWayMultiExec (),
      equitability    (),
      forcedMultiEq   (),
      userHeurStart   (),
      modHeurAlloc    (),
      autoPriority    (),
      skipFailures    (),
      multiRoute      (),
      penExec         (),
      tieBreakPropRt  ());

   myMsgFac () ("globalComp2DdMsg",
      stockReallocation (),
      selSplit          (),
      selectionRecovery (),
      respectStockSLBs  (),
      prefHighStockSLBs (),
      perfPegging       (),
      pipSeqFromHeur    (),
      newPipPggOrder    (),
      truncOffsets      (),
      highPrecisionWD   (),
      nstnResidual      (),
      minimalExcess     (),
      lotSizeTol        (),
      expCutoff         ());
   }

//------------------------------------------------------------------------------

bool WitGlobalComp::multiExecNeeded ()
   {
   return (multiExec_ or localMultiExec_);
   }

//------------------------------------------------------------------------------

void WitGlobalComp::unpostprocess ()
   {
   feasible_ = false;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::clearRevCritListForHeur ()
   {
   witAssert (myHeurAllMgr ()->updatingSoln ());

   revCritList_.clear ();
   }

//------------------------------------------------------------------------------

void WitGlobalComp::addToRevCritListForHeur (
      WitPart * thePart,
      WitPeriod thePer)
   {
   witAssert (myHeurAllMgr ()->updatingSoln ());

   revCritList_.push (thePart, thePer);
   }

//------------------------------------------------------------------------------

void WitGlobalComp::receiveOptRevCritList (WitPartPerStack & thePartPerStack)
   {
   witAssert (myDetOptImpMgr ()->uploadingSoln ());

   revCritList_.takeContentsFrom (thePartPerStack);
   }

//------------------------------------------------------------------------------

void WitGlobalComp::writePdgCritList ()
   {
   WitPclEl * thePclEl;

   stronglyAssert (devMode ());

   fprintf (solnFile (),
      "\n"
      "Pegged Critical List:\n"
      "\n"
      "---------------------------\n"
      "Crit  Crit   Dem  Dem  Ship\n"
      "Part   Per  Part        Per\n"
      "---------------------------\n"
      );

   forEachEl (thePclEl, pgdCritList_)
      fprintf (solnFile (),
         "%4s   %3d    %2s  %3s   %3d\n",
         thePclEl->critPart ()->partName ()        .myCstring (),
         thePclEl->critPer (),
         thePclEl->myDemand ()->demandedPartName ().myCstring (),
         thePclEl->myDemand ()->demandName ()      .myCstring (),
         thePclEl->myShipPer ());
   }

//------------------------------------------------------------------------------

void WitGlobalComp::clearPgdCritList ()
   {
   while (not pgdCritList_.isEmpty ())
      delete pgdCritList_.get ();
   }

//------------------------------------------------------------------------------

void WitGlobalComp::appendToPgdCritListForHeur (WitPclEl * thePclEl)
   {
   witAssert (myHeurAllMgr ()->updatingSoln ());

   pgdCritList_.append (thePclEl);
   }

//------------------------------------------------------------------------------

bool WitGlobalComp::tempParIsSet (int index)
   {
   checkTempParIndex (index);

   return (tempPar (index) != "");
   }

//------------------------------------------------------------------------------

bool WitGlobalComp::tempParAsBool (int index)
   {
   checkTempParIndex (index);

   if (tempPar (index) == "yes")
      return true;

   if (tempPar (index) == "no")
      return false;

   myMsgFac () ("invalidBoolTempParSmsg", index, tempPar (index));

   return false;
   }

//------------------------------------------------------------------------------

int WitGlobalComp::tempParAsInt (int index)
   {
   int theValue;

   checkTempParIndex (index);

   if (sscanf (tempPar (index).myCstring (), "%d", & theValue) != 1)
      myMsgFac () ("invalidIntTempParSmsg", index, tempPar (index));

   return theValue;
   }

//------------------------------------------------------------------------------

double WitGlobalComp::tempParAsDouble (int index)
   {
   double theValue;

   checkTempParIndex (index);

   if (sscanf (tempPar (index).myCstring (), "%lg", & theValue) != 1)
      myMsgFac () ("invalidDoubleTempParSmsg", index, tempPar (index));

   return theValue;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::setNTempPars (int theValue)
   {
   int idx;

   for (idx = 1; idx <= nTempPars (); idx ++)
      delete tempPar_[idx];

   tempPar_.resize (theValue + 1);

   for (idx = 1; idx <= nTempPars (); idx ++)
      tempPar_[idx] = new WitString;
   }

//------------------------------------------------------------------------------

void WitGlobalComp::checkTempParIndex (int i) const
   {
   stronglyAssert (i >= 1);
   stronglyAssert (i <= nTempPars ());
   }

//------------------------------------------------------------------------------

void WitGlobalComp::writeDataID ()
   {
   myDataWriter ()->writeFirstKeyWord ("set");
   myDataWriter ()->writeKeyWord      ("problem");
   }

//------------------------------------------------------------------------------

void WitGlobalComp::writeDataAttrs ()
   {
   WitComponent::writeDataAttrs ();

   myDataWriter ()->writeString (
        "title",
         title (),
      defTitle ());

   myDataWriter ()->writeInt (
        "nPeriods",
         nPeriods (),
      defNPeriods ());

   myDataWriter ()->writeBool (
        "independentOffsets",
         independentOffsets (),
      defIndependentOffsets ());

   myDataWriter ()->writeBool (
        "execEmptyBom",
         execEmptyBom (),
      defExecEmptyBom ());

   myDataWriter ()->writeInt (
        "hashTableSize",
         hashTableSize (),
      defHashTableSize ());

   myDataWriter ()->writeBool (
        "roundReqVols",
         roundReqVols (),
      defRoundReqVols ());

   myDataWriter ()->writeInt (
        "outputPrecision",
         outputPrecision (),
      defOutputPrecision ());

   myDataWriter ()->writeBool (
        "computeCriticalList",
         computeCriticalList (),
      defComputeCriticalList ());

   myDataWriter ()->writeBool (
        "pgdCritListMode",
         pgdCritListMode (),
      defPgdCritListMode ());

   myDataWriter ()->writeBool (
        "wit34Compatible",
         wit34Compatible (),
      defWit34Compatible ());

   myDataWriter ()->writeBool (
        "useFocusHorizons",
         useFocusHorizons (),
      defUseFocusHorizons ());

   myDataWriter ()->writeBool (
        "localBuildAhead",
         localBuildAhead (),
      defLocalBuildAhead ());

   myDataWriter ()->writeBool (
        "multiExec",
         multiExec (),
      defMultiExec ());

   myDataWriter ()->writeBool (
        "twoWayMultiExec",
         twoWayMultiExec (),
      defTwoWayMultiExec ());

   myDataWriter ()->writeBool (
        "localMultiExec",
         localMultiExec (),
      defLocalMultiExec ());

   myDataWriter ()->writeInt (
        "equitability",
         equitability (),
      defEquitability ());

   myDataWriter ()->writeBool (
        "forcedMultiEq",
         forcedMultiEq (),
      defForcedMultiEq ());

   myDataWriter ()->writeBool (
        "autoPriority",
         autoPriority (),
      defAutoPriority ());

   myDataWriter ()->writeBool (
        "skipFailures",
         skipFailures (),
      defSkipFailures ());

   myDataWriter ()->writeBool (
        "multiRoute",
         multiRoute (),
      defMultiRoute ());

   myDataWriter ()->writeBool (
        "truncOffsets",
         truncOffsets (),
      defTruncOffsets ());

   myDataWriter ()->writeDouble (
        "lotSizeTol",
         lotSizeTol (),
      defLotSizeTol ());

   myDataWriter ()->writeDouble (
        "expCutoff",
         expCutoff (),
      defExpCutoff ());

   myDataWriter ()->writeBool (
        "penExec",
         penExec (),
      defPenExec ());

   myDataWriter ()->writeBool (
        "tieBreakPropRt",
         tieBreakPropRt (),
      defTieBreakPropRt ());

   myDataWriter ()->writeBool (
        "stockRealloc",
         stockRealloc (),
      defStockRealloc ());

   myDataWriter ()->writeBool (
        "stockReallocation",
         stockReallocation (),
      defStockReallocation ());

   myDataWriter ()->writeBool (
        "srSelSplit",
         srSelSplit (),
      defSrSelSplit ());

   myDataWriter ()->writeBool (
        "respectStockSLBs",
         respectStockSLBs (),
      defRespectStockSLBs ());

   myDataWriter ()->writeBool (
        "prefHighStockSLBs",
         prefHighStockSLBs (),
      defPrefHighStockSLBs ());

   myDataWriter ()->writeBool (
        "perfPegging",
         perfPegging (),
      defPerfPegging ());

   myDataWriter ()->writeBool (
        "selSplit",
         selSplit (),
      defSelSplit ());

   myDataWriter ()->writeBool (
        "nonMrSelSplit",
         nonMrSelSplit (),
      defNonMrSelSplit ());

   myDataWriter ()->writeBool (
        "selectionRecovery",
         selectionRecovery (),
      defSelectionRecovery ());

   myDataWriter ()->writeBool (
        "userHeurStart",
         userHeurStart (),
      defUserHeurStart ());

   myDataWriter ()->writeBool (
        "modHeurAlloc",
         modHeurAlloc (),
      defModHeurAlloc ());

   myDataWriter ()->writeBool (
        "pipSeqFromHeur",
         pipSeqFromHeur (),
      defPipSeqFromHeur ());

   myDataWriter ()->writeBool (
        "newPipPggOrder",
         newPipPggOrder (),
      defNewPipPggOrder ());

   myDataWriter ()->writeBool (
        "highPrecisionWD",
         highPrecisionWD (),
      defHighPrecisionWD ());

   myDataWriter ()->writeBool (
        "nstnResidual",
         nstnResidual (),
      defNstnResidual ());

   myDataWriter ()->writeBool (
        "minimalExcess",
         minimalExcess (),
      defMinimalExcess ());
   }

//------------------------------------------------------------------------------

void WitGlobalComp::copyAttrsFrom (WitGlobalComp * theGlobalComp)
   {
   WitComponent::copyAttrsFrom (theGlobalComp);

   title_               = theGlobalComp->title_;
   nPeriods_            = theGlobalComp->nPeriods_;
   independentOffsets_  = theGlobalComp->independentOffsets_;
   execEmptyBom_        = theGlobalComp->execEmptyBom_;
   hashTableSize_       = theGlobalComp->hashTableSize_;
   roundReqVols_        = theGlobalComp->roundReqVols_;
   outputPrecision_     = theGlobalComp->outputPrecision_;
   computeCriticalList_ = theGlobalComp->computeCriticalList_;
   wit34Allowed_        = theGlobalComp->wit34Allowed_;
   localBuildAhead_     = theGlobalComp->localBuildAhead_;
   multiExec_           = theGlobalComp->multiExec_;
   twoWayMultiExec_     = theGlobalComp->twoWayMultiExec_;
   localMultiExec_      = theGlobalComp->localMultiExec_;
   equitability_        = theGlobalComp->equitability_;
   forcedMultiEq_       = theGlobalComp->forcedMultiEq_;
   autoPriority_        = theGlobalComp->autoPriority_;
   skipFailures_        = theGlobalComp->skipFailures_;
   multiRoute_          = theGlobalComp->multiRoute_;
   truncOffsets_        = theGlobalComp->truncOffsets_;
   lotSizeTol_          = theGlobalComp->lotSizeTol_;
   expCutoff_           = theGlobalComp->expCutoff_;
   penExec_             = theGlobalComp->penExec_;
   tieBreakPropRt_      = theGlobalComp->tieBreakPropRt_;
   stockRealloc_        = theGlobalComp->stockRealloc_;
   stockReallocation_   = theGlobalComp->stockReallocation_;
   srSelSplit_          = theGlobalComp->srSelSplit_;
   respectStockSLBs_    = theGlobalComp->respectStockSLBs_;
   prefHighStockSLBs_   = theGlobalComp->prefHighStockSLBs_;
   selSplit_            = theGlobalComp->selSplit_;
   nonMrSelSplit_       = theGlobalComp->nonMrSelSplit_;
   selectionRecovery_   = theGlobalComp->selectionRecovery_;
   userHeurStart_       = theGlobalComp->userHeurStart_;
   modHeurAlloc_        = theGlobalComp->modHeurAlloc_;
   pipSeqFromHeur_      = theGlobalComp->pipSeqFromHeur_;
   newPipPggOrder_      = theGlobalComp->newPipPggOrder_;
   highPrecisionWD_     = theGlobalComp->highPrecisionWD_;
   nstnResidual_        = theGlobalComp->nstnResidual_;
   minimalExcess_       = theGlobalComp->minimalExcess_;

   setPgdCritListMode    (theGlobalComp->pgdCritListMode_);
   setUseFocusHorizons   (theGlobalComp->useFocusHorizons_);
   setPerfPegging        (theGlobalComp->perfPegging_);

   setWit34Compatible (theGlobalComp->wit34Compatible_);

   myProblem ()->
      myPipMgr ()->
         copyShipSeq (theGlobalComp->myProblem ()->myPipMgr ());

   myProblem ()->updateNPeriods ();
   }
