#include <stdlib.h>

#include <scenario/src/calendar.h>
#include <scenario/src/dif.h>
#include <scenario/src/difImpl.h>
#include <scenario/src/difTest.h>
#include <scenario/src/difPWatp.h>
#include <scenario/src/scBdATP.h>
#include <scenario/src/scenATP.h>
#include <scenario/src/scenDrct.h>
#include <scenario/src/ipATP.h>
#include <scenario/src/schFcATP.h>
#include <scenario/src/schdDeSt.h>
#include <scenario/src/demdSchP.h>
#include <scenario/src/partSchP.h>

// token definitions
Token tokenGoFromCItoNOI   = {LGFR_ATP_TOKEN_GO_FROM_CI_TO_NOI, NULL};
Token tokenGoFromNOItoATPI = {LGFR_ATP_TOKEN_GO_FROM_NOI_TO_ATPI, NULL};
Token tokenGoFromCItoATPI  = {LGFR_ATP_TOKEN_GO_FROM_CI_TO_ATPI, NULL};
Token tokenGoFromCItoFSS   = {LGFR_ATP_TOKEN_GO_FROM_CI_TO_FSS, NULL};
#ifdef ASKING_FOR_TROUBLE
Token tokenGoFromFSStoNOI  = {LGFR_ATP_TOKEN_GO_FROM_FSS_TO_NOI, NULL};
#endif
Token tokenGoFromFSStoCI   = {LGFR_ATP_TOKEN_GO_FROM_FSS_TO_CI, NULL};
Token tokenGoFromNOItoCI   = {LGFR_ATP_TOKEN_GO_FROM_NOI_TO_CI ,NULL};
Token tokenGoFromATPItoCI  = {LGFR_ATP_TOKEN_GO_FROM_ATPI_TO_CI, NULL};
Token tokenGoFromATPItoNOI = {LGFR_ATP_TOKEN_GO_FROM_ATPI_TO_NOI, NULL};
Token tokenSupplyChanged   = {LGFR_ATP_TOKEN_SUPPLY_CHANGED, NULL};
Token tokenCommittedDemandsChanged = {LGFR_ATP_TOKEN_COMMITTED_DEMANDS_CHANGED, NULL};
Token tokenNewOppDemandsChanged = {LGFR_ATP_TOKEN_NEWOPP_DEMANDS_CHANGED, NULL};
Token tokenATPDemandsChanged  = {LGFR_ATP_TOKEN_ATP_DEMANDS_CHANGED, NULL};
Token tokenReimplode       = {LGFR_ATP_TOKEN_REIMPLODE, NULL};
// Token tokenRecalculateFSS  = {LGFR_ATP_TOKEN_RECALCULATE_FSS, NULL};

// forward declarations
class LgFrSetOfParts;
class LgFrSetOfDemands;

void noAccumFunc(void *);
void noEmitFunc(void *);
void accumNoAction(void *);
void emitNoAction(void *);

void clearSolutionUpToDateCI(void *);
void clearSolutionUpToDateNOI(void *);
void clearSolutionUpToDateATPI(void *);

// void emitReimplodeCI(void *);
// void emitReimplodeNOI(void *);
// void emitReimplodeATPI(void *);
void newImplosionSolution(void *);

// void emitRecalculateFSS(void *);
void emitGoToCI(void *);
void emitGoFromCItoNOI(void *);
void emitGoFromCItoFSS(void *);
void emitGoFromFSStoCI(void *);
void emitGoFromFSStoCIandClearSolutionUpToDateCI(void *);
#ifdef ASKING_FOR_TROUBLE
void emitGoFromFSStoNOI(void *);
#endif
void emitGoFromNOItoATPI(void *);
void emitGoFromCItoATPI(void *);
void emitGoFromATPItoNOI(void *);

/*
States:

LGFR_ATP_STATE_COMMITTED_IMPLODED		
LGFR_ATP_STATE_NEWOPP_IMPLODED			
LGFR_ATP_STATE_ATP_IMPLODED			
LGFR_ATP_STATE_COMMITTED_IMPLODED_RECOVERY	

Tokens:
	1 = LGFR_GO_FROM_CI_TO_NOI
	2 = LGFR_GO_FROM_NOI_TO_ATPI
	3 = LGFR_GO_FROM_CI_TO_FSS
#ifdef ASKING_FOR_TROUBLE
        4 = LGFR_GO_FROM_FSS_TO_NOI
#endif
	5 = LGFR_GO_FROM_FSS_TO_CI
	6 = LGFR_GO_FROM_NOI_TO_CI
	7 = LGFR_GO_FROM_ATPI_TO_CI
	8 = LGFR_SUPPLY_CHANGED
	9 = LGFR_COMMITTED_DEMANDS_CHANGED
	10 = LGFR_NEWOPP_DEMANDS_CHANGED
	11 = LGFR_ATP_DEMANDS_CHANGED
	12 = LGFR_REIMPLODE
//	13 = LGFR_RECALCULATE_FSS
        14 = LGFR_GO_FROM_CI_TO_ATPI
        15 = LGFR_GO_FROM_ATPI_TO_NOI

State Transition Graph:

  ---------------->
  ^     ^         |
  |     |    ---| |
  |    6|   8|  V V                   3			  10,11,12,13
  |    8|   9|  COMMITTED_IMPLODED ----------		-----------
  |    9|  10|  | |            5^	     |		|	    |
  |     |  11|  | |	       8|  	     V		V	    |
  |7    |  12|__| |1,14        9 ------	COMMITTED_IMPLODED_RECOVERY-
  |8    |	  |      		(Committed Imploded with Recovery Plan)
  |9    |_______  V      	   4	  |	
  |10    ------>NEWOPP_IMPLODED<--- asking for trouble
  |   10 |	| |
  |   11|       | |
  |   12|<-----	| |2,14
  |   15  	  |
  |     	  V
  <-------------ATP_IMPLODED	
		  ^	   |11
		  |<------- 12


*** INITIAL SETUP

When constructed, scenATP calls ipATP to set up the parts, demands, and BOMs
information, master supply schedule(MSS), and master demand schedule(MDS).
MDS keeps three kinds of current demands: committed, newOpp, and atp demands.
Three is a solution up to date flag for each of the three states
COMMITTED_IMPLODED, NEWOPP_IMPLODED and ATP_IMPLODED.
Finally, WIP will be initialized with the supply schedule and committed demand
schedule.

*** LGFR_ATP_STATE_COMMITTED_IMPLODED = INITIAL STATE

       Part supply schedule is in master supply schedule and in WIP.
       Committed demand volume is in master demand schedule and in WIP.
       New Opportunity demand volume is in master demand schedule.
       ATP demand volume is in master demand schedule.
       WIP contains full set of supplies and committed demands.
       Committed shipment schedule is available but may not be correct.
       Committed excess supply volume is available when solutionUpToDateCI is
       True.
       
*** LGFR_ATP_STATE_NEWOPP_IMPLODED

       WIP contains committed excess supplies and new opportunity demands.
       NewOPP shipment schedule is available but may not be correct.
       NewOpp excess supply volume is available when solutionUpToDateNOI is 
       True.
       solutionUpToDateCI is True.

*** LGFR_ATP_STATE_ATP_IMPLODED			

       WIP contains new opportunity excess supplies and atp demands.
       ATP shipment schedule is available but may not be correct.
       ATP excess supply volume is available when solutionUpToDateATP is 
       True.
       solutionUpToDateCI is True.
       solutionUpToDateNOI is True.
       
*** LGFR_ATP_STATE_COMMITTED_IMPLODED_RECOVERY	

       WIP contains full set of supplies and committed demands.
       Committed shipment schedule is available and is correct.
       Committed excess supply volume is available.
       solutionUpToDateCI is True.
       FSS controller contains focussed shortage schedule.
*/


/*
       solutionUpToDateCI is True if and only if the committed shipment
       schedule is correct with respect to supply and committed demand 
       volumes.

       solutionUpToDateNOI is True if and only if the new opportunity shipment
       schedule is correct with respect to committed excess supplies and
       new opportunity demands.
	
       solutionUpToDateATP is True if and only if the ATP shipment schedule
       is correct with respect to newopp excess supplies and atp demands.

*/


isAReturnType
LgFrScenarioForATP::isA() const
{
    return id_;
}


