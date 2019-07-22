#ifndef ESO2problem_h
#define ESO2problem_h

//#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <cassert>

#include <wit.h>

#include "partInterplant.h"

typedef std::pair<std::string,int> ESO2stringIntPair;

extern "C" {typedef witReturnCode (STDCALL * witSetNameFloatStar) (
   WitRun * const theWitRun,
   const char * const name,
   const float * const floatVec);}

extern "C" {typedef witReturnCode (STDCALL * witGetNameFloatStarStar) (
   WitRun * const theWitRun,
   const char * const name,
   float ** floatVec);}

extern "C" {typedef witReturnCode (STDCALL * witSetName3FloatStar) (
   WitRun * const theWitRun,
   const char * const operationName,
   const float * const hardLower,
   const float * const softLower,
   const float * const hardUpper);}

extern "C" {typedef witReturnCode (STDCALL * witGetName3FloatStar) (
   WitRun * const theWitRun,
   const char * const operationName,
   float ** hardLower,
   float ** softLower,
   float ** hardUpper);}

extern "C" {typedef witReturnCode (STDCALL * witSetNameIndexFloatStar) (
   WitRun * const theWitRun,
   const char * const name,   
   const int bopEntryIndex,
   const float * const floatVec);}

extern "C" {typedef witReturnCode (STDCALL * witSetNameIndexIndexFloatStar) (
   WitRun * const theWitRun,
   const char * const name,   
   const int bopEntryIndex,   
   const int subEntryIndex,
   const float * const floatVec);}

extern "C" {typedef witReturnCode (STDCALL * witGetNameIndexFloatStarStar) (
   WitRun * const theWitRun,
   const char * const name,   
   const int bopEntryIndex,
   float ** floatVec);}

extern "C" {typedef witReturnCode (STDCALL * witGetNameIndexIntStarStar) (
   WitRun * const theWitRun,
   const char * const name,   
   const int bopEntryIndex,
   int ** intVec);}

extern "C" {typedef witReturnCode (STDCALL * witGetNameIndexIndexFloatStarStar) (
   WitRun * const theWitRun,
   const char * const name,   
   const int bopEntryIndex,   
   const int subEntryIndex,
   float ** floatVec);}

extern "C" {typedef witReturnCode (STDCALL * witGetNameIndexIndexIntStarStar) (
   WitRun * const theWitRun,
   const char * const name,   
   const int bopEntryIndex,   
   const int subEntryIndex,
   int ** intVec);}

extern "C" {typedef witReturnCode (STDCALL * witGetNameIndexIndexIntStar) (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   int * earliestPeriod);}

extern "C" {typedef witReturnCode (STDCALL * witSetDblNameFloatStar) (
   WitRun * const theWitRun,
   const char * const partName,  
   const char * const demandName, 
   const float * const floatVec);}

extern "C" {typedef witReturnCode (STDCALL * witGetDblNameFloatStarStar) (
   WitRun * const theWitRun,
   const char * const partName,
   const char * const demandName,
   float ** floatVec);}

class ESO2problem  {
public:

  //----------------------------------
  // Methods for global attributes.
  //----------------------------------
  // Set/Get String Describing Problem
  void setTitle(const std::string & title);
  std::string getTitle() const;

  // Set/Get Number of Periods in Problem
  void setNPeriods(int nP);
  inline int getNPeriods() const
  {
    int retVal = nPeriods_;
#ifndef NDEBUG
    //std::cout <<"called getNPeriods()" <<std::endl;
    int nP;
    witGetNPeriods(mutableWitRun(),&nP);
    assert(nP==retVal);
#endif
    return retVal;
  }

  // Set/Get Wholesale Fair Market Value Scale Factor
  void setWfmvScaleFactor(float wfmvScaleFactor, float wfmv2ScaleFactor);
  float getWfmvScaleFactor() const;
  float getWfmv2ScaleFactor() const;
  float getWfmvScaleFactor(int demandFileNumber) const;

  // Set/Get flag to indicate if wfmvScale factor is to be applied for supply from plantLoc when satisfying demand from demandFileNumber
  void setApplyLocWfmvScaleFactor(const std::string & plantLoc,int demandFileNumber);
  bool getApplyLocWfmvScaleFactor(const std::string & plantLoc,int demandFileNumber) const;
  void setUseApplyScaleFactorLoc(bool val);
  bool getUseApplyScaleFactorLoc() const;

  // Set/Get priorities of mtm, features, and looseParts.
  // Priorities are:
  // 1 - high. Select this category of supply first.
  // 2 - medium. 
  // 3 - low. Select this category of supply last.
  // 4 - never. Never consume this category of supply to meet demand.
  void setMtmPriority( int p );
  int getMtmPriority() const;
  void setFeaturePriority( int p );
  int getFeaturePriority() const;
  void setLoosePartPriority( int p );
  int getLoosePartPriority() const;
  void setMtm2Priority( int p );
  int getMtm2Priority() const;
  void setFeature2Priority( int p );
  int getFeature2Priority() const;
  void setLoosePart2Priority( int p );
  int getLoosePart2Priority() const;

