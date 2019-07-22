#ifndef LGFR_MULTIPARTSCH_H
#define LGFR_MULTIPARTSCH_H

// This is a  class for representing multiple schedules of parts.

#include <scenario/src/multiSch.h>

class LgFrMultiPartScheduleFloat: public LgFrMultiScheduleFloat {

public:

  // Return the schedule type: has to be PART
  virtual
    LgFrScheduleType
      schedType() const;

  // Return a string suitable for use as the description of the items
  // in the schedule
  virtual
  RWCString
  itemHeading ()
       const;
  
  // Add a schedule if it is not already in the multi-schedule, otherwise
  // just return.
  virtual
    RWBoolean
      newSchedule(const LgFrScheduleDescriptor&);

  // Set a Selection schedule using a FilterDescriptor
  // This method removes the filter from the current selection schedule.
  // Implementation note: this call can be used to change the filter if sd is
  // the descriptor of the old selection-schedule
  // Return TRUE  :if sd is found and changes are made.
  //        FALSE :if sd is not found.
  virtual
    RWBoolean
      filterItemSelectionSchedule(
         const LgFrScheduleDescriptor& sd,
         const LgFrFilterDescriptor &,
         const RWCString & = LgFrFilterNothing,
         const RWCString & = LgFrFilterNothing );


  // Create a copy of the *this on the heap for a new and different scneario
  // and return a pointer to it. (deep copy)
  virtual
    LgFrMultiScheduleFloat*
      clone (LgFrScenarioSmartPointer)
        const;

  // Constructor
  LgFrMultiPartScheduleFloat (LgFrScenarioSmartPointer);
  
  // Default constructor
  LgFrMultiPartScheduleFloat ();

  // Destructor
  virtual
    ~LgFrMultiPartScheduleFloat ();

  // Print this class
  virtual void print() const;
    
  // test this class
  static void contextTest(LgFrScenarioSmartPointer scenarioPtr,
                          LgFrScenarioSmartPointer newScenarioPtr);

// ---------------------------------------------------------------------
//  PROTECTED MEMBERS
// ---------------------------------------------------------------------
protected:

  // Return a (part related) pair compair strategy that is corresponding 
  // to the sorting descriptor and optional second parameter period.
  virtual
    LgFrPairCompareFloat*
      makePairCompareStrategy(LgFrSortDescriptor sortDesc,
                              int period = -1)
        const;

  // Copy constructor (deep copy to a new and different scenario)
  // Used by clone method only so that it can check if the new scenario
  // is different from the current one.
  LgFrMultiPartScheduleFloat (const LgFrMultiPartScheduleFloat&,
                              LgFrScenarioSmartPointer);

  // Given the guiName of a part, return a pointer to it or a null
  // pointer.  The pointer is valid until a parts or demands
  // is added to or taken from the universal set of parts or demands.
  virtual
  const LgFrItem *
  findItem (const RWCString & guiName)
       const;

// ---------------------------------------------------------------------
//  PRIVATE MEMBERS
// ---------------------------------------------------------------------
private:
  
  // Assignment operator (not allowed, use clone(LgFrScenarioSmartPointer) method)
  LgFrMultiPartScheduleFloat&
    operator=(const LgFrMultiPartScheduleFloat&);

  // Copy constructor (not allowed, use clone(LgFrScenarioSmartPointer) method)
  LgFrMultiPartScheduleFloat (const LgFrMultiPartScheduleFloat&);
  
};

#endif
