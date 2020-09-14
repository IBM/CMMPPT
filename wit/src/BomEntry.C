//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "BomEntry.C"
//
// Contains the implementation of class BomEntry.
//------------------------------------------------------------------------------

#include <SubEntry.h>
#include <Part.h>
#include <Operation.h>
#include <GlobalComp.h>
#include <DetOptImpMgr.h>
#include <DataWrit.h>
#include <Pre.h>
#include <Post.h>
#include <CompMgr.h>
#include <IVRealArg.h>
#include <wit.h>
#include <MsgFrag.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------

WitBomEntry::WitBomEntry (WitOperation * theOpn, WitPart * thePart):

      WitConsEntry     (thePart, theOpn->bom ().nElements ()),

      mappingIndex_    (myCompMgr ()->nBomEntries ()),
      myOperation_     (theOpn),
      mySubEntries_    (),
      mandEC_          (defMandEC ()),
      singleSource_    (defSingleSource ()),
      propRtg_         (myProblem (), defPropRtg ()),
      hasSubsInEffect_ (myProblem (), false),
      nonSubVol_       (myProblem (), 0.0)
   {
   witAssert (theOpn->myCompMgr () == thePart->myCompMgr ());

   if (wit34Compatible ())
      offsetRef () = myOperation ()->cycleTime34 ();
         //
         // This results in a default usageTime34 of 0.0.

   myCompMgr   ()->insert (this);
   myOperation ()->insert (this);
   myPart      ()->insert (this);
   }

//------------------------------------------------------------------------------

WitBomEntry::~WitBomEntry ()
   {
   }

//------------------------------------------------------------------------------

const WitDblFlexVec & WitBomEntry::nonSubVol ()
   {
   if (mySubEntries ().isEmpty ())
      return myOperation ()->execVol ();
   else
      return nonSubVol_;
   }

//------------------------------------------------------------------------------

int WitBomEntry::nInstances (WitProblem * theProblem)
   {
   return theProblem->myCompMgr ()->nBomEntries ();
   }

//------------------------------------------------------------------------------
// "Set" functions.
//------------------------------------------------------------------------------

void WitBomEntry::setMandEC (bool theValue)
   {
   prepSetUnpreAttr ();

   mandEC_ = theValue;
   }

//------------------------------------------------------------------------------

void WitBomEntry::setSingleSource (bool theValue)
   {
   prepSetUnpostAttr ();

   singleSource_ = theValue;
   }

//------------------------------------------------------------------------------

void WitBomEntry::setPropRtg (const WitBoolean * theValue)
   {
   witAssert (theValue != NULL);

   prepSetUnpostAttr ();

   copy (propRtg_, theValue);
   }

//------------------------------------------------------------------------------
// Other functions.
//------------------------------------------------------------------------------

void WitBomEntry::storePreMySubEntries (const WitList <WitSubEntry> & theList)
   {
   witAssert (myPreprocessor ()->preprocessing ());

   mySubEntries_ = theList;
   }

//------------------------------------------------------------------------------

void WitBomEntry::storePreHasSubsInEffect (const WitTVec <bool> & theTVec)
   {
   witAssert (myPreprocessor ()->preprocessing ());

   hasSubsInEffect_ = theTVec;
   }

//------------------------------------------------------------------------------

void WitBomEntry::storePostNonSubVol (const WitTVec <double> & theTVec)
   {
   witAssert (myPostprocessor ()->postprocessing ());

   nonSubVol_ = theTVec;
   }

//------------------------------------------------------------------------------

void WitBomEntry::storeOptNonSubVol (WitPeriod thePer, double theValue)
   {
   witAssert (myDetOptImpMgr ()->uploadingSoln ());

   nonSubVol_.elemRef (thePer) = theValue;
   }

//------------------------------------------------------------------------------

void WitBomEntry::insert (WitSubEntry * theSub)
   {
   mySubEntries_.appendUniquely (theSub);
   }

//------------------------------------------------------------------------------

bool WitBomEntry::inserted ()
   {
   return (myOperation ()->bom ().contains (this));
   }

//------------------------------------------------------------------------------

void WitBomEntry::identifyInstance ()
   {
   if (myMsgFac ().mayIssue ("bomEntryInstIDMsg"))
      myMsgFac () ("bomEntryInstIDMsg",
         myOperationName (),
         localIndex (),
         myPartName ());
   }

//------------------------------------------------------------------------------

void WitBomEntry::writeSetClause ()
   {
   myDataWriter ()->writeFirstKeyWord ("set");
   myDataWriter ()->writeKeyWord      ("bomEntry");
   myDataWriter ()->writeStringValue  (myOperationName ());
   myDataWriter ()->writeIntValue     (localIndex      ());
   }

//------------------------------------------------------------------------------