  // Set period when supply in period 1 perishes.
  // A value of 0 indicates that any supply not consumed in period 1,
  // is not available for consumption in period 2.
  // A value of 1 indicates that any supply not consumed in period 1,
  // is available for consumption in period 1.
  // A value of 2 indicates that any supply available in period 1 that
  // is not consumed in period 1 or 2, is available for consumption in period 2.
  // This parameter affects the supply of mtm's, features, and loose parts.
  void setPeriod1SupplyPerishPeriod(int period);
  inline int getPeriod1SupplyPerishPeriod() const
  {
    return period1SupplyPerishPeriod_;
  }

  // Set flag which indicates that inventory may be held at demand location
  void setAllowInventoryHoldingAtDemandLocation(bool allow);
  inline bool getAllowInventoryHoldingAtDemandLocation() const
  {
    return allowInventoryHoldingAtDemandLocation_;
  }
  
  // Set flag which indicates that inventory may be held at demand location
  void setRemoveCycles(bool remove);
  inline bool getRemoveCycles() const
  {
    return removeCycles_;
  }

  // There is a scale factor for each demand file
  void setWacScaleFactors( float wsf1, float wsf2 );
  const std::vector<float> & getWacScaleFactors() const;


  // Set OSL Logfile name
  void setSolverLogFileName(const std::string & name);
  
  //----------------------
  // Methods for mtms.
  //----------------------
  void addMtm         (const std::string & mtmName, const std::string & mtmLoc,float demanCost,float demanTime );
  bool mtmExists      (const std::string & mtmName, const std::string & mtmLoc )const;
  void setMtmSupply   (const std::string & mtmName, const std::string & mtmLoc, int period, float supply );
  void setMtmWfmv     (const std::string & mtmName, const std::string & mtmLoc, int period, float supply );
  void setMtmDemanCost(const std::string & mtmName, const std::string & mtmLoc, int period, float cost );
  void setMtmDemanCost(const std::string & mtmName, const std::string & supplyLoc, const std::string & demanLoc, const std::vector<float> & cost );
  void setMtmDemanCost(const std::string & mtmName, const std::string & supplyLoc, const std::string & demanLoc, int period, float cost );
  void setMtmDemanTime(const std::string & mtmName, const std::string & mtmLoc, int period, float time );
  void setMtmDemanTime(const std::string & mtmName, const std::string & supplyLoc, const std::string & demanLoc, const std::vector<float> &  time );
  void setMtmDemanTime(const std::string & mtmName, const std::string & supplyLoc, const std::string & demanLoc, int period, float time );
 
  // Get list of all mtms added
  void getMtms( std::vector<std::string> & mtmName, std::vector<std::string> &mtmLocs ) const;
  std::vector<float> getMtmAcquireQty(const std::string & mtmName, const std::string & mtmLoc )const;
  std::vector<float> getMtmAcquireQty(const std::string & mtmName, const std::string & srcLoc, const std::string & destLoc )const;

  std::vector<float> getMtmAcquireQtyWithOffset(const std::string & mtmName, const std::string & mtmLoc )const;
  std::vector<float> getMtmAcquireQtyWithOffset(const std::string & mtmName, const std::string & srcLoc, const std::string & destLoc )const;

  std::vector<float> getMtmSupply(const std::string & mtmName, const std::string & mtmLoc )const;
  std::vector<float> getMtmWfmv(const std::string & mtmName, const std::string & mtmLoc ) const;
  std::vector<float> getMtmDemanCost(const std::string & mtmName, const std::string & mtmLoc ) const;
  std::vector<float> getMtmDemanTime(const std::string & mtmName, const std::string & mtmLoc ) const;
  std::vector<float> getMtmDemanTime(const std::string & mtmName, const std::string & supplyLoc, const std::string & demanLoc ) const;
  
  //----------------------
  // Methods for features.
  //----------------------
  void addFeature      (const std::string & featureName, const std::string & featureLoc );
  bool featureExists   (const std::string & featureName, const std::string & featureLoc )const;
  void setFeatureSupply(const std::string & featureName, const std::string & featureLoc, int period, float supply );
  void setFeatureWfmv  (const std::string & featureName, const std::string & featureLoc, int period, float wholeSaleMarketValue );
   
  // Get list of all features added
  void getFeatures( std::vector<std::string> & featureName, std::vector<std::string> &featureLocs ) const;
  std::vector<float> getFeatureAcquireQty(const std::string & featureName, const std::string & featureLoc )const;
  std::vector<float> getFeatureSupply(const std::string & featureName, const std::string & featureLoc )const;
  std::vector<float> getFeatureWfmv(const std::string & featureName, const std::string & featureLoc )const;

  //----------------------
  // Methods for looseParts.
  //----------------------
  void addLoosePart      (const std::string & loosePartName, const std::string & loosePartLoc );
  bool loosePartExists   (const std::string & loosePartName, const std::string & loosePartLoc )const;
  void setLoosePartSupply(const std::string & loosePartName, const std::string & loosePartLoc, int period, float supply );
  void setLoosePartWfmv  (const std::string & loosePartName, const std::string & loosePartLoc, int period, float wholeSaleMarketValue );
   
