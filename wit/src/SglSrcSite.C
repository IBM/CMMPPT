//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "SglSrcSite.C"
//
// Contains the implementation of class SglSrcSite.
//------------------------------------------------------------------------------

#include <SglSrcSite.h>
#include <Selector.h>
#include <RtMgr.h>
#include <RtSite.h>
#include <RtCand.h>
#include <MrMgr.h>
#include <HeurAtor.h>
#include <BopEntry.h>
#include <SubEntry.h>
#include <Part.h>
#include <Operation.h>
#include <GlobalComp.h>
#include <CompMgr.h>

//------------------------------------------------------------------------------
// Implementation of class SglSrcSite.
//------------------------------------------------------------------------------

WitSglSrcSite::WitSglSrcSite (WitRtSite * theRtSite):

      WitProbAssoc        (theRtSite),
      myRtSite_           (theRtSite),
      mySelector_         (theRtSite->myRtMgr ()->mySelector ()),
      myRtCands_          (myProblem ()),
      selRtCand_          (NULL),
      nextRtCand_         (NULL),
      nextRtCandItr_      (),
      altered_            (false),
      curPcIdx_           (-1),
      incumbPcIdx_        (-1),
      curVisited_         (false),
      curUniqueRtCand_    (NULL),
      incumbUniqueRtCand_ (NULL)
   {
   WitRtCand * theRtCand;

   forEachEl (theRtCand, myRtSite_->myRtCands ())
      if (theRtCand->usedByMr ())
         myRtCands_.push (theRtCand);

   myRtCands_.reverse ();

   reset ();
   }

//------------------------------------------------------------------------------

WitSglSrcSite::~WitSglSrcSite ()
   {
   }

//------------------------------------------------------------------------------

bool WitSglSrcSite::isPreferredTo (WitSglSrcSite * othSite)
   {
   if (othSite == NULL)
      return true;

   if (incumbPcIdx_ > othSite->incumbPcIdx_)
      return true;

   if (incumbPcIdx_ < othSite->incumbPcIdx_)
      return false;

   return (myRtSite_->heightIdx () > othSite->myRtSite_->heightIdx ());
   }

//------------------------------------------------------------------------------

bool WitSglSrcSite::nextCandIsSglSrc ()
   {
   stronglyAssert (nextRtCand_ != NULL);

   return (nextRtCand_ == incumbUniqueRtCand_);
   }

//------------------------------------------------------------------------------
    
void WitSglSrcSite::selectNext ()
   {
   stronglyAssert (nextRtCand_ != NULL);

   selRtCand_  = nextRtCand_;
   nextRtCand_ = NULL;

   nextRtCandItr_.advance (nextRtCand_);

   altered_    = true;
   }

//------------------------------------------------------------------------------

void WitSglSrcSite::applySel ()
   {
   WitMrMgr *  theMrMgr;
   WitPeriod   thePer;
   WitRtCand * theMrRtCand;

   if (not sglSrcMode ())
      return;

   theMrMgr = mySelector_->myMrMgr ();

   forEachPeriod (thePer, myProblem ())
      if (theMrMgr->valid (selRtCand_, thePer))
         {
         theMrRtCand = theMrMgr->selRtCand (myRtSite_, thePer);

         if (theMrRtCand != selRtCand_)
            {
            theMrMgr->select (selRtCand_, thePer);

            if (myGlobalComp ()->selectionRecovery ())
               {
               theMrMgr->storeRecoveryPair (myRtSite_, thePer);
               }
            }
         }
   }

//------------------------------------------------------------------------------

void WitSglSrcSite::resumeMS ()
   {
   selRtCand_ = NULL;
   }

//------------------------------------------------------------------------------

void WitSglSrcSite::initCmqFixed ()
   {
   witAssert (curPcIdx_ >= 0);

   curPcIdx_        = -1;
   curVisited_      = false;
   curUniqueRtCand_ = NULL;
   }

//------------------------------------------------------------------------------

void WitSglSrcSite::recordVisit (WitRtCand * theRtCand, int thePcIdx)
   {
   if (not curVisited_)
      curUniqueRtCand_ = theRtCand;

   curVisited_ = true;

   if (curUniqueRtCand_ != NULL)
      if (theRtCand != curUniqueRtCand_)
         curUniqueRtCand_ = NULL;

   if (not mySelector_->myHeurAtor ()->permCommActive ())
      return;

   witAssert (thePcIdx >= 0);

   curPcIdx_ = thePcIdx;
   }

//------------------------------------------------------------------------------

void WitSglSrcSite::resetIncumb ()
   {
   witAssert (incumbPcIdx_ >= 0);

   incumbPcIdx_        = -1;
   incumbUniqueRtCand_ = NULL;
   }

//------------------------------------------------------------------------------

void WitSglSrcSite::saveCurAsIncumb ()
   {
   witAssert (incumbPcIdx_ == -1);
   witAssert (curPcIdx_    >=  0);

   incumbPcIdx_        = curPcIdx_;
   incumbUniqueRtCand_ = curUniqueRtCand_;
   }

//------------------------------------------------------------------------------

bool WitSglSrcSite::expProhibited (WitPeriod thePer)
   {
   if (sglSrcMode ())
      if (not mySelector_->myMrMgr ()->valid (selRtCand_, thePer))
         return true;

   return false;
   }

//------------------------------------------------------------------------------

void WitSglSrcSite::reset ()
   {
   selRtCand_  = NULL;
   nextRtCand_ = NULL;

   myRtCands_.attachItr (nextRtCandItr_);

   nextRtCandItr_.advance (nextRtCand_);

   altered_  = false;
   }

//------------------------------------------------------------------------------

void WitSglSrcSite::prtSel (const char * text)
   {
   fprintf (msgFile (),
      "\n"
      "-------------------------------------\n"
      "-> Single-Source Selection %s\n"
      "-------------------------------------\n"
      "\n",
      text);

   myRtSite_->printID (4, 0);

   fprintf (msgFile (), "   Selection:\n");

   selRtCand_->printID (7, 0);
   }
