//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "BillEntry.C"
//
// Contains the implementation of the following classes:
//
//    BillEntry
//    ConsEntry
//------------------------------------------------------------------------------

#include <StochImpMgr.h>
#include <CompMgr.h>
#include <Part.h>
#include <Operation.h>
#include <SubEntry.h>
#include <BopEntry.h>
#include <DataWrit.h>
#include <Pre.h>
#include <ObjStack.h>
#include <IVRealArg.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class BillEntry.
//------------------------------------------------------------------------------

WitBillEntry::~WitBillEntry ()
   {
   }

//------------------------------------------------------------------------------

const WitString & WitBillEntry::myPartName ()
   {
   return myPart ()->partName ();
   }

//------------------------------------------------------------------------------

const WitString & WitBillEntry::myOperationName ()
   {
   return myOperation ()->operationName ();
   }

//------------------------------------------------------------------------------

const WitDblFlexVec & WitBillEntry::offset ()
   {
   return offset_;
   }

//------------------------------------------------------------------------------

int WitBillEntry::nInstances (WitProblem * theProblem)
   {
   return theProblem->myCompMgr ()->nBillEntries ();
   }

//------------------------------------------------------------------------------

WitPeriod WitBillEntry::defLatestPeriod (WitProblem * theProblem)
   {
   return theProblem->lastPeriod ();
   }

//------------------------------------------------------------------------------

void WitBillEntry::storePreImpactPeriod (WitPeriod thePer, WitPeriod theValue)
   {
   witAssert (myPreprocessor ()->preprocessing ());

   impactPeriod_[thePer] = theValue;
   }

//------------------------------------------------------------------------------

void WitBillEntry::unpreprocess ()
   {
   impactPeriod_ = -1;

   firstEPIndex_.clear ();
   execPeriod_  .clear ();
   }

//------------------------------------------------------------------------------

bool WitBillEntry::storesOffset ()
   {
   return true;
   }

//------------------------------------------------------------------------------

bool WitBillEntry::withinInterval (WitPeriod thePer)
   {
   return (thePer >= earliestPeriod ()) and (thePer <= latestPeriod ());
   }

//------------------------------------------------------------------------------

WitBillEntry::WitBillEntry (WitPart * thePart, int theLocalIndex):

      WitDelComp      (thePart->myCompMgr ()),

      mappingIndex_   (myCompMgr ()->nBillEntries ()),
      myPart_         (thePart),
      localIndex_     (theLocalIndex),

      offset_         (myProblem (), defOffset ()),
      earliestPeriod_ (defEarliestPeriod ()),
      latestPeriod_   (defLatestPeriod (myProblem ())),
      routingShare_   (myProblem (), defRoutingShare ()),

      impactPeriod_   (myProblem (), -1),

      firstEPIndex_   (),
      execPeriod_     ()
   {
   }

//------------------------------------------------------------------------------

void WitBillEntry::writeOffset ()
   {
   myDataWriter ()->writeVector (
      "offset",
      offset (),
      defOffset ());
   }

//------------------------------------------------------------------------------

void WitBillEntry::preConstruct (WitOperation * theOpn, WitPart * thePart)
   {
   witAssert (theOpn  != NULL);
   witAssert (thePart != NULL);

   witAssert (theOpn->myProblem () == thePart->myProblem ());
   }

//------------------------------------------------------------------------------

void WitBillEntry::recountBillEntry ()
   {
   mappingIndex_ = myCompMgr ()->nBillEntries ();
   }

//------------------------------------------------------------------------------
// "Set" functions.
//------------------------------------------------------------------------------

void WitBillEntry::setEarliestPeriod (int theValue)
   {
   witAssert (theValue >= 0);
   witAssert (theValue <= nPeriods ());

   prepSetUnpreAttr ();

   earliestPeriod_ = theValue;
   }

//------------------------------------------------------------------------------

void WitBillEntry::setLatestPeriod (int theValue)
   {
   witAssert (theValue >= 0);
   witAssert (theValue <= lastPeriod ());

   prepSetUnpreAttr ();

   latestPeriod_ = theValue;
   }

//------------------------------------------------------------------------------

void WitBillEntry::setRoutingShare (const WitIVRealArg & theValue)
   {
   WitPeriod thePer;

   witAssert (theValue != NULL);

   if (DEBUG_MODE)
      forEachPeriod (thePer, myProblem ())
         witAssert (theValue[thePer] >= 1.0 - FLOAT_EPSILON);

   prepSetUnpostAttr ();

   routingShare_ = theValue;
   }

