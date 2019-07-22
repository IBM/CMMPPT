#ifndef LGFRINITIALPROBLEMFORDIFTESTING_H
#define LGFRINITIALPROBLEMFORDIFTESTING_H

#include <scenario/src/initProb.h>
#include <scenario/src/scenDifT.h>

// forward references
class LgFrSetOfParts;
class LgFrSetOfDemands;


// added to avoid syntax error in declaration of difTestingScenario below - vra
class LgFrScenarioForDifTestingSmartPointer;


class LgFrInitialProblemForDifTesting: public LgFrInitialProblemAndParameters {

public:
  
virtual
void
feedInto(LgFrLastProblemSolved & lps);
  
  // Returns a description of the problem that is suitable for
  // use as a window heading. This can not be used to set the
  // title; only to query it.
virtual
RWCString
title(const LgFrDataInterfaceEssentials * = NULL) 
       const;

  // Make a clone copy of this object with new Scenario
virtual
LgFrInitialProblemAndParameters*
clone ( LgFrScenarioSmartPointer )
        const;

// the preferred constructor
LgFrInitialProblemForDifTesting(LgFrScenarioSmartPointer);      
       
  // Destructor
~LgFrInitialProblemForDifTesting();

  // Assignment operator.
LgFrInitialProblemForDifTesting&
operator=(
	  const LgFrInitialProblemForDifTesting& rhs);


  // Copy constructor
LgFrInitialProblemForDifTesting(
	      const LgFrInitialProblemForDifTesting& source);
  
  // Default Constructor
LgFrInitialProblemForDifTesting();



protected:

virtual
LgFrScenarioForDifTestingSmartPointer
difTestingScenario()
       const;

};

#endif
