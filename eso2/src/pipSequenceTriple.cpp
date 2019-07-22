#if defined(_MSC_VER)
#pragma warning(disable:4786)
#pragma warning(disable:4503)
#endif

#include <set>
#include <cassert>
#include "pipSequenceTriple.h"


void ESO2pipSequenceTriple::setDemandName(const std::string & dname )
{
  demandName_=dname;
}
void ESO2pipSequenceTriple::setShipPeriod(int period )
{
  shipPeriod_=period;
}
void ESO2pipSequenceTriple::setIncShipVol(float incShipVol)
{
  incShipVol_=incShipVol;
}
void ESO2pipSequenceTriple::setValue(float v)
{
  value_=v;
}

std::string ESO2pipSequenceTriple::getDemandName() const
{
  return demandName_;;
}
int ESO2pipSequenceTriple::getShipPeriod() const
{
  return shipPeriod_;
}
float ESO2pipSequenceTriple::getIncShipVol() const
{
  return incShipVol_;
}
float ESO2pipSequenceTriple::getValue() const
{
  return value_;
}


// default constructor
ESO2pipSequenceTriple::ESO2pipSequenceTriple()
:
demandName_(""),
shipPeriod_(-1),
incShipVol_(-1.f),
value_(-1.f)
{}

ESO2pipSequenceTriple::ESO2pipSequenceTriple(const std::string & dname,int period,float incShipVol,float value)
:
demandName_(dname),
shipPeriod_(period),
incShipVol_(incShipVol),
value_(value)
{}


// destructor

ESO2pipSequenceTriple::~ESO2pipSequenceTriple()
{}

// copy constructor
ESO2pipSequenceTriple::ESO2pipSequenceTriple( const ESO2pipSequenceTriple& source ):
demandName_(source.demandName_),
shipPeriod_(source.shipPeriod_),
incShipVol_(source.incShipVol_),
value_(source.value_)
{};

// assignment operator
ESO2pipSequenceTriple&
ESO2pipSequenceTriple::operator=(const ESO2pipSequenceTriple& rhs)
{  
  if (this != &rhs) {    
    demandName_ = rhs.demandName_;
    shipPeriod_ = rhs.shipPeriod_;
    incShipVol_ = rhs.incShipVol_;
    value_ = rhs.value_;
  }
  return *this;
}

// Self-test
void ESO2pipSequenceTriple::test()
{

  ESO2pipSequenceTriple ad1;

  {
  ESO2pipSequenceTriple ad0("a",1,2.f,3.f);
  assert(ad0.getDemandName()=="a");
  assert(ad0.getShipPeriod()==1);
  assert(ad0.getIncShipVol()==2.f);
  assert(ad0.getValue()==3.f);

  ad0.setDemandName("b");
  ad0.setShipPeriod(2);
  ad0.setIncShipVol(3.f);
  ad0.setValue(4.f);
  assert(ad0.getDemandName()=="b");
  assert(ad0.getShipPeriod()==2);
  assert(ad0.getIncShipVol()==3.f);
  assert(ad0.getValue()==4.f);

  ad1 = ad0;
  
  ad0.setDemandName("c");
  ad0.setShipPeriod(3);
  ad0.setIncShipVol(4.f);
  ad0.setValue(5.f);
  assert(ad0.getDemandName()=="c");
  assert(ad0.getShipPeriod()==3);
  assert(ad0.getIncShipVol()==4.f);
  assert(ad0.getValue()==5.f);

  }

  assert(ad1.getDemandName()=="b");
  assert(ad1.getShipPeriod()==2);
  assert(ad1.getIncShipVol()==3.f);
  assert(ad1.getValue()==4.f);

  // Test set methods
  {
    std::set<ESO2pipSequenceTriple,ESO2sortPipSequenceTriple> s;
    assert(s.size()==0);
    s.insert( ESO2pipSequenceTriple("a",0,1.0f,2.0f) );
    assert(s.size()==1);
    s.insert( ESO2pipSequenceTriple("a",0,1.0f,2.0f) );
    assert(s.size()==1);
    s.insert( ESO2pipSequenceTriple("b",0,1.0f,3.0f) );

    std::set<ESO2pipSequenceTriple,ESO2sortPipSequenceTriple>::const_iterator it;
    it=s.begin();
    assert(it->getDemandName()=="b");
    ++it;
    assert(it->getDemandName()=="a");
  }

  
}


// ---------
// function operator for sorting Pip Sequence Triple
// ---------
bool ESO2sortPipSequenceTriple::operator()( 
    const ESO2pipSequenceTriple & p1, 
    const ESO2pipSequenceTriple & p2) 
const
{
  bool retVal;
  if ( p1.getValue()==p2.getValue() ) {
    if (p1.getShipPeriod()==p2.getShipPeriod() ) {
      if ( p1.getIncShipVol()==p2.getIncShipVol() ) {
        retVal = p1.getDemandName() < p2.getDemandName();
      }
      else {
        // value and shipPeriod are equal. IncShipVol is not equal          
        retVal = p1.getIncShipVol() > p2.getIncShipVol();
      }
    }
    else {
      // value are equal, ship period are not equal          
      retVal = p1.getShipPeriod() < p2.getShipPeriod();
    }
  }
  else {
    // value are not equal
    retVal = p1.getValue() > p2.getValue();
  }
  return retVal;
}
