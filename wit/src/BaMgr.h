//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef BaMgrH
#define BaMgrH

//------------------------------------------------------------------------------
// Header file: "BaMgr.h"
//
// Contains the declaration of class BaMgr.
//------------------------------------------------------------------------------

#include <SelMgr.h>
#include <PtrMap.h>
#include <PairStack.h>
#include <Mapping.h>

//------------------------------------------------------------------------------
// Class BaMgr
//
// Manager of the build-ahead subsystem.
// The build-ahead subsystem is responsible for maintaining the selections for 
// build-ahead.
//
// Class Hierarchy:
//
// ProbAssoc
//    SelMgr
//       BaMgr
//
// Implemented in BuildAhd.C.
//------------------------------------------------------------------------------

class WitBaMgr: public WitSelMgr
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitBaMgr (WitSelector *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitBaMgr ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      static bool instNeededFor (WitProblem * theProblem);
         //
         // Returns TRUE, iff a BaMgr is needed for theProblem.

      WitPeriod selExpPer (WitPart * thePart, WitPeriod fillPer);
         //
         // selExpPer (thePart, fillPer) is the currently selected explosion 
         // period to be used when filling requirements for thePart in fillPer.
         // If no explosion period is selected, then returns the Part's default
         // explosion period for fillPer.

      void print ();
         //
         // Prints the BaMats owned by this BaMgr.

      void saveExpPer (WitPeriod expPer);
         //
         // Saves the value of expPer to be retrieved later.

      void retrieveExpPer (WitPeriod & expPer);
         //
         // Sets expPer to the value saved by saveExpPer.

      void storeRecoveryPt (WitBaMat * theBaMat);
         //
         // Stores theBaMat as a point at which the build-ahead
         // configuration is to be restored to its initial state.

      void recoverInitState ();
         //
         // Restores the build-ahead selection configuration to its original
         // state.

      //------------------------------------------------------------------------
      // Selection Splitting functions.
      //------------------------------------------------------------------------

      virtual bool selSplitNeeded ();
         //
         // Override from class SelMgr.

      bool selIsSplit (WitPart * thePart, WitPeriod fillPer);
         //
         // Returns true, iff there is a split build-period selection for 
         // filling requirements for thePart in fillPer. 
         // Valid only in sel-split mode.

      void defineSplit (
            WitPart *        thePart,
            WitPeriod        fillPer,
            double           reqVol,
            WitPerDblStack & theSplitPairs,
            double &         unbddVol);
         //
         // Sets theSplitPairs to be the split pairs for a split explosion of
         // reqVol units at thePart in fillPer. Sets unbddVol to the splitVol of
         // the unbounded split explosion, if any; otherwise 0.

      void recExpMatSS (WitPart * thePart, WitPeriod fillPer, double expVol);
         //
         // Records the fact expVol units were exploded thru thePart to fill
         // reqs in fillPer.

      virtual void postSplitCommit ();
         //
         // Override from class SelMgr.

      void deferRejection (WitBaMat * theBaMat, WitPeriod expPer);
         //
         // Records to reject expPer as an explosion period for theBaMat at the
         // end of split-commit.

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

      WitPeriod selExpPer (
            WitPart *    thePart, 
            WitPeriod    fillPer,
            WitBaMat * & theBaMat);
         //
         // Returns the explosion period currently selected for filling
         // requirements in fillPer, if any, else -1. In the >= 0 case, sets
         // theBaMat to the corresponding BaMat.

      WitBaPt * myBaPt (WitPart * thePart, WitPeriod fillPer);
         //
         // Returns the BaPt for thePart in fillPer, if any, else NULL.

      noCopyCtorAssign (WitBaMgr);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitBaDir * myBaDirForNstn_;
      WitBaDir * myBaDirForAsap_;
         //
         // The BaDirs owned by this BaMgr for NSTN and ASAP Build-Ahead.

      WitPtrMap <WitMaterial, WitBaMat> myBaMatFor_;
         //
         // myBaMatFor_ (theMat) is the BaMat owned by this BaMgr 
         // corresponding to theMat, if any, else NULL.

      WitPairStack <WitBaMat, WitPeriod> pairsToReject_;
         //
         // Valid only in selection splitting mode.
         // For each pair, (theBaMat, expPer) in pairsToReject_, expPer is to be
         // rejected as an explosion period at the end of the current
         // split-commit.

      WitPeriod savedExpPer_;
         //
         // The expPer saved and retrieved by saveExpPer () and
         // retrieveExpPer ().

      WitObjStack <WitBaMat> alteredBaMats_;
         //
         // Valid only in selection recovery mode.
         // The set of BaMats at which the build-ahead configuration was altered
         // in the current incHeurAlloc.

      WitMapping <WitMaterial, bool> baAlteredAt_;
         //
         // Valid only in selection recovery mode.
         // baAlteredAt_ (theMat) is true, iff the build-ahead configuration was
         // altered at theMat in the current incHeurAlloc.
   };

#endif
