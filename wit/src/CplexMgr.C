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

//------------------------------------------------------------------------------
// Source file: "CplexMgr.C"
//
// Contains the implementations of NonClass functions for working with class
//    CplexMgr. The implementation is conditional on whether or not
//    CPLEX_EMBEDDED is defined.
//
// Contains the implementation of class CplexMgr.
//    The implementation of class CplexMgr is compiled only if CPLEX_EMBEDDED is
//    defined.
//------------------------------------------------------------------------------

#include <CplexMgrNC.h>
#include <CplexMgr.h>
#include <OptProblem.h>

//------------------------------------------------------------------------------
// CPLEX-embedded Implementation of NonClass CplexMgr functions.
//------------------------------------------------------------------------------

#ifdef CPLEX_EMBEDDED

bool WitNonClass::cplexEmbedded ()
   {
   return true;
   }

//------------------------------------------------------------------------------

WitCplexMgr * WitNonClass::newCplexMgr (WitOptProblem * theOptProblem)
   {
   return new WitCplexMgr (theOptProblem);
   }

//------------------------------------------------------------------------------

void WitNonClass::deleteCplexMgr (WitCplexMgr * theCplexMgr)
   {
   delete theCplexMgr;
   }

//------------------------------------------------------------------------------

void WitNonClass::solveOptProb (WitCplexMgr * theCplexMgr)
   {
   theCplexMgr->solveOptProb ();
   }

#endif // CPLEX_EMBEDDED

//------------------------------------------------------------------------------
// Non-CPLEX-embedded Implementation of NonClass CplexMgr functions.
//------------------------------------------------------------------------------

#ifndef CPLEX_EMBEDDED

bool WitNonClass::cplexEmbedded ()
   {
   return false;
   }

//------------------------------------------------------------------------------

WitCplexMgr * WitNonClass::newCplexMgr (WitOptProblem *)
   {
   stronglyAssert (false);

   return NULL;
   }

//------------------------------------------------------------------------------

void WitNonClass::deleteCplexMgr (WitCplexMgr *)
   {
   stronglyAssert (false);
   }

//------------------------------------------------------------------------------

void WitNonClass::solveOptProb (WitCplexMgr *)
   {
   stronglyAssert (false);
   }

#endif // not CPLEX_EMBEDDED

//------------------------------------------------------------------------------
// Implementation of class CplexMgr.
//------------------------------------------------------------------------------

#ifdef CPLEX_EMBEDDED

//------------------------------------------------------------------------------

#include <OptComp.h>
#include <CpxParSpecMgr.h>
#include <CpxParSpec.h>
#include <MultiObjMgr.h>
#include <SaeMgr.h>
#include <OptVar.h>
#include <OptCon.h>
#include <OptStarter.h>
#include <MsgFrag.h>
#include <MsgFac.h>
#include <Timing.h>

#include <float.h>

//------------------------------------------------------------------------------

WitCplexMgr::WitCplexMgr (WitOptProblem * theOptProblem):

      WitProbAssoc  (theOptProblem),
      myOptProblem_ (theOptProblem),
      myCpxEnv_     (NULL),
      myCpxLp_      (NULL),
      myErrCode_    (0)
   {
   setUpCplex ();
   }

//------------------------------------------------------------------------------

WitCplexMgr::~WitCplexMgr ()
   {
   shutDownCplex ();
   }

//------------------------------------------------------------------------------

void WitCplexMgr::solveOptProb ()
   {
   WitTimer::enterSection ("cplex");

   if      (myOptComp ()->multiObjMode ())
      {
      solveOptProbAsLexOpt ();
      }
   else if (myOptProblem_->reSolveMode ())
      {
      reSolveOptProbAsLp ();
      }
   else if (mipMode ())
      {
      solveOptProbAsMip ();
      }
   else
      {
      solveOptProbAsLp ();
      }

   WitTimer::leaveSection ("cplex");
   }

//------------------------------------------------------------------------------

void WitCplexMgr::setUpCplex ()
   {
   int theErrCode;

   myCpxEnv_ = CPXopenCPLEX (& theErrCode);

   if (theErrCode != 0)
      myMsgFac () ("nonZeroOpenCplexErrCodeSmsg", theErrCode);

   stronglyAssert (myCpxEnv_ != NULL);

   //...........................................................................

   setUpLogFile ();

   //...........................................................................

   myCpxLp_ = CPXcreateprob (myCpxEnv_, & myErrCode_, "Implosion");

   checkErrCode ("createprob");

   stronglyAssert (myCpxLp_ != NULL);
   }