// force state change, issue tokens to go from the current_state to state
void
LgFrScenarioForATP::changeState(int state)
{
  int currState = currentState();
  if (currState == state)  {
    return;
  }
  if (currState == LGFR_ATP_STATE_COMMITTED_IMPLODED)  {
    if (state == LGFR_ATP_STATE_NEWOPP_IMPLODED)  {
      theDfa_->InputToken(LGFR_ATP_TOKEN_GO_FROM_CI_TO_NOI);
    }
    else if (state == LGFR_ATP_STATE_ATP_IMPLODED)  {
      // this token is smart and we can optimize
      theDfa_->InputToken(LGFR_ATP_TOKEN_GO_FROM_CI_TO_ATPI);
    }
    else if (state == LGFR_ATP_STATE_COMMITTED_IMPLODED_RECOVERY)  {
      theDfa_->InputToken(LGFR_ATP_TOKEN_GO_FROM_CI_TO_FSS);
    }
  }
  else if (currState == LGFR_ATP_STATE_NEWOPP_IMPLODED)  {
    if (state == LGFR_ATP_STATE_COMMITTED_IMPLODED) {
      theDfa_->InputToken(LGFR_ATP_TOKEN_GO_FROM_NOI_TO_CI);
    }
    else if (state == LGFR_ATP_STATE_ATP_IMPLODED) {
      theDfa_->InputToken(LGFR_ATP_TOKEN_GO_FROM_NOI_TO_ATPI);
    }
    else if (state == LGFR_ATP_STATE_COMMITTED_IMPLODED_RECOVERY) {
      theDfa_->InputToken(LGFR_ATP_TOKEN_GO_FROM_NOI_TO_CI);
      theDfa_->InputToken(LGFR_ATP_TOKEN_GO_FROM_CI_TO_FSS);    
    }
  }
  else if (currState == LGFR_ATP_STATE_ATP_IMPLODED)  {
    if (state == LGFR_ATP_STATE_COMMITTED_IMPLODED) {
      theDfa_->InputToken(LGFR_ATP_TOKEN_GO_FROM_ATPI_TO_CI);
    }
    else if (state == LGFR_ATP_STATE_NEWOPP_IMPLODED) {
      // we can be tricky here if we save the excess supply
      theDfa_->InputToken(LGFR_ATP_TOKEN_GO_FROM_ATPI_TO_NOI);
    }
    else if (state == LGFR_ATP_STATE_COMMITTED_IMPLODED_RECOVERY) {
      theDfa_->InputToken(LGFR_ATP_TOKEN_GO_FROM_ATPI_TO_CI);
      theDfa_->InputToken(LGFR_ATP_TOKEN_GO_FROM_CI_TO_FSS);    
    }
  }
  else if (currState == LGFR_ATP_STATE_COMMITTED_IMPLODED_RECOVERY)  {
    if (state == LGFR_ATP_STATE_COMMITTED_IMPLODED) {
      theDfa_->InputToken(LGFR_ATP_TOKEN_GO_FROM_FSS_TO_CI);
    }
    else if (state == LGFR_ATP_STATE_NEWOPP_IMPLODED) {
#ifdef ASKING_FOR_TROUBLE      
      theDfa_->InputToken(LGFR_ATP_TOKEN_GO_FROM_FSS_TO_NOI);
#else
      theDfa_->InputToken(LGFR_ATP_TOKEN_GO_FROM_FSS_TO_CI);
      theDfa_->InputToken(LGFR_ATP_TOKEN_GO_FROM_CI_TO_NOI);
#endif
    }
    else if (state == LGFR_ATP_STATE_ATP_IMPLODED) {
#ifdef ASKING_FOR_TROUBLE      
      theDfa_->InputToken(LGFR_ATP_TOKEN_GO_FROM_FSS_TO_NOI);
      theDfa_->InputToken(LGFR_ATP_TOKEN_GO_FROM_NOI_TO_ATPI);
#else
      theDfa_->InputToken(LGFR_ATP_TOKEN_GO_FROM_FSS_TO_CI);
      // this token is smart and we can optimize
      theDfa_->InputToken(LGFR_ATP_TOKEN_GO_FROM_CI_TO_ATPI);
#endif
    }
  }
}

// do implosion if the solutionUpToDate flag of the current state is not TRUE
void
LgFrScenarioForATP::newImplosionSolution ()
{
    lastProblemSolved().computeCriticalList(TRUE);
    switch (currentState()) {
    case LGFR_ATP_STATE_COMMITTED_IMPLODED:
	if (!solutionIsUpToDateCI()) {
cerr << "scenATP.C: Imploding for Committed Orders" << endl;
	    this->LgFrScenario::newImplosionSolution();
	    this->setSolutionUpToDateCI();
	}
	break;
    case LGFR_ATP_STATE_NEWOPP_IMPLODED:
	if (!solutionIsUpToDateNOI()) {
cerr << "scenATP.C: Imploding for New-Op Orders" << endl;          
	    this->LgFrScenario::newImplosionSolution();
	    this->setSolutionUpToDateNOI();
	} 
	break;
    case LGFR_ATP_STATE_ATP_IMPLODED:
	if (!solutionIsUpToDateATPI()) {
cerr << "scenATP.C: Imploding for ATP (Forecast) Orders" << endl;
            this->LgFrScenario::newImplosionSolution();
	    this->setSolutionUpToDateATPI();
	}
	break;
    }
};

void
LgFrScenarioForATP::newDemand (const LgFrDemand d, int demandSetNumber)
{
  // Verify that d is in the universal set and demandSetNumber is valid
  assert (this->setOfDemands(LGFR_UNIVERSAL_SET).contains(d));
  assert (demandSetNumber == LGFR_COMMITTED_DEMANDS
          || demandSetNumber == LGFR_NEWOPPOR_DEMANDS
          || demandSetNumber == LGFR_FIRMFORECAST_DEMANDS);

  // Insert d into the requested set
  this->setOfDemands(demandSetNumber).insert(d);

  // Put d into WIT
  LgFrTimeVecFloat ztv(calendar().nPeriods(), 0.0);
  workingImplosionProblem().timeVec(d, ztv);
  abort ();  // I think you have to put it into LPS too --- Wally

  // Give d the right attributes in WIT
  abort ();
#ifdef DONE_FIXING_ATP
  cout << "You're not really done fixing ATP, are you?" << endl;
#endif
}

RWBoolean
LgFrScenarioForATP::newNewOppDemand (const RWCString & demandName,
                                     const LgFrPart & part)
{
  LgFrInitialProblemForATP * ipATP = (LgFrInitialProblemForATP *) &(this->initialProblemAndParameters());
  
  RWBoolean retVal =  ipATP->addNewNewOppDemand(this->lastProblemSolved(),
                                                demandName,
                                                part);

  // tell WIP that the demandVol has changed. This "starts" a model/client
  // notification impulse which gets the demandVol schedules re-populated
  const LgFrDemand * demand =
        this->setOfDemands(LGFR_NEWOPPOR_DEMANDS).findValue(demandName, part.name());
  assert (demand != NULL);
  LgFrTimeVecFloat ztv(this->calendar().nPeriods(), 0.0);
  this->workingImplosionProblem().timeVec(*demand, ztv);
  
  
  // if it worked, then any newOpp or atp implode solutions are invalid
  if (retVal == TRUE)
    this->clearSolutionUpToDateNOI();

  return retVal;
}

LgFrSetOfParts&
LgFrScenarioForATP::setOfParts(int k)
{
  // OK to cast away const. This is a non-const member function
  return (LgFrSetOfParts &) constSetOfParts(k);
}


const
LgFrSetOfParts&
LgFrScenarioForATP::constSetOfParts(int k)
const
{
  switch (k) {
    case LGFR_CAPACITY_SET:
      return capacitySOP_;
    case LGFR_MATERIAL_SET:
      return materialSOP_;
    case LGFR_DEFAULT_SET:
#ifdef DEFAULT_SOP_DIFFERS_FROM_UNIVERSAL_SOP      
       return defaultSOP_;
#else
       return LgFrScenario::constSetOfParts(LGFR_UNIVERSAL_SET);
#endif    
    case LGFR_UNIVERSAL_SET:
       return LgFrScenario::constSetOfParts(LGFR_UNIVERSAL_SET);
    default:
       cerr << "Request for set of parts that does not exist in this scenario"
            << endl;
       abort();
    }
#ifdef DEFAULT_SOP_DIFFERS_FROM_UNIVERSAL_SOP      
  return defaultSOP_;
#else
  return LgFrScenario::constSetOfParts(LGFR_UNIVERSAL_SET);
#endif    
}


LgFrSetOfDemands&
LgFrScenarioForATP::setOfDemands(int k)
{
  // OK to cast away const. This is a non-const member function
  return (LgFrSetOfDemands &) constSetOfDemands(k);
}

