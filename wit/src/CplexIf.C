//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "CplexIf.C"
//
// Contains the implementations of member functions of class OpSolverIf whose
//    implementation depends on whether or not CPLEX_EMBEDDED is defined.
//
// Contains the implementation of class CplexIf.
//    The implementation of class CplexIf is compiled only if CPLEX_EMBEDDED is
//    defined.
//------------------------------------------------------------------------------

#include <CplexIf.h>
#include <OptProblem.h>

//------------------------------------------------------------------------------
// CPLEX-embedded Implementation of OpSolverIf functions.
//------------------------------------------------------------------------------

#ifdef CPLEX_EMBEDDED

bool WitOpSolverIf::cplexEmbedded ()
   {
   return true;
   }

//------------------------------------------------------------------------------

WitOpSolverIf * WitOpSolverIf::newInstanceForCplex (
      WitOptProblem * theOptProblem)
   {
   return new WitCplexIf (theOptProblem);
   }

#endif // CPLEX_EMBEDDED

//------------------------------------------------------------------------------
// CPLEX-embedded Implementation of OpSolverIf functions.
//------------------------------------------------------------------------------

#ifndef CPLEX_EMBEDDED

bool WitOpSolverIf::cplexEmbedded ()
   {
   return false;
   }

//------------------------------------------------------------------------------

WitOpSolverIf * WitOpSolverIf::newInstanceForCplex (WitOptProblem *)
   {
   stronglyAssert (false);

   return NULL;
   }

#endif // not CPLEX_EMBEDDED

//------------------------------------------------------------------------------
// Implementation of class CplexIf.
//------------------------------------------------------------------------------

#ifdef CPLEX_EMBEDDED

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

WitCplexIf::WitCplexIf (WitOptProblem * theOptProblem):

      WitOpSolverIf (theOptProblem),
      myCpxEnv_     (NULL),
      myCpxLp_      (NULL),
      myErrCode_    (0)
   {
   setUpCplex ();
   }

//------------------------------------------------------------------------------

WitCplexIf::~WitCplexIf ()
   {
   shutDownCplex ();
   }

//------------------------------------------------------------------------------

