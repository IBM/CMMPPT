#ifndef SCENARIOFORATP_H
#define SCENARIOFORATP_H
   

#include <scenario/src/dfa.h>
#include <scenario/src/scenario.h>
#include <scenario/src/stateATP.h>

// NOTE : States for State machine are now in statesATP.h !


// forward declarations
class LgFrInitialProblemAndParameters;
class LgFrSupplyScheduleStrategyForATP;
class LgFrDemandVolumeScheduleStrategyForATP;
class LgFrShipmentScheduleStrategyForATP;    
class LgFrBacklogScheduleStrategyForATP;    
class LgFrScheduleFactoryForATP;


class LgFrScenarioForATP : public LgFrScenario
{

  // friends (need access to scenario-specific set of Parts)
  friend class LgFrInitialProblemForATP;
  friend class LgFrSupplyScheduleStrategyForATP;
  friend class LgFrDemandVolumeScheduleStrategyForATP;
  friend class LgFrShipmentScheduleStrategyForATP;    
  friend class LgFrBacklogScheduleStrategyForATP;    
  friend class LgFrScheduleFactoryForATP;
  friend class LgFrFocussedShortageScheduleScheduleStrategyForATP;

  // friends (need access to non-const set of Parts for simpler GUI
  // implementation)
  friend LgFrSetOfParts& guiNewOpFunctionSetOfParts (int);

  friend class LgFrScenarioForATPSmartPointerToConst;
  friend class LgFrScenarioForATPSmartPointer;


  public:

#if 0
  // Temporary for testing atpTest
  LgFrLastProblemSolved &
    lps()
  {
     return LgFrScenario::lastProblemSolved();
  }
#endif

  virtual
     isAReturnType
        isA()
           const;
  
  // Create an implosion solution, replacing the old one if it exists.
  // This does an implosion for whatever state the state machine is in.
  // It does not change the state machine's state.
  void
  newImplosionSolution ();
  
  // Return a const setOfDemands which belongs to this scenario
  virtual
     const
        LgFrSetOfDemands&
           constSetOfDemands(int k = LGFR_DEFAULT_SET)
		const;

  // Return a const setOfParts which belongs to this scenario
  virtual
     const 
        LgFrSetOfParts&
           constSetOfParts(int k = LGFR_DEFAULT_SET)
		const;

  // Add a new demand to LPS and to the appropriate setOfDemands instances.
  // The demand should already be in the universal set of demands (otherwise
  // it was created incorrectly.)
  // The second parameter tells whether this is a committed, forecast, or new-op
  // demand.
  virtual
  void
  newDemand (const LgFrDemand, int demandSetNumber = LGFR_NEWOPPOR_DEMANDS);

  // Add a new NewOpportunity demand into the system.  Default parameters
  // (ie, demandVol, priority, grossRevenue are assigned.  Returns true
  // if operation is successful, FALSE ow.  Reasons for FALSE can be: part
  // is not in WIT or demand already exists.
  virtual
     RWBoolean
        newNewOppDemand (const RWCString & demandName,
                         const LgFrPart & part);
  
#if READY
  // Change a demands type by changing the  appropriate setOfDemands instances.
  // The 2nd & 3rd parameter tells whether this is a committed, forecast, or new-op
  // demand.
  virtual
  void
  changeDemandType (const LgFrDemand,
                    int oldDemandSetNumber = LGFR_NEWOPPOR_DEMANDS,
                    int newDemandSetNumber = LGFR_COMMITTED_DEMANDS
                    )
       
  // Remove a demand from LPS and the appropriate setOfDemands instances.
  // The second parameter tells whether this is a committed, forecast, or new-op
  // demand.
  virtual
  void
  deleteDemand (const LgFrDemand, int demandSetNumber = newOp);
#endif
  
  // return current state of the state machine
  inline int currentState() const { return theDfa_->GetState();}
  // Inputs the next token into Dfa, advance state emitting actions if needed
  inline void inputToken (Token& token) { theDfa_->InputToken(token); }
  // if your token is not in a struct use:
  inline void inputToken (int tokenType, void* tokenContents = NULL)
	{  theDfa_->InputToken(tokenType, tokenContents); }

  int solutionIsUpToDate(int state = LGFR_ATP_STATE_COMMITTED_IMPLODED) const;
  int solutionIsUpToDateCI() const;
  int solutionIsUpToDateNOI() const;
  int solutionIsUpToDateATPI() const;
  void setSolutionUpToDateCI();
  void setSolutionUpToDateNOI();
  void setSolutionUpToDateATPI();
  void clearSolutionUpToDateCI();
  void clearSolutionUpToDateNOI();
  void clearSolutionUpToDateATPI();

  // Action functions for dfa Transition Table entries
  void noAccumFunc();
  void noEmitFunc();
  void accumNoAction();
  void emitNoAction();
   
  void emitGoToCI();
  void emitGoFromCItoNOI();
  void emitGoFromCItoFSS();
  void emitGoFromFSStoCI();
  void emitGoFromFSStoCIandClearSolutionUpToDateCI();
  void emitGoFromFSStoNOI();
  void emitGoFromNOItoATPI();
  void emitGoFromCItoATPI();
  void emitGoFromATPItoNOI();