  // Get list of all looseParts added
  void getLooseParts( std::vector<std::string> & loosePartName, std::vector<std::string> &loosePartLocs ) const;
  std::vector<float> getLoosePartAcquireQty(const std::string & loosePartName, const std::string & loosePartLoc )const;
  std::vector<float> getLoosePartSupply(const std::string & loosePartName, const std::string & loosePartLoc )const;
  std::vector<float> getLoosePartWfmv(const std::string & loosePartName, const std::string & loosePartLoc )const;

  //----------------------
  // Methods for Deman (demanufactuing boms).
  //----------------------
  bool demanBomExists     (const std::string & mtm, const std::string & part, const std::string & loc )const;
  void addDemanBom        (const std::string & mtm, const std::string & part, const std::string & loc );
  void setDemanBomAttachRate(const std::string & mtm, const std::string & part, const std::string & loc, int period, float prodRate );
  std::vector<float> getDemanBomAttachRate( const std::string & mtm, const std::string & part, const std::string & loc ) const;

  std::vector<std::string> getDemanBoms(const std::string & mtm, const std::string & mtmSrcLoc, const std::string & demanLoc)const;

  //----------------------
  // Methods for feature cross referencing
  //----------------------
  bool featureXrefExists      (const std::string & part, const std::string & feature, const std::string & loc )const;
  void addFeatureXref         (const std::string & part, const std::string & feature, const std::string & loc );
  void setFeatureXrefProdRate (const std::string & part, const std::string & feature, const std::string & loc, int period, float prodRate );
  void setFeatureXrefCost     (const std::string & part, const std::string & feature, const std::string & loc, int period, float cost );
  void setFeatureXrefUsageTime(const std::string & part, const std::string & feature, const std::string & loc, int period, float usageTime );
  void setFeatureXrefCost     (const std::string & part, const std::string & feature, const std::string & loc, float cost );
  void setFeatureXrefUsageTime(const std::string & part, const std::string & feature, const std::string & loc, float usageTime );

  void getFeatureXrefs( std::vector<std::string> & parts, std::vector<std::string> & features, std::vector<std::string> &locs ) const;
  std::vector<float> getFeatureXrefVol(const std::string & part,const std::string & feature, const std::string & loc )const;
  std::vector<float> getFeatureXrefCost(const std::string & part,const std::string & feature, const std::string & loc )const;
  std::vector<float> getFeatureXrefProdRate(const std::string & part,const std::string & feature, const std::string & loc )const;

  //----------------------
  // Methods for alt parts
  //----------------------
  bool altExists      (const std::string & partOrFeature, const std::string & altPart, const std::string & loc )const;
  void addAlt         (const std::string & partOrFeature, const std::string & altPart, const std::string & loc, float altCost, float altTime );
  //void setAlternatePartProdRate (const std::string & upLevelPart, const std::string & downLevelPart, const std::string & loc, int period, float prodRate );
  //void setAlternatePartCost     (const std::string & upLevelPart, const std::string & downLevelPart, const std::string & loc, int period, float cost );
  //void setAlternatePartUsageTime(const std::string & upLevelPart, const std::string & downLevelPart, const std::string & loc, int period, float usageTime );
   
  // Get list of all features added
  void getAlts( std::vector<std::string> & partOrFeatures, std::vector<std::string> & altParts, std::vector<std::string> &locs ) const;
  std::set<std::string> getAltPartOrFeature(const std::string & altPart,const std::string & loc) const;
  std::vector<float> getAltVolWithOffset(const std::string & partOrFeature,const std::string & altPart, const std::string & loc )const;
  std::vector<float> getAltVol          (const std::string & partOrFeature,const std::string & altPart, const std::string & loc )const;
  std::vector<float> getAltConvCost(const std::string & partOrFeature,const std::string & altPart, const std::string & loc )const;
  std::vector<float> getAltConvTime(const std::string & partOrFeature,const std::string & altPart, const std::string & loc )const;
 
  // Given a part name return all locations where the part exists
  std::vector<std::string> getPartLocs(const std::string & partName ) const;

  //----------------------
  // Methods for mtmInterplant
  //----------------------  
  bool mtmInterplantExists     (const std::string & mtm, const std::string & srcLoc, const std::string & destLoc )const;
  bool mtmInterplantExists     (const std::string & mtm, const std::string & srcLoc )const;
  void addMtmInterplant        (const std::string & mtm, const std::string & srcLoc, const std::string & destLoc, float defaultMoveCost, float defaultMoveTime  );
  void setMtmInterplantMoveCost(const std::string & mtm, const std::string & srcLoc, const std::string & destLoc, int period, float moveCost );
  void setMtmInterplantMoveTime(const std::string & mtm, const std::string & srcLoc, const std::string & destLoc, int period, float moveTime );
   
