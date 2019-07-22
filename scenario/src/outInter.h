#ifndef LGFR_IMPLOSION_OUTPUT_INTERFACE_H
#define LGFR_IMPLOSION_OUTPUT_INTERFACE_H
// RW2STL -- inserted:
#include <fstream>
#include <string>
// End of RW2STL -- inserted includes.

// This class writes an implosion solution to a file.

// Responsibilities are:
//   1. Write an implosion production schedule.
//   2. Write an implosion shipment schedule.
//   3. Write an implosion critical list.
//   4. Write a focussed shortage schedule (including the focus).
//   5. Write changes made to part supply and demand quantity and priority

// These responsibilities are met by using:
//   1. writeProductionSchedule. (2 methods)
//   2. writeShipmentSchedule. (2 methods)
//   3. writeCriticalList.
//   4. Write a focussed shortage schedule (including the focus).
//   5. writeChangedSupplyVolume writeChangedDemandVolume, 
//      writeChangedPriority



#include <scenario/src/partSchd.h>
#include <scenario/src/fssCtrl.h>
#include <scenario/src/scenario.h>


class LgFrSolutionOutputInterface  {

private:
  // Required to avoid "sorry, not implemented" messages on Sun
  static const std::string writeScheduleTitle;
  static const std::string fopenAccessModeAppend;
  static const std::string writeProductionScheduleTitle;
  static const std::string writeShipmentScheduleTitle;
  static const std::string writeRequirementScheduleTitle;
  static const std::string writeCriticalListTitle;
  static const std::string writeChangedSupplyVolumeTitle;
  static const std::string writeChangedDemandVolumeTitle;
  static const std::string writeChangedPriorityTitle;
  static const std::string writeFocusShortageScheduleTitle;
  static const std::string writeFocusShortageScheduleFocusHorizonTitle;
  static const LgFrSchedulePrintStyle pmTEXT;
public:
  friend class LgFrScenario;

  // Write a schedule.
  virtual
    bool            // true if no error was encountered
      writeSchedule(

         // part schedule to be written
         const LgFrScheduleFloat & schedule,

         // name of file to write  
         const std::string& fileName,

         // Title to be written as first records of file
         const std::string & title = 
            LgFrSolutionOutputInterface::writeScheduleTitle,

         // ofstream constructor mode.
         // Typical values are:
         //     std::ofstream::out | std::ofstream::app Append to an existing file.
         //     std::ofstream::out | std::ofStream::trunc OK to overwrite an existing file.
         const std::ios_base::openmode iosMode = std::ofstream::out 
                                      //   Use LgFrDoesFileExist() to determine
                                      //   existence of a file.
        )
      const;

  // Write output using WIT file formats.
  //
  // fileName: name of file to be written.
  // fopenAccessMode: fopen access mode used when opening file.
  // Typical values are:
  //     "a"          Append to an existing file.
  //     "w"          OK to overwrite an existing file.
  //
  // returns true if no error was encountered
  //
  // Use LgFrDoesFileExist() to determine existence of a file.
  virtual
    bool            // true if no error was encountered
      writeWitProductionSchedule(
         const std::string& fileName,
         const std::string & fopenAccessMode = 
            LgFrSolutionOutputInterface::fopenAccessModeAppend,
	 const LgFrSchedulePrintStyle & ot = pmTEXT)
      const;
  virtual
    bool            // true if no error was encountered
      writeWitShipmentSchedule(
         const std::string& fileName,
         const std::string & fopenAccessMode =             
            LgFrSolutionOutputInterface::fopenAccessModeAppend,
	 const LgFrSchedulePrintStyle & ot = pmTEXT)
      const;
  virtual
    bool            // true if no error was encountered
      writeWitRequirementSchedule(
         const std::string& fileName,
         const std::string & fopenAccessMode =             
            LgFrSolutionOutputInterface::fopenAccessModeAppend,
	 const LgFrSchedulePrintStyle & ot = pmTEXT)
      const;


  // Write methods which do not use WIT file formats.
  //
  // fileName: name of file to be written.
  // title : first record written to file.
  // iosMode : ofstream constructor mode.
  //    Typical values are:
  //        std::ofstream::out | std::ofstream::noreplace  Don't overwrite an existing file.
  //        std::ofstream::out | std::ofstream::app        Append to an existing file.
  //        std::ofstream::out                   OK to overwrite an existing file.
  //
  // returns true if no error was encountered
  //
  // Use LgFrDoesFileExist() to determine existence of a file.

