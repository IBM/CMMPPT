\
// file: dmApData.C
// author: robin

#include <sce/src/dmApData.h>
#include <assert.h>



LgFrSceAppData *
LgFrSceDemandAppData::clone()
const
{
  LgFrSceDemandAppData * temp = new LgFrSceDemandAppData(*this);

  // nothing to deep copy

  return temp;
}



// always use this constructor
LgFrSceDemandAppData::LgFrSceDemandAppData(int hasDemandVolBeenSet,
					   int hasMinDemandVolBeenSet,
                                           size_t nPeriods, 
					   float backlogYieldDefault)
  :   aboveMinDemandVol_ (nPeriods, 0.0),
  backlogYield_ (nPeriods, backlogYieldDefault),
  demandVol_ (nPeriods, 0.0),
  allocVol_ (nPeriods, 0.0),
  pipPriority_ (nPeriods, 0.0),
  hasDemandVolBeenSet_(hasDemandVolBeenSet),
  hasMinDemandVolBeenSet_(hasMinDemandVolBeenSet),
  doesDemandHaveNegativeValues_(0),
  requestDate_(""),
  PIPConsVolSched_(),
  PIPSupplyVolSched_(),
  PIPExecVolSched_(),
  PIPInterplantVolSched_(),
  PIPProdVolSched_(),
  PIPSideVolSched_()
  
{
  // all the work is done in initializer
}


// a better constructor 
LgFrSceDemandAppData::LgFrSceDemandAppData(size_t nPeriods, float backlogYieldDefault)
  :   aboveMinDemandVol_ (nPeriods, 0.0),
  backlogYield_ (nPeriods, backlogYieldDefault),
  demandVol_ (nPeriods, 0.0),
  allocVol_ (nPeriods, 0.0),
  pipPriority_ (nPeriods, 0.0),
  hasDemandVolBeenSet_(0),
  hasMinDemandVolBeenSet_(0),
  doesDemandHaveNegativeValues_(0),
  requestDate_(""),
  PIPConsVolSched_(),
  PIPSupplyVolSched_(),
  PIPExecVolSched_(),
  PIPInterplantVolSched_(),
  PIPProdVolSched_(),
  PIPSideVolSched_()

{
  // all the work is done in initializer
}

// default constructor 
LgFrSceDemandAppData::LgFrSceDemandAppData()
  :   aboveMinDemandVol_ (),
  backlogYield_ (),
  demandVol_ (),
  allocVol_ (),
  pipPriority_ (),
  hasDemandVolBeenSet_(0),
  hasMinDemandVolBeenSet_(0),
  doesDemandHaveNegativeValues_(0),
  requestDate_(""),
  PIPConsVolSched_(),
  PIPSupplyVolSched_(),
  PIPExecVolSched_(),
  PIPInterplantVolSched_(),
  PIPProdVolSched_(),
  PIPSideVolSched_()

{
  assert (1 == 0); // don't use this constructor
}


// get "doesDemandHaveNegativeValues"
int
LgFrSceDemandAppData::doesDemandHaveNegativeValues() const
{
  return doesDemandHaveNegativeValues_;
}

// set "doesDemandHaveNegativeValues"
void
LgFrSceDemandAppData::doesDemandHaveNegativeValues(int doesDemandHaveNegativeValues)
{
  doesDemandHaveNegativeValues_ = doesDemandHaveNegativeValues;
}


// get "hasDemandVolBeenSet"
int
LgFrSceDemandAppData::hasDemandVolBeenSet() const
{
  return hasDemandVolBeenSet_;
}

// set "hasDemandVolBeenSet"
void
LgFrSceDemandAppData::hasDemandVolBeenSet(int hasDemandVolBeenSet)
{
  hasDemandVolBeenSet_ = hasDemandVolBeenSet;
}

// get "hasMinDemandVolBeenSet"
int
LgFrSceDemandAppData::hasMinDemandVolBeenSet() const
{
  return hasMinDemandVolBeenSet_;
}

// set "hasMinDemandVolBeenSet"
void
LgFrSceDemandAppData::hasMinDemandVolBeenSet(int hasMinDemandVolBeenSet)
{
  hasMinDemandVolBeenSet_ = hasMinDemandVolBeenSet;
}




// get "above minimum demand" vols
const
LgFrTimeVecFloat&
LgFrSceDemandAppData::aboveMinDemandVol () const
{
  return aboveMinDemandVol_;
}

// set "above minimum demand" vols
void
LgFrSceDemandAppData::aboveMinDemandVol (const LgFrTimeVecFloat & amDemand)
{                                                  
  aboveMinDemandVol_ = amDemand ;
}

// get backlog yield
const
LgFrTimeVecFloat&  
LgFrSceDemandAppData::backlogYield () const
{
  return backlogYield_ ;
}

// set backlog yield
void
LgFrSceDemandAppData::backlogYield (const LgFrTimeVecFloat & bklgYld)
{
  backlogYield_ = bklgYld ;
}

// get demandVol
const
LgFrTimeVecFloat&  
LgFrSceDemandAppData::demandVol () const
{
  return demandVol_ ;
}

