// file: appData.C
// author: tom and robin

// appData objects for:
//    parts
//    subBomEntries
//    operations

#include <sce/src/appData.h>
#include <sce/src/baseAppData.h>
#include <assert.h>




LgFrSceAppData *
LgFrScePartAppData::clone()
const
{
  LgFrScePartAppData * temp = new LgFrScePartAppData(*this);

  // nothing to deep copy

  return temp;
}





// a better constructor 
LgFrScePartAppData::LgFrScePartAppData(int isPartPcf, size_t unConstrainedStartPeriod)
  :   isPartPcf_ (isPartPcf),
#ifdef MARK_BOGONS
  isPartBogon_ (0),
#endif
  unConstrainedStartPeriod_(unConstrainedStartPeriod),
  nInterplantOps_ (0),
  nAlternatePartOps_ (0),
  nUserDefinedOps_ (0),
  nAggregationOps_ (0),
  index_(0),
  maxWithout_(),
  doesPartHaveNegativeDemands_(0),
  vac_(),
  compCost_(),
  sourceCode_("AF"),
  cycleTimeDays_(),
  cycleTime_()
{
  // all the work is done in initializer
  std::cout << "Warning using an obsolete constructor for LgFrScePartAppData" << std::endl;
}

// best constructor
LgFrScePartAppData::LgFrScePartAppData(int isPartPcf, size_t unConstrainedStartPeriod, int nPeriods)
  :   isPartPcf_ (isPartPcf),
#ifdef MARK_BOGONS
  isPartBogon_ (0),
#endif
  unConstrainedStartPeriod_(unConstrainedStartPeriod),
  nInterplantOps_ (0),
  nAlternatePartOps_ (0),
  nUserDefinedOps_ (0),
  nAggregationOps_ (0),
  index_(0),
  maxWithout_(),
  doesPartHaveNegativeDemands_(0),
  vac_(),
  compCost_(),
  sourceCode_("AF"),
  cycleTimeDays_(nPeriods,0.0),
  cycleTime_(nPeriods,0.0)
{
  // all the work is done in initializer

}




// default constructor 
LgFrScePartAppData::LgFrScePartAppData()
  :   isPartPcf_ (0),
#ifdef MARK_BOGONS
  isPartBogon_ (0),
#endif
  unConstrainedStartPeriod_ (0),
  nInterplantOps_ (0),
  nAlternatePartOps_ (0),
  nUserDefinedOps_ (0),
  nAggregationOps_ (0),
  index_ (0),
  maxWithout_(),
  doesPartHaveNegativeDemands_(0),
  vac_(),
  compCost_(),
  sourceCode_("AF"),
  cycleTimeDays_(),
  cycleTime_()
{
  // do nothing
  std::cout << "Warning using an obsolete constructor for LgFrScePartAppData" << std::endl;
}

// get isPartPcf
int
LgFrScePartAppData::isPartPcf () const
{
  return isPartPcf_;
}

// set Part as Pcf
void
LgFrScePartAppData::isPartPcf (int isPartPcf)
{                                                  
  isPartPcf_ = isPartPcf ;
}

#ifdef MARK_BOGONS
// get isPartBogon
int
LgFrScePartAppData::isPartBogon () const
{
  return isPartBogon_;
}

// set Part as Bogon
void
LgFrScePartAppData::isPartBogon (int isPartBogon)
{                                                  
  isPartBogon_ = isPartBogon ;
}
#endif




// get unConstrainedStartPeriod
size_t
LgFrScePartAppData::unConstrainedStartPeriod() const
{
  return unConstrainedStartPeriod_;
}

// set unConstrainedStartPeriod
void
LgFrScePartAppData::unConstrainedStartPeriod(size_t unConstrainedStartPeriod)
{                                                  
  unConstrainedStartPeriod_ = unConstrainedStartPeriod;
}



// nInterplantOps (number of UNIQUE interplant operations that produce this part)
// get
int
LgFrScePartAppData::nInterplantOps()
{
  return nInterplantOps_;
}
  
// set
void
LgFrScePartAppData::nInterplantOps(int nOps)
{
  nInterplantOps_ = nOps;
}

