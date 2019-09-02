//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef MultiObjMgrH
#define MultiObjMgrH

//------------------------------------------------------------------------------
// Header file: "MultiObjMgr.h"
//
// Contains the declaration of class MultiObjMgr.
//------------------------------------------------------------------------------

#include <Assoc.h>
#include <List.h>
#include <Str.h>

//------------------------------------------------------------------------------
// Class MultiObjMgr
//
// Responsible for managing multiple objectives mode.
//
// Implemented in MultiObj.C.
//------------------------------------------------------------------------------

class WitMultiObjMgr: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitMultiObjMgr (WitProblem *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitMultiObjMgr ();

      //------------------------------------------------------------------------
      // Default value functions.
      //------------------------------------------------------------------------

      defaultValue (const char * defObjectiveListSpec, "Default")

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void copyAttrsFrom (WitMultiObjMgr * theMultiObjMgr);
         //
         // Copies the input attributes of theMultiObjMgr into this MultiObjMgr.

      void writeDataAttrs ();
         //
         // Writes the attributes of this MultiObjMgr to an input data file.

      void writeMultiObjVecs ();
         //
         // Writes the ObjVec attributes to an input data file.

      void display ();
         //
         // Displays the input and preprocessing data for this MultiObjMgr.

      WitObjective * findObjective (const char * theObjName);
         //
         // Finds and returns the Objective belonging to this MultiObjMgr whose
         // name matches theObjName, if any; otherwise returns NULL.

      void validateData ();
         //
         // Verifies that the input data is valid for optimizing implosion in
         // multiple objectives mode.
         // Specifically, this function verifies that the objectiveSeqNos of the
         // Objectives are unique.

      void writeObjValues ();
         //
         // Writes the objValue attribute for each Objective to the message file

      //------------------------------------------------------------------------
      // "Set" functions
      //------------------------------------------------------------------------

      void setObjectiveList (const WitVector <const char *> & theObjNames);
         //
         // Deletes the current Objectives and creates a new set of Objectives,
         // one for each objective name in theObjNames.

      void setCurrentObjective (WitObjective *);

      void setMultiObjTol (double);

      //------------------------------------------------------------------------
      // Default value functions.
      //------------------------------------------------------------------------

      defaultValue (double defMultiObjTol, 1.0e-7)

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (const WitList <WitObjective> &, myObjectives)
      accessFunc (WitObjective *,                 currentObjective)
      accessFunc (double,                         multiObjTol)

      void getObjectiveListSpec (WitString & theObjListSpec);
         //
         // Sets theObjListSpec to the String that specifies the list of
         // Objectives owned by this MultiObjMgr.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      void clearObjectives ();
         //
         // Deletes the contents of myObjectives_ and clears it.

      void checkUniqueSeqNos ();
         //
         // Verifies that the objectiveSeqNos of the Objectives are unique.

      noCopyCtorAssign (WitMultiObjMgr);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitList <WitObjective> myObjectives_;
         //
         // The Objectives owned by this MultiObjMgr.

      WitObjective * currentObjective_;
         //
         // The currently selected Objective.

      double multiObjTol_;
         //
         // Each objective that has already been optimized is constrained to
         // achieve its maximum value minus a tolerance based on multiObjTol_.
   };

#endif
