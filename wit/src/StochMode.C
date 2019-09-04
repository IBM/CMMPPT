//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "StochMode.C"
//
// Contains the implementation of the following classes:
//
//    StochModeMgr
//    StochAssoc
//    StochAttMgr
//    StochAtt
//------------------------------------------------------------------------------

#include <StochAttMgr.h>
#include <StochAtt.h>
#include <StageMgr.h>
#include <ScenMgr.h>
#include <ScenAttMgr.h>
#include <ScenInputAtt.h>
#include <Scenario.h>
#include <StochImpMgr.h>
#include <StochOptMgr.h>
#include <Post.h>
#include <OptStarter.h>
#include <GlobalComp.h>
#include <OptComp.h>
#include <CompMgr.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class StochModeMgr.
//------------------------------------------------------------------------------

WitStochModeMgr::WitStochModeMgr (WitProblem * theProblem):

      WitProbAssoc   (theProblem),

      myStochAttMgr_ (NULL),
      myStageMgr_    (NULL),
      myScenMgr_     (NULL),
      myScenAttMgr_  (NULL),
      myStochOptMgr_ (NULL),
      uploadingData_ (false),
      stochSolnMode_ (false),
      clearingSoln_  (false)
   {
   myMsgFac () ("enterStochModeMsg");

   validateCoreData ();

   myStochAttMgr_ = new WitStochAttMgr       (this);
   myScenMgr_     = new WitScenMgr           (this);
   myScenAttMgr_  = new WitScenAttMgr        (this);

   myStageMgr_    = WitStageMgr::newStageMgr (this);
   }

//------------------------------------------------------------------------------

WitStochModeMgr::~WitStochModeMgr ()
   {
   if (stochSolnMode_)
      clearSoln ();

   myMsgFac () ("leaveStochModeMsg");

   delete myStageMgr_;
   delete myScenAttMgr_;
   delete myScenMgr_;
   delete myStochAttMgr_;
   }

//------------------------------------------------------------------------------

void WitStochModeMgr::copyAttrsFrom (WitStochModeMgr * theStochModeMgr)
   {
   myStageMgr_  ->copyAttrsFrom (theStochModeMgr->myStageMgr_);
   myScenMgr_   ->copyAttrsFrom (theStochModeMgr->myScenMgr_);
   myScenAttMgr_->copyInputFrom (theStochModeMgr->myScenAttMgr_);
   }

//------------------------------------------------------------------------------

void WitStochModeMgr::uploadInput ()
   {
   uploadingData_ = true;

   myScenAttMgr_->uploadInput ();

   uploadingData_ = false;
   }

//------------------------------------------------------------------------------

void WitStochModeMgr::uploadSoln ()
   {
   uploadingData_ = true;

   myScenAttMgr_->uploadSoln ();

   uploadingData_ = false;
   }

//------------------------------------------------------------------------------

void WitStochModeMgr::implode ()
   {
   stronglyAssert (myStochOptMgr_ == NULL);

   myStochOptMgr_ = new WitStochOptMgr (this);

   myStochOptMgr_->implode ();

   delete myStochOptMgr_;

   myStochOptMgr_ = NULL;

   stochSolnMode_ = true;

   myMsgFac () ("enterStochSolnModeMsg");
   }

//------------------------------------------------------------------------------

void WitStochModeMgr::clearSoln ()
   {
   stronglyAssert (stochSolnMode_);

   myMsgFac () ("leaveStochSolnModeMsg");

   stochSolnMode_ = false;

   myScenAttMgr_->clearSoln ();

   clearCompSoln ();
   }

//------------------------------------------------------------------------------

void WitStochModeMgr::clearCompSoln ()
   {
   WitComponent * theComp;

   myPostprocessor ()->unpostprocess ();

   clearingSoln_ = true;

   forEachEl (theComp, myCompMgr ()->allComponents ())
      theComp->clearStochSoln ();

   clearingSoln_ = false;
   }

