#ifndef LGFR_ALGORITHM_H
#define LGFR_ALGORITHM_H

#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif

#include <scenario/src/model.h>

#include <scenario/src/idnumber.h>

// This class if the base class for the algorithm of the logistics framework.
// Currently, these are:
//   Implosion
//   Mrp
//   FocussedShortageScheduleControl

// Forward declaration
class LgFrScenario;

class LgFrAlgorithm : 
   public LgFrSubscription {

public:

  int virtual GetAlgorithmType() const = 0;

  // Return pointer to algorithm of same type which is in the specified
  // scenario.
  virtual
     LgFrAlgorithm *
        counterPart(
           LgFrScenario & )
              const
                 = 0;

  // Constructor.
   LgFrAlgorithm( );

  // Copy constructor
  LgFrAlgorithm( 
     const LgFrAlgorithm& source);
  
  // destructor
  ~LgFrAlgorithm();
  
  // Assignment 
  LgFrAlgorithm& 
    operator=(const LgFrAlgorithm& rhs);
  
private:

};

#endif
