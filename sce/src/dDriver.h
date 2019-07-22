// file: dDriver
// author:  tom

// see usage in sce/src/ipSce.C to see how this should be used

#ifndef  LGFRSCEDEMANDDRIVER_H
#define  LGFRSCEDEMANDDRIVER_H

// RW2STL - inserted:
#include <string>
// End of RW2STL - inserted includes.

#include <stdlib.h>
#include <sce/src/mPlantH.h>
// #include <wit/src/wit.h>


// forward references
class LgFrCalendar;
class WitRun;
class LgFrMultiPlantHelper;

class LgFrSceDemandDriver {  
public:
  
  // compute the demand drivers table 
  virtual
  void
  compute();

  // Turn ON or OFF the subNetAllowed flag of every REAL and DUMMY sub arc or every 
  // REAL and DUMMY operation
  // NOTE:  
  // FINISH_ME: If we ever introduce sub arcs that need to set NetAllowed=false, then this will
  // screw it up and we need to use appData!!  
  void
  setSubsNetAllowed(bool subNetAllowed, LgFrMultiPlantHelper & mpHelper);
  


  // print demand drivers file
  virtual
  void
  print(std::string filename,
	MclFacility * theErrFacil);

  // print demand drivers file
  virtual
  void
  computeAndPrint(std::string filename,
		  MclFacility * theErrFacil);


  void
  getDDincidenceVector(char *  thePart, 
		       float minMwoScaled,
		       int childPartIndex);

  
  
  // default constructor
  LgFrSceDemandDriver();
  
  // general constructor
  LgFrSceDemandDriver(
                      WitRun * const theWitRun, 
                      LgFrMultiPlantHelper * mpHelper);

    
  // copy constructor
  LgFrSceDemandDriver(const LgFrSceDemandDriver& source);
  
  // assignment operator
  LgFrSceDemandDriver&
  operator=(const LgFrSceDemandDriver& rhs);
  
  // destructor
  virtual
  ~LgFrSceDemandDriver();
  
  
  
private:
  WitRun * theWitRun_;
  LgFrMultiPlantHelper * mpHelper_;
  int nParts_;
  float ** incidenceMatrix_;
  int state_;
  
};
#endif
