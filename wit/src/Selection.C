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
// Source file: "Selection.C"
//
// Contains the implementation of the following classes and templates:
//
//    SelMgr
//    SelPt
//    DetSelPt
//    AltPt
//    DetAltPt
//    ReqPtMgr
//    ReqPt
//------------------------------------------------------------------------------

#include <SelMgr.h>
#include <DetSelPt.h>
#include <DetAltPt.h>
#include <ReqPtMgr.h>
#include <ReqPt.h>
#include <Selector.h>
#include <SsrMgr.h>
#include <GlobalComp.h>
#include <Material.h>
#include <BopEntry.h>

//------------------------------------------------------------------------------
// Implementation of class SelMgr.
//------------------------------------------------------------------------------

WitSelMgr::~WitSelMgr ()
   {
   }

//------------------------------------------------------------------------------

WitAltPt * WitSelMgr::myAltPtFor (WitPart *, WitPeriod)
   {
   return NULL;
   }

//------------------------------------------------------------------------------

void WitSelMgr::setIndex ()
   {
   myIndex_ = mySelector_->mySelMgrs ().nElements ();
   }

//------------------------------------------------------------------------------

bool WitSelMgr::isLowerThan (WitSelMgr * theSelMgr)
   {
   return (myIndex_ > theSelMgr->myIndex_);
   }

//------------------------------------------------------------------------------

void WitSelMgr::setUpSelSplit ()
   {
   stronglyAssert (not selSplit_);

   selSplit_ = true;
   }

//------------------------------------------------------------------------------

void WitSelMgr::setBoundedSplit (bool theVal)
   {
   boundedSplit_ = theVal;
   }

//------------------------------------------------------------------------------

void WitSelMgr::postSplitCommit () 
   {
   }

//------------------------------------------------------------------------------

WitSelMgr::WitSelMgr (WitSelector * theSelector):

      WitProbAssoc  (theSelector),
      mySelector_   (theSelector),
      myIndex_      (-1),
      selSplit_     (false),
      boundedSplit_ (false)
   {
   }

//------------------------------------------------------------------------------
// Implementation of class SelPt.
//------------------------------------------------------------------------------

void WitSelPt::attachSplitPt (WitSplitPt * theSplitPt)
   {
   stronglyAssert ( mySplitPt_ == NULL);
   stronglyAssert (theSplitPt  != NULL);

   mySplitPt_ = theSplitPt;

   recSplit ();
   }

//------------------------------------------------------------------------------

void WitSelPt::detachSplitPt ()
   {
   stronglyAssert (mySplitPt_ != NULL);

   mySplitPt_ = NULL;

   recUnsplit ();
   }

//------------------------------------------------------------------------------

void WitSelPt::recordFlowSS (double incFlowVol)
   {
   mySelMgr ()->mySelector ()->recordFlowSS (this, incFlowVol);
   }

//------------------------------------------------------------------------------

bool WitSelPt::sglSrcMode ()
   {
   return false;
   }

//------------------------------------------------------------------------------

WitSelPt::WitSelPt ():
      mySplitPt_ (NULL)
   {
   }

//------------------------------------------------------------------------------

WitSelPt::~WitSelPt ()
   {
   }

//------------------------------------------------------------------------------

void WitSelPt::recSplit ()
   {
   }

//------------------------------------------------------------------------------

void WitSelPt::recUnsplit ()
   {
   }

//------------------------------------------------------------------------------
// Implementation of class DetSelPt.
//------------------------------------------------------------------------------

WitDetSelPt::WitDetSelPt ():

      WitSelPt    (),
      myDetAltPt_ (NULL)
   {
   myDetAltPt_ = new WitDetAltPt (this);
   }

//------------------------------------------------------------------------------

WitDetSelPt::~WitDetSelPt ()
   {
   delete myDetAltPt_;
   }

//------------------------------------------------------------------------------
// Implementation of class AltPt.
//------------------------------------------------------------------------------

WitAltPt::~WitAltPt ()
   {
   }

//------------------------------------------------------------------------------

void WitAltPt::setPending (bool theValue)
   {
   pending_ = theValue;
   }

//------------------------------------------------------------------------------

WitAltPt::WitAltPt ():
      pending_ (false)
   {
   }

//------------------------------------------------------------------------------
// Implementation of class DetAltPt.
//------------------------------------------------------------------------------

WitDetAltPt::WitDetAltPt (WitDetSelPt * theDetSelPt):

      WitAltPt    (),
      myDetSelPt_ (theDetSelPt)
   {
   }

//------------------------------------------------------------------------------

WitDetAltPt::~WitDetAltPt ()
   {
   }

//------------------------------------------------------------------------------

bool WitDetAltPt::hasResAlt ()
   {
   return myDetSelPt_->hasResAlt ();
   }

