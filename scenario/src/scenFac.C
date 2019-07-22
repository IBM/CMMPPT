// ScenarioFacade  - This is an object to encapsilate a scenario, dif, ip,
//                   an mulitSchedules.
//                   The intent is to allow an application (GUI) using this
//                   framework to interface with a single object.  This will
//                   reduce the generallity of the framework, but may provide
//                   a simplied interface to the framework.
// 

#include <iostream.h>

/*#if defined (WIN32) || defined (__OS2__) || defined (__WINDOWS__)
#include <strstrea.h>
#else
#include <strstream.h>
#endif*/

#include <scenario/src/calendar.h>
#include <scenario/src/difPWcmr.h>
#include <scenario/src/difCmrpF.h>
#include <scenario/src/difImpl.h>
#include <scenario/src/scBdCMRP.h>
#include <scenario/src/scenCMRP.h>
#include <scenario/src/scenDrct.h>
#include <scenario/src/schdDeSt.h>
#include <scenario/src/multPSch.h>
#include <scenario/src/multDSch.h>
#include <scenario/src/scenFac.h>


// Perform implosion
void
LgFrScenarioFacade::newImplosionSolution()
{
   scenarioSPtr()->newImplosionSolution();
}


// newMultiSchedule
RWBoolean
LgFrScenarioFacade::newMultiSchedule(
  const RWCString multiScheduleName, const RWCString schedType )
{
  RWCollectableString * msnPtr = new RWCollectableString(multiScheduleName);
  
  // Ensure multiSchedule does not already exist
  if ( multiScheduleDictionary_.contains( msnPtr ) ) {
    cerr <<"LgFrError: LgFrScenarioFacade::newMultiSchedule" <<endl
         <<"           multiSchedule already exists. " <<endl
         <<"           mulitSchedule = " <<multiScheduleName <<endl;
    return FALSE;
  }
  
  LgFrMultiScheduleFloat * msPtr;
  RWCString typeUpperCase(schedType);
  typeUpperCase.toUpper();
  if ( typeUpperCase == "PART" )
    msPtr = new LgFrMultiPartScheduleFloat(scenarioSPtr());
  else if ( typeUpperCase == "DEMAND" ) 
    msPtr = new LgFrMultiDemandScheduleFloat(scenarioSPtr());
  else {
    cerr <<"LgFrError: LgFrScenarioFacade::newMultiSchedule" <<endl
         <<"           Type must be PART or DEMAND" <<endl
         <<"           Type = " <<schedType <<endl;
    return FALSE;                                                                    }

  multiScheduleDictionary_.insertKeyAndValue( msnPtr, msPtr );
  return TRUE;
}


// Return a list of vaild attributes which can be added to a multiSchedule.
LgFrVectorRWCString
LgFrScenarioFacade::supportedScheduleDescriptors(
  const RWCString schedType )
const
{
  LgFrOrderedVectorScheduleDescriptor sdv;
  RWCString typeUpperCase(schedType);
  typeUpperCase.toUpper();
  if ( typeUpperCase == "PART" )
    sdv = scenarioSPtr()->scheduleFactory()->partScheduleDescriptors();
  else if ( typeUpperCase == "DEMAND" ) {
    sdv = scenarioSPtr()->scheduleFactory()->demandScheduleDescriptors();
  }
  else {
    cerr <<"LgFrError: LgFrScenarioFacade::supportedMultiScheduleAttributes" <<endl
         <<"           Type must be PART or DEMAND" <<endl
         <<"           Type = " <<schedType <<endl;
    return FALSE;                                                                    }

  LgFrVectorRWCString retVal(sdv.entries());
  for ( int i=0; i<sdv.entries(); i++ ) {
    retVal[i] = sdv[i].name(scenarioSPtr()->nationalLanguage());
  }
  return retVal;
}

