//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef SsrMgrH
#define SsrMgrH

//------------------------------------------------------------------------------
// Header file: "SsrMgr.h"
//
// Contains the declaration of class SsrMgr.
//------------------------------------------------------------------------------

#include <SelMgr.h>
#include <PtrSched.h>
#include <ObjStack.h>
#include <PerSglStack.h>

//------------------------------------------------------------------------------
// Class SsrMgr
//
// "Selective Stock Reallocation Manager"
// Manager of the selective stock reallocation subsystem.
// The selective stock reallocation subsystem is responsible for maintaining the
// selections for selective stock reallocation.
//
// Class Hierarchy:
//
// ProbAssoc
//    SelMgr
//       SsrMgr
//
// Implemented in SelStRe.C.
//------------------------------------------------------------------------------

class WitSsrMgr: public WitSelMgr
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitSsrMgr (WitSelector *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitSsrMgr ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      static bool instNeededFor (WitHeurAtor * theHeurAtor);
         //
         // Returns TRUE, iff an SsrMgr is needed for theHeurAtor.

      bool srAllowedFrom (WitMaterial * theMat, WitPeriod thePer);
         //
         // Returns true, iff stock reallocation of theMat from thePer is
         // currently allowed.

      void recordSR (WitMaterial * theMat, WitPeriod thePer, double srVol);
         //
         // Records the fact that srVol units of stock reallocation of theMat
         // from thePer is currently being performed.

      void postTempCommit ();
         //
         // Performs the actions to be taken for SSR, selection, just after a 
         // temp-commit.

      void print (WitMaterial * theMat);
         //
         // Prints the SsrPts for theMat.

      void storeRecoveryPair (WitMaterial * theMat, WitPeriod fromPer);
         //
         // Stores (theMat, fromPer) as a pair at which the SSR
         // configuration is to be restored to its initial state.

      void recoverInitState ();
         //
         // Restores the SSR configuration to its initial state.

      //------------------------------------------------------------------------
      // Selection Splitting functions.
      //------------------------------------------------------------------------

      virtual bool selSplitNeeded ();
         //
         // Override from class SelMgr.

      void adjReqAndRollForSS (
            WitMaterial * theMat,
            WitPeriod     rollPer,
            double &      theReqVol,
            double &      rollVol,
            bool &        bddSplit);
         //
         // Adjusts rollVol and theReqVol as neccessary for a split stock
         // reallocation selection at theMat in rollPer.
         // If the rollVol is for a bounded split, sets bddSplit to true.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc  (bool, srSelSplit)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      virtual WitAltPt *   myAltPtFor         (WitBopEntry *,
                                               WitConsEntry *,
                                               WitPeriod,
                                               WitPeriod);
      virtual WitAltPt *   myAltPtFor         (WitPart *, WitPeriod);
      virtual bool         validForNetting    ();
      virtual bool         validForExecBounds ();
      virtual bool         validForLTBounds   ();
      virtual const char * selCaseName        ();
         //
         // Overrides from class SelMgr.

      bool hasanSsrPtFor (WitMaterial * theMat, WitPeriod fromPer);
         //
         // Returns true, iff this SsrMgr owns an SsrPt for theMat and fromPer.

      noCopyCtorAssign (WitSsrMgr);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitPtrSched <WitMaterial, WitSsrPt> mySsrPts_;
         //
         // mySsrPts_.myPtrAt (theMat, fromPer) is the SsrPt owned by this
         // SsrMgr that selects whether or not stock reallocation of theMat is
         // to be allowed from fromPer, if any; else NULL.

      WitObjStack <WitSsrPt> srWasDonePts_;
         //
         // The set of SsrPts owned by this SsrMgr for which srWasDone_ is true.

      bool srSelSplit_;
         //
         // true, iff selection splitting for selection stock reallocation has
         // been requested.

      WitPerSglStack <WitMaterial> myRecoveryPairs_;
         //
         // In selection recovery mode, myRecoveryPairs_ is the set of all pairs
         // (Material, Period) at which the SSR configuration is to be restored
         // to its initial state.
   };

#endif
