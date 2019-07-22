// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
#include <string>
// End of RW2STL -- inserted includes.

#include <assert.h>
#include <math.h>
#include <wit/src/wit.h>

#include <scenario/src/ipCMRP.h>
#include <scenario/src/demand.h>
#include <scenario/src/dif.h>
#include <scenario/src/difCmrpF.h>
#include <scenario/src/difEssSc.h>
#include <scenario/src/calendar.h>
#include <scenario/src/sOfParts.h>
#include <scenario/src/sOfDemds.h>
#include <scenario/src/generVc.h>
#include <scenario/src/outInter.h>
#include <scenario/src/scenCMRP.h>
#include <scenario/src/schFCMRP.h>
#include <scenario/src/schdDeSt.h>
#include <scenario/src/idnumber.h>
#include <scenario/src/partSchP.h>
#include <scenario/src/customer.h>

#include <iostream>

const int DEFAULT_PRIORITY = 10;
const int FORECAST_PRIORITY = 100;

const float DEFAULT_OBJ1SHIPREWARD = 100.f;
const float FORECAST_OBJ1SHIPREWARD = 10.f;

const float DEFAULT_OBJ1SCRAPCOST = .0001f;

// Returns a description of the problem that is suitable for use as a
// window heading. This can not be used to set the title; only to query
// it.
std::string
LgFrInitialProblemForCMRP::title( 
    const LgFrDataInterfaceEssentials* difEssenPtr)
     const
{
  std::string theTitle("Constrained MRP: ");
  theTitle.append(CMRPscenario()->dataInterface()->title(difEssenPtr));
  return theTitle;
}

// get the scenario
LgFrScenarioForCMRPSmartPointer
LgFrInitialProblemForCMRP::CMRPscenario()
     const
{
    if (scenario()->GetScenarioType() != __LGFRSCENARIOFORCMRP)  {
	std::cerr << "the object is not a CMRPscenario and it is supposed to be"
             << std::endl;
	abort();
    }

    return LgFrScenarioForCMRPSmartPointer(scenario());  
}


