#ifndef LgFrDataInterfaceImplementationFromWitDataFile_h
#define LgFrDataInterfaceImplementationFromWitDataFile_h


// This file contains the class: 
//    LgFrDataInterfaceImplementationFromWitDataFile

#include <rw/regexp.h>
#include <wit/src/wit.h>
#include <scenario/src/difWitR.h>

// Forward reference
class LgFrDataInterfaceEssentials;

//---------------------------------------------------------------------------------------
// To register all global functions for this class
//---------------------------------------------------------------------------------------
void
registerWitFileDifImpFunctions(LgFrDataInterfaceImplementation *dImpWF);

//---------------------------------------------------------------------------------------
// To create a LgFrDataInterfaceImplementationFromWitRun object
//---------------------------------------------------------------------------------------
LgFrDataInterfaceImplementation * 
createWitFileDifImp(const RWCString filename);

//---------------------------------------------------------------------
//
//   Data interface from Wit Data File
//
//---------------------------------------------------------------------

class LgFrDataInterfaceImplementationFromWitDataFile : 
               public LgFrDataInterfaceImplementationFromWitRun 
{

public:

  //-------------------------------------------------------------------
  //  
  //   Methods to obtain global attributes
  //
  //-------------------------------------------------------------------

  // Return problem title
  // This should be the name of the underlying dataset.
  virtual
    RWCString
      title(const LgFrDataInterfaceEssentials* = NULL)
        const;

  //--------------------------------------------------------------------
  //
  //   Methods for processing file in minor ways
  //
  //-------------------------------------------------------------------

  // Find out date in header
  static
    LgFrCalendar
      calendar(const RWCString witDataFileName);

  //--------------------------------------------------------------------
  //  
  //   Dynamic type query methods
  //
  //-------------------------------------------------------------------
  virtual
    isAReturnType
      isA()
        const;
    
  //--------------------------------------------------------------------
  //  
  //   Standard class methods
  //
  //-------------------------------------------------------------------

  // Make a clone copy of this object
  virtual
    LgFrDataInterfaceImplementation *
        clone()
           const;
           
  // Preferred Constructor
  LgFrDataInterfaceImplementationFromWitDataFile(
                                   const RWCString filename);

  // Destructor
  virtual
     ~LgFrDataInterfaceImplementationFromWitDataFile();

  // Assignment operator.
  LgFrDataInterfaceImplementationFromWitDataFile&
    operator=(
           const LgFrDataInterfaceImplementationFromWitDataFile& rhs);

  // Copy constructor
  LgFrDataInterfaceImplementationFromWitDataFile(
           const LgFrDataInterfaceImplementationFromWitDataFile& source);

  // Tests this class.  Only returns if all tests passed
  static
    void
      test ();

  // Context test
  static
    void
      contextTest
        (
	  LgFrDataInterface           * ts1difPtr,	// testScenario1's
	  LgFrLastProblemSolved       & lps,            // testScenario1's
	  LgFrDataInterface    	      * ts1dupDifPtr,	// testScenario1dup's
	  LgFrDataInterface           * ts2difPtr,	// testScenario2's
	  const LgFrScenario          * scenPtr
	    );
  

//--------------
//   specific methods
//----------------
protected:

    virtual
      /* const */ WitRun *
        constWitRun(const LgFrDataInterfaceEssentials* = NULL)
          const;

    virtual
      WitRun *
        mutableWitRun(const LgFrDataInterfaceEssentials* = NULL) ;

    virtual
      void
        defineWitRun()
          const;

private:
  
  RWCString filename_;


  // Default Constructor
  LgFrDataInterfaceImplementationFromWitDataFile();

  // Set problem title (not allowed, since the title is filename_ which 
  // can only be set in the constructor)
  virtual
    void
      title(RWCString t);

};


#endif