  // Get list of all mtmInterplants added
  void getMtmDemanLocs(
    std::vector<std::string> & mtms,
    std::vector<std::string> & srcLocs,
    std::vector<std::string> & destLocs ) const;
  std::vector<float> getMtmInterplantMoveCost(const std::string & mtm,const std::string & srcLoc, const std::string & destLoc )const;
  std::vector<float> getMtmInterplantMoveTime(const std::string & mtm,const std::string & srcLoc, const std::string & destLoc )const;
  std::vector<float> getMtmInterplantMoveVol(const std::string & mtm,const std::string & srcLoc, const std::string & destLoc )const;
  
  //----------------------
  // Methods for partInterplant
  //----------------------
  bool partInterplantExists     (const std::string & part, const std::string & srcLoc, const std::string & destLoc )const;
  void addPartInterplant        (const std::string & part, const std::string & srcLoc, const std::string & destLoc, float defaultMoveCost, float defaultMoveTime );
  void setPartInterplantMoveCost(const std::string & part, const std::string & srcLoc, const std::string & destLoc, int period, float moveCost );
  void setPartInterplantMoveTime(const std::string & part, const std::string & srcLoc, const std::string & destLoc, int period, float moveTime );
   
  // Get list of all partInterplants added
  void getPartInterplants( std::vector<std::string> & parts, std::vector<std::string> & srcLocs, std::vector<std::string> &destLocs ) const;
  std::set<std::string> getPartInterplantSrcLocs( const std::string & part, const std::string & destLoc ) const;
  std::vector<float> getPartInterplantMoveVol(const std::string & part,const std::string & srcLoc, const std::string & destLoc )const;
  std::vector<float> getPartInterplantMoveVolWithOffset(const std::string & part,const std::string & srcLoc, const std::string & destLoc )const;
  std::vector<float> getPartInterplantMoveCost(const std::string & part,const std::string & srcLoc, const std::string & destLoc )const;
  std::vector<float> getPartInterplantMoveTime(const std::string & part, const std::string & srcLoc, const std::string & destLoc ) const;
 
  // 
  void addPartsInterplant(const std::string & srcLoc, const std::string & destLoc, float defaultMoveCost, float defaultMoveTime );
  //----------------------
  // Methods for partDemand
  //----------------------
  bool partDemandExists      (const std::string & part, const std::string & custLoc, const std::string & plantLoc )const;
  void addPartDemand         (const std::string & part, const std::string & custLoc, const std::string & plantLoc );
  void setPartDemandDemandVol(const std::string & part, const std::string & custLoc, const std::string & plantLoc, int period, float demandVol );
  void setPartDemandWac      (const std::string & part, const std::string & custLoc, const std::string & plantLoc, int period, float wac );
  void setPartDemandPriority (const std::string & part, const std::string & custLoc, const std::string & plantLoc, int priority );
   
  // Get list of all partDemands added
  void getPartDemands( std::vector<std::string> & parts, std::vector<std::string> & custLocs, std::vector<std::string> &plantLocs ) const;
  std::vector<float> getPartDemandShipVol  (const std::string & part,const std::string & custLoc, const std::string & plantLoc )const;
  std::vector<float> getPartDemandDemandVol(const std::string & part,const std::string & custLoc, const std::string & plantLoc )const;
  std::vector<float> getPartDemandWac(const std::string & part,const std::string & custLoc, const std::string & plantLoc )const;
  std::vector<float> getPartDemandScaledWac(const std::string & part,const std::string & custLoc, const std::string & plantLoc )const;
  int getPartDemandPriority (const std::string & part, const std::string & custLoc, const std::string & plantLoc ) const;

  // Report on details on avail supply for a part demand.
  void reportPartDemand(const std::string & part, const std::string & custLoc, const std::string & plantLoc, int period )const;

  // Pegging information
  std::set<std::string> getPartDemandPeggedAttributeNames(
    const std::string & part, const std::string & custLoc, const std::string & plantLoc, int shipPeriod )const;
  void getPartDemandPegging(
    const std::string & peggedAttributeName,
    const std::string & part, const std::string & custLoc, const std::string & plantLoc, int shipPeriod,

    std::vector<std::string> & items, 
    std::vector<std::string> & locs, 
    std::vector<int> & periods, 
    std::vector<int> & depths, 
    std::vector<float> & quantity ) const;

#if 0
  void getPartDemandPegging(
    const std::string & part, const std::string & custLoc, const std::string & plantLoc, int shipPeriod,

    std::vector<std::string> & acquiredMtms, 
    std::vector<std::string> & acquiredMtmSrcLocs, 
    std::vector<std::string> & acquiredMtmAcquireLocs, 
    std::vector<int> & acquiredMtmAcquirePeriods, 
    std::vector<float> & acquiredMtmAcquireQuantity,

    std::vector<std::string> & demanMtms,
    std::vector<std::string> & demanLocs,
    std::vector<int> & demanPeriods,
    std::vector<float> & demanQuantities,

    std::vector<std::string> & features, 
    std::vector<std::string> & featurePlantLocs, 
    std::vector<int> & featureAcquirePeriods, 
    std::vector<float> & featureAcquireQuantity,

    std::vector<std::string> & looseParts, 
    std::vector<std::string> & loosePartPlantLocs, 
    std::vector<int> & loosePartAcquirePeriods, 
    std::vector<float> & loosePartAcquireQuantity )
    const;
#endif

