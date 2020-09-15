//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef ObjectiveH
#define ObjectiveH

//------------------------------------------------------------------------------
// Header file: "Objective.h"
//
// Contains the declaration of class Objective.
//------------------------------------------------------------------------------

#include <Assoc.h>
#include <Str.h>

//------------------------------------------------------------------------------
// Class Objective
//
// Represents on objective function for multiple objectives mode.
// An instance of this class only contains the global information about the
// represented objective function.
//
// Implemented in MultiObj.C.
//------------------------------------------------------------------------------

class WitObjective: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitObjective (WitMultiObjMgr *, const char * theObjName);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitObjective ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void copyAttrsFrom (WitObjective * theObjective);
         //
         // Copies the input attributes of theObjective into this Objective.

      void writeDataAttrs ();
         //
         // Writes the attributes of this Objective to an input data file.

      void writeObjNameAsCurrent ();
         //
         // Writes myObjName_ (to an input data file) as the current Objective.

      void display ();
         //
         // Displays the input and preprocessing data for this Objective.

      static bool containsVBar (const char * theCstring);
         //
         // Returns true, iff theCstring contains a vertical bar character.

      void storeOptObjValue (double);
         //
         // Sets objValue for this Objective to the argument.
         // Valid only when the optimizing implosion subsystem is uploading the
         // solution.

      bool hasNegativeCosts ();
         //
         // Returns true, iff there is at least one cost coefficient in this
         // Objective that's < 0.

      //------------------------------------------------------------------------
      // Mapping functions
      //------------------------------------------------------------------------

      decMappingIndex (WitObjective)

      static int nInstances (WitProblem *);

      //------------------------------------------------------------------------
      // Default value functions.
      //------------------------------------------------------------------------

      int defObjectiveSeqNo ();

      //------------------------------------------------------------------------
      // "Set" functions
      //------------------------------------------------------------------------

      void setObjectiveSeqNo (int theSeqNo);

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (const WitString &, myObjName)
      accessFunc (int,               objectiveSeqNo)
      accessFunc (double,            objValue)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitObjective);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitMultiObjMgr * const myMultiObjMgr_;
         //
         // The MultiObjMgr that owns this Objective.

      int mappingIndex_;
         //
         // Mapping index: see Mapping.h.
         // == # Objectives constructed before this Objective.

      const WitString myObjName_;
         //
         // The name of this Objective.

      int objectiveSeqNo_;
         //
         // The sequence number for this objective.

      double objValue_;
         //
         // The objective function value of this Objective.
   };

#endif
