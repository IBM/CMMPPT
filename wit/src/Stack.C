//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "Stack.C"
//
// Contains the implementation of the following classes and templates:
//
//    AbsStack
//    AbsStItr
//    AbsObjStack
//    AbsObjStItr
//    PairStack <ObjByPtr, ObjByVal>, with explicit instantiation
//    PerSglStack <ObjByPtr>, with explicit instantiation
//    AbsTripStack
//    AbsLinkMgr
//    ObjLinkMgr
//    PairLinkMgr <ObjByVal>, with explicit instantiation
//    TripLinkMgr
//    AbsLink
//    ObjLink
//    PairLink <ObjByVal>, with explicit instantiation
//    TripLink
//------------------------------------------------------------------------------

#include <ObjStack.h>
#include <PerSglStack.h>
#include <TripStack.h>
#include <LinkMgr.h>

#include <Material.h>
#include <Operation.h>
#include <BomEntry.h>
#include <BopEntry.h>
#include <RtSite.h>
#include <RtCand.h>

//------------------------------------------------------------------------------
// Implementation of class AbsStack.
//------------------------------------------------------------------------------

void WitAbsStack::clear ()
   {
   while (isNonEmpty ())
      popAbs ();
   }

//------------------------------------------------------------------------------

int WitAbsStack::nElements () const
   {
   int          nEl;
   WitAbsLink * theAbsLink;

   nEl = 0;

   theAbsLink = firstAbsLink_;

   while (theAbsLink != NULL)
      {
      nEl ++;

      theAbsLink = theAbsLink->nextAbsLink ();
      }

   return nEl;
   }

//------------------------------------------------------------------------------

void WitAbsStack::reverse ()
   {
   WitAbsLink * oldFirstAbsLink;
   WitAbsLink * theAbsLink;

   oldFirstAbsLink = firstAbsLink_;
   firstAbsLink_   = NULL;

   while (oldFirstAbsLink != NULL)
      {
      theAbsLink      = oldFirstAbsLink;
      oldFirstAbsLink = oldFirstAbsLink->nextAbsLink ();

      theAbsLink->linkTo (firstAbsLink_);

      firstAbsLink_   = theAbsLink;
      }
   }

//------------------------------------------------------------------------------

WitProblem * WitAbsStack::myProblem () const
   {
   return myAbsLinkMgr_->myProblem ();
   }

//------------------------------------------------------------------------------

WitAbsStack::WitAbsStack (WitAbsLinkMgr * theAbsLinkMgr):

      myAbsLinkMgr_ (theAbsLinkMgr),
      firstAbsLink_ (NULL)
   {
   }

//------------------------------------------------------------------------------

WitAbsStack::~WitAbsStack ()
   {
   clear ();
   }

//------------------------------------------------------------------------------

void WitAbsStack::pushAbs ()
   {
   firstAbsLink_ = myAbsLinkMgr_->freshAbsLink (firstAbsLink_);
   }

//------------------------------------------------------------------------------

void WitAbsStack::popAbs ()
   {
   WitAbsLink * theAbsLink;

   witAssert (isNonEmpty ());

   theAbsLink    = firstAbsLink_;
   firstAbsLink_ = theAbsLink->nextAbsLink ();

   myAbsLinkMgr_->recycleAbsLink (theAbsLink);
   }

//------------------------------------------------------------------------------

void WitAbsStack::takeContentsFromAbs (WitAbsStack & thatAbsStack)
   {
   witAssert (isEmpty ());

                firstAbsLink_ = thatAbsStack.firstAbsLink_;
   thatAbsStack.firstAbsLink_ = NULL;
   }

//------------------------------------------------------------------------------

void WitAbsStack::attachItrAbs (WitAbsStItr & theAbsStItr) const
   {
   theAbsStItr.myAbsStack_ = this;
   theAbsStItr.curAbsLink_ = NULL;
   }

//------------------------------------------------------------------------------
// Implementation of class AbsStItr.
//------------------------------------------------------------------------------

WitAbsStItr::WitAbsStItr ():
      myAbsStack_ (NULL),
      curAbsLink_ (NULL)
   {
   }

//------------------------------------------------------------------------------

WitAbsStItr::~WitAbsStItr ()
   {
   }

//------------------------------------------------------------------------------

void WitAbsStItr::advanceAbs ()
   {
   witAssert (myAbsStack_ != NULL);

   curAbsLink_ =
      (curAbsLink_ == NULL)?
         myAbsStack_->firstConstLink ():
         curAbsLink_->nextAbsLink ();
   }

