// file: aEngMgr.C
// author:  tom

#include <string>
#include <iostream>
#include <scenario/src/RWToSTLHelpers.h>
#include <assert.h>
#include <mcl/src/mcl.h>
#include <scenario/src/machdep.h>
#include <sce/src/scenSceP.h> 
#include <scenario/src/calendar.h>
#include <scenario/src/timeVecs.h>
#include <sce/src/aEngMgr.h>
#include <sce/src/dmApData.h>
#include <sce/src/appData.h>
#include <sce/src/ipSce.h>
#include <sce/src/allocLet.h>
#include <sce/src/mPlantH.h>
#include <sce/src/sceParam.h>
#include <sce/src/sceCrtLs.h>
#include <sce/src/scenSce.h>
#include <sce/src/srtStrat.h>

#define AENGMGR_FLT_EPS  0.00001

// #define AENGMGR_DUMP_WIT_DATA_FILE
// #define DEBUG_SET_PURE_OPTIONS
// #define DEBUG_MANDATORY_MAX_ATP
// #define DEBUG_PURE_OPTION_MAX_ATP
// #define DEBUG_CUSTOMER_CHOICE_MAX_ATP



/********************************************************************************
// heurIncAlloc()
********************************************************************************/
void
LgFrSceAllocationEngineMgr::heurIncAlloc()
{
  int nPeriods;
  witGetNPeriods(theWitRun_, &nPeriods);

  LgFrSceSetOfParameters & setOfParameters =  sceScenSP_->setOfParameters();    

  std::string heurAllocBacklogPriority = setOfParameters.getString("heurAllocBacklogPriority");

  int useRolloverBacklogPolicy;
  if      (0 == sce::compareStringsCaseIns( heurAllocBacklogPriority, "memorylessRoll" )) 
    useRolloverBacklogPolicy = 1;
  else if (0 == sce::compareStringsCaseIns( heurAllocBacklogPriority, "immediate" )) 
    useRolloverBacklogPolicy = 0;
  else {
    useRolloverBacklogPolicy = 1;
    std::cerr << "Warning:  illegal value for parameter heurAllocBacklogPriority ==> "
	 << heurAllocBacklogPriority << "\n Using ==> memorylessRoll\n";
  }



  // we'll keep walking until we hit the last allocLet
  int aCount = 0;
  while (aPointer_ < listMax_) {
    LgFrSceAllocLet & myAllocLet = *(allocLetList_[aPointer_]);
    float qtyShip;
    // OK, do the allocation
    witIncHeurAlloc(theWitRun_,
                    myAllocLet.partName().c_str(),
                    myAllocLet.demandName().c_str(),
                    myAllocLet.shipPd(),
                    myAllocLet.qtyDemand(),
                    &qtyShip);

    // regardless of what happened, increment the allocation counter
    aCount++;

    // set the shipQty
    myAllocLet.qtyShip(qtyShip);

    // what is backlogQty
    float backlogQty = myAllocLet.qtyDemand() - qtyShip;

    // set the allocationFlag
    if (backlogQty < 0.001) {
      myAllocLet.allocated(1);
    }

    // Write to trace (if enabled)
    if (traceFlag_)       {

#if 0 
      // TEMPORARY DEBUG
      // get the part list
      int nParts;
      char ** partList;
      witGetParts(theWitRun_, &nParts, &partList);
      std::string p, g;
      int isPartGood;
      std::string p0 = partList[0];
      isPartGood = mpHelper_->isPartSpecialGeoPlannerDemandPart(theWitRun_, p0, p, g);
#endif
      outFile_ << myAllocLet.hrTraceString(theWitRun_, mpHelper_, aCount);
    }
    
    // if we DID fully commit myAllocLet then 
    // increment the allocation pointer and move on
    if (myAllocLet.allocated())  {
      aPointer_++;
    }

    // If we DID NOT  fully ship myAllocLet then ...
    else  {
      int newShipPd = myAllocLet.shipPd() + 1;

      // if we are at the end of the horizon then we are done with 
      // this allocLet and we just increment the pointer
      if (newShipPd == nPeriods) {
	myAllocLet.allocated(1);
        aPointer_++;    
      }

      // For Rollover do the following else {} 
      else if (useRolloverBacklogPolicy) {
	// **********************************************
	// *****  ROLLOVER VIA Pointers *****************
	// **********************************************
	// search through the list for an allocLet to Roll to
	int isThereAnAllocLetInNextShipPd = 0;

	// Check to see if nextShipPd == backlogDependentPtr's demandPd
	LgFrSceAllocLet * backlogDependentPtr =  myAllocLet.backlogDependent();
	if (backlogDependentPtr != 0) {
	  // ok, so there is a potential alloclet to roll this backlog to
	  // we just need to check if the periods are the same
	  int blogDepShipPd = backlogDependentPtr->shipPd();
	  // these asserts will fail if the user sets priority to be increasing over
	  // time for the part,demand pair.  This should be a FINISH_ME to check the 
	  // priority matrix up front.
	  assert(blogDepShipPd >= newShipPd);
	  assert(backlogDependentPtr->allocated() == 0);
	  if (blogDepShipPd == newShipPd) {
	    isThereAnAllocLetInNextShipPd = 1;
	    assert(backlogDependentPtr->qtyShip() < 0.0001);
	    float newDemandQty = backlogDependentPtr->qtyDemand() + backlogQty;
	    // update the demandQty on this rolled-to allocLet (add in the backlog)
	    backlogDependentPtr->qtyDemand(newDemandQty);
	    // mark this current allocLet as being done 
	    myAllocLet.allocated(1);
	    aPointer_++;
	  }
	}

	// If there is not an alloclet to roll to (in the exact period we want)
	// then we must change this alloclet to pickup the new priority according to 
	// the priority matrix.  We also update the shipPd,demandPD, and qty.
	if (! isThereAnAllocLetInNextShipPd) {
	  myAllocLet.shipPd(newShipPd);
	  myAllocLet.demandPd(newShipPd);
	  myAllocLet.qtyDemand(backlogQty);
	  myAllocLet.qtyShip(0.0);
	  int * priorityVec;
	  witGetDemandPriority(theWitRun_, 
			       myAllocLet.partName().c_str(), 
			       myAllocLet.demandName().c_str(), 
			       &priorityVec);
	  myAllocLet.priority(priorityVec[newShipPd]);
	  witFree(priorityVec);
	}


      }  // matches   ....    else if (useRolloverBacklogPolicy) {



      // do this if useRolloverBacklogPolicy == false
      else {
        // increment the shipPd (roll the demand)
        myAllocLet.shipPd(newShipPd);
        myAllocLet.qtyDemand(backlogQty);
        myAllocLet.qtyShip(0.0);
      }

      // Now sort the rest of the list
      if (listMax_ > aPointer_) {
	qsort((allocLetList_+aPointer_),            // starting from the aPointer_
	      listMax_ - aPointer_,                      // residual length
	      sizeof(LgFrSceAllocLet*), 
	      sortStrategy_->compareVoidFunc());        
      }
    }
  }
}



