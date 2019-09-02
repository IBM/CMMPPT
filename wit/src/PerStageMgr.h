//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef PerStageMgrH
#define PerStageMgrH

//------------------------------------------------------------------------------
// Header file: "PerStageMgr.h"
//
// Contains the declaration of class PerStageMgr.
//------------------------------------------------------------------------------

#include <StageMgr.h>
#include <PtrTVec.h>

//------------------------------------------------------------------------------
// Class PerStageMgr
//
// "Period Stage Manager"
// Manages the stage data for stochastic implosion in stage-by-period mode.
//
// Class hierarchy:
//
//    ProbAssoc
//       StageMgr
//          PerStageMgr
//
// Implemented in Stage.C.
//------------------------------------------------------------------------------

class WitPerStageMgr: public WitStageMgr
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitPerStageMgr (WitStochModeMgr *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitPerStageMgr ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      virtual void       setObjectStageIdx (WitNode *, int);
      virtual void       setPeriodStageIdx (WitPeriod, int);
      virtual WitStage * objectStage       (WitNode *);
      virtual WitStage * periodStage       (WitPeriod);
      virtual WitStage * myStageOf         (WitNode *,  WitPeriod);
      virtual bool       includesNode      (WitStage *, WitNode *);
      virtual bool       includesPeriod    (WitStage *, WitPeriod);
      virtual void       writeData         ();
      virtual void       displayData       ();
      virtual void       copyAttrsFrom     (WitStageMgr *);
      virtual void       validateData      ();
      virtual void       issueAttMisMatchHeadMsg ();
         //
         // Overrides from class StageMgr.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      void validateCoreData ();
         //
         // Verifies that the input data for the core problem is valid for
         // entering stochastic mode in stage-by-period mode.

      void validateBomEnts ();
         //
         // Verifies that there are no BOM entries with positive offsets.

      void validateSubs ();
         //
         // Verifies that there are no substitutes with positive offsets.

      void validateBopEnts ();
         //
         // Verifies that there are no BOP entries with positive offsets.

      void checkImpactPeriod (WitBillEntry * theBillEnt, WitPeriod execPer);
         //
         // Verifies that the impactPeriod of theBillEnt in execPer is
         // >= execPer.

      noCopyCtorAssign (WitPerStageMgr);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitPtrTVec <WitStage> periodStage_;
         //
         // periodStage_[thePer] is the stage to which thePer belongs.
   };

#endif
