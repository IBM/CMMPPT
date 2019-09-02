//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "PipPgg.C"
//
// Contains the implementation of the following classes and templates:
//
//    PipPgg
//    PipAttPgg <SrcComp>,          with explicit instantiation
//    PipRcpPgg <SrcComp, RcpComp>, with explicit instantiation
//    PipRcpFac          <RcpComp>
//------------------------------------------------------------------------------

#include <PipPgg.h>
#include <PipAttPgg.h>
#include <PipRcpPgg.h>
#include <PipRcpFac.h>
#include <PipIncPgg.h>
#include <PipBuilder.h>
#include <PipMgr.h>
#include <GlobalComp.h>
#include <Demand.h>
#include <Part.h>
#include <Operation.h>
#include <SubEntry.h>
#include <BopEntry.h>
#include <ObjStack.h>
#include <PairStack.h>
#include <TripStack.h>
#include <Timing.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class PipPgg.
//------------------------------------------------------------------------------

WitPipPgg::WitPipPgg (WitProblem * theProblem):

      WitProbAssoc  (theProblem),
      execVolPgg_   (NULL),
      coExecVolPgg_ (NULL),
      subVolPgg_    (NULL),
      supplyVolPgg_ (NULL),
      prodVolPgg_   (NULL),
      consVolPgg_   (NULL),
      sideVolPgg_   (NULL),
      myPipBuilder_ (NULL)
   {
   execVolPgg_   = new WitPipAttPgg <WitOperation> ("ExecVol",   this);
   coExecVolPgg_ = new WitPipAttPgg <WitBopEntry>  ("CoExecVol", this);
   subVolPgg_    = new WitPipAttPgg <WitSubEntry>  ("SubVol",    this);
   supplyVolPgg_ = new WitPipAttPgg <WitPart>      ("SupplyVol", this);
   prodVolPgg_   = new WitPipAttPgg <WitPart>      ("ProdVol",   this);
   consVolPgg_   = new WitPipAttPgg <WitPart>      ("ConsVol",   this);
   sideVolPgg_   = new WitPipAttPgg <WitPart>      ("SideVol",   this);

   myPipBuilder_ = new WitPipBuilder (this);

   myPipBuilder_->buildPegging ();

   delete myPipBuilder_;

   myPipBuilder_ = NULL;

   if (devMode ())
      countTrips ();

   if (myProblem ()->myGlobalComp ()->inputPauses ())
      myProblem ()->pauseForInput ("at the conclusion of building PIP");
   }

//------------------------------------------------------------------------------

WitPipPgg::~WitPipPgg ()
   {
   delete   sideVolPgg_;
   delete   consVolPgg_;
   delete   prodVolPgg_;
   delete supplyVolPgg_;
   delete    subVolPgg_;
   delete coExecVolPgg_;
   delete   execVolPgg_;
   }

//------------------------------------------------------------------------------

void WitPipPgg::consolidatePeggings ()
   {
     execVolPgg_->consolidate ();
   coExecVolPgg_->consolidate ();
      subVolPgg_->consolidate ();
   supplyVolPgg_->consolidate ();
     prodVolPgg_->consolidate ();
     consVolPgg_->consolidate ();
     sideVolPgg_->consolidate ();
   }

//------------------------------------------------------------------------------

void WitPipPgg::print ()
   {
   fprintf (solnFile (),
      "\n\n"
      "Post-Implosion Pegging:\n");

     execVolPgg_->print ();
   coExecVolPgg_->print ();
      subVolPgg_->print ();
   supplyVolPgg_->print ();
     prodVolPgg_->print ();
     consVolPgg_->print ();
     sideVolPgg_->print ();
   }

//------------------------------------------------------------------------------