// add a schedule to a multiSchedule.
// returns TRUE if schedule created, otherwise false
RWBoolean
LgFrScenarioFacade::newSchedule(
  const RWCString multiScheduleName,
  const RWCString attributeName )
{
  LgFrMultiScheduleFloat * msPtr = findMultiSchedule( multiScheduleName );
  if ( msPtr == NULL ) return FALSE;
  LgFrScheduleDescriptor sd = scheduleDescriptor( attributeName );
  if ( sd == LgFrScheduleDescriptor() ) return FALSE;
  return msPtr -> newSchedule( sd );
}

// Find multiSchedule
LgFrMultiScheduleFloat *
LgFrScenarioFacade::findMultiSchedule(
  const RWCString multiScheduleName )
const
{
  RWCollectableString msn(multiScheduleName);
  LgFrMultiScheduleFloat * retVal =
    (LgFrMultiScheduleFloat *)multiScheduleDictionary_.findValue( &msn );
  if ( retVal == NULL ) {
    cerr <<"LgFrError: LgFrScenarioFacade::findMultiSchedule" <<endl
         <<"           multiSchedule does not exists. " <<endl
         <<"           mulitSchedule = " <<multiScheduleName <<endl;
  }
  return retVal;
}

// Find schedule descriptor in multischedule from string representation of
// the multiSchedule.
const LgFrScheduleDescriptor
LgFrScenarioFacade::findScheduleDescriptorInMultiSchedule(
  const RWCString multiScheduleName,
  const RWCString scheduleDescriptorString,
  RWBoolean & found)
const
{

  const LgFrScheduleDescriptor errorRetVal;
  found = FALSE;
  
  // Find multi schedule
  LgFrMultiScheduleFloat * msPtr = findMultiSchedule( multiScheduleName );
  if ( msPtr == NULL ) return errorRetVal;

  // Find schedule descriptor
  LgFrVectorScheduleDescriptor sdv = msPtr->schedDescVec();
  for ( int i=0; i<sdv.length(); i++ ) {
    if ( sdv[i].name(scenarioSPtr()->nationalLanguage())
         == scheduleDescriptorString ) {
      assert( sdv[i].isA() == __LGFRSCHEDULEDESCRIPTOR );
      found = TRUE;
      return sdv[i];
    }
  }
  for ( i=0; i<sdv.length(); i++ ) {
    if ( sdv[i].abbrev(scenarioSPtr()->nationalLanguage())
         == scheduleDescriptorString ) {
      assert( sdv[i].isA() == __LGFRSCHEDULEDESCRIPTOR );
      found = TRUE;
      return sdv[i];
    }
  }
  
  cerr <<"LgFrError: LgFrScenarioFacade::findScheduleDescriptorInMultiSchedule"
       <<endl
       <<"           Schedule Descriptor Not Found" <<endl
       <<"           scheduleDescriptor = " <<scheduleDescriptorString <<endl;
  return errorRetVal;

}


RWCString 
LgFrScenarioFacade::multiScheduleFormat( RWCString multiScheduleName ) const
{
  LgFrMultiScheduleFloat * msPtr = findMultiSchedule( multiScheduleName );
  if ( msPtr == NULL ) return RWCString();
  RWBoolean useGuiName = TRUE;
  LgFrSchedulePrintStyle style = CSV /* TEXT */ ;
  return msPtr->format( style, useGuiName );
}

void LgFrScenarioFacade::scenarioSPtr( const LgFrScenarioSmartPointer & scenSPtr )
{
  scenarioSPtr_ = scenSPtr;
}