// Populates the WitRun's data structures 
void
LgFrInitialProblemForCMRP::feedInto(
				     LgFrLastProblemSolved & lps)
{

  LgFrDataInterface* difPtr
    = (LgFrDataInterface*)(CMRPscenario()->dataInterface());
  LgFrCalendar& calendar
    = CMRPscenario()->calendar();
  LgFrSetOfParts& universalSOP =
                 CMRPscenario()->setOfParts(LGFR_UNIVERSAL_SET);
  LgFrSetOfParts& defaultSOP   =
                 CMRPscenario()->setOfParts(LGFR_DEFAULT_SET);
  LgFrSetOfParts& constrainedSOP   =
                 CMRPscenario()->setOfParts(LGFR_CONSTRAINED_SET);
  LgFrSetOfParts& produceableSOP  =
                 CMRPscenario()->setOfParts(LGFR_PRODUCEABLE_SET);
  LgFrSetOfParts& procureableSOP   =
                 CMRPscenario()->setOfParts(LGFR_PROCUREABLE_SET);
  LgFrSetOfParts& budgetSOP   =
                 CMRPscenario()->setOfParts(LGFR_BUDGET_SET);
  LgFrSetOfParts& mrpPlannedSOP   =
                 CMRPscenario()->setOfParts(LGFR_MRP_PLANNED_SET);
  LgFrSetOfParts& rawSOP   =
                 CMRPscenario()->setOfParts(LGFR_RAW_SET);
  LgFrSetOfParts& capacitySOP   =
                 CMRPscenario()->setOfParts(LGFR_CAPACITY_SET);
  LgFrSetOfDemands& universalSOD =
                 CMRPscenario()->setOfDemands(LGFR_UNIVERSAL_SET);
  LgFrSetOfDemands& defaultSOD   =
                 CMRPscenario()->setOfDemands(LGFR_DEFAULT_SET);
  LgFrSetOfDemands& committedSOD   =
                 CMRPscenario()->setOfDemands(LGFR_COMMITTED_DEMAND_SET);

  LgFrDataInterfaceEssentialsFromScenario difEssen(&(*CMRPscenario()));

#ifdef LGFR_ALLOW_BUILD_AHEAD
  int buildAheadLimit = calendar.nPeriods() - 1;
#else
  int buildAheadLimit = 0;
#endif


  // get the witRun pointer from lps
  WitRun * theWitRun = lps.witRun();

  witBoolean exists;
  witReturnCode rc;
  int i;
  size_t        numberOfPeriods = calendar.nPeriods();

  // Initialize some attribute values
  LgFrTimeVecFloat obj1ScrapCost(numberOfPeriods,DEFAULT_OBJ1SCRAPCOST);

  // ********************************************************
  // ****** Set some GLOBAL WIT parameters ******************
  // ********************************************************

  // shut off the message about a part name being too long
  rc = witSetMesgTimesPrint( theWitRun, WitTRUE, 338, 0);
  assert (rc < WitWARNING_RC);

  rc = witSetNPeriods(theWitRun, numberOfPeriods);
  assert (rc < WitWARNING_RC);

  rc = witSetBuildWhenEmpty(theWitRun, WitTRUE);
  assert (rc < WitWARNING_RC);

  rc = witSetComputeCriticalList(theWitRun, WitTRUE);
  assert (rc < WitWARNING_RC);

  rc = witSetObjChoice(theWitRun, 1);
  assert (rc < WitWARNING_RC);

#ifdef LGFR_USE_FOCUS_HORIZONS
  rc = witSetUseFocusHorizons( theWitRun, WitTRUE );
  assert (rc < WitWARNING_RC);
#else
  rc = witSetUseFocusHorizons( theWitRun, WitFALSE );
  assert (rc < WitWARNING_RC);
#endif


  // ********************************************************
  // ****** Get all the RAW parts from dif and process ******
  // ********************************************************

  LgFrDataInterface& dif = *difPtr;


  // get the raw parts
  LgFrPartVector pv;
  std::string itemSubTypeStr = "material";
  {
    LgFrPartVector* rawPartsPtr
      = (LgFrPartVector*) dif.partAttribute(pv, itemSubTypeStr,
                                                   "rawParts",
                                                   &difEssen);
    LgFrPartVector& rawParts = *rawPartsPtr;

    // get supplyVol's
    LgFrTimeVecFloatVector* supplyVolPtr
      = (LgFrTimeVecFloatVector*) dif.partAttribute(rawParts,
                                                    itemSubTypeStr,
                                                    "rawSupplyVol",
                                                    &difEssen);
    LgFrTimeVecFloatVector& supplyVol = *supplyVolPtr;

    // get unitCosts
    LgFrFloatVector* unitCostPtr
      = (LgFrFloatVector*) dif.partAttribute(rawParts, itemSubTypeStr,
                                             "rawUnitCost",
                                             &difEssen);
    LgFrFloatVector& unitCost = *unitCostPtr;

    // determine which are constrained
    LgFrIntVector* constrainedPtr
      = (LgFrIntVector*) dif.partAttribute(rawParts, itemSubTypeStr,
                                           "rawConstrained",
                                           &difEssen);
    LgFrIntVector& constrainedRawVec = *constrainedPtr;

    // get procurementLeadTime
    LgFrTimeVecFloatVector* procLeadTimePtr
      = (LgFrTimeVecFloatVector*) dif.partAttribute(rawParts,
                                                    itemSubTypeStr,
                                                    "rawProcLeadTime",
                                                    &difEssen);
    LgFrTimeVecFloatVector& procLeadTime = *procLeadTimePtr;

    // get procurement Costs
    LgFrFloatVector* procCostPtr
      = (LgFrFloatVector*) dif.partAttribute(rawParts, itemSubTypeStr,
                                             "rawProcCost",
                                             &difEssen);
    LgFrFloatVector& procCost = *procCostPtr;
    delete procCostPtr;

    float * negativeCycleTime = new float[numberOfPeriods];
    for (i=0; i<numberOfPeriods; i++)
      negativeCycleTime[i] = -1.0;

    std::cerr <<"INFO: LgFrInitialProblemForCMRP::feedInto() add raw parts" <<std::endl;
    for (i=0; i<rawParts.size(); i++) {

      // Check for part "Purchase_$$", and skip it if found.
      // If the dif data was generated by the CMRP application then
      // it contains the raw part "Purchase_$$".  This part should
      // not be added now.  It gets added latter in this method.
      if ( rawParts[i].name() == "Purchase_$$" ) {
         std::cerr <<"WARNING: Dif returned raw part Purchase_$$." <<std::endl;
         std::cerr <<"WARNING:     Purchase_$$ part is being skipped over." <<std::endl;
         continue;
      }

      // do a sanity check to make sure part is not in WIT yet
      rc = witGetPartExists(theWitRun, rawParts[i].name().c_str(), &exists);
      assert (rc < WitWARNING_RC);
      assert (! exists);

      // add the part as a product

      rc =
         witAddPart (
            theWitRun,
            rawParts[i].name ().c_str (),
            WitPRODUCT);

      assert (rc < WitWARNING_RC);

      rc =
         witSetPartSupplyVol (
            theWitRun,
            rawParts[i].name ().c_str (),
            supplyVol[i].data ());

      assert (rc < WitWARNING_RC);

      rc =
         witSetPartUnitCost (
            theWitRun,
            rawParts[i].name ().c_str (),
            unitCost[i]);

      assert (rc < WitWARNING_RC);

      rc =
         witSetPartObj1ScrapCost (
            theWitRun,
            rawParts[i].name ().c_str (),
            obj1ScrapCost.data ());

      assert (rc < WitWARNING_RC);

      rc =
         witSetPartCycleTime (
            theWitRun,
            rawParts[i].name ().c_str (),
            constrainedRawVec[i]? 
               negativeCycleTime:
               procLeadTime[i].data ());

      assert (rc < WitWARNING_RC);

      // Set Stock Cost From Unit Cost
      LgFrTimeVecFloat stockCost =
        LgFrTimeVecFloat( numberOfPeriods, unitCost[i] ) * .0005f;
      witSetPartObj1StockCost( theWitRun, rawParts[i].name().c_str(),
                               stockCost.data() );

      // put the part into the default and raw SOP
      defaultSOP.insert(rawParts[i]);
      rawSOP.insert(rawParts[i]);

      // add the part to the constrainedSOP if it's constrained otherwise
      // add to the procureableSOP
      if (constrainedRawVec[i])
        constrainedSOP.insert(rawParts[i]);
      else  {
        procureableSOP.insert(rawParts[i]);
        mrpPlannedSOP.insert(rawParts[i]);
      }
    }

    delete procLeadTimePtr;
    delete rawPartsPtr;
    delete constrainedPtr;
    delete unitCostPtr;
    delete supplyVolPtr; 
    delete [] negativeCycleTime;
  }

  // *************************************************************
  // ****** Get all the CAPACITY parts from dif and process ******
  // *************************************************************

  std::cerr <<"INFO: LgFrInitialProblemForCMRP::feedInto() get cap parts from dif" <<std::endl;

  {

    // get the capacity parts
    LgFrPartVector* capacityPartsPtr
      = (LgFrPartVector*) dif.partAttribute(pv, itemSubTypeStr,
                                                   "capacityParts",
                                                   &difEssen);
    LgFrPartVector& capacityParts = *capacityPartsPtr;

    rc = witGetPartExists(theWitRun, capacityParts[0].name().c_str(), &exists);

    // get capacity supplyVol's
    LgFrTimeVecFloatVector* supplyVolPtr
      = (LgFrTimeVecFloatVector*) dif.partAttribute(capacityParts,
                                                    itemSubTypeStr,
                                                    "capacitySupplyVol",
                                                    &difEssen);
    LgFrTimeVecFloatVector& supplyVol = *supplyVolPtr;

    rc = witGetPartExists(theWitRun, capacityParts[0].name().c_str(), &exists);
    // get capacity unitCosts
    LgFrFloatVector* unitCostPtr
      = (LgFrFloatVector*) dif.partAttribute(capacityParts,
                                             itemSubTypeStr,
                                             "capacityUnitCost",
                                             &difEssen);
    LgFrFloatVector& unitCost = *unitCostPtr;

    std::cerr <<"INFO: LgFrInitialProblemForCMRP::feedInto() add cap parts" <<std::endl;
    for ( i=0; i<capacityParts.size(); i++) {
      // do a sanity check to make sure part is not in WIT yet
      rc = witGetPartExists(theWitRun, capacityParts[i].name().c_str(), &exists);
      assert (rc < WitWARNING_RC);
      assert (! exists);
      //    std::cerr << "Adding capacity part: " << capacityParts[i].name() << "\n";

      // add the part as a product

      rc =
         witAddPart (
            theWitRun,
            capacityParts[i].name ().c_str (),
            WitCAPACITY);

      assert (rc < WitWARNING_RC);

      rc =
         witSetPartSupplyVol (
            theWitRun,
            capacityParts[i].name ().c_str (),
            supplyVol[i].data ());

      assert (rc < WitWARNING_RC);

      rc =
         witSetPartUnitCost (
            theWitRun,
            capacityParts[i].name ().c_str (),
            unitCost[i]);

      assert (rc < WitWARNING_RC);

      rc =
         witSetPartObj1ScrapCost (
            theWitRun,
            capacityParts[i].name ().c_str (),
            obj1ScrapCost.data ());

      assert (rc < WitWARNING_RC);

      // put the part into the default and capacity SOP's
      defaultSOP.insert(capacityParts[i]);
      capacitySOP.insert(capacityParts[i]);



    }

    delete supplyVolPtr;
    delete unitCostPtr;
    delete capacityPartsPtr;  
  }

  // ************************************************************
  // ****** Get all the PRODUCT parts from dif and process ******
  // ************************************************************

  std::cerr <<"INFO: LgFrInitialProblemForCMRP::feedInto() get prd parts from dif" <<std::endl;
  {
    // get the product parts
    LgFrPartVector* productPartsPtr
      = (LgFrPartVector*) dif.partAttribute(pv, itemSubTypeStr,
                                                   "productParts",
                                                   &difEssen);
    LgFrPartVector& productParts = *productPartsPtr;

    // get product supplyVol's
    LgFrTimeVecFloatVector* supplyVolPtr
      = (LgFrTimeVecFloatVector*) dif.partAttribute(productParts,
                                                    itemSubTypeStr,
                                                    "productSupplyVol",
                                                    &difEssen);
    LgFrTimeVecFloatVector& supplyVol = *supplyVolPtr;

    // get product cycleTimes
    LgFrTimeVecFloatVector* cycleTimePtr
      = (LgFrTimeVecFloatVector*) dif.partAttribute(productParts,
                                                    itemSubTypeStr,
                                                    "productCycleTime",
                                                    &difEssen);
    LgFrTimeVecFloatVector& cycleTime = *cycleTimePtr;

    // get product unitCosts
    LgFrFloatVector* unitCostPtr
      = (LgFrFloatVector*) dif.partAttribute(productParts,
                                             itemSubTypeStr,
                                             "productUnitCost",
                                             &difEssen);
    LgFrFloatVector& unitCost = *unitCostPtr;

    // get product minLotSize
    LgFrTimeVecFloatVector* minLotSizePtr
      = (LgFrTimeVecFloatVector*) dif.partAttribute(productParts, itemSubTypeStr,
                                                    "productMinLotSize",
                                                    &difEssen);
    LgFrTimeVecFloatVector& minLotSize = *minLotSizePtr;

    std::cerr <<"INFO: LgFrInitialProblemForCMRP::feedInto() add prd parts" <<std::endl;
    for (i=0; i<productParts.size(); i++) {
      // do a sanity check to make sure part is not in WIT yet
      rc = witGetPartExists(theWitRun, productParts[i].name().c_str(), &exists);
      assert (rc < WitWARNING_RC);
      assert (! exists);

      // add the part as a product

      rc =
         witAddPart (
            theWitRun,
            productParts[i].name ().c_str (),
            WitPRODUCT);

      assert (rc < WitWARNING_RC);

      rc =
         witSetPartSupplyVol (
            theWitRun,
            productParts[i].name ().c_str (),
            supplyVol[i].data ());

      assert (rc < WitWARNING_RC);

      rc =
         witSetPartCycleTime (
            theWitRun,
            productParts[i].name ().c_str (),
            cycleTime[i].data ());

      assert (rc < WitWARNING_RC);

      rc =
         witSetPartObj1ScrapCost (
            theWitRun,
            productParts[i].name ().c_str (),
            obj1ScrapCost.data ());

      assert (rc < WitWARNING_RC);

      rc =
         witSetPartUnitCost (
            theWitRun,
            productParts[i].name ().c_str (),
            unitCost[i]);

      assert (rc < WitWARNING_RC);

      rc =
         witSetPartMinLotSize (
            theWitRun,
            productParts[i].name ().c_str (),
            minLotSize[i].data ());

      assert (rc < WitWARNING_RC);

      // Set Stock Cost From Unit Cost
      LgFrTimeVecFloat stockCost =
        LgFrTimeVecFloat( numberOfPeriods, unitCost[i] ) * .0005f;
      witSetPartObj1StockCost( theWitRun, productParts[i].name().c_str(),
                               stockCost.data() );
      
      // put the part into the default and mrpPlanned SOP's
      defaultSOP.insert(productParts[i]);
      mrpPlannedSOP.insert(productParts[i]);

      // put the part into the produceable set
      produceableSOP.insert(productParts[i]);
    }
    delete supplyVolPtr;
    delete unitCostPtr;
    delete cycleTimePtr;
    delete minLotSizePtr;

    // If incLotSize attribute is defined in DIF, then set it.
    {
    bool incLotSizeSupported =  dif.isAttributeFunctionImplemented(
                                         "productIncLotSize",
                                         material,
                                         part );
    if ( !incLotSizeSupported ) {
       std::cerr <<"LgFrINFO: Data Interface Attribute productIncLotSize " <<std::endl
            <<"          is not supported."                           <<std::endl
            <<"          " << title()
            <<std::endl;
    }
    else {
      LgFrTimeVecFloatVector* incLotSizePtr
        = (LgFrTimeVecFloatVector*) dif.partAttribute(productParts,
                                                      itemSubTypeStr,
                                                      "productIncLotSize",
                                                      &difEssen);
      LgFrTimeVecFloatVector& incLotSize = *incLotSizePtr;
      const float fltEps = .01f;
      for (i=0; i<productParts.size(); i++) {
        if ( fabs( incLotSize[i].max() - 1.0 ) > fltEps ) {
	  std::cerr << "LgFrINFO: Setting product part: "
               <<            productParts[i].name()
               <<            std::endl
	       << "          incLotSize to "
               <<            incLotSize[i].format() << std::endl;
        }

        lps.incLotSize( &(productParts[i]), incLotSize[i] );
      }
      delete incLotSizePtr;
    }
    }


    // If yield attribute is defined in DIF, then set it.
    {
    bool yieldSupported =  dif.isAttributeFunctionImplemented(
                                         "productYieldFloat",
                                         material,
                                         part );
    if ( !yieldSupported ) {
       std::cerr <<"LgFrINFO: Data Interface Attribute productYieldFloat " <<std::endl
            <<"          is not supported."                           <<std::endl
            <<"          " << title()
            <<std::endl;
    }
    else {
      LgFrTimeVecFloatVector* yieldPtr
        = (LgFrTimeVecFloatVector*) dif.partAttribute(productParts,
                                                      itemSubTypeStr,
                                                      "productYieldFloat",
                                                      &difEssen);
      LgFrTimeVecFloatVector yield =  *yieldPtr;
      for (i=0; i<productParts.size(); i++) {
        if (yield[i].min() < 100) {
	  std::cerr << "LgFrINFO: Setting product part: "
               <<            productParts[i].name()
               <<            std::endl
	       << "          yield to "
               <<            yield[i].format() << std::endl;
        }

        lps.yield( &(productParts[i]), yield[i] );
      }
      delete yieldPtr;
    }
    }

    delete productPartsPtr;
  }       

  LgFrPart budgetPart = universalSOP.newPart("Purchase_$$");

  // ************************************************************
  // ****** Process DEMANDS *************************************
  // ************************************************************

  std::cerr <<"INFO: LgFrInitialProblemForCMRP::feedInto() get committed demands from dif"
       <<std::endl;
  LgFrDemandVector dv;


  // get the COMMITTED CUSTOMER Orders:
  LgFrDemandVector* committedDemandPtr
    = (LgFrDemandVector*) dif.demandAttribute(dv, "committedDemands",
                                                     &difEssen );
  LgFrDemandVector& committedDemands = * committedDemandPtr; 

  
  // get demandVol's
  LgFrTimeVecFloatVector* committedDemandVolPtr
    = (LgFrTimeVecFloatVector*) dif.demandAttribute(committedDemands,
                                                    "demandVol", &difEssen);
  LgFrTimeVecFloatVector& demandVol = *committedDemandVolPtr;


  // get demand grossRev's
  LgFrFloatVector* committedGrossRevPtr
    = (LgFrFloatVector*) dif.demandAttribute(committedDemands,
                                             "demandGrossRev", &difEssen);
  LgFrFloatVector& grossRev = *committedGrossRevPtr;

  std::cerr <<"INFO: LgFrInitialProblemForCMRP::feedInto() add committed demands" <<std::endl;
  for (i=0; i<committedDemands.size(); i++) {
    // do a few sanity check's
    rc = witGetPartExists(theWitRun,
                          committedDemands[i].partPtr()->name().c_str(), &exists);
    assert (rc < WitWARNING_RC);
    assert (exists);

    rc =
      witAddDemand (
         theWitRun,
         committedDemands[i].partPtr ()->name ().c_str (),
         committedDemands[i].name ().c_str ());

    assert (rc < WitWARNING_RC);

    rc =
      witSetDemandDemandVol (
         theWitRun,
         committedDemands[i].partPtr ()->name ().c_str (),
         committedDemands[i].name ().c_str (),
         demandVol[i].data ());

    assert (rc < WitWARNING_RC);

    rc =
      witSetDemandGrossRev (
         theWitRun,
         committedDemands[i].partPtr ()->name ().c_str (),
         committedDemands[i].name ().c_str (),
         grossRev[i]);

    assert (rc < WitWARNING_RC);

    rc =
      witSetDemandBuildAheadLimit (
         theWitRun,
         committedDemands[i].partPtr ()->name ().c_str (),
         committedDemands[i].name ().c_str (),
         buildAheadLimit);

    assert (rc < WitWARNING_RC);

    // put the demand into the default set
    defaultSOD.insert(committedDemands[i]);
    committedSOD.insert(committedDemands[i]);    
  }


  //  delete committedDemandPtr;
  //  delete committedDemandVolPtr;
  //  delete committedGrossRevPtr;



  // get the firmFORECASTED Demands
  std::cerr <<"INFO: LgFrInitialProblemForCMRP::feedInto() get firmForecast demands from dif"
       <<std::endl;

  LgFrDemandVector* firmForecastDemandsPtr
    = (LgFrDemandVector*) dif.demandAttribute(dv,"firmForecastDemands",
                                                     &difEssen);
  LgFrDemandVector firmForecastDemands = *firmForecastDemandsPtr;

  // nettedForecast -- compute the size of the (potential) netted forecast
  // vector.  (Need to declare firmForecastDemands and totCommDemand here
  // so that they don't go out of scope
  int sizeOfNettedDemandVol;
  if (dif.title(&difEssen) == "witlib") 
    sizeOfNettedDemandVol = firmForecastDemands.size();
  else
    sizeOfNettedDemandVol = 0;
  LgFrTimeVecFloatVector nettedDemandVol(sizeOfNettedDemandVol);
  LgFrSortingPartScheduleFloatSmartPointer totCommDemand;

  // get demandVol's
  LgFrTimeVecFloatVector* firmForecastDemandVolPtr
    = (LgFrTimeVecFloatVector*) dif.demandAttribute(firmForecastDemands,
                                                    "demandVol",
                                                    &difEssen);
  demandVol = *firmForecastDemandVolPtr;

  // MUST net-out the committed demand from the forecast
  // for mapics dif

  if (dif.title(&difEssen) == "witlib")    {
    // get a schFctry
    const LgFrScheduleFactory * sfact = CMRPscenario()->scheduleFactory();
    totCommDemand = sfact->newPartSchedule(LGFRCMRPTotalCommittedDemandVol);
    const LgFrSortingPartScheduleFloat & constTotCommDemand = *totCommDemand;

    for (i=0; i<firmForecastDemands.size(); i++) {
      const LgFrPart * partPtr = firmForecastDemands[i].partPtr();
      const LgFrTimeVecFloat & tcdvTVF
        = (constTotCommDemand[*partPtr]).timeVecFloat();
      const LgFrTimeVecFloat & forecastTVF = demandVol[i];
      LgFrTimeVecFloat nettedForecastTVF = forecastTVF.cumulativeNet(tcdvTVF);
      nettedDemandVol[i] = nettedForecastTVF;
    }
  }

  // get grossRev's
  LgFrFloatVector* firmForecastGrossRevPtr
    = (LgFrFloatVector*) dif.demandAttribute(
                                             firmForecastDemands,
                                             "demandGrossRev",
                                             &difEssen);
  grossRev = *firmForecastGrossRevPtr;

  if (firmForecastDemands.size() > 0)
      std::cerr <<"INFO: LgFrInitialProblemForCMRP::feedInto() add firmForecast demands" 
       <<std::endl;

  for (i=0; i<firmForecastDemands.size(); i++) {
    // do a few sanity check's
    rc = witGetPartExists(theWitRun,
			  firmForecastDemands[i].partPtr()->name().c_str(),
			  &exists);
    assert (rc < WitWARNING_RC);
    assert (exists);

    // add the demand

    rc =
       witAddDemand (
          theWitRun,
          firmForecastDemands[i].partPtr ()->name ().c_str (),
          firmForecastDemands[i].name ().c_str ());

    assert (rc < WitWARNING_RC);

    rc =
       witSetDemandGrossRev (
          theWitRun,
          firmForecastDemands[i].partPtr ()->name ().c_str (),
          firmForecastDemands[i].name ().c_str (),
          grossRev[i]);

    assert (rc < WitWARNING_RC);

    rc =
       witSetDemandBuildAheadLimit (
          theWitRun,
          firmForecastDemands[i].partPtr ()->name ().c_str (),
          firmForecastDemands[i].name ().c_str (),
          buildAheadLimit);

    assert (rc < WitWARNING_RC);

    if (dif.title (& difEssen) == "witlib")
       rc =
          witSetDemandDemandVol (
             theWitRun,
             firmForecastDemands[i].partPtr ()->name ().c_str (),
             firmForecastDemands[i].name ().c_str (),
             nettedDemandVol[i].data ());
    else
       rc =
          witSetDemandDemandVol (
             theWitRun,
             firmForecastDemands[i].partPtr ()->name ().c_str (),
             firmForecastDemands[i].name ().c_str (),
             demandVol[i].data ());

    assert (rc < WitWARNING_RC);

    // put the demand into the default set
    defaultSOD.insert(firmForecastDemands[i]);

  }

  //  delete firmForecastDemandsPtr;
  //  delete firmForecastDemandVolPtr;
  //  delete firmForecastGrossRevPtr;


  std::cerr <<"INFO: LgFrInitialProblemForCMRP::feedInto() set demand prorities " <<std::endl;
  // Set demand priorities
  setDemandPriorities(lps);

  std::cerr <<"INFO: LgFrInitialProblemForCMRP::feedInto() set demand obj1ShipReward " <<std::endl;
  // Set demand ship rewards
  setDemandObj1ShipReward(lps);


  // ********************************************************
  // ****** Set Equitability based on number of demands and periods
  // ********************************************************
  lps.equitability (
#ifdef LGFR_NO_EQUITABILITY
		    1
#else
		      ( numberOfPeriods * 
			( committedDemands.size() 
			  + firmForecastDemands.size() 
			  )
			> 500 
			)
		      ? 10
		      : 100
#endif
		      );

  // ************************************************************
  // ****** Set build-ahead *************************************
  // ************************************************************
  lps.buildAhead (  (buildAheadLimit == 0) ? false : true  );


  delete committedDemandVolPtr;
  delete committedGrossRevPtr;
  delete firmForecastDemandVolPtr;
  delete firmForecastGrossRevPtr;
  delete committedDemandPtr;
  delete firmForecastDemandsPtr;


  // ************************************************************
  // ****** Process BOM *****************************************
  // ************************************************************

  std::cerr <<"INFO: LgFrInitialProblemForCMRP::feedInto() set BOM" <<std::endl;
  dif.partAttribute(
                    pv, itemSubTypeStr, "defineBom", &difEssen);


  // ************************************************************
  // ****** Process Budget Part and BOM Structure ***************
  // ************************************************************

  std::cerr <<"INFO: LgFrInitialProblemForCMRP::feedInto() Process budget" <<std::endl;
  float * budgetSupplyVol = new float[numberOfPeriods];
  for (i=0; i<numberOfPeriods; i++)
    budgetSupplyVol[i] = 100.0;

  rc = witGetPartExists(theWitRun, "Purchase_$$", &exists);
  assert (rc < WitWARNING_RC);
  assert (! exists);


  //  std::cerr << "Adding budget part: Purchase_$$\n";

  rc = witAddPart (theWitRun, "Purchase_$$", WitRAW);

  assert (rc < WitWARNING_RC);

  rc = witSetPartSupplyVol (theWitRun, "Purchase_$$", budgetSupplyVol);

  assert (rc < WitWARNING_RC);

  rc = witSetPartUnitCost (theWitRun, "Purchase_$$", 1.0);

  assert (rc < WitWARNING_RC);

  rc =
     witSetPartObj1ScrapCost (theWitRun, "Purchase_$$", obj1ScrapCost.data ());

  assert (rc < WitWARNING_RC);


  // add the budget part to the universal sop but NOT the default sop

  // LgFrPart budgetPart = universalSOP.newPart("Purchase_$$");


  budgetSOP.insert(budgetPart);

  delete [] budgetSupplyVol;

  std::cerr <<"INFO: Leaving LgFrInitialProblemForCMRP::feedInto()" <<std::endl;

}

