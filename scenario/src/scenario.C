// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
#include <string>
#include <vector>
// End of RW2STL -- inserted includes.
#include <assert.h>

#include <scenario/src/machdep.h>
#include <scenario/src/scenario.h>
#include <scenario/src/fssCtrl.h>
#include <scenario/src/calendar.h>
#include <scenario/src/difSFile.h>
#include <scenario/src/scBdDflt.h>
#include <scenario/src/scenDrct.h>
#include <scenario/src/outInter.h>
#include <scenario/src/difEssSc.h>

std::vector<std::string> LgFrScenario::uniqueNameSet_; // This is used to make 

// Return the schedule factory which belongs to this scenario
const LgFrScheduleFactory*
LgFrScenario::scheduleFactory() const
{
  return (sfPtr_);
}

void
LgFrScenario::newImplosionSolution ()
{
  lps_.implode(wip_);
}

void
LgFrScenario::newExplosionSolution ()
{
  // do the explosion
  lps_.mrp(wip_);
};

// Writ WIT Data file.
// This method will overwrite and existing file.
void
LgFrScenario::writeWitDataFile(
   const std::string& fileName )
{
  lps_.writeWitDataFile( fileName, wip_, "w" );
}

// Return a non-const LgFrSetOfParts
LgFrSetOfParts&
LgFrScenario::setOfParts(int k)
{
  assert(k==LGFR_DEFAULT_SET || k==LGFR_UNIVERSAL_SET);
  return soParts_;
}

// Return a non-const LgFrSetOfDemands
LgFrSetOfDemands&
LgFrScenario::setOfDemands(int k)
{
  assert(k==LGFR_DEFAULT_SET || k==LGFR_UNIVERSAL_SET);
  return soDemands_;
}
// Return a const LgFrSetOfParts
const
LgFrSetOfParts&
LgFrScenario::constSetOfParts(int k)
const
{
  assert(k==LGFR_DEFAULT_SET || k==LGFR_UNIVERSAL_SET);
  return soParts_;
}

// Return a const LgFrSetOfDemands
const
LgFrSetOfDemands&
LgFrScenario::constSetOfDemands(int k)
const
{
  assert(k==LGFR_DEFAULT_SET || k==LGFR_UNIVERSAL_SET);
  return soDemands_;
}


// Return LgFrLastProblemSolved
LgFrLastProblemSolved&
LgFrScenario::lastProblemSolved()
{
  return lps_;
}

// Return LgFrLastProblemSolved
const LgFrLastProblemSolved&
LgFrScenario::lastProblemSolved()
  const
{
  return lps_;
}

// Return the WorkingImplosionProblem which belongs to this scenario
LgFrWorkingImplosionProblem&
LgFrScenario::workingImplosionProblem()
{
  return wip_;
}

const LgFrWorkingImplosionProblem&
LgFrScenario::workingImplosionProblem()
  const
{
  return wip_;
}

// Return LgFrImplode
LgFrImplode&
LgFrScenario::implode()
{
  return implode_;
}

// Return LgFrExplode
LgFrExplode&
LgFrScenario::explode()
{
  return explode_;
}

// Return LgFrUserInputModification
LgFrUserInputModification&
LgFrScenario::userInputModification()
{
  return userInputModification_;
}

// Return LgFrFocussedShortageScheduleControl
LgFrFocussedShortageScheduleControl&
LgFrScenario::focussedShortageScheduleControl()
{
  return fssControl_;
}

  // Return the dif which belongs to this scenario
LgFrDataInterfaceAbstract*
LgFrScenario::dataInterface()
     const
{
  assert (difPtr_ != 0);
  return difPtr_;
}
  
// Return the calendar which belongs to this scenario
LgFrCalendar&
LgFrScenario::calendar()
     const
{
  assert (calendarPtr_ != 0);
  return *calendarPtr_;
}
  
// Set/Get current national language setting of scenario
void
LgFrScenario::nationalLanguage(
 LgFrNationalLanguage lang )
{
  nationalLanguage_ = lang;
}

