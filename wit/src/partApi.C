//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "partApi.C"
//
// Inner API functions relating to Parts.
//------------------------------------------------------------------------------

#include <WitRun.h>
#include <ApiMgr.h>
#include <InputID.h>
#include <ApiCall.h>
#include <ISRealArg.h>
#include <OSRealArg.h>
#include <IVRealArg.h>
#include <OVRealArg.h>
#include <OTDARealArg.h>
#include <Pre.h>
#include <DetOptImpMgr.h>
#include <ExtOptMgr.h>
#include <StochImpMgr.h>
#include <CompMgr.h>
#include <Demand.h>
#include <Material.h>
#include <Capacity.h>
#include <BomEntry.h>
#include <SubEntry.h>
#include <BopEntry.h>
#include <GlobalComp.h>
#include <Post.h>
#include <BoundSet.h>
#include <MsgFrag.h>
#include <MsgFac.h>

#include <float.h>
#include <stdlib.h>

//------------------------------------------------------------------------------
// setGetPartInit
// Initialization of witSetPart* functions.
//------------------------------------------------------------------------------

WitPart * WitRun::setGetPartInit (const char * partName)
   {
   WitPart * thePart;

   forbidNullArg (partName, "partName");

   if (myMsgFac ().mayIssue ("partNameMsg"))
      myMsgFac () ("partNameMsg", partName);

   thePart = getPartP (partName);

   myInputID ()->setMyPart (thePart);

   return thePart;
   }

//------------------------------------------------------------------------------

WitMaterial * WitRun::setGetMaterialInit (
      const char * partName,
      const char * theAttName)
   {
   WitPart *     thePart;
   WitMaterial * theMat;

   thePart = setGetPartInit (partName);

   theMat  = thePart->thisMat ();

   if (theMat == NULL)
      {
      myInputID ()->setMyAttName (theAttName);

      myMsgFac () ("materialReqdSmsg");
      }

   return theMat;
   }

//------------------------------------------------------------------------------

void WitRun::witAddPart (
      const char * partName,
      WitAttr      partCategory)
   {
   myApiMgr_->startInnerFunc ();

   forbidNullArg (partName, "partName");

   if (wit34Compatible ())
      addPart34     (partName, partCategory);
   else
      addPartPost34 (partName, partCategory);
   }

//------------------------------------------------------------------------------

void WitRun::witAddPartWithOperation (
      const char * partAndOperationName)
   {
   myApiMgr_->startInnerFunc ();

   witAddPart (partAndOperationName, WitMATERIAL);

   witAddOperation (partAndOperationName);

   witAddBopEntry (partAndOperationName, partAndOperationName);
   }

//------------------------------------------------------------------------------

