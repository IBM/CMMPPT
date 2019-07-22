
// RW2STL - inserted:
#include <string>
#include <ostream>
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL - inserted includes.


// file: hEngMgr.C
// author: robin and tom

#include <assert.h>
#include <mcl/src/mcl.h>
#include <scenario/src/machdep.h>
#include <sce/src/scenSceP.h> 
#include <scenario/src/calendar.h>
// #include <scenario/src/lastPrbS.h>
#include <scenario/src/timeVecs.h>

#include <sce/src/hEngMgr.h>
#include <sce/src/dmApData.h>
#include <sce/src/appData.h>
#include <sce/src/ipSce.h>
#include <sce/src/mPlantH.h>
#include <sce/src/sceParam.h>
#include <sce/src/sceCrtLs.h>
#include <sce/src/itHelpr.h>
#include <sce/src/scenSce.h>

#define HENGMGR_FLT_EPS  0.00001
// #define HENGMGR_DUMP_WIT_DATA_FILE

// turn this on to get two wit data files generated:
//    one for the first pass
//    another for second pass
// #define DUMP_WIT_DATA_FILE

#ifndef ELIMINATE_OLD_MAIN  
// set ipPtr variable pointer

#if 0
void
LgFrSceHeurEngineMgr::setIpPtr(LgFrInitialProblemForSce * myIpPtr)
{
  ipPtr_ = myIpPtr;
}

// Get ipPtr variable pointer
LgFrInitialProblemForSce *
LgFrSceHeurEngineMgr::getIpPtr()
{
  return ipPtr_;
}
#endif

// set engine variable
void
LgFrSceHeurEngineMgr::setEngine(const std::string & engine)
{
  engine_ = engine;
}

// Get engine variable pointer
std::string 
LgFrSceHeurEngineMgr::getEngine()
{
  return engine_;
}
// default constructor
LgFrSceHeurEngineMgr::LgFrSceHeurEngineMgr()
  :   
      sceScenSP_(),
      theWitRun_ (0),
      theCal_(0),
      mpHelper_(0),
      engine_("")
{
    // all the work is done in initializer
}


// fairly general constructor:
LgFrSceHeurEngineMgr::LgFrSceHeurEngineMgr(
                                         LgFrScenarioForSceSmartPointer sceScenSP,
                                         WitRun * const theWitRun, 
                                         LgFrCalendar * theCal,
                                         LgFrMultiPlantHelper * mpHelper,
                                         const std::string & engine)
  :   
       sceScenSP_(sceScenSP),
      theWitRun_ (theWitRun),
      theCal_(theCal),
      mpHelper_(mpHelper),
      engine_(engine)
{
    // all the work is done in initializer
}



// run the vanilla heuristic or the rollover heuristic implosion
void
LgFrSceHeurEngineMgr::implode(LgFrSceCritList & theCritList)
{

  // ounce of prevention is worth a pound of ...
  assert (engine_ == "heuristic" || engine_ == "rollover");


  if ( engine_ == "heuristic" 
       && (sceScenSP_->nStandaloneFeatureArcs() == 0) 
       && (sceScenSP_->nDemandsWithNonZeroMins() == 0)
       )  {
    // if the engine is heuristic and there are no stand alone features and
    // no minimum demands, then do one plain heuristic implosion.
    //    std::cout << "Executing (heuristic) implosion engine ..." << std::endl;

    LgFrSceSetOfParameters & setOfParameters =  sceScenSP_->setOfParameters();    
    std::cout << "Executing (heuristic) implosion engine using (standard) scheme ..." << std::endl;    

    witPreprocess(theWitRun_);
    
    // Print the optimization form of the echo file
    if (setOfParameters.getBool("printOptEchoFile"))  {
      std::string echoFileName = setOfParameters.getString("optEchoFileName");
      sceScenSP_->setWitInformationalMessaging(theWitRun_, SCE_WIT_MESSAGING_ON);
      witDisplayData( theWitRun_, echoFileName.c_str());
      if (! setOfParameters.getBool("printWitMsgFile"))   {
	sceScenSP_->setWitInformationalMessaging(theWitRun_, SCE_WIT_MESSAGING_OFF);          
      }
    }
    // Dump the optimization version of the wit.data file
    if (setOfParameters.getBool("printOptWitDataFile"))  {
      std::string witDataFileName = setOfParameters.getString("optWitDataFileName");
      sceScenSP_->setWitInformationalMessaging(theWitRun_, SCE_WIT_MESSAGING_ON);
      witWriteData(theWitRun_, witDataFileName.c_str());
      if (! setOfParameters.getBool("printWitMsgFile"))   {
	sceScenSP_->setWitInformationalMessaging(theWitRun_, SCE_WIT_MESSAGING_OFF);          
      }
    }
    
    
    witHeurImplode(theWitRun_);
    
    int printCriticalListFile = setOfParameters.getBool("printCriticalListFile");
    if (printCriticalListFile) 
      theCritList.merge( theWitRun_);
    
    bool doPegging = setOfParameters.getBool("doPegging");
    if (doPegging) {
      witPostprocess(theWitRun_);
      witBuildPip(theWitRun_);
      sceScenSP_->loadPIPDataIntoAppData(theWitRun_);
    }

  }
  
  else {
    // you have to do a multiple iterations of implosions for either rollover, min demands,
    // and/or stand alone features. 
    heurImplodeOptFeaturesAndMins( theCritList );
  }
}

