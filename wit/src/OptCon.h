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

#ifndef OptConH
#define OptConH

//------------------------------------------------------------------------------
// Header file: "OptCon.h"
//
// Contains the declaration of class OptCon.
//------------------------------------------------------------------------------

#include <OptVC.h>

//------------------------------------------------------------------------------
// class OptCon
//
// A constraint in an optimization problem.
//
// Class hierarchy:
//
//    OptVC
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

class WitOptCon: public WitOptVC
   {
   public:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitOptCon (WitOptProblem * theOptProblem);

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      virtual ~WitOptCon ();

      //-----------------------------------------------------------------------
      // Public pure virtual member functions.
      //-----------------------------------------------------------------------

      virtual void generateCoeffs () = 0;
         //
         // Generates the Coeffs of this OptCon.

      //-----------------------------------------------------------------------
      // Overriding public virtual member functions.
      //-----------------------------------------------------------------------

      virtual double solnValue ();
         //
         // Returns dualValue ().

      //-----------------------------------------------------------------------
      // Data access functions.
      //-----------------------------------------------------------------------

      accessFunc (double, dualValue)

      //-----------------------------------------------------------------------
      // Other public member functions.
      //-----------------------------------------------------------------------

      bool screenable ();
         //
         // Returns true, iff this OptCon can be screened.

      void print ();
         //
         // Prints this OptCon to the optProbFile.

      void setDualValue (double);

   protected:

      //-----------------------------------------------------------------------
      // Overriding protected virtual member functions.
      //-----------------------------------------------------------------------

      virtual const char * kindText ();

   private:

      //-----------------------------------------------------------------------
      // Private member functions.
      //-----------------------------------------------------------------------

      noCopyCtorAssign (WitOptCon);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      double dualValue_;
         //
         // Dual solution value.
         // Valid only if needDual is true.
   };

#endif
