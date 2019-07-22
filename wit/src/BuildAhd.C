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
// Source file: "BuildAhd.C"
//
// Contains the implementation of the following classes:
//
//    BaMgr.
//    BaDir.
//    NstnBaDir.
//    AsapBaDir.
//    BaMat.
//    BaPt.
//    BaCand.
//------------------------------------------------------------------------------

#include <HeurAtor.h>
#include <Selector.h>
#include <BaMgr.h>
#include <BaCand.h>
#include <BaDir.h>
#include <BaMat.h>
#include <BaPt.h>
#include <DetAltPt.h>
#include <SglSrcMgr.h>
#include <SplitPt.h>
#include <GlobalComp.h>
#include <Material.h>
#include <BopEntry.h>
#include <MsgFac.h>
#include <PairStack.h>

//------------------------------------------------------------------------------
// Implementation of class BaMgr.
//------------------------------------------------------------------------------

WitBaMgr::WitBaMgr (WitSelector * theSelector):
      WitSelMgr       (theSelector),

      myBaDirForNstn_ (NULL),
      myBaDirForAsap_ (NULL),
      myBaMatFor_     (myProblem ()),
      pairsToReject_  (myProblem ()),
      savedExpPer_    (-1),
      alteredBaMats_  (myProblem ()),
      baAlteredAt_    ()
   {
   bool          nstnMatExists;
   bool          asapMatExists;
   WitMaterial * theMat;

   myBaDirForNstn_ = new WitNstnBaDir (this);
   myBaDirForAsap_ = new WitAsapBaDir (this);

   nstnMatExists   = false;
   asapMatExists   = false;

   forEachMaterial (theMat, myProblem ())
      if (not theMat->producingBopEntries ().isEmpty ())
         {
         if      (theMat->nstnBaNeeded ())
            {
            myBaMatFor_ (theMat) = new WitBaMat (theMat, myBaDirForNstn_, this);

            nstnMatExists        = true;
            }

         else if (theMat->buildAsap ())
            {
            myBaMatFor_ (theMat) = new WitBaMat (theMat, myBaDirForAsap_, this);

            asapMatExists        = true;
            }
         }

   if (myGlobalComp ()->selectionRecovery ())
      baAlteredAt_.allocate (myProblem (), false);

   if (nstnMatExists)
      myMsgFac () ("nstnMsg");

   if (asapMatExists)
      myMsgFac () ("asapMsg");
   }

//------------------------------------------------------------------------------

WitBaMgr::~WitBaMgr ()
   {
   WitMaterial * theMat;

   forEachMaterial (theMat, myProblem ())
      delete myBaMatFor_ (theMat);

   delete myBaDirForAsap_;
   delete myBaDirForNstn_;
   }

//------------------------------------------------------------------------------

bool WitBaMgr::instNeededFor (WitProblem * theProblem)
   {
   WitMaterial * theMat;

   forEachMaterial (theMat, theProblem)
      if (theMat->nstnBaNeeded () or theMat->buildAsap ())
         return true;

   return false;
   }

//------------------------------------------------------------------------------

WitPeriod WitBaMgr::selExpPer (WitPart * thePart, WitPeriod fillPer)
   {
   WitBaMat * theBaMat;
   WitPeriod  expPer;

   expPer = selExpPer (thePart, fillPer, theBaMat);

   return 
      (expPer >= 0)?
         expPer:
         thePart->explosionPeriod (fillPer);
   }

//------------------------------------------------------------------------------

void WitBaMgr::print ()
   {
   WitMaterial * theMat;
   WitBaMat *    theBaMat;

   fprintf (msgFile (),
      "\n"
      "Build-Ahead Period Selections:\n");

   forEachMaterial (theMat, myProblem ())
      {
      theBaMat = myBaMatFor_ (theMat);

      if (theBaMat != NULL)
         theBaMat->print ();
      }
   }

//------------------------------------------------------------------------------

void WitBaMgr::saveExpPer (WitPeriod expPer)
   {
   witAssert (savedExpPer_ == -1);
   witAssert (expPer       != -1);

   savedExpPer_ = expPer;
   }

//------------------------------------------------------------------------------

void WitBaMgr::retrieveExpPer (WitPeriod & expPer)
   {
   witAssert (savedExpPer_ != -1);

   expPer       = savedExpPer_;
   savedExpPer_ = -1;
   }

//------------------------------------------------------------------------------

void WitBaMgr::storeRecoveryPt (WitBaMat * theBaMat)
   {
   witAssert (myGlobalComp ()->selectionRecovery ());

   if (baAlteredAt_ (theBaMat->myMaterial ()))
      return;

   baAlteredAt_ (theBaMat->myMaterial ()) = true;

   alteredBaMats_.push (theBaMat);
   }

