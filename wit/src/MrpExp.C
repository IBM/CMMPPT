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
// Source file: "MrpExp.C"
//
// Contains the implementation of the following classes:
//
// MrpExp
// OrigMrpExp.
//------------------------------------------------------------------------------

#include <OrigMrp.h>
#include <Pre.h>
#include <GlobalComp.h>
#include <Part.h>
#include <Demand.h>
#include <Operation.h>
#include <BomEntry.h>
#include <SubEntry.h>
#include <BopEntry.h>
#include <Timing.h>
#include <MsgFac.h>
#include <ObjStack.h>

//------------------------------------------------------------------------------
// Implementation of class MrpExp.
//------------------------------------------------------------------------------

WitMrpExp::WitMrpExp (WitProblem * theProblem):
      WitProbAssoc (theProblem),

      netSubsMode_ (false)
   {
   }

//------------------------------------------------------------------------------

WitMrpExp::~WitMrpExp ()
   {
   }

//------------------------------------------------------------------------------

void WitMrpExp::mrp ()
   {
   WitTimer::enterSection ("mrp");

   myPreprocessor ()->preprocess ();

   init ();

   explodeAllParts ();

   finish ();

   WitTimer::leaveSection ("mrp");
   }

//------------------------------------------------------------------------------

void WitMrpExp::initDerived ()
   {
   }

//------------------------------------------------------------------------------

void WitMrpExp::init ()
   {
   WitPart *     thePart;
   WitPeriod     thePer;
   WitSubEntry * theSub;

   initDerived ();

   netSupplyVol_.allocate (myProblem (), 0.0);
   myReqVol_    .allocate (myProblem (), 0.0);

   forEachPart (thePart, myProblem ())
      {
      myReqVol_ (thePart) = 0.0;

      forEachPeriod (thePer, myProblem ())
         netSupplyVol_ (thePart)[thePer] = supplyVol (thePart, thePer);
      }

   netSubsMode_ = false;

   hasSubsToNetVec_.resize (myCompMgr ()->nBomEntries (), false);

   forEachSubEntry (theSub, myProblem ())
      if (theSub->mrpNetAllowed ())
         {
         netSubsMode_                       = true;
         hasSubsToNet (theSub->myBomEnt ()) = true;
         }
   }

//------------------------------------------------------------------------------

void WitMrpExp::explodeAllParts ()
   {
   WitPart * thePart;
   WitPeriod thePer;
   bool      success;

   WitObjStack <WitPart> reversedParts (myProblem ());

   revCopyInto (reversedParts, myCompMgr ()->allParts ());

   while (reversedParts.pop (thePart))
      forEachPeriod (thePer, myProblem ())
         {
         myReqVol_ (thePart)[thePer] += demandVol (thePart, thePer);

         myReqVol_ (thePart)[thePer] =
            thePart->net (
               thePer, 
               myReqVol_     (thePart)[thePer], 
               netSupplyVol_ (thePart));

         if (myReqVol_ (thePart)[thePer] > FLOAT_EPSILON)
            {
            success = explode (thePart, thePer);

            if (not success)
               if (myGlobalComp ()->roundReqVols ())
                  myReqVol_ (thePart)[thePer] = 
                     ceil (myReqVol_ (thePart)[thePer]);
            }
         else
            myReqVol_ (thePart)[thePer] = 0.0;
         }
   }

//------------------------------------------------------------------------------

bool WitMrpExp::explode (WitPart * thePart, WitPeriod fillPer)
   {
   WitPeriod      expPer;
   WitBopEntry *  theExpBopEntry;
   WitOperation * theOpn;
   WitPeriod      execPer;
   double         qty;
   WitBomEntry *  theBomEnt;
   WitBopEntry *  theBopEnt;

   expPer = thePart->explosionPeriod (fillPer);

   if (not thePart->explodeable (expPer))
      return false;

   theExpBopEntry = thePart       ->expBopEntry   (expPer);
   theOpn         = theExpBopEntry->myOperation   ();
   execPer        = theExpBopEntry->expExecPeriod ()[expPer];

   qty = myReqVol_ (thePart)[fillPer] / theExpBopEntry->effProdRate ()[execPer];

   qty = theOpn->lotSizeDelta (execPer, execVol (theOpn, execPer), qty);

   if (qty <= FLOAT_EPSILON)
       return false;

   execVol (theOpn, execPer) += qty;

   forEachEl (theBomEnt, theOpn->bom ())
      explodeBomEntry (theBomEnt, execPer, qty);

   forEachEl (theBopEnt, theOpn->bop ())
      explodeBopEntry (theBopEnt, execPer, qty);

   thePart->net (
      fillPer,
      myReqVol_     (thePart)[fillPer],
      netSupplyVol_ (thePart));

   myReqVol_ (thePart)[fillPer]  = 0.0;

   return true;
   }

