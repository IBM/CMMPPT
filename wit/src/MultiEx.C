//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "MultiEx.C"
//
// Contains the implementation of the following classes:
// 
//    MeMgr.
//    MeDir.
//    NstnMeDir.
//    AsapMeDir.
//    MeSitePt.
//    MePt.
//    MeCand.
//------------------------------------------------------------------------------

#include <MeMgr.h>
#include <MeDir.h>
#include <MeCand.h>
#include <MeSitePt.h>
#include <MePt.h>
#include <DetAltPt.h>
#include <SplitPt.h>
#include <Selector.h>
#include <SglSrcMgr.h>
#include <ExecPerSch.h>
#include <GlobalComp.h>
#include <Material.h>
#include <Operation.h>
#include <BopEntry.h>
#include <MsgFac.h>
#include <PtrTVec.h>

//------------------------------------------------------------------------------
// Implementation of class MeMgr.
//------------------------------------------------------------------------------

inline bool WitMeMgr::hasMeSitePts (WitBopEntry * theBopEnt)
   {
   return myMeSitePts_.myPtrTVecAt (theBopEnt).isAllocated ();
   }

//------------------------------------------------------------------------------

WitMeMgr::WitMeMgr (WitSelector * theSelector):

      WitSelMgr        (theSelector),
      myMeDirForNstn_  (NULL),
      myMeDirForAsap_  (NULL),
      myMeSitePts_     (),
      initDirIsAsap_   (false),
      savedExecPer_    (-1),
      myRecoveryPairs_ (myProblem ())
   {
   myMsgFac () ("multiExecMsg");

   myMeDirForNstn_ = new WitNstnMeDir (this);
   myMeDirForAsap_ = new WitAsapMeDir (this);
   
   myMeSitePts_.allocate1D (myProblem ());

   buildMeSitePts ();

   if (myGlobalComp ()->twoWayMultiExec ())
      myMsgFac () ("twmeMsg");
   }

//------------------------------------------------------------------------------

WitMeMgr::~WitMeMgr ()
   {
   WitBopEntry * theBopEnt;
   WitPeriod     expPer;
   WitMeSitePt * theMeSitePt;

   forEachBopEntry (theBopEnt, myProblem ())
      if (hasMeSitePts (theBopEnt))
         {
         expPer = -1;

         while (findNextMeSitePt (theBopEnt, expPer, theMeSitePt))
            delete theMeSitePt;
         }

   delete myMeDirForAsap_;
   delete myMeDirForNstn_;
   }

//------------------------------------------------------------------------------

bool WitMeMgr::instNeededFor (WitProblem * theProblem)
   {
   return theProblem->myGlobalComp ()->multiExecNeeded ();
   }

//------------------------------------------------------------------------------

void WitMeMgr::setInitDirIsAsap (bool theValue)
   {
   initDirIsAsap_ = theValue;
   }

//------------------------------------------------------------------------------

WitPeriod WitMeMgr::selExecPer (WitBopEntry * theBopEnt, WitPeriod expPer)
   {
   WitMePt * theMePt;

   theMePt = myMePtWithSel (theBopEnt, expPer);

   return
      (theMePt != NULL)?
         theMePt->selExecPer ():
         theBopEnt ->expExecPeriod ()[expPer];
   }

//------------------------------------------------------------------------------

void WitMeMgr::print ()
   {
   WitBopEntry * theBopEnt;
   WitPeriod     expPer;
   WitMeSitePt * theMeSitePt;

   fprintf (msgFile (),
      "\n"
      "Multi-Exec Selection Points:\n");

   forEachBopEntry (theBopEnt, myProblem ())
      if (hasMeSitePts (theBopEnt))
         {
         expPer = -1;

         while (findNextMeSitePt (theBopEnt, expPer, theMeSitePt))
            {
            theMeSitePt->myMePtForNstn ()->print ();

            if (myGlobalComp ()->twoWayMultiExec ())
               theMeSitePt->myMePtForAsap ()->print ();
            }
         }

   fprintf (msgFile (), "\n");
   }