  //----------------------
  // Methods for part.
  //----------------------
  bool partExists(const std::string & part, const std::string & partLoc )const;


  //----------------------
  // Remove inactive objects from (wit) model.
  // This is in an effort to improve performance.
  //----------------------
  void pruneWitModel();

  
  //----------------------
  // Attempt to remove cycles from BOM
  //----------------------
  void removeCycles();


  //----------------------
  // Label depth of Wit Objects to determine order that they are performed.
  // This is important when they are done in the same period.
  // This info is needed for the pegged reports
  //----------------------
  void labelObjectDepth(    
    std::map<std::string,int> & operationDepths,
    std::map<std::string,int> & partDepths);

  //----------------------
  // Methods for partShipReportO
  //----------------------
  

  //--------------------
  // Solve Methods. 
  //--------------------
  void solve();
  void solveWithPegging();

  
  //--------------------
  // Key Performance Indicators
  //--------------------
  double getObjValue() const;
  void mtmMetrics(
    double & totalWfmvWithScale,
    double & sumAcquireCost,
    double & sumMoveCost,
    double & sumDemanCost,
    int & numUnique,
    int & numUniqueAcquired,
    double & supplyQuantity,
    double & acquiredQuantity)
    const;
  void featureMetrics(
    double & totalWfmvWithScale,
    double & sumAcquireCost,
    double & sumXrefCost,
    int & numUnique,
    int & numUniqueAcquired,
    double & supplyQuantity,
    double & acquiredQuantity)
    const;
  void loosePartMetrics(
    double & totalWfmvWithScale,
    double & sumAcquireCost,
    double & sumAltCost,
    int & numUnique,
    int & numUniqueAcquired,
    double & supplyQuantity,
    double & acquiredQuantity)
    const;
  void partDemandMetrics(
    double & sumDemandWac,
    double & sumShippedWac,
    std::map<std::string,double> & sumShippedWacComponents,
    int & numUnique,
    int & numUniqueShipped,
    double & sumDemandVol,
    double & sumShipVol)
    const;
  double sumOfWac(std::map<std::string,double> & wacComponents) const;
  double sumOfFeatureWfmvWithScaleFactor() const;
  double sumOfLoosePartWfmvWithScaleFactor() const;
  double sumOfMtmWfmvWithScaleFactor() const;
  double sumOfMtmWfmvWithScaleFactorAndMoveCost() const;
  double sumOfDemanCost() const;
  double sumOfPartMoveCost() const;
  double sumOfMtmMoveCost() const;
  double sumOfFeatureXrefCost() const;
  double sumOfAltCost() const;


  //----------------------------------------------------------------------

  void writeWitData( std::string filename ) const;

  //----------------------------------------------------------------------

  static float bigM() { return 1000000.0f; };

  //----------------------------------------------------------------------

  // default constructor
  ESO2problem();
  
  // copy constructor
  ESO2problem( const ESO2problem& source );
  
  // assignment operator
  ESO2problem&
  operator=(const ESO2problem& rhs);
  
  // destructor
  ~ESO2problem();
  

  // Self-test
  static void test();

private:

  
  //----------------------
  // Methods for part.
  //----------------------
  void addPart(const std::string & part, const std::string & partLoc );
  void addPartWithOutApplyingInterplants(const std::string & part, const std::string & partLoc );
  void reportPart(
    const std::string & witPartName, 
    int period,int recursionDepth,
    float volume,
    float value  ) const;

  // add capacity. ensure scrap cost is high so anything acquired is consumed.
  // this is a wrapper for witAddPart(wr,name,WitCAPACITY);
  void addNonScrapableCapacity(const std::string & name );
  
  // Set prodRates from attach rates
  void setDemanBomAndFeatureXrefProdRates();
  
  // Set wit bounds and ensure that hlb<=slb<=hub
  // This condition is not being met.
  void setExecBounds(
    const std::string opName,
    std::vector<float> hlb,
    std::vector<float> slb,
    std::vector<float> hub );
  
  // Compute multilevel impact period given vector of impactPeriods
  typedef std::vector<int> StdVecInt;
  static std::vector<int> multiLevelImpactPeriod( const std::vector<StdVecInt> & impPersVecs );
  
  // uses kevin's logic to determine if supply being consumed
  // will be from period 0 (usePerishableSupply) or period>0 (don't usePerishableSupply)
   bool useMtmPerishableSupply(
     const std::string & mtm,
     const std::string & srcLoc,
     const std::string & destLoc,
     int period );
   bool useFeaturePerishableSupply(
     const std::string & feature,
     const std::string & featureLoc,
     int period );
   bool useLoosePartPerishableSupply(
     const std::string & part,
     const std::string & partLoc,
     int period );
   bool usePerishableSupply(
     const std::string & witAcquireOpName,
     const std::string & witAcquiredPartName,
     int period );
   
