
// RW2STL - inserted:
#include <string>
#include <iostream>
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL - inserted includes.
#include <stdio.h>
#include <mcl/src/mcl.h>
#include <stdlib.h>
#include <assert.h>
#include <sce/src/demLet.h>
// #include <sce/src/mPlantH.h>
// #include <wit/src/wit.h>


  // gets
std::string LgFrSceDemLet::partName() const { return partName_; }
std::string LgFrSceDemLet::witPartName() const { return witPartName_; }
std::string LgFrSceDemLet::witDemandName() const { return witDemandName_; }
std::string LgFrSceDemLet::sourceLoc() const { return sourceLoc_; }
std::string LgFrSceDemLet::customerLoc() const { return customerLoc_; }
std::string LgFrSceDemLet::demandClass() const { return demandClass_; }
std::string LgFrSceDemLet::demandLevel() const { return demandLevel_; }
std::string LgFrSceDemLet::partClass() const { return partClass_; }
int         LgFrSceDemLet::demandPd() const { return demandPd_; }
float       LgFrSceDemLet::demandVol() const { return demandVol_; }
int         LgFrSceDemLet::priority() const { return priority_; }
std::vector<int> *  LgFrSceDemLet::ruleSignature() {return &ruleSignature_; }

  // sets
void  LgFrSceDemLet::priority(int priority) {priority_ = priority; }
void  LgFrSceDemLet::ruleSignature(std::vector<int> & ruleSig) {ruleSignature_ = ruleSig; }
void  LgFrSceDemLet::partName(std::string & partName) {partName_ = partName; }
void  LgFrSceDemLet::witPartName(std::string & witPartName) {witPartName_ = witPartName; }
void  LgFrSceDemLet::witDemandName(std::string & witDemandName) {witDemandName_ = witDemandName; }
void  LgFrSceDemLet::sourceLoc(std::string & sourceLoc) {sourceLoc_ = sourceLoc; }
void  LgFrSceDemLet::customerLoc(std::string & customerLoc) {customerLoc_ = customerLoc; }
void  LgFrSceDemLet::demandClass(std::string & demandClass) {demandClass_ = demandClass; }
void  LgFrSceDemLet::demandLevel(std::string & demandLevel) {demandLevel_ = demandLevel; }
void  LgFrSceDemLet::partClass(std::string & partClass) {partClass_ = partClass; }
void  LgFrSceDemLet::demandPd(int demandPd) { demandPd_ = demandPd; }
void  LgFrSceDemLet::demandVol(float demandVol) { demandVol_ = demandVol; }




// print the demLet
void
LgFrSceDemLet::print()
{
   std::cout << "\"" 
	     << partName_ << "\",\""
	     << witPartName_ << "\",\""
	     << witDemandName_ << "\",\""
	     << sourceLoc_ << "\",\""
	     << customerLoc_ << "\",\""
	     << demandClass_ << "\",\""
	     << demandLevel_ << "\",\""
	     << partClass_ << "\","
	     << demandPd_ << ","
	     << demandVol_ << ","
	     << priority_ << ":";

   
   std::vector<int>::const_iterator i;
   for(i=ruleSignature_.begin(); i!=ruleSignature_.end(); ++i){
     std::cout << (*i) << ",";
   }
   std::cout << std::endl;
}

  

// preferred constructor 
LgFrSceDemLet::LgFrSceDemLet(std::string & partName, 
			     std::string & witPartName,
			     std::string & witDemandName, 
			     std::string & sourceLoc,
			     std::string & customerLoc,
			     std::string & demandClass,
			     std::string & demandLevel,
			     std::string & partClass,
			     int    demandPd,
			     float  demandVol)
: 
  partName_(partName),
  witPartName_(witPartName),
  witDemandName_(witDemandName),
  sourceLoc_(sourceLoc),
  customerLoc_(customerLoc),
  demandClass_(demandClass),
  demandLevel_(demandLevel),
  partClass_(partClass),
  demandPd_(demandPd),
  demandVol_(demandVol),
  priority_(0),
  ruleSignature_()
  {
    // nothing to do
  }


  // default constructor
LgFrSceDemLet::LgFrSceDemLet()
:
  partName_(),
  witPartName_(),
  witDemandName_(),
  sourceLoc_(),
  customerLoc_(),
  demandClass_(),
  demandLevel_(),
  partClass_(),
  demandPd_(),
  demandVol_(),
  priority_(0),
  ruleSignature_()
{
  // nothing else to do
}


// destructor
LgFrSceDemLet::~LgFrSceDemLet()
{
	// do nothing
}


// assignment operator
LgFrSceDemLet&
LgFrSceDemLet::operator=(const LgFrSceDemLet& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    partName_ = rhs.partName_;
    witPartName_ = rhs.witPartName_;
    witDemandName_ = rhs.witDemandName_;
    sourceLoc_ = rhs.sourceLoc_;
    customerLoc_ = rhs.customerLoc_;
    demandClass_ = rhs.demandClass_;
    demandLevel_ = rhs.demandLevel_;
    partClass_ = rhs.partClass_;
    demandPd_ = rhs.demandPd_;
    demandVol_ = rhs.demandVol_;
    priority_ = rhs.priority_;
    ruleSignature_ = rhs.ruleSignature_;
  }
  return *this;
}

// copy constructor 
LgFrSceDemLet::LgFrSceDemLet(const LgFrSceDemLet& source)
  : 
partName_(source.partName_),
witPartName_(source.witPartName_),
witDemandName_(source.witDemandName_),
  sourceLoc_(source.sourceLoc_),
  customerLoc_(source.customerLoc_),
  demandClass_(source.demandClass_),
  demandLevel_(source.demandLevel_),
  partClass_(source.partClass_),
  demandPd_(source.demandPd_),
  demandVol_(source.demandVol_),
  priority_(source.priority_),
  ruleSignature_(source.ruleSignature_)
{
  // nothing to do
}

