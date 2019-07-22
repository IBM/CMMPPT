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
// Source file: "optApi.C"
//
// Inner API functions relating to global attributes of optimizing implosion.
//------------------------------------------------------------------------------

#include <WitRun.h>
#include <ApiMgr.h>
#include <ApiCall.h>
#include <OptComp.h>
#include <DetOptImpMgr.h>
#include <OptStarter.h>
#include <CplexMgrNC.h>
#include <CpxParSpecMgr.h>
#include <CpxParSpec.h>
#include <MultiObjMgr.h>
#include <Objective.h>
#include <ISRealArg.h>
#include <OSRealArg.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------

void WitRun::witAddDblCplexParSpec (
      const char *         theName,
      const WitISRealArg & theValue)
   {
   WitOptCompApiCall theCall (this);

   forbidNullArg (theName, "theName");

   if (equalCstrings (theName, "NO_PARAM"))
      myMsgFac () ("noParamCpxParamSpecSmsg");

   theCall.myCpxParSpecMgr ()->addDblCpxParSpec (theName, theValue);
   }

//------------------------------------------------------------------------------

void WitRun::witAddIntCplexParSpec (const char * theName, int theValue)
   {
   WitOptCompApiCall theCall (this);

   forbidNullArg (theName, "theName");

   if (equalCstrings (theName, "NO_PARAM"))
      myMsgFac () ("noParamCpxParamSpecSmsg");

   theCall.myCpxParSpecMgr ()->addIntCpxParSpec (theName, theValue);
   }

//------------------------------------------------------------------------------

void WitRun::witGetAccAfterOptImp (WitBoolean * accAfterOptImp)
   {
   WitOptCompApiCall theCall (this);

   apiGet (                 accAfterOptImp, 
                           "accAfterOptImp", 
      theCall.myOptComp ()->accAfterOptImp ());

   issueGetAttrMsg ("accAfterOptImp");
   }

//------------------------------------------------------------------------------

void WitRun::witGetAccAfterSoftLB (WitBoolean * accAfterSoftLB)
   {
   WitOptCompApiCall theCall (this);

   apiGet (                 accAfterSoftLB, 
                           "accAfterSoftLB", 
      theCall.myOptComp ()->accAfterSoftLB ());

   issueGetAttrMsg ("accAfterSoftLB");
   }

//------------------------------------------------------------------------------

void WitRun::witGetAccelerated (WitBoolean * accelerated)
   {
   WitOptCompApiCall theCall (this);

   apiGet (              accelerated,
                        "accelerated",
      myDetOptImpMgr ()->accelerated ());

   issueGetAttrMsg ("accelerated");
   }

//------------------------------------------------------------------------------

void WitRun::witGetBoundsValue (const WitOSRealArg & boundsValue)
   {
   WitOptCompApiCall theCall (this);

   apiGet (boundsValue, "boundsValue", theCall.myOptComp ()->boundsValue ());
   }

//------------------------------------------------------------------------------

void WitRun::witClearCplexParSpecs ()
   {
   WitOptCompApiCall theCall (this);

   theCall.myCpxParSpecMgr ()->clearCpxParSpecs ();
   }

//------------------------------------------------------------------------------

void WitRun::witGetCompPrices (WitBoolean * compPrices)
   {
   WitOptCompApiCall theCall (this);

   apiGet (compPrices, "compPrices", theCall.myOptComp ()->compPrices ());

   issueGetAttrMsg ("compPrices");
   }

//------------------------------------------------------------------------------

