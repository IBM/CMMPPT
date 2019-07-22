#if defined(_MSC_VER)
#pragma warning(disable:4786)
#pragma warning(disable:4503)
#endif

#include "eso2Problem.h"
#include "mtmAppData.h"

  

void ESO2mtmAppData::setWfmv(int period, float wfmv )
{
  wfmvVec_[period] = wfmv;
}
void ESO2mtmAppData::setWfmv(std::vector<float> wfmvVec )
{
  wfmvVec_ = wfmvVec;
}
float ESO2mtmAppData::getWfmv(int period ) const
{
  return wfmvVec_[period];
}
std::vector<float> ESO2mtmAppData::getWfmv() const
{
  return wfmvVec_;
}

void ESO2mtmAppData::setMoveCost(int period, float moveCost )
{
  moveCostVec_[period] = moveCost;
}
void ESO2mtmAppData::setMoveCost(std::vector<float> moveCostVec )
{
  moveCostVec_ = moveCostVec;
}
float ESO2mtmAppData::getMoveCost(int period ) const
{
  return moveCostVec_[period];
}
std::vector<float> ESO2mtmAppData::getMoveCost() const
{
  return moveCostVec_;
}

#if 0
void ESO2mtmAppData::setMoveTime(int period, float moveTime )
{
  moveTimeVec_[period] = moveTime;
}
void ESO2mtmAppData::setMoveTime(std::vector<float> moveTimeVec )
{
  moveTimeVec_ = moveTimeVec;
}
float ESO2mtmAppData::getMoveTime(int period ) const
{
  return moveTimeVec_[period];
}
std::vector<float> ESO2mtmAppData::getMoveTime() const
{
  return moveTimeVec_;
}
#endif

// default constructor
ESO2mtmAppData::ESO2mtmAppData()
:
ESO2appData(),wfmvVec_(),moveCostVec_()
{}

ESO2mtmAppData::ESO2mtmAppData(int nPeriods)
:
ESO2appData(),wfmvVec_(),moveCostVec_()
{
  setVector(nPeriods,ESO2problem::bigM(),wfmvVec_);
  setVector(nPeriods,ESO2problem::bigM(),moveCostVec_);
  //setVector(nPeriods,nPeriods,moveTimeVec_);
}

// destructor

ESO2mtmAppData::~ESO2mtmAppData()
{}

// copy constructor
ESO2mtmAppData::ESO2mtmAppData( const ESO2mtmAppData& source ):
ESO2appData(source),
wfmvVec_(source.wfmvVec_),
moveCostVec_(source.moveCostVec_)
{};

// assignment operator
ESO2mtmAppData&
ESO2mtmAppData::operator=(const ESO2mtmAppData& rhs)
{  
  if (this != &rhs) {        
    ESO2appData::operator=(rhs);
    wfmvVec_ = rhs.wfmvVec_;
    moveCostVec_ = rhs.moveCostVec_;
  }
  return *this;
}

ESO2appData * ESO2mtmAppData::clone() const
{ return (new ESO2mtmAppData(*this));}

// Self-test
void ESO2mtmAppData::test()
{

  ESO2mtmAppData ad1;
  std::vector<float> v;

  {
  ESO2mtmAppData ad0(3);
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

  ESO2mtmAppData ad2(ad0);
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
