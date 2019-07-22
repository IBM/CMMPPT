#ifndef ScheduleFactoryForCMRP_H
#define ScheduleFactoryForCMRP_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/schFctry.h>

class LgFrScheduleFactoryForCMRP: public LgFrScheduleFactory
{
public:

  
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
  LgFrScheduleFactoryForCMRP(LgFrScenarioSmartPointer);
   
  // Destructor
  virtual
    ~LgFrScheduleFactoryForCMRP ();

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
  
  // Assignment operator (not allowed, use clone(LgFrScenarioSmartPointer) instead)
  LgFrScheduleFactoryForCMRP &
    operator=(const LgFrScheduleFactoryForCMRP& rhs);

  // Copy constructor (not allowed, use clone(LgFrScenarioSmartPointer) instead)
  LgFrScheduleFactoryForCMRP (const LgFrScheduleFactoryForCMRP &);

};
#endif