//------------------------------------------------------------------------------

void WitDetAltPt::alterSelection ()
   {
   myDetSelPt_->alterSelection ();
   }

//------------------------------------------------------------------------------

void WitDetAltPt::storeRecoveryPt ()
   {
   myDetSelPt_->storeRecoveryPt ();
   }

//------------------------------------------------------------------------------

void WitDetAltPt::tempAlterSel ()
   {
   myDetSelPt_->tempAlterSel ();
   }

//------------------------------------------------------------------------------

void WitDetAltPt::cancelTempAlt ()
   {
   myDetSelPt_->cancelTempAlt ();
   }

//------------------------------------------------------------------------------

WitSelPt * WitDetAltPt::mySelPt ()
   {
   return myDetSelPt_;
   }

//------------------------------------------------------------------------------

void WitDetAltPt::printAlteration ()
   {
   myDetSelPt_->printAlteration ();
   }

//------------------------------------------------------------------------------
// Implementation of class ReqPtMgr.
//------------------------------------------------------------------------------

WitReqPtMgr::WitReqPtMgr (WitProblem * theProblem):
      WitProbAssoc  (theProblem),

      myReqPts_     (myProblem ()),
      allocedSlots_ (myProblem ()),
      unusedPoints_ (myProblem ())
   {
   }

//------------------------------------------------------------------------------

WitReqPtMgr::~WitReqPtMgr ()
   {
   reset ();

   deleteContents (unusedPoints_);
   }

//------------------------------------------------------------------------------

WitReqPt * WitReqPtMgr::myReqPt (WitPart * thePart, WitPeriod thePer)
   {
   WitReqPt * & theReqPtRef = myReqPts_.myPtrAt (thePart, thePer);

   if (theReqPtRef == NULL)
      {
      if (not unusedPoints_.pop (theReqPtRef))
         theReqPtRef = new WitReqPt (myProblem ());

      if (DEBUG_MODE)
         theReqPtRef->set (thePart, thePer);

      allocedSlots_.push (& theReqPtRef);
      }

   return theReqPtRef;
   }

//------------------------------------------------------------------------------

void WitReqPtMgr::reset ()
   {
   WitReqPt * * theReqPtPtr;

   while (allocedSlots_.pop (theReqPtPtr))
      {
      WitReqPt * & theReqPtRef = * theReqPtPtr;

      theReqPtRef->reset ();

      if (DEBUG_MODE)
         theReqPtRef->set (NULL, -1);

      unusedPoints_.push (theReqPtRef);

      theReqPtRef = NULL;
      }
   }

//------------------------------------------------------------------------------
// Implementation of class ReqPt.
//------------------------------------------------------------------------------

WitReqPt::WitReqPt (WitProblem * theProblem):

      myPart_     (NULL),
      myPer_      (-1),
      directAlts_ (theProblem),
      source_     (NULL),
      hasResAlt_  (false)
   {
   }

//------------------------------------------------------------------------------

WitReqPt::~WitReqPt ()
   {
   }

//------------------------------------------------------------------------------

void WitReqPt::set (WitPart * thePart, WitPeriod thePer)
   {
   myPart_ = thePart;
   myPer_  = thePer;
   }

//------------------------------------------------------------------------------

bool WitReqPt::hasAlt ()
   {
   return (source_ != NULL) or (directAlts_.isNonEmpty ());
   }

//------------------------------------------------------------------------------

void WitReqPt::setSource (WitReqPt * theSource)
   {
   stronglyAssert (theSource != NULL);

   if (not theSource->hasAlt ())
      return;

   if (theSource->hasResAlt ())
      {
      reset ();
            
      hasResAlt_ = true;
      }        

   source_ = theSource;
   }

//------------------------------------------------------------------------------

void WitReqPt::putDirectAlt (WitAltPt * theAltPt)
   {
   witAssert (theAltPt != NULL);

   if (theAltPt->hasResAlt ())
      {
      reset ();
            
      hasResAlt_ = true;
      }        

   directAlts_.push (theAltPt);
   }

//------------------------------------------------------------------------------

void WitReqPt::copyStateOf (WitReqPt * theReqPt)
   {
   directAlts_.revCopyFrom (theReqPt->directAlts_);

   directAlts_.reverse ();

   source_    = theReqPt->source ();

   hasResAlt_ = theReqPt->hasResAlt ();
   }

//------------------------------------------------------------------------------

void WitReqPt::reset ()
   {
   directAlts_.clear ();

   source_    = NULL;
   hasResAlt_ = false;
   }

//------------------------------------------------------------------------------

bool WitReqPt::getDirectAlt (WitAltPt * & theAltPt)
   {
   return directAlts_.pop (theAltPt);
   }
