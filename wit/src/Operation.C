//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "Operation.C"
//
// Contains the implementation of class Operation.
//------------------------------------------------------------------------------

#include <Operation.h>
#include <GlobalComp.h>
#include <BomEntry.h>
#include <SubEntry.h>
#include <BopEntry.h>
#include <OptComp.h>
#include <ExecBS.h>
#include <CompMgr.h>
#include <OrigMrp.h>
#include <DataWrit.h>
#include <wit/src/Variant.h>
#include <Pre.h>
#include <FSS.h>
#include <HeurAllMgr.h>
#include <HeurModifier.h>
#include <OptStarter.h>
#include <DetOptImpMgr.h>
#include <StochImpMgr.h>
#include <IVRealArg.h>
#include <wit.h>
#include <MsgFrag.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class Operation.
//------------------------------------------------------------------------------

WitOperation::WitOperation (const char * theName, WitCompMgr * theCompMgr):

      WitNode           (theName, theCompMgr),
      mappingIndex_     (myCompMgr ()->nOperations ()),
      bom_              (),
      bop_              (),
      yieldRate_        (myProblem (), defYieldRate ()),
      minLotSize_       (myProblem (), defMinLotSize ()),
      incLotSize_       (myProblem (), defIncLotSize ()),
      twoLevelLotSizes_ (defTwoLevelLotSizes ()),
      lotSize2Thresh_   (myProblem (), defLotSize2Thresh ()),
      minLotSize2_      (myProblem (), defMinLotSize2 ()),
      incLotSize2_      (myProblem (), defIncLotSize2 ()),
      execPenalty_      (defExecPenalty ()),
      execCost_         (myProblem (), defExecCost ()),
      intExecVols_      (defIntExecVols ()),
      cycleTime34_      (myProblem (), defCycleTime34 ()),
      execVol_          (myProblem (), 0.0),
      execBounds_       (NULL),
      pipEnabled_       (defPipEnabled ()),
      pipRank_          (defPipRank ()),
      executable_       (myProblem (), false),
      mrpExecVol_       (myProblem (), 0.0),
      fssExecVol_       (myProblem (), 0.0)
   {
   execBounds_ = new WitExecBoundSet (this);

   myCompMgr ()->insert (this);
   }

//------------------------------------------------------------------------------

WitOperation::~WitOperation ()
   {
   delete execBounds_;
   }

//------------------------------------------------------------------------------

int WitOperation::nInstances (WitProblem * theProblem)
   {
   return theProblem->myCompMgr ()->nOperations ();
   }

//------------------------------------------------------------------------------
// "Set" functions.
//------------------------------------------------------------------------------

void WitOperation::setOperationName (const char * theName)
   {
   myCompMgr ()->setOperationName (this, theName);
   }

//------------------------------------------------------------------------------

void WitOperation::setYieldRate (const WitIVRealArg & theValue)
   {
   WitPeriod thePer;

   witAssert (theValue != NULL);

   if (DEBUG_MODE)
      forEachPeriod (thePer, myProblem ())
         witAssert (validYieldRate (theValue[thePer]));

   prepSetUnpreAttr ();

   yieldRate_ = theValue;
   }

//------------------------------------------------------------------------------

void WitOperation::setMinLotSize (const WitIVRealArg & theValue)
   {
   witAssert (theValue != NULL);

   witAssert (theValue.isNonNeg (nPeriods ()));

   prepSetUnpreAttr ();

   minLotSize_ = theValue;
   }

//------------------------------------------------------------------------------

void WitOperation::setIncLotSize (const WitIVRealArg & theValue)
   {
   witAssert (theValue != NULL);

   witAssert (theValue.isNonNeg (nPeriods ()));

   prepSetUnpreAttr ();

   incLotSize_ = theValue;
   }

//------------------------------------------------------------------------------

void WitOperation::setTwoLevelLotSizes (bool theValue)
   {
   prepSetUnpreAttr ();

   twoLevelLotSizes_ = theValue;
   }

