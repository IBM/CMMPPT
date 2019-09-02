//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "WitPart.C"
//
// Contains the implementation of class Part.
//------------------------------------------------------------------------------

#include <Material.h>
#include <Demand.h>
#include <SubEntry.h>
#include <BopEntry.h>
#include <GlobalComp.h>
#include <BoundSet.h>
#include <CompMgr.h>
#include <DataWrit.h>
#include <Pre.h>
#include <Post.h>
#include <FSS.h>
#include <DetOptImpMgr.h>
#include <OrigMrp.h>
#include <StochImpMgr.h>
#include <OptComp.h>
#include <wit.h>
#include <MsgFrag.h>
#include <MsgFac.h>
#include <IVRealArg.h>

//------------------------------------------------------------------------------
// Implementation of class Part.
//------------------------------------------------------------------------------

WitPart::~WitPart ()
   {
   }

//------------------------------------------------------------------------------

WitBopEntry * WitPart::expBopEntry (WitPeriod thePer)
   {
   witAssert (explodeable (thePer));

   return expBopEntry_[thePer];
   }

//------------------------------------------------------------------------------

int WitPart::nInstances (WitProblem * theProblem)
   {
   return theProblem->myCompMgr ()->nParts ();
   }

//------------------------------------------------------------------------------
// "Set" functions.
//------------------------------------------------------------------------------

void WitPart::setPartName (const char * theName)
   {
   myCompMgr ()->setPartName (this, theName);
   }

//------------------------------------------------------------------------------

void WitPart::setSupplyVol (const WitIVRealArg & theValue)
   {
   witAssert (theValue != NULL);

   witAssert (theValue.isNonNeg (nPeriods ()));

   prepSetScenSpecAttr ("supplyVol");

   supplyVol_ = theValue;
   }

//------------------------------------------------------------------------------

void WitPart::setSingleSource (bool theValue)
   {
   prepSetUnpostAttr ();

   singleSource_ = theValue;
   }

//------------------------------------------------------------------------------

void WitPart::setPropRtg (const WitBoolean * theValue)
   {
   witAssert (theValue != NULL);

   prepSetUnpostAttr ();

   copy (propRtg_, theValue);
   }

//------------------------------------------------------------------------------

void WitPart::setBoundedLeadTimes (bool theValue)
   {
   prepSetUnpostAttr ();

   boundedLeadTimes_ = theValue;
   }

//------------------------------------------------------------------------------

void WitPart::setScrapAllowed (bool theValue)
   {
   prepSetUnpreAttr ();

   scrapAllowed_ = theValue;
   }

//------------------------------------------------------------------------------

void WitPart::setScrapCost (const WitIVRealArg & theValue)
   {
   witAssert (theValue != NULL);

   prepSetScenSpecAttr ("scrapCost");

   scrapCost_.curDblFlexVec () = theValue;
   }

//------------------------------------------------------------------------------
// Other functions.
//------------------------------------------------------------------------------

void WitPart::storePreSortedPartIndex (int theValue)
   {
   witAssert (myPreprocessor ()->preprocessing ());

   sortedPartIndex_ = theValue;
   }

//------------------------------------------------------------------------------

void WitPart::storePreBelowList (const WitPtrVec <WitPart> & thePtrVec)
   {
   witAssert (myPreprocessor ()->preprocessing ());

   belowList_.resize (thePtrVec.length ());

   belowList_ = thePtrVec;
   }

//------------------------------------------------------------------------------

void WitPart::storePreExpPerGaps (bool theValue)
   {
   witAssert (myPreprocessor ()->preprocessing ());

   expPerGaps_ = theValue;
   }

//------------------------------------------------------------------------------

void WitPart::storePreProducingBopEntries (
      const WitList <WitBopEntry> & theList)
   {
   witAssert (myPreprocessor ()->preprocessing ());

   producingBopEntries_ = theList;
   }

//------------------------------------------------------------------------------

void WitPart::storePreExpBopEntry (const WitPtrTVec <WitBopEntry> & thePtrTVec)
   {
   witAssert (myPreprocessor ()->preprocessing ());
   
   expBopEntry_.allocate (myProblem ());

   expBopEntry_ = thePtrTVec;
   }

