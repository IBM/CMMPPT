//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "ScenAtt.C"
//
// Contains the implementation of the following classes and templates:
//
//    AbsScenInputAtt
//    ScenInputAtt <Owner>
//    AbsScenSolnAtt
//    ScenSolnAtt  <Owner>
//    ScenAtt      <Owner>
//------------------------------------------------------------------------------

#include <ScenAttMgr.h>
#include <ScenInputAtt.h>
#include <ScenSolnAtt.h>
#include <ScenAtt.h>
#include <StochImpMgr.h>
#include <ScenMgr.h>
#include <Scenario.h>
#include <StochAttMgr.h>
#include <Stage.h>
#include <StageMgr.h>
#include <StochOptMgr.h>
#include <DataWrit.h>
#include <CompMgr.h>
#include <Material.h>
#include <Demand.h>
#include <Operation.h>
#include <SubEntry.h>
#include <BoundSet.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class AbsScenInputAtt.
//------------------------------------------------------------------------------

WitAbsScenInputAtt::~WitAbsScenInputAtt ()
   {
   }

//------------------------------------------------------------------------------

WitAbsScenInputAtt::WitAbsScenInputAtt (WitScenAttMgr * theScenAttMgr):

      WitStochAssoc (theScenAttMgr)
   {
   theScenAttMgr->store (this);
   }

//------------------------------------------------------------------------------
// Implementation of class ScenInputAtt <Owner>.
//------------------------------------------------------------------------------

template <typename Owner>
      WitScenInputAtt <Owner>::WitScenInputAtt (
         WitScenAttMgr *                     theScenAttMgr,
         const char *                        theAttName,
         double                              theDefaultVal,
         const WitDblFlexVec & (Owner::*     theAccFunc) () const,
         void (Owner::*                      theStoreFunc) (
                                                const WitDblFlexVec &),
         WitScenInputAtt * (WitScenAttMgr::* theInstFunc) () const):

         WitAbsScenInputAtt (theScenAttMgr),
         myScenAtt_         (NULL),
         myAttName_         (theAttName),
         myDefaultVal_      (theDefaultVal),
         myAccFunc_         (theAccFunc),
         myStoreFunc_       (theStoreFunc),
         myInstFunc_        (theInstFunc)
   {
   myStochModeMgr ()->
      myStochAttMgr ()->
         makeScenInputStochAtt (myAttName_, this);
   }

//------------------------------------------------------------------------------

template <typename Owner>
      const WitDblFlexVec & WitScenInputAtt <Owner>::myValueFor (
         Owner *       theOwner,
         WitScenario * theScenario)
   {
   if (isDiffedAt (theOwner))
      return myScenAtt_->myValueFor (theOwner, theScenario);
   else
      return (theOwner->*myAccFunc_) ();
   }

//------------------------------------------------------------------------------

template <typename Owner>
      WitScenInputAtt <Owner>::~WitScenInputAtt ()
   {
   delete myScenAtt_;
   }

//------------------------------------------------------------------------------
// Function ScenInputAtt <Owner>::prepSet (WitDelComp *).
// Specialized to Owner = BoundSet vs. anything else.
//------------------------------------------------------------------------------

template <typename Owner>
      void WitScenInputAtt <Owner>::prepSet (WitDelComp * theDelComp)
   {
   Owner * theOwner;

   theOwner = theOwnerFor (theDelComp);

   witAssert (theOwner != NULL);

   prepSetForOwner (theOwner);
   }

//------------------------------------------------------------------------------

template <>
      void WitScenInputAtt <WitBoundSet>::prepSet (WitDelComp *)
   {
   stronglyAssert (false);
   }

//------------------------------------------------------------------------------
// Function ScenInputAtt <Owner>::prepSet (WitBoundSet *).
// Specialized to Owner = BoundSet vs. anything else.
//------------------------------------------------------------------------------

