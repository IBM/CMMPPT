//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef PipPartReqMgrH
#define PipPartReqMgrH

//------------------------------------------------------------------------------
// Header file: "PipPartReqMgr.h"
//
// Contains the declaration of class PipPartReqMgr.
//------------------------------------------------------------------------------

#include <Assoc.h>
#include <PtrTVec.h>
#include <TripStack.h>

//------------------------------------------------------------------------------
// Class PipPartReqMgr
//
// "PIP Part Requirement Manager"
// Responsible for working with the requirements associated with the Part
// currently being pegged (which is called "the current Part").
//------------------------------------------------------------------------------

class WitPipPartReqMgr: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitPipPartReqMgr (WitPipReqMgr *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitPipPartReqMgr ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void setUpPartReqs (WitPart * thePart);
         //
         // Prepares the requirements on thePart for pegging.

      void mergeCurPartReqsIntoWorkSeq (WitPeriod thePer);
         //
         // Takes the requirements for the current part in thePer and merges
         // them in the working requirement sequence.

      bool workSeqIsEmpty ();
         //
         // Returns true, iff the working requirement sequence is currently
         // empty.

      void getNextReq (
            double &           netAttVol,
            WitPipReqBasis * & theBasis,
            double &           incPegVol);
         //
         // On return, theBasis and incPegVol will specify the next
         // requirement to be pegged, which is bounded by netAttVol.
         // This requirement is removed from the working requirement sequence
         // and debited from netAttVol.

      void rollReqsBack (WitPeriod thePer);
         //
         // Rolls the requirements from thePer to thePer - 1, constrained by
         // the net stockVol in thePer - 1. Reduces the net stockVol in 
         // thePer - 1 accordingly and stores any unrolled requirements.

      void rollReqsFwd (WitPeriod thePer);
         //
         // Rolls the requirements from thePer - 1 to thePer. Increases
         // the net stockVol in thePer - 1 accordingly and merges the
         // requirements with the requirements that could not be rolled backward
         // from thePer in the backward pass.

      void restoreUnfilledCurPartReqs (WitPeriod thePer);
         //
         // For each requirement in the working requirement sequence whose fill
         // period is thePer, this function removes the requirement from the
         // working requirement sequence and places it back in the requirements
         // for the current Part in thePer.

      void prtWorkReqSeq (
            const char * theAttname,
            WitPart *    thePart,
            WitPeriod    thePer);
         //
         // Prints the working requirement sequence, just before pegging the
         // attribute given by theAttname for thePart in thePer.

      void prtPeg (
            double           pegVol,
            WitPeriod        fillPer,
            WitPipReqBasis * theBasis);
         //
         // Prints data for pegging pegVol units to theBasis, where the
         // requirement is from fillPer.

      void recordInfeas (WitPart * thePart);
         //
         // Records all unfilled requirements for thePart as infeasibility.

      void prtInfeas ();
         //
         // Prints infeasibility data, as appropriate.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      void compNonExcStockVol (WitMaterial * theMat);
         //
         // Sets netStockVol_[thePer] to the portion of
         // theMat->stockVol ()[thePer] that does not come from excess
         // supplyVol.

      void loadCurPartReqSeqs (WitPart * thePart);
         //
         // Transfers the requirements for thePart in each period to
         // curPartReqSeq_, sorting and consolidating the lists, as well.

      void popOrNull (
            WitPipReqSeq &     theReqSeq,
            WitPipReqBasis * & theBasis,
            WitPeriod &        fillPer,
            double &           incReqVol);
         //
         // Pops theBasis, fillPer, and incReqVol from theReqSeq, or sets
         // them to NULL, -1 and 0.0, if theReqSeq is empty.

      noCopyCtorAssign (WitPipPartReqMgr);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitPipReqMgr * const myReqMgr_;
         //
         // The PipReqMgr for this PipPartReqMgr.

      bool curPartHasStock_;
         //
         // True, iff the current Part is a Material with a positive stockVol in
         // some period.

      WitTVec <double> netStockVol_;
         //
         // netStockVol_[thePer] is the portion of the stockVol of the current
         // Part in thePer that is available for pegging.

      WitPtrTVec <WitPipReqSeq> curPartReqSeq_;
         //
         // The vector of requirement sequences for the current Part. For each
         // period, fillPer, curPartReqSeq_[fillPer} is a list of triples,
         // (theBasis, fillPer, incReqVol), indicating that incReqVol units
         // of requirement for the the current Part in fillPer are pegged to
         // theBasis.

      WitPipReqSeq workReqSeq_;
         //
         // The working requirement sequence for the current Part in some
         // Period.

      WitPtrTVec <WitPipReqSeq> overStockReqSeq_;
         //
         // For each period, thePer > 0, overStockReqSeq_[thePer] is portion of
         // workReqSeq_ that could not be rolled to thePer - 1, due to
         // insufficient available stock in thePer - 1.

      //------------------------------------------------------------------------
      // Maximum infeasibility data.
      //------------------------------------------------------------------------

      double maxInfeas_;
         //
         // The maximum infesibility encountered.

      WitPart * maxInfeasPart_;
      WitPeriod maxInfeasPer_;
         //
         // The Part and period at which maxInfeas_ was encountered.
   };

#endif
