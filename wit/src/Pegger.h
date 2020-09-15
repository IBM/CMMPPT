//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef PeggerH
#define PeggerH

//------------------------------------------------------------------------------
// Header file: "Pegger.h"
//
// Contains the declaration of the following class Pegger.
//------------------------------------------------------------------------------

#include <ObjStack.h>
#include <PairStack.h>
#include <PtrSched.h>
#include <Assoc.h>

//------------------------------------------------------------------------------
// Class Pegger
//
// Responsible for creating and maintaining the pegging of execVols and subVols
// to Demands in heuristic allocation.
//
// Implemented in Pegging.C.
//------------------------------------------------------------------------------

class WitPegger: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitPegger (WitProblem *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitPegger ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void setUpPeggedHeurAlloc (WitHeurAtor *);
         //
         // Sets up pegging for the beginning of heuristic allocation.

      void setUpPeggedIncAlloc (WitDemand * theDemand, WitPeriod shipPer);
         //
         // Sets up pegging for an incHeurAlloc for theDemand in shipPer.

      void pegDeltaVol (
            WitOperation * theOpn,
            WitPeriod      execPer,
            double         deltaVol);
         //
         // Records the following event: 
         //    theOpn->execVol_[execPer] += deltaVol

      void pegDeltaVol (
            WitSubEntry * theSub,
            WitPeriod     execPer,
            double        deltaVol);
         //
         // Records the following event: 
         //    theSub->subVol_ [execPer] += deltaVol

      void shutDownPeggedIncAlloc ();
         //                                
         // Shuts down pegging for an incHeurAlloc.

      void shutDownPeggedHeurAlloc ();
         //
         // Shuts down pegging for the end of heuristic allocation.

      void clear ();
         //
         // Puts the pegging into its initial state.

      void print ();
         //
         // Prints the pegging.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      void pegDeltaVol (
            WitDelComp * theDelComp,
            WitPeriod    execPer,
            WitPegEl *   thePegEl,
            double       deltaVol);
         //
         // Let theOpn or theSub = theDelComp, as appropriate.
         // Records the following event: 
         //    theOpn->execVol_[execPer] += deltaVol, or
         //    theSub->subVol_ [execPer] += deltaVol
         // thePegEl is the correponding PegEl, if one exists.

      void deleteZeroPegEls ();
         //
         // Deletes the PegEls in myPegElStack_ whose peggedVol is 0.

      void printOpnPegEls (const WitObjStack <WitFixedPegEl> & theFixedPegEls);
         //
         // Prints theFixedPegEls, which must be for Operations.

      void printSubPegEls (const WitObjStack <WitFixedPegEl> & theFixedPegEls);
         //
         // Prints theFixedPegEls, which must be for Subs.

      WitPegEl * & curPegElsPtrFor (WitPegEl * thePegEl);
         //
         // Returns (by ref) the ptr in curOpnPegEls_ or curSubPegEls_ for
         // thePegEl.

      noCopyCtorAssign (WitPegger);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitHeurAtor * myHeurAtor_;
         //
         // The HeurAtor for this Pegger, when active; otherwise NULL.

      WitDemand * curDemand_;
         //
         // The Demand at which pegging is currently being performed, if any;
         // otherwise NULL.

      WitPeriod curShipPer_;
         //
         // The shipPer for which pegging is currently being performed, if any;
         // otherwise -1.

      WitObjStack <WitPegEl> myPegElStack_;
         //
         // Working version of the PegEl Stack for curDemand_ and curShipPer_.

      WitPtrSched <WitOperation, WitPegEl> curOpnPegEls_;
         //
         // curOpnPegEls_.myPtrAt (theOpn, execPer) is the PegEl for 
         // theOpn and execPer that pegs to the current Demand and 
         // shipPer, if any; otherwise NULL.
         // Only allocated when this Pegger is active.

      WitPtrSched <WitSubEntry, WitPegEl> curSubPegEls_;
         //
         // curSubPegEls_.myPtrAt (theSub, execPer) is the PegEl for 
         // theSub and execPer that pegs to the current Demand and 
         // shipPer, if any; otherwise NULL.
         // Only allocated when this Pegger is active.

      WitPairStack <WitDemand, WitPeriod> nonEmptyDemPers_;
         //
         // The set of pairs (theDemand, shipPer) such that
         // theDemand->myPegElsFor_ (shipPer) is non-empty.
   };

#endif
