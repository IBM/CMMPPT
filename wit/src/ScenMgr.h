//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef ScenMgrH
#define ScenMgrH

//------------------------------------------------------------------------------
// Header file: "ScenMgr.h"
//
// Contains the declaration of class ScenMgr.
//------------------------------------------------------------------------------

#include <StochAssoc.h>
#include <List.h>
#include <PtrVec.h>

//------------------------------------------------------------------------------
// Class ScenMgr
//
// "Scenario Manager"
// Owns and maintains the Scenarios for stochastic implosion.
//
// Class hierarchy:
//
//    ProbAssoc
//       StochAssoc
//          ScenMgr
//
// Implemented in Scenario.C.
//------------------------------------------------------------------------------

class WitScenMgr: public WitStochAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitScenMgr (WitStochModeMgr *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitScenMgr ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void copyAttrsFrom (WitScenMgr * theScenMgr);
         //
         // Copies the input attributes of theScenMgr into this ScenMgr.

      void writeUndiffData ();
         //
         // Writes the section of the input data file for the undifferentiated
         // scenario-specific attributes.

      void writeDiffData ();
         //
         // Writes the section of the input data file for the differentiated
         // scenario-specific attributes.

      void displayGlobalData ();
         //
         // Displays the global input and preprocessing data for this ScenMgr.

      void displayData ();
         //
         // Does displayData for each Scenario.

      void validateData ();
         //
         // Verifies that the Scenarios are valid for a stochastic implosion.
         // Specifically, verifies that the of the Scenario probabilities is 0.

      //------------------------------------------------------------------------
      // "Set" functions.
      //------------------------------------------------------------------------

      void setCurScenario (WitScenario *);
         //
         // Sets the current Scenario to the arguement.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (const WitList <WitScenario> &, myScenarios)
      accessFunc (WitScenario *,                 curScenario)
      accessFunc (bool,                          isWritingData)

      inline WitScenario * myScenarioFor (int theIdx)
         {
         return myScenarioFor_[theIdx];
         }

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitScenMgr);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitStochModeMgr * const myStochModeMgr_;
         //
         // The StochModeMgr that owns this ScenMgr.

      const int nScenarios_;
         //
         // The number of Scenarios owned by this ScenMgr.

      WitList <WitScenario> myScenarios_;
         //
         // This List of all Scenarios owned by this ScenMgr.

      WitPtrVec <WitScenario> myScenarioFor_;
         //
         // myScenarioFor_[theIdx] is the Scenario owned by this ScenMgr
         // whose index is theIdx.

      WitScenario * curScenario_;
         //
         // The current Scenario.

      bool isWritingData_;
         //
         // True, iff this ScenMgr is currently writing input data.
   };

#endif
