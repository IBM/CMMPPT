#ifndef ScheduleFactoryForATP_H
#define ScheduleFactoryForATP_H

#include <rw/cstring.h>
#include <scenario/src/schFctry.h>

class LgFrScheduleFactoryForATP: public LgFrScheduleFactory
{
public:

  // Create a copy of the *this on the heap and return a pointer to it.
  virtual
    LgFrScheduleFactory*
      clone (LgFrScenarioSmartPointer)
       const;
  
  // Constructor
  LgFrScheduleFactoryForATP(LgFrScenarioSmartPointer);
   
  // Destructor
  virtual
    ~LgFrScheduleFactoryForATP ();

  // hash table initializer
  virtual
  void
  initHashTable ();

  // test this class
  static void test();
  static void contextTest(const LgFrScheduleFactory & f);
  

// ---------------------------------------------------------------------
//  PROTECTED MEMBERS
// --------------------------------------------------------------------- 
protected:

  // Return the schedule type: INPUT or OUTPUT 
  virtual schdType
    scheduleType(const LgFrScheduleDescriptor& desc,
		 const LgFrAttributeScheduleStrategy & ass) const;

  // Return a schedule without getting it from the list of previously-
  // constructed schedules
  virtual
  LgFrSortingPartScheduleFloatSmartPointer 
  newPartScheduleFromScratch (const LgFrScheduleDescriptor& desc,
			      const RWBoolean deepCopy,
			      const RWBoolean toPopulate
			      )
      const;

  // Return a schedule without getting it from the list of previously-
  // constructed schedules
  virtual
  LgFrSortingDemandScheduleFloatSmartPointer 
  newDemandScheduleFromScratch (const LgFrScheduleDescriptor& desc,
				const RWBoolean deepCopy,
				const RWBoolean toPopulate
				)
      const;


// ---------------------------------------------------------------------
//  PRIVATE MEMBERS
// --------------------------------------------------------------------- 
private:

  // Assignment operator (not allowed, use clone(LgFrScenarioSmartPointer) instead)
  LgFrScheduleFactoryForATP &
    operator=(const LgFrScheduleFactoryForATP& rhs);

  // Copy constructor (not allowed, use clone(LgFrScenarioSmartPointer) instead)
  LgFrScheduleFactoryForATP (const LgFrScheduleFactoryForATP &);

};
#endif