//------------------------------------------------------------------------------
// Implementation of class AbsObjStack.
//------------------------------------------------------------------------------

WitAbsObjStack::WitAbsObjStack (WitProblem * theProblem):

      WitAbsStack (theProblem->myObjLinkMgr ())
   {
   }

//------------------------------------------------------------------------------

WitAbsObjStack::~WitAbsObjStack ()
   {
   }

//------------------------------------------------------------------------------

void WitAbsObjStack::revCopyFromAbs (const WitAbsObjStack & theAbsObjStack)
   {
   WitAbsLink * theAbsLink;
   WitObjLink * theObjLink;

   clear ();

   theAbsLink = theAbsObjStack.firstAbsLink ();

   while (theAbsLink != NULL)
      {
      theObjLink = static_cast <WitObjLink *> (theAbsLink);

      pushAbsObj (theObjLink->myObjByVP ());

      theAbsLink = theObjLink->nextAbsLink ();
      }
   }

//------------------------------------------------------------------------------

bool WitAbsObjStack::containsAbs (void * theObjByVP) const
   {
   WitAbsObjStItr theItr;
   void *         theItrObjByVP;

   attachItrAbsObj (theItr);

   while (theItr.advanceAbsObj (theItrObjByVP))
      if (theItrObjByVP == theObjByVP)
         return true;

   return false;
   }

//------------------------------------------------------------------------------

void WitAbsObjStack::pushAbsObj (void * theObjByVP)
   {
   pushAbs ();

   firstObjLink ()->setDataTo (theObjByVP);
   }

//------------------------------------------------------------------------------

bool WitAbsObjStack::popAbsObj (void * & theObjByVP)
   {
   if (isEmpty ())
      return false;

   theObjByVP = firstObjLink ()->myObjByVP ();

   popAbs ();

   return true;
   }

//------------------------------------------------------------------------------

void WitAbsObjStack::attachItrAbsObj (WitAbsObjStItr & theAbsObjStItr) const
   {
   attachItrAbs (theAbsObjStItr);
   }

//------------------------------------------------------------------------------

void WitAbsObjStack::removeAbs (void * theObjByVP)
   {
   WitAbsObjStack revStack (myProblem ());
   void *         poppedObjByVP;

   while (popAbsObj (poppedObjByVP))
      if (poppedObjByVP != theObjByVP)
         revStack.pushAbsObj (poppedObjByVP);

   takeContentsFromAbs (revStack);
   
   reverse ();
   }

//------------------------------------------------------------------------------

void * WitAbsObjStack::firstObjByVP () const
   {
   witAssert (not isEmpty ());

   return firstObjLink ()->myObjByVP ();
   }

//------------------------------------------------------------------------------
// Implementation of class AbsObjStItr.
//------------------------------------------------------------------------------

WitAbsObjStItr::WitAbsObjStItr ():

      WitAbsStItr ()
   {
   }

//------------------------------------------------------------------------------

WitAbsObjStItr::~WitAbsObjStItr ()
   {
   }

//------------------------------------------------------------------------------

bool WitAbsObjStItr::advanceAbsObj (void * & theObjByVP)
   {
   const WitObjLink * theObjLink;

   advanceAbs ();

   theObjLink = static_cast <const WitObjLink *> (curAbsLink ());

   if (theObjLink != NULL)
      {
      theObjByVP = theObjLink->myObjByVP ();

      return true;
      }
   else
      {
      theObjByVP = NULL;

      return false;
      }
   }

//------------------------------------------------------------------------------
// Implementation of NonClass function template revCopyInto.
//------------------------------------------------------------------------------

template <typename ObjByPtr>
      void WitNonClass::revCopyInto (
         WitObjStack <ObjByPtr> &   theObjStack,
         const WitList <ObjByPtr> & theList)
   {
   WitListItr theItr (theList);
   ObjByPtr * theObjByPtr;

   theObjStack.clear ();

   while (theList.advance (theItr, theObjByPtr))
      theObjStack.push (theObjByPtr);
   }

//------------------------------------------------------------------------------
// Explicit instantiation of NonClass function template revCopyInto.
//------------------------------------------------------------------------------

template void WitNonClass::revCopyInto <WitPart> (
   WitObjStack            <WitPart> &,
   const WitList          <WitPart> &);