// **********************************************************************
// heurEqAlloc()
// **********************************************************************
void
LgFrSceAllocationEngineMgr::heurEqAlloc()
{
  char ** eqPartList;
  char ** eqDemandList;
  int   * eqShipPd;
  float * eqDemandQty;
  float * eqShipQty;
  
  int nPeriods;
  witGetNPeriods(theWitRun_, &nPeriods);

  LgFrSceSetOfParameters & setOfParameters =  sceScenSP_->setOfParameters();    

  std::string heurAllocBacklogPriority = setOfParameters.getString("heurAllocBacklogPriority");

  int useRolloverBacklogPolicy;
  if      (0 == sce::compareStringsCaseIns( heurAllocBacklogPriority, "memorylessRoll" )) 
    useRolloverBacklogPolicy = 1;
  else if (0 == sce::compareStringsCaseIns( heurAllocBacklogPriority, "immediate" )) 
    useRolloverBacklogPolicy = 0;
  else {
    useRolloverBacklogPolicy = 1;
    std::cerr << "Warning:  illegal value for parameter heurAllocBacklogPriority ==> "
	 << heurAllocBacklogPriority << "\n Using ==> memorylessRoll\n";
  }
  
  std::cout << "     ... Begin WIT allocations" << std::endl;


  // counters:
  // eCount:  number of api calls to witEqAlloc()
  // aCount:  number of allocLets fed to witEqAlloc()
  // aWithinEq: number of allocLets within current equitableGroup
  // aSuccessWithin: number of AllocLets that were fully "allocated" within current equitableGroup

  //
  // Keep picking off allocLets while the compare is equal

  int aCount = 0;
  int eCount = 0;

  while (aPointer_ < listMax_) {
    // intiialize local counters
    int aWithinEq = 0;
    int aSuccessWithin = 0;

    // get the first one, which will be the reference
    LgFrSceAllocLet & myAllocLet = *(allocLetList_[aPointer_]);
    eCount++;
    aWithinEq++;

    int finishedEquitGroup = 0;
    int isNextEquitable = 0;

    while (! finishedEquitGroup ) {    
      if ((aPointer_ + aWithinEq) < listMax_) {
        LgFrSceAllocLet & nextEquitAlloc = *(allocLetList_[aPointer_+ aWithinEq]);
	// Sorry for the complicated code ... the first conditional below, 
	// (ie, that demandPd's match) is only necessary when we are not using "rollover")
        if ( (useRolloverBacklogPolicy || 
	         (myAllocLet.demandPd() == nextEquitAlloc.demandPd())
	     )       
	     &&
               myAllocLet.shipPd()   == nextEquitAlloc.shipPd()         
	     &&
               myAllocLet.priority() == nextEquitAlloc.priority() 
	    ) {
          isNextEquitable = 1;
          aWithinEq++;
        }
        else {
          isNextEquitable = 0;
        }
      }
      else {
        isNextEquitable = 0;
      } 
      
      // if the next one is not equitable, then we have 
      // the 
      if (! isNextEquitable) {
        // since it's a non-equitable match, we have defined our equitableGroup
        eqShipPd      =  new int[aWithinEq];
        eqDemandQty   =  new float[aWithinEq];
        eqPartList    =  new char * [aWithinEq];
        eqDemandList  =  new char * [aWithinEq];
        



        int j = 0; 
        for(j=0; j<aWithinEq; j++) {
	  LgFrSceAllocLet & jthAllocLet = *(allocLetList_[aPointer_ + j]);
          eqPartList[j]   = strdup((jthAllocLet.partName()).c_str());
          eqDemandList[j] = strdup((jthAllocLet.demandName()).c_str());
          eqShipPd[j]     = jthAllocLet.shipPd();
          eqDemandQty[j]  = jthAllocLet.qtyDemand();
        }

	
        witEqHeurAlloc(theWitRun_,
                       aWithinEq,
                       (const char * const *)eqPartList,
                       (const char * const *)eqDemandList,
                       eqShipPd,
                       eqDemandQty,
                       &eqShipQty
                       );
        
        // check the results and update things
        // loop over each allocLEt within the equitableGroup
        for(j=0; j<aWithinEq; j++) {
	  LgFrSceAllocLet & currAllocLet = *(allocLetList_[aPointer_+ j]);

          // regardless of what happened, increment the allocation counter
          aCount++;
          // set the shipQty
          currAllocLet.qtyShip(eqShipQty[j]);
          // what is backlogQty
          float backlogQty = currAllocLet.qtyDemand() - eqShipQty[j];
          // determine if alloclet is FULLY satisfied and 
          // set the allocationFlag
          if (backlogQty < 0.001) {
            currAllocLet.allocated(1);
          }
          // Write to trace (if enabled)
          if (traceFlag_)       {
            outFile_ << currAllocLet.hrTraceString(theWitRun_, mpHelper_, eCount );
          }
          // if we DID ship everything 
          // increment the allocation pointer and move on
          if (currAllocLet.allocated())  {
            aSuccessWithin++;
          }
          // If we DID NOT  ship everything ...
          // Apply ==>  backlogStrategy
          // finish_me: need to figure this out ... 
          //     perishability 
          //     minimums and cumMins (??)
          //
          // For now we increment the shipPd and let the sortStrategy 
          // redo things.  
          else  {
            int nextShipPd = currAllocLet.shipPd() + 1;
            
            // if we are at the end of the horizon then we are done with this allocLet
            if (nextShipPd == nPeriods) {
              // then set the allocated Flag to 1
              currAllocLet.allocated(1);
              aSuccessWithin++;
            }

	    // For Rollover do the following else {} otherwise do the ifdef 0 else	    
            else if (useRolloverBacklogPolicy) { 





              // **********************************************
	      // *****  ROLLOVER VIA Pointers *****************
	      // **********************************************
	      // search through the list for an allocLet to Roll to
	      int isThereAnAllocLetInNextShipPd = 0;

	      // Check to see if nextShipPd == backlogDependentPtr's demandPd
	      LgFrSceAllocLet * backlogDependentPtr =  currAllocLet.backlogDependent();
	      if (backlogDependentPtr == 0) {
		// There is no one to roll to, so just use this one
		; // noop ??
	      }
	      else {
		int blogDepShipPd = backlogDependentPtr->shipPd();
		assert(blogDepShipPd >= nextShipPd);

		if (backlogDependentPtr->allocated()) {
		  std::cout << "About to fail an assert ...\n";
		  std::cout << "currAllocLet:" << currAllocLet.traceString() << "\n";
		  std::cout << "baclkogDependentAlloclet:" << backlogDependentPtr->traceString() << std::endl;
		  outFile_.close();
		}
		assert(backlogDependentPtr->allocated() == 0);
		if (blogDepShipPd == nextShipPd) {
		  isThereAnAllocLetInNextShipPd = 1;
		  assert(backlogDependentPtr->qtyShip() < 0.0001);
		  float newDemandQty = backlogDependentPtr->qtyDemand() + backlogQty;
		  // update the demandQty on this rolled-to allocLet (add in the backlog)
		  backlogDependentPtr->qtyDemand(newDemandQty);
		  // mark this current allocLet as being done 
		  currAllocLet.allocated(1);
		  aSuccessWithin++;
		}
	      }

	      if (! isThereAnAllocLetInNextShipPd) {
		currAllocLet.shipPd(nextShipPd);
		currAllocLet.demandPd(nextShipPd);
		currAllocLet.qtyDemand(backlogQty);
		currAllocLet.qtyShip(0.0);
		int * priorityVec;
		witGetDemandPriority(theWitRun_, 
				     currAllocLet.partName().c_str(), 
				     currAllocLet.demandName().c_str(), 
				     &priorityVec);
		currAllocLet.priority(priorityVec[nextShipPd]);
		witFree(priorityVec);
	      }
	    }	      
	    // JUST MADE  A BIG MESS !!!!!!!!!!!!!!!!!!!!!!
	    
	    else { 
              // *********************************
	      // *****  NON- ROLLOVER *****************
	      // *********************************
	      currAllocLet.shipPd(nextShipPd);
	      currAllocLet.qtyDemand(backlogQty);
	      currAllocLet.qtyShip(0.0);
            }                      
	    
	    
          }
        }           // end of "for (j=0  ...
        
        // Now sort just the elements within the equitableGroup
        // That will cause the allocated ones to move to the "front" of the subArray
        // Then we move the aPointer exactly aSuccessWithin elements forward
        
        // don't need to do this if all were satisfied
        if (aSuccessWithin < aWithinEq) {
          qsort((allocLetList_+ aPointer_),            // starting from the aPointer_
                aWithinEq,                             // residual length
                sizeof(LgFrSceAllocLet *), 
                sortStrategy_->compareVoidFunc());          
        }
        
        // Increment aPointer_ by aSuccessWithin
        aPointer_ += aSuccessWithin;

        // Do the final sort of the remaining allocLets
        qsort((allocLetList_+ aPointer_),            // starting from the aPointer_
              listMax_ - aPointer_,                  // residual length
              sizeof(LgFrSceAllocLet *), 
              sortStrategy_->compareVoidFunc());          

        // indicate that we've completed an equit group
        finishedEquitGroup = 1;

        for(j=0; j<aWithinEq; j++) {
          free(eqPartList[j]);
          free(eqDemandList[j]);
        }
	delete [] eqShipPd;    
	delete [] eqDemandQty; 
	witFree(eqShipQty);    
	delete [] eqPartList;  
	delete [] eqDemandList;
      }           // end of "if (! isNextEquitable)  ...
    }           // end of "while (! finishedEquitGroup ) ...
  }           // end of "while (aPointer_ < listMax_) ...
  std::cout << "     ...  WIT allocations completed" << std::endl;

}





// **********************************************************************
// **********************************************************************
// implode()
// **********************************************************************
// **********************************************************************
void
LgFrSceAllocationEngineMgr::implode(LgFrSceCritList & theCritList)
{
  // open the trace file
  if (traceFlag_) {
      outFile_.open(traceFileName_.c_str() );//, 0666);
    if (outFile_.fail()) {
      std::cerr << "Unable to open File for output:  Trace File: "
	   << traceFileName_ << "\n" 
	   << "Terminating the program\n\n";
      exit(8);
    }
  }


  LgFrSceSetOfParameters & setOfParameters =  sceScenSP_->setOfParameters();    
  std::string heurAllocSortStrategy = setOfParameters.getString("heurAllocSortStrategy");
  
  std::cout << "Executing (Heur Alloc) " 
	    << usageName_
	    << " engine using ("
       << heurAllocSortStrategy 
       << ") sort strategy ..." 
       << std::endl;    

#ifdef AENGMGR_DUMP_WIT_DATA_FILE
  std::cout << "     ... (debug mode) ... Writing the (heurAlloc) wit data file ..." << std::endl;                      
  sceScenSP_->setWitInformationalMessaging(theWitRun_, SCE_WIT_MESSAGING_ON);
  witWriteData(theWitRun_, "preSetPureOptionDemand.witData");
  sceScenSP_->setWitInformationalMessaging(theWitRun_, SCE_WIT_MESSAGING_OFF);          
#endif

  // Set DemandVols on special Feature parts ... custChoice is set to max feat vol
  // and pureOptional are set to bigM
  this->setPureOptionDemandVols();

  // Test the iput to see if the priorities are non-increasing
  // This is needed for backlogPolicy=memorylessRoll
  this->testForNonIncreasingPriorities();

#ifdef AENGMGR_DUMP_WIT_DATA_FILE
  std::cout << "     ... (debug mode) ... Writing the (heurAlloc) wit data file ..." << std::endl;                      
  sceScenSP_->setWitInformationalMessaging(theWitRun_, SCE_WIT_MESSAGING_ON);
  witWriteData(theWitRun_, "postSetPureOptionDemand.witData");
  witDisplayData( theWitRun_,"debug_aEngMgr.witEcho");
  sceScenSP_->setWitInformationalMessaging(theWitRun_, SCE_WIT_MESSAGING_OFF);          
#endif



  // --- Greedy Search Granularity ---
  std::string indpGreedySearchIncFile = setOfParameters.getString("indpGreedySearchIncFile");
  std::string greedySearchIncFile = setOfParameters.getString("greedySearchIncFile");
  int isGreedySearchIncSetInFile = ( (indpGreedySearchIncFile == "") && (greedySearchIncFile == "")) ? 0 : 1;
  if (! isGreedySearchIncSetInFile) {
    std::cout << "     ... setting Greedy Search Increments Globally." << std::endl;
    std::string greedySearchIncrementMethod = setOfParameters.getString("greedySearchIncrementMethod");
    if (sce::compareStringsCaseIns(greedySearchIncrementMethod, "autoScale") == 0) {
      float searchIncScaleFactor =  setOfParameters.getFloat("searchIncScaleFactor");
      sceScenSP_->setHeuristicSearchIncrement(theWitRun_, greedySearchIncrementMethod, searchIncScaleFactor); 
    }
    else {
      float searchIncAbsValue =  setOfParameters.getFloat("searchIncAbsValue");
      sceScenSP_->setHeuristicSearchIncrement(theWitRun_, greedySearchIncrementMethod, searchIncAbsValue); 
    }
  }
  else {
    //    std::cout << "Skipping Greedy Search Increment Global settings since input files were used." << std::endl;
  }


#if 0
  // FOR MRP TESTING ONLY ... Although make this a formal diagnostic report
  witSetComputeCriticalList(theWitRun_, WitTRUE);
  witSetPgdCritListMode(theWitRun_, WitTRUE);
#endif

  // See if a critical list is wanted.
  witBoolean computeCriticalList;
  witGetComputeCriticalList ( theWitRun_, &computeCriticalList );  


  //   // See if a pegged critical list is wanted.
  //  witBoolean computePeggedCriticalList;
  //  witGetPgdCritListMode ( theWitRun_, &computePeggedCriticalList );  

  //   witBoolean doesWitPipExist;

  int equitability;
  witGetEquitability(theWitRun_, &equitability);


  // Dump the heuristic version of the wit.data file
  if (setOfParameters.getBool("printHeurWitDataFile"))  {
    std::string witDataFileName = setOfParameters.getString("heurWitDataFileName");
    sceScenSP_->setWitInformationalMessaging(theWitRun_, SCE_WIT_MESSAGING_ON);
    witWriteData(theWitRun_, witDataFileName.c_str());
    if (! setOfParameters.getBool("printWitMsgFile"))   {
      sceScenSP_->setWitInformationalMessaging(theWitRun_, SCE_WIT_MESSAGING_OFF);          
    }
  }
  
  



  // tell Wit to get ready
  std::cout << "     ...  witStartHeurAlloc" << std::endl;
  witStartHeurAlloc(theWitRun_);

  

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
  

  std::string heurAllocMinsSortStrategy = setOfParameters.getString("heurAllocMinsSortStrategy");
  int sortID = 1;
  int minsSortId = this->sortID(heurAllocMinsSortStrategy);
  LgFrSceSortStrategy * mySortStrategy = new LgFrSceSortStrategy(minsSortId);
  
  // set allocMgr's strategy.  
  //  std::cout << "     ...  setting sortStrategy" << std::endl;
  this->sortStrategy(mySortStrategy);
  //  std::cout << "     ...  setting sortStrategy OK" << std::endl;
  
  if (sceScenSP_->nDemandsWithNonZeroMins() > 0) {
    
    std::cout << "     ... allocating to mins using (heurAlloc) scheme and ("
	      << heurAllocMinsSortStrategy 
	      << ") sort strategy ..." 
	      << std::endl;    
  }  //   end   if (nDemandsWithNonZeroMins() > 0)  
  
  // intitialize allocLetList_ for minimum demands
  // Get the minimum demands and calculate the residual "aboveMinDemands"
  // First "wave" of allocLets are exclusively for mins
  // The aboveMinDemands are stored in demand appData
  this->initializeAllocDataWithMins();
  
  // allocate this bunch of allocLets
  if  (1 == equitability){
    this->heurIncAlloc();
  }
  else {
    this->heurEqAlloc();
  }
  
  
  //  De-allocate the allocLet list for this pass????
  if (listMax_ > 0) {
    delete [] allocLetList_;
    delete [] vecAllocLetList_;
  }
  
  delete mySortStrategy;
  
  
  
  
  // ==================================================
  // STEP 2: Implode  residual "above min" demand
  //          a) If there are no mins, then the "residual above min demand"
  //             is simply the original demandVol
  // ==================================================  

  
  std::cout << "  ... First pass allocation  using (heurAlloc) scheme and ("
       << heurAllocSortStrategy 
       << ") sort strategy ..." 
       << std::endl;    
  

  int aboveMinSortId = this->sortID(heurAllocSortStrategy);
  mySortStrategy = new LgFrSceSortStrategy(aboveMinSortId);
  
  // set allocMgr's strategy.  
  this->sortStrategy(mySortStrategy);

  // intitialize allocLetList_ for minimum demands
  // Get the minimum demands and calculate the residual "aboveMinDemands"
  // First "wave" of allocLets are exclusively for mins
  // The aboveMinDemands are stored in demand appData
  this->initializeAllocDataAboveMins();

  // allocate this bunch of allocLets
  if  (1 == equitability){
    this->heurIncAlloc();
  }
  else {
    this->heurEqAlloc();
  }


  //  De-allocate the allocLet list for this pass
  if (listMax_ > 0) {
    delete [] allocLetList_;
    delete [] vecAllocLetList_;
  }

  delete mySortStrategy;


  // ==================================================
  // STEP 3: Feature Cleanup implosion ...
  // ==================================================
  //    This iteration needs some explanation
  //       Heuristic is not smart enough to allocate to features
  // ==================================================  
  
  if (sceScenSP_->nStandaloneFeatureArcs() > 0) {
    std::string heurAllocFeatSortStrategy = setOfParameters.getString("heurAllocFeatSortStrategy");
    int featSortId = this->sortID(heurAllocFeatSortStrategy);
    LgFrSceSortStrategy * mySortStrategy = new LgFrSceSortStrategy(featSortId);
    
    std::cout << "  ... Feature pass allocation using (heurAlloc) scheme and ("
	      << heurAllocFeatSortStrategy << ") sort strategy ..." << std::endl;    
    
    this->sortStrategy(mySortStrategy);
    
    // FINISH_ME_61: 
    //    - get rid of wit34
    //    - variable periods
    //    - multiAttr demand, dated vs vector ???
    //    - FINISH for Selective Sets
    //    - Do we need two codes sets ?!?!?!? setPureOption ... ForMaxAtp  ????
    // setPureOptionBuildAllocVols ... determine demandVols for feature 
    bool maxImpliedCommitsForAtp  = setOfParameters.getBool("maxImpliedCommitsForAtp");
    if (maxImpliedCommitsForAtp) 
      this->setPureOptionBuildAllocVolsForMaxATP();
    else
      this->setPureOptionBuildAllocVols();

    this->initializeAllocDataFeaturePass();
    
    
    // implode !!
    if  (1 == equitability){
      this->heurIncAlloc();
    }
    else {
      this->heurEqAlloc();
    }
    
    if (listMax_ > 0) {
      delete [] allocLetList_;
      delete [] vecAllocLetList_;
    }
    delete mySortStrategy;

  } // end of features pass
  
  witFinishHeurAlloc(theWitRun_);
  
  if (computeCriticalList) 
    theCritList.merge( theWitRun_);
  

  bool doPegging = setOfParameters.getBool("doPegging");
  if (doPegging) {
    witPostprocess (theWitRun_ );    
    witBuildPip(theWitRun_);
    sceScenSP_->loadPIPDataIntoAppData(theWitRun_);
  }

  outFile_.close();

}



