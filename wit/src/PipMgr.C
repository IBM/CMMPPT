//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "PipMgr.C"
//
// Contains the implementation of the following classes:
//
//    PipMgr
//    PipSeqMgr
//------------------------------------------------------------------------------

#include <PipMgr.h>
#include <PipSeqMgr.h>
#include <PipPgg.h>
#include <PipAttPgg.h>
#include <PipRcpPgg.h>
#include <Post.h>
#include <Demand.h>
#include <Part.h>
#include <Operation.h>
#include <SubEntry.h>
#include <GlobalComp.h>
#include <DataWrit.h>
#include <Schedule.h>
#include <Timing.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class PipMgr.
//------------------------------------------------------------------------------

WitPipMgr::WitPipMgr (WitProblem * theProblem):

      WitProbAssoc (theProblem),

      mySeqMgr_    (NULL),
      myPgg_       (NULL)
   {
   WitTimer::enterSection ("pip");

   mySeqMgr_ = new WitPipSeqMgr (myProblem ());

   WitTimer::leaveSection ("pip");
   }

//------------------------------------------------------------------------------

WitPipMgr::~WitPipMgr ()
   {
   WitTimer::enterSection ("pip");

   delete myPgg_;
   delete mySeqMgr_;

   WitTimer::leaveSection ("pip");
   }

//------------------------------------------------------------------------------

void WitPipMgr::clearShipSeq ()
   {
   WitTimer::enterSection ("pip");

   clearPegging ();

   mySeqMgr_->clear ();

   WitTimer::leaveSection ("pip");
   }

//------------------------------------------------------------------------------

void WitPipMgr::appendToShipSeq (
      WitDemand * theDemand,
      WitPeriod   shipPer,
      double      incShipVol)
   {
   WitTimer::enterSection ("pip");

   clearPegging ();

   mySeqMgr_->append (theDemand, shipPer, incShipVol);

   WitTimer::leaveSection ("pip");
   }

//------------------------------------------------------------------------------

void WitPipMgr::getShipSeq (WitDemTripStack & theShipSeq)
   {
   WitTimer::enterSection ("pip");

   mySeqMgr_->get (theShipSeq);

   WitTimer::leaveSection ("pip");
   }

//------------------------------------------------------------------------------

void WitPipMgr::displayShipSeq ()
   {
   mySeqMgr_->display ();
   }

//------------------------------------------------------------------------------

void WitPipMgr::copyShipSeq (WitPipMgr * thePipMgr)
   {
   WitTimer::enterSection ("pip");

   mySeqMgr_->copy (thePipMgr->mySeqMgr_);

   WitTimer::leaveSection ("pip");
   }

//------------------------------------------------------------------------------
 
void WitPipMgr::writeShipSeqData ()
   {
   mySeqMgr_->writeData ();
   }

//------------------------------------------------------------------------------

void WitPipMgr::setShipSeqShipPer (WitPeriod shipPer)
   {
   mySeqMgr_->setShipPerForRead (shipPer);
   }

//------------------------------------------------------------------------------

WitPeriod WitPipMgr::shipSeqShipPer ()
   {
   return mySeqMgr_->shipPerForRead ();
   }

//------------------------------------------------------------------------------

void WitPipMgr::buildPegging ()
   {
   WitTimer::enterSection ("pip");

   myMsgFac () ("pipMsg");

   if (not myGlobalComp ()->newPipPggOrder ())
      myMsgFac () ("oldPipPggOrderMsg");

   checkRestrictions ();

   mySeqMgr_->reduce ();

   clearPegging ();

   myPgg_ = new WitPipPgg (myProblem ());

   WitTimer::leaveSection ("pip");
   }

//------------------------------------------------------------------------------

void WitPipMgr::clearPegging ()
   {
   if (not peggingExists ())
      return;

   WitTimer::enterSection ("pip");

   delete myPgg ();
   
   myPgg_ = NULL;

   WitTimer::leaveSection ("pip");
   }

//------------------------------------------------------------------------------

void WitPipMgr::printPegging ()
   {
   if (peggingExists ())
      myPgg ()->print ();
   }

//------------------------------------------------------------------------------

void WitPipMgr::getExecVolPgg (
      WitDemand *       theDemand,
      WitPeriod         shipPer,
      WitOpnTripStack & execVolTrips)
   {
   myPgg ()->execVolPgg ()->getPgg (theDemand, shipPer, execVolTrips);
   }

//------------------------------------------------------------------------------

void WitPipMgr::getCoExecVolPgg (
      WitDemand *          theDemand,
      WitPeriod            shipPer,
      WitBopEntTripStack & coExecVolTrips)
   {
   myPgg ()->coExecVolPgg ()->getPgg (theDemand, shipPer, coExecVolTrips);
   }

//------------------------------------------------------------------------------