template void WitNonClass::revCopyInto <WitOperation> (
   WitObjStack            <WitOperation> &,
   const WitList          <WitOperation> &);

template void WitNonClass::revCopyInto <WitSubEntry> (
   WitObjStack            <WitSubEntry> &,
   const WitList          <WitSubEntry> &);

//------------------------------------------------------------------------------
// Implementation of class template PairStack <ObjByPtr, ObjByVal>.
//------------------------------------------------------------------------------

template <PairStArgDecs> 
      WitPairStack <PairStArgs>::WitPairStack (WitProblem * theProblem):

         WitAbsStack (WitPairLinkMgr <ObjByVal>::theInstanceFor (theProblem))
   {
   }

//------------------------------------------------------------------------------

template <PairStArgDecs> 
      WitPairStack <PairStArgs>::~WitPairStack ()
   {
   }

//------------------------------------------------------------------------------

template <PairStArgDecs> 
      void WitPairStack <PairStArgs>::push (
         ObjByPtr * theObjByPtr,
         ObjByVal   theObjByVal)
   {
   pushAbs ();

   firstPairLink ()->setDataTo (theObjByPtr, theObjByVal);
   }

//------------------------------------------------------------------------------

template <PairStArgDecs> 
      bool WitPairStack <PairStArgs>::pop (
         ObjByPtr * & theObjByPtr, 
         ObjByVal &   theObjByVal)
   {
   if (isEmpty ())
      return false;

   theObjByPtr = reinterpret_cast <ObjByPtr *> (firstPairLink ()->myObjByVP ());

   theObjByVal = firstPairLink ()->myObjByVal ();

   popAbs ();

   return true;
   }

//------------------------------------------------------------------------------

template <PairStArgDecs> 
      void WitPairStack <PairStArgs>::revCopyFrom (
         const WitPairStack & thePairStack)
   {
   WitPairStItr <PairStArgs> theItr;
   ObjByPtr *                theObjByPtr;
   ObjByVal                  theObjByVal;

   clear ();

   thePairStack.attachItr (theItr);

   while (theItr.advance (theObjByPtr, theObjByVal))
      push (theObjByPtr, theObjByVal);
   }

//------------------------------------------------------------------------------

template <PairStArgDecs> 
      ObjByPtr * WitPairStack <PairStArgs>::firstObject () const
   {
   witAssert (isNonEmpty ());

   return reinterpret_cast <ObjByPtr *> (firstPairLink ()->myObjByVP ());
   }

//------------------------------------------------------------------------------

template <PairStArgDecs> 
      ObjByVal WitPairStack <PairStArgs>::firstValue () const
   {
   witAssert (isNonEmpty ());

   return firstPairLink ()->myObjByVal ();
   }

//------------------------------------------------------------------------------

template <PairStArgDecs> 
      bool WitPairStack <PairStArgs>::advance (
         WitPairStItr <PairStArgs> & theItr,
         ObjByPtr * &                theObjByPtr,
         ObjByVal &                  theObjByVal)
   {
   const WitPairLink <ObjByVal> * thePairLink;

   theItr.advanceAbs ();

   thePairLink =
      static_cast <const WitPairLink <ObjByVal> *> (theItr.curAbsLink ());

   if (thePairLink == NULL)
      return false;

   theObjByPtr = reinterpret_cast <ObjByPtr *> (thePairLink->myObjByVP ());

   theObjByVal = thePairLink->myObjByVal ();

   return true;
   }

//------------------------------------------------------------------------------
// Explicit instantiation of class template PairStack <ObjByPtr, ObjByVal>.
//------------------------------------------------------------------------------

template class WitPairStack <bool,                        bool>;
template class WitPairStack <WitPart,                     bool>;

template class WitPairStack <int,                         int>;
template class WitPairStack <WitPart,                     int>;
template class WitPairStack <WitMaterial,                 int>;
template class WitPairStack <WitOperation,                int>;
template class WitPairStack <WitBomEntry,                 int>;
template class WitPairStack <WitSubEntry,                 int>;
template class WitPairStack <WitBopEntry,                 int>;
template class WitPairStack <WitDemand,                   int>;
template class WitPairStack <WitRtCand,                   int>;
template class WitPairStack <WitBaMat,                    int>;
template class WitPairStack <WitRtSite,                   int>;

