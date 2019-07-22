
// RW2STL - inserted:
#include <string>
#include <ostream>
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL - inserted includes.
// file: oEngMgr.C
// author: robin and tom

#include <stdlib.h>

#include <mcl/src/mcl.h>
#include <scenario/src/machdep.h>
#include <sce/src/sceDefin.h>
#include <sce/src/oEngMgr.h>
#include <wit/src/wit.h>
#include <sce/src/ipSce.h>
#include <sce/src/scenSce.h>
#include <sce/src/mPlantH.h>
#include <scenario/src/calendar.h>
#include <sce/src/sceCrtLs.h>
#include <sce/src/sceParam.h>
#include <sce/src/dmApData.h>

#include <assert.h>


#ifndef ELIMINATE_OLD_MAIN  


// default constructor:
LgFrSceOptEngineMgr::LgFrSceOptEngineMgr()
  :   theWitRun_ (0),
      theCal_(0),
      mpHelper_(0),
      sceScenSP_(0)
{
    // all the work is done in initializer
}


// fairly general constructor:
LgFrSceOptEngineMgr::LgFrSceOptEngineMgr(WitRun * const theWitRun, 
                                         LgFrCalendar * theCal,
                                         LgFrMultiPlantHelper * mpHelper,
                                         LgFrScenarioForSceSmartPointer sceScenSP )

  :   theWitRun_ (theWitRun),
      theCal_(theCal),
      mpHelper_(mpHelper),
      sceScenSP_ (sceScenSP)
{
    // all the work is done in initializer
}

