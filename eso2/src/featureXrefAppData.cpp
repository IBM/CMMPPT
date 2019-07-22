#if defined(_MSC_VER)
#pragma warning(disable:4786)
#pragma warning(disable:4503)
#endif

#include "eso2Problem.h"
#include "featureXrefAppData.h"

  

void ESO2featureXrefAppData::setProdRate(int period, float prodRate )
{
  prodRateVec_[period] = prodRate;
}
void ESO2featureXrefAppData::setProdRate(std::vector<float> prodRateVec )
{
  prodRateVec_ = prodRateVec;
}
float ESO2featureXrefAppData::getProdRate(int period ) const
{
  return prodRateVec_[period];
}
std::vector<float> ESO2featureXrefAppData::getProdRate() const
{
  return prodRateVec_;
}

// default constructor
ESO2featureXrefAppData::ESO2featureXrefAppData()
:
ESO2appData(),prodRateVec_()
{}

ESO2featureXrefAppData::ESO2featureXrefAppData(int nPeriods)
:
ESO2appData(),prodRateVec_()
{
  setVector(nPeriods,ESO2problem::bigM(),prodRateVec_);
}

// destructor

ESO2featureXrefAppData::~ESO2featureXrefAppData()
{}

// copy constructor
ESO2featureXrefAppData::ESO2featureXrefAppData( const ESO2featureXrefAppData& source ):
ESO2appData(source),
prodRateVec_(source.prodRateVec_)
{};

// assignment operator
ESO2featureXrefAppData&
ESO2featureXrefAppData::operator=(const ESO2featureXrefAppData& rhs)
{  
  if (this != &rhs) {        
    ESO2appData::operator=(rhs);
    prodRateVec_ = rhs.prodRateVec_;
  }
  return *this;
}

ESO2appData * ESO2featureXrefAppData::clone() const
{ return (new ESO2featureXrefAppData(*this));}

// Self-test
void ESO2featureXrefAppData::test()
{

  ESO2featureXrefAppData ad1;
  std::vector<float> v;

  {
  ESO2featureXrefAppData ad0(3);
  assert(ad0.getProdRate(0)==ESO2problem::bigM());
  assert(ad0.getProdRate(1)==ESO2problem::bigM());
  assert(ad0.getProdRate(2)==ESO2problem::bigM());
  ad0.setProdRate(1,2.f);
  v = ad0.getProdRate();
  assert(v[0]==ESO2problem::bigM());
  assert(v[1]==2.f);
  assert(v[2]==ESO2problem::bigM());
  assert(ad0.getProdRate(0)==ESO2problem::bigM());
  assert(ad0.getProdRate(1)==2.f);
  assert(ad0.getProdRate(2)==ESO2problem::bigM());


  ad1 = ad0;

  ESO2featureXrefAppData ad2(ad0);
  v = ad2.getProdRate();
  assert(v[0]==ESO2problem::bigM());
  assert(v[1]==2.f);
  assert(v[2]==ESO2problem::bigM());
  }

  v = ad1.getProdRate();
  assert(v[0]==ESO2problem::bigM());
  assert(v[1]==2.f);
  assert(v[2]==ESO2problem::bigM());


  
}
