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

#ifndef OSRealArgH
#define OSRealArgH

//------------------------------------------------------------------------------
// Header file: "OSRealArg.h"
//
// Contains the declaration of class OSRealArg.
//------------------------------------------------------------------------------

#include <RealArg.h>

//------------------------------------------------------------------------------
// Class OSRealArg
//
// "Output Scalar Real Argument"
//
// Provides a uniform interface to a certain kind of argument to an API
// function. The underlying argument is a pointer to a scalar real-valued
// variable belonging to the application program. Specifically, the underlying
// argument is of one of the following types:
//
//    float  *
//    double *
//
// Thus:
//    Some of the inner API functions (in WitRun.h) will have arguments of type
//       const OSRealArg &.
//    The corresponding outer API functions (in wit.h) will have arguments of
//       type float * and double *. These are the "underlying arguments".
//    The corresponding variables in the application program are of type float
//       and double. These are called the "application variables".
//
//
// Class Hierarchy:
//
// RealArg
//    OSRealArg
//
// Implemented in RealArg.C.
//------------------------------------------------------------------------------

class WitOSRealArg: public WitRealArg
   {
   public:

      //------------------------------------------------------------------------
      // Public member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitOSRealArg (float *);
         //
         // Conversion from a float * underlying argument.

      WitOSRealArg (double *);
         //
         // Conversion from a double * underlying argument.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitOSRealArg ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void operator = (double theValue) const;
         //
         // Sets the value of the application variable to theValue, converting
         // the type of theValue, if necessary.

      operator void * () const;
         //
         // Returns the underlying argument, converted to a void *.

      noCopyCtor (WitOSRealArg);
         //
         // Public, but not implemented.
         // Making the copy ctor public avoids a syntax error from the
         // MS Visual C++ compiler with the -Za flag.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      double appVarVal () const;
         //
         // Returns the current value of the application variable, converted
         // (if necessary) to a double.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      inline float  & myFlt () const;
      inline double & myDbl () const;
         //
         // These functions return the application variable.
         // Implemented in RealArg.C.

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      float * const myFltPtr_;
         //
         // The underlying argument, if it is a float *.
         // Otherwise NULL.

      double * const myDblPtr_;
         //
         // The underlying argument, if it is a double *.
         // Otherwise NULL.
   };

#endif
