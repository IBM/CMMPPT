//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef ISRealArgH
#define ISRealArgH

//------------------------------------------------------------------------------
// Header file: "ISRealArg.h"
//
// Contains the declaration of class ISRealArg.
//------------------------------------------------------------------------------

#include <RealArg.h>

//------------------------------------------------------------------------------
// Class ISRealArg
//
// "Input Scalar Real Argument"
//
// Provides a uniform interface to a certain kind of argument to an API
// function. The underlying argument is a scalar real-valued variable passed in
// by value from the application program. Specifically, the underlying argument
// is of one of the following types:
//
//    const float
//    const double
//
// Thus:
//    Some of the inner API functions (in WitRun.h) will have arguments of type
//       const ISRealArg &.
//    The corresponding outer API functions (in wit.h) will have arguments of
//       type const float and const double. These are the "underlying 
//       arguments".
//
//
// Class Hierarchy:
//
// RealArg
//    ISRealArg
//
// Implemented in RealArg.C.
//------------------------------------------------------------------------------

class WitISRealArg: public WitRealArg
   {
   public:

      //------------------------------------------------------------------------
      // Public member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitISRealArg (float);
         //
         // Conversion from a float underlying argument.

      WitISRealArg (double);
         //
         // Conversion from a double underlying argument.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitISRealArg ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      inline double asaDbl () const
         {
         return
            floatCase ()?
               static_cast <double> (myFlt_):
                                     myDbl_;
         }

      inline operator double () const
         {
         return asaDbl ();
         }

      noCopyCtorAssign (WitISRealArg);
         //
         // Public, but not implemented.
         // Making the copy ctor public avoids a syntax error from the
         // MS Visual C++ compiler with the -Za flag.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      inline operator float () const;
         //
         // Not implemented.
         // Prevents unintended conversion to float.

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      const float myFlt_;
         //
         // The underlying argument, if it is a float.
         // Otherwise 0.

      const double myDbl_;
         //
         // The underlying argument, if it is a double.
         // Otherwise 0.
   };

#endif
