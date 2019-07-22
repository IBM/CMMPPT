#ifndef LGFR_EXPLODE_H
#define LGFR_EXPLODE_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/algorthm.h>

// This class represents the mrp explosion algorithm

// Forward Reference
class LgFrLastProblemSolved;

class LgFrExplode : public LgFrAlgorithm {

public:

    int virtual GetAlgorithmType() const { return __LGFREXPLODE; }

  // updateFrom - This method is invoked when the algorthm solution has
  // been updated.
  virtual
     void
        updateFrom (
           LgFrModel * p,
           void * d );

  // Return pointer to the explode algorithm in the specified scenario.
  virtual
     LgFrAlgorithm *
        counterPart(
           LgFrScenario & )
              const;

  // Constructor.
   LgFrExplode(
       LgFrLastProblemSolved & lps );

  // Copy constructor
  LgFrExplode (
     const LgFrExplode& source);
  
  // destructor
  ~LgFrExplode();
  
  // Assignment 
  LgFrExplode& 
    operator=(const LgFrExplode& rhs);
  
  // Get Last Problem Solved associated with this explosion alogrithm.
  // If the owner of the lastProblemSolved is deleted by its owner then
  // then the returned reference will be dangling
  virtual
     LgFrLastProblemSolved &
        lastProblemSolved()
           const;

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
  LgFrExplode();

};

#endif
