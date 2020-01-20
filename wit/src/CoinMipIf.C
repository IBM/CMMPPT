//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "CoinMipIf.C"
//
// Contains the implementation of class CoinMipIf.
// If COIN_EMBEDDED is not defined, then only static functions are implemented.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// All #include statements for WIT header files must be located in the
// unconditional part of the source file, so that make depend will work
// correctly.
//------------------------------------------------------------------------------

#include <CoinMipIf.h>
#include <OptProblem.h>
#include <OptVar.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// COIN-embedded Implementation of class CoinMipIf.
//------------------------------------------------------------------------------

#ifdef COIN_EMBEDDED

#include <ClpSimplex.hpp>
#include <OsiClpSolverInterface.hpp>
#include <CbcModel.hpp>

//------------------------------------------------------------------------------

WitCoinMipIf * WitCoinMipIf::newInstance (WitOptSolveMgr * theOptSolveMgr)
   {
   return new WitCoinMipIf (theOptSolveMgr);
   }

//------------------------------------------------------------------------------

WitCoinMipIf::~WitCoinMipIf ()
   {
   shutDownMessageHandler ();

   enterCoin ();

   delete myCbcModel_;

   leaveCoin ();
   }

//------------------------------------------------------------------------------

void WitCoinMipIf::loadIntData ()
   {
   ClpModel *  theClpModel;
   WitOptVar * theOptVar;

   theClpModel = myClpModel ();

   forEachEl (theOptVar, myOptProblem ()->myOptVars ())
      {
      if (theOptVar->isAnIntVar ())
         {
         theClpModel->setInteger (theOptVar->index ());
         }
      }
   }

//------------------------------------------------------------------------------

void WitCoinMipIf::reviseLp ()
   {
   stronglyAssert (false);
   }

//------------------------------------------------------------------------------

void WitCoinMipIf::loadInitSoln (const WitVector <double> &)
   {
   stronglyAssert (false);
   }

//------------------------------------------------------------------------------

void WitCoinMipIf::reSolveLp ()
   {
   stronglyAssert (false);
   }

//------------------------------------------------------------------------------

void WitCoinMipIf::solveLp (bool)
   {
   stronglyAssert (false);
   }

//------------------------------------------------------------------------------

void WitCoinMipIf::solveMip (bool)
   {
   myCbcModel_->branchAndBound ();

   myMsgFac () ("coinNYISmsg", "Optimizing Implosion in MIP mode (3)");
   }

//------------------------------------------------------------------------------

void WitCoinMipIf::getDualSoln (WitVector <double> & dualSoln)
   {
   stronglyAssert (false);
   }

//------------------------------------------------------------------------------

const char * WitCoinMipIf::solverName ()
   {
   return "CBC";
   }

//------------------------------------------------------------------------------

WitCoinMipIf::WitCoinMipIf (WitOptSolveMgr * theOptSolveMgr):

      WitCoinComIf (theOptSolveMgr),
      myCbcModel_  (NULL)
   {
   OsiClpSolverInterface * theOsiClpSI;

   enterCoin ();

   theOsiClpSI = new OsiClpSolverInterface;

   myCbcModel_ = new CbcModel (* theOsiClpSI);

   delete theOsiClpSI;

   leaveCoin ();

   setUpMessageHandler ();
   }

//------------------------------------------------------------------------------

ClpModel * WitCoinMipIf::myClpModel ()
   {
   OsiClpSolverInterface * theOsiClpSI;

   theOsiClpSI =
      dynamic_cast <OsiClpSolverInterface *> (myCbcModel_->solver ());

   return theOsiClpSI->getModelPtr ();
   }

#endif // COIN_EMBEDDED

//------------------------------------------------------------------------------
// Non-COIN-embedded Implementation of class CoinMipIf.
//------------------------------------------------------------------------------

#ifndef COIN_EMBEDDED

WitCoinMipIf * WitCoinMipIf::newInstance (WitOptSolveMgr *)
   {
   stronglyAssert (false);

   return NULL;
   }

#endif // Not COIN_EMBEDDED
