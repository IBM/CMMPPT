#ifndef LGFRWORKINGIMPLOSIONPROBLEM_H
#define LGFRWORKINGIMPLOSIONPROBLEM_H

// RW2STL -- inserted:
// End of RW2STL -- inserted includes.


#include <wit/src/wit.h>
#include <scenario/src/model.h>
#include <scenario/src/generVc.h>
#include <scenario/src/lastPrbS.h>
#include <scenario/src/part.h>
#include <scenario/src/demand.h>
#include <scenario/src/partSchd.h>
#include <scenario/src/demdSchd.h>

// Author: wally
// Represents the current WIT input data associated with a scenario as
// modified from its original state.
// 
// This object responsibilites are:
//   1 Accept part supply changes maintaining original supply and new
//     supply value.
//   2 Accept demand quantitiy changes maintaining original and new
//     demand quantity.
//   3 Accept changes to implosion method (opt or heur), and objective
//     function weights.
//   4 Return list of original and current values of all changed supply
//     and demand quantities.
//   5  Provide current values of:
//       1  Part supplies
//       2  Demand quantities
//       3  Implosion method
//       4  Objective function weights
// If the values requested have not been changed and are not stored in
// this object, then Last Problem Solved is used to obtain the value.

// Typedef used to refer to the optimization method.
typedef enum
  {
    LgFrHEURISTIC
#ifdef OPT_IMPLODE
    , LgFrOPTIMIZE
#endif
  }
  LgFrImplosionMethod;

// Typedef used to refer to the original values or the new ones
typedef enum
  { LgFrORIGINAL, LgFrLATEST  }
  LgFrWipSchedule;

class LgFrWorkingImplosionProblem : public LgFrSubscription
{

public:

  // Return true iff this instance is eligible for use (otherwise it should
  // only be assigned to.
  int
    isValid()
      const;

  ////////////////////////////////////////////////////////////////////
  //
  // A NOTE ON THE NAMES OF METHODS:
  // There are methods named changedParts and changedDemands.  They
  // should be called partsWithSupplyVolumeChanges and
  // demandsWithDemandVolumeChanges.  There are other methods (such
  // as changedPartsCycleTime) that should have their names changed too.
  //
  // A NOTE ON THE DIFFERENT KINDS OF QUERIES:
  //
  // Certain query methods (such as changedParts) only return vectors
  // of parts or demands.  Other queries (such as changedSupplyVolume)
  // return schedules of the differences (deltas) between original
  // schedules and current ones.  The names are too similar and should
  // be changed to have "delta" in them.
  //
  ////////////////////////////////////////////////////////////////////

  // Return a list of all parts whose supply volumes have been changed
  // since this problem was initialized
  LgFrPartVector
    changedParts()
      const;

  // Return a list of all parts whose cycle times have been changed 
  // since this problem was initialized
  LgFrPartVector
    changedPartsCycleTime()
      const;


  // Return a list of all demands that have had their demandVol changed
  // since this problem was initialized.
  LgFrDemandVector
    changedDemands()
      const;

  // Return a list of all demands that have had their priority changed
  // since this problem was initialized.
  LgFrDemandVector
    changedDemandsPriority()
      const;

  // Return a list of all demands that have had their obj1ShipReward changed
  // since this problem was initialized.
  LgFrDemandVector
    changedDemandsObj1ShipReward()
      const;

  // Return a list of all demands that have had their cumShipSoftLowerBound
  // changed since this problem was initialized.
  LgFrDemandVector
    changedDemandsCumShipSoftLowerBound()
      const;
  
  // Return a schedule containing the orignal or latest values of
  // supplyVolume for each part whose supply has changed.
  const LgFrSortingPartScheduleFloat&
      supplyVolume(const LgFrWipSchedule&)
      const;

  // Return a schedule of all changes made to part supplies
  LgFrSortingPartScheduleFloat
      changedSupplyVolume()
      const;

  // Return a schedule of all changes made to part supplies
  LgFrSortingPartScheduleFloat
      changedCycleTime()
      const;

  // Return a schedule of all changes made to demand volumes
  LgFrSortingDemandScheduleFloat
      changedDemandVolume()
      const;

  // Return a schedule of all changes made to demand priorities
  LgFrSortingDemandScheduleFloat
      changedPriority()
      const;

  // Return a schedule of all changes made to demand obj1ShipRewards
  LgFrSortingDemandScheduleFloat
      changedObj1ShipReward()
      const;

  // Return a schedule of all changes made to demand cumShipSoftLowerBound
  LgFrSortingDemandScheduleFloat
      changedCumShipSoftLowerBound()
      const;
  
