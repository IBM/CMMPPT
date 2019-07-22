#ifndef ESO2loosePartAppData_h
#define ESO2loosePartAppData_h

#include "appData.h"

class ESO2loosePartAppData : public ESO2appData {
public:
  
  void setWfmv(int period, float wfmv );
  void setWfmv(std::vector<float> wfmvVec );
  float getWfmv(int period) const;
  std::vector<float> getWfmv() const;
    
  // default constructor
  ESO2loosePartAppData();

  // Typical Constructor
  ESO2loosePartAppData(int nPeriods);
  
  // destructor
  virtual ~ESO2loosePartAppData();
  
  // copy constructor
  ESO2loosePartAppData( const ESO2loosePartAppData& source );
  
  // assignment operator
  ESO2loosePartAppData&
    operator=(const ESO2loosePartAppData& rhs);

  virtual ESO2appData * clone() const;

  // Self-test
  static void test();

private:

  std::vector<float> wfmvVec_;
  
};



#endif
