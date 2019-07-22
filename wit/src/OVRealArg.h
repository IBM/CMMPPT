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

#ifndef OVRealArgH
#define OVRealArgH

//------------------------------------------------------------------------------
// Header file: "OVRealArg.h"
//
// Contains:
//    The declaration           of class OVRealArg.
//    The declaration           of class OVRAElem.
//    The inline implementation of class OVRealArg.
//    The inline implementation of class OVRAElem.
//------------------------------------------------------------------------------

#include <RealArg.h>

//------------------------------------------------------------------------------
// Class OVRealArg
//
// "Output Vector Real Argument"
//
// Provides a uniform interface to a certain kind of argument to an API
// function. The underlying argument is the address of a pointer belonging to
// the application program and it is WIT's responsibility to store at this
// address the pointer to a newly created real-valued c-vector. Specifically,
// the underlying argument is of one of the following types:
//
//    float  * *
//    double * *
//
// Thus:
//    Some of the inner API functions (in WitRun.h) will have arguments of type
//       const OVRealArg &.
//    The corresponding outer API functions (in wit.h) will have arguments of
//       type float * * and double * *. These are the "underlying arguments".
//    The newly created c-vectors are of type float * and double *. These are
//    called the "underlying c-vectors".
//
//
// Class Hierarchy:
//
// RealArg
//    OVRealArg
//
// Implemented in RealArg.C.
//------------------------------------------------------------------------------

class WitOVRealArg: public WitRealArg
   {
   friend class WitOVRAElem;

   public:

      //------------------------------------------------------------------------
      // Public member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitOVRealArg (float * *);
         //
         // Conversion from a float * * underlying argument.

      WitOVRealArg (double * *);
         //
         // Conversion from a double * * underlying argument.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitOVRealArg ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void apiAlloc (int nElems) const;
         //
         // Allocates the underlying c-vector to nElems elements.
         // Uses malloc, so the c-vector can be passed back to the application
         // program.

      void operator = (const WitDblFlexVec & theDblFlexVec) const;
         //
         // Copies the elements of theDblFlexVec into the corresponding elements
         // of the underlying c-vector.

      void operator = (const WitVector <double> & theDblVec) const;
         //
         // Copies the elements of theDblVec into the corresponding elements
         // of the underlying c-vector.

      inline WitOVRAElem operator [] (int theIdx) const;
         //
         // Constructs and returns an OVRAElem that represents the element of
         // this OVRealArg indexed by theIdx.

      operator void * () const;
         //
         // Returns the underlying argument, converted to a void *.

      noCopyCtorAssign (WitOVRealArg);
         //
         // Public, but not implemented.
         // Making the copy ctor public avoids a syntax error from the
         // MS Visual C++ compiler with the -Za flag.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      inline float  * & myFltCVec () const;
      inline double * & myDblCVec () const;
         //
         // These functions return the underlying c-vector.

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      float * * const myFltCVecPtr_;
         //
         // The underlying argument, if it is a float * *.
         // Otherwise NULL.

      double * * const myDblCVecPtr_;
         //
         // The underlying argument, if it is a double * *.
         // Otherwise NULL.
   };

//------------------------------------------------------------------------------
// Class OVRAElem
//
// "Output Vector Real Argument Element"
// Represents an element of an OVRealArg.
// All implementation is inline.
//------------------------------------------------------------------------------

class WitOVRAElem
   {
   public:

      //------------------------------------------------------------------------
      // Public member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      inline WitOVRAElem (const WitOVRealArg &, int);

      inline WitOVRAElem (const WitOVRAElem &);
         //
         // Copy ctor.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      inline ~WitOVRAElem ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      inline void operator = (double theVal) const;
         //
         // Sets the element represented by this OVRAElem to theVal.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      WitOVRAElem & operator = (const WitOVRAElem &);
         //
         // Not implemented.

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      const WitOVRealArg & myOVRealArg_;
      const int            myIdx_;
         //
         // This OVRAElem represents the element of myOVRealArg_ indexed by
         // myIdx_.
   };

//------------------------------------------------------------------------------
// Inline implementation of class OVRealArg.
//------------------------------------------------------------------------------

inline WitOVRAElem WitOVRealArg::operator [] (int theIdx) const
   {
   return WitOVRAElem (* this, theIdx);
   }

//------------------------------------------------------------------------------

inline float * & WitOVRealArg::myFltCVec () const
   {
   witAssert (floatCase ());

   return (* myFltCVecPtr_);
   }

//------------------------------------------------------------------------------

inline double * & WitOVRealArg::myDblCVec () const
   {
   witAssert (doubleCase ());

   return (* myDblCVecPtr_);
   }

//------------------------------------------------------------------------------
// Inline implementation of class OVRAElem.
//------------------------------------------------------------------------------

inline WitOVRAElem::WitOVRAElem (const WitOVRealArg & theOVRealArg, int theIdx):

      myOVRealArg_ (theOVRealArg),
      myIdx_       (theIdx)
   {
   }

//------------------------------------------------------------------------------

inline WitOVRAElem::WitOVRAElem (const WitOVRAElem & theOVRAElem):

      myOVRealArg_ (theOVRAElem.myOVRealArg_),
      myIdx_       (theOVRAElem.myIdx_)
   {
   }

//------------------------------------------------------------------------------

inline WitOVRAElem::~WitOVRAElem ()
   {
   }

//------------------------------------------------------------------------------

inline void WitOVRAElem::operator = (double theVal) const
   {
   if (myOVRealArg_.floatCase ())
      myOVRealArg_.myFltCVec ()[myIdx_] = static_cast <float> (theVal);
   else
      myOVRealArg_.myDblCVec ()[myIdx_] =                      theVal;
   }

#endif
