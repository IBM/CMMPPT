#if defined(_MSC_VER)
#pragma warning(disable:4786)
#pragma warning(disable:4503)
#endif

#include "eso2Problem.h"
#include "featureAppData.h"

  

void ESO2featureAppData::setWfmv(int period, float wfmv )
{
  wfmvVec_[period] = wfmv;
}
void ESO2featureAppData::setWfmv(std::vector<float> wfmvVec )
{
  wfmvVec_ = wfmvVec;
}
float ESO2featureAppData::getWfmv(int period ) const
{
  return wfmvVec_[period];
}
std::vector<float> ESO2featureAppData::getWfmv() const
{
  return wfmvVec_;
}

// default constructor
ESO2featureAppData::ESO2featureAppData()
:
ESO2appData(),wfmvVec_()
{}

ESO2featureAppData::ESO2featureAppData(int nPeriods)
:
ESO2appData(),wfmvVec_()
{
  setVector(nPeriods,ESO2problem::bigM(),wfmvVec_);
}

// destructor

ESO2featureAppData::~ESO2featureAppData()
{}

// copy constructor
ESO2featureAppData::ESO2featureAppData( const ESO2featureAppData& source ):
ESO2appData(source),
wfmvVec_(source.wfmvVec_)
{};

// assignment operator
ESO2featureAppData&
ESO2featureAppData::operator=(const ESO2featureAppData& rhs)
{  
  if (this != &rhs) {        
    ESO2appData::operator=(rhs);
    wfmvVec_ = rhs.wfmvVec_;
  }
  return *this;
}

ESO2appData * ESO2featureAppData::clone() const
{ return (new ESO2featureAppData(*this));}

// Self-test
void ESO2featureAppData::test()
{

  ESO2featureAppData ad1;
  std::vector<float> v;

  {
  ESO2featureAppData ad0(3);
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

  ESO2featureAppData ad2(ad0);
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
