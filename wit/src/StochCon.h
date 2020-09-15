//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef StochConH
#define StochConH

//------------------------------------------------------------------------------
// Header file: "StochCon.h"
//
// Contains the declaration of class StochCon.
//------------------------------------------------------------------------------

#include <OptCon.h>
#include <StochLoc.h>
#include <ScenGroup.h>

//------------------------------------------------------------------------------
// class StochCon
//
// "Stochastic Constraint"
// An OptCon in a stochastic implosion optimization problem.
//
// Class hierarchy:
//
//    OptVC
//       OptCon
//          StochCon
//             StResCon
//             StSubCon
//             StShipCon
//             StSlbCon
//
// Implemented in StochOpt.C.
//------------------------------------------------------------------------------

class WitStochCon: public WitOptCon
   {
   public:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitStochCon (WitStochLoc *);

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      virtual ~WitStochCon ();

      //-----------------------------------------------------------------------
      // Overriding public virtual member functions.
      //-----------------------------------------------------------------------

      virtual WitOptProblem * myOptProblem ();

   protected:

      //-----------------------------------------------------------------------
      // Protected member functions.
      //-----------------------------------------------------------------------

      WitStochLoc * findVarStochLocFor (WitNode * theNode, WitPeriod thePer);
         //
         // Returns the StochLoc for any StochVar for theNode and thePer that
         // is to have Coeffs in this StochCon.

      accessFunc    (WitStochLoc *,   myStochLoc)
      accessWrapper (WitPeriod,       myPer,         myStochLoc_)
      accessWrapper (WitScenGroup *,  myScenGroup,   myStochLoc_)
      accessWrapper (WitStochImpOP *, myStochImpOP,  myStochLoc_)
      accessWrapper (WitScenAttMgr *, myScenAttMgr,  myStochLoc_)
      accessWrapper (WitScenario *,   firstScenario, myScenGroup ())

   private:

      //-----------------------------------------------------------------------
      // Private member functions.
      //-----------------------------------------------------------------------

      //-----------------------------------------------------------------------
      // Overriding private virtual member functions.
      //-----------------------------------------------------------------------

      virtual void printSource ();

      //-----------------------------------------------------------------------
      // Private pure virtual member functions.
      //-----------------------------------------------------------------------

      virtual void printItem () = 0;
         //
         // Prints to the optProbFile text identifying the non-StochLoc data
         // from which this StochCon was generated.

      //-----------------------------------------------------------------------
      // Other private member functions.
      //-----------------------------------------------------------------------

      noCopyCtorAssign (WitStochCon);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitStochLoc * const myStochLoc_;
         //
         // The StochLoc for this StochCon.
   };

#endif
