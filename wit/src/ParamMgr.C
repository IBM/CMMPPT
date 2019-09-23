//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "ParamMgr.C"
//
// Contains the implementation of class ParamMgr.
//------------------------------------------------------------------------------

#include <ParamMgr.h>
#include <Param.h>
#include <SaeMgr.h>
#include <WitRun.h>
#include <GlobalComp.h>
#include <OptComp.h>
#include <OptStarter.h>
#include <StochImpMgr.h>
#include <CompMgr.h>
#include <MsgFac.h>

#include <stdlib.h>

//------------------------------------------------------------------------------
// Implementation of class ParamMgr.
//------------------------------------------------------------------------------

inline WitGlobalComp * WitParamMgr::myGlobalComp ()
   {
   return myProblem ()->myCompMgr ()->myGlobalComp ();
   }

//------------------------------------------------------------------------------

inline WitOptComp * WitParamMgr::myOptComp ()
   {
   return myProblem ()->myCompMgr ()->myOptComp ();
   }

//------------------------------------------------------------------------------

inline WitProblem * WitParamMgr::myProblem ()
   {
   return myWitRun_->mySession()->myProblem ();
   }

//------------------------------------------------------------------------------

WitParamMgr::WitParamMgr (WitRun * theWitRun, const char * theFileName):

      myWitRun_         (theWitRun),
      myMsgFac_         (theWitRun->myMsgFac ()),

      fileName_         (compParamIFName (theFileName)),

             allParams_ (),
       allNonDevParams_ (),
          allDevParams_ (),
       allStringParams_ (),
          allIntParams_ (),
       allDoubleParams_ (),
       allChoiceParams_ (),
      allBoolParams_    (),

      pCurrentParams_   (NULL)
   {
   }

//------------------------------------------------------------------------------

WitParamMgr::~WitParamMgr ()
   {
   while (not allParams_.isEmpty ())
      delete allParams_.get ();
   }

//------------------------------------------------------------------------------

void WitParamMgr::procParams ()
   {
   buildNonDevParams ();
   buildDevParams    ();

   readParams ();

   applyNonDevParams ();
   applyDevParams    ();

   printParams ();
   }

//------------------------------------------------------------------------------

void WitParamMgr::store (WitParam * theParam)
   {
   if (DEBUG_MODE)
      {
      WitParam * dupParam;

      dupParam = 
         allParams_.find (
            & WitParam::itsName,
              theParam->myName ().myCstring ());

      witAssert (dupParam == NULL);
      }
      //
      // Verify that the name is not duplicated.

   allParams_      .append (theParam);
   pCurrentParams_->append (theParam);
   }

//------------------------------------------------------------------------------

const WitString & WitParamMgr::stringParamVal (const char * paramName)
   {
   WitStringParam * theStringParam;

   theStringParam =
      allStringParams_.find (& WitStringParam::itsName, paramName);

   witAssert (theStringParam != NULL);

   return theStringParam->myValue ();
   }

//------------------------------------------------------------------------------

int WitParamMgr::intParamVal (const char * paramName)
   {
   WitIntParam * theIntParam;

   theIntParam = allIntParams_.find (& WitIntParam::itsName, paramName);

   witAssert (theIntParam != NULL);

   return theIntParam->myValue ();
   }

//------------------------------------------------------------------------------

double WitParamMgr::doubleParamVal (const char * paramName)
   {
   WitDoubleParam * theDoubleParam;

   theDoubleParam =
      allDoubleParams_.find (& WitDoubleParam::itsName, paramName);

   witAssert (theDoubleParam != NULL);

   return theDoubleParam->myValue ();
   }

//------------------------------------------------------------------------------

const WitString & WitParamMgr::choiceParamVal (const char * paramName)
   {
   WitChoiceParam * theChoiceParam;

   theChoiceParam =
      allChoiceParams_.find (& WitChoiceParam::itsName, paramName);

   witAssert (theChoiceParam != NULL);

   return theChoiceParam->myValue ();
   }

//------------------------------------------------------------------------------

bool WitParamMgr::boolParamVal (const char * paramName)
   {
   WitChoiceParam * theBoolParam;

   theBoolParam = allBoolParams_.find (& WitChoiceParam::itsName, paramName);

   witAssert (theBoolParam != NULL);

   return (theBoolParam->myValue () == "yes");
   }

//------------------------------------------------------------------------------

const char * WitParamMgr::compParamIFName (const char * theFileName)
   {
   return
      isNullCstring (theFileName)?
         "run.params":
         theFileName;
   }

//------------------------------------------------------------------------------

