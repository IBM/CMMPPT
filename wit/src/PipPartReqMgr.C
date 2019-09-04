//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "PipPartReqMgr.C"
//
// Contains the implementation of class PipPartReqMgr.
//------------------------------------------------------------------------------

#include <PipPartReqMgr.h>
#include <PipReqMgr.h>
#include <PipReqBasis.h>
#include <GlobalComp.h>
#include <Material.h>
#include <Post.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class PipPartReqMgr.
//------------------------------------------------------------------------------

WitPipPartReqMgr::WitPipPartReqMgr (WitPipReqMgr * theReqMgr):

      WitProbAssoc     (theReqMgr),
      myReqMgr_        (theReqMgr),
      curPartHasStock_ (false),
      netStockVol_     (myProblem (), 0.0),
      curPartReqSeq_   (myProblem ()),
      workReqSeq_      (myProblem ()),
      overStockReqSeq_ (myProblem ()),
      maxInfeas_       (0.0),
      maxInfeasPart_   (NULL),
      maxInfeasPer_    (-1)
   {
   WitPeriod thePer;

   forEachPeriod (thePer, myProblem ())
      {
      curPartReqSeq_  [thePer] = new WitPipReqSeq (myProblem ());
      overStockReqSeq_[thePer] = new WitPipReqSeq (myProblem ());
      }
   }

//------------------------------------------------------------------------------

WitPipPartReqMgr::~WitPipPartReqMgr ()
   {
   WitPeriod thePer;

   forEachPeriod (thePer, myProblem ())
      {
      delete overStockReqSeq_[thePer];
      delete curPartReqSeq_  [thePer];
      }
   }

//------------------------------------------------------------------------------

void WitPipPartReqMgr::setUpPartReqs (WitPart * thePart)
   {
   WitMaterial * theMat;

   theMat           = thePart->thisMat ();

   curPartHasStock_ = false;

   if (theMat != NULL)
      if (theMat->stockVol () != 0.0)
         curPartHasStock_ = true;

   if (curPartHasStock_)
      compNonExcStockVol (theMat);
   else
      netStockVol_ = 0.0;

   loadCurPartReqSeqs (thePart);
   }

//------------------------------------------------------------------------------

void WitPipPartReqMgr::mergeCurPartReqsIntoWorkSeq (WitPeriod thePer)
   {
   WitPipReqSeq     curPartSeq (myProblem ());
   WitPipReqSeq     mergedSeq  (myProblem ());
   WitPipReqBasis * workBasis;
   WitPeriod        workPer;
   double           workReqVol;
   WitPipReqBasis * curPartBasis;
   WitPeriod        curPartPer;
   double           curPartReqVol;

   if (not curPartHasStock_)
      {
      workReqSeq_.takeContentsFrom (curPartReqSeq_ (thePer));

      return;
      }

   curPartSeq.takeContentsFrom (curPartReqSeq_ (thePer));

   popOrNull (workReqSeq_, workBasis,    workPer,    workReqVol);
   popOrNull (curPartSeq,  curPartBasis, curPartPer, curPartReqVol);

   while ((workBasis != NULL) and (curPartBasis != NULL))
      if (workBasis->belongsBefore (curPartBasis))
         {
         mergedSeq.push         (workBasis, workPer, workReqVol);
         popOrNull (workReqSeq_, workBasis, workPer, workReqVol);
         }
      else
         {
         mergedSeq.push        (curPartBasis, thePer,     curPartReqVol);
         popOrNull (curPartSeq, curPartBasis, curPartPer, curPartReqVol);
         }

   while (workBasis != NULL)
      {
      mergedSeq.push         (workBasis, workPer, workReqVol);
      popOrNull (workReqSeq_, workBasis, workPer, workReqVol);
      }

   while (curPartBasis != NULL)
      {
      mergedSeq.push        (curPartBasis, thePer,     curPartReqVol);
      popOrNull (curPartSeq, curPartBasis, curPartPer, curPartReqVol);
      }

   mergedSeq.reverse ();

   workReqSeq_.takeContentsFrom (mergedSeq);
   }

//------------------------------------------------------------------------------

bool WitPipPartReqMgr::workSeqIsEmpty ()
   {
   return workReqSeq_.isEmpty ();
   }

//------------------------------------------------------------------------------

