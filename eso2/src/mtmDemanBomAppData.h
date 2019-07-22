#ifndef ESO2mtmDemanBomAppData_h
#define ESO2mtmDemanBomAppData_h

#include "appData.h"

class ESO2mtmDemanBomAppData : public ESO2appData {
public:
  
  void setAttachRate(int period, float attachRate );
  void setAttachRate(std::vector<float> attachRate );
  float getAttachRate(int period) const;
  std::vector<float> getAttachRate() const;
    
  // default constructor
  ESO2mtmDemanBomAppData();

  // Typical Constructor
  ESO2mtmDemanBomAppData(int nPeriods);
  
  // destructor
  virtual ~ESO2mtmDemanBomAppData();
  
  // copy constructor
  ESO2mtmDemanBomAppData( const ESO2mtmDemanBomAppData& source );
  
  // assignment operator
  ESO2mtmDemanBomAppData&
    operator=(const ESO2mtmDemanBomAppData& rhs);

  virtual ESO2appData * clone() const;

  // Self-test
  static void test();

private:

  std::vector<float> attachRateVec_;
  
};



#endif
