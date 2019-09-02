//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef PrSelMgrH
#define PrSelMgrH

//------------------------------------------------------------------------------
// Header file: "PrSelMgr.h"
//
// Contains the declaration of the following classes:
//
//    PrSelMgr.
//    PpoSelMgr.
//    CpoSelMgr.
//------------------------------------------------------------------------------

#include <SelMgr.h>

//------------------------------------------------------------------------------
// Class PrSelMgr
//
// "Proportionate Routing Selection Manager"
// Selection Manager for one of the two cases of proportionate routing.
//
// Class Hierarchy:
//
// ProbAssoc
//    SelMgr
//       PrSelMgr
//          PprSelMgr
//          CprSelMgr
//
// Implemented in PropRtg.C.
//------------------------------------------------------------------------------

class WitPrSelMgr: public WitSelMgr
   {
   public:

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitPrSelMgr ();

   protected:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitPrSelMgr (WitPrCoord *);

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitPrCoord *, myPrCoord)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      virtual bool selSplitNeeded ();
         //
         // Override from class SelMgr.

      noCopyCtorAssign (WitPrSelMgr);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitPrCoord * const myPrCoord_;
         //
         // The PrCoord that owns this PrSelMgr.
   };

//------------------------------------------------------------------------------
// Class PprSelMgr
//
// "Production Proportionate Routing Selection Manager"
// Selection Manager for the production case of proportionate routing.
//
// Class Hierarchy:
//
// ProbAssoc
//    SelMgr
//       PrSelMgr
//          PprSelMgr
//
// Implemented in PropRtg.C.
//------------------------------------------------------------------------------

class WitPprSelMgr: public WitPrSelMgr
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitPprSelMgr (WitPrCoord *);

   private:

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitPprSelMgr ();

      //------------------------------------------------------------------------
      // Other private member functions.
      //------------------------------------------------------------------------

      virtual WitAltPt *   myAltPtFor         (WitBopEntry *,
                                               WitConsEntry *,
                                               WitPeriod,
                                               WitPeriod);
      virtual bool         validForNetting    ();
      virtual bool         validForExecBounds ();
      virtual bool         validForLTBounds   ();
      virtual const char * selCaseName        ();
         //
         // Overrides from class SelMgr.
   };

//------------------------------------------------------------------------------
// Class CprSelMgr
//
// "Consumption Proportionate Routing Selection Manager"
// Selection Manager for the consumption case of proportionate routing.
//
// Class Hierarchy:
//
// ProbAssoc
//    SelMgr
//       PrSelMgr
//          CprSelMgr
//
// Implemented in PropRtg.C.
//------------------------------------------------------------------------------

class WitCprSelMgr: public WitPrSelMgr
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitCprSelMgr (WitPrCoord *);

   private:

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitCprSelMgr ();

      //------------------------------------------------------------------------
      // Other private member functions.
      //------------------------------------------------------------------------

      virtual WitAltPt *   myAltPtFor         (WitBopEntry *,
                                               WitConsEntry *,
                                               WitPeriod,
                                               WitPeriod);
      virtual bool         validForNetting    ();
      virtual bool         validForExecBounds ();
      virtual bool         validForLTBounds   ();
      virtual const char * selCaseName        ();
         //
         // Overrides from class SelMgr.
   };

#endif
