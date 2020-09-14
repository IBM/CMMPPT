//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "Misc.C"
//
// Contains the implementation of the following classes and templates:
//
//    ProbAssoc
//    Variant
//    NodeTable
//    ExecPerSched <Entry>.
//    OptStarter
//------------------------------------------------------------------------------

#include <Assoc.h>
#include <Session.h>
#include <wit/src/Variant.h>
#include <NodeTable.h>
#include <ExecPerSch.h>
#include <OptStarter.h>
#include <GlobalComp.h>
#include <OptComp.h>
#include <Node.h>
#include <BopEntry.h>
#include <FSS.h>
#include <MsgFac.h>
#include <ObjStack.h>

//------------------------------------------------------------------------------
// Implementation of class ProbAssoc.
//------------------------------------------------------------------------------

FILE * WitProbAssoc::msgFile () const
   {
   return myMsgFac ().myFile ();
   }

//------------------------------------------------------------------------------

bool WitProbAssoc::tempParIsSet (int index)
   {
   return myGlobalComp ()->tempParIsSet (index);
   }

//------------------------------------------------------------------------------

FILE * WitProbAssoc::openFile (const char * fileName, const char * mode)
   {
   return myProblem_->mySession ()->openFile (fileName, mode);
   }

//------------------------------------------------------------------------------

bool WitProbAssoc::fssComputed () const
   {
   return myProblem_->myFSS ()->computed ();
   }

//------------------------------------------------------------------------------

bool WitProbAssoc::selPrintLevelAt (int theLevel)
   {
   return (myGlobalComp ()->selPrintLevel () >= theLevel);
   }

//------------------------------------------------------------------------------

WitProbAssoc::WitProbAssoc (WitProblem * theProblem):

      myProblem_ (NULL)
   {
   witAssert (theProblem != NULL);

   myProblem_ = theProblem;
   }

//------------------------------------------------------------------------------

WitProbAssoc::WitProbAssoc (const WitProbAssoc * theProbAssoc):

      myProblem_ (theProbAssoc->myProblem_)
   {
   }

//------------------------------------------------------------------------------

WitProbAssoc::~WitProbAssoc ()
   {
   }

//------------------------------------------------------------------------------
// Implementation of class Variant
//------------------------------------------------------------------------------

WitVariant::WitVariant (WitProblem * theProblem):

      myProblem_        (theProblem),
      myFragID_         (""),
      internalSoln_     (false),
      compsSecScheds_   (false),
      respectsLotSizes_ (false),
      respectsOptCons_  (false)
   {
   myProblem ()->store (this);
   }

//------------------------------------------------------------------------------

WitVariant::~WitVariant ()
   {
   }

//------------------------------------------------------------------------------

void WitVariant::configure ()
   {
   bool isOpt;
   bool isExtOpt;
   bool isHeur;
   bool isHeurOpt;
   bool isInput;

   isOpt     = (this == myProblem ()->optVariant     ());
   isExtOpt  = (this == myProblem ()->extOptVariant  ());
   isHeur    = (this == myProblem ()->heurVariant    ());
   isHeurOpt = (this == myProblem ()->heurOptVariant ());
   isInput   = (this == myProblem ()->inputVariant   ());

   myFragID_ =
      isOpt?     "optImpFrag":
      isExtOpt?  "extOptImpFrag":
      isHeur?    "heurImpAllFrag":
      isHeurOpt? "heurOptImpFrag":
      isInput?   "userInputFrag":
                 "";

   internalSoln_     = isOpt or isExtOpt or isHeur or isHeurOpt;
   compsSecScheds_   = isOpt or isExtOpt;
   respectsLotSizes_ =                      isHeur              or isInput;
   respectsOptCons_  = isOpt or isExtOpt                        or isInput;
   }

//------------------------------------------------------------------------------

void WitVariant::becomesCurrent ()
   {
   myProblem ()->makeCurrent (this);
   }

//------------------------------------------------------------------------------

bool WitVariant::isCurrent ()
   {
   return (this == myProblem ()->currentVariant ());
   }

//------------------------------------------------------------------------------
// Implementation of class NodeTable.
//------------------------------------------------------------------------------

void WitNodeTable::remove (WitNode * theNode)
   {
   const char *         theName;
   WitNodeMap::iterator theItr;

   cachedNode_ = NULL;

   theName     = theNode->nodeName ().myCstring ();

   theItr      = myNodeMap_.find (theName);

   witAssert (theItr != myNodeMap_.end ());

   myNodeMap_.erase (theItr);
   }

//------------------------------------------------------------------------------

