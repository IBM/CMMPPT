//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "CompMgr.C"
//
// Contains the implementation of the following classes:
//
//    CompMgr.
//    DelCompItr.
//------------------------------------------------------------------------------

#include <CompMgr.h>
#include <DelCompItr.h>
#include <NodeTable.h>
#include <GlobalComp.h>
#include <OptComp.h>
#include <Material.h>
#include <Capacity.h>
#include <Operation.h>
#include <SubEntry.h>
#include <BopEntry.h>
#include <Demand.h>
#include <BoundSet.h>
#include <HeurAllMgr.h>
#include <Pre.h>
#include <PipMgr.h>
#include <ApiMgr.h>
#include <MsgFac.h>
#include <PtrVec.h>

//------------------------------------------------------------------------------
// Implementation of class CompMgr.
//------------------------------------------------------------------------------

WitCompMgr::WitCompMgr (WitProblem * theProblem):

      myProblem_        (theProblem),
      myMsgFac_         (theProblem->myMsgFac ()),

      myGlobalComp_     (NULL),
      myOptComp_        (NULL),
      allParts_         (),
      allOperations_    (),

      allComponents_    (),
      myPartTable_      (NULL),
      myOpnTable_       (NULL),

      nParts_           (0),
      nMaterials_       (0),
      nOperations_      (0),
      nBomEntries_      (0),
      nSubEntries_      (0),
      nBopEntries_      (0),
      nDemands_         (0),
      nBoundSets_       (0),

      cachedDemand_     (NULL),
      cachedBomEntry_   (NULL),
      cachedSubEntry_   (NULL),
      cachedBopEntry_   (NULL),

      myDelCompItr_     (NULL),
      purging_          (false),
      recountingComp_   (NULL),
      deletingComp_     (NULL),
      clearingDepLists_ (false),
      settingNodeName_  (false)
   {
   myPartTable_   = new WitDerNodeTable <WitPart>      (myProblem_);
   myOpnTable_    = new WitDerNodeTable <WitOperation> (myProblem_);
   myDelCompItr_  = new WitDelCompItr                  (myProblem_);
   }

//------------------------------------------------------------------------------

WitCompMgr::~WitCompMgr ()
   {
   delete myDelCompItr_;
   delete myPartTable_;
   delete myOpnTable_;

   myPartTable_ = NULL;
   myOpnTable_  = NULL;

   clearCaches ();
   deleteComps ();
   }

//------------------------------------------------------------------------------

void WitCompMgr::insert (WitGlobalComp * theGlobalComp)
   {
   insertComp (theGlobalComp);

   witAssert (myGlobalComp_ == NULL);

   myGlobalComp_ = theGlobalComp;
   }

//------------------------------------------------------------------------------

void WitCompMgr::insert (WitOptComp * theOptComp)
   {
   insertComp (theOptComp);

   witAssert (myOptComp_ == NULL);

   myOptComp_ = theOptComp;
   }

//------------------------------------------------------------------------------

void WitCompMgr::insert (WitMaterial * theMat)
   {
   insertPart (theMat);

   ++ nMaterials_;
   }

//------------------------------------------------------------------------------

void WitCompMgr::insert (WitCapacity * theCap)
   {
   insertPart (theCap);
   }

//------------------------------------------------------------------------------

void WitCompMgr::insert (WitOperation * theOpn)
   {
   insertComp            (theOpn);
   myOpnTable_  ->insert (theOpn);
   allOperations_.append (theOpn);

   ++ nOperations_;
   }

//------------------------------------------------------------------------------

void WitCompMgr::insert (WitBomEntry * theBomEnt)
   {
   insertComp (theBomEnt);

   cachedBomEntry_ = theBomEnt;

   ++ nBomEntries_;
   }

//------------------------------------------------------------------------------

void WitCompMgr::insert (WitSubEntry * theSub)
   {
   insertComp (theSub);

   cachedSubEntry_ = theSub;

   ++ nSubEntries_;
   }

//------------------------------------------------------------------------------

void WitCompMgr::insert (WitBopEntry * theBopEnt)
   {
   insertComp (theBopEnt);

   cachedBopEntry_ = theBopEnt;

   ++ nBopEntries_;
   }

//------------------------------------------------------------------------------

void WitCompMgr::insert (WitDemand * theDemand)
   {
   insertComp (theDemand);

   cachedDemand_ = theDemand;

   ++ nDemands_;
   }

//------------------------------------------------------------------------------

