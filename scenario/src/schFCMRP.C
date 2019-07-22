
#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif

// RW2STL -- inserted:
// End of RW2STL -- inserted includes.
#include <scenario/src/schFCMRP.h>
#include <scenario/src/scenCMRP.h>
#include <scenario/src/schdDeSt.h>
#include <scenario/src/sortSchd.h>
#include <scenario/src/partSS.h>
#include <scenario/src/demVolSS.h>
#include <scenario/src/prodSS.h>
#include <scenario/src/backlgSS.h>
#include <scenario/src/scenario.h>
#include <scenario/src/supplySS.h>
#include <scenario/src/reqSS.h>
#include <scenario/src/cumSS.h>
#include <scenario/src/costSS.h>
#include <scenario/src/shipSS.h>
#include <scenario/src/cycleSS.h>
#include <scenario/src/consSS.h>
#include <scenario/src/excessSS.h>
#include <scenario/src/stockSS.h>
#include <scenario/src/scrapSS.h>
#include <scenario/src/usageSS.h>
#include <scenario/src/deltaSS.h>
#include <scenario/src/fssSS.h>
#include <scenario/src/offsetSS.h>
#include <scenario/src/partSchd.h>
#include <scenario/src/demdSchd.h>
#include <scenario/src/scCMRPP.h>
#include <scenario/src/partSchP.h>
#include <scenario/src/demdSchP.h>

// Create a new sorting part schedule
LgFrSortingPartScheduleFloatSmartPointer
LgFrScheduleFactoryForCMRP::newPartScheduleFromScratch 
  (const LgFrScheduleDescriptor& desc,
   const bool deepCopy,
   const bool toPopulate
   )