//------------------------------------------------------------------------------

void WitMeMgr::saveExecPer (WitPeriod execPer)
   {
   witAssert (savedExecPer_ == -1);
   witAssert (execPer       != -1);

   savedExecPer_ = execPer;
   }

//------------------------------------------------------------------------------

void WitMeMgr::retrieveExecPer (WitPeriod & execPer)
   {
   witAssert (savedExecPer_ != -1);

   execPer       = savedExecPer_;
   savedExecPer_ = -1;
   }

//------------------------------------------------------------------------------

void WitMeMgr::storeRecoveryPair (WitBopEntry * theBopEnt, WitPeriod expPer)
   {
   myRecoveryPairs_.push (theBopEnt, expPer);
   }

//------------------------------------------------------------------------------

void WitMeMgr::recoverInitState ()
   {
   WitBopEntry * theBopEnt;
   WitPeriod     expPer;
   WitMeSitePt * theMeSitePt;

   while (myRecoveryPairs_.pop (theBopEnt, expPer))
      {
      theMeSitePt = myMeSitePts_.myPtrAt (theBopEnt, expPer);

      theMeSitePt->   myMePtForNstn ()->recoverInitState ();

      if (myGlobalComp ()->twoWayMultiExec ())
         theMeSitePt->myMePtForAsap ()->recoverInitState ();
      }
   }

//------------------------------------------------------------------------------

bool WitMeMgr::selSplitNeeded ()
   {
   WitBopEntry * theBopEnt;

   if (myGlobalComp ()->selSplit ())
      return true;

   if (myGlobalComp ()->nonMrSelSplit ())
      return true;

   forEachBopEntry (theBopEnt, myProblem ())
      if (theBopEnt->expAllowed ())
         if (theBopEnt->meSelSplit ())
            return true;

   return false;
   }

//------------------------------------------------------------------------------

bool WitMeMgr::selIsSplit (WitBopEntry * theBopEnt, WitPeriod expPer)
   {
   WitMePt * theMePt;

   if (not selSplit ())
      return false;

   theMePt = myMePtWithSel (theBopEnt, expPer);

   if (theMePt == NULL)
      return false;

   return theMePt->isSplit ();
   }

//------------------------------------------------------------------------------

void WitMeMgr::defineSplit (
      WitBopEntry *    theBopEnt,
      WitPeriod        expPer,
      double           expVol,
      WitPerDblStack & theSplitPairs,
      double &         unbddVol)
   {
   WitSelCandDblStack theAbsSplitPairs (myProblem ());
   WitSelCand *       theSelCand;
   double             splitVol;
   WitPeriod          execPer;
   WitFixedPer *      theFixedPer;

   witAssert (selIsSplit (theBopEnt, expPer));

   myMePtWithSel (theBopEnt, expPer)->
      mySplitPt ()->
         defineSplit (expVol, theAbsSplitPairs, unbddVol);

   theSplitPairs.clear ();

   while (theAbsSplitPairs.pop (theSelCand, splitVol))
      {
      theSelCand->getMeData (execPer);

      theFixedPer = mySelector ()->myFixedPer ()[execPer];

      theSplitPairs.push (theFixedPer, splitVol);
      }
   }

//------------------------------------------------------------------------------

void WitMeMgr::recBopEntExecPerSS (
      WitBopEntry * theBopEnt, 
      WitPeriod     execPer, 
      double        expVol)
   {
   WitPeriod     expPer;
   WitMeSitePt * theMeSitePt;
   WitMeDir *    theMeDir;

   witAssert (selSplit ());

   if (not hasMeSitePts (theBopEnt))
      return;

   expPer      = theBopEnt->impactPeriod ()[execPer];

   theMeSitePt = myMeSitePts_.myPtrAt (theBopEnt, expPer);

   if (theMeSitePt == NULL)
      return;

   theMeDir = chooseMeDir (theBopEnt->myPart ());

   theMeDir->myMePt (theMeSitePt)->recordFlowSS (expVol);
   }

//------------------------------------------------------------------------------