WitPart * WitCompMgr::myMatchFor (WitPart * thePart)
   {
   WitPart * matchPart;

   matchPart = findPart (thePart->partName ().myCstring ());

   witAssert (matchPart != NULL);

   return matchPart;
   }

//------------------------------------------------------------------------------

WitMaterial * WitCompMgr::myMatchFor (WitMaterial * theMat)
   {
   WitPart *     thePart;
   WitMaterial * matchMat;

   thePart   = theMat;

   matchMat  = myMatchFor (thePart)->thisMat ();

   witAssert (matchMat != NULL);

   return matchMat;
   }

//------------------------------------------------------------------------------

WitDemand * WitCompMgr::myMatchFor (WitDemand * theDemand)
   {
   WitPart *    matchPart;
   const char * theDemName;
   WitDemand *  matchDemand;

   matchPart   = myMatchFor (theDemand->demandedPart ());

   theDemName  = theDemand->demandName ().myCstring ();

   matchDemand = findDemand (matchPart, theDemName);

   witAssert (matchDemand != NULL);

   return matchDemand;
   }

//------------------------------------------------------------------------------

WitOperation * WitCompMgr::myMatchFor (WitOperation * theOpn)
   {
   WitOperation * matchOperation;

   matchOperation = findOperation (theOpn->operationName ().myCstring ());

   witAssert (matchOperation != NULL);

   return matchOperation;
   }

//------------------------------------------------------------------------------

WitSubEntry * WitCompMgr::myMatchFor (WitSubEntry * theSub)
   {
   WitOperation * matchOperation;
   WitBomEntry *  matchBomEnt;
   WitSubEntry *  matchSub;

   matchOperation = myMatchFor (theSub->myOperation ());

   matchBomEnt    = findBomEntry (matchOperation, theSub->myBomEntIndex ());

   witAssert (matchBomEnt != NULL);

   matchSub       = findSubEntry (matchBomEnt,    theSub->localIndex ());

   witAssert (matchSub    != NULL);

   return matchSub;
   }

//------------------------------------------------------------------------------

WitBoundSet * WitCompMgr::myMatchFor (WitBoundSet * theBoundSet)
   {
   return theBoundSet->myMatchIn (this);
   }

//------------------------------------------------------------------------------

WitPart * WitCompMgr::findPart (const char * theName) const
   {
   return myPartTable_->findDerNode (theName);
   }

//------------------------------------------------------------------------------

WitOperation * WitCompMgr::findOperation (const char * theName) const
   {
   return myOpnTable_->findDerNode (theName);
   }

//------------------------------------------------------------------------------

WitBomEntry * WitCompMgr::findBomEntry (WitOperation * theOpn, int theIndex)
   {
   WitBomEntry * theBomEnt;

   witAssert (theOpn->myCompMgr () == this);

   if (cachedBomEntry_ != NULL)
      if (   cachedBomEntry_->myOperation () == theOpn)
         if (cachedBomEntry_->localIndex  () == theIndex)
            return cachedBomEntry_;

   forEachEl (theBomEnt, theOpn->bom ())
      if (theBomEnt->localIndex () == theIndex)
         {
         cachedBomEntry_ = theBomEnt;

         return cachedBomEntry_;
         }

   return NULL;
   }

//------------------------------------------------------------------------------

WitSubEntry * WitCompMgr::findSubEntry (WitBomEntry * theBomEnt, int theIndex)
   {
   WitSubEntry * theSub;

   witAssert (theBomEnt->myCompMgr () == this);

   if (cachedSubEntry_ != NULL)
      if (   cachedSubEntry_->myBomEnt () == theBomEnt)
         if (cachedSubEntry_->localIndex () == theIndex)
            return cachedSubEntry_;

   forEachEl (theSub, theBomEnt->mySubEntries ())
      if (theSub->localIndex () == theIndex)
         {
         cachedSubEntry_ = theSub;

         return cachedSubEntry_;
         }

   return NULL;
   }

//------------------------------------------------------------------------------

WitBopEntry * WitCompMgr::findBopEntry (WitOperation * theOpn, int theIndex)
   {
   WitBopEntry * theBopEnt;

   witAssert (theOpn->myCompMgr () == this);

   if (cachedBopEntry_ != NULL)
      if (   cachedBopEntry_->myOperation () == theOpn)
         if (cachedBopEntry_->localIndex  () == theIndex)
            return cachedBopEntry_;

   forEachEl (theBopEnt, theOpn->bop ())
      if (theBopEnt->localIndex () == theIndex)
         {
         cachedBopEntry_ = theBopEnt;

         return cachedBopEntry_;
         }

   return NULL;
   }