void WitNodeTable::purge ()
   {
   WitNodeMap::iterator theItr;

   cachedNode_ = NULL;

   theItr      = myNodeMap_.begin ();

   while (theItr != myNodeMap_.end ())
      {
      if (theItr->second->selForDel ())
         myNodeMap_.erase (theItr ++);
      else
         theItr ++;
      }
   }

//------------------------------------------------------------------------------

WitNodeTable::WitNodeTable (WitProblem * theProblem):

      WitProbAssoc (theProblem),

      myNodeMap_   (),
      cachedNode_  (NULL)
   {
   }

//------------------------------------------------------------------------------

WitNodeTable::~WitNodeTable ()
   {
   }

//------------------------------------------------------------------------------

void WitNodeTable::insertNode (WitNode * theNode)
   {
   const char * theName;

   theName = theNode->nodeName ().myCstring ();

   witAssert (findNode (theName) == NULL);

   myNodeMap_[theName] = theNode;

   cachedNode_         = theNode;
   }

//------------------------------------------------------------------------------

WitNode * WitNodeTable::findNode (const char * theName)
   {
   WitNodeMap::iterator theItr;

   if (cachedNode_ != NULL)
      if (cachedNode_->nodeName () == theName)
         return cachedNode_;

   theItr = myNodeMap_.find (theName);

   if (theItr == myNodeMap_.end ())
      return NULL;

   cachedNode_ = theItr->second;

   return theItr->second;
   }

//------------------------------------------------------------------------------
// Implementation of class template ExecPerSched.
//------------------------------------------------------------------------------

template <typename Entry>
      WitExecPerSched <Entry>::WitExecPerSched (WitProblem * theProblem):

         WitProbAssoc  (theProblem),
         firstExecPer_ (theProblem, -1),
         lastExecPer_  (theProblem, -2)
   {
   WitObjStack <Entry> theEntries (myProblem ());
   Entry *             theEntry;
   WitPeriod           execPer;
   WitPeriod           impactPer;

   Entry::getAllInstances (myProblem (), theEntries);

   while (theEntries.pop (theEntry))
      forEachPeriod (execPer, myProblem ())
         {
         if (not theEntry->inEffect (execPer))
            continue;

         impactPer = theEntry->impactPeriod ()[execPer];

         if (firstExecPer_ (theEntry)[impactPer] == -1)
             firstExecPer_ (theEntry)[impactPer] = execPer;

         lastExecPer_      (theEntry)[impactPer] = execPer;
         }
   }

//------------------------------------------------------------------------------

template <typename Entry>
      WitExecPerSched <Entry>::~WitExecPerSched ()
   {
   }

//------------------------------------------------------------------------------

template <typename Entry>
      void WitExecPerSched <Entry>::getExecPerRange (
         Entry *     theEntry,
         WitPeriod   impactPer,
         WitPeriod & execPerFirst,
         WitPeriod & execPerLast)
   {
   execPerFirst = firstExecPer_ (theEntry)[impactPer];
   execPerLast  = lastExecPer_  (theEntry)[impactPer];
   }

//------------------------------------------------------------------------------
// Explicit instantiation of class template ExecPerSched <Entry>.
//------------------------------------------------------------------------------

template class WitExecPerSched <WitBillEntry>;
template class WitExecPerSched <WitBopEntry>;

//------------------------------------------------------------------------------
// Implementation of class OptStarter.
//------------------------------------------------------------------------------

WitOptStarter::WitOptStarter (
         WitOptComp * theOptComp,
         const char * theParamValue,
         WitAttr      theApiAttr,
         WitMsgID     theMsgID,
         bool         externalVal):

      myOptComp_  (theOptComp),
      paramValue_ (theParamValue),
      myApiAttr_  (theApiAttr),
      myMsgID_    (theMsgID),
      external_   (externalVal)
   {
   myOptComp ()->store (this);
   }

//------------------------------------------------------------------------------

WitOptStarter::~WitOptStarter ()
   {
   }

//------------------------------------------------------------------------------

void WitOptStarter::beChosen ()
   {
   myOptComp ()->choose (this);
   }

//------------------------------------------------------------------------------

bool WitOptStarter::isChosen ()
   {
   return (this == myOptComp ()->optInitMethod ());
   }

//------------------------------------------------------------------------------

const WitString & WitOptStarter::paramValue (
      const WitOptStarter * theOptStarter)
   {
   return theOptStarter->paramValue ();
   }

//------------------------------------------------------------------------------

void WitOptStarter::display ()
   {
   myOptComp ()->myMsgFac () (myMsgID_);
   }
