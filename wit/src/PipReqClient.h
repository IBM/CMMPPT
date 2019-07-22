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

#ifndef PipReqClientH
#define PipReqClientH

//------------------------------------------------------------------------------
// Header file: "PipReqClient.h"
//
// Contains the declaration of template class PipReqClient <RcpComp>.
//------------------------------------------------------------------------------

#include <Util.h>

//------------------------------------------------------------------------------
// Class template PipReqClient <RcpComp>
//
// "PIP Requirement Client"
// Represents a (RcpComp, Period) pair.
// RcpComp may be Demand or Operation.
// Specificially, PipReqClient <Demand> represents a shipment sequence element.
//
// Implemented in PipReq.C.
//------------------------------------------------------------------------------

template <typename RcpComp> 
      class WitPipReqClient
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitPipReqClient (RcpComp * theRcpComp, WitPeriod rcpPer, int theSeqNo);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitPipReqClient ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      void getData (RcpComp * & theRcpComp, WitPeriod & rcpPer);
         //
         // Sets theRcpComp and rcpPer to the data for this PipReqClient.

      void print (FILE * theFile, const char * leadingBlanks);
         //
         // Prints this PipReqClient to theFile.
         // leadingBlanks is to be a string of blanks to preceed the print.

      accessFunc (int, mySeqNo)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitPipReqClient);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      RcpComp * const myRcpComp_;
         //
         // The RcpComp of this PipReqClient, or NULL for the vacuous recipient.

      const WitPeriod myRcpPer_;
         //
         // The recipient period of this PipReqClient.

      const int mySeqNo_;
         //
         // The sequence number of this PipReqClient.
   };

#endif
