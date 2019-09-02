//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef PipReqCListH
#define PipReqCListH

//------------------------------------------------------------------------------
// Header file: "PipReqCList.h"
//
// Contains the declaration of template class PipReqCList <RcpComp>.
//------------------------------------------------------------------------------

#include <PtrVec.h>

//------------------------------------------------------------------------------
// Class template PipReqCList <RcpComp>
//
// "PIP Requirement Client List"
// Represents an ordered list of PipReqClient <RcpComp>.
//
// Implemented in PipReq.C.
//------------------------------------------------------------------------------

template <typename RcpComp> 
      class WitPipReqCList
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitPipReqCList (const WitPtrVec <WitPipReqClient <RcpComp> > &);

      WitPipReqCList (
            const WitPipReqBasisStack & theBases,
            WitProblem *);
         //
         // Constructs a PipCList as the union of the PipClist <RcpComp>
         // instances in theBases.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitPipReqCList ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      bool belongsBefore (WitPipReqCList <RcpComp> * theReqCList);
         //
         // Returns true, iff this PipReqCList belongs strictly ahead of
         // theReqCList in an ordered list.

      bool isEquivalentTo (WitPipReqCList <RcpComp> * theReqCList);
         //
         // Returns true, iff this WitPipReqCList lists the same PipReqClients
         // as theReqCList.

      void print (FILE * theFile, const char * leadingBlanks);
         //
         // Prints this PipReqCList to theFile.
         // leadingBlanks is to be a string of blanks to preceed the print.

      void attachClientItr (
            WitPtrVecItr <WitPipReqClient <RcpComp> > & theClientItr);
         //
         // Attaches theClientItr to myClientVec_.

      accessFunc (const WitPtrVec <WitPipReqClient <RcpComp> > &, myClientVec)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      void getClientsFromBases (
            WitObjStack <WitPipReqClient <RcpComp> > & theClients,
            const WitPipReqBasisStack &                theBases);
         //
         // Finds all of the PipReqClient <RcpComp> instances in theBases and
         // stores them in theClients.

      void loadClientVec (
            WitObjStack <WitPipReqClient <RcpComp> > & theClients);
         //
         // Loads the ReqClients in theClients into myClientVec_, in reverse
         // order.

      static bool compareClients (
            WitPipReqClient <RcpComp> * theClient1,
            WitPipReqClient <RcpComp> * theClient2);
         //
         // Returns true, iff theClient1 belongs ahead of theClient2.

      void getUniqueClientsFromVec (
            WitObjStack <WitPipReqClient <RcpComp> > & theClients);
         //
         // Loads the ReqClients in myClientVec_ into theClients, eliminating
         // consecutuve duplicates.

      inline int length ()
         {
         return myClientVec_.length ();
         }

      noCopyCtorAssign (WitPipReqCList);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitPtrVec <WitPipReqClient <RcpComp> > myClientVec_;
         //
         // The ordered list of ReqClients for this PipReqCList.
   };

#endif