// purely-optional-feature fix using multiple implosion iterations of either vanilla heuristic
// or the rollover heuristic implosion engine 
void
LgFrSceHeurEngineMgr::heurImplodeOptFeaturesAndMins(LgFrSceCritList & theCritList)
{

  bool doPegging = sceScenSP_->setOfParameters().getBool("doPegging");

  //  std::cout << "Executing heurImplodeOptFeaturesAndMins with "<< engine_ << std::endl;
  if (engine_ == "heuristic")
    std::cout << "Executing (heuristic) implosion engine using (standard) scheme ..." << std::endl;
  else
    std::cout << "Executing (heuristic) implosion engine using (rollover) scheme ..." << std::endl;    
  
  // ////////////////////////////////////////////////////////////////////////////
  //                                                                          //
  //        Get a whole heck of a lot of stuff and initialize it              //
  //                                                                          //
  // ////////////////////////////////////////////////////////////////////////////
  
  // get an iterative imploder helper to do the dirty work... 
  LgFrSceIterativeImpHelper  iterativeImpHelper;

  // get the number of periods
  int nPeriods;
  witGetNPeriods( theWitRun_, &nPeriods ) ;
  
  // get the part list 
  int nParts ;
  char ** partList ;            
  witGetParts( theWitRun_, &nParts, &partList ) ;

  // get the operation names so don't have to keep calling API routines
  int  nOperations ;      
  char ** operationList ;    
  witGetOperations( theWitRun_, &nOperations, &operationList );



  // re-set part and operation indices, this is done primarily for CUSTOMER_CHOICE_FEATURES
  mpHelper_->setAllWitPartIndices(theWitRun_);
  mpHelper_->setCCOperationIndices(theWitRun_);
  

  // get the demand names so don't have to keep calling API routines
  int * nDemandsList ;      
  char *** allDemandsList ;    
  iterativeImpHelper.getDemandLists( theWitRun_, nParts, partList,
                                     &nDemandsList, &allDemandsList );
  
  // we're going to muck with demand vols.
  // So, make a copy of the original demand vols to clean-up the corrupted witRun at the end
  // remember to free the allDemandsVols up at the end
  float *** allDemandsVols;
  iterativeImpHelper.getDemandVols ( theWitRun_, nPeriods, nParts, partList, nDemandsList,
                                     allDemandsList, &allDemandsVols );
    
  // determine if minimum lot sizes are used global and for each part
  witBoolean isMinLotSizeUsedAnywhere ;
  witBoolean * isMinLotSizeUsedForOperation ;
  float ** minLotSizes;
  iterativeImpHelper.getMinLotSizeFlagsAndVols( theWitRun_, nOperations, nPeriods,
                                                operationList, &isMinLotSizeUsedAnywhere,
                                                &isMinLotSizeUsedForOperation, &minLotSizes );

  // get a copy of the original supply vols so that we can set them back
  // when we're done
  float ** allSupplyVols;
  iterativeImpHelper.getSupplyVols( theWitRun_, nParts, 
                                    partList, &allSupplyVols);

  // get a copy of the production hard upper bounds so we can restore
  // them to their original values after we are through mucking them up
  float ** allExecUpperBounds;
  iterativeImpHelper.getExecUpperBounds( theWitRun_, nOperations, operationList, &allExecUpperBounds );

  
  // See if a critical list is wanted.
  witBoolean computeCriticalList;
  witGetComputeCriticalList ( theWitRun_, &computeCriticalList );  
  
  // get the number of BOM entries for each operation to minimize number of API calls
  // we'll need this info to update the global substitute BOM production schedule
  // (remember to clean up this memory later)
  int * nBom = 0; 
  iterativeImpHelper.getNBom ( theWitRun_, nOperations, operationList, &nBom );
  
  // get the number of substitutes on each BOM entree on each operation to minimize API calling
  // we'll need this info to update the global substitute BOM production schedule
  // (remember to clean up this memory later)
  int ** nSubsBom = 0; 
  iterativeImpHelper.getNSubsBom ( theWitRun_, nOperations, operationList, nBom, &nSubsBom);   
  
  // allocate and initialize global shipment schedule
  // (remember to clean up this memory later)
  float *** globalShipSchedule = 0;
  iterativeImpHelper.initGlobalShipSchedule ( &globalShipSchedule,
                                              nPeriods,
                                              nParts,
                                              nDemandsList );
  
  //
  // allocate and initialize global execution schedule
  // (remember to clean up this memory later)
  float ** globalExecSchedule = 0;
  iterativeImpHelper.initGlobalExecSchedule ( &globalExecSchedule,
                                              nPeriods,
                                              nOperations );

  
  // allocate and initialize global substitute production schedule
  // (remember to clean up this memory later)
  float **** globalSubsBomProdSchedule = 0;
  iterativeImpHelper.initGlobalSubsBomProdSchedule ( &globalSubsBomProdSchedule,
                                                     nPeriods,
                                                     nOperations,
                                                     nBom,
                                                     nSubsBom);

  // ////////////////////////////////////////////////////////////////////////////
  //                                                                          //
  //   We're all set up. Now for the fun...multiple implosion iterations      //
  //   to handle rollover, features, and min demands as needed                //
  //                                                                          //
  // ////////////////////////////////////////////////////////////////////////////

  // ==================================================
  // STEP 1: First Implode to MINIMUM Demands ...
  //          a) if there are no mins, then skip this whole step
  //          b) set demand vols to the mins (stored in witCumShipSoftLowerBound)
  //             AND store the residual demand (aboveMinDemandVol) in the
  //             demandAppData for each demand.
  //          c) Do ONE basic witHeurImplode to satisfy mins.  Note: we
  //             may need to address user's concerns that they want some
  //             control over the allocation of mins!
  //          d) Update the global (output) schedules: ship, prod, subsProd
  //          e) Update minLotSize, supplyVol, and prodVolUB in witRun (prep for next iteration)
  //          f) merge critical list into the global one
  //          g) Load the residual demands vols into the WitRun (prep for next iteration)
  //             (these residual demands are stored in demand App data)
  // ==================================================  
  
  if (sceScenSP_->nDemandsWithNonZeroMins() > 0) {
    // Get the minimum demands and calculate the residual "aboveMinDemands"
    // Do one heuristic implosions on the minimum demands.
    // Set the demand vol equal to the residual "above min" demands.
    setMinDemandsAndCalculateAboveMinDemands( theWitRun_, nPeriods,
                                              nParts, partList,
                                              nDemandsList, allDemandsList,
                                              allDemandsVols );
#ifdef HENGMGR_DUMP_WIT_DATA_FILE
    std::cout << "Writing the (heur mins pass) wit data file ..." << std::endl;                      
    sceScenSP_->setWitInformationalMessaging(theWitRun_, SCE_WIT_MESSAGING_ON);
    witWriteData(theWitRun_, "witDataForMins.data");
    sceScenSP_->setWitInformationalMessaging(theWitRun_, SCE_WIT_MESSAGING_OFF);          

#endif


    // 2.30: allow user to specify which engine to use for mins
    //    Note: the updateGlobalSchedules and prepOperationConstraintsForNextImplosionIteration
    //          only needs to be done for heuristic. The Rollover does it within rolloverHeurImplode
    LgFrSceSetOfParameters & setOfParameters =  sceScenSP_->setOfParameters();    
    std::string minsEngine = setOfParameters.getString("minsEngine");

    if (minsEngine != "rollover")  {
      std::cout << "     ... allocating to mins using (heuristic) scheme  ..." << std::endl;
      witHeurImplode(theWitRun_);

      if (doPegging) {
	witPostprocess(theWitRun_);
	witBuildPip(theWitRun_);
	sceScenSP_->loadPIPDataIntoAppData(theWitRun_);
      }


      if (computeCriticalList) 
	theCritList.merge( theWitRun_);

      iterativeImpHelper.updateGlobalSchedules( theWitRun_, nPeriods, nParts, nOperations,
						partList, operationList,
						nDemandsList, allDemandsList,
						nBom, nSubsBom,
						globalShipSchedule,
						globalExecSchedule,
						globalSubsBomProdSchedule);
      
      iterativeImpHelper.prepOperationConstraintsForNextImplosionIteration ( theWitRun_, nPeriods, nParts,
									     partList, nOperations, operationList,
									     globalExecSchedule,
									     allExecUpperBounds,
									     isMinLotSizeUsedAnywhere, 
									     isMinLotSizeUsedForOperation );
    }

    else {
      std::cout << "     ... allocating to mins using (rollover) scheme  ..." << std::endl;
      rolloverHeurImplode( theWitRun_, theCritList, globalShipSchedule, globalExecSchedule,
			   globalSubsBomProdSchedule, allExecUpperBounds,
			   computeCriticalList, nPeriods, nParts, nOperations, partList,
			   operationList, nBom, nSubsBom, 
			   isMinLotSizeUsedAnywhere,
			   isMinLotSizeUsedForOperation ) ;
    }

    


    // get the "above min demands" out of the demand app data and set the demand
    // vols for the implosion (roll or heur) on the residual demand
    LgFrSceDemandAppData * dmdAppDataPtr;
    int i = 0; // Pulled out of the for below by RW2STL
    for (i=0; i<nParts; i++)  {
      int j = 0; // Pulled out of the for below by RW2STL
      for (j=0; j<nDemandsList[i]; j++)  {
        witGetDemandAppData( theWitRun_, partList[i], allDemandsList[i][j],
                             (void **) &dmdAppDataPtr);
        LgFrTimeVecFloat demandVol( dmdAppDataPtr->aboveMinDemandVol() );
        assert (demandVol.min() >= 0.0 );
        witSetDemandDemandVol( theWitRun_, partList[i], allDemandsList[i][j],
                               demandVol.data() );
      }
    }
  } // end if (min demands exist)

  // ==================================================
  // STEP 2: Implode  residual "above min" demand
  //          a) If there are no mins, then the "residual above min demand"
  //             is simply the original demandVol
  //          b) run either witHeurImplode or rolloverImplode depending on
  //             on user's preference.
  //          c) Update the global (output) schedules: ship, prod, subsProd
  //          d) Update minLotSize, supplyVol, and prodVolUB in witRun (prep for next iteration)
  //          e) merge critical list into the global one
  //          NOTE:  c,d,e don't need to be done when using rollover,
  //                 because they're done by rollover itself.
  // ==================================================  
  
  // if min demands existed, then the demands are set to the residual "above min" demand
  // if min demands did not exist, then the demand is equal to the orginal demand.
  // For which ever demand, implode with your favorite engine and update.
  if (engine_ == "heuristic")  {
#ifdef HENGMGR_DUMP_WIT_DATA_FILE
    std::cout << "Writing the (heur first pass) wit data file ..." << std::endl;                      
    sceScenSP_->setWitInformationalMessaging(theWitRun_, SCE_WIT_MESSAGING_ON);
    witWriteData(theWitRun_, "witDataForFirstPass.data");
    sceScenSP_->setWitInformationalMessaging(theWitRun_, SCE_WIT_MESSAGING_OFF);          

#endif

    witHeurImplode( theWitRun_ );

    if (doPegging) {
      witPostprocess(theWitRun_);
      witBuildPip(theWitRun_);
      sceScenSP_->loadPIPDataIntoAppData(theWitRun_);
    }


    if (computeCriticalList) 
      theCritList.merge( theWitRun_);

    iterativeImpHelper.updateGlobalSchedules( theWitRun_, nPeriods, nParts, nOperations,
                                              partList, operationList,
                                              nDemandsList, allDemandsList,
                                              nBom, nSubsBom,
                                              globalShipSchedule,
                                              globalExecSchedule, 
					      globalSubsBomProdSchedule);

    iterativeImpHelper.prepOperationConstraintsForNextImplosionIteration(theWitRun_, nPeriods, nParts,
									 partList, nOperations, operationList, 
									 globalExecSchedule, 
									 allExecUpperBounds,
									 isMinLotSizeUsedAnywhere, 
									 isMinLotSizeUsedForOperation );
  }
  else  {
    // Do an "iterative" rollover.
    // The global prod, ship, and sub scheds, minLotSize, and supply vols are updated
    // after each of the Nperiod implosion iterations.
    // Ship late limits are returned to their passed in values.
    assert (engine_ == "rollover");
#ifdef HENGMGR_DUMP_WIT_DATA_FILE
    std::cout << "Writing the (heur first pass) wit data file ..." << std::endl;                      
    sceScenSP_->setWitInformationalMessaging(theWitRun_, SCE_WIT_MESSAGING_ON);
    witWriteData(theWitRun_, "witDataForFirstPassRollover.data");
    sceScenSP_->setWitInformationalMessaging(theWitRun_, SCE_WIT_MESSAGING_OFF);          

#endif

    rolloverHeurImplode( theWitRun_, theCritList, globalShipSchedule, globalExecSchedule,
                         globalSubsBomProdSchedule, allExecUpperBounds,
                         computeCriticalList, nPeriods, nParts, nOperations, partList,
                         operationList, nBom, nSubsBom, 
			 isMinLotSizeUsedAnywhere,
                         isMinLotSizeUsedForOperation ) ;
  }





  // ==================================================
  // STEP 3: Feature Cleanup implosion ...
  // ==================================================
  //    This iteration needs some explanation
  //       Heuristic is not smart enough to allocate to features
  //


   
  // ==================================================  
  
  if (sceScenSP_->nStandaloneFeatureArcs() > 0) {
    // if there are have features, we need to do another round of implosion
    // determine how much of the feature can be built.

    // set the demand on the feature nodes, and zero out the other demand
    setPureOptionBuildDemand( theWitRun_, nPeriods, nParts, partList, 
			      nOperations, operationList, nDemandsList,
                              allDemandsList, allSupplyVols, nBom, nSubsBom, mpHelper_, iterativeImpHelper,
                              globalSubsBomProdSchedule, globalShipSchedule,
			      allDemandsVols, globalExecSchedule);
    

    
    if (engine_ == "heuristic")  {
#ifdef HENGMGR_DUMP_WIT_DATA_FILE
      std::cout << "Writing the (heur second pass) wit data file ..." << std::endl;                      
      sceScenSP_->setWitInformationalMessaging(theWitRun_, SCE_WIT_MESSAGING_ON);
      witWriteData(theWitRun_, "witDataForFeaturesPass.data");
      sceScenSP_->setWitInformationalMessaging(theWitRun_, SCE_WIT_MESSAGING_OFF);          
#endif

      witHeurImplode( theWitRun_ );

      if (doPegging) {
	witPostprocess(theWitRun_);
	witBuildPip(theWitRun_);
	sceScenSP_->loadPIPDataIntoAppData(theWitRun_);
      }


      if (computeCriticalList) 
        theCritList.merge( theWitRun_);
      
      iterativeImpHelper.updateGlobalSchedules( theWitRun_, nPeriods, nParts, nOperations,
                                                partList, operationList, 
                                                nDemandsList, allDemandsList,
                                                nBom, nSubsBom,
                                                globalShipSchedule,
                                                globalExecSchedule,                                  
                                                globalSubsBomProdSchedule);
      
      iterativeImpHelper.prepOperationConstraintsForNextImplosionIteration(theWitRun_, nPeriods, nParts,
									   partList, nOperations, operationList,
									   globalExecSchedule, 
									   allExecUpperBounds,
									   isMinLotSizeUsedAnywhere, 
									   isMinLotSizeUsedForOperation );

    }
    else  {
      // Do an "iterative" rollover.
      // The global prod, ship, and sub scheds, minLotSize, and supply vols are updated
      // after each of the Nperiod implosion iterations.
      // Ship late limits are returned to their passed in values.
      assert (engine_ == "rollover");
      rolloverHeurImplode( theWitRun_, theCritList, globalShipSchedule, globalExecSchedule,
                           globalSubsBomProdSchedule, allExecUpperBounds,
                           computeCriticalList, nPeriods, nParts, nOperations, partList,
                           operationList, nBom, nSubsBom, isMinLotSizeUsedAnywhere,
                           isMinLotSizeUsedForOperation ) ;
    }
  } // end if (stand-alone features exist)






  // put the global solution schedules in theWitRun
  iterativeImpHelper.stuffGlobalSolutionSchedules( theWitRun_, nPeriods, nParts, nOperations,
                                                   nDemandsList, allDemandsList,
                                                   partList, operationList, nBom, nSubsBom,
                                                   globalShipSchedule,
                                                   globalExecSchedule,                                
                                                   globalSubsBomProdSchedule );
  
  // Uncorrupt the WitRun: set the minLotSizes back to original values
  if (isMinLotSizeUsedAnywhere) 
    iterativeImpHelper.setMinLotSize( theWitRun_, nPeriods, nOperations, operationList,
                                      isMinLotSizeUsedForOperation, minLotSizes );

  // Uncorrupt the WitRun: set the supplyVols back to original values
  iterativeImpHelper.setSupplyVols( theWitRun_, nParts, partList, allSupplyVols );

  // Uncorrupt the WitRun: set the production hard upper bounds back to their original values
  iterativeImpHelper.setExecUpperBounds( theWitRun_, nOperations, operationList, allExecUpperBounds );
  // Uncorrupt the WitRun: set the demands back to their original values (but leave
  // the feature demands as they are now.  (this is because their original values
  // are zero.
  this->setAllDemandVolsExceptFeatureDemands(nParts, partList,
                                             nDemandsList, allDemandsList,
                                             allDemandsVols);
  //post-process
  witPostprocess (theWitRun_ );    

  //
  // Clean-Up: free up all the memory we malloc-ed and no longer need
  //
  // clean-up global ship and prod schedule
  int i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<nParts; i++ ) {

    int j = 0; // Pulled out of the for below by RW2STL
    for ( j=0; j<nDemandsList[i]; j++) 
      portableFree(globalShipSchedule[i][j] );
    portableFree(globalShipSchedule[i]);
  }
  portableFree(globalShipSchedule);
  
  int o = 0; // Pulled out of the for below by RW2STL
  for ( o=0; o<nOperations; o++){

    portableFree(allExecUpperBounds[o]);
    if (isMinLotSizeUsedForOperation[o])
      portableFree(minLotSizes[o]);

    int j = 0; // Pulled out of the for below by RW2STL
    for (j=0; j<nBom[o]; j++){
      int k = 0; // Pulled out of the for below by RW2STL
      for (k=0; k<nSubsBom[o][j]; k++) 
        portableFree ( globalSubsBomProdSchedule[o][j][k] );
      portableFree ( globalSubsBomProdSchedule[o][j] );
    }
    portableFree ( globalSubsBomProdSchedule[o] );
    portableFree (nSubsBom[o]);
  }
  portableFree ( globalSubsBomProdSchedule );

  
  for (i=0; i<nParts; i++) {
    portableFree(allSupplyVols[i]);
  }
  portableFree(allSupplyVols);
  portableFree(allExecUpperBounds);
  portableFree(minLotSizes);
  portableFree (nSubsBom);
  portableFree (nBom);
  portableFree(isMinLotSizeUsedForOperation);


  // clean-up global exec schedule
  for (o=0; o<nOperations; o++ )
    portableFree( globalExecSchedule[o] );
  portableFree(globalExecSchedule);  

  
  // clean-up allDemandsList
  for ( i=0; i<nParts; i++ ) {
    int j = 0; // Pulled out of the for below by RW2STL
    for ( j=0; j<nDemandsList[i]; j++ )  {
      portableFree( allDemandsList[i][j] );
      portableFree( allDemandsVols[i][j] );
    }
    portableFree ( allDemandsList[i] );
    portableFree ( allDemandsVols[i] );
  }
  portableFree( allDemandsList );
  portableFree( allDemandsVols );
  portableFree( nDemandsList );
  
  //clean-up partList
  for ( i=0; i<nParts; i++ )
    portableFree( partList[i] );
  portableFree(partList);

  //clean-up operationList
  for ( o=0; o<nOperations; o++ )
    portableFree( operationList[o] );
  portableFree(operationList);  
  
}
 