const
LgFrSetOfDemands&
LgFrScenarioForATP::constSetOfDemands(int k)
const
{
  assert(k > LGFR_NULL_DEMANDS && k < LGFR_LAST_DEMANDS);
  switch (k) {
    case LGFR_DEFAULT_SET:
       return defaultSOD_;
    case LGFR_UNIVERSAL_SET:
       return LgFrScenario::constSetOfDemands(LGFR_UNIVERSAL_SET);
    case LGFR_COMMITTED_DEMANDS:
       return committedSOD_;
    case LGFR_FIRMFORECAST_DEMANDS:
       return firmForecastSOD_;
#ifdef USING_RISK_FORECAST
    case LGFR_RISKFORECAST_DEMANDS:
       return riskForecastSOD_;
#endif
    case LGFR_NEWOPPOR_DEMANDS:
       return newOpportunitySOD_;
    default:
       cerr << "Request for set of demands that does not exist in this scenario"
            << endl;
       abort();
     }
  return defaultSOD_;
}

LgFrSortingPartScheduleFloat*
LgFrScenarioForATP::masterPartVolumes()
{
  return masterPartVolumesPtr_;
}

const LgFrSortingPartScheduleFloat*
LgFrScenarioForATP::masterPartVolumes() const
{
  return masterPartVolumesPtr_;
}

LgFrSortingDemandScheduleFloat*
LgFrScenarioForATP::masterDemandVolumes()
{
  return masterDemandVolumesPtr_;
}

const LgFrSortingDemandScheduleFloat*
LgFrScenarioForATP::masterDemandVolumes() const
{
  return masterDemandVolumesPtr_;
}


void
LgFrScenarioForATP::setWipDemandsForState (int s)
{
  const LgFrScheduleDescriptor * const
    relevantDemandVolDesc	// relevant to state s
      =
	(s==LGFR_ATP_STATE_COMMITTED_IMPLODED ? &LGFRCommittedDemandVolume
	 : (s==LGFR_ATP_STATE_NEWOPP_IMPLODED ? &LGFRNewOpportunityDemandVolume
	    : (s==LGFR_ATP_STATE_ATP_IMPLODED ? &LGFRFirmForecastDemandVolume
	       : (s==LGFR_ATP_STATE_COMMITTED_IMPLODED_RECOVERY ?
		                                    &LGFRCommittedDemandVolume
		  : NULL ) ) ) );
  assert (relevantDemandVolDesc != NULL);

  LgFrSortingDemandScheduleFloatSmartPointer newDemandVol // get relevant demand volumes
    = (  this->scheduleFactory()->newDemandSchedule(*relevantDemandVolDesc)  );
  overlayDemand (*newDemandVol, *masterDemandVolumesPtr_); // copy them into WIP

}


void
LgFrScenarioForATP::setWipPartsFromSchedule
  (const LgFrSortingPartScheduleFloat & s )
{
  overlaySupply (s, *masterPartVolumesPtr_);
}



// Drop in a demandVol schedule
void
LgFrScenarioForATP::overlayDemand( 
     const LgFrSortingDemandScheduleFloat & newVol,
     const LgFrSortingDemandScheduleFloat & oldVol)

{

  // Give overlayVol a schedule that
  // contains newVol's data, and zero timeVecs for demands that are in oldVol
  // but not newVol
  const LgFrSortingDemandScheduleFloat overlayVol(
       oldVol.op(
#ifdef WIN32
  // Other compilers don't need this cast-not req'd for MSVC4, req'd for MSVC2
          (LgFrTimeVecFloatOpFunction)
#endif
          &LgFrTimeVecFloat::returnRHS, newVol ) );
  
  // Drop in the overlayVol into WIT
  int nDemands = overlayVol.entries();
  workingImplosionProblem().beginSeriesOfChanges();
  for (int i=0; i<nDemands; i++)
     workingImplosionProblem().timeVec(  overlayVol[i].demand(),
                                         overlayVol[i].timeVecFloat());
  workingImplosionProblem().endSeriesOfChanges();
}


// Drop in a supplyVol schedule
void
LgFrScenarioForATP::overlaySupply( 
     const LgFrSortingPartScheduleFloat & newVol,
     const LgFrSortingPartScheduleFloat & oldVol)

{

  // Give overlayVol a schedule that
  // contains newVol's data, and zero timeVecs for parts that are in oldVol
  // but not newVol
  const LgFrSortingPartScheduleFloat overlayVol(
       oldVol.op(
#ifdef WIN32
  // Other compilers don't need this cast-not req'd for MSVC4, req'd for MSVC2
          (LgFrTimeVecFloatOpFunction)
#endif
          &LgFrTimeVecFloat::returnRHS, newVol ) );

  // Drop in the overlayVol into WIT
  int nSupplys = overlayVol.entries();
  workingImplosionProblem().beginSeriesOfChanges();
  for (int i=0; i<nSupplys; i++)
     workingImplosionProblem().timeVec(  overlayVol[i].part(),
                     overlayVol[i].timeVecFloat());
  workingImplosionProblem().endSeriesOfChanges();
}


// Constructor
LgFrScenarioForATP::LgFrScenarioForATP(
    const LgFrDataInterfaceAbstract&    dif,
    const LgFrCalendar&         calendar)
: LgFrScenario(dif, calendar),
#ifdef DEFAULT_SOP_DIFFERS_FROM_UNIVERSAL_SOP      
  defaultSOP_(0),
#endif  
  defaultSOD_(0),
  capacitySOP_(),          // specifying 0 capacity should never be necessary
  materialSOP_(),          // specifying 0 capacity should never be necessary
  committedSOD_(0),
  newOpportunitySOD_(0),
  firmForecastSOD_(0),
#ifdef USING_RISK_FORECAST
  riskForecastSOD_(0),
#endif
  masterPartVolumesPtr_(new LgFrSortingPartScheduleFloat() ),
  masterDemandVolumesPtr_(new LgFrSortingDemandScheduleFloat() ),
  committedImplodedExcessSupplySchedulePtr_(new LgFrSortingPartScheduleFloat()),
  committedSolutionUpToDate_(FALSE),
  newOppSolutionUpToDate_(FALSE),
  atpSolutionUpToDate_(FALSE),
  atpTransTable_( new DfaTransRec[LGFR_MAXLENGTH_TRANSTABLE] ),
  theDfa_(new LgFrDfa(atpTransTable_, this)),
  id_(__LGFRSCENARIOFORATP)
{
  populateDfaTransTableForATP(atpTransTable_, LGFR_MAXLENGTH_TRANSTABLE);
  // builder does the rest
}

// Destructor 
LgFrScenarioForATP::~LgFrScenarioForATP()
{
  // Remove master schedules after the schedules that subscribe to them have been
  // deleted!  (Otherwise the subscribers remove_subscription calls will fail.)
  delete masterDemandVolumesPtr_;
  delete masterPartVolumesPtr_;

  // remove excess supply schedule
  delete committedImplodedExcessSupplySchedulePtr_;
  
  // delete the state machine and the transition table
  delete atpTransTable_;
  delete theDfa_;
}

// Assignment operator.
LgFrScenarioForATP&
LgFrScenarioForATP::operator=(
   const LgFrScenarioForATP& rhs)
{
    cerr << "Don't use LgFrScenarioForATP::operator=(const LgFrScenarioForATP&)" << endl;
    assert(0);
/*
   if (this != &rhs) {           // Check for assignment to self
     (this->LgFrScenario::operator= ) (rhs);
#ifdef DEFAULT_SOP_DIFFERS_FROM_UNIVERSAL_SOP
     defaultSOP_ = rhs.defaultSOP_;
#endif     
     defaultSOD_ = rhs.defaultSOD_;
     capacitySOP_ = rhs.capacitySOP_;
     materialSOP_ = rhs.materialSOP_; 
     committedSOD_ = rhs.committedSOD_;
     newOpportunitySOD_ = rhs.newOpportunitySOD_;
     firmForecastSOD_ = rhs.firmForecastSOD_;
#ifdef USING_RISK_FORECAST
     riskForecastSOD_ = rhs.riskForecastSOD_;
#endif
     delete masterDemandVolumesPtr_;
     delete masterPartVolumesPtr_;
     masterPartVolumesPtr_
       = (new LgFrSortingPartScheduleFloat(*(rhs.masterPartVolumesPtr_),
                                           *this)
          );
     masterDemandVolumesPtr_
       = (new LgFrSortingDemandScheduleFloat(*(rhs.masterDemandVolumesPtr_),
                                             *this)
          );
     delete committedImplodedExcessSupplySchedulePtr_;
     committedImplodedExcessSupplySchedulePtr_
       = (new LgFrSortingPartScheduleFloat(
         *(rhs.committedImplodedExcessSupplySchedulePtr_), *this) );
     
     
     committedSolutionUpToDate_ = rhs.committedSolutionUpToDate_;
     newOppSolutionUpToDate_ = rhs.newOppSolutionUpToDate_;
     atpSolutionUpToDate_ = rhs.atpSolutionUpToDate_;

     atpTransTable_ = rhs.atpTransTable_;
     theDfa_ = rhs.theDfa_;
   }
*/
   return *this;
}


