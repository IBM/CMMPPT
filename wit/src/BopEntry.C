//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "BopEntry.C"
//
// Contains the implementation of class BopEntry.
// Contains the implementation of class template ExecPerSched <Entry>.
//------------------------------------------------------------------------------

#include <BopEntry.h>
#include <CompMgr.h>
#include <GlobalComp.h>
#include <Part.h>
#include <Operation.h>
#include <DataWrit.h>
#include <Pre.h>
#include <wit.h>
#include <ObjStack.h>
#include <IVRealArg.h>
#include <MsgFrag.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class BopEntry.
//------------------------------------------------------------------------------

WitBopEntry::WitBopEntry (WitOperation * theOpn, WitPart * thePart):

      WitBillEntry   (thePart, theOpn->bop ().nElements ()),

      mappingIndex_  (myCompMgr ()->nBopEntries ()),
      myOperation_   (theOpn),

      productRate_   (myProblem (), defProductRate ()),
      expAllowed_    (defExpAllowed ()),
      expAversion_   (defExpAversion ()),
      meSelSplit_    (defMeSelSplit ()),

      effProdRate_   (myProblem (), 0.0),
      expExecPeriod_ (myProblem (), -1)
   {
   witAssert (theOpn->myCompMgr () == thePart->myCompMgr ());

   myCompMgr   ()->insert (this);
   myOperation ()->insert (this);
   myPart      ()->insert (this);
   }

//------------------------------------------------------------------------------

WitBopEntry::~WitBopEntry ()
   {
   }

//------------------------------------------------------------------------------

int WitBopEntry::nInstances (WitProblem * theProblem)
   {
   return theProblem->myCompMgr ()->nBopEntries ();
   }

//------------------------------------------------------------------------------
// "Set" functions.
//------------------------------------------------------------------------------

void WitBopEntry::setProductRate (const WitIVRealArg & theValue)
   {
   witAssert (theValue != NULL);

   witAssert (theValue.isNonNeg (nPeriods ()));

   prepSetUnpreAttr ();

   productRate_ = theValue;
   }

//------------------------------------------------------------------------------

void WitBopEntry::setExpAllowed (bool theValue)
   {
   prepSetUnpreAttr ();

   expAllowed_ = theValue;
   }

//------------------------------------------------------------------------------

void WitBopEntry::setExpAversion (double theValue)
   {
   prepSetUnpreAttr ();

   expAversion_ = theValue;
   }

//------------------------------------------------------------------------------

void WitBopEntry::setMeSelSplit (bool theValue)
   {
   prepSetUnpostAttr ();

   meSelSplit_ = theValue;
   }

//------------------------------------------------------------------------------
// Other functions.
//------------------------------------------------------------------------------

void WitBopEntry::storePreEffProdRate (const WitTVec <double> & theTVec)
   {
   witAssert (myPreprocessor ()->preprocessing ());

   effProdRate_ = theTVec;
   }

//------------------------------------------------------------------------------

void WitBopEntry::storePreExpExecPeriod (const WitTVec <WitPeriod> & theTVec)
   {
   witAssert (myPreprocessor ()->preprocessing ());

   expExecPeriod_ = theTVec;
   }

//------------------------------------------------------------------------------

bool WitBopEntry::inserted ()
   {
   return (myOperation ()->bop ().contains (this));
   }

//------------------------------------------------------------------------------

void WitBopEntry::identifyInstance ()
   {
   if (myMsgFac ().mayIssue ("bopEntryInstIDMsg"))
      myMsgFac () ("bopEntryInstIDMsg",
         myOperationName (),
         localIndex (),
         myPartName ());
   }

//------------------------------------------------------------------------------

void WitBopEntry::writeSetClause ()
   {
   myDataWriter ()->writeFirstKeyWord ("set");
   myDataWriter ()->writeKeyWord      ("bopEntry");
   myDataWriter ()->writeStringValue  (myOperationName ());
   myDataWriter ()->writeIntValue     (localIndex      ());
   }

//------------------------------------------------------------------------------