LgFrNationalLanguage
LgFrScenario::nationalLanguage()
     const
{
  return nationalLanguage_;
}

// ---------------------------------------------------------------------
// Return number of periods
// ---------------------------------------------------------------------
size_t
  LgFrScenario::numberOfPeriods()
    const
{
  return lps_.numberOfPeriods();
}

// Return a const ref of LgFrSolutionOutputInterface
const LgFrSolutionOutputInterface& 
LgFrScenario::solutionOutputInterface() const
{
  assert (soiPtr_ != 0);
  return *soiPtr_;
}


std::string
LgFrScenario::title()
     const
{
  return uniqueName_;
}

int
LgFrScenario::isValid()
     const
{
  // FINISH
  return difPtr_ != 0;
}

// Gets *ippPtr_
LgFrInitialProblemAndParameters&
LgFrScenario::initialProblemAndParameters ()
{
  assert (ippPtr_ != 0);
  return *ippPtr_;
}

// Sets ippPtr_
void
LgFrScenario::initialProblemAndParameters (LgFrInitialProblemAndParameters *ip)
{
  ippPtr_ = ip;
}

// Sets sfPtr_
void
LgFrScenario::scheduleFactory (LgFrScheduleFactory *scp)
{
  sfPtr_ = scp;
}


// Sets uniqueName_
void
LgFrScenario::uniqueName (std::string n)
{
  uniqueName_ = n;
}

// Constructor used by LgFrScenarioBuilderForDefault to build a scenario.
// Should only be used by LgFrScenarioBuilderForDefault.

LgFrScenario::LgFrScenario (const LgFrDataInterfaceAbstract&    dif,
                            const LgFrCalendar&             calendar)
: soParts_(0),
  soDemands_(0),
  calendarPtr_(calendar.clone()),
  nationalLanguage_( English ),
  difPtr_(dif.clone()),
  sfPtr_(0),
  ippPtr_(0),
  lps_(),
  wip_(lps_, soParts_, soDemands_),
  implode_(lps_),
  explode_(lps_),
  userInputModification_(wip_),
  fssControl_(implode_, soParts_, soDemands_),
  soiPtr_ (  new LgFrSolutionOutputInterface(this)  ),
  uniqueName_()
{
  // Rest of the initialization is done in the builder
}



// Destructor
LgFrScenario::~LgFrScenario()
{
  //assert (uniqueNameSet_.contains(&ts));
  //uniqueNameSet_.removeAndDestroy (&ts); // Puts this name up for grabs again
  size_t i = 0;
  for( i = 0; i < uniqueNameSet_.size(); i++ )
      if( uniqueNameSet_[i] == uniqueName_ )
          break;
  assert( uniqueNameSet_[i] == uniqueName_ );
  uniqueNameSet_.erase( uniqueNameSet_.begin()+i );

//  std::string *p0 = uniqueNameSet_.erase( &uniqueName_ );
//  std::string *p1 = uniqueNameSet_.end();

//  assert (uniqueNameSet_.erase( &uniqueName_ ) != uniqueNameSet_.end() );

  delete soiPtr_;
  delete ippPtr_;
  ippPtr_ = 0;
  delete sfPtr_;
  sfPtr_ = 0;
  delete difPtr_; 
  delete calendarPtr_;
//  soiPtr_ = 0;
//  difPtr_ = 0;
//  calendarPtr_ = 0;
}