//------------------------------------------------------------------------------

WitDemand * WitCompMgr::findDemand (WitPart * thePart, const char * theName)
   {
   WitDemand * theDemand;

   witAssert (thePart->myCompMgr () == this);

   if (cachedDemand_ != NULL)
      if (cachedDemand_->demandedPart () == thePart)
         if (cachedDemand_->demandName () == theName)
            return cachedDemand_;

   cachedDemand_ =
      thePart->myDemands ().find (& WitDemand::demandName, theName);

   return cachedDemand_;
   }

//------------------------------------------------------------------------------

void WitCompMgr::recountGlobalComp (WitGlobalComp * theGlobalComp)
   {
   recountComponent (theGlobalComp);
   }

//------------------------------------------------------------------------------

void WitCompMgr::recountOptComp (WitOptComp * theOptComp)
   {
   recountComponent (theOptComp);
   }

//------------------------------------------------------------------------------

void WitCompMgr::recountMaterial (WitMaterial * theMat)
   {
   recountPart (theMat);

   ++ nMaterials_;
   }

//------------------------------------------------------------------------------

void WitCompMgr::recountCapacity (WitCapacity * theCap)
   {
   recountPart (theCap);
   }

//------------------------------------------------------------------------------

void WitCompMgr::recountOperation (WitOperation * theOpn)
   {
   recountComponent (theOpn);

   ++ nOperations_;
   }

//------------------------------------------------------------------------------

void WitCompMgr::recountBomEntry (WitBomEntry * theBomEnt)
   {
   recountConsEntry (theBomEnt);

   ++ nBomEntries_;
   }

//------------------------------------------------------------------------------

void WitCompMgr::recountSubEntry (WitSubEntry * theSub)
   {
   recountConsEntry (theSub);

   ++ nSubEntries_;
   }

//------------------------------------------------------------------------------

void WitCompMgr::recountBopEntry (WitBopEntry * theBopEnt)
   {
   recountComponent (theBopEnt);

   ++ nBopEntries_;
   }

//------------------------------------------------------------------------------

void WitCompMgr::recountDemand (WitDemand * theDemand)
   {
   recountComponent (theDemand);

   ++ nDemands_;
   }

//------------------------------------------------------------------------------

void WitCompMgr::setPartName (WitPart * thePart, const char * theName)
   {
   myPartTable_->remove (thePart);

   setNodeName (thePart, theName);

   myPartTable_->insert (thePart);
   }

//------------------------------------------------------------------------------

void WitCompMgr::setOperationName (WitOperation * theOpn, const char * theName)
   {
   myOpnTable_->remove (theOpn);

   setNodeName (theOpn, theName);

   myOpnTable_->insert (theOpn);
   }

//------------------------------------------------------------------------------

int WitCompMgr::nBoundSetsSpecified () const
   {
   int            nSpec;
   WitComponent * theComp;
   WitBoundSet *  theBoundSet;

   nSpec = 0;

   forEachEl (theComp, allComponents ())
      {
      theBoundSet = theComp->myBoundSet ();

      if (theBoundSet != NULL)
         if (not theBoundSet->isAtDefault ())
            ++ nSpec;
      }

   return nSpec;
   }

//------------------------------------------------------------------------------