//------------------------------------------------------------------------------

void WitOperation::setLotSize2Thresh (const WitIVRealArg & theValue)
   {
   witAssert (theValue != NULL);

   witAssert (theValue.isNonNeg (nPeriods ()));

   prepSetUnpreAttr ();

   lotSize2Thresh_ = theValue;
   }

//------------------------------------------------------------------------------

void WitOperation::setMinLotSize2 (const WitIVRealArg & theValue)
   {
   witAssert (theValue != NULL);

   witAssert (theValue.isNonNeg (nPeriods ()));

   prepSetUnpreAttr ();

   minLotSize2_ = theValue;
   }

//------------------------------------------------------------------------------

void WitOperation::setIncLotSize2 (const WitIVRealArg & theValue)
   {
   witAssert (theValue != NULL);

   witAssert (theValue.isNonNeg (nPeriods ()));

   prepSetUnpreAttr ();

   incLotSize2_ = theValue;
   }

//------------------------------------------------------------------------------

void WitOperation::setExecPenalty (double theValue)
   {
   witAssert (theValue >= 0.0);

   prepSetUnpostAttr ();

   execPenalty_ = theValue;
   }

//------------------------------------------------------------------------------

void WitOperation::setExecCost (const WitIVRealArg & theValue)
   {
   witAssert (theValue != NULL);

   prepSetScenSpecAttr ("execCost");

   execCost_.curDblFlexVec () = theValue;
   }

//------------------------------------------------------------------------------

void WitOperation::setIntExecVols (bool theValue)
   {
   prepSetUnpreAttr ();

   intExecVols_ = theValue;
   }

//------------------------------------------------------------------------------

void WitOperation::setExecVol (const WitIVRealArg & theValue)
   {
   witAssert (theValue != NULL);

   witAssert (theValue.isNonNeg (nPeriods ()));

   if (myProblem ()->myHeurAllMgr ()->modifiable ())
      {
      prepSetNonUnpostAttr ();

      myProblem ()->myHeurModifier ()->modifyForExecVol (this, theValue);
      }
   else
      {
      myDetOptImpMgr ()->shutDown ();

      myOptComp ()->schedOptStarter ()->beChosen ();

      prepSetUnpostAttr ();
      }

   execVol_ = theValue;
   }

//------------------------------------------------------------------------------

void WitOperation::setCycleTime34 (const WitIVRealArg & theValue)
   {
   WitBomEntry * theBomEnt;

   witAssert (theValue != NULL);

   witAssert (wit34Compatible ());

   prepSetUnpreAttr ();

   forEachEl (theBomEnt, bom ())
      theBomEnt->adjustOffset (theValue);

   cycleTime34_ = theValue;
   }

//------------------------------------------------------------------------------

void WitOperation::setPipEnabled (bool theValue)
   {
   prepSetUnpostAttr ();

   pipEnabled_ = theValue;
   }

//------------------------------------------------------------------------------

void WitOperation::setPipRank (int theValue)
   {
   prepSetUnpostAttr ();

   pipRank_ = theValue;
   }

//------------------------------------------------------------------------------

void WitOperation::storePreExecutable (WitPeriod thePer, bool theValue)
   {
   witAssert (myPreprocessor ()->preprocessing ());

   executable_[thePer] = theValue;
   }

//------------------------------------------------------------------------------

void WitOperation::storeHeurExecVol (WitPeriod thePer, double theValue)
   {
   witAssert (myHeurAllMgr ()->updatingSoln ());

   execVol_.elemRef (thePer) = theValue;
   }

//------------------------------------------------------------------------------

void WitOperation::storeOptExecVol (WitPeriod thePer, double theValue)
   {
   witAssert (myDetOptImpMgr ()->uploadingSoln ());

   execVol_.elemRef (thePer) = theValue;
   }

//------------------------------------------------------------------------------