// Assignment operator.
LgFrScenario&
LgFrScenario::operator=(
			const LgFrScenario& rhs)
{
    std::cerr << "should not use LgFrScenario::operator=(const LgFrScenario&)" << std::endl;
    assert(0);
/*
  if (rhs.isValid()  &&  uniqueName_ != rhs.uniqueName_) {
    soParts_ = rhs.soParts_;
    soDemands_ = rhs.soDemands_;
    delete calendarPtr_;
    calendarPtr_ = (rhs.calendarPtr_->clone());
    delete difPtr_;
    difPtr_ = (rhs.difPtr_)->clone(this);  // need a smart pointer here but can't get it
    delete sfPtr_;
    sfPtr_ = (rhs.sfPtr_)->clone(this);
    delete ippPtr_;
    ippPtr_ = (rhs.ippPtr_)->clone(this);
    lps_ = rhs.lps_;
    wip_.copyInPlace (rhs.wip_);
    // Do not change implode_.  It should keep subscribing to lps_.
    // Do not change explode_.  It should keep subscribing to lps_.
    // Do not change userInputModification_.  It should keep subscribing to wip_.
    fssControl_.copyInPlace (rhs.fssControl_);
    delete soiPtr_;
    soiPtr_ = new LgFrSolutionOutputInterface(this); 

    std::string ts(uniqueName_);
    assert (uniqueNameSet_.contains(&ts));
    uniqueNameSet_.removeAndDestroy (&ts); // Puts old name up for grabs again
    uniqueName_ = this->deriveUniqueName ();
  }
*/
  return *this;

}

// Copy constructor - deep copy
//  now IPP cloning happens with the ScenarioSmartPointer - should not
//   be called by anyone else
LgFrScenario::LgFrScenario(
			   const LgFrScenario& source
			   )
: soParts_(source.soParts_),
  soDemands_(source.soDemands_),
  calendarPtr_( source.calendarPtr_->clone() ),
  nationalLanguage_( source.nationalLanguage_ ),
  difPtr_( (source.difPtr_)->clone() ), 
  sfPtr_( source.sfPtr_ ), // a temporary hack to derive unique name.  the
                             //  SmartPointer::deepCopy() will fix up sfPtr_
  ippPtr_( source.ippPtr_ ), // a temporary hack to derive unique name.  the
                             //  SmartPointer::deepCopy() will fix up ippPtr_
  lps_(source.lps_),
  wip_(source.wip_, lps_, soParts_, soDemands_),
  implode_(lps_),
  explode_(lps_),
  userInputModification_(wip_),
  fssControl_(source.fssControl_, implode_, soParts_, soDemands_),
  soiPtr_(  new LgFrSolutionOutputInterface(this)  ),
  uniqueName_()
{
  uniqueName_ = this->deriveUniqueName ();
  sfPtr_ = 0;
  ippPtr_ = 0;
}

// Default Constructor
LgFrScenario::LgFrScenario()
: soParts_(0),
  soDemands_(0),
  calendarPtr_(0),
  nationalLanguage_(English),
  difPtr_(0),
  sfPtr_(0),
  ippPtr_(0),
  lps_(),
  wip_(lps_, soParts_, soDemands_),
  implode_(lps_),
  explode_(lps_),
  userInputModification_(wip_),
  fssControl_(implode_,soParts_,soDemands_),
  soiPtr_ (0),
  uniqueName_()
{
  uniqueName_ = this->deriveUniqueName ();
}

std::string
LgFrScenario::deriveUniqueName()
     const
{
  std::string baseName;
  LgFrDataInterfaceEssentialsFromScenario difEssen(this);
  if (! this->isValid())
      baseName = "noname";
  else
      baseName = ippPtr_->title(&difEssen);
  std::string trialName (baseName);
  // Find unique name for *this
  int sequenceNumber = 0; 
  bool foundUnique = false;
  while( !foundUnique )
  {
    trialName = baseName;
    if( sequenceNumber++ )
    {
        char str[10];
        sprintf (str, ":%i", sequenceNumber);
        trialName += str;
    }
    sce::strip( trialName );
    size_t index0 = 0;
    for( index0 = 0; index0 < uniqueNameSet_.size(); index0++ )
        if( uniqueNameSet_[index0] == trialName )
            break;

    foundUnique = ( index0 == uniqueNameSet_.size() ? true : false );
  }

/*if (uniqueNameSet_.contains (&trialName)) {
    int i = 1; 
    while (uniqueNameSet_.contains (&trialName)) {
      ++i;
      char String[5];
      sprintf (String, "%-i", i);
      std::string tempName (baseName + ":" + String);
      trialName = tempName.strip(std::string::both);
    }
  }*/
  uniqueNameSet_.push_back (trialName);
  return trialName;
}