template class WitPairStack <double,                      double>;
template class WitPairStack <WitFixedPer,                 double>;
template class WitPairStack <WitRtCand,                   double>;
template class WitPairStack <WitSelPt,                    double>;
template class WitPairStack <WitSelCand,                  double>;
template class WitPairStack <WitPipReqClient <WitDemand>, double>;
template class WitPairStack <WitPipReqBasis,              double>;

template class WitPairStack <void *,                      void *>;

//------------------------------------------------------------------------------
// Implementation of class template PerSglStack <ObjByPtr>.
//------------------------------------------------------------------------------

template <typename ObjByPtr> 
      WitPerSglStack <ObjByPtr>::WitPerSglStack (WitProblem * theProblem):

      myPairs_ (theProblem),
      inStack_ ()
   {
   }

//------------------------------------------------------------------------------

template <typename ObjByPtr> 
      WitPerSglStack <ObjByPtr>::~WitPerSglStack ()
   {
   }

//------------------------------------------------------------------------------

template <typename ObjByPtr> 
      void WitPerSglStack <ObjByPtr>::push (
         ObjByPtr * theObjByPtr,
         WitPeriod  thePer)
   {
   if (inStack_.domainSize () == 0)
      {
      inStack_.allocate1D (myPairs_.myProblem ());
      }

   if (not inStack_ (theObjByPtr).isAllocated ())
      {
      inStack_ (theObjByPtr).allocate (myPairs_.myProblem (), false);
      }

   if (inStack_ (theObjByPtr)[thePer])
      return;

   myPairs_.push (theObjByPtr, thePer);

   inStack_ (theObjByPtr)[thePer] = true;
   }

//------------------------------------------------------------------------------

template <typename ObjByPtr> 
      bool WitPerSglStack <ObjByPtr>::pop (
         ObjByPtr * & theObjByPtr,
         WitPeriod &  thePer)
   {
   if (myPairs_.isEmpty ())
      return false;

   myPairs_.pop (theObjByPtr, thePer);

   inStack_ (theObjByPtr)[thePer] = false;

   return true;
   }

//------------------------------------------------------------------------------
// Explicit instantiation of class template PerSglStack <ObjByPtr>.
//------------------------------------------------------------------------------

template class WitPerSglStack <WitPart>;
template class WitPerSglStack <WitMaterial>;
template class WitPerSglStack <WitOperation>;
template class WitPerSglStack <WitBomEntry>;
template class WitPerSglStack <WitBopEntry>;
template class WitPerSglStack <WitRtSite>;
template class WitPerSglStack <WitRtCand>;

//------------------------------------------------------------------------------
// Implementation of class AbsTripStack.
//------------------------------------------------------------------------------

WitAbsTripStack::WitAbsTripStack (WitProblem * theProblem):

      WitAbsStack (theProblem->myTripLinkMgr ())
   {
   }

//------------------------------------------------------------------------------

WitAbsTripStack::~WitAbsTripStack ()
   {
   }

//------------------------------------------------------------------------------

void WitAbsTripStack::pushAbsTrip (
      void *    theObjByVP,
      WitPeriod thePer,
      double    theDbl)
   {
   pushAbs ();

   firstTripLink ()->setDataTo (theObjByVP, thePer, theDbl);
   }

//------------------------------------------------------------------------------

bool WitAbsTripStack::popAbsTrip (
      void * &    theObjByVP,
      WitPeriod & thePer,
      double &    theDbl)
   {
   if (isEmpty ())
      return false;

   firstTripLink ()->getData (theObjByVP, thePer, theDbl);

   popAbs ();

   return true;
   }

//------------------------------------------------------------------------------

WitTripLink * WitAbsTripStack::firstTripLink () const
   {
   return static_cast <WitTripLink *> (firstAbsLink ());
   }

//------------------------------------------------------------------------------

void WitAbsTripStack::revCopyFromAbs (const WitAbsTripStack & theAbsTripStack)
   {
   WitAbsLink *  theAbsLink;
   WitTripLink * theTripLink;
   void *        theObjByVP;
   WitPeriod     thePer;
   double        theDbl;

   clear ();

   theAbsLink = theAbsTripStack.firstAbsLink ();

   while (theAbsLink != NULL)
      {
      theTripLink = static_cast <WitTripLink *> (theAbsLink);

      theTripLink->getData (theObjByVP, thePer, theDbl);

      pushAbsTrip (theObjByVP, thePer, theDbl);

      theAbsLink = theAbsLink->nextAbsLink ();
      }
   }

//------------------------------------------------------------------------------
// Implementation of class AbsLinkMgr.
//------------------------------------------------------------------------------