//------------------------------------------------------------------------------

void WitCplexMgr::setUpLogFile ()
   {
   const char * theFileName;
   CPXFILEptr   theCpxFile;

   theFileName = myOptComp ()->solverLogFileName ().myCstring ();

   theCpxFile  = CPXfopen (theFileName, "w");

   if (theCpxFile == NULL)
      {
      myMsgFac () ("cpxfopenSmsg", theFileName, "w");
      }

   //...........................................................................

   myErrCode_ = CPXsetlogfile (myCpxEnv_, theCpxFile);

   checkErrCode ("CPXsetlogfile");
   }

//------------------------------------------------------------------------------

void WitCplexMgr::shutDownCplex ()
   {
   myErrCode_ = CPXfreeprob (myCpxEnv_, & myCpxLp_);

   checkErrCode ("CPXfreeprob");

   //...........................................................................

   shutDownLogFile ();

   //...........................................................................

   myErrCode_ = CPXcloseCPLEX (& myCpxEnv_);

   checkErrCode ("CPXcloseCPLEX");
   }

//------------------------------------------------------------------------------

void WitCplexMgr::shutDownLogFile ()
   {
   CPXFILEptr theCpxFile;

   myErrCode_ = CPXgetlogfile (myCpxEnv_, & theCpxFile);

   checkErrCode ("CPXgetlogfile");

   //...........................................................................

   myErrCode_ = CPXsetlogfile (myCpxEnv_, NULL);

   checkErrCode ("CPXsetlogfile");

   //...........................................................................

   myErrCode_ = CPXfclose (theCpxFile);

   checkErrCode ("CPXfclose");
   }

//------------------------------------------------------------------------------

void WitCplexMgr::solveOptProbAsLp ()
   {
   issueSolveMsg ();

   loadLp ();

   writeMps ();

   setLpMethodByOptStarter ();

   loadInitSoln ();

   solveLp (myOptProblem_->needDual ());

   storePrimalSoln ();

   if (myOptProblem_->needDual ())
      storeDualSoln ();
   }

//------------------------------------------------------------------------------

void WitCplexMgr::reSolveOptProbAsLp ()
   {
   myMsgFac () ("reSolveLpMsg");

   reviseLp ();

   writeMps ();

   setIntParam (CPX_PARAM_LPMETHOD, CPX_ALG_DUAL);

   solveLp (myOptProblem_->needDual ());

   storePrimalSoln ();

   if (myOptProblem_->needDual ())
      storeDualSoln ();
   }

//------------------------------------------------------------------------------

void WitCplexMgr::solveOptProbAsMip ()
   {
   issueSolveMsg ();

   loadLp ();

   loadIntData ();

   writeMps ();

   solveMip (false);

   storePrimalSoln ();
   }

//------------------------------------------------------------------------------

void WitCplexMgr::solveOptProbAsLexOpt ()
   {
   issueSolveMsg ();

   myMsgFac () ("lexOptMsg");

   loadLp ();

   if (mipMode ())
      loadIntData ();

   writeMps ();

   solveLexOpt ();

   storePrimalSoln ();
   }

//------------------------------------------------------------------------------

void WitCplexMgr::issueSolveMsg ()
   {
   CPXCCHARptr theVersionID;

   myMsgFac () ("solveOptProblemMsg",
      myMsgFac ().myFrag (mipMode ()? "mipFrag": "lpFrag"));

   theVersionID = CPXversion (myCpxEnv_);

   stronglyAssert (theVersionID != NULL);

   myMsgFac () ("cplexVersionMsg", theVersionID);
   }

//------------------------------------------------------------------------------