//------------------------------------------------------------------------------

void WitPart::storePostConsVol (const WitTVec <double> & theTVec)
   {
   witAssert (myPostprocessor ()->postprocessing ());

   consVol_ = theTVec;
   }

//------------------------------------------------------------------------------

void WitPart::storePostProdVol (const WitTVec <double> & theTVec)
   {
   witAssert (myPostprocessor ()->postprocessing ());

   prodVol_ = theTVec;
   }

//------------------------------------------------------------------------------

void WitPart::storePostScrapVol (const WitTVec <double> & theTVec)
   {
   witAssert (myPostprocessor ()->postprocessing ());

   scrapVol_ = theTVec;
   }

//------------------------------------------------------------------------------

void WitPart::storePostResidualVol (const WitTVec <double> & theTVec)
   {
   witAssert (myPostprocessor ()->postprocessing ());

   residualVol_ = theTVec;
   }

//------------------------------------------------------------------------------

void WitPart::storeOptScrapVol (WitPeriod thePer, double theValue)
   {
   witAssert (myDetOptImpMgr ()->uploadingSoln ());

   scrapVol_.elemRef (thePer) = theValue;
   }

//------------------------------------------------------------------------------

void WitPart::storeOptShadowPrice (WitPeriod thePer, double theValue)
   {
   witAssert (myDetOptImpMgr ()->uploadingSoln ());

   shadowPrice_.elemRef (thePer) = theValue;
   }

//------------------------------------------------------------------------------

void WitPart::storeStochSupplyVol (const WitDblFlexVec & theFlexVec)
   {
   witAssert (myStochImpMgr ()->uploadingData ());

   supplyVol_ = theFlexVec;
   }

//------------------------------------------------------------------------------

void WitPart::storeStochScrapCost (const WitDblFlexVec & theFlexVec)
   {
   witAssert (myStochImpMgr ()->uploadingData ());

   scrapCost_.curDblFlexVec () = theFlexVec;
   }

//------------------------------------------------------------------------------

void WitPart::storeStochScrapVol (const WitDblFlexVec & theFlexVec)
   {
   witAssert (myStochImpMgr ()->uploadingData ());

   scrapVol_ = theFlexVec;
   }

//------------------------------------------------------------------------------

void WitPart::storeMrpReqVol (const WitTVec <double> & theTVec)
   {
   witAssert (myOrigMrpExp ()->uploadingSoln ());

   reqVol_ = theTVec;
   }

//------------------------------------------------------------------------------

void WitPart::storeMrpMrpConsVol (const WitTVec <double> & theTVec)
   {
   witAssert (myOrigMrpExp ()->uploadingSoln ());

   mrpConsVol_ = theTVec;
   }

//------------------------------------------------------------------------------

void WitPart::storeMrpMrpResidualVol (const WitTVec <double> & theTVec)
   {
   witAssert (myOrigMrpExp ()->uploadingSoln ());

   mrpResidualVol_ = theTVec;
   }

//------------------------------------------------------------------------------

void WitPart::storeFssFocShortageVol (const WitTVec <double> & theTVec)
   {
   witAssert (myFSS ()->uploadingSoln ());

   focShortageVol_ = theTVec;
   }

//------------------------------------------------------------------------------

void WitPart::insert (WitBomEntry * theBomEnt)
   {
   consumingBomEntries_.appendUniquely (theBomEnt);
   }

//------------------------------------------------------------------------------

void WitPart::insert (WitSubEntry * theSub)
   {
   consumingSubEntries_.appendUniquely (theSub);
   }

//------------------------------------------------------------------------------

void WitPart::insert (WitBopEntry * theBopEnt)
   {
   producingBopEntries_.appendUniquely (theBopEnt);
   }

//------------------------------------------------------------------------------

void WitPart::insert (WitDemand * theDemand)
   {
   myDemands_.appendUniquely (theDemand);
   }

//------------------------------------------------------------------------------

bool WitPart::inserted ()
   {
   return (myCompMgr ()->findPart (partName ().myCstring ()) == this);
   }

//------------------------------------------------------------------------------

void WitPart::writeMultiObjVecs (WitObjective * theObj)
   {
   scrapCost_.writeDataMulti ("scrapCost", this, theObj);
   }

