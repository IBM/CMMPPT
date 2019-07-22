
// RW2STL - inserted:
#include <string>
#include <iostream>
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL - inserted includes.
#include <stdio.h>
#include <mcl/src/mcl.h>
#include <stdlib.h>
#include <assert.h>
#include <sce/src/allocLet.h>
#include <sce/src/mPlantH.h>
#include <wit/src/wit.h>



  // set/get  qtyDemand Amount
void LgFrSceAllocLet::qtyDemand(const float qty)  { qtyDemand_ = qty; }
float LgFrSceAllocLet::qtyDemand() const { return qtyDemand_; }


  // set/get  qtyShip Amount 
void LgFrSceAllocLet::qtyShip(const float qty) { qtyShip_ = qty; }
float LgFrSceAllocLet::qtyShip() const { return qtyShip_; }


  // set/get the  demandPd 
void LgFrSceAllocLet::demandPd(const int reqPd) { demandPd_ = reqPd; }
int LgFrSceAllocLet::demandPd() const { return demandPd_; }

  // set/get the  shipPd 
void LgFrSceAllocLet::shipPd(const int shipPd) { shipPd_ = shipPd; }
int LgFrSceAllocLet::shipPd() const { return shipPd_; }

  // set/get the priority 
void LgFrSceAllocLet::priority(const int prio) { priority_ = prio; }
int LgFrSceAllocLet::priority() const { return priority_; }

  // set/get the allocated flag
void LgFrSceAllocLet::allocated(int allocFlag) { allocated_ = allocFlag; }
int LgFrSceAllocLet::allocated() const { return allocated_; }


  // get partName and demandName as RWCStrings
std::string LgFrSceAllocLet::demandName() const { return demandName_; }
std::string LgFrSceAllocLet::partName() const { return partName_; }

  // set partName and DemandName (as char *)
void LgFrSceAllocLet::demandName(char * dName) { demandName_ = dName; }
void LgFrSceAllocLet::partName(char * pName) { partName_ = pName; }


  // set/get the dependent 
void LgFrSceAllocLet::backlogDependent(LgFrSceAllocLet * dependent) {backlogDependent_ = dependent;}
LgFrSceAllocLet * LgFrSceAllocLet::backlogDependent() {return backlogDependent_;}



// print the allocLet
void
LgFrSceAllocLet::print()
{
   std::cout << "\"" << partName_ << "\","
        << "\"" << demandName_ << "\","
        <<  priority_ << ","
        <<  demandPd_ << ","
        <<  shipPd_ << ","
        <<  qtyDemand_ << ","
        <<  qtyShip_ << ","
        <<  allocated_ << ","
        <<  shipLateLimit_ << "\n";
}

  // create a TRACE STRING
std::string
LgFrSceAllocLet::traceString()
{
  std::string ts;
  ts = "\"" 
    + partName_ + "\",\""
    + demandName_ + "\","
    + sce::stringFrom((long) priority_) + ","
    +  sce::stringFrom((long) demandPd_) + ","
    +  sce::stringFrom((long) shipPd_) + ","
    +  sce::stringFrom((float) qtyDemand_) + ","
    +  sce::stringFrom((float) qtyShip_) + ","
    +  sce::stringFrom((long) shipLateLimit_) + ","
    +  sce::stringFrom((long) allocated_) + "\n";

  return ts;
}


  // create a Human Readable TRACE STRING