void WitCplexIf::solveOptProb ()
   {
   WitTimer::enterSection ("cplex");

   if      (myOptComp ()->multiObjMode ())
      {
      solveOptProbAsLexOpt ();
      }
   else if (myOptProblem ()->reSolveMode ())
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

void WitCplexIf::setUpCplex ()
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

void WitCplexIf::setUpLogFile ()
   {
   const char * theFileName;

   theFileName = myOptComp ()->solverLogFileName ().myCstring ();

   myErrCode_  = CPXsetlogfilename (myCpxEnv_, theFileName, "w");

   checkErrCode ("CPXsetlogfilename");
   }

//------------------------------------------------------------------------------

void WitCplexIf::shutDownCplex ()
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

void WitCplexIf::shutDownLogFile ()
   {
   myErrCode_  = CPXsetlogfilename (myCpxEnv_, NULL, "");

   checkErrCode ("CPXsetlogfilename");
   }

//------------------------------------------------------------------------------

void WitCplexIf::solveOptProbAsLp ()
   {
   issueSolveMsg ();

   loadLp ();

   writeMps ();

   setLpMethodByOptStarter ();

   loadInitSoln ();

   solveLp (myOptProblem ()->needDual ());

   storePrimalSoln ();

   if (myOptProblem ()->needDual ())
      storeDualSoln ();
   }

//------------------------------------------------------------------------------

void WitCplexIf::reSolveOptProbAsLp ()
   {
   myMsgFac () ("reSolveLpMsg");

   reviseLp ();

   writeMps ();

   setIntParam (CPX_PARAM_LPMETHOD, CPX_ALG_DUAL);

   solveLp (myOptProblem ()->needDual ());

   storePrimalSoln ();

   if (myOptProblem ()->needDual ())
      storeDualSoln ();
   }

//------------------------------------------------------------------------------

void WitCplexIf::solveOptProbAsMip ()
   {
   issueSolveMsg ();

   loadLp ();

   loadIntData ();

   writeMps ();

   solveMip (false);

   storePrimalSoln ();
   }

//------------------------------------------------------------------------------

void WitCplexIf::solveOptProbAsLexOpt ()
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

void WitCplexIf::issueSolveMsg ()
   {
   CPXCCHARptr theVersionID;

   myMsgFac () ("solveOptProblemMsg",
      myMsgFac ().myFrag (mipMode ()? "mipFrag": "lpFrag"));

   theVersionID = CPXversion (myCpxEnv_);

   stronglyAssert (theVersionID != NULL);

   myMsgFac () ("cplexVersionMsg", theVersionID);
   }

//------------------------------------------------------------------------------

void WitCplexIf::loadLp ()
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
         myOptProblem ()->nOptVars (),
         myOptProblem ()->nOptCons (),
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

void WitCplexIf::getRowData (
      WitVector <double> & rhs,
      WitVector <char> &   sense)
   {
   WitOptCon * theOptCon;
   int         theIdx;

   WitTimer::enterSection ("opt-prob");

   rhs  .resize (myOptProblem ()->nOptCons ());
   sense.resize (myOptProblem ()->nOptCons ());

   forEachEl (theOptCon, myOptProblem ()->myOptCons ())
      {
      theIdx = theOptCon->index ();

      getConData (rhs[theIdx], sense[theIdx], theOptCon);
      }

   WitTimer::leaveSection ("opt-prob");
   }

//------------------------------------------------------------------------------

void WitCplexIf::getColumnData (
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

   ncols = myOptProblem ()->nOptVars ();

   objective.resize (ncols);
   matcnt   .resize (ncols);
   lb       .resize (ncols);
   ub       .resize (ncols);

   myOptProblem ()->getMatrixByCols (matbeg, matind, matval);

   forEachEl (theOptVar, myOptProblem ()->myOptVars ())
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

void WitCplexIf::reviseLp ()
   {
   reviseBounds    ();
   reviseRHS       ();
   reviseObjCoeffs ();
   }

//------------------------------------------------------------------------------

void WitCplexIf::reviseBounds ()
   {
   int                cnt;
   WitVector <int>    indices;
   WitVector <char>   lu;
   WitVector <double> bd;
   WitOptVar *        theOptVar;
   int                theColIdx;

   WitTimer::enterSection ("opt-prob");

   cnt = myOptProblem ()->nOptVars ();

   indices.resize (cnt);
   lu     .resize (cnt);
   bd     .resize (cnt);

   theColIdx = -1;

   forEachEl (theOptVar, myOptProblem ()->myOptVars ())
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

   forEachEl (theOptVar, myOptProblem ()->myOptVars ())
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

void WitCplexIf::reviseRHS ()
   {
   int                cnt;
   WitVector <int>    indices;
   WitVector <double> values;
   WitOptCon *        theOptCon;
   int                theRowIdx;
   char               sense;

   WitTimer::enterSection ("opt-prob");

   cnt = myOptProblem ()->nOptCons ();

   indices.resize (cnt);
   values .resize (cnt);

   theRowIdx = -1;

   forEachEl (theOptCon, myOptProblem ()->myOptCons ())
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

void WitCplexIf::getConData (double & rhs, char & sense, WitOptCon * theOptCon)
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

void WitCplexIf::reviseObjCoeffs ()
   {
   int                cnt;
   WitVector <int>    indices;
   WitVector <double> values;
   WitOptVar *        theOptVar;
   int                theColIdx;

   WitTimer::enterSection ("opt-prob");

   cnt = myOptProblem ()->nOptVars ();

   indices.resize (cnt);
   values .resize (cnt);

   theColIdx = -1;

   forEachEl (theOptVar, myOptProblem ()->myOptVars ())
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

void WitCplexIf::loadIntData ()
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

   forEachEl (theOptVar, myOptProblem ()->myOptVars ())
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

int WitCplexIf::countIntVars ()
   {
   int         nIntVars;
   WitOptVar * theOptVar;

   WitTimer::enterSection ("opt-prob");

   nIntVars = 0;

   forEachEl (theOptVar, myOptProblem ()->myOptVars ())
      {
      if (theOptVar->isAnIntVar ())
         nIntVars ++;
      }

   WitTimer::leaveSection ("opt-prob");

   return nIntVars;
   }

//------------------------------------------------------------------------------

void WitCplexIf::writeMps ()
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

void WitCplexIf::solveLexOpt ()
   {
   WitOptVar *              prevOptVar;
   WitPtrVecItr <WitOptVar> theOptVarItr;
   WitOptVar *              theOptVar;

   if (devMode ())
      WitTimer::getTimeAndChargeToCurrent ();

   if (not mipMode ())
      loadInitSoln ();

   prevOptVar = NULL;

   myOptProblem ()->myLexOptVarSeq ().attachItr (theOptVarItr);

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

void WitCplexIf::setObjCoef (WitOptVar * theOptVar, double theVal)
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

void WitCplexIf::lockLexObjElemVal (WitOptVar * theOptVar)
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

void WitCplexIf::setLpMethodByOptStarter ()
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

void WitCplexIf::solveLp (bool optNeeded)
   {
   setSpecCpxPars ();

   myErrCode_ =
      CPXlpopt (myCpxEnv_, myCpxLp_);

   checkErrCode ("CPXlpopt");

   checkLpSolnStatus (optNeeded);

   printLpSolveInfo ();
   }

//------------------------------------------------------------------------------

void WitCplexIf::loadInitSoln ()
   {
   WitVector <double> initSoln;
   WitOptVar *        theVar;
      
   if (not myOptComp ()->optInitMethod ()->external ())
      return;

   WitTimer::enterSection ("opt-prob");

   initSoln.resize (myOptProblem ()->nOptVars (), 0.0);

   forEachEl (theVar, myOptProblem ()->myOptVars ())
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

void WitCplexIf::printLpSolveInfo ()
   {
   int nSimpItns;
   int nPh1Itns;

   nSimpItns = CPXgetitcnt     (myCpxEnv_, myCpxLp_);
   nPh1Itns  = CPXgetphase1cnt (myCpxEnv_, myCpxLp_);

   myMsgFac () ("lpSolveInfoMsg", nSimpItns, nPh1Itns);
   }

//------------------------------------------------------------------------------

void WitCplexIf::checkLpSolnStatus (bool optNeeded)
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

void WitCplexIf::repEarlyTermLpSolnStatus (bool optNeeded)
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

void WitCplexIf::solveMip (bool optNeeded)
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

void WitCplexIf::printMipSolveInfo ()
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

void WitCplexIf::checkMipSolnStatus (bool optNeeded)
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

void WitCplexIf::issueStatusMsg (WitMsgID theMsgID)
   {
   myMsgFac () (theMsgID,
      myOptComp ()->cplexStatusCode (),
      myOptComp ()->cplexStatusText ());
   }

//------------------------------------------------------------------------------

void WitCplexIf::storePrimalSoln ()
   {
   WitVector <double> primalSoln;
   WitOptVar *        theVar;
   int                theIdx;

   primalSoln.resize (myOptProblem ()->nOptVars ());

   myErrCode_ =
      CPXgetx (
         myCpxEnv_,
         myCpxLp_,
         primalSoln.myCVecForUpdate (),
         0,
         myOptProblem ()->nOptVars () - 1);

   checkErrCode ("CPXgetx");

   WitTimer::enterSection ("opt-prob");

   forEachEl (theVar, myOptProblem ()->myOptVars ())
      {
      theIdx = theVar->index ();

      theVar->setPrimalValue (primalSoln[theIdx]);
      }

   WitTimer::leaveSection ("opt-prob");
   }

//------------------------------------------------------------------------------

void WitCplexIf::storeDualSoln ()
   {
   WitVector <double> dualSoln;
   WitOptCon *        theCon;
   int                theIdx;

   dualSoln.resize (myOptProblem ()->nOptCons ());

   myErrCode_ =
      CPXgetpi (
         myCpxEnv_,
         myCpxLp_,
         dualSoln.myCVecForUpdate (),
         0,
         myOptProblem ()->nOptCons () - 1);

   checkErrCode ("CPXgetpi");

   WitTimer::enterSection ("opt-prob");

   forEachEl (theCon, myOptProblem ()->myOptCons ())
      {
      theIdx = theCon->index ();

      theCon->setDualValue (dualSoln[theIdx]);
      }

   WitTimer::leaveSection ("opt-prob");
   }

//------------------------------------------------------------------------------

void WitCplexIf::setSpecCpxPars ()
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

void WitCplexIf::setSpecIntCpxPar (WitCpxParSpec * theCpxParSpec)
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

void WitCplexIf::setSpecDblCpxPar (WitCpxParSpec * theCpxParSpec)
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

void WitCplexIf::checkCpxParamType (WitCpxParSpec * theCpxParSpec)
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

int WitCplexIf::findCpxParamNum (WitCpxParSpec * theCpxParSpec)
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

void WitCplexIf::setIntParam (int theCpxParam, int theValue)
   {
   myErrCode_ =
      CPXsetintparam (myCpxEnv_, theCpxParam, theValue);

   checkErrCode ("CPXsetintparam");
   }

//------------------------------------------------------------------------------

void WitCplexIf::storeCplexStatus ()
   {
   int  theStatCode;
   char theStatText[57];

   theStatCode = CPXgetstat (myCpxEnv_, myCpxLp_);

   CPXgetstatstring (myCpxEnv_, theStatCode, theStatText);

   myOptComp ()->storeCplexStatusCode (theStatCode);
   myOptComp ()->storeCplexStatusText (theStatText);
   }

//------------------------------------------------------------------------------

void WitCplexIf::storeObjBoundInfo ()
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

void WitCplexIf::checkErrCode (const char * theFuncName)
   {
   if (myErrCode_ == 0)
      return;

   char theErrStr[4096] = "                   Unknown CPLEX error\n";

   CPXgeterrorstring (myCpxEnv_, myErrCode_, theErrStr);

   myMsgFac () ("nonZeroCplexErrCodeSmsg", theErrStr, theFuncName);
   }

//------------------------------------------------------------------------------

bool WitCplexIf::mipMode ()
   {
   return myOptComp ()->mipMode ();
   }

//------------------------------------------------------------------------------

#endif // CPLEX_EMBEDDED