// Set demand priorities based on the type of demand
void
LgFrInitialProblemForCMRP::setDemandPriorities (
  LgFrLastProblemSolved & lps )
{
  witReturnCode rc;
  WitRun * theWitRun = lps.witRun();

  int numberOfPeriods;
  rc = witGetNPeriods( theWitRun, &numberOfPeriods );
  assert (rc < WitWARNING_RC);

  LgFrSetOfDemands& defaultSOD   =
                 CMRPscenario()->setOfDemands(LGFR_DEFAULT_SET);
  LgFrSetOfDemands& committedSOD   =
                 CMRPscenario()->setOfDemands(LGFR_COMMITTED_DEMAND_SET);

  int * defaultPriority = new int[numberOfPeriods];
  int * forecastPriority = new int[numberOfPeriods];
  int * priority;

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i<numberOfPeriods; i++ ) 
     defaultPriority[i] = DEFAULT_PRIORITY;
  for ( i = 0; i<numberOfPeriods; i++ ) 
     forecastPriority[i] = FORECAST_PRIORITY;

  for ( i = 0; i<defaultSOD.size(); i++ ) {

     if (  committedSOD.contains( defaultSOD[i] )  )
        priority = defaultPriority;
     else 
        priority = forecastPriority;

     rc = witSetDemandPriority(theWitRun, 
                               defaultSOD[i].partPtr()->name().c_str(),
                               defaultSOD[i].name().c_str(),
                               priority );
     assert (rc < WitWARNING_RC);

  }

  delete [] defaultPriority;
  delete [] forecastPriority;
}