void WitAbsLinkMgr::deleteInstance (WitAbsLinkMgr * theAbsLinkMgr)
   {
   WitAbsLink * theAbsLink;
   WitAbsLink * deadAbsLink;

   if (theAbsLinkMgr == NULL)
      return;

   theAbsLink = theAbsLinkMgr->firstUnusedAbsLink_;

   while (theAbsLink != NULL)
      {
      deadAbsLink = theAbsLink;
      theAbsLink  = theAbsLink->nextAbsLink ();

      theAbsLinkMgr->deleteAbsLink (deadAbsLink);
      }

   delete theAbsLinkMgr;
   }

//------------------------------------------------------------------------------

WitAbsLink * WitAbsLinkMgr::freshAbsLink (WitAbsLink * theAbsLink)
   {
   WitAbsLink * theFreshAbsLink;

   if (firstUnusedAbsLink_ == NULL)
      theFreshAbsLink     = newAbsLink ();
   else
      {
      theFreshAbsLink     = firstUnusedAbsLink_;
      firstUnusedAbsLink_ = theFreshAbsLink->nextAbsLink ();
      }

   theFreshAbsLink->linkTo (theAbsLink);

   return theFreshAbsLink;
   }

//------------------------------------------------------------------------------

void WitAbsLinkMgr::recycleAbsLink (WitAbsLink * theAbsLink)
   {
   theAbsLink->linkTo (firstUnusedAbsLink_);

   firstUnusedAbsLink_ = theAbsLink;
   }

//------------------------------------------------------------------------------

WitAbsLinkMgr::WitAbsLinkMgr (WitProblem * theProblem):
      WitProbAssoc        (theProblem),

      firstUnusedAbsLink_ (NULL)
   {
   }

//------------------------------------------------------------------------------

WitAbsLinkMgr::~WitAbsLinkMgr ()
   {
   }

//------------------------------------------------------------------------------
// Implementation of class ObjLinkMgr.
//------------------------------------------------------------------------------

WitObjLinkMgr::WitObjLinkMgr (WitProblem * theProblem):
      WitAbsLinkMgr (theProblem)
   {
   }

//------------------------------------------------------------------------------

WitObjLinkMgr::~WitObjLinkMgr ()
   {
   }

//------------------------------------------------------------------------------

WitAbsLink * WitObjLinkMgr::newAbsLink ()
   {
   return new WitObjLink;
   }

//------------------------------------------------------------------------------

void WitObjLinkMgr::deleteAbsLink (WitAbsLink * theAbsLink)
   {
   delete static_cast <WitObjLink *> (theAbsLink);
   }

//------------------------------------------------------------------------------
// Implementation of class template PairLinkMgr <ObjByVal>.
//------------------------------------------------------------------------------

template <typename ObjByVal> 
      WitPairLinkMgr <ObjByVal>::WitPairLinkMgr (WitProblem * theProblem):

         WitAbsLinkMgr (theProblem)
   {
   }

//------------------------------------------------------------------------------

template <typename ObjByVal> 
      WitPairLinkMgr <ObjByVal>::~WitPairLinkMgr ()
   {
   }

//------------------------------------------------------------------------------

template <typename ObjByVal> 
      WitAbsLink * WitPairLinkMgr <ObjByVal>::newAbsLink ()
   {
   return new WitPairLink <ObjByVal>;
   }

//------------------------------------------------------------------------------

template <typename ObjByVal> 
      void WitPairLinkMgr <ObjByVal>::deleteAbsLink (WitAbsLink * theAbsLink)
   {
   delete static_cast <WitPairLink <ObjByVal> *> (theAbsLink);
   }

//------------------------------------------------------------------------------
// Non-parametric implementation of class template PairLinkMgr <ObjByVal> member
// functions.
//------------------------------------------------------------------------------

template <>
      WitPairLinkMgr <int> * WitPairLinkMgr <int>::theInstanceFor (
         WitProblem * theProblem)
   {
   return theProblem->myIntPairLinkMgr ();
   }

//------------------------------------------------------------------------------

template <>
      WitPairLinkMgr <bool> * WitPairLinkMgr <bool>::theInstanceFor (
         WitProblem * theProblem)
   {
   return theProblem->myBoolPairLinkMgr ();
   }

//------------------------------------------------------------------------------

