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
// Source file: "MatCap.C"
//
// Contains the implementation of the following classes:
//
//    Material
//    Capacity
//------------------------------------------------------------------------------

#include <Material.h>
#include <Capacity.h>
#include <GlobalComp.h>
#include <StockBS.h>
#include <CompMgr.h>
#include <DataWrit.h>
#include <wit.h>
#include <Pre.h>
#include <Post.h>
#include <DetOptImpMgr.h>
#include <StochImpMgr.h>
#include <OptComp.h>
#include <IVRealArg.h>
#include <MsgFac.h>
#include <MsgFrag.h>

//------------------------------------------------------------------------------
// Implementation of class Material.
//------------------------------------------------------------------------------

WitMaterial::WitMaterial (const char * theName, WitCompMgr * theCompMgr):

      WitPart       (theName, theCompMgr),

      mappingIndex_ (myCompMgr ()->nMaterials ()),
      stockCost_    (myProblem (), defStockCost ()),
      stockBounds_  (NULL),
      myBaDir_      (noBa),
      buildAheadUB_ (myProblem (), defBuildAheadUB (myProblem ())),
      baSelSplit_   (defBaSelSplit ()),
      asapPipOrder_ (defAsapPipOrder ()),
      mandEC_       (myProblem (), false),
      stockVol_     (myProblem (), 0.0)
   {
   stockBounds_ = new WitStockBoundSet (this);

   myCompMgr ()->insert (this);
   }

//------------------------------------------------------------------------------

WitMaterial::~WitMaterial ()
   {
   delete stockBounds_;
   }

//------------------------------------------------------------------------------

int WitMaterial::nInstances (WitProblem * theProblem)
   {
   return theProblem->myCompMgr ()->nMaterials ();
   }

//------------------------------------------------------------------------------

int WitMaterial::defBuildAheadUB (WitProblem * theProblem)
   {
   return theProblem->lastPeriod ();
   }

//------------------------------------------------------------------------------
// "Set" functions.
//------------------------------------------------------------------------------

void WitMaterial::setStockCost (const WitIVRealArg & theValue)
   {
   witAssert (theValue != NULL);

   prepSetScenSpecAttr ("stockCost");

   stockCost_.curDblFlexVec () = theValue;
   }

//------------------------------------------------------------------------------

void WitMaterial::setBuildNstn (bool theValue)
   {
   prepSetUnpostAttr ();

   if (theValue != buildNstn ())
      myBaDir_ = theValue? nstn: noBa;
   }

//------------------------------------------------------------------------------

void WitMaterial::setBuildAsap (bool theValue)
   {
   prepSetUnpostAttr ();

   if (theValue != buildAsap ())
      myBaDir_ = theValue? asap: noBa;
   }

//------------------------------------------------------------------------------

void WitMaterial::setBuildAheadUB (const int * theValue)
   {
   WitPeriod thePer;

   witAssert (theValue != NULL);

   if (DEBUG_MODE)
      forEachPeriod (thePer, myProblem ())
         {
         witAssert    (theValue[thePer] >= 0);
         witAssert    (theValue[thePer] <  nPeriods ());

         if (thePer <= nPeriods () - 2)
            witAssert (theValue[thePer + 1] <= theValue[thePer] + 1);
         }

   prepSetUnpostAttr ();

   buildAheadUB_ = theValue;
   }

//------------------------------------------------------------------------------

void WitMaterial::setBaSelSplit (bool theValue)
   {
   prepSetUnpostAttr ();

   baSelSplit_ = theValue;
   }

//------------------------------------------------------------------------------

void WitMaterial::setAsapPipOrder (bool theValue)
   {
   prepSetUnpostAttr ();

   asapPipOrder_ = theValue;
   }

//------------------------------------------------------------------------------

void WitMaterial::storePreMandEC (WitPeriod thePer, bool theValue)
   {
   witAssert (myPreprocessor ()->preprocessing ());

   mandEC_.elemRef (thePer) = theValue;
   }

//------------------------------------------------------------------------------

void WitMaterial::storePostStockVol (const WitTVec <double> & theTVec)
   {
   witAssert (myPostprocessor ()->postprocessing ());

   stockVol_ = theTVec;
   }

//------------------------------------------------------------------------------

void WitMaterial::storeOptStockVol (WitPeriod thePer, double theValue)
   {
   witAssert (myDetOptImpMgr ()->uploadingSoln ());

   stockVol_.elemRef (thePer) = theValue;
   }

