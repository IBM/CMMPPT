//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef IVRealArgH
#define IVRealArgH

//------------------------------------------------------------------------------
// Header file: "IVRealArg.h"
//
// Contains the declaration of class IVRealArg.
//------------------------------------------------------------------------------

#include <RealArg.h>

//------------------------------------------------------------------------------
// Class IVRealArg
//
// "Input Vector Real Argument"
//
// Provides a uniform interface to a certain kind of argument to an API
// function. The underlying argument is a vector real-valued variable passed in
// from the application program. Specifically, the underlying argument is of one
// of the following types:
//
//    const float  * const
//    const double * const
//
// Thus:
//    Some of the inner API functions (in WitRun.h) will have arguments of type
//       const IVRealArg &.
//    The corresponding outer API functions (in wit.h) will have arguments of
//       type "const float * const" and "const double * const". These are the
//       "underlying arguments".
//
//
// Class Hierarchy:
//
// RealArg
//    IVRealArg
//
// Implemented in RealArg.C.
//------------------------------------------------------------------------------

class WitIVRealArg: public WitRealArg
   {
   public:

      //------------------------------------------------------------------------
      // Public member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitIVRealArg (const float *);
         //
         // Conversion from a const float * underlying argument.

      WitIVRealArg (const double *);
         //
         // Conversion from a const double * underlying argument.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitIVRealArg ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void copyInto (WitDblFlexVec & theDblFlexVec) const;
         //
         // Copies the elements of this IVRealArg into the corresponding
         // elements of theDblFlexVec.

      inline double operator [] (int theIdx) const;
         //
         // Returns the element of this IVRealArg indexed by theIdx.

      operator const void * () const;
         //
         // Returns the underlying argument, converted to a const void *.

      bool isNonNeg (int length) const;
         //
         // Returns true, iff underlying argument is non-negative for length
         // elements.

      void issueSetDblVecMsg (
            const char * theAttName,
            WitApiCall & theApiCall)
            const;
         //
         // Issues the message for setting a double vector attribute to the
         // underlying argument.
         // theAttName is the name of the attribute.
         // theApiCall issues the message.

      void issueBoundVecMsg (
            WitBoundSet * theBoundSet,
            WitMsgFragID  theFragID,
            WitApiCall &  theApiCall)
            const;
         //
         // Issues the message for setting one of the vectors of theBoundSet to
         // theFragID, where theFragID indicates a message fragment indicating
         // the vector being set.
         // theApiCall issues the message.

      noCopyCtorAssign (WitIVRealArg);
         //
         // Public, but not implemented.
         // Making the copy ctor public avoids a syntax error from the
         // MS Visual C++ compiler with the -Za flag.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      inline const float  * myFltCVec () const;
      inline const double * myDblCVec () const;
         //
         // These functions return the underlying argument.

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      const float * const myFltCVec_;
         //
         // The underlying argument, if it is a const float *.
         // Otherwise NULL.

      const double * const myDblCVec_;
         //
         // The underlying argument, if it is a const double *.
         // Otherwise NULL.
   };

//------------------------------------------------------------------------------
// Implementation of inlines.
//------------------------------------------------------------------------------

inline double WitIVRealArg::operator [] (int theIdx) const
   {
   return
      floatCase ()?
         static_cast <double> (myFltCVec ()[theIdx]):
                               myDblCVec ()[theIdx];
   }

//------------------------------------------------------------------------------

inline const float * WitIVRealArg::myFltCVec () const
   {
   witAssert (floatCase ());

   return (myFltCVec_);
   }

//------------------------------------------------------------------------------

inline const double * WitIVRealArg::myDblCVec () const
   {
   witAssert (doubleCase ());

   return (myDblCVec_);
   }

//------------------------------------------------------------------------------

#endif