// Customized copy constructor
LgFrScenarioForATP::LgFrScenarioForATP(
   const LgFrScenarioForATP& source )
: LgFrScenario( source ),
#ifdef DEFAULT_SOP_DIFFERS_FROM_UNIVERSAL_SOP
  defaultSOP_(source.defaultSOP_),
#endif  
  defaultSOD_(source.defaultSOD_),
  capacitySOP_(source.capacitySOP_),
  materialSOP_(source.materialSOP_),
  committedSOD_(source.committedSOD_),
  newOpportunitySOD_(source.newOpportunitySOD_),
  firmForecastSOD_(source.firmForecastSOD_),
#ifdef USING_RISK_FORECAST
  riskForecastSOD_(source.riskForecastSOD_),
#endif
  masterPartVolumesPtr_
    (new LgFrSortingPartScheduleFloat(*(source.masterPartVolumesPtr_),
                                      *this                           ) ),
  masterDemandVolumesPtr_
    (new LgFrSortingDemandScheduleFloat(*(source.masterDemandVolumesPtr_),
                                          *this                            ) ),
  committedImplodedExcessSupplySchedulePtr_
       (new LgFrSortingPartScheduleFloat(
         *(source.committedImplodedExcessSupplySchedulePtr_), *this) ),
  committedSolutionUpToDate_(source.committedSolutionUpToDate_),
  newOppSolutionUpToDate_(source.newOppSolutionUpToDate_),
  atpSolutionUpToDate_(source.atpSolutionUpToDate_),
  atpTransTable_( new DfaTransRec[LGFR_MAXLENGTH_TRANSTABLE] ),
  theDfa_(new LgFrDfa(atpTransTable_, this)),
  id_(__LGFRSCENARIOFORATP)
{
  // state machine is the same for all scenATPs, so we just built it
  populateDfaTransTableForATP(atpTransTable_, LGFR_MAXLENGTH_TRANSTABLE);
}

// Default constructor
LgFrScenarioForATP::LgFrScenarioForATP()
: LgFrScenario(),
  masterPartVolumesPtr_( new LgFrSortingPartScheduleFloat() ),
  masterDemandVolumesPtr_( new LgFrSortingDemandScheduleFloat() ),
  committedImplodedExcessSupplySchedulePtr_(new LgFrSortingPartScheduleFloat()),  
  committedSolutionUpToDate_(FALSE),
  newOppSolutionUpToDate_(FALSE),
  atpSolutionUpToDate_(FALSE),
  atpTransTable_( new DfaTransRec[LGFR_MAXLENGTH_TRANSTABLE] ),
  theDfa_(new LgFrDfa(atpTransTable_, this)),
  id_(__LGFRSCENARIOFORATP)
// SOD_s and SOP_s will be set by ipATP...
{
  populateDfaTransTableForATP(atpTransTable_, LGFR_MAXLENGTH_TRANSTABLE);
}

// check whether the solution is up to date for current state 
int
LgFrScenarioForATP::solutionIsUpToDate(int state) const
{
	if (state == LGFR_ATP_STATE_COMMITTED_IMPLODED)
		return committedSolutionUpToDate_;
	else if (state == LGFR_ATP_STATE_NEWOPP_IMPLODED)
		return newOppSolutionUpToDate_;
	else if (state == LGFR_ATP_STATE_ATP_IMPLODED)
		return atpSolutionUpToDate_;
	else if (state == LGFR_ATP_STATE_COMMITTED_IMPLODED_RECOVERY)
		return committedSolutionUpToDate_;
	else {
		cerr << "solutionIsUpToDate(): Illegal state "
		     << state << endl;
		return FALSE;
		// should we exit here?
	}
}

int LgFrScenarioForATP::solutionIsUpToDateCI() const
{
	return committedSolutionUpToDate_;
}

int LgFrScenarioForATP::solutionIsUpToDateNOI() const
{
	return newOppSolutionUpToDate_;
}

int LgFrScenarioForATP::solutionIsUpToDateATPI() const
{
	return atpSolutionUpToDate_;
}

void
LgFrScenarioForATP::setSolutionUpToDateCI()
{
	committedSolutionUpToDate_ = TRUE;
}

void
LgFrScenarioForATP::setSolutionUpToDateNOI()
{
	newOppSolutionUpToDate_ = TRUE;
}

void
LgFrScenarioForATP::setSolutionUpToDateATPI()
{
	atpSolutionUpToDate_ = TRUE;
}

void
LgFrScenarioForATP::clearSolutionUpToDateCI()
{
	committedSolutionUpToDate_ = FALSE;
	newOppSolutionUpToDate_ = FALSE;
	atpSolutionUpToDate_ = FALSE;
}

void
LgFrScenarioForATP::clearSolutionUpToDateNOI()
{
	newOppSolutionUpToDate_ = FALSE;
	atpSolutionUpToDate_ = FALSE;
}

void 
LgFrScenarioForATP::clearSolutionUpToDateATPI()
{
	atpSolutionUpToDate_ = FALSE;
}