//------------------------------------------------------------------------------

void WitBaMgr::recoverInitState ()
   {
   WitBaMat * theBaMat;

   while (alteredBaMats_.pop (theBaMat))
      if (baAlteredAt_ (theBaMat->myMaterial ()))
         {
         theBaMat->recoverInitState ();

         baAlteredAt_ (theBaMat->myMaterial ()) = false;
         }
   }

//------------------------------------------------------------------------------

bool WitBaMgr::selSplitNeeded ()
   {
   WitMaterial * theMat;

   if (myGlobalComp ()->selSplit ())
      return true;

   if (myGlobalComp ()->nonMrSelSplit ())
      return true;

   forEachMaterial (theMat, myProblem ())
      if (theMat->nstnBaNeeded () or theMat->buildAsap ())
         if (theMat->baSelSplit ())
            return true;

   return false;
   }

//------------------------------------------------------------------------------

bool WitBaMgr::selIsSplit (WitPart * thePart, WitPeriod fillPer)
   {
   WitBaPt * theBaPt;

   if (not selSplit ())
      return false;

   theBaPt = myBaPt (thePart, fillPer);

   if (theBaPt == NULL)
      return false;

   return theBaPt->isSplit ();
   }

//------------------------------------------------------------------------------

void WitBaMgr::defineSplit (
      WitPart *        thePart,
      WitPeriod        fillPer,
      double           reqVol,
      WitPerDblStack & theSplitPairs,
      double &         unbddVol)
   {
   WitSelCandDblStack theAbsSplitPairs (myProblem ());
   WitSelCand *       theSelCand;
   double             splitVol;
   WitPeriod          expPer;
   WitFixedPer *      theFixedPer;

   witAssert (selIsSplit (thePart, fillPer));

   myBaPt (thePart, fillPer)->
      mySplitPt ()->
         defineSplit (reqVol, theAbsSplitPairs, unbddVol);

   theSplitPairs.clear ();

   while (theAbsSplitPairs.pop (theSelCand, splitVol))
      {
      theSelCand->getBaData (expPer);

      theFixedPer = mySelector ()->myFixedPer ()[expPer];

      theSplitPairs.push (theFixedPer, splitVol);
      }
   }

//------------------------------------------------------------------------------

void WitBaMgr::recExpMatSS (WitPart * thePart, WitPeriod fillPer, double expVol)
   {
   WitBaPt * theBaPt;

   witAssert (selSplit ());

   if (thePart == mySelector ()->myHeurAtor ()->topPart ())
      return;

   theBaPt = myBaPt (thePart, fillPer);

   if (theBaPt != NULL)
      theBaPt->recordFlowSS (expVol);
   }

//------------------------------------------------------------------------------

void WitBaMgr::postSplitCommit ()
   {
   WitBaMat * theBaMat;
   WitPeriod  expPer;

   witAssert (selSplit ());

   while (pairsToReject_.pop (theBaMat, expPer))
      theBaMat->reject (expPer);
   }

//------------------------------------------------------------------------------

void WitBaMgr::deferRejection (WitBaMat * theBaMat, WitPeriod expPer)
   {
   witAssert (selSplit ());

   witAssert (mySelector ()->splitCommActive ());

   pairsToReject_.push (theBaMat, expPer);
   }

//------------------------------------------------------------------------------

WitAltPt * WitBaMgr::myAltPtFor (
      WitBopEntry * theBopEnt,
      WitConsEntry *,
      WitPeriod,
      WitPeriod     fillPer)
   {
   return myAltPtFor (theBopEnt->myPart (), fillPer);
   }

//------------------------------------------------------------------------------

WitAltPt * WitBaMgr::myAltPtFor (WitPart * thePart, WitPeriod fillPer)
   {
   WitBaMat * theBaMat;
   WitPeriod  expPer;

   expPer = selExpPer (thePart, fillPer, theBaMat);

   if (expPer < 0)
      return NULL;

   return theBaMat->myBaPt ()[fillPer]->myDetAltPt ();
   }

//------------------------------------------------------------------------------

bool WitBaMgr::validForNetting ()
   {
   return true;
   }

//------------------------------------------------------------------------------

bool WitBaMgr::validForExecBounds ()
   {
   return true;
   }

//------------------------------------------------------------------------------

bool WitBaMgr::validForLTBounds ()
   {
   return true;
   }

//------------------------------------------------------------------------------

const char * WitBaMgr::selCaseName ()
   {
   return "Build-Ahead";
   }

