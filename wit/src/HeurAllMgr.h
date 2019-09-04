//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef HeurAllMgrH
#define HeurAllMgrH

//------------------------------------------------------------------------------
// Header file: "HeurAllMgr.h"
//
// Contains the declaration of class HeurAllMgr.
//------------------------------------------------------------------------------

#include <Assoc.h>

//------------------------------------------------------------------------------
// class HeurAllMgr
//
// This class provides the interface between heuristic allocation and the rest
// of WIT. An instance of this class exists of long as the Problem that owns it
// exists.
//
// Implemented in HeurAll.C.
//------------------------------------------------------------------------------

class WitHeurAllMgr: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitHeurAllMgr (WitProblem * theProblem);
         //
         // ProbAssoc <<< theProblem.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitHeurAllMgr ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void setUpPegging ();
         //
         // Takes action appropriate for when myGlobalComp ()->perfPegging_
         // has just been set to true.

      void clearPegging ();
         //
         // Puts the pegging into its initial state.

      void shutDownPegging ();
         //
         // Takes action appropriate for when myGlobalComp ()->perfPegging_
         // has just been set to false.

      void start ();
         //
         // Begins heuristic allocation.

      void startForOpt ();
         //
         // Begins heuristic allocation for optimizing implosion.

      double incHeurAlloc (
            WitDemand * theDemand, 
            WitPeriod   thePer, 
            double      desIncVol,
            bool        asapMultiExec);
         //
         // Increments the heuristic allocation.
         // Attempts to increase theDemand->shipVol ()[thePer] by as much as is
         // feasible up to desIncVol.
         // Issues an error if heuristic allocation is not active.
         // In two-way multi-exec mode, ASAP Multi-Exec will be used for
         // top-level JIT demand, iff asapMultiExec is true. When not in two-way
         // multi-exec mode, asapMultiExec must be false.

      void reset ();
         //
         // Terminates heuristic allocation.

      void finish ();
         //
         // Completes heuristic allocation.

      void printMeDir (bool asapMultiExec);
         //
         // Prints a multi-exec direction.

      void printPegging ();
         //
         // Prints the pegging.

      void printAvailSched ();
         //
         // Prints the availability schedule.

      bool modifiable ();
         //
         // Returns true, iff modifiable heuristic allocation is currently
         // active.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (bool, updatingSoln)

      inline bool active ()
         {
         return (myHeurAtor_ != NULL);
         }

      WitHeurModifier * myHeurModifier ();

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitHeurAllMgr);

      void start (bool forOpt);
         //
         // Begins heuristic allocation.
         // forOpt == true indicates heur allocation is for opt implosion.

      accessNonNull (WitHeurAtor *, myHeurAtor)

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Other private member data.
      //------------------------------------------------------------------------

      WitHeurAtor * myHeurAtor_;
         //
         // The HeurAtor that performs heuristic allocation for this HeurAllMgr,
         // if heuristic allocation is active; otherwise NULL.

      WitPegger * myPegger_;
         //
         // The Pegger owned by this HeurAllMgr, if
         // myGlobalComp ()->perfPegging_ is true; otherwise NULL.

      bool updatingSoln_;
         //
         // true, iff heuristic allocation is currently updating the implosion
         // solution.
   };

#endif