void WitCplexMgr::loadLp ()
   {
   WitVector <double> objective;
   WitVector <double> rhs;
   WitVector <char>   sense;
   WitVector <int>    matbeg;
   WitVector <int>    matcnt;
   WitVector <int>    matind;
   WitVector <double> matval;
   WitVector <double> lb;
   WitVector <double> ub;

   getRowData    (rhs,       sense);
   getColumnData (objective, matbeg, matcnt, matind, matval, lb, ub);

   myErrCode_ =
      CPXcopylp (
         myCpxEnv_,
         myCpxLp_,
         myOptProblem_->nOptVars (),
         myOptProblem_->nOptCons (),
         -1,
         objective.myCVec (),
         rhs      .myCVec (),
         sense    .myCVec (),
         matbeg   .myCVec (),
         matcnt   .myCVec (),
         matind   .myCVec (),
         matval   .myCVec (),
         lb       .myCVec (),
         ub       .myCVec (),
         NULL);

   checkErrCode ("CPXcopylp");
   }

//------------------------------------------------------------------------------

void WitCplexMgr::getRowData (
      WitVector <double> & rhs,
      WitVector <char> &   sense)
   {
   WitOptCon * theOptCon;
   int         theIdx;

   WitTimer::enterSection ("opt-prob");

   rhs  .resize (myOptProblem_->nOptCons ());
   sense.resize (myOptProblem_->nOptCons ());

   forEachEl (theOptCon, myOptProblem_->myOptCons ())
      {
      theIdx = theOptCon->index ();

      getConData (rhs[theIdx], sense[theIdx], theOptCon);
      }

   WitTimer::leaveSection ("opt-prob");
   }

//------------------------------------------------------------------------------

void WitCplexMgr::getColumnData (
      WitVector <double> & objective,
      WitVector <int> &    matbeg,
      WitVector <int> &    matcnt,
      WitVector <int> &    matind,
      WitVector <double> & matval,
      WitVector <double> & lb,
      WitVector <double> & ub)
   {
   int         ncols;
   WitOptVar * theOptVar;
   int         theIdx;

   WitTimer::enterSection ("opt-prob");

   ncols = myOptProblem_->nOptVars ();

   objective.resize (ncols);
   matcnt   .resize (ncols);
   lb       .resize (ncols);
   ub       .resize (ncols);

   myOptProblem_->getMatrixByCols (matbeg, matind, matval);

   forEachEl (theOptVar, myOptProblem_->myOptVars ())
      {
      theIdx            = theOptVar->index ();

      objective[theIdx] = theOptVar->objCoeff ();

      matcnt[theIdx]    = matbeg[theIdx + 1] - matbeg[theIdx];

      lb[theIdx]        = theOptVar->bounds ().lower ();
      ub[theIdx]        = theOptVar->bounds ().upper ();
      }

   WitTimer::leaveSection ("opt-prob");
   }

//------------------------------------------------------------------------------

void WitCplexMgr::reviseLp ()
   {
   reviseBounds    ();
   reviseRHS       ();
   reviseObjCoeffs ();
   }

//------------------------------------------------------------------------------

void WitCplexMgr::reviseBounds ()
   {
   int                cnt;
   WitVector <int>    indices;
   WitVector <char>   lu;
   WitVector <double> bd;
   WitOptVar *        theOptVar;
   int                theColIdx;

   WitTimer::enterSection ("opt-prob");

   cnt = myOptProblem_->nOptVars ();

   indices.resize (cnt);
   lu     .resize (cnt);
   bd     .resize (cnt);

   theColIdx = -1;

   forEachEl (theOptVar, myOptProblem_->myOptVars ())
      {
      theColIdx ++;

      witAssert (theColIdx == theOptVar->index ());

      indices[theColIdx] = theColIdx;
      lu     [theColIdx] = 'L';
      bd     [theColIdx] = theOptVar->bounds ().lower ();
      }

   myErrCode_ =
      CPXchgbds (
         myCpxEnv_,
         myCpxLp_,
         cnt,
         indices.myCVec (),
         lu     .myCVec (),
         bd     .myCVec ());

   checkErrCode ("CPXchgbds");

   theColIdx = -1;

   forEachEl (theOptVar, myOptProblem_->myOptVars ())
      {
      theColIdx ++;

      lu[theColIdx] = 'U';
      bd[theColIdx] = theOptVar->bounds ().upper ();
      }

   myErrCode_ =
      CPXchgbds (
         myCpxEnv_,
         myCpxLp_,
         cnt,
         indices.myCVec (),
         lu     .myCVec (),
         bd     .myCVec ());

   checkErrCode ("myCpxEnv");

   WitTimer::leaveSection ("opt-prob");
   }

//------------------------------------------------------------------------------

