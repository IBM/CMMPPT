//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef FSSH
#define FSSH

//------------------------------------------------------------------------------
// Header file: "FSS.h"
//
// Contains the declaration of class FSS.
//------------------------------------------------------------------------------

#include <Assoc.h>
#include <Schedule.h>

//------------------------------------------------------------------------------
// class FSS.
//
// Responsible for computing the Focussed Shortage Schedule.
//------------------------------------------------------------------------------

class WitFSS: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitFSS (WitProblem * theProblem);
         //
         // ProbAssoc <<< theProblem.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitFSS ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (bool, computed)
      accessFunc (bool, uploadingSoln)

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void compute ();
         //
         // Computes the Focussed Shortage Schedule for the current inputs.
         // It is an error to call this function when
         // myProblem ()->postprocessed () is false.

      void compFssShipVol (
            WitDemand *     theDemand,
            WitDblFlexVec & fssShipVolRef);
         //
         // Computes the fssShipVol of theDemand in useFocusHorizons mode and
         // sets fssShipVolRef to it.

      double mrpSupplyVol (WitPart * thePart, WitPeriod thePer) const;
         //
         // Returns the supplyVol for thePart in thePer for the MRP problem.
         // Valid only during MRP for FSS.

      double mrpDemandVol (WitPart * thePart, WitPeriod thePer) const;
         //
         // Returns the demandVol for thePart in thePer for the MRP problem.
         // Valid only during MRP for FSS.

      void uncompute ();
         //
         // Puts this FSS into an uncomputed state.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitFSS);

      void initSoln ();
         //
         // Initializes the FSS solution.

      void defineMrp ();
         //
         // Sets up the data for the MRP problem.

      void defineUnimplode ();
         //
         // Defines the unimplosion problem.

      double deltaFssShipVol (WitPart * thePart, WitPeriod thePer);
         //
         // Returns sum over all demands for thePart in thePer of:
         //    fssShipVol[thePer] - shipVol[thePer]

      void unimplode ();
         //
         // Finds a reduced execution sched for meeting the desired, achieved
         // shipment sched.

      void unimplode (
            WitOperation * uiOpn,
            WitBopEntry *  uiBopEnt,
            WitPart *      uiPart,
            WitPeriod      execPer,
            WitPeriod &    scrapPer);
         //
         // Reduces the execVol of uiOpn in period execPer as appropriate
         // for unimplosion.

      void rollOverScrap (
            WitMaterial * theMat,
            WitPeriod &   scrapPer,
            WitPeriod     prodPer);
         //
         // Rolls scrap from period scrapPer to prodPer as appropriate for
         // unimplosion. scrapPer is updated to prodPer.
         // No-op, if scrapPer >= prodPer.

      void unimplodeBom (
            WitOperation * uiOpn,
            WitPeriod      execPer,
            double         deltaExecVol);
         //
         // Reduces the BomEntries and Subs for uiOpn corresponding to
         // a reduction of deltaExecVol in the execVol in period execPer.

      void unimplodeSubs (
            WitBomEntry * theBomEnt,
            WitPeriod     execPer,
            double        deltaExecVol);
         //
         // Reduces the subs for theBomEnt corresponding to a reduction of
         // deltaExecVol in the execVol of theBomEnt->myOperation () in
         // period execPer.

      void compMrpIncAvail ();
         //
         // Computes mrpIncAvail_.

      void doMrp ();
         //
         // Does MRP for FSS.

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      bool computed_;
         //
         // true iff the Focussed Shortage Schedule has been computed and the
         // input data for FSS has not been updated since then.

      bool uploadingSoln_;
         //
         // True, iff the FSS subsystem is currently uploading the FSS solution.

      WitSchedule <WitPart, double> uiScrapVol_;
         //
         // Scrap sched for unimplode.

      WitSchedule <WitMaterial, double> uiStockVol_;
         //
         // Stock sched for unimplode.

      WitSchedule <WitPart, double> mrpIncAvail_;
         //
         // The incremental availability schedule for the MRP.
         //
         // If mrpIncAvail_ (thePart, thePer) == x > 0.0,
         //    then x additional units of thePart become available in thePer.
         //
         // If mrpIncAvail_ (thePart, thePer) == - x < 0.0,
         //    then x fewer units of thePart are available in thePer than in
         //    thePer - 1.

      //------------------------------------------------------------------------
      // FSS solution Schedules.
      //------------------------------------------------------------------------

      WitSchedule <WitOperation, double> fssExecVol_;
      WitSchedule <WitSubEntry,  double> fssSubVol_;
   };

#endif
