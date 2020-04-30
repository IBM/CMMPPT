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

#include <CoinIf.h>

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
//       CoinIf
//          CoinMipIf
//------------------------------------------------------------------------------

class WitCoinMipIf: public WitCoinIf
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitCoinMipIf (WitOptProblem *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitCoinMipIf ();

      //------------------------------------------------------------------------
      // Overrides from class SolverIf.
      //------------------------------------------------------------------------

      virtual bool         lexOptNeedsReload ();
      virtual void         loadIntData       ();
      virtual void         solveMip          ();
      virtual const char * solverName        ();

   private:

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
         // Override from class CoinIf.

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      CbcModel * myCbcModel_;
         //
         // The CbcModel owned by this CoinMipIf.
   };

#endif
