//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef CplexIfH
#define CplexIfH

//------------------------------------------------------------------------------
// Header file: "CplexIf.h"
//
// Contains the declaration of class CplexIf.
//------------------------------------------------------------------------------

#include <SolverIf.h>

//------------------------------------------------------------------------------
// The following typedefs are defined in the CPLEX header file cpxconst.h.
// They are being duplicated here so as to avoid the need to #include CPLEX
// header files from this header file, in order to allow this header file to be
// #included by files that aren't set up to imbed CPLEX code. 
//------------------------------------------------------------------------------

typedef struct cpxenv * CPXENVptr;
typedef struct cpxlp  * CPXLPptr;

//------------------------------------------------------------------------------
// class CplexIf
//
// "CPLEX Interface"
// Responsible for all interactions with CPLEX.
//
// Class Hierarchy:
//
// ProbAssoc
//    SolverIf
//       CplexIf
//------------------------------------------------------------------------------

class WitCplexIf: public WitSolverIf
   {
   public:

      //------------------------------------------------------------------------
      // Static public member functions.
      //------------------------------------------------------------------------

      static bool cplexEmbedded ();
         //
         // Returns true, iff CPLEX embedded into the current build of WIT.

      static WitCplexIf * newInstanceIfAllowed (WitOptProblem * theOptProblem);
         //
         // If CPLEX is embedded,
         //    creates and returns a new CplexIf for theOptProblem.
         // If CPLEX is not embedded,
         //    issues a fatal error.

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitCplexIf (WitOptProblem *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitCplexIf ();

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

      virtual void reSolveOptProbAsLp     ();
      virtual void solveOptProbAsMip      ();
      virtual void solveOptProbAsLexOpt   ();
      virtual void finishSolveOptProbAsLp ();
      virtual void issueSolveMsg          ();
      virtual void loadLp                 ();
      virtual void writeMpsSS             ();
         //
         // Overrides from class SolverIf.

      void getRowData (
            WitVector <double> & rhs,
            WitVector <char> &   sense);
         //
         // Retrieves the row portion of the LP aspect of the problem in the
         // representation required for CPXcopylp.

      void getMatcnt (
                  WitVector <int> & matcnt,
            const WitVector <int> & matbeg);
         //
         // Computes matcnt from matbeg as required for CPXcopylp.

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

      noCopyCtorAssign (WitCplexIf);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      CPXENVptr myCpxEnv_;
         //
         // The CPLEX environment for this CplexIf.

      CPXLPptr myCpxLp_;
         //
         // The CPLEX LP problem for this CplexIf.

      int myErrCode_;
         //
         // The error code from the most recent call to CPLEX, if any.
   };

#endif