//------------------------------------------------------------------------------

void WitBillEntry::setOffset (const WitIVRealArg & theValue)
   {
   witAssert (theValue != NULL);

   witAssert (storesOffset ());

   prepSetUnpreAttr ();

   offset_ = theValue;
   }

//------------------------------------------------------------------------------
// Other functions.
//------------------------------------------------------------------------------

void WitBillEntry::getAllInstances (
      WitProblem *                 theProblem,
      WitObjStack <WitBillEntry> & theBillEntries)
   {
   WitBomEntry * theBomEnt;
   WitSubEntry * theSub;
   WitBopEntry * theBopEnt;

   theBillEntries.clear ();

   forEachBomEntry (theBomEnt, theProblem)
      theBillEntries.push (theBomEnt);
      
   forEachSubEntry (theSub, theProblem)
      theBillEntries.push (theSub);
      
   forEachBopEntry (theBopEnt, theProblem)
      theBillEntries.push (theBopEnt);

   theBillEntries.reverse ();
   }

//------------------------------------------------------------------------------

void WitBillEntry::reLocalIndex (WitProblem * theProblem)
   {
   WitOperation * theOpn;
   WitBomEntry  * theBomEnt;

   forEachOperation (theOpn, theProblem)
      {
      reLocalIndex (theOpn->bom ());
      reLocalIndex (theOpn->bop ());

      forEachEl (theBomEnt, theOpn->bom ())
         reLocalIndex (theBomEnt->mySubEntries ());
      }
   }

//------------------------------------------------------------------------------

void WitBillEntry::displayMandEC ()
   {
   }

//------------------------------------------------------------------------------

void WitBillEntry::writeDataAttrs ()
   {
   WitDelComp::writeDataAttrs ();

   myDataWriter ()->writeInt (
      "earliestPeriod",
      earliestPeriod (),
      defEarliestPeriod ());

   myDataWriter ()->writeInt (
      "latestPeriod",
      latestPeriod (),
      defLatestPeriod (myProblem ()));

   myDataWriter ()->writeVector (
      "routingShare",
      routingShare_,
      defRoutingShare ());
   }

//------------------------------------------------------------------------------

void WitBillEntry::copyAttrsFrom (WitBillEntry * theBillEnt)
   {
   WitDelComp::copyAttrsFrom (theBillEnt);

   if (storesOffset ())
      offset_      = theBillEnt->offset_;
   earliestPeriod_ = theBillEnt->earliestPeriod_;
   latestPeriod_   = theBillEnt->latestPeriod_;
   routingShare_   = theBillEnt->routingShare_;

   }

//------------------------------------------------------------------------------

void WitBillEntry::display ()
   {
   myMsgFac () ("billEntryDdMsg",
      mappingIndex_,
      earliestPeriod (),
      latestPeriod ());

   myProblem ()->display ("routingShareDdMsg", routingShare ());

   myProblem ()->display ("offsetDdMsg", offset ());

   displayMandEC ();

   if (myPreprocessor ()->preprocessed ())
      myMsgFac () ("impactPeriodDdMsg", impactPeriod ());

   WitDelComp::display ();
   }

//------------------------------------------------------------------------------
// Macro implementReLocalIndex (BillEntryClass)
//
// Implements WitBillEntry::reLocalIndex (const WitList <BillEntryClass> &)
// BillEntryClass must be derived from BillEntry.
//------------------------------------------------------------------------------

#define implementReLocalIndex(BillEntryClass)                                  \
                                                                               \
void WitBillEntry::reLocalIndex (const WitList <BillEntryClass> & theList)     \
   {                                                                           \
   BillEntryClass * theDerBillEntry;                                           \
   WitBillEntry *   theBillEnt;                                                \
   int              theIndex;                                                  \
                                                                               \
   if (theList.isEmpty ())                                                     \
      return;                                                                  \
                                                                               \
   if (theList.last ()->localIndex () == theList.nElements () - 1)             \
      return;                                                                  \
                                                                               \
   theIndex = 0;                                                               \
                                                                               \
   forEachEl (theDerBillEntry, theList)                                        \
      {                                                                        \
      theBillEnt              = theDerBillEntry;                               \
      theBillEnt->localIndex_ = theIndex ++;                                   \
      }                                                                        \
   }                                                                           \

//------------------------------------------------------------------------------

implementReLocalIndex (WitBomEntry)
implementReLocalIndex (WitSubEntry)
implementReLocalIndex (WitBopEntry)

