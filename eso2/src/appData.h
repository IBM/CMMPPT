#ifndef ESO2appData_h
#define ESO2appData_h

#include <vector>
#include <map>
#include <cassert>

class ESO2appData  {
public:
  virtual ESO2appData * clone() const = 0;

  // destructor
  virtual ~ESO2appData(){};

protected:
  
  // constructor
  ESO2appData(){};
  
  // copy constructor
  ESO2appData( const ESO2appData& source ){};
  
  // assignment operator
  ESO2appData & operator=(const ESO2appData& rhs)
  {  
    if (this != &rhs) {        
    // nothing to do here
    }
    return *this;
  }
  
  // Self-test
  //static void test();

  static void setVector(size_t len, const float * inputVec, std::vector<float> & outputVec )
  {
    size_t i;
    for ( i=0; outputVec.size()<len; ++i ) outputVec.push_back(0.0);
    for ( i=0; i<len; ++i ) outputVec[i] = inputVec[i];
  };
  
  static void setVector(size_t len, float inputScalar, std::vector<float> & outputVec )
  {
    size_t i;
    for ( i=0; outputVec.size()<len; ++i ) outputVec.push_back(0.0);
    for ( i=0; i<len; ++i ) outputVec[i] = inputScalar;
  };

  //v2=v2+v1
  static void addVector(const std::vector<float> & v1, std::vector<float> & v2)
  { 
    assert(v1.size()==v2.size());
    for (size_t t=0; t<v1.size(); t++) v2[t]+=v1[t];
  };

};



#endif
