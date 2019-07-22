// Author: wally and Jung-Mu

#include <rw/rstream.h>

#include <scenario/src/descSchd.h>
#include <scenario/src/schdDesc.h>
#include <scenario/src/sortSchP.h>

// ---------------------------------------------------------------------
// isA(), newSpecies(), copy(), and some other things are defined
// by this macro
// ---------------------------------------------------------------------

RWDEFINE_COLLECTABLE(LgFrDescriptorSchedulePairFloat,
                     __LGFRDESCRIPTORSCHEDULEPAIRFLOAT)
     
// ---------------------------------------------------------------------
// Print out the data
// ---------------------------------------------------------------------
void 
LgFrDescriptorSchedulePairFloat::print() const
{
  cout << "\n*******************************************************************";
  cout << "\nDescriptor data for this LgFrDescriptorSchedulePairFloat: ";
  cout << descriptor_.name();
  cout << "\nschedule size = " << schedule_->entries() << endl;
  cout << "\nSchedule data: \n";
  schedule_->print();
}

// ---------------------------------------------------------------------
// hash method
// ---------------------------------------------------------------------

unsigned
LgFrDescriptorSchedulePairFloat::hash() const
{
  return (descriptor_.name().hash());
}

// ---------------------------------------------------------------------
// isEqual method, returns TRUE if they have the same descriptor
// ---------------------------------------------------------------------
RWBoolean
LgFrDescriptorSchedulePairFloat::isEqual(
                                         const RWCollectable* t
                                         ) const
{
  assert (t->isA()==__LGFRDESCRIPTORSCHEDULEPAIRFLOAT);
  const LgFrDescriptorSchedulePairFloat *rhs = (const LgFrDescriptorSchedulePairFloat*) t;
  return (this->descriptor_ == rhs->descriptor_);
}

// ---------------------------------------------------------------------
// compareTo method, uses descriptor to do the comparison.
// should return 0  if *this "is equal to" *source.
//               >0 if *this "is larger" than *source.
//               <0 if *this "is smaller" than *source.
// C lib. function memcmp() is used to do the comparison.
// ---------------------------------------------------------------------
int 
LgFrDescriptorSchedulePairFloat::compareTo(
                                           const RWCollectable* source
                                           ) const
{
  assert (source->isA()==__LGFRDESCRIPTORSCHEDULEPAIRFLOAT);
  const LgFrDescriptorSchedulePairFloat* sourceDSPPtr = 
    (const LgFrDescriptorSchedulePairFloat*)source;
  int majorCompare 
    = (  (this->descriptor_.name() ) . compareTo(
                                          sourceDSPPtr->descriptor_.name() ) 
       );
  return majorCompare;
}

// ---------------------------------------------------------------------
// This function is only used to query the descriptor, not used to set it.
// ---------------------------------------------------------------------
const LgFrScheduleDescriptor&
LgFrDescriptorSchedulePairFloat::descriptor() const
{
  return (descriptor_);
}

// ---------------------------------------------------------------------
// This function is only used to query the schedule, not used to set it.
// ---------------------------------------------------------------------
LgFrSortingScheduleFloatSmartPointerToConst
LgFrDescriptorSchedulePairFloat::constSchedule() const
{
  return (schedule_);
}

// ---------------------------------------------------------------------
// This function is used to return a non-const pointer to the schedule.
// The schedule's data can be changed using this pointer.
// ---------------------------------------------------------------------
LgFrSortingScheduleFloatSmartPointer
LgFrDescriptorSchedulePairFloat::schedule() 
{
  return (schedule_);
}

// ---------------------------------------------------------------------
// Equality test
// ---------------------------------------------------------------------
int
LgFrDescriptorSchedulePairFloat::operator== (const LgFrDescriptorSchedulePairFloat& rhs)
const
{
  return (this->isEqual (&rhs));
}

// ---------------------------------------------------------------------
// Constructor that sets schedule and descriptor.
// Note: This method does *not* copy the schedule.
// ---------------------------------------------------------------------
LgFrDescriptorSchedulePairFloat::
LgFrDescriptorSchedulePairFloat(const LgFrScheduleDescriptor& descriptor,
                                LgFrSortingScheduleFloatSmartPointer schedulePtr)
: 
  LgFrSubscription(),
  descriptor_ (descriptor), schedule_ (schedulePtr)
{
  if ( !schedule_.null() ) schedule_->addDependent( this );
}

// ---------------------------------------------------------------------
// Default constructor
// ---------------------------------------------------------------------
LgFrDescriptorSchedulePairFloat::LgFrDescriptorSchedulePairFloat()
: 
  LgFrSubscription(),
  descriptor_ (),
  schedule_ (NULL)	// Needed to avoid bug in cfront
{
  // All the work is done in the initialization part
}

// ---------------------------------------------------------------------
// Copy constructor
// ---------------------------------------------------------------------
LgFrDescriptorSchedulePairFloat::LgFrDescriptorSchedulePairFloat
  (const LgFrDescriptorSchedulePairFloat& source)
: 
  LgFrSubscription(),
  descriptor_ (source.descriptor_),
  schedule_ (source.schedule_->clone())
{
  if ( !schedule_.null() ) schedule_->addDependent( this );
}

// ---------------------------------------------------------------------
// Copy constructor with new Scenario
// ---------------------------------------------------------------------
LgFrDescriptorSchedulePairFloat::LgFrDescriptorSchedulePairFloat (
   const LgFrDescriptorSchedulePairFloat& source,
   LgFrScenario& newScenario)
