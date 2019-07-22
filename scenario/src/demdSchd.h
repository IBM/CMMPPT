#ifndef LGFR_SORTING_DEMAND_SCHEDULE_H
#define LGFR_SORTING_DEMAND_SCHEDULE_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.


#include <scenario/src/sortSchd.h>
#include <scenario/src/machdep.h>
//#include <scenario/src/demdBgPC.h>

// Forward reference
class LgFrDemandScheduleStrategy;


class LgFrSortingDemandScheduleFloat: public LgFrSortingScheduleFloat {
public:

  // Set Schedule Strategy
  virtual
     void
        attributeScheduleStrategy(
           const LgFrDemandScheduleStrategy & );

  // Get Schedule Strategy
  // If this object ceases to exist or changes its attributeScheduleStrategy, 
  // then the returned pointer will be dangling 
  virtual
     const LgFrDemandScheduleStrategy *
        attributeScheduleStrategy()
           const;

  // Add pair of demand - TimeVecFloat's to the end of the collection
  virtual
  void
    insert (const LgFrItemTimeVecPairFloat pair);

  // Add demand (demand) and timeVecFloat(tvf) to the end of the collection
  virtual
  void
    insert (
       const LgFrDemand& demand,
       const LgFrTimeVecFloat tvf);

  // Add ItemTimeVecPairFloat pptvf  to the collection at position index.  
  // Everything afterwards is shifted to a higher index.
  virtual
  void
    insertAt (const size_t index,
	      const LgFrItemTimeVecPairFloat pitvf);

  // Add demand (demand) and timeVecFloatPair (tvf) to 
  // collection at position index.
  // Everything afterwards is shifted to a higher index.
  virtual
  void
    insertAt (
       const size_t index,
       const LgFrDemand& demand,
       const LgFrTimeVecFloat tvf);

  // Insert Demand if it isn't already in the schedule.  In any case,
  // associate timeVec with demand.
  virtual
    void
      insertKeyAndValue(
        const LgFrDemand& demand, 
          const LgFrTimeVecFloat& value);

  // Return demand at location i
  virtual
    LgFrDemand
      demand (const size_t i)
	const;

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

  // Return the LgFrItemTimeVecPairFloat whose item matches demand.
  // If there is no such LgFrItemTimeVecPairFloat, an assert error occurs.
  virtual
     const LgFrItemTimeVecPairFloat &
       operator[](
          const LgFrDemand& demand)
  const;

  // Since this derived class defines one overloading of operator[], it
  // hides all of the inherited overloadings of operator[], so here they are.
  virtual
     LgFrItemTimeVecPairFloat 
       operator[](
          const size_t& i )
             const;


  // Perform diadic schedule operation.
  // vectorOp is the operation applied to the timeVecFloat portions
  // of the same items in the lhs and rhs schedule.
  // If an item is not in the lhs or rhs schedule then a vector of
  // zero is used.
  //
  // For example:
  //
  //   LgFrSortingDemandScheduleFloat lhs, rhs;
  //   LgFrSortingDemandScheduleFloat
  //      diff( lhs.op(rhs,LgFrTimeVecFloat::subtract);
  //
  //   diff contains lhs - rhs
  //
  LgFrSortingDemandScheduleFloat
  op(
     LgFrTimeVecFloatOpFunction vectorOp,
     const LgFrSortingDemandScheduleFloat& rhs,
     float defaultValue = 0.0f )
  const;

  // Perform diadic schedule operation.
  //   schedule operation float
  // scalarOp is the operation applied to the timeVecFloat portions
  // of each item in the schedule.
  //
  // For example:
  //
  //   LgFrScheduleFloat lhs;
  //   LgFrScheduleFloat DoubleLhs( lhs.op(rhs,::operation*);
  //
  //   DoubleLhs contains lhs * 2
  //
  LgFrSortingDemandScheduleFloat
     op(
        LgFrTimeVecFloatRhsScalarOpFunction scalarOp,
        const float rhs )
     const;
  LgFrSortingDemandScheduleFloat
     op(
        const float lhs,
        LgFrTimeVecFloatLhsScalarOpFunction scalarOp )
     const;

  
   // Perform operation on column (timePeriod) of schedule.
   //
   // See LgFrSchedule::columnOp for further details.
   LgFrSortingDemandScheduleFloat
      columnOp(
         const size_t period,
         float (*scalarOperation) (const float, const float),
         const float rhs )
      const;
  