void WitRun::witCopyPartData (
      const char * dupPartName,
      WitRun *     origWitRun,
      const char * origPartName)
   {
   startCopyObjData (origWitRun);

   myMsgFac ()  ("dupObjectMsg", myMsgFac ().myFrag ("partFrag"));

   WitPartApiCall dupCall (this,      dupPartName);

   myMsgFac () ("origObjectMsg", myMsgFac ().myFrag ("partFrag"));

   WitPartApiCall origCall (origWitRun, origPartName);

   WitPart *  dupPart;
   WitPart * origPart;

    dupPart =  dupCall.myPart ();
   origPart = origCall.myPart ();

   if (dupPart == origPart)
      myMsgFac () ("copyObjectToSelfSmsg", myMsgFac ().myFrag ("partFrag"));

   if      ((dupPart->thisMat () != NULL) and (origPart->thisMat () != NULL))
      {
      dupPart->thisMat ()->copyAttrsFrom (origPart->thisMat ());
      }
   else if ((dupPart->thisCap () != NULL) and (origPart->thisCap () != NULL))
      {
      dupPart->thisCap ()->copyAttrsFrom (origPart->thisCap ());
      }
   else
      myMsgFac () ("copyPartCatMismatchSmsg",
          dupPart->partName     (),
          dupPart->categoryFrag (),
         origPart->partName     (),
         origPart->categoryFrag ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetFocusShortageVol (
      int   *                lenList,
      char  * * *            partList,
      const WitOTDARealArg & shortageVolList)
   {
   int       nShortParts;
   WitPart * thePart;
   int       theIdx;

   myApiMgr_->startInnerFunc ();

   forbidNullArg (lenList,         "lenList");
   forbidNullArg (partList,        "partList");
   forbidNullArg (shortageVolList, "shortageVolList");

   forbidStochMode ();

   myProblem ()->computeFssIfNeeded ();

   nShortParts = 0;

   forEachPart (thePart, myProblem ())
      if (thePart->focShortageVol () != 0.0)
         nShortParts ++;

   * lenList = nShortParts;

   apiAlloc (partList, nShortParts);

   shortageVolList.apiAlloc (nShortParts, nPeriods ());

   theIdx = -1;

   forEachPart (thePart, myProblem ())
      if (thePart->focShortageVol () != 0.0)
         {
         theIdx ++;

         (* partList)[theIdx] = thePart->partName ().apiDupCstring ();

         shortageVolList.copyIntoRow (theIdx, thePart->focShortageVol ());
         }
   }

//------------------------------------------------------------------------------

void WitRun::witGetObjItrPart (char * * partName)
   {
   WitPart * thePart;

   myApiMgr_->startInnerFunc ();

   thePart =
      curItrDelCompForGet (WitAT_PART, "partFrag")->
         thisPart ();

   stronglyAssert (thePart != NULL);

   apiGet (partName, "partName", thePart->partName ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartAppData (
      const char * partName,
      void * *     appData)
   {
   WitPartApiCall theCall (this, partName);

   apiGet (appData, "appData", theCall.myPart ()->appData ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartAsapPipOrder (
      const char * partName,
      WitBoolean * asapPipOrder)
   {
   WitMaterialApiCall theCall (this, partName, "asapPipOrder");

   apiGet (
                             asapPipOrder, 
                            "asapPipOrder", 
      theCall.myMaterial ()->asapPipOrder ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartBaSelSplit (
      const char * partName,
      WitBoolean * baSelSplit)
   {
   WitMaterialApiCall theCall (this, partName, "baSelSplit");

   apiGet (baSelSplit, "baSelSplit", theCall.myMaterial ()->baSelSplit ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartBelowList (
      const char * partName,
      int *        lenList,
      char * * *   partNameList)
   {
   WitPartApiCall theCall (this, partName);

   forbidNullArg (lenList,      "lenList");
   forbidNullArg (partNameList, "partNameList");

   int &                  lenListRef      = * lenList;
   char * * &             partNameListRef = * partNameList;
   WitPart *              thePart;
   WitPtrVecItr <WitPart> theItr;

   myProblem ()->myPreprocessor ()->preprocess ();

   lenListRef = theCall.myPart ()->belowList ().length ();

   apiAlloc (partNameList, lenListRef);

   theCall.myPart ()->belowList ().attachItr (theItr);

   while (theItr.advance (thePart))
      partNameListRef[theItr.myIdx ()] = thePart->partName ().apiDupCstring ();
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartBoundedLeadTimes (
      const char * partName,
      WitBoolean * boundedLeadTimes)
   {
   WitPartApiCall theCall (this, partName);

   apiGet (
                         boundedLeadTimes, 
                        "boundedLeadTimes", 
      theCall.myPart ()->boundedLeadTimes ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartBuildAheadUB (
      const char * partName,
      int * *      buildAheadUB)
   {
   WitMaterialApiCall theCall (this, partName, "buildAheadUB");

   apiGet (                  buildAheadUB, 
                            "buildAheadUB", 
      theCall.myMaterial ()->buildAheadUB ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartBuildAheadLimit (
      const char * partName,
      int *        buildAheadLimit)
   {
   int * buildAheadUBval;

   myApiMgr_->startInnerFunc ();

   if (myMsgFac ().mayIssue ("getVecAttrAsScalMsg"))
      myMsgFac () (
         "getVecAttrAsScalMsg",
         apiFuncName (),
         "part",
         "buildAheadLimit",
         "buildAheadUB");

   witGetPartBuildAheadUB (partName, & buildAheadUBval);

   forbidNullArg (buildAheadLimit, "buildAheadLimit");

   * buildAheadLimit = buildAheadUBval[0];

   free (buildAheadUBval);
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartBuildAsap (
      const char * partName,
      WitBoolean * buildAsap)
   {
   WitMaterialApiCall theCall (this, partName, "buildAsap");

   apiGet (buildAsap, "buildAsap", theCall.myMaterial ()->buildAsap ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartBuildNstn (
      const char * partName,
      WitBoolean * buildNstn)
   {
   WitMaterialApiCall theCall (this, partName, "buildNstn");

   apiGet (buildNstn, "buildNstn", theCall.myMaterial ()->buildNstn ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartCategory (
      const char * partName,
      WitAttr * category)
   {
   WitPartApiCall theCall (this, partName);

   apiGet (category, "category", theCall.myPart ()->apiCategory ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartConsVol (
      const char *         partName,
      const WitOVRealArg & consVol)
   {
   WitPartApiCall theCall (this, partName);

   apiGet (consVol, "consVol", theCall.myPart ()->consVol ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartConsumingBomEntry (
      const char * partName,
      int          consIndex,
      char * *     consumingOperationName,
      int *        bomEntryIndex)
   {
   WitPartApiCall theCall (this, partName);

   WitBomEntry *  theBomEnt;

   if (consIndex < 0 or
       consIndex >= theCall.myPart ()->consumingBomEntries ().nElements ())
      myMsgFac () ("bomEntryConsIndexRangeSmsg",
         consIndex,
         0,
         theCall.myPart ()->consumingBomEntries ().nElements ());

   theBomEnt = theCall.myPart ()->consumingBomEntries ().at (consIndex);

   * consumingOperationName = theBomEnt->myOperationName ().apiDupCstring ();
   * bomEntryIndex          = theBomEnt->localIndex ();
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartConsumingSubsBomEntry (
      const char * partName,
      int          consIndex,
      char * *     consumingOperationName,
      int *        bomEntryIndex,
      int *        subsBomEntryIndex)
   {
   WitSubEntry * theSub;

   WitPartApiCall theCall (this, partName);

      // Make sure consIndex is valid.
      //
   if (consIndex < 0 or
       consIndex >= theCall.myPart ()->consumingSubEntries ().nElements ())
      myMsgFac () ("subEntryConsIndexRangeSmsg",
         consIndex,
         0,
         theCall.myPart ()->consumingSubEntries ().nElements ());

   theSub = theCall.myPart ()->consumingSubEntries ().at (consIndex);

   * consumingOperationName = theSub->myOperationName ().apiDupCstring ();
   * bomEntryIndex          = theSub->myBomEntIndex ();
   * subsBomEntryIndex      = theSub->localIndex ();
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartCycleTime (
      const char *         partName,
      const WitOVRealArg & inpVal)
   {
   myApiMgr_->startInnerFunc ();

   if (wit34Compatible ())
      {
      myMsgFac () ("getting34AttributeWithCompatibilityCycleTime",
         apiFuncName ());

      witGetOperationCycleTime34 (partName, inpVal);
      }
   else
      myMsgFac () ("getting34AttributeWithoutCompatibilityCycleTime",
         apiFuncName ());
   }

//------------------------------------------------------------------------------
// Return a list of all demand names for a material part.
// If the part is a capacity, there is no list of demands; in this case, an
// empty list is returned (for upward compatability).
//------------------------------------------------------------------------------

void WitRun::witGetPartDemands (
      const char * partName,
      int  *       lenDemandList,
      char * * *   demandList)
   {
   WitDemand * theDemand;
   int         i;

   WitPartApiCall theCall (this, partName);

   apiGet (
      lenDemandList, 
      "lenDemandList", 
      theCall.myPart ()->myDemands ().nElements ());

   apiAlloc (demandList, * lenDemandList);

   i = 0;

   forEachEl (theDemand, theCall.myPart ()->myDemands ())
      (* demandList)[i++] = theDemand->demandName ().apiDupCstring ();
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartExcessVol (
      const char *         partName,
      const WitOVRealArg & excessVol)
   {
   WitPeriod thePer;

   WitPartApiCall theCall (this, partName);

   forbidNullArg (excessVol, "excessVol");

   apiAlloc (excessVol, nPeriods ());

   forEachPeriod (thePer, myProblem ())
      excessVol[thePer] =
         myProblem ()->
            myPostprocessor ()->
               compExcessVol (theCall.myPart (), thePer);
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartExists (
      const char * partNameParm,
      WitBoolean * exists)
   {
   WitPart * thePart;

   myApiMgr_->startInnerFunc ();

   thePart  = myCompMgr ()->findPart (partNameParm);

   * exists = (thePart != NULL);
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartFocusShortageVol (
      const char *         partName,
      const WitOVRealArg & focusShortageVol)
   {
   WitPartApiCall theCall (this, partName);

   forbidStochMode ();

   myProblem ()->computeFssIfNeeded ();

   apiGet (              focusShortageVol, 
                        "focusShortageVol", 
      theCall.myPart ()->focShortageVol ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartIncLotSize (
      const char *         partName,
      const WitOVRealArg & incLotSize)
   {
   myApiMgr_->startInnerFunc ();

   prepGet34Attr ("Part", "incLotSize", "Operation", "incLotSize");

   witGetOperationIncLotSize (partName, incLotSize);
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartMinLotSize (
      const char *         partName,
      const WitOVRealArg & minLotSize)
   {
   myApiMgr_->startInnerFunc ();

   prepGet34Attr ("Part", "minLotSize", "Operation", "minLotSize");

   witGetOperationMinLotSize (partName, minLotSize);
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartMrpConsVol (
      const char *         partName,
      const WitOVRealArg & mrpConsVol)
   {
   WitPartApiCall theCall (this, partName);

   apiGet (mrpConsVol, "mrpConsVol", theCall.myPart ()->mrpConsVol ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartMrpExcessVol (
      const char *         partName,
      const WitOVRealArg & mrpExcessVol)
   {
   WitPeriod thePer;

   WitPartApiCall theCall (this, partName);

   forbidNullArg (mrpExcessVol, "mrpExcessVol");

   apiAlloc (mrpExcessVol, nPeriods ());

   forEachPeriod (thePer, myProblem ())
      mrpExcessVol[thePer] =
         min (
            theCall.myPart ()->mrpResidualVol ()[thePer],
            theCall.myPart ()->supplyVol      ()[thePer]);
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartMrpProdVol (
      const char *         partName,
      const WitOVRealArg & mrpProdVol)
   {
   myApiMgr_->startInnerFunc ();

   prepGet34Attr ("Part", "mrpProdVol", "Operation", "mrpExecVol");

   witGetOperationMrpExecVol (partName, mrpProdVol);
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartMrpResidualVol (
      const char *         partName,
      const WitOVRealArg & mrpResidualVol)
   {
   WitPartApiCall theCall (this, partName);

   apiGet (              mrpResidualVol, 
                        "mrpResidualVol", 
      theCall.myPart ()->mrpResidualVol ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartNBomEntries (
      const char * partName,
      int *        inpVal)
   {
   WitPart * partP;

   myApiMgr_->startInnerFunc ();

   if (wit34Compatible ())
      {
      partP = setGetPartInit (partName);

         // If part is not a product, then return 0, otherwise get nBomEntries
         // from operation.
         //
      if (partP->apiCategory() != WitPRODUCT)
         {
         myMsgFac () ("getting34AttributeNBomEntriesForNonProduct",
            partName,
            partP->categoryFrag ());

         * inpVal = 0;
         }
      else
         {
         myMsgFac () ("getting34AttributeWithCompatibilityMode",
            apiFuncName (),
            "Part",
            "nBomEntries",
            "Operation",
            "nBomEntries");

         witGetOperationNBomEntries (partName, inpVal);
         }

      myInputID ()->setMyPart (NULL);
      }
   else
      myMsgFac () ("getting34AttributeWithoutCompatibilityMode",
         apiFuncName (),
         "Part",
         "nBomEntries",
         "Operation",
         "nBomEntries");
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartNConsumingBomEntries (
      const char * partName,
      int *        nConsumingBomEntries)
   {
   WitPartApiCall theCall (this, partName);

   apiGet (
       nConsumingBomEntries,
      "nConsumingBomEntries",
      theCall.myPart ()->consumingBomEntries ().nElements ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartNConsumingSubsBomEntries (
      const char * partName,
      int *        nConsumingSubsBomEntries)
   {
   WitPartApiCall theCall (this, partName);

   apiGet (
       nConsumingSubsBomEntries,
      "nConsumingSubsBomEntries",
      theCall.myPart ()->consumingSubEntries ().nElements ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartNProducingBopEntries (
      const char * partName,
      int * nProducingBopEntries)
   {
   WitPartApiCall theCall (this, partName);

   apiGet (
       nProducingBopEntries,
      "nProducingBopEntries",
      theCall.myPart ()->producingBopEntries ().nElements ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartObjectStage (
      const char * partName,
      int *        objectStage)
   {
   WitPartApiCall theCall (this, partName);

   theCall.getObjectStage (objectStage);
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartObj1ScrapCost (
      const char *         partName,
      const WitOVRealArg & obj1ScrapCost)
   {
   myApiMgr_->startInnerFunc ();

   if (myMsgFac ().mayIssue ("getAttrNewNameMsg"))
      myMsgFac () (
         "getAttrNewNameMsg",
         apiFuncName (),
         "part",
         "obj1ScrapCost",
         "scrapCost");

   witGetPartScrapCost (partName, obj1ScrapCost);
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartObj1StockCost (
      const char *         partName,
      const WitOVRealArg & obj1StockCost)
   {
   myApiMgr_->startInnerFunc ();

   if (myMsgFac ().mayIssue ("getAttrNewNameMsg"))
      myMsgFac () (
         "getAttrNewNameMsg",
         apiFuncName (),
         "part",
         "obj1StockCost",
         "stockCost");

   witGetPartStockCost (partName, obj1StockCost);
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartObj1ProdCost (
      const char *         partName,
      const WitOVRealArg & obj1ProdCost)
   {
   myApiMgr_->startInnerFunc ();

   prepGet34Attr ("Part", "obj1ProdCost", "Operation", "obj1ExecCost");

   witGetOperationObj1ExecCost (partName, obj1ProdCost);
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartProdBounds (
      const char *         partName,
      const WitOVRealArg & hardLB,
      const WitOVRealArg & softLB,
      const WitOVRealArg & hardUB)
   {
   myApiMgr_->startInnerFunc ();

   prepGet34Attr ("Part", "prodBounds", "Operation", "execBounds");

   witGetOperationExecBounds (partName, hardLB, softLB, hardUB);
   }

//------------------------------------------------------------------------------
// prodVol, an attribute which has a new meaning with Wit V4.0.
// Wit V3.4 provides set and get functions.
// Wit V4.0 only provides a get function.
//------------------------------------------------------------------------------

void WitRun::witGetPartProdVol (
      const char *         partName,
      const WitOVRealArg & prodVol)
   {
   myApiMgr_->startInnerFunc ();

   if (wit34Compatible ())
      {
      prepGet34Attr ("Part", "prodVol", "Operation", "execVol");

      witGetOperationExecVol (partName, prodVol);
      }
   else
      {
      WitPartApiCall theCall (this, partName);

      apiGet (prodVol, "prodVol", theCall.myPart ()->prodVol ());
      }
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartProducingBopEntry (
      const char * partName,
      int          prodIndex,
      char * *     producingOperationName,
      int *        bopEntryIndex)
   {
   WitBopEntry * theBopEnt;

   WitPartApiCall theCall (this, partName);

   if (prodIndex < 0 or
       prodIndex >= theCall.myPart ()->producingBopEntries ().nElements ())
      myMsgFac () ("bopEntryProdIndexRangeSmsg",
         prodIndex,
         0,
         theCall.myPart ()->producingBopEntries ().nElements ());

   theBopEnt = theCall.myPart ()->producingBopEntries ().at (prodIndex);

   * producingOperationName = theBopEnt->myOperationName ().apiDupCstring ();
   * bopEntryIndex          = theBopEnt->localIndex ();
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartReqVol (
      const char *         partName,
      const WitOVRealArg & reqVol)
   {
   WitPartApiCall theCall (this, partName);

   apiGet (reqVol, "reqVol", theCall.myPart ()->reqVol ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartResidualVol (
      const char *         partName,
      const WitOVRealArg & residualVol)
   {
   WitPartApiCall theCall (this, partName);

   apiGet (residualVol, "residualVol", theCall.myPart ()->residualVol ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartResourceConIndex (
      const char * partName,
      const int    thePer,
      int *        resourceConIndex)
   {
   WitPartApiCall theCall (this, partName);

   prepGetIndex (thePer);

   apiGet (
       resourceConIndex,
      "resourceConIndex",
      myDetOptImpMgr ()->
         myExtOptMgr ()->
            resourceConIndex (theCall.myPart (), thePer));
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartScrapVarIndex (
      const char * partName,
      const int    thePer,
      int *        scrapVarIndex)
   {
   WitPartApiCall theCall (this, partName);

   prepGetIndex (thePer);

   apiGet (
       scrapVarIndex,
      "scrapVarIndex",
      myDetOptImpMgr ()->
         myExtOptMgr ()->
            scrapVarIndex (theCall.myPart (), thePer));
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartPropRouting (
      const char * partName,
      WitBoolean * propRouting)
   {
   WitPartApiCall theCall (this, partName);

   if (myMsgFac ().mayIssue ("getVecAttrAsScalMsg"))
      myMsgFac () (
         "getVecAttrAsScalMsg",
         apiFuncName (),
         "part",
         "propRouting",
         "propRtg");

   apiGet (propRouting, "propRouting", theCall.myPart ()->propRtg ()[0]);
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartPropRtg (
      const char *   partName,
      WitBoolean * * propRtg)
   {
   WitPartApiCall theCall (this, partName);

   apiGet (propRtg, "propRtg", theCall.myPart ()->propRtg ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetParts (
      int  *     lenPartList,
      char * * * partList)
   {
   WitPart * partj;
   int       theIdx;

   myApiMgr_->startInnerFunc ();

   theIdx = 0;

   * lenPartList = myCompMgr ()->nParts ();

   apiAlloc (partList, * lenPartList);

   forEachPart (partj, myProblem ())
      (* partList)[theIdx ++] = partj->partName ().apiDupCstring ();
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartScrapAllowed (
      const char * partName,
      WitBoolean * scrapAllowed)
   {
   WitPartApiCall theCall (this, partName);

   apiGet (scrapAllowed, "scrapAllowed", theCall.myPart ()->scrapAllowed ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartScrapCost (
      const char *         partName,
      const WitOVRealArg & scrapCost)
   {
   WitPartApiCall theCall (this, partName);

   apiGet (
       scrapCost,
      "scrapCost",
      theCall.myPart ()->scrapCost ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartScrapVol (
      const char *         partName,
      const WitOVRealArg & scrapVol)
   {
   WitPartApiCall theCall (this, partName);

   apiGet (scrapVol, "scrapVol", theCall.myPart ()->scrapVol ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartSelForDel (
      const char * partName,
      WitBoolean * selForDel)
   {
   WitPartApiCall theCall (this, partName);

   apiGet (selForDel, "selForDel", theCall.myPart ()->selForDel ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartShadowPrice (
      const char *         partName,
      const WitOVRealArg & shadowPrice)
   {
   WitPartApiCall theCall (this, partName);

   apiGet (shadowPrice, "shadowPrice", theCall.myPart ()->shadowPrice ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartSingleSource (
      const char * partName,
      WitBoolean * singleSource)
   {
   WitPartApiCall theCall (this, partName);

   apiGet (singleSource, "singleSource", theCall.myPart ()->singleSource ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartStockBounds (
      const char *         partName,
      const WitOVRealArg & hardLB,
      const WitOVRealArg & softLB,
      const WitOVRealArg & hardUB)
   {
   WitMaterialApiCall theCall (this, partName, "stockBounds");

   apiGet (hardLB, softLB, hardUB, theCall.myMaterial ()->stockBounds ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartStockCost (
      const char *         partName,
      const WitOVRealArg & stockCost)
   {
   WitMaterialApiCall theCall (this, partName, "stockCost");

   apiGet (
       stockCost,
      "stockCost",
      theCall.myMaterial ()->stockCost ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartStockSlbConIndex (
      const char * partName,
      const int    thePer,
      int *        stockSlbConIndex)
   {
   WitMaterialApiCall theCall (this, partName, "stockSlbConIndex");

   prepGetIndex (thePer);

   apiGet (
       stockSlbConIndex,
      "stockSlbConIndex",
      myDetOptImpMgr ()->
         myExtOptMgr ()->
            stockSlbConIndex (theCall.myMaterial (), thePer));
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartStockSlbvVarIndex (
      const char * partName,
      const int    thePer,
      int *        stockSlbvVarIndex)
   {
   WitMaterialApiCall theCall (this, partName, "stockSlbvVarIndex");

   prepGetIndex (thePer);

   apiGet (
       stockSlbvVarIndex,
      "stockSlbvVarIndex",
      myDetOptImpMgr ()->
         myExtOptMgr ()->
            stockSlbvVarIndex (theCall.myMaterial (), thePer));
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartStockVarIndex (
      const char * partName,
      const int    thePer,
      int *        stockVarIndex)
   {
   WitMaterialApiCall theCall (this, partName, "stockVarIndex");

   prepGetIndex (thePer);

   apiGet (
       stockVarIndex,
      "stockVarIndex",
      myDetOptImpMgr ()->
         myExtOptMgr ()->
            stockVarIndex (theCall.myMaterial (), thePer));
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartStockVol (
      const char *         partName,
      const WitOVRealArg & stockVol)
   {
   WitMaterialApiCall theCall (this, partName, "stockVol");

   apiGet (stockVol, "stockVol", theCall.myMaterial ()->stockVol ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartSupplyVol (
      const char *         partName,
      const WitOVRealArg & supplyVol)
   {
   WitPartApiCall theCall (this, partName);

   apiGet (supplyVol, "supplyVol", theCall.myPart ()->supplyVol ());
   }

//------------------------------------------------------------------------------

void WitRun::witGetPartYield (
      const char * partName,
      int * *      yield)
   {
   myApiMgr_->startInnerFunc ();

   prepGet34Attr ("Part", "yield", "Operation", "yield");

   witGetOperationYield (partName, yield);
   }

//------------------------------------------------------------------------------

void WitRun::witSetPartAppData (
      const char * partName,
      void *       appData)
   {
   WitPartApiCall theCall (this, partName);

   theCall.setAppData (appData);
   }

//------------------------------------------------------------------------------

void WitRun::witSetPartAsapPipOrder (
      const char * partName,
      WitBoolean   asapPipOrder)
   {
   WitMaterialApiCall theCall (this, partName, "asapPipOrder");

   theCall.prepBool (    
                            "asapPipOrder", 
      theCall.myMaterial ()->asapPipOrder (), 
                             asapPipOrder);

   if (theCall.argsOK ())
      theCall.myMaterial ()->setAsapPipOrder (asaBool (asapPipOrder));
   }

//------------------------------------------------------------------------------

void WitRun::witSetPartBaSelSplit (
      const char * partName,
      WitBoolean   baSelSplit)
   {
   WitMaterialApiCall theCall (this, partName, "baSelSplit");

   theCall.prepBool (    
                            "baSelSplit", 
      theCall.myMaterial ()->baSelSplit (), 
                             baSelSplit);

   if (theCall.argsOK ())
      theCall.myMaterial ()->setBaSelSplit (asaBool (baSelSplit));
   }

//------------------------------------------------------------------------------

void WitRun::witSetPartBoundedLeadTimes (
      const char * partName,
      WitBoolean   boundedLeadTimes)
   {
   WitPartApiCall theCall (this, partName);

   theCall.prepBool (    
                        "boundedLeadTimes", 
      theCall.myPart ()->boundedLeadTimes (), 
                         boundedLeadTimes);

   if (theCall.argsOK ())
      theCall.myPart ()->setBoundedLeadTimes (asaBool (boundedLeadTimes));
   }

//------------------------------------------------------------------------------

void WitRun::witSetPartBuildAheadUB (
      const char * partName,
      const int *  buildAheadUB)
   {
   WitMaterialApiCall theCall (this, partName, "buildAheadUB");

   theCall.prepBuildAheadUB (buildAheadUB);

   if (theCall.argsOK ())
      theCall.myMaterial ()->setBuildAheadUB (buildAheadUB);
   }

//------------------------------------------------------------------------------

void WitRun::witSetPartBuildAheadLimit (
      const char * partName,
      int          buildAheadLimit)
   {
   WitTVec <int> & buildAheadUBVec = myApiMgr ()->reserveTempIntVec ();

   myApiMgr_->startInnerFunc ();

   if (myMsgFac ().mayIssue ("setVecAttrToScalMsg"))
      myMsgFac () (
         "setVecAttrToScalMsg",
         apiFuncName (),
         "part",
         "buildAheadLimit",
         "buildAheadUB");

   buildAheadUBVec = buildAheadLimit;

   witSetPartBuildAheadUB (partName, buildAheadUBVec.myCVec ());

   myApiMgr ()->releaseTempVecs ();
   }

//------------------------------------------------------------------------------

void WitRun::witSetPartBuildAsap (
      const char * partName,
      WitBoolean   buildAsap)
   {
   WitMaterialApiCall theCall (this, partName, "buildAsap");

   theCall.prepBa (
      "buildAsap", 
      theCall.myMaterial ()->buildAsap (), 
      buildAsap, 
      "buildNstn");

   if (theCall.argsOK ())
      theCall.myMaterial ()->setBuildAsap (asaBool (buildAsap));
   }

//------------------------------------------------------------------------------

void WitRun::witSetPartBuildNstn (
      const char * partName,
      WitBoolean   buildNstn)
   {
   WitMaterialApiCall theCall (this, partName, "buildNstn");

   theCall.prepBa (
      "buildNstn", 
      theCall.myMaterial ()->buildNstn (), 
      buildNstn, 
      "buildAsap");

   if (theCall.argsOK ())
      theCall.myMaterial ()->setBuildNstn (asaBool (buildNstn));
   }

//------------------------------------------------------------------------------

void WitRun::witSetPartCycleTime (
      const char *         partName,
      const WitIVRealArg & inpVal)
   {
   myApiMgr_->startInnerFunc ();

   if (wit34Compatible ())
      {
      myMsgFac () ("setting34AttributeWithCompatibilityCycleTime",
         apiFuncName ());

      witSetOperationCycleTime34 (partName, inpVal);
      }
   else
      myMsgFac () ("setting34AttributeWithoutCompatibilityCycleTime",
         apiFuncName ());
   }

//------------------------------------------------------------------------------

void WitRun::witSetPartIncLotSize (
      const char *         partName,
      const WitIVRealArg & incLotSize)
   {
   myApiMgr_->startInnerFunc ();

   prepSet34Attr ("Part", "incLotSize", "Operation", "incLotSize");

   witSetOperationIncLotSize (partName, incLotSize);
   }

//------------------------------------------------------------------------------

void WitRun::witSetPartMinLotSize (
      const char *         partName,
      const WitIVRealArg & minLotSize)
   {
   myApiMgr_->startInnerFunc ();

   prepSet34Attr ("Part", "minLotSize", "Operation", "minLotSize");

   witSetOperationMinLotSize (partName, minLotSize);
   }

//------------------------------------------------------------------------------

void WitRun::witSetPartObj1ProdCost (
      const char *         partName,
      const WitIVRealArg & obj1ProdCost)
   {
   myApiMgr_->startInnerFunc ();

   prepSet34Attr ("Part", "obj1ProdCost", "Operation", "obj1ExecCost");

   witSetOperationObj1ExecCost (partName, obj1ProdCost);
   }

//------------------------------------------------------------------------------

void WitRun::witSetPartObj1ScrapCost (
      const char *         partName,
      const WitIVRealArg & obj1ScrapCost)
   {
   myApiMgr_->startInnerFunc ();

   if (myMsgFac ().mayIssue ("setAttrNewNameMsg"))
      myMsgFac () (
         "setAttrNewNameMsg",
         apiFuncName (),
         "part",
         "obj1ScrapCost",
         "scrapCost");

   witSetPartScrapCost (partName, obj1ScrapCost);
   }

//------------------------------------------------------------------------------

void WitRun::witSetPartObj1StockCost (
      const char *         partName,
      const WitIVRealArg & obj1StockCost)
   {
   myApiMgr_->startInnerFunc ();

   if (myMsgFac ().mayIssue ("setAttrNewNameMsg"))
      myMsgFac () (
         "setAttrNewNameMsg",
         apiFuncName (),
         "part",
         "obj1StockCost",
         "stockCost");

   witSetPartStockCost (partName, obj1StockCost);
   }

//------------------------------------------------------------------------------

void WitRun::witSetPartObjectStage (
      const char * partName,
      int          objectStage)
   {
   WitPartApiCall theCall (this, partName);

   theCall.setObjectStage (objectStage);
   }

//------------------------------------------------------------------------------

void WitRun::witSetPartPartName (
      const char * partName,
      const char * newName)
   {
   WitPartApiCall theCall (this, partName);

   forbidNullArg (newName, "newName");

   if (isNullCstring (newName))
      myMsgFac () ("nullStringSmsg", "newName");

   if (myCompMgr ()->findPart (newName))
      myMsgFac () ("setNodeNameToDupSmsg",
         "witSetPartPartName",
         myMsgFac ().myFrag ("partFrag"),
         newName);

   theCall.prepString (
                        "partName",
      theCall.myPart ()->partName (),
                          newName);

   if (theCall.argsOK ())
      theCall.myPart ()->setPartName (newName);
   }

//------------------------------------------------------------------------------

void WitRun::witSetPartProdBounds (
    const char *         partName,
    const WitIVRealArg & hardLB,
    const WitIVRealArg & softLB,
    const WitIVRealArg & hardUB)
   {
   myApiMgr_->startInnerFunc ();

   prepSet34Attr ("Part", "prodBounds", "Operation", "execBounds");

   witSetOperationExecBounds (partName, hardLB, softLB, hardUB);
   }

//------------------------------------------------------------------------------
// prodVol, an attribute which has a new meaning with Wit V4.0.
// Wit V3.4 provides set and get functions.
// Wit V4.0 only provides a get function.
//------------------------------------------------------------------------------

void WitRun::witSetPartProdVol (
      const char *         partName,
      const WitIVRealArg & prodVol)
   {
   myApiMgr_->startInnerFunc ();

   prepSet34Attr ("Part", "prodVol", "Operation", "execVol");

   witSetOperationExecVol (partName, prodVol);
   }

//------------------------------------------------------------------------------

void WitRun::witSetPartPropRouting (
      const char * partName,
      WitBoolean   propRouting)
   {
   WitTVec <WitBoolean> & propRtgVec = myApiMgr ()->reserveTempBooleanVec ();

   myApiMgr_->startInnerFunc ();

   if (myMsgFac ().mayIssue ("setVecAttrToScalMsg"))
      myMsgFac () (
         "setVecAttrToScalMsg",
         apiFuncName (),
         "part",
         "propRouting",
         "propRtg");

   propRtgVec = propRouting;

   witSetPartPropRtg (partName, propRtgVec.myCVec ());

   myApiMgr ()->releaseTempVecs ();
   }

//------------------------------------------------------------------------------

void WitRun::witSetPartPropRtg (
      const char *       partName,
      const WitBoolean * propRtg)
   {
   WitPartApiCall theCall (this, partName);

   theCall.prepBoolVec ("propRtg", propRtg);

   if (theCall.argsOK ())
      theCall.myPart ()->setPropRtg (propRtg);
   }

//------------------------------------------------------------------------------

void WitRun::witSetPartScrapAllowed (
      const char * partName, WitBoolean scrapAllowed)
   {
   WitPartApiCall theCall (this, partName);

   theCall.prepBool (    
                        "scrapAllowed", 
      theCall.myPart ()->scrapAllowed (), 
                         scrapAllowed);

   if (theCall.argsOK ())
      theCall.myPart ()->setScrapAllowed (asaBool (scrapAllowed));
   }

//------------------------------------------------------------------------------

void WitRun::witSetPartScrapCost (
      const char *         partName,
      const WitIVRealArg & scrapCost)
   {
   WitPeriod thePer;

   WitPartApiCall theCall (this, partName);

   theCall.prepDblVec ("scrapCost", scrapCost);

   if (theCall.argsOK ())
      {
      forEachPeriod (thePer, myProblem ())
         if (scrapCost[thePer] < 0.0)
            myMsgFac () (
               "negPartCostWmsg",
               "scrapCost",
               partName,
               scrapCost[thePer],
               thePer);

      theCall.myPart ()->setScrapCost (scrapCost);
      }
   }

//------------------------------------------------------------------------------

void WitRun::witSetPartSelForDel (
      const char * partName,
      WitBoolean   selForDel)
   {
   WitPartApiCall theCall (this, partName);

   theCall.setSelForDel (selForDel);
   }

//------------------------------------------------------------------------------

void WitRun::witSetPartSingleSource (
      const char * partName, WitBoolean singleSource)
   {
   WitPartApiCall theCall (this, partName);

   theCall.prepBool (    
                        "singleSource", 
      theCall.myPart ()->singleSource (), 
                         singleSource);

   if (theCall.argsOK ())
      theCall.myPart ()->setSingleSource (asaBool (singleSource));
   }

//------------------------------------------------------------------------------

void WitRun::witSetPartStockBounds (
      const char *         partName,
      const WitIVRealArg & hardLB,
      const WitIVRealArg & softLB,
      const WitIVRealArg & hardUB)
   {
   WitMaterialApiCall theCall (this, partName, "stockBounds");

   theCall.setBoundSet (
      theCall.myMaterial ()->stockBounds (), 
      hardLB, 
      softLB, 
      hardUB);
   }

//------------------------------------------------------------------------------

void WitRun::witSetPartStockCost (
      const char *         partName,
      const WitIVRealArg & stockCost)
   {
   WitMaterialApiCall theCall (this, partName, "stockCost");
   WitPeriod          thePer;

   theCall.prepDblVec ("stockCost", stockCost);

   if (theCall.argsOK ())
      {
      forEachPeriod (thePer, myProblem ())
         if (stockCost[thePer] < 0.0)
            myMsgFac () (
               "negPartCostWmsg",
               "stockCost",
               partName,
               stockCost[thePer],
               thePer);

      theCall.myMaterial ()->setStockCost (stockCost);
      }
   }

//------------------------------------------------------------------------------

void WitRun::witSetPartSupplyVol (
      const char *         partName,
      const WitIVRealArg & supplyVol)
   {
   WitPartApiCall theCall (this, partName);

   theCall.prepNonNegDblVec ("supplyVol", supplyVol);

   if (theCall.argsOK ())
      theCall.myPart ()->setSupplyVol (supplyVol);
   }

//------------------------------------------------------------------------------

void WitRun::witSetPartYield (
      const char * partName,
      const int *  yield)
   {
   myApiMgr_->startInnerFunc ();

   prepSet34Attr ("Part", "yield", "Operation", "yield");

   witSetOperationYield (partName, yield);
   }

//------------------------------------------------------------------------------

WitPart * WitRun::addPart34 (const char * partName, WitAttr partCategory)
   {
   if (partCategory == WitPRODUCT)
      {
      myMsgFac () ("addProductPartWith34Compatability", apiFuncName ());

      addPartPost34 (partName, WitMATERIAL);

      witAddOperation (partName);

      witAddBopEntry (partName, partName);
      }
   else if (partCategory == WitRAW)
      {
      myMsgFac () ("addRawPartWith34Compatability", apiFuncName ());

      addPartPost34 (partName, WitMATERIAL);
      }
   else if (partCategory == WitCAPACITY)
      addPartPost34 (partName, WitCAPACITY);

   else if (partCategory == WitMATERIAL)
      addPartPost34 (partName, WitMATERIAL);

   else
      myMsgFac () ("unknownPartCategorySmsg", partCategory);

   return getPartP (partName);
   }

//------------------------------------------------------------------------------

WitPart * WitRun::addPartPost34 (const char * partName, WitAttr partCategory)
   {
   if (myMsgFac ().mayIssue ("partNameTypeApMsg"))
      myMsgFac () ("partNameTypeApMsg", partName, partCategory);

   if    (partCategory != WitMATERIAL)
      if (partCategory != WitCAPACITY)
         myMsgFac () ("unknownPartCategorySmsg", partCategory);

   if (isNullCstring (partName))
      myMsgFac () ("nullStringSmsg", "partName");

   if (myCompMgr ()->findPart (partName))
      myMsgFac () ("dupPartApMsg", partName);

   addingDelComp ("partFrag");

   checkNPeriodsWasSet ();

   if (partCategory == WitCAPACITY)
      return new WitCapacity (partName, myCompMgr ());
   else
      return new WitMaterial (partName, myCompMgr ());
   }

//------------------------------------------------------------------------------
// getPartP
// Return pointer to part.  If not defined issue message.
//------------------------------------------------------------------------------

WitPart * WitRun::getPartP (const char * partName)
   {
   WitPart * partP;

   partP = myCompMgr ()->findPart (partName);

   if (partP == NULL)
      myMsgFac () ("undefinedPartMsg", partName);

   return partP;
   }
