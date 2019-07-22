//------------------------------------------------------------------------------
// WIT-J C++ Source File AttBldr.C.
//
// Contains the implementation of class AttBldr.
//------------------------------------------------------------------------------

#include    <AttBldr.h>
#include <ScalarAtts.h>
#include <VectorAtts.h>
#include  <StringJOR.h>

#include   <iostream>

#include     <stdlib.h>

//------------------------------------------------------------------------------

WitJ::AttBldr::AttBldr (Coordinator * theCoord):

      JavaAccObj     (theCoord),
      mostRecentAtt_ (NULL)
   {
   scopedAttName_.reserve (100);

   buildAtts ();
   }

//------------------------------------------------------------------------------

WitJ::AttBldr::~AttBldr ()
   {
   while (! myAttMap_.empty ())
      {
      delete myAttMap_.begin ()->second;

      myAttMap_.erase (myAttMap_.begin ());
      }
   }

//------------------------------------------------------------------------------

WitJ::AttBldr * WitJ::AttBldr::asAttBldr ()
   {
   return this;
   }

//------------------------------------------------------------------------------

void WitJ::AttBldr::acquireAtt (Att * theAtt)
   {
   scopedAttName_  = theAtt->getCompClassName ();

   scopedAttName_ += ".";

   scopedAttName_ += theAtt->getAttName ();

   if (myAttMap_.count (scopedAttName_) > 0)
      dupAttError (theAtt);

   myAttMap_[scopedAttName_] = theAtt;

   if (mostRecentAtt_ != NULL)
      if (theAtt->getAttName () < mostRecentAtt_->getAttName ())
         {
         outOfOrderError (theAtt);
         }

   mostRecentAtt_ = theAtt;
   }

//------------------------------------------------------------------------------

void WitJ::AttBldr::dupAttError (Att * theAtt)
   {
   std::cout
      << "\n"
      << "WIT-J Internal Error:\n"
      << "\n"
      << "   Two or more C++ Atts have the same scoped name:\n"
      << "\n"
      << "      " << theAtt->getCompClassName () << "."
                  << theAtt->getAttName       () << "\n"
      << "\n"
      << "Application Program aborted by WIT-J.\n";

   EXCEPTION_ASSERT (false);
   }

//------------------------------------------------------------------------------

void WitJ::AttBldr::outOfOrderError (Att * theAtt)
   {
   std::cout
      << "\n"
      << "WIT-J Internal Error:\n"
      << "\n"
      << "   C++ Atts entered out-of-order:\n"
      << "\n"
      << "      " << mostRecentAtt_->getAttName () << "\n"
      << "      " << theAtt        ->getAttName () << "\n"
      << "\n"
      << "Application Program aborted by WIT-J.\n";

   EXCEPTION_ASSERT (false);
   }

//------------------------------------------------------------------------------

bool WitJ::AttBldr::hasAttFor (const char * scopedAttNameCStr)
   {
   return (myAttMap_.count (scopedAttNameCStr) != 0);
   }

//------------------------------------------------------------------------------

WitJ::Att * WitJ::AttBldr::handOverAttFor (const char * scopedAttNameCStr)
   {
   std::map <std::string, Att *>::iterator theItr;
   Att *                                   theAtt;

   scopedAttName_ = scopedAttNameCStr;

   theItr = myAttMap_.find (scopedAttName_);

   EXCEPTION_ASSERT (theItr != myAttMap_.end ());

   theAtt = theItr->second;

   myAttMap_.erase (theItr);

   return theAtt;
   }

//------------------------------------------------------------------------------

void WitJ::AttBldr::getAllAttNames (StringJOR & theNamesJOR)
   {
   std::string                             theNames;
   std::map <std::string, Att *>::iterator theItr;

   theNames = "";

   for (theItr  = myAttMap_.begin ();
        theItr != myAttMap_.end ();
        theItr ++)
      {
      if (! theNames.empty ())
         {
         theNames += ", ";
         }

      theNames += theItr->first;
      }

   theNamesJOR.makeJavaString (theNames.c_str ());
   }

//------------------------------------------------------------------------------