// Test the iput to see if the priorities are non-increasing
// This is needed for backlogPolicy=memorylessRoll
//   testForNonIncreasingPriorities();
//  ------------------------------- 
void
LgFrSceAllocationEngineMgr::testForNonIncreasingPriorities()
{


  int   * priority;
  int nPeriods;
  char  *partName, *demandName;
  // float * demandVol;
  
    
  witGetNPeriods(theWitRun_, &nPeriods);
  int nParts;
  char  **partList;

  LgFrSceSetOfParameters & setOfParameters =  sceScenSP_->setOfParameters();    
  std::string heurAllocBacklogPriority = setOfParameters.getString("heurAllocBacklogPriority");
  int useRolloverBacklogPolicy = 1;

  if  (0 == sce::compareStringsCaseIns( heurAllocBacklogPriority, "memorylessRoll" )) 
    useRolloverBacklogPolicy = 1;
  else if (0 == sce::compareStringsCaseIns( heurAllocBacklogPriority, "immediate" )) 
    useRolloverBacklogPolicy = 0;
  else {
    useRolloverBacklogPolicy = 1;
    std::cerr << "Warning:  illegal value for parameter heurAllocBacklogPriority ==> "
	 << heurAllocBacklogPriority << "\n Using ==> memorylessRoll\n";
  }

  // if we are not using backlogRollover, then we don't need this test
  if (!useRolloverBacklogPolicy)
    return;


  std::cout << "     ... testing priorities for non-increasing values" << std::endl;

  witGetParts(theWitRun_, &nParts, &partList);




  // first count how many allocLets we need ==> listMax.
  // NOTE: we only count non-zero minDemandVols
  int hasIncreasingPriorityMessageBeenToldYet = 0;
  int i = 0; 
  for(i=0; i<nParts; i++){
    partName = partList[i];
    int nDemands;
    char ** dList;
    witGetPartDemands(theWitRun_, partList[i], &nDemands, &dList);
    int j = 0; 
    for(j=0; j<nDemands; j++){
      demandName = dList[j];
      witGetDemandPriority( theWitRun_, partName, demandName,
                                 &priority);
      int prevPrio = -INT_MAX;

      int hasPriorityChanged = 0;
	
      int t = 0; 
      for (t=0; t<nPeriods; t++)   {
        if (prevPrio > priority[t]) {
	  if (! hasIncreasingPriorityMessageBeenToldYet) {
	    std::cout << "Warning: priority vector is increasing ... \n"
		      << "Part: " << partName << "\n"
		      << "Demand: " << demandName << "\n"
		      << "period: " << t << std::endl;
	    hasIncreasingPriorityMessageBeenToldYet = 1;
	  }
	  // temp fix !!!
	  priority[t-1] = priority[t];
	  hasPriorityChanged = 1;
	}
	
	prevPrio = priority[t];
      }
      if (hasPriorityChanged)
	witSetDemandPriority( theWitRun_, partName, demandName,
                                 priority);

      witFree(dList[j]);
      witFree(priority);
    }
    witFree(dList);
    witFree(partList[i]);
  }
  witFree(partList);
  //  std::cout << "     ... done testing priorities for non-increasing values" << std::endl;
}







// ******************************************************************************
// initializeAllocDataWithMins()
// ------------------------------
// Set up allocLet List for second (main) pass through Heuristic Allocation Pass
//
// Loop through the CumShipBounds to derive the MinimumDemandVols
// - Create an allocLet for each positive minDemandVol
// - Compute and store away the aboveMinDemandVols in demand app data
// - QSORT the allocLetList
// - Note: the calling program is responsible for deleting any demand app data 
//         ptrs that are "new-ed" in here.
// - FINISH_ME: Put allocVol in appData
// ********************************************************************************
void
LgFrSceAllocationEngineMgr::initializeAllocDataWithMins()
{
  int   * priority;
  int nPeriods;
  char  *partName, *demandName;
  float * demandVol;
  
    
  witGetNPeriods(theWitRun_, &nPeriods);
  int nParts;
  char  **partList;
  witGetParts(theWitRun_, &nParts, &partList);

  std::cout << "     ... initializing allocData for mins" << std::endl;

  aPointer_ = 0;
  listMax_ = 0;

  // first count how many allocLets we need ==> listMax.
  // NOTE: we only count non-zero minDemandVols
  int i = 0; 
  for(i=0; i<nParts; i++){
    partName = partList[i];
    int nDemands;
    char ** dList;
    witGetPartDemands(theWitRun_, partList[i], &nDemands, &dList);
    int j = 0; 
    for(j=0; j<nDemands; j++){
      demandName = dList[j];
      float * cumShipBound, * hardLower, * hardUpper;
      witGetDemandCumShipBounds( theWitRun_, partName, demandName,
                                 &hardLower,
                                 &cumShipBound,
                                 &hardUpper);
      witFree ( hardLower);
      witFree ( hardUpper );
      
      // un-cum the lower bounds and use them as the demand vols for the first pass
      LgFrTimeVecFloat minDemandVol =
        LgFrTimeVecFloat(nPeriods,cumShipBound).undoCum();
      witFree ( cumShipBound );
      // make sure the de-cummed demand vol is non-negative
      assert ( minDemandVol >= 0.0 );

      int t = 0; 
      for (t=0; t<nPeriods; t++)   {
        if (minDemandVol[t] > 0.0)
	  listMax_++;
      }
      witFree(dList[j]);
    }
    witFree(dList);
  }



  // Now allocate the vector of allocLet Ptrs
  vecAllocLetList_ = new LgFrSceAllocLet [listMax_];
  // and the vector of allocLets
  allocLetList_ =     new LgFrSceAllocLet * [listMax_];
  
  LgFrSceAllocLet * dependeeAllocLet = 0;

  // now load each allocLet
  for( i=0; i<nParts; i++){
    partName = partList[i];
    int nDemands;
    char ** dList;
    witGetPartDemands(theWitRun_, partName, &nDemands, &dList);
    int j = 0; 
    for(j=0; j<nDemands; j++){
      demandName = dList[j];
      float * cumShipBound, * hardLower, * hardUpper;
      witGetDemandCumShipBounds( theWitRun_, partName, demandName,
                                 &hardLower,
                                 &cumShipBound,
                                 &hardUpper);
      witFree ( hardLower);
      witFree ( hardUpper );
      
      // un-cum the lower bounds and use them as the alloclet vols for the first pass
      LgFrTimeVecFloat minDemandVol =
        LgFrTimeVecFloat(nPeriods,cumShipBound).undoCum();
      witFree ( cumShipBound );
      // make sure the de-cummed demand vol is non-negative
      assert ( minDemandVol >= 0.0 );
      
      
      // reset the dependee ptr to 0
      dependeeAllocLet = 0;

      witGetDemandPriority(theWitRun_, partName, demandName, &priority);
      witGetDemandDemandVol(theWitRun_, partName, demandName, &demandVol);
      int t = 0; 
      for (t=0; t<nPeriods; t++) {
        if (minDemandVol[t] > 0.0) {
          LgFrSceAllocLet & myAllocLet = vecAllocLetList_[aPointer_];
          allocLetList_[aPointer_] = &myAllocLet;
          myAllocLet.partName(partName);
          myAllocLet.demandName(demandName);
          myAllocLet.priority(priority[t]);
          myAllocLet.qtyDemand(minDemandVol[t]);
          myAllocLet.demandPd(t);
          myAllocLet.shipPd(t);
          myAllocLet.allocated(0);

	  if (dependeeAllocLet != 0) {
	    // if there's a dependee, then he's waiting to know the current one
	    dependeeAllocLet->backlogDependent(&myAllocLet);
	  }
	  // remember this allocLet
	  dependeeAllocLet = &myAllocLet;
          aPointer_++;
        }
      }




      // the amt of demand in excess of the minimum (i.e. "above the min demand")
      // is demand vol used in the second pass. Calculate it and store as appData
      // in the workingWitRun
      LgFrSceDemandAppData * demandAppDataPtr = 0 ;
      witGetDemandAppData( theWitRun_, partName, demandName,
                           (void **) &demandAppDataPtr );
      if ( demandAppDataPtr == 0 ) {
        demandAppDataPtr = new LgFrSceDemandAppData ((size_t) nPeriods, 1.0);
        assert ( demandAppDataPtr != 0 );
        witSetDemandAppData( theWitRun_, partName, demandName,  
                            (void *) demandAppDataPtr );
      }
      LgFrTimeVecFloat aboveMinDemandVol((size_t) nPeriods, 0.0);
      for (t=0; t<nPeriods; t++)
	aboveMinDemandVol[t] = demandVol[t] - minDemandVol[t];
      demandAppDataPtr->aboveMinDemandVol ( aboveMinDemandVol );
      float mytemp;
      mytemp = aboveMinDemandVol.min();
      if (mytemp < 0)
         printf("HERE/n");
      assert ( aboveMinDemandVol.min() >= -0.01 );


      witFree(dList[j]);
      witFree(demandVol);
      witFree(priority);
    }
    witFree(dList);
    witFree(partList[i]);
  }
  witFree(partList);
  
  assert(aPointer_ == listMax_);
  aPointer_ = 0;

  qsort(allocLetList_, 
        listMax_, 
	sizeof(LgFrSceAllocLet*), 
        sortStrategy_->compareVoidFunc());

  std::cout << "     ... DONE initializing allocData for mins" << std::endl;

}