void WitOperation::storeStochExecCost (const WitDblFlexVec & theFlexVec)
   {
   witAssert (myStochImpMgr ()->uploadingData ());

   execCost_.curDblFlexVec () = theFlexVec;
   }

//------------------------------------------------------------------------------

void WitOperation::storeStochExecVol (const WitDblFlexVec & theFlexVec)
   {
   witAssert (myStochImpMgr ()->uploadingData ());

   execVol_ = theFlexVec;
   }

//------------------------------------------------------------------------------

void WitOperation::storeMrpMrpExecVol (const WitTVec <double> & theTVec)
   {
   witAssert (myOrigMrpExp ()->uploadingSoln ());

   mrpExecVol_ = theTVec;
   }

//------------------------------------------------------------------------------

void WitOperation::storeFssFssExecVol (const WitTVec <double> & theTVec)
   {
   witAssert (myFSS ()->uploadingSoln ());

   fssExecVol_ = theTVec;
   }

//------------------------------------------------------------------------------
// Other functions.
//------------------------------------------------------------------------------

void WitOperation::insert (WitBomEntry * theBomEnt)
   {
   bom_.appendUniquely (theBomEnt);
   }

//------------------------------------------------------------------------------

void WitOperation::insert (WitBopEntry * theBopEnt)
   {
   bop_.appendUniquely (theBopEnt);
   }

//------------------------------------------------------------------------------

bool WitOperation::inserted ()
   {
   return (myCompMgr ()->findOperation (operationName ().myCstring ()) == this);
   }

//------------------------------------------------------------------------------

void WitOperation::writeMultiObjVecs (WitObjective * theObj)
   {
   execCost_.writeDataMulti ("execCost", this, theObj);
   }

//------------------------------------------------------------------------------

void WitOperation::writeSolnData ()
   {
   if (execVol () == defExecVol ())
      return;

   myDataWriter ()->writeFirstKeyWord ("set");
   myDataWriter ()->writeKeyWord      ("operation");
   myDataWriter ()->writeStringValue  (operationName ());

   myDataWriter ()->writeVector ("execVol", execVol (), defExecVol ());

   myDataWriter ()->writeEndCommand ();
   }

//------------------------------------------------------------------------------

void WitOperation::copySelfInto (WitCompMgr * theCompMgr)
   {
   WitOperation * theOpn;

   theOpn = new WitOperation (operationName ().myCstring (), theCompMgr);

   theOpn->copyAttrsFrom (this);
   }

//------------------------------------------------------------------------------

void WitOperation::recount ()
   {
   mappingIndex_ = myCompMgr ()->nOperations ();

   myCompMgr ()->recountOperation (this);
   }

//------------------------------------------------------------------------------

void WitOperation::unpreprocess ()
   {
   executable_ = false;
   }

//------------------------------------------------------------------------------

void WitOperation::clearOptSoln  ()
   {                              
   witAssert (myDetOptImpMgr ()->clearingSoln ());

   execVol_ = 0.0;
   }

//------------------------------------------------------------------------------

void WitOperation::clearStochSoln ()
   {
   WitComponent::clearStochSoln ();

   execVol_ = 0.0;
   }

//------------------------------------------------------------------------------

WitOperation * WitOperation::thisOpn ()
   {
   return this;
   }

//------------------------------------------------------------------------------

void WitOperation::identifyInstance ()
   {
   if (myMsgFac ().mayIssue ("operationInstIDMsg"))
      myMsgFac () ("operationInstIDMsg", operationName ());
   }

//------------------------------------------------------------------------------

void WitOperation::writeSetClause ()
   {
   myDataWriter ()->writeFirstKeyWord ("set");
   myDataWriter ()->writeKeyWord      ("operation");
   myDataWriter ()->writeStringValue  (operationName ());
   }

//------------------------------------------------------------------------------

WitMsgFrag WitOperation::objTypeMsgFrag ()
   {
   return myMsgFac ().myFrag ("operationFrag");
   }

//------------------------------------------------------------------------------

WitAttr WitOperation::apiObjItrState ()
   {
   return WitAT_OPERATION;
   }