template <>
      WitPairLinkMgr <double> * WitPairLinkMgr <double>::theInstanceFor (
         WitProblem * theProblem)
   {
   return theProblem->myDblPairLinkMgr ();
   }

//------------------------------------------------------------------------------

template <>
      WitPairLinkMgr <void *> * 
         WitPairLinkMgr <void *>::theInstanceFor (WitProblem * theProblem)
   {
   return theProblem->myVoidPtrPairLinkMgr ();
   }

//------------------------------------------------------------------------------
// Explicit instantiation of class template PairLink <ObjByVal>.
//------------------------------------------------------------------------------

template class WitPairLinkMgr <bool>;
template class WitPairLinkMgr <int>;
template class WitPairLinkMgr <double>;
template class WitPairLinkMgr <void *>;

//------------------------------------------------------------------------------
// Implementation of class TripLinkMgr.
//------------------------------------------------------------------------------

WitTripLinkMgr::WitTripLinkMgr (WitProblem * theProblem):
      WitAbsLinkMgr (theProblem)
   {
   }

//------------------------------------------------------------------------------

WitTripLinkMgr::~WitTripLinkMgr ()
   {
   }

//------------------------------------------------------------------------------

WitAbsLink * WitTripLinkMgr::newAbsLink ()
   {
   return new WitTripLink;
   }

//------------------------------------------------------------------------------

void WitTripLinkMgr::deleteAbsLink (WitAbsLink * theAbsLink)
   {
   delete static_cast <WitTripLink *> (theAbsLink);
   }

//------------------------------------------------------------------------------
// Implementation of class AbsLink.
//------------------------------------------------------------------------------

void WitAbsLink::linkTo (WitAbsLink * theAbsLink)
   {
   nextAbsLink_ = theAbsLink;
   }

//------------------------------------------------------------------------------

WitAbsLink::WitAbsLink ():
      nextAbsLink_ (NULL)
   {
   }

//------------------------------------------------------------------------------

WitAbsLink::~WitAbsLink ()
   {
   }

//------------------------------------------------------------------------------
// Implementation of class ObjLink.
//------------------------------------------------------------------------------

WitObjLink::WitObjLink ():
      WitAbsLink (),

      myObjByVP_ (NULL)
   {
   }

//------------------------------------------------------------------------------

WitObjLink::~WitObjLink ()
   {
   }

//------------------------------------------------------------------------------

void WitObjLink::setDataTo (void * theObjByVP)
   {
   myObjByVP_ = theObjByVP;
   }

//------------------------------------------------------------------------------
// Implementation of class template PairLink <ObjByVal>.
//------------------------------------------------------------------------------

template <typename ObjByVal> 
      WitPairLink <ObjByVal>::WitPairLink ():
         WitAbsLink  (),

         myObjByVP_  (NULL),
         myObjByVal_ ()
   {
   }

//------------------------------------------------------------------------------

template <typename ObjByVal> 
      WitPairLink <ObjByVal>::~WitPairLink ()
   {
   }

//------------------------------------------------------------------------------

template <typename ObjByVal> 
      void WitPairLink <ObjByVal>::setDataTo (
         void *   theObjByVP,
         ObjByVal theObjByVal)
   {
   myObjByVP_  = theObjByVP;
   myObjByVal_ = theObjByVal;
   }

//------------------------------------------------------------------------------
// Explicit instantiation of class template PairLink <ObjByVal>.
//------------------------------------------------------------------------------

template class WitPairLink <bool>;
template class WitPairLink <int>;
template class WitPairLink <double>;
template class WitPairLink <void *>;

//------------------------------------------------------------------------------
// Implementation of class TripLink.
//------------------------------------------------------------------------------

WitTripLink::WitTripLink ():
      WitAbsLink (),

      myObjByVP_ (NULL),
      myPer_     (-1),
      myDbl_     (0.0)
   {
   }

//------------------------------------------------------------------------------

WitTripLink::~WitTripLink ()
   {
   }

//------------------------------------------------------------------------------

void WitTripLink::setDataTo (
      void *    theObjByVP,
      WitPeriod thePer,
      double    theDbl)
   {
   myObjByVP_ = theObjByVP;
   myPer_     = thePer;
   myDbl_     = theDbl;
   }

//------------------------------------------------------------------------------

void WitTripLink::getData (
      void * &    theObjByVP,
      WitPeriod & thePer,
      double &    theDbl)
      const
   {
   theObjByVP = myObjByVP_;
   thePer     = myPer_;
   theDbl     = myDbl_;
   }
