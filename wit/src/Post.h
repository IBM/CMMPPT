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

#ifndef PostH
#define PostH

//------------------------------------------------------------------------------
// Header file: "Post.h"
//
// Contains the declaration of class Postprocessor.
//------------------------------------------------------------------------------

#include <Assoc.h>

//------------------------------------------------------------------------------
// class Postprocessor
//
// Responsible for performing postprocessing.
//------------------------------------------------------------------------------

class WitPostprocessor: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitPostprocessor (WitProblem * theProblem);
         //
         // ProbAssoc <<< theProblem.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitPostprocessor ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void postprocess ();
         //
         // Performs postprocessing based on the current production and
         // shipment schedules and myProblem ()->currentVariant ().

      void unpostprocess ();
         //
         // Does unpostprocessing for myProblem ().

      void compNonSubVol (
            WitBomEntry *      theBomEnt,
            WitTVec <double> & nonSubVolVec);
         //
         // Computes the value for theBomEnt->nonSubVol_[execPer] and stores it
         // in nonSubVolVec.

      void compProdVol (WitPart * thePart, WitTVec <double> & prodVolVec);
         //
         // Computes the value for thePart->prodVol and stores it in prodVolVec.

      double compExcessVol (WitPart * thePart, WitPeriod thePer);
         //
         // Computes and returns the excessVol for thePart in thePer.
         // This is the amount by which thePart->supplyVol ()[thePer] could be
         // decreased while keeping the solution feasible.
         // Valid only in postprocessed mode.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (bool,          postprocessing)
      accessFunc (bool,          postprocessed)
      accessFunc (WitFeasChkr *, myFeasChkr)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitPostprocessor);

      void compNonSubVols ();
         //
         // Computes nonSubVol_ for each BomEntry.

      void compConsVols ();
         //
         // Computes consVol for each Part.

      void compProdVols ();
         //
         // Computes prodVol for each Part.

      void compCapacityScrapVols ();
         //
         // Computes scrapVol for each Capacity.

      void compMaterialStockScrapVols ();
         //
         // Computes stockVol and scrapVol for each Material.

      double unstockedScrapVol (WitPart * thePart, WitPeriod t);
         //
         // Computes and returns what thePart->scrapVol ()[t] would be if
         // stocking were not allowed. (May return a negative number.)

      void compCumShipVols ();
         //
         // Computes cumShipVol_ for each Demand.

      void compCapResidVols ();
         //
         // Computes residualVol_ for each Capacity.

      void compMatResidVolsAsap ();
         //
         // Computes residualVol_ for each Material in nstnResidual == false
         // mode.

      void compMatResidVolsNstn ();
         //
         // Computes residualVol_ for each Material in nstnResidual == true
         // mode.

      double compCumConsVol (
            WitMaterial *      theMat, 
            WitTVec <double> & unstockConsVol);
         //
         // Computes and returns the cumulative generalized consumption volume
         // for theMat in period 0.
         // Stores the portion that cannot be carried over as stock in each
         // period in unstockConsVol.

      double stockBndGap (
            WitMaterial * theMat,
            WitPeriod     thePer,
            bool &        gapIsFinite);
         //
         // Returns the gap between the upper and lower bounds and stock for
         // theMat in thePer.
         // On return, gapIsFinite is true, iff the gap is finite.
         //    If it is false, the return valued is to be ignored.

      double stockLB (WitMaterial * theMat, WitPeriod thePer);
         //
         // Returns min (stock softLB, stockVol).

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      bool postprocessing_;
         //
         // true, iff postprocessing is currently being performed.

      bool postprocessed_;
         //
         // true, iff postprocessing has been performed and the
         // input data and implosion solution has not been updated since.

      WitFeasChkr * myFeasChkr_;
         //
         // The FeasChkr owned by this Postprocessor.
   };

#endif