// Populate the Dfa tranistion table for ATP
void
LgFrScenarioForATP::populateDfaTransTableForATP(DfaTransPtr atpTable, int tableSize)
{
	DfaTransPtr transPtr = atpTable;
	int i = 0;
	
	// define the transition table

	transPtr->state = LGFR_ATP_STATE_COMMITTED_IMPLODED;
	transPtr->token = tokenGoFromCItoNOI;
	transPtr->newState = LGFR_ATP_STATE_NEWOPP_IMPLODED;
	transPtr->accumFunc = ::noAccumFunc;
	transPtr->emitFunc = ::emitGoFromCItoNOI;

	transPtr++; i++;
	assert( i < tableSize );
	transPtr->state = LGFR_ATP_STATE_COMMITTED_IMPLODED;
	transPtr->token = tokenGoFromCItoFSS;
	transPtr->newState = LGFR_ATP_STATE_COMMITTED_IMPLODED_RECOVERY;
	transPtr->accumFunc = ::noAccumFunc;
	transPtr->emitFunc = ::emitGoFromCItoFSS;

	transPtr++; i++;
	assert( i < tableSize );
	transPtr->state = LGFR_ATP_STATE_COMMITTED_IMPLODED;
	transPtr->token = tokenSupplyChanged;
	transPtr->newState = STATE_SAME;
	transPtr->accumFunc = ::clearSolutionUpToDateCI;
	transPtr->emitFunc = ::noEmitFunc;

	transPtr++; i++;
	assert( i < tableSize );
	transPtr->state = LGFR_ATP_STATE_COMMITTED_IMPLODED;
	transPtr->token = tokenCommittedDemandsChanged;
	transPtr->newState = STATE_SAME;
	transPtr->accumFunc = ::clearSolutionUpToDateCI;
	transPtr->emitFunc = ::noEmitFunc;

	transPtr++; i++;
	assert( i < tableSize );
	transPtr->state = LGFR_ATP_STATE_COMMITTED_IMPLODED;
	transPtr->token = tokenNewOppDemandsChanged;
	transPtr->newState = STATE_SAME;
	transPtr->accumFunc = ::clearSolutionUpToDateNOI;
	transPtr->emitFunc = ::noEmitFunc;

	transPtr++; i++;
	assert( i < tableSize );
	transPtr->state = LGFR_ATP_STATE_COMMITTED_IMPLODED;
	transPtr->token = tokenATPDemandsChanged;
	transPtr->newState = STATE_SAME;
	transPtr->accumFunc = ::clearSolutionUpToDateATPI;
	transPtr->emitFunc = ::noEmitFunc;

	transPtr++; i++;
	assert( i < tableSize );
	transPtr->state = LGFR_ATP_STATE_COMMITTED_IMPLODED;
	transPtr->token = tokenReimplode;
	transPtr->newState = STATE_SAME;
	transPtr->accumFunc = ::newImplosionSolution;
	transPtr->emitFunc = ::noEmitFunc;

#ifdef ASKING_FOR_TROUBLE
	transPtr++; i++;
	assert( i < tableSize );
	transPtr->state = LGFR_ATP_STATE_COMMITTED_IMPLODED_RECOVERY;
	transPtr->token = tokenGoFromFSStoNOI;
	transPtr->newState = LGFR_ATP_STATE_NEWOPP_IMPLODED;
	transPtr->accumFunc = ::noAccumFunc;
	transPtr->emitFunc = ::emitGoFromFSStoNOI;
#endif
        
	transPtr++; i++;
	assert( i < tableSize );
	transPtr->state = LGFR_ATP_STATE_COMMITTED_IMPLODED_RECOVERY;
	transPtr->token = tokenGoFromFSStoCI;
	transPtr->newState = LGFR_ATP_STATE_COMMITTED_IMPLODED;
	transPtr->accumFunc = ::noAccumFunc;
	transPtr->emitFunc = ::emitGoFromFSStoCI;

	transPtr++; i++;
	assert( i < tableSize );
	transPtr->state = LGFR_ATP_STATE_COMMITTED_IMPLODED_RECOVERY;
	transPtr->token = tokenSupplyChanged;
	transPtr->newState = LGFR_ATP_STATE_COMMITTED_IMPLODED;
	transPtr->accumFunc = ::noAccumFunc;
	transPtr->emitFunc = ::emitGoFromFSStoCIandClearSolutionUpToDateCI;

	transPtr++; i++;
	assert( i < tableSize );
	transPtr->state = LGFR_ATP_STATE_COMMITTED_IMPLODED_RECOVERY;
	transPtr->token = tokenCommittedDemandsChanged;
	transPtr->newState = LGFR_ATP_STATE_COMMITTED_IMPLODED;
	transPtr->accumFunc = ::noAccumFunc;
	transPtr->emitFunc = ::emitGoFromFSStoCIandClearSolutionUpToDateCI;

	transPtr++; i++;
	assert( i < tableSize );
	transPtr->state = LGFR_ATP_STATE_COMMITTED_IMPLODED_RECOVERY;
	transPtr->token = tokenNewOppDemandsChanged;
	transPtr->newState = STATE_SAME;
	transPtr->accumFunc = ::clearSolutionUpToDateNOI;
	transPtr->emitFunc = ::noEmitFunc;

	transPtr++; i++;
	assert( i < tableSize );
	transPtr->state = LGFR_ATP_STATE_COMMITTED_IMPLODED_RECOVERY;
	transPtr->token = tokenATPDemandsChanged;
	transPtr->newState = STATE_SAME;
	transPtr->accumFunc = ::clearSolutionUpToDateATPI;
	transPtr->emitFunc = ::noEmitFunc;

	transPtr++; i++;
	assert( i < tableSize );
	transPtr->state = LGFR_ATP_STATE_NEWOPP_IMPLODED;
	transPtr->token = tokenGoFromNOItoATPI;
	transPtr->newState = LGFR_ATP_STATE_ATP_IMPLODED;
	transPtr->accumFunc = ::noAccumFunc;
	transPtr->emitFunc = ::emitGoFromNOItoATPI;

	transPtr++; i++;
	assert( i < tableSize );
	transPtr->state = LGFR_ATP_STATE_NEWOPP_IMPLODED;
	transPtr->token = tokenGoFromNOItoCI;
	transPtr->newState = LGFR_ATP_STATE_COMMITTED_IMPLODED;
	transPtr->accumFunc = ::noAccumFunc;
	transPtr->emitFunc = ::emitGoToCI;

	transPtr++; i++;
	assert( i < tableSize );
	transPtr->state = LGFR_ATP_STATE_NEWOPP_IMPLODED;
	transPtr->token = tokenSupplyChanged;
	transPtr->newState = LGFR_ATP_STATE_COMMITTED_IMPLODED;
	transPtr->accumFunc = ::noAccumFunc;
	transPtr->emitFunc = ::emitGoToCI;

	transPtr++; i++;
	assert( i < tableSize );
	transPtr->state = LGFR_ATP_STATE_NEWOPP_IMPLODED;
	transPtr->token = tokenCommittedDemandsChanged;
	transPtr->newState = LGFR_ATP_STATE_COMMITTED_IMPLODED;
	transPtr->accumFunc = ::noAccumFunc;
	transPtr->emitFunc = ::emitGoToCI;

	transPtr++; i++;
	assert( i < tableSize );
	transPtr->state = LGFR_ATP_STATE_NEWOPP_IMPLODED;
	transPtr->token = tokenNewOppDemandsChanged;
	transPtr->newState = STATE_SAME;
	transPtr->accumFunc = ::clearSolutionUpToDateNOI;
	transPtr->emitFunc = ::noEmitFunc;

	transPtr++; i++;
	assert( i < tableSize );
	transPtr->state = LGFR_ATP_STATE_NEWOPP_IMPLODED;
	transPtr->token = tokenATPDemandsChanged;
	transPtr->newState = STATE_SAME;
	transPtr->accumFunc = ::clearSolutionUpToDateATPI;
	transPtr->emitFunc = ::noEmitFunc;

	transPtr++; i++;
	assert( i < tableSize );
	transPtr->state = LGFR_ATP_STATE_NEWOPP_IMPLODED;
	transPtr->token = tokenReimplode;
	transPtr->newState = STATE_SAME;
	transPtr->accumFunc = ::newImplosionSolution;
	transPtr->emitFunc = ::noEmitFunc;

	transPtr++; i++;
	assert( i < tableSize );
	transPtr->state = LGFR_ATP_STATE_ATP_IMPLODED;
	transPtr->token = tokenGoFromATPItoCI;
	transPtr->newState = LGFR_ATP_STATE_COMMITTED_IMPLODED;
	transPtr->accumFunc = ::noAccumFunc;
	transPtr->emitFunc = ::emitGoToCI;

	transPtr++; i++;
	assert( i < tableSize );
	transPtr->state = LGFR_ATP_STATE_ATP_IMPLODED;
	transPtr->token = tokenSupplyChanged;
	transPtr->newState = LGFR_ATP_STATE_COMMITTED_IMPLODED;
	transPtr->accumFunc = ::noAccumFunc;
	transPtr->emitFunc = ::emitGoToCI;

	transPtr++; i++;
	assert( i < tableSize );
	transPtr->state = LGFR_ATP_STATE_ATP_IMPLODED;
	transPtr->token = tokenCommittedDemandsChanged;
	transPtr->newState = LGFR_ATP_STATE_COMMITTED_IMPLODED;
	transPtr->accumFunc = ::noAccumFunc;
	transPtr->emitFunc = ::emitGoToCI;

	transPtr++; i++;
	assert( i < tableSize );
	transPtr->state = LGFR_ATP_STATE_ATP_IMPLODED;
	transPtr->token = tokenNewOppDemandsChanged;
	transPtr->newState = LGFR_ATP_STATE_COMMITTED_IMPLODED;
	transPtr->accumFunc = ::noAccumFunc;
	transPtr->emitFunc = ::emitGoToCI;

	transPtr++; i++;
	assert( i < tableSize );
	transPtr->state = LGFR_ATP_STATE_ATP_IMPLODED;
	transPtr->token = tokenATPDemandsChanged;
	transPtr->newState = STATE_SAME;
	transPtr->accumFunc = ::clearSolutionUpToDateATPI;
	transPtr->emitFunc = ::noEmitFunc;

	transPtr++; i++;
	assert( i < tableSize );
	transPtr->state = LGFR_ATP_STATE_ATP_IMPLODED;
	transPtr->token = tokenReimplode;
	transPtr->newState = STATE_SAME;
	transPtr->accumFunc = ::newImplosionSolution;
	transPtr->emitFunc = ::noEmitFunc;

	transPtr++; i++;
	assert( i < tableSize );
	transPtr->state = LGFR_ATP_STATE_COMMITTED_IMPLODED;
	transPtr->token = tokenGoFromCItoATPI;
	transPtr->newState = LGFR_ATP_STATE_ATP_IMPLODED;
	transPtr->accumFunc = ::noAccumFunc;
	transPtr->emitFunc = ::emitGoFromCItoATPI;


	transPtr++; i++;
	assert( i < tableSize );
	transPtr->state = LGFR_ATP_STATE_ATP_IMPLODED;
	transPtr->token = tokenGoFromATPItoNOI;
	transPtr->newState = LGFR_ATP_STATE_NEWOPP_IMPLODED;
	transPtr->accumFunc = ::noAccumFunc;
	transPtr->emitFunc = ::emitGoFromATPItoNOI;
        
	transPtr++; i++;
	assert( i < tableSize );
	transPtr->state = STATE_EOLIST;

}

