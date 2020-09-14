//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef FeasChkrH
#define FeasChkrH

//------------------------------------------------------------------------------
// Header file: "FeasChkr.h"
//
// Contains the declaration of the following class:
//
//    FeasChkr
//------------------------------------------------------------------------------

#include <Assoc.h>

//------------------------------------------------------------------------------
// Class FeasChkr
//
// "Feasibility Checker"
// Responsible for verifying the feasiblity of an implosion solution.
//
// Class Hierarchy:
//
// ProbAssoc
//    FeasChkr
//------------------------------------------------------------------------------

class WitFeasChkr: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitFeasChkr (WitProblem *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitFeasChkr ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void compFeasible ();
         //
         // Computes maxViolation_, feasible_.

      void unpostprocess ();
         //
         // Takes action appropriate to undoing postprocessing.

      void verifyUhsFeas ();
         //
         // Verifies the feasibility of the current solution, assuming that it
         // is the user-specified heuristic starting solution.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (double, maxViolation)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitFeasChkr);

      void checkFeasReq ();
         //
         // Records as maxViolation: max violation of all "required"
         // contraints, i.e., those that apply regardless of the Variant.

      void checkScrapVolNonneg   ();
      void checkStockVolNonneg   ();
      void checkExecVolNonneg    ();
      void checkSubVolNonneg     ();
      void checkShipVolNonneg    ();
      void checkCumShipVolNonneg ();
         // 
         // Records violations of nonnegativity for each solution attribute.

      void checkStockVols ();
         //
         // Records violations of stockVol == 0, when stock is not allowed.

      void checkExecVols ();
         //
         // Records violations of execVol == 0, when the operation is not
         // executable.

      void checkNonSubVols ();
         //
         // Records violations of nonSubVol >= 0.

      void checkShipVols ();
         //
         // Records violations of shipVol <= demandVol.

      void checkCumShipVols ();
         //
         // Records violations of cumShipVol <= cumDemandVol.

      void checkFeasAllBounds ();
         //
         // Records max bound violation for all bounds.

      void checkFeasLimitedBounds ();
         //
         // Records:
         //    max bound violation for the "limited" subset of bounds
         //    (those respected by heuristic implosion).

      void checkFeasHardLB (WitBoundSet * theBoundSet);
         //
         // Records hard LB violation for theBoundset.

      void checkFeasHardUB (WitBoundSet * theBoundSet);
         //
         // Records hard UB violation for theBoundset.

      void checkFeasLotSizes ();
         //
         // Records max lot size violation.

      void checkDisScrap ();
         //
         // Records max disallowed scrap violation.

      void checkIntegrality ();
         //
         // Records max integrality violation.

      void checkExecVolInt ();
         //
         // Records max execVol integrality violation.

      void checkSubVolInt ();
         //
         // Records max subVol integrality violation.

      void checkShipVolInt ();
         //
         // Records max shipVol integrality violation.

      void recordViolation (double theViolation, bool & reportNeeded);
         //
         // maxViolation_ = max (maxViolation_, theViolation)
         // On return, reportNeeded is true, iff an increase to maxViolation_ is
         // to be reported.

      void verifyUhsNonExecCon ();
      void verifyUhsSubCon ();
      void verifyUhsCumDemandCon ();
      void verifyUhsExecHUBCon ();
      void verifyUhsLotSizeCon ();
         //
         // Each of these function tests the current solution for satisfaction
         // of a specific class of constraints, assuming that the current
         // solution is the user-specified heuristic starting solution.
         // If a violation is found, a warning is issued.
         // Exception: In verifyUhsNonExecCon, if a violation is found, a severe
         // error is issued.

      double lotSizeViol (WitOperation * theOpn, WitPeriod execPer);
         //
         // Computes and returns the amount by which theOpn->execVol ()[execPer]
         // violates the lot-size constraint.

      double integralityViol (double theValue);
         //
         // Returns the distance from theValue to the nearest integer.

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      double maxViolation_;
         //
         // The maximum constraint violation in the solution.
   };

#endif