// This is member function mucks up the demands - if you want to get back your
// original demands, you best have saved them somewhere safe before you invoke
// this member function.
//
// Calculate amount of pure option we want to try and build
// (code snipped from LgFrInitialProblemForSce::printStandAloneFeatureAnalysisReport).
// Set the demand on the pure options nodes in the WitRun and zero-out all other
// demands.
//
// increment the global number of demands, the global demand list, and the global shipment
//
void 
LgFrSceHeurEngineMgr::setPureOptionBuildDemand(
         WitRun * const theWitRun,   // WIT environment   
         const int nPeriods,         // Number of periods 
         const int nParts,           // Number of parts   
         char ** partList,           // List of part names
         const int nOperations,
         char ** operationList,
         int * nDemandsList,         // number of demands on each part
         char *** allDemandsList,    // list of demands names
	 float ** allSupplyVols,
         int * nBom,                 // Number of BOM entries for each part
         int ** nSubsBom,            // Number of substitutes on each part's BOM entries 
	 LgFrMultiPlantHelper * mpHelper,
         LgFrSceIterativeImpHelper  & iterativeImpHelper,
         float **** glbSubsProdVol,  // substitute bom production vol
         float *** glbShipSched,
	 float *** allDemandsVols,
	 float ** globalExecSchedule)
{
  std::string plannerTopLevelPart;
  std::string geo;

  std::cout << "Executing  setPureOptionBuildDemand..." << std::endl;  


  LgFrSceSetOfParameters & setOfParameters =  sceScenSP_->setOfParameters();    
  std::string featureRatioPegMethod = setOfParameters.getString("featureRatioPegMethod");
  //  std::string pegToMachineDemandPeriod;
  //  std::string pegToMachineShipPeriod;

  LgFrTimeVecFloat zeroTimeVec( (size_t) nPeriods, 0.0 );
  
  // Zero-out all the demand vols.
  // As we loop through, the demands are going to be incremented so it's important to
  // start with all demands set equal to zero
  iterativeImpHelper.zeroOutAllDemands (theWitRun, nPeriods, nParts,
                                        partList, nDemandsList,
                                        allDemandsList);

  // Loop once for each operation.
  int o = 0; // Pulled out of the for below by RW2STL
  for ( o=0; o<nOperations; o++ ) {
    std::string fullWitGeoPlannerDemandOperation(operationList[o]);
    // The only place we find specialFeatureParts is as children of GPD parts.
    if (! mpHelper->isOperationSpecialGeoPlannerDemand(theWitRun, fullWitGeoPlannerDemandOperation,
						     plannerTopLevelPart, geo))  {
      continue;
    }

    // Now loop through the children of the GPD operation
    int j = 0; // Pulled out of the for below by RW2STL
    for (j=0; j<nBom[o]; j++)  {

      // get the name of the child part, 
      char * fullWitFeaturePart;
      witGetBomEntryConsumedPart(theWitRun, fullWitGeoPlannerDemandOperation.c_str(), j, &fullWitFeaturePart);

      // We are only interested in special Feature Parts which, at this time,
      // can either be custChoice or stdalone
      if (!  mpHelper->isPartSpecialFeature(theWitRun, fullWitFeaturePart)) {
	portableFree(fullWitFeaturePart);
	continue;
      }

      std::string gg, pp, ppdf, fp;

      // + --------------------------------
      // + If its a CUSTOMER_CHOICE_FEATURES
      // + --------------------------------
      if (mpHelper->isPartSpecialCustChoiceFeature(theWitRun, fullWitFeaturePart, 
						   gg, pp, ppdf, fp)) {
	    
	std::string featureDemandName = 
	  mpHelper->custChoiceFeatureDemandName(fullWitGeoPlannerDemandOperation );
	    
	    
	int gpdIndex = mpHelper->witPartIndex(theWitRun, fullWitGeoPlannerDemandOperation.c_str());
	float * gpdDemVol =  allDemandsVols[gpdIndex][0];
	LgFrTimeVecFloat gpdExecVol ((size_t) nPeriods, globalExecSchedule[o]);
	float usageRate;
	witGetBomEntryUsageRate(theWitRun, fullWitGeoPlannerDemandOperation.c_str(), j, &usageRate);	      
	// featRatio is stored in GPD->SFP bom's appData
	LgFrTimeVecFloat featRatio = mpHelper->custChoiceFeatureRatio(theWitRun, fullWitFeaturePart);


	// total features needed
	LgFrTimeVecFloat featureDemandPreUsageRate(featRatio);

	// This *may* be obsolete.  It's an attempt to adjust the execVols back to the 
	// the period when the demand was due. 
	LgFrTimeVecFloat tiedBackGpdExecVol(gpdExecVol);

	// if the ratio's are constant over time, then life is easy
	int areRatiosConstant = 0;
	if (featRatio.max() - featRatio.min() < 0.0001) {
	  areRatiosConstant = 1;

	  // featureDemandPreUsageRate is GPD ships times the featRatio	  
	  int t = 0; // Pulled out of the for below by RW2STL
	  for ( t=0; t<nPeriods; t++)
	    featureDemandPreUsageRate[t] = featRatio[t] * tiedBackGpdExecVol[t];
	}

	// if the ratio's change over time, then life is a pain in the ass
	// here are two ways to implement, bith of which are problematic
	// For now we have an emergency fix that forces ESG user's to make
	// all ratio's equal.   Thus this case of "ratio's changing" won't bite us
	// However, there are other issues to consider ... 
	//    MachFeat Supply, 
	//    MachFeat Offsets
	//    GeoSpource transit times
	//    anything else ????
	// If's a major FINISH_ME: to clear this up!!!   We may need to retract some 
	// sce functionality
	else {




#if 1
	  // ----------------------------------------------------------	  
	  // ---- Method 1 ----
	  // This method was flawed and was to be be replaced by method 2,
	  // but that was screwing up also

	  // Compute tiedBackGpdExecVol ...
	  int t = 0; // Pulled out of the for below by RW2STL
	  for (t=0; t<nPeriods; t++) {
	    float demExcess = gpdDemVol[t] - tiedBackGpdExecVol[t];
	    // if this period matches, then continue
	    if (demExcess < .001)
	      continue;
	    // otherwise, look for future commits to pull in
	    int t2 = 0; // Pulled out of the for below by RW2STL
	    for (t2=t+1; t2<nPeriods; t2++) {
	      // if period t2 execVol covers period t delta, then pull it in
	      if (tiedBackGpdExecVol[t2]  >= demExcess) {
		tiedBackGpdExecVol[t] += demExcess;
		tiedBackGpdExecVol[t2] -= demExcess;
		demExcess = 0.0;
		break;
	      }
	      else {
		tiedBackGpdExecVol[t] += tiedBackGpdExecVol[t2];
		demExcess -= tiedBackGpdExecVol[t2];
		tiedBackGpdExecVol[t2] = 0.0;
	      }
	    }
	    // now if you didn't clear the demand gap, then you're done
	    // since you've (necesarily) exhausted all future commits
	    // So break out of the loop
	    if (demExcess > 0.001) {
	      break;
	    }
	  }
	  // End: Compute ==> tiedBackGpdExecVol 	    
	  // now compute the total Features Needed ...
	  // featureDemandPreUsageRate is basically the GPD shipVol times the featRatio
	    
	  for ( t=0; t<nPeriods; t++)
	    featureDemandPreUsageRate[t] *= tiedBackGpdExecVol[t];
	  //		featureDemandPreUsageRate[t] *= gpdDemVol[t];
	    
#endif 	    
	
#if 0
	  // ----------------------------------------------------------	  
	  // ---- Method 2 ----
	  // This method was supposed to be an improvement over Method 1
	  // but it has problems
	  
	  int t = 0; // Pulled out of the for below by RW2STL
	  for (t=0; t<nPeriods; t++) {
	    float demandFromPeriodT2satisfiedinPeriodT = 0.0;	  
	    // lets loop over the previous periods (t2 form 0 to t-1) and pick up any 
	    // un-"allocated" gpdDemVol.
	    int t2 = 0; // Pulled out of the for below by RW2STL
	    for (t2=0; t2<t; t2++) {
	      demandFromPeriodT2satisfiedinPeriodT =
		(gpdDemVol[t2] < tiedBackGpdExecVol[t])  ?  gpdDemVol[t2] : 0.0;
	      // use the ratio in place in period t2 figure out how much features are needed in t
	      featureDemandPreUsageRate[t] += demandFromPeriodT2satisfiedinPeriodT * featRatio[t2];
	      // decrecment both gpdDemVol and tiedBackGpdExecVol
	      gpdDemVol[t2] -= demandFromPeriodT2satisfiedinPeriodT;
	      tiedBackGpdExecVol[t] -= demandFromPeriodT2satisfiedinPeriodT;
	    }
	    
	    //  after the loop t2=t and we simply take the min{gpdDemVol[t] and tiedBackGpdExecVol[t]
	    demandFromPeriodT2satisfiedinPeriodT =
	      (gpdDemVol[t2] < tiedBackGpdExecVol[t])  ?  gpdDemVol[t2] : tiedBackGpdExecVol[t];
	    featureDemandPreUsageRate[t] += demandFromPeriodT2satisfiedinPeriodT * featRatio[t2];
	    // decrement both gpdDemVol and tiedBackGpdExecVol
	    gpdDemVol[t2] -= demandFromPeriodT2satisfiedinPeriodT;
	    tiedBackGpdExecVol[t] -= demandFromPeriodT2satisfiedinPeriodT;
	    
	    // now it most cases, tiedBackGpdExecVol[t] should be Zero
	    // if it's not, then roll it
	    if (tiedBackGpdExecVol[t] >0.0001) {
	      if (t == (nPeriods - 1)) {
		std::cout << "Big Mess of an error ... look here!!!" << std::endl;
		exit(8);
	      }
	      
	      // roll it
	      tiedBackGpdExecVol[t+1] += tiedBackGpdExecVol[t];
	      tiedBackGpdExecVol[t] = 0.0;
	    }
	  }
#endif
	  // ----------------------------------------------------------	  
	}            // matches the else  regarding featratio's changing over time
	  
	
	// here's the amount of features supplied in first pass (pre usageRate)
	//     featuresShippedPreUsageRate = supplyVolCCfeaturePart + execVolCCfeaturePart;
	LgFrTimeVecFloat featuresShippedPreUsageRate((size_t) nPeriods, 0.0);
	int ccSfpIndex = mpHelper->witPartIndex(theWitRun, fullWitFeaturePart);
	int ccSfoIndex = mpHelper->witOperationIndex(theWitRun, fullWitFeaturePart);
	LgFrTimeVecFloat supplyVolCCfeaturePart( (size_t) nPeriods, allSupplyVols[ccSfpIndex]);
	LgFrTimeVecFloat execVolCCfeaturePart((size_t) nPeriods, globalExecSchedule[ccSfoIndex]);
	int t = 0; // Pulled out of the for below by RW2STL
	for ( t=0; t<nPeriods; t++)
	  featuresShippedPreUsageRate[t] = supplyVolCCfeaturePart[t] + execVolCCfeaturePart[t];
	    
	    
	    // here's the straight net of what you need for the second pass (pre usageRate)
	LgFrTimeVecFloat secondPassFeatureDemandVol(featureDemandPreUsageRate);
	for ( t=0; t<nPeriods; t++)
	  secondPassFeatureDemandVol[t] -= featuresShippedPreUsageRate[t];

	    
#if 0
	// THIS IS THE OLD OLD WAY ... should get rid of it eventually ...

	// here's the amount of features supplied in first pass (pre usageRate)
	LgFrTimeVecFloat featuresShippedPreUsageRate((size_t) nPeriods, 0.0);
	for ( t=0; t<nPeriods; t++)
	  featuresShippedPreUsageRate[t] = gpdExecVol[t] - subVol[t];
	    
	    
	    // here's the straight net of what you need for the second pass (pre usageRate)
	LgFrTimeVecFloat secondPassFeatureDemandVol(featureDemandPreUsageRate);
	for ( t=0; t<nPeriods; t++)
	  secondPassFeatureDemandVol[t] -= featuresShippedPreUsageRate[t];
#endif
	    

#if 0      
	// ===> 11/30/2001  	 trying this over ... this doesn't work
	// Roll negative (secondPassFeatureDemandVol) forward.  This needs to  happen because:
	//   - Machine Feature Supply (ie, FEN5) 
	//   - wierd offsetting issues (??)
	for ( t=0; t<nPeriods-1; t++)
	  if (secondPassFeatureDemandVol[t] < 0) {
	    secondPassFeatureDemandVol[t+1] += secondPassFeatureDemandVol[t];
	    secondPassFeatureDemandVol[t] = 0;
	  }
	if (secondPassFeatureDemandVol[nPeriods - 1] < 0) 
	  secondPassFeatureDemandVol[nPeriods - 1] = 0;
	    
	// now insure that the cum of secondPassFeatureDemandVol does not exceed the 
	// cum of machines shipped!
	for ( t=0; t<nPeriods; t++) {
	  // add in what you rolled from last period
	  float delta = secondPassFeatureDemandVol[t] - gpdExecVol[t];
	  // do you need to roll?
	  if (delta > 0) {
	    secondPassFeatureDemandVol[t] = gpdExecVol[t];
	    if (t+1 < nPeriods)
	      secondPassFeatureDemandVol[t+1] += delta;
	  }
	  // or not roll?
	  else {
	    if (t+1 < nPeriods)
	      gpdExecVol[t+1] -= delta;
	  }
	}  //  end ... Roll negative forward
#else

	// ---------------------------------------------
	// Roll Negatives Forward:  approach  11/30/2001
	// ---------------------------------------------
	//       (now we have the luxury that all ratio's are constant)
	// Walk backwards through the timeVec  from pd n-1 to 0
	// if you find a negative carry it along and work it off
	for ( t=nPeriods-1; t>0; t--) {
	  if (secondPassFeatureDemandVol[t] < 0) {
	    secondPassFeatureDemandVol[t-1] += secondPassFeatureDemandVol[t];
	    secondPassFeatureDemandVol[t] = 0;
	  }
	}
	// ok, so what happens if you can't work it all off? ... punt!
	// 
	if (secondPassFeatureDemandVol[0] < 0) {
	  if (secondPassFeatureDemandVol[0] < -0.01) {
	    std::cout << "Internal SCE Code Alert: SCE00645AH.  Please notify the SCE Development Team.\n";
	  }
	  secondPassFeatureDemandVol[0] = 0;
	}
	// ---------------------------------------------
	// END: Roll Negatives Forward:  approach  11/30/2001
	// ---------------------------------------------
	

	// now insure that the cum of secondPassFeatureDemandVol does not exceed the 
	// cum of machines shipped!
	for ( t=0; t<nPeriods; t++) {
	  // add in what you rolled from last period
	  float delta = secondPassFeatureDemandVol[t] - gpdExecVol[t];
	  // do you need to roll?
	  if (delta > 0) {
	    secondPassFeatureDemandVol[t] = gpdExecVol[t];
	    if (t+1 < nPeriods)
	      secondPassFeatureDemandVol[t+1] += delta;
	  }
	  // or not roll?
	  else {
	    if (t+1 < nPeriods)
	      gpdExecVol[t+1] -= delta;
	  }
	}  //  end ... Roll negative forward
#endif

	// now we apply the usageRate
	for ( t=0; t<nPeriods-1; t++)
	  secondPassFeatureDemandVol[t] *= usageRate;
	    
	    
	    // YEAH, we've computed the second pass demand vol!!
	    // set it
	witSetDemandDemandVol(theWitRun, fullWitFeaturePart, featureDemandName.c_str(),
			      secondPassFeatureDemandVol.data());

	portableFree(fullWitFeaturePart);
	continue;
      }  //  end  ...        if (mpHelper->isPartSpecialCustChoiceFeature())  ...



      // + --------------------------------
      // + If its a STANDALONE FEATURE
      // + --------------------------------
      // Just for the sake of it ....
      assert (mpHelper->isPartSpecialStandaloneFeature(theWitRun, fullWitFeaturePart, 
						       gg, pp, ppdf, fp));

      // TRE: Just for assertion, sake ...
      assert(nSubsBom[o][j] == 1);


      // if there are substitutes, then the child might be a pure option
      // note that one must look at the NullSub part to determine which of the three
      // type of STANDALONE FEATUREs it is ...
      if (nSubsBom[o][j] > 0)   {
	// look at each of the substitues.
	// As of 5/31/96, the model should only allow ONE substitute, though.
	int sj = 0; // Pulled out of the for below by RW2STL
	for (sj=0; sj<nSubsBom[o][j]; sj++)  {
	  char * sub;
	  witGetSubsBomEntryConsumedPart(theWitRun, fullWitGeoPlannerDemandOperation.c_str(), j, sj, &sub);
	  // the substitute should be a  special null subs

	  // the null sub will be one of:
	  //    globalNullSub (Purely optional standalone feature)
	  //    globalMaxWithoutNullsub (purely mandatory standalone feature)
	  //    maxWithoutNullsub  (standalone feature with 0<maxWithout <1)
	  
	  // ---------------------------------------------------
	  // if its a GLOBAL MAXWITHOUT (ie, Mandatory Feature)
	  // ---------------------------------------------------
	  //  then we can skip it because there  never be any pureOption demandVol
	  std::string pppdf;
	  if (mpHelper->isPartSpecialGlobalMaxWithoutNullSubstitute(theWitRun, sub, pppdf)) {
	    portableFree(sub);
	    continue;
	  }
	  
	  
	  // -----------------------------------------------------------------------
	  // if its a PURE Option or a MWO (ie, purely optional or partly mandatory)
	  // -----------------------------------------------------------------------
	  // --------------------------------------------------------------------------------

	  // OK, if you've gotten to here then you've found a PURE OPTION!
	  // The child is the pure option.
	      
	  // First time you find the pure option add the build demand
	  // The default demand volume of a time vec of all 0.0
	  // There could be multiple arcs between the child and parent AND there could
	  // be multiple substitutes on each arc. We want at most one pureOptionBuildDemandName
	  // Zero out all the demands in theWitRun
	  // for every (special geo planner demand part)-(pure option) pair.
	  //
	  // Multiple BOM entries between the parent and the child could be used to implement
	  // different feature rates over time.
	  std::string featureDemandName = mpHelper->pureOptionBuildDemandName(fullWitGeoPlannerDemandOperation );
	      
	  // Find out what the current demand vol is on the pure option
	  // and calculate how much you want to increment the demand by.
	  float * pureOptBldDemVol;
	  witGetDemandDemandVol( theWitRun, fullWitFeaturePart, featureDemandName.c_str(), &pureOptBldDemVol);
	      
	  // --------------------------------------------------------------------------------
	  //
	  // This is the place where the rubber hits the road in feature modeling.
	  // This is where the demand for features is calculated.
	  // Implement you favorite algorithm for calculating the amount of a feature
	  // to be built in the second pass of the heuristic HERE.
	  //
	      
	  //
	  // This code says that amount of the null substituted used times the feature ratio
	  // (ie. subsProdVol * usageRate) is the amt of pure option to try and build
	  //
	  // The feature ratio used is the feature ratio in effect during the box
	  // ship period (which may have no similarity with the feature rate
	  // that was in effect in the period during which the demand originated from).
	  
	  // *****************************************************************************
	  // The amount of the produced part due to this substitute is given by
	  // subProdVol.
	  LgFrTimeVecFloat subProdVol ((size_t) nPeriods, glbSubsProdVol[o][j][sj]);
	  
	  if ( featureRatioPegMethod == "pegToMachineShipPeriod" ) {
	    
	    // But, we need the usageRatio...
	    float usageRatio;
	    witGetBomEntryUsageRate(theWitRun, fullWitGeoPlannerDemandOperation.c_str(), j, &usageRatio);
		
	    //	      int * impactPeriod;
	    //	      witGetBomEntryImpactPeriod(theWitRun, fullWitGeoPlannerDemandOperation.c_str(), j, &impactPeriod);
	    // FINISH_ME: this is not right yet
		
	    // ...to calculate the amount of the substitute that was consumed
	    // which is equal to the amount of the pure option we want to try and build.
		
	    int t = 0; // Pulled out of the for below by RW2STL
	    for (t=0; t<nPeriods; t++) {
	      if (subProdVol[t] > 0.0001) {
		//		  assert(impactPeriod[t] >= 0);
		//		  pureOptBldDemVol[impactPeriod[t]] += (usageRatio * subProdVol[t]);
		pureOptBldDemVol[t] += (usageRatio * subProdVol[t]);
	      }
	      //	      portableFree(impactPeriod);
	    }
	  }
	      
	      
	  // Set the new demandVol in theWitRun
	  // Note creating a timeVec for error checking purpose only
	  LgFrTimeVecFloat pureOptBldDemVolTV ( (size_t) nPeriods, pureOptBldDemVol);
	  assert (pureOptBldDemVolTV.min() >= 0.0);
	  witSetDemandDemandVol ( theWitRun, fullWitFeaturePart, featureDemandName.c_str(),
				  pureOptBldDemVol);
	  portableFree( pureOptBldDemVol );
	  portableFree(sub);
	}  // loop over subs 

      }
      portableFree(fullWitFeaturePart);
    }
  }
    
  
}

