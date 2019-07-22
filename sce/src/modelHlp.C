 // file: modelHlp.C  (model helper)
// author: robin

#include <stdlib.h>
#include <assert.h>
#include <mcl/src/mcl.h>
#include <scenario/src/machdep.h>
#include <sce/src/sceDefin.h>
#include <sce/src/modelHlp.h>

#ifndef ELIMINATE_OLD_MAIN

// default constructor:
LgFrSceHeurModelHelper::LgFrSceHeurModelHelper()
// JP suggests a more elegant way to do this is to make
// the default constructor "private".
{
  // don't use the default constructor
  assert (1 == 0);
}


// fairly general constructor:
LgFrSceHeurModelHelper::LgFrSceHeurModelHelper( LgFrSceSetOfParameters & setOfParameters )
 :
  defaultPriority_(setOfParameters.getInt("defaultPriority")),
  defaultExplicitBopPreference_(setOfParameters.getFloat("defaultExplicitBopPreference")),
  defaultInterPlantBopPreference_(setOfParameters.getFloat("defaultInterPlantBopPreference")),
  defaultAggregateBopPreference_(setOfParameters.getFloat("defaultAggregateBopPreference")),
  defaultDefaultBopPreference_(setOfParameters.getFloat("defaultDefaultBopPreference")),
  defaultShipLateLimit_(setOfParameters.getInt("defaultShipLateLimit"))
{
    // all the work is done in initializer
}

// get the default priority
const
int
LgFrSceHeurModelHelper::defaultPriority () const
{
  return defaultPriority_;
}


// get default explicit Bop preference
const
float
LgFrSceHeurModelHelper::defaultExplicitBopPreference () const
{
  return defaultExplicitBopPreference_;
}

// get default aggregate Bop preference
const
float
LgFrSceHeurModelHelper::defaultAggregateBopPreference () const
{
  return defaultAggregateBopPreference_;
}


// get default interplant Bop preference
const
float
LgFrSceHeurModelHelper::defaultInterPlantBopPreference () const
{
  return defaultInterPlantBopPreference_;
}


// get default Default Bop preference
const
float
LgFrSceHeurModelHelper::defaultDefaultBopPreference () const
{
  return defaultDefaultBopPreference_;
}

// get default ship late limit
const
int
LgFrSceHeurModelHelper::defaultShipLateLimit () const
{
  return defaultShipLateLimit_;
}

// Return a copy in the heap
LgFrSceHeurModelHelper*
LgFrSceHeurModelHelper::clone()
     const
{
  return new LgFrSceHeurModelHelper(*this);
}



 // copy constructor
LgFrSceHeurModelHelper::LgFrSceHeurModelHelper(const LgFrSceHeurModelHelper& source)
  :
  defaultPriority_(source.defaultPriority_),
  defaultExplicitBopPreference_(source.defaultExplicitBopPreference_),
  defaultInterPlantBopPreference_(source.defaultInterPlantBopPreference_),
  defaultAggregateBopPreference_(source.defaultAggregateBopPreference_),
  defaultDefaultBopPreference_(source.defaultDefaultBopPreference_),
  defaultShipLateLimit_(source.defaultShipLateLimit_)
{
  // nothing to do
}

// assignment operator
LgFrSceHeurModelHelper&
LgFrSceHeurModelHelper::operator=(const LgFrSceHeurModelHelper& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    defaultPriority_ = rhs.defaultPriority_;
    defaultExplicitBopPreference_ = rhs.defaultExplicitBopPreference_;
    defaultInterPlantBopPreference_ = rhs.defaultInterPlantBopPreference_;
    defaultAggregateBopPreference_ = rhs.defaultAggregateBopPreference_;
    defaultDefaultBopPreference_= rhs.defaultDefaultBopPreference_;
    defaultShipLateLimit_= rhs.defaultShipLateLimit_;
    
  }
  return *this;
}

// destructor
LgFrSceHeurModelHelper::~LgFrSceHeurModelHelper()
{
  // nothing to do
}


#ifdef NDEBUG
#undef NDEBUG
#endif



// self-test
void
LgFrSceHeurModelHelper::test()
{
  // do nothing, for now 
 }

// ----------------------------------------------------------------------

// default constructor:
LgFrSceLpModelHelper::LgFrSceLpModelHelper()

{
  // don't use the default constructor
  assert (1 == 0); 
}


// fairly general constructor:
LgFrSceLpModelHelper::LgFrSceLpModelHelper( LgFrSceSetOfParameters & setOfParameters)
:
  defaultObj1ShipReward_(setOfParameters.getFloat("defaultObj1ShipReward")),
  periodicInterestRate_(setOfParameters.getFloat("periodicInterestRate")), 
  obj1DefaultStockCostRealParts_(setOfParameters.getFloat("obj1DefaultStockCostRealParts")), 
  defaultObj1ProdCostRealParts_(setOfParameters.getFloat("defaultObj1ProdCostRealParts")), 
  obj1ProdCostDiscRateRealParts_(setOfParameters.getFloat("obj1ProdCostDiscRateRealParts")), 
  defaultObj1ExecCostRealOperations_(setOfParameters.getFloat("defaultObj1ExecCostRealOperations")), 
  obj1ExecCostDiscRateRealOperations_(setOfParameters.getFloat("obj1ExecCostDiscRateRealOperations")),
  optionRatioSupplyStockPenalty_(setOfParameters.getFloat("optionRatioSupplyStockPenalty")),
  defaultOptionShipReward_(setOfParameters.getFloat("defaultOptionShipReward")), 
  featureSetStockPenalty_(setOfParameters.getFloat("featureSetStockPenalty")), 
  featureSetScrapPenalty_(setOfParameters.getFloat("featureSetScrapPenalty")), 
  defaultSubPenalty_(setOfParameters.getFloat("defaultSubPenalty"))
{
    // all the work is done in initializer
}


