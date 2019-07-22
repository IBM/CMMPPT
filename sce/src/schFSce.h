#ifndef ScheduleFactoryForSce_H
#define ScheduleFactoryForSce_H

#include <scenario/src/schFctry.h>

class LgFrScheduleFactoryForSce: public LgFrScheduleFactory
{
public:

  // Create a new sorting part schedule
  virtual LgFrSortingPartScheduleFloatSmartPointer
    newPartScheduleFromScratch(const LgFrScheduleDescriptor& desc,
		      const bool deepCopy = true,
		      const bool toPopulate = true
		      ) const;

  // Create a new sorting demand schedule
  virtual LgFrSortingDemandScheduleFloatSmartPointer
    newDemandScheduleFromScratch(const LgFrScheduleDescriptor& desc,
		      const bool deepCopy = true,
		      const bool toPopulate = true
		      ) const;

  // Create a copy of the *this on the heap and return a pointer to it.
  virtual
    LgFrScheduleFactory*
      clone (LgFrScenarioSmartPointer)
       const;
  
  // Constructor
  LgFrScheduleFactoryForSce(LgFrScenarioSmartPointer);
   
  // Destructor
  virtual
    ~LgFrScheduleFactoryForSce ();

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

// ---------------------------------------------------------------------
//  PRIVATE MEMBERS
// --------------------------------------------------------------------- 
private:

  // Assignment operator (not allowed, use clone(LgFrScenarioSmartPointer) instead)
  LgFrScheduleFactoryForSce &
    operator=(const LgFrScheduleFactoryForSce& rhs);

  // Copy constructor (not allowed, use clone(LgFrScenarioSmartPointer) instead)
  LgFrScheduleFactoryForSce (const LgFrScheduleFactoryForSce &);

};
#endif


