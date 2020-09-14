//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
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
// Class CplexIf
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

      static WitCplexIf * newInstance (WitOptProblem * theOptProblem);
         //
         // If CPLEX is embedded,
         //    creates and returns a new CplexIf for theOptProblem.
         // If CPLEX is not embedded,
         //    issues a fatal error.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitCplexIf ();

      //------------------------------------------------------------------------
      // Overrides from class SolverIf.
      //------------------------------------------------------------------------

      virtual void         issueVersionMsg ();
      virtual void         loadLp          ();
      virtual void         loadIntData     ();
      virtual void         reviseLp        ();
      virtual void         solverWriteMps  ();
      virtual void         loadInitSoln    (const WitVector <double> &);
      virtual void         solveLp         (bool);
      virtual void         reSolveLp       ();
      virtual void         reSolveLexLp    ();
      virtual void         solveMip        ();
      virtual void         setVarLB        (WitOptVar *, double);
      virtual void         setObjCoeff     (WitOptVar *, double);
      virtual double       primalVarVal    (WitOptVar *);
      virtual void         getPrimalSoln   (WitVector <double> &);
      virtual void         getDualSoln     (WitVector <double> &);
      virtual const char * solverName      ();

   private:

      //------------------------------------------------------------------------
      // Private constructor functions.
      //------------------------------------------------------------------------

      WitCplexIf (WitOptProblem *);

      //------------------------------------------------------------------------
      // Other private member functions.
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

      void reviseColData ();
         //
         // Revises the bounds and objective coefficients of variables of the
         // LP problem that was previously loaded into CPLEX.

      void getColIndices (WitVector <int> & indices);
         //
         // Sets indices to a vector of column indices for the opt problem.

      void reviseRHS ();
         //
         // Revises the right-hand-side values of the LP problem that was
         // previously loaded into CPLEX.

      void getConData (double & rhs, char & sense, WitOptCon * theOptCon);
         //
         // Sets rhs and sense to the CPLEX RHS and constraint sense for
         // theOptCon.

      void printLpSolveInfo ();
         //
         // Prints some information about the LP solve.

      void checkLpSolnStatus ();
         //
         // Checks the status of the LP solution.

      void printMipSolveInfo ();
         //
         // Prints some information about the MIP solve.

      void checkMipSolnStatus ();
         //
         // Checks the status of the MIP solution.

      void issueStatusMsg (WitMsgID theMsgID);
         //
         // Issues the Msg identitied by theMsgID, passing it the solution
         // status code and text as arguments.

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

      static void enterCplex ();
         //
         // Indicates, for timing purposes, that CPLEX functions are about to
         // be called.

      static void leaveCplex ();
         //
         // Indicates, for timing purposes, that CPLEX functions are no longer
         // about to be called.

      noCopyCtorAssign (WitCplexIf);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      CPXENVptr myCpxEnv_;
         //
         // The CPLEX environment owned by this CplexIf.

      CPXLPptr myCpxLp_;
         //
         // The CPLEX LP problem owned by this CplexIf.

      int myErrCode_;
         //
         // The error code from the most recent call to CPLEX, if any.
   };

#endif