template <typename Owner>
      void WitScenInputAtt <Owner>::prepSet (WitBoundSet *)
   {
   stronglyAssert (false);
   }

//------------------------------------------------------------------------------

template <>
      void WitScenInputAtt <WitBoundSet>::prepSet (WitBoundSet * theBoundSet)
   {
   prepSetForOwner (theBoundSet);
   }

//------------------------------------------------------------------------------

template <typename Owner>
      void WitScenInputAtt <Owner>::downloadData (WitScenario * theScenario)
   {
   Owner * theOwner;

   if (myScenAtt_ != NULL)
      forEachEl (theOwner, myScenAtt_->allOwners ())
         if (myScenAtt_->isAllocatedFor (theOwner))
             myScenAtt_->myValueFor     (theOwner, theScenario) =
               (theOwner->*myAccFunc_) ();
   }

//------------------------------------------------------------------------------

template <typename Owner>
      void WitScenInputAtt <Owner>::uploadData (WitScenario * theScenario)
   {
   Owner * theOwner;

   if (myScenAtt_ != NULL)
      forEachEl (theOwner, myScenAtt_->allOwners ())
         if (myScenAtt_->isAllocatedFor (theOwner))
            (theOwner->*myStoreFunc_) (myValueFor (theOwner, theScenario));
   }

//------------------------------------------------------------------------------

template <typename Owner>
      void WitScenInputAtt <Owner>::copyDataFrom (WitScenAttMgr * srcScenAttMgr)
   {
   Owner *              dstOwner;
   WitScenario *        dstScenario;
   WitScenInputAtt *    srcScenInputAtt;
   WitScenAtt <Owner> * srcScenAtt;
   Owner *              srcOwner;
   WitScenario *        srcScenario;
   int                  theScenIdx;

   witAssert (myScenAtt_ == NULL);

   srcScenInputAtt = (srcScenAttMgr->*myInstFunc_) ();

   srcScenAtt      = srcScenInputAtt->myScenAtt_;

   if (srcScenAtt == NULL)
      return;

   myScenAtt_ = new WitScenAtt <Owner> (myScenAttMgr ());

   forEachEl (dstOwner, myScenAtt_->allOwners ())
      {
      srcOwner = srcScenAttMgr->myCompMgr ()->myMatchFor (dstOwner);

      if (not srcScenAtt->isAllocatedFor (srcOwner))
         continue;

      myScenAtt_->allocateFor (dstOwner);

      forEachEl (dstScenario, myScenMgr ()->myScenarios ())
         {
         theScenIdx  = dstScenario->myIndex ();

         srcScenario =
            srcScenInputAtt->myScenMgr ()->myScenarioFor (theScenIdx);

         myScenAtt_   ->myValueFor (dstOwner, dstScenario) =
            srcScenAtt->myValueFor (srcOwner, srcScenario);
         }
      }
   }

//------------------------------------------------------------------------------
// Function
//    ScenInputAtt <Owner>::writeData (Component *, Scenario *, bool, bool &).
// Specialized to Owner = BoundSet vs. anything else.
//------------------------------------------------------------------------------

template <typename Owner>
      void WitScenInputAtt <Owner>::writeData (
         WitComponent * theComp,
         WitScenario *  theScenario,
         bool           diffedSec,
         bool &         attWritten)
   {
   Owner * theOwner;

   theOwner = theOwnerFor (theComp);

   if (theOwner == NULL)
      return;

   writeDataForOwner (theOwner, theScenario, diffedSec, attWritten);
   }

//------------------------------------------------------------------------------

template <>
      void WitScenInputAtt <WitBoundSet>::writeData (
         WitComponent *,
         WitScenario *,
         bool,
         bool &)
   {
   stronglyAssert (false);
   }

//------------------------------------------------------------------------------
// Function
//    ScenInputAtt <Owner>::writeData (BoundSet *, Scenario *, bool, bool &).
// Specialized to Owner = BoundSet vs. anything else.
//------------------------------------------------------------------------------