void WitBopEntry::copySelfInto (WitCompMgr * theCompMgr)
   {
   WitOperation * theOpn;
   WitPart *      thePart;
   WitBopEntry *  theBopEnt;

   theOpn    = theCompMgr->myMatchFor (myOperation ());
   thePart   = theCompMgr->myMatchFor (myPart      ());

   theBopEnt = new WitBopEntry (theOpn, thePart);

   theBopEnt->copyAttrsFrom (this);
   }

//------------------------------------------------------------------------------

void WitBopEntry::recount ()
   {
   recountBillEntry ();

   mappingIndex_ = myCompMgr ()->nBopEntries ();

   myCompMgr ()->recountBopEntry (this);
   }

//------------------------------------------------------------------------------

void WitBopEntry::unpreprocess ()
   {
   expExecPeriod_ = -1;
   effProdRate_   =  0.0;

   WitBillEntry::unpreprocess ();
   }

//------------------------------------------------------------------------------

WitBopEntry * WitBopEntry::thisBopEnt ()
   {
   return this;
   }

//------------------------------------------------------------------------------

WitMsgFrag WitBopEntry::objTypeMsgFrag ()
   {
   return myMsgFac ().myFrag ("bopEntryFrag");
   }

//------------------------------------------------------------------------------

WitAttr WitBopEntry::apiObjItrState ()
   {
   return WitAT_BOP_ENTRY;
   }

//------------------------------------------------------------------------------

void WitBopEntry::display ()
   {
   myMsgFac () ("bopEntryDdMsg",
      myOperationName (),
      localIndex (),
      myPartName (),
      mappingIndex_,
      expAllowed (),
      expAversion (),
      meSelSplit ());

   myProblem ()->display ("productRateDdMsg", productRate_);

   if (myPreprocessor ()->preprocessed ())
      {
      myProblem ()->display ("effProdRateDdMsg", effProdRate_);

      if (expAllowed ())
         myMsgFac () ("expExecPeriodDdMsg", expExecPeriod_);
      }

   WitBillEntry::display ();
   }

//------------------------------------------------------------------------------

void WitBopEntry::copyAttrsFrom (WitBopEntry * theBopEnt)
   {
   WitBillEntry::copyAttrsFrom (theBopEnt);

   productRate_ = theBopEnt->productRate_;
   expAllowed_  = theBopEnt->expAllowed_;
   expAversion_ = theBopEnt->expAversion_;
   meSelSplit_  = theBopEnt->meSelSplit_;
   }

//------------------------------------------------------------------------------

bool WitBopEntry::execPerOKForExp (WitPeriod execPer)
   {
   return effProdRate_[execPer] >= myGlobalComp ()->expCutoff ();
   }

//------------------------------------------------------------------------------

void WitBopEntry::getAllInstances (
      WitProblem *                theProblem,
      WitObjStack <WitBopEntry> & theBopEntries)
   {
   WitBopEntry * theBopEnt;

   theBopEntries.clear ();

   forEachBopEntry (theBopEnt, theProblem)
      theBopEntries.push (theBopEnt);

   theBopEntries.reverse ();
   }

//------------------------------------------------------------------------------

WitOperation * WitBopEntry::derivedOperation ()
   {
   return myOperation ();
   }

//------------------------------------------------------------------------------

void WitBopEntry::writeDataID ()
   {
   myDataWriter ()->writeFirstKeyWord ("add");
   myDataWriter ()->writeKeyWord      ("bopEntry");
   myDataWriter ()->writeStringValue  (myOperationName ());
   myDataWriter ()->writeStringValue  (myPartName ());
   }

//------------------------------------------------------------------------------

void WitBopEntry::writeDataAttrs ()
   {
   WitBillEntry::writeDataAttrs ();

   writeOffset ();

   myDataWriter ()->writeVector (
      "productRate",
      productRate (),
      defProductRate ());

   myDataWriter ()->writeBool (
      "expAllowed",
      expAllowed (),
      defExpAllowed ());

   myDataWriter ()->writeDouble (
      "expAversion",
      expAversion (),
      defExpAversion ());

   myDataWriter ()->writeBool (
      "meSelSplit",
      meSelSplit (),
      defMeSelSplit ());
   }

//------------------------------------------------------------------------------

void WitBopEntry::findPrereqs (WitDelComp * & prereq1, WitDelComp * & prereq2)
   {
   prereq1 = myOperation ();
   prereq2 = myPart ();
   }
