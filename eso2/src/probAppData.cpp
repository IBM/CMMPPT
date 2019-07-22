#if defined(_MSC_VER)
#pragma warning(disable:4786)
#pragma warning(disable:4503)
#endif

#include <vector>
#include "probAppData.h"

void ESO2probAppData::setWfmvScaleFactor(float sf)
{
  wfmvScaleFactor_ = sf;
}
float ESO2probAppData::getWfmvScaleFactor() const
{
  return wfmvScaleFactor_;
}

void ESO2probAppData::setWfmv2ScaleFactor(float sf)
{
  wfmv2ScaleFactor_ = sf;
}
float ESO2probAppData::getWfmv2ScaleFactor() const
{
  return wfmv2ScaleFactor_;
}

// default constructor
ESO2probAppData::ESO2probAppData()
:
ESO2appData(),wfmvScaleFactor_(1.f),wfmv2ScaleFactor_(1.0)
{
}

// destructor

ESO2probAppData::~ESO2probAppData()
{}

// copy constructor
ESO2probAppData::ESO2probAppData( const ESO2probAppData& source ):
ESO2appData(source),
wfmvScaleFactor_(source.wfmvScaleFactor_),
wfmv2ScaleFactor_(source.wfmv2ScaleFactor_)
{};

// assignment operator
ESO2probAppData&
ESO2probAppData::operator=(const ESO2probAppData& rhs)
{  
  if (this != &rhs) {        
    ESO2appData::operator=(rhs);
    wfmvScaleFactor_ = rhs.wfmvScaleFactor_;
    wfmv2ScaleFactor_ = rhs.wfmv2ScaleFactor_;
  }
  return *this;
}

ESO2appData * ESO2probAppData::clone() const
{ return (new ESO2probAppData(*this));}

// Self-test
void ESO2probAppData::test()
{

  ESO2probAppData ad0;
  assert(ad0.getWfmvScaleFactor()==1.0f);
  assert(ad0.getWfmv2ScaleFactor()==1.0f);

  ad0.setWfmvScaleFactor(0.2f);  
  assert(ad0.getWfmvScaleFactor()==0.2f);

  ad0.setWfmv2ScaleFactor(0.3f);  
  assert(ad0.getWfmv2ScaleFactor()==0.3f);
  
}
