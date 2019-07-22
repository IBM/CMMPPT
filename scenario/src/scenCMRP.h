#ifndef SCENARIOFORCMRP_H
#define SCENARIOFORCMRP_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/scenario.h>

// Set codes.  Not using enums because subclasses can have more sets
// The following were set by the base class
// #define LGFR_DEFAULT_SET       0
// #define LGFR_UNIVERSAL_SET     1
#define    LGFR_PROCUREABLE_SET   2
#define    LGFR_PRODUCEABLE_SET   3
#define    LGFR_CONSTRAINED_SET   4
#define    LGFR_BUDGET_SET        5
#define    LGFR_RAW_SET           6
#define    LGFR_MRP_PLANNED_SET   7
#ifdef LGFR_CAPACITY_SET
#undef LGFR_CAPACITY_SET
#endif
#define    LGFR_CAPACITY_SET      8

#define    LGFR_COMMITTED_DEMAND_SET 2


// forward declarations
class LgFrInitialProblemAndParameters;


class LgFrScenarioForCMRP : public LgFrScenario
{

  // friends (need access to scenario-specific set of Parts)
friend class LgFrInitialProblemForCMRP;
friend class LgFrInitialProblemForCMRPwithPrioritiesFromDif;
// friend class LgFrScheduleFactoryForCMRP;

  friend class LgFrScenarioForCMRPSmartPointerToConst;
  friend class LgFrScenarioForCMRPSmartPointer;

  public:

  virtual int GetScenarioType() const { return __LGFRSCENARIOFORCMRP; }

  // Create an implosion solution, replacing the old one if it exists
  virtual
    void
      newImplosionSolution ();

  // Create an explosion solution, replacing the old one if it exists
  virtual
    void
      newExplosionSolution ();

  // Return a const setOfParts which belongs to this scenario
  virtual
     const 
        LgFrSetOfParts&
           constSetOfParts(int k = LGFR_DEFAULT_SET)
              const;

#if 1
  // Temporary for testing cmrpTest
  LgFrLastProblemSolved &
    lps()
  {
     return LgFrScenario::lastProblemSolved();
  }
#endif

  // Return a const setOfDemands which belongs to this scenario
  virtual
     const
        LgFrSetOfDemands&
           constSetOfDemands(int k = LGFR_DEFAULT_SET)
              const;

  // Return the set of demands that contain at least one time-period
  // that has a priority numerically less than or equal to all of the other
  // priorities
    virtual
    LgFrSetOfDemands
    mostUrgentDemands(const LgFrSortingDemandScheduleFloat&
	                       prioritySchedule)
        const;

  // Make WIT use or not use focus horizons.
    virtual
      void
	useFocusHorizons (bool);

  // Constructor
  LgFrScenarioForCMRP ( const LgFrDataInterfaceAbstract&,
		        const LgFrCalendar&);


  // Destructor
  ~LgFrScenarioForCMRP();



  // Default Constructor
  LgFrScenarioForCMRP();

  // Self-test
  static void test();

 protected:
  // Copy constructor
  LgFrScenarioForCMRP(
               const LgFrScenarioForCMRP& source);

  // Return a non-const setOfParts which belongs to this scenario
  virtual 
     LgFrSetOfParts&
        setOfParts(int k = LGFR_DEFAULT_SET);

  // Return a non-const setOfDemands which belongs to this scenario
  virtual
     LgFrSetOfDemands&
        setOfDemands(int k= LGFR_DEFAULT_SET);
  
 private:
  // Assignment operator.
  LgFrScenarioForCMRP&
    operator=(
              const LgFrScenarioForCMRP& rhs);

   LgFrSetOfPartsWithMovableParts defaultSOP_;       // Set of default Parts
   LgFrSetOfDemands               defaultSOD_;     // Set of default Demands

   LgFrSetOfDemands committedSOD_;   // Set of committed Demands   

   LgFrSetOfPartsWithMovableParts procureableSOP_;   // Set of procureable Parts
   LgFrSetOfPartsWithMovableParts constrainedSOP_;   // Set of constrained Parts
   LgFrSetOfPartsWithMovableParts produceableSOP_;   // Set of produceable Parts
   LgFrSetOfPartsWithMovableParts rawSOP_;           // Set of raw Parts (can't
                                                     // distinguish these from 
                                                     // wit)
   LgFrSetOfPartsWithMovableParts mrpPlannedSOP_;    // Set of material 
                                                     // (non-capacity) parts
   LgFrSetOfPartsWithMovableParts capacitySOP_;      // Set of capacity parts
   LgFrSetOfPartsWithMovableParts budgetSOP_;        // Set of Parts for 
                                                     // (dummy) budget part(s)

   // ID tag for casting smart pointers
   int id_;
};

#endif
