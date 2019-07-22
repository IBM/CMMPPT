// file: sceFssMg.h
// author: robin and tom

#ifndef  LGFRSCEFSSMANAGER_H
#define  LGFRSCEFSSMANAGER_H

// RW2STL - inserted:
#include <string>
// End of RW2STL - inserted includes.


#include <fstream>
#include <scenario/src/generVc.h>
#include <sce/src/scenSceP.h>
#include <sce/src/scenSce.h>

#ifdef SCE_ODBC

#include <sce/src/db.h>

#endif
// forward references
class LgFrMultiPlantHelper;
class WitRun;
class LgFrCalendar;
class LgFrSceFssableExploderDecorator;

class LgFrSceFssMgr {
  
public:

  
  // default constructor
  LgFrSceFssMgr();
  
  // general constructor
  LgFrSceFssMgr(WitRun * const theWitRun, 
                LgFrCalendar * theCal,
                LgFrMultiPlantHelper * mpHelper,
                LgFrScenarioForSce * scenScePtr
                );

  void
  runFss(std::string & fssType,
	 std::string & fssMethod,
	 std::string & fssFeatureMethod,
	 int         fssHorizon,
	 std::string & fssOutputFileName,
	 std::string & fssInputSetFileName);
    


  // run FSS singleton
  void
  singletonFss();
  
  // run FSS general set
  void
  generalSetFss();

  // run FSS general ECA set
  void
  machineFss();
  
  void
  getNextFssSet(LgFrStringVector & focusPartNameList,
                WitRun *  theWitRun,                
                std::string & theFssSetName,
                std::string & fssSetNameReadFromLastTime,
                std::string & partReadFromlastTime,
                std::string & FSSFile,
                std::ifstream & fssInFile,
                std::ofstream & scrubOutFile,
		long & totalRec,  // counters for sce i/o log reporting
		long & goodRec,
		long & numDistinctSetIDs,
		long & badRec,
		long & commentRec);
  
  void
  getNextSingletonFssSet(LgFrStringVector & focusPartNameList,
                         WitRun *    theWitRun,
                         int    &    singletonPartIndex,
                         std::string & theFssSetName,
                         int         nParts,
                         char **     partList);
  
  
  

  // copy constructor
  LgFrSceFssMgr(const LgFrSceFssMgr& source);
  
  // assignment operator
  LgFrSceFssMgr&
  operator=(const LgFrSceFssMgr& rhs);
  
  // destructor
  virtual
  ~LgFrSceFssMgr();


void
fssAnalysisBlue(
    LgFrSceFssableExploderDecorator * myExploder,
    std::string & fssMethod,    
    LgFrStringVector & focusPartNameList,
    int         nStandaloneFeatureArcs,
    std::string & fssFeatureScheme,
    int       nParts,
    char   ** partList,
    float  ** forwardRollableStockVols,        
    float *** FSSReqPtr,
    float *** FSSCovPtr);

  

void getDemandLists(
    WitRun   * theWitRun,
    int        nParts, 
    char    ** partList, 
    int     ** nDemandsPtr,
    char  **** demandListPtr);


int getSupplyVols(
    WitRun  * theWitRun,
    int       nParts, 
    char   ** partList, 
    float *** supplyVolsPtr);


int getForwardRollableStockSchedule(
    WitRun  * theWitRun,
    int       nParts, 
    char   ** partList, 
    float *** forwardRollableStockVolsPtr);
  
void getDemandVols(
    WitRun   * theWitRun,
    int        nParts, 
    char    ** partList, 
    int      * nDemands,
    char   *** dList,
    float **** demandVolsPtr);

void getShipVols(
    WitRun   * theWitRun, 
    int        nParts, 
    char    ** partList, 
    int      * nDemands,
    char   *** dList,
    float **** shipVolsPtr);



  // ================================================================ 
  // ================================================================
  

int
isFssSetSatisfiedWithinHorizon(WitRun *  theWitRun,
                               LgFrStringVector & focusPartNameList,
                               int fssHorizon);

// now returns the number of records printed
long  
printFssGeneralFile(WitRun * theWitRun,
                    int      nParts,
                    char  ** partList,
                    std::string & setID,
                    float ** FSSReq,
                    float ** FSSCov,
                    std::ofstream & fssOutFile);

// also returns the number of records printed
long
printModifiedFssFile(WitRun * theWitRun,
                     int      nParts,
                     char  ** partList,
                     std::string & setID,
                     float ** FSSReq,
                     float ** FSSCov,
                     std::ofstream & fssOutFile);
  


void 
adjustFssCovSupplyForFeatures(WitRun * localWitRun,
                              WitRun * originalWitRun,
                              const char  * focusPart,
                              const char  * focusDemand,
                              float * focusShipVol);
  
void 
adjustFssCovDemandForFeatures(WitRun * localWitRun,
                              WitRun * originalWitRun,
                              const char  * focusPart,
                              const char  * focusDemand,
                              float * focusShipVol);

void 
adjustFssReqSupplyForFeatures(WitRun * localWitRun,
                              WitRun * originalWitRun,
                              const char  * focusPart,
                              const char  * focusDemand,
                              float * focusDemandVol);
void 
adjustFssReqDemandForFeaturesToZero(WitRun * localWitRun,
                                    WitRun * originalWitRun,
                                    const char  * focusPart,
                                    const char  * focusDemand,
                                    float * zeroVec);
  
void 
adjustFssReqSupplyForFeaturesToZero(WitRun * localWitRun,
                                    WitRun * originalWitRun,
                                    const char  * focusPart,
                                    const char  * focusDemand,
                                    float * zeroVec);
  
  
#ifdef SCE_ODBC

void
insertFssSingleSchemeDB(WitRun * theWitRun,
                        int      nParts,
                        char  ** partList,
                        std::string & setID,
                        float ** FSSReq,
                        float ** FSSCov);


#endif
  
  // self-test
  static void test();

  
private:
  WitRun * theWitRun_;
  LgFrCalendar * theCal_;
  LgFrMultiPlantHelper * mpHelper_;
  LgFrScenarioForSce* scenScePtr_;
  
};
#endif
