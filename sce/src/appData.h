// file: appData.h
// author: tom

// appData objects for:
//    parts
//    subBomEntries


#ifndef  LGFRSCEAPPDATA_H
#define  LGFRSCEAPPDATA_H

#include <stdlib.h>
#include <sce/src/sceDefin.h>
#include <scenario/src/timeVecs.h>
#include <sce/src/baseAppData.h>
#include <assert.h>

class LgFrScePartAppData  :
    public LgFrSceAppData
{
public:

virtual
LgFrSceAppData *
clone()
  const;




// get isPartPcf
int
isPartPcf() const;

// set Part as Pcf
void
isPartPcf(int isPartPcf);

#ifdef MARK_BOGONS
// get isPartBogon
int
isPartBogon () const;

// set Part as Bogon
void
isPartBogon (int isPartBogon);
#endif


// get unConstrainedStartPeriod
size_t
unConstrainedStartPeriod() const;


// set unConstrainedStartPeriod
void
unConstrainedStartPeriod(size_t unConstrainedStartPeriod);

// nInterplantOps (number of UNIQUE interplant operations that produce this part)
// get
int
nInterplantOps();
  


// set
void
nInterplantOps(int nOps);

// nAlternatePartOps (number of UNIQUE alternatePart operations that produce this part)
// get
int
nAlternatePartOps();



// set
void
nAlternatePartOps(int nOps);


// nUserDefinedOps (number of UNIQUE user-defined operations that produce this part)
// get
int
nUserDefinedOps();
  
// set
void
nUserDefinedOps(int nOps);

// nAggregationOps (number of UNIQUE aggregation operations that produce this part)
// get
int
nAggregationOps();
  
// set
void
nAggregationOps(int nOps);

// index for index'ing into witGetParts partList
void
index(int index);

int
index();
  
// get maxWithout (only meaningful for special Feature parts)
const
LgFrTimeVecFloat &
maxWithout() const;

// set maxWithout (only meaningful for special Feature parts)
void
maxWithout(const LgFrTimeVecFloat &  maxWithout);



// get doesPartHaveNegativeDemands
int
doesPartHaveNegativeDemands() const;

// set doesPartHaveNegativeDemands
void
doesPartHaveNegativeDemands(int doesPartHaveNegativeDemands);

// get vac
const
LgFrTimeVecFloat &
vac() const;

// set vac
void
vac(const LgFrTimeVecFloat &  vac);


// get compCost
const
LgFrTimeVecFloat &
compCost() const;

// set compCost
void
compCost(const LgFrTimeVecFloat &  compCost);


// get sourceCode
const 
std::string & 
sourceCode() const;

// set sourceCode
void
sourceCode(const std::string &  sourceCode);


// get cycleTimeDays 
const
LgFrTimeVecFloat &
cycleTimeDays() const;

// set cycleTimeDays
void
cycleTimeDays(const LgFrTimeVecFloat &  cycleTimeDays);

// get cycleTime
const
LgFrTimeVecFloat &
cycleTime() const;

// set cycleTime
void
cycleTime(const LgFrTimeVecFloat &  cycleTime);

  
// default constructor.
LgFrScePartAppData(); 


// best constructor
  LgFrScePartAppData(int isPartPcf, size_t unConstrainedStartPeriod, int nPeriods);


// copy constructor
LgFrScePartAppData(const LgFrScePartAppData& source);

// assignment operator
LgFrScePartAppData&
operator=(const LgFrScePartAppData& rhs);

// destructor
virtual
~LgFrScePartAppData();

// self-test
static void test();
  
  
private:


// better constructor
LgFrScePartAppData(int isPartPcf, size_t unConstrainedStartPeriod);




  int isPartPcf_;
#ifdef MARK_BOGONS
  int isPartBogon_;
#endif
  size_t unConstrainedStartPeriod_;
  int nInterplantOps_;
  int nAlternatePartOps_;
  int nUserDefinedOps_;
  int nAggregationOps_;
  int index_;
  LgFrTimeVecFloat maxWithout_;
  int doesPartHaveNegativeDemands_;
  LgFrTimeVecFloat vac_;  
  LgFrTimeVecFloat compCost_;  
  std::string sourceCode_; 
  LgFrTimeVecFloat cycleTimeDays_;
  LgFrTimeVecFloat cycleTime_;
};




// ------------------------------------------------------------------
class LgFrSceCCOperationAppData  :
    public LgFrSceAppData
{
public:

virtual
LgFrSceAppData *
clone()
  const;



// index for index'ing into witGetOperations opList
void
index(int index);

int
index();
  

  
  
// default constructor.
LgFrSceCCOperationAppData(); 

// better constructor
LgFrSceCCOperationAppData(int index);


// copy constructor
LgFrSceCCOperationAppData(const LgFrSceCCOperationAppData& source);

// assignment operator
LgFrSceCCOperationAppData&
operator=(const LgFrSceCCOperationAppData& rhs);

// destructor
virtual
~LgFrSceCCOperationAppData();

// self-test
static void test();
  
  
private:
  int index_;
};