// Change value in multiSchedule
RWBoolean
LgFrScenarioFacade::changeValueInMultiSchedule(
  const RWCString multiScheduleName,
  const RWCString scheduleDescriptor,
  const RWCString item,
  const int timeVecIndex,
  const float newValue )
{
  // Find multi schedule
  LgFrMultiScheduleFloat * msPtr = findMultiSchedule( multiScheduleName );
  if ( msPtr == NULL ) return FALSE;

  // Find schedule descriptor
  RWBoolean found;
  const LgFrScheduleDescriptor & sd =
    findScheduleDescriptorInMultiSchedule(multiScheduleName,
                                          scheduleDescriptor,
                                          found);
  if ( !found ) return FALSE;

  // Check to see if it is a valid change
  RWBoolean validChange =
    validValueChangeInMultiSchedule(multiScheduleName,
                                    scheduleDescriptor, item,
                                    timeVecIndex,
                                    newValue); 
  if ( !validChange ) {
    cerr <<"LgFrError: LgFrScenarioFacade::changeValueInMultiSchedule"
         <<endl
         <<"           Schedule can not be changed as requested" <<endl;
    return FALSE;
  }

  return msPtr->changeValueInSchedule(item, sd, timeVecIndex, newValue);
  
}

// valid value change in multiSchedule
RWBoolean
LgFrScenarioFacade::validValueChangeInMultiSchedule(
  const RWCString multiScheduleName,
  const RWCString scheduleDescriptor,
  const RWCString item,
  const int timeVecIndex,
  const float newValue )
const
{
  // Find multi schedule
  LgFrMultiScheduleFloat * msPtr = findMultiSchedule( multiScheduleName );
  if ( msPtr == NULL ) return FALSE;

  // Find schedule descriptor
  RWBoolean found;
  const LgFrScheduleDescriptor & sd =
    findScheduleDescriptorInMultiSchedule(multiScheduleName,
                                          scheduleDescriptor,
                                          found);
  if ( !found ) return FALSE;

  // Check to see if it is mutable
  RWBoolean isMutable =
    mutableAttributeInMultiSchedule( multiScheduleName,scheduleDescriptor ); 
  if ( !isMutable ) {
    cerr <<"LgFrError: LgFrScenarioFacade::validValueChangeInMultiSchedule"
         <<endl
         <<"           schedule descriptor is not mutable." <<endl
         <<"           scheduleDescriptor = " <<scheduleDescriptor <<endl;
    return FALSE;
  }

  // Get current time vec
  const LgFrTimeVecFloat * tvPtr = msPtr->getTimeVecFloat( item, sd );
  if ( tvPtr == NULL ) {
    cerr <<"LgFrError: LgFrScenarioFacade::validValueChangeInMultiSchedule"
         <<endl
         <<"           Item is not in multi schedule" <<endl
         <<"           item = " <<item <<endl;
    return FALSE;
  }
  LgFrTimeVecFloat tv( *tvPtr );

  // Modify time vec to proposed value
  tv[timeVecIndex] = newValue;

  // Test to see if new time vec is valid
  RWBoolean retVal;
  if ( INVALID != (msPtr->validity(sd,tv)) ) retVal = TRUE;
  else                                       retVal = FALSE;

  return retVal;
}

// Can an attribute value in a multi schedule be changed.
RWBoolean
LgFrScenarioFacade::mutableAttributeInMultiSchedule(
  const RWCString multiScheduleName,
  const RWCString scheduleDescriptor)
const
{
  // Find multi schedule
  LgFrMultiScheduleFloat * msPtr = findMultiSchedule( multiScheduleName );
  if ( msPtr == NULL ) return FALSE;
  
  // Find schedule descriptor
  RWBoolean found;
  const LgFrScheduleDescriptor & sd =
    findScheduleDescriptorInMultiSchedule(multiScheduleName,
                                          scheduleDescriptor,
                                          found);
  if ( !found ) return FALSE;
  assert( sd.isA() == __LGFRSCHEDULEDESCRIPTOR );
  
  return msPtr->isMutable(sd);
}

LgFrScenarioSmartPointer LgFrScenarioFacade::scenarioSPtr() const
{
  return scenarioSPtr_;
}

// Get schedule descriptor from schedule descriptor name
LgFrScheduleDescriptor
LgFrScenarioFacade::scheduleDescriptor(
  RWCString scheduleDescriptorName )