#ifdef NDEBUG
#undef NDEBUG
#endif

// Self-test
void
LgFrScenario::test()
{

  // Make objects needed by the tests
  LgFrDataInterfaceSpecialPurposeFromFile
    simpleProb ("../../../../wit/wit-data/wally/simple/wit.data");
  LgFrDataInterfaceSpecialPurposeFromFile
    diner12Prob ("../../../../wit/wit-data/examples/diner12/wit.data");
  LgFrDataInterfaceSpecialPurposeFromFile
    obj1Prob ("../../../../wit/wit-data/standard/obj1/wit.data");
  LgFrDataInterfaceSpecialPurposeFromFile
    brendaDeniseProb ("../../../../wit/wit-data/brenda/denise/wit.data");
  LgFrDataInterfaceSpecialPurposeFromFile
    csiCase11Prob ("../../../../wit/wit-data/csi/case11/wit.data");

  LgFrCalendar calInst;
  LgFrScenarioBuilderForDefault builder;
  LgFrScenarioDirector director(&builder);

  // Create an instance using the builder
  director.construct(simpleProb, calInst);
  LgFrScenarioSmartPointer testScenarioPtr = builder.getScenario();
  LgFrScenarioSmartPointer simpleScenario = testScenarioPtr;
  assert(simpleScenario->soParts_.size() == 7);
  assert(simpleScenario->soDemands_.size() == 3);
  
  // Test the default and deep copy constructors
  LgFrScenarioSmartPointer nullScenario(new LgFrScenario);

  LgFrScenarioSmartPointer simpleScenarioDup = simpleScenario.deepCopy();

  LgFrScenarioSmartPointer assignee(new LgFrScenario);
  assert(&(simpleScenarioDup->soParts_) != &(simpleScenario->soParts_));
  assert((simpleScenarioDup->dataInterface())!= (simpleScenario->dataInterface()));
  assert(simpleScenarioDup->soParts_.size() == 7);
  assert(simpleScenarioDup->soDemands_.size() == 3);
  assert(simpleScenarioDup->initialProblemAndParameters().title() == 
            simpleScenario->initialProblemAndParameters().title() ); 
  assert(nullScenario->title() != assignee->title());
  assert(nullScenario->title() != simpleScenario->title());
  assert(nullScenario->title() != simpleScenarioDup->title());
  assert(simpleScenario->title() != simpleScenarioDup->title());


  // Test the assignment operator - shallow copy
  assignee = simpleScenario;
  assert(assignee->soParts_.size() == 7);
  assert(assignee->soDemands_.size() == 3);

  // Make sure LgFrInitialProblemFromFiles clone method worked correctly
  assert(      assignee->initialProblemAndParameters().title() == 
         simpleScenario->initialProblemAndParameters().title() ); 

  // Test the isValid method (and the result of the previous operations
  assert (! ( nullScenario->isValid() ) );
  assert (simpleScenario->isValid());
  assert (simpleScenarioDup->isValid());
  assert (assignee->isValid());
 
  // Test the heuristic implosion method on some input data
 LgFrDataInterfaceSpecialPurposeFromFile  
    bigProb ("../../../../wit/wit-data/brenda/cmos26/wit.data");
  director.construct(bigProb, calInst);  // create and initialize
  LgFrScenarioSmartPointer bigScenarioPtr = builder.getScenario();
  LgFrScenario& bigScenario = *bigScenarioPtr;

  bigScenario.wip_.implosionMethod(LgFrHEURISTIC); // set implosion method
  bigScenario.newImplosionSolution();		// run implosion
  // find a demand
  const LgFrDemand  *adp = bigScenario.soDemands_.findValue("I/P_97N", 
	  "0000055F5206");
  LgFrTimeVecFloat shipQ 			// get some shipment data
    (bigScenario.lps_.shipmentVolume(adp));
  assert (3826.9 < shipQ[1]  &&  shipQ[1] < 3827.1);	// it should be 3827.0
  assert (shipQ[0] == 0  && shipQ[2] == 0);

  // Test lastProblemSolved()
  // by invoking a member funciton of LgFrLastProblemSolved using the
  // the object returned by scenario's member function lastProblemSolved().
  assert ( ( bigScenario.lastProblemSolved() ).numberOfPeriods() == 27 );
  {
     LgFrLastProblemSolved & lps = bigScenario.lastProblemSolved();
     lps.computeCriticalList(true);
  }

  // Test setOfParts()
  assert( ( bigScenario.setOfParts() ).size() == 256);

  // Test setOfDemands()
  assert( ( bigScenario.setOfDemands() ).size() == 319);

  // Test workingImplosionProblem()
  {
    LgFrWorkingImplosionProblem & wip = bigScenario.workingImplosionProblem();
    director.construct (obj1Prob, calInst);  // create and initialize
    LgFrScenarioSmartPointer obj1p = builder.getScenario();
    LgFrWorkingImplosionProblem::contextTest (obj1p->wip_, obj1p->soParts_,
                                              obj1p->soDemands_);
  }



  // Test writeWitDataFile()
  {
     std::string witFile = LgFrTempFileName();
     director.construct (diner12Prob, calInst); 
     LgFrScenarioSmartPointer scenPtr = builder.getScenario();
     scenPtr->writeWitDataFile( witFile );
     // assert (unlink (witFile.c_str()) == 0);
  }

// works up to here

  // Test LgFrLastProblemSolved
  {
    director.construct (brendaDeniseProb, calInst);  // create and initialize
    LgFrScenarioSmartPointer bdp = builder.getScenario();
    LgFrLastProblemSolved::contextTest( bdp->wip_, bdp->lps_, 
                              bdp->soParts_, bdp->soDemands_ );
  }

  // Test LgFrImplode
  {
    director.construct (diner12Prob, calInst);  // create and initialize
    LgFrScenarioSmartPointer d12p = builder.getScenario();
    LgFrImplode::contextTest (d12p->lps_, d12p->soParts_);
  }

  // Test LgFrExplode
  {
    director.construct (diner12Prob, calInst);  // create and initialize
    LgFrScenarioSmartPointer d12p = builder.getScenario();
    LgFrExplode::contextTest (d12p->lps_, d12p->soParts_);
  }

  // Test UserInputModification
  {
    director.construct (diner12Prob, calInst);  // create and initialize
    LgFrScenarioSmartPointer d12p = builder.getScenario();
    LgFrUserInputModification::contextTest (d12p->wip_);
  }





  // Test LgFrFocussedShortageScheduleControl
  {
    director.construct (diner12Prob, calInst);  // create and initialize
    LgFrScenarioSmartPointer d12p = builder.getScenario();

    // do a deep copy here
    LgFrScenarioSmartPointer dupScen = d12p.deepCopy();
    LgFrFocussedShortageScheduleControl::contextTest
      (*d12p, d12p->wip_, d12p->lps_, d12p->soParts_, d12p->soDemands_,
       d12p->fssControl_,
       *dupScen, dupScen->fssControl_);
  }



  // Test LgFrScheduleFactory and LgFrSolutionOutputInterface
  {
    director.construct (diner12Prob, calInst);  // create and initialize
    LgFrScenarioSmartPointer d12p = builder.getScenario();

    LgFrScheduleFactory::contextTest(*(d12p->sfPtr_), d12p);

    director.construct (csiCase11Prob, calInst);  // create and initialize
    LgFrScenarioSmartPointer cc11p = builder.getScenario();

    LgFrSolutionOutputInterface::contextTest ( 
       cc11p, cc11p->wip_, cc11p->lps_, cc11p->soParts_, cc11p->soDemands_,
       d12p, d12p->soDemands_, d12p->fssControl_ );

    // Release smart pointers to schedules in the schedule factories so
    // that the scenario can be deleted when the current block exists.
    d12p -> sfPtr_ -> clearAndDestroy();
  }

}