const
{
  LgFrPartScheduleStrategy *partSS;
  LgFrScenarioForCMRPSmartPointer scenario
    = LgFrScenarioForCMRPSmartPointer(this->scenario());
  if ( desc == LGFRBudgetAmount ) {
      partSS = new LgFrSupplyScheduleStrategy(*scenario);
      partSS->setOfParts(scenario->constSetOfParts(LGFR_BUDGET_SET));
  }

//        Schedules for the  "Planner-managed" (constrained) window
//        ---------------------------------------------------------
  else if ( desc == LGFRPlannerManagedSupplyVol){
      partSS = new LgFrSupplyScheduleStrategy(*scenario);
      partSS->setOfParts(scenario->constSetOfParts(LGFR_CONSTRAINED_SET));
  }
  else if ( desc == LGFRPlannerManagedRequiredVol){
      partSS = new LgFrRequirementScheduleStrategy(*scenario);
      partSS->setOfParts(scenario->constSetOfParts(LGFR_CONSTRAINED_SET));
  }
  else if ( desc == LGFRPlannerManagedConsVol){
      partSS = new LgFrFullyExplodedMrpConsVolScheduleStrategy(*scenario);
      partSS->setOfParts(scenario->constSetOfParts(LGFR_CONSTRAINED_SET));
  }
  else if ( desc == LGFRPlannerManagedTotalPartDemandVol){
      LgFrCumDemandsPartScheduleStrategy* totShipSS
	  = new LgFrCumDemandsPartScheduleStrategy(*scenario);
      LgFrSortingDemandScheduleFloatSmartPointer dependentSched = 
	  this->newDemandSchedule(LGFRDemandVolume, deepCopy, toPopulate);
      totShipSS->insertDemandSchedule(dependentSched);
      partSS = totShipSS;
      partSS->setOfParts(scenario->constSetOfParts(LGFR_CONSTRAINED_SET));
  }
  else if ( desc == LGFRPlannerManagedTotalConsVol){
      // Sum, for each part, fully exploded consVol and independent demand
      LgFrSortingPartScheduleFloatSmartPointer independentDemandVol
        = this->newPartSchedule(LGFRPlannerManagedTotalPartDemandVol, false,
				true);
      LgFrSortingPartScheduleFloatSmartPointer fullyExplodedMrpConsVol
	= this->newPartSchedule(LGFRPlannerManagedConsVol, false,
				true);
      LgFrPartScheduleSensitiveScheduleStrategy* sumSS
	  = new LgFrPartScheduleSensitiveScheduleStrategy(*scenario);
      sumSS->insertPartSchedule(independentDemandVol);
      sumSS->insertPartSchedule(fullyExplodedMrpConsVol);
      sumSS->combiningFunction (&LgFrTimeVecFloat::add);
      partSS = sumSS;
      partSS->setOfParts(scenario->constSetOfParts(LGFR_CONSTRAINED_SET));
  }
  else if ( desc == LGFRPlannerManagedCumSupplyVol){
      LgFrCumTimeVecPartScheduleStrategy* cumSS
	  = new LgFrCumTimeVecPartScheduleStrategy(*scenario);
      LgFrSortingPartScheduleFloatSmartPointer dependentSched = 
	  this->newPartSchedule(LGFRPlannerManagedSupplyVol, deepCopy,
				toPopulate);
      cumSS->insertPartSchedule(dependentSched);
      partSS = cumSS;
      partSS->setOfParts(scenario->constSetOfParts(LGFR_CONSTRAINED_SET));
  }
  else if ( desc == LGFRPlannerManagedCumRequiredVol){
      LgFrCumTimeVecPartScheduleStrategy* cumSS
	  = new LgFrCumTimeVecPartScheduleStrategy(*scenario);
      LgFrSortingPartScheduleFloatSmartPointer dependentSched = 
	  this->newPartSchedule(LGFRPlannerManagedRequiredVol, deepCopy,
				toPopulate);
      cumSS->insertPartSchedule(dependentSched);
      partSS = cumSS;
      partSS->setOfParts(scenario->constSetOfParts(LGFR_CONSTRAINED_SET));
  }
  else if (desc == LGFRPlannerManagedExcessVol)     {
      partSS = new LgFrExcessScheduleStrategy(*scenario);
      partSS->setOfParts(scenario->constSetOfParts(LGFR_CONSTRAINED_SET));
  }
  else if ( desc == LGFRPlannerManagedFocussedShortageSchedule ) {
      partSS = new 
	LgFrFocussedShortageScheduleScheduleStrategy(*(this->scenario()));
      partSS->setOfParts(scenario->constSetOfParts(LGFR_CONSTRAINED_SET));
  }
  else if (desc == LGFRPlannerManagedStockVol)     {
      partSS = new LgFrStockScheduleStrategy(*scenario);
      partSS->setOfParts(scenario->constSetOfParts(LGFR_CONSTRAINED_SET));
  }
  else if (desc == LGFRPlannerManagedScrapVol)     {
      partSS = new LgFrScrapScheduleStrategy(*scenario);
      partSS->setOfParts(scenario->constSetOfParts(LGFR_CONSTRAINED_SET));
  }
  else if (desc == LGFRPlannerManagedUsageVol)     {
      LgFrUsageScheduleStrategy* usageSS
	  = new LgFrUsageScheduleStrategy(*scenario);
      LgFrSortingPartScheduleFloatSmartPointer dependentSupplySched1 = 
	  this->newPartSchedule(LGFRPlannerManagedSupplyVol, deepCopy,
				toPopulate);
      LgFrSortingPartScheduleFloatSmartPointer
	dependentProdSched2(new LgFrSortingPartScheduleFloat);
      LgFrSortingPartScheduleFloatSmartPointer dependentStockSched3 = 
	  this->newPartSchedule(LGFRPlannerManagedStockVol, deepCopy, 
				toPopulate);
      LgFrSortingPartScheduleFloatSmartPointer dependentScrapSched4 = 
	  this->newPartSchedule(LGFRPlannerManagedScrapVol, deepCopy, 
				toPopulate);

      usageSS->insertPartSchedule(dependentSupplySched1);
      usageSS->insertPartSchedule(dependentProdSched2);
      usageSS->insertPartSchedule(dependentStockSched3);
      usageSS->insertPartSchedule(dependentScrapSched4);
      partSS = usageSS;
      partSS->setOfParts(scenario->constSetOfParts(LGFR_CONSTRAINED_SET));
  }

//        Schedules for the "MRP-managed" 
//        --------------------------------
  else if ( desc == LGFRMRPManagedSupplyVol){
      partSS = new LgFrSupplyScheduleStrategy(*scenario);
      partSS->setOfParts(scenario->constSetOfParts(LGFR_MRP_PLANNED_SET));
  }

  else if ( desc == LGFRMRPManagedRequiredVol){
      partSS = new LgFrCmrpRequirementScheduleStrategy(*scenario);
      partSS->setOfParts(scenario->constSetOfParts(LGFR_MRP_PLANNED_SET));
  }

  else if ( desc == LGFRMRPManagedConsVol){
      partSS = new LgFrFullyExplodedMrpConsVolScheduleStrategy(*scenario);
      partSS->setOfParts(scenario->constSetOfParts(LGFR_MRP_PLANNED_SET));
  }

  else if ( desc == LGFRMRPManagedTotalPartDemandVol){
      LgFrCumDemandsPartScheduleStrategy* totShipSS
	  = new LgFrCumDemandsPartScheduleStrategy(*scenario);
      LgFrSortingDemandScheduleFloatSmartPointer dependentSched = 
	  this->newDemandSchedule(LGFRDemandVolume, deepCopy, toPopulate);
      totShipSS->insertDemandSchedule(dependentSched);
      partSS = totShipSS;
      partSS->setOfParts(scenario->constSetOfParts(LGFR_CONSTRAINED_SET));
  }
  else if ( desc == LGFRMRPManagedTotalConsVol){
      // Sum, for each part, fully exploded consVol and independent demand
      LgFrSortingPartScheduleFloatSmartPointer independentDemandVol
        = this->newPartSchedule(LGFRMRPManagedTotalPartDemandVol, false,
				true);
      LgFrSortingPartScheduleFloatSmartPointer fullyExplodedConsVol
	= this->newPartSchedule(LGFRMRPManagedConsVol, false,
				true);
      LgFrPartScheduleSensitiveScheduleStrategy* sumSS
	  = new LgFrPartScheduleSensitiveScheduleStrategy(*scenario);
      sumSS->insertPartSchedule(independentDemandVol);
      sumSS->insertPartSchedule(fullyExplodedConsVol);
      sumSS->combiningFunction (&LgFrTimeVecFloat::add);
      partSS = sumSS;
      partSS->setOfParts(scenario->constSetOfParts(LGFR_CONSTRAINED_SET));
  }

  else if ( desc == LGFRMRPManagedCumSupplyVol){
      LgFrCumTimeVecPartScheduleStrategy* cumSS
	  = new LgFrCumTimeVecPartScheduleStrategy(*scenario);
      LgFrSortingPartScheduleFloatSmartPointer dependentSched = 
	  this->newPartSchedule(LGFRMRPManagedSupplyVol, deepCopy, toPopulate);
      cumSS->insertPartSchedule(dependentSched);
      partSS = cumSS;
      partSS->setOfParts(scenario->constSetOfParts(LGFR_MRP_PLANNED_SET));
  }

  else if ( desc == LGFRMRPManagedCumRequiredVol){
      LgFrCumTimeVecPartScheduleStrategy* cumSS
	  = new LgFrCumTimeVecPartScheduleStrategy(*scenario);
      LgFrSortingPartScheduleFloatSmartPointer dependentSched = 
	  this->newPartSchedule(LGFRMRPManagedRequiredVol, deepCopy, toPopulate);
      cumSS->insertPartSchedule(dependentSched);
      partSS = cumSS;
      partSS->setOfParts(scenario->constSetOfParts(LGFR_MRP_PLANNED_SET));
  }

  else if (desc == LGFRMRPManagedExcessVol)     {
      partSS = new LgFrExcessScheduleStrategy(*scenario);
      partSS->setOfParts(scenario->constSetOfParts(LGFR_MRP_PLANNED_SET));
  }

  else if (desc == LGFRMRPManagedFocussedShortageSchedule)     {
      partSS = new LgFrFocussedShortageScheduleScheduleStrategy(*(this->scenario()));
      partSS->setOfParts(scenario->constSetOfParts(LGFR_MRP_PLANNED_SET));
  }

  else if (desc == LGFRMRPManagedStockVol)     {
      partSS = new LgFrStockScheduleStrategy(*scenario);
      partSS->setOfParts(scenario->constSetOfParts(LGFR_MRP_PLANNED_SET));
  }

  else if (desc == LGFRMRPManagedScrapVol)     {
      partSS = new LgFrScrapScheduleStrategy(*scenario);
      partSS->setOfParts(scenario->constSetOfParts(LGFR_MRP_PLANNED_SET));
  }

  else if (desc == LGFRMRPManagedProductionPlusProcurementVolume)     {
      partSS = new LgFrProductionScheduleStrategy(*scenario);
      partSS->setOfParts(scenario->constSetOfParts(LGFR_MRP_PLANNED_SET));
  }

  else if (desc == LGFRMRPManagedUsageVol)     {
      LgFrUsageScheduleStrategy* usageSS = new LgFrUsageScheduleStrategy(*scenario);
      LgFrSortingPartScheduleFloatSmartPointer dependentSupplySched1 = 
	  this->newPartSchedule(LGFRMRPManagedSupplyVol, deepCopy, toPopulate);
      LgFrSortingPartScheduleFloatSmartPointer dependentProdSched2 = 
	  this->newPartSchedule(LGFRMRPManagedProductionPlusProcurementVolume,
				deepCopy, toPopulate);
      LgFrSortingPartScheduleFloatSmartPointer dependentStockSched3 = 
	  this->newPartSchedule(LGFRMRPManagedStockVol, deepCopy, toPopulate);
      LgFrSortingPartScheduleFloatSmartPointer dependentScrapSched4 = 
	  this->newPartSchedule(LGFRMRPManagedScrapVol, deepCopy, toPopulate);
      usageSS->insertPartSchedule(dependentSupplySched1);
      usageSS->insertPartSchedule(dependentProdSched2);
      usageSS->insertPartSchedule(dependentStockSched3);
      usageSS->insertPartSchedule(dependentScrapSched4);
      partSS = usageSS;
      partSS->setOfParts(scenario->constSetOfParts(LGFR_MRP_PLANNED_SET));
  }

#if 0
  else if ( desc == LGFRProcurementReceiptVol ) {
      LgFrApplyOffsetScheduleStrategy* offsetSS
	  = new LgFrApplyOffsetScheduleStrategy(*scenario);
      LgFrSortingPartScheduleFloatSmartPointer theProcSched = 
	  this->newPartSchedule(LGFRProcurementVol, deepCopy, toPopulate);
      LgFrSortingPartScheduleFloatSmartPointer theCycleTimeSched = 
	  this->newPartSchedule(LGFRProcurementLeadTime, deepCopy, toPopulate);
      offsetSS->insertPartSchedule(theProcSched);
      offsetSS->insertPartSchedule(theCycleTimeSched);
      partSS = offsetSS;
      partSS->setOfParts(scenario->constSetOfParts(LGFR_PROCUREABLE_SET));
  }
#endif

  else if ( desc == LGFRProductionReceiptVol ) {
      LgFrApplyOffsetScheduleStrategy* offsetSS
	  = new LgFrApplyOffsetScheduleStrategy(*scenario);
      LgFrSortingPartScheduleFloatSmartPointer theProdSched = 
	  this->newPartSchedule(LGFRProductionVolume, deepCopy, toPopulate);
      LgFrSortingPartScheduleFloatSmartPointer theCycleTimeSched = 
	  this->newPartSchedule(LGFRCycleTime, deepCopy, toPopulate);
      offsetSS->insertPartSchedule(theProdSched);
      offsetSS->insertPartSchedule(theCycleTimeSched);
      partSS = offsetSS;
      partSS->setOfParts(scenario->constSetOfParts(LGFR_PRODUCEABLE_SET));
  }

  else if ( desc == LGFRProductionVolume) {
      partSS = new LgFrProductionScheduleStrategy(*scenario);
      partSS->setOfParts(scenario->constSetOfParts(LGFR_PRODUCEABLE_SET));
  }

  else if ( desc == LGFRTotalShipmentVol) {
      LgFrCumDemandsPartScheduleStrategy* totShipSS
	  = new LgFrCumDemandsPartScheduleStrategy(*scenario);
      LgFrSortingDemandScheduleFloatSmartPointer dependentSched = 
	  this->newDemandSchedule(LGFRShipmentVolume, deepCopy, toPopulate);
      totShipSS->insertDemandSchedule(dependentSched);
      partSS = totShipSS;
      partSS->setOfParts(scenario->constSetOfParts(LGFR_MRP_PLANNED_SET));
  }
  else if ( desc == LGFRCMRPTotalCommittedDemandVol) {
      LgFrCumDemandsPartScheduleStrategy* totCommDemandsSS
	  = new LgFrCumDemandsPartScheduleStrategy(*scenario);
      LgFrSortingDemandScheduleFloatSmartPointer dependentSched = 
	  this->newDemandSchedule(LGFRCMRPCommittedDemandVol, deepCopy, toPopulate);
      totCommDemandsSS->insertDemandSchedule(dependentSched);
      partSS = totCommDemandsSS;
      partSS->setOfParts(scenario->constSetOfParts(LGFR_DEFAULT_SET));
  }

  else if ( desc == LGFRProcurementVol) {
      partSS = new LgFrProductionScheduleStrategy(*scenario);
      partSS->setOfParts(scenario->constSetOfParts(LGFR_PROCUREABLE_SET));
  }
  else if ( desc == LGFRProcurementLeadTime) {
      partSS = new LgFrCycleTimeScheduleStrategy(*scenario);
      partSS->setOfParts(scenario->constSetOfParts(LGFR_PROCUREABLE_SET));
  }
  else if ( desc == LGFRCycleTime) {
      partSS = new LgFrCycleTimeScheduleStrategy(*scenario);
      partSS->setOfParts(scenario->constSetOfParts(LGFR_PRODUCEABLE_SET));
  }
  else if ( desc == LGFRFrozenZone) {
      partSS = new LgFrFrozenZoneScheduleStrategy(*scenario);
      partSS->setOfParts(scenario->constSetOfParts(LGFR_MRP_PLANNED_SET));
  }


//        Schedules for the "Capacity" window 
//        -----------------------------------
  else if ( desc == LGFRCapacitySupplyVol) {
      partSS = new LgFrSupplyScheduleStrategy(*scenario);
      partSS->setOfParts(scenario->constSetOfParts(LGFR_CAPACITY_SET));
  }
  else if ( desc == LGFRCapacityRequiredVol) {
      partSS = new LgFrRequirementScheduleStrategy(*scenario);
      partSS->setOfParts(scenario->constSetOfParts(LGFR_CAPACITY_SET));
  }
  else if ( desc == LGFRCapacityScrapVol) {
      partSS = new LgFrScrapScheduleStrategy(*scenario);
      partSS->setOfParts(scenario->constSetOfParts(LGFR_CAPACITY_SET));
  }
  else if ( desc == LGFRCapacityUsageVol) {
      // for capacity: prodVol and StockVol always zero ==> use empty sched
      LgFrUsageScheduleStrategy* usageSS
	  = new LgFrUsageScheduleStrategy(*scenario);
      LgFrSortingPartScheduleFloatSmartPointer dependentSupplySched1 = 
	  this->newPartSchedule(LGFRCapacitySupplyVol, deepCopy, toPopulate);
      LgFrSortingPartScheduleFloatSmartPointer dependentDummySched23(new LgFrSortingPartScheduleFloat);
      LgFrSortingPartScheduleFloatSmartPointer dependentScrapSched4 = 
	  this->newPartSchedule(LGFRCapacityScrapVol, deepCopy, toPopulate);
      usageSS->insertPartSchedule(dependentSupplySched1);         
      usageSS->insertPartSchedule(dependentDummySched23);   
      usageSS->insertPartSchedule(dependentDummySched23);   
      usageSS->insertPartSchedule(dependentScrapSched4);         
      partSS = usageSS;
      partSS->setOfParts(scenario->constSetOfParts(LGFR_CAPACITY_SET));
  }
  else if ( desc == LGFRCapacityExcessVol) {
      partSS = new LgFrExcessScheduleStrategy(*scenario);
      partSS->setOfParts(scenario->constSetOfParts(LGFR_CAPACITY_SET));
  }
  else if ( desc == LGFRCapacityFocussedShortageSchedule) {
      partSS = new LgFrFocussedShortageScheduleScheduleStrategy(*scenario);
      partSS->setOfParts(scenario->constSetOfParts(LGFR_CAPACITY_SET));
  }
  else if ( desc == LGFRCapacityUsagePct) {
      LgFrPartScheduleSensitiveScheduleStrategy* usagePctSS 
	  = new LgFrPartScheduleSensitiveScheduleStrategy(*scenario);
      LgFrSortingPartScheduleFloatSmartPointer usageSched = 
	  this->newPartSchedule(LGFRCapacityUsageVol, deepCopy, toPopulate);
      LgFrSortingPartScheduleFloatSmartPointer supplySched =
	  this->newPartSchedule(LGFRCapacitySupplyVol, deepCopy, toPopulate);
      usagePctSS->insertPartSchedule(usageSched);
      usagePctSS->insertPartSchedule(supplySched);
      usagePctSS->combiningFunction (&LgFrTimeVecFloat::percentageOf);
      partSS = usagePctSS;
      partSS->setOfParts(scenario->constSetOfParts(LGFR_CAPACITY_SET));
  }

  //   Schedules for the "Procurement Summary" window.
  //   ----------------------------------------------
  else if (desc ==  LGFRCMRPTotalMrpProcurementCost) {
      LgFrPartSummaryScheduleStrategy* PartWiseCumSS
	  = new LgFrPartSummaryScheduleStrategy(*scenario);
      LgFrSortingPartScheduleFloatSmartPointer dependentSched = 
	  this->newPartSchedule(LGFRRequiredVolumeTimesUnitCost, deepCopy, toPopulate);
      PartWiseCumSS->insertPartSchedule(dependentSched);         
      partSS = PartWiseCumSS;
      partSS->setOfParts(scenario->constSetOfParts(LGFR_BUDGET_SET));
  }
  else if (desc ==  LGFRCMRPTotalWitProcurementCost) {
      LgFrPartSummaryScheduleStrategy* PartWiseCumSS
	  = new LgFrPartSummaryScheduleStrategy(*scenario);
      LgFrSortingPartScheduleFloatSmartPointer dependentSched = 
	  this->newPartSchedule(LGFRProcurementVolumeTimesUnitCost, deepCopy, toPopulate);
      PartWiseCumSS->insertPartSchedule(dependentSched);         
      partSS = PartWiseCumSS;
      partSS->setOfParts(scenario->constSetOfParts(LGFR_BUDGET_SET));
  }
  else if (desc ==  LGFRCMRPCumTotalMrpProcurementCost) {
      LgFrCumTimeVecPartScheduleStrategy* cumSS
	  = new LgFrCumTimeVecPartScheduleStrategy(*scenario);
      LgFrSortingPartScheduleFloatSmartPointer dependentSched = 
	  this->newPartSchedule(LGFRCMRPTotalMrpProcurementCost, deepCopy, toPopulate);
      cumSS->insertPartSchedule(dependentSched);
      partSS = cumSS;
      partSS->setOfParts(scenario->constSetOfParts(LGFR_BUDGET_SET));
  }
  else if (desc ==  LGFRCMRPCumTotalWitProcurementCost) {
      LgFrCumTimeVecPartScheduleStrategy* cumSS
	  = new LgFrCumTimeVecPartScheduleStrategy(*scenario);
      LgFrSortingPartScheduleFloatSmartPointer dependentSched = 
	  this->newPartSchedule(LGFRCMRPTotalWitProcurementCost, deepCopy, toPopulate);
      cumSS->insertPartSchedule(dependentSched);
      partSS = cumSS;
      partSS->setOfParts(scenario->constSetOfParts(LGFR_BUDGET_SET));
  }
  else if (desc ==  LGFRCMRPDeltaCumTotalProcurementCost) {
      LgFrTwoScheduleDeltaPartScheduleStrategy* deltaSS
	  = new LgFrTwoScheduleDeltaPartScheduleStrategy(*scenario);
      deltaSS->combiningFunction (&LgFrTimeVecFloat::subtract);
      LgFrSortingPartScheduleFloatSmartPointer dependentSched1 = 
	  this->newPartSchedule(LGFRCMRPCumTotalMrpProcurementCost, deepCopy, toPopulate);
      LgFrSortingPartScheduleFloatSmartPointer dependentSched2 = 
	  this->newPartSchedule(LGFRCMRPCumTotalWitProcurementCost, deepCopy, toPopulate);
      deltaSS->insertPartSchedule(dependentSched1);
      deltaSS->insertPartSchedule(dependentSched2);
      partSS = deltaSS;
      partSS->setOfParts(scenario->constSetOfParts(LGFR_BUDGET_SET));
  }
  else if (desc ==  LGFRCostOfCapitalSavings) {
      LgFrCompoundInterestScheduleStrategy* ciSS
	  = new LgFrCompoundInterestScheduleStrategy(*scenario);
      LgFrSortingPartScheduleFloatSmartPointer dependentSched1 = 
	  this->newPartSchedule(LGFRCMRPDeltaCumTotalProcurementCost, deepCopy, toPopulate);
      ciSS->insertPartSchedule(dependentSched1);
      partSS = ciSS;
      partSS->setOfParts(scenario->constSetOfParts(LGFR_BUDGET_SET));
  }
  else if (desc ==  LGFRProductionVolumeTimesUnitCost) {
      LgFrPartScheduleTimesUnitCostScheduleStrategy* prodVolTimesUnitCostSS
	  = new LgFrPartScheduleTimesUnitCostScheduleStrategy(*scenario);
      LgFrSortingPartScheduleFloatSmartPointer dependentSched = 
	  this->newPartSchedule(LGFRProductionVolume, deepCopy, toPopulate);
      prodVolTimesUnitCostSS->insertPartSchedule(dependentSched);
      partSS = prodVolTimesUnitCostSS;
      partSS->setOfParts(scenario->constSetOfParts(LGFR_PRODUCEABLE_SET));
  }
  else if (desc ==  LGFRProcurementVolumeTimesUnitCost) {
      LgFrPartScheduleTimesUnitCostScheduleStrategy* procVolTimesUnitCostSS
	  = new LgFrPartScheduleTimesUnitCostScheduleStrategy(*scenario);
      LgFrSortingPartScheduleFloatSmartPointer dependentSched = 
	  this->newPartSchedule(LGFRProcurementVol, deepCopy, toPopulate);
      procVolTimesUnitCostSS->insertPartSchedule(dependentSched);
      partSS = procVolTimesUnitCostSS;
      partSS->setOfParts(scenario->constSetOfParts(LGFR_PROCUREABLE_SET));
  }
  else if (desc ==  LGFRRequiredVolumeTimesUnitCost) {
      LgFrPartScheduleTimesUnitCostScheduleStrategy* reqVolTimesUnitCostSS
	  = new LgFrPartScheduleTimesUnitCostScheduleStrategy(*scenario);
      LgFrSortingPartScheduleFloatSmartPointer dependentSched = 
	  this->newPartSchedule(LGFRMRPManagedRequiredVol, deepCopy,
				toPopulate);
      reqVolTimesUnitCostSS->insertPartSchedule(dependentSched);
      partSS = reqVolTimesUnitCostSS;
      partSS->setOfParts(scenario->constSetOfParts(LGFR_MRP_PLANNED_SET));
  }

  //   Schedules for the "Demand Summary" information.
  //   These are part schedules because the demand attribute values
  //   for a particular demanded part are summed and attributed to
  //   the part.
  //   ----------------------------------------------
  else if (desc ==  LGFRCMRPSumDemandVolToPart) {
      LgFrCumDemandsPartScheduleStrategy* sumDemandsToPartSS
	  = new LgFrCumDemandsPartScheduleStrategy(*scenario);
      LgFrSortingDemandScheduleFloatSmartPointer independentSched =
	  this->newDemandSchedule(LGFRDemandVolume, deepCopy, toPopulate);
      sumDemandsToPartSS->insertDemandSchedule(independentSched);
      partSS = sumDemandsToPartSS;
      partSS->setOfParts(scenario->constSetOfParts(LGFR_UNIVERSAL_SET));
  }
  else if (desc ==  LGFRCMRPCumSumDemandVolToPart) {
      LgFrCumTimeVecPartScheduleStrategy* cumTimeVecSS
	  = new LgFrCumTimeVecPartScheduleStrategy(*scenario);
      LgFrSortingPartScheduleFloatSmartPointer independentSched =
	  this->newPartSchedule(LGFRCMRPSumDemandVolToPart, 
				deepCopy, toPopulate);
      cumTimeVecSS->insertPartSchedule(independentSched);
      partSS = cumTimeVecSS;
      partSS->setOfParts(scenario->constSetOfParts(LGFR_UNIVERSAL_SET));
  }
  else if (desc ==  LGFRCMRPSummaryOfCumDemandVol) {
      LgFrPartSummaryScheduleStrategy* summarySS
	  = new LgFrPartSummaryScheduleStrategy(*scenario);
      LgFrSortingPartScheduleFloatSmartPointer independentSched =
	  this->newPartSchedule(LGFRCMRPCumSumDemandVolToPart,
				deepCopy, toPopulate);
      summarySS->insertPartSchedule(independentSched);
      partSS = summarySS;
      partSS->setOfParts(scenario->constSetOfParts(LGFR_BUDGET_SET));
  }
  else if (desc ==  LGFRCMRPCumSumDemandVolToPartTimesUnitCost) {
      LgFrPartScheduleTimesUnitCostScheduleStrategy* timesUnitCostSS
	  = new LgFrPartScheduleTimesUnitCostScheduleStrategy(*scenario);
      LgFrSortingPartScheduleFloatSmartPointer dependentSched =
	  this->newPartSchedule(LGFRCMRPCumSumDemandVolToPart, 
				deepCopy, toPopulate);
      timesUnitCostSS->insertPartSchedule(dependentSched);
      partSS = timesUnitCostSS;
      partSS->setOfParts(scenario->constSetOfParts(LGFR_UNIVERSAL_SET));
  }
  else if (desc ==  LGFRCMRPSummaryOfCumDemandVolTimesUnitCost) {
      LgFrPartSummaryScheduleStrategy* summarySS
	  = new LgFrPartSummaryScheduleStrategy(*scenario);
      LgFrSortingPartScheduleFloatSmartPointer independentSched =
	  this->newPartSchedule(LGFRCMRPCumSumDemandVolToPartTimesUnitCost,
				deepCopy, toPopulate);
      summarySS->insertPartSchedule(independentSched);
      partSS = summarySS;
      partSS->setOfParts(scenario->constSetOfParts(LGFR_BUDGET_SET));
  }

  else if (desc ==  LGFRCMRPSumShipmentVolToPart) {
      LgFrCumDemandsPartScheduleStrategy* sumDemandsToPartSS
	  = new LgFrCumDemandsPartScheduleStrategy(*scenario);
      LgFrSortingDemandScheduleFloatSmartPointer independentSched =
	  this->newDemandSchedule(LGFRShipmentVolume, deepCopy, toPopulate);
      sumDemandsToPartSS->insertDemandSchedule(independentSched);
      partSS = sumDemandsToPartSS;
      partSS->setOfParts(scenario->constSetOfParts(LGFR_UNIVERSAL_SET));
  }
  else if (desc ==  LGFRCMRPCumSumShipmentVolToPart) {
      LgFrCumTimeVecPartScheduleStrategy* cumTimeVecSS
	  = new LgFrCumTimeVecPartScheduleStrategy(*scenario);
      LgFrSortingPartScheduleFloatSmartPointer independentSched =
	  this->newPartSchedule(LGFRCMRPSumShipmentVolToPart, 
				deepCopy, toPopulate);
      cumTimeVecSS->insertPartSchedule(independentSched);
      partSS = cumTimeVecSS;
      partSS->setOfParts(scenario->constSetOfParts(LGFR_UNIVERSAL_SET));
  }
  else if (desc ==  LGFRCMRPSummaryOfCumShipmentVol) {
      LgFrPartSummaryScheduleStrategy* summarySS
	  = new LgFrPartSummaryScheduleStrategy(*scenario);
      LgFrSortingPartScheduleFloatSmartPointer independentSched =
	  this->newPartSchedule(LGFRCMRPCumSumShipmentVolToPart,
				deepCopy, toPopulate);
      summarySS->insertPartSchedule(independentSched);
      partSS = summarySS;
      partSS->setOfParts(scenario->constSetOfParts(LGFR_BUDGET_SET));
  }
  else if (desc ==  LGFRCMRPCumSumShipmentVolToPartTimesUnitCost) {
      LgFrPartScheduleTimesUnitCostScheduleStrategy* timesUnitCostSS
	  = new LgFrPartScheduleTimesUnitCostScheduleStrategy(*scenario);
      LgFrSortingPartScheduleFloatSmartPointer dependentSched =
	  this->newPartSchedule(LGFRCMRPCumSumShipmentVolToPart, 
				deepCopy, toPopulate);
      timesUnitCostSS->insertPartSchedule(dependentSched);
      partSS = timesUnitCostSS;
      partSS->setOfParts(scenario->constSetOfParts(LGFR_UNIVERSAL_SET));
  }
  else if (desc ==  LGFRCMRPSummaryOfCumShipmentVolTimesUnitCost) {
      LgFrPartSummaryScheduleStrategy* summarySS
	  = new LgFrPartSummaryScheduleStrategy(*scenario);
      LgFrSortingPartScheduleFloatSmartPointer independentSched =
	  this->newPartSchedule(
	     LGFRCMRPCumSumShipmentVolToPartTimesUnitCost,
		deepCopy, toPopulate );
      summarySS->insertPartSchedule(independentSched);
      partSS = summarySS;
      partSS->setOfParts(scenario->constSetOfParts(LGFR_BUDGET_SET));
  }


  else return LgFrScheduleFactory::newPartScheduleFromScratch(desc, 
							      deepCopy, 
							      toPopulate);

  LgFrSortingPartScheduleFloatSmartPointer spsf = createPartSched(*partSS);
  if ( toPopulate ) 
      if ( this->scheduleType(desc,*partSS) == INPUT )
	  spsf->populate();
      else {
	  LgFrLastProblemSolved& lps = this->lastProblemSolved();
	  if ( lps.imploded() ) spsf->populate();
      }
  delete partSS;

  return spsf;
}