//------------------------------------------------------------------------------

void WitMaterial::storeStochStockCost (const WitDblFlexVec & theFlexVec)
   {
   witAssert (myStochImpMgr ()->uploadingData ());

   stockCost_.curDblFlexVec () = theFlexVec;
   }

//------------------------------------------------------------------------------

void WitMaterial::storeStochStockVol (const WitDblFlexVec & theFlexVec)
   {
   witAssert (myStochImpMgr ()->uploadingData ());

   stockVol_ = theFlexVec;
   }

//------------------------------------------------------------------------------
// Other functions.
//------------------------------------------------------------------------------

void WitMaterial::writeMultiObjVecs (WitObjective * theObj)
   {
   WitPart::writeMultiObjVecs (theObj);

   stockCost_.writeDataMulti ("stockCost", this, theObj);
   }

//------------------------------------------------------------------------------

void WitMaterial::copySelfInto (WitCompMgr * theCompMgr)
   {
   WitMaterial * theMat;

   theMat = new WitMaterial (partName ().myCstring (), theCompMgr);

   theMat->copyAttrsFrom (this);
   }

//------------------------------------------------------------------------------

void WitMaterial::recount ()
   {
   recountPart ();

   mappingIndex_ = myCompMgr ()->nMaterials ();

   myCompMgr ()->recountMaterial (this);
   }

//------------------------------------------------------------------------------

void WitMaterial::unpreprocess ()
   {
   mandEC_ = false;

   WitPart::unpreprocess ();
   }

//------------------------------------------------------------------------------

void WitMaterial::clearOptSoln  ()
   {                              
   witAssert (myDetOptImpMgr ()->clearingSoln ());

   WitPart::clearOptSoln  ();

   stockVol_ = 0.0;
   }

//------------------------------------------------------------------------------

void WitMaterial::clearStochSoln ()
   {
   WitPart::clearStochSoln ();

   stockVol_ = 0.0;
   }

//------------------------------------------------------------------------------

WitMaterial * WitMaterial::thisMat ()
   {
   return this;
   }

//------------------------------------------------------------------------------

WitMsgFrag WitMaterial::categoryFrag ()
   {
   WitMsgFragID theFragID;

   theFragID =
      not wit34Compatible ()?            "materialFrag":
      producingBopEntries ().isEmpty ()? "rawMaterialFrag":
                                         "productFrag";

   return myMsgFac ().myFrag (theFragID);
   }

//------------------------------------------------------------------------------

WitAttr WitMaterial::apiCategory ()
   {
   if (not wit34Compatible ())
      return WitMATERIAL;

   if (producingBopEntries ().isEmpty ())
      return WitRAW;

   return WitPRODUCT;
   }

//------------------------------------------------------------------------------

bool WitMaterial::canStock (WitPeriod thePer)
   {
   return not mandEC ()[thePer];
   }

//------------------------------------------------------------------------------

void WitMaterial::display ()
   {
   WitPart::display ();

   myMsgFac () ("materialDdMsg", 
      partName (), 
      mappingIndex_,
      nstnBaNeeded (),
      buildAsap (),
      baSelSplit_,
      asapPipOrder_);

   myProblem ()->display ("buildAheadUBDdMsg", buildAheadUB_);
   myProblem ()->display ("stockCostDdMsg",    stockCost_);

   stockBounds_->display ();

   if (myPreprocessor ()->preprocessed ())
      myProblem ()->display ("mecDdMsg", mandEC_);
   }

//------------------------------------------------------------------------------

void WitMaterial::copyAttrsFrom (WitMaterial * theMat)
   {
   WitPart::copyAttrsFrom (theMat);

   stockCost_      = theMat->stockCost_;
   myBaDir_        = theMat->myBaDir_;
   buildAheadUB_   = theMat->buildAheadUB_;
   baSelSplit_     = theMat->baSelSplit_;
   asapPipOrder_   = theMat->asapPipOrder_;
   stockBounds_->
      copyAttrsFrom (theMat->stockBounds_);
   }

//------------------------------------------------------------------------------

bool WitMaterial::nstnBaNeeded ()
   {
   return
      buildNstn ()?                        true:
      buildAsap ()?                        false:
      myGlobalComp ()->localBuildAhead ()? true:
                                           false;
   }