void WitPipPgg::countTrips ()
   {
   int nTrips;

   nTrips =
          execVolPgg_->totTrips ()
      + coExecVolPgg_->totTrips ()
      +    subVolPgg_->totTrips ()
      + supplyVolPgg_->totTrips ()
      +   prodVolPgg_->totTrips ()
      +   consVolPgg_->totTrips ()
      +   sideVolPgg_->totTrips ();

   myMsgFac () ("totPegTripsMsg", nTrips);
   }

//------------------------------------------------------------------------------
// Implementation of class template PipAttPgg <SrcComp>.
//------------------------------------------------------------------------------

template <typename SrcComp> 
      WitPipAttPgg <SrcComp>::WitPipAttPgg (
            const char * theAttName,
            WitPipPgg *  thePipPgg):

         WitProbAssoc (thePipPgg),
         myAttName_   (theAttName),
         myPipPgg_    (thePipPgg),
         myPggToDems_ (NULL),
         myPggToOpns_ (NULL)
   {
   myPggToDems_ = new WitPipRcpPgg <SrcComp, WitDemand>    (this);
   myPggToOpns_ = new WitPipRcpPgg <SrcComp, WitOperation> (this);
   }

//------------------------------------------------------------------------------

template <typename SrcComp> 
      WitPipAttPgg <SrcComp>::~WitPipAttPgg ()
   {
   delete myPggToOpns_;
   delete myPggToDems_;
   }

//------------------------------------------------------------------------------

template <typename SrcComp> 
      void WitPipAttPgg <SrcComp>::appendIncPgg (
         SrcComp * theSrcComp,
         WitPeriod srcPer)
   {
   myPggToDems_->
      appendIncPgg (
         theSrcComp,
         srcPer,
         myPipPgg_->myPipBuilder ()->myIncPggToDems ());

   myPggToOpns_->
      appendIncPgg (
         theSrcComp,
         srcPer,
         myPipPgg_->myPipBuilder ()->myIncPggToOpns ());
   }

//------------------------------------------------------------------------------

template <typename SrcComp> 
      void WitPipAttPgg <SrcComp>::getPgg (
         WitDemand *              theDemand,
         WitPeriod                shipPer,
         WitTripStack <SrcComp> & thePegTrips)
   {
   myPggToDems_->getPegTrips (theDemand, shipPer, thePegTrips);
   }

//------------------------------------------------------------------------------

template <typename SrcComp> 
      void WitPipAttPgg <SrcComp>::getPgg (
         WitOperation *           theOpn,
         WitPeriod                execPer,
         WitTripStack <SrcComp> & thePegTrips)
   {
   myPggToOpns_->getPegTrips (theOpn, execPer, thePegTrips);
   }

//------------------------------------------------------------------------------

template <typename SrcComp> 
      void WitPipAttPgg <SrcComp>::print ()
   {
   fprintf (solnFile (),
      "\n"
      "%s Pegging:\n",
      myAttName_);

   myPggToDems_->print ();
   myPggToOpns_->print ();
   }

//------------------------------------------------------------------------------

template <typename SrcComp> 
      void WitPipAttPgg <SrcComp>::consolidate ()
   {
   myPggToDems_->consolidate ();
   myPggToOpns_->consolidate ();
   }

//------------------------------------------------------------------------------

template <typename SrcComp> 
      int WitPipAttPgg <SrcComp>::totTrips ()
   {
   return
      myPggToDems_->totTrips ()
    + myPggToOpns_->totTrips ();
   }

//------------------------------------------------------------------------------

template <>
      void WitPipAttPgg <WitOperation>::getAllSrcComps (
         WitList <WitOperation> & theOpnList)
   {
   WitOperation * theOpn;

   theOpnList.clear ();

   forEachOperation (theOpn, myProblem ())
      {
      theOpnList.append (theOpn);
      }
   }

//------------------------------------------------------------------------------

template <>
      void WitPipAttPgg <WitBopEntry>::getAllSrcComps (
         WitList <WitBopEntry> & theBopEntList)
   {
   WitBopEntry * theBopEnt;

   theBopEntList.clear ();

   forEachBopEntry (theBopEnt, myProblem ())
      {
      theBopEntList.append (theBopEnt);
      }
   }