void
LgFrScenarioForATP::noAccumFunc()
{
	cerr << "no accumFunc is defined" << endl;
	cerr << "current state: " << currentState() << endl;
	cerr << "current token: " << theDfa_->GetCurrToken()->type << endl;
}

void
LgFrScenarioForATP::noEmitFunc()
{
	cerr << "no emitFunc is defined" << endl;
	cerr << "current state: " << currentState() << endl;
	cerr << "current token: " << theDfa_->GetCurrToken()->type << endl;
}

inline void
LgFrScenarioForATP::accumNoAction()
{
// no action is needed
}

inline void
LgFrScenarioForATP::emitNoAction()
{
// no action is needed
}

/*
void
LgFrScenarioForATP::emitRecalculateFSS()
{
// the function is here for future enhancement
// do nothing for now
	cerr << "emitRecalculateFSS() not implemented yet." << endl;
//	if (!this->solutionIsUpToDateCI()) {
//		newImplosionSolution();
//		this->setSolutionUpToDateCI();
//	}

//	getLatestFSS();
}
*/

// Go from the current state to COMMITTED_IMPLODED.
// Actions:
//	Set the supply volume schedule in WIP to the supply volumes in MSS
//	Set the demand volumes in WIP to committed demands, set all other
//	non-committed demand volumes in WIP to 0
//	clear all solutionUpToDate flags
void
LgFrScenarioForATP::emitGoToCI()
{
	// send supplies in masterPartVolumes to WIP
	int nSupplys = masterPartVolumes()->entries();
	const LgFrSortingScheduleFloat partVolumes(*masterPartVolumes());
        workingImplosionProblem().beginSeriesOfChanges();
	for (int i=0; i<nSupplys; i++) {
		workingImplosionProblem().timeVec(
			partVolumes[i].part(), partVolumes[i].timeVecFloat());
	}
        workingImplosionProblem().endSeriesOfChanges();

	//	Set the demand volumes in WIP to committed demands
	setWipDemandsForState(LGFR_ATP_STATE_COMMITTED_IMPLODED);

	// clear all solutionUpToDate flags
	this->clearSolutionUpToDateCI();
}

// Go from state CI to NOI
// Actions:
//	If solutionUpToDate == FALSE then	do implosion
//	Set the supply volumes in WIP to the excessive supply volumes
//	Set the demand volumes in WIP to the newOpp demand schedule in MDS
//	clear solutionUpToDateNOI_ and solutionUpToDateATPI_
void
LgFrScenarioForATP::emitGoFromCItoNOI()
{
  if (!this->solutionIsUpToDateCI()) {
    newImplosionSolution();
    this->setSolutionUpToDateCI();
  }

  // Set the supply volumes in WIP to the excessive supply volumes
  setSupplyVolumesToExcessVolumes();

  // Save excess supply volume
  delete committedImplodedExcessSupplySchedulePtr_;
  LgFrSortingPartScheduleFloatSmartPointer tmp = 
      this->scheduleFactory()->newPartSchedule(LGFRExcessVolume, TRUE, TRUE);
  committedImplodedExcessSupplySchedulePtr_ = new LgFrSortingPartScheduleFloat();
  int numEntries = tmp->entries();
  for (int j = 0; j < numEntries; j++)
    committedImplodedExcessSupplySchedulePtr_->
      insert(((LgFrScheduleFloat&)(*tmp)).operator[]((size_t)j));


  // Set the demand volumes in WIP to the newOpp demand schedule in MDS
  setWipDemandsForState(LGFR_ATP_STATE_NEWOPP_IMPLODED);

  this->clearSolutionUpToDateNOI();
}

#ifdef ASKING_FOR_TROUBLE
// I took this out 10/22/94 because even though it seems to be the same
// as going from CI to NOI, unless the ASS's for ATP are written to handle
// it, they won't get populated with the results of the CI implosion.

// Go from FSS to NOI
// Actions:
//	Remove all demands from the FSS controller
//	If solutionUpToDate == FALSE then	do implosion
//	Set the supply volumes in WIP to the excess supply volumes.
//	Set the demand volumes in WIP to the newOpp demand volumes in MDS
//	and all demand volumes of non-newOpp demands to 0
//	Set solutionUpToDateNOI and solutionUpToDateATPI to FALSE.
void
LgFrScenarioForATP::emitGoFromFSStoNOI()
{
        this->focussedShortageScheduleControl().clearFocusHorizons();
	// rest is same as the above funtion
	emitGoFromCItoNOI();
}
#endif

// Go from CI to FSS
// Actions:
//	If solutionUpToDateCI == FALSE then
//		do implosion; set solutionUpToDateCI to TRUE.
//	Feed shortage demands and periods into FSS controller
//	Do FSS, get Latest Focussed Shortage Schedule
void
LgFrScenarioForATP::emitGoFromCItoFSS()
{
  if (!this->solutionIsUpToDateCI()) {
    // LgFrFocussedShortageScheduleScheduleStrategyForATP::populate()
    //   needs to know that we're coming from CI to FSS.  Since the
    //   state machine isn't updated yet, this method will look
    //   at the token.
    newImplosionSolution();
    this->setSolutionUpToDateCI();
  }
  
  // Save excess supply volume
  delete committedImplodedExcessSupplySchedulePtr_;
  LgFrSortingPartScheduleFloatSmartPointer tmp = 
      this->scheduleFactory()->newPartSchedule(LGFRExcessVolume, TRUE, TRUE);
  committedImplodedExcessSupplySchedulePtr_ 
    = new LgFrSortingPartScheduleFloat();
  int numEntries = tmp->entries();
  for (int j = 0; j < numEntries; j++)
    committedImplodedExcessSupplySchedulePtr_->
      insert(((LgFrScheduleFloat&)(*tmp)).operator[]((size_t)j));
  
  getLatestFSS();
}

// Go from FSS to CI
// Actions:
//	Remove all demands from FSS controller
void
LgFrScenarioForATP::emitGoFromFSStoCI()
{
        this->focussedShortageScheduleControl().clearFocusHorizons();
}

// Go from FSS to CI and clearSolutionUpToDateCI
// Actions:
//	Remove all demands from FSS controller
//	Call clearSolutionUpToDateCI()
void
LgFrScenarioForATP::emitGoFromFSStoCIandClearSolutionUpToDateCI()
{
        this->focussedShortageScheduleControl().clearFocusHorizons();
	this->clearSolutionUpToDateCI();
}


// Go from NOI to ATPI
// Actions:
// 	If solutionUpToDateNOI == FALSE then
//		do implosion; set solutionUptoDateNOI to TRUE
//	Set the supply volumes in WIP to the excessive supply volumes.
//	Set the demand volumes in WIP to the ATP demand volumes in MDS,
//	Set all non-ATP demand volumes in WIP to zero.
//	Set solutionUptoDateATPI to FALSE
	
void
LgFrScenarioForATP::emitGoFromNOItoATPI()
{
	if (!this->solutionIsUpToDateNOI()) {
		newImplosionSolution();
		this->setSolutionUpToDateNOI();
	}

	setSupplyVolumesToExcessVolumes();
	setWipDemandsForState(LGFR_ATP_STATE_ATP_IMPLODED);
	this->clearSolutionUpToDateATPI();
}

// Go from state CI to ATPI
// Actions:
//      If there are no new opportunity demands (we can bypass an implosion step) {
//	  If solutionUpToDate == FALSE then	do implosion
//	  Set the supply volumes in WIP to the excessive supply volumes
//	  Set the demand volumes in WIP to the ATP demand schedule in MDS
//        set solutionUpToDateNOI_ (but we really don't have any useful data here)
//	  clear solutionUpToDateATPI_
//      }
//      else  {
//        go to NOI state
//        go to ATP state
//      }
//
void
LgFrScenarioForATP::emitGoFromCItoATPI()
{
  if (constSetOfDemands(LGFR_NEWOPPOR_DEMANDS).entries() == 0) {
    if (!this->solutionIsUpToDateCI()) {
      newImplosionSolution();
      this->setSolutionUpToDateCI();
    }

    // Set the supply volumes in WIP to the excessive supply volumes
    setSupplyVolumesToExcessVolumes();

    // Save excess supply volume
    delete committedImplodedExcessSupplySchedulePtr_;
    LgFrSortingPartScheduleFloatSmartPointer tmp = 
      this->scheduleFactory()->newPartSchedule(LGFRExcessVolume, TRUE, TRUE);
    committedImplodedExcessSupplySchedulePtr_ = new LgFrSortingPartScheduleFloat();
    int numEntries = tmp->entries();
    for (int j = 0; j < numEntries; j++)
      committedImplodedExcessSupplySchedulePtr_->
        insert(((LgFrScheduleFloat&)(*tmp)).operator[]((size_t)j));


    // Set the demand volumes in WIP to the newOpp demand schedule in MDS
    setWipDemandsForState(LGFR_ATP_STATE_ATP_IMPLODED);
      
    this->setSolutionUpToDateNOI();
    this->clearSolutionUpToDateATPI();
  }
  else  {
    
    // have to do it the inefficient way as if we call emitGoFromCItoNOI() and
    //   emitGoFromNOItoATPI() it doesn't work as newImplosionSolution looks
    //   at the state machine and we aren't changing it.  For now just manually
    //   step through the state machine.
    changeState(LGFR_ATP_STATE_NEWOPP_IMPLODED);
    changeState(LGFR_ATP_STATE_ATP_IMPLODED);
  }
}

