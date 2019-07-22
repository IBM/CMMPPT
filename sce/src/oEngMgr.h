// file: oEngMgr.h
// author: robin and tom

#ifndef  LGFRSCEOPTENGINEMGR_H
#define  LGFRSCEOPTENGINEMGR_H


#include <sce/src/scenSceP.h>


// forward references
class WitRun;
class LgFrSceCritList;
class LgFrCalendar;
class LgFrMultiPlantHelper;
class LgFrInitialProblemForSce;

class LgFrSceOptEngineMgr {
  
public:
  
  // do set up and run optimization
  virtual
  void
  implode(LgFrSceCritList & theCritList);
  
#if 0
  // Get ipPtr variable pointer
  LgFrInitialProblemForSce *
  getIpPtr();
  
  // set ipPtr variable pointer
  void
  setIpPtr(LgFrInitialProblemForSce * ipPtr);
#endif
  
  // default constructor
  LgFrSceOptEngineMgr();
  
  // general constructor
  LgFrSceOptEngineMgr(WitRun * const theWitRun, 
                      LgFrCalendar * theCal,
                      LgFrMultiPlantHelper * mpHelper,
                      LgFrScenarioForSceSmartPointer sceScenSP);

  // the original model was setup so that it would work for
  // mrp and heuristic.  To run the model in optimization mode,
  // we must change the structure back.
  void   
  reStructureForOptimization();
  
    
  // copy constructor
  LgFrSceOptEngineMgr(const LgFrSceOptEngineMgr& source);
  
  // assignment operator
  LgFrSceOptEngineMgr&
  operator=(const LgFrSceOptEngineMgr& rhs);
  
  // destructor
  virtual
  ~LgFrSceOptEngineMgr();
  
  
  // self-test
  static void test();
  
private:
  WitRun * theWitRun_;
  LgFrCalendar * theCal_;
  LgFrMultiPlantHelper * mpHelper_;
  LgFrScenarioForSceSmartPointer sceScenSP_;
};
#endif
