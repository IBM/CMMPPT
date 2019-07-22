#ifndef LGFRMULTISCHEDULEHELPER_H
#define LGFRMULTISCHEDULEHELPER_H

#include <assert.h>

#include <rw/cstring.h>
#include <rw/collect.h>

#include <scenario/src/idnumber.h>
#include <scenario/src/schdDesc.h>
#include <scenario/src/sortSchd.h>
#include <scenario/src/sortSchP.h>

// Forward declarations
class LgFrScenario;

// This class is for representing LgFrScheduleDescriptor-LgFrSortingScheduleFloat
// pairs
// Note: This is a helper class for LgFrMultiScheduleFloat.
// The comparison and equality functions use only the descriptor
// Note: This class manages the storage of both parts of the pair, even
// though it takes and returns pointers to the LgFrSortingScheduleFloat.
class LgFrDescriptorSchedulePairFloat : 
  // public RWCollectable, 
  public LgFrSubscription  
{
  
  // isA(), newSpecies(), copy(), and some other things are declared
  // by this macro
  RWDECLARE_COLLECTABLE(LgFrDescriptorSchedulePairFloat)
       
public:
  virtual
  unsigned
  hash() 
  const;
       
  virtual
  RWBoolean
  isEqual(const RWCollectable*) 
  const;
       
  // compareTo method, uses DescriptorSchedulePairFloat descriptor to do the
  // comparison.
  // should return 0  if this->descriptor_ "is equal to" source->descriptor_.
  //               >0 if this->descriptor_ "is larger" than source->descriptor_.
  //               <0 if this->descriptor_ "is smaller" than source->descriptor_.
       
  virtual int compareTo(const RWCollectable*) const;


  // Query methods.
  // Descriptor
  const LgFrScheduleDescriptor& descriptor () const;    // Get
  // LgFrSortingScheduleFloat
  LgFrSortingScheduleFloatSmartPointerToConst constSchedule () const;    // Get
  
  // This function is used to return a non-const pointer to the schedule.
  // The schedule's data can be changed using this pointer.
  LgFrSortingScheduleFloatSmartPointer schedule();
  
  // Constructor that sets schedule and descriptor.
  // Note: This method does *not* copy the schedule.  However, this once
  // you give the schedule to the LgFrDescriptorSchedulePairFloat, it "owns"
  // it and the schedule had better be on the heap.  See the destructor
  // description for more.
  LgFrDescriptorSchedulePairFloat(const LgFrScheduleDescriptor& descriptor,
                                  LgFrSortingScheduleFloatSmartPointer schedulePtr); 

  // Create a copy of the *this on the heap for a new scneario
  // and return a pointer to it. (deep copy)
  virtual
    LgFrDescriptorSchedulePairFloat*
      clone (LgFrScenario&)
        const;

  // Create a copy of the *this on the heap for the same scenario
  // and return a pointer to it. (deep copy)
  virtual
    LgFrDescriptorSchedulePairFloat*
      clone ()
        const;
	
  // Default constructor
  LgFrDescriptorSchedulePairFloat();
  
  // Copy constructor
  // Note: This method does a deep copy
  LgFrDescriptorSchedulePairFloat (const LgFrDescriptorSchedulePairFloat& source);
  
  // Copy constructor with new scenario
  // Note: This method does a deep copy
  LgFrDescriptorSchedulePairFloat (const LgFrDescriptorSchedulePairFloat& source,
                                   LgFrScenario& newScenario);
  
  // Destructor
  // Note: This method destroys the schedule
  virtual
    ~LgFrDescriptorSchedulePairFloat();
  
  // Assignment ( in the same scenario )
  // Note: This method does a deep copy
  LgFrDescriptorSchedulePairFloat& 
  operator=(const LgFrDescriptorSchedulePairFloat& rhs);
  
  // Equality
  virtual
  int
  operator==(const LgFrDescriptorSchedulePairFloat&)
  const;
       
  // Print DescriptorSchedulePairFloat data
  virtual void print() const;
       
  // test this class
  // Expects the data in testScenario to come from
  // "/u/implode/wit/data/examples/diner12/wit.data".
  // Expects testScenarioDup to be a copy of testScenario
  // Other expected parameters are described in comments below.
  // contextTest takes ownership of sched1 and sched2.  In other words,
  // it deletes them when it doesn't need them anymore.
  static void contextTest
          (LgFrScenario 			& testScenario,
           const LgFrScheduleDescriptor 	& desc1, // LGFRSupplyVolume
           LgFrSortingScheduleFloatSmartPointer	sched1,// supply schedule
           const LgFrScheduleDescriptor 	& desc2, // LGFRProductionVolume
           LgFrSortingScheduleFloatSmartPointer	sched2,// production schedule
           LgFrScenario 			& testScenarioDup
           );
  
private:
  LgFrScheduleDescriptor descriptor_;
  LgFrSortingScheduleFloatSmartPointer schedule_; // Has to be a pointer so that
                                       // subclasses can be used.
};

#endif