//------------------------------------------------------------------------------

template <>
      void WitPipAttPgg <WitSubEntry>::getAllSrcComps (
         WitList <WitSubEntry> & theSubList)
   {
   WitSubEntry * theSub;

   theSubList.clear ();

   forEachSubEntry (theSub, myProblem ())
      {
      theSubList.append (theSub);
      }
   }

//------------------------------------------------------------------------------

template <>
      void WitPipAttPgg <WitPart>::getAllSrcComps (
         WitList <WitPart> & thePartList)
   {
   WitPart * thePart;

   thePartList.clear ();

   forEachPart (thePart, myProblem ())
      {
      thePartList.append (thePart);
      }
   }

//------------------------------------------------------------------------------

template <>
      void WitPipAttPgg <WitOperation>::prtSrcComp (WitOperation * theOpn)
   {
   fprintf (solnFile (), "  %3s", theOpn->operationName ().myCstring ());
   }

//------------------------------------------------------------------------------

template <>
      void WitPipAttPgg <WitBopEntry>::prtSrcComp (WitBopEntry * theBopEnt)
   {
   fprintf (solnFile (),
      "  %3s  %3d",
      theBopEnt->myOperationName ().myCstring (),
      theBopEnt->localIndex ());
   }

//------------------------------------------------------------------------------

template <>
      void WitPipAttPgg <WitSubEntry>::prtSrcComp (WitSubEntry * theSub)
   {
   fprintf (solnFile (),
      "  %3s  %3d  %3d",
      theSub->myOperationName ().myCstring (),
      theSub->myBomEntIndex (),
      theSub->localIndex ());
   }

//------------------------------------------------------------------------------

template <>
      void WitPipAttPgg <WitPart>::prtSrcComp (WitPart * thePart)
   {
   fprintf (solnFile (), "  %4s", thePart->partName ().myCstring ());
   }

//------------------------------------------------------------------------------

template <>
      const char * WitPipAttPgg <WitOperation>::getSrcCompLabel ()
   {
   return "Opn";
   }

//------------------------------------------------------------------------------

template <>
      const char * WitPipAttPgg <WitBopEntry>::getSrcCompLabel ()
   {
   return "Opn  Idx";
   }

//------------------------------------------------------------------------------

template <>
      const char * WitPipAttPgg <WitSubEntry>::getSrcCompLabel ()
   {
   return "Opn  Idx  Idx";
   }

//------------------------------------------------------------------------------

template <>
      const char * WitPipAttPgg <WitPart>::getSrcCompLabel ()
   {
   return "Part";
   }

//------------------------------------------------------------------------------

template <>
      const char * WitPipAttPgg <WitOperation>::getSrcCompHyphens ()
   {
   return "---";
   }

//------------------------------------------------------------------------------

template <>
      const char * WitPipAttPgg <WitBopEntry>::getSrcCompHyphens ()
   {
   return "--------";
   }

//------------------------------------------------------------------------------

template <>
      const char * WitPipAttPgg <WitSubEntry>::getSrcCompHyphens ()
   {
   return "-------------";
   }

//------------------------------------------------------------------------------

template <>
      const char * WitPipAttPgg <WitPart>::getSrcCompHyphens ()
   {
   return "----";
   }

//------------------------------------------------------------------------------
// Explicit instantiation of class template PipAttPgg <SrcComp>.
//------------------------------------------------------------------------------

template class WitPipAttPgg <WitOperation>;
template class WitPipAttPgg <WitBopEntry>;
template class WitPipAttPgg <WitSubEntry>;
template class WitPipAttPgg <WitPart>;

//------------------------------------------------------------------------------
// Implementation of class template PipRcpPgg <SrcComp, RcpComp>.
//------------------------------------------------------------------------------

