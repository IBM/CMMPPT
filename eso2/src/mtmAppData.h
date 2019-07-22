#ifndef ESO2mtmAppData_h
#define ESO2mtmAppData_h

#include "appData.h"

class ESO2mtmAppData : public ESO2appData {
public:
  
  void setWfmv(int period, float wfmv );
  void setWfmv(std::vector<float> wfmvVec );
  float getWfmv(int period) const;
  std::vector<float> getWfmv() const;

  
  void setMoveCost(int period, float cost );
  void setMoveCost(std::vector<float> costVec );
  float getMoveCost(int period) const;
  std::vector<float> getMoveCost() const;
#if 0
  void setMoveTime(int period, float time );
  void setMoveTime(std::vector<float> timeVec );
  float getMoveTime(int period) const;
  std::vector<float> getMoveTime() const;
#endif
    
  // default constructor
  ESO2mtmAppData();

  // Typical Constructor
  ESO2mtmAppData(int nPeriods);
  
  // destructor
  virtual ~ESO2mtmAppData();
  
  // copy constructor
  ESO2mtmAppData( const ESO2mtmAppData& source );
  
  // assignment operator
  ESO2mtmAppData&
    operator=(const ESO2mtmAppData& rhs);

  virtual ESO2appData * clone() const;

  // Self-test
  static void test();

private:

  std::vector<float> wfmvVec_;
  std::vector<float> moveCostVec_;
  //std::vector<float> moveTimeVec_;
  
};



#endif