// Set demand vols
// Use this member function to clean-up the mucked demand vols
void
LgFrSceHeurEngineMgr::setAllDemandVolsExceptFeatureDemands ( int nParts,
                                                             char ** partList,
                                                             int *  nDemandsList,
                                                             char *** allDemandsList,
                                                             float *** allDemandsVols )
{
  std::string plannerTopLevelPart;
  std::string geo;
  int i = 0; // Pulled out of the for below by RW2STL
  for (i=0; i<nParts; i++) {
    int j = 0; // Pulled out of the for below by RW2STL
    for (j=0; j<nDemandsList[i]; j++) {
      if (! this->mpHelper_->isDemandSpecialPureOptionBuildDemand( this->theWitRun_,
                                                                   partList[i],
                                                                   allDemandsList[i][j],
                                                                   plannerTopLevelPart,
                                                                   geo)) {
        witSetDemandDemandVol (this->theWitRun_, partList[i], allDemandsList[i][j],
                               allDemandsVols[i][j]) ;
      }
    }
  }
}  

// Part of an alternative method to calculate the (second pass) demand vols for features.
// This method pegs the shipVolWithoutFeature to the liveDemandVol, and using the
// featureRatio in the live demand period, calculates the demand vol of the feature in the second pass.
 
void
LgFrSceHeurEngineMgr::calculateOneFeaturesBuildDemandVol ( int nPeriods,
                                                           float * liveDemandVol,
                                                           float * shipVolWithoutFeature,
                                                           float * featureRatio,
                                                           float * featureBuildDemandVols )
{
  // initialize the featureBuildDemandVols to zero
  int t = 0; // Pulled out of the for below by RW2STL
  for (t=0; t<nPeriods; t++)
    featureBuildDemandVols[t] = 0.0;

 // need two counters.
 // td is the period for the "d"emand and ts is the period for the "ship"
 int td = 0;
 
 int ts = 0; // Pulled out of the for below by RW2STL
 for ( ts=0; ts<nPeriods; ts++) {
   // if nothing to allocate then go on to the next shipVol bucket
   if ( shipVolWithoutFeature[ts] < HENGMGR_FLT_EPS )
     continue;

   while ( shipVolWithoutFeature[ts] > liveDemandVol[td] ) {
     featureBuildDemandVols[ts] += liveDemandVol[td] * featureRatio[td];
     shipVolWithoutFeature[ts] -= liveDemandVol[td];
     td++;
   }

   featureBuildDemandVols[ts] += shipVolWithoutFeature[ts] * featureRatio[td];
   liveDemandVol[td] -= shipVolWithoutFeature[ts];
 }

}

