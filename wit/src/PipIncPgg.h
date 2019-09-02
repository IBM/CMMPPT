//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef PipIncPggH
#define PipIncPggH

//------------------------------------------------------------------------------
// Header file: "PipIncPgg.h"
//
// Contains the declaration of template class PipIncPgg <RcpComp>.
//------------------------------------------------------------------------------

#include <Assoc.h>
#include <Schedule.h>
#include <PairStack.h>

//------------------------------------------------------------------------------
// Class template PipIncPgg <RcpComp>
//
// "PIP Incremental Pegging"
// Represents the set of incremental changes that are to be made to a pegging to
// a recipient case corresponding to a single pegging source.
//
// Implemented in PipPgg.C.
//------------------------------------------------------------------------------

template <typename RcpComp> 
      class WitPipIncPgg: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitPipIncPgg (WitProblem *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitPipIncPgg ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void addToIncPegVol (double pegVol, WitPipReqBasis * theBasis);
         //
         // Prepares to peg pegVol units to all of the recipients in theBasis.

      bool getIncPggTrip (
            RcpComp * & theRcpComp,
            WitPeriod & rcpPer,
            double &    pegVol);
         //
         // If this PipIncPgg contains at least one incremental pegging triple,
         // this function:
         //    Sets the arguments to one such triple.
         //    Removes the triple from this PipIncPgg.
         //    Returns true.
         // Otherwise, this function returns false.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      static bool advanceClientItr (
            WitPtrVecItr <WitPipReqClient <RcpComp> > & theClientItr,
            RcpComp * &                                 theRcpComp,
            WitPeriod &                                 rcpPer);
         //
         // Advances theClientItr.
         // Returns true, iff another non-vacuous ReqClient was found.
         // If one is found, theRcpComp and rcpPer are set to the pair.

      noCopyCtorAssign (WitPipIncPgg);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitSchedule <RcpComp, double> incPegVol_;
         //
         // incPegVol_ (theRcpComp)[rcpPer] is the amount of the current 
         // attribute for the current SrcComp in the current pegging period that
         // is to be pegged to theRcpComp in rcpPer.

      WitSchedule <RcpComp, bool> addedTo_;
         //
         // During a call to addToIncPegVol (...),
         // addedTo_ (theRcpComp)[rcpPer] is true, iff the function has
         // already added to incPegVol_ (theRcpComp)[rcpPer].

      WitPairStack <RcpComp, WitPeriod> posRcpPairs_;
         //
         // The set of pairs (theRcpComp, rcpPer) such that
         // incPegVol_ (theRcpComp)[rcpPer] > 0.
   };

#endif
