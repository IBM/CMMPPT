// file: hEngMgr.h
// author: robin and tom

#ifndef  LGFRSCEHEURENGINEMGR_H
#define  LGFRSCEHEURENGINEMGR_H

// RW2STL - inserted:
#include <string>
// End of RW2STL - inserted includes.

#include <stdlib.h>
#include <wit/src/wit.h>

#include <sce/src/sceDefin.h>
#include <sce/src/scenSceP.h>

// forward references
class LgFrSceIterativeImpHelper;
class LgFrCalendar;
class LgFrSceCritList;
class LgFrInitialProblemForSce;
class LgFrMultiPlantHelper;

class LgFrSceHeurEngineMgr {
  
public:
  
  // do set up and run optimization
  virtual
  void
  implode(LgFrSceCritList & theCritList);

  // place holder right now for code to do 2-pass implosion for feature building
  virtual
  void
  heurImplodeOptFeaturesAndMins(LgFrSceCritList & theCritList);



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
  setPureOptionBuildDemand(
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
         LgFrSceIterativeImpHelper & iterativeImpHelper,
         float **** glbSubsProdVol,  // substitute bom production vol
         float *** glbShipSched,
         float *** allDemandsVols,
	 float ** globalExecSchedule);
  

 // get engine variable pointer
  std::string 
  getEngine();

  // set engine variable
  void
  setEngine(const std::string & engine);

  
  // preform rollover heuristic "incrementally"
  void
  rolloverHeurImplode( WitRun * const theWitRun,
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
                       witBoolean * isMinLotSizeUsedForOperation );

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
  LgFrSceHeurEngineMgr();
  
  // general constructor
  LgFrSceHeurEngineMgr(
                      LgFrScenarioForSceSmartPointer sceScenSP,
                      WitRun * const theWitRun, 
                      LgFrCalendar * theCal,
                      LgFrMultiPlantHelper * mpHelper,
                      const std::string & engine);

    
  // copy constructor
  LgFrSceHeurEngineMgr(const LgFrSceHeurEngineMgr& source);
  
  // assignment operator
  LgFrSceHeurEngineMgr&
  operator=(const LgFrSceHeurEngineMgr& rhs);
  
  // destructor
  virtual
  ~LgFrSceHeurEngineMgr();
  
  
  // self-test
  static void test();
  
private:
  LgFrScenarioForSceSmartPointer sceScenSP_;
  WitRun * theWitRun_;
  LgFrCalendar * theCal_;
  LgFrMultiPlantHelper * mpHelper_;
  std::string engine_;
  
};
#endif