std::string
LgFrSceAllocLet::hrTraceString(WitRun * const theWitRun, 
			       LgFrMultiPlantHelper * mpHelper, 
			       int equitCount)
{
  std::string ts;
  std::string p, g, f, pdf;
  std::string printPartName, printDemandName;
  
  if (mpHelper->isPartSpecialGeoPlannerDemandPart(theWitRun, partName_, p, g)) {
    printPartName = p;
    printDemandName = g;
  }
  else if (mpHelper->isPartSpecialCustChoiceFeature(theWitRun, partName_, g, p, pdf, f))   {
    printPartName = f + "->" + p ;
    printDemandName = g;
  }
  else if (mpHelper->isPartSpecialStandaloneFeature(theWitRun, partName_, g, p, pdf, f))   {
    printPartName = f + "->" + p ;
    printDemandName = g;
  }
  // in this case it's a normal part with normal demand (ie, direct independent demand)
  else {
    printPartName = partName_;
    printDemandName = demandName_;
  }
	   
    
    

  ts = sce::stringFrom((long) equitCount) + ",\""
    + printPartName + "\",\""
    + printDemandName + "\","
    + sce::stringFrom((long) (1000000000 - priority_)) + ","
    +  sce::stringFrom((long) demandPd_) + ","
    +  sce::stringFrom((long) shipPd_) + ","
    +  sce::stringFrom((double) qtyDemand_) + ","
    +  sce::stringFrom((double) qtyShip_) + ","
    +  sce::stringFrom((long) shipLateLimit_) + ","
    +  sce::stringFrom((long) allocated_) + "\n";
  
  return ts;
}



  // preferred constructor 
LgFrSceAllocLet::LgFrSceAllocLet(std::string & partName, 
			   std::string & demandName,
                           int    priority,
                           int    demandPd,
                           int    shipPd,       
                           float  qtyDemand,
	                   int    shipLateLimit)
: 
  partName_(partName),
  demandName_(demandName),
  priority_(priority),
  demandPd_(demandPd),
  shipPd_(shipPd),
  qtyDemand_(qtyDemand),
  qtyShip_(0.0),
  shipLateLimit_(shipLateLimit),
  allocated_(0),
  backlogDependent_(0)
  {
// FINISH_ME
  }


  // Construct from a Trace String
LgFrSceAllocLet::LgFrSceAllocLet(std::string & traceString)
:
  partName_(),
  demandName_(),
  priority_(0),
  demandPd_(0),
  shipPd_(0),
  qtyDemand_(0.0),
  qtyShip_(0.0),
  shipLateLimit_(0),
  allocated_(0),
  backlogDependent_(0)
{
  // FINISH_ME: determine format
  //            maybe use an XML format

  // parse traceString into components

}






  // default constructor
LgFrSceAllocLet::LgFrSceAllocLet()
:
  partName_(),
  demandName_(),
  priority_(0),
  demandPd_(0),
  shipPd_(0),
  qtyDemand_(0.0),
  qtyShip_(0.0),
  shipLateLimit_(0),
  allocated_(0),
  backlogDependent_(0)
  {
    // nothing else to do
  }


  // destructor
LgFrSceAllocLet::~LgFrSceAllocLet()
{
	// do nothing
}


// assignment operator
LgFrSceAllocLet&
LgFrSceAllocLet::operator=(const LgFrSceAllocLet& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    partName_ = rhs.partName_;
    demandName_ = rhs.demandName_;
    priority_ = rhs.priority_;
    demandPd_ = rhs.demandPd_;
    shipPd_ = rhs.shipPd_;
    qtyDemand_ = rhs.qtyDemand_;
    qtyShip_ = rhs.qtyShip_;
    shipLateLimit_ = rhs.shipLateLimit_;
    allocated_ = rhs.allocated_;
    backlogDependent_ = rhs.backlogDependent_;
  }
  return *this;
}

// copy constructor 
LgFrSceAllocLet::LgFrSceAllocLet(const LgFrSceAllocLet& source)
  : partName_(source.partName_),
  demandName_(source.demandName_),
  priority_(source.priority_),
  demandPd_(source.demandPd_),
  shipPd_(source.shipPd_),
  qtyDemand_(source.qtyDemand_),
  qtyShip_(source.qtyShip_),
  shipLateLimit_(source.shipLateLimit_),
  allocated_(source.allocated_),
  backlogDependent_(source.backlogDependent_)
{
  // nothing to do
}




