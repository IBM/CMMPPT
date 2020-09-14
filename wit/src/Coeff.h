//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef CoeffH
#define CoeffH

//------------------------------------------------------------------------------
// Header file: "Coeff.h"
//
// Contains the declaration of class Coeff.
//------------------------------------------------------------------------------

#include <Util.h>

//------------------------------------------------------------------------------
// class Coeff
//
// The coefficient of a variable in a constraint or objective in the
// optimization problem, i.e., a matrix element.
// Thus if theCoeff is an instance of this class, it follows that the
// coefficient of theCoeff->myVar () in theCoeff->myCon () is
// theCoeff->myValue ().
//
// Implemented in OptMisc.C.
//------------------------------------------------------------------------------

class WitCoeff
   {
   public:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitCoeff (WitOptCon * theCon, WitOptVar * theVar, double theValue);
         //
         // myCon_   <-- theCon.
         // myVar_   <-- theVar.
         // myValue_ <-- theValue.

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      ~WitCoeff ();

      //-----------------------------------------------------------------------
      // Data Access functions.
      //-----------------------------------------------------------------------

      accessFunc (WitOptCon *, myCon)
      accessFunc (WitOptVar *, myVar)
      accessFunc (double,      myValue)

      //-----------------------------------------------------------------------
      // Other public member functions.
      //-----------------------------------------------------------------------

      inline void increment (const double & delta)
         {
         myValue_ += delta;
         }

   private:

      //-----------------------------------------------------------------------
      // Private member functions.
      //-----------------------------------------------------------------------

      noCopyCtorAssign (WitCoeff);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitOptCon * const myCon_;
         //
         // The OptCon in which this Coeff appears.

      WitOptVar * const myVar_;
         //
         // The OptVar of which this Coeff is a coefficient.

      double myValue_;
         //
         // The coefficient value.
   };

#endif