void WitBomEntry::copySelfInto (WitCompMgr * theCompMgr)
   {
   WitOperation * theOpn;
   WitPart *      thePart;
   WitBomEntry *  theBomEnt;

   theOpn    = theCompMgr->myMatchFor (myOperation ());
   thePart   = theCompMgr->myMatchFor (myPart      ());

   theBomEnt = new WitBomEntry (theOpn, thePart);

   theBomEnt->copyAttrsFrom (this);
   }

//------------------------------------------------------------------------------

void WitBomEntry::recount ()
   {
   recountConsEntry ();

   mappingIndex_ = myCompMgr ()->nBomEntries ();

   myCompMgr ()->recountBomEntry (this);
   }

//------------------------------------------------------------------------------

void WitBomEntry::unpreprocess ()
   {
   hasSubsInEffect_ = false;
   nonSubVol_       = 0.0;

   WitConsEntry::unpreprocess ();
   }

//------------------------------------------------------------------------------

WitBomEntry * WitBomEntry::thisBomEnt ()
   {
   return this;
   }

//------------------------------------------------------------------------------

WitMsgFrag WitBomEntry::objTypeMsgFrag ()
   {
   return myMsgFac ().myFrag ("bomEntryFrag");
   }

//------------------------------------------------------------------------------

WitAttr WitBomEntry::apiObjItrState ()
   {
   return WitAT_BOM_ENTRY;
   }

//------------------------------------------------------------------------------

void WitBomEntry::copyAttrsFrom (WitBomEntry * theBomEnt)
   {
   WitConsEntry::copyAttrsFrom (theBomEnt);

   mandEC_       = theBomEnt->mandEC_;
   singleSource_ = theBomEnt->singleSource_;
   propRtg_      = theBomEnt->propRtg_;
   }

//------------------------------------------------------------------------------

void WitBomEntry::setUsageTime34 (double theValue)
   {
   WitPeriod thePer;

   witAssert (wit34Compatible ());

   prepSetUnpreAttr ();

   if (myOperation ()->cycleTime34 () == myOperation ()->cycleTime34 ()[0])
      offsetRef () = (myOperation ()->cycleTime34 ()[0] - theValue);
   else
      forEachPeriod (thePer, myProblem ())
         offsetRef ().elemRef (thePer) =
            myOperation ()->cycleTime34 ()[thePer] - theValue;
   }

//------------------------------------------------------------------------------

double WitBomEntry::usageTime34 (WitPeriod thePer)
   {
   double ut34;

   witAssert (wit34Compatible ());

   ut34 = myOperation ()->cycleTime34 ()[thePer] - offset ()[thePer];

   return ut34;
   }

//------------------------------------------------------------------------------

double WitBomEntry::usageTime34 ()
   {
   return usageTime34 (0);
   }

//------------------------------------------------------------------------------

void WitBomEntry::adjustOffset (const WitIVRealArg & newCycleTime34)
   {
   WitPeriod thePer;

   forEachPeriod (thePer, myProblem ())
      offsetRef ().elemRef (thePer) +=
         newCycleTime34[thePer] - myOperation ()->cycleTime34 ()[thePer];
   }

//------------------------------------------------------------------------------

void WitBomEntry::display ()
   {
   WitSubEntry * theSub;

   myMsgFac () ("bomEntryDdMsg",
      myOperationName (),
      localIndex (),
      myPartName (),
      mappingIndex_,
      singleSource ());

   if (wit34Compatible ())
      myMsgFac () ("usageTime34DdMsg", usageTime34 ());

   myProblem ()->display ("propRtgDdMsg", propRtg_);

   if (myPreprocessor ()->preprocessed ())
      if (not mySubEntries ().isEmpty ())
         myProblem ()->display (
            "hasSubsInEffectDdMsg",
            hasSubsInEffect ());

   if (not mySubEntries ().isEmpty ())
      {
      myMsgFac () ("bomSubHeadingDdMsg");

      forEachEl (theSub, mySubEntries ())
         myMsgFac () ("bomSubDataDdMsg",
            theSub->myPartName (),
            theSub->localIndex ());
      }

   WitConsEntry::display ();
   }

//------------------------------------------------------------------------------

