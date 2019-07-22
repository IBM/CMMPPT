// file: priorMgr.h
// author:  tom

#ifndef  LGFRSCEPRIORITYMANAGER_H
#define  LGFRSCEPRIORITYMANAGER_H

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
class LgFrInitialProblemForSce;
class LgFrMultiPlantHelper;
class LgFrSceFlatFileMgr;
class LgFrSceDemLet;
class LgFrScePriorityRule;
class LgFrSceBacklogStrategy;
class LgFrSceScrubber;


typedef int(*LgFrSceDemLetCompareVoidFnPtr)(const void*,const void*);
// typedef int(*LgFrSceCompareVoidFnPtr)(const void*,const void*);


class LgFrScePriorityMgr {
  
public:

  // The main routine, it executes everything ... for now
  void
  setPriorities(const std::string & fileName);

  void
  initializeDemLets();

  void 
  calculateRuleSignature(LgFrSceDemLet * myDemLet);


  void 
  processPriorityManagerRulesFile(const std::string & fileName);



  // set flag to print trace
  void   trace(int traceFlag);

  // set filename for trace
  void   traceFileName(std::string & traceFile);

  void printDemLets();

  void assignWitPriorityFromDemLets();

  void
  testForNonIncreasingPriorities();


  // use these calls to get the right compare function for sorting
  virtual LgFrSceDemLetCompareVoidFnPtr
  compareVoidDemLetFunc();
  virtual LgFrSceDemLetCompareVoidFnPtr
  compareVoidPrRuleFunc();




  void 
  openInputFileAndScrubOutputFile(const std::string & fileName,
				  std::string fileDescription,
				  std::ifstream & inFile,
				  std::ofstream & outFile,
				  LgFrSceScrubber * myScrubber,
				  MclFacility * theErrFacil);






  void
  dumpaVec(std::string aMessage, int * theVec);


  
  // default constructor
  LgFrScePriorityMgr();
  
  // general constructor
  LgFrScePriorityMgr( LgFrScenarioForSceSmartPointer sceScenSP,
		      WitRun                     * const theWitRun,
		      LgFrMultiPlantHelper       * mpHelper,
		      LgFrSceFlatFileMgr         * flatFileMgr,
		      LgFrCalendar               * theCal);

    
  // copy constructor
  LgFrScePriorityMgr(const LgFrScePriorityMgr& source);
  
  // assignment operator
  LgFrScePriorityMgr&
  operator=(const LgFrScePriorityMgr& rhs);
  
  // destructor
  virtual
  ~LgFrScePriorityMgr();
  
  
  // self-test
  static void test();
  
private:
  int                          nRules_;
  int                          nUniquePrValues_;
  LgFrScePriorityRule       ** rulePtrVec_;       
  LgFrSceDemLet             *  vecDemLetList_;  
  LgFrSceDemLet             ** demLetList_;     
  LgFrCalendar               * theCal_;

  LgFrScenarioForSceSmartPointer sceScenSP_;
  WitRun                     * theWitRun_;
  LgFrMultiPlantHelper       * mpHelper_;
  LgFrSceFlatFileMgr         * flatFileMgr_;

  int                          traceFlag_;           // 1 => print a trace, 0 => No Trace
  std::string                  traceFileName_;       // filename of trace file
  int                          listMax_;
  int                          aPointer_;
  std::ifstream                inFile_;
  std::ofstream                outFile_;
};
#endif