template <typename SrcComp, typename RcpComp>
      WitPipRcpPgg <SrcComp, RcpComp>::WitPipRcpPgg (
            WitPipAttPgg <SrcComp> * thePipAttPgg):

         WitProbAssoc (thePipAttPgg),
         myPipAttPgg_ (thePipAttPgg),
         myPegTrips_  ()
   {
   WitObjStack  <RcpComp>            theRcpComps (myProblem ());
   WitPairStack <RcpComp, WitPeriod> theRcpPairs (myProblem ());
   RcpComp *                         theRcpComp;
   WitPeriod                         rcpPer;

   myPegTrips_.allocate1D (myProblem ());

   WitPipRcpFac <RcpComp>::getRcpComps (theRcpComps, myProblem ());

   while (theRcpComps.pop (theRcpComp))
      {
      myPegTrips_.myPtrTVecAt (theRcpComp).allocate (myProblem ());
      }

   WitPipRcpFac <RcpComp>::getRcpPairs (theRcpPairs, myProblem ());
                         
   while (theRcpPairs.pop (theRcpComp, rcpPer))
      if (myPegTrips_.myPtrAt (theRcpComp, rcpPer) == NULL)
         {
         myPegTrips_.myPtrAt (theRcpComp, rcpPer) =
            new WitTripStack <SrcComp> (myProblem ());
         }
   }

//------------------------------------------------------------------------------

template <typename SrcComp, typename RcpComp>
      WitPipRcpPgg <SrcComp, RcpComp>::~WitPipRcpPgg ()
   {
   WitObjStack <RcpComp> theRcpComps (myProblem ());
   RcpComp *             theRcpComp;
   WitPeriod             rcpPer;

   WitPipRcpFac <RcpComp>::getRcpComps (theRcpComps, myProblem ());

   while (theRcpComps.pop (theRcpComp))
      forEachPeriod (rcpPer, myProblem ())
         delete myPegTrips_.myPtrAt (theRcpComp, rcpPer);
   }

//------------------------------------------------------------------------------

template <typename SrcComp, typename RcpComp>
      void WitPipRcpPgg <SrcComp, RcpComp>::appendIncPgg (
            SrcComp *                theSrcComp,
            WitPeriod                srcPer,
            WitPipIncPgg <RcpComp> * theIncPgg)
   {
   RcpComp * theRcpComp;
   WitPeriod rcpPer;
   double    pegVol;

   while (theIncPgg->getIncPggTrip (theRcpComp, rcpPer, pegVol))
      {
      myPegTrips_.
         myElemAt (theRcpComp, rcpPer).
            push (theSrcComp, srcPer, pegVol);
      }
   }

//------------------------------------------------------------------------------

template <typename SrcComp, typename RcpComp>
      void WitPipRcpPgg <SrcComp, RcpComp>::consolidate ()
   {
   WitSchedule  <SrcComp, double>    totPegVol;
   WitObjStack <RcpComp>             theRcpComps (myProblem ());
   RcpComp *                         theRcpComp;
   WitPeriod                         rcpPer;
   WitTripStack <SrcComp> *          thePegTripsPtr;
   SrcComp *                         theSrcComp;
   WitPeriod                         srcPer;
   double                            pegVol;
   WitPairStack <SrcComp, WitPeriod> pegPairs (myProblem ());

   totPegVol.allocate1D (myProblem ());

   WitPipRcpFac <RcpComp>::getRcpComps (theRcpComps, myProblem ());

   while (theRcpComps.pop (theRcpComp))
      forEachPeriod (rcpPer, myProblem ())
         {
         thePegTripsPtr = myPegTrips_.myPtrAt (theRcpComp, rcpPer);

         if (thePegTripsPtr == NULL)
            continue;

         while (thePegTripsPtr->pop (theSrcComp, srcPer, pegVol))
            {
            if (not totPegVol (theSrcComp).isAllocated ())
               totPegVol (theSrcComp).allocate (myProblem (), 0.0);

            totPegVol (theSrcComp)[srcPer] += pegVol;

            pegPairs.push (theSrcComp, srcPer);
            }

         while (pegPairs.pop (theSrcComp, srcPer))
            {
            pegVol = totPegVol (theSrcComp)[srcPer];

            if (pegVol == 0.0)
               continue;

            totPegVol (theSrcComp)[srcPer] = 0.0;

            thePegTripsPtr->push (theSrcComp, srcPer, pegVol);
            }
         }
   }