  // Return a part's supply vector
  LgFrTimeVecFloat
    timeVec (const LgFrPart part)
      const;

  // Change a part's supply vector
  void
    timeVec (const LgFrPart part, 
	     const LgFrTimeVecFloat& timeVec);

  // Return a demand's demand quantity vector
  LgFrTimeVecFloat
    timeVec (const LgFrDemand demand)
      const;

  // Change a demand's demand quantity vector
  void
    timeVec (const LgFrDemand demand,
	     const LgFrTimeVecFloat& timeVec);

  // Return a demand's priority vector
  LgFrTimeVecFloat
    priority (const LgFrDemand demand)
      const;

  // Change a demand's priority vector
  void
    priority (const LgFrDemand demand,
	     const LgFrTimeVecFloat& timeVec);

  // Return a part's cycleTime vector
  LgFrTimeVecFloat
    cycleTime (const LgFrPart part)
      const;

  // Change a part's cycleTime vector
  void
    cycleTime (const LgFrPart part,
	       const LgFrTimeVecFloat& timeVec);

  // Return a demand's obj1ShipReward vector
  LgFrTimeVecFloat
    obj1ShipReward (const LgFrDemand demand)
      const;

  // Change a demand's obj1ShipReward vector
  void
    obj1ShipReward (const LgFrDemand demand,
             const LgFrTimeVecFloat& timeVec);

  // Return a demand's cumShipSoftLowerBound vector
  LgFrTimeVecFloat
    cumShipSoftLowerBound (const LgFrDemand demand)
      const;

  // Change a demand's cumShipSoftLowerBound vector
  void
    cumShipSoftLowerBound (const LgFrDemand demand,
             const LgFrTimeVecFloat& timeVec);
  

  // Set Implode Method
  void
     implosionMethod( 
        const LgFrImplosionMethod meth );

  // Get Implode Method
  LgFrImplosionMethod
     implosionMethod( )
        const;

  // Set Cost of Capital
  void
     capitalCost(
        const float capCost );

  // Get Cost of Capital
  float
     capitalCost()
        const;

  // Set heuristic equitable allocation
  void
     equitability(
        const int newValue );

  // Get heuristic equitable allocation
  int
     equitability()
        const;

  // Set heuristic build ahead or not
  void
     buildAhead(
        const bool flag );

  // Get heuristic build ahead or not
  bool
     buildAhead()
        const;

  // Set heuristic compute critical list or not
  void
     computeCriticalList(
        const bool flag );

  // Get heuristic compute critical list or not
  bool
     computeCriticalList()
        const;

  // Set objective function weight on bounds
  void
     weightBounds(
        const float newValue );

  // Get objective function weight on bounds
  float
     weightBounds()
        const;


  // transaction methods
  void beginSeriesOfChanges();
  void endSeriesOfChanges();


   //-------------------------------------------------------------------------
   //
   // Subscribe methods
   //
   //-------------------------------------------------------------------------

   // Inform object m when the implosion solution or MRP solution
   // has changed.  The object added must have a member function
   // localUpdate( LgFrModel * p, void * d ).  
   // When the working implosion problem
   // is changed then m.updateFrom( this, LgFrUserInputModification() ) is invoked.
   //
   // See LgFrModel and LgFrModelClient.
   void 
      addDependent( 
         LgFrModelClient * m);

   // No longer inform object m when working implosion problem has changed.
   void 
      removeDependent(
         LgFrModelClient * m );

   // Not used since wip does not subscribe to any other objects.
   void
      localUpdate(
         LgFrModel *,
               void * );


        
  // Constructor for a given environment.
  LgFrWorkingImplosionProblem(
			      LgFrLastProblemSolved& myLps,
			      LgFrSetOfParts& mySoParts,
			      LgFrSetOfDemands& mySoDemands);

  // Another "copy constructor".  This is the one to use when you are
  // making a deep copy of a scenario or doing something similar
  LgFrWorkingImplosionProblem(
			      const LgFrWorkingImplosionProblem & source,
			      LgFrLastProblemSolved& myLps,
			      const LgFrSetOfParts& mySoParts,
			      const LgFrSetOfDemands& mySoDemands);

  // This method copies the data that is contained in the source to *this
  // without changing the soPartsPtr_, soDemandsPtr_, lpsPtr_ of *this.
  virtual
    void
    copyInPlace (const LgFrWorkingImplosionProblem& source);
  
  // Destructor
  virtual
    ~LgFrWorkingImplosionProblem();
  