//------------------------------------------------------------------------------

void WitStochModeMgr::passObjValue (double theValue)
   {
   witAssert (myStochOptMgr ()->uploadingSoln ());

   uploadingData_ = true;

   myOptComp ()->storeStochObjValue (theValue);

   uploadingData_ = false;
   }

//------------------------------------------------------------------------------

void WitStochModeMgr::passBoundsValue (double theValue)
   {
   witAssert (myStochOptMgr ()->uploadingSoln ());

   uploadingData_ = true;

   myOptComp ()->storeStochBoundsValue (theValue);

   uploadingData_ = false;
   }

//------------------------------------------------------------------------------

void WitStochModeMgr::validateCoreData ()
   {
   verifyGlobalBool (  "computeCriticalList", 
      myGlobalComp  ()->computeCriticalList (), false);

   verifyGlobalBool (  "compPrices",
      myOptComp     ()->compPrices          (), false);

   verifyGlobalBool (  "accAfterOptImp",
      myOptComp     ()->accAfterOptImp      (), false);

   verifyGlobalBool (  "accAfterSoftLB",
      myOptComp     ()->accAfterSoftLB      (), false);

   verifyGlobalBool (  "mipMode",
      myOptComp     ()->mipMode             (), false);

   if (myCompMgr ()->nParts () == 0)
      myMsgFac () ("noPartsStochModeSmsg");
   }

//------------------------------------------------------------------------------

void WitStochModeMgr::verifyGlobalBool (
      const char * theAttName,
      bool         theVal,
      bool         reqVal)
   {
   if (theVal != reqVal)
      myMsgFac () ("invBoolAttStochModeSmsg", theAttName, reqVal);
   }

//------------------------------------------------------------------------------
// Implementation of class StochAssoc.
//------------------------------------------------------------------------------

WitStochAssoc::WitStochAssoc (WitStochModeMgr * theStochModeMgr):
                        
      WitProbAssoc    (theStochModeMgr),
      myStochModeMgr_ (theStochModeMgr)
   {
   }

//------------------------------------------------------------------------------

WitStochAssoc::WitStochAssoc (WitStochAssoc * theStochAssoc):
                        
      WitProbAssoc    (theStochAssoc),
      myStochModeMgr_ (theStochAssoc->myStochModeMgr_)
   {
   }

//------------------------------------------------------------------------------

WitStochAssoc::~WitStochAssoc ()
   {
   }

//------------------------------------------------------------------------------
// Implementation of class StochAttMgr.
//------------------------------------------------------------------------------

WitStochAttMgr::WitStochAttMgr (WitStochModeMgr * theStochModeMgr):

      WitStochAssoc   (theStochModeMgr),
      myStochAttMap_  (),
      cachedStochAtt_ (NULL)
   {
   makeStochAtts ();
   }

//------------------------------------------------------------------------------

WitStochAttMgr::~WitStochAttMgr ()
   {
   WitStochAtt * theStochAtt;

   while (not myStochAttMap_.empty ())
      {
      theStochAtt = myStochAttMap_.begin ()->second;

      myStochAttMap_.erase (myStochAttMap_.begin ());

      delete theStochAtt;
      }
   }

//------------------------------------------------------------------------------

void WitStochAttMgr::makeScenInputStochAtt (
      const char *         theName,
      WitAbsScenInputAtt * theAbsScenInputAtt)
   {
   makeStochAtt (theName, theAbsScenInputAtt);

   findStochAtt (theName)->setIsScenSpec        (true);
   findStochAtt (theName)->setCanSetInStochMode (true);
   findStochAtt (theName)->setCanSetInSolnMode  (false);
   }

//------------------------------------------------------------------------------

