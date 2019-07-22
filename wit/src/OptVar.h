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

#ifndef OptVarH
#define OptVarH

//------------------------------------------------------------------------------
// Header file: "OptVar.h"
//
// Contains the declaration of class OptVar.
//------------------------------------------------------------------------------

#include <OptVC.h>

//------------------------------------------------------------------------------
// class OptVar
//
// A variable in an optimization problem.
//
// Class hierarchy:
//
//    OptVC
//       OptVar
//          DetVar
//             ScrapVar
//             NonSubVar
//             SubVar
//             ShipVar
//             BoundedVar
//                ExecVar
//                StockVar
//                CumShipVar
//             SlbvVar
//             MultiObjVar
//             NtbvVar
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
// Implemented in OptMisc.C.
//------------------------------------------------------------------------------

class WitOptVar: public WitOptVC
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitOptVar (WitOptProblem * theOptProblem);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitOptVar ();

      //------------------------------------------------------------------------
      // Overriding public virtual member functions.
      //------------------------------------------------------------------------

      virtual double solnValue ();
         //
         // Returns primalValue ().

      //------------------------------------------------------------------------
      // Other public virtual member functions.
      //------------------------------------------------------------------------

      virtual double compObjCoeff () = 0;
         //
         // Computes and returns the coefficient of this OptVar in the
         // objective function.

      virtual double indepInitValue ();
         //
         // Returns an initial primal value for this OptVar, determined
         // independently of other OptVars.
         // The base class implementation returns 0.0, which should be used for
         // OptVars whose initial value is dependent on other OptVars and
         // will be computed in OptProblem::initDepVars.

      virtual bool isAnIntVar ();
         //
         // Returns true, iff this OptVar is an integer variable.
         // The base class implementation returns false.

      virtual const char * lexObjElemName ();
         //
         // If this OptVar represents an element of a lexicographic objective
         // function, its name is returned; otherwise a fatal error is issued.

      virtual void reportLexUnbounded ();
         //
         // If this OptVar represents an element of a lexicographic objective
         // function, this function does any special error reporting for an
         // unbounded solution for the element, prior to the reporting that will
         // be done by the CplexMgr;
         // otherwise issues a fatal error.

      //------------------------------------------------------------------------
      // mand<derived OptVar> functions.
      //
      // In each case, if this OptVar is actually an instance of the
      // indicated derived class, the function returns that instance;
      // otherwise, a fatal error is issued.
      //------------------------------------------------------------------------

      virtual WitDetVar *   mandDetVar   ();
      virtual WitStochVar * mandStochVar ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (double, primalValue)
      accessFunc (double, objCoeff)

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void incCoeff (double delta);
         //
         // Calls myOptProblem ()->incCoeff (...).

      void generateObjCoeff ();
         //
         // Computes and stores the value of objCoeff ().

      void print ();
         //
         // Prints this OptVar to the optProbFile.

      double reducedReward ();
         //
         // Computes and returns the reduced reward value for this OptVar for
         // the current dual solution.

      void setPrimalValue (double);

   protected:

      //------------------------------------------------------------------------
      // Overriding protected virtual member functions.
      //------------------------------------------------------------------------

      virtual const char * kindText ();

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitOptVar);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      double primalValue_;
         //
         // Primal solution value.

      double objCoeff_;
         //
         // The coefficient value of this OptVar in the main objective.
   };

#endif