// nAlternatePartOps (number of UNIQUE alternate operations that produce this part)
// get
int
LgFrScePartAppData::nAlternatePartOps()
{
  return nAlternatePartOps_;
}
  
// set
void
LgFrScePartAppData::nAlternatePartOps(int nOps)
{
  nAlternatePartOps_ = nOps;
}




// nUserDefinedOps (number of UNIQUE user-defined operations that produce this part)
// get
int
LgFrScePartAppData::nUserDefinedOps()
{
  return nUserDefinedOps_;
}
  
// set
void
LgFrScePartAppData::nUserDefinedOps(int nOps)
{
  nUserDefinedOps_ = nOps;
}

// nAggregationOps (number of UNIQUE aggregation operations that produce this part)
// get
int
LgFrScePartAppData::nAggregationOps()
{
  return nAggregationOps_;
}
  
// set
void
LgFrScePartAppData::nAggregationOps(int nOps)
{
  nAggregationOps_ = nOps;
}


// index (only for use in MRP Demand ORders File)
int
LgFrScePartAppData::index()
{
  return index_;
}

void
LgFrScePartAppData::index(int index)
{
  index_ = index;
}

// get maxWithout
const
LgFrTimeVecFloat &
LgFrScePartAppData::maxWithout() const
{
  return maxWithout_;
}

// set maxWithout
void
LgFrScePartAppData::maxWithout(const LgFrTimeVecFloat &  maxWithout)
{
  maxWithout_ = maxWithout;
}

// get doesPartHaveNegativeDemands
int
LgFrScePartAppData::doesPartHaveNegativeDemands() const
{
  return doesPartHaveNegativeDemands_;
}

// set doesPartHaveNegativeDemands
void
LgFrScePartAppData::doesPartHaveNegativeDemands(int doesPartHaveNegativeDemands)
{
  doesPartHaveNegativeDemands_ = doesPartHaveNegativeDemands;
}

// get vac
const
LgFrTimeVecFloat &
LgFrScePartAppData::vac() const
{
  return vac_;
}

// set vac
void
LgFrScePartAppData::vac(const LgFrTimeVecFloat &  vac)
{
  vac_ = vac;
}

// get compCost
const
LgFrTimeVecFloat &
LgFrScePartAppData::compCost() const
{
  return compCost_;
}

// set compCost
void
LgFrScePartAppData::compCost(const LgFrTimeVecFloat &  compCost)
{
  compCost_ = compCost;
}

// get sourceCode
const
std::string &
LgFrScePartAppData::sourceCode() const
{
  return sourceCode_;
}

// set sourceCode
void
LgFrScePartAppData::sourceCode(const std::string &  sourceCode)
{
  sourceCode_ = sourceCode;
}

// get cycleTimeDays
const
LgFrTimeVecFloat &
LgFrScePartAppData::cycleTimeDays() const
{
  return cycleTimeDays_;
}

// set cycleTimeDays
void
LgFrScePartAppData::cycleTimeDays(const LgFrTimeVecFloat &  cycleTimeDays)
{
  cycleTimeDays_ = cycleTimeDays;
}

// get cycleTime
const
LgFrTimeVecFloat &
LgFrScePartAppData::cycleTime() const
{
  return cycleTime_;
}

// set cycleTime
void
LgFrScePartAppData::cycleTime(const LgFrTimeVecFloat &  cycleTime)
{
  cycleTime_ = cycleTime;
}


// assignment operator
LgFrScePartAppData&
LgFrScePartAppData::operator=(const LgFrScePartAppData& rhs)

{
  if (this != &rhs) {		// Check for assignment to self
    isPartPcf_ = rhs.isPartPcf_;
#ifdef MARK_BOGONS
    isPartBogon_  = rhs.isPartBogon_;
#endif
    unConstrainedStartPeriod_ = rhs.unConstrainedStartPeriod_;
    nInterplantOps_ = rhs.nInterplantOps_;
    nAlternatePartOps_ = rhs.nAlternatePartOps_;
    nUserDefinedOps_ = rhs.nUserDefinedOps_;
    nAggregationOps_ = rhs.nAggregationOps_;
    index_ = rhs.index_;
    maxWithout_ = rhs.maxWithout_;
    doesPartHaveNegativeDemands_ = rhs.doesPartHaveNegativeDemands_;    
    vac_ = rhs.vac_;    
    compCost_ = rhs.compCost_;    
    sourceCode_ = rhs.sourceCode_;
    cycleTimeDays_ = rhs.cycleTimeDays_;    
    cycleTime_ = rhs.cycleTime_;    
  }
  return *this;
}