//------------------------------------------------------------------------------

template <typename SrcComp, typename RcpComp>
      void WitPipRcpPgg <SrcComp, RcpComp>::getPegTrips (
         RcpComp *                theRcpComp,
         WitPeriod                rcpPer,
         WitTripStack <SrcComp> & thePegTrips)
   {
   WitTimer::enterSection ("pip");

   thePegTrips.clear ();

   if (myPegTrips_.myPtrTVecAt (theRcpComp).isAllocated ())
      if (myPegTrips_.myPtrAt (theRcpComp, rcpPer) != NULL)
         thePegTrips.
            revCopyFrom (myPegTrips_.myElemAt (theRcpComp, rcpPer));

   WitTimer::leaveSection ("pip");
   }

//------------------------------------------------------------------------------

template <typename SrcComp, typename RcpComp>
      int WitPipRcpPgg <SrcComp, RcpComp>::totTrips ()
   {
   int                   nTrips;
   WitObjStack <RcpComp> theRcpComps (myProblem ());
   RcpComp *             theRcpComp;
   WitPeriod             rcpPer;

   nTrips = 0;

   WitPipRcpFac <RcpComp>::getRcpComps (theRcpComps, myProblem ());

   while (theRcpComps.pop (theRcpComp))
      forEachPeriod (rcpPer, myProblem ())
         if (myPegTrips_.myPtrAt (theRcpComp, rcpPer) != NULL)
            nTrips += myPegTrips_.myElemAt (theRcpComp, rcpPer).nElements ();

   return nTrips;
   }

//------------------------------------------------------------------------------

template <typename SrcComp, typename RcpComp>
      void WitPipRcpPgg <SrcComp, RcpComp>::print ()
   {
   WitObjStack <RcpComp> theRcpComps (myProblem ());
   RcpComp *             theRcpComp;
   WitPeriod             rcpPer;

   WitPipRcpFac <RcpComp>::getRcpComps (theRcpComps, myProblem ());

   theRcpComps.reverse ();

   if (theRcpComps.isEmpty ())
      return;

   fprintf (solnFile (),
      "\n"
      "   ----------%s----------------\n"
      "   %3s  Per  %s  Per  %9s\n"
      "   ----------%s----------------\n",
      myPipAttPgg_->          getSrcCompHyphens (),
      WitPipRcpFac <RcpComp>::getRcpCompLabel   (),
      myPipAttPgg_->          getSrcCompLabel   (),
      myPipAttPgg_->          myAttName         (),
      myPipAttPgg_->          getSrcCompHyphens ());

   while (theRcpComps.pop (theRcpComp))
      forEachPeriod (rcpPer, myProblem ())
         prtRcpPgg (theRcpComp, rcpPer);
   }

//------------------------------------------------------------------------------

template <typename SrcComp, typename RcpComp>
      void WitPipRcpPgg <SrcComp, RcpComp>::prtRcpPgg (
         RcpComp * theRcpComp,
         WitPeriod rcpPer)
   {
   bool                   pegVolPrinted;
   WitTripStack <SrcComp> thePegTrips (myProblem ());
   SrcComp *              theSrcComp;
   WitPeriod              srcPer;
   double                 pegVol;

   pegVolPrinted = false;

   getPegTrips (theRcpComp, rcpPer, thePegTrips);

   while (thePegTrips.pop (theSrcComp, srcPer, pegVol))
      {
      if (pegVolPrinted)
         fprintf (solnFile (), "           ");
      else
         fprintf (solnFile (),
            "   %3s  %3d",
            WitPipRcpFac <RcpComp>::getRcpCompName (theRcpComp),
            rcpPer);

      myPipAttPgg_->prtSrcComp (theSrcComp);

      fprintf (solnFile (), "  %3d  %9.3f\n", srcPer, pegVol);

      pegVolPrinted = true;
      }
   }