//------------------------------------------------------------------------------

void WitPart::unpreprocess ()
   {
   expBopEntry_.clear ();

   belowList_.resize (0);
   }

//------------------------------------------------------------------------------

void WitPart::clearOptSoln  ()
   {                              
   witAssert (myDetOptImpMgr ()->clearingSoln ());

   scrapVol_    = 0.0;
   shadowPrice_ = 0.0;
   }

//------------------------------------------------------------------------------

void WitPart::clearStochSoln ()
   {
   WitComponent::clearStochSoln ();

   prodVol_     = 0.0;
   consVol_     = 0.0;
   scrapVol_    = 0.0;
   residualVol_ = 0.0;
   }

//------------------------------------------------------------------------------

WitPart * WitPart::thisPart ()
   {
   return this;
   }

//------------------------------------------------------------------------------

void WitPart::identifyInstance ()
   {
   if (myMsgFac ().mayIssue ("partInstIDMsg"))
      myMsgFac () ("partInstIDMsg", partName ());
   }

//------------------------------------------------------------------------------

void WitPart::writeSetClause ()
   {
   myDataWriter ()->writeFirstKeyWord ("set");
   myDataWriter ()->writeKeyWord      ("part");
   myDataWriter ()->writeStringValue  (partName ());
   }

//------------------------------------------------------------------------------

WitMsgFrag WitPart::objTypeMsgFrag ()
   {
   return myMsgFac ().myFrag ("partFrag");
   }

//------------------------------------------------------------------------------

WitAttr WitPart::apiObjItrState ()
   {
   return WitAT_PART;
   }

//------------------------------------------------------------------------------

int WitPart::nodeIndex () const
   {
   return mappingIndex_;
   }

//------------------------------------------------------------------------------

void WitPart::display ()
   {
   WitDemand   *          theDemand;
   WitBomEntry *          theBomEnt;
   WitSubEntry *          theSub;
   WitBopEntry *          theBopEnt;
   WitPart     *          thePart;
   WitPeriod              thePer;
   WitPtrVecItr <WitPart> theItr;

   myMsgFac () ("partIdDdMsg", partName ());

   myMsgFac () ("partScalarsDdMsg",
      categoryFrag (),
      mappingIndex_,
      singleSource_,
      boundedLeadTimes_,
      scrapAllowed_);

   if (myPreprocessor ()->preprocessed ())
      myMsgFac () ("partPreDdMsg", sortedPartIndex_, expPerGaps_);

   myProblem ()->display ("extSupplyVolDdMsg", supplyVol_);
   myProblem ()->display ("scrapCostDdMsg",    scrapCost_);
   myProblem ()->display ("propRtgDdMsg",      propRtg_);

   if (not myDemands_.isEmpty ())
      {
      myMsgFac () ("myDemandsHeadingDdMsg");

      forEachEl (theDemand, myDemands_)
         myMsgFac () ("myDemandsDataDdMsg", theDemand->demandName ());
      }

   if (not consumingBomEntries_.isEmpty ())
      {
      myMsgFac () ("consBomHeadingDdMsg");

      forEachEl (theBomEnt, consumingBomEntries_)
         myMsgFac () ("consBomDataDdMsg",
            theBomEnt->myOperationName (), theBomEnt->localIndex ());
      }

   if (not consumingSubEntries_.isEmpty ())
      {
      myMsgFac () ("consSubsHeadingDdMsg");

      forEachEl (theSub, consumingSubEntries_)
         myMsgFac () ("consSubsDataDdMsg",
            theSub->myOperationName (),
            theSub->myBomEnt ()->myPartName (),
            theSub->myBomEntIndex (),
            theSub->localIndex ());
      }

   if (not producingBopEntries_.isEmpty ())
      {
      myMsgFac () ("prodBopHeadingDdMsg");

      forEachEl (theBopEnt, producingBopEntries_)
         myMsgFac () ("prodBopDataDdMsg",
            theBopEnt->myOperationName (),
            theBopEnt->localIndex ());
      }

   if (myPreprocessor ()->preprocessed ())
      if (not producingBopEntries_.isEmpty ())
         {
         myMsgFac () ("expBopHeadDdMsg");

         forEachPeriod (thePer, myProblem ())
            if (explodeable (thePer))
               myMsgFac () ("expBopDataDdMsg",
                  thePer,
                  expBopEntry (thePer)->myOperationName (),
                  expBopEntry (thePer)->localIndex ());
         }

   if (myPreprocessor ()->preprocessed ())
      {
      myMsgFac () ("belowHeadingDdMsg");

      belowList_.attachItr (theItr);

      while (theItr.advance (thePart))
         myMsgFac () ("belowDataDdMsg", thePart->partName ());
      }

   WitDelComp::display ();
   }

