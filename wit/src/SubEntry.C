//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "SubEntry.C"
//
// Contains the implementation of class SubEntry.
//------------------------------------------------------------------------------

#include <SubEntry.h>
#include <GlobalComp.h>
#include <OptComp.h>
#include <Part.h>
#include <CompMgr.h>
#include <DataWrit.h>
#include <HeurAllMgr.h>
#include <HeurModifier.h>
#include <OptStarter.h>
#include <DetOptImpMgr.h>
#include <OrigMrp.h>
#include <FSS.h>
#include <StochImpMgr.h>
#include <wit.h>
#include <IVRealArg.h>
#include <MsgFrag.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------

WitSubEntry::WitSubEntry (WitBomEntry * theBomEnt, WitPart * thePart):

      WitConsEntry    (thePart, theBomEnt->mySubEntries ().nElements ()),

      mappingIndex_   (myCompMgr ()->nSubEntries ()),
      myBomEnt_       (theBomEnt),

      subCost_        (myProblem (), defSubCost ()),
      intSubVols_     (defIntSubVols ()),
      expAllowed_     (defExpAllowed ()),
      netAllowed_     (defNetAllowed ()),
      mrpNetAllowed_  (defMrpNetAllowed ()),
      expNetAversion_ (defExpNetAversion ()),
      subVol_         (myProblem (), 0.0),
      mrpSubVol_      (myProblem (), 0.0),
      fssSubVol_      (myProblem (), 0.0)
   {
   witAssert (theBomEnt->myCompMgr () == thePart->myCompMgr ());

   myCompMgr ()->insert (this);
   myBomEnt  ()->insert (this);
   myPart    ()->insert (this);
   }

//------------------------------------------------------------------------------

WitSubEntry::~WitSubEntry ()
   {
   }

//------------------------------------------------------------------------------

const WitDblFlexVec & WitSubEntry::offset ()
   {
   return
      myGlobalComp ()->independentOffsets ()?
         WitBillEntry::offset ():
         myBomEnt ()->offset ();
   }

//------------------------------------------------------------------------------

int WitSubEntry::nInstances (WitProblem * theProblem)
   {
   return theProblem->myCompMgr ()->nSubEntries ();
   }

//------------------------------------------------------------------------------
// "Set" functions.
//------------------------------------------------------------------------------

void WitSubEntry::setSubCost (const WitIVRealArg & theValue)
   {
   witAssert (theValue != NULL);

   prepSetScenSpecAttr ("subCost");

   subCost_.curDblFlexVec () = theValue;
   }

//------------------------------------------------------------------------------

void WitSubEntry::setIntSubVols (bool theValue)
   {
   prepSetUnpreAttr ();

   intSubVols_ = theValue;
   }

//------------------------------------------------------------------------------

void WitSubEntry::setExpAllowed (bool theValue)
   {
   prepSetUnpreAttr ();

   expAllowed_ = theValue;
   }

//------------------------------------------------------------------------------

void WitSubEntry::setNetAllowed (bool theValue)
   {
   prepSetUnpreAttr ();

   netAllowed_ = theValue;
   }

//------------------------------------------------------------------------------

void WitSubEntry::setMrpNetAllowed (bool theValue)
   {
   prepSetUnpostAttr ();

   mrpNetAllowed_ = theValue;
   }

//------------------------------------------------------------------------------

void WitSubEntry::setExpNetAversion (double theValue)
   {
   prepSetUnpostAttr ();

   expNetAversion_ = theValue;
   }

//------------------------------------------------------------------------------

void WitSubEntry::setSubVol (const WitIVRealArg & theValue)
   {
   witAssert (theValue != NULL);

   witAssert (theValue.isNonNeg (nPeriods ()));

   if (myProblem ()->myHeurAllMgr ()->modifiable ())
      {
      prepSetNonUnpostAttr ();

      myProblem ()->myHeurModifier ()->modifyForSubVol (this, theValue);
      }
   else
      {
      myDetOptImpMgr ()->shutDown ();

      myOptComp ()->schedOptStarter ()->beChosen ();

      prepSetUnpostAttr ();
      }

   subVol_ = theValue;
   }