void WitPipPartReqMgr::getNextReq (
      double &           netAttVol,
      WitPipReqBasis * & theBasis,
      double &           incPegVol)
   {
   WitPeriod fillPer;
   double    incReqVol;

   witAssert (not workSeqIsEmpty ());
   witAssert (netAttVol > NET_TOL);

   workReqSeq_.pop (theBasis, fillPer, incReqVol);

   incPegVol  = min (incReqVol, netAttVol);
      
   incReqVol -= incPegVol;
   netAttVol -= incPegVol;

   if (incReqVol > NET_TOL)
      workReqSeq_.push (theBasis, fillPer, incReqVol);

   if (myGlobalComp ()->printDebug ())
      prtPeg (incPegVol, fillPer, theBasis);
   }

//------------------------------------------------------------------------------

void WitPipPartReqMgr::rollReqsBack (WitPeriod thePer)
   {
   WitPipReqBasis * theBasis;
   WitPeriod        fillPer;
   double           incReqVol;
   double           rollVol;
   WitPipReqSeq     rolledSeq (myProblem ());

   witAssert (thePer > 0);

   while (not workSeqIsEmpty () and (netStockVol_[thePer - 1] > NET_TOL))
      {
      workReqSeq_.pop (theBasis, fillPer, incReqVol);

      rollVol = min (incReqVol, netStockVol_[thePer - 1]);

      rolledSeq.push (theBasis, fillPer, rollVol);

      netStockVol_[thePer - 1] -= rollVol;
      incReqVol                -= rollVol;

      if (incReqVol > NET_TOL)
         workReqSeq_.push (theBasis, fillPer, incReqVol);
      }

   overStockReqSeq_ (thePer).takeContentsFrom (workReqSeq_);

   rolledSeq.reverse ();

   workReqSeq_.takeContentsFrom (rolledSeq);
   }

//------------------------------------------------------------------------------

void WitPipPartReqMgr::rollReqsFwd (WitPeriod thePer)
   {
   WitPipReqBasis * theBasis;
   WitPeriod        fillPer;
   double           incReqVol;
   WitPipReqSeq     rolledSeq (myProblem ());
   WitPipReqBasis * theBasisOv;
   WitPeriod        fillPerOv;
   double           incReqVolOv;

   if (not curPartHasStock_)
      {
      workReqSeq_.takeContentsFrom (overStockReqSeq_ (thePer));

      return;
      }

   while (not workSeqIsEmpty ())
      {
      workReqSeq_.pop  (theBasis, fillPer, incReqVol);
      rolledSeq  .push (theBasis, fillPer, incReqVol);
     
      netStockVol_[thePer - 1] += incReqVol;
      }

   if (rolledSeq.isNonEmpty ())
      if (overStockReqSeq_ (thePer).isNonEmpty ())
         {
         rolledSeq                .pop (theBasis,   fillPer,   incReqVol);
         overStockReqSeq_ (thePer).pop (theBasisOv, fillPerOv, incReqVolOv);

         if ((theBasis == theBasisOv) and (fillPer == fillPerOv))
            {
            rolledSeq.push (theBasis,   fillPer,   incReqVol + incReqVolOv);
            }
         else
            {
            rolledSeq.push (theBasis,   fillPer,   incReqVol);
            rolledSeq.push (theBasisOv, fillPerOv, incReqVolOv);
            }
         }

   while (overStockReqSeq_ (thePer).isNonEmpty ())
      {
      overStockReqSeq_ (thePer).pop  (theBasis, fillPer, incReqVol);
      rolledSeq                .push (theBasis, fillPer, incReqVol);
      }

   rolledSeq.reverse ();

   workReqSeq_.takeContentsFrom (rolledSeq);
   }

//------------------------------------------------------------------------------

void WitPipPartReqMgr::restoreUnfilledCurPartReqs (WitPeriod thePer)
   {
   WitPipReqBasis * theBasis;
   WitPeriod        fillPer;
   double           incReqVol;
   WitPipReqSeq     reducedSeq (myProblem ());

   witAssert (curPartReqSeq_ (thePer).isEmpty ());

   if (not curPartHasStock_)
      {
      curPartReqSeq_ (thePer).takeContentsFrom (workReqSeq_);

      return;
      }

   while (not workSeqIsEmpty ())
      {
      workReqSeq_               .pop  (theBasis, fillPer, incReqVol);

      if (fillPer == thePer)
         curPartReqSeq_ (thePer).push (theBasis, fillPer, incReqVol);
      else
         reducedSeq             .push (theBasis, fillPer, incReqVol);
      }

   curPartReqSeq_ (thePer).reverse ();
   reducedSeq             .reverse ();

   workReqSeq_.takeContentsFrom (reducedSeq);
   }

//------------------------------------------------------------------------------

