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

//------------------------------------------------------------------------------
// All #include statements for WIT header files must be located in the
// unconditional part of the source file, so that make depend will work
// correctly.
//------------------------------------------------------------------------------

#include <CplexIf.h>
#include <OptComp.h>
#include <OptProblem.h>
#include <CpxParSpecMgr.h>
#include <CpxParSpec.h>
#include <OptVar.h>
#include <OptCon.h>
#include <OptStarter.h>
#include <MsgFac.h>
#include <Timing.h>

//------------------------------------------------------------------------------
// CPLEX-embedded implementation of class CplexIf.
//------------------------------------------------------------------------------

#ifdef CPLEX_EMBEDDED

#include <cplex.h>
#include <float.h>

//------------------------------------------------------------------------------

bool WitCplexIf::cplexEmbedded ()
   {
   return true;
   }

//------------------------------------------------------------------------------

WitCplexIf * WitCplexIf::newInstance (WitOptProblem * theOptProblem)
   {
   return new WitCplexIf (theOptProblem);
   }

//------------------------------------------------------------------------------

WitCplexIf::~WitCplexIf ()
   {
   shutDownCplex ();
   }

//------------------------------------------------------------------------------

void WitCplexIf::issueVersionMsg ()
   {
   CPXCCHARptr theVersionID;

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

   myOptProblem ()->getColumnData (lb, ub, objective);

   enterCplex ();

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

   leaveCplex ();
   }

//------------------------------------------------------------------------------

void WitCplexIf::loadIntData ()
   {
   int              nIntVars;
   WitVector <int>  theIndices;
   WitVector <char> theTypeChars;
   int              theIdx;
   WitOptVar *      theOptVar;

   nIntVars = 0;

   forEachEl (theOptVar, myOptProblem ()->myOptVars ())
      if (theOptVar->isAnIntVar ())
         {
         nIntVars ++;
         }

   theIndices  .resize (nIntVars);
   theTypeChars.resize (nIntVars, 'I');

   theIdx = -1;

   forEachEl (theOptVar, myOptProblem ()->myOptVars ())
      if (theOptVar->isAnIntVar ())
         {
         theIdx ++;

         theIndices[theIdx] = theOptVar->index ();
         }

   enterCplex ();

   myErrCode_ =
      CPXchgctype (
         myCpxEnv_,
         myCpxLp_,
         nIntVars,
         theIndices  .myCVec (),
         theTypeChars.myCVec ());

   checkErrCode ("CPXchgctype");

   leaveCplex ();
   }

//------------------------------------------------------------------------------

void WitCplexIf::reviseLp ()
   {
   reviseColData ();
   reviseRHS     ();
   }

//------------------------------------------------------------------------------

void WitCplexIf::solverWriteMps ()
   {
   enterCplex ();

   myErrCode_ = CPXwriteprob (myCpxEnv_, myCpxLp_, "opt-prob.mps", NULL);

   checkErrCode ("CPXwriteprob");

   leaveCplex ();
   }

//------------------------------------------------------------------------------

void WitCplexIf::loadInitSoln (const WitVector <double> & initSoln)
   {
   enterCplex ();

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

   leaveCplex ();
   }

//------------------------------------------------------------------------------

void WitCplexIf::solveLp (bool reqPrimalSimplex)
   {
   setIntParam (
      CPX_PARAM_LPMETHOD,
      reqPrimalSimplex? CPX_ALG_PRIMAL: CPX_ALG_AUTOMATIC);

   setSpecCpxPars ();

   enterCplex ();

   myErrCode_ = CPXlpopt (myCpxEnv_, myCpxLp_);

   checkErrCode ("CPXlpopt");

   leaveCplex ();

   checkLpSolnStatus ();

   printLpSolveInfo ();
   }

//------------------------------------------------------------------------------

void WitCplexIf::reSolveLp ()
   {
   solveLp (false);
   }

//------------------------------------------------------------------------------

void WitCplexIf::reSolveLexLp ()
   {
   solveLp (true);
   }

//------------------------------------------------------------------------------

void WitCplexIf::solveMip ()
   {
   setSpecCpxPars ();

   enterCplex ();

   myErrCode_ =
      CPXmipopt (myCpxEnv_, myCpxLp_);

   checkErrCode ("CPXmipopt");

   leaveCplex ();

   checkMipSolnStatus ();

   storeObjBoundInfo ();

   printMipSolveInfo ();
   }

//------------------------------------------------------------------------------

void WitCplexIf::setVarLB (WitOptVar * theOptVar, double theLB)
   {
   int    indices[1];
   char   lu     [1];
   double bd     [1];

   indices[0] = theOptVar->index ();
   lu     [0] = 'L';
   bd     [0] = theLB;

   enterCplex ();

   myErrCode_ =
      CPXchgbds (
         myCpxEnv_,
         myCpxLp_,
         1,
         indices,
         lu,
         bd);

   checkErrCode ("CPXchgbds");

   leaveCplex ();
   }

//------------------------------------------------------------------------------

void WitCplexIf::setObjCoeff (WitOptVar * theOptVar, double theVal)
   {
   int    indices[1];
   double values [1];

   indices[0] = theOptVar->index ();
   values [0] = theVal;

   enterCplex ();

   myErrCode_ =
      CPXchgobj (
         myCpxEnv_,
         myCpxLp_,
         1,
         indices,
         values);

   checkErrCode ("CPXchgobj");

   leaveCplex ();
   }

//------------------------------------------------------------------------------

double WitCplexIf::primalVarVal (WitOptVar * theOptVar)
   {
   double primalSoln[1];

   enterCplex ();

   myErrCode_ =
      CPXgetx (
         myCpxEnv_,
         myCpxLp_,
         primalSoln,
         theOptVar->index (),
         theOptVar->index ());

   checkErrCode ("CPXgetx");

   leaveCplex ();

   return primalSoln[0];
   }