WitAltPt * WitMeMgr::myAltPtFor (
      WitBopEntry * theBopEnt,
      WitConsEntry *,
      WitPeriod     execPer,
      WitPeriod)
   {
   WitPeriod expPer;
   WitMePt * theMePt;

   expPer  = theBopEnt->impactPeriod ()[execPer];

   theMePt = myMePtWithSel (theBopEnt, expPer);

   return
      (theMePt != NULL)?
         theMePt->myDetAltPt ():
         NULL;
   }

//------------------------------------------------------------------------------

bool WitMeMgr::validForNetting ()
   {
   return true;
   }

//------------------------------------------------------------------------------

bool WitMeMgr::validForExecBounds ()
   {
   return true;
   }

//------------------------------------------------------------------------------

bool WitMeMgr::validForLTBounds ()
   {
   return false;
   }

//------------------------------------------------------------------------------

const char * WitMeMgr::selCaseName ()
   {
   return "Multi-Exec";
   }

//------------------------------------------------------------------------------

void WitMeMgr::buildMeSitePts ()
   {
   WitBopEntry * theBopEnt;
   WitPeriod     expPer;
   WitPeriod     execPerF;
   WitPeriod     execPerL;

   WitExecPerSched <WitBopEntry> * theExecPerSched;

   theExecPerSched = new WitExecPerSched <WitBopEntry> (myProblem ());

   forEachBopEntry (theBopEnt, myProblem ())
      forEachPeriod (expPer, myProblem ())
         {
         theExecPerSched->getExecPerRange (
            theBopEnt,
            expPer,
            execPerF,
            execPerL);

         for (; execPerF <= execPerL; execPerF ++)
            if (theBopEnt->execPerOKForExp (execPerF))
               break;

         for (; execPerL >= execPerF; execPerL --)
            if (theBopEnt->execPerOKForExp (execPerL))
               break;

         if (execPerF >= execPerL)
            continue;

         if (not myMeSitePts_.myPtrTVecAt (theBopEnt).isAllocated ())
            myMeSitePts_.myPtrTVecAt (theBopEnt).allocate (myProblem ());

         myMeSitePts_.myPtrAt (theBopEnt, expPer) = 
            new WitMeSitePt (
               theBopEnt,
               execPerF,
               execPerL,
               this);
         }

   delete theExecPerSched;
   }

//------------------------------------------------------------------------------

WitMePt * WitMeMgr::myMePtWithSel (WitBopEntry * theBopEnt, WitPeriod expPer)
   {
   WitMeSitePt * theMeSitePt;
   WitMeDir *    theMeDir;
   WitMePt *     theMePt;

   if (not hasMeSitePts (theBopEnt))
      return NULL;

   theMeSitePt = myMeSitePts_.myPtrAt (theBopEnt, expPer);

   if (theMeSitePt == NULL)
      return NULL;

   theMeDir = chooseMeDir (theBopEnt->myPart ());

   theMePt  = theMeDir->myMePt (theMeSitePt);

   return
      (theMePt->selExecPer () >= 0)?
         theMePt:
         NULL;
   }

//------------------------------------------------------------------------------

WitMeDir * WitMeMgr::chooseMeDir (WitPart * thePart)
   {
   WitMaterial * theMat;

   if (not myGlobalComp ()->twoWayMultiExec ())
      return myMeDirForNstn_;

   if (initDirIsAsap_)
      return myMeDirForAsap_;

   theMat = thePart->thisMat ();

   if (theMat != NULL)
      if (theMat->buildAsap ())
         return myMeDirForAsap_;

   return myMeDirForNstn_;
   }

//------------------------------------------------------------------------------

bool WitMeMgr::findNextMeSitePt (
      WitBopEntry *   theBopEnt,
      WitPeriod &     expPer,
      WitMeSitePt * & theMeSitePt)
   {
   while (++ expPer < nPeriods ())
      {
      theMeSitePt = myMeSitePts_.myPtrAt (theBopEnt, expPer);
   
      if (theMeSitePt != NULL)
         return true;
      }

   return false;
   }

//------------------------------------------------------------------------------
// Implementation of class MeDir.
//------------------------------------------------------------------------------