void WitPipMgr::getSubVolPgg (
      WitDemand *       theDemand,
      WitPeriod         shipPer,
      WitSubTripStack & subVolTrips)
   {
   myPgg ()->subVolPgg ()->getPgg (theDemand, shipPer, subVolTrips);
   }

//------------------------------------------------------------------------------

void WitPipMgr::getSupplyVolPgg (
      WitDemand *        theDemand,
      WitPeriod          shipPer,
      WitPartTripStack & supplyVolTrips)
   {
   myPgg ()->supplyVolPgg ()->getPgg (theDemand, shipPer, supplyVolTrips);
   }

//------------------------------------------------------------------------------

void WitPipMgr::getProdVolPgg (
      WitDemand *        theDemand,
      WitPeriod          shipPer,
      WitPartTripStack & prodVolTrips)
   {
   myPgg ()->prodVolPgg ()->getPgg (theDemand, shipPer, prodVolTrips);
   }

//------------------------------------------------------------------------------

void WitPipMgr::getConsVolPgg (
      WitDemand *        theDemand,
      WitPeriod          shipPer,
      WitPartTripStack & consVolTrips)
   {
   myPgg ()->consVolPgg ()->getPgg (theDemand, shipPer, consVolTrips);
   }

//------------------------------------------------------------------------------

void WitPipMgr::getSideVolPgg (
      WitDemand *        theDemand,
      WitPeriod          shipPer,
      WitPartTripStack & sideVolTrips)
   {
   myPgg ()->sideVolPgg ()->getPgg (theDemand, shipPer, sideVolTrips);
   }

//------------------------------------------------------------------------------

void WitPipMgr::getExecVolPgg (
      WitOperation *    theOpn,
      WitPeriod         execPer,
      WitOpnTripStack & execVolTrips)
   {
   myPgg ()->execVolPgg ()->getPgg (theOpn, execPer, execVolTrips);
   }

//------------------------------------------------------------------------------

void WitPipMgr::getCoExecVolPgg (
      WitOperation *       theOpn,
      WitPeriod            execPer,
      WitBopEntTripStack & coExecVolTrips)
   {
   myPgg ()->coExecVolPgg ()->getPgg (theOpn, execPer, coExecVolTrips);
   }

//------------------------------------------------------------------------------

void WitPipMgr::getSubVolPgg (
      WitOperation *    theOpn,
      WitPeriod         execPer,
      WitSubTripStack & subVolTrips)
   {
   myPgg ()->subVolPgg ()->getPgg (theOpn, execPer, subVolTrips);
   }

//------------------------------------------------------------------------------

void WitPipMgr::getSupplyVolPgg (
      WitOperation *     theOpn,
      WitPeriod          execPer,
      WitPartTripStack & supplyVolTrips)
   {
   myPgg ()->supplyVolPgg ()->getPgg (theOpn, execPer, supplyVolTrips);
   }

//------------------------------------------------------------------------------

void WitPipMgr::getProdVolPgg (
      WitOperation *     theOpn,
      WitPeriod          execPer,
      WitPartTripStack & prodVolTrips)
   {
   myPgg ()->prodVolPgg ()->getPgg (theOpn, execPer, prodVolTrips);
   }

//------------------------------------------------------------------------------

void WitPipMgr::getConsVolPgg (
      WitOperation *     theOpn,
      WitPeriod          execPer,
      WitPartTripStack & consVolTrips)
   {
   myPgg ()->consVolPgg ()->getPgg (theOpn, execPer, consVolTrips);
   }

//------------------------------------------------------------------------------

void WitPipMgr::getSideVolPgg (
      WitOperation *     theOpn,
      WitPeriod          execPer,
      WitPartTripStack & sideVolTrips)
   {
   myPgg ()->sideVolPgg ()->getPgg (theOpn, execPer, sideVolTrips);
   }

//------------------------------------------------------------------------------

void WitPipMgr::checkRestrictions ()
   {
   WitSubEntry * theSub;
   WitPeriod     execPer;

   if (not myPostprocessor ()->postprocessed ())
      myMsgFac () ("unpostPipSmsg");

   forEachSubEntry (theSub, myProblem ())
      forEachPeriod (execPer, myProblem ())
         if (theSub->consRate ()[execPer] < 0.0)
            myMsgFac () ("pipNegSubConsRateSmsg",
               theSub->myOperationName (),
               theSub->myBomEntIndex (),
               theSub->localIndex (),
               execPer,
               theSub->consRate ()[execPer]);
   }

//------------------------------------------------------------------------------
// Implementation of class PipSeqMgr.
//------------------------------------------------------------------------------

WitPipSeqMgr::WitPipSeqMgr (WitProblem * theProblem):

      WitProbAssoc    (theProblem),
      revShipSeq_     (theProblem),
      shipPerForRead_ (-1)
   {
   }

//------------------------------------------------------------------------------