void WitJ::AttBldr::buildAtts ()
   {
   new   ProbScalarAtt <int>    (this,                  "accAfterOptImp",
                                                   witGetAccAfterOptImp,
                                                   witSetAccAfterOptImp);

   new   ProbScalarAtt <int>    (this,                  "accAfterSoftLB",
                                                   witGetAccAfterSoftLB,
                                                   witSetAccAfterSoftLB);

   new   ProbScalarAtt <int>    (this,                  "accelerated",
                                                   witGetAccelerated);

   new   PartScalarAtt <int>    (this,                  "asapPipOrder",
                                               witGetPartAsapPipOrder,
                                               witSetPartAsapPipOrder);

   new   ProbScalarAtt <int>    (this,                  "autoPriority",
                                                   witGetAutoPriority,
                                                   witSetAutoPriority);

   new   PartScalarAtt <int>    (this,                  "boundedLeadTimes",
                                               witGetPartBoundedLeadTimes,
                                               witSetPartBoundedLeadTimes);

   new   ProbScalarAtt <double> (this,                  "boundsValue",
                                                   witGetBoundsValueDbl);

   new   PartVectorAtt <int>    (this,                  "buildAheadUB",
                                               witGetPartBuildAheadUB,
                                               witSetPartBuildAheadUB);

   new   PartScalarAtt <int>    (this,                  "buildAsap",
                                               witGetPartBuildAsap,
                                               witSetPartBuildAsap);

   new   PartScalarAtt <int>    (this,                  "buildNstn",
                                               witGetPartBuildNstn,
                                               witSetPartBuildNstn);

   new   ProbScalarAtt <int>    (this,                  "compPrices",
                                                   witGetCompPrices,
                                                   witSetCompPrices);

   new   ProbScalarAtt <int>    (this,                  "computeCriticalList",
                                                   witGetComputeCriticalList,
                                                   witSetComputeCriticalList);

   new BomEntVectorAtt <double> (this,                  "consRate",
                                           witGetBomEntryConsRateDbl,
                                           witSetBomEntryConsRateDbl);

   new    SubVectorAtt <double> (this,                  "consRate",
                                       witGetSubsBomEntryConsRateDbl,
                                       witSetSubsBomEntryConsRateDbl);

   new   PartVectorAtt <double> (this,                  "consVol",
                                               witGetPartConsVolDbl);

   new   ProbScalarAtt <int>    (this,                  "cplexEmbedded",
                                                   witGetCplexEmbedded);

   new   ProbScalarAtt <double> (this,                  "cplexMipBound",
                                                   witGetCplexMipBoundDbl);

   new   ProbScalarAtt <double> (this,                  "cplexMipRelGap",
                                                   witGetCplexMipRelGapDbl);

   new   ProbScalarAtt <double> (this,                  "cplexParSpecDblVal",
                                                   witGetCplexParSpecDblValDbl,
                                                   witSetCplexParSpecDblValDbl);

   new   ProbScalarAtt <int>    (this,                  "cplexParSpecIntVal",
                                                   witGetCplexParSpecIntVal,
                                                   witSetCplexParSpecIntVal);

   new   ProbScalarAttString    (this,                  "cplexParSpecName",
                                                   witGetCplexParSpecName,
                                                   witSetCplexParSpecName);

   new   ProbScalarAtt <int>    (this,                  "cplexStatusCode",
                                                   witGetCplexStatusCode);

   new   ProbScalarAttString    (this,                  "cplexStatusText",
                                                   witGetCplexStatusText);

   new DemandVectorAtt <double> (this,                  "cumShipReward",
                                             witGetDemandCumShipRewardDbl,
                                             witSetDemandCumShipRewardDbl);

   new   ProbScalarAttString    (this,                  "currentObjective",
                                                   witGetCurrentObjective,
                                                   witSetCurrentObjective);

   new   ProbScalarAtt <int>    (this,                  "currentScenario",
                                                   witGetCurrentScenario,
                                                   witSetCurrentScenario);

   new DemandVectorAtt <double> (this,                  "demandVol",
                                             witGetDemandDemandVolDbl,
                                             witSetDemandDemandVolDbl);

   new BomEntScalarAtt <int>    (this,                  "earliestPeriod",
                                           witGetBomEntryEarliestPeriod,
                                           witSetBomEntryEarliestPeriod);

   new BopEntScalarAtt <int>    (this,                  "earliestPeriod",
                                           witGetBopEntryEarliestPeriod,
                                           witSetBopEntryEarliestPeriod);

   new    SubScalarAtt <int>    (this,                  "earliestPeriod",
                                       witGetSubsBomEntryEarliestPeriod,
                                       witSetSubsBomEntryEarliestPeriod);

   new   ProbScalarAtt <int>    (this,                  "equitability",
                                                   witGetEquitability,
                                                   witSetEquitability);

   new   PartVectorAtt <double> (this,                  "excessVol",
                                               witGetPartExcessVolDbl);

   new    OpnVectorAtt <double> (this,                  "execCost",
                                          witGetOperationExecCostDbl,
                                          witSetOperationExecCostDbl);

   new   ProbScalarAtt <int>    (this,                  "execEmptyBom",
                                                   witGetExecEmptyBom,
                                                   witSetExecEmptyBom);

   new BomEntScalarAtt <double> (this,                  "execPenalty",
                                           witGetBomEntryExecPenaltyDbl,
                                           witSetBomEntryExecPenaltyDbl);

   new    OpnScalarAtt <double> (this,                  "execPenalty",
                                          witGetOperationExecPenaltyDbl,
                                          witSetOperationExecPenaltyDbl);

   new    SubScalarAtt <double> (this,                  "execPenalty",
                                       witGetSubsBomEntryExecPenaltyDbl,
                                       witSetSubsBomEntryExecPenaltyDbl);

   new    OpnVectorAtt <double> (this,                  "execVol",
                                          witGetOperationExecVolDbl,
                                          witSetOperationExecVolDbl);

   new    OpnVectorAtt <int>    (this,                  "executable",
                                          witGetOperationExecutable);

   new BopEntScalarAtt <int>    (this,                  "expAllowed",
                                           witGetBopEntryExpAllowed,
                                           witSetBopEntryExpAllowed);

   new    SubScalarAtt <int>    (this,                  "expAllowed",
                                       witGetSubsBomEntryExpAllowed,
                                       witSetSubsBomEntryExpAllowed);

   new BopEntScalarAtt <double> (this,                  "expAversion",
                                           witGetBopEntryExpAversionDbl,
                                           witSetBopEntryExpAversionDbl);

   new   ProbScalarAtt <double> (this,                  "expCutoff",
                                                   witGetExpCutoffDbl,
                                                   witSetExpCutoffDbl);

   new    SubScalarAtt <double> (this,                  "expNetAversion",
                                       witGetSubsBomEntryExpNetAversionDbl,
                                       witSetSubsBomEntryExpNetAversionDbl);

   new   ProbScalarAtt <int>    (this,                  "extOptActive",
                                                   witGetExtOptActive);

   new BomEntScalarAtt <double> (this,                  "falloutRate",
                                           witGetBomEntryFalloutRateDbl,
                                           witSetBomEntryFalloutRateDbl);

   new    SubScalarAtt <double> (this,                  "falloutRate",
                                       witGetSubsBomEntryFalloutRateDbl,
                                       witSetSubsBomEntryFalloutRateDbl);

   new   ProbScalarAtt <int>    (this,                  "feasible",
                                                   witGetFeasible);

   new DemandScalarAtt <int>    (this,                  "focusHorizon",
                                             witGetDemandFocusHorizon,
                                             witSetDemandFocusHorizon);

   new   PartVectorAtt <double> (this,                  "focusShortageVol",
                                               witGetPartFocusShortageVolDbl);

   new   ProbScalarAtt <int>    (this,                  "forcedMultiEq",
                                                   witGetForcedMultiEq,
                                                   witSetForcedMultiEq);

   new    OpnVectorAtt <double> (this,                  "fssExecVol",
                                          witGetOperationFssExecVolDbl);

   new DemandVectorAtt <double> (this,                  "fssShipVol",
                                             witGetDemandFssShipVolDbl,
                                             witSetDemandFssShipVolDbl);

   new    SubVectorAtt <double> (this,                  "fssSubVol",
                                       witGetSubsBomEntryFssSubVolDbl);

   new   ProbScalarAtt <int>    (this,                  "heurAllocActive",
                                                   witGetHeurAllocActive);

   new   ProbScalarAtt <int>    (this,                  "highPrecisionWD",
                                                   witGetHighPrecisionWD,
                                                   witSetHighPrecisionWD);

   new BomEntVectorAtt <int>    (this,                  "impactPeriod",
                                           witGetBomEntryImpactPeriod);

   new BopEntVectorAtt <int>    (this,                  "impactPeriod",
                                           witGetBopEntryImpactPeriod);

   new    SubVectorAtt <int>    (this,                  "impactPeriod",
                                       witGetSubsBomEntryImpactPeriod);

   new    OpnVectorAtt <double> (this,                  "incLotSize",
                                          witGetOperationIncLotSizeDbl,
                                          witSetOperationIncLotSizeDbl);

   new    OpnVectorAtt <double> (this,                  "incLotSize2",
                                          witGetOperationIncLotSize2Dbl,
                                          witSetOperationIncLotSize2Dbl);

   new   ProbScalarAtt <int>    (this,                  "independentOffsets",
                                                   witGetIndependentOffsets,
                                                   witSetIndependentOffsets);

   new    OpnScalarAtt <int>    (this,                  "intExecVols",
                                          witGetOperationIntExecVols,
                                          witSetOperationIntExecVols);

   new DemandScalarAtt <int>    (this,                  "intShipVols",
                                             witGetDemandIntShipVols,
                                             witSetDemandIntShipVols);

   new    SubScalarAtt <int>    (this,                  "intSubVols",
                                       witGetSubsBomEntryIntSubVols,
                                       witSetSubsBomEntryIntSubVols);

   new BomEntScalarAtt <int>    (this,                  "latestPeriod",
                                           witGetBomEntryLatestPeriod,
                                           witSetBomEntryLatestPeriod);

   new BopEntScalarAtt <int>    (this,                  "latestPeriod",
                                           witGetBopEntryLatestPeriod,
                                           witSetBopEntryLatestPeriod);

   new    SubScalarAtt <int>    (this,                  "latestPeriod",
                                       witGetSubsBomEntryLatestPeriod,
                                       witSetSubsBomEntryLatestPeriod);

   new DemandVectorAtt <int>    (this,                  "leadTimeUB",
                                             witGetDemandLeadTimeUB,
                                             witSetDemandLeadTimeUB);

   new    OpnVectorAtt <double> (this,                  "lotSize2Thresh",
                                          witGetOperationLotSize2ThreshDbl,
                                          witSetOperationLotSize2ThreshDbl);

   new   ProbScalarAtt <double> (this,                  "lotSizeTol",
                                                   witGetLotSizeTolDbl,
                                                   witSetLotSizeTolDbl);

   new BomEntScalarAtt <int>    (this,                  "mandEC",
                                           witGetBomEntryMandEC,
                                           witSetBomEntryMandEC);

   new    OpnVectorAtt <double> (this,                  "minLotSize",
                                          witGetOperationMinLotSizeDbl,
                                          witSetOperationMinLotSizeDbl);

   new    OpnVectorAtt <double> (this,                  "minLotSize2",
                                          witGetOperationMinLotSize2Dbl,
                                          witSetOperationMinLotSize2Dbl);

   new   ProbScalarAtt <int>    (this,                  "minimalExcess",
                                                   witGetMinimalExcess,
                                                   witSetMinimalExcess);

   new   ProbScalarAtt <int>    (this,                  "mipMode",
                                                   witGetMipMode,
                                                   witSetMipMode);

   new   ProbScalarAtt <int>    (this,                  "modHeurAlloc",
                                                   witGetModHeurAlloc,
                                                   witSetModHeurAlloc);

   new   PartVectorAtt <double> (this,                  "mrpConsVol",
                                               witGetPartMrpConsVolDbl);

   new   PartVectorAtt <double> (this,                  "mrpExcessVol",
                                               witGetPartMrpExcessVolDbl);

   new    OpnVectorAtt <double> (this,                  "mrpExecVol",
                                          witGetOperationMrpExecVolDbl);

   new    SubScalarAtt <int>    (this,                  "mrpNetAllowed",
                                       witGetSubsBomEntryMrpNetAllowed,
                                       witSetSubsBomEntryMrpNetAllowed);

   new   PartVectorAtt <double> (this,                  "mrpResidualVol",
                                               witGetPartMrpResidualVolDbl);

   new    SubVectorAtt <double> (this,                  "mrpSubVol",
                                       witGetSubsBomEntryMrpSubVolDbl);

   new   ProbScalarAtt <int>    (this,                  "multiExec",
                                                   witGetMultiExec,
                                                   witSetMultiExec);

   new   ProbScalarAtt <int>    (this,                  "multiObjMode",
                                                   witGetMultiObjMode,
                                                   witSetMultiObjMode);

   new   ProbScalarAtt <double> (this,                  "multiObjTol",
                                                   witGetMultiObjTolDbl,
                                                   witSetMultiObjTolDbl);

   new   ProbScalarAtt <int>    (this,                  "multiRoute",
                                                   witGetMultiRoute,
                                                   witSetMultiRoute);

   new   ProbScalarAtt <int>    (this,                  "nScenarios",
                                                   witGetNScenarios,
                                                   witSetNScenarios);

   new    SubScalarAtt <int>    (this,                  "netAllowed",
                                       witGetSubsBomEntryNetAllowed,
                                       witSetSubsBomEntryNetAllowed);

   new   ProbScalarAtt <int>    (this,                  "nstnResidual",
                                                   witGetNstnResidual,
                                                   witSetNstnResidual);

   new   ProbScalarAtt <double> (this,                  "objValue",
                                                   witGetObjValueDbl);

   new    OpnScalarAtt <int>    (this,                  "objectStage",
                                          witGetOperationObjectStage,
                                          witSetOperationObjectStage);

   new   PartScalarAtt <int>    (this,                  "objectStage",
                                               witGetPartObjectStage,
                                               witSetPartObjectStage);

   new   ProbScalarAttString    (this,                  "objectiveListSpec",
                                                   witGetObjectiveListSpec,
                                                   witSetObjectiveListSpec);

   new   ProbScalarAtt <int>    (this,                  "objectiveSeqNo",
                                                   witGetObjectiveSeqNo,
                                                   witSetObjectiveSeqNo);

   new BomEntVectorAtt <double> (this,                  "offset",
                                           witGetBomEntryOffsetDbl,
                                           witSetBomEntryOffsetDbl);

   new BopEntVectorAtt <double> (this,                  "offset",
                                           witGetBopEntryOffsetDbl,
                                           witSetBopEntryOffsetDbl);

   new    SubVectorAtt <double> (this,                  "offset",
                                       witGetSubsBomEntryOffsetDbl,
                                       witSetSubsBomEntryOffsetDbl);

   new   ProbScalarAtt <int>    (this,                  "penExec",
                                                   witGetPenExec,
                                                   witSetPenExec);

   new   ProbScalarAtt <int>    (this,                  "perfPegging",
                                                   witGetPerfPegging,
                                                   witSetPerfPegging);

   new   ProbVectorAtt <int>    (this,                  "periodStage",
                                                   witGetPeriodStage,
                                                   witSetPeriodStage);

   new   ProbScalarAtt <int>    (this,                  "pgdCritListMode",
                                                   witGetPgdCritListMode,
                                                   witSetPgdCritListMode);

   new    OpnScalarAtt <int>    (this,                  "pipEnabled",
                                          witGetOperationPipEnabled,
                                          witSetOperationPipEnabled);

   new   ProbScalarAtt <int>    (this,                  "pipExists",
                                                   witGetPipExists);

   new    OpnScalarAtt <int>    (this,                  "pipRank",
                                          witGetOperationPipRank,
                                          witSetOperationPipRank);

   new   ProbScalarAtt <int>    (this,                  "pipSeqFromHeur",
                                                   witGetPipSeqFromHeur,
                                                   witSetPipSeqFromHeur);

   new   ProbScalarAtt <int>    (this,                  "postprocessed",
                                                   witGetPostprocessed);

   new   ProbScalarAtt <int>    (this,                  "prefHighStockSLBs",
                                                   witGetPrefHighStockSLBs,
                                                   witSetPrefHighStockSLBs);

   new   ProbScalarAtt <int>    (this,                  "preprocessed",
                                                   witGetPreprocessed);

   new DemandVectorAtt <int>    (this,                  "priority",
                                             witGetDemandPriority,
                                             witSetDemandPriority);

   new   ProbScalarAtt <double> (this,                  "probability",
                                                   witGetProbabilityDbl,
                                                   witSetProbabilityDbl);

   new   PartVectorAtt <double> (this,                  "prodVol",
                                               witGetPartProdVolDbl);

   new BopEntVectorAtt <double> (this,                  "productRate",
                                           witGetBopEntryProductRateDbl,
                                           witSetBopEntryProductRateDbl);

   new BomEntVectorAtt <int>    (this,                  "propRtg",
                                           witGetBomEntryPropRtg,
                                           witSetBomEntryPropRtg);

   new   PartVectorAtt <int>    (this,                  "propRtg",
                                               witGetPartPropRtg,
                                               witSetPartPropRtg);

   new   PartVectorAtt <double> (this,                  "reqVol",
                                               witGetPartReqVolDbl);

   new   PartVectorAtt <double> (this,                  "residualVol",
                                               witGetPartResidualVolDbl);

   new   ProbScalarAtt <int>    (this,                  "respectStockSLBs",
                                                   witGetRespectStockSLBs,
                                                   witSetRespectStockSLBs);

   new   ProbScalarAtt <int>    (this,                  "roundReqVols",
                                                   witGetRoundReqVols,
                                                   witSetRoundReqVols);

   new BomEntVectorAtt <double> (this,                  "routingShare",
                                           witGetBomEntryRoutingShareDbl,
                                           witSetBomEntryRoutingShareDbl);

   new BopEntVectorAtt <double> (this,                  "routingShare",
                                           witGetBopEntryRoutingShareDbl,
                                           witSetBopEntryRoutingShareDbl);

   new    SubVectorAtt <double> (this,                  "routingShare",
                                       witGetSubsBomEntryRoutingShareDbl,
                                       witSetSubsBomEntryRoutingShareDbl);

   new   PartScalarAtt <int>    (this,                  "scrapAllowed",
                                               witGetPartScrapAllowed,
                                               witSetPartScrapAllowed);

   new   PartVectorAtt <double> (this,                  "scrapCost",
                                               witGetPartScrapCostDbl,
                                               witSetPartScrapCostDbl);

   new   PartVectorAtt <double> (this,                  "scrapVol",
                                               witGetPartScrapVolDbl);

   new DemandScalarAtt <double> (this,                  "searchInc",
                                             witGetDemandSearchIncDbl,
                                             witSetDemandSearchIncDbl);

   new BomEntScalarAtt <int>    (this,                  "selForDel",
                                           witGetBomEntrySelForDel,
                                           witSetBomEntrySelForDel);

   new BopEntScalarAtt <int>    (this,                  "selForDel",
                                           witGetBopEntrySelForDel,
                                           witSetBopEntrySelForDel);

   new DemandScalarAtt <int>    (this,                  "selForDel",
                                             witGetDemandSelForDel,
                                             witSetDemandSelForDel);

   new    OpnScalarAtt <int>    (this,                  "selForDel",
                                          witGetOperationSelForDel,
                                          witSetOperationSelForDel);

   new   PartScalarAtt <int>    (this,                  "selForDel",
                                               witGetPartSelForDel,
                                               witSetPartSelForDel);

   new    SubScalarAtt <int>    (this,                  "selForDel",
                                       witGetSubsBomEntrySelForDel,
                                       witSetSubsBomEntrySelForDel);

   new   ProbScalarAtt <int>    (this,                  "selSplit",
                                                   witGetSelSplit,
                                                   witSetSelSplit);

   new   ProbScalarAtt <int>    (this,                  "selectionRecovery",
                                                   witGetSelectionRecovery,
                                                   witSetSelectionRecovery);

   new   PartVectorAtt <double> (this,                  "shadowPrice",
                                               witGetPartShadowPriceDbl);

   new DemandScalarAtt <int>    (this,                  "shipLateAllowed",
                                             witGetDemandShipLateAllowed,
                                             witSetDemandShipLateAllowed);

   new DemandVectorAtt <int>    (this,                  "shipLateUB",
                                             witGetDemandShipLateUB,
                                             witSetDemandShipLateUB);

   new DemandVectorAtt <double> (this,                  "shipReward",
                                             witGetDemandShipRewardDbl,
                                             witSetDemandShipRewardDbl);

   new DemandVectorAtt <double> (this,                  "shipVol",
                                             witGetDemandShipVolDbl,
                                             witSetDemandShipVolDbl);

   new BomEntScalarAtt <int>    (this,                  "singleSource",
                                           witGetBomEntrySingleSource,
                                           witSetBomEntrySingleSource);

   new   PartScalarAtt <int>    (this,                  "singleSource",
                                               witGetPartSingleSource,
                                               witSetPartSingleSource);

   new   ProbScalarAtt <int>    (this,                  "skipFailures",
                                                   witGetSkipFailures,
                                                   witSetSkipFailures);

   new   ProbScalarAttString    (this,                  "solverLogFileName",
                                                   witGetSolverLogFileName,
                                                   witSetSolverLogFileName);

   new   ProbScalarAtt <int>    (this,                  "stageByObject",
                                                   witGetStageByObject,
                                                   witSetStageByObject);

   new   ProbScalarAtt <int>    (this,                  "stochMode",
                                                   witGetStochMode,
                                                   witSetStochMode);

   new   ProbScalarAtt <int>    (this,                  "stochSolnMode",
                                                   witGetStochSolnMode);

   new   PartVectorAtt <double> (this,                  "stockCost",
                                               witGetPartStockCostDbl,
                                               witSetPartStockCostDbl);

   new   ProbScalarAtt <int>    (this,                  "stockReallocation",
                                                   witGetStockReallocation,
                                                   witSetStockReallocation);

   new   PartVectorAtt <double> (this,                  "stockVol",
                                               witGetPartStockVolDbl);

   new    SubVectorAtt <double> (this,                  "subCost",
                                       witGetSubsBomEntrySubCostDbl,
                                       witSetSubsBomEntrySubCostDbl);

   new    SubVectorAtt <double> (this,                  "subVol",
                                       witGetSubsBomEntrySubVolDbl,
                                       witSetSubsBomEntrySubVolDbl);

   new   PartVectorAtt <double> (this,                  "supplyVol",
                                               witGetPartSupplyVolDbl,
                                               witSetPartSupplyVolDbl);

   new   ProbScalarAtt <int>    (this,                  "tieBreakPropRt",
                                                   witGetTieBreakPropRt,
                                                   witSetTieBreakPropRt);

   new   ProbScalarAttString    (this,                  "title",
                                                   witGetTitle,
                                                   witSetTitle);

   new   ProbScalarAtt <int>    (this,                  "truncOffsets",
                                                   witGetTruncOffsets,
                                                   witSetTruncOffsets);

   new    OpnScalarAtt <int>    (this,                  "twoLevelLotSizes",
                                          witGetOperationTwoLevelLotSizes,
                                          witSetOperationTwoLevelLotSizes);

   new   ProbScalarAtt <int>    (this,                  "twoWayMultiExec",
                                                   witGetTwoWayMultiExec,
                                                   witSetTwoWayMultiExec);

   new   ProbScalarAtt <int>    (this,                  "useFocusHorizons",
                                                   witGetUseFocusHorizons,
                                                   witSetUseFocusHorizons);

   new   ProbScalarAtt <int>    (this,                  "userHeurStart",
                                                   witGetUserHeurStart,
                                                   witSetUserHeurStart);

   new   ProbScalarAtt <double> (this,                  "wbounds",
                                                   witGetWboundsDbl,
                                                   witSetWboundsDbl);

   new    OpnVectorAtt <double> (this,                  "yieldRate",
                                          witGetOperationYieldRateDbl,
                                          witSetOperationYieldRateDbl);

   mostRecentAtt_ = NULL;
   }

//------------------------------------------------------------------------------
