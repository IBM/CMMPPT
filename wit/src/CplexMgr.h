//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef CplexMgrH
#define CplexMgrH

//------------------------------------------------------------------------------
// Header file: "CplexMgr.h"
//
// Contains the declaration of class CplexMgr.
//    The declaration compiled only if CPLEX_EMBEDDED is defined.
//
// NOTE:
//    This head file should be #included only by CplexMgr.C
//    This is because the Makefiles apply the proper compiler flags for the
//    CPLEX_EMBEDDED macro and the #include <cplex.h> statement only when
//    compiling CplexMgr.C
//------------------------------------------------------------------------------

#ifdef CPLEX_EMBEDDED

#include <Assoc.h>

#include <cplex.h>

//------------------------------------------------------------------------------
// class CplexMgr
//
// Responsible for all interactions with CPLEX.
//------------------------------------------------------------------------------

class WitCplexMgr: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitCplexMgr (WitOptProblem *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitCplexMgr ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      virtual void solveOptProb ();
         //
         // Solves the optimization problem by invoking CPLEX.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      void setUpCplex ();
         //
         // Sets up CPLEX.

      void setUpLogFile ();
         //
         // Sets up the CPLEX log file.

      void shutDownCplex ();
         //
         // Shuts down CPLEX.

      void shutDownLogFile ();
         //
         // Shuts down the CPLEX log file.

      void solveOptProbAsLp ();
         //
         // Loads, solves and retrieves the solution to the optimization problem
         // as an LP for a first solve.

      void reSolveOptProbAsLp ();
         //
         // Loads, solves and retrieves the solution to the optimization problem
         // as an LP for a re-solve.

      void solveOptProbAsMip ();
         //
         // Loads, solves and retrieves the solution to the optimization problem
         // as a MIP.

      void solveOptProbAsLexOpt ();
         //
         // Loads, solves and retrieves the solution to the optimization problem
         // as a lexicographic optimization.

      void issueSolveMsg ();
         //
         // Issues a msg for the solve.

      void loadLp ();
         //
         // Loads the optimization problem into CPLEX as an LP.

      void getRowData (
            WitVector <double> & rhs,
            WitVector <char> &   sense);
         //
         // Retrieves the row portion of the LP aspect of the problem in the
         // representation required for CPXcopylp.

      void getColumnData (
            WitVector <double> & objective,
            WitVector <int> &    matbeg,
            WitVector <int> &    matcnt,
            WitVector <int> &    matind,
            WitVector <double> & matval,
            WitVector <double> & lb,
            WitVector <double> & ub);
         //
         // Retrieves the column portion of the LP aspect of the problem in the
         // representation required for CPXcopylp.

      void reviseLp ();
         //
         // Revises the LP problem that was previously loaded into CPLEX.

      void reviseBounds ();
         //
         // Revises the upper and lower bounds on variables of the LP problem
         // that was previously loaded into CPLEX.

      void reviseRHS ();
         //
         // Revises the right-hand-side values of the LP problem that was
         // previously loaded into CPLEX.

      void getConData (double & rhs, char & sense, WitOptCon * theOptCon);
         //
         // Sets rhs and sense to the CPLEX RHS and constraint sense for
         // theOptCon.

      void reviseObjCoeffs ();
         //
         // Revises the objective function coefficients of the LP problem that
         // was previously loaded into CPLEX.

      void loadIntData ();
         //
         // Loads the integrality data of the optimization problem into CPLEX.

      int countIntVars ();
         //
         // Counts and returns the number of integer variables in the
         // optimization problem.

      void writeMps ();
         //
         // Writes an MPS file of the opt problem entered into CPLEX, if
         // appropriate.

      void solveLexOpt ();
         //
         // Makes appropriate calls to CPLEX to solve the optimization problem
         // as a lexicographic optimization.

      void setObjCoef (WitOptVar * theOptVar, double theVal);
         //
         // Sets the objective coefficient of theOptVar to theVal.

      void lockLexObjElemVal (WitOptVar * theOptVar);
         //
         // Assuming theOptVar represents a lexicographic objective element that
         // has just been maximized, this function locks the variable at its
         // maximum value minus a tolerance.

      void setLpMethodByOptStarter ();
         //
         // Sets the LP method to be used by CPXlpopt based on the OptStarter.

      void solveLp (bool optNeeded);
         //
         // Makes appropriate calls to CPLEX to solve the optimization problem
         // as an LP.
         // optNeeded is to be true, iff an optimal solution is required.

      void loadInitSoln ();
         //
         // Loads the initial primal solution from myOptProblem () into CPLEX,
         // as needed.

      void printLpSolveInfo ();
         //
         // Prints some information about the LP solve.

      void checkLpSolnStatus (bool optNeeded);
         //
         // Checks the status of the LP solution.
         // optNeeded is to be true, iff an optimal solution is required.

      void repEarlyTermLpSolnStatus (bool optNeeded);
         //
         // Reports the status of the LP solution, in cases where the solve
         // routine terminated early.
         // optNeeded is to be true, iff an optimal solution is required.

      void solveMip (bool optNeeded);
         //
         // Makes appropriate calls to CPLEX to solve the optimization problem
         // as a MIP.
         // optNeeded is to be true, iff an optimal solution is required.

      void printMipSolveInfo ();
         //
         // Prints some information about the MIP solve.

      void checkMipSolnStatus (bool optNeeded);
         //
         // Checks the status of the MIP solution.
         // optNeeded is to be true, iff an optimal solution is required.

      void issueStatusMsg (WitMsgID theMsgID);
         //
         // Issues the Msg identitied by theMsgID, passing it the solution
         // status code and text as arguments.

      void storePrimalSoln ();
         //
         // Stores the primal solution in myOptProblem ().

      void storeDualSoln ();
         //
         // Stores the dual solution in myOptProblem ().

      void setSpecCpxPars ();
         //
         // Sets the CPLEX parameters as specified by the CpxParSpecs.

      void setSpecIntCpxPar (WitCpxParSpec * theCpxParSpec);
         //
         // Sets the CPLEX parameter as specified by theCpxParSpec.
         // The type of theCpxParSpec must be int.

      void setSpecDblCpxPar (WitCpxParSpec * theCpxParSpec);
         //
         // Sets the CPLEX parameter as specified by theCpxParSpec.
         // The type of theCpxParSpec must be double.

      void checkCpxParamType (WitCpxParSpec * theCpxParSpec);
         //
         // Verifies that the type of the CPLEX parameter for theCpxParSpec
         // matches the type of theCpxParSpec.

      int findCpxParamNum (WitCpxParSpec * theCpxParSpec);
         //
         // Determines and returns the CPLEX parameter number for
         // theCpxParSpec, if any; otherwise issues a severe error.

      void setIntParam (int theCpxParam, int theValue);
         //
         // Sets the value of the CPLEX parameter indicated by theCpxParam to
         // theValue.

      void storeCplexStatus ();
         //
         // Retrieves the solution status code and text from CPLEX and stores it
         // in myOptComp ().

      void storeObjBoundInfo ();
         //
         // Retrieves the objective function bound and MIP relative gap from
         // CPLEX and stores it in myOptComp ().

      void checkErrCode (const char * theFuncName);
         //
         // If myErrCode_ is not 0, issues a severe message indicating
         // theErrCode as a CPLEX error code and theFuncName as the name of the
         // CPLEX function that returned the error code.

      bool mipMode ();
         //
         // Returns true, iff myOptProblem is a MIP.

      noCopyCtorAssign (WitCplexMgr);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitOptProblem * const myOptProblem_;
         //
         // The OptProblem that owns this CplexMgr.

      CPXENVptr myCpxEnv_;
         //
         // The CPLEX environment for this CplexMgr.

      CPXLPptr myCpxLp_;
         //
         // The CPLEX LP problem for this CplexMgr.

      int myErrCode_;
         //
         // The error code from the most recent call to CPLEX, if any.
   };

#endif // CPLEX_EMBEDDED

#endif
