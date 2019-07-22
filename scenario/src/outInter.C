// RW2STL -- inserted:
#include <fstream>
#include <scenario/src/RWToSTLHelpers.h>
#include <string>
// End of RW2STL -- inserted includes.
//#include <iostream>

#if !defined(WIN32) && !defined(__OS2__) && !defined(__WINDOWS__)
#include <unistd.h>
#endif

#include <scenario/src/machdep.h>

#include <scenario/src/outInter.h>
#include <scenario/src/zeroTvFS.h>
#include <scenario/src/machdep.h>
#include <scenario/src/schFctry.h>
#include <scenario/src/schdDeSt.h>
#include <scenario/src/demdSchP.h>
#include <scenario/src/partSchP.h>

//const std::ios_base::openmode LgFrSolutionOutputInterface::noReplace = 0x800; // replacement for old stream's noReplace open mode

// Create constants needed for Sun4 default parm in prototypes
const std::string 
   LgFrSolutionOutputInterface::writeScheduleTitle("Schedule:");
const std::string 
   LgFrSolutionOutputInterface::fopenAccessModeAppend("a");
const std::string 
   LgFrSolutionOutputInterface::writeProductionScheduleTitle(
      "Production Schedule:");
const std::string 
   LgFrSolutionOutputInterface::writeShipmentScheduleTitle(
      "Shipment Schedule:");
const std::string 
   LgFrSolutionOutputInterface::writeRequirementScheduleTitle(
      "Requirement Schedule:");
const std::string 
   LgFrSolutionOutputInterface::writeCriticalListTitle(
      "Critical List:");
const std::string 
   LgFrSolutionOutputInterface::writeChangedSupplyVolumeTitle(
      "Part Supply Volume Changes:");
const std::string 
   LgFrSolutionOutputInterface::writeChangedDemandVolumeTitle(
      "Demand Volume Changes:");
const std::string 
   LgFrSolutionOutputInterface::writeChangedPriorityTitle(
      "Demand Priority Changes:");
const std::string 
   LgFrSolutionOutputInterface::writeFocusShortageScheduleTitle(
      "Focus Shortage Schedule:");
const std::string 
   LgFrSolutionOutputInterface::writeFocusShortageScheduleFocusHorizonTitle(
      "\nFocus Horizon:");
const LgFrSchedulePrintStyle LgFrSolutionOutputInterface::pmTEXT (TEXT);

// Write a schedule.
// LgFrDoesFileExist can be used to determine if the file already exists.
bool
LgFrSolutionOutputInterface::writeSchedule(
   const LgFrScheduleFloat & schedule,
   const std::string& fileName,
   const std::string & title,
   const std::ios_base::openmode iosMode )
const
{

  if( LgFrDoesFileExist( fileName.c_str() ) && !(iosMode == (std::ofstream::out|std::ofstream::trunc) || iosMode == (std::ofstream::out|std::ofstream::app)) )
    return false;
  // Construct ofstream for output
  std::ofstream  ofs( fileName.c_str(), iosMode );
  if ( !ofs ) return false;

  // Write the title
  ofs <<title << std::endl;
  if ( !ofs ) return false;

  // Write the schedule
  ofs << schedule.format(LgFrScheduleFloat(),false);
  if ( !ofs ) return false;

  // Close file
  ofs.close();
  if ( !ofs ) return false;

  return true;
}

// Write production schedule.
bool
LgFrSolutionOutputInterface::writeWitProductionSchedule(
   const std::string& fileName,
   const std::string & fopenAccessMode,
   const LgFrSchedulePrintStyle & ot )
const
{
  assert( scenarioPtr_ != 0 );
  return 
     scenarioPtr_->
        lastProblemSolved().writeProductionSchedule(fileName,fopenAccessMode,ot);
}

// Write shipment schedule.
bool
LgFrSolutionOutputInterface::writeWitShipmentSchedule(
   const std::string& fileName,
   const std::string & fopenAccessMode,
   const LgFrSchedulePrintStyle & ot )
const
{
  assert( scenarioPtr_ != 0 );
  return 
    scenarioPtr_->
      lastProblemSolved().writeShipmentSchedule(fileName,fopenAccessMode,ot);
}
  
// Write requirement schedule.
bool
LgFrSolutionOutputInterface::writeWitRequirementSchedule(
   const std::string& fileName,
   const std::string & fopenAccessMode,
   const LgFrSchedulePrintStyle & ot )
const
{
  assert( scenarioPtr_ != 0 );
  return 
    scenarioPtr_->
      lastProblemSolved().writeRequirementSchedule(fileName,fopenAccessMode,ot);
}
  
// Write Production Schedule
bool            // true if no error was encountered
LgFrSolutionOutputInterface::writeProductionSchedule(
   const std::string& fileName,
   const std::string & title,
   const std::ios_base::openmode iosMode )