WitMeDir::~WitMeDir ()
   {
   }

//------------------------------------------------------------------------------

WitMeDir::WitMeDir (WitMeMgr * theMeMgr):

      WitProbAssoc (theMeMgr),
      myMeMgr_     (theMeMgr)
   {
   }

//------------------------------------------------------------------------------
// Implementation of class NstnMeDir.
//------------------------------------------------------------------------------

WitNstnMeDir::WitNstnMeDir (WitMeMgr * theMeMgr):
      WitMeDir (theMeMgr)
   {
   }

//------------------------------------------------------------------------------

WitNstnMeDir::~WitNstnMeDir ()
   {
   }

//------------------------------------------------------------------------------

WitMeDir * WitNstnMeDir::oppMeDir ()
   {
   return myMeMgr ()->myMeDirForAsap ();
   }

//------------------------------------------------------------------------------

WitMePt * WitNstnMeDir::myMePt (WitMeSitePt * theMeSitePt)
   {
   return theMeSitePt->myMePtForNstn ();
   }

//------------------------------------------------------------------------------

WitPeriod WitNstnMeDir::initSelExecPer (WitMeSitePt * theMeSitePt)
   {
   return theMeSitePt->lastExecPer ();
   }

//------------------------------------------------------------------------------

WitPeriod WitNstnMeDir::nextExecPer (
      WitPeriod     execPer,
      WitMeSitePt * theMeSitePt)
   {
   witAssert (execPer >= 0);

   if (execPer == theMeSitePt->firstExecPer ())
      return -1;

   if (myGlobalComp ()->twoWayMultiExec ())
      if (execPer == theMeSitePt->myMePtForAsap ()->selExecPer ())
         return -1;

   return execPer - 1;
   }

//------------------------------------------------------------------------------

const char * WitNstnMeDir::myID ()
   {
   return "NSTN";
   }

//------------------------------------------------------------------------------
// Implementation of class AsapMeDir.
//------------------------------------------------------------------------------

WitAsapMeDir::WitAsapMeDir (WitMeMgr * theMeMgr):
      WitMeDir (theMeMgr)
   {
   }

//------------------------------------------------------------------------------

WitAsapMeDir::~WitAsapMeDir ()
   {
   }

//------------------------------------------------------------------------------

WitMeDir * WitAsapMeDir::oppMeDir ()
   {
   return myMeMgr ()->myMeDirForNstn ();
   }

//------------------------------------------------------------------------------

WitMePt * WitAsapMeDir::myMePt (WitMeSitePt * theMeSitePt)
   {
   return theMeSitePt->myMePtForAsap ();
   }

//------------------------------------------------------------------------------

WitPeriod WitAsapMeDir::initSelExecPer (WitMeSitePt * theMeSitePt)
   {
   return theMeSitePt->firstExecPer ();
   }

//------------------------------------------------------------------------------

WitPeriod WitAsapMeDir::nextExecPer (
      WitPeriod     execPer,
      WitMeSitePt * theMeSitePt)
   {
   witAssert (execPer >= 0);

   if (execPer == theMeSitePt->lastExecPer ())
      return -1;

   if (execPer == theMeSitePt->myMePtForNstn ()->selExecPer ())
      return -1;

   return execPer + 1;
   }

//------------------------------------------------------------------------------

const char * WitAsapMeDir::myID ()
   {
   return "ASAP";
   }

//------------------------------------------------------------------------------
// Implementation of class MeSitePt.
//------------------------------------------------------------------------------

WitMeSitePt::WitMeSitePt (
      WitBopEntry * theBopEnt,
      WitPeriod     firstExecPerVal,
      WitPeriod     lastExecPerVal,
      WitMeMgr *    theMeMgr):

      myBopEnt_      (theBopEnt),
      firstExecPer_  (firstExecPerVal),
      lastExecPer_   (lastExecPerVal),
      myMePtForNstn_ (NULL),
      myMePtForAsap_ (NULL)
   {
   myMePtForNstn_ = new WitMePt (this, theMeMgr->myMeDirForNstn ());

   if (theMeMgr->myGlobalComp ()->twoWayMultiExec ())
      myMePtForAsap_ = new WitMePt (this, theMeMgr->myMeDirForAsap ());
   }

