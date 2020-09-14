//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "bomApi.C"
//
// Inner API functions relating to BomEntries.
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
#include <GlobalComp.h>
#include <MsgFrag.h>
#include <MsgFac.h>

#include <stdlib.h>

//------------------------------------------------------------------------------
// setBomEntryInit
// Initilization of witSetBomEntry* functions.
//------------------------------------------------------------------------------

WitBomEntry * WitRun::setGetBomEntryInit (
      const char * operationName,
      int          bomEntryIndex)
   {
   forbidNullArg (operationName, "operationName");

   if (myMsgFac ().mayIssue ("operationNameMsg"))
      myMsgFac () ("operationNameMsg", operationName );

   if (myMsgFac ().mayIssue ("bomEntryIndexMsg"))
      myMsgFac () ("bomEntryIndexMsg", bomEntryIndex);

   return getBomEntryP (operationName, bomEntryIndex);
   }

//------------------------------------------------------------------------------

void WitRun::witAddBomEntry (
      const char * consumingOperationName,
      const char * consumedPartName)
   {
   WitOperation * theOpn;
   WitPart *      thePart;

   myApiMgr_->startInnerFunc ();

   forbidNullArg (consumingOperationName, "consumingOperationName");
   forbidNullArg (consumedPartName,       "consumedPartName");

   if (myMsgFac ().mayIssue ("addBomEntryMsg"))
      myMsgFac () ("addBomEntryMsg",
         consumingOperationName,
         consumedPartName);

   theOpn = myCompMgr ()->findOperation (consumingOperationName);

   if (theOpn == NULL)
      myMsgFac () ("undefinedOperationMsg", consumingOperationName);

   myInputID ()->setMyOperation (theOpn);

   thePart = myCompMgr ()->findPart (consumedPartName);

   if (thePart == NULL)
      myMsgFac () ("undefinedPartMsg", consumedPartName);

   addingDelComp ("bomEntryFrag");

   new WitBomEntry (theOpn, thePart);

   myInputID ()->setMyOperation (NULL);
   }

//------------------------------------------------------------------------------

