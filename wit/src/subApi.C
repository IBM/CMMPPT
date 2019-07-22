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
// Source file: subApi.C"
//
// Inner API functions relating to SubEntries.
//------------------------------------------------------------------------------

#include <WitRun.h>
#include <ApiMgr.h>
#include <InputID.h>
#include <ApiCall.h>
#include <ISRealArg.h>
#include <OSRealArg.h>
#include <IVRealArg.h>
#include <OVRealArg.h>
#include <Pre.h>
#include <DetOptImpMgr.h>
#include <ExtOptMgr.h>
#include <CompMgr.h>
#include <Operation.h>
#include <BomEntry.h>
#include <SubEntry.h>
#include <MsgFrag.h>
#include <MsgFac.h>

#include <stdlib.h>

//------------------------------------------------------------------------------
// setGetSubBomEntryInit
// Initilization of wit[S|G]etSubBomEntry* functions.
//------------------------------------------------------------------------------

WitSubEntry * WitRun::setGetSubBomEntryInit (
      const char * operationName,
      int          bomEntryIndex,
      int          subsBomEntryIndex)
   {
   forbidNullArg (operationName, "operationName");

   if (myMsgFac ().mayIssue ("operationNameMsg"))
      myMsgFac () ("operationNameMsg",      operationName );

   if (myMsgFac ().mayIssue ("bomEntryIndexMsg"))
      myMsgFac () ("bomEntryIndexMsg", bomEntryIndex );

   if (myMsgFac ().mayIssue ("subsBomEntryIndexMsg"))
      myMsgFac () ("subsBomEntryIndexMsg", subsBomEntryIndex );

   return getSubBomEntryP (operationName, bomEntryIndex, subsBomEntryIndex);
   }

//------------------------------------------------------------------------------

void WitRun::witAddSubsBomEntry (
      const char * operationName,
      int          bomIndex,
      const char * consumedPartName)
   {
   WitPart *      thePart;
   WitOperation * theOpn;
   WitBomEntry *  theBomEnt;

   myApiMgr_->startInnerFunc ();

   forbidNullArg (operationName,    "operationName");
   forbidNullArg (consumedPartName, "consumedPartName");

   if (myMsgFac ().mayIssue ("parmsAsbeMsg"))
      myMsgFac () ("parmsAsbeMsg",
         operationName,
         bomIndex,
         consumedPartName);

   theOpn = myCompMgr ()->findOperation (operationName);

   if (theOpn == NULL)
      myMsgFac () ("undefinedOperationMsg", operationName);

   myInputID ()->setMyOperation (theOpn);

   if (theOpn->bom ().isEmpty ())
      myMsgFac () ("emptyBom");

   if (bomIndex < 0 or bomIndex >= theOpn->bom ().nElements())
      myMsgFac () ("bomEntryIndexRangeSmsg",
         bomIndex,
         0,
         theOpn->bom ().nElements ());

   theBomEnt = myCompMgr ()->findBomEntry (theOpn, bomIndex);

   witAssert (theBomEnt != NULL);

   myInputID ()->setMyBomEntry (theBomEnt);

   thePart = myCompMgr ()->findPart (consumedPartName);

   if (thePart == NULL)
      myMsgFac () ("undefinedPartMsg", consumedPartName);

   addingDelComp ("subEntryFrag");

   new WitSubEntry (theBomEnt, thePart);

   myInputID ()->setMyOperation (NULL);
   myInputID ()->setMyBomEntry  (NULL);
   }

//------------------------------------------------------------------------------