// Dan's algorithm for calculating the amount of demand lost due to backlogYield.
// original source code from:  /u/dpc/implode/sce/src/test2.c
// Given the demandVol, the shipVol, and the backlogYield vector, this method calculates
// the amount of demandVol that was "lost" due to applying the backlogYield (where shipLateLimit = 0).
//
// Example: demand= ( 2, 4, 6 ), ship=( 1, 2, 3 ) and backlogYld=( 0.6, 0.6, 0.6 )
// then the lostDemandVol=( 0.400, 1.040, 1.824)

void
LgFrSceHeurEngineMgr::calculateLostDemandUnderBacklogYield ( int nPeriods,
                                                             float * demandVol,
                                                             float * shipVol,
                                                             float * backlogYield,
                                                             // output vectors
                                                             float * lostDemandVol,
                                                             float * backlog)
{
  float cumDemand, cumShip, cumLost;
  // initialize local variables  
  cumDemand = 0.0;
  cumShip = 0.0;
  cumLost = 0.0;

  // march through the periods, and iteratively calculate the amount
  // of demand lost in each period and the backlog

  // assumes your float * are nPeriods long
  int t = 0; // Pulled out of the for below by RW2STL
  for(t=0; t<nPeriods; t++) {
    cumDemand += demandVol[t];
    cumShip += shipVol[t];
    backlog[t] = cumDemand - cumShip - cumLost;
    lostDemandVol[t] = (1.0 - backlogYield[t]) * backlog[t];
    cumLost += lostDemandVol[t];
  }

}