//------------------------------------------------------------------------------

WitPeriod WitBaMgr::selExpPer (
      WitPart *    thePart, 
      WitPeriod    fillPer,
      WitBaMat * & theBaMat)
   {
   WitMaterial * theMat;

   theMat = thePart->thisMat ();

   if (theMat == NULL)
      return -1;

   theBaMat = myBaMatFor_ (theMat);

   if (theBaMat == NULL)
      return -1;

   return theBaMat->myBaPt ()[fillPer]->selExpPer ();
   }

//------------------------------------------------------------------------------

WitBaPt * WitBaMgr::myBaPt (WitPart * thePart, WitPeriod fillPer)
   {
   WitMaterial * theMat;
   WitBaMat *    theBaMat;

   theMat = thePart->thisMat ();

   if (theMat == NULL)
      return NULL;

   theBaMat = myBaMatFor_ (theMat);

   if (theBaMat == NULL)
      return NULL;

   return theBaMat->myBaPt ()[fillPer];
   }

//------------------------------------------------------------------------------
// Implementation of class BaDir.
//------------------------------------------------------------------------------

WitBaDir::~WitBaDir ()
   {
   }

//------------------------------------------------------------------------------

WitBaDir::WitBaDir (WitBaMgr * theBaMgr):

      WitProbAssoc (theBaMgr),
      myBaMgr_     (theBaMgr)
   {
   }

//------------------------------------------------------------------------------
// Implementation of class NstnBaDir.
//------------------------------------------------------------------------------

WitNstnBaDir::WitNstnBaDir (WitBaMgr * theBaMgr):
      WitBaDir (theBaMgr)
   {
   }

//------------------------------------------------------------------------------

WitNstnBaDir::~WitNstnBaDir ()
   {
   }

//------------------------------------------------------------------------------

void WitNstnBaDir::findInitExpPers (
      WitMaterial *,
      WitTVec <WitPeriod> & expPerVec)
   {
   WitPeriod fillPer;

   forEachPeriod (fillPer, myProblem ())
      expPerVec[fillPer] = fillPer;
   }

//------------------------------------------------------------------------------

WitPeriod WitNstnBaDir::findInitExpPer (WitMaterial * theMat, WitPeriod fillPer)
   {
   return fillPer;
   }

//------------------------------------------------------------------------------

void WitNstnBaDir::advanceExpPer (
      WitMaterial * theMat,
      WitPeriod     fillPer, 
      WitPeriod &   expPer)
   {
   expPer --;

   if (expPer >= 0) 
      if (expPer >= fillPer - theMat->buildAheadUB ()[fillPer])
         if (not theMat->mandEC ()[expPer])
            return;

   expPer = -1;
   }

//------------------------------------------------------------------------------
// Implementation of class AsapBaDir.
//------------------------------------------------------------------------------

WitAsapBaDir::WitAsapBaDir (WitBaMgr * theBaMgr):
      WitBaDir (theBaMgr)
   {
   }

//------------------------------------------------------------------------------

WitAsapBaDir::~WitAsapBaDir ()
   {
   }

//------------------------------------------------------------------------------

void WitAsapBaDir::findInitExpPers (
      WitMaterial *         theMat,
      WitTVec <WitPeriod> & expPerVec)
   {
   WitPeriod fillPer;
   WitPeriod firstStockPer;

   firstStockPer = 0;

   forEachPeriod (fillPer, myProblem ())
      {
      expPerVec[fillPer] =
         max (firstStockPer, fillPer - theMat->buildAheadUB ()[fillPer]);

      if (theMat->mandEC ()[fillPer])
         firstStockPer = fillPer + 1;
      }
   }

//------------------------------------------------------------------------------

WitPeriod WitAsapBaDir::findInitExpPer (WitMaterial * theMat, WitPeriod fillPer)
   {
   WitPeriod expPer;

   if (theMat->mandEC () == false)
      {
      return max (0, fillPer - theMat->buildAheadUB ()[fillPer]);
      }

   for (expPer = fillPer; ; expPer --)
      {
      if (expPer == 0)
         return expPer;

      if (expPer == fillPer - theMat->buildAheadUB ()[fillPer])
         return expPer;
      
      if (theMat->mandEC ()[expPer - 1])
         return expPer;
      }
   }

//------------------------------------------------------------------------------

void WitAsapBaDir::advanceExpPer (
      WitMaterial *,
      WitPeriod     fillPer, 
      WitPeriod &   expPer)
   {
   expPer ++;

   if (expPer > fillPer)
      expPer = -1;
   }

//------------------------------------------------------------------------------
// Implementation of class BaMat.
//------------------------------------------------------------------------------

