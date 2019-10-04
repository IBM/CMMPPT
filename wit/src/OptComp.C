//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "OptComp.C"
//
// Contains the implementation of class OptComp.
//------------------------------------------------------------------------------

#include <OptComp.h>
#include <OptStarter.h>
#include <CpxParSpecMgr.h>
#include <MultiObjMgr.h>
#include <Objective.h>
#include <Pre.h>
#include <Part.h>
#include <DataWrit.h>
#include <DetOptImpMgr.h>
#include <StochImpMgr.h>
#include <Material.h>
#include <Operation.h>
#include <SubEntry.h>
#include <OpSolverIf.h>
#include <wit/src/Variant.h>
#include <wit.h>
#include <CompMgr.h>
#include <MsgFac.h>
#include <MsgFrag.h>

//------------------------------------------------------------------------------

inline double & WitOptComp::sglObjValue () const
   {
   witAssert (not multiObjMode_);

   return * sglObjValuePtr_;
   }

//------------------------------------------------------------------------------

WitOptComp::WitOptComp (WitCompMgr * theCompMgr):
      WitComponent       (theCompMgr),

       heurOptStarter_   (NULL),
        accOptStarter_   (NULL),
      schedOptStarter_   (NULL),
      crashOptStarter_   (NULL),

      allOptStarters_    (),
      myCpxParSpecMgr_   (NULL),
      myMultiObjMgr_     (NULL),

      compPrices_        (defCompPrices        ()),
      accAfterOptImp_    (defAccAfterOptImp    ()),
      accAfterSoftLB_    (defAccAfterSoftLB    ()),
      mipMode_           (defMipMode           ()),
      multiObjMode_      (defMultiObjMode      ()),
      objChoice_         (defObjChoice         ()),
      wbounds_           (defWbounds           ()),
      optInitMethod_     (NULL),
      solverLogFileName_ (defSolverLogFileName ()),
      sglObjValuePtr_    (NULL),
      boundsValue_       (0.0),
      boundsValueValid_  (false),
      cplexStatusCode_   (-1),
      cplexStatusText_   ("The CPLEX solution status is not yet defined."),
      cplexMipBound_     (0.0),
      cplexMipRelGap_    (-1.0),
      printOptProblem_   (false),
      printMatByCols_    (false),
      printInitSoln_     (false),
      printOptSoln_      (false),
      printMps_          (false),
      skipScreening_     (false)
   {
   myCpxParSpecMgr_ = new WitCpxParSpecMgr (myProblem ());

   sglObjValuePtr_ = new double (0.0);

   setUpOptStarters ();

   myCompMgr ()->insert (this);
   }

//------------------------------------------------------------------------------

WitOptComp::~WitOptComp ()
   {
   delete myMultiObjMgr_;
   delete myCpxParSpecMgr_;

   while (not  allOptStarters_.isEmpty ())
      delete allOptStarters_.get ();

   delete sglObjValuePtr_;
   }

//------------------------------------------------------------------------------

WitOptStarter * WitOptComp::defOptInitMethod () const
   {
   return crashOptStarter ();
   }

//------------------------------------------------------------------------------
// "Set" functions.
//------------------------------------------------------------------------------

void WitOptComp::setCompPrices (bool theValue)
   {
   WitPart * thePart;

   prepSetUnpostAttr ();

   if (compPrices_)
      if (not theValue)
         forEachPart (thePart, myProblem ())
            thePart->resetShadowPrice ();

   compPrices_ = theValue;
   }

//------------------------------------------------------------------------------

void WitOptComp::setAccAfterOptImp (bool theValue)
   {
   if (not theValue)
      myDetOptImpMgr ()->shutDown ();

   prepSetUnpostAttr ();

   accAfterOptImp_ = theValue;

   checkAccAfter ();
   }

//------------------------------------------------------------------------------