void WitParamMgr::buildNonDevParams ()
   {
   pCurrentParams_ = & allNonDevParams_;

   buildStringParam (
      "data_ifname",
      "Name of data input file",
      "wit.data");

   buildStringParam (
      "log_ofname",
      "Name of status log output file",
      "wit.log");

   buildStringParam (
      "echo_ofname",
      "Name of echo output file",
      "echo.out");

   buildStringParam (
      "pre_ofname",
      "Name of pre-processing output file",
      "pre.out");

   buildStringParam (
      "solver_ofname",
      "Name of solver log output file",
      myOptComp ()->solverLogFileName ().myCstring ());

   buildStringParam (
      "soln_ofname",
      "Name of comprehensive soln. output file",
      "soln.out");

   buildStringParam (
      "exec_ofname",
      "Name of execution schedule output file",
      "exec.out");

   buildStringParam (
      "ship_ofname",
      "Name of shipment schedule output file",
      "ship.out");

   buildStringParam (
      "mrpsup_ofname",
      "Name of MRP supply schedule output file",
      "mrpsup.out");

   buildStringParam (
      "critical_ofname",
      "Name of critical part sched output file",
      "critical.out");

   buildBoolParam (
      "print_echo",
      "Print echo of data?",
      false);

   buildBoolParam (
      "print_pre",
      "Print pre-processed data?",
      false);

   buildBoolParam (
      "print_global",
      "Print global data?",
      false);

   buildBoolParam (
      "print_exec",
      "Print execution schedule?",
      true);

   buildBoolParam (
      "print_ship",
      "Print shipment schedule?",
      true);

   buildBoolParam (
      "print_soln",
      "Print comprehensive implosion solution?",
      false);

   buildActionParam ();

   buildBoolParam (
      "auto_pri",
      "Automatically generate priorities?",
      myGlobalComp ()->autoPriority ());

   buildIntParam (
      "n_critical",
      "# of critical parts to list",
      -1, -1,
      0);

   buildIntParam (
      "equitability",
      "Equitable allocation precision",
      1, 100,
      myGlobalComp ()->equitability ());

   buildIntParam (
      "outputPrecision",
      "Precision of prod and ship sched files",
      0, -1,
      myGlobalComp ()->outputPrecision ());

   pCurrentParams_ = NULL;
   }

//------------------------------------------------------------------------------

void WitParamMgr::buildDevParams ()
   {
   int       theIdx;
   WitString tempParName;
   WitString tempParText;

   pCurrentParams_ = & allDevParams_;

   buildStringParam (
      "data_ifname2",
      "Name of 2nd input data file",
      "");

   buildBoolParam (
      "printOptProblem",
      "Print the optimization problem?",
      myOptComp ()->printOptProblem ());

   buildBoolParam (
      "printMatByCols",
      "Print matrix in column major format?",
      myOptComp ()->printMatByCols ());

   buildBoolParam (
      "printInitSoln",
      "Print initial optimization solution?",
      myOptComp ()->printInitSoln ());

   buildBoolParam (
      "printOptSoln",
      "Print final optimization solution?",
      myOptComp ()->printOptSoln ());

   buildBoolParam (
      "printMps",
      "Print an MPS file of the opt problem?",
      myOptComp ()->printMps ());

   buildBoolParam (
      "skipScreening",
      "Skip screening of the opt problem?",
      myOptComp ()->skipScreening ());

   buildOptInitMethodParam ();

   buildBoolParam (
      "writeData",
      "Write an input data file?",
      false);

   buildStringParam (
      "data_ofname",
      "Name of input data file to be written",
      "data.out");

   buildBoolParam (
      "writeSolnData",
      "Write a solution data file?",
      false);

   buildStringParam (
      "sdata_ofname",
      "Name of soln data file to be written",
      "soln.data");

   buildBoolParam (
      "readDataMsgs",
      "Issue API info msgs during readData?",
      myGlobalComp ()->readDataMsgs ());

   buildBoolParam (
      "printEqAlloc",
      "Print equitable allocation targets?",
      myGlobalComp ()->printEqAlloc ());

   buildIntParam (
      "selPrintLevel",
      "Print level for multi-select",
      0, 4,
      myGlobalComp ()->selPrintLevel ());

   buildBoolParam (
      "makeVolServ",
      "Make the obj func vol serv?",
      false);

   buildBoolParam (
      "compVolServ",
      "Compute volume serviceability?",
      false);

   buildBoolParam (
      "crashOnFatal",
      "Crash after issuing a fatal error msg?",
      false);

   buildBoolParam (
      "twmeByDemand",
      "Two-way multi-exec by demand?",
      myGlobalComp ()->twmeByDemand ());

   buildBoolParam (
      "prtIncAlloc",
      "Print incHeurAlloc data?",
      false);

   buildDoubleParam (
      "splitTol",
      "Selection splitting tolerance",
      1e-6,
      1.0);

   buildDoubleParam (
      "splitRes",
      "Selection splitting resolution",
      1e-6,
      1.0);

   buildIntParam (
      "splitItrUB",
      "Split iteration upper bound",
      1, -1,
      myGlobalComp ()->splitItrUB ());

   buildBoolParam (
      "invokePip",
      "Invoke Post-Implosion Pegging?",
      false);

   buildBoolParam (
      "inputPauses",
      "Pause for user input?",
      myGlobalComp ()->inputPauses ());

   buildBoolParam (
      "prtPipMaxRatio",
      "Print PIP max ratio data?",
      myGlobalComp ()->prtPipMaxRatio ());

   buildBoolParam (
      "printDebug",
      "Print debugging data?",
      myGlobalComp ()->printDebug ());

   buildBoolParam (
      "printExcessVol",
      "Print excessVol?",
      myGlobalComp ()->printExcessVol ());

   buildBoolParam (
      "newPipPggOrder",
      "Use new PIP pegging order?",
      myGlobalComp ()->newPipPggOrder ());

   buildBoolParam (
      "pipEnableAll",
      "Set all operations as PIP enabled?",
      false);

      // Build tempPars.
      //
   for (theIdx = 1; theIdx <= myGlobalComp ()->nTempPars (); theIdx ++)
      {
      getTempParName (theIdx, tempParName);
      getTempParText (theIdx, tempParText);

      buildStringParam (
         tempParName.myCstring (),
         tempParText.myCstring (),
         myGlobalComp ()->tempPar (theIdx).myCstring ());
      }

   pCurrentParams_ = NULL;
   }