//------------------------------------------------------------------------------

int WitOperation::nodeIndex () const
   {
   return myCompMgr ()->nParts () + mappingIndex_;
   }

//------------------------------------------------------------------------------

bool WitOperation::validYieldRate (double theValue)
   {
   if    (theValue >= 0.01 - FLOAT_EPSILON)
      if (theValue <= 1.0  + FLOAT_EPSILON)
         return true;

   return (theValue == 0.0);
   }

//------------------------------------------------------------------------------

bool WitOperation::cycleTime34PreventsExec (WitPeriod thePer)
   {
   witAssert (wit34Compatible ());

   if (cycleTime34 ()[thePer] < 0.0)
      return true;

   if (static_cast <int> (cycleTime34 ()[thePer]) > thePer)
      return true;

   return false;
   }

//------------------------------------------------------------------------------

void WitOperation::setWit34Compatible (bool newValue)
   {
   WitPeriod thePer;

   if (wit34Compatible ())
      if (not newValue)
         {
         forEachPeriod (thePer, myProblem ())
            if (cycleTime34PreventsExec (thePer))
               if (yieldRate_[thePer] != 0.0)
                  yieldRate_.elemRef (thePer) = 0.0;

         cycleTime34_ = 0.0;
         }
   }

//------------------------------------------------------------------------------

void WitOperation::copyAttrsFrom (WitOperation * theOpn)
   {
   WitNode::copyAttrsFrom (theOpn);

   yieldRate_         = theOpn->yieldRate_;
   minLotSize_        = theOpn->minLotSize_;
   incLotSize_        = theOpn->incLotSize_;
   twoLevelLotSizes_  = theOpn->twoLevelLotSizes_;
   lotSize2Thresh_    = theOpn->lotSize2Thresh_;
   minLotSize2_       = theOpn->minLotSize2_;
   incLotSize2_       = theOpn->incLotSize2_;
   execPenalty_       = theOpn->execPenalty_;
   execCost_          = theOpn->execCost_;
   intExecVols_       = theOpn->intExecVols_;
   pipEnabled_        = theOpn->pipEnabled_;
   pipRank_           = theOpn->pipRank_;

   if (wit34Compatible ())
      cycleTime34_    = theOpn->cycleTime34_;

   execBounds_->
      copyAttrsFrom    (theOpn->execBounds_);
   }

//------------------------------------------------------------------------------

bool WitOperation::isCoproducing (WitPeriod execPer)
   {
   int           nBroadProduced;
   WitBopEntry * theBopEnt;
   WitBomEntry * theBomEnt;
   WitSubEntry * theSub;

   nBroadProduced = 0;

   forEachEl (theBopEnt, bop ())
      if (theBopEnt->productRate ()[execPer] > 0.0)
         ++ nBroadProduced;

   forEachEl (theBomEnt, bom ())
      {
      if (theBomEnt->consRate ()[execPer] < 0.0)
         ++ nBroadProduced;

      forEachEl (theSub, theBomEnt->mySubEntries ())
         if (theSub->consRate ()[execPer] < 0.0)
            ++ nBroadProduced;
      }

   return (nBroadProduced > 1);
   }

//------------------------------------------------------------------------------

