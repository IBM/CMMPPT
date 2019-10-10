//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef OptProblemH
#define OptProblemH

//------------------------------------------------------------------------------
// Header file: "OptProblem.h"
//
// Contains the declaration of class OptProblem.
//------------------------------------------------------------------------------

#include <List.h>
#include <ObjStack.h>
#include <PtrVec.h>
#include <Assoc.h>

//------------------------------------------------------------------------------
// class OptProblem.
//
// An optimization problem, either:
//    a linear programming problem, or
//    a mixed integer programming problem.
//
// Class hierarchy:
//
//    ProbAssoc
//       OptProblem
//          DetImpOP
//          StochImpOP
//------------------------------------------------------------------------------

class WitOptProblem: public WitProbAssoc
   {
   public:

      //-----------------------------------------------------------------------
      // Public pure virtual member functions.
      //-----------------------------------------------------------------------

      virtual bool needDual () = 0;
         //
         // Returns true, iff a dual solution is needed.

      virtual bool reSolveMode () = 0;
         //
         // Returns true, iff solving this OptProblem would be a re-solve.

      //-----------------------------------------------------------------------
      // Data access functions.
      //-----------------------------------------------------------------------

      accessFunc (const WitList <WitOptVar> &,   myOptVars)
      accessFunc (const WitList <WitOptCon> &,   myOptCons)
      accessFunc (int,                           nCoeffs)
      accessFunc (const WitPtrVec <WitOptVar> &, myLexOptVarSeq)
      accessFunc (FILE *,                        optProbFile)

      inline int nOptVars ()
         {
         return myOptVars ().nElements ();
         }

      inline int nOptCons ()
         {
         return myOptCons ().nElements ();
         }

      //-----------------------------------------------------------------------
      // Other public member functions.
      //-----------------------------------------------------------------------

      void store (WitOptVar * theVar);
         //
         // theVar is a new OptVar created for this OptProblem.
         // This function stores theVar.

      void store (WitOptCon * theCon);
         //
         // theCon is a new OptCon created for this OptProblem.
         // This function stores theCon.

      void generateFixed ();
         //
         // Generates the fixed aspects of this OptProblem (the aspects that do
         // not change in accelerated mode).

      void countIntVars ();
         //
         // Sets nIntVars_.

      void incCoeff (WitOptVar * theVar, double delta);
         //
         // Adds delta to the Coeff of theVar in curCon_, constructing it,
         // if neccesary.  No-op, if delta is (nearly) 0.0.

      void generateFlexible ();
         //
         // Generates the flexible aspects of this OptProblem (the aspects that
         // may change in accelerated mode).

      const WitBoundPair & preScreenBounds (const WitOptVar * theVar);
         //
         // Returns the value that theVar->bounds () held just prior to
         // screening. Valid only if saveBounds has been called.

      const WitBoundPair & preScreenBounds (const WitOptCon * theCon);
         //
         // Returns the value that theCon->bounds () held just prior to
         // screening. Valid only if saveBounds has been called.

      void calcInitSoln ();
         //
         // Calculates an initial solution.

      void getMatrixByCols (
            WitVector <int> &    firstCoeffIdx,
            WitVector <int> &    rowIdx,
            WitVector <double> & coeffVal);
         //
         // Retrieves the constraint matrix in the column major ordered format.

      void print ();
         //
         // Prints this OptProblem.

      void solve ();
         //
         // Solves this OptProblem.

      double compObjValue ();
         //
         // Computes and returns the objective fnuction value for the current
         // primal solution.

   protected:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitOptProblem (WitProblem *);

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      virtual ~WitOptProblem ();

      //-----------------------------------------------------------------------
      // Protected pure virtual member functions.
      //-----------------------------------------------------------------------

      virtual void generateVariables () = 0;
         //
         // Generates all variables.

      virtual void generateConstraints () = 0;
         //
         // Generates all OptCons.

      virtual void generateLexOptVarSeq (WitPtrVec <WitOptVar> & theOptVarSeq);
         //
         // Sets theOptVarSeq to be the sequence of OptVars to be
         // lexicographically maximized.
         // Valid only in multiple objectives mode.

      virtual void postGenFixed () = 0;
         //
         // Takes actions appropriate at the conclusion of generateFixed ().

      virtual void postGenFlexible () = 0;
         //
         // Takes actions appropriate at the conclusion of generateFlexible ().

      virtual void printOptVCHeading () = 0;
         //
         // Prints a heading for OptVCs to optProbFile ().

      //-----------------------------------------------------------------------
      // Protected non-pure virtual member functions.
      //-----------------------------------------------------------------------

      virtual void displayDerived ();
         //
         // Display any problem-specific message in pre-release mode.
         // This default version is a no-op.

      virtual void printDerived ();
         //
         // Prints problem-specific aspects of this OptProblem.
         // This default version is a no-op.

      virtual void initDepVars ();
         //
         // Initializes the dependent variables, i.e., those variables whose
         // initial value depends on the initial value of other variables.

      //-----------------------------------------------------------------------
      // Other protected member functions.
      //-----------------------------------------------------------------------

      void saveBounds ();
         //
         // Copies bounds for each OptVar and OptCon into varPreScreenBounds_ 
         // and conPreScreenBounds_.

      int screen (WitOptCon * theCon);
         //
         // Performs generic screening on theCon and the variables it
         // intersects.
         // Returns # OptVars fixed.
         // No-op, if theCon is NULL.

   private:

      //-----------------------------------------------------------------------
      // Private member functions.
      //-----------------------------------------------------------------------

      void generateCoeffs ();
         //
         // Generates all Coeffs for all OptCons and stores them in the
         // OptVC myCoeffs_ vectors.

      void buildCoeffs ();
         //
         // Generates all Coeffs for all OptCons.

      void allocVarCoeffs ();
         //
         // Invokes allocMyCoeffs for each OptVar in this OptProblem.

      void storeCoeffs ();
         //
         // Stores all Coeffs in each OptVC, in OptCon order for OptVars and
         // OptVar order for OptCons.

      inline WitCoeff * & varLastCoeff (const WitOptVar * theVar);
         //
         // Return the most recently generated Coeff whose OptVar is theVar, or
         // NULL is none exists.

      void generateObjCoeffs ();
         //
         // For each OptVar, theVar, computes theVar->objCoeff.

      void printVars ();
         //
         // Prints all OptVars in this OptProblem.

      void printCons ();
         //
         // Prints all OptCons in this OptProblem.

      void printObjFunc ();
         //
         // Prints the objective function for this OptProblem.

      void printLexSeq ();
         //
         // Prints the sequence of OptVars to be lex maximized.
         // Valid only in multiple objectives mode.

      void prtMatrixByCols ();
         //
         // Prints the matrix in the column major ordered format.

      virtual void doScreening () = 0;
         //
         // Performs screening of constraints and variables.

      WitOpSolverIf * newOpSolverIf ();
         //
         // Creates and returns a new OpSolverIf for this OptProblem.
         // Issues a fatal error, if neither COIN nor CPLEX is embedded.

      void reconstructDual ();
         //
         // Assuming the optimization problem was screened and an optimal dual
         // solution to the screened problem has been obtained, this function
         // modifies the dual solution, in order to make it optimal and feasible
         // for the dual of the unscreened problem.

      noCopyCtorAssign (WitOptProblem);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitOpSolverIf * myOpSolverIf_;
         //
         // The OpSolverIf owned by this OptProblem, when there is one;
         // otherwise, NULL.
         // Always NULL, if CPLEX is not embedded.

      WitList <WitOptVar> myOptVars_;
         //
         // The list of all OptVars in this OptProblem.

      WitList <WitOptCon> myOptCons_;
         //
         // The list of all OptCons in this OptProblem.

      int nCoeffs_;
         //
         // # Coeffs in optimization problem.

      WitOptCon * curCon_;
         //
         // The OptCon whose Coeffs are currently being generated.

      WitObjStack <WitCoeff> curConCoeffs_;
         //
         // The Coeffs for curCon_;

      WitPtrVec <WitCoeff> varLastCoeff_;
         //
         // varLastCoeff_[theVar->index ()] is the most recently
         // generated Coeff whose OptVar is theVar, or NULL is none exists.
         // Allocated only during buildCoeffs.

      int nIntVars_;
         //
         // # integer variables in this OptProblem.

      WitPtrVec <WitOptVar> myLexOptVarSeq_;
         //
         // In multiple objectives mode:
         //    The sequence of OptVars to be lexicographically maximized.
         // In single objective mode:
         //    Empty

      WitPtrVec <WitBoundPair> varPreScreenBounds_;
         //
         // Stores the return values of preScreenBounds (OptVar *).

      WitPtrVec <WitBoundPair> conPreScreenBounds_;
         //
         // Stores the return values of preScreenBounds (OptCon *).

      WitList <WitCoeff> screenedCoeffs_;
         //
         // For each Coeff in screenedCoeffs_, theCoeff:
         //    theCoeff->myVar () is a variable that was screened out.
         //    theCoeff->myCon () is the constraint that enabled the
         //    variable to be screened out.
         // The Coeffs are stored in reverse of the order in which the
         // screening was performed.
         // Valid iff needDual () is true.

      FILE * optProbFile_;
         //
         // Optimization problem output file.
   };

#endif
