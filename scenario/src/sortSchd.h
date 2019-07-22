#ifndef LGFR_SORTING_SCHEDULE_H
#define LGFR_SORTING_SCHEDULE_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

// This a base class which uses the Strategy Pattern for sorting schedules.

#include <scenario/src/model.h>

#include <scenario/src/generVc.h>
#include <scenario/src/schedLF.h>
#include <scenario/src/schdStrt.h>

// Forward references
class LgFrAttributeScheduleStrategy;
class LgFrPairCompareFloat;
class LgFrFilterStrategy;
class LgFrScenario;
class LgFrSortingScheduleFloatSmartPointer;

class LgFrSortingScheduleFloat : 
   public LgFrScheduleFloat,
   public LgFrSubscription
{
   
   friend class LgFrScheduleFactory;
   friend class LgFrMultiScheduleFloat;
   friend class LgFrMultiScheduleAttributeIterator;
   friend class LgFrMultiScheduleItemIterator;

public:

  // Methods inherited from LgFrSchedule include:
  //  contains, findValue, entries

  // Performs sort.
  // If the comparisonStrategy parameter is 0 then the schedule is
  // returned to its original order.
  virtual
    void
      sort(
         const LgFrPairCompareFloat* const comparisonStrategy = 0,
         const bool ascending = true );

  // Get comparison strategy.
  // If this object ceases to exist, 
  // then the returned pointer will be dangling.
  virtual
    const LgFrPairCompareFloat * 
        compareStrategy ()
  const;

  // Return true if schedule is in original unsorted order.
  // true is returned iff compareStrategy()==0 && isSortAscending().
  virtual
    bool
      isInNaturalOrder()
         const;

  // Return true if schedule is in ascending order
  virtual
    bool
      isSortAscending()
         const;

  // Fill in the schedule using the schedule strategy
  virtual
     void
        populate();

  // Set the filter startegy
  virtual
     void
        filterStrategy(
           const LgFrFilterStrategy & );

  // Get Filter Strategy
  // If this object ceases to exist or changes its filterStrategy
  // then the returned reference will be dangling
  virtual
     const LgFrFilterStrategy &
        filterStrategy()
           const;

  // True iff the data in this schedule can be changed by "ordinary"
  // clients.  If false, this schedule has an associated ASS that updates
  // the schedule but that doesn't allow changes in the schedule to be
  // propagated into the scenario subsystem.
  virtual
    bool
      isMutable ();

  // True iff the data in this schedule is in the acceptable range of 
  // values.  
  virtual
    LgFrValidity
      validity ( const LgFrTimeVecFloat& );

  // Add pair of item - TimeVecFloat's to the end of the collection.
  virtual
     void
       insert (const LgFrItemTimeVecPairFloat pair);

  // Add pair of item - TimeVecFloat's to the end of the collection
  // without propagating the change to other parts of the system
  virtual
     void
       localInsert (const LgFrItemTimeVecPairFloat& pair);

  // Add new item to the collection at position index.  Everything afterwards
  // is shifted to a higher index.
  virtual
     void
        insertAt (const size_t index,
                  const LgFrItemTimeVecPairFloat pair);

  // Add new item to the collection at position index without propagating
  // the change to other parts of the system.  Everything afterwards
  // is shifted to a higher index.
  virtual
     void
        localInsertAt (const size_t index,
		       const LgFrItemTimeVecPairFloat& pair);

  // Insert Item if it isn't already in the schedule.  In any case,
  // associate timeVec with item.
  void
     insertKeyAndValue(
        const LgFrItem& item,
        const LgFrTimeVecFloat& value);
 
  // Insert Item if it isn't already in the schedule without propagating
  // the change to other parts of the system.  In any case,
  // associate timeVec with item.
  void
     localInsertKeyAndValue(
        const LgFrItem& item,
        const LgFrTimeVecFloat& value);


  // Make clone copy of this object
  virtual
     LgFrSortingScheduleFloat *
        clone() 
           const;

  // Make clone copy of this object with a new Scenario
  virtual
     LgFrSortingScheduleFloat *
        clone(LgFrScenario &) 
           const;

 
  // Return the i'th ItemTimeVecPairFloat
  virtual
     LgFrItemTimeVecPairFloat
        operator[](const size_t& i )
           const;

  // Since this derived class defines one overloading of operator[], it
  // hides all of the inherited overloadings of operator[], so here they are.

  virtual
     const LgFrItemTimeVecPairFloat &
        operator[](const LgFrItem& item)
  const;

  // Remove all pairs from schedule without propagating
  // the change to other parts of the system.
  // Pairs are ItemTimeVecPairFloat.
  virtual
     void
        localClear();

  // Change the size of the schedule
  virtual
     void
        reserve (
           const size_t newCapacity);

  // localUpdate - This method is invoked when the data used by the populate
  // method has changed.
  virtual
     void
        localUpdate (
           LgFrModel * p,
           void * );

  // Perform diadic schedule operation.
  // vectorOp is the operation applied to the timeVecFloat portions
  // of the same items in the lhs and rhs schedule.
  // If an item is not in the lhs or rhs schedule then a vector of
  // zero is used.
  //
  // For example:
  //
  //   LgFrSortingScheduleFloat lhs, rhs;
  //   LgFrSortingScheduleFloat
  //      diff( lhs.op(rhs,LgFrTimeVecFloat::subtract);
  //
  //   diff contains lhs - rhs
  //
  LgFrSortingScheduleFloat
  op(
     LgFrTimeVecFloatOpFunction vectorOp, 
     const LgFrSortingScheduleFloat& rhs,
     float defaultValue = 0.0f )
  const;
  
   // Perform operation on column (timePeriod) of schedule.
   //
   // See LgFrSchedule::columnOp for further details.
   LgFrSortingScheduleFloat
      columnOp(
         const size_t period,
         float (*scalarOperation) (const float, const float),
         const float rhs )
      const;
  
  // Assignment operator
  LgFrSortingScheduleFloat &
    operator=(
	      const LgFrSortingScheduleFloat& rhs);

  // Copy constructor
  // The resulting copy has the same natural ordering as the source schedule.
  LgFrSortingScheduleFloat (const LgFrSortingScheduleFloat & source);

  // Copy constructor with new Scenario
  // The resulting copy has the same natural ordering as the source schedule.
  LgFrSortingScheduleFloat (const LgFrSortingScheduleFloat & source,
                            LgFrScenario &);

  // Constructor for a set expected to have up to n parts 
  // (and default constructor)
  LgFrSortingScheduleFloat (const size_t n = 0);

  // Destructor
  ~LgFrSortingScheduleFloat ();

  // Print
  void
     print()
        const;

  // test this class
  static void test();

protected:

  // Set Schedule Strategy
  void
     attributeScheduleStrategy(
        const LgFrAttributeScheduleStrategy & );

  // Get Schedule Strategy
  // If this object ceases to exist or changes its attributeScheduleStrategy,
  // then the returned pointer will be dangling
  const LgFrAttributeScheduleStrategy *
     attributeScheduleStrategy()
        const;

  virtual 
     LgFrItemTimeVecPairFloat &
        operator[](const size_t& i );

  virtual
     LgFrItemTimeVecPairFloat &
        operator[](const LgFrItem& item);

private:

  LgFrSize_tVector   permutations_;
  LgFrPairCompareFloat *    compareStrategyPtr_;
  LgFrAttributeScheduleStrategy *    attributeScheduleStrategyPtr_;
  LgFrFilterStrategy *      filterStrategyPtr_;
  bool                 ascending_;

  // If and when LgFr is running in a multiprocess environment
  // then this routine will implement a lock on the global
  // variables used to implement sorting.
  virtual
    void
      prepareToSort();

  // If and when LgFr is running in a multiprocess environment
  // then this routine will implement an unlock on the global
  // variables used to implement sorting.
  virtual
    void
      doneSorting();

  // Set comparison strategy
  virtual
     void
        compareStrategy (
           const LgFrPairCompareFloat * const);

  // Inform *attributeScheduleStrategyPtr_ of a pair that
  // is being changed in *this, if *this has an *attributeScheduleStrategyPtr_.
  virtual
    void
      propagateUpdate (
		       const LgFrItemTimeVecPairFloat &);

};


// Addition Operator
LgFrSortingScheduleFloat
   operator+ (
      const LgFrSortingScheduleFloat& lhs,
      const LgFrSortingScheduleFloat& rhs);

// Subtraction Operator
LgFrSortingScheduleFloat
   operator- (
      const LgFrSortingScheduleFloat& lhs,
      const LgFrSortingScheduleFloat& rhs);



#endif