void WitCplexMgr::reviseRHS ()
   {
   int                cnt;
   WitVector <int>    indices;
   WitVector <double> values;
   WitOptCon *        theOptCon;
   int                theRowIdx;
   char               sense;

   WitTimer::enterSection ("opt-prob");

   cnt = myOptProblem_->nOptCons ();

   indices.resize (cnt);
   values .resize (cnt);

   theRowIdx = -1;

   forEachEl (theOptCon, myOptProblem_->myOptCons ())
      {
      theRowIdx ++;

      witAssert (theRowIdx == theOptCon->index ());

      indices[theRowIdx] = theRowIdx;

      getConData (values[theRowIdx], sense, theOptCon);
      }

   myErrCode_ =
      CPXchgrhs (
         myCpxEnv_,
         myCpxLp_,
         cnt,
         indices.myCVec (),
         values .myCVec ());

   checkErrCode ("CPXchgrhs");

   WitTimer::leaveSection ("opt-prob");
   }

//------------------------------------------------------------------------------

void WitCplexMgr::getConData (double & rhs, char & sense, WitOptCon * theOptCon)
   {
   double theLB;
   double theUB;

   theLB  = theOptCon->bounds ().lower ();
   theUB  = theOptCon->bounds ().upper ();

   if (theLB == theUB)
      {
      rhs   = theLB;
      sense = 'E';
      }
   else if (theUB == + DBL_MAX)
      {
      rhs   = theLB;
      sense = 'G';
      }
   else if (theLB == - DBL_MAX)
      {
      rhs   = theUB;
      sense = 'L';
      }
   else
      {
      stronglyAssert (false);
      }
   }

//------------------------------------------------------------------------------

void WitCplexMgr::reviseObjCoeffs ()
   {
   int                cnt;
   WitVector <int>    indices;
   WitVector <double> values;
   WitOptVar *        theOptVar;
   int                theColIdx;

   WitTimer::enterSection ("opt-prob");

   cnt = myOptProblem_->nOptVars ();

   indices.resize (cnt);
   values .resize (cnt);

   theColIdx = -1;

   forEachEl (theOptVar, myOptProblem_->myOptVars ())
      {
      theColIdx ++;

      witAssert (theColIdx == theOptVar->index ());

      indices[theColIdx] = theColIdx;
      values [theColIdx] = theOptVar->objCoeff ();
      }

   myErrCode_ =
      CPXchgobj (
         myCpxEnv_,
         myCpxLp_,
         cnt,
         indices.myCVec (),
         values .myCVec ());

   checkErrCode ("CPXchgobj");

   WitTimer::leaveSection ("opt-prob");
   }

//------------------------------------------------------------------------------

void WitCplexMgr::loadIntData ()
   {
   int              nIntVars;
   WitVector <int>  theIndices;
   WitVector <char> theTypeChars;
   int              theIdx;
   WitOptVar *      theOptVar;

   WitTimer::enterSection ("opt-prob");

   nIntVars = countIntVars ();

   theIndices  .resize (nIntVars);
   theTypeChars.resize (nIntVars, 'I');

   theIdx = -1;

   forEachEl (theOptVar, myOptProblem_->myOptVars ())
      {
      if (theOptVar->isAnIntVar ())
         {
         theIdx ++;

         theIndices[theIdx] = theOptVar->index ();
         }
      }

   WitTimer::leaveSection ("opt-prob");

   myErrCode_ =
      CPXchgctype (
         myCpxEnv_,
         myCpxLp_,
         nIntVars,
         theIndices  .myCVec (),
         theTypeChars.myCVec ());

   checkErrCode ("CPXchgctype");
   }

//------------------------------------------------------------------------------

int WitCplexMgr::countIntVars ()
   {
   int         nIntVars;
   WitOptVar * theOptVar;

   WitTimer::enterSection ("opt-prob");

   nIntVars = 0;

   forEachEl (theOptVar, myOptProblem_->myOptVars ())
      {
      if (theOptVar->isAnIntVar ())
         nIntVars ++;
      }

   WitTimer::leaveSection ("opt-prob");

   return nIntVars;
   }

//------------------------------------------------------------------------------

void WitCplexMgr::writeMps ()
   {
   if (myOptComp ()->printMps ())
      {
      myMsgFac () ("mpsFileMsg");

      myErrCode_ =
         CPXwriteprob (
            myCpxEnv_,
            myCpxLp_,
            "opt-prob.mps",
            NULL);

      checkErrCode ("CPXwriteprob");
      }
   }