void WitOperation::display ()
   {
   WitBomEntry * theBomEnt;
   WitBopEntry * theBopEnt;

   myMsgFac () ("opnIdDdMsg", operationName ());

   myMsgFac () ("opnScalarsDdMsg", 
      mappingIndex_,
      twoLevelLotSizes_,
      execPenalty_,
      intExecVols_,
      pipEnabled_,
      pipRank_);

   if (wit34Compatible ())
      myProblem ()->display ("cycleTime34DdMsg", cycleTime34_);

   myProblem ()->display ("yieldRateDdMsg",  yieldRate_);
   myProblem ()->display ("minLotSizeDdMsg", minLotSize_);
   myProblem ()->display ("incLotSizeDdMsg", incLotSize_);

   if (twoLevelLotSizes_)
      {
      myProblem ()->display ("lotSize2ThreshDdMsg", lotSize2Thresh_);
      myProblem ()->display ("minLotSize2DdMsg",    minLotSize2_);
      myProblem ()->display ("incLotSize2DdMsg",    incLotSize2_);
      }

   myProblem ()->display ("execCostDdMsg", execCost_);

   execBounds_->display ();

   if (myPreprocessor ()->preprocessed ())
      myMsgFac () ("executableDdMsg", executable_);

   myMsgFac () ("procBomHeadingDdMsg");

   if (bom_.isEmpty ())
      myMsgFac () ("noneDdMsg");
   else
      {
      myMsgFac () ("procBomHeading2DdMsg");

      forEachEl (theBomEnt, bom_)
         myMsgFac () ("procBomDataDdMsg",
            theBomEnt->myPartName (),
            theBomEnt->localIndex ());
      }

   myMsgFac () ("procBopHeadingDdMsg");

   if (bop_.isEmpty ())
      myMsgFac () ("noneDdMsg");
   else
      {
      myMsgFac () ("procBopHeading2DdMsg");

      forEachEl (theBopEnt, bop_)
         myMsgFac () ("procBopDataDdMsg",
            theBopEnt->myPartName (),
            theBopEnt->localIndex ());
      }

   WitDelComp::display ();
   }

//------------------------------------------------------------------------------

void WitOperation::clearExecVolForHeur ()
   {
   witAssert (myHeurAllMgr ()->updatingSoln ());

   execVol_ = 0.0;
   }

//------------------------------------------------------------------------------

double WitOperation::lotSizeDelta (
      WitPeriod thePer,
      double    theExecVol,
      double    delta)
   {
   double preLsVol;
   double lsVol;

   if (not lotSizeMode (thePer))
      return delta;

   preLsVol = theExecVol + delta;

   if (preLsVol < FLOAT_EPSILON)
      return delta;

   if (not twoLevelLotSizes_)
      lsVol =
         lotSizedExecVol (
            thePer, 
            preLsVol,
            minLotSize_, 
            incLotSize_);
   else
      lsVol = tllsExecVol (thePer, preLsVol);

   return lsVol - theExecVol;
   }

//------------------------------------------------------------------------------

void WitOperation::writeSoln ()
   {
   WitPeriod thePer;

   fprintf (solnFile (),
      "\nOperation %s:\n",
      operationName ().myCstring ());

   fprintf (solnFile (), "Period      ExecVol");

   if (fssComputed ())
      fprintf (solnFile (), "   FssExecVol");

   fprintf (solnFile (), "\n");

   forEachPeriod (thePer, myProblem ())
      {
      fprintf (solnFile (), "%6d", thePer);

      writeSolnValue (execVol ()[thePer]);

      if (fssComputed ())
         writeSolnValue (fssExecVol ()[thePer]);

      fprintf (solnFile (), "\n");
      }
   }

//------------------------------------------------------------------------------

void WitOperation::writeMrpSoln ()
   {
   WitPeriod thePer;

   fprintf (solnFile (), "\nOperation %s:\n", operationName ().myCstring ());

   fprintf (solnFile (), "Period   MrpExecVol\n");

   forEachPeriod (thePer, myProblem ())
      {
      fprintf (solnFile (), "%6d", thePer);

      writeSolnValue (mrpExecVol_[thePer]);

      fprintf (solnFile (), "\n");
      }
   }

//------------------------------------------------------------------------------

WitBoundSet * WitOperation::myBoundSet ()
   {
   return execBounds_;
   }

//------------------------------------------------------------------------------

void WitOperation::writeDataID ()
   {
   myDataWriter ()->writeFirstKeyWord ("add");
   myDataWriter ()->writeKeyWord      ("operation");
   myDataWriter ()->writeStringValue  (operationName ());
   }

//------------------------------------------------------------------------------

