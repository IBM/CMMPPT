//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef MrSelMgrH
#define MrSelMgrH

//------------------------------------------------------------------------------
// Header file: "MrSelMgr.h"
//
// Contains the declaration of the following classes:
//
//    MrSelMgr.
//    PmrSelMgr.
//    CmrSelMgr.
//------------------------------------------------------------------------------

#include <SelMgr.h>
#include <PtrMap.h>

//------------------------------------------------------------------------------
// Class MrSelMgr
//
// "Multi-Route Selection Manager"
// Selection Manager for one of the two cases of Multi-Route.
//
// Class Hierarchy:
//
// ProbAssoc
//    SelMgr
//       MrSelMgr
//          PmrSelMgr
//          CmrSelMgr
//
// Implemented in MultiRoute.C.
//------------------------------------------------------------------------------

class WitMrSelMgr: public WitSelMgr
   {
   public:

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitMrSelMgr ();

   protected:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitMrSelMgr (WitMrCoord *);

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitMrCoord *, myMrCoord)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      virtual bool selSplitNeeded ();
         //
         // Overrides from class SelMgr.

      noCopyCtorAssign (WitMrSelMgr);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitMrCoord * const myMrCoord_;
         //
         // The MrCoord that owns this MrSelMgr.
   };

//------------------------------------------------------------------------------
// Class PmrSelMgr
//
// "Production Multi-Route Selection Manager"
// Selection Manager for the production case of Multi-Route.
//
// Class Hierarchy:
//
// ProbAssoc
//    SelMgr
//       MrSelMgr
//          PmrSelMgr
//
// Implemented in MrMgr.C.
//------------------------------------------------------------------------------

class WitPmrSelMgr: public WitMrSelMgr
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitPmrSelMgr (WitMrCoord *);

   private:

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitPmrSelMgr ();

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

      noCopyCtorAssign (WitPmrSelMgr);
   };

//------------------------------------------------------------------------------
// Class CmrSelMgr
//
// "Consumption Multi-Route Selection Manager"
// Selection Manager for the consumption case of Multi-Route.
//
// Class Hierarchy:
//
// ProbAssoc
//    SelMgr
//       MrSelMgr
//          CmrSelMgr
//
// Implemented in MrMgr.C.
//------------------------------------------------------------------------------

class WitCmrSelMgr: public WitMrSelMgr
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitCmrSelMgr (WitMrCoord *);

   private:

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitCmrSelMgr ();

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

      noCopyCtorAssign (WitCmrSelMgr);
   };

#endif
