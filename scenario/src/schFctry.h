#ifndef ScheduleFactory_H
#define ScheduleFactory_H
// RW2STL -- inserted:
#include <map>
#include <string>
// End of RW2STL -- inserted includes.

// This is a base class used to create default set of 
// sorting[Part|Demand]ScheduleFloat.  The derived subclass of this class
// will implement the code to handle application-specific cases.

// With a unique string passed in, a set of objects designed to work
// together will be created with respect to the particular application.  
// They are sorting[Part|Demand]ScheduleStrategy,
// specific kind of setOf[Parts|Demands], and the resulted
// sorting[Part|Demand]ScheduleFloat.
// The sorting[Part|Demand]ScheduleFloat will be returned either populated
// or not depending on the population flag.

// sorting[Part|Demand]ScheduleStrategy is set in this class and will be cloned/
// owned by the schedule.  This class has the reference to the scenario that it
// belongs to so that it can get the reference of sop/sod.

#include <scenario/src/schdDesc.h>
#include <scenario/src/lastPrbS.h>
#include <scenario/src/scenP.h>
#include <scenario/src/demdSchP.h>
#include <scenario/src/partSchP.h>

enum schdType { INPUT, OUTPUT };

// Forword declaration
class LgFrScenario;
class LgFrSortingScheduleFloat;
class LgFrSortingPartScheduleFloatSmartPointer;
class LgFrSortingDemandScheduleFloatSmartPointer;



class LgFrFilterStrategy;
class LgFrPartScheduleStrategy;
class LgFrDemandScheduleStrategy;
class LgFrScenarioBuilder;
class LgFrScenarioSmartPointer;



typedef std::map<std::string,LgFrSortingDemandScheduleFloatSmartPointer> NameDemandDictionary;
typedef std::map<std::string,LgFrSortingPartScheduleFloatSmartPointer> NamePartDictionary;

class LgFrScheduleFactory
{

friend class LgFrScenarioBuilder;
  
public:

  
  // Create a new sorting part schedule and the default is to populate.
  virtual LgFrSortingPartScheduleFloatSmartPointer
    newPartSchedule  (const LgFrScheduleDescriptor& desc,
		      const bool deepCopy = true,
		      const bool toPopulate = true
		      ) const;

  // Create a new sorting demand schedule and the default is to populate.
  virtual LgFrSortingDemandScheduleFloatSmartPointer
    newDemandSchedule(const LgFrScheduleDescriptor& desc,
		      const bool deepCopy = true,
		      const bool toPopulate = true
		      ) const;

  // Removes the schedule factory's links to the schedules it has already created.
  // Call this before trying to delete the factory's scenario.  (See the comment
  // before this member function's implementation for the reason.)
  virtual
  void
  clearAndDestroy();

  // Return an abbreviation for a given LgFrScheduleDescriptor
  virtual std::string
    abbreviationOf (const LgFrScheduleDescriptor & desc) const;

  // Return list of schedule descriptors which can be used by factory to
  // create schedules.
  virtual
   LgFrScheduleDescriptorVector
     demandScheduleDescriptors()
       const;
  virtual
   LgFrScheduleDescriptorVector
     partScheduleDescriptors()
       const;

    
  // Create a copy of the *this on the heap and return a pointer to it.
  virtual
    LgFrScheduleFactory*
      clone (LgFrScenarioSmartPointer)
        const;

  // Constructor
  LgFrScheduleFactory(LgFrScenarioSmartPointer);
   
  // Destructor
  virtual
    ~LgFrScheduleFactory ();

  // test this class
  static void test();
  // Expects *testScenario to contain data from
  // /u/implode/wit/data/examples/diner12/wit.data and expects
  // f to be the scedule factory for testScenario
  static void contextTest(const LgFrScheduleFactory & f,
			  LgFrScenarioSmartPointer & testScenario);


  // ---------------------------------------------------------------------
  // Protected member functions
  // ---------------------------------------------------------------------
protected:
    
  // ---------------------------------------------------------------------
  // Create a Part Schedule and set it's strategy to partSS strategy
  // and optional filterstrategy
  // ---------------------------------------------------------------------
  LgFrSortingPartScheduleFloatSmartPointer 
  createPartSched(
     const LgFrPartScheduleStrategy & partSS,
     const LgFrFilterStrategy * filterStrat = 0,
     const LgFrPairCompareFloat * pairComparePtr = 0,
     const bool ascending = true
                    ) const;
  
