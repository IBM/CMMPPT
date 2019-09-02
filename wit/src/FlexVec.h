//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef FlexVecH
#define FlexVecH

//------------------------------------------------------------------------------
// Header file: "FlexVec.h"
//
// Contains the declaration of class template FlexVec <Elem>.
// Contains the declaration of class          DblFlexVec.
//------------------------------------------------------------------------------

#include <FlexVAsst.h>

//------------------------------------------------------------------------------
// Class template FlexVec <Elem>
//
// "Flexible Vector"
// A vector whose representation changes dynamically between a scalar
// representation and a vector representation.
// The length of the represented vector is nPeriods for the corresponding
// Problem.
//------------------------------------------------------------------------------

template <typename Elem> 
      class WitFlexVec
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions
      //------------------------------------------------------------------------

      WitFlexVec (WitProblem *, Elem initValue);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitFlexVec ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void operator = (const WitFlexVec &);
      void operator = (Elem);
      void operator = (const WitTVec <Elem> &);
      void operator = (const Elem *);
         //
         // Assignment operators.

      void copyInto (WitTVec <Elem> &) const;
      void copyInto (Elem *)           const;
         //
         // Each of these functions copies the elements of this FlexVec
         // into the corresponding elements of the argument vector.

      inline Elem operator [] (WitPeriod thePer) const;
         //
         // Returns the value of the element of the represented vector indexed
         // by thePer.

      inline Elem & elemRef (int theIndex);
         //
         // Same as [], but returns a reference.

      bool operator == (Elem) const;
      bool operator != (Elem) const;
      bool operator >= (Elem) const;

      accessWrapper (int,          nPeriods,  curAsst_)
      accessWrapper (WitProblem *, myProblem, curAsst_)

   protected:

      //------------------------------------------------------------------------
      // Protected member functions.
      //------------------------------------------------------------------------

      accessWrapper (bool, vectorMode, curAsst_)
      accessWrapper (bool, scalarMode, curAsst_)

      inline void beInVectorMode ()
         {
         if (scalarMode ())
            switchToVectorMode ();
         }

      inline void beInScalarMode ()
         {
         if (vectorMode ())
            switchToScalarMode ();
         }

      inline Elem * myCVec () const
         {
         witAssert (vectorMode ());

         return myCVecFromUnion_;
         }

      inline Elem & myScalar ()
         {
         witAssert (scalarMode ());

         return myScalarFromUnion_;
         }

      inline Elem myScalar () const
         {
         witAssert (scalarMode ());

         return myScalarFromUnion_;
         }

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtor (WitFlexVec);

      void checkBounds (WitPeriod thePer) const;
         //
         // Verifies that thePer is in [0, nPeriods ()).

      void switchToVectorMode ();
         //
         // Valid only in scalar mode.
         // Switches to   vector mode.

      void switchToScalarMode ();
         //
         // Valid only in vector mode.
         // Switches to   scalar mode.

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitFlexVAsst * curAsst_;
         //
         // The current FlexVAsst for this FlexVec.

      union
         {
         Elem * myCVecFromUnion_;
            //
            // The c-vector representation of the vector.
            // Valid only in vector mode.

         Elem myScalarFromUnion_;
            //
            // The scalar representation of the vector.
            // Valid only in scalar mode.
         };
   };

//------------------------------------------------------------------------------
// Implementation of inlines.
//------------------------------------------------------------------------------

template <typename Elem> 
      inline Elem WitFlexVec <Elem>::operator [] (WitPeriod thePer) const
   {
   ifDebug (checkBounds (thePer));

   return
      curAsst_->vectorMode ()?
         myCVec   ()[thePer]:
         myScalar ();
   }

//------------------------------------------------------------------------------

template <typename Elem>
      inline Elem & WitFlexVec <Elem>::elemRef (WitPeriod thePer)
   {
   ifDebug (checkBounds (thePer));

   beInVectorMode ();

   return myCVec ()[thePer];
   }

//------------------------------------------------------------------------------
// Class DblFlexVec
//
// "Double Flexible Vector"
//------------------------------------------------------------------------------

class WitDblFlexVec: public WitFlexVec <double>
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions
      //------------------------------------------------------------------------

      WitDblFlexVec (WitProblem *, double initValue);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitDblFlexVec ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void operator = (const WitDblFlexVec &);
      void operator = (double);
      void operator = (const WitTVec <double> &);
      void operator = (const double *);
      void operator = (const float *);
      void operator = (const WitIVRealArg &);
         //
         // Assignment operators.

      void copyInto (double *) const;
      void copyInto (float *)  const;
         //
         // Each of these functions copies the elements of this DblFlexVec
         // into the corresponding elements of the argument vector.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      inline WitFlexVec <double> & thisFlexVec ()
         {
         return (* this);
         }

      inline const WitFlexVec <double> & thisFlexVec () const
         {
         return (* this);
         }

      noCopyCtor (WitDblFlexVec);
   };

//------------------------------------------------------------------------------
// NonClass functions with arguments belonging to specific FlexVec classes.
//------------------------------------------------------------------------------

namespace WitNonClass
   {
   void copy (WitFlexVec <bool> & theFlexVec, const WitBoolean * theCVec);

   void copy (WitBoolean * theCVec, const WitFlexVec <bool> & theFlexVec);
   };

#endif