// Set demand obj1ShipReward based on the type of demand
void
LgFrInitialProblemForCMRP::setDemandObj1ShipReward (
  LgFrLastProblemSolved & lps )
{
  witReturnCode rc;
  WitRun * theWitRun = lps.witRun();

  int numberOfPeriods;
  rc = witGetNPeriods( theWitRun, &numberOfPeriods );
  assert (rc < WitWARNING_RC);

  LgFrSetOfDemands& defaultSOD   =
                 CMRPscenario()->setOfDemands(LGFR_DEFAULT_SET);
  LgFrSetOfDemands& committedSOD   =
                 CMRPscenario()->setOfDemands(LGFR_COMMITTED_DEMAND_SET);

  LgFrTimeVecFloat defaultObj1ShipReward(
     numberOfPeriods,DEFAULT_OBJ1SHIPREWARD);
  LgFrTimeVecFloat forecastObj1ShipReward(
     numberOfPeriods,FORECAST_OBJ1SHIPREWARD);
  LgFrTimeVecFloat * obj1ShipReward;

  defaultObj1ShipReward  = DEFAULT_OBJ1SHIPREWARD;
  forecastObj1ShipReward = FORECAST_OBJ1SHIPREWARD;

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i<defaultSOD.size(); i++ ) {

     if (  committedSOD.contains( defaultSOD[i] )  )
        obj1ShipReward = &defaultObj1ShipReward;
     else
        obj1ShipReward = &forecastObj1ShipReward;

     rc = witSetDemandObj1ShipReward(theWitRun,
                               defaultSOD[i].partPtr()->name().c_str(),
                               defaultSOD[i].name().c_str(),
                               obj1ShipReward->data() );
     assert (rc < WitWARNING_RC);

     rc = witSetDemandObj1CumShipReward(theWitRun,
                               defaultSOD[i].partPtr()->name().c_str(),
                               defaultSOD[i].name().c_str(),
                               (.001f*(*obj1ShipReward)).data() );
     assert (rc < WitWARNING_RC);

  }

}