  // force state change
  void changeState(int);
  
  // Constructor 
  LgFrScenarioForATP ( const LgFrDataInterfaceAbstract&,
		        const LgFrCalendar&);


  // Destructor
  ~LgFrScenarioForATP();



  // Default Constructor
  LgFrScenarioForATP();

  // Self-test
  static void test();

protected:
  // Copy constructor
  LgFrScenarioForATP(const LgFrScenarioForATP& source);


  // Return a non-const setOfParts which belongs to this scenario
  virtual 
     LgFrSetOfParts&
        setOfParts(int k = LGFR_DEFAULT_SET);

  // Return a non-const setOfDemands which belongs to this scenario
  virtual
     LgFrSetOfDemands&
        setOfDemands(int k= LGFR_DEFAULT_SET);

  virtual
  LgFrSortingPartScheduleFloat* masterPartVolumes();

  virtual
  const
  LgFrSortingPartScheduleFloat* masterPartVolumes() const;

  virtual
  LgFrSortingDemandScheduleFloat* masterDemandVolumes();

  virtual
  const
  LgFrSortingDemandScheduleFloat* masterDemandVolumes() const;

      
private:
  // Assignment operator.
  LgFrScenarioForATP&
    operator=(const LgFrScenarioForATP& rhs);

  // Set the WIP so that all of the demands related to state s have the order
  // volumes and all of the other demands have all-zero volumes.
  void
  setWipDemandsForState (int s);

  // Set the WIP so that it is identical to s.  In
  // other words, if a part is not in s, give it an all-zero
  // timevec, otherwise give it the timevec in s, and do this
  // for all the parts in the scenario
  void
  setWipPartsFromSchedule (const LgFrSortingPartScheduleFloat & s );

  // Set the demandVols in WIT with a sortingDemandScheduleFloat.
  // NOTE: The new demands are ubiquitous!  Any demand in oldVol and not 
  // in newVol becomes 0.
  void
     overlayDemand( const LgFrSortingDemandScheduleFloat & newVol,
                    const LgFrSortingDemandScheduleFloat & oldVol);

  // Set the supplyVols in WIT with a sortingPartScheduleFloat.
  // NOTE: The new supplys are ubiquitous!  Any supply in oldVol and not 
  // in newVol becomes 0.
  void
     overlaySupply( const LgFrSortingPartScheduleFloat & newVol,
                    const LgFrSortingPartScheduleFloat & oldVol);

  // reset focus horizon to the back ordered demands for the last period
  void setFSSInputToDefault();

  // polulate the Dfa transition table for ATP
  void populateDfaTransTableForATP(DfaTransPtr atpTable, int tableSize);

  // set supply volumes used for the next implode step to the excess
  //  volumes at the current state
  void setSupplyVolumesToExcessVolumes();

  // update FSS but don't return it
  void getLatestFSS();

  // get the state machine (used by
  //   LgFrFocussedShortageScheduleScheduleStrategyForATP)
  const LgFrDfa* dfa() const;

  // private variables
#ifdef DEFAULT_SOP_DIFFERS_FROM_UNIVERSAL_SOP
  LgFrSetOfPartsWithMovableParts defaultSOP_;       // Set of default Parts
#endif  
  LgFrSetOfDemands defaultSOD_;     // Set of default Demands

  LgFrSetOfPartsWithMovableParts capacitySOP_;        // Set of capacity parts
  LgFrSetOfPartsWithMovableParts materialSOP_;        // Set of material parts
                                                      // This includes both
                                                      // raw and product parts.

  LgFrSetOfDemands committedSOD_;     // Set of committed demands
  LgFrSetOfDemands newOpportunitySOD_;// Set of new opportunity demands
  LgFrSetOfDemands firmForecastSOD_;  // Set of firm forecast demands
#ifdef USING_RISK_FORECAST
  LgFrSetOfDemands riskForecastSOD_;  // Set of risk forecast demands
#endif

  
  // Supply volumes that are input to committed order implosion.  These are the
  // part supplies that are available for all of the different kinds of demands.
  LgFrSortingPartScheduleFloat * masterPartVolumesPtr_;  
                               
  // Demand volumes for all of the demands
  LgFrSortingDemandScheduleFloat * masterDemandVolumesPtr_;


  // keep a copy of the excess supply schedule from the last COMMITTED_IMPLODED
  // run
  LgFrSortingPartScheduleFloat * committedImplodedExcessSupplySchedulePtr_;
       
  // whether solution is up to date for the state.
  // will change to have three solutionUpToDate_ flags
  int committedSolutionUpToDate_;
  int newOppSolutionUpToDate_;
  int atpSolutionUpToDate_;

  // save pointer to transTable for destructor
  DfaTransPtr atpTransTable_;

  // State machine
  LgFrDfa *theDfa_;

  // ID tag for casting smart pointers
  int id_;
};

#endif