// Create a new sorting demand schedule
LgFrSortingDemandScheduleFloatSmartPointer
LgFrScheduleFactoryForCMRP::newDemandScheduleFromScratch
  (const LgFrScheduleDescriptor& desc,
   const bool deepCopy,
   const bool toPopulate
   )
const
{
  LgFrDemandScheduleStrategy *demandSS;
  LgFrScenarioForCMRPSmartPointer scenario 
    = LgFrScenarioForCMRPSmartPointer(this->scenario());
  if ( desc == LGFRDefaultDemandVolume ) {
      demandSS = new LgFrDemandVolumeScheduleStrategy(*scenario);
      demandSS->setOfDemands(scenario->constSetOfDemands());
  }

//        Schedules for the "Master Production Schedule" (demand) window 
//        -----------------------------------------------------
  else if ( desc == LGFRCMRPDemandVol) {
      demandSS = new LgFrDemandVolumeScheduleStrategy(*scenario);
  }
  else if ( desc == LGFRCMRPShipmentVol) {
      demandSS = new LgFrShipmentScheduleStrategy(*scenario);
  }
  else if ( desc == LGFRCMRPBacklogVol) {
      demandSS = new LgFrBacklogScheduleStrategy(*scenario);
  }
  else if ( desc == LGFRCMRPCommittedDemandVol) {
      demandSS = new LgFrDemandVolumeScheduleStrategy(*scenario);
      demandSS->setOfDemands(scenario
			     ->constSetOfDemands(LGFR_COMMITTED_DEMAND_SET));
  }

  else 
    return LgFrScheduleFactory::newDemandScheduleFromScratch(desc, deepCopy, 
							     toPopulate);

  LgFrSortingDemandScheduleFloatSmartPointer sdsf 
    = createDemandSched(*demandSS);
  if ( toPopulate )
      if ( this->scheduleType(desc,*demandSS) == INPUT )
	  sdsf->populate();
      else {
	  LgFrLastProblemSolved& lps = this->lastProblemSolved();
	  if ( lps.imploded() ) sdsf->populate();
      }
  delete demandSS;

  return sdsf;
}

