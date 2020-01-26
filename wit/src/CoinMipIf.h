//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef CoinMipIfH
#define CoinMipIfH

//------------------------------------------------------------------------------
// Header file: "CoinMipIf.h"
//
// Contains the declaration of class CoinMipIf.
//------------------------------------------------------------------------------

#include <CoinComIf.h>

class CbcModel;

//------------------------------------------------------------------------------
// Class CoinMipIf
//
// "COIN MIP Interface"
// Responsible for all interactions with COIN that occur in MIP mode only.
//
// Class Hierarchy:
//
// ProbAssoc
//    SolverIf
//       CoinComIf
//          CoinMipIf
//------------------------------------------------------------------------------

class WitCoinMipIf: public WitCoinComIf
   {
   public:

      //------------------------------------------------------------------------
      // Static public member functions.
      //------------------------------------------------------------------------

      static WitCoinMipIf * newInstance (WitOptSolveMgr * theOptSolveMgr);
         //
         // If COIN is embedded,
         //    creates and returns a new CoinMipIf for theOptSolveMgr.
         // If COIN is not embedded,
         //    issues a fatal error.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitCoinMipIf ();

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

      WitCoinMipIf (WitOptSolveMgr *);

      //------------------------------------------------------------------------
      // Other private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitCoinMipIf);

      void checkMipSolnStatus ();
         //
         // Checks the status of the MIP solution.

      void printMipSolveInfo ();
         //
         // Prints some information about the MIP solve.

      virtual OsiSolverInterface * myOsiSI ();
         //
         // Override from class CoinComIf.

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      CbcModel * myCbcModel_;
         //
         // The CbcModel owned by this CoinMipIf.
   };

#endif
