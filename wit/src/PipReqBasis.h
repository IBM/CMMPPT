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

#ifndef PipReqBasisH
#define PipReqBasisH

//------------------------------------------------------------------------------
// Header file: "PipReqBasis.h"
//
// Contains the declaration of class PipReqBasis.
//------------------------------------------------------------------------------

#include <Util.h>

//------------------------------------------------------------------------------
// Class PipReqBasis
//
// "Post-Implosion Pegging Requirement Basis"
//
// Represents the source of a requirement in PIP.
//
// Implemented in PipReq.C.
//------------------------------------------------------------------------------

class WitPipReqBasis
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitPipReqBasis (WitPipReqClient <WitDemand> *, WitProblem *);

      WitPipReqBasis (
            WitPipReqBasis *                 theBasis,
            WitPipReqClient <WitOperation> * theOpnClient);
         //
         // Constructs a PipReqBasis that's the same as theBasis, except thst
         // theOpnClient is added.

      WitPipReqBasis (const WitPipReqBasisStack & theBases, WitProblem *);
         //
         // Constructs a PipReqBasis whose ReqClient list of each type is the
         // union of those in theBases.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitPipReqBasis ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      bool belongsBefore (WitPipReqBasis * theBasis);
         //
         // Returns true, iff this PipReqBasis belongs strictly ahead of
         // theBasis in an ordered list.

      bool isEquivalentTo (WitPipReqBasis * theBasis);
         //
         // Returns true, iff this PipReqBasis is equivalent to theBasis.
         // (i.e. it lists the same DemReqClients.)

      void print (FILE * theFile, const char * leadingBlanks);
         //
         // Prints this PipReqBasis to theFile.
         // leadingBlanks is to be a string of blanks to preceed the print.

      void attachClientItr (WitPtrVecItr <WitPipReqClient <WitDemand>    > &);
      void attachClientItr (WitPtrVecItr <WitPipReqClient <WitOperation> > &);
         //
         // Each function attaches the argument to the relevant ReqCList of
         // this PipReqBasis.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      static void formByInsertion (
                  WitPtrVec <WitPipReqClient <WitOperation> > & newOpnClientVec,
            const WitPtrVec <WitPipReqClient <WitOperation> > & oldOpnClientVec,
                  WitPipReqClient <WitOperation> *              newOpnClient);
         //
         // Sets newOpnClientVec to oldOpnClientVec, with newOpnClient inserted.

      noCopyCtorAssign (WitPipReqBasis);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitPipReqCList <WitDemand>    * myDemCList_;
      WitPipReqCList <WitOperation> * myOpnCList_;
         //
         // The ordered list of PipReqCList <Demand>    for this PipReqBasis.
         // The ordered list of PipReqCList <Operation> for this PipReqBasis.
   };

#endif