void WitRun::witCopySubsBomEntryData (
      const char * dupOperationName,
      int          dupBomEntryIndex,
      int          dupSubsBomEntryIndex,
      WitRun *     origWitRun,
      const char * origOperationName,
      int          origBomEntryIndex,
      int          origSubsBomEntryIndex)
   {
   startCopyObjData (origWitRun);

   myMsgFac ()  ("dupObjectMsg", myMsgFac ().myFrag ("subEntryFrag"));

   WitSubEntryApiCall dupCall  (
      this,
      dupOperationName,
      dupBomEntryIndex,
      dupSubsBomEntryIndex);

   myMsgFac () ("origObjectMsg", myMsgFac ().myFrag ("subEntryFrag"));

   WitSubEntryApiCall origCall  (
      origWitRun,
      origOperationName,
      origBomEntryIndex,
      origSubsBomEntryIndex);

   if (dupCall.mySub () == origCall.mySub ())
      myMsgFac () ("copyObjectToSelfSmsg",
         myMsgFac ().myFrag ("subEntryFrag"));

   dupCall.mySub ()->copyAttrsFrom (origCall.mySub ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetObjItrSubsBomEntry (
      char * * consumingOperationName,
      int *    bomEntryIndex,
      int *    subsBomEntryIndex)
   {
   WitSubEntry * theSub;

   myApiMgr_->startInnerFunc ();

   theSub =
      curItrDelCompForGet (WitAT_SUB_ENTRY, "subEntryFrag")->
         thisSub ();

   stronglyAssert (theSub != NULL);

   apiGet (
      consumingOperationName, 
      "consumingOperationName", 
      theSub->myOperationName ());

   apiGet (bomEntryIndex,     "bomEntryIndex",     theSub->myBomEntIndex ());
   apiGet (subsBomEntryIndex, "subsBomEntryIndex", theSub->localIndex ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetSubsBomEntryAppData (
      const char * consumingOperationName,
      int          bomEntryIndex,
      int          subsBomEntryIndex,
      void * *     appData)
   {
   WitSubEntryApiCall theCall (
      this,
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex);

   apiGet (appData, "appData", theCall.mySub ()->appData ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetSubsBomEntryConsRate (
      const char *         consumingOperationName,
      int                  bomEntryIndex,
      int                  subsBomEntryIndex,
      const WitOVRealArg & consRate)
   {
   WitSubEntryApiCall theCall (
      this,
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex);

   apiGet (consRate, "consRate", theCall.mySub ()->consRate ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetSubsBomEntryConsumedPart (
      const char * consumingOperationName,
      int          bomEntryIndex,
      int          subsBomEntryIndex,
      char * *     consumedPartName )
   {
   WitSubEntryApiCall theCall (
      this,
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex);

   apiGet (
       consumedPartName, 
      "consumedPartName", 
      theCall.mySub ()->myPartName ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetSubsBomEntryEarliestPeriod (
     const char * consumingOperationName,
     int          bomEntryIndex,
     int          subsBomEntryIndex,
     int *        earliestPeriod)
   {
   WitSubEntryApiCall theCall (
      this,
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex);

   apiGet (             earliestPeriod, 
                       "earliestPeriod", 
      theCall.mySub ()->earliestPeriod ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetSubsBomEntryExecPenalty (
      const char *         consumingOperationName,
      int                  bomEntryIndex,
      int                  subsBomEntryIndex,
      const WitOSRealArg & execPenalty)
   {
   WitSubEntryApiCall theCall (
      this,
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex);

   apiGet (execPenalty, "execPenalty", theCall.mySub ()->execPenalty ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetSubsBomEntryExpAllowed (
      const char * consumingOperationName,
      int          bomEntryIndex,
      int          subsBomEntryIndex,
      WitBoolean * expAllowed)
   {
   WitSubEntryApiCall theCall (
      this,
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex);

   apiGet (expAllowed, "expAllowed", theCall.mySub ()->expAllowed ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetSubsBomEntryExpNetAversion (
      const char *         consumingOperationName,
      int                  bomEntryIndex,
      int                  subsBomEntryIndex,
      const WitOSRealArg & expNetAversion)
   {
   WitSubEntryApiCall theCall (
      this,
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex);

   apiGet (             expNetAversion, 
                       "expNetAversion", 
      theCall.mySub ()->expNetAversion ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetSubsBomEntryFallout (
      const char * consumingOperationName,
      int          bomEntryIndex,
      int          subsBomEntryIndex,
      int *        fallout)
   {
   double theFalloutRate;

   myApiMgr_->startInnerFunc ();

   myMsgFac () (
      "getPre50AttrMsg",
      apiFuncName (),
      "subEntry",
      "fallout",
      "falloutRate");

   witGetSubsBomEntryFalloutRate (
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex,
      & theFalloutRate);

   apiGet (fallout, "fallout", fracToPercent (theFalloutRate));
   }

//------------------------------------------------------------------------------

void WitRun::witGetSubsBomEntryFalloutRate (
      const char *         consumingOperationName,
      int                  bomEntryIndex,
      int                  subsBomEntryIndex,
      const WitOSRealArg & falloutRate)
   {
   WitSubEntryApiCall theCall (
      this,
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex);

   apiGet (falloutRate, "falloutRate", theCall.mySub ()->falloutRate ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetSubsBomEntryFssSubVol (
      const char *         consumingOperationName,
      int                  bomEntryIndex,
      int                  subsBomEntryIndex,
      const WitOVRealArg & fssSubVol)
   {
   WitSubEntryApiCall theCall (
      this,
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex);

   forbidStochMode ();

   myProblem ()->computeFssIfNeeded ();

   apiGet (fssSubVol, "fssSubVol", theCall.mySub ()->fssSubVol ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetSubsBomEntryImpactPeriod (
      const char * consumingOperationName,
      int          bomEntryIndex,
      int          subsBomEntryIndex,
      int **       impactPeriod)
   {
   WitSubEntryApiCall theCall (
      this,
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex);

   myProblem ()->myPreprocessor ()->preprocess ();

   apiGet (impactPeriod, "impactPeriod", theCall.mySub ()->impactPeriod ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetSubsBomEntryIntSubVols (
      const char * consumingOperationName,
      int          bomEntryIndex,
      int          subsBomEntryIndex,
      WitBoolean * intSubVols)
   {
   WitSubEntryApiCall theCall (
      this,
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex);

   apiGet (intSubVols, "intSubVols", theCall.mySub ()->intSubVols ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetSubsBomEntryLatestPeriod (
      const char * consumingOperationName,
      int          bomEntryIndex,
      int          subsBomEntryIndex,
      int *        latestPeriod)
   {
   WitSubEntryApiCall theCall (
      this,
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex);

   apiGet (latestPeriod, "latestPeriod", theCall.mySub ()->latestPeriod ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetSubsBomEntryMrpNetAllowed (
      const char * consumingOperationName,
      int          bomEntryIndex,
      int          subsBomEntryIndex,
      WitBoolean * mrpNetAllowed )
   {
   WitSubEntryApiCall theCall (
      this,
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex);

   apiGet (mrpNetAllowed, "mrpNetAllowed", theCall.mySub ()->mrpNetAllowed ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetSubsBomEntryMrpSubVol (
      const char *         consumingOperationName,
      int                  bomEntryIndex,
      int                  subsBomEntryIndex,
      const WitOVRealArg & mrpSubVol)
   {
   WitSubEntryApiCall theCall (
      this,
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex);

   apiGet (mrpSubVol, "mrpSubVol", theCall.mySub ()->mrpSubVol ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetSubsBomEntryNetAllowed (
      const char * consumingOperationName,
      int          bomEntryIndex,
      int          subsBomEntryIndex,
      WitBoolean * netAllowed )
   {
   WitSubEntryApiCall theCall (
      this,
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex);

   apiGet (netAllowed, "netAllowed", theCall.mySub ()->netAllowed ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetSubsBomEntryObj1SubCost (
     const char *         consumingOperationName,
     int                  bomEntryIndex,
     int                  subsBomEntryIndex,
     const WitOVRealArg & obj1SubCost)
   {
   myApiMgr_->startInnerFunc ();

   if (myMsgFac ().mayIssue ("getAttrNewNameMsg"))
      myMsgFac () (
         "getAttrNewNameMsg",
         apiFuncName (),
         "substitute BOM entry",
         "obj1SubCost",
         "subCost");

   witGetSubsBomEntrySubCost (
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex,
      obj1SubCost);
   }

//------------------------------------------------------------------------------

void WitRun::witGetSubsBomEntryOffset (
      const char *         consumingOperationName,
      int                  bomEntryIndex,
      int                  subsBomEntryIndex,
      const WitOVRealArg & offset)
   {
   WitSubEntryApiCall theCall (
      this,
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex);

   apiGet (offset, "offset", theCall.mySub ()->offset ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetSubsBomEntryProdVol (
      const char *         consumingOperationName,
      int                  bomEntryIndex,
      int                  subsBomEntryIndex,
      const WitOVRealArg & prodVol)
   {
   myApiMgr_->startInnerFunc ();

   prepGet34Attr ("SubsBomEntry", "prodVol", "SubsBomEntry", "subVol");

   witGetSubsBomEntrySubVol (
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex,
      prodVol);
   }

//------------------------------------------------------------------------------

void WitRun::witGetSubsBomEntryRoutingShare (
      const char *         consumingOperationName,
      int                  bomEntryIndex,
      int                  subsBomEntryIndex,
      const WitOVRealArg & routingShare)
   {
   WitSubEntryApiCall theCall (
      this,
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex);

   apiGet (routingShare, "routingShare", theCall.mySub ()->routingShare ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetSubsBomEntrySelForDel (
      const char * consumingOperationName,
      int          bomEntryIndex,
      int          subsBomEntryIndex,
      WitBoolean * selForDel)
   {
   WitSubEntryApiCall theCall (
      this,
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex);

   apiGet (selForDel, "selForDel", theCall.mySub ()->selForDel ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetSubsBomEntrySubCost (
     const char *         consumingOperationName,
     int                  bomEntryIndex,
     int                  subsBomEntryIndex,
     const WitOVRealArg & subCost)
   {
   WitSubEntryApiCall theCall (
      this,
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex);

   apiGet (             subCost, 
                       "subCost", 
      theCall.mySub ()->subCost ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetSubsBomEntrySubVarIndex (
      const char * consumingOperationName,
      int          bomEntryIndex,
      int          subsBomEntryIndex,
      const int    thePer,
      int *        subVarIndex)
   {
   WitSubEntryApiCall theCall (
      this,
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex);

   prepGetIndex (thePer);

   apiGet (
       subVarIndex,
      "subVarIndex",
      myDetOptImpMgr ()->
         myExtOptMgr ()->
            subVarIndex (theCall.mySub (), thePer));
   }

//------------------------------------------------------------------------------

void WitRun::witGetSubsBomEntrySubVol (
      const char *         consumingOperationName,
      int                  bomEntryIndex,
      int                  subsBomEntryIndex,
      const WitOVRealArg & subVol)
   {
   WitSubEntryApiCall theCall (
      this,
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex);

   apiGet (subVol, "subVol", theCall.mySub ()->subVol ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetSubsBomEntryUsageRate (
      const char *         consumingOperationName,
      int                  bomEntryIndex,
      int                  subsBomEntryIndex,
      const WitOSRealArg & usageRate)
   {
   double * consRateCVec;

   myApiMgr_->startInnerFunc ();

   if (myMsgFac ().mayIssue ("getVecAttrAsScalMsg"))
      myMsgFac () (
         "getVecAttrAsScalMsg",
         apiFuncName (),
         "substitute BOM entry",
         "usageRate",
         "consRate");

   witGetSubsBomEntryConsRate (
      consumingOperationName, 
      bomEntryIndex, 
      subsBomEntryIndex,
      & consRateCVec);

   forbidNullArg (usageRate, "usageRate");

   usageRate = consRateCVec[0];

   free (consRateCVec);
   }

//------------------------------------------------------------------------------

void WitRun::witSetSubsBomEntryAppData (
      const char * consumingOperationName,
      int          bomEntryIndex,
      int          subsBomEntryIndex,
      void *       appData)
   {
   WitSubEntryApiCall theCall (
      this,
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex);

   theCall.setAppData (appData);
   }

//------------------------------------------------------------------------------

void WitRun::witSetSubsBomEntryConsRate (
      const char *         consumingOperationName,
      int                  bomEntryIndex,
      int                  subsBomEntryIndex,
      const WitIVRealArg & consRate)
   {
   WitSubEntryApiCall theCall (
      this,
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex);

   theCall.prepDblVec ("consRate", consRate);

   if (theCall.argsOK ())
      theCall.mySub ()->setConsRate (consRate);
   }

//------------------------------------------------------------------------------

void WitRun::witSetSubsBomEntryEarliestPeriod (
      const char * consumingOperationName,
      int          bomEntryIndex,
      int          subsBomEntryIndex,
      int          earliestPeriod)
   {
   WitSubEntryApiCall theCall (
      this,
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex);

   theCall.setEarliestPeriod (earliestPeriod);
   }

//------------------------------------------------------------------------------

void WitRun::witSetSubsBomEntryExecPenalty (
      const char *         consumingOperationName,
      int                  bomEntryIndex,
      int                  subsBomEntryIndex,
      const WitISRealArg & execPenalty)
   {
   WitSubEntryApiCall theCall (
      this,
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex);

   theCall.prepNonNegDouble (
      "execPenalty", 
      theCall.mySub ()->execPenalty (),
      execPenalty);

   if (theCall.argsOK ())
      theCall.mySub ()->setExecPenalty (execPenalty);
   }

//------------------------------------------------------------------------------

void WitRun::witSetSubsBomEntryExpAllowed (
      const char * consumingOperationName,
      int          bomEntryIndex,
      int          subsBomEntryIndex,
      WitBoolean   expAllowed)
   {
   WitSubEntryApiCall theCall (
      this,
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex);

   theCall.prepBool (
                       "expAllowed", 
      theCall.mySub ()->expAllowed (), 
                        expAllowed);

   if (theCall.argsOK ())
      theCall.mySub ()->setExpAllowed (asaBool (expAllowed));
   }

//------------------------------------------------------------------------------

void WitRun::witSetSubsBomEntryExpNetAversion (
      const char *         consumingOperationName,
      int                  bomEntryIndex,
      int                  subsBomEntryIndex,
      const WitISRealArg & expNetAversion)
   {
   WitSubEntryApiCall theCall (
      this,
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex);

   theCall.prepDouble (
      "expNetAversion", 
      theCall.mySub ()->expNetAversion (), 
      expNetAversion);

   if (theCall.argsOK ())
      theCall.mySub ()->setExpNetAversion (expNetAversion);
   }

//------------------------------------------------------------------------------

void WitRun::witSetSubsBomEntryFallout (
      const char * consumingOperationName,
      int          bomEntryIndex,
      int          subsBomEntryIndex,
      int          fallout)
   {
   myApiMgr_->startInnerFunc ();

   myMsgFac () (
      "setPre50AttrMsg",
      apiFuncName (),
      "subEntry",
      "fallout",
      "falloutRate");

   witSetSubsBomEntryFalloutRate (
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex,
      percentToFrac (fallout));
   }

//------------------------------------------------------------------------------

void WitRun::witSetSubsBomEntryFalloutRate (
      const char *         consumingOperationName,
      int                  bomEntryIndex,
      int                  subsBomEntryIndex,
      const WitISRealArg & falloutRate)
   {
   WitSubEntryApiCall theCall (
      this,
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex);

   theCall.prepNonNegUBDouble (
      "falloutRate", 
      theCall.mySub ()->falloutRate (), 
      falloutRate,
      0.99 + FLOAT_EPSILON,
      0.99);

   if (theCall.argsOK ())
      theCall.mySub ()->setFalloutRate (falloutRate);
   }

//------------------------------------------------------------------------------

void WitRun::witSetSubsBomEntryIntSubVols (
      const char * consumingOperationName,
      int          bomEntryIndex,
      int          subsBomEntryIndex,
      WitBoolean   intSubVols)
   {
   WitSubEntryApiCall theCall (
      this,
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex);

   theCall.prepBool (
                       "intSubVols", 
      theCall.mySub ()->intSubVols (), 
                        intSubVols);

   if (theCall.argsOK ())
      theCall.mySub ()->setIntSubVols (asaBool (intSubVols));
   }

//------------------------------------------------------------------------------

void WitRun::witSetSubsBomEntryLatestPeriod (
      const char * consumingOperationName,
      int          bomEntryIndex,
      int          subsBomEntryIndex,
      int          latestPeriod)
   {
   WitSubEntryApiCall theCall (
      this,
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex);

   theCall.setLatestPeriod (latestPeriod);
   }

//------------------------------------------------------------------------------

void WitRun::witSetSubsBomEntryMrpNetAllowed (
      const char * consumingOperationName,
      int          bomEntryIndex,
      int          subsBomEntryIndex,
      WitBoolean   mrpNetAllowed)
   {
   WitSubEntryApiCall theCall (
      this,
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex);

   theCall.prepBool (
      "mrpNetAllowed", 
      theCall.mySub ()->mrpNetAllowed (), 
      mrpNetAllowed);

   if (theCall.argsOK ())
      theCall.mySub ()->setMrpNetAllowed (asaBool (mrpNetAllowed));
   }

//------------------------------------------------------------------------------

void WitRun::witSetSubsBomEntryNetAllowed (
      const char * consumingOperationName,
      int          bomEntryIndex,
      int          subsBomEntryIndex,
      WitBoolean   netAllowed)
   {
   WitSubEntryApiCall theCall (
      this,
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex);

   theCall.prepBool (
                       "netAllowed", 
      theCall.mySub ()->netAllowed (),
                        netAllowed);

   if (theCall.argsOK ())
      theCall.mySub ()->setNetAllowed (asaBool (netAllowed));
   }

//------------------------------------------------------------------------------

void WitRun::witSetSubsBomEntryObj1SubCost (
      const char *         consumingOperationName,
      int                  bomEntryIndex,
      int                  subsBomEntryIndex,
      const WitIVRealArg & obj1SubCost )
   {
   myApiMgr_->startInnerFunc ();

   if (myMsgFac ().mayIssue ("setAttrNewNameMsg"))
      myMsgFac () (
         "setAttrNewNameMsg",
         apiFuncName (),
         "substitute BOM entry",
         "obj1SubCost",
         "subCost");

      witSetSubsBomEntrySubCost (
         consumingOperationName,
         bomEntryIndex,
         subsBomEntryIndex,
         obj1SubCost);
   }

//------------------------------------------------------------------------------

void WitRun::witSetSubsBomEntryOffset (
      const char *         consumingOperationName,
      int                  bomEntryIndex,
      int                  subsBomEntryIndex,
      const WitIVRealArg & offset)
   {
   WitSubEntryApiCall theCall (
      this,
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex);

   theCall.setOffset (offset);
   }

//------------------------------------------------------------------------------

void WitRun::witSetSubsBomEntryProdVol (
      const char *         consumingOperationName,
      int                  bomEntryIndex,
      int                  subsBomEntryIndex,
      const WitIVRealArg & prodVol)
   {
   myApiMgr_->startInnerFunc ();

   prepSet34Attr ("SubsBomEntry", "prodVol", "SubsBomEntry", "subVol");

   witSetSubsBomEntrySubVol (
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex,
      prodVol);
   }

//------------------------------------------------------------------------------

void WitRun::witSetSubsBomEntryRoutingShare (
      const char *         consumingOperationName,
      int                  bomEntryIndex,
      int                  subsBomEntryIndex,
      const WitIVRealArg & routingShare)
   {
   WitSubEntryApiCall theCall (
      this,
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex);

   theCall.setRoutingShare (routingShare);
   }

//------------------------------------------------------------------------------

void WitRun::witSetSubsBomEntrySelForDel (
      const char * consumingOperationName,
      int          bomEntryIndex,
      int          subsBomEntryIndex,
      WitBoolean   selForDel)
   {
   WitSubEntryApiCall theCall (
      this,
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex);

   theCall.setSelForDel (selForDel);
   }

//------------------------------------------------------------------------------

void WitRun::witSetSubsBomEntrySubCost (
      const char *         consumingOperationName,
      int                  bomEntryIndex,
      int                  subsBomEntryIndex,
      const WitIVRealArg & subCost )
   {
   WitSubEntryApiCall theCall (
      this,
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex);

   WitPeriod thePer;

   theCall.prepDblVec ("subCost", subCost);

   if (theCall.argsOK ())
      {
      forEachPeriod (thePer, myProblem ())
         if (subCost[thePer] < 0.0)
            myMsgFac () (
               "negSubCostWmsg",
               "subCost",
               consumingOperationName,
               bomEntryIndex,
               subsBomEntryIndex,
               theCall.mySub ()->myPartName (),
               subCost[thePer],
               thePer);

      theCall.mySub ()->setSubCost (subCost);
      }
   }

//------------------------------------------------------------------------------

void WitRun::witSetSubsBomEntrySubVol (
      const char *         consumingOperationName,
      int                  bomEntryIndex,
      int                  subsBomEntryIndex,
      const WitIVRealArg & subVol)
   {
   WitSubEntryApiCall theCall (
      this,
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex);

   theCall.prepNonNegDblVec ("subVol", subVol);

   if (theCall.argsOK ())
      theCall.mySub ()->setSubVol (subVol);
   }

//------------------------------------------------------------------------------

void WitRun::witSetSubsBomEntryUsageRate (
      const char *         consumingOperationName,
      int                  bomEntryIndex,
      int                  subsBomEntryIndex,
      const WitISRealArg & usageRate)
   {
   WitTVec <double> & consRateVec = myApiMgr ()->reserveTempDblVec ();

   myApiMgr_->startInnerFunc ();

   if (myMsgFac ().mayIssue ("setVecAttrToScalMsg"))
      myMsgFac () (
         "setVecAttrToScalMsg",
         apiFuncName (),
         "substitute BOM entry",
         "usageRate",
         "consRate");

   consRateVec = usageRate;

   witSetSubsBomEntryConsRate (
      consumingOperationName,
      bomEntryIndex,
      subsBomEntryIndex,
      consRateVec.myCVec ());

   myApiMgr ()->releaseTempVecs ();
   }

//------------------------------------------------------------------------------
// getSubBomEntryP
// Get point to a substitute BOM entry
//------------------------------------------------------------------------------

WitSubEntry * WitRun::getSubBomEntryP (
      const char * operationName,
      int          bomEntryIndex,
      int          subsBomEntryIndex)
   {
   WitSubEntry * retVal;
   WitBomEntry * entryP;

   entryP = getBomEntryP (operationName, bomEntryIndex);

   if (entryP->mySubEntries ().isEmpty ())
      myMsgFac () ("bomEntryWoSubsSmsg");

   if (  subsBomEntryIndex < 0 or
         subsBomEntryIndex >= entryP->mySubEntries ().nElements ())
      myMsgFac () ("subsBomEntryIndexRangeSmsg",
         subsBomEntryIndex,
         0,
         entryP->mySubEntries ().nElements ());

   retVal = myCompMgr ()->findSubEntry (entryP, subsBomEntryIndex);

   witAssert (retVal != NULL);

   myInputID ()->setMySubEntry (retVal);

   return retVal;
   }
