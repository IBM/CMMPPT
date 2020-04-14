//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//----------------------------------------------------------------------------
// Source file: "OptProblem.C"
//
// Contains the implementation of class OptProblem.
//----------------------------------------------------------------------------

#include <OptProblem.h>
#include <OptVar.h>
#include <OptCon.h>
#include <Coeff.h>
#include <OptSolveMgr.h>
#include <OptComp.h>
#include <Timing.h>
#include <MsgFrag.h>
#include <MsgFac.h>
#include <Session.h>

#include <float.h>

//------------------------------------------------------------------------------
// Implementation of class OptProblem.
//------------------------------------------------------------------------------

inline WitCoeff * & WitOptProblem::varLastCoeff (const WitOptVar * theVar)
   {
   return varLastCoeff_[theVar->index ()];
   }

//------------------------------------------------------------------------------

void WitOptProblem::store (WitOptVar * theVar)
   {
   myOptVars_.append (theVar);
   }

//------------------------------------------------------------------------------

void WitOptProblem::store (WitOptCon * theCon)
   {
   myOptCons_.append (theCon);
   }

//------------------------------------------------------------------------------

void WitOptProblem::generateFixed ()
   {
   generateVariables ();

   countIntVars ();

   generateConstraints ();

   generateCoeffs ();

   if (myOptComp ()->multiObjMode ())
      {
      generateLexOptVarSeq (myLexOptVarSeq_);
      }

   postGenFixed ();

   myMsgFac () ("lpMipDimMsg",
      myMsgFac ().myFrag (myOptComp ()->mipMode ()? "mipFrag": "lpFrag"),
      nOptVars (),
      nOptCons (),
      nCoeffs  ());

   if (myOptComp ()->mipMode ())
      myMsgFac () ("nIntVarsMsg", nIntVars_);

   if (devMode ())
      displayDerived ();
   }

//------------------------------------------------------------------------------

void WitOptProblem::countIntVars ()
   {
   WitOptVar * theVar;

   nIntVars_ = 0;

   forEachEl (theVar, myOptVars_)
      if (theVar->isAnIntVar ())
         nIntVars_ ++;
   }

//------------------------------------------------------------------------------

void WitOptProblem::incCoeff (WitOptVar * theVar, double delta)
   {
   if (fabs (delta) <= FLOAT_EPSILON)
      return;

   WitCoeff * theCoeff;

   theCoeff = varLastCoeff (theVar);

   if (theCoeff != NULL)
      if (theCoeff->myCon () == curCon_)
         {
         theCoeff->increment (delta);

         return;
         }

   theCoeff = new WitCoeff (curCon_, theVar, delta);

   varLastCoeff (theVar) = theCoeff;

   curConCoeffs_.push (theCoeff);

   nCoeffs_ ++;
   }

//------------------------------------------------------------------------------

void WitOptProblem::generateFlexible ()
   {
   WitOptVar * theOptVar;
   WitOptCon * theOptCon;

   forEachEl (theOptVar, myOptVars_)
      theOptVar->compBounds ();

   forEachEl (theOptCon, myOptCons_)
      theOptCon->compBounds ();

   generateObjCoeffs ();

   postGenFlexible ();
   }

//------------------------------------------------------------------------------

const WitBoundPair & WitOptProblem::preScreenBounds (const WitOptVar * theVar)
   {
   return varPreScreenBounds_.myElemAt (theVar->index ());
   }

//------------------------------------------------------------------------------

const WitBoundPair & WitOptProblem::preScreenBounds (const WitOptCon * theCon)
   {
   return conPreScreenBounds_.myElemAt (theCon->index ());
   }

//------------------------------------------------------------------------------
// calcInitSoln calculates an initial solution.
// First the independent variable are initialized, by calling indepInitValue.
// Then the dependent variables are initialized with a call to initDepVars.
//------------------------------------------------------------------------------

void WitOptProblem::calcInitSoln ()
   {
   WitOptVar * theVar;

   forEachEl (theVar, myOptVars_)
      theVar->setPrimalValue (theVar->indepInitValue ());

   initDepVars ();
   }

//------------------------------------------------------------------------------

