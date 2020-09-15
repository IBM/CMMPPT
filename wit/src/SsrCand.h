//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef SsrCandH
#define SsrCandH

//------------------------------------------------------------------------------
// Header file: "SsrCand.h"
//
// Contains the declaration of class SsrCand.
//------------------------------------------------------------------------------

#include <SelCand.h>

//------------------------------------------------------------------------------
// Class SsrCand
//
// "Selective Stock Reallocation Candidate"
// A SelCand for Selective Stock Reallocation.
// Represents whether or not stock reallocation is allowed for a given Material
// in a given period.
//
// Class Hierarchy:
//
// ProbAssoc
//    SelCand
//       SsrCand
//
// Implemented in SelStRe.C.
//------------------------------------------------------------------------------

class WitSsrCand: public WitSelCand
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitSsrCand (bool, WitProblem *);

   private:

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitSsrCand ();

      //------------------------------------------------------------------------
      // Other private member functions.
      //------------------------------------------------------------------------

      virtual void prtID     ();
      virtual void getSsrData (bool &);
         //
         // Overrides from class SelCand.

      noCopyCtorAssign (WitSsrCand);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      const bool srAllowed_;
         //
         // Given any Material and period for which this SsrCand is selected,
         // stock reallocation from the period for the Material is allowed, iff
         // srAllowed_ is true.
   };

#endif
