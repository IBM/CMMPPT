//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "PenExec.C"
//
// Contains the implementation of the following classes:
//
//    PenExMgr
//    ExpRest
//------------------------------------------------------------------------------

#include <PenExMgr.h>
#include <ExpRest.h>
#include <RtAn.h>
#include <HeurAtor.h>
#include <Selector.h>
#include <DataRepos.h>
#include <Material.h>
#include <BopEntry.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class PenExMgr.
//------------------------------------------------------------------------------

WitPenExMgr::WitPenExMgr (WitSelector * theSelector):

      WitProbAssoc  (theSelector),
      myExpRest_    (NULL),
      myRtAnalyzer_ (NULL)
   {
   myMsgFac () ("penExecMsg");

   myExpRest_    = new WitExpRest    (theSelector->myHeurAtor ());
   myRtAnalyzer_ = new WitRtAnalyzer (theSelector, myExpRest_);
   }

//------------------------------------------------------------------------------

WitPenExMgr::~WitPenExMgr ()
   {
   delete myRtAnalyzer_;
   delete myExpRest_;
   }

//------------------------------------------------------------------------------

void WitPenExMgr::modifyRouting ()
   {
   myRtAnalyzer_->modifyRouting ();
   }

//------------------------------------------------------------------------------

void WitPenExMgr::findHypoModPts (WitObjStack <WitSelPt> & theModPts)
   {
   myRtAnalyzer_->findHypoModPts (theModPts);
   }

//------------------------------------------------------------------------------

void WitPenExMgr::recCommitVol (double commitVol)
   {
   myRtAnalyzer_->recCommitVol (commitVol);
   }

//------------------------------------------------------------------------------

void WitPenExMgr::placeReqs (
      WitPart * srcPart,
      WitPeriod srcPer,
      WitPart * reqPart,
      WitPeriod reqPer)
   {
   myExpRest_->placeReqs (srcPart, srcPer, reqPart, reqPer);
   }

//------------------------------------------------------------------------------

void WitPenExMgr::recExpBeyondNet (
      WitPart * thePart, 
      WitPeriod thePer, 
      bool      netForConsEnt)
   {
   myExpRest_->recExpBeyondNet (thePart, thePer, netForConsEnt);
   }

//------------------------------------------------------------------------------

void WitPenExMgr::postExpBomEnt ()
   {
   myExpRest_->postExpBomEnt ();
   }

//------------------------------------------------------------------------------

bool WitPenExMgr::restExpAllowed (WitBopEntry * theBopEnt, WitPeriod fillPer)
   {
   return myExpRest_->restExpAllowed (theBopEnt, fillPer);
   }

//------------------------------------------------------------------------------

void WitPenExMgr::recordFundShortage ()
   {
   myExpRest_->recordFundShortage ();
   }

//------------------------------------------------------------------------------

void WitPenExMgr::reqPsSupRep (WitPart * thePart, WitPeriod thePer)
   {
   myExpRest_->reqPsSupRep (thePart, thePer);
   }

//------------------------------------------------------------------------------

void WitPenExMgr::postTempCommit (bool success)
   {
   myExpRest_->postTempCommit (success);
   }

//------------------------------------------------------------------------------

void WitPenExMgr::updatePsSup ()
   {
   myExpRest_->updatePsSup ();
   }

//------------------------------------------------------------------------------

void WitPenExMgr::replenishPsSups ()
   {
   myExpRest_->replenishPsSups ();
   }

//------------------------------------------------------------------------------

void WitPenExMgr::print ()
   {
   myExpRest_->print ();
   }

//------------------------------------------------------------------------------

bool WitPenExMgr::failRestShortOnly ()
   {
   return myExpRest_->failRestShortOnly ();
   }

//------------------------------------------------------------------------------
// Implementation of class ExpRest.
//------------------------------------------------------------------------------

WitExpRest::WitExpRest (WitHeurAtor * theHeurAtor):

      WitProbAssoc       (theHeurAtor),
      myHeurAtor_        (theHeurAtor),
      hasPosPenalty_     (myProblem (), false),
      hasPseudoSup_      (),
      curExhaustReqs_    (myProblem ()),
      failExhaustReqs_   (myProblem ()),
      curFundShort_      (false),
      failRestShortOnly_ (false),
      repReqs_           (myProblem ()),
      unanReqs_          (myProblem (), false),
      unanReqsPairs_     (myProblem ()),
      unanByConsEntNet_  (false),
      tempMode_          (false),
      psSupRepos_        (NULL)
   {
   WitPart * thePart;

   findPosPenalties ();

   hasPseudoSup_.allocate1D (myProblem ());

   psSupRepos_ = new WitDataRepos (myProblem ());
   
   forEachPart (thePart, myProblem ())
      init (thePart);
   }

