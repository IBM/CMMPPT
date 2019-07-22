#ifndef ESO2probAppData_h
#define ESO2probAppData_h

#include "appData.h"

class ESO2probAppData : public ESO2appData {
public:
  
  void setWfmvScaleFactor(float sf);
  float getWfmvScaleFactor() const;

  void setWfmv2ScaleFactor(float sf);
  float getWfmv2ScaleFactor() const;
    
  // default constructor
  ESO2probAppData();
  
  // destructor
  virtual ~ESO2probAppData();
  
  // copy constructor
  ESO2probAppData( const ESO2probAppData& source );
  
  // assignment operator
  ESO2probAppData&
    operator=(const ESO2probAppData& rhs);

  virtual ESO2appData * clone() const;

  // Self-test
  static void test();

private:

  float wfmvScaleFactor_;
  float wfmv2ScaleFactor_;
  
};



#endif
