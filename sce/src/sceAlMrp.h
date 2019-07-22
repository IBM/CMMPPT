// file: sceAlMrp.h
// author:  tom/dan

#ifndef  LGFRSCEALLOCATEDMRP_H
#define  LGFRSCEALLOCATEDMRP_H

// RW2STL - inserted:
#include <string>
// End of RW2STL - inserted includes.

#include <stdlib.h>
#include <witutil/src/allocMrp.h>


// forward references
class LgFrMultiPlantHelper;
class LgFrSceSetOfParameters;
class LgFrSceFlatFileMgr;


class LgFrSceAllocatedMrp : public WUtlAllocatedMrp {  
public:
  

  // print the Allocated MRP in SCE format
  virtual
  int
  scePrint(std::string& filename, std::string& indepfileName);
  
  // default constructor
  LgFrSceAllocatedMrp();
  
  // general constructor
  LgFrSceAllocatedMrp(
                      WitRun * const theWitRun, 
                      LgFrMultiPlantHelper * mpHelper,
		      const char aMrpSeparatorChar,
		      LgFrSceSetOfParameters * setOfParameters);

    
  
  // destructor
  virtual
  ~LgFrSceAllocatedMrp();
  
  
  
private:

  // ===== Private member functions: DO Not CALL ====

  // copy constructor
  LgFrSceAllocatedMrp(const LgFrSceAllocatedMrp& source);
  
  // assignment operator
  LgFrSceAllocatedMrp&
  operator=(const LgFrSceAllocatedMrp& rhs);


  // preferred print method but not implemented, so its private
  // print the Allocated MRP in SCE format
  virtual
  int
  printWithoutAccessToAMrpGuts(std::string filename,
			       MclFacility * theErrFacil,
			       LgFrSceFlatFileMgr * fFileMgr);



  // =========  Member DAta =============

  LgFrMultiPlantHelper * mpHelper_;
  LgFrSceSetOfParameters * setOfParameters_;
  
};
#endif
