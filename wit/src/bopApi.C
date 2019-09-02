//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "bopApi.C"
//
// Inner API functions relating to BopEntries.
//------------------------------------------------------------------------------

#include <WitRun.h>
#include <ApiMgr.h>
#include <InputID.h>
#include <ApiCall.h>
#include <Pre.h>
#include <CompMgr.h>
#include <Operation.h>
#include <BopEntry.h>
#include <ISRealArg.h>
#include <OSRealArg.h>
#include <IVRealArg.h>
#include <OVRealArg.h>
#include <MsgFrag.h>
#include <MsgFac.h>

#include <stdlib.h>

//------------------------------------------------------------------------------
// setGetBopEntryInit
// Initilization of wit[GS]etBopEntry* functions.
//------------------------------------------------------------------------------

WitBopEntry * WitRun::setGetBopEntryInit (
      const char * producingOperationName,
      int          bopEntryIndex)
   {
   forbidNullArg (producingOperationName, "producingOperationName");

   if (myMsgFac ().mayIssue ("operationNameMsg"))
      myMsgFac () ("operationNameMsg", producingOperationName);

   if (myMsgFac ().mayIssue ("bopEntryIndexMsg"))
      myMsgFac () ("bopEntryIndexMsg", bopEntryIndex);

   return getBopEntryP (producingOperationName, bopEntryIndex);
   }

//------------------------------------------------------------------------------

void WitRun::witAddBopEntry (
      const char * producingOperationName,
      const char * producedPartName)
   {
   WitOperation * operationP;
   WitPart *      partP;

   myApiMgr_->startInnerFunc ();

   forbidNullArg (producingOperationName, "producingOperationName");
   forbidNullArg (producedPartName,       "producedPartName");

   if (myMsgFac ().mayIssue ("bopAddMsg"))
      myMsgFac () ("bopAddMsg", producedPartName,producingOperationName);

   operationP = myCompMgr ()->findOperation (producingOperationName);

   if (operationP == NULL)
      myMsgFac () ("undefinedOperationMsg", producingOperationName);

   myInputID ()->setMyOperation (operationP);

   partP = myCompMgr ()->findPart (producedPartName);

   if (partP == NULL)
      myMsgFac () ("undefinedPartMsg", producedPartName);

   addingDelComp ("bopEntryFrag");

   new WitBopEntry (operationP , partP);

   myInputID ()->setMyOperation (NULL);
   }

//------------------------------------------------------------------------------

