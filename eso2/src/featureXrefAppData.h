#ifndef ESO2featureXrefAppData_h
#define ESO2featureXrefAppData_h

#include "appData.h"

class ESO2featureXrefAppData : public ESO2appData {
public:
  
  void setProdRate(int period, float prodRate );
  void setProdRate(std::vector<float> prodRate );
  float getProdRate(int period) const;
  std::vector<float> getProdRate() const;
    
  // default constructor
  ESO2featureXrefAppData();

  // Typical Constructor
  ESO2featureXrefAppData(int nPeriods);
  
  // destructor
  virtual ~ESO2featureXrefAppData();
  
  // copy constructor
  ESO2featureXrefAppData( const ESO2featureXrefAppData& source );
  
  // assignment operator
  ESO2featureXrefAppData&
    operator=(const ESO2featureXrefAppData& rhs);

  virtual ESO2appData * clone() const;

  // Self-test
  static void test();

private:

  std::vector<float> prodRateVec_;
  
};



#endif