void WitStochAttMgr::makeScenSolnStochAtt (const char * theName)
   {
   makeStochAtt (theName);
   findStochAtt (theName)->setIsScenSpec        (true);
   findStochAtt (theName)->setCanSetInStochMode (false);
   findStochAtt (theName)->setCanSetInSolnMode  (false);
   }

//------------------------------------------------------------------------------

bool WitStochAttMgr::valueOf (
      bool (WitStochAtt::* theAccFunc) () const, 
      const char *         theAttName)
   {
   WitStochAtt * theStochAtt;

   theStochAtt = findStochAtt (theAttName);

   if (theStochAtt == NULL)
      return false;
   else
      return (theStochAtt->*theAccFunc) ();
   }

//------------------------------------------------------------------------------

void WitStochAttMgr::prepSetStochModeAttr (const char * theAttName)
   {
   WitStochAtt * theStochAtt;

   theStochAtt = findStochAtt (theAttName);

   stronglyAssert (theStochAtt != NULL);

   stronglyAssert (theStochAtt->canSetInStochMode ());

   if (stochSolnMode ())
      stronglyAssert (theStochAtt->canSetInSolnMode ());

   stronglyAssert (theStochAtt->myAbsScenInputAtt () == NULL);
   }

//------------------------------------------------------------------------------

void WitStochAttMgr::prepSetScenSpecAttr (
      const char * theAttName,
      WitDelComp * theDelComp)
   {
   WitStochAtt *        theStochAtt;
   WitAbsScenInputAtt * theAbsScenInputAtt;

   stronglyAssert (not stochSolnMode ());

   theStochAtt = findStochAtt (theAttName);

   stronglyAssert (theStochAtt != NULL);

   theAbsScenInputAtt = theStochAtt->myAbsScenInputAtt ();

   stronglyAssert (theAbsScenInputAtt != NULL);

   theAbsScenInputAtt->prepSet (theDelComp);
   }

//------------------------------------------------------------------------------

void WitStochAttMgr::prepSetBoundSet (WitBoundSet * theBoundSet)
   {
   prepSetBoundSetAttr ("hardLB", theBoundSet);
   prepSetBoundSetAttr ("softLB", theBoundSet);
   prepSetBoundSetAttr ("hardUB", theBoundSet);
   }

//------------------------------------------------------------------------------

void WitStochAttMgr::prepSetBoundSetAttr (
      const char *  theAttName,
      WitBoundSet * theBoundSet)
   {
   WitStochAtt *        theStochAtt;
   WitAbsScenInputAtt * theAbsScenInputAtt;

   theStochAtt = findStochAtt (theAttName);

   stronglyAssert (theStochAtt != NULL);

   stronglyAssert (not stochSolnMode ());

   theAbsScenInputAtt = theStochAtt->myAbsScenInputAtt ();

   stronglyAssert (theAbsScenInputAtt != NULL);

   theAbsScenInputAtt->prepSet (theBoundSet);
   }

//------------------------------------------------------------------------------