  // Default Constructor
  // The result of this is an instance for which isValid() is false.
  LgFrWorkingImplosionProblem();

  // Self-tester
  static
    void
      test();

  // Expects the data in aWip to come from
  // "/u/implode/wit/data/standard/obj1/wit.data"
  static
  void
  contextTest
    (LgFrWorkingImplosionProblem & aWip,
     LgFrSetOfParts & sop,		// from same scenario as aWip
     LgFrSetOfDemands & sod);		// from same scenario as aWip

private:
  // Assignment operator.
  virtual
    LgFrWorkingImplosionProblem&
      operator=(
		const LgFrWorkingImplosionProblem& rhs);
  
  // Copy constructor.
  LgFrWorkingImplosionProblem(
			      const LgFrWorkingImplosionProblem & source);
  
  const LgFrSetOfParts* soPartsPtr_;
  const LgFrSetOfDemands* soDemandsPtr_;
  LgFrLastProblemSolved* lpsPtr_;

  // For parts whose supply quantities have changed, *(partSchedules_[0])
  // has the original parts and timevec and *(partSchedules_[1]) has the
  // latest values
  LgFrSortingPartScheduleFloat partSchedules_[2];
  // For demands whose supply quantities have changed, *(demandSchedules_[0])
  // has the original demands and timevec and *(demandSchedules_[1]) has the
  // latest values
  LgFrSortingDemandScheduleFloat demandSchedules_[2];
  // For demands whose priorities have changed, *(prioritySchedules_[0])
  // has the original demands and timevec and *(prioritySchedules_[1]) has the
  // latest values
  LgFrSortingDemandScheduleFloat prioritySchedules_[2];

  // For parts whose cycle times have changed, *(cycleTimeSchedules_[0])
  // has the original parts and timevec and *(cycleTimeSchedules_[1]) has the
  // latest values
  LgFrSortingPartScheduleFloat cycleTimeSchedules_[2];

  // For demands whose obj1ShipReward have changed, 
  // *(obj1ShipRewardSchedules_[0]) has the original demands and timevec and 
  // *(obj1ShipRewardSchedules_[1]) has the latest values.
  LgFrSortingDemandScheduleFloat obj1ShipRewardSchedules_[2];

  // For demands whose cumShipSoftLowerBound have changed, 
  // *(cumShipSoftLowerBound_[0]) has the original demands and timevec and 
  // *(cumShipSoftLowerBound_[1]) has the latest values.
  LgFrSortingDemandScheduleFloat cumShipSoftLowerBoundSchedules_[2];
  
  // Current implosion method
  LgFrIntVector implosionMethod_;

  // Default implosion method
  LgFrImplosionMethod defaultImplosionMethod_;

  // Used to index in following vectors of length 3
  enum {isChanged, orig, latest};

  // Store WIT Global parameters.
  // Indices:
  //   isChanged - parameter has been changed.
  //   orig      - original parameter value.
  //   latest    - the current value.
  LgFrIntVector       equitability_;
  LgFrBoolVector buildAhead_;
  LgFrBoolVector computeCriticalList_;
  LgFrFloatVector     weightBounds_;

  // flag to minimize update of subscribers to wit.  if user
  //  are going to change the state of WIP using many calls
  //  we can defer the call of changed() until the transaction
  //  if finished 
  int inTransaction_;

  // private methods

  // Set WIT Float attribute
  void
     setWitFloat(
        const float newValue,
        LgFrFloatVector & statusVec,
        float (LgFrLastProblemSolved::*lpsGetFunc) () const );

  // Get WIT Float attribute
  float
     getWitFloat(
        const LgFrFloatVector statusVec,
        float (LgFrLastProblemSolved::*lpsGetFunc) () const)
           const;

  // Set WIT boolean attribute
  void
     setWitbool(
        const bool newValue,
        LgFrBoolVector & statusVec,
        bool (LgFrLastProblemSolved::*lpsGetFunc) () const );

  // Get WIT boolean attribute
  bool
     getWitbool(
        const LgFrBoolVector statusVec,
        bool (LgFrLastProblemSolved::*lpsGetFunc) () const)
           const;

  // Set WIT int attribute
  void
     setWitInt(
        const int newValue,
        LgFrIntVector & statusVec,
        int (LgFrLastProblemSolved::*lpsGetFunc) () const );

  // Get WIT int attribute
  int
     getWitInt(
        const LgFrIntVector statusVec,
        int (LgFrLastProblemSolved::*lpsGetFunc) () const)
           const;

};

#endif
