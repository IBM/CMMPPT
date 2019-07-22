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

#ifndef BaCandH
#define BaCandH

//------------------------------------------------------------------------------
// Header file: "BaCand.h"
//
// Contains the declaration of class BaCand.
//------------------------------------------------------------------------------

#include <SelCand.h>

//------------------------------------------------------------------------------
// Class BaCand
//
// "Build-Ahead Candidate"
// Represents a candidate for selection by build-ahead.
// In this case, the selection candidate is an explosion period to be used by
// some Material in order to fill requirements in some fill period.
//
// Class Hierarchy:
//
// SelCand
//    BaCand
//
// Implemented in BuildAhd.C.
//------------------------------------------------------------------------------

class WitBaCand: public WitSelCand
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitBaCand (WitPeriod, WitProblem *);

   private:

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitBaCand ();

      //------------------------------------------------------------------------
      // Other private member functions.
      //------------------------------------------------------------------------

      virtual void prtID     ();
      virtual void getBaData (WitPeriod &);
         //
         // Overrides from class SelCand.

      noCopyCtorAssign (WitBaCand);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      const WitPeriod myExpPer_;
         //
         // The candidate explosion period for this BaCand.
   };

#endif