//------------------------------------------------------------------------------
// rollBackScrap.
// Moves scrapping of theMat backward in time so all
// scrapping of theMat is done as early as possible. The volume of
// scrap, rollBackVol, that can be rolled back from period t+1 to period t is
// the minimum of the amount by which the stock volume in period t exceeds its
// soft lower bound (if any) and the scrap volume in period t+1.
//------------------------------------------------------------------------------

void WitMaterial::rollBackScrap (
      WitTVec <double> & theStockVol,
      WitTVec <double> & theScrapVol)
   {
   WitPeriod thePer;
   double    rollBackVol;

   for (thePer = lastPeriod () - 1; thePer >= 0; thePer --)
      {
      rollBackVol = theStockVol[thePer] - stockBounds_->softLB ()[thePer];

      setToMin (rollBackVol, theScrapVol[thePer + 1]);

      if (rollBackVol > FLOAT_EPSILON)
         {
         theScrapVol[thePer + 1] -= rollBackVol;
         theStockVol[thePer]     -= rollBackVol;
         theScrapVol[thePer]     += rollBackVol;
         }
      }
   }

//------------------------------------------------------------------------------

WitBoundSet * WitMaterial::myBoundSet ()
   {
   return stockBounds_;
   }

//------------------------------------------------------------------------------

const char * WitMaterial::categoryKeyWord ()
   {
   return "material";
   }

//------------------------------------------------------------------------------

void WitMaterial::writeDataAttrs ()
   {
   WitPart::writeDataAttrs ();

   myDataWriter ()->writeObjVec (
        "stockCost",
         stockCost_,
      defStockCost ());

   stockBounds_->writeData ();

   myDataWriter ()->writeBool ("buildNstn", buildNstn (), false);
   myDataWriter ()->writeBool ("buildAsap", buildAsap (), false);

   myDataWriter ()->writeVector (
      "buildAheadUB",
      buildAheadUB (),
      defBuildAheadUB (myProblem ()));

   myDataWriter ()->writeBool ("baSelSplit", baSelSplit (), defBaSelSplit ());

   myDataWriter ()->
      writeBool ("asapPipOrder", asapPipOrder (), defAsapPipOrder ());
   }

//------------------------------------------------------------------------------

WitPeriod WitMaterial::findExplosionPeriod (WitPeriod fillPer)
   {
   WitPeriod expPer;

   for (expPer = fillPer; expPer >= 0; expPer --)
      {
      if (expPer < fillPer)
         if (mandEC ()[expPer])
            break;

      if (explodeable (expPer))
         return expPer;
      }

   return fillPer;
   }

//------------------------------------------------------------------------------
// Implementation of class Capacity.
//------------------------------------------------------------------------------

WitCapacity::WitCapacity (const char * theName, WitCompMgr * theCompMgr):

      WitPart (theName, theCompMgr)
   {
   myCompMgr ()->insert (this);
   }

//------------------------------------------------------------------------------

WitCapacity::~WitCapacity ()
   {
   }

//------------------------------------------------------------------------------

void WitCapacity::copySelfInto (WitCompMgr * theCompMgr)
   {
   WitCapacity * theCap;

   theCap = new WitCapacity (partName ().myCstring (), theCompMgr);

   theCap->copyAttrsFrom (this);
   }

//------------------------------------------------------------------------------

void WitCapacity::recount ()
   {
   recountPart ();

   myCompMgr ()->recountCapacity (this);
   }

//------------------------------------------------------------------------------

WitCapacity * WitCapacity::thisCap ()
   {
   return this;
   }

//------------------------------------------------------------------------------

WitMsgFrag WitCapacity::categoryFrag ()
   {
   return myMsgFac ().myFrag ("capacityFrag");
   }

//------------------------------------------------------------------------------

WitAttr WitCapacity::apiCategory ()
   {
   return WitCAPACITY;
   }

//------------------------------------------------------------------------------

bool WitCapacity::canStock (WitPeriod)
   {
   return false;
   }

//------------------------------------------------------------------------------

void WitCapacity::copyAttrsFrom (WitCapacity * theCap)
   {
   WitPart::copyAttrsFrom (theCap);
   }

//------------------------------------------------------------------------------

const char * WitCapacity::categoryKeyWord ()
   {
   return "capacity";
   }

//------------------------------------------------------------------------------

void WitCapacity::writeDataAttrs ()
   {
   WitPart::writeDataAttrs ();
   }

//------------------------------------------------------------------------------

WitPeriod WitCapacity::findExplosionPeriod (WitPeriod)
   {
   stronglyAssert (false);

   return -1;
   }