void WitPipPartReqMgr::prtWorkReqSeq (
      const char * theAttname,
      WitPart *    thePart,
      WitPeriod    thePer)
   {
   WitPipReqSeq     theReqSeq (myProblem ());
   WitPipReqBasis * theBasis;
   WitPeriod        fillPer;
   double           incReqVol;

   witAssert (myGlobalComp ()->printDebug ());
   
   theReqSeq.revCopyFrom (workReqSeq_);

   theReqSeq.reverse ();

   fprintf (msgFile (),
      "\n"
      "Pegging to %s:\n"
      "   Part:       %s\n"
      "   Peg period: %d\n"
      "\n"
      "   Requirement sequence:\n",
      theAttname,
      thePart->partName ().myCstring (),
      thePer);

   while (theReqSeq.pop (theBasis, fillPer, incReqVol))
      {
      fprintf (msgFile (),
         "\n"
         "      Fill Period: %d\n"
         "      ReqVol:      %.0f\n",
         fillPer,
         incReqVol);

      theBasis->print (msgFile (), "      ");
      }
   }

//------------------------------------------------------------------------------

void WitPipPartReqMgr::prtPeg (
      double           pegVol,
      WitPeriod        fillPer,
      WitPipReqBasis * theBasis)
   {
   witAssert (myGlobalComp ()->printDebug ());

   fprintf (msgFile (),
      "\n"
      "   Pegging:\n"
      "      PegVol:      %.0f\n"
      "      Fill Period: %d\n",
      pegVol,
      fillPer);

   theBasis->print (msgFile (), "      ");
   }

//------------------------------------------------------------------------------

void WitPipPartReqMgr::recordInfeas (WitPart * thePart)
   {
   WitPeriod        thePer;
   double           totReqVol;
   WitPipReqBasis * theBasis;
   WitPeriod        fillPer;
   double           incReqVol;

   forEachPeriod (thePer, myProblem ())
      {
      totReqVol = 0.0;

      while (curPartReqSeq_ (thePer).isNonEmpty ())
         {
         curPartReqSeq_ (thePer).pop (theBasis, fillPer, incReqVol);

         totReqVol += incReqVol;
         }

      if (totReqVol > maxInfeas_)
         {
         maxInfeas_     = totReqVol;
         maxInfeasPart_ = thePart;
         maxInfeasPer_  = thePer;
         }
      }
   }

//------------------------------------------------------------------------------

void WitPipPartReqMgr::prtInfeas ()
   {
   myMsgFac () ("pipMaxInfeasMsg", maxInfeas_);

   if (maxInfeas_ > 0.0)
      myMsgFac () ("pipMaxInfDataMsg",
         maxInfeasPart_->partName (),
         maxInfeasPer_);
   }

//------------------------------------------------------------------------------

void WitPipPartReqMgr::compNonExcStockVol (WitMaterial * theMat)
   {
   double    excStockVol;
   WitPeriod thePer;

   excStockVol = 0.0;

   forEachPeriod (thePer, myProblem ())
      {
      excStockVol +=
         myProblem ()->myPostprocessor ()->compExcessVol (theMat, thePer);

      excStockVol -= theMat->scrapVol ()[thePer];

      setToMax (excStockVol, 0.0);

      setToMin (excStockVol, theMat->stockVol ()[thePer]);

      netStockVol_[thePer] = theMat->stockVol ()[thePer] - excStockVol;
      }
   }

//------------------------------------------------------------------------------

void WitPipPartReqMgr::loadCurPartReqSeqs (WitPart * thePart)
   {
   WitPipReqList    theReqList (myProblem ());
   WitPeriod        fillPer;
   WitPipReqBasis * theBasis;
   double           incReqVol;

   forEachPeriod (fillPer, myProblem ())
      {
      myReqMgr_->loadCurReqList (thePart, fillPer);

      myReqMgr_->getCurReqList  (theReqList);

      while (theReqList.pop (theBasis, incReqVol))
         curPartReqSeq_ (fillPer).push (theBasis, fillPer, incReqVol);

      curPartReqSeq_ (fillPer).reverse ();
      }
   }

//------------------------------------------------------------------------------

void WitPipPartReqMgr::popOrNull (
      WitPipReqSeq &     theReqSeq,
      WitPipReqBasis * & theBasis,
      WitPeriod &        fillPer,
      double &           incReqVol)
   {
   if (theReqSeq.isNonEmpty ())
      theReqSeq.pop (theBasis, fillPer, incReqVol);
   else
      {
      theBasis  = NULL;
      fillPer   = -1;
      incReqVol = 0.0;
      }
   }