template <typename Owner>
      void WitScenInputAtt <Owner>::writeData (
         WitBoundSet *,
         WitScenario *,
         bool,
         bool &)
   {
   stronglyAssert (false);
   }

//------------------------------------------------------------------------------

template <>
      void WitScenInputAtt <WitBoundSet>::writeData (
         WitBoundSet * theBoundSet,
         WitScenario * theScenario,
         bool          diffedSec,
         bool &        attWritten)
   {
   writeDataForOwner (theBoundSet, theScenario, diffedSec, attWritten);
   }

//------------------------------------------------------------------------------
// Function ScenInputAtt <Owner>::ownedByBoundSet ().
// Specialized to Owner = BoundSet vs. anything else.
//------------------------------------------------------------------------------

template <typename Owner>
      bool WitScenInputAtt <Owner>::ownedByBoundSet ()
   {
   return false;
   }

//------------------------------------------------------------------------------

template <>
      bool WitScenInputAtt <WitBoundSet>::ownedByBoundSet ()
   {
   return true;
   }

//------------------------------------------------------------------------------

template <typename Owner>
      void WitScenInputAtt <Owner>::prepSetForOwner (Owner * theOwner)
   {
   WitScenario * theScenario;

   if (myScenAtt_ == NULL)
      myScenAtt_ = new WitScenAtt <Owner> (myScenAttMgr ());

   if (not myScenAtt_->isAllocatedFor (theOwner))
      {
      myScenAtt_->allocateFor (theOwner);

      forEachEl (theScenario, myScenMgr ()->myScenarios ())
         myScenAtt_->myValueFor (theOwner, theScenario) =
            (theOwner->*myAccFunc_) ();
      }
   }

//------------------------------------------------------------------------------

template <typename Owner>
      void WitScenInputAtt <Owner>::validateData ()
   {
   Owner *   theOwner;
   WitNode * theNode;
   WitPeriod thePer;

   if (myScenAtt_ == NULL)
      return;

   forEachEl (theOwner, myScenAtt_->allOwners ())
      {
      if (not myScenAtt_->isAllocatedFor (theOwner))
         continue;

      theNode = stageNode (theOwner);

      forEachPeriod (thePer, myProblem ())
         if (myStageMgr ()->myStageOf (theNode, thePer)->isStage0 ())
            checkSameInAllScenarios (theOwner, thePer);
      }
   }

//------------------------------------------------------------------------------
// Template specialized implementation of member function
// ScenInputAtt <Owner>::theOwnerFor (...).
//------------------------------------------------------------------------------

template <>
      WitPart * WitScenInputAtt <WitPart>::theOwnerFor (WitComponent * theComp)
   {
   return theComp->thisPart ();
   }

//------------------------------------------------------------------------------

template <>
      WitMaterial * WitScenInputAtt <WitMaterial>::theOwnerFor (
         WitComponent * theComp)
   {
   return theComp->thisMat ();
   }

//------------------------------------------------------------------------------

template <>
      WitDemand * WitScenInputAtt <WitDemand>::theOwnerFor (
         WitComponent * theComp)
   {
   return theComp->thisDemand ();
   }

//------------------------------------------------------------------------------

template <>
      WitOperation * WitScenInputAtt <WitOperation>::theOwnerFor (
         WitComponent * theComp)
   {
   return theComp->thisOpn ();
   }

//------------------------------------------------------------------------------

template <>
      WitSubEntry * WitScenInputAtt <WitSubEntry>::theOwnerFor (
         WitComponent * theComp)
   {
   return theComp->thisSub ();
   }

//------------------------------------------------------------------------------

template <>
      WitBoundSet * WitScenInputAtt <WitBoundSet>::theOwnerFor (WitComponent *)
   {
   stronglyAssert (false);

   return NULL;
   }

//------------------------------------------------------------------------------