// Dan's algorithm for calculating the amount of demand lost due to shipLateLimit.
// original source code from: /u/dpc/implode/sce/src/test3.c
void
LgFrSceHeurEngineMgr::calculateLostDemandUnderShipLateLimit ( int nPeriods,
                                                              int shipLateLimit,
                                                              float * demandVol,
                                                              float * shipVol,
                                                              // output vectors
                                                              float * lostDemandVol,
                                                              float * backlog )


{
  // Here, "cum" means cummulative over periods 0 to t, except for cumShipThruSLL.
  // cumShipThruSLL = shipVol[0] +...+ shipVol[t+ShipLateLimit]
  float cumDemand, cumShipThruSLL, cumLost, cumShip;

  float * loss = (float *) malloc (nPeriods * sizeof (float));
  assert (loss!=0);
  int t = 0; // Pulled out of the for below by RW2STL
  for (t=0; t<nPeriods; t++)
    loss[t] = 0.0;

  float * cumLostVec = (float *) malloc (nPeriods * sizeof (float));
  assert (cumLostVec!=0);
  for ( t=0; t<nPeriods; t++)
    cumLostVec[t] = 0.0;
  
  cumDemand = 0.0;
  cumShipThruSLL = 0.0;
  cumShip = 0.0;
  cumLost = 0.0;
  for( t=0; t<shipLateLimit; t++) {
    cumShipThruSLL += shipVol[t];
  }

  // March through the periods and iteratively calculate the amount of
  // demandVol lost due to hitting the shipLateLimit.
  // We operate under the same assumption as the heuristic-fullfill
  // oldest demand first.
  
  int shipLateLimitHorizon = nPeriods-shipLateLimit;
  for ( t=0; t<nPeriods; t++ )   {
    cumDemand += demandVol[t];
    
    // In periods 0 thru (nPeriods-1-shipLateLimit), demand is either satisfied
    // or it is lost (due to the shipLateLimit).
    if ( t < shipLateLimitHorizon )   {
      cumShipThruSLL += shipVol[ t+shipLateLimit ];
      loss[t] = cumDemand - cumShipThruSLL - cumLost;
    }

    // In periods (nPeriods-shipLateLimit) thru (nPeriods-1) demand is either
    // satisfied or it is "unmet". The demand in these late periods never
    // hit the shipLateLimit.
    else {
      loss[t] = 0.0;
    }
    
    if (loss[t] < 0)
      loss[t] = 0.0;
    
    cumLost += loss[t];
  }
  
  // Dan shifts the lostDemandVol vector over to report the
  // the amount lost in the period where we defined it to be lost

  for ( t=0; t<shipLateLimit; t++ )
    lostDemandVol[t] = 0.0;
  for ( t=shipLateLimit; t<nPeriods; t++)
    lostDemandVol[t] = loss[t-shipLateLimit];

  cumLostVec[0]=lostDemandVol[0];
  for (t=1; t<nPeriods; t++)
    cumLostVec[t]=cumLostVec[t-1]+lostDemandVol[t];
  
  portableFree (loss);

  // backlog calculation
  cumShip = shipVol[0];
  cumDemand = demandVol[0];
  backlog[0]=cumDemand-cumShip;
  for (t=1; t<nPeriods; t++) {
    cumShip += shipVol[t];
    cumDemand += demandVol[t];
    backlog[t] = cumDemand - cumShip - cumLostVec[t-1];
  }

  portableFree (cumLostVec);
  
}
  

