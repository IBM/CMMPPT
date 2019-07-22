// ScenarioFacade - This is an object to encapsilate a scenario, dif, ip,
//                   and mulitSchedules.
//                   The intent is to allow an application (GUI) using the 
//                   framework to interface with a single object.  This will
//                   reduce the generallity of the framework, but may provide
//                   a simplied interface to the framework.
//
// This object follows the Facade pattern
// 
#ifndef cmrpFac_h
#define cmrpFac_h

#include <stddef.h>

#include <rw/hashdict.h>
#include <rw/cstring.h>

#include <scenario/src/scenP.h>
#include <scenario/src/scCMRPP.h>
#include <scenario/src/generVc.h>
#include <scenario/src/schdDesc.h>

//Forward declarations
class LgFrMultiScheduleFloat;

class LgFrScenarioFacade
{

public:
  
  // do an implosion
  void newImplosionSolution();

  // create a new multiSchedule.
  // returns TRUE if multischedule created, otherwise false.
  // multiScheduleName : client specified name which will be used to identify this
  //                     new multiSchedule. An existing multiSchedule with this
  //                     name can not previously exist
  // schedType         : Part or Demand. Identifies type of multiSchedule to be
  //                     created.
  RWBoolean newMultiSchedule(
                             const RWCString multiScheduleName,
                             const RWCString schedType );

  // Return a list of vaild attributes which can be added to a multiSchedule.
  // schedType         : Part or Demand. Identifies type of attribute list to
  //                     be returned.
  virtual
    LgFrVectorRWCString
      supportedScheduleDescriptors(
        const RWCString schedType )
      const;
  
  // add a schedule to a multiSchedule.
  // returns TRUE if schedule created, otherwise false.
  // multiScheduleName : name of an existing multiSchedule.
  // attributeName     : name attribute to be added to multiSchedule
  virtual
    RWBoolean
      newSchedule(
        const RWCString multiScheduleName,
         const RWCString attributeName );

  // Return a multiSchedule
  RWCString multiScheduleFormat( RWCString multScheduleName ) const;

  // Change value in multiSchedule
  // Return TRUE : schedule is found and changed.
  //        FALSE: schedule is not found.
  // timeVecIndex: The first element is element 0.
  virtual
    RWBoolean
      changeValueInMultiSchedule(
                            const RWCString multiScheduleName,
			    const RWCString scheduleDescriptor,
                            const RWCString item,
			    const int timeVecIndex,
			    const float newValue);

  // Return true if proposed change to multiSchedule is valid
  virtual
    RWBoolean
      validValueChangeInMultiSchedule(
        const RWCString multiScheduleName,
        const RWCString scheduleDescriptor,
        const RWCString item,
        const int timeVecIndex,
        const float newValue )
          const;
  
  // Can an attribute value in a multi schedule be changed.
  // Return TRUE : value can be changed
  //        FALSE" value can not be changed
  virtual
    RWBoolean
      mutableAttributeInMultiSchedule(
                            const RWCString multiScheduleName,
			    const RWCString scheduleDescriptor
			    )
        const;
   
  // Default Constructor.
  LgFrScenarioFacade();
  
  // Destructor
  virtual
  ~LgFrScenarioFacade();

  private:
  
  // Copy Constructor
  LgFrScenarioFacade( const LgFrScenarioFacade & );

  // Assignment operator
  LgFrScenarioFacade & operator=(const LgFrScenarioFacade & rhs);

protected:
  
  void scenarioSPtr( const LgFrScenarioSmartPointer & scenSPtr );
  LgFrScenarioSmartPointer scenarioSPtr() const; 
  LgFrScenarioSmartPointer scenarioSPtr_;

  // Get schedule descriptor from schedule descriptor name
  LgFrScheduleDescriptor
    scheduleDescriptor(
      RWCString scheduleDescriptorName )
    const;

private:

  // Find multischedule pointer in multiScheduleDictionary using
  // multiSchedule name as lookup key.
  virtual
    LgFrMultiScheduleFloat * findMultiSchedule(
      const RWCString multiScheduleName )
        const;

  // Find schedule descriptor in multischedule from string representation of
  // the multiSchedule.
  // If the descriptor is found then found is returned as true.
  virtual
    const LgFrScheduleDescriptor findScheduleDescriptorInMultiSchedule(
      const RWCString multiScheduleName,
      const RWCString scheduleDescriptor,
      RWBoolean & found )
        const;
  
  // Dictionary of multiSchedules. Key is string identifying multiSchedule.
  RWHashDictionary multiScheduleDictionary_;
};

class LgFrCmrpScenarioFacade : public LgFrScenarioFacade
{

public:
  
  void scenarioForCmrpSPtr( const LgFrScenarioForCMRPSmartPointer & scenSPtr );
  LgFrScenarioForCMRPSmartPointer scenarioForCmrpSPtr() const; 
   
  // Default Constructor.
  LgFrCmrpScenarioFacade();
  
  // Destructor
  virtual
  ~LgFrCmrpScenarioFacade();

  // test this class
  static void test();
  
private:
  
  // Copy Constructor
  LgFrCmrpScenarioFacade( const LgFrCmrpScenarioFacade & );

  // Assignment operator
  LgFrCmrpScenarioFacade & operator=(const LgFrCmrpScenarioFacade & rhs);

};

#endif