//------------------------------------------------------------------------------

WitExpRest::~WitExpRest ()
   {
   delete psSupRepos_;
   }

//------------------------------------------------------------------------------

void WitExpRest::placeReqs (
      WitPart * srcPart,
      WitPeriod srcPer,
      WitPart * reqPart,
      WitPeriod reqPer)
   {
   if (unanReqs_ (srcPart)[srcPer] or unanByConsEntNet_)
      setUnanReqs (reqPart, reqPer);
   }

//------------------------------------------------------------------------------

void WitExpRest::recExpBeyondNet (
      WitPart *  thePart, 
      WitPeriod  thePer, 
      bool       netForConsEnt)
   {
   if (not hasPseudoSup (thePart, thePer))
      return;

   curExhaustReqs_.push (thePart, thePer);

   if (netForConsEnt)
      unanByConsEntNet_ = true;
   else
      setUnanReqs (thePart, thePer);
   }

//------------------------------------------------------------------------------

void WitExpRest::postExpBomEnt ()
   {
   unanByConsEntNet_ = false;
   }

//------------------------------------------------------------------------------

bool WitExpRest::restExpAllowed (
      WitBopEntry * theBopEnt, 
      WitPeriod     fillPer)
   {
   if (hasPosPenalty_ (theBopEnt->myOperation ()))
      if (unanReqs_ (theBopEnt->myPart ())[fillPer])
         return false;

   return true;
   }

//------------------------------------------------------------------------------

void WitExpRest::recordFundShortage ()
   {
   stronglyAssert (not curFundShort_);

   curFundShort_ = true;
   }

//------------------------------------------------------------------------------

void WitExpRest::reqPsSupRep (WitPart * thePart, WitPeriod thePer)
   {
   repReqs_.push (thePart, thePer);
   }

//------------------------------------------------------------------------------

void WitExpRest::postTempCommit (bool success)
   {
   WitPart * thePart;
   WitPeriod thePer;

   if (success)
      curExhaustReqs_.clear ();
   else
      {
      failExhaustReqs_.clear ();

      failExhaustReqs_.takeContentsFrom (curExhaustReqs_);

      failRestShortOnly_ = not curFundShort_;

      while (unanReqsPairs_.pop (thePart, thePer))
         unanReqs_ (thePart)[thePer] = false;
      }

   curFundShort_ = false;
   }

//------------------------------------------------------------------------------

void WitExpRest::updatePsSup ()
   {
   WitPairStItr <WitPart, WitPeriod> theItr;
   WitPart *                         thePart;
   WitPeriod                         fillPer;
   WitMaterial *                     theMat;

   failExhaustReqs_.attachItr (theItr);

   while (theItr.advance (thePart, fillPer))
      {
      if (not hasPseudoSup_ (thePart)[fillPer])
         continue;

      exhaustPsSup (thePart, fillPer);

      if (selPrintLevelAt (3))
         printExhRep (thePart, fillPer, true);

      theMat = thePart->thisMat ();

      if (theMat != NULL)
         exhaustOtherPers (theMat, fillPer);
      }

   if (not tempMode_)
      {
      failExhaustReqs_.clear ();

      failRestShortOnly_ = false;
      }
   }

//------------------------------------------------------------------------------

void WitExpRest::startTempMode ()
   {
   stronglyAssert (not tempMode_);

   tempMode_ = true;
   }

//------------------------------------------------------------------------------

void WitExpRest::finishTempMode ()
   {
   stronglyAssert (tempMode_);

   psSupRepos_->restore ();

   tempMode_ = false;
   }

//------------------------------------------------------------------------------

void WitExpRest::replenishPsSups ()
   {
   WitPart * thePart;
   WitPeriod reqPer;
   WitPeriod thePer;

   while (repReqs_.pop (thePart, reqPer))
      {
      if (not myHeurAtor_->incAvailPos (thePart, reqPer))
         continue;

      if (not hasPseudoSup_ (thePart).isAllocated ())
         hasPseudoSup_ (thePart).allocate (myProblem (), false);

      for (thePer = reqPer; thePer < nPeriods (); thePer ++)
         {
         if (hasPseudoSup_ (thePart)[thePer])
            break;

         hasPseudoSup_ (thePart)[thePer] = true;

         if (selPrintLevelAt (3))
            printExhRep (thePart, thePer, (thePer == reqPer));

         if (not thePart->canStock (thePer))
            break;
         }
      }
   }