  // Set element of Part/Operation float* attribute, given period and fltValue
  void witSetNameAttribute(
    witGetNameFloatStarStar witGetFunc,
    witSetNameFloatStar     witSetFunc,
    const std::string & name,
    int period, 
    float supply ); 
  
  // Set Part/Operation float* attribute, given std::vector<float>
  void witSetNameAttribute(
    witSetNameFloatStar     witSetFunc,
    const std::string & name,
    const std::vector<float> & value );

  // Set Part/Operation float* attribute, given float
  void witSetNameAttribute(
    witSetNameFloatStar     witSetFunc,
    const std::string & name,
    float value );
  
  // Set Part/Operation bound attribute, given three std::vector<float> vectors
  void witSetNameBoundAttribute(
    witSetName3FloatStar     witSetFunc,
    const std::string & name,
    const std::vector<float> & hlb,
    const std::vector<float> & slb,
    const std::vector<float> & hub );

  // Set element on bom/bop float* attribute, given period and fltValue
  void witSetArcAttribute(
    witGetNameIndexFloatStarStar witGetFunc,
    witSetNameIndexFloatStar     witSetFunc,
    const std::string & opName,
    int index,
    int period, 
    float flt );

  // Set element on bom/bop float* attribute, given std::vector<float>
  void witSetArcAttribute(
    witSetNameIndexFloatStar     witSetFunc,
    const std::string & opName,
    int index,
    const std::vector<float> & value );

  // Set element on bom/bop float* attribute, given float
  void witSetArcAttribute(
    witSetNameIndexFloatStar     witSetFunc,
    const std::string & opName,
    int index,
    float value );    

  // Set element on demand float* attribute, given period and fltValue
  void witSetDemandAttribute(
    witGetDblNameFloatStarStar      witGetFunc,
    witSetDblNameFloatStar          witSetFunc,
    const std::string & partName,
    const std::string & demandName,
    int period, 
    float flt );    

  // Set element on demand float* attribute, given std::vector<float>
  void witSetDemandAttribute(
    witSetDblNameFloatStar          witSetFunc,
    const std::string & partName,
    const std::string & demandName,
    const std::vector<float> & fltVec );
  
  // Get vector of wit part/Operation float* attribute
  std::vector<float> witGetNameAttribute(
    witGetNameFloatStarStar witGetFunc,
    const std::string & name )const;

  // Get vector of bom/bop float* attribute
  std::vector<float> witGetArcAttribute(
    witGetNameIndexFloatStarStar     witGetFunc,
    const std::string & opName,
    int index )const;
  // Get vector of bom/bop int* attribute
  std::vector<int> witGetArcAttribute(
    witGetNameIndexIntStarStar     witGetFunc,
    const std::string & opName,
    int index )const;

  
  // Get vector of sub float* attribute
  std::vector<float> witGetSubArcAttribute(
    witGetNameIndexIndexFloatStarStar     witGetFunc,
    const std::string & opName,
    int index, int subIndex )const;
  // Get vector of sub int* attribute
  std::vector<int> witGetSubArcAttribute(
    witGetNameIndexIndexIntStarStar     witGetFunc,
    const std::string & opName,
    int index, int subIndex )const;
  // Get vector of sub int attribute
  int witGetSubArcAttribute(
    witGetNameIndexIndexIntStar     witGetFunc,
    const std::string & opName,
    int index, int subIndex )const;
  
  // Set sub float* attribute, given period and fltValue
  void witSetSubArcAttribute(
    witGetNameIndexIndexFloatStarStar witGetFunc,
    witSetNameIndexIndexFloatStar     witSetFunc,
    const std::string & opName,
    int index, int subIndex,
    int period, 
    float flt );

  // Get vector of wit demand float* attribute
  std::vector<float> witGetDemandAttribute(
    witGetDblNameFloatStarStar witGetFunc,
    const std::string & partName,
    const std::string & demandName )const;

  
  // Get Part/Operation bound attributes, given three std::vector<float> vectors
  void witGetNameBoundAttribute(
    witGetName3FloatStar     witGetFunc,
    const std::string & name,
    std::vector<float> & hlb,
    std::vector<float> & slb,
    std::vector<float> & hub )const;

  std::vector<std::string> witGetOperProducts(const std::string & opName) const;

  // MTM Name Methods
  static std::string igfMtmName(const std::string & mtm,const std::string & mtmLoc );
  static std::string acquireMtmName(const std::string & mtm,const std::string & mtmSrcLoc,const std::string & destLoc );
  static std::string serverMtmName(const std::string & mtm,const std::string & mtmSrcLoc,const std::string & destLoc  );
  static std::string demanMtmName(const std::string & mtm,const std::string & mtmSrcLoc,const std::string & destLoc  );
  static std::string baseMtmName(const std::string & mtm,const std::string & mtmDestLoc );
  static std::string mtmFromMtmName(const std::string & mtmName);
  static std::string mtmDestLocFromMtmName(const std::string & mtmName);
  static std::string mtmSrcLocFromMtmName(const std::string & mtmName);
  static bool isMtmDemanName( const std::string & witName );
  static bool isAcquireMtmName( const std::string & witName );