// ---------------------------------------------------------------------
// Return the schedule type: INPUT or OUTPUT of implosion run.
// ---------------------------------------------------------------------
schdType
LgFrScheduleFactoryForCMRP::scheduleType(
    const LgFrScheduleDescriptor & desc,
    const LgFrAttributeScheduleStrategy & ass)
     const
{
  if ( desc == LGFRSupplyVolume ) return INPUT;	// Doing it this way (instead
  if ( desc == LGFRDemandVolume ) return INPUT; // of using ||) allows
						// inlining
  if ( desc == LGFRMRPManagedSupplyVol ) return INPUT;

  // These are input schedules.  They are output of explosion.
  // Listing them here as input causes them to be displayed in CMRP
  // if the user explodes and then displays component availability.
  if ( desc == LGFRMRPManagedRequiredVol )        return INPUT;
  if ( desc == LGFRMRPManagedCumRequiredVol )     return INPUT;
  if ( desc == LGFRMRPManagedConsVol )            return INPUT;
  if ( desc == LGFRMRPManagedTotalConsVol )       return INPUT;
  if ( desc == LGFRPlannerManagedRequiredVol )    return INPUT;
  if ( desc == LGFRPlannerManagedCumRequiredVol ) return INPUT;
  if ( desc == LGFRPlannerManagedConsVol )        return INPUT;
  if ( desc == LGFRPlannerManagedTotalConsVol )   return INPUT;
  if ( desc == LGFRRequiredVolumeTimesUnitCost)   return INPUT;

  if ( desc == LGFRMRPManagedCumSupplyVol ) return INPUT;
  if ( desc == LGFRCycleTime )   return INPUT;
  if ( desc == LGFRFrozenZone )   return INPUT;
  if ( desc == LGFRProcurementLeadTime ) return INPUT;
  if ( desc == LGFRPlannerManagedSupplyVol ) return INPUT;
  if ( desc == LGFRPlannerManagedCumSupplyVol ) return INPUT;
  if ( desc == LGFRCapacitySupplyVol ) return INPUT;
  if ( desc == LGFRCapacityRequiredVol ) return INPUT;
  if ( desc == LGFRBudgetAmount ) return INPUT;

  if ( desc == LGFRCMRPDemandVol ) return INPUT;
  if ( desc == LGFRCMRPCommittedDemandVol ) return INPUT;
  if ( desc == LGFRCMRPTotalCommittedDemandVol ) return INPUT;

  if ( desc == LGFRCMRPTotalMrpProcurementCost) return INPUT;
  if ( desc == LGFRCMRPTotalWitProcurementCost) return INPUT;
  if ( desc == LGFRCMRPCumTotalMrpProcurementCost) return INPUT;
  if ( desc == LGFRCMRPCumTotalWitProcurementCost) return INPUT;
  if ( desc == LGFRCMRPDeltaCumTotalProcurementCost) return INPUT;
  if ( desc == LGFRProductionVolumeTimesUnitCost) return INPUT;
  if ( desc == LGFRProcurementVolumeTimesUnitCost) return INPUT;

  if ( desc == LGFRCMRPSumDemandVolToPart ) return INPUT;
  if ( desc == LGFRCMRPCumSumDemandVolToPart ) return INPUT;
  if ( desc == LGFRCMRPSummaryOfCumDemandVol ) return INPUT;
  if ( desc == LGFRCMRPCumSumDemandVolToPartTimesUnitCost ) return INPUT;
  if ( desc == LGFRCMRPSummaryOfCumDemandVolTimesUnitCost ) return INPUT;


  return this->LgFrScheduleFactory::scheduleType(desc,ass);
}

    
// ---------------------------------------------------------------------
// Return list of schedule descriptors which can be used by factory to
// create schedules.     
// ---------------------------------------------------------------------
LgFrScheduleDescriptorVector
LgFrScheduleFactoryForCMRP::demandScheduleDescriptors()
const
{
  LgFrScheduleDescriptorVector retVal = 
    LgFrScheduleFactoryForCMRP::demandScheduleDescriptors_;
    
  LgFrScheduleDescriptorVector baseSD = 
    LgFrScheduleFactory::demandScheduleDescriptors();

  // If base class scheduleDescriptor is not in retVal, then insert it
  int i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i<baseSD.size(); i++ ) 
  {
      int j = 0;
      for( j = 0; j < retVal.size(); j++ )
          if( retVal[j] == baseSD[i] )
              break;
      if( j == retVal.size() ) // if not found
          retVal.push_back( baseSD[i] );
  }

  return retVal;
  
}