WitBaMat::WitBaMat (
         WitMaterial * theMat, 
         WitBaDir *    theBaDir, 
         WitBaMgr *    theBaMgr):

      WitProbAssoc (theMat),
      myMaterial_  (theMat),
      myBaDir_     (theBaDir),
      myBaMgr_     (theBaMgr),
      myBaPt_      (myProblem ()),
      useable_     (myProblem (), true)
   {
   WitPeriod fillPer;

   forEachPeriod (fillPer, myProblem ())
      myBaPt_[fillPer] = new WitBaPt (this, fillPer);

   WitBaPt::setInitSelExpPers (this);

   rejectUnexplodablePers ();
   }

//------------------------------------------------------------------------------

WitBaMat::~WitBaMat ()
   {
   deleteContents (myBaPt_);
   }

//------------------------------------------------------------------------------

void WitBaMat::findInitExpPers (WitTVec <WitPeriod> & expPerVec)
   {
   myBaDir_->findInitExpPers (myMaterial_, expPerVec);
   }

//------------------------------------------------------------------------------

void WitBaMat::reject (WitPeriod expPer)
   {
   if (not useable_[expPer])
      return;

   if (myBaMgr_->mySelector ()->sglSrc ())
      myBaMgr_->mySelector ()->mySglSrcMgr ()->saveConfigVal (useable_[expPer]);

   useable_[expPer] = false;

   deselect (expPer);
   }

//------------------------------------------------------------------------------

WitPeriod WitBaMat::nextUseableExpPer (WitPeriod fillPer)
   {
   WitPeriod expPer;

   expPer = myBaPt_[fillPer]->selExpPer ();

   while (true)
      {
      myBaDir_->advanceExpPer (myMaterial_, fillPer, expPer);

      if (expPer < 0)
         return -1;

      if (useable_[expPer])
         return expPer;
      }

   return -1; 
      //                       
      // Avoids a compiler warning.
   }

//------------------------------------------------------------------------------

WitPeriod WitBaMat::findInitExpPer (WitPeriod fillPer)
   {
   return myBaDir_->findInitExpPer (myMaterial_, fillPer);
   }

//------------------------------------------------------------------------------

void WitBaMat::print ()
   {
   WitPeriod fillPer;
   WitPeriod expPer;

   fprintf (msgFile (),
      "\n"
      "Build-Ahead Material %s:\n\n"
      "    Exp Per   Useable?\n",
      myMaterial_->partName ().myCstring ());

   forEachPeriod (expPer, myProblem ())
      fprintf (msgFile (), 
         "    %7d   %7s\n",
         expPer,
         myMsgFac ().boolText (useable_[expPer]));
   
   fprintf (msgFile (), 
      "\n"
      "   Fill Per   Exp Per\n");

   forEachPeriod (fillPer, myProblem ())
      {
      fprintf (msgFile (), "   %8d", fillPer);

      expPer = myBaPt ()[fillPer]->selExpPer ();

      if (expPer >= 0)
         fprintf (msgFile (), "   %7d\n", expPer);
      else
         fprintf (msgFile (), "         *\n");
      }
   }

//------------------------------------------------------------------------------

void WitBaMat::recoverInitState ()
   {
   useable_ = true;

   WitBaPt::setInitSelExpPers (this);

   rejectUnexplodablePers ();

   if (selPrintLevelAt (3))
      printRecovery ();
   }

//------------------------------------------------------------------------------

void WitBaMat::rejectUnexplodablePers ()
   {
   WitPeriod expPer;

   forEachPeriod (expPer, myProblem ())
      if (not myMaterial_->explodeable (expPer))
         {
         useable_[expPer] = false;

         deselect (expPer);
         }
   }

//------------------------------------------------------------------------------

void WitBaMat::deselect (WitPeriod expPer)
   {
   WitPeriod fillPer;

   for (fillPer = expPer; fillPer < nPeriods (); fillPer ++)
      if (myBaPt_[fillPer]->selExpPer () == expPer)
          myBaPt_[fillPer]->advanceSel ();
   }

//------------------------------------------------------------------------------

void WitBaMat::printRecovery ()
   {
   fprintf (msgFile (),
      "   Initial build-ahead configuration recovered for part %s.\n\n",
      myMaterial_->partName ().myCstring ());
   }

//------------------------------------------------------------------------------
// Implementation of class BaPt.
//------------------------------------------------------------------------------

WitBaPt::WitBaPt (WitBaMat * theBaMat, WitPeriod fillPer):

      WitDetSelPt (),

      myBaMat_    (theBaMat),
      myFillPer_  (fillPer),
      selExpPer_  (-1)
   {
   }