//------------------------------------------------------------------------------
// Implementation of class ConsEntry.
//------------------------------------------------------------------------------

WitConsEntry::~WitConsEntry ()
   {
   }

//------------------------------------------------------------------------------

int WitConsEntry::nInstances (WitProblem * theProblem)
   {
   return theProblem->myCompMgr ()->nConsEntries ();
   }

//------------------------------------------------------------------------------
// "Set" functions.
//------------------------------------------------------------------------------

void WitConsEntry::setConsRate (const WitIVRealArg & theValue)
   {
   witAssert (theValue != NULL);

   prepSetUnpreAttr ();

   consRate_ = theValue;
   }

//------------------------------------------------------------------------------

void WitConsEntry::setFalloutRate (double theValue)
   {
   witAssert (theValue >= 0.0);
   witAssert (theValue <= 0.99 + FLOAT_EPSILON);

   prepSetUnpreAttr ();

   falloutRate_ = theValue;
   }

//------------------------------------------------------------------------------

void WitConsEntry::setExecPenalty (double theValue)
   {
   witAssert (theValue >= 0.0);

   prepSetUnpostAttr ();

   execPenalty_ = theValue;
   }

//------------------------------------------------------------------------------
// Other functions.
//------------------------------------------------------------------------------

void WitConsEntry::storePreEffConsRate (const WitTVec <double> & theTVec)
   {
   witAssert (myPreprocessor ()->preprocessing ());

   effConsRate_ = theTVec;
   }

//------------------------------------------------------------------------------

void WitConsEntry::storePreEffConsRate (double theValue)
   {
   witAssert (myPreprocessor ()->preprocessing ());

   effConsRate_ = theValue;
   }

//------------------------------------------------------------------------------

void WitConsEntry::unpreprocess ()
   {
   effConsRate_ = 0.0;

   WitBillEntry::unpreprocess ();
   }

//------------------------------------------------------------------------------

WitConsEntry * WitConsEntry::thisConsEnt ()
   {
   return this;
   }

//------------------------------------------------------------------------------

void WitConsEntry::incConsVol (
            WitTVec <double> & consVolArg,
      const WitTVec <double> & execVolArg)
   {
   WitPeriod thePer;

   forEachPeriod (thePer, myProblem ())
      if (inEffect (thePer))
         consVolArg[impactPeriod ()[thePer]] += 
            effConsRate ()[thePer] * execVolArg[thePer];
   }

//------------------------------------------------------------------------------

double WitConsEntry::effCost (WitPeriod thePer)
   {
   return 0.0;
   }

//------------------------------------------------------------------------------

WitConsEntry::WitConsEntry (WitPart * thePart, int theLocalIndex):

      WitBillEntry  (thePart, theLocalIndex),

      mappingIndex_ (myCompMgr ()->nConsEntries ()),
      consRate_     (myProblem (), defConsRate ()),
      falloutRate_  (defFalloutRate ()),
      execPenalty_  (defExecPenalty ()),
      effConsRate_  (myProblem (), 0.0)
   {
   }

//------------------------------------------------------------------------------

void WitConsEntry::recountConsEntry ()
   {
   recountBillEntry ();

   mappingIndex_ = myCompMgr ()->nConsEntries ();
   }

//------------------------------------------------------------------------------

void WitConsEntry::writeDataAttrs ()
   {
   WitBillEntry::writeDataAttrs ();

   myDataWriter ()->writeVector (
      "consRate",
      consRate_,
      defConsRate ());

   myDataWriter ()->writeDouble (
      "falloutRate",
      falloutRate_,
      defFalloutRate ());

   myDataWriter ()->writeDouble (
      "execPenalty",
      execPenalty_,
      defExecPenalty ());
   }

//------------------------------------------------------------------------------

void WitConsEntry::copyAttrsFrom (WitConsEntry * theConsEnt)
   {
   WitBillEntry::copyAttrsFrom (theConsEnt);

   consRate_    = theConsEnt->consRate_;
   falloutRate_ = theConsEnt->falloutRate_;
   execPenalty_ = theConsEnt->execPenalty_;
   }

//------------------------------------------------------------------------------

void WitConsEntry::display ()
   {
   myMsgFac () ("consEntryDdMsg", 
      mappingIndex_,
      falloutRate_,
      execPenalty_);

   myProblem ()->display ("consRateDdMsg", consRate_);

   if (myPreprocessor ()->preprocessed ())
      myProblem ()->display ("effConsRateDdMsg", effConsRate_);

   WitBillEntry::display ();
   }