void WitCompMgr::purgeData ()
   {
   WitComponent * theComp;
   WitDelComp   * theDelComp;
   int            nUserSel;
   int            nTotSel;
   int            nDelCompsBefore;

   WitList <WitComponent> remainingComps;
      //
      // The List of all Components not deleted by this purge.

   WitList <WitComponent> selComps;
      //
      // The List of all Components to be deleted, in reverse creation order.

   if (myDelCompItr_->active ())
      myMsgFac () ("purgeWhileItrActSmsg");

   if (myGlobalComp ()->perfPegging ())
      myProblem ()->myHeurAllMgr ()->clearPegging ();

   myProblem ()->myPipMgr ()->clearShipSeq ();

   myGlobalComp ()->clearPgdCritList ();

   myMsgFac () ("purgeDataMsg");
   myMsgFac ().divide ();

   nUserSel = nSelForDel ();

   if (nUserSel == 0)
      {
      myMsgFac () ("noSelForDelsMsg");
      myMsgFac ().divide ();

      return;
      }

   myProblem ()->myPreprocessor ()->unpreprocess ();

   nDelCompsBefore = nDelComps ();

   purging_        = true;

   forEachEl (theComp, allComponents ())
      {
      theDelComp = theComp->thisDelComp ();

      if (theDelComp != NULL)
         theDelComp->identifyOrAutoSel ();
      }

   nTotSel = nSelForDel ();

   myMsgFac () ("delCompsMsg", nUserSel, nTotSel - nUserSel, nTotSel);

   forEachEl (theComp, allComponents ())
      theComp->purgeCompLists ();

   purgeCompLists ();

   clearCaches ();

   forEachEl (theComp, allComponents ())
      {
      theDelComp = theComp->thisDelComp ();

      if ((theDelComp != NULL) and theDelComp->selForDel ())
         selComps.prepend (theComp);
      else
         remainingComps.append (theComp);
      }

   allComponents_ = remainingComps;

   while (not selComps.isEmpty ())
      deleteComp (selComps.get ());

   WitBillEntry::reLocalIndex (myProblem ());

   recountComps ();

   purging_ = false;

   witAssert (nDelCompsBefore - nTotSel == nDelComps ());

   myMsgFac () ("purgeCompleteMsg",
      nDelCompsBefore,
      nTotSel,
      nDelComps ());
   }

//------------------------------------------------------------------------------

bool WitCompMgr::isReindexing (WitBoundSet * theBoundSet) const
   {
   if (recountingComp_ != NULL)
      if (theBoundSet == recountingComp_->myBoundSet ())
         return true;

   return false;
   }

//------------------------------------------------------------------------------

bool WitCompMgr::isDeleting (WitComponent * theComp) const
   {
   return (theComp == deletingComp_);
   }

//------------------------------------------------------------------------------

void WitCompMgr::storePreSortedNodes (const WitList <WitNode> & theNodes)
   {
   WitNode *      theNode;
   WitPart *      thePart;
   WitOperation * theOpn;

   witAssert (myProblem ()->myPreprocessor ()->preprocessing ());

   allParts_     .clear ();
   allOperations_.clear ();

   forEachEl (theNode, theNodes)
      {     
      thePart = theNode->thisPart ();

      if (thePart != NULL)
         allParts_.append (thePart);

      theOpn = theNode->thisOpn ();

      if (theOpn != NULL)
         allOperations_.append (theOpn);
      }
   }

//------------------------------------------------------------------------------

void WitCompMgr::recountComponent (WitComponent * theComp)
   {
   witAssert (theComp == recountingComp_);

   recountingComp_ = NULL;
      //
      // Prevents this function from being called more than once for the same
      // Component.
   }

//------------------------------------------------------------------------------

void WitCompMgr::recountPart (WitPart * thePart)
   {
   recountComponent (thePart);

   ++ nParts_;
   }

//------------------------------------------------------------------------------

void WitCompMgr::recountBillEntry (WitBillEntry * theBillEnt)
   {
   recountComponent (theBillEnt);
   }

//------------------------------------------------------------------------------

void WitCompMgr::recountConsEntry (WitConsEntry * theConsEnt)
   {
   recountBillEntry (theConsEnt);
   }

//------------------------------------------------------------------------------

void WitCompMgr::insertPart (WitPart * thePart)
   {
   insertComp           (thePart);
   myPartTable_->insert (thePart);
   allParts_.    append (thePart);

   ++ nParts_;
   }

//------------------------------------------------------------------------------

void WitCompMgr::insertComp (WitComponent * theComp)
   {
   witAssert (theComp->myCompMgr () == this);

   witAssert (not theComp->inserted ());

   allComponents_.append (theComp);

   if (theComp->myBoundSet () != NULL)
      ++ nBoundSets_;
   }

//------------------------------------------------------------------------------

void WitCompMgr::setNodeName (WitNode * theNode, const char * theName)
   {
   witAssert (theName != NULL);

   witAssert (not isNullCstring (theName));

   myProblem ()->prepSetNonUnpostAttr ();

   settingNodeName_ = true;

   theNode->storeCompMgrNodeName (theName);

   settingNodeName_ = false;
   }

//------------------------------------------------------------------------------

int WitCompMgr::nSelForDel () const
   {
   WitComponent * theComp;
   WitDelComp   * theDelComp;
   int            nSel;

   nSel = 0;

   forEachEl (theComp, allComponents ())
      {
      theDelComp = theComp->thisDelComp ();

      if (theDelComp != NULL)
         if (theDelComp->selForDel ())
            ++ nSel;
      }

   return nSel;
   }