template <typename Owner>
      void WitScenInputAtt <Owner>::writeDataForOwner (
            Owner *        theOwner,
            WitScenario *  theScenario,
            bool           diffedSec,
            bool &         attWritten)
   {
   if (isDiffedAt (theOwner) != diffedSec)
      return;

   if (myValueFor (theOwner, theScenario) == myDefaultVal_)
      return;

   if (not attWritten)
      theOwner->writeSetClause ();

   myDataWriter ()->
      writeVector (
         myAttName_,
         myValueFor (theOwner, theScenario),
         myDefaultVal_);

   attWritten = true;
   }

//------------------------------------------------------------------------------

template <>
      WitNode * WitScenInputAtt <WitPart>::stageNode (WitPart * thePart)
   {
   return thePart;
   }

//------------------------------------------------------------------------------

template <>
      WitNode * WitScenInputAtt <WitMaterial>::stageNode (WitMaterial * theMat)
   {
   return theMat;
   }

//------------------------------------------------------------------------------

template <>
      WitNode * WitScenInputAtt <WitDemand>::stageNode (WitDemand * theDemand)
   {
   return theDemand->demandedPart ();
   }

//------------------------------------------------------------------------------

template <>
      WitNode * WitScenInputAtt <WitOperation>::stageNode (
         WitOperation * theOpn)
   {
   return theOpn;
   }

//------------------------------------------------------------------------------

template <>
      WitNode * WitScenInputAtt <WitSubEntry>::stageNode (WitSubEntry * theSub)
   {
   return theSub->myOperation ();
   }

//------------------------------------------------------------------------------

template <>
      WitNode * WitScenInputAtt <WitBoundSet>::stageNode (
         WitBoundSet * theBoundSet)
   {
   return theBoundSet->stageNode ();
   }

//------------------------------------------------------------------------------

template <typename Owner>
      void WitScenInputAtt <Owner>::checkSameInAllScenarios (
         Owner *   theOwner,
         WitPeriod thePer)
   {
   WitScenario * theScen0;
   double        attVal0;
   WitScenario * theScenX;
   double        attValX;

   theScen0 = myScenMgr ()->myScenarioFor (0);

   attVal0  = myScenAtt_->myValueFor (theOwner, theScen0)[thePer];

   forEachEl (theScenX, myScenMgr ()->myScenarios ())
      if (theScenX->myIndex () != 0)
         {
         attValX = myScenAtt_->myValueFor (theOwner, theScenX)[thePer];

         if (attValX != attVal0)
            {
            myStageMgr ()->issueAttMisMatchHeadMsg ();

            issueMisMatchOwnerMsg (theOwner);

            myMsgFac () ("stage0AttMisMatchTailSmsg",
               myAttName_,
               thePer,
               attVal0,
               theScenX->myIndex (),
               attValX);
            }
         }
   }

//------------------------------------------------------------------------------
// Template specialized implementation of member function
// ScenInputAtt <Owner>::issueMisMatchOwnerMsg (theOwner)
//------------------------------------------------------------------------------

template <>
      void WitScenInputAtt <WitPart>::issueMisMatchOwnerMsg (WitPart * thePart)
   {
   myMsgFac () ("stage0AttMisMatchPartSmsg", thePart->partName ());
   }

//------------------------------------------------------------------------------

template <>
      void WitScenInputAtt <WitMaterial>::issueMisMatchOwnerMsg (
         WitMaterial * theMat)
   {
   myMsgFac () ("stage0AttMisMatchPartSmsg", theMat->partName ());
   }

//------------------------------------------------------------------------------

template <>
      void WitScenInputAtt <WitDemand>::issueMisMatchOwnerMsg (
         WitDemand * theDemand)
   {
   myMsgFac () ("stage0AttMisMatchDemandSmsg",
      theDemand->demandedPartName (),
      theDemand->demandName       ());
   }

//------------------------------------------------------------------------------

