//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "CplexIf.C"
//
// Contains the implementation of class CplexIf.
// If CPLEX_EMBEDDED is not defined, then only static functions are implemented.
//------------------------------------------------------------------------------

#include <CplexIf.h>

//------------------------------------------------------------------------------
// CPLEX-embedded implementation of class CplexIf.
//------------------------------------------------------------------------------

#ifdef CPLEX_EMBEDDED

#include <OptComp.h>
#include <OptProblem.h>
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

#include <cplex.h>
#include <float.h>

//------------------------------------------------------------------------------

bool WitCplexIf::cplexEmbedded ()
   {
   return true;
   }

//------------------------------------------------------------------------------

WitCplexIf * WitCplexIf::newInstanceIfAllowed (WitOptProblem * theOptProblem)
   {
   return new WitCplexIf (theOptProblem);
   }

//------------------------------------------------------------------------------

WitCplexIf::WitCplexIf (WitOptProblem * theOptProblem):

      WitSolverIf (theOptProblem),
      myCpxEnv_   (NULL),
      myCpxLp_    (NULL),
      myErrCode_  (0)
   {
   setUpCplex ();
   }

//------------------------------------------------------------------------------

WitCplexIf::~WitCplexIf ()
   {
   shutDownCplex ();
   }

//------------------------------------------------------------------------------

void WitCplexIf::setUpCplex ()
   {
   int theErrCode;

   enteringCplex ();

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

   leftCplex ();
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
   enteringCplex ();

   myErrCode_ = CPXfreeprob (myCpxEnv_, & myCpxLp_);

   checkErrCode ("CPXfreeprob");

   //...........................................................................

   shutDownLogFile ();

   //...........................................................................

   myErrCode_ = CPXcloseCPLEX (& myCpxEnv_);

   checkErrCode ("CPXcloseCPLEX");

   leftCplex ();
   }

//------------------------------------------------------------------------------

void WitCplexIf::shutDownLogFile ()
   {
   myErrCode_  = CPXsetlogfilename (myCpxEnv_, NULL, "");

   checkErrCode ("CPXsetlogfilename");
   }

//------------------------------------------------------------------------------