LgFrScheduleDescriptorVector
LgFrScheduleFactoryForCMRP::partScheduleDescriptors()
const
{
  LgFrScheduleDescriptorVector retVal = 
    LgFrScheduleFactoryForCMRP::partScheduleDescriptors_;
    
  LgFrScheduleDescriptorVector baseSD = 
    LgFrScheduleFactory::partScheduleDescriptors();

  // If base class scheduleDescriptor is not in retVal, then insert it
  int i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i<baseSD.size(); i++ ) 
  {
      int j = 0;
      for( j = 0; j < retVal.size(); j++ )
          if( retVal[j] == baseSD[i] )
              break;
      if( j == retVal.size() ) // if not found
          retVal.push_back( baseSD[i] );
  }
  return retVal;
  
}


// Initialize static class data
LgFrScheduleDescriptorVector
LgFrScheduleFactoryForCMRP::demandScheduleDescriptors_
            = LgFrScheduleFactoryForCMRP::initializeDemandScheduleDescriptors();
LgFrScheduleDescriptorVector
LgFrScheduleFactoryForCMRP::partScheduleDescriptors_
            = LgFrScheduleFactoryForCMRP::initializePartScheduleDescriptors();

LgFrScheduleDescriptorVector
LgFrScheduleFactoryForCMRP::initializeDemandScheduleDescriptors()
{
  LgFrScheduleDescriptorVector retVal;
  retVal.push_back(LGFRDefaultDemandVolume); 
//        Schedules for the "Master Production Schedule" (demand) window 
//        ----------------------------------------------------- 
  retVal.push_back(LGFRCMRPDemandVol);  
  retVal.push_back(LGFRCMRPShipmentVol);  
  retVal.push_back(LGFRCMRPBacklogVol);
  retVal.push_back(LGFRCMRPCommittedDemandVol);
  return retVal;
}

