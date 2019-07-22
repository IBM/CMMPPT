// file: modelHlp.h (model helper)
// author: robin

#ifndef  LGFRSCEMODELHELPER_H
#define  LGFRSCEMODELHELPER_H

//#include <scenario/src/part.h>
#include <sce/src/sceParam.h>



// forward References
//class WitRun;


// SCE heuristic model helper class
// Has as it's member data all the
// default parameters for a non-optimization model
class LgFrSceHeurModelHelper {

public:
  
// default constructor
LgFrSceHeurModelHelper();

// general constructor
LgFrSceHeurModelHelper( LgFrSceSetOfParameters & setOfParameters);

  // get default priority
  const
  int
  defaultPriority () const ;

  // get default explicit bop preference
  const
  float
  defaultExplicitBopPreference () const ;

  // get default aggregate bop preference
  const
  float
  defaultAggregateBopPreference () const ;

  // get default interPlant bop preference
  const
  float
  defaultInterPlantBopPreference () const ;


  // get default default bop preference
  const
  float
  defaultDefaultBopPreference () const ;

  // get default default bop preference
  const
  int
  defaultShipLateLimit () const ;

  
  
// Return a copy in the heap
LgFrSceHeurModelHelper*
clone()
const;


// copy constructor
LgFrSceHeurModelHelper(const LgFrSceHeurModelHelper& source);

// assignment operator
LgFrSceHeurModelHelper&
operator=(const LgFrSceHeurModelHelper& rhs);

// destructor
virtual
~LgFrSceHeurModelHelper();

     
// self-test
static void test();

private:

  int defaultPriority_ ;
  float defaultExplicitBopPreference_ ;
  float defaultInterPlantBopPreference_ ;
  float defaultAggregateBopPreference_ ;
  float defaultDefaultBopPreference_ ;    
  int defaultShipLateLimit_ ;
};
// ----------
// SCE lp model helper class
// Has as it's member data all the
// default parameters for an optimization model
class LgFrSceLpModelHelper {

public:
  
// default constructor
LgFrSceLpModelHelper();

// general constructor
LgFrSceLpModelHelper(LgFrSceSetOfParameters & setOfParameters);

// get default obj1 ship reward
const
float
defaultObj1ShipReward () const ;

// get default periodic interest rate
const
float
periodicInterestRate () const ;  

// get default Stock Cost Real Parts
const
float
obj1DefaultStockCostRealParts () const ;  


// get default obj1 prod cost real parts
const
float
defaultObj1ProdCostRealParts () const ;
  
// get default obj1 prod cost disc rate real parts
const
float
obj1ProdCostDiscRateRealParts () const ;
  
// get default obj1 prod cost real parts exec cost real operations
const
float
defaultObj1ExecCostRealOperations () const ;
  
// get obj1 exec cost disc rate real ops
const
float
obj1ExecCostDiscRateRealOperations () const ;
  
// get option ration supply stock penalty
const
float
optionRatioSupplyStockPenalty () const ;
  
// get default option ship reward
const
float
defaultOptionShipReward () const ;

// get featureSetStockPenalty
const
float
featureSetStockPenalty () const ;

// get featureSetStockPenalty
const
float
featureSetScrapPenalty () const ;
  
// get featureSetStockPenalty
const
float
defaultSubPenalty () const ; 
  
  
// Return a copy in the heap
LgFrSceLpModelHelper*
clone()
const;


// copy constructor
LgFrSceLpModelHelper(const LgFrSceLpModelHelper& source);

// assignment operator
LgFrSceLpModelHelper&
operator=(const LgFrSceLpModelHelper& rhs);

// destructor
virtual
~LgFrSceLpModelHelper();

     
// self-test
static void test();

private:

  float defaultObj1ShipReward_ ;
  float periodicInterestRate_ ;
  float obj1DefaultStockCostRealParts_;
  float defaultObj1ProdCostRealParts_ ;
  float obj1ProdCostDiscRateRealParts_ ;
  float defaultObj1ExecCostRealOperations_ ;
  float obj1ExecCostDiscRateRealOperations_ ;
  float optionRatioSupplyStockPenalty_ ;
  float defaultOptionShipReward_ ;
  float featureSetStockPenalty_ ;
  float featureSetScrapPenalty_ ;
  float defaultSubPenalty_ ;

};

#endif
