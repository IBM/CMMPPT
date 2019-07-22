// file: aEngMgr.h
// author:  tom

#ifndef  LGFRSCEALLOCATIONENGINEMGR_H
#define  LGFRSCEALLOCATIONENGINEMGR_H

// RW2STL - inserted:
#include <string>
// End of RW2STL - inserted includes.

#include <stdlib.h>
#include <wit/src/wit.h>
//#include <fstream.h>

#include <sce/src/sceDefin.h>
#include <sce/src/scenSceP.h>



// forward references
class LgFrCalendar;
class LgFrSceCritList;
class LgFrInitialProblemForSce;
class LgFrMultiPlantHelper;
class LgFrSceAllocLet;
class LgFrSceBacklogStrategy;
class LgFrSceSortStrategy  ;

class LgFrSceAllocationEngineMgr {
  
public:

  // Set the sort strategy
  // NOTE: this will eventually become an object
  void sortStrategy(LgFrSceSortStrategy * sStrat);

  // Set the bklg strategy
  void backlogStrategy(LgFrSceBacklogStrategy *  bStrat);

  // set flag to print trace
  void   trace(int traceFlag);

  // set filename for trace
  void   traceFileName(std::string & traceFile);


  void
  initializeAllocDataWithMins();

  void
  initializeAllocDataAboveMins();

  void
  initializeAllocDataFeaturePass();

  void
  heurIncAlloc();

  void
  heurEqAlloc();

  void
  testForNonIncreasingPriorities();


  
  // do set up and run optimization
  void
  implode(LgFrSceCritList & theCritList);


  // converts a sortStrategy String into a sortID integer (see wafr/src/srtStrat.h)
  int
  sortID(std::string & sortStrategyString);


// ---------------------------------------------------------------------------
  // returns the sortID for the current sortStrategy
int
sortID();



  // This is member function mucks up the demands - if you want to get back your
  // original demands, you best have saved them somewhere safe before you invoke
  // this member function.
  //
  // Calculate amount of pure option we want to try and build
  // (code snipped from LgFrInitialProblemForSce::printStandAloneFeatureAnalysisReport, then
  //  unabashedly mutated).
  // Set the demand on the pure options nodes in the WitRun and zero-out all other
  // demands.
  void
  setPureOptionBuildAllocVols();
  void
  setPureOptionBuildAllocVolsForMaxATP();
  void 
  setPureOptionDemandVols();
  


 // get usageName variable pointer
  std::string 
  getUsageName();

  // set usageName variable
  void
  setUsageName(const std::string & usageName);

  

  // alternative method to calculate the (second pass) demand vols for features
  // This method pegs the shipVolWithoutFeature to the liveDemandVol, and using the
  // appropriate featureRatio, calculates the demand vol to try to build of the feature. 
  void
  calculateOneFeaturesBuildDemandVol ( int nPeriods,
                                     float * liveDemandVol,
                                     float * shipVolWithoutFeature,
                                     float * featureRatio,
                                     float * featureBuildDemandVols );

  
  // Set demand vols
  // Use this member function to clean-up the mucked demand vols
  void
  setAllDemandVolsExceptFeatureDemands ( int nParts,
                                       char ** partList,
                                       int *  nDemandsList,
                                       char *** allDemandsList,
                                       float *** allDemandsVols );


  // Dan's algorithm for calculating the amount of demand lost due to backlogYield.
  // original source code from:  /u/dpc/implode/sce/src/test2.c
  // Given the demandVol, the shipVol, and the backlogYield vector, this method calculates
  // the amount of demandVol that was "lost" due to applying the backlogYield (where shipLateLimit = 0).
  //
  // Example: demand= ( 2, 4, 6 ), ship=( 1, 2, 3 ) and backlogYld=( 0.6, 0.6, 0.6 )
  // then the lostDemandVol=( 0.400, 1.040, 1.824)

  void
  calculateLostDemandUnderBacklogYield ( int nPeriods,
                                         float * demandVol,
                                         float * shipVol,
                                         float * backlogYield,
                                         // method populates the lostDemandVol vector
                                         float * lostDemandVol,
                                         float * backlog);

  // Dan's algorithm for calculating the amount of demand lost due to shipLateLimit.
  // Also returns the backlog vector. 
  // original source code from: /u/dpc/implode/sce/src/test3.c
  void
  calculateLostDemandUnderShipLateLimit ( int nPeriods,
                                          int shipLateLimit,
                                          float * demandVol,
                                          float * shipVol,
                                          // output vectors
                                          float * lostDemandVol,
                                          float * backlog);  


void
dumpaVec(std::string aMessage, LgFrTimeVecFloat & theVec);

void
dumpaVec(std::string aMessage, float * theVec);




  /* Calculates the minimum demand vols by un-cumming the cum ship soft lb's of theWitRun
   * The residual "above min demands" vols are calculated and stored as demand appData.
   * Remeber to clean up any "new-ed" demand appData.
   */
  void
  setMinDemandsAndCalculateAboveMinDemands(
                                           WitRun *     theWitRun,
                                           int          nPeriods,
                                           int          nParts,
                                           char **      partList,
                                           int *        nDemandsList,
                                           char ***     allDemandsList,
                                           float ***    allDemandsVols );
  
  // default constructor
  LgFrSceAllocationEngineMgr();
  
  // general constructor
  LgFrSceAllocationEngineMgr(
                      LgFrScenarioForSceSmartPointer sceScenSP,
                      WitRun * const theWitRun, 
                      LgFrMultiPlantHelper * mpHelper,
                      std::string & usageName);

    
  // copy constructor
  LgFrSceAllocationEngineMgr(const LgFrSceAllocationEngineMgr& source);
  
  // assignment operator
  LgFrSceAllocationEngineMgr&
  operator=(const LgFrSceAllocationEngineMgr& rhs);
  
  // destructor
  virtual
  ~LgFrSceAllocationEngineMgr();
  
  
  // self-test
  static void test();
  
private:
  LgFrScenarioForSceSmartPointer sceScenSP_;
  WitRun * theWitRun_;
  LgFrMultiPlantHelper * mpHelper_;
  std::string usageName_;
  LgFrSceAllocLet           *  vecAllocLetList_;        // Set of AllocLets
  LgFrSceAllocLet           ** allocLetList_;        // Set of AllocLets
  LgFrSceSortStrategy       * sortStrategy_;       
  LgFrSceBacklogStrategy    * bklgStrategy_; 
  int                       traceFlag_;           // 1 => print a trace, 0 => No Trace
  std::string                 traceFileName_;       // filename of trace file
  int                       listMax_;
  int                       aPointer_;
  std::ofstream                  outFile_;
};
#endif