//------------------------------------------------------------------------------

void WitMrpExp::explodeBomEntry (
      WitBomEntry * theBomEnt,
      WitPeriod     execPer,
      double        qty)
   {
   double    reqQty;
   WitPart * thePart;
   WitPeriod consPer;


   if (not theBomEnt->inEffect (execPer))
      return;

   if (myCompMgr ()->hasSubEntries ())
      if (theBomEnt->hasSubsInEffect ()[execPer])
         if (hasSubsToNet (theBomEnt))
            netConsEntries (theBomEnt, execPer, qty);

   reqQty = qty * theBomEnt->effConsRate ()[execPer];

   if (fabs (reqQty) <= FLOAT_EPSILON)
      return;

   thePart = theBomEnt->myPart ();
   consPer = theBomEnt->impactPeriod ()[execPer];

   if (reqQty > 0.0)
      myReqVol_     (thePart)[consPer] += reqQty;
   else
      netSupplyVol_ (thePart)[consPer] -= reqQty;
   }

//------------------------------------------------------------------------------

void WitMrpExp::netConsEntries (
      WitBomEntry * theBomEnt,
      WitPeriod     execPer,
      double &      qty)
   {
   WitSubEntry * theSub;
   double        oldQty;

   netConsEntry (theBomEnt, execPer, qty);

   if (qty < NET_TOL)
      return;

   forEachEl (theSub, theBomEnt->mySubEntries ())
      if (theSub->mrpNetAllowed ())
         if (theSub->inEffect (execPer))
            {
            oldQty = qty;

            netConsEntry (theSub, execPer, qty);

            subVol (theSub, execPer) += positivePart (oldQty - qty);

            if (qty < NET_TOL)
               return;
            }
   }

//------------------------------------------------------------------------------

void WitMrpExp::netConsEntry (
      WitConsEntry * theConsEnt,
      WitPeriod      execPer,
      double &       qty)
   {
   double    reqConsQty;
   WitPart * thePart;
   WitPeriod consPer;
   double    netConsQty;


   reqConsQty = theConsEnt->effConsRate ()[execPer] * qty;
   thePart    = theConsEnt->myPart ();
   consPer    = theConsEnt->impactPeriod ()[execPer];

   if (theConsEnt->effConsRate ()[execPer] >= FLOAT_EPSILON)
      {
      netConsQty = thePart->net (consPer, reqConsQty, netSupplyVol_ (thePart));

      qty = netConsQty / theConsEnt->effConsRate ()[execPer];

      if (qty <= NET_TOL)
         qty = 0.0;
      }
   else
      {
      if (theConsEnt->effConsRate ()[execPer] <= - FLOAT_EPSILON)
         netSupplyVol_ (thePart)[consPer] -= reqConsQty;

      qty = 0.0;
      }
   }

//------------------------------------------------------------------------------

void WitMrpExp::explodeBopEntry (
      WitBopEntry * theBopEnt,
      WitPeriod     execPer,
      double        qty)
   {
   WitPeriod prodPer;
   double    prodQty;

   if (not theBopEnt->inEffect (execPer))
      return;

   prodPer =       theBopEnt->impactPeriod ()[execPer];
   prodQty = qty * theBopEnt->effProdRate  ()[execPer];

   if (prodQty > FLOAT_EPSILON)
      netSupplyVol_ (theBopEnt->myPart ())[prodPer] += prodQty;
   }

//------------------------------------------------------------------------------

bool & WitMrpExp::hasSubsToNet (WitBomEntry * theBomEnt)
   {
   return hasSubsToNetVec_[theBomEnt->mappingIndex ()];
   }

//------------------------------------------------------------------------------

void WitMrpExp::finish ()
   {
   finishDerived ();

   hasSubsToNetVec_.clear ();
   myReqVol_       .clear ();
   netSupplyVol_   .clear ();
   }

//------------------------------------------------------------------------------
// Implementation of class OrigMrpExp.
//------------------------------------------------------------------------------

WitOrigMrpExp::WitOrigMrpExp (WitProblem * theProblem):
      WitMrpExp   (theProblem),

      exploding_     (false),
      uploadingSoln_ (false),
      mrpExecVol_    (),
      mrpSubVol_     ()
   {                  
   }