// get default obj1 ship reward
const
float
LgFrSceLpModelHelper::defaultObj1ShipReward () const
{
  return defaultObj1ShipReward_;
}


// get default period interest rate 
const
float
LgFrSceLpModelHelper::periodicInterestRate () const
{
  return periodicInterestRate_;
}

// get default stock cost real parts 
const
float
LgFrSceLpModelHelper::obj1DefaultStockCostRealParts () const
{
  return obj1DefaultStockCostRealParts_;
}

// get default obj1 prod cost for real parts
const
float
LgFrSceLpModelHelper::defaultObj1ProdCostRealParts () const
{
  return defaultObj1ProdCostRealParts_;
}


// get default obj1 prod cost discount rate for real parts
const
float
LgFrSceLpModelHelper::obj1ProdCostDiscRateRealParts () const
{
  return obj1ProdCostDiscRateRealParts_;
}

// get default obj1 execution cost 
// for "real" operations
const
float
LgFrSceLpModelHelper::defaultObj1ExecCostRealOperations () const
{
  return defaultObj1ExecCostRealOperations_;
}

// get default obj1 execution cost discount rate
// for "real" operations
const
float
LgFrSceLpModelHelper::obj1ExecCostDiscRateRealOperations () const
{
  return obj1ExecCostDiscRateRealOperations_;
}

// get default option ratio supply stock penalty
const
float
LgFrSceLpModelHelper::optionRatioSupplyStockPenalty () const
{
  return optionRatioSupplyStockPenalty_;
}

// get default option ship reward
const
float
LgFrSceLpModelHelper::defaultOptionShipReward() const
{
  return defaultOptionShipReward_;
}

// get default feature set stock penalty
const
float
LgFrSceLpModelHelper::featureSetStockPenalty() const
{
  return featureSetStockPenalty_;
}

// get default feature set scrap penalty
const
float
LgFrSceLpModelHelper::featureSetScrapPenalty() const
{
  return featureSetScrapPenalty_;
}

// get default sub penalty set scrap penalty
const
float
LgFrSceLpModelHelper::defaultSubPenalty() const
{
  return defaultSubPenalty_;
}


// Return a copy in the heap
LgFrSceLpModelHelper*
LgFrSceLpModelHelper::clone()
     const
{
  return new LgFrSceLpModelHelper(*this);
}

// copy constructor
LgFrSceLpModelHelper::LgFrSceLpModelHelper(const LgFrSceLpModelHelper& source)
:
  defaultObj1ShipReward_(source.defaultObj1ShipReward_),
  periodicInterestRate_(source.periodicInterestRate_), 
  obj1DefaultStockCostRealParts_(source.obj1DefaultStockCostRealParts_), 
  defaultObj1ProdCostRealParts_(source.defaultObj1ProdCostRealParts_), 
  obj1ProdCostDiscRateRealParts_(source.obj1ProdCostDiscRateRealParts_), 
  defaultObj1ExecCostRealOperations_(source.defaultObj1ExecCostRealOperations_), 
  obj1ExecCostDiscRateRealOperations_(source.obj1ExecCostDiscRateRealOperations_),
  optionRatioSupplyStockPenalty_(source.optionRatioSupplyStockPenalty_),
  defaultOptionShipReward_(source.defaultOptionShipReward_), 
  featureSetStockPenalty_(source.featureSetStockPenalty_), 
  featureSetScrapPenalty_(source.featureSetScrapPenalty_), 
  defaultSubPenalty_(source.defaultSubPenalty_)

{
  // nothing to do
}

// assignment operator
LgFrSceLpModelHelper&
LgFrSceLpModelHelper::operator=(const LgFrSceLpModelHelper& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    defaultObj1ShipReward_ = rhs.defaultObj1ShipReward_;
    periodicInterestRate_ = rhs.periodicInterestRate_;
    obj1DefaultStockCostRealParts_ = rhs.obj1DefaultStockCostRealParts_;
    defaultObj1ProdCostRealParts_ = rhs.defaultObj1ProdCostRealParts_;
    obj1ProdCostDiscRateRealParts_ = rhs.obj1ProdCostDiscRateRealParts_;
    defaultObj1ExecCostRealOperations_= rhs.defaultObj1ExecCostRealOperations_;
    obj1ExecCostDiscRateRealOperations_= rhs.obj1ExecCostDiscRateRealOperations_;
    optionRatioSupplyStockPenalty_ = rhs.optionRatioSupplyStockPenalty_;
    defaultOptionShipReward_ = rhs.defaultOptionShipReward_;
    featureSetStockPenalty_ = rhs.featureSetStockPenalty_;
    featureSetScrapPenalty_ = rhs.featureSetScrapPenalty_; 
    defaultSubPenalty_ = rhs.defaultSubPenalty_;
  }
  return *this;
}

// destructor
LgFrSceLpModelHelper::~LgFrSceLpModelHelper()
{
  // nothing to do
}


#ifdef NDEBUG
#undef NDEBUG
#endif



// self-test
void
LgFrSceLpModelHelper::test()
{
  // do nothing, for now 
 }



#endif