//--------------------------------------------------------------------
class LgFrSceSubBomAppData :
    public LgFrSceAppData
{
public:

  virtual
  LgFrSceAppData *
  clone()
    const;

  // default constructor. Don't use.
  LgFrSceSubBomAppData (); 

  // better constructor
  LgFrSceSubBomAppData (float defaultPreference, int isOptExplodableFlag);

  // get "preference"
  const
  float
  preference () const ;
  
  // set "preference"
  void
  preference (const float pref);

  // get "isOptExplodableFlag"
  const
  int
  isObjectInEffectForOptExplosion () const ;
  
  // set "isOptExplodableFlag"
  void
  isObjectInEffectForOptExplosion (const int isOptExplodableFlag);

  
  // copy constructor
  LgFrSceSubBomAppData(const LgFrSceSubBomAppData& source);

  // assignment operator
  LgFrSceSubBomAppData&
  operator=(const LgFrSceSubBomAppData& rhs);

  // destructor
  virtual
  ~LgFrSceSubBomAppData();

  // self-test
  static void test();
  
private:
  // The preference on the sub-bom is used to determine
  // which sub-bom entry to explode in witMrp and the hueristic.
 
  // The sub-bom entry corresponding to the
  // "regular" build has preference 0.0
  // The lower the preferences (negatives are ok), the more important. 
  // With the lp, the "preference" is interpreted as a cost.
  // Default = 0.0
  float preference_ ;
  int isObjectInEffectForOptExplosion_;
};

// --------------------------------------------------------------------
//--------------------------------------------------------------------
class LgFrSceCustChoiceBomAppData :
    public LgFrSceAppData
{
public:

  virtual
  LgFrSceAppData *
  clone()
    const;

  // default constructor. Don't use.
  LgFrSceCustChoiceBomAppData (); 

  // even better constructor
  LgFrSceCustChoiceBomAppData (LgFrTimeVecFloat & featRatio);


  // get the feature ratio
  const
  LgFrTimeVecFloat&
  featRatio() const;

  // set  the feature ratio
  void
  featRatio (const LgFrTimeVecFloat & featRatio);

  // get the dummySupplyVolForMrpAdjustment
  const
  LgFrTimeVecFloat&
  dummySupplyVolForMrpAdjustment() const;

  // set  the dummySupplyVolForMrpAdjustment
  void
  dummySupplyVolForMrpAdjustment(const LgFrTimeVecFloat & dummySupplyVolForMrpAdjustment);


  // copy constructor
  LgFrSceCustChoiceBomAppData(const LgFrSceCustChoiceBomAppData& source);

  // assignment operator
  LgFrSceCustChoiceBomAppData&
  operator=(const LgFrSceCustChoiceBomAppData& rhs);

  // destructor
  virtual
  ~LgFrSceCustChoiceBomAppData();
  
private:
  LgFrTimeVecFloat featRatio_;
  LgFrTimeVecFloat dummySupplyVolForMrpAdjustment_;
};


//--------------------------------------------------------------------
class LgFrSceInterplantOpAppData :
    public LgFrSceAppData

{
public:

  virtual
    LgFrSceAppData *
    clone()
    const;

  // default constructor. Don't use.
  LgFrSceInterplantOpAppData (); 

  // even better constructor
  LgFrSceInterplantOpAppData (LgFrTimeVecFloat mrpExecVol);


  // get "mrp jumper arc adjusted exec vols"
  const
  LgFrTimeVecFloat&
  mrpExecVol () const ;

  // get "jumper arc adjusted exec vols"
  const
  LgFrTimeVecFloat&
  execVol () const ;
  
  
  // set  "mrp jumper arc adjusted exec vols"
  void
  mrpExecVol (const LgFrTimeVecFloat & mrpExecVol);

  // set  " jumper arc adjusted exec vols"
  void
  execVol (const LgFrTimeVecFloat & execVol);

  // copy constructor
  LgFrSceInterplantOpAppData(const LgFrSceInterplantOpAppData& source);

  // assignment operator
  LgFrSceInterplantOpAppData&
  operator=(const LgFrSceInterplantOpAppData& rhs);

  // destructor
  virtual
  ~LgFrSceInterplantOpAppData();
  
private:
  LgFrTimeVecFloat mrpExecVol_ ;
  LgFrTimeVecFloat execVol_ ;
  
};




#endif





