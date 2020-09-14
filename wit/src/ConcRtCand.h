//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef ConcRtCandH
#define ConcRtCandH

//------------------------------------------------------------------------------
// Header file: "ConcRtCand.h"
//
// Contains the declaration of the following classes:
//
//    ProdRtCand.
//    ConsRtCand.
//
// These are the "concrete" RtCand classes.
//------------------------------------------------------------------------------

#include <RtCand.h>

//------------------------------------------------------------------------------
// Class ProdRtCand
//
// "Production Routing Candidate"
// Represents a BopEntry in its role as a candidate for production routing.
//
// Class Hierarchy:
//
// ProbAssoc
//    RtCand
//       ProdRtCand
//
// Implemented in Routing.C.
//------------------------------------------------------------------------------

class WitProdRtCand: public WitRtCand
   {
   public:

      //------------------------------------------------------------------------
      // Public member functions.
      //------------------------------------------------------------------------

      static WitProdRtCand * newInstance (WitBopEntry *, WitRtMgr *);
         //
         // Constructs and returns a new ProdRtCand for the arguments.

   private:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitProdRtCand (WitBopEntry *, WitRtSite *, WitRtMgr *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitProdRtCand ();

      //------------------------------------------------------------------------
      // Overrides from class RtCand.
      //------------------------------------------------------------------------

      virtual bool usedByMr   ();
      virtual void printID    (int, int);
      virtual bool isEligible (WitPeriod);

      //------------------------------------------------------------------------
      // Other private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitProdRtCand);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitBopEntry * const myBopEnt_;
         //
         // The BopEntry represented by this ProdRtCand.
   };

//------------------------------------------------------------------------------
// Class ConsRtCand
//
// "Consumption Routing Candidate"
// Represents a ConsEntry in its role as a candidate for production routing.
//
// Class Hierarchy:
//
// ProbAssoc
//    RtCand
//       ConsRtCand
//
// Implemented in Routing.C.
//------------------------------------------------------------------------------

class WitConsRtCand: public WitRtCand
   {
   public:

      //------------------------------------------------------------------------
      // Public member functions.
      //------------------------------------------------------------------------

      static WitConsRtCand * newInstance (WitConsEntry *, WitRtMgr *);
         //
         // Constructs and returns a new ConsRtCand for the arguments.

   private:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitConsRtCand (WitConsEntry *, WitRtSite *, WitRtMgr *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitConsRtCand ();

      //------------------------------------------------------------------------
      // Overrides from class RtCand.
      //------------------------------------------------------------------------

      virtual bool usedByMr   ();
      virtual void printID    (int, int);
      virtual bool isEligible (WitPeriod);

      //------------------------------------------------------------------------
      // Other private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitConsRtCand);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitConsEntry * const myConsEnt_;
         //
         // The ConsEntry represented by this ConsRtCand.
   };

#endif