const
{
  LgFrScheduleDescriptor errorRetVal;

  LgFrOrderedVectorScheduleDescriptor sdv;
  int i;

  sdv = scenarioSPtr()->scheduleFactory()->partScheduleDescriptors();
  for ( i=0; i<sdv.entries(); i++ ){
    if ( sdv[i].name(scenarioSPtr()->nationalLanguage())
         == scheduleDescriptorName ) return sdv[i];
  }

  sdv = scenarioSPtr()->scheduleFactory()->demandScheduleDescriptors();
  for ( i=0; i<sdv.entries(); i++ ){
    if ( sdv[i].name(scenarioSPtr()->nationalLanguage())
         == scheduleDescriptorName ) return sdv[i];
  }
  
  cerr <<"LgFrError: LgFrScenarioFacade::scheduleDescriptor"
       <<endl
       <<"           Schedule Descriptor Not Supported" <<endl
       <<"           scheduleDescriptor = " <<scheduleDescriptorName <<endl;
  return errorRetVal;

}

// Default constructor
LgFrScenarioFacade::LgFrScenarioFacade()
:
  scenarioSPtr_(NULL),
  multiScheduleDictionary_()
{
  // nothing to do here
}

//Destructor
LgFrScenarioFacade::~LgFrScenarioFacade()
{
  // free all multischedules which were allocated by this object
  multiScheduleDictionary_.clearAndDestroy();
}


LgFrScenarioFacade::LgFrScenarioFacade( const LgFrScenarioFacade & source )
  :
  scenarioSPtr_(source.scenarioSPtr_),
  multiScheduleDictionary_(source.multiScheduleDictionary_)
{
  // Nothing to do here
  // THIS A PRIVATE MEMBER FUNCTION BECAUSE IT WILL NOT WORK CORRECTLY.
}

LgFrScenarioFacade &
LgFrScenarioFacade::operator=( const LgFrScenarioFacade &rhs )
{
  // THIS A PRIVATE MEMBER FUNCTION BECAUSE IT WILL NOT WORK CORRECTLY.
  if ( this!=&rhs ) {
    scenarioSPtr_ = rhs.scenarioSPtr_;
    multiScheduleDictionary_ = rhs.multiScheduleDictionary_;
  }
  return *this;
}

//----------------------------------------------------------------------------
//
// LgFrCmrpScenarioFacade
//
//----------------------------------------------------------------------------
void
LgFrCmrpScenarioFacade::scenarioForCmrpSPtr(
  const LgFrScenarioForCMRPSmartPointer & scenSPtr )
{
  scenarioSPtr_ = scenSPtr;
}

LgFrScenarioForCMRPSmartPointer LgFrCmrpScenarioFacade::scenarioForCmrpSPtr() const
{
  return LgFrScenarioFacade::scenarioSPtr();
}

// Default constructor
LgFrCmrpScenarioFacade::LgFrCmrpScenarioFacade()
:
  LgFrScenarioFacade()
{
  LgFrCalendar calendar;

#if 0
  // For now hard code dif from specific dif
  RWCString cmrpFileName = "/u/implode/fasano/cmrpData/bmw";
  // RWCString cmrpFileName = "/u/implode/fasano/cmrpData/auto2";
  calendar = 
    LgFrDataInterfaceImplementationFromCmrpFiles::calendar( cmrpFileName );
  LgFrDataInterfaceImplementationFromCmrpFiles difImp( cmrpFileName );
  registerCmrpFilesDifImpFunctions(&difImp);
  LgFrDataInterface dif(&difImp);
  LgFrScenarioBuilderForCMRPwithPrioritiesFromDif builder;

#else
  calendar = LgFrCalendar(8, RWTime(RWDate("8/01/95")));
  LgFrDataInterfaceImplementation* difImpPtr
        = createPWcmrpDifImp();
  registerPWcmrpDifImpFunctions(difImpPtr);
  LgFrDataInterface dif(difImpPtr);
  delete difImpPtr;
  difImpPtr = NULL;
  LgFrScenarioBuilderForCMRP builder;
#endif

  LgFrScenarioDirector director(&builder);
  director.construct(dif, calendar);

  scenarioSPtr( builder.getScenario() );

}


