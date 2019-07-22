#if defined(_MSC_VER)
#pragma warning(disable:4786)
#pragma warning(disable:4503)
#endif

#include "eso2Problem.h"
#include "partInterplant.h"

  


// destructor

ESO2partInterplant::~ESO2partInterplant()
{}

// copy constructor
ESO2partInterplant::ESO2partInterplant( const ESO2partInterplant& source ):
srcLoc_(source.srcLoc_),
destLoc_(source.destLoc_),
defaultMoveCost_(source.defaultMoveCost_),
defaultMoveTime_(source.defaultMoveTime_)
{};

// assignment operator
ESO2partInterplant&
ESO2partInterplant::operator=(const ESO2partInterplant& rhs)
{  
  if (this != &rhs) { 
    srcLoc_ = rhs.srcLoc_;
    destLoc_ = rhs.destLoc_;
    defaultMoveCost_ = rhs.defaultMoveCost_;
    defaultMoveTime_ = rhs.defaultMoveTime_;
  }
  return *this;
}

#if 0
ESO2appData * ESO2partInterplant::clone() const
{ return (new ESO2partInterplant(*this));}

// Self-test
void ESO2partInterplant::test()
{

  ESO2partInterplant ad1;
  std::vector<float> v;

  {
  ESO2partInterplant ad0(3);
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

  assert(ad0.getMoveCost(0)==ESO2problem::bigM());
  assert(ad0.getMoveCost(1)==ESO2problem::bigM());
  assert(ad0.getMoveCost(2)==ESO2problem::bigM());
  ad0.setMoveCost(2,2.2f);
  v = ad0.getMoveCost();
  assert(v[0]==ESO2problem::bigM());
  assert(v[1]==ESO2problem::bigM());
  assert(v[2]==2.2f);
  v[0]=3.2f;
  ad0.setMoveCost(v);
  v = ad0.getMoveCost();
  assert(v[0]==3.2f);
  assert(v[1]==ESO2problem::bigM());
  assert(v[2]==2.2f);

  ad1 = ad0;

  ESO2partInterplant ad2(ad0);
  v = ad2.getWfmv();
  assert(v[0]==ESO2problem::bigM());
  assert(v[1]==2.f);
  assert(v[2]==ESO2problem::bigM());
  v = ad2.getMoveCost();
  assert(v[0]==3.2f);
  assert(v[1]==ESO2problem::bigM());
  assert(v[2]==2.2f);
  }

  v = ad1.getWfmv();
  assert(v[0]==ESO2problem::bigM());
  assert(v[1]==2.f);
  assert(v[2]==ESO2problem::bigM());
  v = ad1.getMoveCost();
  assert(v[0]==3.2f);
  assert(v[1]==ESO2problem::bigM());
  assert(v[2]==2.2f);


  
}
#endif