void WitRun::witCopyBopEntryData (
      const char * dupOperationName,
      int          dupBopEntryIndex,
      WitRun *     origWitRun,
      const char * origOperationName,
      int          origBopEntryIndex)
   {
   startCopyObjData (origWitRun);

   myMsgFac ()  ("dupObjectMsg", myMsgFac ().myFrag ("bopEntryFrag"));

   WitBopEntryApiCall dupCall  (
      this,
      dupOperationName,
      dupBopEntryIndex);

   myMsgFac () ("origObjectMsg", myMsgFac ().myFrag ("bopEntryFrag"));

   WitBopEntryApiCall origCall  (
      origWitRun,
      origOperationName,
      origBopEntryIndex);

   if (dupCall.myBopEnt () == origCall.myBopEnt ())
      myMsgFac () ("copyObjectToSelfSmsg",
         myMsgFac ().myFrag ("bopEntryFrag"));

   dupCall.myBopEnt ()->copyAttrsFrom (origCall.myBopEnt ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetObjItrBopEntry (
      char * * producingOperationName,
      int *    bopEntryIndex)
   {
   WitBopEntry * theBopEnt;

   myApiMgr_->startInnerFunc ();

   theBopEnt =
      curItrDelCompForGet (WitAT_BOP_ENTRY, "bopEntryFrag")->
         thisBopEnt ();

   stronglyAssert (theBopEnt != NULL);

   apiGet (
      producingOperationName, 
      "producingOperationName", 
      theBopEnt->myOperationName ());

   apiGet (bopEntryIndex, "bopEntryIndex", theBopEnt->localIndex ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetBopEntryAppData (
      const char * producingOperationName,
      int          bopEntryIndex,
      void * *     appData)
   {
   WitBopEntryApiCall theCall (this, producingOperationName, bopEntryIndex);

   apiGet (appData, "appData", theCall.myBopEnt ()->appData ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetBopEntryByProduct (
      const char * producingOperationName,
      int          bopEntryIndex,
      WitBoolean * byProduct)
   {
   WitBoolean expAllowedVal;

   myApiMgr_->startInnerFunc ();

   myMsgFac () (
      "getPre50AttrMsg",
      apiFuncName (),
      "bopEntry",
      "byProduct",
      "expAllowed");

   witGetBopEntryExpAllowed (
      producingOperationName,
      bopEntryIndex,
      & expAllowedVal);

   apiGet (byProduct, "byProduct", not expAllowedVal);
   }

//------------------------------------------------------------------------------

void WitRun::witGetBopEntryEarliestPeriod (
      const char * producingOperationName,
      int          bopEntryIndex,
      int *        earliestPeriod)
   {
   WitBopEntryApiCall theCall (this, producingOperationName, bopEntryIndex);

   apiGet (                earliestPeriod, 
                          "earliestPeriod", 
      theCall.myBopEnt ()->earliestPeriod ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetBopEntryExpAllowed (
      const char * producingOperationName,
      int          bopEntryIndex,
      WitBoolean * expAllowed)
   {
   WitBopEntryApiCall theCall (this, producingOperationName, bopEntryIndex);;

   apiGet (expAllowed, "expAllowed", theCall.myBopEnt ()->expAllowed ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetBopEntryExpAversion (
      const char *         producingOperationName,
      int                  bopEntryIndex,
      const WitOSRealArg & expAversion)
   {
   WitBopEntryApiCall theCall (this, producingOperationName, bopEntryIndex);;

   apiGet (
      expAversion,
      "expAversion",
      theCall.myBopEnt ()->expAversion ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetBopEntryImpactPeriod (
      const char * producingOperationName,
      int          bopEntryIndex,
      int * *      impactPeriod)
   {
   WitBopEntryApiCall theCall (this, producingOperationName, bopEntryIndex);;

   myProblem ()->myPreprocessor ()->preprocess ();

   apiGet (impactPeriod, "impactPeriod", theCall.myBopEnt ()->impactPeriod ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetBopEntryLatestPeriod (
      const char * producingOperationName,
      int          bopEntryIndex,
      int *        latestPeriod)
   {
   WitBopEntryApiCall theCall (this, producingOperationName, bopEntryIndex);;

   apiGet (latestPeriod, "latestPeriod", theCall.myBopEnt ()->latestPeriod ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetBopEntryMeSelSplit (
      const char * producingOperationName,
      int          bopEntryIndex,
      WitBoolean * meSelSplit)
   {
   WitBopEntryApiCall theCall (this, producingOperationName, bopEntryIndex);;

   apiGet (meSelSplit, "meSelSplit", theCall.myBopEnt ()->meSelSplit ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetBopEntryOffset (
      const char *         producingOperationName,
      int                  bopEntryIndex,
      const WitOVRealArg & offset)
   {
   WitBopEntryApiCall theCall (this, producingOperationName, bopEntryIndex);;

   apiGet (offset, "offset", theCall.myBopEnt ()->offset ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetBopEntryProdRate (
      const char *         producingOperationName,
      int                  bopEntryIndex,
      const WitOSRealArg & prodRate)
   {
   double * productRateCVec;

   myApiMgr_->startInnerFunc ();

   if (myMsgFac ().mayIssue ("getVecAttrAsScalMsg"))
      myMsgFac () (
         "getVecAttrAsScalMsg",
         apiFuncName (),
         "BOP entry",
         "prodRate",
         "productRate");

   witGetBopEntryProductRate (
      producingOperationName, 
      bopEntryIndex, 
      & productRateCVec);

   forbidNullArg (prodRate, "prodRate");

   prodRate = productRateCVec[0];

   free (productRateCVec);
   }

//------------------------------------------------------------------------------

void WitRun::witGetBopEntryProductRate (
      const char *         producingOperationName,
      int                  bopEntryIndex,
      const WitOVRealArg & productRate)
   {
   WitBopEntryApiCall theCall (this, producingOperationName, bopEntryIndex);;

   apiGet (productRate, "productRate", theCall.myBopEnt ()->productRate ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetBopEntryProducedPart (
     const char * producingOperationName,
     int          bopEntryIndex,
     char * *     producedPartName)
   {
   WitBopEntryApiCall theCall (this, producingOperationName, bopEntryIndex);;

   apiGet (
       producedPartName, 
      "producedPartName", 
      theCall.myBopEnt ()->myPartName ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetBopEntryRoutingShare (
      const char *         producingOperationName,
      int                  bopEntryIndex,
      const WitOVRealArg & routingShare)
   {
   WitBopEntryApiCall theCall (this, producingOperationName, bopEntryIndex);;

   apiGet (routingShare, "routingShare", theCall.myBopEnt ()->routingShare ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetBopEntrySelForDel (
      const char * producingOperationName,
      int          bopEntryIndex,
      WitBoolean * selForDel)
   {
   WitBopEntryApiCall theCall (this, producingOperationName, bopEntryIndex);;

   apiGet (selForDel, "selForDel", theCall.myBopEnt ()->selForDel ());
   }

//------------------------------------------------------------------------------

void WitRun::witSetBopEntryAppData (
      const char * producingOperationName,
      int          bopEntryIndex,
      void *       appData)
   {
   WitBopEntryApiCall theCall (this, producingOperationName, bopEntryIndex);;

   theCall.setAppData (appData);
   }

//------------------------------------------------------------------------------

void WitRun::witSetBopEntryByProduct (
      const char * producingOperationName,
      int          bopEntryIndex,
      int          byProduct)
   {
   myApiMgr_->startInnerFunc ();

   myMsgFac () (
      "setPre50AttrMsg",
      apiFuncName (),
      "bopEntry",
      "byProduct",
      "expAllowed");

   witSetBopEntryExpAllowed (
      producingOperationName,
      bopEntryIndex,
      not byProduct);
   }

//------------------------------------------------------------------------------

void WitRun::witSetBopEntryEarliestPeriod (
      const char * producingOperationName,
      int          bopEntryIndex,
      int earliestPeriod)
   {
   WitBopEntryApiCall theCall (this, producingOperationName, bopEntryIndex);;

   theCall.setEarliestPeriod (earliestPeriod);
   }

//------------------------------------------------------------------------------

void WitRun::witSetBopEntryExpAllowed (
      const char * producingOperationName,
      int          bopEntryIndex,
      int          expAllowed)
   {
   WitBopEntryApiCall theCall (this, producingOperationName, bopEntryIndex);;

   theCall.prepBool (
                          "expAllowed", 
      theCall.myBopEnt ()->expAllowed (), 
                           expAllowed);

   if (theCall.argsOK ())
      theCall.myBopEnt ()->setExpAllowed (asaBool (expAllowed));
   }

//------------------------------------------------------------------------------

void WitRun::witSetBopEntryExpAversion (
      const char *         producingOperationName,
      int                  bopEntryIndex,
      const WitISRealArg & expAversion)
   {
   WitBopEntryApiCall theCall (this, producingOperationName, bopEntryIndex);;

   theCall.prepDouble (
      "expAversion", 
      theCall.myBopEnt ()->expAversion (),
      expAversion);

   if (theCall.argsOK ())
      theCall.myBopEnt ()->setExpAversion (expAversion);
   }

//------------------------------------------------------------------------------

void WitRun::witSetBopEntryLatestPeriod (
      const char * producingOperationName,
      int          bopEntryIndex,
      int latestPeriod)
   {
   WitBopEntryApiCall theCall (this, producingOperationName, bopEntryIndex);;

   theCall.setLatestPeriod (latestPeriod);
   }

//------------------------------------------------------------------------------

void WitRun::witSetBopEntryMeSelSplit (
      const char * producingOperationName,
      int          bopEntryIndex,
      int          meSelSplit)
   {
   WitBopEntryApiCall theCall (this, producingOperationName, bopEntryIndex);;

   theCall.prepBool (
                          "meSelSplit", 
      theCall.myBopEnt ()->meSelSplit (), 
                           meSelSplit);

   if (theCall.argsOK ())
      theCall.myBopEnt ()->setMeSelSplit (asaBool (meSelSplit));
   }

//------------------------------------------------------------------------------

void WitRun::witSetBopEntryOffset (
      const char *         producingOperationName,
      int                  bopEntryIndex,
      const WitIVRealArg & offset)
   {
   WitBopEntryApiCall theCall (this, producingOperationName, bopEntryIndex);;

   theCall.setOffset (offset);
   }

//------------------------------------------------------------------------------

void WitRun::witSetBopEntryProdRate (
      const char *         producingOperationName,
      int                  bopEntryIndex,
      const WitISRealArg & prodRate)
   {
   WitTVec <double> & productRateVec = myApiMgr ()->reserveTempDblVec ();

   myApiMgr_->startInnerFunc ();

   if (myMsgFac ().mayIssue ("setVecAttrToScalMsg"))
      myMsgFac () (
         "setVecAttrToScalMsg",
         apiFuncName (),
         "BOP entry",
         "prodRate",
         "productRate");

   productRateVec = prodRate;

   witSetBopEntryProductRate (
      producingOperationName,
      bopEntryIndex,
      productRateVec.myCVec ());

   myApiMgr ()->releaseTempVecs ();
   }

//------------------------------------------------------------------------------

void WitRun::witSetBopEntryProductRate (
      const char *         producingOperationName,
      int                  bopEntryIndex,
      const WitIVRealArg & productRate)
   {
   WitBopEntryApiCall theCall (this, producingOperationName, bopEntryIndex);;

   theCall.prepNonNegDblVec ("productRate", productRate);

   if (theCall.argsOK ())
      theCall.myBopEnt ()->setProductRate (productRate);
   }

//------------------------------------------------------------------------------

void WitRun::witSetBopEntryRoutingShare (
      const char *         producingOperationName,
      int                  bopEntryIndex,
      const WitIVRealArg & routingShare)
   {
   WitBopEntryApiCall theCall (this, producingOperationName, bopEntryIndex);;

   theCall.setRoutingShare (routingShare);
   }

//------------------------------------------------------------------------------

void WitRun::witSetBopEntrySelForDel (
      const char * producingOperationName,
      int          bopEntryIndex,
      int          selForDel)
   {
   WitBopEntryApiCall theCall (this, producingOperationName, bopEntryIndex);;

   theCall.setSelForDel (selForDel);
   }

//------------------------------------------------------------------------------
// getBopEntryP
// Get pointer to BopEntry.
//------------------------------------------------------------------------------

WitBopEntry * WitRun::getBopEntryP (
      const char *  producingOperationName,
      int           bopEntryIndex)
   {
   WitBopEntry *  retVal;
   WitOperation * operationP ;

   operationP  = getOperationP(producingOperationName);

   myInputID ()->setMyOperation (operationP);

   if (operationP ->bop ().isEmpty ())
      myMsgFac () ("emptyBopSmsg");

   if (bopEntryIndex < 0 or bopEntryIndex >= operationP->bop ().nElements())
      myMsgFac () ("bopEntryIndexRangeSmsg",
         bopEntryIndex,
         0,
         operationP ->bop ().nElements());

   retVal = myCompMgr ()->findBopEntry (operationP, bopEntryIndex);

   witAssert (retVal != NULL);

   myInputID ()->setMyBopEntry (retVal);

   return retVal;
   }