// set demandVol
void
LgFrSceDemandAppData::demandVol (const LgFrTimeVecFloat & dVol)
{
  demandVol_ = dVol ;
}

// get allocVol
const
LgFrTimeVecFloat&  
LgFrSceDemandAppData::allocVol () const
{
  return allocVol_ ;
}

// set allocVol
void
LgFrSceDemandAppData::allocVol (const LgFrTimeVecFloat & dVol)
{
  allocVol_ = dVol ;
}

// get pipPriority
const
LgFrTimeVecFloat&  
LgFrSceDemandAppData::pipPriority () const
{
  return pipPriority_ ;
}

// set pipPriority
void
LgFrSceDemandAppData::pipPriority (const LgFrTimeVecFloat & pP)
{
  pipPriority_ = pP ;
}


// get requestDate
const
std::string&  
LgFrSceDemandAppData::requestDate () const
{
  return requestDate_ ;
}

// set requestDate
void
LgFrSceDemandAppData::requestDate (const std::string & requestDate)
{
  requestDate_ = requestDate ;
}


// get 
LgFrSortingPartScheduleFloat &  
LgFrSceDemandAppData::PIPSched(int pegType)
{
  switch (pegType)
    {
    case PEG_TYPE_SUPPLY:
      return PIPSupplyVolSched_;
    case PEG_TYPE_CONSVOL:
      return PIPConsVolSched_;
    case PEG_TYPE_EXECVOL:
      return PIPExecVolSched_;
    case PEG_TYPE_INTERPLANT:
      return PIPInterplantVolSched_;
    case PEG_TYPE_PRODVOL:
      return PIPProdVolSched_;
    case PEG_TYPE_SIDEVOL:
      return PIPSideVolSched_;
    }
  
}




// get 
LgFrSortingPartScheduleFloat &  
LgFrSceDemandAppData::PIPConsVolSched()
{
  return PIPConsVolSched_;
}


// get 
LgFrSortingPartScheduleFloat &  
LgFrSceDemandAppData::PIPSupplyVolSched()
{
  return PIPSupplyVolSched_;
}



// get 
LgFrSortingPartScheduleFloat &  
LgFrSceDemandAppData::PIPExecVolSched()
{
  return PIPExecVolSched_;
}

// get 
LgFrSortingPartScheduleFloat &  
LgFrSceDemandAppData::  PIPInterplantVolSched()
{
  return PIPInterplantVolSched_;
}


// get 
LgFrSortingPartScheduleFloat &  
LgFrSceDemandAppData::PIPProdVolSched()
{
  return PIPProdVolSched_;
}

// get 
LgFrSortingPartScheduleFloat &  
LgFrSceDemandAppData::PIPSideVolSched()
{
  return PIPSideVolSched_;
}



// assignment operator
LgFrSceDemandAppData&
LgFrSceDemandAppData::operator=(const LgFrSceDemandAppData& rhs)

{
  if (this != &rhs) {		// Check for assignment to self
    aboveMinDemandVol_ = rhs.aboveMinDemandVol_;
    backlogYield_ = rhs.backlogYield_;
    demandVol_ = rhs.demandVol_;
    allocVol_ = rhs.allocVol_;
    pipPriority_ = rhs.pipPriority_;
    hasDemandVolBeenSet_ = rhs.hasDemandVolBeenSet_;
    hasMinDemandVolBeenSet_ = rhs.hasMinDemandVolBeenSet_;
    requestDate_ = rhs.requestDate_;
    PIPConsVolSched_ = rhs.PIPConsVolSched_;
    PIPSupplyVolSched_ = rhs.PIPSupplyVolSched_;
    PIPExecVolSched_ = rhs.PIPExecVolSched_;
    PIPInterplantVolSched_ = rhs.PIPInterplantVolSched_;
    PIPProdVolSched_ = rhs.PIPProdVolSched_;
    PIPSideVolSched_ = rhs.PIPSideVolSched_;

  }
  return *this;
}

// copy constructor 
LgFrSceDemandAppData::LgFrSceDemandAppData(const LgFrSceDemandAppData& source)
  :   aboveMinDemandVol_ (source.aboveMinDemandVol_),
  backlogYield_ (source.backlogYield_),
  demandVol_ (source.demandVol_),
  allocVol_ (source.allocVol_),
  pipPriority_ (source.pipPriority_),
  hasDemandVolBeenSet_ (source.hasDemandVolBeenSet_),
  hasMinDemandVolBeenSet_ (source.hasMinDemandVolBeenSet_),
  requestDate_ (source.requestDate_),
  PIPConsVolSched_ (source.PIPConsVolSched_),
  PIPSupplyVolSched_ (source.PIPSupplyVolSched_),
  PIPExecVolSched_ (source.PIPExecVolSched_),
  PIPInterplantVolSched_ (source.PIPInterplantVolSched_),
  PIPProdVolSched_ (source.PIPProdVolSched_),
  PIPSideVolSched_ (source.PIPSideVolSched_)

{
  // nothing to do
}

// destructor
LgFrSceDemandAppData::~LgFrSceDemandAppData()
{
  
}