template <>
      void WitScenInputAtt <WitOperation>::issueMisMatchOwnerMsg (
         WitOperation * theOpn)
   {
   myMsgFac () ("stage0AttMisMatchOpnSmsg", theOpn->operationName ());
   }

//------------------------------------------------------------------------------

template <>
      void WitScenInputAtt <WitSubEntry>::issueMisMatchOwnerMsg (
         WitSubEntry * theSub)
   {
   myMsgFac () ("stage0AttMisMatchSubSmsg",
      theSub->myOperationName (),
      theSub->myBomEntIndex   (),
      theSub->localIndex      ());
   }

//------------------------------------------------------------------------------

template <>
      void WitScenInputAtt <WitBoundSet>::issueMisMatchOwnerMsg (
         WitBoundSet * theBoundSet)
   {
   theBoundSet->issueStage0MMCompMsg ();

   myMsgFac () ("stage0AttMisMatchBoundSetSmsg", theBoundSet->myAttName ());
   }

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

template <typename Owner>
      bool WitScenInputAtt <Owner>::isDiffedAt (Owner * theOwner)
   {
   if (myScenAtt_ != NULL)
      if (myScenAtt_->isAllocatedFor (theOwner))
         return true;

   return false;
   }

//------------------------------------------------------------------------------
// Explicit instantiation of class ScenInputAtt <Owner>.
//------------------------------------------------------------------------------

template class WitScenInputAtt <WitPart>;
template class WitScenInputAtt <WitMaterial>;
template class WitScenInputAtt <WitDemand>;
template class WitScenInputAtt <WitOperation>;
template class WitScenInputAtt <WitSubEntry>;
template class WitScenInputAtt <WitBoundSet>;

//------------------------------------------------------------------------------
// Implementation of class AbsScenSolnAtt.
//------------------------------------------------------------------------------

WitAbsScenSolnAtt::~WitAbsScenSolnAtt ()
   {
   }

//------------------------------------------------------------------------------

WitAbsScenSolnAtt::WitAbsScenSolnAtt (WitScenAttMgr * theScenAttMgr):

      WitStochAssoc (theScenAttMgr)
   {
   theScenAttMgr->store (this);
   }

//------------------------------------------------------------------------------
// Implementation of class template ScenSolnAtt <Owner>.
//------------------------------------------------------------------------------

template <typename Owner>
      WitScenSolnAtt <Owner>::WitScenSolnAtt (
            WitScenAttMgr * theScenAttMgr,
            const char *    theAttName,
            void (Owner::*  theStoreFunc) (const WitDblFlexVec &)):

         WitAbsScenSolnAtt (theScenAttMgr),
         myScenAtt_        (NULL),
         myStoreFunc_      (theStoreFunc)
   {
   Owner * theOwner;

   myScenAtt_ = new WitScenAtt <Owner> (theScenAttMgr);

   forEachEl (theOwner, myScenAtt_->allOwners ())
      myScenAtt_->allocateFor (theOwner);

   myStochModeMgr ()->myStochAttMgr ()->makeScenSolnStochAtt (theAttName);
   }

//------------------------------------------------------------------------------

template <typename Owner>
      void WitScenSolnAtt <Owner>::storeValueFor (
         Owner *       theOwner,
         WitScenario * theScenario,
         WitPeriod     thePer,
         double        theValue)
   {
   witAssert (myStochModeMgr ()->myStochOptMgr ()->uploadingSoln ());

   if (theValue <= 0.0)
      return;

   myScenAtt_->myValueFor (theOwner, theScenario).elemRef (thePer) = theValue;
   }

//------------------------------------------------------------------------------

template <typename Owner>
      WitScenSolnAtt <Owner>::~WitScenSolnAtt ()
   {
   delete myScenAtt_;
   }

//------------------------------------------------------------------------------

