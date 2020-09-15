//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef StochImpMgrH
#define StochImpMgrH

//------------------------------------------------------------------------------
// Header file: "StochImpMgr.h"
//
// Contains the declaration of class StochImpMgr.
//------------------------------------------------------------------------------

#include <Assoc.h>

//------------------------------------------------------------------------------
// Class StochImpMgr
//
// "Stochastic Implosion Manager"
// The interface to the stochastic implosion subsystem.
//------------------------------------------------------------------------------

class WitStochImpMgr: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitStochImpMgr (WitProblem *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitStochImpMgr ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void copyAttrsFrom (WitStochImpMgr * theStochImpMgr);
         //
         // Copies the input attributes of theStochImpMgr into this StochImpMgr.

      void writeData ();
         //
         // Writes the section of the input data file for this StochImpMgr.

      bool skipWriteData (const char * theAttName, WitComponent * theComp);
         //
         // Returns true, iff writeData for the attribute whose name is given by
         // theAttName is to be skipped for theComp.

      void displayGlobalData ();
         //
         // Displays the global input and preprocessing data for this
         // StochImpMgr.

      //------------------------------------------------------------------------
      // Default value functions.
      //------------------------------------------------------------------------

      defaultValue (bool defStageByObject,  false)
      defaultValue (int  defNScenarios,     1)
      defaultValue (bool defStochMode,      false)
      defaultValue (int  defPeriodStageIdx, 0)

      //------------------------------------------------------------------------
      // "Set" functions.
      //------------------------------------------------------------------------

      void setStageByObject (bool);
      void setNScenarios    (int);
      void setStochMode     (bool);

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (bool, stageByObject)
      accessFunc (int,  nScenarios)
      accessFunc (bool, stochMode)

      //------------------------------------------------------------------------
      // The following functions are only valid in stochastic mode.
      //------------------------------------------------------------------------

      void prepSetStochModeAttr (const char * theAttName);
         //
         // Takes action appropriate just before the value of the attribute
         // whose name is given by theName is to be set.
         // The attribute is allowed to be set in stochastic mode.
         // The attribute must not be a scenario-specific attribute associated
         // with either a DelComp or a BoundSet.

      void prepSetScenSpecAttr (
            const char * theAttName,
            WitDelComp * theDelComp);
         //
         // Takes action appropriate just before the value of the
         // scenario-specific attribute whose name is given by theName is to be
         // set for theDelComp.

      void prepSetBoundSet (WitBoundSet * theBoundSet);
         //
         // Takes action appropriate just before the value of theBoundSet is to
         // be set.

      bool isScenSpec (const char * theAttName);
         //
         // Returns true, iff theAttName is the name of a scenario-specific
         // attribute.

      bool canSetInStochMode (const char * theAttName);
         //
         // Returns true, iff theAttName is the name of an attribute that can be
         // set in stochastic mode.

      bool canSetInStochSolnMode (const char * theAttName);
         //
         // Returns true, iff theAttName is the name of an attribute that can be
         // set in stochastic solution mode.

      bool uploadingData ();
         //
         // Returns true, iff the stochastic implosion subsystem is currently
         // uploading scenario-specific data to the owning Components.

      void displayDataStochMode ();
         //
         // Does portion of displayData specific to stochastic mode.

      void implode ();
         //
         // Performs stochastic implosion.

      void clearSoln ();
         //
         // Clears the stochastic implosion solution.

      //------------------------------------------------------------------------
      // "Set" functions for stochastic mode.
      //------------------------------------------------------------------------

      void setCurrentScenario (int);
      void setProbability     (double);

      void setObjectStageIdx (WitNode * theNode, int theStageIdx);
         //
         // Sets the objectStageIdx of theNode to theStageIdx.

      void setPeriodStageIdx (WitPeriod thePer, int theStageIdx);
         //
         // Sets the periodStageIdx of thePer to theStageIdx.

      //------------------------------------------------------------------------
      // Data access functions for stochastic mode.
      //------------------------------------------------------------------------

      int    currentScenario ();
      double probability     ();
      int    objectStageIdx  (WitNode *);
      int    periodStageIdx  (WitPeriod);
      bool   stochSolnMode   ();
      bool   clearingSoln    ();

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      bool writeDataNeeded ();
         //
         // Returns true, iff writeData needs to write anything.

      void writeDataID ();
         //
         // Writes text identifying this StochImpMgr to an input data file.

      void writeDataAttrs ();
         //
         // Writes the attributes of this StochImpMgr to an input data file.

      void setSolnAttr (
                  WitDblFlexVec &    theAttr,
            const WitTVec <double> & theTVec);
         //
         // Sets theAttr to theTVec or to 0, if theTVec is unallocated.

      accessNonNull (WitStochModeMgr *, myStochModeMgr)

      noCopyCtorAssign (WitStochImpMgr);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      bool stageByObject_;
         //
         // If true,  stage-by-object mode is to be used.
         // If false, stage-by-period mode is to be used.

      int nScenarios_;
         //
         // The number of scenarios for stochastic implosion.

      bool stochMode_;
         //
         // True, iff the WitRun is currently in stochastic mode.

      WitStochModeMgr * myStochModeMgr_;
         //
         // The StochModeMgr owned by this StochImpMgr during stochastic mode;
         // otherwise NULL.
   };

#endif