  // Assignment operator
  virtual
  LgFrSortingDemandScheduleFloat &
    operator=(
	      const LgFrSortingDemandScheduleFloat& rhs);

  // Copy constructor
  LgFrSortingDemandScheduleFloat (const LgFrSortingDemandScheduleFloat &);

  // Copy constructor with new Scenario
  LgFrSortingDemandScheduleFloat (const LgFrSortingDemandScheduleFloat &,
                                  LgFrScenario &);

  // Constructor for a set expected to have up to n demand 
  // (and default constructor)
  LgFrSortingDemandScheduleFloat (const size_t n = DEFAULT_VECTOR_CAPACITY);

  // Destructor
  ~LgFrSortingDemandScheduleFloat ();

  // test this class
  static void test();

  static void contextTest(LgFrScenario & testScenario);

protected:
	  
  // Return the LgFrItemTimeVecPairFloat whose item matches demand.
  // If there is no such LgFrItemTimeVecPairFloat, a new one is created
  // (using the default LgFrTimeVecFloat constructor) and inserted at
  // the end.
  virtual
     LgFrItemTimeVecPairFloat &
       operator[](
          const LgFrDemand& demand);

  virtual
     LgFrItemTimeVecPairFloat &
       operator[](
          const size_t& i );

private:
  // Since this derived class defines one overloading of operator[], it
  // hides all of the inherited overloadings of operator[]
  virtual
     LgFrItemTimeVecPairFloat &
       operator[](
          const LgFrItem& item);	// This should not be used by
  					// clients of this class
  virtual
     const LgFrItemTimeVecPairFloat &
       operator[](
          const LgFrItem& item) 	// This should not be used by
  const;                                // clients of this class
  
//  friend void LgFrDemandBacklogTimesGrossRevenuePairCompare::test();
};


// Addition Operator
// ( schedule + schedule )
LgFrSortingDemandScheduleFloat
   operator+ (
      const LgFrSortingDemandScheduleFloat& lhs, 
      const LgFrSortingDemandScheduleFloat& rhs);

// Subtraction Operator
// ( schedule - schedule )
LgFrSortingDemandScheduleFloat
   operator- (
      const LgFrSortingDemandScheduleFloat& lhs, 
      const LgFrSortingDemandScheduleFloat& rhs);

// Multiplication Operator
// ( schedule * float )
LgFrSortingDemandScheduleFloat
   operator* (
      const LgFrSortingDemandScheduleFloat& lhs,
      const float rhs);

// ( float * schedule )
LgFrSortingDemandScheduleFloat
   operator* (
      const float lhs,
      const LgFrSortingDemandScheduleFloat& rhs );

// Division Operator
// ( schedule / float )
LgFrSortingDemandScheduleFloat
   operator* (
      const LgFrSortingDemandScheduleFloat& lhs,
      const float rhs);

// ( float / schedule )
LgFrSortingDemandScheduleFloat
   operator* (
      const float lhs,
      const LgFrSortingDemandScheduleFloat& rhs );


// Addition Operator
// ( schedule + float )
LgFrSortingDemandScheduleFloat
   operator+ (
      const LgFrSortingDemandScheduleFloat& lhs,
      const float rhs);

// ( float + schedule )
LgFrSortingDemandScheduleFloat
   operator+ (
      const float lhs,
      const LgFrSortingDemandScheduleFloat& rhs );

// Subtraction Operator
// ( schedule - float )
LgFrSortingDemandScheduleFloat
   operator- (
      const LgFrSortingDemandScheduleFloat& lhs,
      const float rhs);

// ( float - schedule )
LgFrSortingDemandScheduleFloat
   operator- (
      const float lhs,
      const LgFrSortingDemandScheduleFloat& rhs );

#endif