// copy constructor 
LgFrSceHeurEngineMgr::LgFrSceHeurEngineMgr(const LgFrSceHeurEngineMgr& source)
  :   
      sceScenSP_ (source.sceScenSP_),
      theWitRun_ (source.theWitRun_),
      theCal_(source.theCal_),
      mpHelper_(source.mpHelper_),
      engine_(source.engine_)
{
  // nothing to do
}

// assignment operator
LgFrSceHeurEngineMgr&
LgFrSceHeurEngineMgr::operator=(const LgFrSceHeurEngineMgr& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    sceScenSP_ = rhs.sceScenSP_;
    theWitRun_ = rhs.theWitRun_;
    theCal_ = rhs.theCal_;
    mpHelper_ = rhs.mpHelper_;
    engine_ = rhs.engine_;
  }
  return *this;
}

// destructor
LgFrSceHeurEngineMgr::~LgFrSceHeurEngineMgr()
{
  // nothing to do, 
}


/*******************************************************************************
* Set the demand vols in theWitRun equal to the minimum demands which are
* the uncummed cum ship lbs.
* Calclate the residual "above min" demand vol and store it in demand app data.
*
* Note: the calling program is responsible for deleting any demand app data 
*       ptrs that are "new-ed" in here.
********************************************************************************/
void
LgFrSceHeurEngineMgr::setMinDemandsAndCalculateAboveMinDemands(
                                                      WitRun * theWitRun,
                                                      int      nPeriods,
                                                      int      nParts,
                                                      char **  partList,
                                                      int *    nDemandsList,
                                                      char *** allDemandsList,
                                                      float *** allDemandsVols )
{
  //get the cum ship soft lower bounds, which are the cummulated
  //minimum demand volumes
  int i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<nParts; i++ ) {
    int j = 0; // Pulled out of the for below by RW2STL
    for ( j=0; j<nDemandsList[i]; j++ ) {
      float * cumShipBound, * hardLower, * hardUpper;
      witGetDemandCumShipBounds( theWitRun, partList[i], allDemandsList[i][j],
                                 &hardLower,
                                 &cumShipBound,
                                 &hardUpper);
      portableFree ( hardLower);
      portableFree ( hardUpper );
      
      // un-cum the lower bounds and use them as the demand vols for the first pass
      LgFrTimeVecFloat minDemandVol =
        LgFrTimeVecFloat(nPeriods,cumShipBound).undoCum();
      portableFree ( cumShipBound );
      // make sure the de-cummed demand vol is non-negative
      assert ( minDemandVol >= 0.0 );
      witSetDemandDemandVol( theWitRun, partList[i],
                             allDemandsList[i][j],
                             minDemandVol.data() );

      //get the total demand
      LgFrTimeVecFloat demandVol = LgFrTimeVecFloat ( nPeriods, allDemandsVols[i][j] );

      // the amt of demand in excess of the minimum (i.e. "above the min demand")
      // is demand vol used in the second pass. Calculate it and store as appData
      // in the workingWitRun
      LgFrSceDemandAppData * demandAppDataPtr = 0 ;
      witGetDemandAppData( theWitRun, partList[i], allDemandsList[i][j],
                           (void **) &demandAppDataPtr );
      if ( demandAppDataPtr == 0 ) {
        demandAppDataPtr = new LgFrSceDemandAppData ((size_t) nPeriods, 1.0);
        assert ( demandAppDataPtr != 0 );
        witSetDemandAppData( theWitRun, partList[i], allDemandsList[i][j],  
                            (void *) demandAppDataPtr );
      }
      demandAppDataPtr->aboveMinDemandVol ( demandVol - minDemandVol );
      float mytemp;
      mytemp = demandAppDataPtr->aboveMinDemandVol().min();
      if (mytemp < 0)
         printf("HERE/n");
      assert ( demandAppDataPtr->aboveMinDemandVol().min() >= -0.01 );
      
    } // end j loop     
   } //end i loop
}