// run the optimization, using the hueristic solution as a starting solution
void
LgFrSceOptEngineMgr::implode(LgFrSceCritList & theCritList)
{


#ifndef SCE_OPT_IMPLODE
  (*(sceScenSP_->getErrFacility()))("OptEngineNotSupported");

  std::cerr << "This version of SCE does not support the Optimization Engine.\n";
  exit(1);
#else
    
  LgFrSceSetOfParameters & setOfParameters =  sceScenSP_->setOfParameters();   
  assert(sceScenSP_->getEngine() == "optimization");

  bool optInitFromZero = setOfParameters.getBool("optInitFromZero");
  int printCriticalListFile = setOfParameters.getBool("printCriticalListFile");

  if (optInitFromZero) {
    std::cout << "Initializing Optimization with Zero solution ..." << std::endl;    
  }
  else {
    // =================================================
    // === Run the Heuristic engine to hot start  ======
    // =================================================
    std::cout << "Initializing optimization with heuristic implosion solution  ..." << std::endl;
    witHeurImplode(theWitRun_);
    if (printCriticalListFile)
      theCritList.merge( theWitRun_);
  }

   
  // ===================================
  // === Run the Optimization engine ===
  // ===================================
  
  std::cout << "Preparing for optimization engine ..." << std::endl;
    
  // change some stuff to make it right for optimization
  this->reStructureForOptimization();
  
  // tell wit to run off the existing ship and prod schedules
  // (ie, don't let it run the heuristic again)
#ifdef SCE_OPT_IMPLODE  
  witSetOptInitMethod(theWitRun_, WitSCHED_OPT_INIT_METHOD);
#endif  

  // sce 6.2 ... turn on the cplex, baby
  bool useCplexSolver = setOfParameters.getBool("useCplexSolver");
  //  if (useCplexSolver)
  //    witSetCplexSelected(theWitRun_, WitTRUE);
  //  else 
  //    witSetCoinSelected(theWitRun_, WitTRUE);



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

  std::string usingSolver;
  if (useCplexSolver) 
    usingSolver = "CPLEX solver";
  else
    usingSolver = "COIN solver";
  std::cout << "Executing (optimization) implosion engine using " << usingSolver << std::endl;
  witOptImplode(theWitRun_);
  std::cout << "......... (optimization) implosion engine completed" << std::endl;
  if (printCriticalListFile)
      theCritList.merge( theWitRun_);

  // ======================================================
  bool doPegging = setOfParameters.getBool("doPegging");    
  if (doPegging) {
    std::cout << "Starting Post Implosion Pegging ... " << std::endl;
    int nPeriods;
    int nParts, nDemands;  
    char ** partList, ** demList; 
    int i, j, t;            
    
    // develop the pip sequence
    WitRun * seqWitRun;
    witNewRun(&seqWitRun);
    sceScenSP_->setWitInformationalMessaging(seqWitRun, SCE_WIT_MESSAGING_OFF);          
    witInitialize(seqWitRun);
    witGetNPeriods(theWitRun_, &nPeriods);
    witSetNPeriods(seqWitRun, nPeriods);
    int equit;
    witGetEquitability(theWitRun_, &equit);
    witSetEquitability(seqWitRun, equit);
    witSetForcedMultiEq(seqWitRun, WitTRUE);

    witGetParts( theWitRun_, &nParts, &partList );

    // Loop thru every part, looking for special cases to fix
    for (i=0; i<nParts; i++)  { 
      witGetPartDemands(theWitRun_, partList[i], &nDemands, &demList);
      if (nDemands > 0) {
	witAddPartWithOperation(seqWitRun, partList[i]);
	for (j=0; j<nDemands; j++) {
	  witAddDemand(seqWitRun, partList[i], demList[j]);
	  float * shipVol;
	  witGetDemandShipVol(theWitRun_, partList[i], demList[j], &shipVol);
	  witSetDemandDemandVol(seqWitRun, partList[i], demList[j], shipVol);
	  int * priority;
	  witGetDemandPriority(theWitRun_, partList[i], demList[j], &priority);

	  // get it From AppData !!
	  LgFrTimeVecFloat pipPriority(nPeriods, 0.0);
	  int t;
	  for (t=0; t<nPeriods; t++)
	    pipPriority[t] = priority[t];
	  witFree(priority);

	  LgFrSceDemandAppData * demandAppDataPtr;
	  witGetDemandAppData(theWitRun_,
			      partList[i], 
			      demList[j],

			      (void **) &demandAppDataPtr);
	  //	  std::cout << "loading pip prioriity for demand: [" << partList[i] << "," << demList[j] << "] =";
	  // If the demandAppDataPtr has been set, see if the pipPriority has been set.
	  // If it hasn't (i.e. the backlogYield is 0), then set it. Otherwise, do nothing.
	  if (demandAppDataPtr != 0)  {
	    pipPriority = demandAppDataPtr->pipPriority();
	    //	    std::cout << (int) pipPriority[2] << " (found in app data)" << std::endl;
	  }
	  //	  else {
	  //	    std::cout << (int)  pipPriority[2] << " (NOT found in app data)" << std::endl;
	  //	  }
	    
	  witSetDemandPriority(seqWitRun, partList[i], demList[j], pipPriority.intData());
	  witFree(shipVol);
	  witFree(demList[j]);
	}
      }
      witFree(partList[i]);
      witFree(demList);
    }
    witFree(partList);

    witClearPipSeq(seqWitRun);
    witSetPipSeqFromHeur(seqWitRun, WitTRUE);

    witHeurImplode(seqWitRun);

    witPostprocess(seqWitRun);
    witBuildPip(seqWitRun);

#if 0
    sceScenSP_->setWitInformationalMessaging(seqWitRun, SCE_WIT_MESSAGING_ON);
    witWriteData(seqWitRun,"wit.data.seqWitRun");
    sceScenSP_->setWitInformationalMessaging(seqWitRun, SCE_WIT_MESSAGING_OFF);
#endif

    int seqLenList;
    char ** seqPartList;
    char ** seqDemandList;
    int * seqShipPerList;
    float * seqShipVolList;

    witGetPipSeq(seqWitRun, 
		 &seqLenList,
		 &seqPartList, 
		 &seqDemandList,
		 &seqShipPerList,
		 &seqShipVolList);

    for (i=0; i<seqLenList; i++) {
      witAppendToPipSeq(theWitRun_,
			seqPartList[i], 
			seqDemandList[i],
			seqShipPerList[i],
			seqShipVolList[i]);
      witFree(seqPartList[i]);
      witFree(seqDemandList[i]);
    }
    witFree(seqPartList);
    witFree(seqDemandList);
    witFree(seqShipPerList);
    witFree(seqShipVolList);
    
    witDeleteRun(seqWitRun);
    
    witBuildPip(theWitRun_);
    std::cout << "........WIT Post Implosion Pegging available  ... " << std::endl;
    sceScenSP_->loadPIPDataIntoAppData(theWitRun_);
    std::cout << "Pegging of Optimization Implosion solution is COMPLETE" << std::endl;
  }  
#endif
}
  
  

