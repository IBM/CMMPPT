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

#ifndef HeurModifierH
#define HeurModifierH

//------------------------------------------------------------------------------
// Header file: "HeurModifier.h"
//
// Contains the declaration of class HeurModifier.
//------------------------------------------------------------------------------

#include <Assoc.h>
#include <PerSglStack.h>
#include <ObjStack.h>
#include <Mapping.h>

//------------------------------------------------------------------------------
// Class HeurModifier
//
// "Heuristic Modifier"
// Responsible for implementing the modifiable heuristic allocation capability.
//------------------------------------------------------------------------------

class WitHeurModifier: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitHeurModifier (WitAvailSched *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitHeurModifier ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void modifyForExecVol (
            WitOperation *       theOpn,
            const WitIVRealArg & theVec);
         //
         // Modifies the state of heuristic allocation for setting
         // theOpn->execVol () to theVec.

      void modifyForSubVol (
            WitSubEntry *        theSub,
            const WitIVRealArg & theVec);
         //
         // Modifies the state of heuristic allocation for setting
         // theSub->subVol () to theVec.

      void modifyForShipVol (
            WitDemand *          theDemand,
            const WitIVRealArg & theVec);
         //
         // Modifies the state of heuristic allocation for setting
         // theDemand->shipVol () to theVec.

      void checkFeasibility ();
         //
         // Performs feasibility checking.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      void checkPreconditions ();
         //
         // Verifies that the required preconditions for modifiable heuristic
         // allocation are satisfied.

      //------------------------------------------------------------------------

      void recNeedsForExecVol (
            WitOperation *       theOpn,
            const WitIVRealArg & theVec);
         //
         // Records all necessary feasibility checking for setting
         // theOpn->execVol () to theVec.

      void recNeedsForSubVol (
            WitSubEntry *        theSub,
            const WitIVRealArg & theVec);
         //
         // Records all necessary feasibility checking for setting
         // theSub->subVol () to theVec.

      void recNeedsForShipVol (
            WitDemand *          theDemand,
            const WitIVRealArg & theVec);
         //
         // Records all necessary feasibility checking for setting
         // theDemand->shipVol () to theVec.

      //------------------------------------------------------------------------

      void recPartPairNeed (WitPart * thePart, WitPeriod thePer);
         //
         // Records that feasibility checking is needed for thePart in thePer.

      //------------------------------------------------------------------------

      void modAvailForExecVol (
            WitOperation *       theOpn,
            const WitIVRealArg & theVec);
         //
         // Modifies the availability schedule for setting theOpn->execVol () to
         // theVec.

      void modAvailForSubVol (
            WitSubEntry *        theSub,
            const WitIVRealArg & theVec);
         //
         // Modifies the availability schedule for setting theSub->subVol () to
         // theVec.

      void modAvailForShipVol (
            WitDemand *          theDemand,
            const WitIVRealArg & theVec);
         //
         // Modifies the availability schedule for setting theDemand->shipVol ()
         // to theVec.

      //------------------------------------------------------------------------

      void checkCapPairs ();
         //
         // Performs feasibility checking on all needed (Operation, Period)
         // pairs.

      void checkMats ();
         //
         // Performs feasibility checking on all needed Materials.

      void checkOpnPairs ();
         //
         // Performs feasibility checking on all needed (Operation, Period)
         // pairs.

      void checkBomEntPairs ();
         //
         // Performs feasibility checking on all needed (BomEntry, Period)
         // pairs.

      void checkDemands ();
         //
         // Performs feasibility checking on all needed Demands.

      //------------------------------------------------------------------------

      void checkScrapCon (WitCapacity * theCap, WitPeriod thePer);
         //
         // Checks the scrapVol constraint for theCap in thePer.

      void checkScrapCon (WitMaterial * theMat);
         //
         // Checks the scrapVol constraint for theMat in all periods.

      void checkNonExecCon (WitOperation * theOpn, WitPeriod execPer);
         //
         // Checks the non-executable constraint for theOpn in execPer.

      void checkExecHUBCon (WitOperation * theOpn, WitPeriod execPer);
         //
         // Checks the hard upper bound constraint for theOpn in execPer.

      void checkLotSizeCon (WitOperation * theOpn, WitPeriod execPer);
         //
         // Checks the lot size constraint for theOpn in execPer.

      void checkSubCon (WitBomEntry * theBomEnt, WitPeriod execPer);
         //
         // Checks the substitution constraint for theBomEnt in execPer.

      void checkCumShipCon (WitDemand * theDemand);
         //
         // Checks the cumulative shipment constraint for theDemand in all
         // periods.

      //------------------------------------------------------------------------

      noCopyCtorAssign (WitHeurModifier);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitAvailSched * const myAvailSched_;
         //
         // The AvailSched for this HeurModifier.

      WitPerSglStack <WitPart>      toChkCapPairs_;
      WitPerSglStack <WitOperation> toChkOpnPairs_;
      WitPerSglStack <WitBomEntry>  toChkBomEntPairs_;
         //
         // toChkCapPairs_ is the set of pairs (theCap, thePer) for which
         // feasibility checking is needed. The Capacities are stored as Parts.
         //
         // toChkOpnPairs_ is the set of pairs (theOpn, execPer) for which
         // feasibility checking is needed.
         //
         // toChkBomEntPairs_ is the set of pairs (theBomEnt, execPer) for which
         // feasibility checking is needed.

      WitObjStack <WitMaterial> toChkMats_;
      WitObjStack <WitDemand>   toChkDemands_;
         //
         // toChkMats_ is the set of Materials for which 
         // feasibility checking is needed.
         //
         // toChkDemands_ is the set of Demands for which 
         // feasibility checking is needed.

      WitMapping <WitMaterial, bool> chkNeededMat_;
      WitMapping <WitDemand,   bool> chkNeededDemand_;
         //
         // chkNeededMat_ (theMat) is true, iff feasibility checking is
         // needed for theMat.
         //
         // chkNeededDemand_ (theDemand) is true, iff feasibility checking is
         // needed for theDemand.
   };

#endif
