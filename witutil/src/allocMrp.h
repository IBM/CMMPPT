// file: allocMrp.h
// author:  dan/tom

#ifndef  WUTLALLOCATEDMRP_H
#define  WUTLALLOCATEDMRP_H

#include <stdlib.h>
#include <string>

// forward declarations
class WitRun;


class WUtlAllocatedMrp {  
public:
  
  // compute the allocated MRP
  virtual
  int
  computeAndPopulate(const int version);

  // print the Allocated MRP in generic format
  virtual
  int
  print(const char * filename);

    
  // general constructor ... takes a populated witRun, and a separator character
  WUtlAllocatedMrp(WitRun * const theWitRun, const char separatorChar);

  // destructor
  virtual
  ~WUtlAllocatedMrp();

  int 
  state();

  int 
  version();
 
  int 
  nPeriods();

  char
  separatorChar();

protected:
  // these methods make the witRuns accessible to any sub classes
  WitRun *
  witRun();

  WitRun *
  localWitRun();

private:

  // copy constructor
  WUtlAllocatedMrp(const WUtlAllocatedMrp& source);
  
  // assignment operator
  WUtlAllocatedMrp&
  operator=(const WUtlAllocatedMrp& rhs);
  
  // default constructor
  WUtlAllocatedMrp();
  
  
  
private:
  WitRun *  theWitRun_;
  WitRun * localWitRun_;
  char separatorChar_;
  int state_;
  int version_;
  int nPeriods_;  
};
#endif