WitPipSeqMgr::~WitPipSeqMgr ()
   {
   clear ();
   }

//------------------------------------------------------------------------------

void WitPipSeqMgr::clear ()
   {
   revShipSeq_.clear ();
   }

//------------------------------------------------------------------------------

void WitPipSeqMgr::append (
      WitDemand * theDemand,
      WitPeriod   shipPer,
      double      incShipVol)
   {
   witAssert (incShipVol >= 0.0);

   if (incShipVol >= NET_TOL)
      revShipSeq_.push (theDemand, shipPer, incShipVol);
   }

//------------------------------------------------------------------------------

void WitPipSeqMgr::get (WitDemTripStack & theShipSeq)
   {
   theShipSeq.revCopyFrom (revShipSeq_);
   }

//------------------------------------------------------------------------------

void WitPipSeqMgr::reduce ()
   {
   WitSchedule <WitDemand, double> availShipVol  (myProblem (), 0.0);
   WitDemTripStack                 theShipSeq    (myProblem ());

   WitDemand * theDemand;
   WitPeriod   shipPer;
   double      incShipVol;
   double      reduc;
   double      maxRed;
   WitDemand * maxRedDemand;
   WitPeriod   maxRedPer;

   maxRed = 0.0;

   forEachDemand (theDemand, myProblem ())
      availShipVol (theDemand) = theDemand->shipVol ();

   theShipSeq.takeContentsFrom (revShipSeq_);

   theShipSeq.reverse ();

   while (theShipSeq.pop (theDemand, shipPer, incShipVol))
      {
      if (incShipVol > availShipVol (theDemand)[shipPer])
         {
         reduc = incShipVol - availShipVol (theDemand)[shipPer];

         if (reduc > maxRed)
            {
            maxRed       = reduc;
            maxRedDemand = theDemand;
            maxRedPer    = shipPer;
            }

         incShipVol = availShipVol (theDemand)[shipPer];
         }

      availShipVol (theDemand)[shipPer] -= incShipVol;

      if (incShipVol > NET_TOL)
         append (theDemand, shipPer, incShipVol);
      }

   if (maxRed > 0.001)
      myMsgFac () ("pipSeqReducWmsg",
         maxRedDemand->demandedPartName (),
         maxRedDemand->demandName (),
         maxRedPer,
         maxRed);
   }

//------------------------------------------------------------------------------

void WitPipSeqMgr::display ()
   {
   WitDemTripStack theShipSeq (myProblem ());
   WitDemand *     theDemand;
   WitPeriod       shipPer;
   double          incShipVol;

   myMsgFac () ("pipSeqHeadingDdMsg");

   if (revShipSeq_.isEmpty ())
      {
      myMsgFac () ("EmptyPipSeqDdMsg");

      return;
      }

   get (theShipSeq);

   while (theShipSeq.pop (theDemand, shipPer, incShipVol))
      myMsgFac () ("pipShipTripDdMsg",
         theDemand->demandedPartName (),
         theDemand->demandName (),
         shipPer,
         incShipVol);
   }

//------------------------------------------------------------------------------

void WitPipSeqMgr::copy (WitPipSeqMgr * theSeqMgr)
   {
   WitDemTripStack theShipSeq (myProblem ());

   theSeqMgr->get (theShipSeq);

   theShipSeq.reverse ();

   revShipSeq_.takeContentsFrom (theShipSeq);
   }

//------------------------------------------------------------------------------

void WitPipSeqMgr::writeData ()
   {
   WitDemTripStack theShipSeq (myProblem ());
   WitDemand *     theDemand;
   WitPeriod       shipPer;
   double          incShipVol;

   if (revShipSeq_.isEmpty ())
      return;

   myMsgFac () ("pipSeqPreCommentWdMsg");

   myDataWriter ()->writeFirstKeyWord ("set");
   myDataWriter ()->writeKeyWord      ("problem");

   myDataWriter ()->writeBool         ("pipSeqClearsNow", true);

   myDataWriter ()->writeEndCommand ();

   get (theShipSeq);

   while (theShipSeq.pop (theDemand, shipPer, incShipVol))
      {
      myDataWriter ()->writeFirstKeyWord ("set");
      myDataWriter ()->writeKeyWord      ("demand");
      myDataWriter ()->writeStringValue  (theDemand->demandedPartName ());
      myDataWriter ()->writeStringValue  (theDemand->demandName       ());

      myDataWriter ()->writeInt          ("pipSeqShipPeriod", shipPer);

      myDataWriter ()->writeDouble       ("pipSeqIncShipVol", incShipVol);

      myDataWriter ()->writeEndCommand ();
      }

   myMsgFac () ("pipSeqPostCommentWdMsg");
   }

//------------------------------------------------------------------------------

void WitPipSeqMgr::setShipPerForRead (WitPeriod shipPer)
   {
   shipPerForRead_ = shipPer;
   }