// copy constructor 
LgFrScePartAppData::LgFrScePartAppData(const LgFrScePartAppData& source)
  :   isPartPcf_ (source.isPartPcf_),
#ifdef MARK_BOGONS
  isPartBogon_ (source.isPartBogon_),
#endif
  unConstrainedStartPeriod_(source.unConstrainedStartPeriod_),
  nInterplantOps_(source.nInterplantOps_),
  nAlternatePartOps_(source.nAlternatePartOps_),
  nUserDefinedOps_(source.nUserDefinedOps_),
  nAggregationOps_(source.nAggregationOps_),
  index_(source.index_),
  maxWithout_(source.maxWithout_),
  doesPartHaveNegativeDemands_(source.doesPartHaveNegativeDemands_),
  vac_(source.vac_),
  compCost_(source.compCost_),
  sourceCode_(source.sourceCode_),
  cycleTimeDays_(source.cycleTimeDays_),
  cycleTime_(source.cycleTime_)
{
  // nothing to do
}

// destructor
LgFrScePartAppData::~LgFrScePartAppData()
{
  // nothing to do, 
}






// ---------------------------------------------------------------------
//
// Special Customer Choice Operation  app data
//
// ---------------------------------------------------------------------

LgFrSceAppData *
LgFrSceCCOperationAppData::clone()
const
{
  LgFrSceCCOperationAppData * temp = new LgFrSceCCOperationAppData(*this);

  // nothing to deep copy

  return temp;
}


// a better constructor 
LgFrSceCCOperationAppData::LgFrSceCCOperationAppData(int index)
  :   index_(index)
{
  // all the work is done in initializer
}

// default constructor 
LgFrSceCCOperationAppData::LgFrSceCCOperationAppData()
  :   index_ (0)
{
  // do nothing
}


// index 
int
LgFrSceCCOperationAppData::index()
{
  return index_;
}


void
LgFrSceCCOperationAppData::index(int index)
{
  index_ = index;
}


// assignment operator
LgFrSceCCOperationAppData&
LgFrSceCCOperationAppData::operator=(const LgFrSceCCOperationAppData& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    index_ = rhs.index_;
  }
  return *this;
}

// copy constructor 
LgFrSceCCOperationAppData::LgFrSceCCOperationAppData(const LgFrSceCCOperationAppData& source)
  :   index_(source.index_)
{
  // nothing to do
}

// destructor
LgFrSceCCOperationAppData::~LgFrSceCCOperationAppData()
{
  // nothing to do, 
}













// --------------------------------------------------------------------
//
//  sub bom app data
//
// --------------------------------------------------------------------

LgFrSceAppData *
LgFrSceSubBomAppData::clone()
const
{
  LgFrSceSubBomAppData * temp = new LgFrSceSubBomAppData(*this);

  // nothing to deep copy

  return temp;
}



// a better constructor 
LgFrSceSubBomAppData::LgFrSceSubBomAppData(float defaultPreference, int isOptExplodableFlag)
  :   preference_ (defaultPreference),
      isObjectInEffectForOptExplosion_ (isOptExplodableFlag)
      
{
  // all the work is done in initializer
}

// default constructor 
LgFrSceSubBomAppData::LgFrSceSubBomAppData()
  :   preference_ (0.0),
      isObjectInEffectForOptExplosion_ (0)
{
  // nothing to do
}

// get "preference" 
const
float
LgFrSceSubBomAppData::preference () const
{
  return preference_;
}

// set "preference"
void
LgFrSceSubBomAppData::preference (const float pref)
{                                                  
  preference_ = pref ;
}

// get "isObjectInEffectForOptExplosion" 
const
int
LgFrSceSubBomAppData::isObjectInEffectForOptExplosion () const
{
  return isObjectInEffectForOptExplosion_;
}