// Go from state ATPI to NOI
// Actions:
//      If committedImplode solution is up to date  {
//        just set up problem and we're done
//      }
//      else  {
//        go to CI 
//        go to NOI
//      }
//
void
LgFrScenarioForATP::emitGoFromATPItoNOI()
{

  // Set the supply volumes in WIP to the excessive supply volumes

  if (solutionIsUpToDateCI())  {
    setWipPartsFromSchedule(*committedImplodedExcessSupplySchedulePtr_);
    
    // Set the demand volumes in WIP to the newOpp demand schedule in MDS
    setWipDemandsForState(LGFR_ATP_STATE_NEWOPP_IMPLODED);
      
    this->clearSolutionUpToDateNOI();
  }
  else  {
    changeState(LGFR_ATP_STATE_COMMITTED_IMPLODED);
    changeState(LGFR_ATP_STATE_NEWOPP_IMPLODED);
  }
}


// reset the supply volumes in WIP to the excess volumes 

void
LgFrScenarioForATP::setSupplyVolumesToExcessVolumes()
{
	LgFrSortingPartScheduleFloatSmartPointer excessVol =
	    this->scheduleFactory()->newPartSchedule(LGFRExcessVolume, TRUE, TRUE);
	setWipPartsFromSchedule(*excessVol);
}

// get latest focused shortage schedule
void
LgFrScenarioForATP::getLatestFSS()
{
        this->setFSSInputToDefault(); 
        this->focussedShortageScheduleControl().updateFocussedShortageSchedule();
}

void
LgFrScenarioForATP::setFSSInputToDefault()
{
  // Get schedule for backlogged demands that are non-zero
  LgFrSortingDemandScheduleFloatSmartPointer backSchPtr
    = this->scheduleFactory()->newDemandSchedule(LGFRCommittedNonZeroBacklog, TRUE, TRUE);

  // Put all of the demands in *backSchPtr into the FSSController with the
  // horizon set to the last period.

  // clear old focus horizons
  this->focussedShortageScheduleControl().clearFocusHorizons();
  
  for (int i = 0; i < backSchPtr->entries(); i++) {
    this->focussedShortageScheduleControl().focusHorizon(
				backSchPtr->demand(i),
				this->lastProblemSolved().numberOfPeriods()-1);
  }

}


// Define global functions that call the member action functions
#define DefineRegularToMemberActionFunc(FuncName) \
void FuncName(void * scenarioATPPtr) \
{ \
((LgFrScenarioForATP *) scenarioATPPtr)->FuncName(); \
}

DefineRegularToMemberActionFunc(noAccumFunc)
DefineRegularToMemberActionFunc(noEmitFunc)
DefineRegularToMemberActionFunc(accumNoAction)
DefineRegularToMemberActionFunc(emitNoAction)

DefineRegularToMemberActionFunc(clearSolutionUpToDateCI)
DefineRegularToMemberActionFunc(clearSolutionUpToDateNOI)
DefineRegularToMemberActionFunc(clearSolutionUpToDateATPI)

DefineRegularToMemberActionFunc(newImplosionSolution)
// DefineRegularToMemberActionFunc(emitRecalculateFSS)
DefineRegularToMemberActionFunc(emitGoToCI)
DefineRegularToMemberActionFunc(emitGoFromCItoNOI)
DefineRegularToMemberActionFunc(emitGoFromCItoFSS)
DefineRegularToMemberActionFunc(emitGoFromFSStoCI)
DefineRegularToMemberActionFunc(emitGoFromFSStoCIandClearSolutionUpToDateCI)
#ifdef ASKING_FOR_TROUBLE
DefineRegularToMemberActionFunc(emitGoFromFSStoNOI)
#endif
DefineRegularToMemberActionFunc(emitGoFromNOItoATPI)
DefineRegularToMemberActionFunc(emitGoFromCItoATPI)
DefineRegularToMemberActionFunc(emitGoFromATPItoNOI)


const LgFrDfa*
LgFrScenarioForATP::dfa() const
{
  return theDfa_;
}

#ifdef NDEBUG
#undef NDEBUG
#endif