//------------------------------------------------------------------------------

bool WitPart::explodeable (WitPeriod thePer)
   {
   return expBopEntry_.isAllocated () and expBopEntry_[thePer] != NULL;
   }

//------------------------------------------------------------------------------

double WitPart::net (
      WitPeriod          reqPer,
      double             reqQty,
      WitTVec <double> & supply)
   {
   WitPeriod supplyPer;
   double    qty;

   qty = reqQty;

   for (supplyPer = reqPer; supplyPer >= 0; -- supplyPer)
      {
      if (supplyPer < reqPer)
         if (not canStock (supplyPer))
            break;

      if (supply[supplyPer] > NET_TOL)
         {
         WitNonClass::net (qty, supply[supplyPer]);

         if (qty <= NET_TOL)
            return 0.0;
         }
      }

   return qty;
   }

//------------------------------------------------------------------------------

void WitPart::writeSoln ()
   {
   WitPeriod     thePer;
   WitMaterial * theMat;
   WitDemand *   theDemand;
   double        shipVolVal;

   fprintf (solnFile (), "\nPart %s:\n", partName ().myCstring ());

   fprintf (solnFile (),
              "Period"
       "    SupplyVol"
       "      ProdVol"
       "      ConsVol"
       "      ShipVol"
       "     StockVol"
       "     ScrapVol"
       "  ResidualVol");

   if (myGlobalComp ()->printExcessVol ())
      fprintf (solnFile (), "    ExcessVol");

   else if (fssComputed ())
      fprintf (solnFile (), "  ShortageVol");

   fprintf (solnFile (), "\n");

   forEachPeriod (thePer, myProblem ())
      {
      shipVolVal = 0.0;

      forEachEl (theDemand, myDemands ())
         shipVolVal += theDemand->shipVol ()[thePer];

      fprintf (solnFile (), "%6d", thePer);

      writeSolnValue (supplyVol_[thePer]);
      writeSolnValue (prodVol_  [thePer]);
      writeSolnValue (consVol_  [thePer]);
      writeSolnValue (shipVolVal);

      theMat = thisMat ();

      writeSolnValue (
         (theMat != NULL)?
            theMat->stockVol ()[thePer]:
            0.0);

      writeSolnValue (scrapVol_   [thePer]);
      writeSolnValue (residualVol_[thePer]);

      if (myGlobalComp ()->printExcessVol ())
         writeSolnValue (myPostprocessor ()->compExcessVol (this, thePer));

      else if (fssComputed ())
         writeSolnValue (focShortageVol_[thePer]);

      fprintf (solnFile (), "\n");
      }
   }

//------------------------------------------------------------------------------

void WitPart::writeDualSoln ()
   {
   WitPeriod thePer;

   witAssert (myOptComp ()->compPrices ());

   fprintf (solnFile (), "\nPart %s:\n", partName ().myCstring ());

   fprintf (solnFile (),"Period  Shadow Price\n");

   forEachPeriod (thePer, myProblem ())
      {
      fprintf (solnFile (), "%6d ", thePer);

      writeSolnValue (shadowPrice_[thePer]);

      fprintf (solnFile (), "\n");
      }
   }

//------------------------------------------------------------------------------

void WitPart::writeMrpSoln ()
   {
   WitPeriod thePer;

   fprintf (solnFile (), "\nPart %s:\n", partName ().myCstring ());

   fprintf (solnFile (),
              "Period"
       "    SupplyVol"
       "   MrpConsVol"
       "       ReqVol"
       "  MrpResidualVol\n");

   forEachPeriod (thePer, myProblem ())
      {
      fprintf (solnFile (), "%6d", thePer);

      writeSolnValue (supplyVol  ()[thePer]);
      writeSolnValue (mrpConsVol ()[thePer]);
      writeSolnValue (reqVol     ()[thePer]);

      fprintf (solnFile (), "   ");

      writeSolnValue (mrpResidualVol ()[thePer]);

      fprintf (solnFile (), "\n");
      }
   }