void WitOptComp::setAccAfterSoftLB (bool theValue)
   {
   prepSetUnpostAttr ();

   if (theValue != accAfterSoftLB ())
      myDetOptImpMgr ()->shutDown ();

   accAfterSoftLB_ = theValue;

   checkAccAfter ();
   }

//------------------------------------------------------------------------------

void WitOptComp::setMipMode (bool theValue)
   {
   prepSetUnpreAttr ();

   mipMode_ = theValue;
   }

//------------------------------------------------------------------------------

void WitOptComp::setMultiObjMode (bool theValue)
   {
   witAssert (myCompMgr ()->nNodes () == 0);

   witAssert (not myStochImpMgr ()->stochMode ());

   prepSetUnpreAttr ();

   if (multiObjMode_ == theValue)
      return;

   if (theValue)
      {
      myMultiObjMgr_  = new WitMultiObjMgr (myProblem ());

      delete sglObjValuePtr_;

      sglObjValuePtr_ = NULL;
      }
   else
      {
      delete myMultiObjMgr_;

      myMultiObjMgr_  = NULL;

      sglObjValuePtr_ = new double (0.0);
      }

   multiObjMode_ = theValue;
   }

//------------------------------------------------------------------------------

void WitOptComp::setObjChoice (int theValue)
   {
   witAssert (theValue >= 0);
   witAssert (theValue <= 1);

   prepSetUnpreAttr ();

   objChoice_ = theValue;
   }

//------------------------------------------------------------------------------

void WitOptComp::setWbounds (double theValue)
   {
   witAssert (theValue >= 0.0);

   prepSetUnpostAttr ();

   wbounds_ = theValue;
   }

//------------------------------------------------------------------------------

void WitOptComp::setSolverLogFileName (const char * theValue)
   {
   witAssert (theValue != NULL);

   prepSetNonUnpostAttr ();

   solverLogFileName_ = theValue;
   }

//------------------------------------------------------------------------------

void WitOptComp::setPrintOptProblem (bool theValue)
   {
   printOptProblem_ = theValue;
   }

//------------------------------------------------------------------------------

void WitOptComp::setPrintMatByCols (bool theValue)
   {
   printMatByCols_ = theValue;
   }

//------------------------------------------------------------------------------

void WitOptComp::setPrintInitSoln (bool theValue)
   {
   printInitSoln_ = theValue;
   }

//------------------------------------------------------------------------------

void WitOptComp::setPrintOptSoln (bool theValue)
   {
   printOptSoln_ = theValue;
   }

//------------------------------------------------------------------------------

void WitOptComp::setPrintMps (bool theValue)
   {
   printMps_ = theValue;
   }

//------------------------------------------------------------------------------

void WitOptComp::setSkipScreening (bool theValue)
   {
   skipScreening_ = theValue;
   }

//------------------------------------------------------------------------------
// Other functions.
//------------------------------------------------------------------------------

void WitOptComp::storeOptObjValue (double theValue)
   {
   witAssert (myDetOptImpMgr ()->uploadingSoln ());

   sglObjValue () = theValue;
   }

//------------------------------------------------------------------------------

void WitOptComp::storeOptBoundsValue (double theValue)
   {
   witAssert (myDetOptImpMgr ()->uploadingSoln ());

   boundsValue_      = theValue;
   boundsValueValid_ = true;
   }

//------------------------------------------------------------------------------

void WitOptComp::storeStochObjValue (double theValue)
   {
   witAssert (myStochImpMgr ()->uploadingData ());

   sglObjValue () = theValue;
   }

//------------------------------------------------------------------------------

void WitOptComp::storeStochBoundsValue (double theValue)
   {
   witAssert (myStochImpMgr ()->uploadingData ());

   boundsValue_      = theValue;
   boundsValueValid_ = true;
   }

//------------------------------------------------------------------------------