//------------------------------------------------------------------------------

WitBaPt::~WitBaPt ()
   {
   }

//------------------------------------------------------------------------------

void WitBaPt::setInitSelExpPers (WitBaMat * theBaMat)
   {
   WitTVec <WitPeriod> expPerVec (theBaMat->myProblem (), -1);
   WitPeriod           fillPer;

   theBaMat->findInitExpPers (expPerVec);

   forEachPeriod (fillPer, theBaMat->myProblem ())
      {
      theBaMat->myBaPt ()[fillPer]->selExpPer_ = expPerVec[fillPer];
      }
   }

//------------------------------------------------------------------------------

void WitBaPt::advanceSel ()
   {
   select (myBaMat_->nextUseableExpPer (myFillPer_));
   }

//------------------------------------------------------------------------------

void WitBaPt::select (WitPeriod expPer)
   {
   if (mySelMgr ()->mySelector ()->sglSrc ())
      mySelMgr ()->mySelector ()->mySglSrcMgr ()->saveConfigVal (selExpPer_);

   selExpPer_ = expPer;
   }

//------------------------------------------------------------------------------

WitSelMgr * WitBaPt::mySelMgr ()
   {
   return myBaMat_->myBaMgr ();
   }

//------------------------------------------------------------------------------

void WitBaPt::prtID ()
   {
   fprintf (myBaMat_->myBaMgr ()->msgFile (),
      "   Part:             %s\n"
      "   Fill Per:         %d\n",
      myBaMat_->myMaterial ()->partName ().myCstring (),
      myFillPer_);
   }

//------------------------------------------------------------------------------

bool WitBaPt::splittable ()
   {
   if (myBaMat_->myBaMgr ()->myGlobalComp ()->selSplit ())
      return true;

   if (myBaMat_->myBaMgr ()->myGlobalComp ()->nonMrSelSplit ())
      return true;

   return myBaMat_->myMaterial ()->baSelSplit ();
   }

//------------------------------------------------------------------------------

void WitBaPt::getSortData (WitPart * & thePart, WitPeriod & thePer)
   {
   thePart = myBaMat_->myMaterial ();
   thePer  = myFillPer_;
   }

//------------------------------------------------------------------------------

WitSelCand * WitBaPt::newSelCand ()
   {
   return new WitBaCand (selExpPer_, myBaMat_->myProblem ());
   }

//------------------------------------------------------------------------------

bool WitBaPt::hasResAlt ()
   {
   WitPeriod expPer;

   expPer = myBaMat_->nextUseableExpPer (myFillPer_);

   return (expPer >= 0);
   }

//------------------------------------------------------------------------------

void WitBaPt::alterSelection ()
   {
   WitBaMgr * theBaMgr;

   witAssert (selExpPer_ >= 0);

   theBaMgr = myBaMat_->myBaMgr ();

   if (theBaMgr->selSplit () and theBaMgr->mySelector ()->splitCommActive ())
      {
      theBaMgr->deferRejection (myBaMat_, selExpPer_);

      advanceSel ();
      }
   else
      myBaMat_->reject (selExpPer_);
   }

//------------------------------------------------------------------------------

void WitBaPt::storeRecoveryPt ()
   {
   myBaMat_->myBaMgr ()->storeRecoveryPt (myBaMat_);
   }

//------------------------------------------------------------------------------

void WitBaPt::tempAlterSel ()
   {
   myBaMat_->myBaMgr ()->saveExpPer (selExpPer_);

   advanceSel ();
   }

//------------------------------------------------------------------------------

void WitBaPt::cancelTempAlt ()
   {
   WitPeriod expPer;

   myBaMat_->myBaMgr ()->retrieveExpPer (expPer);

   select (expPer);
   }

//------------------------------------------------------------------------------

void WitBaPt::printAlteration ()
   {
   myBaMat_->print ();
   }

//------------------------------------------------------------------------------
// Implementation of class BaCand.
//------------------------------------------------------------------------------

WitBaCand::WitBaCand (WitPeriod expPer, WitProblem * theProblem):

      WitSelCand (theProblem),
      myExpPer_  (expPer)
   {
   }

//------------------------------------------------------------------------------

WitBaCand::~WitBaCand ()
   {
   }

//------------------------------------------------------------------------------

void WitBaCand::prtID ()
   {
   fprintf (msgFile (), "      Exp Period:    %d\n", myExpPer_);
   }

//------------------------------------------------------------------------------

void WitBaCand::getBaData (WitPeriod & expPer)
   {
   expPer = myExpPer_;
   }
