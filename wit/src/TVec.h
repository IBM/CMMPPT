//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef TVecH
#define TVecH

//------------------------------------------------------------------------------
// Header file: "TVec.h"
//
// Contains the declaration and inline implementation of class template TVec.
//------------------------------------------------------------------------------

#include <Vector.h>

//------------------------------------------------------------------------------
// Class template TVec <Elem>
//
// A WitVector <Elem> of length Problem::nPeriods, for some Problem.
// Implemented by inheriting from WitVector <Elem>.
// Implemented in Vecs.C.
//------------------------------------------------------------------------------

template <typename Elem> 
      class WitTVec: public WitVector <Elem>
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitTVec ();

      WitTVec (WitProblem *, Elem initValue);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitTVec ();

      //------------------------------------------------------------------------
      // Public member functions.
      //------------------------------------------------------------------------

      inline void operator = (const WitTVec & theTVec)
         {
         thisVector () = theTVec;
         }

      inline void operator = (Elem theScalar)
         {
         thisVector () = theScalar;
         }

      inline void operator = (const Elem * theCVec)
         {
         thisVector () = theCVec;
         }

      void operator = (const WitFlexVec <Elem> &);

      void allocate (WitProblem *, Elem theScalar);

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      inline WitVector <Elem> & thisVector ()
         {
         return (* this);
         }

      noCopyCtor (WitTVec);
   };

#endif