//------------------------------------------------------------------------------

void WitExpRest::print ()
   {
   WitPart * thePart;
   WitPeriod thePer;

   fprintf (msgFile (), 
      "\n"
      "Parts and Periods with Pseudo-Supply:\n\n");

   forEachPart (thePart, myProblem ())
      if (hasPseudoSup_ (thePart).isAllocated ())
         forEachPeriod (thePer, myProblem ())
            if (hasPseudoSup_ (thePart)[thePer])
               fprintf (msgFile (), 
                  "   Part %s, Per %d\n", 
                  thePart->partName ().myCstring (),
                  thePer);
   }

//------------------------------------------------------------------------------

void WitExpRest::exhaustOtherPers (WitMaterial * theMat, WitPeriod fillPer)
   {
   WitPeriod thePer;

   for (thePer = fillPer - 1; thePer >= 0; thePer --)
      {
      if (theMat->mandEC ()[thePer])
         break;

      if (not hasPseudoSup_ (theMat)[thePer])
         break;

      exhaustPsSup (theMat, thePer);

      if (selPrintLevelAt (3))
         printExhRep (theMat, thePer, false);
      }

   for (thePer = fillPer + 1; thePer < nPeriods (); thePer ++)
      {
      if (theMat->mandEC ()[thePer - 1])
         break;

      if (theMat->supplyVol ()[thePer] > NET_TOL)
         break;

      exhaustPsSup (theMat, thePer);

      if (selPrintLevelAt (3))
         printExhRep (theMat, thePer, false);
      }
   }

//------------------------------------------------------------------------------

void WitExpRest::exhaustPsSup (WitPart * thePart, WitPeriod thePer)
   {
   if (tempMode_)
      psSupRepos_->saveVal (hasPseudoSup_ (thePart)[thePer]);

   hasPseudoSup_ (thePart)[thePer] = false;
   }

//------------------------------------------------------------------------------

void WitExpRest::setUnanReqs (WitPart * thePart, WitPeriod thePer)
   {
   if (unanReqs_ (thePart)[thePer])
      return;

   unanReqs_ (thePart)[thePer] = true;

   unanReqsPairs_.push (thePart, thePer);
   }

//------------------------------------------------------------------------------

void WitExpRest::findPosPenalties ()
   {
   WitOperation * theOpn;
   WitBomEntry *  theBomEnt;
   WitSubEntry *  theSub;

   forEachOperation (theOpn, myProblem ())
      if (theOpn->execPenalty () > 0.0)
         hasPosPenalty_ (theOpn) = true;

   forEachBomEntry (theBomEnt, myProblem ())
      if (theBomEnt->execPenalty () > 0.0)
         hasPosPenalty_ (theBomEnt->myOperation ()) = true;

   forEachSubEntry (theSub, myProblem ())
      if (theSub->execPenalty () > 0.0)
         hasPosPenalty_ (theSub->myOperation ()) = true;
   }

//------------------------------------------------------------------------------
// init
//
// carryOver: true, iff supply from an earlier period carries over to the 
//            current period.
//------------------------------------------------------------------------------

void WitExpRest::init (WitPart * thePart)
   {
   WitPeriod fillPer;
   bool      carryOver;

   carryOver = false;

   forEachPeriod (fillPer, myProblem ())
      {
      if (not carryOver)
         if (thePart->supplyVol ()[fillPer] <= NET_TOL)
            continue;

      if (not hasPseudoSup_ (thePart).isAllocated ())
         hasPseudoSup_ (thePart).allocate (myProblem (), false);

      hasPseudoSup_ (thePart)[fillPer] = true;

      carryOver = thePart->canStock (fillPer);
      }
   }

//------------------------------------------------------------------------------

void WitExpRest::printExhRep (
      WitPart * thePart, 
      WitPeriod fillPer, 
      bool      firstPer)
   {
   const char * changeText;

   if (tempMode_)
      return;

   changeText = 
      hasPseudoSup (thePart, fillPer)?
         "replenished":
         "exhausted";

   if (firstPer)
      fprintf (msgFile (), "\n");

   fprintf (msgFile (), 
      "Pseudo-supply %s for part %s in period %d.\n",
      changeText,
      thePart->partName ().myCstring (),
      fillPer);
   }