void WitRun::witGetCplexEmbedded (WitBoolean * cplexEmbedded)
   {
   WitOptCompApiCall theCall (this);

   apiGet (cplexEmbedded, "cplexEmbedded", WitNonClass::cplexEmbedded ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetCplexMipBound (const WitOSRealArg & cplexMipBound)
   {
   WitOptCompApiCall theCall (this);

   apiGet (                 cplexMipBound,
                           "cplexMipBound",
      theCall.myOptComp ()->cplexMipBound ());

   issueGetAttrMsg ("cplexMipBound");
   }

//------------------------------------------------------------------------------

void WitRun::witGetCplexMipRelGap (const WitOSRealArg & cplexMipRelGap)
   {
   WitOptCompApiCall theCall (this);

   apiGet (                 cplexMipRelGap,
                           "cplexMipRelGap",
      theCall.myOptComp ()->cplexMipRelGap ());

   issueGetAttrMsg ("cplexMipRelGap");
   }

//------------------------------------------------------------------------------

void WitRun::witGetCplexParSpecDblVal (const WitOSRealArg & cplexParSpecDblVal)
   {
   WitOptCompApiCall theCall (this);

   apiGet (                       cplexParSpecDblVal,
                                 "cplexParSpecDblVal",
      theCall.myCpxParSpecMgr ()->cplexParSpecDblVal ());

   issueGetAttrMsg ("cplexParSpecDblVal");
   }

//------------------------------------------------------------------------------

void WitRun::witGetCplexParSpecIntVal (int * cplexParSpecIntVal)
   {
   WitOptCompApiCall theCall (this);

   apiGet (
                                  cplexParSpecIntVal,
                                 "cplexParSpecIntVal",
      theCall.myCpxParSpecMgr ()->cplexParSpecIntVal ());

   issueGetAttrMsg ("cplexParSpecIntVal");
   }

//------------------------------------------------------------------------------

void WitRun::witGetCplexParSpecName (char * * cplexParSpecName)
   {
   WitOptCompApiCall theCall (this);

   apiGet (                       cplexParSpecName,
                                 "cplexParSpecName",
      theCall.myCpxParSpecMgr ()->cplexParSpecName ());

   issueGetAttrMsg ("cplexParSpecName");
   }

//------------------------------------------------------------------------------

void WitRun::witGetCplexStatusCode (int * cplexStatusCode)
   {
   WitOptCompApiCall theCall (this);

   apiGet (                 cplexStatusCode,
                           "cplexStatusCode",
      theCall.myOptComp ()->cplexStatusCode ());

   issueGetAttrMsg ("cplexStatusCode");
   }

//------------------------------------------------------------------------------

void WitRun::witGetCplexStatusText (char * * cplexStatusText)
   {
   WitOptCompApiCall theCall (this);

   apiGet (                 cplexStatusText, 
                           "cplexStatusText", 
      theCall.myOptComp ()->cplexStatusText ());

   issueGetAttrMsg ("cplexStatusText");
   }

//------------------------------------------------------------------------------

void WitRun::witGetCurrentObjective (char * * theObjName)
   {
   WitOptCompApiCall theCall (this);

   theCall.reqMultiObjMode ("currentObjective");

   apiGet (
              theObjName,
             "currentObjective",
      theCall.currentObjective ()->myObjName ());

   issueGetAttrMsg ("currentObjective");
   }

//------------------------------------------------------------------------------

void WitRun::witGetDblCplexParSpec (
      const char *         theName,
      WitBoolean *         dblSpecExists,
      const WitOSRealArg & theValue)
   {
   WitOptCompApiCall theCall (this);
   WitCpxParSpec *   theSpec;

   forbidNullArg (theName, "theName");

   * dblSpecExists = WitFALSE;

   theSpec         = theCall.myCpxParSpecMgr ()->findCpxParSpec (theName);

   if (theSpec != NULL)
      if (not theSpec->valTypeIsInt ())
         {
         * dblSpecExists = WitTRUE;

         theValue        = theSpec->myDblVal ();
         }
   }

//------------------------------------------------------------------------------

void WitRun::witGetExtOptActive (WitBoolean * extOptActive)
   {
   WitOptCompApiCall theCall (this);

   apiGet (              extOptActive,
                        "extOptActive",
      myDetOptImpMgr ()->extOptActive ());

   issueGetAttrMsg ("extOptActive");
   }

//------------------------------------------------------------------------------

void WitRun::witGetIntCplexParSpec (
      const char * theName,
      WitBoolean * intSpecExists,
      int *        theValue)
   {
   WitOptCompApiCall theCall (this);
   WitCpxParSpec *   theSpec;

   forbidNullArg (theName, "theName");

   * intSpecExists = WitFALSE;

   theSpec         = theCall.myCpxParSpecMgr ()->findCpxParSpec (theName);

   if (theSpec != NULL)
      if (theSpec->valTypeIsInt ())
         {
         * intSpecExists = WitTRUE;

         * theValue      = theSpec->myIntVal ();
         }
   }

//------------------------------------------------------------------------------

void WitRun::witGetMipMode (WitBoolean * mipMode)
   {
   WitOptCompApiCall theCall (this);

   apiGet (                 mipMode,
                           "mipMode",
      theCall.myOptComp ()->mipMode ());

   issueGetAttrMsg ("mipMode");
   }

//------------------------------------------------------------------------------

void WitRun::witGetMultiObjMode (WitBoolean * multiObjMode)
   {
   WitOptCompApiCall theCall (this);

   apiGet (                 multiObjMode,
                           "multiObjMode",
      theCall.myOptComp ()->multiObjMode ());

   issueGetAttrMsg ("multiObjMode");
   }

//------------------------------------------------------------------------------

void WitRun::witGetMultiObjTol (const WitOSRealArg & multiObjTol)
   {
   WitOptCompApiCall theCall (this);

   theCall.reqMultiObjMode ("multiObjTol");

   apiGet (                     multiObjTol,
                               "multiObjTol",
      theCall.myMultiObjMgr ()->multiObjTol ());

   issueGetAttrMsg ("multiObjTol");
   }

//------------------------------------------------------------------------------

void WitRun::witGetObjChoice (int * objChoice)
   {
   WitOptCompApiCall theCall (this);

   apiGet (                 objChoice,
                           "objChoice",
      theCall.myOptComp ()->objChoice ());

   issueGetAttrMsg ("objChoice");
   }

//------------------------------------------------------------------------------

void WitRun::witGetObjValue (const WitOSRealArg & objValue)
   {
   WitOptCompApiCall theCall (this);

   apiGet (objValue, "objValue", theCall.myOptComp ()->objValue ());

   issueGetAttrMsg ("objValue");
   }

//------------------------------------------------------------------------------

void WitRun::witGetObjValues (
      const WitOSRealArg & objValue,
      const WitOSRealArg & boundsValue)
   {
   WitOptCompApiCall theCall (this);

   apiGet (objValue,    "objValue",    theCall.myOptComp ()->objValue    ());
   apiGet (boundsValue, "boundsValue", theCall.myOptComp ()->boundsValue ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetObjectiveList (
      int  *     lenList,
      char * * * objectiveList)
   {
   WitOptCompApiCall theCall (this);
   int               theIdx;
   WitObjective *    theObj;

   theCall.reqMultiObjMode ("objectiveList");

   * lenList = theCall.myMultiObjMgr ()->myObjectives ().nElements ();

   apiAlloc (objectiveList, * lenList);

   theIdx = -1;

   forEachEl (theObj, theCall.myMultiObjMgr ()->myObjectives ())
      {
      theIdx ++;

      (* objectiveList)[theIdx] = theObj->myObjName ().apiDupCstring ();
      }
   }

//------------------------------------------------------------------------------

void WitRun::witGetObjectiveListSpec (char * * objectiveListSpec)
   {
   WitOptCompApiCall theCall (this);
   WitString         theObjListSpec;

   theCall.reqMultiObjMode ("objectiveListSpec");

   theCall.myMultiObjMgr ()->getObjectiveListSpec (theObjListSpec);

   apiGet (
       objectiveListSpec,
      "objectiveListSpec",
          theObjListSpec);

   issueGetAttrMsg ("objectiveListSpec");
   }

//------------------------------------------------------------------------------

void WitRun::witGetObjectiveSeqNo (int * objectiveSeqNo)
   {
   WitOptCompApiCall theCall (this);

   theCall.reqMultiObjMode ("objectiveSeqNo");

   apiGet (
                                   objectiveSeqNo,
                                  "objectiveSeqNo",
      theCall.currentObjective ()->objectiveSeqNo ());

   issueGetAttrMsg ("objectiveSeqNo");
   }

//------------------------------------------------------------------------------

void WitRun::witGetOptInitMethod (WitAttr * optInitMethod)
   {
   WitOptCompApiCall theCall (this);

   apiGet (
       optInitMethod,
      "optInitMethod",
      theCall.myOptComp ()->optInitMethod ()->myApiAttr ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetSolverLogFileName (char * * solverLogFileName)
   {
   WitOptCompApiCall theCall (this);

   apiGet (                 solverLogFileName, 
                           "solverLogFileName", 
      theCall.myOptComp ()->solverLogFileName ());

   issueGetAttrMsg ("solverLogFileName");
   }

//------------------------------------------------------------------------------

void WitRun::witGetWbounds (const WitOSRealArg & wbounds)
   {
   WitOptCompApiCall theCall (this);

   apiGet (wbounds, "wbounds", theCall.myOptComp ()->wbounds ());

   issueGetAttrMsg ("wbounds");
   }

//------------------------------------------------------------------------------

void WitRun::witSetAccAfterOptImp (WitBoolean newValue)
   {
   WitOptCompApiCall theCall (this);

   theCall.prepBool (
      "accAfterOptImp", 
      theCall.myOptComp ()->accAfterOptImp (), 
      newValue);

   if (theCall.argsOK ())
      theCall.myOptComp ()->setAccAfterOptImp (asaBool (newValue));
   }

//------------------------------------------------------------------------------

void WitRun::witSetAccAfterSoftLB (WitBoolean newValue)
   {
   WitOptCompApiCall theCall (this);

   theCall.prepBool (
      "accAfterSoftLB", 
      theCall.myOptComp ()->accAfterSoftLB (), 
      newValue);

   if (theCall.argsOK ())
      theCall.myOptComp ()->setAccAfterSoftLB (asaBool (newValue));
   }

//------------------------------------------------------------------------------

void WitRun::witSetCompPrices (WitBoolean newValue)
   {
   WitOptCompApiCall theCall (this);

   theCall.prepBool (
      "compPrices", 
      theCall.myOptComp ()->compPrices (), 
      newValue);

   if (theCall.argsOK ())
      theCall.myOptComp ()->setCompPrices (asaBool (newValue));
   }

//------------------------------------------------------------------------------

void WitRun::witSetCplexParSpecDblVal (const WitISRealArg & cplexParSpecDblVal)
   {
   WitOptCompApiCall theCall (this);

   theCall.prepDouble (
                                 "cplexParSpecDblVal", 
      theCall.myCpxParSpecMgr ()->cplexParSpecDblVal (),
                                  cplexParSpecDblVal);

   if (theCall.argsOK ())
      theCall.myCpxParSpecMgr ()->setCplexParSpecDblVal (cplexParSpecDblVal);
   }

//------------------------------------------------------------------------------

void WitRun::witSetCplexParSpecIntVal (int cplexParSpecIntVal)
   {
   WitOptCompApiCall theCall (this);

   theCall.prepInt (
                                 "cplexParSpecIntVal",
      theCall.myCpxParSpecMgr ()->cplexParSpecIntVal (),
                                  cplexParSpecIntVal);

   if (theCall.argsOK ())
      theCall.myCpxParSpecMgr ()->setCplexParSpecIntVal (cplexParSpecIntVal);
   }

//------------------------------------------------------------------------------

void WitRun::witSetCplexParSpecName (const char * cplexParSpecName)
   {
   WitOptCompApiCall theCall (this);

   theCall.prepString (
      "cplexParSpecName", 
      theCall.myCpxParSpecMgr ()->cplexParSpecName (), 
      cplexParSpecName);

   if (theCall.argsOK ())
      theCall.myCpxParSpecMgr ()->setCplexParSpecName (cplexParSpecName);
   }

//------------------------------------------------------------------------------

void WitRun::witSetCurrentObjective (const char * theObjName)
   {
   WitOptCompApiCall theCall (this);
   WitObjective *    theObj;

   theCall.reqMultiObjMode ("currentObjective");

   theCall.prepString (
      "currentObjective", 
      theCall.currentObjective ()->myObjName (), 
      theObjName);

   theObj = theCall.myMultiObjMgr ()->findObjective (theObjName);

   if (theObj == NULL)
      myMsgFac () ("setCurObjToNonMatchNameSmsg", theObjName);

   if (theCall.argsOK ())
      theCall.myMultiObjMgr ()->setCurrentObjective (theObj);
   }

//------------------------------------------------------------------------------

void WitRun::witSetMipMode (WitBoolean newValue)
   {
   WitOptCompApiCall theCall (this);

   theCall.prepBool (
      "mipMode", 
      theCall.myOptComp ()->mipMode (), 
      newValue);

   if (theCall.argsOK ())
      theCall.myOptComp ()->setMipMode (asaBool (newValue));
   }

//------------------------------------------------------------------------------

void WitRun::witSetMultiObjMode (WitBoolean newValue)
   {
   WitOptCompApiCall theCall (this);

   theCall.reqNoPartsOrOpns ("multiObjMode");

   theCall.prepBool (
      "multiObjMode",
      theCall.myOptComp ()->multiObjMode (),
      newValue);

   if (theCall.argsOK ())
      theCall.myOptComp ()->setMultiObjMode (asaBool (newValue));
   }

//------------------------------------------------------------------------------

void WitRun::witSetMultiObjTol (const WitISRealArg & theValue)
   {
   WitOptCompApiCall theCall (this);

   theCall.reqMultiObjMode ("multiObjTol");

   theCall.prepNonNegUBDouble (
      "multiObjTol", 
      theCall.myMultiObjMgr ()->multiObjTol (),
      theValue,
      .001001,
      .001);

   if (theCall.argsOK ())
      theCall.myMultiObjMgr ()->setMultiObjTol (theValue);
   }

//------------------------------------------------------------------------------

void WitRun::witSetObjChoice (int objChoice)
   {
   WitOptCompApiCall theCall (this);

   theCall.prepRangedInt (
      "objChoice",
      theCall.myOptComp ()->objChoice (),
      objChoice,
      0,
      1);

   if (theCall.argsOK ())
      theCall.myOptComp ()->setObjChoice (objChoice);
   }

//------------------------------------------------------------------------------

void WitRun::witSetObjectiveList (
      int                  lenList,
      const char * const * objectiveList)
   { 
   WitOptCompApiCall        theCall (this);
   WitVector <const char *> theObjNames;
   int                      theIdx;

   theCall.reqMultiObjMode ("objectiveList");

   theCall.reqNoPartsOrOpns ("objectiveList");

   if (lenList < 1)
      myMsgFac () ("intArgLowSmsg", "lenList", lenList, 1);

   forbidNullArg (objectiveList, "objectiveList");

   theObjNames.resize (lenList, NULL);

   theObjNames = objectiveList;

   for (theIdx = 0; theIdx < lenList; theIdx ++)
      {
      if (theObjNames[theIdx] == NULL)
         myMsgFac () ("nullVecElemSmsg", "objectiveList", theIdx);

      if (WitObjective::containsVBar      (theObjNames[theIdx]))
         myMsgFac () ("vbarInObjNameSmsg", theObjNames[theIdx]);
      }

   checkForDupObjNames (theObjNames);

   theCall.myMultiObjMgr ()->setObjectiveList (theObjNames);
   }

//------------------------------------------------------------------------------

void WitRun::witSetObjectiveListSpec (const char * objectiveListSpec)
   {
   WitOptCompApiCall        theCall (this);
   char *                   theObjName;
   WitList <char>           theObjNameList;
   WitVector <const char *> theObjNameVec;
   int                      theIdx;
   WitString                oldSpec;
   

   theCall.reqMultiObjMode  ("objectiveListSpec");
   theCall.reqNoPartsOrOpns ("objectiveListSpec");

   forbidNullArg (objectiveListSpec, "objectiveListSpec");

   parseObjectiveListSpec (objectiveListSpec, theObjNameList);

   theObjNameVec.resize (theObjNameList.nElements (), NULL);

   theIdx = -1;

   forEachEl (theObjName, theObjNameList)
      {
      theIdx ++;

      theObjNameVec[theIdx] = theObjName;
      }

   checkForDupObjNames (theObjNameVec);

   theCall.myMultiObjMgr ()->getObjectiveListSpec (oldSpec);

   if (myMsgFac ().mayIssue ("setStringAttrMsg"))
      myMsgFac () ("setStringAttrMsg",
         "objectiveListSpec",
         oldSpec,
         objectiveListSpec);

   theCall.myMultiObjMgr ()->setObjectiveList (theObjNameVec);

   theObjNameVec.clear ();

   while (not theObjNameList.isEmpty ())
      {
      delete[] theObjNameList.get ();
      }
   }

//------------------------------------------------------------------------------

void WitRun::witSetObjectiveSeqNo (int objectiveSeqNo)
   {
   WitOptCompApiCall theCall (this);

   theCall.reqMultiObjMode ("objectiveSeqNo");

   theCall.prepRangedInt (
                                  "objectiveSeqNo",
      theCall.currentObjective ()->objectiveSeqNo (),
                                   objectiveSeqNo,
      1,
      theCall.myMultiObjMgr ()->myObjectives ().nElements ());

   if (theCall.argsOK ())
      theCall.currentObjective ()->setObjectiveSeqNo (objectiveSeqNo);
   }

//------------------------------------------------------------------------------

void WitRun::witSetOptInitMethod (WitAttr optInitMethod)
   {
   WitOptCompApiCall theCall (this);

   WitOptStarter *   theOptStarter;
   WitOptStarter *   currentOptStarter;

   theOptStarter = NULL;

      // Find the OptStarter that matches the Attr.
      //
   forEachEl (currentOptStarter, theCall.myOptComp ()->allOptStarters ())
      if (currentOptStarter->myApiAttr () == optInitMethod)
         theOptStarter = currentOptStarter;

   if (theOptStarter == NULL)
      myMsgFac () ("unknownOptStarterSmsg", optInitMethod);

   if (theOptStarter == theCall.myOptComp ()->accOptStarter ())
      myMsgFac () ("setAccInitSmsg");

   myMsgFac () ("setWitAttrAttrMsg",
      "optInitMethod",
      theCall.myOptComp ()->optInitMethod ()->myApiAttr (),
      optInitMethod);

   myDetOptImpMgr ()->shutDown ();
   
   theOptStarter->beChosen ();
   }

//------------------------------------------------------------------------------

void WitRun::witSetSolverLogFileName (const char * solverLogFileName)
   {
   WitOptCompApiCall theCall (this);

   theCall.prepString (
      "solverLogFileName", 
      theCall.myOptComp ()->solverLogFileName (), 
      solverLogFileName);

   if (theCall.argsOK ())
      theCall.myOptComp ()->setSolverLogFileName (solverLogFileName);
   }

//------------------------------------------------------------------------------

void WitRun::witSetWbounds (const WitISRealArg & theVal)
   {
   WitOptCompApiCall theCall (this);

   theCall.prepNonNegDouble (
      "wbounds", 
      theCall.myOptComp ()->wbounds (), 
      theVal);

   if (theCall.argsOK ())
      theCall.myOptComp ()->setWbounds (theVal);
   }

//------------------------------------------------------------------------------

void WitRun::parseObjectiveListSpec (
      const char *     objectiveListSpec,
      WitList <char> & theObjNameList)
   {
   const char * curPosition;
   int          theNameLength;
   char *       theObjName;

   theObjNameList.clear ();

   curPosition = objectiveListSpec;

   while (true)
      {
      theNameLength = strcspn (curPosition, "|");

      theObjName    = new char[theNameLength + 1];

      strncpy (theObjName, curPosition, theNameLength);

      theObjName[theNameLength] = '\0';

      theObjNameList.append (theObjName);

      curPosition += theNameLength;

      if (* curPosition == '\0')
         return;

      stronglyAssert (* curPosition == '|');

      curPosition ++;
      }
   }

//------------------------------------------------------------------------------

void WitRun::checkForDupObjNames (const WitVector <const char *> & theObjNames)
   {
   int theIdx;
   int priorIdx;

   for    (theIdx   = 0; theIdx   < theObjNames.length (); theIdx   ++)
      for (priorIdx = 0; priorIdx < theIdx;                priorIdx ++)
         {
         if (equalCstrings (theObjNames[theIdx], theObjNames[priorIdx]))
            {
            myMsgFac () ("dupObjNameSmsg", theObjNames[theIdx]);
            }
      }
   }
