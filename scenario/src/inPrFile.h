#ifndef LGFRINITIALPROBLEMFROMFILE_H
#define LGFRINITIALPROBLEMFROMFILE_H

// RW2STL -- inserted:
#include <string>
// End of RW2STL -- inserted includes.
// File: LgFrInitialProblemFromFile.h
// Author: wally

#include <scenario/src/initProb.h>

// Knows the wit.data file representing the initial problem and is able
// to read the file to initialize a WitRun.

class LgFrLastProblemSolved;

class LgFrInitialProblemFromFile : public LgFrInitialProblemAndParameters {
  
public:
  // Populates the LgFrWitRunVeneer's data structures using the data in
  // fileName_ if it is not "", or the data from dif if fileName_ is "".
  virtual
    void
      feedInto(
               LgFrLastProblemSolved & lps);
  
  // Returns a description of the problem that is suitable for
  // use as a window heading. This can not be used to set the
  // title; only to query it.
  virtual
    std::string
      title(const LgFrDataInterfaceEssentials * = 0) 
	const;

  // Returns the file name of the wit.data file that the initial
  // parameters are coming from.
  virtual
    std::string
      fileName()
	const;

  // Make a clone copy of this object with new Scenario
  virtual
    LgFrInitialProblemAndParameters*
      clone ( LgFrScenarioSmartPointer )
          const;

  // Constructor to Create an instance for the file named in the first parameter
  LgFrInitialProblemFromFile(
			     const std::string& fileName,
                             LgFrScenarioSmartPointer scenario);

  // Destructor
  ~LgFrInitialProblemFromFile();

  // Assignment operator.
  LgFrInitialProblemFromFile&
    operator=(
	      const LgFrInitialProblemFromFile& rhs);

  // Copy constructor
  LgFrInitialProblemFromFile(
			     const LgFrInitialProblemFromFile& source);
  
  // Default Constructor
  LgFrInitialProblemFromFile();

  // Tests this class.  Only returns if all tests passed
  static
    void
      test ();

private:
  void
  createPartsAndFillSoP (LgFrLastProblemSolved & lps,
                         LgFrSetOfParts & sop);

  void
  createDemandsAndFillSoD (LgFrLastProblemSolved & lps,
                           LgFrSetOfParts & sop,
                           LgFrSetOfDemands & sod);

  // The name of the wit.data file the initial problem comes from
  std::string fileName_;
  
};

#endif
