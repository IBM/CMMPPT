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

#ifndef FlexVAsstH
#define FlexVAsstH

//------------------------------------------------------------------------------
// Header file: "FlexVAsst.h"
//
// Contains the declaration of class FlexVAsst.
//------------------------------------------------------------------------------

#include <Assoc.h>

//------------------------------------------------------------------------------
// Class FlexVAsst
//
// "Flexible Vector Assistant"
// A FlexVAsst fulfills the following reponsibilities for any FlexVec that is
// associated with it:
// * Indicates whether the FlexVec is in scalar mode or vector mode.
// * Stores the relevent value of nPeriods.
//
// Class Hierarchy:
//
// ProbAssoc
//    FlexVAsst
//------------------------------------------------------------------------------

class WitFlexVAsst: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Public member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitFlexVAsst (WitProblem *, bool vectorModeVal);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitFlexVAsst ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void updateNPeriods ();
         //
         // Sets nPeriods_ to the current value in myGlobalComp ().

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (bool, vectorMode)
      accessFunc (int,  nPeriods)

      inline bool scalarMode ()
         {
         return (not vectorMode_);
         }

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitFlexVAsst);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      const bool vectorMode_;
         //
         // If true,  any FlexVec associated with this FlexVAsst is considered
         // to be in vector mode.
         // If false, any FlexVec associated with this FlexVAsst is considered
         // to be in scalar mode.

      int nPeriods_;
         //
         // nPeriods for the Problem associated with this FlexVAsst.
         // Any FlexVec associated with this FlexVAsst will be have nPeriods_
         // elements.
   };

#endif