//////////////////////////////////////////////////////////////////////////////////
// overloading rollover to be an "incremental" rollover
//
// You give me a witRun, global schedules, critList and supporting stuff with
// the demands you want imploded on in theWitRun (potentially after you've done other implosions)
// and this method does rollover with perishability on the demands and increments the
// the global schedules and critList and supporting stuff
// WITHOUT copying theWitRun or setting up unneeded duplicates of the structures you already have.
// So, you get the solution without a lot of overhead
//
// At the end of this method, the witRun is in "incremental" state,
// that is ready for another incremental implosion (either heuristic or rollover).
// The minLotSizes and supplyVols are correct for a next implosion.
// The shipLateLimit is returned to the value that was passed in.
// The demand volumes are returned to the values that were passed in.
// No post-processing is preformed.
//
//////////////////////////////////////////////////////////////////////////////////

void
LgFrSceHeurEngineMgr::rolloverHeurImplode( WitRun * const theWitRun,
                                           LgFrSceCritList & theCritList,
                                           float *** glbShipSched,
                                           float ** glbExecSched,
                                           float **** glbSubProdSched,
                                           float ** glbExecUpperBounds,
                                           witBoolean computeCriticalList,
                                           int nPeriods,
                                           int nParts,
                                           int nOperations,                                           
                                           char ** partList,
                                           char ** operationList,                                           
                                           int * nBom,
                                           int ** nSubsBom,
                                           witBoolean isMinLotSizeUsedAnywhere,
                                           witBoolean * isMinLotSizeUsedForOperation )                                          
{
  // get an iterative implosion helper to aid with the dirty work
  LgFrSceIterativeImpHelper  iIHelper;
  
  int i, j, t;
  
  // get the demands and create a copy of the demandVols
  int * numDemands;
  char *** glbDemList;
  float *** glbDemVols;
  iIHelper.getDemandLists( theWitRun, nParts, partList, &numDemands, &glbDemList);
  iIHelper.getDemandVols( theWitRun, nPeriods, nParts, partList, numDemands,
                           glbDemList, &glbDemVols);

  // the workingDemandVol vector is used through out the iterations of implosions
  LgFrTimeVecFloat  workingDemandVol(nPeriods, 0.0) ;

  
  // initialize default backlog yield vector 1.0 (ie. entire backlog rolls over)
  // Note: length = nPeriods even though only nPeriods-1 entries are meaningful
  // This is for consistency in the time-vec world. The last entry is garbage.
  LgFrTimeVecFloat defaultBacklogYield(nPeriods, 1.0);
  
  // save ship late limits 
  int ** glbShipLateLimits;
  iIHelper.getShipLateLimits( theWitRun, nPeriods, nParts, partList,
                               numDemands, glbDemList, &glbShipLateLimits );
                               
  // Set shipLateLimit = 0 for all demands
  for ( i=0; i<nParts; i++ ) {
    for ( j=0; j<numDemands[i]; j++ ) {
      witSetDemandShipLateLimit( theWitRun, partList[i], glbDemList[i][j], 0 );
    }
  }

  
  // Loop once for each time periods
  LgFrSceDemandAppData * dmdAppDataPtr;

  for ( t=0; t<nPeriods; t++ ) {

    // set the demand volumes
    if (t == 0) {
      // In period 0, no backlog or perishability to worry about
      for ( i=0; i<nParts; i++ ) {
        for ( j=0; j<numDemands[i]; j++ ) {
          workingDemandVol[0] =  glbDemVols[i][j][0];
          witSetDemandDemandVol( theWitRun,
                                 partList[i], glbDemList[i][j],
                                 workingDemandVol.data() );
        }
      }
    }
    else  {
      // In periods 1, ...,nPeriods-1, demands account for backlog and perishability
      for ( i=0; i<nParts; i++ ) {
        for ( j=0; j<numDemands[i]; j++ ) {
          float unmetDemand ;
          workingDemandVol[t-1] = 0.0;
          
          // calculate the unmet demand from the previous implosion and use it
          // to determine the demand for the current implosion iteration
          float * shipVol;
          float * demandVol;
          witGetDemandShipVol( theWitRun, partList[i],
                               glbDemList[i][j], &shipVol);
          witGetDemandDemandVol( theWitRun, partList[i], glbDemList[i][j], &demandVol );
          
          // since demand was non-zero only in period t-1, and shipLateLimit=0
          // the only unmet demand occurs in period t-1.
          unmetDemand = demandVol[t-1] - shipVol[t-1];

          // unmetDemand theoretically IS >=0,
          // But to be extra safe...
          if (unmetDemand < HENGMGR_FLT_EPS ) 
            unmetDemand = 0.0;
          
          portableFree(shipVol);
          portableFree(demandVol);
          
          // get backlog Yield from the Wit Run and
          // apply backlogYield to backlog ( ie. unmetDemand) from previous implosion  
          witGetDemandAppData(theWitRun, partList[i], glbDemList[i][j],
                              (void **) &dmdAppDataPtr);
          
          LgFrTimeVecFloat backlogYield(nPeriods);
          if (dmdAppDataPtr == 0)
            backlogYield = defaultBacklogYield;
          else 
            backlogYield = dmdAppDataPtr->backlogYield() ;  
          assert (backlogYield[t-1] >= 0.0 && backlogYield[t-1] <= 1.0 );     // just being careful
          
          workingDemandVol[t] =  glbDemVols[i][j][t]
                                 + (backlogYield[t-1] * unmetDemand);
          
          // being extra careful about numerical hazards...
          assert ( workingDemandVol[t] >= 0.0);
          if ( workingDemandVol[t] < HENGMGR_FLT_EPS)
            workingDemandVol[t] = 0.0;

          witSetDemandDemandVol( theWitRun, partList[i],
                                 glbDemList[i][j], workingDemandVol.data());
        } /* end j loop */
      } /* end i loop */
    } // end else t=1,...,Nperiods-1

    // implode of the period t demands
    witHeurImplode( theWitRun );

    bool doPegging = sceScenSP_->setOfParameters().getBool("doPegging");
    if (doPegging) {
      witPostprocess(theWitRun_);
      witBuildPip(theWitRun_);
      sceScenSP_->loadPIPDataIntoAppData(theWitRun_);
    }


#ifdef ROLL_DEBUG
    printf ( "\n\nRLH: The result of imploding in time period %d :", t);
    witWriteShipSched ( theWitRun, WitSTDOUT,  WitCSV);
#endif
      
    // update critical list
    if (computeCriticalList) {
      theCritList.merge(theWitRun);
    }
    
    // update the global ship schedule, global production schedule, and
    // global substitute BOM entry production schedule
    iIHelper.updateGlobalSchedules( theWitRun, nPeriods, nParts, nOperations,
                                    partList, operationList, numDemands,
                                    glbDemList, nBom, nSubsBom,
                                    glbShipSched,
                                    glbExecSched,                                         
                                    glbSubProdSched );

    
    iIHelper.prepOperationConstraintsForNextImplosionIteration ( theWitRun, nPeriods, nParts,
								 partList, nOperations, operationList,
								 glbExecSched, 
								 glbExecUpperBounds,
								 isMinLotSizeUsedAnywhere, 
								 isMinLotSizeUsedForOperation );

    
  } /* end loop over t */
  
  // restore the shipLateLimits
  iIHelper.setShipLateLimits ( theWitRun, nParts, partList,
                               numDemands, glbDemList, glbShipLateLimits );

  // restore the demand volumes to the values which were in theWitRun
  // at the beginning of the rollover
  iIHelper.setDemandVols ( theWitRun, nParts, partList,
                            numDemands, glbDemList, glbDemVols ); 
  
  // free the stored ship late limits
  for (i=0; i<nParts; i++)
    portableFree( glbShipLateLimits[i] );
  portableFree( glbShipLateLimits );
  
  // free the stored demand vols
  for (i=0; i<nParts; i++) {
    for (j=0; j<numDemands[i]; j++) {
      portableFree( glbDemList[i][j] );
      portableFree( glbDemVols[i][j] );
    }
    portableFree( glbDemList[i] );
    portableFree( glbDemVols[i] );
  }
  portableFree( glbDemList );
  portableFree( glbDemVols );
  portableFree ( numDemands );

} /* end function */
 



#ifdef NDEBUG
#undef NDEBUG
#endif



// self-test : can I self test ??
void
LgFrSceHeurEngineMgr::test()
{
  // do-nothing test  
 }

 
#endif
