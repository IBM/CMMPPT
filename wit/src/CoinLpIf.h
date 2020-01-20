//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef CoinLpIfH
#define CoinLpIfH

//------------------------------------------------------------------------------
// Header file: "CoinLpIf.h"
//
// Contains the declaration of class CoinLpIf.
//------------------------------------------------------------------------------

#include <CoinComIf.h>

class ClpSimplex;

//------------------------------------------------------------------------------
// Class CoinLpIf
//
// "COIN LP Interface"
// Responsible for all interactions with COIN that occur in LP mode only.
//
// Class Hierarchy:
//
// ProbAssoc
//    SolverIf
//       CoinComIf
//          CoinLpIf
//------------------------------------------------------------------------------

class WitCoinLpIf: public WitCoinComIf
   {
   public:

      //------------------------------------------------------------------------
      // Static public member functions.
      //------------------------------------------------------------------------

      static WitCoinLpIf * newInstance (WitOptSolveMgr * theOptSolveMgr);
         //
         // If COIN is embedded,
         //    creates and returns a new CoinLpIf for theOptSolveMgr.
         // If COIN is not embedded,
         //    issues a fatal error.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitCoinLpIf ();

      //------------------------------------------------------------------------
      // Overrides from class SolverIf.
      //------------------------------------------------------------------------

      virtual void         loadIntData  ();
      virtual void         reviseLp     ();
      virtual void         loadInitSoln (const WitVector <double> &);
      virtual void         reSolveLp    ();
      virtual void         solveLp      (bool);
      virtual void         solveMip     (bool);
      virtual void         getDualSoln  (WitVector <double> &);
      virtual const char * solverName   ();

   private:

      //------------------------------------------------------------------------
      // Private constructor functions.
      //------------------------------------------------------------------------

      WitCoinLpIf (WitOptSolveMgr *);

      //------------------------------------------------------------------------
      // Other private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitCoinLpIf);

      virtual ClpModel * myClpModel ();
         //
         // Override from class CoinComIf.

      void reviseVarBounds ();
         //
         // Revises the upper and lower bounds on variables of the LP problem
         // that was previously loaded into CLP.

      void reviseConBounds ();
         //
         // Revises the upper and lower bounds on constraints of the LP problem
         // that was previously loaded into CLP.

      void reviseObjCoeffs ();
         //
         // Revises the objective function coefficients of the LP problem that
         // was previously loaded into CLP.

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      ClpSimplex * myClpSimplex_;
         //
         // The ClpSimplex owned by this CoinLpIf.
   };

#endif