//------------------------------------------------------------------------------

WitMeSitePt::~WitMeSitePt ()
   {
   delete myMePtForNstn_;
   delete myMePtForAsap_;
   }

//------------------------------------------------------------------------------

WitPeriod WitMeSitePt::nextExecPer (WitPeriod execPer, WitMeDir * theMeDir)
   {
   while (true)
      {
      execPer = theMeDir->nextExecPer (execPer, this);

      if (execPer < 0)
         return -1;

      if (myBopEnt_->execPerOKForExp (execPer))
         if (myBopEnt_->impactPeriod ()[execPer] == myExpPer ())
            return execPer;
      }

   return -1;
      //
      // Avoids a compiler warning.
   }

//------------------------------------------------------------------------------

void WitMeSitePt::prtID ()
   {
   fprintf (myMePtForNstn_->myMeDir ()->myMsgFac ().myFile (),
      "   Operation:        %s\n"
      "   BOP Entry #:      %d\n"
      "   Exp Per:          %d\n",
      myBopEnt_->myOperationName ().myCstring (),
      myBopEnt_->localIndex (),
      myExpPer ());
   }

//------------------------------------------------------------------------------

WitPeriod WitMeSitePt::myExpPer ()
   {
   return myBopEnt_->impactPeriod ()[firstExecPer_];
   }

//------------------------------------------------------------------------------
// Implementation of class MePt.
//------------------------------------------------------------------------------

WitMePt::WitMePt (WitMeSitePt * theMeSitePt, WitMeDir * theMeDir):
      WitDetSelPt (),

      myMeSitePt_ (theMeSitePt),
      myMeDir_    (theMeDir),
      selExecPer_ (-1)
   {              
   select (myMeDir_->initSelExecPer (myMeSitePt_));
   }

//------------------------------------------------------------------------------

WitMePt::~WitMePt ()
   {
   }

//------------------------------------------------------------------------------

void WitMePt::print ()
   {
   FILE *        theFile;
   WitBopEntry * theBopEnt; 

   theFile   = myMeDir_->myMsgFac ().myFile ();

   theBopEnt = myMeSitePt_->myBopEnt ();
 
   fprintf (theFile,
      "\n"
      "Multi-Exec Selection Point:\n"
      "   Operation %s, BopEntry #%d, Exp Per %d, %s\n",
      theBopEnt->myOperationName ().myCstring (),
      theBopEnt->localIndex (),
      myMeSitePt_->myExpPer (),
      myMeDir_->myID ());

   if (selExecPer_ >= 0)
      fprintf (theFile, "      Selected Exec Per: %d\n", selExecPer_);
   else
      fprintf (theFile, "      Selected Exec Per: None\n");
   }

//------------------------------------------------------------------------------

void WitMePt::select (WitPeriod execPer)
   {
   if (mySelMgr ()->mySelector ()->sglSrc ())
      mySelMgr ()->mySelector ()->mySglSrcMgr ()->saveConfigVal (selExecPer_);

   selExecPer_ = execPer;
   }

//------------------------------------------------------------------------------

void WitMePt::recoverInitState ()
   {
   selExecPer_ = myMeDir_->initSelExecPer (myMeSitePt_);

   if (mySelMgr ()->selPrintLevelAt (3))
      printRecovery ();
   }

//------------------------------------------------------------------------------

WitSelMgr * WitMePt::mySelMgr ()
   {
   return myMeDir_->myMeMgr ();
   }

//------------------------------------------------------------------------------

void WitMePt::prtID ()
   {
   myMeSitePt_->prtID ();

   fprintf (myMeDir_->myMsgFac ().myFile (),
      "   Direction:        %s\n",
      myMeDir_->myID ());
   }

//------------------------------------------------------------------------------

