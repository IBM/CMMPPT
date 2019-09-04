//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef MeCandH
#define MeCandH

//------------------------------------------------------------------------------
// Header file: "MeCand.h"
//
// Contains the declaration of class MeCand.
//------------------------------------------------------------------------------

#include <SelCand.h>

//------------------------------------------------------------------------------
// Class MeCand
//
// "Multi-Exec Candidate"
// Represents a candidate for selection by multi-exec.
// In this case, the selection candidate is an execution period to be used by
// some BopEntry to produce in some explosion period.
//
// Class Hierarchy:
//
// SelCand
//    MeCand
//
// Implemented in MultiEx.C.
//------------------------------------------------------------------------------

class WitMeCand: public WitSelCand
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitMeCand (WitPeriod, WitProblem *);

   private:

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitMeCand ();

      //------------------------------------------------------------------------
      // Other private member functions.
      //------------------------------------------------------------------------

      virtual void prtID     ();
      virtual void getMeData (WitPeriod &);
         //
         // Overrides from class SelCand.

      noCopyCtorAssign (WitMeCand);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      const WitPeriod myExecPer_;
         //
         // The candidate execution period for this MeCand.
   };

#endif