// set "isObjectInEffectForOptExplosion"
void
LgFrSceSubBomAppData::isObjectInEffectForOptExplosion (const int isOptExplodableFlag)
{                                                  
  isObjectInEffectForOptExplosion_ = isOptExplodableFlag ;
}



// assignment operator
LgFrSceSubBomAppData&
LgFrSceSubBomAppData::operator=(const LgFrSceSubBomAppData& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    preference_ = rhs.preference_ ;
    isObjectInEffectForOptExplosion_ = rhs.isObjectInEffectForOptExplosion_ ;
  }
  return *this;
}

// copy constructor 
LgFrSceSubBomAppData::LgFrSceSubBomAppData(const LgFrSceSubBomAppData& source)
  :   preference_ (source.preference_),
      isObjectInEffectForOptExplosion_ (source.isObjectInEffectForOptExplosion_)
{
  // nothing to do
}

// destructor
LgFrSceSubBomAppData::~LgFrSceSubBomAppData()
{
  // nothing to do, 
}

// --------------------------------------------------------------------


// --------------------------------------------------------------------
//
//  customer choice BOM App data ...
//
// --------------------------------------------------------------------

LgFrSceAppData *
LgFrSceCustChoiceBomAppData::clone()
const
{
  LgFrSceCustChoiceBomAppData * temp = new LgFrSceCustChoiceBomAppData(*this);

  // nothing to deep copy

  return temp;
}



// a better constructor 
LgFrSceCustChoiceBomAppData::LgFrSceCustChoiceBomAppData(LgFrTimeVecFloat & featRatio)
  :   featRatio_ (featRatio),
      dummySupplyVolForMrpAdjustment_(0,0.0)
{
  // all the work is done in initializer
}

// default constructor 
LgFrSceCustChoiceBomAppData::LgFrSceCustChoiceBomAppData()
  :   featRatio_ (0.0),
      dummySupplyVolForMrpAdjustment_(0,0.0)
{
  // nothing to do
}

// get featratio
const
LgFrTimeVecFloat &
LgFrSceCustChoiceBomAppData::featRatio () const
{
  return featRatio_;
}

// set "featRatio"
void
LgFrSceCustChoiceBomAppData::featRatio (const LgFrTimeVecFloat & featRatio)
{                                                  
  featRatio_ = featRatio;
}


// get the dummySupplyVolForMrpAdjustment
const
LgFrTimeVecFloat&
LgFrSceCustChoiceBomAppData::dummySupplyVolForMrpAdjustment() const
{
  return dummySupplyVolForMrpAdjustment_;
}

  // set  the dummySupplyVolForMrpAdjustment
void
LgFrSceCustChoiceBomAppData::dummySupplyVolForMrpAdjustment(const LgFrTimeVecFloat & dummySupplyVolForMrpAdjustment)
{                                                  
  dummySupplyVolForMrpAdjustment_ = dummySupplyVolForMrpAdjustment;
}


// assignment operator
LgFrSceCustChoiceBomAppData&
LgFrSceCustChoiceBomAppData::operator=(const LgFrSceCustChoiceBomAppData& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    featRatio_ = rhs.featRatio_ ;
    dummySupplyVolForMrpAdjustment_ = rhs.dummySupplyVolForMrpAdjustment_;
  }
  return *this;
}

// copy constructor 
LgFrSceCustChoiceBomAppData::LgFrSceCustChoiceBomAppData(const LgFrSceCustChoiceBomAppData& source)
  :   featRatio_ (source.featRatio_),
      dummySupplyVolForMrpAdjustment_ (source.dummySupplyVolForMrpAdjustment_)
{
  // nothing to do
}

// destructor
LgFrSceCustChoiceBomAppData::~LgFrSceCustChoiceBomAppData()
{
  // nothing to do, 
}

// --------------------------------------------------------------------




//
// --------------------------------------------------------------------
//
//  (interplant) operation data
//
// --------------------------------------------------------------------


LgFrSceAppData *
LgFrSceInterplantOpAppData::clone()
const
{
  LgFrSceInterplantOpAppData * temp = new LgFrSceInterplantOpAppData(*this);

  // nothing to deep copy

  return temp;
}