void WitRun::witCopyBomEntryData (
      const char * dupOperationName,
      int          dupBomEntryIndex,
      WitRun *     origWitRun,
      const char * origOperationName,
      int          origBomEntryIndex)
   {
   startCopyObjData (origWitRun);

   myMsgFac ()  ("dupObjectMsg", myMsgFac ().myFrag ("bomEntryFrag"));

   WitBomEntryApiCall dupCall  (
      this,
      dupOperationName,
      dupBomEntryIndex);

   myMsgFac () ("origObjectMsg", myMsgFac ().myFrag ("bomEntryFrag"));

   WitBomEntryApiCall origCall  (
      origWitRun,
      origOperationName,
      origBomEntryIndex);

   if (dupCall.myBomEnt () == origCall.myBomEnt ())
      myMsgFac () ("copyObjectToSelfSmsg",
         myMsgFac ().myFrag ("bomEntryFrag"));

   dupCall.myBomEnt ()->copyAttrsFrom (origCall.myBomEnt ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetObjItrBomEntry (
      char * * consumingOperationName,
      int *    bomEntryIndex)
   {
   WitBomEntry * theBomEnt;

   myApiMgr_->startInnerFunc ();

   theBomEnt =
      curItrDelCompForGet (WitAT_BOM_ENTRY, "bomEntryFrag")->
         thisBomEnt ();

   stronglyAssert (theBomEnt != NULL);

   apiGet (
      consumingOperationName, 
      "consumingOperationName", 
      theBomEnt->myOperationName ());

   apiGet (bomEntryIndex, "bomEntryIndex", theBomEnt->localIndex ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetBomEntryAppData (
      const char * consumingOperationName,
      int          bomEntryIndex,
      void * *     appData)
   {
   WitBomEntryApiCall theCall (this, consumingOperationName, bomEntryIndex);

   apiGet (appData, "appData", theCall.myBomEnt ()->appData ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetBomEntryConsRate (
      const char *         consumingOperationName,
      int                  bomEntryIndex,
      const WitOVRealArg & consRate)
   {
   WitBomEntryApiCall theCall (this, consumingOperationName, bomEntryIndex);

   apiGet (consRate, "consRate", theCall.myBomEnt ()->consRate ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetBomEntryConsumedPart(
      const char * consumingOperationName,
      int          bomEntryIndex,
      char * *     consumedPartName)
   {
   WitBomEntryApiCall theCall (this, consumingOperationName, bomEntryIndex);

   apiGet (
       consumedPartName, 
      "consumedPartName", 
      theCall.myBomEnt ()->myPartName ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetBomEntryEarliestPeriod (
      const char * consumingOperationName,
      int          bomEntryIndex,
      int *        earliestPeriod)
   {
   WitBomEntryApiCall theCall (this, consumingOperationName, bomEntryIndex);

   apiGet (                earliestPeriod, 
                          "earliestPeriod", 
      theCall.myBomEnt ()->earliestPeriod ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetBomEntryExecPenalty (
      const char *         consumingOperationName,
      int                  bomEntryIndex,
      const WitOSRealArg & execPenalty)
   {
   WitBomEntryApiCall theCall (this, consumingOperationName, bomEntryIndex);

   apiGet (execPenalty, "execPenalty", theCall.myBomEnt ()->execPenalty ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetBomEntryFallout (
      const char * consumingOperationName,
      int          bomEntryIndex,
      int *        fallout )
   {
   double theFalloutRate;

   myApiMgr_->startInnerFunc ();

   myMsgFac () (
      "getPre50AttrMsg",
      apiFuncName (),
      "bomEntry",
      "fallout",
      "falloutRate");

   witGetBomEntryFalloutRate (
      consumingOperationName,
      bomEntryIndex,
      & theFalloutRate);

   apiGet (fallout, "fallout", fracToPercent (theFalloutRate));
   }

//------------------------------------------------------------------------------

void WitRun::witGetBomEntryFalloutRate (
      const char *         consumingOperationName,
      int                  bomEntryIndex,
      const WitOSRealArg & falloutRate)
   {
   WitBomEntryApiCall theCall (this, consumingOperationName, bomEntryIndex);

   apiGet (falloutRate, "falloutRate", theCall.myBomEnt ()->falloutRate ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetBomEntryImpactPeriod (
      const char * consumingOperationName,
      int          bomEntryIndex,
      int **       impactPeriod)
   {
   WitBomEntryApiCall theCall (this, consumingOperationName, bomEntryIndex);

   myProblem ()->myPreprocessor ()->preprocess ();

   apiGet (impactPeriod, "impactPeriod", theCall.myBomEnt ()->impactPeriod ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetBomEntryLatestPeriod (
      const char * consumingOperationName,
      int          bomEntryIndex,
      int *        latestPeriod)
   {
   WitBomEntryApiCall theCall (this, consumingOperationName, bomEntryIndex);

   apiGet (latestPeriod, "latestPeriod", theCall.myBomEnt ()->latestPeriod ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetBomEntryMandEC (
      const char * consumingOperationName,
      int          bomEntryIndex,
      WitBoolean * mandEC)
   {
   WitBomEntryApiCall theCall (this, consumingOperationName, bomEntryIndex);

   apiGet (mandEC, "mandEC", theCall.myBomEnt ()->mandEC ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetBomEntryNSubsBomEntries (
      const char * consumingOperationName,
      int          bomEntryIndex,
      int *        nSubsBomEntries)
   {
   WitBomEntryApiCall theCall (this, consumingOperationName, bomEntryIndex);

   apiGet (
       nSubsBomEntries,
      "nSubsBomEntries",
      theCall.myBomEnt ()->mySubEntries ().nElements ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetBomEntryNonSubVarIndex (
      const char * consumingOperationName,
      int          bomEntryIndex,
      const int    thePer,
      int *        nonSubVarIndex)
   {
   WitBomEntryApiCall theCall (this, consumingOperationName, bomEntryIndex);

   prepGetIndex (thePer);

   apiGet (
       nonSubVarIndex,
      "nonSubVarIndex",
      myDetOptImpMgr ()->
         myExtOptMgr ()->
            nonSubVarIndex (theCall.myBomEnt (), thePer));
   }

//------------------------------------------------------------------------------

void WitRun::witGetBomEntryOffset (
      const char *         consumingOperationName,
      int                  bomEntryIndex,
      const WitOVRealArg & offset)
   {
   WitBomEntryApiCall theCall (this, consumingOperationName, bomEntryIndex);

   apiGet (offset, "offset", theCall.myBomEnt ()->offset ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetBomEntryPropRouting (
      const char * consumingOperationName,
      int          bomEntryIndex,
      WitBoolean * propRouting)
   {
   WitBomEntryApiCall theCall (this, consumingOperationName, bomEntryIndex);

   if (myMsgFac ().mayIssue ("getVecAttrAsScalMsg"))
      myMsgFac () (
         "getVecAttrAsScalMsg",
         apiFuncName (),
         "BOM entry",
         "propRouting",
         "propRtg");

   apiGet (propRouting, "propRouting", theCall.myBomEnt ()->propRtg ()[0]);
   }

//------------------------------------------------------------------------------

void WitRun::witGetBomEntryPropRtg (
      const char *   consumingOperationName,
      int            bomEntryIndex,
      WitBoolean * * propRtg)
   {
   WitBomEntryApiCall theCall (this, consumingOperationName, bomEntryIndex);

   apiGet (propRtg, "propRtg", theCall.myBomEnt ()->propRtg ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetBomEntryRoutingShare (
      const char *         consumingOperationName,
      int                  bomEntryIndex,
      const WitOVRealArg & routingShare)
   {
   WitBomEntryApiCall theCall (this, consumingOperationName, bomEntryIndex);

   apiGet (routingShare, "routingShare", theCall.myBomEnt ()->routingShare ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetBomEntrySelForDel (
      const char * consumingOperationName,
      int          bomEntryIndex,
      int *        selForDel)
   {
   WitBomEntryApiCall theCall (this, consumingOperationName, bomEntryIndex);

   apiGet (selForDel, "selForDel", theCall.myBomEnt ()->selForDel ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetBomEntrySingleSource (
      const char * consumingOperationName,
      int          bomEntryIndex,
      WitBoolean * singleSource)
   {
   WitBomEntryApiCall theCall (this, consumingOperationName, bomEntryIndex);

   apiGet (singleSource, "singleSource", theCall.myBomEnt ()->singleSource ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetBomEntrySubConIndex (
      const char * consumingOperationName,
      int          bomEntryIndex,
      const int    thePer,
      int *        subConIndex)
   {
   WitBomEntryApiCall theCall (this, consumingOperationName, bomEntryIndex);

   prepGetIndex (thePer);

   apiGet (
       subConIndex,
      "subConIndex",
      myDetOptImpMgr ()->
         myExtOptMgr ()->
            subConIndex (theCall.myBomEnt (), thePer));
   }

//------------------------------------------------------------------------------

void WitRun::witGetBomEntryUsageRate (
      const char *         consumingOperationName,
      int                  bomEntryIndex,
      const WitOSRealArg & usageRate)
   {
   double * consRateCVec;

   myApiMgr_->startInnerFunc ();

   if (myMsgFac ().mayIssue ("getVecAttrAsScalMsg"))
      myMsgFac () (
         "getVecAttrAsScalMsg",
         apiFuncName (),
         "BOM entry",
         "usageRate",
         "consRate");

   witGetBomEntryConsRate (
      consumingOperationName, 
      bomEntryIndex, 
      & consRateCVec);

   forbidNullArg (usageRate, "usageRate");

   usageRate = consRateCVec[0];

   free (consRateCVec);
   }

//------------------------------------------------------------------------------

void WitRun::witGetBomEntryUsageTime (
      const char *         consumingOperationName,
      int                  bomEntryIndex,
      const WitOSRealArg & usageTime)
   {
   WitBomEntryApiCall theCall (this, consumingOperationName, bomEntryIndex);

   if (not wit34Compatible ())
      myMsgFac () ("getting34AttributeWithoutCompatibilityUsageTime",
         apiFuncName ());

   myMsgFac () ("getting34AttributeWithCompatibilityUsageTime", apiFuncName ());

   apiGet (usageTime, "usageTime", theCall.myBomEnt ()->usageTime34 ());
   }

//------------------------------------------------------------------------------

void WitRun::witSetBomEntryAppData (
      const char * consumingOperationName,
      int          bomEntryIndex,
      void *       appData)
   {
   WitBomEntryApiCall theCall (this, consumingOperationName, bomEntryIndex);

   theCall.setAppData (appData);
   }

//------------------------------------------------------------------------------

void WitRun::witSetBomEntryConsRate (
      const char *         consumingOperationName,
      int                  bomEntryIndex,
      const WitIVRealArg & consRate)
   {
   WitPeriod thePer;

   WitBomEntryApiCall theCall (this, consumingOperationName, bomEntryIndex);

   theCall.prepDblVec ("consRate", consRate);

   if (theCall.argsOK ())
      theCall.myBomEnt ()->setConsRate (consRate);

   forEachPeriod (thePer, myProblem ())
      if (consRate[thePer] < 0.0)
         myMsgFac () ("negConsRateMsg",
            bomEntryIndex,
            consumingOperationName,
            consRate[thePer],
            thePer);
   }

//------------------------------------------------------------------------------

void WitRun::witSetBomEntryEarliestPeriod (
      const char * consumingOperationName,
      int          bomEntryIndex,
      int          earliestPeriod)
   {
   WitBomEntryApiCall theCall (this, consumingOperationName, bomEntryIndex);

   theCall.setEarliestPeriod (earliestPeriod);
   }

//------------------------------------------------------------------------------

void WitRun::witSetBomEntryExecPenalty (
      const char *         consumingOperationName,
      int                  bomEntryIndex,
      const WitISRealArg & execPenalty)
   {
   WitBomEntryApiCall theCall (this, consumingOperationName, bomEntryIndex);

   theCall.prepNonNegDouble (
      "execPenalty", 
      theCall.myBomEnt ()->execPenalty (),
      execPenalty);

   if (theCall.argsOK ())
      theCall.myBomEnt ()->setExecPenalty (execPenalty);
   }

//------------------------------------------------------------------------------

void WitRun::witSetBomEntryFallout (
      const char * consumingOperationName,
      int          bomEntryIndex,
      int          fallout)
   {
   myApiMgr_->startInnerFunc ();

   myMsgFac () (
      "setPre50AttrMsg",
      apiFuncName (),
      "bomEntry",
      "fallout",
      "falloutRate");

   witSetBomEntryFalloutRate (
      consumingOperationName,
      bomEntryIndex,
      percentToFrac (fallout));
   }

//------------------------------------------------------------------------------

void WitRun::witSetBomEntryFalloutRate (
      const char *         consumingOperationName,
      int                  bomEntryIndex,
      const WitISRealArg & falloutRate)
   {
   WitBomEntryApiCall theCall (this, consumingOperationName, bomEntryIndex);

   theCall.prepNonNegUBDouble (
      "falloutRate", 
      theCall.myBomEnt ()->falloutRate (), 
      falloutRate,
      0.99 + FLOAT_EPSILON,
      0.99);

   if (theCall.argsOK ())
      theCall.myBomEnt ()->setFalloutRate (falloutRate);
   }

//------------------------------------------------------------------------------

void WitRun::witSetBomEntryLatestPeriod (
      const char * consumingOperationName,
      int          bomEntryIndex,
      int          latestPeriod)
   {
   WitBomEntryApiCall theCall (this, consumingOperationName, bomEntryIndex);

   theCall.setLatestPeriod (latestPeriod);
   }

//------------------------------------------------------------------------------

void WitRun::witSetBomEntryMandEC (
      const char * consumingOperationName,
      int          bomEntryIndex,
      WitBoolean   mandEC)
   {
   WitBomEntryApiCall theCall (this, consumingOperationName, bomEntryIndex);

   theCall.prepBool ("mandEC", theCall.myBomEnt ()->mandEC (), mandEC);

   if (theCall.argsOK ())
      theCall.myBomEnt ()->setMandEC (asaBool (mandEC));
   }

//------------------------------------------------------------------------------

void WitRun::witSetBomEntryOffset (
      const char *         consumingOperationName,
      int                  bomEntryIndex,
      const WitIVRealArg & offset)
   {
   WitBomEntryApiCall theCall (this, consumingOperationName, bomEntryIndex);

   theCall.setOffset (offset);
   }

//------------------------------------------------------------------------------

void WitRun::witSetBomEntryPropRouting (
      const char * consumingOperationName,
      int          bomEntryIndex,
      WitBoolean   propRouting)
   {
   WitTVec <WitBoolean> & propRtgVec = myApiMgr ()->reserveTempBooleanVec ();

   myApiMgr_->startInnerFunc ();

   if (myMsgFac ().mayIssue ("setVecAttrToScalMsg"))
      myMsgFac () (
         "setVecAttrToScalMsg",
         apiFuncName (),
         "BOM entry",
         "propRouting",
         "propRtg");

   propRtgVec = propRouting;

   witSetBomEntryPropRtg (
      consumingOperationName,
      bomEntryIndex,
      propRtgVec.myCVec ());

   myApiMgr ()->releaseTempVecs ();
   }

//------------------------------------------------------------------------------

void WitRun::witSetBomEntryPropRtg (
      const char *       consumingOperationName,
      int                bomEntryIndex,
      const WitBoolean * propRtg)
   {
   WitBomEntryApiCall theCall (this, consumingOperationName, bomEntryIndex);

   theCall.prepBoolVec ("propRtg", propRtg);

   if (theCall.argsOK ())
      theCall.myBomEnt ()->setPropRtg (propRtg);
   }

//------------------------------------------------------------------------------

void WitRun::witSetBomEntryRoutingShare (
      const char *         consumingOperationName,
      int                  bomEntryIndex,
      const WitIVRealArg & routingShare)
   {
   WitBomEntryApiCall theCall (this, consumingOperationName, bomEntryIndex);

   theCall.setRoutingShare (routingShare);
   }

//------------------------------------------------------------------------------

void WitRun::witSetBomEntrySelForDel (
      const char * consumingOperationName,
      int          bomEntryIndex,
      int          selForDel)
   {
   WitBomEntryApiCall theCall (this, consumingOperationName, bomEntryIndex);

   theCall.setSelForDel (selForDel);
   }

//------------------------------------------------------------------------------

void WitRun::witSetBomEntrySingleSource (
      const char * consumingOperationName,
      int          bomEntryIndex,
      WitBoolean   singleSource)
   {
   WitBomEntryApiCall theCall (this, consumingOperationName, bomEntryIndex);

   theCall.prepBool (
                          "singleSource", 
      theCall.myBomEnt ()->singleSource (), 
                           singleSource);

   if (theCall.argsOK ())
      theCall.myBomEnt ()->setSingleSource (asaBool (singleSource));
   }

//------------------------------------------------------------------------------

void WitRun::witSetBomEntryUsageRate (
      const char *         consumingOperationName,
      int                  bomEntryIndex,
      const WitISRealArg & usageRate)
   {
   WitTVec <double> & consRateVec = myApiMgr ()->reserveTempDblVec ();

   myApiMgr_->startInnerFunc ();

   if (myMsgFac ().mayIssue ("setVecAttrToScalMsg"))
      myMsgFac () (
         "setVecAttrToScalMsg",
         apiFuncName (),
         "BOM entry",
         "usageRate",
         "consRate");

   consRateVec = usageRate;

   witSetBomEntryConsRate (
      consumingOperationName,
      bomEntryIndex,
      consRateVec.myCVec ());

   myApiMgr ()->releaseTempVecs ();
   }

//------------------------------------------------------------------------------

void WitRun::witSetBomEntryUsageTime (
      const char *         consumingOperationName,
      int                  bomEntryIndex,
      const WitISRealArg & inpVal)
   {
   WitBomEntryApiCall theCall (this, consumingOperationName, bomEntryIndex);

   if (not wit34Compatible ())
      myMsgFac () ("setting34AttributeWithoutCompatibilityUsageTime",
         apiFuncName () );

   myMsgFac () ("setting34AttributeWithCompatibilityUsageTime", apiFuncName ());

   theCall.prepDouble (
      "usageTime",
      theCall.myBomEnt ()->usageTime34 (),
      inpVal);

   if (theCall.argsOK ())
      theCall.myBomEnt ()->setUsageTime34 (inpVal);
   }

//------------------------------------------------------------------------------
// getBomEntryP
// Get pointer to WitBomEntry.
//------------------------------------------------------------------------------

WitBomEntry * WitRun::getBomEntryP (
      const char * consumingOperationName,
      int          bomEntryIndex)
   {
   WitBomEntry *  retVal;
   WitOperation * theOpn;

   theOpn = getOperationP (consumingOperationName);

   myInputID ()->setMyOperation (theOpn);

   if (theOpn->bom ().isEmpty ())
      myMsgFac () ("emptyBom");

   if (bomEntryIndex < 0 or bomEntryIndex >= theOpn->bom ().nElements ())
      myMsgFac () ("bomEntryIndexRangeSmsg",
         bomEntryIndex,
         0,
         theOpn->bom ().nElements ());

   retVal = myCompMgr ()->findBomEntry (theOpn, bomEntryIndex);

   witAssert (retVal != NULL);

   myInputID ()->setMyBomEntry (retVal);

   return retVal;
   }
