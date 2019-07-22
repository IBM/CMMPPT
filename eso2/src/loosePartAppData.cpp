#if defined(_MSC_VER)
#pragma warning(disable:4786)
#pragma warning(disable:4503)
#endif

#include "eso2Problem.h"
#include "loosePartAppData.h"

  

void ESO2loosePartAppData::setWfmv(int period, float wfmv )
{
  wfmvVec_[period] = wfmv;
}
void ESO2loosePartAppData::setWfmv(std::vector<float> wfmvVec )
{
  wfmvVec_ = wfmvVec;
}
float ESO2loosePartAppData::getWfmv(int period ) const
{
  return wfmvVec_[period];
}
std::vector<float> ESO2loosePartAppData::getWfmv() const
{
  return wfmvVec_;
}

// default constructor
ESO2loosePartAppData::ESO2loosePartAppData()
:
ESO2appData(),wfmvVec_()
{}

ESO2loosePartAppData::ESO2loosePartAppData(int nPeriods)
:
ESO2appData(),wfmvVec_()
{
  setVector(nPeriods,ESO2problem::bigM(),wfmvVec_);
}

// destructor

ESO2loosePartAppData::~ESO2loosePartAppData()
{}

// copy constructor
ESO2loosePartAppData::ESO2loosePartAppData( const ESO2loosePartAppData& source ):
ESO2appData(source),
wfmvVec_(source.wfmvVec_)
{};

// assignment operator
ESO2loosePartAppData&
ESO2loosePartAppData::operator=(const ESO2loosePartAppData& rhs)
{  
  if (this != &rhs) {        
    ESO2appData::operator=(rhs);
    wfmvVec_ = rhs.wfmvVec_;
  }
  return *this;
}

ESO2appData * ESO2loosePartAppData::clone() const
{ return (new ESO2loosePartAppData(*this));}

// Self-test
void ESO2loosePartAppData::test()
{

  ESO2loosePartAppData ad1;
  std::vector<float> v;

  {
  ESO2loosePartAppData ad0(3);
  assert(ad0.getWfmv(0)==ESO2problem::bigM());
  assert(ad0.getWfmv(1)==ESO2problem::bigM());
  assert(ad0.getWfmv(2)==ESO2problem::bigM());
  ad0.setWfmv(1,2.f);
  v = ad0.getWfmv();
  assert(v[0]==ESO2problem::bigM());
  assert(v[1]==2.f);
  assert(v[2]==ESO2problem::bigM());
  assert(ad0.getWfmv(0)==ESO2problem::bigM());
  assert(ad0.getWfmv(1)==2.f);
  assert(ad0.getWfmv(2)==ESO2problem::bigM());


  ad1 = ad0;

  ESO2loosePartAppData ad2(ad0);
  v = ad2.getWfmv();
  assert(v[0]==ESO2problem::bigM());
  assert(v[1]==2.f);
  assert(v[2]==ESO2problem::bigM());
  }

  v = ad1.getWfmv();
  assert(v[0]==ESO2problem::bigM());
  assert(v[1]==2.f);
  assert(v[2]==ESO2problem::bigM());


  
}