//------------------------------------------------------------------------------

void WitCplexIf::getPrimalSoln (WitVector <double> & primalSoln)
   {
   enterCplex ();

   myErrCode_ =
      CPXgetx (
         myCpxEnv_,
         myCpxLp_,
         primalSoln.myCVecForUpdate (),
         0,
         myOptProblem ()->nOptVars () - 1);

   checkErrCode ("CPXgetx");

   leaveCplex ();
   }

//------------------------------------------------------------------------------

void WitCplexIf::getDualSoln (WitVector <double> & dualSoln)
   {
   enterCplex ();

   myErrCode_ =
      CPXgetpi (
         myCpxEnv_,
         myCpxLp_,
         dualSoln.myCVecForUpdate (),
         0,
         myOptProblem ()->nOptCons () - 1);

   checkErrCode ("CPXgetpi");

   leaveCplex ();
   }

//------------------------------------------------------------------------------

const char * WitCplexIf::solverName ()
   {
   return "CPLEX";
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

void WitCplexIf::setUpCplex ()
   {
   int theErrCode;

   enterCplex ();

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

   leaveCplex ();
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
   enterCplex ();

   myErrCode_ = CPXfreeprob (myCpxEnv_, & myCpxLp_);

   checkErrCode ("CPXfreeprob");

   //...........................................................................

   shutDownLogFile ();

   //...........................................................................

   myErrCode_ = CPXcloseCPLEX (& myCpxEnv_);

   checkErrCode ("CPXcloseCPLEX");

   leaveCplex ();
   }

//------------------------------------------------------------------------------

void WitCplexIf::shutDownLogFile ()
   {
   myErrCode_  = CPXsetlogfilename (myCpxEnv_, NULL, "");

   checkErrCode ("CPXsetlogfilename");
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

void WitCplexIf::reviseColData ()
   {
   WitVector <int>    indices;
   WitVector <double> theLBVec;
   WitVector <double> theUBVec;
   WitVector <double> theObjVec;
   WitVector <char>   theLUVec;

   getColIndices (indices);

   myOptProblem ()->getColumnData (theLBVec, theUBVec, theObjVec);

   theLUVec.resize (myOptProblem ()->nOptVars ());

   theLUVec = 'L'; 

   enterCplex ();

   myErrCode_ =
      CPXchgbds (
         myCpxEnv_,
         myCpxLp_,
         myOptProblem ()->nOptVars (),
         indices .myCVec (),
         theLUVec.myCVec (),
         theLBVec.myCVec ());

   checkErrCode ("CPXchgbds");

   theLUVec = 'U'; 

   myErrCode_ =
      CPXchgbds (
         myCpxEnv_,
         myCpxLp_,
         myOptProblem ()->nOptVars (),
         indices .myCVec (),
         theLUVec.myCVec (),
         theUBVec.myCVec ());

   checkErrCode ("CPXchgbds");

   myErrCode_ =
      CPXchgobj (
         myCpxEnv_,
         myCpxLp_,
         myOptProblem ()->nOptVars (),
         indices  .myCVec (),
         theObjVec.myCVec ());

   checkErrCode ("CPXchgobj");

   leaveCplex ();
   }

//------------------------------------------------------------------------------

void WitCplexIf::getColIndices (WitVector <int> & indices)
   {
   WitOptVar * theOptVar;
   int         theColIdx;

   indices.resize (myOptProblem ()->nOptVars ());

   theColIdx = -1;

   forEachEl (theOptVar, myOptProblem ()->myOptVars ())
      {
      theColIdx ++;

      witAssert (theColIdx == theOptVar->index ());

      indices[theColIdx] = theColIdx;
      }
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

   enterCplex ();

   myErrCode_ =
      CPXchgrhs (
         myCpxEnv_,
         myCpxLp_,
         cnt,
         indices.myCVec (),
         values .myCVec ());

   checkErrCode ("CPXchgrhs");

   leaveCplex ();
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

void WitCplexIf::printLpSolveInfo ()
   {
   int nSimpItns;
   int nPh1Itns;

   nSimpItns = CPXgetitcnt     (myCpxEnv_, myCpxLp_);
   nPh1Itns  = CPXgetphase1cnt (myCpxEnv_, myCpxLp_);

   myMsgFac () ("nSimplexItersMsg", nSimpItns);

   myMsgFac () ("nPhase1ItersMsg",  nPh1Itns);
   }

//------------------------------------------------------------------------------

void WitCplexIf::checkLpSolnStatus ()
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
         issueStatusMsg ("nonOptLpCpxStatSmsg");

      default:
         issueStatusMsg ("unexpCpxStatSmsg");
      }
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

void WitCplexIf::checkMipSolnStatus ()
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
         issueStatusMsg ("nonOptMipCpxStatSmsg");

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

   enterCplex ();

   myErrCode_      =
      CPXgetparamnum (
         myCpxEnv_,
         theParamName.myCstring (),
       & theParamNum);

   if (myErrCode_ == CPXERR_BAD_PARAM_NAME)
      myMsgFac () ("badCpxParamNameSmsg", theCpxParSpec->myName ());

   checkErrCode ("getparamnum");

   leaveCplex ();

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

void WitCplexIf::enterCplex ()
   {
   WitTimer::enterSection ("cplex");
   }

//------------------------------------------------------------------------------

void WitCplexIf::leaveCplex ()
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

WitCplexIf * WitCplexIf::newInstance (WitOptProblem *)
   {
   stronglyAssert (false);

   return NULL;
   }

#endif // not CPLEX_EMBEDDED
