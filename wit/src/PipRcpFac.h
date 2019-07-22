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

#ifndef PipRcpFacH
#define PipRcpFacH

//------------------------------------------------------------------------------
// Header file: "PipRcpFac.h"
//
// Contains the declaration of template class PipRcpFac <RcpComp>.
//------------------------------------------------------------------------------

#include <Util.h>

//------------------------------------------------------------------------------
// Class template PipRcpFac <RcpComp>
//
// "PIP Recipient Facility"
// Provides functions for working with the recipient Component class of a
// PipRcpPgg <RcpComp, SrcComp>.
// This is a static class template: Instances cannot be created.
//
// Implemented in PipPgg.C.
//------------------------------------------------------------------------------

template <typename RcpComp> 
      class WitPipRcpFac
   {
   public:

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      static void getRcpPairs (
            WitPairStack <RcpComp, WitPeriod> & theRcpPairs,
            WitProblem *                        theProblem);
         //
         // On return, theRcpPairs will be a list of all PIP recipient pairs for
         // theProblem.

      static void getRcpComps (
            WitObjStack <RcpComp> & theRcpComps,
            WitProblem *            theProblem);
         //
         // On return, theRcpComps will be a list of all PIP enabled RcpComps in
         // theProblem. The list will be in reverse order.

      static const char * getRcpCompLabel ();
         //
         // Returns a c-string identifying the RcpComp class.

      static const char * getRcpCompName (RcpComp * theRcpComp);
         //
         // Returns the name of theRcpComp.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitPipRcpFac ();

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitPipRcpFac ();
   };

#endif