//------------------------------------------------------------------------------

void WitParamMgr::buildActionParam ()
   {
   WitChoiceParam * const theParam =
      buildChoiceParam (
         "action",
         "Action to be performed");

   theParam->addChoice ("preproc");
   theParam->addChoice ("mrp");
   theParam->addChoice ("heur");
   theParam->addChoice ("opt");
   theParam->addChoice ("stoch");

   if (devMode ())
      theParam->addChoice ("eval");

   theParam->choose ("opt");
   }

//------------------------------------------------------------------------------

void WitParamMgr::buildOptInitMethodParam ()
   {
   WitChoiceParam * optInitParam;
   WitOptStarter *  theOptStarter;

   optInitParam =
      buildChoiceParam (
         "optInitMethod",
         "Override value for optInitMethod");

   optInitParam->addChoice ("none");

   forEachEl (theOptStarter, myOptComp ()->allOptStarters ())
      optInitParam->addChoice (theOptStarter->paramValue ().myCstring ());

   optInitParam->choose ("none");
   }

//------------------------------------------------------------------------------

void WitParamMgr::buildStringParam (
      const char * theName,
      const char * theText,
      const char * initValue)
   {
   allStringParams_.append (
      new WitStringParam (theName, theText, initValue, this));
   }

//------------------------------------------------------------------------------

void WitParamMgr::buildIntParam (
      const char * theName,
      const char * theText,
      int          theLB,
      int          theUB,
      int          initValue)
   {
   allIntParams_.append (
      new WitIntParam (theName, theText, theLB, theUB, initValue, this));
   }

//------------------------------------------------------------------------------

void WitParamMgr::buildDoubleParam (
      const char * theName,
      const char * theText,
      double       theLB,
      double       initValue)
   {
   allDoubleParams_.append (
      new WitDoubleParam (theName, theText, theLB, initValue, this));
   }

//------------------------------------------------------------------------------

WitChoiceParam * WitParamMgr::buildChoiceParam (
      const char * theName,
      const char * theText)
   {
   WitChoiceParam * theChoiceParam;

   theChoiceParam = new WitChoiceParam (theName, theText, this);

   allChoiceParams_.append (theChoiceParam);

   return theChoiceParam;
   }

//------------------------------------------------------------------------------

void WitParamMgr::buildBoolParam (
      const char * theName,
      const char * theText,
      bool         initValue)
   {
   WitChoiceParam * theBoolParam;

   theBoolParam = new WitChoiceParam (theName, theText, this);

   allBoolParams_.append (theBoolParam);

   theBoolParam->addChoice ("yes");
   theBoolParam->addChoice ("no");

   theBoolParam->choose (initValue? "yes": "no");
   }

//------------------------------------------------------------------------------