// Make scenario clone copy of object
LgFrInitialProblemAndParameters *
LgFrInitialProblemForCMRP::clone(
   LgFrScenarioSmartPointer newScenario)
const
{
  return new LgFrInitialProblemForCMRP(newScenario);
}



LgFrInitialProblemForCMRP::~LgFrInitialProblemForCMRP()
{
  // Nothing to do, so do nothing but return
}


// Default Constructor
LgFrInitialProblemForCMRP::LgFrInitialProblemForCMRP()
:
LgFrInitialProblemAndParameters(
			LgFrScenarioSmartPointer::nullScenarioSmartPointer)
{
  // All the work is done in the initialization part
}

// the preferred Constructor
LgFrInitialProblemForCMRP::LgFrInitialProblemForCMRP(
    LgFrScenarioSmartPointer scenario)
:
    LgFrInitialProblemAndParameters(scenario)
{
  // All the work is done in the initialization part
}


// *************************************************************************
//
// LgFrScenarioForCMRPwithPrioritiesFromDif
//
// *************************************************************************


// Set demand priorities based on the type of demand
void
LgFrInitialProblemForCMRPwithPrioritiesFromDif::setDemandPriorities (
  LgFrLastProblemSolved & lps )
{

  LgFrSetOfDemands& defaultSOD = CMRPscenario()->setOfDemands(LGFR_DEFAULT_SET);

  LgFrDataInterface*  difPtr
      = (LgFrDataInterface*)(CMRPscenario()->dataInterface());

  LgFrDataInterface& difWithPriorities = *difPtr;

  // Make dif supports returning of priorities
  assert( difWithPriorities.dataInterfaceImplementation()->GetDIIType() 
	     == __LGFRDATAINTERFACEIMPLEMENTATIONFROMCMRPFILES
	  || difWithPriorities.dataInterfaceImplementation()->GetDIIType()
	     == __LGFRDATAINTERFACEIMPLEMENTATIONFROMWITDATAFILE);

  // get demandPriority's
  LgFrDataInterfaceEssentialsFromScenario difEssen(&(*CMRPscenario()));
  LgFrTimeVecFloatVector* demandPriorityPtr
      = (LgFrTimeVecFloatVector*) difWithPriorities.demandAttribute(
	  defaultSOD, "demandPriority", &difEssen);
  LgFrTimeVecFloatVector& priorityVec = *demandPriorityPtr;

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i<setOfDemands().size(); i++ ) {

     lps.priority( &defaultSOD[i], priorityVec[i] );

  }
  delete demandPriorityPtr;
  
}


