//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef StochOptMgrH
#define StochOptMgrH

//------------------------------------------------------------------------------
// Header file: "StochOptMgr.h"
//
// Contains the declaration of class StochOptMgr.
//------------------------------------------------------------------------------

#include <StochAssoc.h>

//------------------------------------------------------------------------------
// Class StochOptMgr
//
// "Stochastic Optimization Manager"
// Performs stochastic optimizing implosion. An instance of this class exists
// for the duration of a call to witStochImplode. Owned by the StochModeMgr.
//
// Class hierarchy:
//
//    ProbAssoc
//       StochAssoc
//          StochOptMgr
//
// Implemented in StochOpt.C.
//------------------------------------------------------------------------------

class WitStochOptMgr: public WitStochAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitStochOptMgr (WitStochModeMgr *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitStochOptMgr ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void implode ();
         //
         // Performs stochastic implosion.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (bool, uploadingSoln)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      void startStochImp ();
         //
         // Initiates stochastic optimizing implosion.

      void finishStochImp ();
         //
         // Initiates stochastic optimizing implosion.

      void uploadObjValues ();
         //
         // Uploads the objective function values to the owning objects.

      void uploadScenSoln ();
         //
         // Uploads the Scenario-specific aspects of the stochastic implosion
         // solution to the Scenarios.

      noCopyCtorAssign (WitStochOptMgr);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitStochImpOP * myStochImpOP_;
         //
         // The StochImpOP owned by this StochOptMgr, if any; otherwise NULL

      bool uploadingSoln_;
         //
         // True, iff the stochastic implosion solution is currently being
         // uploaded to the owning objects.
   };

#endif