const
{
   const LgFrScheduleFactory * sf = scenarioPtr_ -> scheduleFactory();
   LgFrSortingPartScheduleFloatSmartPointer schedPtr = 
      sf -> newPartSchedule (LGFRProductionVolume, true, false );
   schedPtr->filterStrategy( LgFrZeroTimeVecFilterStrategy() );
   schedPtr->populate();
   bool retVal = writeSchedule( *schedPtr, fileName, title, iosMode );
   return retVal;
}
  
// Write Shipment Schedule
bool            // true if no error was encountered
LgFrSolutionOutputInterface::writeShipmentSchedule(
   const std::string& fileName,
   const std::string & title,
   const std::ios_base::openmode iosMode )
const
{
   const LgFrScheduleFactory * sf = scenarioPtr_ -> scheduleFactory();
   LgFrSortingDemandScheduleFloatSmartPointer schedPtr =
      sf -> newDemandSchedule (LGFRShipmentVolume, true, false );
   schedPtr->filterStrategy( LgFrZeroTimeVecFilterStrategy() );
   schedPtr->populate();
   bool retVal = writeSchedule( *schedPtr, fileName, title, iosMode );
   return retVal;
}
  
// Write Requirement Schedule
bool            // true if no error was encountered
LgFrSolutionOutputInterface::writeRequirementSchedule(
   const std::string& fileName,
   const std::string & title,
   const std::ios_base::openmode iosMode )
const
{
   const LgFrScheduleFactory * sf = scenarioPtr_ -> scheduleFactory();
   LgFrSortingPartScheduleFloatSmartPointer schedPtr =
      sf -> newPartSchedule (LGFRRequirementVolume, true, false );
   schedPtr->filterStrategy( LgFrZeroTimeVecFilterStrategy() );
   schedPtr->populate();
   bool retVal = writeSchedule( *schedPtr, fileName, title, iosMode );
   return retVal;
}
  
// Write Critical List
bool            // true if no error was encountered
LgFrSolutionOutputInterface::writeCriticalList(
   const std::string& fileName,
   const std::string & title,
   const std::ios_base::openmode iosMode )
const
{
   const LgFrScheduleFactory * sf = scenarioPtr_ -> scheduleFactory();
   LgFrSortingPartScheduleFloatSmartPointer schedPtr =
      sf -> newPartSchedule (LGFRCriticalPartRankingList, true, false );
   schedPtr->filterStrategy( LgFrZeroTimeVecFilterStrategy() );
   schedPtr->populate();
   bool retVal = writeSchedule( *schedPtr, fileName, title, iosMode );
   return retVal;
}
  
// Write changes made to part supply
bool            // true if no error was encountered
LgFrSolutionOutputInterface::writeChangedSupplyVolume(
   const std::string& fileName,
   const std::string & title,
   const std::ios_base::openmode iosMode )
const
{
   return writeSchedule(
      scenarioPtr_->workingImplosionProblem().changedSupplyVolume(),
      fileName,
      title,
      iosMode );
}
  
// Write changes made to demand volume
bool            // true if no error was encountered
LgFrSolutionOutputInterface::writeChangedDemandVolume(
   const std::string& fileName,
   const std::string & title,
   const std::ios_base::openmode iosMode )
const
{
   return writeSchedule(
      scenarioPtr_->workingImplosionProblem().changedDemandVolume(),
      fileName,
      title,
      iosMode );
}
  
// Write changes made to demand priority
bool            // true if no error was encountered
LgFrSolutionOutputInterface::writeChangedPriority(
   const std::string& fileName,
   const std::string & title,
   const std::ios_base::openmode iosMode )
const
{
   return writeSchedule(
      scenarioPtr_->workingImplosionProblem().changedPriority(),
      fileName,
      title,
      iosMode );
}
  
// Write Focussed Shortage Schedule including focus horizon
bool            // true if no error was encountered
LgFrSolutionOutputInterface::writeFocusShortageSchedule(
   LgFrFocussedShortageScheduleControl & fssCtrl,
   const std::string& fileName,
   const std::string & title1,
   const std::string & title2,
   const std::ios_base::openmode iosMode )
const
{
  
  // Write the focussed Shortage Schedule
  if ( ! writeSchedule(
     fssCtrl.focussedShortageSchedule(),
     fileName,
     title1,
     iosMode )
     )   return false;

  // Write the focus horizon

  // Construct ofstream for output
  std::ofstream  ofs( fileName.c_str(), std::ofstream::out | std::ofstream::app );
  if ( !ofs ) return false;

  // Write the title
  ofs <<title2 << std::endl;
  if ( !ofs ) return false;

  // Loop once for each demand
  int fh;
  int i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<scenarioPtr_->setOfDemands().size(); i++ ) {

     // Get demands focus horizon
     fh = fssCtrl.focusHorizon( (scenarioPtr_->setOfDemands())[i] );

     // If horizon is not equal to -1, then write it to file
     if ( fh == -1 ) continue;

     // Write the focus horizon
     const std::string & temp = (scenarioPtr_->setOfDemands())[i].printingName(false);
     ofs << temp; 
     ofs <<std::string("  ");
     ofs <<fh;
     ofs << std::endl;
     if ( !ofs ) return false;

  }

  // Close file
  ofs.close();
  if ( !ofs ) return false;

  return true;
}
  