LgFrCmrpScenarioFacade::~LgFrCmrpScenarioFacade()
{
  // Nothing to do here
}


LgFrCmrpScenarioFacade::LgFrCmrpScenarioFacade( const LgFrCmrpScenarioFacade & source )
//:
  // LgFrScenarioFacade(source)
{
  // THIS A PRIVATE MEMBER FUNCTION BECAUSE IT WILL NOT WORK CORRECTLY.
  // Nothing to do here
}

LgFrCmrpScenarioFacade &
LgFrCmrpScenarioFacade::operator=( const LgFrCmrpScenarioFacade &rhs )
{
  if ( this!=&rhs ) {
    // (this->LgFrScenarioFacade::operator= ) (rhs);
  }
  return *this;
}

//#ifdef NDEBUG
//#undef NDEBUG
//#endif
// test LgFrCmrpScenarioFacade
void
LgFrCmrpScenarioFacade::test()
{
  // Test constructor
  LgFrCmrpScenarioFacade cmrpFacade;

  // Test multiSchedule Creation
  assert(  cmrpFacade.newMultiSchedule( "Demand", "Demand" ) );
  assert( !cmrpFacade.newMultiSchedule( "Demand", "Demand" ) );

  // cout <<cmrpFacade.multiScheduleFormat( "Demand" ) <<endl;

  // Test supported attributes method
  LgFrVectorRWCString partAttrib =
    cmrpFacade.supportedScheduleDescriptors("part");
  //for ( int i=0; i<partAttrib.length(); i++ ) cout <<partAttrib[i] <<endl;
  LgFrVectorRWCString demandAttrib =
    cmrpFacade.supportedScheduleDescriptors("demand");
  //for ( i=0; i<demandAttrib.length(); i++ ) cout <<demandAttrib[i] <<endl;
  assert( demandAttrib[1] = "CMRP Demand Volume" );
  assert( demandAttrib[2] = "CMRP Shipment Volume" );

  // For testing add a few schedules to this multischedule
  assert( cmrpFacade.newSchedule("Demand", demandAttrib[1] ) );
  assert( cmrpFacade.newSchedule("Demand", demandAttrib[2] ) );
  assert( cmrpFacade.newSchedule("Demand", demandAttrib[3] ) );
  assert( !cmrpFacade.newSchedule("Not a MS", demandAttrib[1] ) );
  assert( !cmrpFacade.newSchedule("Demand", "Not a valid attribute name" ) );
  
  // mutable method
  assert( cmrpFacade.mutableAttributeInMultiSchedule( "Demand",
                                                      demandAttrib[1] ) );
  assert( !cmrpFacade.mutableAttributeInMultiSchedule( "Demand",
                                                      demandAttrib[2] ) );
  assert( !cmrpFacade.mutableAttributeInMultiSchedule( "Demand",
                                                       "Not a SD") );
  assert( !cmrpFacade.mutableAttributeInMultiSchedule( "Not a MS",
                                                      demandAttrib[1] ) );

  // valid change method
  assert(
    cmrpFacade.validValueChangeInMultiSchedule(
                                     "Demand",
                                     demandAttrib[1],
                                     "Computer_2,\n  Robert_Oppenheimer",
                                     3,
                                     2000. )
    );
  assert(
    !cmrpFacade.validValueChangeInMultiSchedule(
                                      "Demand",
                                      demandAttrib[1],
                                      "Computer_2,\n  Robert_Oppenheimer",
                                      3,
                                      -2000. )
    );

  // Test new implosion solution
  cmrpFacade.newImplosionSolution();

  // Test change value in multischedule
  cmrpFacade.changeValueInMultiSchedule(
                              "Demand",
                              demandAttrib[1],
                              "Computer_2,\n  Robert_Oppenheimer",
                              3,
                              2000. );  
  
  cout <<cmrpFacade.multiScheduleFormat( "Demand" ) <<endl;
  
  
}