// Set demand obj1ShipReward based on the type of demand
void
LgFrInitialProblemForCMRPwithPrioritiesFromDif::setDemandObj1ShipReward (
  LgFrLastProblemSolved & lps )
{
  LgFrSetOfDemands& defaultSOD = CMRPscenario()->setOfDemands(LGFR_DEFAULT_SET);

  LgFrDataInterface*  difPtr
      = (LgFrDataInterface*)(CMRPscenario()->dataInterface());

  LgFrDataInterface& difWithPriorities = *difPtr;

  // Make dif supports returning of obj1ShipReward
  assert( difWithPriorities.dataInterfaceImplementation()->GetDIIType() 
	     == __LGFRDATAINTERFACEIMPLEMENTATIONFROMCMRPFILES
	  || difWithPriorities.dataInterfaceImplementation()->GetDIIType()
	     == __LGFRDATAINTERFACEIMPLEMENTATIONFROMWITDATAFILE);


  // Get obj1ShipRewardVect
  LgFrDataInterfaceEssentialsFromScenario difEssen(&(*CMRPscenario()));
  LgFrTimeVecFloatVector* obj1ShipRewardVectPtr
   = (LgFrTimeVecFloatVector*) difWithPriorities.demandAttribute
      (defaultSOD, "demandObj1ShipReward", &difEssen);
  LgFrTimeVecFloatVector& obj1ShipRewardVec = *obj1ShipRewardVectPtr;

  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i<setOfDemands().size(); i++ ) {

     lps.obj1ShipReward( &defaultSOD[i], obj1ShipRewardVec[i] );
     lps.obj1CumShipReward( &defaultSOD[i], obj1ShipRewardVec[i]*.001f );

  }

  delete obj1ShipRewardVectPtr;

}