//------------------------------------------------------------------------------

void WitCompMgr::purgeCompLists ()
   {
   myPartTable_->purge ();

   WitDelComp::purgeSelComps (allParts_);

   myOpnTable_->purge ();

   WitDelComp::purgeSelComps (allOperations_);
   }

//------------------------------------------------------------------------------

void WitCompMgr::clearCaches ()
   {
   cachedDemand_   = NULL;
   cachedBomEntry_ = NULL;
   cachedSubEntry_ = NULL;
   cachedBopEntry_ = NULL;
   }

//------------------------------------------------------------------------------

void WitCompMgr::recountComps ()
   {
   WitComponent * theComp;

   nParts_      = 0;
   nMaterials_  = 0;
   nOperations_ = 0;
   nBomEntries_ = 0;
   nSubEntries_ = 0;
   nBopEntries_ = 0;
   nDemands_    = 0;
   nBoundSets_  = 0;

   forEachEl (theComp, allComponents ())
      {
      recountingComp_ = theComp;

      if (theComp->myBoundSet () != NULL)
         {
         theComp->reindexBoundSet ();

         ++ nBoundSets_;
         }

      theComp->recount ();

      witAssert (recountingComp_ == NULL);
      }

   if (nParts_ + nOperations_ == 0)
      myProblem ()->clearTVecs ();
   }

//------------------------------------------------------------------------------

void WitCompMgr::deleteComps ()
   {
   WitPtrVec <WitComponent> indexedComp;
   WitComponent *           theComp;
   int                      i;


   indexedComp.resize (allComponents ().nElements ());

   i = -1;

   clearingDepLists_ = true;

   forEachEl (theComp, allComponents ())
      {
      theComp->clearDepLists ();

      indexedComp[++ i] = theComp;
      }

   clearingDepLists_ = false;

   allComponents_.clear ();

   for (i = indexedComp.length () - 1; i >= 0; -- i)
      {
      deleteComp (indexedComp[i]);

      indexedComp[i] = NULL;
      }
   }

//------------------------------------------------------------------------------

void WitCompMgr::deleteComp (WitComponent * theComp)
   {
   deletingComp_ = theComp;
   delete          theComp;
   deletingComp_ = NULL;
   }

//------------------------------------------------------------------------------
// Implementation of class DelCompItr.
//------------------------------------------------------------------------------

WitDelCompItr::WitDelCompItr (WitProblem * theProblem):
      WitProbAssoc    (theProblem),

      myDelCompStack_ (theProblem),
      myStItr_        (),
      curDelComp_     (NULL)
   {
   myDelCompStack_.attachItr (myStItr_);
   }

//------------------------------------------------------------------------------

WitDelCompItr::~WitDelCompItr ()
   {
   }

//------------------------------------------------------------------------------

void WitDelCompItr::reset ()
   {
   curDelComp_ = NULL;

   myDelCompStack_.attachItr (myStItr_);

   myDelCompStack_.clear ();
   }

//------------------------------------------------------------------------------

void WitDelCompItr::advance ()
   {
   bool foundDelComp;

   if (curDelComp_ == NULL)
      setup ();

   foundDelComp = myStItr_.advance (curDelComp_);

   if (foundDelComp)
      {
      if (myMsgFac ().mayIssue ("advancedToObjMsg"))
         myMsgFac () ("advancedToObjMsg");

      curDelComp_->identify ();
      }
   else
      {
      if (myMsgFac ().mayIssue ("advancedToEndMsg"))
         myMsgFac () ("advancedToEndMsg");

      reset ();
      }
   }

//------------------------------------------------------------------------------

WitAttr WitDelCompItr::apiObjItrState ()
   {
   return
      active ()?
         curDelComp ()->apiObjItrState ():
         WitINACTIVE;
   }

//------------------------------------------------------------------------------

void WitDelCompItr::setup ()
   {
   WitComponent * theComp;
   WitDelComp *   theDelComp;

   stronglyAssert (myDelCompStack_.isEmpty ());

   forEachEl (theComp, myCompMgr ()->allComponents ())
      {
      theDelComp = theComp->thisDelComp ();

      if (theDelComp != NULL)
         myDelCompStack_.push (theDelComp);
      }

   myDelCompStack_.reverse ();

   myDelCompStack_.attachItr (myStItr_);
   }
