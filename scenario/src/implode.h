#ifndef LGFR_IMPLODE_H
#define LGFR_IMPLODE_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.
#define LGFR_IMPLODE_H

#include <scenario/src/algorthm.h>
#include <scenario/src/sOfParts.h>

// This class represents the implosion algorithm

// Forward references
class LgFrLastProblemSolved;

class LgFrImplode : public LgFrAlgorithm {

public:

  int virtual GetAlgorithmType() const { return __LGFRIMPLODE; }

  // updateFrom - This method is invoked when the algorthm solution has
  // been updated.
  virtual
     void
        updateFrom (
           LgFrModel * p,
           void * d );

  // Return pointer to the implode algorithm in the specified scenario.
  virtual
     LgFrAlgorithm *
        counterPart(
           LgFrScenario & )
              const;

  // Get Last Problem Solved associated with this implosion alogrithm.
  // If the owner of the lastProblemSolved is deleted by its owner then
  // then the returned reference will be dangling
  virtual
     LgFrLastProblemSolved &
        lastProblemSolved()
           const;

  // Constructor.
   LgFrImplode(
       LgFrLastProblemSolved & lps );

  // Copy constructor
  LgFrImplode (
     const LgFrImplode& source);
  
  // destructor
  ~LgFrImplode();
  
  
  // test this class
  static void test();

  // Test this class in the context of it's owner(s).
  // The data for this test should have come from
  // "/u/implode/wit/data/examples/diner12/wit.data"
  static void contextTest(LgFrLastProblemSolved       &lps,
                          LgFrSetOfParts              &sop);

private:

  LgFrLastProblemSolved  * lpsPtr_;

  const int id_;

  // default constructor
  LgFrImplode();

  // Assignment 
  LgFrImplode& 
    operator=(const LgFrImplode& rhs);

};

#endif