void WitOperation::writeDataAttrs ()
   {
   WitNode::writeDataAttrs ();

   myDataWriter ()->writeVector (
      "yieldRate",
      yieldRate_,
      defYieldRate ());

   myDataWriter ()->writeVector (
      "minLotSize",
      minLotSize_,
      defMinLotSize ());

   myDataWriter ()->writeVector (
      "incLotSize",
      incLotSize_,
      defIncLotSize ());

   myDataWriter ()->writeBool (
      "twoLevelLotSizes",
      twoLevelLotSizes_,
      defTwoLevelLotSizes ());

   myDataWriter ()->writeVector (
      "lotSize2Thresh",
      lotSize2Thresh_,
      defLotSize2Thresh ());

   myDataWriter ()->writeVector (
      "minLotSize2",
      minLotSize2_,
      defMinLotSize2 ());

   myDataWriter ()->writeVector (
      "incLotSize2",
      incLotSize2_,
      defIncLotSize2 ());

   myDataWriter ()->writeDouble (
      "execPenalty",
      execPenalty_,
      defExecPenalty ());

   myDataWriter ()->writeObjVec (
        "execCost",
         execCost_,
      defExecCost ());

   myDataWriter ()->writeBool (
      "intExecVols",
      intExecVols_,
      defIntExecVols ());

   if (wit34Compatible ())
      myDataWriter ()->writeVector (
         "cycleTime34",
         cycleTime34_,
         defCycleTime34 ());

   myDataWriter ()->writeBool (
      "pipEnabled",
      pipEnabled_,
      defPipEnabled ());

   myDataWriter ()->writeInt (
      "pipRank",
      pipRank_,
      defPipRank ());

   execBounds_->writeData ();
   }

//------------------------------------------------------------------------------

void WitOperation::purgeCompListsSpec ()
   {
   purgeSelComps (bom_);
   purgeSelComps (bop_);
   }

//------------------------------------------------------------------------------

void WitOperation::clearDepListsSpec ()
   {
   bom_.clear ();
   bop_.clear ();
   }

//------------------------------------------------------------------------------

bool WitOperation::lotSizeMode (WitPeriod thePer)
   {
   if (not myGlobalComp ()->lotSizesExist ())
      return false;

   if (not myProblem ()->myOrigMrpExp ()->exploding ())
      if (not myProblem ()->currentVariant ()->respectsLotSizes ())
         return false;

   if (incLotSize_[thePer] < 0.0009)
      return false;

   return true;
   }

//------------------------------------------------------------------------------

double WitOperation::tllsExecVol (WitPeriod thePer, double theExecVol)
   {
   double ls2t;
   double ls1Vol;

   ls2t = lotSize2Thresh_[thePer];

   if (theExecVol >= ls2t - myGlobalComp ()->lotSizeTol ())
      return
         lotSizedExecVol (
            thePer,
            theExecVol,
            minLotSize2_,
            incLotSize2_);

   ls1Vol =
      lotSizedExecVol (
         thePer,
         theExecVol,
         minLotSize_,
         incLotSize_);

   if (ls1Vol >= ls2t - myGlobalComp ()->lotSizeTol ())
      return
         lotSizedExecVol (
            thePer,
            ls2t,
            minLotSize2_,
            incLotSize2_);
   else
      return ls1Vol;
   }

//------------------------------------------------------------------------------

double WitOperation::lotSizedExecVol (
      WitPeriod             thePer,
      double                theExecVol,
      const WitDblFlexVec & mlsVec,
      const WitDblFlexVec & ilsVec)
   {
   double mls;
   double ils;
   double gridCoord;

   witAssert (theExecVol >= NET_TOL);

   mls       = mlsVec[thePer];
   ils       = ilsVec[thePer];

   gridCoord = (theExecVol - mls) / ils;

   gridCoord = ceil (gridCoord - myGlobalComp ()->lotSizeTol ());

   if (gridCoord < 0.0)
      gridCoord = 0.0;

   return mls + gridCoord * ils;
   }