//------------------------------------------------------------------------------
// Explicit instantiation of class template PipRcpPgg <SrcComp, RcpComp>.
//------------------------------------------------------------------------------

template class WitPipRcpPgg <WitOperation, WitDemand>;
template class WitPipRcpPgg <WitOperation, WitOperation>;
template class WitPipRcpPgg <WitBopEntry,  WitDemand>;
template class WitPipRcpPgg <WitBopEntry,  WitOperation>;
template class WitPipRcpPgg <WitSubEntry,  WitDemand>;
template class WitPipRcpPgg <WitSubEntry,  WitOperation>;
template class WitPipRcpPgg <WitPart,      WitDemand>;
template class WitPipRcpPgg <WitPart,      WitOperation>;

//------------------------------------------------------------------------------
// Implementation of class template PipRcpFac <RcpComp>.
//------------------------------------------------------------------------------

template <>
      void WitPipRcpFac <WitDemand>::getRcpPairs (
         WitPairStack <WitDemand, WitPeriod> & theDemPairs,
         WitProblem *                          theProblem)
   {
   WitDemTripStack theShipSeq (theProblem);
   WitDemand *     theDemand;
   WitPeriod       shipPer;
   double          incShipVol;

   theDemPairs.clear ();

   theProblem->myPipMgr ()->getShipSeq (theShipSeq);

   while (theShipSeq.pop (theDemand, shipPer, incShipVol))
      theDemPairs.push (theDemand, shipPer);
   }

//------------------------------------------------------------------------------

template <>
      void WitPipRcpFac <WitOperation>::getRcpPairs (
         WitPairStack <WitOperation, WitPeriod> & theOpnPairs,
         WitProblem *                             theProblem)
   {
   WitOperation * theOpn;
   WitPeriod      execPer;
   
   theOpnPairs.clear ();

   forEachOperation (theOpn, theProblem)
      if (theOpn->pipEnabled ())
         forEachPeriod (execPer, theProblem)
            {
            theOpnPairs.push (theOpn, execPer);
            }
   }

//------------------------------------------------------------------------------

template <>
      void WitPipRcpFac <WitDemand>::getRcpComps (
         WitObjStack <WitDemand> & theDemands,
         WitProblem *              theProblem)
   {
   WitDemand * theDemand;

   theDemands.clear ();

   forEachDemand (theDemand, theProblem)
      theDemands.push (theDemand);
   }

//------------------------------------------------------------------------------

template <>
      void WitPipRcpFac <WitOperation>::getRcpComps (
         WitObjStack <WitOperation> & theOpns,
         WitProblem *                 theProblem)
   {
   WitOperation * theOpn;

   theOpns.clear ();

   forEachOperation (theOpn, theProblem)
      if (theOpn->pipEnabled ())
         theOpns.push (theOpn);
   }

//------------------------------------------------------------------------------

template <>
      const char * WitPipRcpFac <WitDemand>::getRcpCompLabel ()
   {
   return "Dem";
   }

//------------------------------------------------------------------------------

template <>
      const char * WitPipRcpFac <WitOperation>::getRcpCompLabel ()
   {
   return "Opn";
   }

//------------------------------------------------------------------------------

template <>
      const char * WitPipRcpFac <WitDemand>::getRcpCompName (
         WitDemand * theDemand)
   {
   return theDemand->demandName ().myCstring ();
   }

//------------------------------------------------------------------------------

template <>
      const char * WitPipRcpFac <WitOperation>::getRcpCompName (
         WitOperation * theOpn)
   {
   return theOpn->operationName ().myCstring ();
   }