void WitBomEntry::writeSubSoln ()
   {
   WitSubEntry * theSub;
   WitPeriod     thePer;

   if (mySubEntries ().isEmpty ())
      return;

   fprintf (solnFile (), "\n"
      "Consuming Operation: %s,  Part Replaced: %s,  BOM Entry #%d:\n\n",
      myOperationName ().myCstring (),
      myPartName ().myCstring (),
      localIndex ());

   fprintf (solnFile (),
         "Period  Substitute       Sub #"
         "                     Volume");

   if (fssComputed ())
      fprintf (solnFile (), "                          Volume");

   fprintf (solnFile (), "\n");

   forEachPeriod (thePer, myProblem ())
      {
      if (hasSubsInEffect ()[thePer])
         {
         fprintf (solnFile (),
            "   %3d                          Non-SubVol: ",
            thePer);

         writeSolnValue (nonSubVol ()[thePer]);

         fprintf (solnFile (), "\n");
         }

      forEachEl (theSub, mySubEntries ())
         if (theSub->inEffect (thePer))
            {
            fprintf (solnFile (),
               "   %3d  %-12s  %8d      SubVol: ",
               thePer,
               theSub->myPartName ().myCstring (),
               theSub->localIndex ());

            writeSolnValue (theSub->subVol ()[thePer]);

            if (fssComputed ())
               {
               fprintf (solnFile (), "        FssSubVol: ");

               writeSolnValue (theSub->fssSubVol ()[thePer]);
               }

            fprintf (solnFile (), "\n");
            }

      if (hasSubsInEffect ()[thePer])
         {
         fprintf (solnFile (),
            "   %3d                             ExecVol: ",
            thePer);

         writeSolnValue (myOperation ()->execVol ()[thePer]);

         fprintf (solnFile (), "\n\n");
         }
      }
   }

//------------------------------------------------------------------------------

void WitBomEntry::writeMrpSubSoln ()
   {
   WitSubEntry * theSub;
   WitPeriod     t;

   if (mySubEntries ().isEmpty ())
      return;

   fprintf (solnFile (), "\n"
      "Consuming Operation: %s,  Part Replaced: %s,  BOM Entry #%d:\n\n",
      myOperationName ().myCstring (),
      myPartName ().myCstring (),
      localIndex ());

   fprintf (solnFile (),
         "Period  Substitute       Sub #"
         "                          Volume\n");

   forEachPeriod (t, myProblem ())
      {
      forEachEl (theSub, mySubEntries ())
         if (theSub->inEffect (t))
            {
            fprintf (solnFile (),
               "   %3d  %-12s  %8d        MrpSubVol: ",
               t,
               theSub->myPartName ().myCstring (),
               theSub->localIndex ());

            writeSolnValue (theSub->mrpSubVol ()[t]);

            fprintf (solnFile (), "\n");
            }

      if (hasSubsInEffect ()[t])
         {
         fprintf (solnFile (),
            "   %3d                               MrpExecVol: ",
            t);

         writeSolnValue (myOperation ()->mrpExecVol ()[t]);

         fprintf (solnFile (), "\n\n");
         }
      }
   }

//------------------------------------------------------------------------------

WitOperation * WitBomEntry::derivedOperation ()
   {
   return myOperation ();
   }

//------------------------------------------------------------------------------

void WitBomEntry::displayMandEC ()
   {
   myMsgFac () ("mandECDdMsg", mandEC ());
   }

//------------------------------------------------------------------------------

WitBomEntry * WitBomEntry::derivedBomEntry ()
   {
   return this;
   }

//------------------------------------------------------------------------------

void WitBomEntry::writeDataID ()
   {
   myDataWriter ()->writeFirstKeyWord ("add");
   myDataWriter ()->writeKeyWord      ("bomEntry");
   myDataWriter ()->writeStringValue  (myOperationName ());
   myDataWriter ()->writeStringValue  (myPartName ());
   }

//------------------------------------------------------------------------------

void WitBomEntry::writeDataAttrs ()
   {
   WitConsEntry::writeDataAttrs ();

   if (wit34Compatible ())
      writeOffset34 ();
   else
      writeOffset ();

   myDataWriter ()->writeBool (
      "mandEC",
      mandEC (),
      defMandEC ());

   myDataWriter ()->writeBool (
      "singleSource",
      singleSource (),
      defSingleSource ());

   myDataWriter ()->writeVector (
      "propRtg",
      propRtg_,
      defPropRtg ());
   }

//------------------------------------------------------------------------------

void WitBomEntry::findPrereqs (WitDelComp * & prereq1, WitDelComp * & prereq2)
   {
   prereq1 = myOperation ();
   prereq2 = myPart ();
   }

//------------------------------------------------------------------------------

void WitBomEntry::purgeCompListsSpec ()
   {
   purgeSelComps (mySubEntries_);
   }

//------------------------------------------------------------------------------

void WitBomEntry::clearDepListsSpec ()
   {
   mySubEntries_.clear ();
   }

//------------------------------------------------------------------------------

void WitBomEntry::writeOffset34 ()
   {
   WitMsgID  theMsgID;
   WitPeriod thePer;

   myProblem ()->myDataWriter ()->writeFirstKeyWord ("offset");

   theMsgID =
      myProblem ()->myGlobalComp ()->highPrecisionWD ()?
         "doubleDenseVectorHpWdMsg":
         "doubleDenseVectorWdMsg";

   myMsgFac () ("startVectorWdMsg", "dense");

   forEachPeriod (thePer, myProblem ())
      myMsgFac () (theMsgID,
         ((thePer == 0)? "": " "),
         offset ()[thePer]);

   myMsgFac () ("endVectorWdMsg");
   }
