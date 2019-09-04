//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef HeurAtorH
#define HeurAtorH

//------------------------------------------------------------------------------
// Header file: "HeurAtor.h"
//
// Contains the declaration of class HeurAtor.
//------------------------------------------------------------------------------

#include <Demand.h>
#include <Operation.h>
#include <SubEntry.h>
#include <Schedule.h>
#include <Mapping.h>

//------------------------------------------------------------------------------
// class HeurAtor
//
// This class is responsible for actually performing a heuristic allocation.
// An instance of this class exists only when heuristic allocation is active.
//------------------------------------------------------------------------------

class WitHeurAtor: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitHeurAtor (WitProblem *, WitPegger *, bool forOpt);
         //
         // forOpt == true indicates heur allocation is for opt implosion.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitHeurAtor ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      double incHeurAlloc (
            WitDemand * theDemand, 
            WitPeriod   thePer, 
            double      desIncVol,
            bool        asapMultiExec);
         //
         // Performs a heuristic allocation.
         // See HeurAllMgr::inc for argument meanings.

      void enforceStockSLBs ();
         //
         // Enforces stock soft lower bounds.

      void commitMaxQtySel (double & netQty);
         //
         // Commits the heuristically maximum possible quantity of topPart_ 
         // in topPer_, for a given selection configuration.

      bool tempCommit (double qty);
         //
         // Calls commit on temp data.
         // Returns true iff the commit attempt succeeded.

      void permCommit1SS ();
         //
         // Does a perm-commit of 1 unit for selection splitting purposes.
         // Valid only in sel-split mode.

      bool incAvailPos (WitPart * thePart, WitPeriod thePer);
         //
         // Returns true, iff there is positive incremental availability of
         // thePart in thePer.

      bool selectiveSR (WitMaterial *);
         //
         // See AvailSched.

      void saveVal (int    & theVar);
      void saveVal (double & theVar);
         //
         // Saves the current value of theVar.

      void saveExecVol (WitOperation * theOpn, WitPeriod thePer);
         //
         // Saves the execVol for theOpn in thePer.

      void saveSubVol (WitSubEntry * theSub, WitPeriod thePer);
         //
         // Saves the subVol for theSub in thePer.

      void printAvailSched ();
         //
         // Prints the availability schedule.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (bool,   buildAhead)
      accessFunc (int,    ltbMinPer)
      accessFunc (bool,   tempCommActive)
      accessFunc (bool,   permCommActive)
      accessFunc (bool,   failedOn1)
      accessFunc (bool,   userHeurStart)
      accessFunc (double, searchInc)
                                  
      accessNonNull (WitHeurModifier *, myHeurModifier)
      accessNonNull (WitSelector *,     mySelector)
      accessNonNull (WitPart *,         topPart)
      accessNonNull (WitSsrMgr *,       mySsrMgr)

      inline bool multiSel () 
         {
         return (mySelector_ != NULL);
         }

      inline WitPeriod topPer ()
         {
         witAssert (topPer_ >= 0);

         return topPer_;
         }

      inline const WitMapping <WitBomEntry, bool> & hasSubsToNet ()
         {
         return hasSubsToNet_;
         }

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitHeurAtor);

      void initBlocked ();
         //
         // Initializes blocked_.

      void setUpSelection ();
         //
         // Sets up the Selection Subsystem, as needed.

      void initSoln ();
         //
         // Initializes the implosion solution.

      void compLeadTimeBounds ();
         //
         // Computes and sets the value of leadTimeBounds_.

      void setUpLeadTimeBounds ();
         //
         // Sets up the lead time bounds capability.

      double compDemandedIncVol (
            WitDemand * theDemand,
            WitPeriod   thePer,
            double      desIncVol);
         //
         // Computes and returns the portion of desIncVol that's demanded for
         // theDemand in thePer.

      double minUnmetCumDem (WitDemand * theDemand, WitPeriod thePer);
         //
         // Computes and returns the min value of cumDemandVol - cumShipVol over
         // {thePer, ..., nPeriods - 1}

      void preIncHeurAlloc (
            WitDemand * theDemand, 
            WitPeriod   thePer, 
            bool        asapMultiExec);
         //
         // Takes actions appropriate at the beginning of incHeurAlloc.

      void doBaByDemand (
            WitDemand * theDemand, 
            WitPeriod   lastPer,
            double &    netQty);
         //
         // Does build-ahead by demand and theDemand in lastPer and earlier
         // periods.

      void postIncHeurAlloc (
            WitDemand * theDemand, 
            WitPeriod   thePer, 
            double      desIncVol,
            double      incVol);
         //
         // Takes actions appropriate at the conclusion of incHeurAlloc.

      void enforceStockSLBs (WitPart * thePart);
         //
         // Enforces the stock soft lower bounds for thePart in all periods.

      void enforceStockSLB (WitMaterial * theMat, WitPeriod thePer);
         //
         // Enforces the stock soft lower bound for theMat in thePer.
         // This function assumes that none of theMat's stockVol in thePer has
         // already been reserved for stock softLBs. Thus this function should
         // only be called once for each Material and period.

      void commitMaxQty (WitPart * thePart, WitPeriod thePer, double & netQty);
         //
         // Commits the heuristically maximum possible quantity of thePart in
         // thePer, considering multiple selections, if appropriate.

      void permCommit (double qty);
         //
         // Calls commit on perm data.
         // Verifies that the commit attempt succeeded.

      double findMaxQty (double desQty);
         //
         // Determines the heuristic maximum lotsize-feasible quantity of
         // topPart_ that is <= desQty and can be made in topPer_.
         // The search considers only lotsize feasible quantities.
         // If the minimum lot-sized quantity is not feasible, 0.0 is returned.
         // Otherwise, if the desired quantity is feasible, it is returned.
         // Otherwise the max quantity is found by a binary search.

      double binarySearch (double incQty);
         //
         // Does the binary search aspect of findMaxQty, where 1.0 is assumed
         // feasible and desQty is assumed infeasible.

      bool commit (double qty);
         //
         // Attempts to commit qty units of topPart () to a shipment in 
         // topPer (). 
         // Returns true iff the attempt succeeded.

      bool innerCommit (double qty);
         //
         // Does most of the work for commit.

      bool fillReq (
            WitPart * thePart, 
            WitPeriod fillPer, 
            double    theReqVol);
         //
         // Attempts to meet theReqVol units of requirement of thePart in
         // fillPer. Returns true iff the attempt succeeded.

      bool fillNetReq (
            WitPart * thePart, 
            WitPeriod fillPer, 
            double    theReqVol);
         //
         // Attempts to meet theReqVol units of requirement of thePart in
         // fillPer by explosion. Returns true iff the attempt succeeded.

      bool fillNetReqBASS (
            WitPart * thePart, 
            WitPeriod fillPer, 
            double    netReqVol);
         //
         // Similar to fillNetReq.
         // Valid only in sel-split mode.

      bool explodePart (
            WitPart * thePart, 
            WitPeriod expPer,
            double    expVol,
            WitPeriod fillPer);
         //
         // Attempts to explode expVol units thru thePart in period expPer, to
         // fill demand in fillPer. 
         // Returns true iff the explosion was successful.

      bool explodePartSS (
            WitPart * thePart, 
            WitPeriod expPer,
            double    expVol,
            WitPeriod fillPer);
         //
         // Performs selection-split explosions of expVol units of thePart 
         // in expPer in order to meet requirements in fillPer.
         // Valid only in sel-split mode.

      bool explodePartPRSS (
            WitPart * thePart, 
            WitPeriod expPer,
            double    expVol,
            WitPeriod fillPer);
         //
         // Explodes expVol units of thePart in expPer in order to meet
         // requirements in fillPer, for prop-routing selection splitting.

      bool explodePartMRSS (
            WitPart * thePart, 
            WitPeriod expPer,
            double    expVol,
            WitPeriod fillPer);
         //
         // Explodes expVol units of thePart in expPer in order to meet
         // requirements in fillPer, for multi-route selection splitting.

      bool explodePartPR (
            WitPart * thePart,
            WitPeriod expPer,
            double    expVol,
            WitPeriod fillPer);
         //
         // Performs a proportionate explosion of expVol units of thePart 
         // in expPer in order to meet requirements in fillPer.
         // Valid only in proportionate routing mode.

      bool explodeBopEntSS (
            WitBopEntry * theBopEnt,
            WitPeriod     expPer,
            double        expVol,
            WitPeriod     fillPer);
         //
         // Explodes theBopEnt in expPer.
         // Valid only in sel-split mode.

      bool explodeBopEntMESS (
            WitBopEntry * theBopEnt,
            WitPeriod     expPer,
            double        expVol,
            WitPeriod     fillPer);
         //
         // Explodes theBopEnt in expPer, using sel-split for multi-exec.

      bool explodeBopEnt (
            WitBopEntry * theBopEnt,
            WitPeriod     execPer,
            double        qty,
            WitPeriod     fillPer);
         //
         // Attempts to explode qty units thru theBopEnt by executing in period
         // execPer, to fill demand in fillPer. 
         // Returns true iff the explosion was successful.

      bool checkExecBound (
            WitBopEntry * theBopEnt,
            WitPeriod     execPer,
            double        oldExecVol,
            double        deltaExecVol,
            WitPeriod     fillPer);
         //
         // Returns true, iff exploding thru theBopEnt by increasing
         // the Operation's execVol in execPer from oldExecVol by deltaExecVol,
         // to fill demand in fillPer would satisfy the appropriate upper bound
         // on execVol.  
         // In the false case, records the failure, if appropriate.

      void recClShortage (WitPart * thePart, WitPeriod fillPer);
         //
         // Records a shortage of thePart in fillPer for the critical list and
         // the pegged critical list.

      void innerExplodeBopEnt (
            WitBopEntry * theBopEnt,
            WitPeriod     execPer,
            double        qty,
            double        deltaExecVol,
            WitPeriod     fillPer);
         //
         // Explodes qty units thru theBopEnt by executing deltaExecVol units in
         // execPer, to fill demand in fillPer. 

      void explodeBopEntries (
            WitBopEntry * theBopEnt,
            WitPeriod     execPer,
            double        deltaExecVol);
         //
         // Does the BopEntry accounting for exploding deltaExecVol units 
         // through theBopEnt in execPer.

      void netAndRollReq (
            WitMaterial * theMat,
            WitPeriod     thePer,
            double &      netReqVol,
            bool &        bddSsrSplit);
         //
         // Nets netReqVol for against the availability of theMat in thePer 
         // and rolls netted amount to the next period. If the rollVol is for a
         // bounded split of SSR, sets bddSsrSplit to true.
         // Valid only in stock reallocation mode.

      void explodeBomEntry (
            WitBomEntry * theBomEnt,
            WitPeriod     execPer,
            double        deltaExecVol,
            WitPeriod     fillPer,
            WitBopEntry * theBopEnt);
         //
         // Explodes deltaExecVol units along theBomEnt in exec period execPer,
         // for filling demand in fillPer, exploding thru theBopEnt.

      void explodeBomEntryMRSS (
            WitBomEntry * theBomEnt,
            WitPeriod     execPer,
            double        deltaExecVol,
            WitPeriod     fillPer,
            WitBopEntry * theBopEnt);
         //
         // Explodes deltaExecVol units along theBomEnt in execPer, for filling
         // demand in fillPer, exploding thru theBopEnt, for multi-route
         // selection splitting.

      void netConsEntries (
            WitBomEntry * theBomEnt,
            WitPeriod     execPer,
            double &      netExecQty,
            WitPeriod     fillPer,
            WitBopEntry * theBopEnt);
         //
         // Nets the requirement for theBomEnt using both theBomEnt and
         // its SubEntries.

      void netSubEntry (
            WitSubEntry * theSub,
            WitPeriod     execPer,
            double &      netExecQty,
            WitPeriod     fillPer,
            WitBopEntry * theBopEnt);
         //
         // Nets the requirement of an Operation along a SubEntry.

      void netConsEntry (
            WitConsEntry * theConsEnt,
            WitPeriod      execPer,
            double &       netExecQty,
            WitPeriod      fillPer,
            WitBopEntry *  theBopEnt);
         //
         // Nets the requirement of an Operation along a ConsEntry.

      void explodeBomEntryPRSS (
            WitBomEntry * theBomEnt,
            WitPeriod     execPer,
            double        deltaExecVol,
            WitPeriod     fillPer,
            WitBopEntry * theBopEnt);
         //
         // Does a split proportionate explosion of deltaExecVol units along
         // theBomEnt in exec period execPer, filling demand in fillPer,
         // exploding thru theBopEnt.

      void explodeBomEntryPR (
            WitBomEntry * theBomEnt,
            WitPeriod     execPer,
            double        deltaExecVol,
            WitPeriod     fillPer,
            WitBopEntry * theBopEnt);
         //
         // Does a proportionate explosion of deltaExecVol units along theBomEnt
         // in exec period execPer, filling demand in fillPer, exploding thru
         // theBopEnt.

      void explodeConsEntry (
            WitConsEntry * theConsEnt,
            WitPeriod      execPer,
            double         netExecQty,
            WitPeriod      fillPer,
            WitBopEntry *  theBopEnt);
         //
         // Explodes netExecQty units along theConsEnt in exec period execPer
         // for filling demand in fillPer, exploding thru theBopEnt.

      void explodeSub (
            WitConsEntry * theConsEnt,
            WitPeriod      execPer,
            double         netExecQty);
         //
         // Performs whatever additional actions are appropriate for exploding
         // netExecQty units thru theConsEnt in execPer, when theConsEnt is a
         // SubEntry, if necessary.

      inline bool critListMode () 
         {
         return (myHeurCritList_ != NULL);
         }

      inline bool pclMode () 
         {
         return (myPclBldr_ != NULL);
         }

      WitBopEntry * expBopEntry (WitPart * thePart, WitPeriod expPer);
         //
         // Returns the BopEntry to be used for explosions in order to produce
         // thePart in expPer.

      WitPeriod execPerFor (WitBopEntry * theBopEnt, WitPeriod expPer);
         //
         // Returns the execution period to be used when exploding through 
         // theBopEnt in expPer.

      //------------------------------------------------------------------------

      accessNonNull (WitHeurCritList *, myHeurCritList)
      accessNonNull (WitPclBldr *,      myPclBldr)
      accessNonNull (WitMrMgr *,        myMrMgr)
      accessNonNull (WitPrMgr *,        myPrMgr)
      accessNonNull (WitBaMgr *,        myBaMgr)
      accessNonNull (WitMeMgr *,        myMeMgr)
      accessNonNull (WitPenExMgr *,     myPenExMgr)
      accessNonNull (WitSglSrcMgr *,    mySglSrcMgr)

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Associations.
      //------------------------------------------------------------------------

      WitAvailSched * myAvailSched_;
         //
         // The AvailSched owned by this HeurAtor.

      WitReqSched * myReqSched_;
         //
         // The requirement schedule for this HeurAtor.

      WitHeurCritList * myHeurCritList_;
         //
         // The HeurCritList owned by this HeurAtor, if any; otherwise NULL.

      WitPclBldr * myPclBldr_;
         //
         // The PclBldr owned by this HeurAtor in pegged crit list mode;
         // otherwise NULL.

      WitHeurModifier * myHeurModifier_;
         //
         // The HeurModifier owned by this HeurAtor, during modifiable heuristic
         // allocation; otherwise NULL.

      WitSelector * mySelector_;
         //
         // The Selector owned by this HeurAtor, if any; otherwise NULL.

      WitMrMgr * myMrMgr_;
         //
         // The MrMgr associated with this HeurAtor, if multiRoute_ is true;
         // otherwise NULL.

      WitPrMgr * myPrMgr_;
         //
         // The PrMgr associated with this HeurAtor, in new propRtg mode;
         // otherwise NULL.

      WitBaMgr * myBaMgr_;
         //
         // The BaMgr associated with this HeurAtor, if buildAhead_ is true;
         // otherwise NULL.

      WitMeMgr * myMeMgr_;
         //
         // The MeMgr associated with this HeurAtor, if multiExec_ is true;
         // otherwise NULL.

      WitSsrMgr * mySsrMgr_;
         //
         // The SsrMgr associated with this HeurAtor, if any; otherwise NULL.

      WitPenExMgr * myPenExMgr_;
         //
         // The PenExMgr associated with this HeurAtor, if penExec_ is true;
         // otherwise NULL.

      WitSglSrcMgr * mySglSrcMgr_;
         //
         // The SglSrcMgr associated with this HeurAtor, if sglSrc_ is
         // true; otherwise NULL.
 
      WitPegger * myPegger_;
         //
         // The Pegger for this HeurAtor, if perfPegging_ is true; 
         // otherwise NULL.

      WitDataRepos * tempCommRepos_;
         //
         // Saves values during a temp commit.

      //------------------------------------------------------------------------
      // Other private member data.
      //------------------------------------------------------------------------

      bool multiRoute_;
         //
         // true, iff the multiple routes technique is to be performed.

      bool buildAhead_;
         //
         // true, iff build-ahead by Material is to be performed.

      bool multiExec_;
         //
         // true, iff the multiple execution periods technique is to be 
         // performed.

      bool penExec_;
         //
         // true, iff penalized execution is to be performed.

      bool selSplit_;
         //
         // true, iff selection splitting is to be performed.

      bool sglSrc_;
         //
         // true, iff new single-source is to be performed.

      bool perfPegging_;
         //
         // true, iff pegging is to be performed.

      bool userHeurStart_;
         //
         // true, iff user-specified heuristic starting solution is to be used.

      bool finiteExecBounds_;
         //
         // true, iff myProblem () includes any execBounds with finite hardUBs.

      bool leadTimeBounds_;
         //
         // true, iff the lead time bounds capability is to be used.

      int ltbMinPer_;
         //
         // If leadTimeBounds_ is true, then ltbMinPer_ is the earliest period
         // eligible for explosion in the current incHeurAlloc.

      bool tempCommActive_;
         //
         // true, iff heuristic allocation is currently performing a temp
         // commit.

      bool permCommActive_;
         //
         // true, iff heuristic allocation is currently performing a perm
         // commit.

      WitSchedule <WitPart, WitPeriod> blocked_;
         //
         // blocked_ (thePart)[thePer] is true, iff an attempt to meet external
         // demand for thePart in thePer has failed.
         // Only allocated for Parts with positive demand.

      WitMapping <WitBomEntry, bool> hasSubsToNet_;
         //
         // hasSubsToNet_ (theBomEnt) is true, iff theBomEnt has at least one
         // substitute for which netAllowed is true.

      double searchInc_;
         //
         // Search increment for the current Demand, if any;
         // otherwise 1.0.

      WitPart * topPart_;
         //
         // The Part for which commitMaxQty was called, if any, else NULL.

      WitPeriod topPer_;
         //
         // The Period for which commitMaxQty was called, if any, else -1.

      int nMaxCommits_;
         //
         // # calls to commitMaxQty.

      int nSelCommits_;
         //
         // # calls to commitMaxQtySel.

      int nBsearches_;
         //
         // # binary searches.

      int nTempComms_;
      int nPermComms_;
      int nComms_;
         //
         // # calls to:
         //    tempCommit
         //    permCommit
         //    commit

      bool failedOn1_;
         //
         // true, iff the final failing temp-commit was for one unit.
   };

#endif