// copy constructor 
LgFrSceOptEngineMgr::LgFrSceOptEngineMgr(const LgFrSceOptEngineMgr& source)
  :   theWitRun_ (source.theWitRun_),
      theCal_(source.theCal_),
      mpHelper_(source.mpHelper_),
      sceScenSP_ (source.sceScenSP_)
  
{
  // nothing to do
}

// assignment operator
LgFrSceOptEngineMgr&
LgFrSceOptEngineMgr::operator=(const LgFrSceOptEngineMgr& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    theWitRun_ = rhs.theWitRun_;
    theCal_ = rhs.theCal_;
    mpHelper_ = rhs.mpHelper_;
    sceScenSP_ = rhs.sceScenSP_ ;
  }
  return *this;
}

// destructor
LgFrSceOptEngineMgr::~LgFrSceOptEngineMgr()
{
  // nothing to do, 
}


// the original model was setup so that it would work for
// mrp and heuristic.  To run the model in optimization mode,
// we must change the structure back.
void   
LgFrSceOptEngineMgr::reStructureForOptimization()
{
  int nPeriods;
  int nParts;  
  char ** partList; 
  int i, t;            

  witGetParts( theWitRun_, &nParts, &partList );
  nPeriods = theCal_->nPeriods();

  std::string plannerTopLevelPart;
  std::string geo;
  std::string bbCategory;
  std::string mfgOptionPart;


  // Loop thru every part, looking for special cases to fix
  for (i=0; i<nParts; i++)  { 
    std::string fullWitPartname(partList[i]);

    // This is what you do for special LT and GT capacity nodes:
    //      the supplyVol must be set back to zero
    if (mpHelper_->isPartSpecialLTbbCapacity(theWitRun_, fullWitPartname,
                                           plannerTopLevelPart, geo, bbCategory)
        ||
        mpHelper_->isPartSpecialGTbbCapacity(theWitRun_, fullWitPartname,
                                           plannerTopLevelPart, geo, bbCategory))   {

      float * zeroVec = new float[nPeriods];
      for (t=0; t<nPeriods; t++)
        zeroVec[t] = 0.0;

      witSetPartSupplyVol(theWitRun_, fullWitPartname.c_str(), zeroVec);
      delete [] zeroVec;
    }


#ifndef BUILDING_BLOCKS_VIA_MULTI_BOPS
    // This is what you do for special BB category parts:
    //     1) For every BOM entry to a special Option Dummy, the
    //        usage rate must be set to 0.0.
          
    if (mpHelper_->isPartSpecialBbCategory(theWitRun_, fullWitPartname, plannerTopLevelPart,
                                         geo, bbCategory)) {

      // loop through the bom entries 
      int nBom;
      witGetOperationNBomEntries(theWitRun_, fullWitPartname.c_str(), &nBom);

      std::string pp, g, bb, mfo;
      char * child;
      int j = 0; // Pulled out of the for below by RW2STL
      for (j=0; j<nBom; j++) {
        witGetBomEntryConsumedPart(theWitRun_, fullWitPartname.c_str(), j, &child);

        // if the child is SOD, then set usageRate to 0.0
        if (mpHelper_->isPartSpecialOptionDummy(theWitRun_, child, pp, g, bb, mfo)) {
          witSetBomEntryUsageRate(theWitRun_, fullWitPartname.c_str(), j, 0.0);              
        }

        witFree(child);
      }
    }
#endif
  }

#ifdef BUILDING_BLOCKS_VIA_MULTI_BOPS
  // 2.40: FINISH_ME: this implementation
#endif


  // Cleanup memory
  for ( i=0; i<nParts; i++) 
    witFree(partList[i]);
  witFree( partList );

}









#ifdef NDEBUG
#undef NDEBUG
#endif



// self-test : can I self test ??
void
LgFrSceOptEngineMgr::test()
{
  
 }

#endif