//------------------------------------------------------------------------------

WitOrigMrpExp::~WitOrigMrpExp ()
   {
   }

//------------------------------------------------------------------------------

void WitOrigMrpExp::initDerived ()
   {
   myMsgFac () ("mrpMsg");

   mrpExecVol_.allocate (myProblem (), 0.0);
   mrpSubVol_ .allocate (myProblem (), 0.0);

   exploding_ = true;
   }

//------------------------------------------------------------------------------

double WitOrigMrpExp::supplyVol (WitPart * thePart, WitPeriod thePer)
   {
   return thePart->supplyVol ()[thePer];
   }

//------------------------------------------------------------------------------

double WitOrigMrpExp::demandVol (WitPart * thePart, WitPeriod thePer)
   {
   WitDemand * theDemand;
   double      theDemandVol;

   theDemandVol = 0.0;

   forEachEl (theDemand, thePart->myDemands ())
      theDemandVol += theDemand->demandVol ()[thePer];

   return theDemandVol;
   }

//------------------------------------------------------------------------------

double & WitOrigMrpExp::execVol (WitOperation * theOpn, WitPeriod thePer)
   {
   return mrpExecVol_ (theOpn)[thePer];
   }

//------------------------------------------------------------------------------

double & WitOrigMrpExp::subVol (WitSubEntry * theSub, WitPeriod thePer)
   {
   return mrpSubVol_ (theSub)[thePer];
   }

//------------------------------------------------------------------------------

void WitOrigMrpExp::finishDerived ()
   {
   exploding_ = false;

   uploadSoln ();

   mrpExecVol_.clear ();
   mrpSubVol_ .clear ();
   }

//------------------------------------------------------------------------------

void WitOrigMrpExp::uploadSoln ()
   {
   WitPart *      thePart;
   WitOperation * theOpn;
   WitSubEntry *  theSub;

   uploadingSoln_ = true;

   forEachPart (thePart, myProblem ())
      {
      thePart->storeMrpReqVol         (myReqVol     () (thePart));
      thePart->storeMrpMrpResidualVol (netSupplyVol () (thePart));
      }

   forEachOperation (theOpn, myProblem ())
      theOpn->storeMrpMrpExecVol (mrpExecVol_ (theOpn));

   forEachSubEntry (theSub, myProblem ())
      theSub->storeMrpMrpSubVol (mrpSubVol_ (theSub));

   compMrpConsVols ();

   uploadingSoln_ = false;
   }

//------------------------------------------------------------------------------

void WitOrigMrpExp::compMrpConsVols ()
   {
   WitPart *        thePart;
   WitBomEntry *    theBomEnt;
   WitSubEntry *    theSub;
   WitPeriod        thePer;
   WitTVec <double> mrpNonSubVolVec (myProblem (), 0.0);
   WitTVec <double> mrpSubVolVec    (myProblem (), 0.0);
   WitTVec <double> mrpConsVolVec   (myProblem (), 0.0);
   double           totMrpSubVol;

   forEachPart (thePart, myProblem ())
      {
      mrpConsVolVec = 0.0;

      forEachEl (theBomEnt, thePart->consumingBomEntries ())
         {
         if (netSubsMode () and not theBomEnt->mySubEntries ().isEmpty ())
            forEachPeriod (thePer, myProblem ())
               {
               totMrpSubVol = 0.0;

               forEachEl (theSub, theBomEnt->mySubEntries ())
                  if (theSub->mrpNetAllowed ())
                     if (theSub->inEffect (thePer))
                        totMrpSubVol += theSub->mrpSubVol ()[thePer];

               mrpNonSubVolVec[thePer] =
                    mrpExecVol_ (theBomEnt->myOperation ())[thePer]
                  - totMrpSubVol;
               }
         else
            mrpNonSubVolVec = mrpExecVol_ (theBomEnt->myOperation ());

         theBomEnt->incConsVol (mrpConsVolVec, mrpNonSubVolVec);
         }

      if (netSubsMode ())
         forEachEl (theSub, thePart->consumingSubEntries ())
            if (theSub->mrpNetAllowed ())
               {
               mrpSubVolVec = theSub->mrpSubVol ();

               theSub->incConsVol (mrpConsVolVec, mrpSubVolVec);
               }

      thePart->storeMrpMrpConsVol (mrpConsVolVec);
      }
   }