void WitOptProblem::print ()
   {
   WitTimer::enterSection ("extra");

   myMsgFac () ("writeOptProblemMsg");

   optProbFile_ = openFile ("opt-prob.out", "w");

   myProblem ()->mySession ()->writeHeading (optProbFile ());

   if (myOptComp ()->mipMode ())
      fprintf (optProbFile (),
         "Mixed Integer Programming Problem\n"
         "===== ======= =========== =======\n\n\n");
   else
      fprintf (optProbFile (),
         "Linear Programming Problem\n"
         "====== =========== =======\n\n\n");

   fprintf (optProbFile (), "# Variables:    %8d\n",   nOptVars ());

   if (myOptComp ()->mipMode ())
      fprintf (optProbFile (), 
                            "# Integer Vars: %8d\n",   nIntVars_);

   fprintf (optProbFile (), "# Constraints:  %8d\n",   nOptCons ());
   fprintf (optProbFile (), "# Coefficients: %8d\n\n", nCoeffs ());

   printVars    ();
   printCons    ();
   printObjFunc ();

   if (myOptComp ()->multiObjMode ())
      {
      printLexSeq ();
      }

   printDerived ();

   if (myOptComp ()->printMatByCols ())
      prtMatrixByCols ();

   fclose (optProbFile ());

   optProbFile_ = NULL;

   WitTimer::leaveSection ("extra");
   }

//------------------------------------------------------------------------------

WitOptProblem::WitOptProblem (WitProblem * theProblem):

      WitProbAssoc    (theProblem),

      nCoeffs_        (0),
      curCon_         (NULL),
      curConCoeffs_   (myProblem ()),
      myLexOptVarSeq_ (),
      optProbFile_    (NULL),
      myOptSolveMgr_  (NULL)
   {
   }

//------------------------------------------------------------------------------

WitOptProblem::~WitOptProblem ()
   {
   int idx;

   delete myOptSolveMgr_;

   while (not myOptCons_.isEmpty ())
      delete myOptCons_.get ();

   while (not myOptVars_.isEmpty ())
      delete myOptVars_.get ();

   for (idx = 0; idx < conPreScreenBounds_.length (); idx ++)
      delete conPreScreenBounds_[idx];

   for (idx = 0; idx < varPreScreenBounds_.length (); idx ++)
      delete varPreScreenBounds_[idx];
   }

//------------------------------------------------------------------------------

void WitOptProblem::generateLexOptVarSeq (WitPtrVec <WitOptVar> & theOptVarSeq)
   {
   stronglyAssert (false);
   }

//------------------------------------------------------------------------------

void WitOptProblem::solve ()
   {
   if (not myOptComp ()->skipScreening ())
      doScreening ();

   if (myOptComp ()->printOptProblem ())
      print ();

   if (myOptSolveMgr_ == NULL)
       myOptSolveMgr_ = new WitOptSolveMgr (this);

   myOptSolveMgr_->solveOptProb ();

   if (needDual ())
      reconstructDual ();
   }

//------------------------------------------------------------------------------

double WitOptProblem::compObjValue ()
   {
   WitOptVar * theOptVar;
   double      theObjValue;

   theObjValue = 0.0;

   forEachEl (theOptVar, myOptVars_)
      {
      theObjValue +=
         theOptVar->objCoeff    () *
         theOptVar->primalValue ();
      }

   return theObjValue;
   }

//------------------------------------------------------------------------------

void WitOptProblem::getMatrixByCols (
      WitVector <int> &    firstCoeffIdx,
      WitVector <int> &    rowIdx,
      WitVector <double> & coeffVal)
   {
   WitCoeffItr theItr;
   WitOptVar * theVar;
   WitCoeff *  theCoeff;
   int         idx;

   rowIdx       .resize (nCoeffs_);
   firstCoeffIdx.resize (nOptVars () + 1);
   coeffVal     .resize (nCoeffs_);

   idx = 0;

   forEachEl (theVar, myOptVars_)
      {
      firstCoeffIdx[theVar->index ()] = idx;

      theVar->attachCoeffItr (theItr);

      while (theItr.advance (theCoeff))
         {
         rowIdx  [idx] = theCoeff->myCon ()->index ();

         coeffVal[idx] = theCoeff->myValue ();

         idx ++;
         }
      }

   firstCoeffIdx[nOptVars ()] = idx;

   witAssert (idx == nCoeffs ());
   }

//------------------------------------------------------------------------------

void WitOptProblem::getColumnData (
      WitVector <double> & colLB,
      WitVector <double> & colUB,
      WitVector <double> & colObj)
   {
   WitOptVar * theOptVar;

   colLB .resize (nOptVars ());
   colUB .resize (nOptVars ());
   colObj.resize (nOptVars ());

   forEachEl (theOptVar, myOptVars_)
      {
      colLB [theOptVar->index ()] = theOptVar->bounds ().lower ();
      colUB [theOptVar->index ()] = theOptVar->bounds ().upper ();

      colObj[theOptVar->index ()] = theOptVar->objCoeff ();
      }
   }

//------------------------------------------------------------------------------

