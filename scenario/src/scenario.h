#ifndef LGFRSCENARIO_H
#define LGFRSCENARIO_H
// RW2STL -- inserted:
#include <string>
#include <vector>
// End of RW2STL -- inserted includes.
// Author: wally

#include <scenario/src/constant.h>
#include <scenario/src/schdDesc.h>
#include <scenario/src/demand.h>
#include <scenario/src/part.h>
#include <scenario/src/initProb.h>
#include <scenario/src/wrkgProb.h>
#include <scenario/src/lastPrbS.h>
#include <scenario/src/implode.h>
#include <scenario/src/explode.h>
#include <scenario/src/fssCtrl.h>
#include <scenario/src/usrInMod.h>
#include <scenario/src/idnumber.h>
#include <scenario/src/pairComp.h>

class LgFrFocussedShortageScheduleControl;	// Forward declarations
class LgFrCalendar;
class LgFrScenarioBuilder;
class LgFrSolutionOutputInterface;
class LgFrScenarioSmartPointer;
class LgFrDataInterfaceAbstract;



// This object can manipulate, implode, and present results for a
// particular scenario.
// Instance responsibilities:
//   -  Create a Working Implosion Problem 
//   -  Create a Last Problem Solved
//   -  Create and maintain lists and dictionaries of all parts and demands
//   -  Help LastProblemSolved create a focused shortage schedule for a 
// given demand/time period set
//   -  Implosion Output Interface


// Role of smart pointers
//  many of the "friend" methods use the scenario object to access
//   member functions. Since you can't really leak memory here,
//   you don't need to smart pointer the arguments to these methods.


// had to add this line in order to avoid syntax error concerning
// the uses of this class in members below.
// vacpp complained that is was ambiguous.
// I don't know why.
// Perhaps in older versions of the compiler the friend declarations
// below also implicitly declared the class as well?
class LgFrScheduleFactory;

class LgFrScenario 
{
   
  friend class LgFrScenarioBuilder;
  friend class LgFrInitialProblemAndParameters;
  friend class LgFrDataInterfaceEssentialsFromScenario;
  friend class LgFrSolutionOutputInterface;
  friend class LgFrScheduleFactory;

  // smart pointer classes
  friend class LgFrScenarioSmartPointerToConst;
  friend class LgFrScenarioSmartPointer;
  friend class LgFrScenarioForATPSmartPointerToConst;
  friend class LgFrScenarioForATPSmartPointer;
  friend class LgFrScenarioForCMRPSmartPointerToConst;
  friend class LgFrScenarioForCMRPSmartPointer;
  friend class LgFrScenarioForDifTestingSmartPointerToConst;
  friend class LgFrScenarioForDifTestingSmartPointer;
  friend class LgFrTestScenarioSmartPointerToConst;
  friend class LgFrTestScenarioSmartPointer;
  friend class LgFrScenarioForSceSmartPointerToConst;
  friend class LgFrScenarioForSceSmartPointer;
  friend class LgFrScenarioForImanSmartPointerToConst;
  friend class LgFrScenarioForImanSmartPointer;
  friend class LgFrScenarioForSmanSmartPointerToConst;
  friend class LgFrScenarioForSmanSmartPointer;

  
  friend void filterStrategiesTest();
  friend void pairCompareStrategiesTest();
  friend void attributeScheduleStrategiesTest();
  friend void dataInterfaceFromMapicsTest();
  friend void dataInterfaceTest();

  // Note: the bases classes for IPP and DIF should have protected methods for
  // accessing the relevant parts of scenario and the subclasses should
  // use them but this has not all been implemented yet.
    
  // LgFrAttributeScheduleStrategy Friends
  friend
     LgFrSetOfParts&
        LgFrAttributeScheduleStrategy::setOfParts(
           LgFrScenario&);

  friend
     LgFrSetOfDemands&
        LgFrAttributeScheduleStrategy::setOfDemands(
           LgFrScenario&);