  static bool isAcquireName( const std::string & witName );

  // Feature Name Methods
  static std::string igfFeatureName(const std::string & feature,const std::string & loc );
  static std::string acquireFeatureName(const std::string & feature,const std::string & loc );
  static std::string baseFeatureName(const std::string & feature,const std::string & loc );
  static std::string featureFromFeatureName(const std::string & featureName);
  static std::string locFromFeatureName(const std::string & featureName);
  static bool isAcquireFeatureName( const std::string & witName );

  // Loose Part Name Methods
  static std::string igfLoosePartName(const std::string & loosePart,const std::string & loc );
  static std::string acquireLoosePartName(const std::string & loosePart,const std::string & loc );
  static std::string baseLoosePartName(const std::string & loosePart,const std::string & loc );
  static std::string loosePartFromLoosePartName(const std::string & loosePartName);
  static std::string locFromLoosePartName(const std::string & loosePartName);
  static bool isAcquireLoosePartName( const std::string & witName );

  // feature cross reference name methods
  static std::string featureXrefName(const std::string & part, const std::string & feature, const std::string & loc);
  static std::string partFromFeatureXrefName(const std::string & featXrefName );
  static std::string featureFromFeatureXrefName(const std::string & featXrefName);
  static std::string locFromFeatureXrefName(const std::string & featXrefName);
  static bool isFeatureXrefName( const std::string & witName );

  // alternate name methods
  static std::string altName(const std::string & upLevelPart, const std::string & downLevelPart, const std::string & loc);
  static std::string partFromAltName(const std::string & etnCspName);
  static std::string altPartFromAltName(const std::string & etnCspName);
  static std::string locFromAltName(const std::string & etnCspName);
  static bool isAltName( const std::string & witName );

  // partInterplant name methods
  static std::string partInterplantName(const std::string & part, const std::string & srcLoc, const std::string & destLoc);
  static std::string partFromPartInterplantName(const std::string & partInterplantName);
  static std::string srcLocFromPartInterplantName(const std::string & partInterplantName);
  static std::string destLocFromPartInterplantName(const std::string & partInterplantName);
  static bool isPartInterplantName( const std::string & witName );

  // partDemand name methods
  static std::string partDemandName(const std::string & part, const std::string & custLoc, const std::string & plantLoc);
  static std::string partDemandRegulatorName(const std::string & part, const std::string & custLoc, const std::string & plantLoc);
  static std::string partDemandInventoryName(const std::string & part, const std::string & custLoc, const std::string & plantLoc);
  static std::string partFromPartDemandName(const std::string & partDemandName);
  static std::string custLocFromPartDemandName(const std::string & partDemandName);
  static std::string plantLocFromPartDemandName(const std::string & partDemandName);

  // Part Name Methods  
  static std::string partName(const std::string & part,const std::string & loc );
  static std::string moveablePartName(const std::string & part,const std::string & loc );
  static std::string partFromPartName( const std::string & partName );
  static std::string locFromPartName( const std::string & partName );

  // Cycle Breaking Name Methods
  static std::string cycleBreakingPartName(const std::vector<std::string> & partNames);
  static std::string cycleBreakingOperationName(const std::string & ecOpNameInCycle, const std::vector<std::string> & partNamesInCycle);

  // String Utilities
  static std::string textBetween(const std::string & src, const std::string & before,const std::string & after);
  static std::string textAfter(const std::string & src, const std::string & before);
  static bool beginsWith(const std::string & src, const std::string & begString);
  static bool contains(const std::string & haystack, const std::string & needle);

  // get index of bop entry  
  int getBopEntryIndex( 
    const std::string & mtm, 
    const std::string & part, 
    const std::string & loc ) const;
  int getBopEntryIndex( 
    const std::string & opName, 
    const std::string & partName ) const;

  void addSubsForPeriod1SupplyPerishPeriod(
       const std::string & acquireOperation, 
       const std::string & acquiredPart,
       float moveTime);
    
  public:
  inline std::vector<float> witFloatStarToStlVec(const float * inputVec ) const
  {
    std::vector<float>  retVal;
    int np = getNPeriods();
    int i;
    for ( i=0; i<np; ++i ) retVal.push_back(inputVec[i]);
    return retVal;
  };

  inline std::vector<int> witIntStarToStlVec(const int * inputVec ) const
  {
    std::vector<int>  retVal;
    int np = getNPeriods();
    int i;
    for ( i=0; i<np; ++i ) retVal.push_back(inputVec[i]);
    return retVal;
  };

  inline float * stlVecToFloatStar(const std::vector<float> & inputVector ) const
  {
    size_t len = inputVector.size();
    float * retVal = NULL;
    if ( len > 0 ) {
      //retVal = (float *) malloc(len*sizeof(float));
      retVal = new float[len];
      size_t i;
      for ( i=0; i<len; ++i ) retVal[i] = inputVector[i];
    }
    return retVal;
  };
  
  inline float * floatToConstFloatStar(float input ) const
  {
    int len = getNPeriods();
    //float * retVal = (float *) malloc(len*sizeof(float));
    float * retVal = new float[len];
    int i;
    for ( i=0; i<len; ++i ) retVal[i] = input;
    return retVal;
  };
  