  // ---------------------------------------------------------------------
  // Create a Demand Schedule and set it's strategy to demandSS strategy
  // and optional filterstrategy
  // ---------------------------------------------------------------------
  LgFrSortingDemandScheduleFloatSmartPointer
  createDemandSched(
     const LgFrDemandScheduleStrategy & demandSS,
     const LgFrFilterStrategy * filterStrat = 0,
     const LgFrPairCompareFloat * pairComparePtr = 0,
     const bool ascending = true
                    ) const;
  
  LgFrScenarioSmartPointer scenario() const;

  // Return the LastProblemSolved which belongs to this scenario
  LgFrLastProblemSolved& lastProblemSolved() const;

  // Return the schedule type: INPUT or OUTPUT of implosion run.
  virtual schdType
    scheduleType(const LgFrScheduleDescriptor& desc,
		 const LgFrAttributeScheduleStrategy & ass) const;

  // Return a schedule created by using a previously-created schedule
  virtual
  LgFrSortingPartScheduleFloatSmartPointer
  newPartScheduleFromExistingOne (const LgFrScheduleDescriptor& desc,
				  const bool deepCopy,
				  const bool toPopulate,
				  LgFrSortingPartScheduleFloatSmartPointer 
				    schedulePtrPtr)
      const;

  // Return a schedule without getting it from the list of previously-
  // constructed schedules
  virtual
  LgFrSortingPartScheduleFloatSmartPointer 
  newPartScheduleFromScratch (const LgFrScheduleDescriptor& desc,
			      const bool deepCopy,
			      const bool toPopulate
			      )
      const;

  // Return a schedule without getting it from the list of previously-
  // constructed schedules
  virtual
  LgFrSortingDemandScheduleFloatSmartPointer 
  newDemandScheduleFromScratch (const LgFrScheduleDescriptor& desc,
				const bool deepCopy,
				const bool toPopulate
				)
      const;

  // Return a schedule created by using a previously-created schedule
  virtual
  LgFrSortingDemandScheduleFloatSmartPointer
  newDemandScheduleFromExistingOne (const LgFrScheduleDescriptor& desc,
				    const bool deepCopy,
				    const bool toPopulate,
				    LgFrSortingDemandScheduleFloatSmartPointer
				      schedulePtrPtr)
      const;

  // dictionaries for part and demand schedules already allocated by
  //   this schedule factory

  // Returns the factory's demandSchedules_
  NameDemandDictionary &  demandSchedules();

  // Returns the factory's partSchedules_
  NamePartDictionary &   partSchedules();


  void insertDemandSchedule(const LgFrScheduleDescriptor&,
		     LgFrSortingDemandScheduleFloatSmartPointer);

  void insertPartSchedule(const LgFrScheduleDescriptor&,
		     LgFrSortingPartScheduleFloatSmartPointer);

  // Vector of descriptors which this factory supports.
  static LgFrScheduleDescriptorVector demandScheduleDescriptors_;  
  static LgFrScheduleDescriptorVector partScheduleDescriptors_;
  
  // ---------------------------------------------------------------------
  //  PRIVATE MEMBERS
  // --------------------------------------------------------------------- 
private:

  // used to initialize demandScheduleDescriptors_ and partScheduleDescriptors_ 
  static LgFrScheduleDescriptorVector initializeDemandScheduleDescriptors();
  static LgFrScheduleDescriptorVector initializePartScheduleDescriptors();

  // Assignment operator (not allowed , use clone(LgFrScenarioSmartPointer)
  // instead )
  LgFrScheduleFactory &
    operator=(const LgFrScheduleFactory& rhs);   

  // Copy constructor (not allowed , use clone(LgFrScenarioSmartPointer) 
  // instead )
  LgFrScheduleFactory (const LgFrScheduleFactory &);

  // Default constructor (not allowed , use a different constructor instead)
  LgFrScheduleFactory ();

  virtual void printWarning () const;

  LgFrScenarioSmartPointer scenarioPtr_;

  typedef std::pair<std::string,LgFrSortingDemandScheduleFloatSmartPointer> NameDemandPair;
  NameDemandDictionary demandSchedules_;

  typedef std::pair<std::string,LgFrSortingPartScheduleFloatSmartPointer> NamePartPair;
  NamePartDictionary partSchedules_;
};

#endif