void WitParamMgr::readParams ()
   {
   FILE *     paramFile;
   int        fscanfRC;
   WitString  paramName  (80);
   WitString  valueToken (80);
   WitParam * theParam;

   WitList <WitParam> * pAllValidParams =
      devMode ()?
        &allParams_:
        &allNonDevParams_;

   myMsgFac () ("readParamsMsg");

   paramFile =
      myWitRun_->mySession ()->openFile (fileName ().myCstring (), "r");

   while (true)
      {
      fscanfRC = fscanf (paramFile, "%80s", paramName.myCstringForUpdate ());

      if (fscanfRC == EOF)
         break;

      if (fscanfRC == 0)
         myMsgFac () ("paramReadErrorReadProblemName", fileName ());

      theParam = 
         pAllValidParams->find (WitParam::itsName, paramName.myCstring ());

      if (theParam == NULL)
         myMsgFac () ("unrecParamSmsg", fileName (), paramName);

      fscanfRC = fscanf (paramFile, "%80s", valueToken.myCstringForUpdate ());

      if (fscanfRC == EOF)
         myMsgFac () ("paramReadErrorEofName", fileName (), paramName);

      if (fscanfRC == 0)
         myMsgFac () ("paramReadErrorReadProblemValue", fileName (), paramName);

      theParam->parse (valueToken);
      }

   fclose (paramFile);
   }

//------------------------------------------------------------------------------

void WitParamMgr::applyNonDevParams ()
   {
   myWitRun_->
      witSetMesgFileName (true, stringParamVal ("log_ofname").myCstring ());

   myWitRun_->mySession ()->printHeading ();

   myWitRun_->
      witSetSolverLogFileName (stringParamVal ("solver_ofname").myCstring ());

   myWitRun_->witSetAutoPriority (boolParamVal ("auto_pri"));

   myWitRun_->witSetComputeCriticalList (intParamVal ("n_critical") != 0);

   myWitRun_->witSetEquitability (intParamVal ("equitability"));

   myWitRun_->witSetOutputPrecision (intParamVal("outputPrecision"));
   }

//------------------------------------------------------------------------------

void WitParamMgr::applyDevParams ()
   {
   double    splitTol;
   double    splitRes;
   int       theIdx;
   WitString tempParName;

   myOptComp    ()->setPrintOptProblem (boolParamVal ("printOptProblem"));
   myOptComp    ()->setPrintMatByCols  (boolParamVal ("printMatByCols"));
   myOptComp    ()->setPrintInitSoln   (boolParamVal ("printInitSoln"));
   myOptComp    ()->setPrintOptSoln    (boolParamVal ("printOptSoln"));
   myOptComp    ()->setPrintMps        (boolParamVal ("printMps"));
   myOptComp    ()->setSkipScreening   (boolParamVal ("skipScreening"));

   myGlobalComp ()->setPrintIncAlloc   (boolParamVal ("prtIncAlloc"));
   myGlobalComp ()->setPrintEqAlloc    (boolParamVal ("printEqAlloc"));
   myGlobalComp ()->setSelPrintLevel   (intParamVal  ("selPrintLevel"));
   myGlobalComp ()->setTwmeByDemand    (boolParamVal ("twmeByDemand"));
   myGlobalComp ()->setSplitItrUB      (intParamVal  ("splitItrUB"));
   myGlobalComp ()->setInputPauses     (boolParamVal ("inputPauses"));
   myGlobalComp ()->setPrtPipMaxRatio  (boolParamVal ("prtPipMaxRatio"));
   myGlobalComp ()->setPrintDebug      (boolParamVal ("printDebug"));
   myGlobalComp ()->setPrintExcessVol  (boolParamVal ("printExcessVol"));
   myGlobalComp ()->setNewPipPggOrder  (boolParamVal ("newPipPggOrder"));

   splitTol = doubleParamVal ("splitTol");
   splitRes = doubleParamVal ("splitRes");

   if (splitTol < splitRes)
      myMsgFac () ("genericSmsg", "splitTol < splitRes");

   myGlobalComp ()->setSplitTol (splitTol);
   myGlobalComp ()->setSplitRes (splitRes);

   myMsgFac ().setCrashOnFatal (boolParamVal ("crashOnFatal"));

   for (theIdx = 1; theIdx <= myGlobalComp ()->nTempPars (); theIdx ++)
      {
      getTempParName (theIdx, tempParName);

      myGlobalComp ()->setTempPar (
         theIdx, 
         stringParamVal (tempParName.myCstring ()));
      }
   }

//------------------------------------------------------------------------------

void WitParamMgr::printParams ()
   {
   WitParam * theParam;

   myMsgFac () ("nonDevParamHeadingMsg", fileName ());

   forEachEl (theParam, allNonDevParams_)
      theParam->print ();

   if (devMode ())
      {
      myMsgFac () ("devParamHeadingMsg");

      forEachEl (theParam, allDevParams_)
         theParam->print ();
      }

   myMsgFac () ("blankMsg");
   }

//------------------------------------------------------------------------------

void WitParamMgr::getTempParName (int theIdx, WitString & theString)
   {
   theString.resize (100);

   sprintf (theString.myCstringForUpdate (), "tempPar%d", theIdx);
   }

//------------------------------------------------------------------------------

void WitParamMgr::getTempParText (int theIdx, WitString & theString)
   {
   theString.resize (100);

   sprintf (
      theString.myCstringForUpdate (),
      "Temporary development parameter #%d",
      theIdx);
   }