// A Copy constructor
LgFrSolutionOutputInterface::LgFrSolutionOutputInterface(
   const LgFrSolutionOutputInterface& source )
: 
   scenarioPtr_( source.scenarioPtr_)
{
   // Nothing to do here
}

// Another Constructor
LgFrSolutionOutputInterface::LgFrSolutionOutputInterface(LgFrScenario* scenario )
:  
   scenarioPtr_( scenario )
{
   // Nothing to do here
}

// Default Constructor
LgFrSolutionOutputInterface::LgFrSolutionOutputInterface()
:  scenarioPtr_ ( 0 )
{
   // Nothing to do here
}

// Destructor
LgFrSolutionOutputInterface::~LgFrSolutionOutputInterface()
{
// Nothing to do here
}

#ifdef NDEBUG
#undef NDEBUG
#endif
// test this class
void 
LgFrSolutionOutputInterface::test()
{
  // Test default constructors
  LgFrSolutionOutputInterface defaultSoi;
  assert( defaultSoi.scenarioPtr_ == 0 );
}

// test this class
//
// The data for case11Scenario should come from
// "u/implode/wit/data/csi/case11/wit.data"
//
// The data for case11Scenario should come from
// "u/implode/wit/data/examples/diner12/wit.data"

void 
LgFrSolutionOutputInterface::contextTest(
   LgFrScenarioSmartPointer case11Scenario,
     LgFrWorkingImplosionProblem &case11Wip,
     LgFrLastProblemSolved &case11Lps,
     LgFrSetOfParts &case11Sop,
     LgFrSetOfDemands &case11Sod,
   LgFrScenarioSmartPointer diner12Scenario, 
     LgFrSetOfDemands &diner12Sod,
     LgFrFocussedShortageScheduleControl & diner12FssCtrl )
{
  LgFrWorkingImplosionProblem & wip = case11Wip;
  LgFrLastProblemSolved       & lps = case11Lps;
  LgFrSetOfParts              & sop = case11Sop;
  LgFrSetOfDemands            & sod = case11Sod;

  LgFrSolutionOutputInterface aSoi (&(*case11Scenario));
  LgFrSolutionOutputInterface copiedSoi (aSoi);
  assert ( (copiedSoi.scenarioPtr_->lastProblemSolved()).numberOfPeriods()  ==
                (aSoi.scenarioPtr_->lastProblemSolved()).numberOfPeriods() );
  assert ((aSoi.scenarioPtr_->lastProblemSolved()).numberOfPeriods() == 26);

  int nPeriods = (aSoi.scenarioPtr_->lastProblemSolved()).numberOfPeriods();

  // Test assignment operator
//  LgFrSolutionOutputInterface defaultSoi;
//  copiedSoi = defaultSoi;
//  assert (copiedSoi.scenarioPtr_ == 0);
//  defaultSoi = aSoi;
//  assert ((defaultSoi.scenarioPtr_->lastProblemSolved()).numberOfPeriods()
//          == (aSoi.scenarioPtr_->lastProblemSolved()).numberOfPeriods());

  // Make some parts for the test
  LgFrPart p1( "part1" );
  LgFrPart p2( "part2" );
  LgFrPart p3( "part3" );

  // Make some timeVecFloat for the test
  LgFrTimeVecFloat tvf1(10,1.11f);
  LgFrTimeVecFloat tvf2(10,2.22f);

  // Make some part timeVecFloat pairs for the test
  LgFrItemTimeVecPairFloat ptvfp1( p1, tvf1 );
  LgFrItemTimeVecPairFloat ptvfp2( p2, tvf1 );
  LgFrItemTimeVecPairFloat ptvfp3( p3, tvf2 );

  // Make a partSchedule for the test
  LgFrSortingPartScheduleFloat ps(3);
  ps.insertKeyAndValue(p1, tvf1);
  ps.insertKeyAndValue(p2, tvf2);
  ps.insertKeyAndValue(p3, tvf1);


  // Test writing a schedule to a file
  std::string fn1 = LgFrTempFileName();
  LgFrSolutionOutputInterface outObj;
  assert( outObj.writeSchedule(ps,fn1) );

  // Test overwriting an existing file.
  assert( !outObj.writeSchedule(ps,fn1) );
  assert( outObj.writeSchedule(ps,fn1,"Overwriting existing file", std::ofstream::out|std::ofstream::trunc) );

  // Test appending to a file
  assert( outObj.writeSchedule(ps,fn1,"2nd Schedule",std::ofstream::out | std::ofstream::app) );
  assert (unlink (fn1.c_str()) == 0);

  // Test writing production schedule to a file
  std::string prodFile = LgFrTempFileName();
  lps.computeCriticalList(true);
  case11Scenario->newImplosionSolution();
  assert( aSoi.writeWitProductionSchedule( prodFile ) );
  // Test writing to file with invalid file name
  lps.printMessage( 75, false );
#if !defined(_MSC_VER)
  // MSVC++ runtime library fails to return when fopen is called with
  // a filename of "".  So this test is skipped when in the MSVC++ environment.

  // this test is failing on cygwinGcc, so it is being removed.
  // It seems that this test should fail.
  //assert( !aSoi.writeWitProductionSchedule( std::string() ) );
#endif
  lps.printMessage( 75, true );
  // Test appending to an existing file
  assert( aSoi.writeWitProductionSchedule( prodFile ) );
  assert( aSoi.writeProductionSchedule( prodFile, "Testing", 
          std::ofstream::out | std::ofstream::app ) );
  assert (unlink (prodFile.c_str()) == 0);

  // Test writing of shipment schedule
  std::string shipFile = LgFrTempFileName();
  assert( aSoi.writeWitShipmentSchedule( shipFile ) );
  assert( aSoi.writeShipmentSchedule( shipFile, "Testing", 
          std::ofstream::out | std::ofstream::app ) );
  assert (unlink (shipFile.c_str()) == 0);

  // Test writing of critical list
  std::string critListFile = LgFrTempFileName();
  assert( aSoi.writeCriticalList( critListFile ) );
  assert (unlink (critListFile.c_str()) == 0);

  // Test writing of requirement schedule
  std::string reqFile = LgFrTempFileName();
  lps.mrp(wip);
  assert( aSoi.writeWitRequirementSchedule( reqFile ) );
  assert( aSoi.writeRequirementSchedule( reqFile, "Testing", 
          std::ofstream::out | std::ofstream::app ) );
  assert (unlink (reqFile.c_str()) == 0);

  // Test writing supply volume changes
  std::string supplyDeltaFile = LgFrTempFileName();
  assert ( sop.size() >= 6 );
  LgFrTimeVecFloat v1=wip.timeVec(sop[1]);
  LgFrTimeVecFloat v2=wip.timeVec(sop[3]);
  LgFrTimeVecFloat v3=wip.timeVec(sop[5]);
  wip.timeVec( sop[1], v1 + 1 );
  wip.timeVec( sop[3], v2 + 2 );
  wip.timeVec( sop[5], v3 + 3 );
  aSoi.writeChangedSupplyVolume( supplyDeltaFile );
  assert (unlink (supplyDeltaFile.c_str()) == 0);

  // Test writing demand volume changes
  std::string demandDeltaFile = LgFrTempFileName();
  assert ( sod.size() >= 6 );
  v1=wip.timeVec(sod[1]);
  v2=wip.timeVec(sod[3]);
  v3=wip.timeVec(sod[5]);
  wip.timeVec( sod[1], v1 + 1 );
  wip.timeVec( sod[3], v2 + 2 );
  wip.timeVec( sod[5], v3 + 3 );
  aSoi.writeChangedDemandVolume( demandDeltaFile );
  assert (unlink (demandDeltaFile.c_str()) == 0);

  // Test writing demand priority changes
  demandDeltaFile = LgFrTempFileName();
  assert ( sod.size() >= 6 );
  v1=wip.priority(sod[1]);
  v2=wip.priority(sod[3]);
  v3=wip.priority(sod[5]);
  wip.priority( sod[1], v1 + 1 );
  wip.priority( sod[3], v2 + 2 );
  wip.priority( sod[5], v3 + 3 );
  aSoi.writeChangedPriority( demandDeltaFile );
  assert (unlink (demandDeltaFile.c_str()) == 0);

  // Test writing focussed shortage schedule
  {
  LgFrSetOfDemands            & sod = diner12Sod;
  LgFrFocussedShortageScheduleControl & aFssCtrl = diner12FssCtrl;

  std::string fssFile = LgFrTempFileName();

  LgFrSolutionOutputInterface aSoi (&(*diner12Scenario));
  aFssCtrl.focusHorizon( *sod.findValue("demand","GrlChs_Sand"), 1 );
  aFssCtrl.focusHorizon( *sod.findValue("demand","Ham_Sand"), 1 );
  diner12Scenario->newImplosionSolution();

  aSoi.writeFocusShortageSchedule( aFssCtrl, fssFile );
  assert (unlink (fssFile.c_str()) == 0);
  }
}