// Make scenario clone copy of object
LgFrInitialProblemAndParameters *
LgFrInitialProblemForCMRPwithPrioritiesFromDif::clone(
   LgFrScenarioSmartPointer newScenario)
const
{
  LgFrInitialProblemAndParameters* newInst =
      new LgFrInitialProblemForCMRPwithPrioritiesFromDif(newScenario);
  return (newInst);
}



LgFrInitialProblemForCMRPwithPrioritiesFromDif::~LgFrInitialProblemForCMRPwithPrioritiesFromDif()
{
  // Nothing to do, so do nothing but return
}

// Default Constructor
LgFrInitialProblemForCMRPwithPrioritiesFromDif::
   LgFrInitialProblemForCMRPwithPrioritiesFromDif()
:
    LgFrInitialProblemForCMRP(
					LgFrScenarioSmartPointer::nullScenarioSmartPointer)
{
  // All the work is done in the initialization part
}

// the preferred Constructor
LgFrInitialProblemForCMRPwithPrioritiesFromDif::LgFrInitialProblemForCMRPwithPrioritiesFromDif(LgFrScenarioSmartPointer scenario)
:
    LgFrInitialProblemForCMRP(scenario)
{
  // All the work is done in the initialization part
}



// *************************************************************************
//
// Test Methods
//
// *************************************************************************
#ifdef NDEBUG
#undef NDEBUG
#endif

// Test this class
void
LgFrInitialProblemForCMRP::test()
{


}


