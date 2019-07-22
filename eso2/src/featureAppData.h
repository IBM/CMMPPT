#ifndef ESO2featureAppData_h
#define ESO2featureAppData_h

#include "appData.h"

class ESO2featureAppData : public ESO2appData {
public:
  
  void setWfmv(int period, float wfmv );
  void setWfmv(std::vector<float> wfmvVec );
  float getWfmv(int period) const;
  std::vector<float> getWfmv() const;
    
  // default constructor
  ESO2featureAppData();

  // Typical Constructor
  ESO2featureAppData(int nPeriods);
  
  // destructor
  virtual ~ESO2featureAppData();
  
  // copy constructor
  ESO2featureAppData( const ESO2featureAppData& source );
  
  // assignment operator
  ESO2featureAppData&
    operator=(const ESO2featureAppData& rhs);

  virtual ESO2appData * clone() const;

  // Self-test
  static void test();

private:

  std::vector<float> wfmvVec_;
  
};



#endif
