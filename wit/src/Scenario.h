//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef ScenarioH
#define ScenarioH

//------------------------------------------------------------------------------
// Header file: "Scenario.h"
//
// Contains the declaration of class Scenario.
//------------------------------------------------------------------------------

#include <Assoc.h>

//------------------------------------------------------------------------------
// Class Scenario
//
// Represents a scenario for stochastic implosion.
//------------------------------------------------------------------------------

class WitScenario: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitScenario (WitProblem *, int theIdx);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitScenario ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void copyAttrsFrom (WitScenario * thatScenario);
         //
         // Copies the input attributes stored by thatScenario into this
         // Scenario.

      void writeData ();
         //
         // Writes the section of the input data file for this Scenario.

      void writeMyIndex ();
         //
         // writeData for myIndex_.

      //------------------------------------------------------------------------
      // Default value functions.
      //------------------------------------------------------------------------

      double defProbability ();

      //------------------------------------------------------------------------
      // "Set" functions.
      //------------------------------------------------------------------------

      void setProbability (double);

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (int,    myIndex)
      accessFunc (double, probability)

      //------------------------------------------------------------------------
      // Mapping functions
      //------------------------------------------------------------------------

      inline int mappingIndex () const
         {
         return myIndex_;
         }
         //
         // Mapping index: See Mapping.h.

      inline static void verifyMappingIndex (WitScenario * &)
         {
         }
         //
         // See decMappingIndex in macs.h.

      static int nInstances (WitProblem *);

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      void writeProbability ();
         //
         // writeData for probability_.

      noCopyCtorAssign (WitScenario);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      const int myIndex_;
         //
         // The index of this Scenario_;

      double probability_;
         //
         // The probability of this Scenario.
   };

#endif