// Context sensitive test called by scenCMRP.test()
void
LgFrInitialProblemForCMRP::contextTest(
    LgFrInitialProblemForCMRP        & ippTest, // ippTest,
    LgFrInitialProblemForCMRP        & ippPWcmrp)   // ippPWcmrp)
{
  std::cerr << "\nEntering IPPforCMRP::contextTest\n";

  LgFrCalendar&     calendar   = ippTest.CMRPscenario()->calendar();
  LgFrSetOfParts& universalSOP =
                 ippTest.CMRPscenario()->setOfParts(LGFR_UNIVERSAL_SET);
  LgFrSetOfParts& defaultSOP   =
                 ippTest.CMRPscenario()->setOfParts(LGFR_DEFAULT_SET);
  LgFrSetOfParts& constrainedSOP   =
                 ippTest.CMRPscenario()->setOfParts(LGFR_CONSTRAINED_SET);
  LgFrSetOfParts& produceableSOP  =
                 ippTest.CMRPscenario()->setOfParts(LGFR_PRODUCEABLE_SET);
  LgFrSetOfParts& procureableSOP   =
                 ippTest.CMRPscenario()->setOfParts(LGFR_PROCUREABLE_SET);
  LgFrSetOfParts& budgetSOP   =
                 ippTest.CMRPscenario()->setOfParts(LGFR_BUDGET_SET);
  LgFrSetOfParts& mrpPlannedSOP   =
                 ippTest.CMRPscenario()->setOfParts(LGFR_MRP_PLANNED_SET);
  LgFrSetOfParts& rawSOP   =
                 ippTest.CMRPscenario()->setOfParts(LGFR_RAW_SET);
  LgFrSetOfParts& capacitySOP   =
                 ippTest.CMRPscenario()->setOfParts(LGFR_CAPACITY_SET);
  LgFrSetOfDemands& universalSOD =
                 ippTest.CMRPscenario()->setOfDemands(LGFR_UNIVERSAL_SET);
  LgFrSetOfDemands& defaultSOD   =
                 ippTest.CMRPscenario()->setOfDemands(LGFR_DEFAULT_SET);
  LgFrSetOfDemands& committedSOD   =
                 ippTest.CMRPscenario()->setOfDemands(LGFR_COMMITTED_DEMAND_SET);


  assert(universalSOP.size() == 4);
  assert(defaultSOP.size() == 3);
  assert(budgetSOP.size() == 1);
  assert(constrainedSOP.size() == 0);
  assert(produceableSOP.size() == 1);
  assert(procureableSOP.size() == 1);
//  assert(mrpPlannedSOP.size() == 2);
  assert(rawSOP.size() == 1);
  assert(capacitySOP.size() == 1);
  assert(universalSOD.size() == 2);
  assert(defaultSOD.size() == 2);
  assert(committedSOD.size() == 1);



  // Now test the problem fedInto via DIFFromPWcmrp
  // ===============================================
  LgFrCalendar&     calendarPWcmrp = ippPWcmrp.CMRPscenario()->calendar();
  LgFrSetOfParts& universalSOPPWcmrp =
                 ippPWcmrp.CMRPscenario()->setOfParts(LGFR_UNIVERSAL_SET);
  LgFrSetOfParts& defaultSOPPWcmrp   =
                 ippPWcmrp.CMRPscenario()->setOfParts(LGFR_DEFAULT_SET);
  LgFrSetOfParts& constrainedSOPPWcmrp   =
                 ippPWcmrp.CMRPscenario()->setOfParts(LGFR_CONSTRAINED_SET);
  LgFrSetOfParts& produceableSOPPWcmrp  =
                 ippPWcmrp.CMRPscenario()->setOfParts(LGFR_PRODUCEABLE_SET);
  LgFrSetOfParts& procureableSOPPWcmrp   =
                 ippPWcmrp.CMRPscenario()->setOfParts(LGFR_PROCUREABLE_SET);
  LgFrSetOfParts& budgetSOPPWcmrp   =
                 ippPWcmrp.CMRPscenario()->setOfParts(LGFR_BUDGET_SET);
  LgFrSetOfDemands& universalSODPWcmrp =
                 ippPWcmrp.CMRPscenario()->setOfDemands(LGFR_UNIVERSAL_SET);
  LgFrSetOfDemands& defaultSODPWcmrp   =
                 ippPWcmrp.CMRPscenario()->setOfDemands(LGFR_DEFAULT_SET);

  WitRun * theWitRun = ippPWcmrp.CMRPscenario()->lastProblemSolved().witRun();

  // raw = 9, product = 12, capacity = 4, budget = 1
  assert(universalSOPPWcmrp.size() == 26);
  assert(defaultSOPPWcmrp.size() == 25);
  assert(budgetSOPPWcmrp.size() == 1);
  assert(constrainedSOPPWcmrp.size() == 2);
  assert(produceableSOPPWcmrp.size() == 12);
  assert(procureableSOPPWcmrp .size() == 7);
  // committed = 13, firm = 12
  assert(universalSODPWcmrp.size() == 25);
  assert(defaultSODPWcmrp.size() == 25);


  int nBom;
  witReturnCode rc;

  rc = witGetPartNBomEntries(theWitRun, "System_Board_1", &nBom);
  assert (rc < WitWARNING_RC);
  assert( nBom == 5);


#if defined(_MSC_VER)
  rc = witDisplayData(theWitRun, "./CMRP.display.data");
#else
  rc = witDisplayData(theWitRun, "/tmp/CMRP.display.data");
#endif
  assert (rc < WitWARNING_RC);


  const LgFrSolutionOutputInterface& soi = ippPWcmrp.CMRPscenario()->solutionOutputInterface();

  // implode
  ippPWcmrp.CMRPscenario()->newImplosionSolution();


#if defined(_MSC_VER)
  // write the wit data file
  ippPWcmrp.CMRPscenario()->lastProblemSolved().writeWitDataFile(
                               "./CMRP.wit.data",
                               ippPWcmrp.CMRPscenario()->workingImplosionProblem());
  soi.writeWitProductionSchedule("./CMRP.prod.out");
  soi.writeWitShipmentSchedule("./CMRP.ship.out");
#else
  // write the wit data file
  ippPWcmrp.CMRPscenario()->lastProblemSolved().writeWitDataFile(
                               "/tmp/CMRP.wit.data",
                               ippPWcmrp.CMRPscenario()->workingImplosionProblem());
  soi.writeWitProductionSchedule("/tmp/CMRP.prod.out");
  soi.writeWitShipmentSchedule("/tmp/CMRP.ship.out");
#endif

}




// Test this class
void
LgFrInitialProblemForCMRPwithPrioritiesFromDif::test()
{

}


// Context sensitive test
// dif needs to be constructed using string cmrpData/difTest
// use LgFrDataInterfaceImplementationFromCmrpFiles
void
LgFrInitialProblemForCMRPwithPrioritiesFromDif::contextTest(
          LgFrInitialProblemForCMRPwithPrioritiesFromDif   & ipp,
          LgFrDataInterface				   & dif )
{
  std::cerr << "\nEntering IPPforCMRPwithPrioritiesFromDif::contextTest\n";

  LgFrCalendar&     calendar   = ipp.CMRPscenario()->calendar();

  LgFrSetOfDemands& universalSOD =
                 ipp.CMRPscenario()->setOfDemands(LGFR_UNIVERSAL_SET);
  assert(universalSOD.size() == 2);

  LgFrDemandVector demVec;
  demVec.reserve( universalSOD.size() );
  size_t d = 0; // Pulled out of the for below by RW2STL
  for ( d=0; d<universalSOD.size(); d++ ) {
    demVec.push_back( universalSOD[d] );
  } 

  // get demandPriority's
  LgFrDataInterfaceEssentialsFromScenario difEssen(&(*(ipp.CMRPscenario())));
  LgFrTimeVecFloatVector* demandPriorityPtr
      = (LgFrTimeVecFloatVector*) dif.demandAttribute(
	  demVec, "demandPriority", &difEssen);
  LgFrTimeVecFloatVector & demandPriority = *demandPriorityPtr;
  assert( demandPriority.size() == 2 );
  assert( demandPriority[0] == 5. );
  assert( demandPriority[1] == 4. );

  delete demandPriorityPtr;
}