  friend
     LgFrWorkingImplosionProblem&
        LgFrAttributeScheduleStrategy::workingImplosionProblem(
           LgFrScenario&);

  friend
     LgFrLastProblemSolved&
        LgFrAttributeScheduleStrategy::lastProblemSolved(
           LgFrScenario&);

  friend
     const LgFrLastProblemSolved&
        LgFrAttributeScheduleStrategy::lastProblemSolved(
           const LgFrScenario&)
     const;

  friend
     LgFrImplode&
        LgFrAttributeScheduleStrategy::implode(
           LgFrScenario&);

  friend
     LgFrExplode&
        LgFrAttributeScheduleStrategy::explode(
           LgFrScenario&);

  friend
     LgFrUserInputModification&
        LgFrAttributeScheduleStrategy::userInputModification(
           LgFrScenario&);
  
  friend
     LgFrFocussedShortageScheduleControl&
        LgFrAttributeScheduleStrategy::focussedShortageScheduleControl(
           LgFrScenario&);


  // LgFrPairCompareFloat Friends
  friend
     const LgFrSetOfParts&
        LgFrPairCompareFloat::constSetOfParts(
           int,
           LgFrScenario&);

  friend
     const LgFrSetOfDemands&
        LgFrPairCompareFloat::constSetOfDemands(
           int,
           LgFrScenario&);

  friend
     LgFrLastProblemSolved&
        LgFrPairCompareFloat::lastProblemSolved(
           LgFrScenario&);

  friend
     LgFrWorkingImplosionProblem&
        LgFrPairCompareFloat::workingImplosionProblem(
           LgFrScenario&);

  friend
     LgFrImplode&
        LgFrPairCompareFloat::implode(
           LgFrScenario&);

  // LgFrAlgorithm Friends

  friend
     LgFrFocussedShortageScheduleControl&
        LgFrPairCompareFloat::focussedShortageScheduleControl(
           LgFrScenario&);

  
  friend
     const LgFrLastProblemSolved&
        LgFrFilterStrategy::lastProblemSolved(
           const LgFrScenario&)
     const;

  friend
     LgFrAlgorithm *
        LgFrFocussedShortageScheduleControl::counterPart(
           LgFrScenario&) 
              const;
  
  friend
     LgFrAlgorithm *
        LgFrImplode::counterPart(
           LgFrScenario&) 
              const;
  
  friend
     LgFrAlgorithm *
        LgFrExplode::counterPart(
           LgFrScenario&) 
              const;

  friend
     LgFrAlgorithm *
        LgFrUserInputModification::counterPart(
           LgFrScenario&) 
              const;

public:

  virtual int GetScenarioType() const { return __LGFRSCENARIO; }

  // Create an implosion solution, replacing the old one if it exists
  virtual
    void
      newImplosionSolution ();

  // Create an explosion solution, replacing the old one if it exists
  virtual
    void
      newExplosionSolution ();

  // Write WIT Data file,
  // including changes made in the Working Implosion Problem.
  // This method will overwrite an existing file.
  virtual
    void
      writeWitDataFile(
        const std::string& filename );
 

  // Return the focussed shortage schedule which belongs to this scenario
  virtual
  LgFrFocussedShortageScheduleControl&
     focussedShortageScheduleControl();
  
    // Return the schedule factory which belongs to this scenario
  virtual
    const LgFrScheduleFactory*
     scheduleFactory() const;

  // Return a const setOfParts which belongs to this scenario
  virtual
     const 
        LgFrSetOfParts&
           constSetOfParts(int k = LGFR_DEFAULT_SET)
              const;

  // Return a const setOfDemands which belongs to this scenario
  virtual
     const
        LgFrSetOfDemands&
           constSetOfDemands(int k = LGFR_DEFAULT_SET)
              const;

  
  // Return the "name" of the scenario.  This is a unique name that
  // can be displayed or printed to identify the instance.
  virtual
    std::string
      title()
	const;

  // Return the calendar which belongs to this scenario
  virtual
  LgFrCalendar&
    calendar()

       const;
       
