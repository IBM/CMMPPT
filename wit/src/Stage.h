//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef StageH
#define StageH

//------------------------------------------------------------------------------
// Header file: "Stage.h"
//
// Contains the declaration of class Stage.
//------------------------------------------------------------------------------

#include <StochAssoc.h>

//------------------------------------------------------------------------------
// Class Stage
//
// A stage for stochastic implosion.
//------------------------------------------------------------------------------

class WitStage: public WitStochAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitStage (int theIdx, WitStochModeMgr *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitStage ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (int, myIndex)

      inline bool isStage0 ()
         {
         return (myIndex_ == 0);
         }

      inline bool isStage1 ()
         {
         return (myIndex_ == 1);
         }

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      bool includes (WitNode * theNode);
         //
         // Stage-by-object mode: returns true, iff theNode belongs to this
         //                       Stage.
         // Stage-by-period mode: returns true, unconditionally.

      bool includes (WitPeriod thePer);
         //
         // Stage-by-object mode: returns true, iff thePer belongs to this Stage
         // Stage-by-period mode: returns true, unconditionally.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitStage);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      const int myIndex_;
         //
         // The index of this Stage.
   };

#endif