//------------------------------------------------------------------------------
// Other functions.
//------------------------------------------------------------------------------

void WitSubEntry::storeHeurSubVol (WitPeriod thePer, double theValue)
   {
   witAssert (myHeurAllMgr ()->updatingSoln ());

   subVol_.elemRef (thePer) = theValue;
   }

//------------------------------------------------------------------------------

void WitSubEntry::storeOptSubVol (WitPeriod thePer, double theValue)
   {
   witAssert (myDetOptImpMgr ()->uploadingSoln ());

   subVol_.elemRef (thePer) = theValue;
   }

//------------------------------------------------------------------------------

void WitSubEntry::storeStochSubCost (const WitDblFlexVec & theFlexVec)
   {
   witAssert (myStochImpMgr ()->uploadingData ());

   subCost_.curDblFlexVec () = theFlexVec;
   }

//------------------------------------------------------------------------------

void WitSubEntry::storeStochSubVol (const WitDblFlexVec & theFlexVec)
   {
   witAssert (myStochImpMgr ()->uploadingData ());

   subVol_ = theFlexVec;
   }

//------------------------------------------------------------------------------

void WitSubEntry::storeMrpMrpSubVol (const WitTVec <double> & theTVec)
   {
   witAssert (myOrigMrpExp ()->uploadingSoln ());

   mrpSubVol_ = theTVec;
   }

//------------------------------------------------------------------------------

void WitSubEntry::storeFssFssSubVol (const WitTVec <double> & theTVec)
   {
   witAssert (myFSS ()->uploadingSoln ());

   fssSubVol_ = theTVec;
   }

//------------------------------------------------------------------------------

bool WitSubEntry::inserted ()
   {
   return (myBomEnt ()->mySubEntries ().contains (this));
   }

//------------------------------------------------------------------------------

void WitSubEntry::writeMultiObjVecs (WitObjective * theObj)
   {
   subCost_.writeDataMulti ("subCost", this, theObj);
   }

//------------------------------------------------------------------------------

void WitSubEntry::identifyInstance ()
   {
   if (myMsgFac ().mayIssue ("subEntryInstIDMsg"))
      myMsgFac () ("subEntryInstIDMsg",
         myOperationName (),
         myBomEnt ()->localIndex (),
         myBomEnt ()->myPartName (),
         localIndex (),
         myPartName ());
   }

//------------------------------------------------------------------------------

void WitSubEntry::writeSetClause ()
   {
   myDataWriter ()->writeFirstKeyWord ("set");
   myDataWriter ()->writeKeyWord      ("subEntry");
   myDataWriter ()->writeStringValue  (myOperationName ());
   myDataWriter ()->writeIntValue     (myBomEntIndex   ());
   myDataWriter ()->writeIntValue     (localIndex      ());
   }

//------------------------------------------------------------------------------

void WitSubEntry::copySelfInto (WitCompMgr * theCompMgr)
   {
   WitOperation * theOpn;
   WitPart *      thePart;
   WitBomEntry *  theBomEnt;
   WitSubEntry *  theSub;

   theOpn    = theCompMgr->myMatchFor (myOperation ());
   thePart   = theCompMgr->myMatchFor (myPart      ());

   theBomEnt = theCompMgr->findBomEntry (theOpn, myBomEntIndex ());

   theSub    = new WitSubEntry (theBomEnt, thePart);

   theSub->copyAttrsFrom (this);
   }

//------------------------------------------------------------------------------

void WitSubEntry::recount ()
   {
   recountConsEntry ();

   mappingIndex_ = myCompMgr ()->nSubEntries ();

   myCompMgr ()->recountSubEntry (this);
   }

//------------------------------------------------------------------------------

void WitSubEntry::clearOptSoln  ()
   {                              
   witAssert (myDetOptImpMgr ()->clearingSoln ());

   subVol_ = 0.0;
   }

//------------------------------------------------------------------------------

void WitSubEntry::clearStochSoln ()
   {
   WitComponent::clearStochSoln ();

   subVol_ = 0.0;
   }

//------------------------------------------------------------------------------

WitSubEntry * WitSubEntry::thisSub ()
   {
   return this;
   }

//------------------------------------------------------------------------------