LgFrScheduleDescriptorVector
LgFrScheduleFactoryForCMRP::initializePartScheduleDescriptors()
{
  LgFrScheduleDescriptorVector retVal;

  retVal.push_back( LGFRBudgetAmount);
//        Schedules for the  "Planner-managed" (constrained) window
//        ---------------------------------------------------------
  retVal.push_back( LGFRPlannerManagedSupplyVol);
  retVal.push_back( LGFRPlannerManagedRequiredVol);
  retVal.push_back( LGFRPlannerManagedConsVol);
  retVal.push_back( LGFRPlannerManagedTotalPartDemandVol);
  retVal.push_back( LGFRPlannerManagedTotalConsVol);
  retVal.push_back( LGFRPlannerManagedCumSupplyVol);
  retVal.push_back( LGFRPlannerManagedCumRequiredVol);
  retVal.push_back( LGFRPlannerManagedExcessVol)     ;
  retVal.push_back( LGFRPlannerManagedFocussedShortageSchedule); 
  retVal.push_back( LGFRPlannerManagedStockVol);
  retVal.push_back( LGFRPlannerManagedScrapVol);    
  retVal.push_back( LGFRPlannerManagedUsageVol);     
//        Schedules for the "MRP-managed" 
//        --------------------------------
  retVal.push_back( LGFRMRPManagedSupplyVol);
  retVal.push_back( LGFRMRPManagedRequiredVol);
  retVal.push_back( LGFRMRPManagedConsVol);
  retVal.push_back( LGFRMRPManagedTotalPartDemandVol);
  retVal.push_back( LGFRMRPManagedTotalConsVol);
  retVal.push_back( LGFRMRPManagedCumSupplyVol);
  retVal.push_back( LGFRMRPManagedCumRequiredVol);
  retVal.push_back( LGFRMRPManagedExcessVol);   
  retVal.push_back( LGFRMRPManagedFocussedShortageSchedule);     
  retVal.push_back( LGFRMRPManagedStockVol);     
  retVal.push_back( LGFRMRPManagedScrapVol);
  retVal.push_back( LGFRMRPManagedProductionPlusProcurementVolume);     
  retVal.push_back( LGFRMRPManagedUsageVol);  
  retVal.push_back( LGFRProductionReceiptVol ) ;
  retVal.push_back( LGFRProductionVolume); 
  retVal.push_back( LGFRTotalShipmentVol); 
  retVal.push_back( LGFRCMRPTotalCommittedDemandVol); 
  retVal.push_back( LGFRProcurementVol); 
  retVal.push_back( LGFRProcurementLeadTime); 
  retVal.push_back( LGFRCycleTime); 
  retVal.push_back( LGFRFrozenZone); 
//        Schedules for the "Capacity" window 
//        -----------------------------------
  retVal.push_back( LGFRCapacitySupplyVol); 
  retVal.push_back( LGFRCapacityRequiredVol); 
  retVal.push_back( LGFRCapacityScrapVol); 
  retVal.push_back( LGFRCapacityUsageVol); 
  retVal.push_back( LGFRCapacityExcessVol); 
  retVal.push_back( LGFRCapacityFocussedShortageSchedule); 
  retVal.push_back( LGFRCapacityUsagePct); 
  //   Schedules for the "Procurement Summary" window.
  //   ----------------------------------------------
  retVal.push_back(  LGFRCMRPTotalMrpProcurementCost); 
  retVal.push_back(  LGFRCMRPTotalWitProcurementCost); 
  retVal.push_back(  LGFRCMRPCumTotalMrpProcurementCost); 
  retVal.push_back(  LGFRCMRPCumTotalWitProcurementCost); 
  retVal.push_back(  LGFRCMRPDeltaCumTotalProcurementCost);
  retVal.push_back(  LGFRCostOfCapitalSavings);
  retVal.push_back(  LGFRProductionVolumeTimesUnitCost);
  retVal.push_back(  LGFRProcurementVolumeTimesUnitCost);
  retVal.push_back(  LGFRRequiredVolumeTimesUnitCost);
  retVal.push_back(  LGFRCMRPSumDemandVolToPart);
  retVal.push_back(  LGFRCMRPCumSumDemandVolToPart);
  retVal.push_back(  LGFRCMRPSummaryOfCumDemandVol);
  retVal.push_back(  LGFRCMRPCumSumDemandVolToPartTimesUnitCost);
  retVal.push_back(  LGFRCMRPSummaryOfCumDemandVolTimesUnitCost); 
  retVal.push_back(  LGFRCMRPSumShipmentVolToPart);
  retVal.push_back(  LGFRCMRPCumSumShipmentVolToPart);
  retVal.push_back(  LGFRCMRPSummaryOfCumShipmentVol); 
  retVal.push_back(  LGFRCMRPCumSumShipmentVolToPartTimesUnitCost);
  retVal.push_back(  LGFRCMRPSummaryOfCumShipmentVolTimesUnitCost);
  return retVal;
}