  virtual
    bool            // true if no error was encountered
      writeProductionSchedule(
         const std::string& fileName,
         const std::string & title = 
            LgFrSolutionOutputInterface::writeProductionScheduleTitle,
         const std::ios_base::openmode iosMode = std::ofstream::app
         )
      const;
      // Note: This file does not include substitute usage

  virtual
    bool            // true if no error was encountered
      writeShipmentSchedule(
         const std::string& fileName,
         const std::string & title = 
            LgFrSolutionOutputInterface::writeShipmentScheduleTitle,
         const std::ios_base::openmode iosMode = std::ofstream::app
         )
      const;

  virtual
    bool            // true if no error was encountered
      writeRequirementSchedule(
         const std::string& fileName,
         const std::string & title = 
            LgFrSolutionOutputInterface::writeRequirementScheduleTitle,
         const std::ios_base::openmode iosMode = std::ofstream::app
         )
      const;

  virtual
    bool            // true if no error was encountered
      writeCriticalList(
         const std::string& fileName,
         const std::string & title = 
            LgFrSolutionOutputInterface::writeCriticalListTitle,
         const std::ios_base::openmode iosMode = std::ofstream::app
         )
      const;

  virtual
    bool            // true if no error was encountered
      writeChangedSupplyVolume(
         const std::string& fileName,
         const std::string & title = 
            LgFrSolutionOutputInterface::writeChangedSupplyVolumeTitle,
         const std::ios_base::openmode iosMode = std::ofstream::app
         )
      const;

  virtual
    bool            // true if no error was encountered
      writeChangedDemandVolume(
         const std::string& fileName,
         const std::string & title = 
            LgFrSolutionOutputInterface::writeChangedDemandVolumeTitle,
         const std::ios_base::openmode iosMode = std::ofstream::app
         )
      const;

  virtual
    bool            // true if no error was encountered
      writeChangedPriority(
         const std::string& fileName,
         const std::string & title = 
            LgFrSolutionOutputInterface::writeChangedPriorityTitle,
         const std::ios_base::openmode iosMode = std::ofstream::app
         )
      const;

  // Write Focus Shortage Schedule including focus horizon
  //
  // fileName: name of file to be written.
  // title1 : Record written for focus shortage schedule title.
  // title2 : Record written for focus horizon title.
  // iosMode : ofstream constructor mode.
  //    Typical values are:
  //        std::ofstream::out | std::ofstream::noreplace  Don't overwrite an existing file.
  //        std::ofstream::out | std::ofstream::app        Append to an existing file.
  //        std::ofstream::out                   OK to overwrite an existing file.
  //
  // returns true if no error was encountered
  //
  virtual
    bool            // true if no error was encountered
      writeFocusShortageSchedule(
         LgFrFocussedShortageScheduleControl & fssCtrl,
         const std::string& fileName,
         const std::string & title1 = 
            LgFrSolutionOutputInterface::writeFocusShortageScheduleTitle,
         const std::string & title2 = 
            LgFrSolutionOutputInterface::
               writeFocusShortageScheduleFocusHorizonTitle,
         const std::ios_base::openmode iosMode = std::ofstream::app
         )
      const;


  // Constructor
  LgFrSolutionOutputInterface();

  // Destructor
  ~LgFrSolutionOutputInterface();

  // test this class
  static void test();

  // The data for case11Scenario should come from
  // "u/implode/wit/data/csi/case11/wit.data"
  //
  // The data for case11Scenario should come from
  // "u/implode/wit/data/examples/diner12/wit.data"
  static void contextTest(
    LgFrScenarioSmartPointer case11Scenario,
      LgFrWorkingImplosionProblem &wip,
      LgFrLastProblemSolved &lps,
      LgFrSetOfParts &sop,
      LgFrSetOfDemands &sod,
    LgFrScenarioSmartPointer diner12Scenario,
     LgFrSetOfDemands &diner12Sod,
     LgFrFocussedShortageScheduleControl & diner12FssCtrl );

  //  static const std::ios_base::openmode noReplace;
private:

  LgFrScenario* const scenarioPtr_;

  // Assignment operator
  LgFrSolutionOutputInterface &
    operator=(
	      const LgFrSolutionOutputInterface& rhs);

  // A Copy constructor
  LgFrSolutionOutputInterface(
     const LgFrSolutionOutputInterface& source );

  // Another Constructor - should only be used by the Scenario constructor
  LgFrSolutionOutputInterface(
     LgFrScenario* scenario );

};

#endif
