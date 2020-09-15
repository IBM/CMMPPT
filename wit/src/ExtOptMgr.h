//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef ExtOptMgrH
#define ExtOptMgrH

//------------------------------------------------------------------------------
// Header file: "ExtOptMgr.h"
//
// Contains the declaration of the following class:
//
//    ExtOptMgr
//------------------------------------------------------------------------------

#include <Assoc.h>

//------------------------------------------------------------------------------
// Class ExtOptMgr
//
// "External Optimizing Implosion Manager"
// Responsible for performing most of the tasks that are specific to external
// optimizing implosion.
//------------------------------------------------------------------------------

class WitExtOptMgr: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitExtOptMgr (WitDetImpOP *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitExtOptMgr ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void getLpProb (
            WitVector <int> &    start,
            WitVector <int> &    index,
            WitVector <double> & value,
            WitVector <double> & collb,
            WitVector <double> & colub,
            WitVector <double> & obj,
            WitVector <double> & rowlb,
            WitVector <double> & rowub);
         //
         // Retrieves data specifying the LP portion of the optimization
         // problem, using "standard column major ordered format".

      void getIntVarIndices (WitVector <int> & indices);
         //
         // Retrieves a list of the column indices of all of the
         // integer-constrained variables in the optimization problem.

      void setSoln (const WitIVRealArg & theVector);
         //
         // Sets the primal solution to theVector.

      int       scrapVarIndex (WitPart *,      WitPeriod);
      int      nonSubVarIndex (WitBomEntry *,  WitPeriod);
      int         subVarIndex (WitSubEntry *,  WitPeriod);
      int        shipVarIndex (WitDemand *,    WitPeriod);
      int        execVarIndex (WitOperation *, WitPeriod);
      int       stockVarIndex (WitMaterial *,  WitPeriod);
      int     cumShipVarIndex (WitDemand *,    WitPeriod);
      int    execSlbvVarIndex (WitOperation *, WitPeriod);
      int   stockSlbvVarIndex (WitMaterial *,  WitPeriod);
      int cumShipSlbvVarIndex (WitDemand *,    WitPeriod);
         //
         // Each of the above functions returns the colmun index of the
         // indicated opt variable corresponding to the arguments, or -1, if no
         // such variable exists.
         //
         // Indicated variables:
         //    Scrap
         //    Non-Substitution
         //    Substitution
         //    Shipment
         //    Execution
         //    Stock
         //    Cumulative Shipment
         //    Execution           SLBV
         //    Stock               SLBV
         //    Cumulative Shipment SLBV

      int   resourceConIndex (WitPart *,      WitPeriod);
      int        subConIndex (WitBomEntry *,  WitPeriod);
      int       shipConIndex (WitDemand *,    WitPeriod);
      int    execSlbConIndex (WitOperation *, WitPeriod);
      int   stockSlbConIndex (WitMaterial *,  WitPeriod);
      int cumShipSlbConIndex (WitDemand *,    WitPeriod);
         //
         // Each of the above functions returns the row index of the indicated
         // constraint corresponding to the arguments, or -1, if no such
         // constraint exists.
         //
         // Indicated constraints:
         //    Resource Allocation
         //    Substitution
         //    Shipment Cumulation
         //    Execution           SLB
         //    Stock               SLB
         //    Cumulative Shipment SLB

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (bool, solnProvided)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      int slbvVarIndexOf (WitBoundedVar * theBoundedVar);
         //
         // Returns the index of the SlbvVar for theBoundedVar, if any;
         // otherwise -1.

      int slbConIndexOf (WitBoundedVar * theBoundedVar);
         //
         // Returns the index of the SlbCon for theBoundedVar, if any;
         // otherwise -1.

      int indexOf (WitOptVC * theOptVC);
         //
         // Returns theOptVC->index (), or -1, if theOptVC is NULL.

      noCopyCtorAssign (WitExtOptMgr);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitDetImpOP * const myDetImpOP_;
         //
         // The DetImpOP for this ExtOptMgr.

      bool solnProvided_;
         //
         // True, iff a solution has been provides through setSoln.
   };

#endif
