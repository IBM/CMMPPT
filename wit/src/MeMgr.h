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

#ifndef MeMgrH
#define MeMgrH

//------------------------------------------------------------------------------
// Header file: "MeMgr.h"
//
// Contains the declaration of class MeMgr.
//------------------------------------------------------------------------------

#include <SelMgr.h>
#include <PtrSched.h>
#include <PerSglStack.h>

//------------------------------------------------------------------------------
// Class MeMgr
//
// Responsible for owning and coordinating the objects that implement
// multi-exec.
//
// Class Hierarchy:
//
// ProbAssoc
//    SelMgr
//       MeMgr
//
// Implemented in MultiEx.C.
//------------------------------------------------------------------------------

class WitMeMgr: public WitSelMgr
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitMeMgr (WitSelector *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitMeMgr ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      static bool instNeededFor (WitProblem * theProblem);
         //
         // Returns TRUE, iff an MeMgr is needed for theProblem.

      void setInitDirIsAsap (bool theValue);
         //
         // Sets initDirIsAsap.

      WitPeriod selExecPer (WitBopEntry * theBopEnt, WitPeriod expPer);
         //
         // Returns the currently selected execution period to be used when
         // exploding through theBopEnt in period expPer.

      void print ();
         //
         // Prints the multi-exec subsystem.

      void saveExecPer (WitPeriod execPer);
         //
         // Saves the value of execPer to be retrieved later.

      void retrieveExecPer (WitPeriod & execPer);
         //
         // Sets execPer to the value saved by saveExecPer.

      void storeRecoveryPair (WitBopEntry * theBopEnt, WitPeriod expPer);
         //
         // Stores (theBopEnt, expPer) as a pair at which the multi-exec
         // configuration is to be restored to its initial state.

      void recoverInitState ();
         //
         // Restores the multi-exec configuration to its initial state.

      //------------------------------------------------------------------------
      // Selection Splitting functions.
      //------------------------------------------------------------------------

      virtual bool selSplitNeeded ();
         //
         // Override from class SelMgr.

      bool selIsSplit (WitBopEntry * theBopEnt, WitPeriod expPer);
         //
         // Returns true, iff there is a split selection for exploding
         // thru theBopEnt in expPer. Valid only in sel-split mode.

      void defineSplit (
            WitBopEntry *    theBopEnt,
            WitPeriod        expPer,
            double           expVol,
            WitPerDblStack & theSplitPairs,
            double &         unbddVol);
         //
         // Sets theSplitPairs to be the split pairs for a split explosion of
         // expVol units thru theBopEnt in expPer. Sets unbddVol to the splitVol
         // of the unbounded split explosion, if any; otherwise 0.

      void recBopEntExecPerSS (
            WitBopEntry * theBopEnt, 
            WitPeriod     execPer, 
            double        expVol);
         //
         // Records the fact expVol units were exploded thru theBopEnt in
         // execPer.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitMeDir *, myMeDirForNstn)
      accessFunc (WitMeDir *, myMeDirForAsap)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
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

      void buildMeSitePts ();
         //
         // Builds the MeSitePts to be owned by this MeMgr.

      WitMePt * myMePtWithSel (WitBopEntry * theBopEnt, WitPeriod expPer);
         //
         // Returns the MePt to be used when exploding through theBopEnt in
         // period expPer, or NULL, if none exists, or it has no selection.

      WitMePt * myMePt (WitBopEntry * theBopEnt, WitPeriod expPer);
         //
         // Returns the MePt to be used when exploding through theBopEnt in
         // period expPer, or NULL, if none exists.

      WitMeDir * chooseMeDir (WitPart * thePart);
         //
         // Returns the MeDir appropriate for exploding thePart.

      bool findNextMeSitePt (
            WitBopEntry *   theBopEnt,
            WitPeriod &     expPer,
            WitMeSitePt * & theMeSitePt);
         //
         // Sets expPer to the next next explosion period after the current
         // value of expPer for which theBopEnt has a MeSitePt, if any, and in
         // that case, sets theMeSitePt to that MeSitePt. 
         // Returns true, iff another MeSitePt was found.

      inline bool hasMeSitePts (WitBopEntry * theBopEnt);
         //
         // Returns true, iff this MeMgr owns any MeSitePts for theBopEnt.

      noCopyCtorAssign (WitMeMgr);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitMeDir * myMeDirForNstn_;
      WitMeDir * myMeDirForAsap_;
         //
         // The MeDirs owned by this MeMgr for NSTN and ASAP multi-exec.

      WitPtrSched <WitBopEntry, WitMeSitePt> myMeSitePts_;
         //
         // myMeSitePts_.myPtrAt (theBopEnt, expPer) is the MeSitePt
         // owned by this MeMgr corresponding to theBopEnt and expPer.

      bool initDirIsAsap_;
         //
         // true, iff the intial multi-exec direction is ASAP.

      WitPeriod savedExecPer_;
         //
         // The execPer saved and retrieved by saveExecPer () and
         // retrieveExecPer ().

      WitPerSglStack <WitBopEntry> myRecoveryPairs_;
         //
         // In selection recovery mode, myRecoveryPairs_ is the set of all pairs
         // (BopEntry, Period) at which the multi-exec configuration is to be
         // restored to its initial state.
   };

#endif
