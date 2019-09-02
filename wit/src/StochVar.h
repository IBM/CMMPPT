//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef StochVarH
#define StochVarH

//------------------------------------------------------------------------------
// Header file: "StochVar.h"
//
// Contains the declaration of class StochVar.
//------------------------------------------------------------------------------

#include <OptVar.h>
#include <StochLoc.h>
#include <ScenGroup.h>

//------------------------------------------------------------------------------
// class StochVar
//
// An OptVar in a stochastic implosion optimization problem.
//
// Class hierarchy:
//
//    OptVC
//       OptVar
//          StochVar
//             StScrapVar
//             StNonSubVar
//             StSubVar
//             StShipVar
//             StBddVar
//                StExecVar
//                StStockVar
//                StCumShipVar
//             StSlbvVar
//
// Implemented in StochOpt.C.
//------------------------------------------------------------------------------

class WitStochVar: public WitOptVar
   {
   public:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitStochVar (WitStochLoc *);

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      virtual ~WitStochVar ();

      //-----------------------------------------------------------------------
      // Overriding public virtual member functions.
      //-----------------------------------------------------------------------

      virtual WitOptProblem * myOptProblem ();
      virtual WitStochVar *   mandStochVar ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      virtual void genStSlbvVarIfNeeded ();
         //
         // Generates an StSlbvVar for this StochVar, if it needs one.

      virtual void genStSlbConIfNeeded ();
         //
         // Generates an StSlbCon  for this StochVar, if it needs one.

      virtual double stBoundsCoeffVal ();
         //
         // Computes and returns the value of the Coeff of this StochVar in the
         // stochastic bounds objective.

      virtual void uploadSolnTo (WitScenario * theScenario) = 0;
         //
         // Uploads the primal solution value of this StochVar to theScenario.

      virtual bool isStSlbvVar ();
         //
         // Returns true, iff this StochVar is an StSlbvVar.

      //-----------------------------------------------------------------------
      // Data access functions.
      //-----------------------------------------------------------------------

      accessFunc    (WitStochLoc *,   myStochLoc)
      accessWrapper (WitScenGroup *,  myScenGroup,  myStochLoc_)
      accessWrapper (double,          totalProb,    myScenGroup ())

   protected:

      //-----------------------------------------------------------------------
      // Protected member functions.
      //-----------------------------------------------------------------------

      accessWrapper (WitPeriod,       myPer,         myStochLoc_)
      accessWrapper (WitStochImpOP *, myStochImpOP,  myStochLoc_)
      accessWrapper (WitScenAttMgr *, myScenAttMgr,  myStochLoc_)
      accessWrapper (const WitList <WitScenario> &, 
                                      myScenarios,   myScenGroup ())
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
         // from which this StochVar was generated.

      //-----------------------------------------------------------------------
      // Other private member functions.
      //-----------------------------------------------------------------------

      noCopyCtorAssign (WitStochVar);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitStochLoc * const myStochLoc_;
         //
         // The StochLoc for this StochVar.
   };

#endif