//------------------------------------------------------------------------------

void WitCplexMgr::solveLexOpt ()
   {
   WitOptVar *              prevOptVar;
   WitPtrVecItr <WitOptVar> theOptVarItr;
   WitOptVar *              theOptVar;

   if (devMode ())
      WitTimer::getTimeAndChargeToCurrent ();

   if (not mipMode ())
      loadInitSoln ();

   prevOptVar = NULL;

   myOptProblem_->myLexOptVarSeq ().attachItr (theOptVarItr);

   while (theOptVarItr.advance (theOptVar))
      {
      myMsgFac () ("optLexObjElemMsg", theOptVar->lexObjElemName ());

      if (prevOptVar != NULL)
         {
         lockLexObjElemVal (prevOptVar);

         setObjCoef (prevOptVar, 0.0);
         }

      setObjCoef (theOptVar, 1.0);

      if (mipMode ())
         {
         solveMip (true);
         }
      else
         {
         if (prevOptVar != NULL)
            setIntParam (CPX_PARAM_LPMETHOD, CPX_ALG_PRIMAL);
         else
            setLpMethodByOptStarter ();

         solveLp (true);
         }

      if (devMode ())
         if (WitSaeMgr::standAloneMode ())
            myMsgFac () ("lexObjElemCpuTimeMsg",
               WitTimer::getTimeAndChargeToCurrent ());

      prevOptVar = theOptVar;
      }
   }

//------------------------------------------------------------------------------

void WitCplexMgr::setObjCoef (WitOptVar * theOptVar, double theVal)
   {
   int    indices[1];
   double values [1];

   indices[0]  = theOptVar->index ();
   values [0]  = theVal;

   myErrCode_ =
      CPXchgobj (
         myCpxEnv_,
         myCpxLp_,
         1,
         indices,
         values);

   checkErrCode ("CPXchgobj");
   }

//------------------------------------------------------------------------------

void WitCplexMgr::lockLexObjElemVal (WitOptVar * theOptVar)
   {
   int    theColIdx;
   double primalSoln[1];
   double optObjVal;
   double moTol;
   double theTol;
   int    indices[1];
   char   lu     [1];
   double bd     [1];

   theColIdx     = theOptVar->index ();

   myErrCode_ =
      CPXgetx (
         myCpxEnv_,
         myCpxLp_,
         primalSoln,
         theColIdx, theColIdx);

   checkErrCode ("CPXgetx");

   optObjVal     = primalSoln[0];

   moTol         = myOptComp ()->myMultiObjMgr ()->multiObjTol ();

   theTol        = WitNonClass::max (moTol * fabs (optObjVal), moTol);

   indices[0]    = theColIdx;
   lu     [0]    = 'L';
   bd     [0]    = optObjVal - theTol;

   myErrCode_ =
      CPXchgbds (
         myCpxEnv_,
         myCpxLp_,
         1,
         indices,
         lu,
         bd);

   checkErrCode ("CPXchgbds");
   }

//------------------------------------------------------------------------------

void WitCplexMgr::setLpMethodByOptStarter ()
   {
   if (myOptComp ()->crashOptStarter ()->isChosen ())
      {
      setIntParam (CPX_PARAM_LPMETHOD, CPX_ALG_DUAL);
      }
   else
      {
      setIntParam (CPX_PARAM_LPMETHOD, CPX_ALG_PRIMAL);
      }
   }

//------------------------------------------------------------------------------

void WitCplexMgr::solveLp (bool optNeeded)
   {
   setSpecCpxPars ();

   myErrCode_ =
      CPXlpopt (myCpxEnv_, myCpxLp_);

   checkErrCode ("CPXlpopt");

   checkLpSolnStatus (optNeeded);

   printLpSolveInfo ();
   }

//------------------------------------------------------------------------------

void WitCplexMgr::loadInitSoln ()
   {
   WitVector <double> initSoln;
   WitOptVar *        theVar;
      
   if (not myOptComp ()->optInitMethod ()->external ())
      return;

   WitTimer::enterSection ("opt-prob");

   initSoln.resize (myOptProblem_->nOptVars (), 0.0);

   forEachEl (theVar, myOptProblem_->myOptVars ())
      initSoln[theVar->index ()] = theVar->primalValue ();

   WitTimer::leaveSection ("opt-prob");

   myErrCode_ =
      CPXcopystart (
         myCpxEnv_,
         myCpxLp_,
         NULL,
         NULL,
         initSoln.myCVec (),
         NULL,
         NULL,
         NULL);

   checkErrCode ("CPXcopystart");
   }