template <typename Owner>
      void WitScenSolnAtt <Owner>::uploadData (WitScenario * theScenario)
   {
   Owner * theOwner;

   forEachEl (theOwner, myScenAtt_->allOwners ())
      (theOwner->*myStoreFunc_) (
         myScenAtt_->myValueFor (theOwner, theScenario));
   }

//------------------------------------------------------------------------------
  
template <typename Owner>
      void WitScenSolnAtt <Owner>::clearData ()
   {
   Owner *       theOwner;
   WitScenario * theScenario;

   forEachEl (theOwner, myScenAtt_->allOwners ())
      forEachEl (theScenario, myScenMgr ()->myScenarios ())
         myScenAtt_->myValueFor (theOwner, theScenario) = 0.0;
   }

//------------------------------------------------------------------------------
// Explicit instantiation of class ScenSolnAtt <Owner>.
//------------------------------------------------------------------------------

template class WitScenSolnAtt <WitPart>;
template class WitScenSolnAtt <WitMaterial>;
template class WitScenSolnAtt <WitDemand>;
template class WitScenSolnAtt <WitOperation>;
template class WitScenSolnAtt <WitSubEntry>;

//------------------------------------------------------------------------------
// Implementation of class template ScenAtt <Owner>.
//------------------------------------------------------------------------------

template <typename Owner>
      WitScenAtt <Owner>::WitScenAtt (WitScenAttMgr * theScenAttMgr):

         WitStochAssoc (theScenAttMgr),
         myScenAttMgr_ (theScenAttMgr),
         myValue_      (myProblem ())
   {
   }

//------------------------------------------------------------------------------

template <typename Owner>
      WitScenAtt <Owner>::~WitScenAtt ()
   {
   Owner *       theOwner;
   WitScenario * theScenario;

   forEachEl (theOwner, allOwners ())
      if (isAllocatedFor (theOwner))
         {
         forEachEl (theScenario, myScenMgr ()->myScenarios ())
            delete myValue_.myElemAt (theOwner) (theScenario);

         delete myValue_ (theOwner);
         }
   }

//------------------------------------------------------------------------------
// Template specialized implementation of member function
// ScenAtt <Owner>::allOwners (...).
//------------------------------------------------------------------------------

template <> 
      const WitList <WitPart> & WitScenAtt <WitPart>::allOwners ()
   {
   return myScenAttMgr_->myCompMgr ()->allParts ();
   }

//------------------------------------------------------------------------------

template <>
      const WitList <WitMaterial> & WitScenAtt <WitMaterial>::allOwners ()
   {
   return myScenAttMgr_->allMats ();
   }

//------------------------------------------------------------------------------

template <>
      const WitList <WitDemand> & WitScenAtt <WitDemand>::allOwners ()
   {
   return myScenAttMgr_->allDemands ();
   }

//------------------------------------------------------------------------------

template <>
      const WitList <WitOperation> & WitScenAtt <WitOperation>::allOwners ()
   {
   return myScenAttMgr_->myCompMgr ()->allOperations ();
   }

//------------------------------------------------------------------------------

template <>
      const WitList <WitSubEntry> & WitScenAtt <WitSubEntry>::allOwners ()
   {
   return myScenAttMgr_->allSubs ();
   }

//------------------------------------------------------------------------------

template <> 
      const WitList <WitBoundSet> & WitScenAtt <WitBoundSet>::allOwners ()
   {
   return myScenAttMgr_->allBoundSets ();
   }

//------------------------------------------------------------------------------

template <typename Owner>
      void WitScenAtt <Owner>::allocateFor (Owner * theOwner)
   {
   WitScenario * theScenario;

   witAssert (not isAllocatedFor (theOwner));

   myValue_ (theOwner) =
      new WitPtrMap <WitScenario, WitDblFlexVec> (myProblem ());

   forEachEl (theScenario, myScenMgr ()->myScenarios ())
      myValue_.myElemAt (theOwner) (theScenario) =
         new WitDblFlexVec (myProblem (), 0.0);
   }