void
LgFrScenarioForATP::test()
{

  // Check the LgFrDataInterfaceImplmentationForTest
  // ===============================  
  LgFrCalendar calendar1;
  LgFrDataInterfaceImplementation* protoDif1ImpPtr
      = createDifTestImp();
  protoDif1ImpPtr->title("Test problem for LgFrDataInterfaceImplementation");
  registerDifTestImpFunctions(protoDif1ImpPtr);

  LgFrDataInterface protoDif1(protoDif1ImpPtr);
  delete protoDif1ImpPtr;  // free it from heap since dif constr clone's it's own copy
  protoDif1ImpPtr = NULL;

  // Create a scenario using the builder
  LgFrScenarioBuilderForATP builder1;
  LgFrScenarioDirector director1(&builder1);  
  director1.construct(protoDif1, calendar1);
  LgFrScenarioForATPSmartPointer testScenarioPtr1 = builder1.getScenario();

  // Make a deep copy of it
  LgFrScenarioForATPSmartPointer testScenarioDupPtr1
    = testScenarioPtr1.deepCopy();

  // Do some downcasts
  LgFrDataInterface * tsDifPtr1 
      = (LgFrDataInterface*) (testScenarioPtr1->dataInterface());

  LgFrDataInterfaceImplementation* tsDifImpPtr1
       = tsDifPtr1->dataInterfaceImplementation();

  LgFrDataInterface tsTestDif1(tsDifImpPtr1);
  LgFrDataInterface* tsTestDifPtr1 = &tsTestDif1;

  assert (tsDifPtr1 != NULL
          &&  tsDifPtr1->isA() == __LGFRDATAINTERFACE);

  assert(tsDifPtr1->dataInterfaceImplementation()->isA() 
	 == __LGFRDATAINTERFACEIMPLEMENTATION);
          
  LgFrDataInterface * tsDupDifPtr1 
      = (LgFrDataInterface*) (testScenarioDupPtr1->dataInterface());

  assert (tsDupDifPtr1 != NULL
          &&  tsDupDifPtr1->isA() == __LGFRDATAINTERFACE);

  assert(tsDupDifPtr1->dataInterfaceImplementation()->isA() 
	 == __LGFRDATAINTERFACEIMPLEMENTATION);

  LgFrDataInterfaceImplementation* tsDupDifImpPtr1
     = tsDupDifPtr1->dataInterfaceImplementation();

  LgFrDataInterface tsDupTestDif1(tsDupDifImpPtr1);
  LgFrDataInterface* tsDupTestDifPtr1 = &tsDupTestDif1;

  
  // Check for typo's in the downcasting code
  assert (tsTestDifPtr1 != tsDupTestDifPtr1);

  // And test the test dif
  difTestContextTest(
      *testScenarioPtr1, 
      testScenarioPtr1->lastProblemSolved(),
      *tsTestDifPtr1,
      *testScenarioDupPtr1, 
      *tsDupTestDifPtr1
      );


  // Test the LgFrDataInterfaceFromPWatp
  // ===================================== 
  LgFrCalendar calendar2(8, RWTime(RWDate("9/20/94")));
  LgFrDataInterfaceImplementation* protoDif2ImpPtr
      = createPWatpDifImp();
  protoDif2ImpPtr->title("internal data set pw2");
  registerPWatpDifImpFunctions(protoDif2ImpPtr);

  LgFrDataInterface protoDif2(protoDif2ImpPtr);
  delete protoDif2ImpPtr;
  protoDif2ImpPtr = NULL;

  // Create a scenario using the builder
  LgFrScenarioBuilderForATP builder2;
  LgFrScenarioDirector director2(&builder2);  
  director2.construct(protoDif2, calendar2);
  LgFrScenarioForATPSmartPointer testScenarioPtr2 = builder2.getScenario();

  // Shouldn't have to downcast.  Add a method in scenATP to get the ippATP
  LgFrInitialProblemForATP * testIpp2 = (LgFrInitialProblemForATP *)
                        &(testScenarioPtr2->initialProblemAndParameters());
 
  // Make a deep copy of it
  LgFrScenarioForATPSmartPointer testScenarioDupPtr2
    = testScenarioPtr2.deepCopy();

  // Do some downcasts
  LgFrDataInterface * tsDifPtr2 
      = (LgFrDataInterface*)(testScenarioPtr2->dataInterface());

  assert (tsDifPtr2 != NULL
          &&  tsDifPtr2->isA() == __LGFRDATAINTERFACE);

  assert(tsDifPtr2->dataInterfaceImplementation()->isA() 
	 == __LGFRDATAINTERFACEIMPLEMENTATION);

  LgFrDataInterfaceImplementation* tsDifImpPtr2
       = tsDifPtr2->dataInterfaceImplementation();

  LgFrDataInterface tsTestDif2(tsDifImpPtr2);
  LgFrDataInterface* tsTestDifPtr2 = &tsTestDif2;
          
  LgFrDataInterface * tsDupDifPtr2 
      = (LgFrDataInterface*)(testScenarioDupPtr2->dataInterface());

  assert (tsDupDifPtr2 != NULL
          &&  tsDupDifPtr2->isA() == __LGFRDATAINTERFACE);

  assert(tsDupDifPtr2->dataInterfaceImplementation()->isA() 
	 == __LGFRDATAINTERFACEIMPLEMENTATION);

  LgFrDataInterfaceImplementation* tsDupDifImpPtr2
     = tsDupDifPtr2->dataInterfaceImplementation();

  LgFrDataInterface tsDupTestDif2(tsDupDifImpPtr2);
  LgFrDataInterface* tsDupTestDifPtr2 = &tsDupTestDif2;

  // Check for typo's in the downcasting code
  assert (tsTestDifPtr2 != tsDupTestDifPtr2);

  // Check LgFrTestDataInterface
  // Context test for difPWatp
  difPWatpContextTest(
      *tsTestDifPtr2,
      *tsDupTestDifPtr2,
      &(*testScenarioPtr2)
     );


  // Check LgFrInitialProblemForATP
  // ===============================
  // Must re-create the scenario's because the dif's contextTest() methods
  // mucked around with the witRun's.
  LgFrCalendar calendar3;
  LgFrDataInterfaceImplementation* protoDif3ImpPtr
      = createDifTestImp();
  protoDif3ImpPtr->title("Test problem for LgFrDataInterfaceImplementation");
  registerDifTestImpFunctions(protoDif3ImpPtr);

  LgFrDataInterface protoDif3(protoDif3ImpPtr);
  delete protoDif3ImpPtr;
  protoDif3ImpPtr = NULL;

  // Create a scenario using the builder
  LgFrScenarioBuilderForATP builder3;
  LgFrScenarioDirector director3(&builder3);  
  director3.construct(protoDif3, calendar3);
  LgFrScenarioForATPSmartPointer testScenarioPtr3 = builder3.getScenario();
  LgFrInitialProblemForATP * testIpp3 = (LgFrInitialProblemForATP *)
                        &(testScenarioPtr3->initialProblemAndParameters());
  // Make a copy of it
  LgFrScenarioForATPSmartPointer testScenarioDupPtr3
    = testScenarioPtr3.deepCopy();

  // Do some downcasts
  LgFrDataInterface * tsDifPtr3 
      = (LgFrDataInterface*)(testScenarioPtr3->dataInterface());

  assert (tsDifPtr3 != NULL
          &&  tsDifPtr3->isA() == __LGFRDATAINTERFACE);

  assert(tsDifPtr3->dataInterfaceImplementation()->isA() 
	 == __LGFRDATAINTERFACEIMPLEMENTATION);

  LgFrDataInterfaceImplementation* tsDifImpPtr3
       = tsDifPtr3->dataInterfaceImplementation();

  LgFrDataInterface tsTestDif3(tsDifImpPtr3);
  LgFrDataInterface* tsTestDifPtr3 = &tsTestDif3;
          
  LgFrDataInterface * tsDupDifPtr3 
      = (LgFrDataInterface*)(testScenarioDupPtr3->dataInterface());

  assert (tsDupDifPtr3 != NULL
          &&  tsDupDifPtr3->isA() == __LGFRDATAINTERFACE);

  assert(tsDupDifPtr3->dataInterfaceImplementation()->isA() 
	 == __LGFRDATAINTERFACEIMPLEMENTATION);

  LgFrDataInterfaceImplementation* tsDupDifImpPtr3
     = tsDupDifPtr3->dataInterfaceImplementation();

  //  LgFrDataInterface tsDupTestDif3(tsDupDifImpPtr3->clone());
  LgFrDataInterface tsDupTestDif3(tsDupDifImpPtr3);
  LgFrDataInterface* tsDupTestDifPtr3 = &tsDupTestDif3;

  // Check for typo's in the downcasting code
  assert (tsTestDifPtr3 != tsDupTestDifPtr3);


   // Check LgFrInitialProblemForATP
  // ===============================
  // Must re-create the scenario's because the dif's contextTest() methods
  // mucked around with  the witRun's.
  // Now use difPWatp to test

  LgFrCalendar calendar4(8, RWTime(RWDate("7/15/94")));
  LgFrDataInterfaceImplementation* protoDif4ImpPtr
      = createPWatpDifImp();
  protoDif4ImpPtr->title("internal data set pw2");
  registerPWatpDifImpFunctions(protoDif4ImpPtr);
  LgFrDataInterface protoDif4(protoDif4ImpPtr);
  delete protoDif4ImpPtr;
  protoDif4ImpPtr = NULL;

  // Create a scenario using the builder
  LgFrScenarioBuilderForATP builder4;
  LgFrScenarioDirector director4(&builder4);  
  director4.construct(protoDif4, calendar4);

  LgFrScenarioForATPSmartPointer testScenarioPtr4 = builder4.getScenario();

  LgFrInitialProblemForATP * testIpp4 = (LgFrInitialProblemForATP *)
                        &(testScenarioPtr4->initialProblemAndParameters());

  // Make a deep copy of it
  LgFrScenarioForATPSmartPointer testScenarioDupPtr4
    = testScenarioPtr4.deepCopy();

  // Do some downcasts
  LgFrDataInterface * tsDifPtr4 
      = (LgFrDataInterface*)(testScenarioPtr4->dataInterface());

  assert (tsDifPtr4 != NULL
          &&  tsDifPtr4->isA() == __LGFRDATAINTERFACE);

  assert(tsDifPtr4->dataInterfaceImplementation()->isA() 
	 == __LGFRDATAINTERFACEIMPLEMENTATION);

  LgFrDataInterfaceImplementation* tsDifImpPtr4
       = tsDifPtr4->dataInterfaceImplementation();

  LgFrDataInterface tsTestDif4(tsDifImpPtr4);
  LgFrDataInterface* tsTestDifPtr4 = &tsTestDif4;

  LgFrDataInterface * tsDupDifPtr4 
      = (LgFrDataInterface*)(testScenarioDupPtr4->dataInterface());

  assert (tsDupDifPtr4 != NULL
          &&  tsDupDifPtr4->isA() == __LGFRDATAINTERFACE);

  assert(tsDupDifPtr4->dataInterfaceImplementation()->isA() 
	 == __LGFRDATAINTERFACEIMPLEMENTATION);

  LgFrDataInterfaceImplementation* tsDupDifImpPtr4
     = tsDupDifPtr4->dataInterfaceImplementation();

  LgFrDataInterface tsDupTestDif4(tsDupDifImpPtr4);
  LgFrDataInterface* tsDupTestDifPtr4 = &tsDupTestDif4;

  // Check for typo's in the downcasting code
  assert (tsTestDifPtr4 != tsDupTestDifPtr4);

  LgFrInitialProblemForATP::contextTest (*testIpp3, *testIpp4);

  // Test LgFrScheduleFactoryForATP
  LgFrScheduleFactoryForATP::contextTest 
      (*(testScenarioPtr4->scheduleFactory()));

  // Release smart pointers to schedules in the schedule factories so
  // that the scenario can be deleted when the current block exists.
#define releaseMacro(ptr)	\
  ( (LgFrScheduleFactory *)    /* Cast away const */	\
    (ptr->scheduleFactory()) )			\
    -> clearAndDestroy()
  releaseMacro(testScenarioPtr4);
  releaseMacro(testScenarioPtr3);
  releaseMacro(testScenarioPtr2);
  releaseMacro(testScenarioPtr1);
}
