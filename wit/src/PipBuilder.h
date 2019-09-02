//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef PipBuilderH
#define PipBuilderH

//------------------------------------------------------------------------------
// Header file: "PipBuilder.h"
//
// Contains the declaration of class PipBuilder.
//------------------------------------------------------------------------------

#include <Assoc.h>
#include <Schedule.h>

//------------------------------------------------------------------------------
// Class PipBuilder
//
// "Post-Implosion Pegging Builder"
//
// Responsible for building a post-implosion pegging.
//------------------------------------------------------------------------------

class WitPipBuilder: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitPipBuilder (WitPipPgg *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitPipBuilder ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void buildPegging ();
         //
         // Builds the pegging.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitPipIncPgg <WitDemand>    *, myIncPggToDems)
      accessFunc (WitPipIncPgg <WitOperation> *, myIncPggToOpns)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      void validateData ();
         //
         // Validates the data for PIP.

      void compSideVols (WitSchedule <WitPart, double> & sideVolSched);
         //
         // Computes side-effects volumes and stores them in sideVolSched.

      void compBopSideVols (WitSchedule <WitPart, double> & sideVolSched);
         //
         // Computes side-effects volumes due to BopEntries and stores them in
         // sideVolSched.

      void compBomSideVols (WitSchedule <WitPart, double> & sideVolSched);
         //
         // Computes side-effects volumes due to BomEntries and stores them in
         // sideVolSched.

      void prtSideVols (const WitSchedule <WitPart, double> & sideVolSched);
         //
         // Prints side-effect volumes stored in sideVolSched.

      void placeShipmentReqs ();
         //
         // Places requirement from the shipment sequence.

      void explodeAllReqs ();
         //
         // Explodes all requirements.

      void fillPartReqs (WitPart * thePart);
         //
         // Fills the requirements on thePart.

      void pegToAllAttsNpo (WitPart * thePart);
         //
         // Pegs the requirements on thePart to all attributes.
         // Uses the new pegging order.

      void pegToAtt (
            WitPart *              thePart,
            void (WitPipBuilder::* thePegFunc) (WitPart *, WitPeriod));
         //
         // Pegs the requirements on thePart to one attribute.
         // thePegFunc should be one of the following functions:
         //
         //    netWithSupply
         //    explodeToProd
         //    netWithSideVol

      bool asapPggOrder (WitPart * thePart);
         //
         // Returns true, iff ASAP pegging order is to be used on thePart.

      void pegToAllAttsOpo (WitPart * thePart);
         //
         // Pegs the requirements on thePart to all attributes.
         // Uses the old pegging order.

      void netWithSupply (WitPart * thePart, WitPeriod thePer);
         //
         // Fills (as much as possible) the requirements on thePart in thePer
         // by netting against the supply.

      void explodeToProd (WitPart * thePart, WitPeriod thePer);
         //
         // Fills (as much as possible) the requirements on thePart in thePer
         // by exploding to production.

      void explodeBopEntry (WitBopEntry * theBopEnt, WitPeriod thePer);
         //
         // Fills (as much as possible) the requirements on theBopEnt->myPart ()
         // in thePer by exploding to production.

      void explodeToExecPer (WitBopEntry * theBopEnt, WitPeriod execPer);
         //
         // Fills (as much as possible) the requirements on theBopEnt->myPart ()
         // by exploding to production with execution period execPer.

      void netWithSideVol (WitPart * thePart, WitPeriod thePer);
         //
         // Fills (as much as possible) the requirements on thePart in thePer
         // by netting against the side-effect volume.

      void pegAttToWorkReqs (
            double                   theAttVol,
            WitPipAttPgg <WitPart> * theAttPgg,
            WitPart *                thePart,
            WitPeriod                thePer,
            WitPipReqList *          pgdReqListPtr);
         //
         // Pegs at most theAttVol units of theAttPgg for thePart in thePer to
         // the requirements in the working requirement sequence, removing the 
         // requirements from the sequence. If pgdReqListPtr != NULL, it is set
         // to the list of requirements that were pegged to.
         // Shipments are pegged to.
         // In PIP to Opns mode, Operation pairs are also pegged to.

      void fillOpnReqs (WitOperation * theOpn);
         //
         // Fills the requirements on theOpn.

      void explodeBomEntry (WitBomEntry * theBomEnt, WitPeriod execPer);
         //
         // Explodes the requirements on theBomEnt in execPer.

      void pegSubVols (WitBomEntry * theBomEnt, WitPeriod execPer);
         //
         // Pegs the subVols of the substitutes for theBomEnt in execPer.

      void explodeConsEntry (
            WitConsEntry * theConsEnt,
            WitPeriod      execPer,
            double         totExecVol);
         // 
         // Explodes no more than totExecVol units of the requirements on
         // theConsEnt->myOperation () in execPer through theConsEnt.

      void addToIncPegVol (double pegVol, WitPipReqBasis * theBasis);
         //
         // Prepares to peg pegVol units to all of the shipments (and optionally
         // all of Operation pairs) in theBasis.

      void dispMaxPegRatio ();
         //
         // Computes and prints the maximum pegging ratio data.

      void prtMaxPegRatio (
            double         ratio,
            WitOperation * theOpn,
            WitPeriod      execPer);
         //
         // Prints the maximum pegging ratio data.

      noCopyCtorAssign (WitPipBuilder);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitPipPgg * const myPipPgg_;
         //
         // The PipPgg that owns this PipBuilder.

      WitPipReqMgr * myReqMgr_;
         //
         // The PipReqMgr owned by this PipBuilder.

      WitPipPartReqMgr * myPartReqMgr_;
         //
         // The PipPartReqMgr owned by this PipBuilder.

      WitExecPerSched <WitBopEntry> * myExecPerSched_;
         //
         // The ExecPerSched owned by this PipBuilder.

      WitSchedule <WitPart, double> sideVol_;
         //
         // sideVol_ (thePart)[thePer] is the side-effect volume of thePart in
         // thePer.

      WitPipIncPgg <WitDemand>    * myIncPggToDems_;
      WitPipIncPgg <WitOperation> * myIncPggToOpns_;
         //
         // The incremental peggings to Demands and Operations owned by this
         // PipBuilder.
   };

#endif
