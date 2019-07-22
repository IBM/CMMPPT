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

#ifndef CpxParSpecH
#define CpxParSpecH

//------------------------------------------------------------------------------
// Header file: "CpxParSpec.h"
//
// Contains the declaration of class CpxParSpec.
//------------------------------------------------------------------------------

#include <Assoc.h>
#include <Str.h>

//------------------------------------------------------------------------------
// Class CpxParSpec
//
// "CPLEX Parameter Specification"
//
// A CpxParSpec represents an instruction to set a specified CPLEX parameter to
// a specified value.
//
// Each CpxParSpec has:
//   * A specified name, which, when prefixed with "CPX_PARAM_", is to match the
//     name of the specified CPLEX parameter.
//   * A specified value type (either int or double) which is to be the value
//     type of the specified CPLEX parameter.
//   * A specified value, matching the specified value type, which is the value
//     to which the specified CPLEX parameter is to be set.
//
// A CpxParSpec is either valid or invalid.
// A CpxParSpec is valid, iff both of the following hold:
//   * Its name matches the name of an actual CPLEX parameter.
//   * Its value type matches the type of the CPLEX parameter.
//
// During optimizing implosion and stochastic implosion with CPLEX selected,
// just before a CPLEX solve routine is invoked, the following actions will be
// performed for each CpxParSpec in the Problem:
//   * If the CpxParSpec is invalid, a severe error will be issued.
//   * If the CpxParSpec is   valid, the value of the specified CPLEX
//     parameter will be set to the specified value.
//------------------------------------------------------------------------------

class WitCpxParSpec: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitCpxParSpec (WitProblem *, const char *, int);
      WitCpxParSpec (WitProblem *, const char *, double);

      WitCpxParSpec (WitProblem *, WitCpxParSpec *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitCpxParSpec ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (const WitString &, myName)
      accessFunc (bool,              valTypeIsInt)

      int    myIntVal () const;
      double myDblVal () const;

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void display ();
         //
         // Displays the input data for this CpxParSpec.

      void writeData ();
         //
         // Writes this CpxParSpec to an input data file.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitCpxParSpec);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      const WitString myName_;
         //
         // The name of this CpxParSpec.

      const bool valTypeIsInt_;
         //
         // If true,  the specified value type is int.
         // If false, the specified value type is double.

      const int    myIntVal_;
      const double myDblVal_;
         //
         // If valTypeIsInt_ is true,
         //    myIntVal_ is the specified value.
         //    myDblVal_ is 0.0.
         //
         // If valTypeIsInt_ is false,
         //    myIntVal_ is 0.
         //    myDblVal_ is the specified value.
   };

#endif
