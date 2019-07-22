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

#ifndef OptVCH
#define OptVCH

//------------------------------------------------------------------------------
// Header file: "OptVC.h"
//
// Contains the declaration of class OptVC.
//------------------------------------------------------------------------------

#include <BoundPair.h>
#include <PtrVec.h>

//------------------------------------------------------------------------------
// class OptVC
//
// "Optimization Variable / Constraint"
// A variable or constraint in an optimization problem.
//
// Class hierarchy:
//
//    OptVC
//       OptVar
//          DetVar
//             ScrapVar
//             NonSubVar
//             SubVar
//             ShipVar
//             BoundedVar
//                ExecVar
//                StockVar
//                CumShipVar
//             SlbvVar
//             MultiObjVar
//             NtbvVar
//          StochVar
//             StScrapVar
//             StNonSubVar
//             StSubVar
//             StShipVar
//             StBddVar
//                StExecVar
//                StStockVar
//                StCumShipVar
//             StSlbvVar
//       OptCon
//          DetCon
//             ResourceCon
//             SubCon
//             ShipCon
//             SlbCon
//             MultiObjCon
//             TbvCon
//          StochCon
//             StResCon
//             StSubCon
//             StShipCon
//             StSlbCon
//
// Implemented in OptMisc.C.
//------------------------------------------------------------------------------

class WitOptVC
   {
   public:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitOptVC (WitOptProblem * theOptProblem, int theIndex);
         //
         // index_ <-- theIndex.

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      virtual ~WitOptVC ();

      //-----------------------------------------------------------------------
      // Public pure virtual member functions.
      //-----------------------------------------------------------------------

      virtual WitOptProblem * myOptProblem () = 0;
         //
         // Returns the OptProblem that owns this OptVC.

      virtual double solnValue () = 0;
         //
         // Returns:
         //   primalValue for OptVars.
         //   dualValue () for OptCons.

      //-----------------------------------------------------------------------
      // Public static member functions.
      //-----------------------------------------------------------------------

      static void writeExtReal (
            FILE * outFile,
            const char * fFormat,
            const char * sFormat,
            double val);
         //
         // Writes val to outFile.
         // If val is finite, fFormat is used as the format.
         // If val is infinite, an appropriate string is printed using
         // sFormat as the format.

      //-----------------------------------------------------------------------
      // Data access functions.
      //-----------------------------------------------------------------------

      accessFunc (const WitBoundPair &, bounds)
      accessFunc (int,                  index)

      //-----------------------------------------------------------------------
      // Other public member functions.
      //-----------------------------------------------------------------------

      inline WitBoundPair & bounds ()
            //
            // Non-const access.
         {
         return bounds_;
         }

      void allocMyCoeffs (int nCoeffsForOptVC);
         //
         // Allocates myCoeffVec_ to nCoeffsForOptVC.

      void clearMyCoeffs ();
         //
         // Clears myCoeffVec_.

      void store (WitCoeff * theCoeff);
         //
         // Stores theCoeff in myCoeffVec_.

      void compBounds ();
         //
         // Computes the bounds for this OptVC.

      void printID ();
         //
         // Print info identifying this OptVC to the optProbFile.

      void printBounds (const WitBoundPair & preScreenBounds);
         //
         // Prints bounds and preScreenBounds for this OptVC to the optProbFile.

      void attachCoeffItr (WitCoeffItr & theItr);
         //
         // Attaches theItr to myCoeffVec_.

   protected:

      //-----------------------------------------------------------------------
      // Protected pure virtual member functions.
      //-----------------------------------------------------------------------

      virtual double upperBoundVal () = 0;
      virtual double lowerBoundVal () = 0;
         //
         // Computes and returns the upper and lower bounds for this OptVC.

      virtual const char * kindText () = 0;
         //
         // Returns a string identifying the high level derived class (e.g.,
         // OptVar), of which this OptVC is an instance.

      virtual const char * classText () = 0;
         //
         // Returns a string identifying the low level derived class (e.g.,
         // StockVar), of which this OptVC is an instance.
         // No more than 12 characters

      virtual void printSource () = 0;
         //
         // Prints to myOptProblem ()->optProbFile () text identifying the data
         // from which this OptVC was generated.

      //-----------------------------------------------------------------------
      // Other protected member functions.
      //-----------------------------------------------------------------------

      WitProblem * myProblem ();

   private:

      //-----------------------------------------------------------------------
      // Private member functions.
      //-----------------------------------------------------------------------

      noCopyCtorAssign (WitOptVC);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitPtrVec <WitCoeff> myCoeffVec_;
         //
         // The vector of Coeffs that belong to this OptVC_.

      int nCoeffs_;
         //
         // The # of Coeffs that belong to this OptVC_.

      WitBoundPair bounds_;
         //
         // Lower and upper bounds.

      const int index_;
         //
         // Index for arrays of OptVars or OptCons.
         // OptVars and OptCons are indexed separately; i.e., the first
         // OptVar has index 0 and the first OptCon has index 0.
   };

#endif