void WitOptComp::storeCplexStatusCode (int theValue)
   {
   witAssert (WitOpSolverIf::cplexEmbedded ());

   cplexStatusCode_ = theValue;
   }

//------------------------------------------------------------------------------

void WitOptComp::storeCplexStatusText (const char * theValue)
   {
   witAssert (WitOpSolverIf::cplexEmbedded ());

   cplexStatusText_ = theValue;
   }

//------------------------------------------------------------------------------

void WitOptComp::storeCplexMipBound (double theValue)
   {
   witAssert (WitOpSolverIf::cplexEmbedded ());

   cplexMipBound_ = theValue;
   }

//------------------------------------------------------------------------------

void WitOptComp::storeCplexMipRelGap (double theValue)
   {
   witAssert (WitOpSolverIf::cplexEmbedded ());
   
   cplexMipRelGap_ = theValue;
   }

//------------------------------------------------------------------------------

bool WitOptComp::inserted ()
   {
   return (myCompMgr ()->myOptComp () == this);
   }

//------------------------------------------------------------------------------

void WitOptComp::copySelfInto (WitCompMgr * theCompMgr)
   {
   theCompMgr->myOptComp ()->copyAttrsFrom (this);
   }

//------------------------------------------------------------------------------

void WitOptComp::recount ()
   {
   myCompMgr ()->recountOptComp (this);
   }

//------------------------------------------------------------------------------

void WitOptComp::clearStochSoln ()
   {
   WitComponent::clearStochSoln ();

   boundsValueValid_ = false;
   boundsValue_      = 0.0;
   sglObjValue ()    = 0.0;
   }

//------------------------------------------------------------------------------

double WitOptComp::objValue () const
   {
   return
      multiObjMode_?
         myMultiObjMgr_->currentObjective ()->objValue ():
         sglObjValue ();
   }

//------------------------------------------------------------------------------

void WitOptComp::unpostprocess ()
   {
   boundsValueValid_ = false;
   }

//------------------------------------------------------------------------------

void WitOptComp::display ()
   {
   myMsgFac () ("optAttDdMsg",
      WitOpSolverIf::coinEmbedded  (),
      WitOpSolverIf::cplexEmbedded (),
      compPrices_,
      accAfterOptImp_,
      accAfterSoftLB_,
      mipMode_,
      multiObjMode_,
      wbounds_,
      solverLogFileName_);

   if (multiObjMode_)
      myMultiObjMgr ()->display ();
   }

//------------------------------------------------------------------------------

void WitOptComp::choose (WitOptStarter * theOptStarter)
   {
   witAssert (theOptStarter != NULL);

   witAssert (theOptStarter->myOptComp () == this);

   if (theOptStarter == accOptStarter_)
      stronglyAssert     (myDetOptImpMgr ()->accelerated ());
   else
      stronglyAssert (not myDetOptImpMgr ()->accelerated ());

   optInitMethod_ = theOptStarter;
   }

//------------------------------------------------------------------------------

void WitOptComp::store (WitOptStarter * theOptStarter)
   {
   allOptStarters_.append (theOptStarter);
   }

//------------------------------------------------------------------------------

bool WitOptComp::negativeCostsExist ()
   {
   WitPart *      thePart;
   WitMaterial *  theMat;
   WitOperation * theOpn;
   WitSubEntry *  theSub;

   forEachPart (thePart, myProblem ())
      if (not (thePart->scrapCost () >= 0.0))
         return true;

   forEachMaterial (theMat, myProblem ())
      if (not (theMat->stockCost () >= 0.0))
         return true;

   forEachOperation (theOpn, myProblem ())
      if (not (theOpn->execCost () >= 0.0))
         return true;

   forEachSubEntry (theSub, myProblem ())
      if (not (theSub->subCost () >= 0.0))
         return true;

   return false;
   }

//------------------------------------------------------------------------------