  // Return number of periods
  size_t
    numberOfPeriods() const;
    
  // Get a const ref of LgFrSolutionOutputInterface
  virtual
  const LgFrSolutionOutputInterface &
  solutionOutputInterface()
       const;

  // Set/Get the current national language of the scenario
  virtual
    void
      nationalLanguage(
        LgFrNationalLanguage lang );
  
  virtual
    LgFrNationalLanguage
      nationalLanguage()
        const;
  
  // Return true if this object is usable for anything other than
  // on L.H.S. of assignment.
  virtual
    int
      isValid()
	const;

  // This is the constructor you should use to create a scenario from 
  // "scratch".
  LgFrScenario ( const LgFrDataInterfaceAbstract&,
		 const LgFrCalendar&);

  // Destructor
  virtual
    ~LgFrScenario();



  // Default Constructor
  LgFrScenario();

  // Self-test
  static void test();

  // Return the WorkingImplosionProblem which belongs to this scenario
  virtual
  LgFrWorkingImplosionProblem&
  workingImplosionProblem();

  virtual
  const LgFrWorkingImplosionProblem&
  workingImplosionProblem()
  const;

protected:
  // Copy constructor - uses deep copy
  LgFrScenario(
	       const LgFrScenario& source);

  // Return a non-const setOfParts which belongs to this scenario
  virtual 
     LgFrSetOfParts&
        setOfParts(int k = LGFR_DEFAULT_SET);

  // Return a non-const setOfDemands which belongs to this scenario
  virtual
     LgFrSetOfDemands&
        setOfDemands(int k= LGFR_DEFAULT_SET);

  // Return the LastProblemSolved which belongs to this scenario
  virtual
  LgFrLastProblemSolved&
     lastProblemSolved();
  
  virtual
  const LgFrLastProblemSolved&
     lastProblemSolved()
         const;
  
  // Return the initialProblemAndParameters which belongs to this scenario
  virtual
  LgFrInitialProblemAndParameters&
     initialProblemAndParameters();
  
  // Return the dif which belongs to this scenario
  virtual
  LgFrDataInterfaceAbstract*
     dataInterface()
       const;
  
  // Return the Implode which belongs to this scenario
  virtual
  LgFrImplode&
     implode();

  // Return the Explode which belongs to this scenario
  virtual
  LgFrExplode&
     explode();

  // Return the UserInputModification which belongs to this scenario
  virtual
  LgFrUserInputModification&
     userInputModification();

private:

  // Assignment operator. - do stuff via smart pointer
  LgFrScenario&
     operator=(
	const LgFrScenario& rhs);


  // Sets ippPtr_
  virtual
  void
  initialProblemAndParameters (LgFrInitialProblemAndParameters *);


  // Sets sfPtr_
  virtual
  void
  scheduleFactory (LgFrScheduleFactory*);


  // Sets uniqueName_
  virtual
  void
  uniqueName (std::string);

  // Return a unique name for *this
  virtual
  std::string
  deriveUniqueName() const; 
  

  LgFrSetOfPartsWithUnmovableParts    soParts_;        // Universal Set
  LgFrSetOfDemands                    soDemands_;      // Universal Set
  LgFrCalendar*                       calendarPtr_;
  LgFrNationalLanguage                nationalLanguage_;
  LgFrDataInterfaceAbstract*          difPtr_;
  LgFrScheduleFactory*                sfPtr_;
  LgFrInitialProblemAndParameters*    ippPtr_;
  LgFrLastProblemSolved               lps_;
  LgFrWorkingImplosionProblem         wip_;
  LgFrImplode                         implode_;
  LgFrExplode                         explode_;
  LgFrUserInputModification           userInputModification_;
  LgFrFocussedShortageScheduleControl fssControl_;
  LgFrSolutionOutputInterface*        soiPtr_;
  std::string                         uniqueName_;    // Name of instance

  static std::vector<std::string>     uniqueNameSet_; // This is used to make 
						                              // sure that each 
						                              // instance has a unique
						                              // name
};

#endif