void WitOptProblem::getRowData (
      WitVector <double> & rowLB,
      WitVector <double> & rowUB)
   {
   WitOptCon * theOptCon;

   rowLB .resize (nOptCons ());
   rowUB .resize (nOptCons ());

   forEachEl (theOptCon, myOptCons_)
      {
      rowLB [theOptCon->index ()] = theOptCon->bounds ().lower ();
      rowUB [theOptCon->index ()] = theOptCon->bounds ().upper ();
      }
   }

//------------------------------------------------------------------------------

void WitOptProblem::displayDerived ()
   {
   }

//------------------------------------------------------------------------------

void WitOptProblem::printDerived ()
   {
   }

//------------------------------------------------------------------------------
// initDepVars.
// Initializes the dependent variables, i.e., those variables whose
// initial value depends on the initial value of other variables.
//------------------------------------------------------------------------------

void WitOptProblem::initDepVars ()
   {
   }

//------------------------------------------------------------------------------

void WitOptProblem::saveBounds ()
   {
   WitOptVar * theVar;
   WitOptCon * theCon;

   varPreScreenBounds_.resize (nOptVars ());
   conPreScreenBounds_.resize (nOptCons ());

   forEachEl (theVar, myOptVars_)
      {
      varPreScreenBounds_          [theVar->index ()] = new WitBoundPair;

      varPreScreenBounds_.myElemAt (theVar->index ()) = theVar->bounds ();
      }

   forEachEl (theCon, myOptCons_)
      {
      conPreScreenBounds_          [theCon->index ()] = new WitBoundPair;

      conPreScreenBounds_.myElemAt (theCon->index ()) = theCon->bounds ();
      }
   }

//------------------------------------------------------------------------------

int WitOptProblem::screen (WitOptCon * theCon)
   {
   int         nVarsFixed;
   WitCoeffItr theItr;
   WitCoeff *  theCoeff;

   nVarsFixed = 0;

   if (theCon == NULL)
      return 0;

   if (not theCon->screenable ())
      return 0;

   theCon->attachCoeffItr (theItr);

   while (theItr.advance (theCoeff))
      {
      if (theCoeff->myValue () <= FLOAT_EPSILON)
         continue;

      if (theCoeff->myVar ()->bounds ().upper () == 0.0)
         continue;

      theCoeff->myVar ()->bounds ().upper () = 0.0;

      if (needDual ())
         screenedCoeffs_.prepend (theCoeff);

      ++ nVarsFixed;
      }

   return nVarsFixed;
   }

//------------------------------------------------------------------------------

void WitOptProblem::generateCoeffs ()
   {
   buildCoeffs ();

   allocVarCoeffs ();

   storeCoeffs ();
   }

//------------------------------------------------------------------------------

void WitOptProblem::buildCoeffs ()
   {
   WitCoeff * theCoeff;

   nCoeffs_ = 0;

   varLastCoeff_.resize (nOptVars ());
                
   forEachEl (curCon_, myOptCons_)
      {
      curCon_->generateCoeffs ();

      curCon_->allocMyCoeffs (curConCoeffs_.nElements ());

      while (curConCoeffs_.pop (theCoeff))
         curCon_->store (theCoeff);
      }

   varLastCoeff_.clear  ();
   }

//------------------------------------------------------------------------------

void WitOptProblem::allocVarCoeffs ()
   {
   WitOptVar * theVar;
   WitOptCon * theCon;
   WitCoeffItr theItr;
   WitCoeff *  theCoeff;

   WitVector <int> nCoeffsForVar (nOptVars (), 0);
      //
      // nCoeffsForVar[theVar->index ()] is # Coeffs for theVar.

   forEachEl (theCon, myOptCons_)
      {
      theCon->attachCoeffItr (theItr);

      while (theItr.advance (theCoeff))
         ++ nCoeffsForVar[theCoeff->myVar ()->index ()];
      }

   forEachEl (theVar, myOptVars_)
      theVar->allocMyCoeffs (nCoeffsForVar[theVar->index ()]);
   }

//------------------------------------------------------------------------------

void WitOptProblem::storeCoeffs ()
   {
   WitOptCon * theCon;
   WitOptVar * theVar;
   WitCoeffItr theItr;
   WitCoeff *  theCoeff;

   forEachEl (theCon, myOptCons_)
      {
      theCon->attachCoeffItr (theItr);

      while (theItr.advance (theCoeff))
         theCoeff->myVar ()->store (theCoeff);

      theCon->clearMyCoeffs ();
      }

   forEachEl (theVar, myOptVars_)
      {
      theVar->attachCoeffItr (theItr);

      while (theItr.advance (theCoeff))
         theCoeff->myCon ()->store (theCoeff);
      }
   }