//------------------------------------------------------------------------------

void WitPart::resetShadowPrice ()
   {
   shadowPrice_ = 0.0;
   }

//------------------------------------------------------------------------------

WitPart::WitPart (const char * theName, WitCompMgr * theCompMgr):
      WitNode              (theName, theCompMgr),

      mappingIndex_        (myCompMgr ()->nParts ()),
      myDemands_           (),
      consumingBomEntries_ (),
      consumingSubEntries_ (),
      producingBopEntries_ (),
      supplyVol_           (myProblem (), defSupplyVol        ()),
      singleSource_        (              defSingleSource     ()),
      propRtg_             (myProblem (), defPropRtg          ()),
      boundedLeadTimes_    (              defBoundedLeadTimes ()),
      scrapAllowed_        (              defScrapAllowed     ()),
      scrapCost_           (myProblem (), defScrapCost        ()),
      sortedPartIndex_     (-1),
      expBopEntry_         (),
      belowList_           (),
      expPerGaps_          (false),
      shadowPrice_         (myProblem (), 0.0),
      consVol_             (myProblem (), 0.0),
      prodVol_             (myProblem (), 0.0),
      scrapVol_            (myProblem (), 0.0),
      residualVol_         (myProblem (), 0.0),
      reqVol_              (myProblem (), 0.0),
      mrpConsVol_          (myProblem (), 0.0),
      mrpResidualVol_      (myProblem (), 0.0),
      focShortageVol_      (myProblem (), 0.0)
   {
   }

//------------------------------------------------------------------------------

void WitPart::recountPart ()
   {
   mappingIndex_ = myCompMgr ()->nParts ();
   }

//------------------------------------------------------------------------------

void WitPart::writeDataAttrs ()
   {
   WitNode::writeDataAttrs ();

   myDataWriter ()->writeVector (
        "supplyVol",
         supplyVol_,
      defSupplyVol ());

   myDataWriter ()->writeBool (
        "singleSource",
         singleSource_,
      defSingleSource ());

   myDataWriter ()->writeVector (
        "propRtg",
         propRtg_,
      defPropRtg ());

   myDataWriter ()->writeBool (
        "boundedLeadTimes",
         boundedLeadTimes_,
      defBoundedLeadTimes ());

   myDataWriter ()->writeBool (
        "scrapAllowed",
         scrapAllowed_,
      defScrapAllowed ());

   myDataWriter ()->writeObjVec (
        "scrapCost",
         scrapCost_,
      defScrapCost ());
   }

//------------------------------------------------------------------------------

void WitPart::copyAttrsFrom (WitPart * thePart)
   {
   WitNode::copyAttrsFrom (thePart);

   supplyVol_        = thePart->supplyVol_;
   singleSource_     = thePart->singleSource_;
   propRtg_          = thePart->propRtg_;
   boundedLeadTimes_ = thePart->boundedLeadTimes_;
   scrapAllowed_     = thePart->scrapAllowed_;
   scrapCost_        = thePart->scrapCost_;
   }

//------------------------------------------------------------------------------

void WitPart::writeDataID ()
   {
   myDataWriter ()->writeFirstKeyWord ("add");
   myDataWriter ()->writeKeyWord      ("part");
   myDataWriter ()->writeStringValue  (partName ());
   myDataWriter ()->writeKeyWord      (categoryKeyWord ());
   }

//------------------------------------------------------------------------------

void WitPart::purgeCompListsSpec ()
   {
   purgeSelComps (consumingBomEntries_);
   purgeSelComps (consumingSubEntries_);
   purgeSelComps (producingBopEntries_);
   purgeSelComps (myDemands_);
   }

//------------------------------------------------------------------------------

void WitPart::clearDepListsSpec ()
   {
   consumingBomEntries_.clear ();
   consumingSubEntries_.clear ();
   producingBopEntries_.clear ();
   myDemands_          .clear ();
   }
