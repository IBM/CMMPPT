//==============================================================================
// IBM Confidential
//
// OCO Source Materials
//
// Watson Implosion Technology
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//
// The Source code for this program is not published or otherwise divested of
// its trade secrets, irrespective of what has been deposited with the U. S.
// Copyright office.
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
