#ifndef LGFR_SORTING_PART_SCHEDULE_H
#define LGFR_SORTING_PART_SCHEDULE_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.
#define LGFR_SORTING_PART_SCHEDULE_H


#include <scenario/src/sortSchd.h>
//#include <scenario/src/schdSnPC.h>

// Forward reference
class LgFrPartScheduleStrategy;

class LgFrSortingPartScheduleFloat: public LgFrSortingScheduleFloat {
public:

  // Set Schedule Strategy
  virtual
     void
        attributeScheduleStrategy(
           const LgFrPartScheduleStrategy & );

  // Get Schedule Strategy
  // If this object ceases to exist or changes its attributeScheduleStrategy, 
  // then the returned pointer will be dangling 
  virtual
     const LgFrPartScheduleStrategy *
        attributeScheduleStrategy()
           const;

  // Add  part - TimeVecFloat - pair to the end of the collection
  virtual
  void
    insert (const LgFrItemTimeVecPairFloat pair);

  // Add ItemTimeVecPairFloat pptvf  to the collection at position index.  
  // Everything afterwards is shifted to a higher index.
  virtual
  void
    insertAt (const size_t index,
	      const LgFrItemTimeVecPairFloat pitvf);

  // Insert Part if it isn't already in the schedule.  In any case,
  // associate timeVec with part.
  virtual
    void
      insertKeyAndValue(
         const LgFrPart& part, 
         const LgFrTimeVecFloat& value);

  // Return part at location i
  virtual
    LgFrPart
      part (const size_t i)
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


  // Return the LgFrItemTimeVecPairFloat whose item matches part.
  // If there is no such LgFrItemTimeVecPairFloat, an assert error occurs.
  virtual
     const LgFrItemTimeVecPairFloat &
       operator[](
          const LgFrPart& part)
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
  //   LgFrSortingPartScheduleFloat lhs, rhs;
  //   LgFrSortingPartScheduleFloat 
  //      diff( lhs.op(rhs,LgFrTimeVecFloat::subtract);
  //
  //   diff contains lhs - rhs 
  //
  LgFrSortingPartScheduleFloat
  op(
     LgFrTimeVecFloatOpFunction vectorOp,
     const LgFrSortingPartScheduleFloat& rhs,
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
  LgFrSortingPartScheduleFloat
     op(
        LgFrTimeVecFloatRhsScalarOpFunction scalarOp,
        const float rhs)
     const;
  LgFrSortingPartScheduleFloat
     op(
        const float lhs,
        LgFrTimeVecFloatLhsScalarOpFunction scalarOp )
     const;
  
   // Perform operation on column (timePeriod) of schedule.
   //
   // See LgFrSchedule::columnOp for further details.
   LgFrSortingPartScheduleFloat
      columnOp(
         const size_t period,
         float (*scalarOperation) (const float, const float),
         const float rhs )
      const;

  // Assignment operator
  virtual
  LgFrSortingPartScheduleFloat &
    operator=(
	      const LgFrSortingPartScheduleFloat& rhs);

  // Copy constructor
  LgFrSortingPartScheduleFloat (const LgFrSortingPartScheduleFloat &);

  // Copy constructor with a new Scenario
  LgFrSortingPartScheduleFloat (const LgFrSortingPartScheduleFloat &,
                                LgFrScenario &);

  // Constructor for a set expected to have up to n parts 
  // (and default constructor)
  LgFrSortingPartScheduleFloat (const size_t n = 0);

  // Destructor
  ~LgFrSortingPartScheduleFloat ();

  // test this class
  static void test();

  // test this class
  // Expects the data in testScenario to come from
  // "/u/implode/wit/data/examples/diner12/wit.data"
  static
  void
  contextTest
    (
     LgFrScenario			& testScenario,
     LgFrWorkingImplosionProblem	& wip,		// testScenario's
     LgFrSetOfParts			& sop);		// testScenario's
  
protected:

  // Return the LgFrItemTimeVecPairFloat whose item matches part.
  // If there is no such LgFrItemTimeVecPairFloat, a new one is created
  // (using the default LgFrTimeVecFloat constructor) and inserted at
  // the end.
  virtual
     LgFrItemTimeVecPairFloat &
       operator[](
          const LgFrPart& part);

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
          const LgFrItem& item)
  const;                                // This should not be used by
 
                                         // clients of this class
//  friend void LgFrScheduleSensitivePairCompareFloat::test();
};

// Addition Operator
// ( schedule + schedule )
LgFrSortingPartScheduleFloat
   operator+ (
      const LgFrSortingPartScheduleFloat& lhs, 
      const LgFrSortingPartScheduleFloat& rhs);

// Subtraction Operator
// ( schedule - schedule )
LgFrSortingPartScheduleFloat
   operator- (
      const LgFrSortingPartScheduleFloat& lhs, 
      const LgFrSortingPartScheduleFloat& rhs);

// Multiplication Operator
// ( schedule * float )
LgFrSortingPartScheduleFloat
   operator* (
      const LgFrSortingPartScheduleFloat& lhs, 
      const float rhs);

// ( float * schedule )
LgFrSortingPartScheduleFloat
   operator* (
      const float lhs,
      const LgFrSortingPartScheduleFloat& rhs );

// Division Operator
// ( schedule / float )
LgFrSortingPartScheduleFloat
   operator* (
      const LgFrSortingPartScheduleFloat& lhs,
      const float rhs);

// ( float / schedule )
LgFrSortingPartScheduleFloat
   operator* (
      const float lhs,
      const LgFrSortingPartScheduleFloat& rhs );


// Addition Operator
// ( schedule + float )
LgFrSortingPartScheduleFloat
   operator+ (
      const LgFrSortingPartScheduleFloat& lhs, 
      const float rhs);

// ( float + schedule )
LgFrSortingPartScheduleFloat
   operator+ (
      const float lhs,
      const LgFrSortingPartScheduleFloat& rhs );

// Subtraction Operator
// ( schedule - float )
LgFrSortingPartScheduleFloat
   operator- (
      const LgFrSortingPartScheduleFloat& lhs,
      const float rhs);

// ( float - schedule )
LgFrSortingPartScheduleFloat
   operator- (
      const float lhs,
      const LgFrSortingPartScheduleFloat& rhs );


#endif
