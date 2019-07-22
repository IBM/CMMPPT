#ifndef ESO2partInterplant_h
#define ESO2partInterplant_h

#include <string>

class ESO2partInterplant {
public:
  
  const std::string & getSrcLoc(){return srcLoc_;}; 
  const std::string & getDestLoc(){return destLoc_;}; 
  float getDefaultMoveCost(){return defaultMoveCost_;};
  float getDefaultMoveTime(){return defaultMoveTime_;};
    

  // Typical Constructor
  ESO2partInterplant(
    const std::string & srcLoc,
    const std::string & destLoc,
    float defaultMoveCost,
    float defaultMoveTime)
    : 
  srcLoc_(srcLoc),destLoc_(destLoc),defaultMoveCost_(defaultMoveCost),defaultMoveTime_(defaultMoveTime)
  {};
  
  // destructor
  virtual ~ESO2partInterplant();
  
  // copy constructor
  ESO2partInterplant( const ESO2partInterplant& source );
  
  // assignment operator
  ESO2partInterplant&
    operator=(const ESO2partInterplant& rhs);

  // Self-test
  //static void test();

private:
  
  // default constructor
  ESO2partInterplant();

  std::string srcLoc_;
  std::string destLoc_;
  float defaultMoveCost_;
  float defaultMoveTime_;
  
};



#endif