bool WitMePt::splittable ()
   {
   if (myMeDir_->myGlobalComp ()->selSplit ())
      return true;

   if (myMeDir_->myGlobalComp ()->nonMrSelSplit ())
      return true;

   return myMeSitePt_->myBopEnt ()->meSelSplit ();
   }

//------------------------------------------------------------------------------

void WitMePt::getSortData (WitPart * & thePart, WitPeriod & thePer)
   {
   thePart = myMeSitePt_->myBopEnt ()->myPart ();
   thePer  = myMeSitePt_->myExpPer ();
   }

//------------------------------------------------------------------------------

WitSelCand * WitMePt::newSelCand ()
   {
   return new WitMeCand (selExecPer_, myMeDir_->myProblem ());
   }

//------------------------------------------------------------------------------

bool WitMePt::hasResAlt ()
   {
   WitPeriod execPer;

   witAssert (selExecPer_ >= 0);

   execPer = myMeSitePt_->nextExecPer (selExecPer_, myMeDir_);

   return (execPer >= 0);
   }

//------------------------------------------------------------------------------

void WitMePt::alterSelection ()
   {
   advanceSel ();

   if (myMeDir_->selPrintLevelAt (2))
      print ();

   if (myMeDir_->myGlobalComp ()->twoWayMultiExec ())
      if (selExecPer_ < 0)
         myMeDir_->oppMeDir ()->myMePt (myMeSitePt_)->deSelectAsNeeded ();
   }

//------------------------------------------------------------------------------

void WitMePt::storeRecoveryPt ()
   {
   myMeDir_->
      myMeMgr ()->
         storeRecoveryPair (
            myMeSitePt_->myBopEnt (),
            myMeSitePt_->myExpPer ());
   }

//------------------------------------------------------------------------------

void WitMePt::tempAlterSel ()
   {
   myMeDir_->myMeMgr ()->saveExecPer (selExecPer_);

   advanceSel ();
   }

//------------------------------------------------------------------------------

void WitMePt::cancelTempAlt ()
   {
   WitPeriod execPer;

   myMeDir_->myMeMgr ()->retrieveExecPer (execPer);

   select (execPer);
   }

//------------------------------------------------------------------------------

void WitMePt::printAlteration ()
   {
   }

//------------------------------------------------------------------------------

void WitMePt::advanceSel ()
   {
   WitPeriod execPer;

   witAssert (selExecPer_ >= 0);

   execPer = myMeSitePt_->nextExecPer (selExecPer_, myMeDir_);

   select (execPer);
   }

//------------------------------------------------------------------------------

void WitMePt::deSelectAsNeeded ()
   {
   if (selExecPer_ < 0)
      return;

   select (-1);

   if (myMeDir_->selPrintLevelAt (2))
      print ();
   }

//------------------------------------------------------------------------------

void WitMePt::printRecovery ()
   {
   fprintf (myMeDir_->myMsgFac ().myFile (),
      "\n"
      "   Initial Multi-Exec Selection Recovered:\n"
      "      Operation:  %s\n"
      "      BopEntry #: %d\n"
      "      Exp Per:    %d\n"
      "      Direction:  %s\n"
      "      Exec Per:   %d\n\n",
      myMeSitePt_->myBopEnt ()->myOperationName ().myCstring (),
      myMeSitePt_->myBopEnt ()->localIndex (),
      myMeSitePt_->myExpPer (),
      myMeDir_   ->myID     (),
      selExecPer_);
   }

//------------------------------------------------------------------------------
// Implementation of class MeCand.
//------------------------------------------------------------------------------

WitMeCand::WitMeCand (WitPeriod execPer, WitProblem * theProblem):

      WitSelCand (theProblem),
      myExecPer_ (execPer)
   {
   }

//------------------------------------------------------------------------------

WitMeCand::~WitMeCand ()
   {
   }

//------------------------------------------------------------------------------

void WitMeCand::prtID ()
   {
   fprintf (msgFile (), "      Exec Per:      %d\n", myExecPer_);
   }

//------------------------------------------------------------------------------

void WitMeCand::getMeData (WitPeriod & execPer)
   {
   execPer = myExecPer_;
   }