//------------------------------------------------------------------------------

void WitOptProblem::generateObjCoeffs ()
   {
   WitOptVar * theVar;

   forEachEl (theVar, myOptVars_)
      theVar->generateObjCoeff ();
   }

//------------------------------------------------------------------------------

void WitOptProblem::printVars ()
   {
   WitOptVar * theVar;

   fprintf (optProbFile (),
      "\n\n"
      "Variables\n"
      "=========\n");

   fprintf (optProbFile (), "Var ");

   printOptVCHeading ();

   fprintf (optProbFile (), " I  Obj Cff Low Bnd  Up Bnd   PS LB   PS UB\n");

   forEachEl (theVar, myOptVars_)
      theVar->print ();
   }

//------------------------------------------------------------------------------

void WitOptProblem::printCons ()
   {
   WitOptCon * theCon;

   fprintf (optProbFile (),
      "\n\n"
      "Constraints\n"
      "===========\n");

   fprintf (optProbFile (), "Con ");

   printOptVCHeading ();

   fprintf (optProbFile (), " Low Bnd  Up Bnd   PS LB   PS UB\n");

   fprintf (optProbFile (), "Var ");

   printOptVCHeading ();

   fprintf (optProbFile (), "   Coeff\n");

   forEachEl (theCon, myOptCons_)
      theCon->print ();
   }

//------------------------------------------------------------------------------

void WitOptProblem::printObjFunc ()
   {
   WitOptVar * theOptVar;
   double      theObjCoeff;

   fprintf (optProbFile (),
      "\n\n"
      "Objective Function\n"
      "==================\n");

   fprintf (optProbFile (), "Var ");

   printOptVCHeading ();

   fprintf (optProbFile (), "    Coeff\n");

   forEachEl (theOptVar, myOptVars_)
      {
      theObjCoeff = theOptVar->objCoeff ();

      if (fabs (theObjCoeff) > FLOAT_EPSILON)
         {
         theOptVar->printID ();

         fprintf (optProbFile_, " %8.1f\n", theObjCoeff);
         }
      }
   }

//------------------------------------------------------------------------------

void WitOptProblem::printLexSeq ()
   {
   WitPtrVecItr <WitOptVar> theOptVarItr;
   WitOptVar *              theOptVar;

   stronglyAssert (myOptComp ()->multiObjMode ());

   fprintf (optProbFile (),
      "\n\n"
      "Sequence of Variables to be Lexicographically Maximized\n"
      "=======================================================\n\n");

   myLexOptVarSeq_.attachItr (theOptVarItr);

   while (theOptVarItr.advance (theOptVar))
      {
      theOptVar->printID ();

      fprintf (optProbFile (), "\n");
      }
   }

//------------------------------------------------------------------------------

void WitOptProblem::prtMatrixByCols ()
   {
   WitVector <int>    firstCoeffIdx;
   WitVector <int>    rowIdx;
   WitVector <double> coeffVal;
   int                theIdx;

   getMatrixByCols (firstCoeffIdx, rowIdx, coeffVal);

   fprintf (optProbFile (),
      "\n\n\n"
      "Matrix by Columns\n"
      "====== == =======\n\n");

   fprintf (optProbFile (), "\nColumn #      Elem #\n");

   for (theIdx = 0; theIdx <= nOptVars (); theIdx ++)
      fprintf (optProbFile (), "%8d  %8d\n", theIdx, firstCoeffIdx[theIdx]);

   fprintf (optProbFile (), "\n  Elem #     Row #    Coefficient\n");

   for (theIdx = 0; theIdx < nCoeffs (); theIdx ++)
      fprintf (
         optProbFile (),
         "%8d  %8d  %13.3f\n",
         theIdx,
         rowIdx  [theIdx],
         coeffVal[theIdx]);
   }

//------------------------------------------------------------------------------

void WitOptProblem::reconstructDual ()
   {
   int        nElim;
   WitCoeff * theCoeff;
   double     redReward;
   double     newDualVal;

   nElim = 0;

   while (not screenedCoeffs_.isEmpty ())
      {
      theCoeff  = screenedCoeffs_.get ();

      redReward = theCoeff->myVar ()->reducedReward ();

      if (redReward > FLOAT_EPSILON)
         {
         newDualVal = 
              theCoeff->myCon ()->dualValue ()
            + (redReward / theCoeff->myValue ());

         theCoeff->myCon ()->setDualValue (newDualVal);

         ++ nElim;
         }
      }

   if (devMode ())
      myMsgFac () ("nDualElimMsg", nElim);
   }