void WitStochAttMgr::makeStochAtts ()
   {
   makeStochAtt ("probability"    );
   findStochAtt ("probability"    )->setIsScenSpec        (true);
   findStochAtt ("probability"    )->setCanSetInStochMode (true);
   findStochAtt ("probability"    )->setCanSetInSolnMode  (false);

   makeStochAtt ("objectStage"    );
   findStochAtt ("objectStage"    )->setIsScenSpec        (false);
   findStochAtt ("objectStage"    )->setCanSetInStochMode (true);
   findStochAtt ("objectStage"    )->setCanSetInSolnMode  (false);

   makeStochAtt ("periodStage"    );
   findStochAtt ("periodStage"    )->setIsScenSpec        (false);
   findStochAtt ("periodStage"    )->setCanSetInStochMode (true);
   findStochAtt ("periodStage"    )->setCanSetInSolnMode  (false);

   makeStochAtt ("stochMode"      );
   findStochAtt ("stochMode"      )->setIsScenSpec        (false);
   findStochAtt ("stochMode"      )->setCanSetInStochMode (true);
   findStochAtt ("stochMode"      )->setCanSetInSolnMode  (true);

   makeStochAtt ("currentScenario");
   findStochAtt ("currentScenario")->setIsScenSpec        (false);
   findStochAtt ("currentScenario")->setCanSetInStochMode (true);
   findStochAtt ("currentScenario")->setCanSetInSolnMode  (true);

   makeStochAtt ("appData"        );
   findStochAtt ("appData"        )->setIsScenSpec        (false);
   findStochAtt ("appData"        )->setCanSetInStochMode (true);
   findStochAtt ("appData"        )->setCanSetInSolnMode  (true);

   makeStochAtt ("stockBounds"    );
   findStochAtt ("stockBounds"    )->setIsScenSpec        (true);
   findStochAtt ("stockBounds"    )->setCanSetInStochMode (true);
   findStochAtt ("stockBounds"    )->setCanSetInSolnMode  (false);

   makeStochAtt ("cumShipBounds"  );
   findStochAtt ("cumShipBounds"  )->setIsScenSpec        (true);
   findStochAtt ("cumShipBounds"  )->setCanSetInStochMode (true);
   findStochAtt ("cumShipBounds"  )->setCanSetInSolnMode  (false);

   makeStochAtt ("execBounds"     );
   findStochAtt ("execBounds"     )->setIsScenSpec        (true);
   findStochAtt ("execBounds"     )->setCanSetInStochMode (true);
   findStochAtt ("execBounds"     )->setCanSetInSolnMode  (false);
   }

//------------------------------------------------------------------------------

void WitStochAttMgr::makeStochAtt (const char * theName)
   {
   makeStochAtt (theName, NULL);
   }

//------------------------------------------------------------------------------

void WitStochAttMgr::makeStochAtt (
      const char *         theName,
      WitAbsScenInputAtt * theAbsScenInputAtt)
   {
   WitStochAtt * theStochAtt;
   const char *  storedName;

   stronglyAssert (findStochAtt (theName) == NULL);

   theStochAtt                = new WitStochAtt (theName, theAbsScenInputAtt);

   storedName                 = theStochAtt->myName ().myCstring ();

   myStochAttMap_[storedName] = theStochAtt;
   }

//------------------------------------------------------------------------------

WitStochAtt * WitStochAttMgr::findStochAtt (const char * theName)
   {
   WitStochAttMap::iterator theItr;

   if (cachedStochAtt_ != NULL)
      if (cachedStochAtt_->myName () == theName)
         return cachedStochAtt_;

   theItr = myStochAttMap_.find (theName);

   if (theItr != myStochAttMap_.end ())
      cachedStochAtt_ = theItr->second;
   else
      cachedStochAtt_ = NULL;

   return cachedStochAtt_;
   }

//------------------------------------------------------------------------------
// Implementation of class StochAtt.
//------------------------------------------------------------------------------

WitStochAtt::WitStochAtt (
         const char *         theName,
         WitAbsScenInputAtt * theAbsScenInputAtt):

      myName_            (theName),
      isScenSpec_        (false),
      canSetInStochMode_ (false),
      canSetInSolnMode_  (false),
      myAbsScenInputAtt_ (theAbsScenInputAtt)
   {
   }

//------------------------------------------------------------------------------

WitStochAtt::~WitStochAtt ()
   {
   }

//------------------------------------------------------------------------------

void WitStochAtt::setIsScenSpec (bool theValue)
   {
   isScenSpec_ = theValue;
   }

//------------------------------------------------------------------------------

void WitStochAtt::setCanSetInStochMode (bool theValue)
   {
   canSetInStochMode_ = theValue;
   }

//------------------------------------------------------------------------------

void WitStochAtt::setCanSetInSolnMode (bool theValue)
   {
   canSetInSolnMode_ = theValue;
   }