WitMsgFrag WitSubEntry::objTypeMsgFrag ()
   {
   return myMsgFac ().myFrag ("subEntryFrag");
   }

//------------------------------------------------------------------------------

WitAttr WitSubEntry::apiObjItrState ()
   {
   return WitAT_SUB_ENTRY;
   }

//------------------------------------------------------------------------------

bool WitSubEntry::storesOffset ()
   {
   return myGlobalComp ()->independentOffsets ();
   }

//------------------------------------------------------------------------------

void WitSubEntry::writeSolnData ()
   {
   if (subVol () == defSubVol ())
      return;

   myDataWriter ()->writeFirstKeyWord ("set");
   myDataWriter ()->writeKeyWord      ("subEntry");
   myDataWriter ()->writeStringValue  (myOperationName ());
   myDataWriter ()->writeIntValue     (myBomEntIndex ());
   myDataWriter ()->writeIntValue     (localIndex ());

   myDataWriter ()->writeVector       ("subVol", subVol (), defSubVol ());

   myDataWriter ()->writeEndCommand ();
   }

//------------------------------------------------------------------------------

void WitSubEntry::copyAttrsFrom (WitSubEntry * theSub)
   {
   WitConsEntry::copyAttrsFrom (theSub);

   subCost_        = theSub->subCost_;
   intSubVols_     = theSub->intSubVols_;
   expAllowed_     = theSub->expAllowed_;
   netAllowed_     = theSub->netAllowed_;
   mrpNetAllowed_  = theSub->mrpNetAllowed_;
   expNetAversion_ = theSub->expNetAversion_;
   }

//------------------------------------------------------------------------------

void WitSubEntry::display ()
   {
   myMsgFac () ("subEntryDdMsg",
      myOperationName (),
      myBomEntIndex (),
      myBomEnt ()->myPartName (),
      localIndex (),
      myPartName (),
      mappingIndex_,
      intSubVols (),
      expAllowed (),
      netAllowed (),
      mrpNetAllowed (),
      expNetAversion ());

   myProblem ()->display ("subCostDdMsg", subCost_);

   WitConsEntry::display ();
   }

//------------------------------------------------------------------------------

void WitSubEntry::clearSubVolForHeur ()
   {
   witAssert (myHeurAllMgr ()->updatingSoln ());

   subVol_ = 0.0;
   }

//------------------------------------------------------------------------------

WitOperation * WitSubEntry::derivedOperation ()
   {
   return myOperation ();
   }

//------------------------------------------------------------------------------

WitBomEntry * WitSubEntry::derivedBomEntry ()
   {
   return myBomEnt ();
   }

//------------------------------------------------------------------------------

void WitSubEntry::writeDataID ()
   {
   myDataWriter ()->writeFirstKeyWord ("add");
   myDataWriter ()->writeKeyWord      ("subEntry");
   myDataWriter ()->writeStringValue  (myOperationName ());
   myDataWriter ()->writeIntValue     (myBomEntIndex ());
   myDataWriter ()->writeStringValue  (myPartName ());
   }

//------------------------------------------------------------------------------

void WitSubEntry::writeDataAttrs ()
   {
   WitConsEntry::writeDataAttrs ();

   if (myGlobalComp ()->independentOffsets ())
      writeOffset ();

   myDataWriter ()->writeObjVec (
        "subCost",
         subCost_,
      defSubCost ());

   myDataWriter ()->writeBool (
        "intSubVols",
         intSubVols (),
      defIntSubVols ());

   myDataWriter ()->writeBool (
        "expAllowed",
         expAllowed (),
      defExpAllowed ());

   myDataWriter ()->writeBool (
        "netAllowed",
         netAllowed (),
      defNetAllowed ());

   myDataWriter ()->writeBool (
        "mrpNetAllowed",
         mrpNetAllowed (),
      defMrpNetAllowed ());

   myDataWriter ()->writeDouble (
        "expNetAversion",
         expNetAversion (),
      defExpNetAversion ());
   }

//------------------------------------------------------------------------------

void WitSubEntry::findPrereqs (WitDelComp * & prereq1, WitDelComp * & prereq2)
   {
   prereq1 = myBomEnt ();
   prereq2 = myPart ();
   }
