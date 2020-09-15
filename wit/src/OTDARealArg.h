//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef OTDARealArgH
#define OTDARealArgH

//------------------------------------------------------------------------------
// Header file: "OTDARealArg.h"
//
// Contains the declaration of class OTDARealArg.
//------------------------------------------------------------------------------

#include <RealArg.h>

//------------------------------------------------------------------------------
// Class OTDARealArg
//
// "Output Two-Dimensional Array Real Argument"
//
// Provides a uniform interface to a certain kind of argument to an API
// function. The underlying argument is the address of a pointer belonging to
// the application program and it is WIT's responsibility to store at this
// address the pointer to a newly created real-valued C-style 2-dimensional
// array. Specifically, the underlying argument is of one of the following
// types:
//
//    float  * * *
//    double * * *
//
// Thus:
//    Some of the inner API functions (in WitRun.h) will have arguments of type
//       const OTDARealArg &.
//    The corresponding outer API functions (in wit.h) will have arguments of
//       type float * * * and double * * *. These are the "underlying
//       arguments".
//    The newly created 2-D arrays are of type float * * and double * *. These
//    are called the "underlying arrays".
//
//
// Class Hierarchy:
//
// RealArg
//    OTDARealArg
//
// Implemented in RealArg.C.
//------------------------------------------------------------------------------

class WitOTDARealArg: public WitRealArg
   {
   public:

      //------------------------------------------------------------------------
      // Public member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitOTDARealArg (float * * *);
         //
         // Conversion from a float * * * underlying argument.

      WitOTDARealArg (double * * *);
         //
         // Conversion from a double * * * underlying argument.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitOTDARealArg ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void apiAlloc (int nRows, int nCols) const;
         //
         // Allocates the underlying array to nRows by nCols.
         // Uses malloc, so the array can be passed back to the application
         // program.

      void copyIntoRow (
            int                   theIdx,
            const WitDblFlexVec & theDblFlexVec)
            const;
         //
         // Copies the elements of theDblFlexVec into the corresponding elements
         // of the row of the underlying array indexed by theIdx.

      operator void * () const;
         //
         // Returns the underlying argument, converted to a void *.

      noCopyCtorAssign (WitOTDARealArg);
         //
         // Public, but not implemented.
         // Making the copy ctor public avoids a syntax error from the
         // MS Visual C++ compiler with the -Za flag.

   private:

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      float * * * const myFltArPtr_;
         //
         // The underlying argument, if it is a float * * *.
         // Otherwise NULL.

      double * * * const myDblArPtr_;
         //
         // The underlying argument, if it is a double * * *.
         // Otherwise NULL.
   };

#endif