void WitCplexIf::reSolveOptProbAsLp ()
   {
   myMsgFac () ("reSolveLpMsg");

   reviseLp ();

   writeMps ();

   setUseDualSimplex (true);

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

void WitCplexIf::finishSolveOptProbAsLp ()
   {
   storePrimalSoln ();

   if (myOptProblem ()->needDual ())
      storeDualSoln ();
   }

//------------------------------------------------------------------------------

void WitCplexIf::issueSolveMsg ()
   {
   CPXCCHARptr theVersionID;

   myMsgFac () ("solveOptProblemMsg",
      myMsgFac ().myFrag (mipMode ()? "mipFrag": "lpFrag"),
      "CPLEX");

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

   getRowData (rhs, sense);

   myOptProblem ()->getMatrixByCols (matbeg, matind, matval);

   getMatcnt (matcnt, matbeg);

   getColumnData (lb, ub, objective);

   enteringCplex ();

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

   leftCplex ();
   }

//------------------------------------------------------------------------------

void WitCplexIf::writeMpsSS ()
   {
   enteringCplex ();

   myErrCode_ = CPXwriteprob (myCpxEnv_, myCpxLp_, "opt-prob.mps", NULL);

   checkErrCode ("CPXwriteprob");

   leftCplex ();
   }

//------------------------------------------------------------------------------

void WitCplexIf::loadInitSolnSS (const double * initSoln)
   {
   enteringCplex ();

   myErrCode_ =
      CPXcopystart (
         myCpxEnv_,
         myCpxLp_,
         NULL,
         NULL,
         initSoln,
         NULL,
         NULL,
         NULL);

   checkErrCode ("CPXcopystart");

   leftCplex ();
   }

//------------------------------------------------------------------------------

void WitCplexIf::solveLp (bool optNeeded)
   {
   setIntParam (
      CPX_PARAM_LPMETHOD,
      useDualSimplex ()? CPX_ALG_DUAL: CPX_ALG_PRIMAL);

   setSpecCpxPars ();

   enteringCplex ();

   myErrCode_ = CPXlpopt (myCpxEnv_, myCpxLp_);

   checkErrCode ("CPXlpopt");

   leftCplex ();

   checkLpSolnStatus (optNeeded);

   printLpSolveInfo ();
   }

//------------------------------------------------------------------------------

void WitCplexIf::getRowData (
      WitVector <double> & rhs,
      WitVector <char> &   sense)
   {
   WitOptCon * theOptCon;
   int         theIdx;

   rhs  .resize (myOptProblem ()->nOptCons ());
   sense.resize (myOptProblem ()->nOptCons ());

   forEachEl (theOptCon, myOptProblem ()->myOptCons ())
      {
      theIdx = theOptCon->index ();

      getConData (rhs[theIdx], sense[theIdx], theOptCon);
      }
   }

//------------------------------------------------------------------------------

void WitCplexIf::getMatcnt (
            WitVector <int> & matcnt,
      const WitVector <int> & matbeg)
   {
   WitOptVar * theOptVar;
   int         theIdx;

   matcnt.resize (myOptProblem ()->nOptVars ());

   forEachEl (theOptVar, myOptProblem ()->myOptVars ())
      {
      theIdx         = theOptVar->index ();

      matcnt[theIdx] = matbeg[theIdx + 1] - matbeg[theIdx];
      }
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

   enteringCplex ();

   myErrCode_ =
      CPXchgbds (
         myCpxEnv_,
         myCpxLp_,
         cnt,
         indices.myCVec (),
         lu     .myCVec (),
         bd     .myCVec ());

   checkErrCode ("CPXchgbds");

   leftCplex ();

   theColIdx = -1;

   forEachEl (theOptVar, myOptProblem ()->myOptVars ())
      {
      theColIdx ++;

      lu[theColIdx] = 'U';
      bd[theColIdx] = theOptVar->bounds ().upper ();
      }

   enteringCplex ();

   myErrCode_ =
      CPXchgbds (
         myCpxEnv_,
         myCpxLp_,
         cnt,
         indices.myCVec (),
         lu     .myCVec (),
         bd     .myCVec ());

   checkErrCode ("myCpxEnv");

   leftCplex ();
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

   enteringCplex ();

   myErrCode_ =
      CPXchgrhs (
         myCpxEnv_,
         myCpxLp_,
         cnt,
         indices.myCVec (),
         values .myCVec ());

   checkErrCode ("CPXchgrhs");

   leftCplex ();
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

   enteringCplex ();

   myErrCode_ =
      CPXchgobj (
         myCpxEnv_,
         myCpxLp_,
         cnt,
         indices.myCVec (),
         values .myCVec ());

   checkErrCode ("CPXchgobj");

   leftCplex ();
   }

//------------------------------------------------------------------------------

void WitCplexIf::loadIntData ()
   {
   int              nIntVars;
   WitVector <int>  theIndices;
   WitVector <char> theTypeChars;
   int              theIdx;
   WitOptVar *      theOptVar;

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

   enteringCplex ();

   myErrCode_ =
      CPXchgctype (
         myCpxEnv_,
         myCpxLp_,
         nIntVars,
         theIndices  .myCVec (),
         theTypeChars.myCVec ());

   checkErrCode ("CPXchgctype");

   leftCplex ();
   }

//------------------------------------------------------------------------------

int WitCplexIf::countIntVars ()
   {
   int         nIntVars;
   WitOptVar * theOptVar;

   nIntVars = 0;

   forEachEl (theOptVar, myOptProblem ()->myOptVars ())
      {
      if (theOptVar->isAnIntVar ())
         nIntVars ++;
      }

   return nIntVars;
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
            {
            setUseDualSimplex (false);
            }
         else
            {
            setUseDualSimplex (myOptComp ()->crashOptStarter ()->isChosen ());
            }

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

   enteringCplex ();

   myErrCode_ =
      CPXchgobj (
         myCpxEnv_,
         myCpxLp_,
         1,
         indices,
         values);

   checkErrCode ("CPXchgobj");

   leftCplex ();
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

   enteringCplex ();

   myErrCode_ =
      CPXgetx (
         myCpxEnv_,
         myCpxLp_,
         primalSoln,
         theColIdx, theColIdx);

   checkErrCode ("CPXgetx");

   leftCplex ();

   optObjVal     = primalSoln[0];

   moTol         = myOptComp ()->myMultiObjMgr ()->multiObjTol ();

   theTol        = WitNonClass::max (moTol * fabs (optObjVal), moTol);

   indices[0]    = theColIdx;
   lu     [0]    = 'L';
   bd     [0]    = optObjVal - theTol;

   enteringCplex ();

   myErrCode_ =
      CPXchgbds (
         myCpxEnv_,
         myCpxLp_,
         1,
         indices,
         lu,
         bd);

   checkErrCode ("CPXchgbds");

   leftCplex ();
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

   enteringCplex ();

   myErrCode_ =
      CPXmipopt (myCpxEnv_, myCpxLp_);

   checkErrCode ("CPXmipopt");

   leftCplex ();

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

   enteringCplex ();

   myErrCode_ =
      CPXgetx (
         myCpxEnv_,
         myCpxLp_,
         primalSoln.myCVecForUpdate (),
         0,
         myOptProblem ()->nOptVars () - 1);

   checkErrCode ("CPXgetx");

   leftCplex ();

   forEachEl (theVar, myOptProblem ()->myOptVars ())
      {
      theIdx = theVar->index ();

      theVar->setPrimalValue (primalSoln[theIdx]);
      }
   }

//------------------------------------------------------------------------------

void WitCplexIf::storeDualSoln ()
   {
   WitVector <double> dualSoln;
   WitOptCon *        theCon;
   int                theIdx;

   dualSoln.resize (myOptProblem ()->nOptCons ());

   enteringCplex ();

   myErrCode_ =
      CPXgetpi (
         myCpxEnv_,
         myCpxLp_,
         dualSoln.myCVecForUpdate (),
         0,
         myOptProblem ()->nOptCons () - 1);

   checkErrCode ("CPXgetpi");

   leftCplex ();

   forEachEl (theCon, myOptProblem ()->myOptCons ())
      {
      theIdx = theCon->index ();

      theCon->setDualValue (dualSoln[theIdx]);
      }
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

   enteringCplex ();

   myErrCode_      =
      CPXgetparamnum (
         myCpxEnv_,
         theParamName.myCstring (),
       & theParamNum);

   if (myErrCode_ == CPXERR_BAD_PARAM_NAME)
      myMsgFac () ("badCpxParamNameSmsg", theCpxParSpec->myName ());

   checkErrCode ("getparamnum");

   leftCplex ();

   return theParamNum;
   }

//------------------------------------------------------------------------------

void WitCplexIf::setIntParam (int theCpxParam, int theValue)
   {
   myErrCode_ = CPXsetintparam (myCpxEnv_, theCpxParam, theValue);

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

   myErrCode_ = CPXgetbestobjval (myCpxEnv_, myCpxLp_, & theMipBound);

   checkErrCode ("CPXgetbestobjval");

   myErrCode_ = CPXgetmiprelgap (myCpxEnv_, myCpxLp_, & theMipRelGap);

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

void WitCplexIf::enteringCplex ()
   {
   WitTimer::enterSection ("cplex");
   }

//------------------------------------------------------------------------------

void WitCplexIf::leftCplex ()
   {
   WitTimer::leaveSection ("cplex");
   }

//------------------------------------------------------------------------------

#endif // CPLEX_EMBEDDED

//------------------------------------------------------------------------------
// Non-CPLEX-embedded implementation of class CplexIf.
//------------------------------------------------------------------------------

#ifndef CPLEX_EMBEDDED

//------------------------------------------------------------------------------

bool WitCplexIf::cplexEmbedded ()
   {
   return false;
   }

//------------------------------------------------------------------------------

WitCplexIf * WitCplexIf::newInstanceIfAllowed (WitOptProblem *)
   {
   stronglyAssert (false);

   return NULL;
   }

#endif // not CPLEX_EMBEDDED