:
  LgFrSubscription(),
  descriptor_ (source.descriptor_),
  schedule_ (source.schedule_->clone(newScenario))
{
  if ( !schedule_.null() ) schedule_->addDependent( this );
}

// ---------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------
LgFrDescriptorSchedulePairFloat::~LgFrDescriptorSchedulePairFloat()
{
  if ( !schedule_.null() ) schedule_->removeDependent( this );
}

// ---------------------------------------------------------------------
// Create a copy of the *this on the heap for the same scenario
// and return a pointer to it.
// ---------------------------------------------------------------------
LgFrDescriptorSchedulePairFloat*
  LgFrDescriptorSchedulePairFloat::clone ()
     const
{
  return new LgFrDescriptorSchedulePairFloat(*this);
}

// ---------------------------------------------------------------------
// Create a copy of the *this on the heap for a new scenario
// and return a pointer to it.
// ---------------------------------------------------------------------
LgFrDescriptorSchedulePairFloat*
  LgFrDescriptorSchedulePairFloat::clone (LgFrScenario& newScenario)
     const
{
  return new LgFrDescriptorSchedulePairFloat(*this, newScenario);
}
// ---------------------------------------------------------------------
// Assignment  (in the same scenario)
// ---------------------------------------------------------------------
LgFrDescriptorSchedulePairFloat&
LgFrDescriptorSchedulePairFloat::operator=(const LgFrDescriptorSchedulePairFloat& rhs)
{
  if (this != &rhs) {
    descriptor_ = rhs.descriptor_;
    if ( !schedule_.null() ) schedule_->removeDependent( this );
    schedule_ = rhs.schedule_->clone();
    if ( !schedule_.null() ) schedule_->addDependent( this );
  }
  return (*this);
}

// ---------------------------------------------------------------------
// test LgFrDescriptorSchedulePairFloat
// ---------------------------------------------------------------------

#include <scenario/src/schdDeSt.h>

#ifdef NDEBUG
#undef NDEBUG
#endif

// Expects the data in testScenario to come from
// "/u/implode/wit/data/examples/diner12/wit.data".
// Expects testScenarioDup to be a copy of testScenario
void
LgFrDescriptorSchedulePairFloat::contextTest
  (LgFrScenario 			& /* testScenario */,
   const LgFrScheduleDescriptor 	& desc1,
   LgFrSortingScheduleFloatSmartPointer	sched1,	// testScenario's
   const LgFrScheduleDescriptor 	& desc2,
   LgFrSortingScheduleFloatSmartPointer	sched2,	// testScenario's
   LgFrScenario 			& testScenarioDup
   )
{
  const LgFrScheduleDescriptor supplyD (LGFRSupplyVolume);
  const LgFrScheduleDescriptor productD (LGFRProductionVolume);
  // Test correctness of arguments (partially)
  assert (desc1 == supplyD);
  assert (desc2 == productD);

  // Test constructors and clone methods
  LgFrDescriptorSchedulePairFloat defaultDSPair; 
  LgFrDescriptorSchedulePairFloat supplyDSPair (supplyD, sched1);
  LgFrDescriptorSchedulePairFloat * supplyDSPairdupPtr = supplyDSPair.clone();
  LgFrDescriptorSchedulePairFloat * supplyDSPairExtraPtr = supplyDSPair
                                                           .clone(testScenarioDup);
  LgFrDescriptorSchedulePairFloat productDSPair(productD, sched2);
  LgFrDescriptorSchedulePairFloat productDSPairdup (productDSPair);

  // Test assignment
  defaultDSPair = supplyDSPair;
  assert (defaultDSPair.descriptor() == supplyDSPair.descriptor() );
  assert (defaultDSPair.schedule() -> entries()
          == supplyDSPair.schedule() -> entries() );
  {
    // change the left operand of the assignment and compare it to the right
    // operand to make sure the assignment really did a deep copy
    defaultDSPair.schedule() -> localClear();
    assert (defaultDSPair.schedule() -> entries() == 0
            && supplyDSPair.schedule() -> entries() != 0);
  }
  
  // Test comparison
  assert (supplyDSPair == defaultDSPair);
  assert (supplyDSPair == *supplyDSPairdupPtr);
  assert (productDSPair == productDSPairdup);
  assert (! (supplyDSPair == productDSPair));
  assert (! (defaultDSPair   == productDSPair));
  assert (! (*supplyDSPairdupPtr == productDSPair));

  // Test hashing, isEqual, compareTo
  assert (supplyDSPair.hash() == defaultDSPair.hash());
  assert (defaultDSPair.isEqual(supplyDSPairdupPtr));
  assert (defaultDSPair.compareTo(&supplyDSPair) == 0);
  assert (supplyDSPair.compareTo(&productDSPair) > 0);
  assert (productDSPair.compareTo(supplyDSPairdupPtr) < 0);

  // Test query methods
  assert (productDSPair.descriptor() == productD);
  int nPartsInDiner12 = supplyDSPairdupPtr -> constSchedule() -> entries();
  assert ( nPartsInDiner12 == 19 );
  assert ( nPartsInDiner12 == supplyDSPairdupPtr -> schedule() -> entries() );
  assert ( nPartsInDiner12 == supplyDSPairExtraPtr -> schedule() -> entries() );

  // test destructors
  delete supplyDSPairdupPtr;
  delete supplyDSPairExtraPtr;
}