//------------------------------------------------------------------------------

void WitCplexMgr::printLpSolveInfo ()
   {
   int nSimpItns;
   int nPh1Itns;

   nSimpItns = CPXgetitcnt     (myCpxEnv_, myCpxLp_);
   nPh1Itns  = CPXgetphase1cnt (myCpxEnv_, myCpxLp_);

   myMsgFac () ("lpSolveInfoMsg", nSimpItns, nPh1Itns);
   }

//------------------------------------------------------------------------------

void WitCplexMgr::checkLpSolnStatus (bool optNeeded)
   {
   storeCplexStatus ();

   switch (myOptComp ()->cplexStatusCode ())
      {
      case CPX_STAT_OPTIMAL:
         {
         myMsgFac () ("optSolnFoundMsg");

         return;
         }

      case CPX_STAT_INFEASIBLE:
         myMsgFac () ("infeasSmsg");

      case CPX_STAT_UNBOUNDED:
      case CPX_STAT_INForUNBD:
         myMsgFac () ("unboundedOrInfeasSmsg");

      case CPX_STAT_ABORT_IT_LIM:
      case CPX_STAT_ABORT_OBJ_LIM:
      case CPX_STAT_ABORT_TIME_LIM:
      case CPX_STAT_NUM_BEST:
      case CPX_STAT_OPTIMAL_INFEAS:
         {
         repEarlyTermLpSolnStatus (optNeeded);

         return;
         }

      default:
         issueStatusMsg ("unexpCpxStatSmsg");
      }
   }

//------------------------------------------------------------------------------

void WitCplexMgr::repEarlyTermLpSolnStatus (bool optNeeded)
   {
   int primalFeas;

   myErrCode_ =
      CPXsolninfo (
         myCpxEnv_,
         myCpxLp_,
         NULL,
         NULL,
       & primalFeas,
         NULL);

   checkErrCode ("CPXsolninfo");

   if (not primalFeas)
      issueStatusMsg ("inFeasLpCpxStatSmsg");

   else if (optNeeded)
      issueStatusMsg ("nonOptLpCpxStatSmsg");

   else
      issueStatusMsg ("nonOptButFeasLpCpxStatWmsg");
   }

//------------------------------------------------------------------------------

void WitCplexMgr::solveMip (bool optNeeded)
   {
   if (countIntVars () == 0)
      myMsgFac () ("mipModeNoIntVarsSmsg");

   setSpecCpxPars ();

   myErrCode_ =
      CPXmipopt (myCpxEnv_, myCpxLp_);

   checkErrCode ("CPXmipopt");

   checkMipSolnStatus (optNeeded);

   storeObjBoundInfo ();

   printMipSolveInfo ();
   }

//------------------------------------------------------------------------------

void WitCplexMgr::printMipSolveInfo ()
   {
   int    nSimpItns;
   int    nNodes;
   double percentGap;

   nSimpItns  = CPXgetmipitcnt (myCpxEnv_, myCpxLp_);
   nNodes     = CPXgetnodecnt  (myCpxEnv_, myCpxLp_);

   percentGap = myOptComp ()->cplexMipRelGap () * 100.0;

   myMsgFac () ("mipSolveInfoMsg",
      nSimpItns,
      nNodes,
      percentGap,
      myOptComp ()->cplexMipBound ());
   }

//------------------------------------------------------------------------------