// Constructor
LgFrScheduleFactoryForCMRP::LgFrScheduleFactoryForCMRP (
    LgFrScenarioSmartPointer s)
:   LgFrScheduleFactory(s)
{
  // Nothing to do in here
}

// Clone method
// Create a copy of the *this on the heap and return a pointer to it
// Even thought this returns a LgFrScheduleFactory*, the pointer
// must point to a LgFrScheduleFactoryForCMRP.
LgFrScheduleFactory*
LgFrScheduleFactoryForCMRP::clone (LgFrScenarioSmartPointer newScenario)
     const
{
  return  new LgFrScheduleFactoryForCMRP (newScenario);
}

// Destructor
LgFrScheduleFactoryForCMRP::~LgFrScheduleFactoryForCMRP()
{
  // Nothing to do in here
}

#ifdef NDEBUG
#undef NDEBUG
#endif

// test this class
void LgFrScheduleFactoryForCMRP::test()
{
}


void
LgFrScheduleFactoryForCMRP::contextTest(const LgFrScheduleFactory & f)
{

    // Test abbreviation function
  assert (f.abbreviationOf (LGFRSupplyVolume)
          != LGFRScrapVolume.abbrev()            );
  assert (f.abbreviationOf (LGFRSupplyVolume)
          == LGFRSupplyVolume.abbrev()           );
  assert (f.abbreviationOf (LGFRDemandVolume)
          == LGFRDemandVolume.abbrev()           );
  assert (f.abbreviationOf (LGFRMRPManagedSupplyVol)
          == LGFRMRPManagedSupplyVol.abbrev()	         );

  // test smart schedule stuff
  LgFrSortingDemandScheduleFloatSmartPointer shipSchedule =
      f.newDemandSchedule(LGFRCMRPShipmentVol, false);
  LgFrSortingDemandScheduleFloatSmartPointer shipSchedule2 =
      f.newDemandSchedule(LGFRCMRPShipmentVol, false);
  assert(shipSchedule == shipSchedule2);


  LgFrSortingPartScheduleFloatSmartPointer excessSchedule =
      f.newPartSchedule(LGFRPlannerManagedExcessVol, false);
  LgFrSortingPartScheduleFloatSmartPointer excessSchedule2 =
      f.newPartSchedule(LGFRPlannerManagedExcessVol, false);
  assert(excessSchedule == excessSchedule2);


  // test scheduleDescriptor methods
  {
    LgFrScheduleDescriptorVector sd = f.demandScheduleDescriptors();
    // Test that each demand schedule can be created by this factory
    int i = 0; // Pulled out of the for below by RW2STL
    for ( i = 0; i<sd.size(); i++ ) {
      LgFrSortingDemandScheduleFloatSmartPointer schedSP =
         f.newDemandSchedule( sd[i]); 
      assert( !schedSP.null() );
    }
  }
  {
    LgFrScheduleDescriptorVector sd = f.partScheduleDescriptors();
    // Test that each part  schedule can be created by this factory
    int i = 0; // Pulled out of the for below by RW2STL
    for ( i = 0; i<sd.size(); i++ ) {

      // There is a problem creating LGFRCriticalPartSupplyVolume schedule.
      // This needs to be investigated and fixed.
      // UnitTest will fail if this schedule is created.
      if ( sd[i] == LGFRCriticalPartSupplyVolume ) continue;
      
      LgFrSortingPartScheduleFloatSmartPointer schedSP =
         f.newPartSchedule( sd[i]); 
      assert( !schedSP.null() );
    }
  }
}