void WitOptComp::requireCoinOrCplex ()
   {
   if (not WitOpSolverIf::coinEmbedded ())
      if (not WitOpSolverIf::cplexEmbedded ())
         myMsgFac () ("coinOrCplexNeededSmsg");
   }

//------------------------------------------------------------------------------

void WitOptComp::writeObjValue ()
   {
   if (multiObjMode_)
      {
      myMultiObjMgr_->writeObjValues ();
      }
   else
      myMsgFac () ("sglObjValueMsg", objValue ());
   }

//------------------------------------------------------------------------------

void WitOptComp::setUpOptStarters ()
   {
   heurOptStarter_ =
      new WitOptStarter (
        this,
        "heur",
        WitHEUR_OPT_INIT_METHOD,
        "heurOptStarterMsg",
        true);

   accOptStarter_ =
      new WitOptStarter (
        this,
        "acc",
        WitACC_OPT_INIT_METHOD,
        "accOptStarterMsg",
        false);

   schedOptStarter_ =
      new WitOptStarter (
        this,
        "sched",
        WitSCHED_OPT_INIT_METHOD,
        "schedOptStarterMsg",
        true);

   crashOptStarter_ =
      new WitOptStarter (
        this,
        "crash",
        WitCRASH_OPT_INIT_METHOD,
        "crashOptStarterMsg",
        false);

   choose (defOptInitMethod ());
   }

//------------------------------------------------------------------------------

void WitOptComp::checkAccAfter ()
   {
   if (accAfterSoftLB ())
      if (not accAfterOptImp ())
         myMsgFac () ("accAfterSoftLBNotOptImpMsg");
   }

//------------------------------------------------------------------------------

void WitOptComp::writeDataID ()
   {
   myDataWriter ()->writeFirstKeyWord ("set");
   myDataWriter ()->writeKeyWord      ("problem");
   }

//------------------------------------------------------------------------------

void WitOptComp::writeDataAttrs ()
   {
   WitComponent::writeDataAttrs ();

   myDataWriter ()->writeBool (
        "compPrices",
         compPrices (),
      defCompPrices ());

   myDataWriter ()->writeBool (
        "accAfterOptImp",
         accAfterOptImp (),
      defAccAfterOptImp ());

   myDataWriter ()->writeBool (
        "accAfterSoftLB",
         accAfterSoftLB (),
      defAccAfterSoftLB ());

   myDataWriter ()->writeBool (
        "mipMode",
         mipMode (),
      defMipMode ());

   myDataWriter ()->writeInt   (
        "objChoice",
         objChoice (),
      defObjChoice ());

   myDataWriter ()->writeDouble (
        "wbounds",
         wbounds (),
      defWbounds ());

   myDataWriter ()->writeString (
        "solverLogFileName",
         solverLogFileName (),
      defSolverLogFileName ());

   myDataWriter ()->writeBool (
        "multiObjMode",
         multiObjMode (),
      defMultiObjMode ());

   if (multiObjMode_)
      myMultiObjMgr_->writeDataAttrs ();
   }

//------------------------------------------------------------------------------

void WitOptComp::copyAttrsFrom (WitOptComp * theOptComp)
   {
   WitComponent::copyAttrsFrom (theOptComp);

   compPrices_        = theOptComp->compPrices_;
   mipMode_           = theOptComp->mipMode_;
   wbounds_           = theOptComp->wbounds_;
   solverLogFileName_ = theOptComp->solverLogFileName_;
   objChoice_         = theOptComp->objChoice_;

   setAccAfterOptImp   (theOptComp->accAfterOptImp_);
   setAccAfterSoftLB   (theOptComp->accAfterSoftLB_);
   setMultiObjMode     (theOptComp->multiObjMode_);

   myCpxParSpecMgr_->  copyAttrsFrom (theOptComp->myCpxParSpecMgr_);

   if (multiObjMode_)
      myMultiObjMgr_->copyAttrsFrom (theOptComp->myMultiObjMgr_);
   }