// an even better constructor 
LgFrSceInterplantOpAppData::LgFrSceInterplantOpAppData( LgFrTimeVecFloat mrpExecVol)
  :   mrpExecVol_ (mrpExecVol),
      execVol_ (0)
{
  // all the work is done in initializer
}


// default constructor 
LgFrSceInterplantOpAppData::LgFrSceInterplantOpAppData()
  :   mrpExecVol_ (),
      execVol_()

{
  assert (1 == 0); // don't use this constructor
}

// get "mrp jumper arc adjusted exec vols"
const
LgFrTimeVecFloat&
LgFrSceInterplantOpAppData::mrpExecVol () const
{
  return mrpExecVol_;
}

// get " jumper arc adjusted exec vols"
const
LgFrTimeVecFloat&
LgFrSceInterplantOpAppData::execVol () const
{
  return execVol_;
}

// set "mrp jumper arc adjusted exec vols"
void
LgFrSceInterplantOpAppData::mrpExecVol (const LgFrTimeVecFloat & mrpExecVol)
{                                                  
  mrpExecVol_ = mrpExecVol ;
}

// set "jumper arc adjusted exec vols"
void
LgFrSceInterplantOpAppData::execVol (const LgFrTimeVecFloat & execVol)
{                                                  
  execVol_ = execVol ;
}

// assignment operator
LgFrSceInterplantOpAppData&
LgFrSceInterplantOpAppData::operator=(const LgFrSceInterplantOpAppData& rhs)

{
  if (this != &rhs) {		// Check for assignment to self
    mrpExecVol_ = rhs.mrpExecVol_;
    execVol_ = rhs.execVol_;
  }
  return *this;
}

// copy constructor 
LgFrSceInterplantOpAppData::LgFrSceInterplantOpAppData(const LgFrSceInterplantOpAppData& source)
  :   mrpExecVol_ (source.mrpExecVol_),
      execVol_ (source.execVol_)
{
  // nothing to do
}

// destructor
LgFrSceInterplantOpAppData::~LgFrSceInterplantOpAppData()
{
  // nothing to do, 
}





#ifdef NDEBUG
#undef NDEBUG
#endif


// self-test 
void
LgFrScePartAppData::test()
{
  // create default
  LgFrScePartAppData myPartAppData;
  assert(myPartAppData.isPartPcf() == 0);
  assert(myPartAppData.unConstrainedStartPeriod() == 0);

  // set methods
  myPartAppData.isPartPcf(1);
  myPartAppData.unConstrainedStartPeriod(21);
  
  assert(myPartAppData.isPartPcf() == 1);
  assert(myPartAppData.unConstrainedStartPeriod() == 21);  

  // copy constructor
  LgFrScePartAppData localCopy(myPartAppData);

  // check that the copy worked
  assert(localCopy.unConstrainedStartPeriod() == 21);

  // assignment operator
  localCopy.unConstrainedStartPeriod(15);
  myPartAppData = localCopy;
  assert(myPartAppData.unConstrainedStartPeriod() == 15);

  // new another
  LgFrScePartAppData * newPtr = new LgFrScePartAppData(localCopy);
  assert(newPtr->unConstrainedStartPeriod() == 15);
  // set the new value
  newPtr->unConstrainedStartPeriod(11);
  delete newPtr;

  // make sure the localCopy never changed
  assert(localCopy.unConstrainedStartPeriod() == 15);
}





void
LgFrSceSubBomAppData::test()
{
  // create default
  LgFrSceSubBomAppData mySubBomAppData;
  assert(mySubBomAppData.preference() == 0.0);

  // set methods
  mySubBomAppData.preference(21.);
  
  assert(mySubBomAppData.preference() == 21.);  

  // copy constructor
  LgFrSceSubBomAppData localCopy(mySubBomAppData);

  // check that the copy worked
  assert(localCopy.preference() == 21.);

  // assignment operator
  localCopy.preference(1.5);
  mySubBomAppData = localCopy;
  assert(mySubBomAppData.preference() == 1.5);

  // new another
  LgFrSceSubBomAppData * newPtr = new LgFrSceSubBomAppData(localCopy);
  assert(newPtr->preference() == 1.5);
  // set the new value
  newPtr->preference(11);
  delete newPtr;

  // make sure the localCopy never changed
  assert(localCopy.preference() == 1.5);
}