// ******************************************************************************
// initializeAllocDataAboveMins()
// ------------------------------
// Set up allocLet List for second (main) pass through Heuristic Allocation Pass
// - FINISH_ME: Put allocVol in appData
// ********************************************************************************
void
LgFrSceAllocationEngineMgr::initializeAllocDataAboveMins()
{
  

  int   * priority;
  int nPeriods;
  char  *partName, *demandName;
    
  witGetNPeriods(theWitRun_, &nPeriods);
  int nParts;
  char  **partList;
  witGetParts(theWitRun_, &nParts, &partList);

  // status logging only 
  std::cout << "     ... entering initializeAllocDataAboveMins() " << std::endl;

  aPointer_ = 0;
  listMax_ = 0;

  // first count how many allocLets we need ==> listMax.
  // NOTE: we only count non-zero demandVols
  int i = 0; 
  for(i=0; i<nParts; i++){
    partName = partList[i];

    // skip over any special feature parts.  Any demand on these is for the next pass
    if (mpHelper_->isPartSpecialFeature(theWitRun_, partName)) {
      continue;
    }

    int nDemands;
    char ** dList;
    witGetPartDemands(theWitRun_, partList[i], &nDemands, &dList);
    int j = 0; 
    for(j=0; j<nDemands; j++){

      demandName = dList[j];

      // get the "above min demands" out of the demand app data
      LgFrSceDemandAppData * dmdAppDataPtr;
      witGetDemandAppData( theWitRun_, partName, demandName,
			   (void **) &dmdAppDataPtr);
      LgFrTimeVecFloat demandVol( dmdAppDataPtr->aboveMinDemandVol() );
      assert (demandVol.min() >= 0.0 );

      int t = 0; 
      for (t=0; t<nPeriods; t++)   {
        if (demandVol[t] > 0.0)
	  listMax_++;
      }
      witFree(dList[j]);
    }
    witFree(dList);
  }




  // Now allocate the vector of allocLet Ptrs
  vecAllocLetList_ = new LgFrSceAllocLet [listMax_];
  // and the vector of allocLets
  allocLetList_ = new LgFrSceAllocLet * [listMax_];

  // status logging only 
  std::cout << "     ... allocLetList size:" << listMax_ << std::endl;
  

  // now load each allocLet
  for( i=0; i<nParts; i++){
    partName = partList[i];

    // skip over any special feature parts.  Any demand on these is for the next pass
    if (mpHelper_->isPartSpecialFeature(theWitRun_, partName)) {
      witFree(partList[i]);
      continue;
    }

    int nDemands;
    char ** dList;
    witGetPartDemands(theWitRun_, partName, &nDemands, &dList);
    int j = 0; 
    for(j=0; j<nDemands; j++){
      demandName = dList[j];

      LgFrSceAllocLet * dependeeAllocLet = 0;
      witGetDemandPriority(theWitRun_, partName, demandName, &priority);
      
      // get the "above min demands" out of the demand app data
      LgFrSceDemandAppData * dmdAppDataPtr;
      witGetDemandAppData( theWitRun_, partName, demandName,
			   (void **) &dmdAppDataPtr);
      LgFrTimeVecFloat demandVol( dmdAppDataPtr->aboveMinDemandVol() );
      assert (demandVol.min() >= 0.0 );

      int t = 0; 
      for (t=0; t<nPeriods; t++)   {
        if (demandVol[t] > 0.0){
          LgFrSceAllocLet & myAllocLet = vecAllocLetList_[aPointer_];
          allocLetList_[aPointer_] = &myAllocLet;
          myAllocLet.partName(partName);
          myAllocLet.demandName(demandName);
          myAllocLet.priority(priority[t]);
          myAllocLet.qtyDemand(demandVol[t]);
          myAllocLet.demandPd(t);
          myAllocLet.shipPd(t);
          myAllocLet.allocated(0);

	  if (dependeeAllocLet != 0) {
	    // if there's a dependee, then he's waiting to know the current one
	    dependeeAllocLet->backlogDependent(&myAllocLet);
	  }
	  // remember this allocLet
	  dependeeAllocLet = &myAllocLet;
          aPointer_++;
        }
      }


      witFree(dList[j]);
      witFree(priority);
    }
    witFree(dList);
    witFree(partList[i]);
  }
  witFree(partList);
  
  assert(aPointer_ == listMax_);
  aPointer_ = 0;

  // status logging only 
  std::cout << "     ... initialized allocLetLists" << std::endl;


  // status logging only 
  std::cout << "     ... starting qsort of allocLetList" << std::endl;


  qsort(allocLetList_, 
        listMax_, 
	sizeof(LgFrSceAllocLet*), 
        sortStrategy_->compareVoidFunc());

  // status logging only 
  std::cout << "     ... finished  initial qsort of allocLetList" << std::endl;

}

// ******************************************************************************
// setPureOptionDemandVols()
// ------------------------------
// Must front-load the demandVols for the special feature demands
// ********************************************************************************
void 
LgFrSceAllocationEngineMgr::setPureOptionDemandVols()
{

  std::cout << "     ... executing  setPureOptionDemandVols..." << std::endl;  

  LgFrSceSetOfParameters & setOfParameters =  sceScenSP_->setOfParameters();    

  // get the number of periods
  int nPeriods;
  witGetNPeriods( theWitRun_, &nPeriods ) ;

  // get the operation list
  int  nOperations ;      
  char ** operationList ;    
  witGetOperations( theWitRun_, &nOperations, &operationList );

  float * bigMdemandVol = new float[nPeriods];
  int t = 0; 
  for (t=0; t<nPeriods; t++)
    bigMdemandVol[t] = 98765431.f;


  std::string featureRatioPegMethod = setOfParameters.getString("featureRatioPegMethod");
  //  choices ==>  pegToMachineDemandPeriod  or  pegToMachineShipPeriod;


  // Loop once for each operation.
  int o = 0; 
  for ( o=0; o<nOperations; o++ ) {
    std::string fullWitGeoPlannerDemandOperation(operationList[o]);
    std::string plannerTopLevelPart;
    std::string geo;

    // Start with  GPD parts ...  then look for  specialFeatureParts as children. 
    if (! mpHelper_->isOperationSpecialGeoPlannerDemand(theWitRun_, fullWitGeoPlannerDemandOperation,
						     plannerTopLevelPart, geo))  {
      witFree (operationList[o]);
      continue;
    }
    int nBoms;
    witGetOperationNBomEntries( theWitRun_, operationList[o], &nBoms );
    // Now loop through the children of the GPD operation
    int j = 0; 
    for (j=0; j<nBoms; j++)  {

      // get the name of the child part, 
      char * fullWitFeaturePart;
      witGetBomEntryConsumedPart(theWitRun_, fullWitGeoPlannerDemandOperation.c_str(), j, &fullWitFeaturePart);

      // We are only interested in special Feature Parts which, at this time,
      // can either be custChoice or stdalone
      if (!  mpHelper_->isPartSpecialFeature(theWitRun_, fullWitFeaturePart)) {
	witFree(fullWitFeaturePart);
	continue;
      }
      std::string gg, pp, ppdf, fp;

      // + --------------------------------
      // + If its a CUSTOMER_CHOICE_FEATURES
      // + --------------------------------
      if (mpHelper_->isPartSpecialCustChoiceFeature(theWitRun_, fullWitFeaturePart, 
						   gg, pp, ppdf, fp)) {
	    
	    
	// Here we process Customer Choice Features
	// total possible demandVol on CC features is
	//          demandVol(GPD) * usageRate * featratio
	    
	// Multiple BOM entries between the parent and the child could be used to implement
	// different feature rates over time.
	std::string featureDemandName = mpHelper_->custChoiceFeatureDemandName(fullWitGeoPlannerDemandOperation );
	    
	float * gpdDemVol;
	witGetDemandDemandVol( theWitRun_, fullWitGeoPlannerDemandOperation.c_str(), geo.c_str(), &gpdDemVol);
	float usageRate;
	witGetBomEntryUsageRate(theWitRun_, fullWitGeoPlannerDemandOperation.c_str(), j, &usageRate);	      
	// featRatio is stored in GPD->SFP bom's appData
	LgFrTimeVecFloat featRatio = mpHelper_->custChoiceFeatureRatio(theWitRun_, fullWitFeaturePart);
    
	float * featDemandVol;
	witGetDemandDemandVol(theWitRun_, fullWitFeaturePart, featureDemandName.c_str(), &featDemandVol);
	// sanity check !!!
	LgFrTimeVecFloat featDvTVF((size_t) nPeriods, featDemandVol);
	assert(featDvTVF.max() < 0.001);

	// now compute the total Features Needed ...
	int t = 0; 
	for ( t=0; t<nPeriods; t++)
	  featDemandVol[t] = featRatio[t] * gpdDemVol[t] * usageRate;
	    
	witSetDemandDemandVol(theWitRun_, fullWitFeaturePart, featureDemandName.c_str(), featDemandVol);

	witFree(featDemandVol);
	witFree(gpdDemVol);
	witFree(fullWitFeaturePart);
	continue;
      }   //  end  ...        if (mpHelper->isPartSpecialCustChoiceFeature())  ...
      

      // + --------------------------------
      // + If its a STANDALONE FEATURE
      // + --------------------------------
      // Just for the sake of it ....
      assert (mpHelper_->isPartSpecialStandaloneFeature(theWitRun_, fullWitFeaturePart, 
							gg, pp, ppdf, fp));
      
      int nDemands;
      char ** dList;
      witGetPartDemands(theWitRun_, fullWitFeaturePart, &nDemands, &dList);
      int j = 0; 
      for (j=0; j<nDemands; j++) {
	witSetDemandDemandVol(theWitRun_, fullWitFeaturePart, dList[j], bigMdemandVol);
	witFree(dList[j]);
      }
      witFree(dList);
      witFree(fullWitFeaturePart);
    }      // end  ...  for (int j=0; j<nBoms; j++)  
    witFree (operationList[o]);
  }    // end  ...    for ( int o=0; o<nOperations; o++ ) 
  witFree(operationList);
  delete [] bigMdemandVol;
}



// use dumpaVec for debugging 
void
LgFrSceAllocationEngineMgr::dumpaVec(std::string aMessage, LgFrTimeVecFloat & theVec)
{

  int nPeriods;
  witGetNPeriods(theWitRun_, &nPeriods);

  std::cout << aMessage << ": [";
  int t = 0; 
  for (t=0; t<nPeriods-1; t++)
    std::cout << theVec[t] << ",";
  std::cout << theVec[t] << "]" << std::endl;
}