void WitCplexMgr::checkMipSolnStatus (bool optNeeded)
   {
   storeCplexStatus ();

   switch (myOptComp ()->cplexStatusCode ())
      {
      case CPXMIP_OPTIMAL:
      case CPXMIP_OPTIMAL_TOL:
         {
         myMsgFac () ("optSolnFoundMsg");

         return;
         }

      case CPXMIP_INFEASIBLE:
         myMsgFac () ("infeasSmsg");

      case CPXMIP_UNBOUNDED:
      case CPXMIP_INForUNBD:
         myMsgFac () ("unboundedOrInfeasSmsg");

      case CPXMIP_ABORT_FEAS:
      case CPXMIP_FAIL_FEAS:
      case CPXMIP_FAIL_FEAS_NO_TREE:
      case CPXMIP_MEM_LIM_FEAS:
      case CPXMIP_NODE_LIM_FEAS:
      case CPXMIP_SOL_LIM:
      case CPXMIP_TIME_LIM_FEAS:
         {
         if (optNeeded)
            issueStatusMsg ("nonOptMipCpxStatSmsg");
         else
            issueStatusMsg ("nonOptButFeasMipCpxStatWmsg");

         return;
         }

      case CPXMIP_ABORT_INFEAS:
      case CPXMIP_FAIL_INFEAS:
      case CPXMIP_FAIL_INFEAS_NO_TREE:
      case CPXMIP_MEM_LIM_INFEAS:
      case CPXMIP_NODE_LIM_INFEAS:
      case CPXMIP_OPTIMAL_INFEAS:
      case CPXMIP_TIME_LIM_INFEAS:
         issueStatusMsg ("inFeasMipCpxStatSmsg");

      default:
         issueStatusMsg ("unexpCpxStatSmsg");
      }
   }

//------------------------------------------------------------------------------

void WitCplexMgr::issueStatusMsg (WitMsgID theMsgID)
   {
   myMsgFac () (theMsgID,
      myOptComp ()->cplexStatusCode (),
      myOptComp ()->cplexStatusText ());
   }

//------------------------------------------------------------------------------

void WitCplexMgr::storePrimalSoln ()
   {
   WitVector <double> primalSoln;
   WitOptVar *        theVar;
   int                theIdx;

   primalSoln.resize (myOptProblem_->nOptVars ());

   myErrCode_ =
      CPXgetx (
         myCpxEnv_,
         myCpxLp_,
         primalSoln.myCVecForUpdate (),
         0,
         myOptProblem_->nOptVars () - 1);

   checkErrCode ("CPXgetx");

   WitTimer::enterSection ("opt-prob");

   forEachEl (theVar, myOptProblem_->myOptVars ())
      {
      theIdx = theVar->index ();

      theVar->setPrimalValue (primalSoln[theIdx]);
      }

   WitTimer::leaveSection ("opt-prob");
   }

//------------------------------------------------------------------------------

void WitCplexMgr::storeDualSoln ()
   {
   WitVector <double> dualSoln;
   WitOptCon *        theCon;
   int                theIdx;

   dualSoln.resize (myOptProblem_->nOptCons ());

   myErrCode_ =
      CPXgetpi (
         myCpxEnv_,
         myCpxLp_,
         dualSoln.myCVecForUpdate (),
         0,
         myOptProblem_->nOptCons () - 1);

   checkErrCode ("CPXgetpi");

   WitTimer::enterSection ("opt-prob");

   forEachEl (theCon, myOptProblem_->myOptCons ())
      {
      theIdx = theCon->index ();

      theCon->setDualValue (dualSoln[theIdx]);
      }

   WitTimer::leaveSection ("opt-prob");
   }

//------------------------------------------------------------------------------

void WitCplexMgr::setSpecCpxPars ()
   {
   WitCpxParSpec * theCpxParSpec;

   forEachEl (theCpxParSpec, myOptComp ()->myCpxParSpecMgr ()->myCpxParSpecs ())
      {
      if (theCpxParSpec->valTypeIsInt ())
         setSpecIntCpxPar (theCpxParSpec);
      else
         setSpecDblCpxPar (theCpxParSpec);
      }
   }

//------------------------------------------------------------------------------

void WitCplexMgr::setSpecIntCpxPar (WitCpxParSpec * theCpxParSpec)
   {
   int theParamNum;

   theParamNum = findCpxParamNum (theCpxParSpec);

   checkCpxParamType (theCpxParSpec);

   myErrCode_ =
      CPXsetintparam (
         myCpxEnv_,
         theParamNum,
         theCpxParSpec->myIntVal ());

   checkErrCode ("CPXsetintparam");

   myMsgFac () ("intCpxParSetMsg",
      theCpxParSpec->myName   (),
      theCpxParSpec->myIntVal ());
   }

//------------------------------------------------------------------------------

