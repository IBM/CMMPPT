//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef StochModeMgrH
#define StochModeMgrH

//------------------------------------------------------------------------------
// Header file: "StochModeMgr.h"
//
// Contains the declaration of class StochModeMgr.
//------------------------------------------------------------------------------

#include <Assoc.h>

//------------------------------------------------------------------------------
// Class StochModeMgr
//
// "Stochastic Mode Manager"
// Coordinates and participates in the aspects of stochastic implosion that
// require stochastic mode. An instance of this class exists in a WitRun,
// iff the WitRun is currently in stochastic mode.
//
// Implemented in StochMode.C.
//------------------------------------------------------------------------------

class WitStochModeMgr: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitStochModeMgr (WitProblem *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitStochModeMgr ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void copyAttrsFrom (WitStochModeMgr * theStochModeMgr);
         //
         // Copies the input attributes of theStochModeMgr into this
         // StochModeMgr.

      void uploadInput ();
         //
         // Uploads the values of the stochastic input attributes from the
         // current Scenario to the owning Components.

      void uploadSoln ();
         //
         // Uploads the values of the stochastic solution attributes from the
         // current Scenario to the owning Components.

      void implode ();
         //
         // Performs stochastic implosion.

      void clearSoln ();
         //
         // Clears the stochastic implosion solution.

      void clearCompSoln ();
         //
         // Clears the stochastic implosion solution in the Components.

      //------------------------------------------------------------------------
      // pass<SolnAttr> functions.
      // Each of these functions passes the value of indicated stochastic
      // solution attribute directly on to the owning Component.
      // Valid only when the stochastic implosion solution is being uploaded
      // from the StochOptMgr.
      //------------------------------------------------------------------------

      void passObjValue    (double);
      void passBoundsValue (double);

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc    (WitStochAttMgr *, myStochAttMgr)
      accessFunc    (WitStageMgr *,    myStageMgr)
      accessFunc    (WitScenMgr *,     myScenMgr)
      accessFunc    (WitScenAttMgr *,  myScenAttMgr)
      accessFunc    (bool,             uploadingData)
      accessFunc    (bool,             stochSolnMode)
      accessFunc    (bool,             clearingSoln)

      accessNonNull (WitStochOptMgr *, myStochOptMgr)

      inline bool imploding ()
         {
         return (myStochOptMgr_ != NULL);
         }

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      void validateCoreData ();
         //
         // Verifies that the input data for the core problem is valid for
         // entering stochastic mode.

      void verifyGlobalBool (
            const char * theAttName,
            bool         theVal,
            bool         reqVal);
         //
         // Verifies the value of a global boolean attribute.
         // theAttName is the       name of the attribute.
         // theVal is the          value of the attribute.
         // reqVal is the required value of the attribute.

      noCopyCtorAssign (WitStochModeMgr);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitStochAttMgr * myStochAttMgr_;
         //
         // The StochAttMgr owned by this StochModeMgr.

      WitStageMgr * myStageMgr_;
         //
         // The StageMgr owned by this StochModeMgr.

      WitScenMgr * myScenMgr_;
         //
         // The ScenMgr owned by this StochModeMgr.

      WitScenAttMgr * myScenAttMgr_;
         //
         // The ScenAttMgr owned by this StochModeMgr.

      WitStochOptMgr * myStochOptMgr_;
         //
         // The StochOptMgr owned by this StochModeMgr during stochastic
         // implosion; otherwise NULL.

      bool uploadingData_;
         //
         // True, iff the stochastic implosion subsystem is currently uploading
         // scenario-specific data to the owning Components.

      bool stochSolnMode_;
         //
         // True, iff the WitRun is currently in stochastic solution mode.

      bool clearingSoln_;
         //
         // True, iff the stochastic solution is currently being cleared.
   };

#endif