void
LgFrSceAllocationEngineMgr::dumpaVec(std::string aMessage, float * theVec)
{

  int nPeriods;
  witGetNPeriods(theWitRun_, &nPeriods);

  std::cout << aMessage << ": [";
  int t = 0; 
  for (t=0; t<nPeriods-1; t++)
    std::cout << theVec[t] << ",";
  std::cout << theVec[t] << "]" << std::endl;
}



// ******************************************************************************
// setPureOptionBuildAllocVols()
// ------------------------------
// Calculate allocVols for  for features pass of  Heuristic Allocation Pass
//   - allocVol's are placed   in appData
//   - This routine does not create  allocLets.  
//   - Instead it sets up the allocVols in dmdAppData ... in preparation for allocLets
//   - Most of this is clipped from setPureOptionBuildDemand() which is in hEngMgr
// - DEVELOP_HERE
// ********************************************************************************
void 
LgFrSceAllocationEngineMgr::setPureOptionBuildAllocVols()
{

  std::cout << "     ... executing  setPureOptionBuildAllocVols..." << std::endl;  

  LgFrSceSetOfParameters & setOfParameters =  sceScenSP_->setOfParameters();    

  // get the number of periods
  int nPeriods;
  witGetNPeriods( theWitRun_, &nPeriods ) ;

  // get the operation list
  int  nOperations ;      
  char ** operationList ;    
  witGetOperations( theWitRun_, &nOperations, &operationList );


  // this is obsolete.  We are moving towards no longer allowing ratio's to change.
  std::string featureRatioPegMethod = setOfParameters.getString("featureRatioPegMethod");
  //  choices ==>  pegToMachineDemandPeriod  or  pegToMachineShipPeriod;


  // Loop once for each operation.
  int o = 0; 
  for ( o=0; o<nOperations; o++ ) {
    std::string fullWitGeoPlannerDemandOperation(operationList[o]);
    std::string plannerTopLevelPart;
    std::string geo;

    // Start with  GPD parts ...  then look for  specialFeatureParts as children. 
    if (! mpHelper_->isOperationSpecialGeoPlannerDemand(theWitRun_, fullWitGeoPlannerDemandOperation,
						     plannerTopLevelPart, geo))  {
      continue;
    }
    int nBoms;
    witGetOperationNBomEntries( theWitRun_, operationList[o], &nBoms );
    // Now loop through the children of the GPD operation
    int j = 0; 
    for (j=0; j<nBoms; j++)  {

      // get the name of the child part, 
      char * fullWitFeaturePart;
      witGetBomEntryConsumedPart(theWitRun_, fullWitGeoPlannerDemandOperation.c_str(), j, &fullWitFeaturePart);

      // We are only interested in special Feature Parts which, at this time,
      // can either be custChoice or stdalone
      if (!  mpHelper_->isPartSpecialFeature(theWitRun_, fullWitFeaturePart)) {
	witFree(fullWitFeaturePart);
	continue;
      }
      std::string gg, pp, ppdf, fp;

      // + --------------------------------
      // + If its a CUSTOMER_CHOICE_FEATURES
      // + --------------------------------
      if (mpHelper_->isPartSpecialCustChoiceFeature(theWitRun_, fullWitFeaturePart, 
						   gg, pp, ppdf, fp)) {


	std::string featureDemandName = 
	  mpHelper_->custChoiceFeatureDemandName(fullWitGeoPlannerDemandOperation );
	    
	float * gpdDemVol;
	witGetDemandDemandVol( theWitRun_, fullWitGeoPlannerDemandOperation.c_str(), geo.c_str(), &gpdDemVol);
	float * gpdExecVol;
	witGetOperationExecVol( theWitRun_, fullWitGeoPlannerDemandOperation.c_str(), &gpdExecVol);
	float usageRate;
	witGetBomEntryUsageRate(theWitRun_, fullWitGeoPlannerDemandOperation.c_str(), j, &usageRate);	      
	// featRatio is stored in GPD->SFP bom's appData
	LgFrTimeVecFloat featRatio = mpHelper_->custChoiceFeatureRatio(theWitRun_, fullWitFeaturePart);

	// total features needed
	LgFrTimeVecFloat featureDemandPreUsageRate((size_t) nPeriods, 0.0);

	// This *may* be obsolete.  It's an attempt to adjust the execVols back to the 
	// the period when the demand was due. 
	LgFrTimeVecFloat tiedBackGpdExecVol((size_t) nPeriods, gpdExecVol);



#ifdef DEBUG_SET_PURE_OPTIONS
	std::cout << "\n\n******\nGPD:" << fullWitGeoPlannerDemandOperation << std::endl;
	std::cout << "CC Feature:" << fullWitFeaturePart << std::endl;
	dumpaVec("gpdDemVol:", gpdDemVol);
	dumpaVec("gpdExecVol:", gpdExecVol);
	dumpaVec("featRatio:", featRatio);
#endif


	// if the ratio's are constant over time, then life is easy
	int areRatiosConstant = 0;
	if (featRatio.max() - featRatio.min() < 0.0001) {
	  areRatiosConstant = 1;
	  
	  // featureDemandPreUsageRate is GPD ships times the featRatio	  
	  int t = 0; 
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
	  // ---- Method 1: only used when ratio's change over time  ----
	  // This method was flawed and was to be be replaced by method 2,
	  // but that was screwing up also

	  // Compute ==> tiedBackGpdExecVol 
	  int t = 0; 
	  for (t=0; t<nPeriods; t++) {
	    float demExcess = gpdDemVol[t] - tiedBackGpdExecVol[t];
	    // if this period matches, then continue
	    if (demExcess < .001)
	      continue;
	    // otherwise, look for future commits to pull in
	    int t2 = 0; 
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
	    featureDemandPreUsageRate[t] = featRatio[t] * tiedBackGpdExecVol[t];
#endif

#if 0
	  // ----------------------------------------------------------	  
	  // ---- Method 2: only used when ratios change over time  ----
	  // This method was supposed to be an improvement over Method 1
	  // but it has problems
	  int t = 0; 
	  for (t=0; t<nPeriods; t++) {
	    float demandFromPeriodT2satisfiedinPeriodT = 0.0;	  
	    // lets loop over the previous periods (t2 form 0 to t-1) and pick up any 
	    // un-"allocated" gpdDemVol.
	    int t2 = 0; 
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
	}  // matches the else  regarding featratio's changing over time


#ifdef DEBUG_SET_PURE_OPTIONS
	dumpaVec("featureDemandPreUsageRate:", featureDemandPreUsageRate);
	dumpaVec("tiedBackGpdExecVol:", tiedBackGpdExecVol);
#endif

	// here's the amount of features supplied in first pass (pre usageRate)
	//     featuresShippedPreUsageRate = supplyVolCCfeaturePart + execVolCCfeaturePart;
	LgFrTimeVecFloat featuresShippedPreUsageRate((size_t) nPeriods, 0.0);

	float *  supplyVolCCfeaturePart;
	witGetPartSupplyVol(theWitRun_, fullWitFeaturePart, &supplyVolCCfeaturePart);
	float *  execVolCCfeaturePart;
	witGetOperationExecVol(theWitRun_, fullWitFeaturePart, &execVolCCfeaturePart);

	int t = 0; 
	for ( t=0; t<nPeriods; t++) {
	  featuresShippedPreUsageRate[t] = supplyVolCCfeaturePart[t] + execVolCCfeaturePart[t];
	}

	// here's the straight net of what you need for the second pass (pre usageRate)
	LgFrTimeVecFloat secondPassFeatureDemandVol((size_t) nPeriods, 0.0);
	for ( t=0; t<nPeriods; t++)
	  secondPassFeatureDemandVol[t] = featureDemandPreUsageRate[t] - featuresShippedPreUsageRate[t];


#ifdef DEBUG_SET_PURE_OPTIONS
	dumpaVec("supplyVolCCfeaturePart:", supplyVolCCfeaturePart);
	dumpaVec("execVolCCfeaturePart:", execVolCCfeaturePart);
	dumpaVec("featuresShippedPreUsageRate:", featuresShippedPreUsageRate);
	dumpaVec("secondPassFeatureDemandVol:", secondPassFeatureDemandVol);
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
#ifdef DEBUG_SET_PURE_OPTIONS
	std::cout << "... rolled negatives  forward of secondPassFeatureDemandVol\n";
	dumpaVec("secondPassFeatureDemandVol:", secondPassFeatureDemandVol);
#endif
	// ok, so what happens if you can't work it all off? ... punt!
	// 
	if (secondPassFeatureDemandVol[0] < 0) {
	  if (secondPassFeatureDemandVol[0] < -0.01) {
	    std::cout << "Internal SCE Code Alert: SCE00645AH.  Please notify the SCE Development Team.\n";


	    std::cout << "\n\n******\nGPD:" << fullWitGeoPlannerDemandOperation << std::endl;
	    std::cout << "CC Feature:" << fullWitFeaturePart << std::endl;
	    dumpaVec("gpdDemVol:", gpdDemVol);
	    dumpaVec("gpdExecVol:", gpdExecVol);
	    dumpaVec("featRatio:", featRatio);
	    dumpaVec("supplyVolCCfeaturePart:", supplyVolCCfeaturePart);
	    dumpaVec("execVolCCfeaturePart:", execVolCCfeaturePart);
	    dumpaVec("featuresShippedPreUsageRate:", featuresShippedPreUsageRate);
	    dumpaVec("secondPassFeatureDemandVol:", secondPassFeatureDemandVol);
	  }
	  secondPassFeatureDemandVol[0] = 0;
#ifdef DEBUG_SET_PURE_OPTIONS
	  std::cout << "... but had a negative in pd 0 so we flatten it.\n";
	  dumpaVec("secondPassFeatureDemandVol:", secondPassFeatureDemandVol);
#endif
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


#ifdef DEBUG_SET_PURE_OPTIONS
	  std::cout << "... just adjusted for spfd exceeding gpdExecVol.\n";
	  dumpaVec("secondPassFeatureDemandVol:", secondPassFeatureDemandVol);
#endif



	// now we apply the usageRate
	for ( t=0; t<nPeriods; t++)
	  secondPassFeatureDemandVol[t] *= usageRate;


#ifdef DEBUG_SET_PURE_OPTIONS
	if (usageRate < 1 || usageRate>1) {
	  std::cout << "... just applied non-unit usageRate\n";
	  dumpaVec("secondPassFeatureDemandVol:", secondPassFeatureDemandVol);
	}
#endif


#ifdef DEBUG_SET_PURE_OPTIONS
	// and now we're done debugging
	std::cout << "****** \n\n";
#endif


	    
	    
	// YEAH, we've computed the second pass demand vol!!
	// set it in appData
	LgFrSceDemandAppData * dmdAppDataPtr;
	witGetDemandAppData( theWitRun_, fullWitFeaturePart, featureDemandName.c_str(),
			     (void **) &dmdAppDataPtr);
	if ( dmdAppDataPtr == 0 ) {
	  dmdAppDataPtr = new LgFrSceDemandAppData ((size_t) nPeriods, 1.0);
	  assert ( dmdAppDataPtr != 0 );
	  witSetDemandAppData( theWitRun_, fullWitFeaturePart, featureDemandName.c_str(),  
			       (void *) dmdAppDataPtr );
	}
	dmdAppDataPtr->allocVol(secondPassFeatureDemandVol);
	

	// FINISH_ME: Must set SpecialFeatureDemandVols prior to incHeurAlloc.  
	//   - migrate away from using demandVol at all ... use allocVol
	//   - check back in code to see if we can set it.  We may need multiple 
	//     implementations in ipSce.C based on engine!!!

	witFree(gpdDemVol);
	witFree(gpdExecVol);
	witFree(supplyVolCCfeaturePart);
	witFree(execVolCCfeaturePart);
	witFree(fullWitFeaturePart);
	continue;
      }   //  end  ...        if (mpHelper->isPartSpecialCustChoiceFeature())  ...
      


      // + --------------------------------
      // + If its a STANDALONE FEATURE
      // + --------------------------------
      // Just for the sake of it ....
      assert (mpHelper_->isPartSpecialStandaloneFeature(theWitRun_, fullWitFeaturePart, 
						       gg, pp, ppdf, fp));
      


      // if there are substitutes, then the child might be a pure option
      // note that one must look at the NullSub part to determine which of the three
      // type of STANDALONE FEATUREs it is ...
      int nSubs;
      witGetBomEntryNSubsBomEntries(theWitRun_, fullWitGeoPlannerDemandOperation.c_str(), j, &nSubs);

      if (nSubs > 0)   {

	assert (nSubs == 1);

	char * nullSub;
	witGetSubsBomEntryConsumedPart(theWitRun_, fullWitGeoPlannerDemandOperation.c_str(), j, 0, &nullSub);
	
	// the nullSub will be one of:
	//    globalNullSub (Purely optional standalone feature)
	//    globalMaxWithoutNullsub (purely mandatory standalone feature)
	//    maxWithoutNullsub  (standalone feature with 0<maxWithout <1)
	
	// -------------------------------------------------------------
	// if its a Mandatory Feature (global maxwithout) ... Skip It
	// -------------------------------------------------------------
	// skip it because there  never be any pureOption demandVol for mandatory features
	std::string pppdf;
	if (mpHelper_->isPartSpecialGlobalMaxWithoutNullSubstitute(theWitRun_, nullSub, pppdf)) {
	  witFree(nullSub);
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
	std::string featureDemandName = mpHelper_->pureOptionBuildDemandName(fullWitGeoPlannerDemandOperation );
	

	
	// --------------------------------------------------------------------------------
	//
	// This is the place where the rubber hits the road in feature modeling.
	// This is where the demand for features is calculated.
	// Implement you favorite algorithm for calculating the amount of a feature
	// to be built in the second pass of the heuristic HERE.
	//
	
	//
	// This code says that amount of the null substituted used times the feature ratio
	// (ie. subsVol * usageRate) is the amt of pure option to try and build
	//
	// The feature ratio used is the feature ratio in effect during the box
	// ship period (which may have no similarity with the feature rate
	// that was in effect in the period during which the demand originated from).
	  
	// *****************************************************************************
	// The amount of the produced part due to this substitute is given by subVol.

	LgFrTimeVecFloat pureOptBldDemVol((size_t) nPeriods, 0.0);
	
	// get the subVol of the nullSub
	float * subVol;
	witGetSubsBomEntrySubVol(theWitRun_, fullWitGeoPlannerDemandOperation.c_str(), j, 0, &subVol);
	

	
	if ( featureRatioPegMethod == "pegToMachineShipPeriod" ) {
	  // FINISH_ME: check this out!!!  
	  //     .... The default if pegToMachineShipPeriod but esg wants pegToMachineDemandPeriod
	  
	  
	  // But, we need the usageRatio...
	  // FINISH_ME: 2001/07/19: this now needs to be consRate!!!
	  float usageRatio;
	  witGetBomEntryUsageRate(theWitRun_, fullWitGeoPlannerDemandOperation.c_str(), j, &usageRatio);
	  
	  // this was originally commented out with the following FINISH_ME: check it out, please.
	  //	      int * impactPeriod;
	  //	      witGetBomEntryImpactPeriod(theWitRun_, fullWitGeoPlannerDemandOperation.c_str(), j, &impactPeriod);
	  // FINISH_ME: this is not right yet
	  
	  // ...to calculate the amount of the substitute that was consumed
	  // which is equal to the amount of the pure option we want to try and build.
	  
	  int t = 0; 
	  for (t=0; t<nPeriods; t++) {
	    if (subVol[t] > 0.0001) {
	      //		  assert(impactPeriod[t] >= 0);
	      //		  pureOptBldDemVol[impactPeriod[t]] += (usageRatio * subVol[t]);
	      pureOptBldDemVol[t] = usageRatio * subVol[t];
	    }
	    //	      witFree(impactPeriod);
	  }
	}


	assert (pureOptBldDemVol.min() >= 0.0);	
	
	// YEAH, we've computed the second pass demand vol!!
	// set it in appData
	LgFrSceDemandAppData * dmdAppDataPtr;
	witGetDemandAppData( theWitRun_, fullWitFeaturePart, featureDemandName.c_str(),
			     (void **) &dmdAppDataPtr);
	if ( dmdAppDataPtr == 0 ) {
	  dmdAppDataPtr = new LgFrSceDemandAppData ((size_t) nPeriods, 1.0);
	  assert ( dmdAppDataPtr != 0 );
	  witSetDemandAppData( theWitRun_, fullWitFeaturePart, featureDemandName.c_str(),  
			       (void *) dmdAppDataPtr );
	}
	dmdAppDataPtr->allocVol(pureOptBldDemVol);


	// cleanup
	witFree(subVol);
	witFree(nullSub);
      }  // end    ...    if (nSubs > 0)   
      witFree(fullWitFeaturePart);
    }      // end  ...  for (int j=0; j<nBoms; j++)  
    witFree (operationList[o]);
  }    // end  ...    for ( int o=0; o<nOperations; o++ ) 
  witFree(operationList);
}




// ******************************************************************************
// setPureOptionBuildAllocVolsForMaxATP()
// --------------------------------------
// Calculate allocVols for  for features pass of  Heuristic Allocation Pass
//   - allocVol's are placed   in appData
//   - This routine does not create  allocLets.  
//   - Instead it sets up the allocVols in dmdAppData ... in preparation for allocLets
// - DEVELOP_HERE
// ********************************************************************************
void 
LgFrSceAllocationEngineMgr::setPureOptionBuildAllocVolsForMaxATP()
{

  std::cout << "     ... executing  setPureOptionBuildAllocVolsForMaxATP ..." << std::endl;  

  LgFrSceSetOfParameters & setOfParameters =  sceScenSP_->setOfParameters();    

  // get the number of periods
  int nPeriods;
  witGetNPeriods( theWitRun_, &nPeriods ) ;

  std::string featureRatioPegMethod = setOfParameters.getString("featureRatioPegMethod");

  // get the operation list
  int  nOperations ;      
  char ** operationList ;    
  witGetOperations( theWitRun_, &nOperations, &operationList );



  // Loop once for each operation.
  int o = 0; 
  for ( o=0; o<nOperations; o++ ) {
    std::string fullWitGeoPlannerDemandOperation(operationList[o]);
    std::string plannerTopLevelPart;
    std::string geo;

    // Start with  GPD parts ...  then look for  specialFeatureParts as children. 
    if (! mpHelper_->isOperationSpecialGeoPlannerDemand(theWitRun_, fullWitGeoPlannerDemandOperation,
							plannerTopLevelPart, geo))  {
      witFree (operationList[o]);
      continue;
    }
    int nBoms;
    witGetOperationNBomEntries( theWitRun_, operationList[o], &nBoms );
    // Now loop through the children of the GPD operation
    int j = 0; 
    for (j=0; j<nBoms; j++)  {

      // get the name of the child part, 
      char * fullWitFeaturePart;
      witGetBomEntryConsumedPart(theWitRun_, fullWitGeoPlannerDemandOperation.c_str(), j, &fullWitFeaturePart);

      // We are only interested in special Feature Parts which, at this time,
      // can either be custChoice or stdalone
      if (!  mpHelper_->isPartSpecialFeature(theWitRun_, fullWitFeaturePart)) {
	witFree(fullWitFeaturePart);
	continue;
      }
      std::string gg, pp, ppdf, fp;

      // + --------------------------------
      // + If its a CUSTOMER_CHOICE_FEATURE
      // + --------------------------------
      if (mpHelper_->isPartSpecialCustChoiceFeature(theWitRun_, fullWitFeaturePart, gg, pp, ppdf, fp)) {

	std::string featureDemandName = mpHelper_->custChoiceFeatureDemandName(fullWitGeoPlannerDemandOperation );
	    
	float * gpdDemVol;
	witGetDemandDemandVol( theWitRun_, fullWitGeoPlannerDemandOperation.c_str(), geo.c_str(), &gpdDemVol);
	float * gpdExecVol;
	witGetOperationExecVol( theWitRun_, fullWitGeoPlannerDemandOperation.c_str(), &gpdExecVol);
	float * consRate;
	witGetBomEntryConsRate(theWitRun_, fullWitGeoPlannerDemandOperation.c_str(), j, &consRate);	      
	// featRatio is stored in GPD->SFP bom's appData
	LgFrTimeVecFloat featRatio = mpHelper_->custChoiceFeatureRatio(theWitRun_, fullWitFeaturePart);

	// total features demanded (boxDemand * featratio)
	LgFrTimeVecFloat featureDemand((size_t) nPeriods, 0.0);
	
	// total features committed 
	LgFrTimeVecFloat featureCommit((size_t) nPeriods, 0.0);
	// cum (featureDemand - featureCommit)
	LgFrTimeVecFloat cumDelta((size_t) nPeriods, 0.0);
	// second pass demand vol
	LgFrTimeVecFloat featureAllocVol((size_t) nPeriods, 0.0);


	float *  supplyVolCCfeaturePart;
	witGetPartSupplyVol(theWitRun_, fullWitFeaturePart, &supplyVolCCfeaturePart);
	float *  execVolCCfeaturePart;
	witGetOperationExecVol(theWitRun_, fullWitFeaturePart, &execVolCCfeaturePart);


	float prevPeriodDelta=0.0;
	int t = 0; 
	for (t=0; t<nPeriods; t++) {
	  featureDemand[t] = gpdDemVol[t] * featRatio[t] * consRate[t];
	  featureCommit[t] = supplyVolCCfeaturePart[t] + execVolCCfeaturePart[t];
	  cumDelta[t] = prevPeriodDelta + featureDemand[t] - featureCommit[t];
	  prevPeriodDelta = cumDelta[t];
	}
	
	float shortfall = cumDelta[nPeriods - 1];
	for (t=nPeriods-1; t>0; t--) {
	  if (shortfall < .00001)
	    break;
	  if (shortfall >= cumDelta[t-1])  {
	    featureAllocVol[t] = shortfall - cumDelta[t-1];
	    shortfall -= featureAllocVol[t];
	  }
	  else {
	    featureAllocVol[t] = 0;
	  }
	}
	featureAllocVol[0] = shortfall;

	
	// YEAH, we've computed the second pass demand vol!!
	// set it in appData
	LgFrSceDemandAppData * dmdAppDataPtr;
	witGetDemandAppData( theWitRun_, fullWitFeaturePart, featureDemandName.c_str(),
			     (void **) &dmdAppDataPtr);
	if ( dmdAppDataPtr == 0 ) {
	  dmdAppDataPtr = new LgFrSceDemandAppData ((size_t) nPeriods, 1.0);
	  assert ( dmdAppDataPtr != 0 );
	  witSetDemandAppData( theWitRun_, fullWitFeaturePart, featureDemandName.c_str(),  
			       (void *) dmdAppDataPtr );
	}
	dmdAppDataPtr->allocVol(featureAllocVol);
	

#ifdef DEBUG_CUSTOMER_CHOICE_MAX_ATP
	std::cout << "\n***DEBUG: MaxATP, CustomerChoice \nGPD:" << fullWitGeoPlannerDemandOperation << ",   ";
	std::cout << "CC Feature:" << fullWitFeaturePart << std::endl;
	dumpaVec("gpdDemVol:", gpdDemVol);
	dumpaVec("gpdExecVol:", gpdExecVol);
	dumpaVec("featRatio:", featRatio);
	dumpaVec("supplyVolCCfeaturePart:", supplyVolCCfeaturePart);
	dumpaVec("execVolCCfeaturePart:", execVolCCfeaturePart);
	dumpaVec("featureDemand:", featureDemand);
	dumpaVec("featureCommit:", featureCommit);
	dumpaVec("cumDelta:", cumDelta);
	dumpaVec("featureAllocVol:", featureAllocVol);
#endif

	witFree(gpdDemVol);
	witFree(gpdExecVol);
	witFree(supplyVolCCfeaturePart);
	witFree(execVolCCfeaturePart);
	witFree(fullWitFeaturePart);
	continue;
      }   //  end  ...        if (mpHelper->isPartSpecialCustChoiceFeature())  ...
      


      // At this point we know it's a STANDALONE FEATURE
      // look at the substitute(s) of the Special Standalone Feature 
      // the nullSub will be one of:
      //    globalNullSub           ==> optional            maxWithout = 1
      //    globalMaxWithoutNullsub ==> mandatory           maxWithout = 0
      //    maxWithoutNullsub       ==> tweener         0 < maxWithout < 1
      assert (mpHelper_->isPartSpecialStandaloneFeature(theWitRun_, fullWitFeaturePart, 
						       gg, pp, ppdf, fp));
      int nSubs;
      witGetBomEntryNSubsBomEntries(theWitRun_, fullWitGeoPlannerDemandOperation.c_str(), j, &nSubs);
      if (nSubs > 0)   {
	assert (nSubs == 1);
	char * nullSub;
	witGetSubsBomEntryConsumedPart(theWitRun_, fullWitGeoPlannerDemandOperation.c_str(), j, 0, &nullSub);
	std::string pppdf;

	// + --------------------------------
	// + If its a MANDATORY_FEATURE
	// + --------------------------------
	if (mpHelper_->isPartSpecialGlobalMaxWithoutNullSubstitute(theWitRun_, nullSub, pppdf)) {

	  std::string featureDemandName = mpHelper_->pureOptionBuildDemandName(fullWitGeoPlannerDemandOperation );	  
	  
	  float * gpdDemVol;
	  witGetDemandDemandVol( theWitRun_, fullWitGeoPlannerDemandOperation.c_str(), geo.c_str(), &gpdDemVol);
	  float * gpdExecVol;
	  witGetOperationExecVol( theWitRun_, fullWitGeoPlannerDemandOperation.c_str(), &gpdExecVol);

	  float featRatio;
	  witGetBomEntryUsageRate(theWitRun_, fullWitGeoPlannerDemandOperation.c_str(), j, &featRatio);

	  // total features demanded (unsquared)
	  LgFrTimeVecFloat featureDemand((size_t) nPeriods, 0.0);

	  // total features committed (squared)
	  LgFrTimeVecFloat featureCommit((size_t) nPeriods, 0.0);
	  
	  LgFrTimeVecFloat cumDelta((size_t) nPeriods, 0.0);
	  LgFrTimeVecFloat featureAllocVol((size_t) nPeriods, 0.0);

	  int t = 0; 
	  float prevPeriodDelta=0.0;
	  for (t=0; t<nPeriods; t++) {
	    featureDemand[t] = gpdDemVol[t] * featRatio;
	    featureCommit[t] = gpdExecVol[t] * featRatio;
            cumDelta[t] = prevPeriodDelta + featureDemand[t] - featureCommit[t];
	    prevPeriodDelta = cumDelta[t];
	  }

	  float shortfall = cumDelta[nPeriods - 1];
	  for (t=nPeriods-1; t>0; t--) {
	    if (shortfall < .00001)
	      break;
	    if (shortfall >= cumDelta[t-1])  {
	      featureAllocVol[t] = shortfall - cumDelta[t-1];
	      shortfall -= featureAllocVol[t];
	    }
	    else {
	      featureAllocVol[t] = 0;
	    }
	  }
	  featureAllocVol[0] = shortfall;


	  
	  // featureAllocVol is the second pass demand vol
	  // set it in appData
	  LgFrSceDemandAppData * dmdAppDataPtr;
	  witGetDemandAppData( theWitRun_, fullWitFeaturePart, featureDemandName.c_str(),
			       (void **) &dmdAppDataPtr);
	  if ( dmdAppDataPtr == 0 ) {
	    dmdAppDataPtr = new LgFrSceDemandAppData ((size_t) nPeriods, 1.0);
	    assert ( dmdAppDataPtr != 0 );
	    witSetDemandAppData( theWitRun_, fullWitFeaturePart, featureDemandName.c_str(),  
				 (void *) dmdAppDataPtr );
	  }
	  dmdAppDataPtr->allocVol(featureAllocVol);



#ifdef DEBUG_MANDATORY_MAX_ATP
	  std::cout << "\n***DEBUG: MaxATP, Mandatory \nGPD:" << fullWitGeoPlannerDemandOperation << ",  ";
	  std::cout << "Mandatory Feature:" << fullWitFeaturePart << std::endl;
	  dumpaVec("gpdDemVol:", gpdDemVol);
	  dumpaVec("gpdExecVol:", gpdExecVol);
	  std::cout << "featRatio: " << featRatio << "\n";
	  dumpaVec("featureDemand:", featureDemand);
	  dumpaVec("featureCommit:", featureCommit);
	  dumpaVec("cumDelta:", cumDelta);
	  dumpaVec("featureAllocVol:", featureAllocVol);
#endif

	  witFree(gpdDemVol);
	  witFree(gpdExecVol);
	}
	

	else   {
	  // -----------------------------------------------------------------------
	  // if its a PURE Option or a MWO tweener
	  // -----------------------------------------------------------------------
	  // --------------------------------------------------------------------------------
	  std::string featureDemandName = mpHelper_->pureOptionBuildDemandName(fullWitGeoPlannerDemandOperation );
	  
	  LgFrTimeVecFloat pureOptBldDemVol((size_t) nPeriods, 0.0);


	  float * gpdDemVol;
	  witGetDemandDemandVol( theWitRun_, fullWitGeoPlannerDemandOperation.c_str(), geo.c_str(), &gpdDemVol);
	  float * gpdExecVol;
	  witGetOperationExecVol( theWitRun_, fullWitGeoPlannerDemandOperation.c_str(), &gpdExecVol);
	  float * consRate;
	  witGetBomEntryConsRate(theWitRun_, fullWitGeoPlannerDemandOperation.c_str(), j, &consRate);	      
	  // featRatio is stored in GPD->SFP bom's appData
	  //	  LgFrTimeVecFloat featRatio = mpHelper_->custChoiceFeatureRatio(theWitRun_, fullWitFeaturePart);

	  // total features demanded (boxDemand * featratio)
	  LgFrTimeVecFloat featureDemand((size_t) nPeriods, 0.0);
	
	  // total features committed 
	  LgFrTimeVecFloat featureCommit((size_t) nPeriods, 0.0);
	  // cum (featureDemand - featureCommit)
	  LgFrTimeVecFloat cumDelta((size_t) nPeriods, 0.0);
	  // second pass demand vol
	  LgFrTimeVecFloat featureAllocVol((size_t) nPeriods, 0.0);


	  float *  supplyVolSpecialFeaturePart;
	  witGetPartSupplyVol(theWitRun_, fullWitFeaturePart, &supplyVolSpecialFeaturePart);
	  float *  execVolSpecialFeaturePart;
	  witGetOperationExecVol(theWitRun_, fullWitFeaturePart, &execVolSpecialFeaturePart);


	  float prevPeriodDelta=0.0;
	  int t = 0; 
	  for (t=0; t<nPeriods; t++) {
	    featureDemand[t] = gpdDemVol[t] * consRate[t];
	    featureCommit[t] = supplyVolSpecialFeaturePart[t] + execVolSpecialFeaturePart[t];
	    cumDelta[t] = prevPeriodDelta + featureDemand[t] - featureCommit[t];
	    prevPeriodDelta = cumDelta[t];
	  }
	  
	  float shortfall = cumDelta[nPeriods - 1];
	  for (t=nPeriods-1; t>0; t--) {
	    if (shortfall < .00001)
	      break;
	    if (shortfall >= cumDelta[t-1])  {
	      featureAllocVol[t] = shortfall - cumDelta[t-1];
	      shortfall -= featureAllocVol[t];
	    }
	    else {
	      featureAllocVol[t] = 0;
	    }
	  }
	  featureAllocVol[0] = shortfall;


#ifdef DEBUG_PURE_OPTION_MAX_ATP
	  std::cout << "\n***DEBUG: MaxATP, PureOption \nGPD:" << fullWitGeoPlannerDemandOperation << ",   ";
	  std::cout << "CC Feature:" << fullWitFeaturePart << std::endl;
	  dumpaVec("gpdDemVol:", gpdDemVol);
	  dumpaVec("gpdExecVol:", gpdExecVol);
	  //	dumpaVec("featRatio:", usageRate);
	  std::cout << "featRatio:" << usageRate << std::endl;
	  dumpaVec("supplyVolSpecialFeaturePart:", supplyVolSpecialFeaturePart);
	  dumpaVec("execVolSpecialFeaturePart:", execVolSpecialFeaturePart);
	  dumpaVec("featureDemand:", featureDemand);
	  dumpaVec("featureCommit:", featureCommit);
	  dumpaVec("cumDelta:", cumDelta);
	  dumpaVec("featureAllocVol:", featureAllocVol);
#endif

	  
	  
	  
	  
	  
	  // YEAH, we've computed the second pass demand vol!!
	  // set it in appData
	  LgFrSceDemandAppData * dmdAppDataPtr;
	  witGetDemandAppData( theWitRun_, fullWitFeaturePart, featureDemandName.c_str(),
			       (void **) &dmdAppDataPtr);
	  if ( dmdAppDataPtr == 0 ) {
	    dmdAppDataPtr = new LgFrSceDemandAppData ((size_t) nPeriods, 1.0);
	    assert ( dmdAppDataPtr != 0 );
	    witSetDemandAppData( theWitRun_, fullWitFeaturePart, featureDemandName.c_str(),  
				 (void *) dmdAppDataPtr );
	  }
	  dmdAppDataPtr->allocVol(featureAllocVol);
	  
	  
	  // cleanup
	  witFree(gpdDemVol);
	  witFree(gpdExecVol);
	  witFree(consRate);
	  witFree(supplyVolSpecialFeaturePart);
	  witFree(execVolSpecialFeaturePart);
	}  // end (else it's a  NULL SUB for pure options or MWO)
	witFree(nullSub);
      }  // end    ...    if (nSubs > 0)   
      witFree(fullWitFeaturePart);
    }      // end  ...  for (int j=0; j<nBoms; j++)  
    witFree (operationList[o]);
  }    // end  ...    for ( int o=0; o<nOperations; o++ ) 
  witFree(operationList);
}





// ******************************************************************************
// initializeAllocDataFeaturePass()

// ------------------------------
// Set up allocLet List for features pass of  Heuristic Allocation Pass
// - FINISH_ME: Put allocVol in appData
// - DEVELOP_HERE
//       - Create an allocLet for each positive special features pass
//       - QSORT the allocLetList
//       - Note: the calling program is responsible for deleting any demand app data 
//                ptrs that are "new-ed" in here.
// ********************************************************************************
void
LgFrSceAllocationEngineMgr::initializeAllocDataFeaturePass()
{
  std::cout << "     ... executing initializeAllocDataFeaturePass ..." << std::endl;  

  int   * priority;
  int nPeriods;
  
  witGetNPeriods(theWitRun_, &nPeriods);
  int nParts;
  char  **partList;
  witGetParts(theWitRun_, &nParts, &partList);
  
  aPointer_ = 0;
  listMax_ = 0;
  
  // first count how many allocLets we need ==> listMax.
  // NOTE: we only count non-zero minDemandVols
  int i = 0; 
  for(i=0; i<nParts; i++) {

    char * fullWitFeaturePart = partList[i];

    // We are only interested in special Feature Parts which, at this time,
    // can either be custChoice or stdalone
    if (!  mpHelper_->isPartSpecialFeature(theWitRun_, fullWitFeaturePart)) {
      continue;
    }

    int nDemands;
    char ** dList;
    witGetPartDemands(theWitRun_, fullWitFeaturePart, &nDemands, &dList);
    LgFrSceDemandAppData * dmdAppDataPtr;
    witGetDemandAppData( theWitRun_, fullWitFeaturePart, dList[0],
			 (void **) &dmdAppDataPtr);
    if ( dmdAppDataPtr == 0 ) {
      witFree(dList[0]);
      witFree(dList);
      continue;
    }

    LgFrTimeVecFloat allocVol(dmdAppDataPtr->allocVol());
    int t = 0; 
    for (t=0; t<nPeriods; t++) {
      if (allocVol[t] > 0.0) {
         listMax_++;
      }
    }
    
    witFree(dList[0]);
    witFree(dList);
  }

  // status logging only 
  std::cout << "     ... allocLetList size:" << listMax_ << std::endl;

  // Now allocate the vector of allocLet Ptrs
  vecAllocLetList_ = new LgFrSceAllocLet [listMax_];
  // and the vector of allocLets
  allocLetList_ = new LgFrSceAllocLet * [listMax_];
  
  LgFrSceAllocLet * dependeeAllocLet = 0;

  // now load each allocLet
  for( i=0; i<nParts; i++){
    
    char * fullWitFeaturePart = partList[i];
    
    // We are only interested in special Feature Parts which, at this time,
    // can either be custChoice or stdalone
    if (!  mpHelper_->isPartSpecialFeature(theWitRun_, fullWitFeaturePart)) {
      witFree(partList[i]);
      continue;
    }
    
    // reset the dependee ptr to 0
    dependeeAllocLet = 0;
    
    int nDemands;
    char ** dList;
    witGetPartDemands(theWitRun_, fullWitFeaturePart, &nDemands, &dList);
    
    LgFrSceDemandAppData * dmdAppDataPtr;
    witGetDemandAppData( theWitRun_, fullWitFeaturePart, dList[0],
			 (void **) &dmdAppDataPtr);
    if ( dmdAppDataPtr == 0 ) {
      witFree(dList[0]);
      witFree(dList);
      witFree(partList[i]);
      continue;
    }
    
    LgFrTimeVecFloat allocVol(dmdAppDataPtr->allocVol());
    witGetDemandPriority(theWitRun_, fullWitFeaturePart, dList[0], &priority);
    
    
    int t = 0; 
    for (t=0; t<nPeriods; t++) {
      if (allocVol[t] > 0.0) {
	LgFrSceAllocLet & myAllocLet = vecAllocLetList_[aPointer_];
	allocLetList_[aPointer_] = &myAllocLet;
	myAllocLet.partName(fullWitFeaturePart);
	myAllocLet.demandName(dList[0]);
	myAllocLet.priority(priority[t]);
	myAllocLet.qtyDemand(allocVol[t]);
	myAllocLet.demandPd(t);
	myAllocLet.shipPd(t);
	myAllocLet.allocated(0);
	
	if (dependeeAllocLet != 0) {
	  // if there's a dependee, then he's waiting to know the current one
	  dependeeAllocLet->backlogDependent(&myAllocLet);
	}
	// remember this allocLet
	dependeeAllocLet = &myAllocLet;
	aPointer_++;
      }
    }
    
    witFree(dList[0]);
    witFree(dList);
    witFree(priority);
    witFree(partList[i]);
  }
  witFree(partList);
  
  assert(aPointer_ == listMax_);
  aPointer_ = 0;
  
  qsort(allocLetList_, 
        listMax_, 
	sizeof(LgFrSceAllocLet*), 
        sortStrategy_->compareVoidFunc());

}








 








 

// Part of an alternative method to calculate the (second pass) demand vols for features.
// This method pegs the shipVolWithoutFeature to the liveDemandVol, and using the
// featureRatio in the live demand period, calculates the demand vol of the feature in the second pass.
 
void
LgFrSceAllocationEngineMgr::calculateOneFeaturesBuildDemandVol ( int nPeriods,
                                                           float * liveDemandVol,
                                                           float * shipVolWithoutFeature,
                                                           float * featureRatio,
                                                           float * featureBuildDemandVols )
{
  // initialize the featureBuildDemandVols to zero
  int t = 0; 
  for (t=0; t<nPeriods; t++)
    featureBuildDemandVols[t] = 0.0;

 // need two counters.
 // td is the period for the "d"emand and ts is the period for the "ship"
 int td = 0;
 
 int ts = 0; 
 for ( ts=0; ts<nPeriods; ts++) {
   // if nothing to allocate then go on to the next shipVol bucket
   if ( shipVolWithoutFeature[ts] < AENGMGR_FLT_EPS )
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
LgFrSceAllocationEngineMgr::calculateLostDemandUnderBacklogYield ( int nPeriods,
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
  int t = 0; 
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
LgFrSceAllocationEngineMgr::calculateLostDemandUnderShipLateLimit ( int nPeriods,
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
  int t = 0; 
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
  
  free (loss);

  // backlog calculation
  cumShip = shipVol[0];
  cumDemand = demandVol[0];
  backlog[0]=cumDemand-cumShip;
  for (t=1; t<nPeriods; t++) {
    cumShip += shipVol[t];
    cumDemand += demandVol[t];
    backlog[t] = cumDemand - cumShip - cumLostVec[t-1];
  }

  free (cumLostVec);
  
}
  

// copy constructor 
LgFrSceAllocationEngineMgr::LgFrSceAllocationEngineMgr(const LgFrSceAllocationEngineMgr& source)
  :   
  sceScenSP_ (source.sceScenSP_),
  theWitRun_ (source.theWitRun_),
  mpHelper_(source.mpHelper_),
  usageName_(source.usageName_)
{
  // nothing to do
}

// assignment operator
LgFrSceAllocationEngineMgr&
LgFrSceAllocationEngineMgr::operator=(const LgFrSceAllocationEngineMgr& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    sceScenSP_ = rhs.sceScenSP_;
    theWitRun_ = rhs.theWitRun_;
    mpHelper_ = rhs.mpHelper_;
    usageName_ = rhs.usageName_;
  }
  return *this;
}

// destructor
LgFrSceAllocationEngineMgr::~LgFrSceAllocationEngineMgr()
{

}


 
 
// ---------------------------------------------------------------------------
int
LgFrSceAllocationEngineMgr::sortID(std::string & sortStrategyString)
{
  if (0 == sce::compareStringsCaseIns( sortStrategyString, "truePriority" ))
    return LgFrSceSortId_PDS;
  else if (0 == sce::compareStringsCaseIns( sortStrategyString, "PDS" ))
    return LgFrSceSortId_PDS;
  else if (0 == sce::compareStringsCaseIns( sortStrategyString, "PSD" ))
    return LgFrSceSortId_PSD;
  else if (0 == sce::compareStringsCaseIns( sortStrategyString, "SPD" ))
    return LgFrSceSortId_SPD;
  else if (0 == sce::compareStringsCaseIns( sortStrategyString, "SDP" ))
    return LgFrSceSortId_SDP;
  else if (0 == sce::compareStringsCaseIns( sortStrategyString, "DSP" ))
    return LgFrSceSortId_DSP;
  else if (0 == sce::compareStringsCaseIns( sortStrategyString, "DPS" ))
    return LgFrSceSortId_DPS;

  else {
    std::cerr << "Error:  illegal sortStrrategy parameter ==> "
	 << sortStrategyString << "\n Terminating SCE\n";
    exit(8);
  }
  return 0;
}




// ---------------------------------------------------------------------------
int
LgFrSceAllocationEngineMgr::sortID()
{
  
    return sortStrategy_->sortId();
}




// Set the sort strategy
void 
LgFrSceAllocationEngineMgr::sortStrategy(LgFrSceSortStrategy * sStrat)
  {
    sortStrategy_ = sStrat;
  }

// Set the bklg strategy
// NOTE: this will eventually become an object
void 
LgFrSceAllocationEngineMgr::backlogStrategy(LgFrSceBacklogStrategy * bStrat)
  {
    bklgStrategy_ = bStrat;
  }


  // set flag to print trace
void   
LgFrSceAllocationEngineMgr::trace(int traceFlag)
  {
    traceFlag_ = traceFlag;
  }

  // set filename for trace
void   
LgFrSceAllocationEngineMgr::traceFileName(std::string & traceFile)
  {
    traceFileName_ = traceFile;
  }


// set usageName variable
void
LgFrSceAllocationEngineMgr::setUsageName(const std::string & usageName)
{
  usageName_ = usageName;
}

// Get usageName variable pointer
std::string 
LgFrSceAllocationEngineMgr::getUsageName()
{
  return usageName_;
}




// default constructor
LgFrSceAllocationEngineMgr::LgFrSceAllocationEngineMgr()
  :   
  sceScenSP_(),
  theWitRun_ (0),
  mpHelper_(0),
  usageName_(""),
  vecAllocLetList_(0),
  allocLetList_(0),
  sortStrategy_(0),
  bklgStrategy_(0),
  traceFlag_(0),
  traceFileName_(""),
  listMax_(0),
  aPointer_(0),
  outFile_()
{
    // all the work is done in initializer
}


// fairly general constructor:
LgFrSceAllocationEngineMgr::LgFrSceAllocationEngineMgr(
						       LgFrScenarioForSceSmartPointer sceScenSP,
						       WitRun * const theWitRun, 
						       LgFrMultiPlantHelper * mpHelper,
						       std::string & usageName)
  :   
  sceScenSP_(sceScenSP),
  theWitRun_ (theWitRun),
  mpHelper_(mpHelper),
  usageName_(usageName),
  vecAllocLetList_(0),
  allocLetList_(0),
  sortStrategy_(0),
  bklgStrategy_(0),
  traceFlag_(0),
  traceFileName_(""),
  listMax_(0),
  aPointer_(0),
  outFile_()
{
  LgFrSceSetOfParameters & setOfParameters =  sceScenSP_->setOfParameters();    

  if (usageName == "explosion") {
    traceFlag_ = setOfParameters.getBool("printHeurAllocExplodeTrace");
    traceFileName_ = setOfParameters.getString("heurAllocExplodeTraceFileName");
  }
  else {
    traceFlag_ = setOfParameters.getBool("printHeurAllocTrace");
    traceFileName_ = setOfParameters.getString("heurAllocTraceFileName");
  }
  
  
  
}















#ifdef NDEBUG
#undef NDEBUG
#endif



// self-test : can I self test ??
void
LgFrSceAllocationEngineMgr::test()
{
  // do-nothing test  
}

 