void WitCplexMgr::setSpecDblCpxPar (WitCpxParSpec * theCpxParSpec)
   {
   int theParamNum;

   theParamNum = findCpxParamNum (theCpxParSpec);

   checkCpxParamType (theCpxParSpec);

   myErrCode_ =
      CPXsetdblparam (
         myCpxEnv_,
         theParamNum,
         theCpxParSpec->myDblVal ());

   checkErrCode ("CPXsetdblparam");

   myMsgFac () ("dblCpxParSetMsg",
      theCpxParSpec->myName   (),
      theCpxParSpec->myDblVal ());
   }

//------------------------------------------------------------------------------

void WitCplexMgr::checkCpxParamType (WitCpxParSpec * theCpxParSpec)
   {
   int theParamNum;
   int theParamType;

   theParamNum = findCpxParamNum (theCpxParSpec);

   myErrCode_  =
      CPXgetparamtype (myCpxEnv_, theParamNum, & theParamType);

   checkErrCode ("CPXgetparamtype");

   if (theCpxParSpec->valTypeIsInt ())
      {
      if    (theParamType != CPX_PARAMTYPE_INT)
         if (theParamType != CPX_PARAMTYPE_LONG)
            myMsgFac () ("wrongCpxParamTypeSmsg",
               theCpxParSpec->myName (),
               "Int",
               theParamNum);
      }
   else
      {
      if (theParamType != CPX_PARAMTYPE_DOUBLE)
         myMsgFac () ("wrongCpxParamTypeSmsg",
            theCpxParSpec->myName (),
            "Double",
            theParamNum);
      }
   }

//------------------------------------------------------------------------------

int WitCplexMgr::findCpxParamNum (WitCpxParSpec * theCpxParSpec)
   {
   int       theParamNameLen;
   WitString theParamName;
   int       theParamNum;

   theParamNameLen = theCpxParSpec->myName ().length () + 10;

   theParamName.resize (theParamNameLen);

   sprintf (
      theParamName.myCstringForUpdate (),
      "CPX_PARAM_%s",
      theCpxParSpec->myName ().myCstring ());

   myErrCode_      =
      CPXgetparamnum (
         myCpxEnv_,
         theParamName.myCstring (),
       & theParamNum);

   if (myErrCode_ == CPXERR_BAD_PARAM_NAME)
      myMsgFac () ("badCpxParamNameSmsg", theCpxParSpec->myName ());

   checkErrCode ("getparamnum");

   return theParamNum;
   }

//------------------------------------------------------------------------------

void WitCplexMgr::setIntParam (int theCpxParam, int theValue)
   {
   myErrCode_ =
      CPXsetintparam (myCpxEnv_, theCpxParam, theValue);

   checkErrCode ("CPXsetintparam");
   }

//------------------------------------------------------------------------------

void WitCplexMgr::storeCplexStatus ()
   {
   int  theStatCode;
   char theStatText[57];

   theStatCode = CPXgetstat (myCpxEnv_, myCpxLp_);

   CPXgetstatstring (myCpxEnv_, theStatCode, theStatText);

   myOptComp ()->storeCplexStatusCode (theStatCode);
   myOptComp ()->storeCplexStatusText (theStatText);
   }

//------------------------------------------------------------------------------

void WitCplexMgr::storeObjBoundInfo ()
   {
   double theMipBound;
   double theMipRelGap;

   myErrCode_ =
      CPXgetbestobjval (myCpxEnv_, myCpxLp_, & theMipBound);

   checkErrCode ("CPXgetbestobjval");

   myErrCode_ =
      CPXgetmiprelgap (myCpxEnv_, myCpxLp_, & theMipRelGap);

   checkErrCode ("CPXgetmiprelgap");

   myOptComp ()->storeCplexMipBound  (theMipBound);
   myOptComp ()->storeCplexMipRelGap (theMipRelGap);
   }

//------------------------------------------------------------------------------

void WitCplexMgr::checkErrCode (const char * theFuncName)
   {
   if (myErrCode_ == 0)
      return;

   char theErrStr[4096] = "                   Unknown CPLEX error\n";

   CPXgeterrorstring (myCpxEnv_, myErrCode_, theErrStr);

   myMsgFac () ("nonZeroCplexErrCodeSmsg", theErrStr, theFuncName);
   }

//------------------------------------------------------------------------------

bool WitCplexMgr::mipMode ()
   {
   return myOptComp ()->mipMode ();
   }

//------------------------------------------------------------------------------

#endif // CPLEX_EMBEDDED