  inline std::vector<float> floatToStlVec(float input ) const
  {
    int len = getNPeriods();
    std::vector<float> retVal;
    int i;
    for ( i=0; i<len; ++i ) retVal.push_back(input);
    return retVal;
  };
#if 0
  inline std::vector<int> intToStlVec(int input ) const
  {
    int len = getNPeriods();
    std::vector<int> retVal;
    int i;
    for ( i=0; i<len; ++i ) retVal.push_back(input);
    return retVal;
  };
#endif

  private:

  int getMtmPriority(int demandPriority) const;
  int getFeaturePriority(int demandPriority) const;
  int getLoosePartPriority(int demandPriority) const;

  void setDemanBomAndFeatureXrefProdRatesAreSet(bool v) {
    demanBomAndFeatureXrefProdRatesAreSet_=v;
  };
  bool getDemanBomProdRatesAreSet() const {
    return demanBomAndFeatureXrefProdRatesAreSet_;
  };

  // Get an operations execVol shifted by Bom offset (requires that operation only has 1 bom).
  // An example use is a move operation.  The execVol provides the arrival period of the move.  The shifted
  // execVol returned by this method returns the execVol when the move starts.
  std::vector<float> getOffsetExecVol(const std::string & witOpName ) const;

  //------------------------
  bool witGetDemandExists(const std::string & demandedPartName,const std::string & demandName) const;
  bool witGetSubsBomEntryExists(
                                           const std::string & operationName,
                                           int bomEntry,
                                           int subEntry) const;
  static void compareWitSolutionsTimeVec(
                                const std::string & id,
                                const std::vector<float> & execTv1,
                                const std::vector<float> & execTv2, 
                                const std::vector<float> & costTv1,
                                const std::vector<float> & costTv2 );
  static void compareWitSolutions( const ESO2problem & p1,  const ESO2problem & p2 );
  //------------------------
  
  // Internal method used by pruning function
  void updateActivePartList(const std::string & partName,std::set<std::string> &activeParts) const;

  // Perform wit pegging and save results in eso2Problem data structures
  void doPegging();

private:
  // if the typedef is not used, then ms compiler will not compile program
  typedef std::set<std::string> ESOstringSet;
  typedef std::set<std::string>::iterator ESOstringSetIterator;
  typedef std::set<std::string>::const_iterator ESOstringSetConstIterator;
  typedef std::map<std::string,ESOstringSet> ESOmapStringToStringSet;
  typedef std::map<std::string,ESOstringSet>::iterator ESOmapStringToStringSetIterator;
  typedef std::map<std::string,ESOstringSet>::const_iterator ESOmapStringToStringSetConstIterator;

  typedef std::vector<ESO2partInterplant> ESO2partInterplantVec;
  typedef std::map<std::string,ESO2partInterplantVec> ESO2mapStringToPartInterplantVec;

  void addMtmAcquireLoc(const std::string & mtmSrcLoc, const std::string & mtmAcquireLoc ); 
  bool mtmAcquireLocExists(const std::string & mtmName, const std::string & mtmAcquireLoc ) const;
  bool mtmAcquireExists(const std::string & mtmName ) const;
  const ESOstringSet & getMtmAcquireLocs(const std::string & mtmSrcLoc)const;

  void addPartOrFeatureLoc(const std::string & partOrFeatureName, const std::string & loc );
  void removePartOrFeatureLoc(const std::string & partOrFeatureName, const std::string & loc );

  WitRun * mutableWitRun() const;
  WitRun * witRun();

  // return number of calls to witImplode
  int implosionCount() const;

  void gutsOfCopy( const ESO2problem& source );
  void gutsOfDestructor();

  mutable WitRun * wr_;
  int implosionCount_;

  int nPeriods_; // cached for performance

  std::set<std::string> mtmNames_; 
  std::set<std::string> featureNames_; 
  std::set<std::string> loosePartNames_; 
  std::set<std::string> xrefFeatures_; 
  std::set<std::string> alts_; 
  std::set<std::string> partInterplants_;  
  std::set<std::string> partDemands_; 

  bool demanBomAndFeatureXrefProdRatesAreSet_;

  ESOmapStringToStringSet mtmAcquireLocs_;
  ESOmapStringToStringSet partOrFeatureLocs_;  // given a part, what locations can it be found at.
  ESO2mapStringToPartInterplantVec defaultPartInterplants_;  

  ESOstringSet perishableSupplyWarningMsgTested_;

  int mtmPriority_;
  int featurePriority_;
  int loosePartPriority_;
  int mtm2Priority_;
  int feature2Priority_;
  int loosePart2Priority_;

  std::vector<float> wacScaleFactors_;
  std::set<std::pair<std::string,int> > wfmvLocScaleFactorSfDemandFilePair_;
  bool allowInventoryHoldingAtDemandLocation_;
  bool removeCycles_;
  bool useApplyScaleFactorLoc_;
  int period1SupplyPerishPeriod_;

};


#endif
