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

#ifndef PipReqSpecH
#define PipReqSpecH

//------------------------------------------------------------------------------
// Header file: "PipReqSpec.h"
//
// Contains the declaration of class PipReqSpec.
//------------------------------------------------------------------------------

#include <Util.h>

//------------------------------------------------------------------------------
// Class PipReqSpec
//
// "Post-Implosion Pegging Requirement Specification"
//
// Specifies a pegged requirement for PIP.
//
// Implemented in PipReq.C.
//------------------------------------------------------------------------------

class WitPipReqSpec
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitPipReqSpec ();

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitPipReqSpec ();

      //------------------------------------------------------------------------
      // Other Public member functions.
      //------------------------------------------------------------------------

      void setData (WitPipReqBasis *, double);
         //
         // Sets the data to the arguments.

      void getData (WitPipReqBasis * &, double &);
         //
         // Sets the arguments to the data.

      accessFunc (WitPipReqBasis *, myBasis)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitPipReqSpec);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitPipReqBasis * myBasis_;
         //
         // The PipReqBasis that is designated as the source of the requirement.

      double myIncReqVol_;
         //
         // The incremental requirement volume.
   };

#endif
