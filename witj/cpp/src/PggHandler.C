//------------------------------------------------------------------------------
// WIT-J C++ Source File PggHandler.C.
//
// Contains the implementation of the class PggHandler.
//------------------------------------------------------------------------------

#include   <PggHandler.h>
#include  <Coordinator.h>
#include         <Part.h>
#include    <Operation.h>
#include   <Substitute.h>
#include     <BopEntry.h>
#include       <Demand.h>
#include <ArrayListJOR.h>

#include          <wit.h>

//------------------------------------------------------------------------------

WitJ::PggHandler::PggHandler (Coordinator * theCoord):

      myCoordinator_  (theCoord),
      myWitRun_       (theCoord->getWitRun ()),
      nTriples_       (-1),
      myPartNameCVec_ (NULL),
      myOpnNameCVec_  (NULL),
      myBopIdxCVec_   (NULL),
      myBomIdxCVec_   (NULL),
      mySubIdxCVec_   (NULL),
      myPeriodCVec_   (NULL),
      myVolumeCVec_   (NULL)
   {
   }

//------------------------------------------------------------------------------

WitJ::PggHandler::~PggHandler ()
   {
   }

//------------------------------------------------------------------------------

void WitJ::PggHandler::getConsVolPip (
      Demand *             theDemand,
      int                  shipPer,
      PeggingTripleALJOR & theTripALJOR)
   {
   witGetDemandConsVolPipDbl (
      myWitRun_,
      theDemand->getPartName   (),
      theDemand->getDemandName (),
      shipPer,
    & nTriples_,
    & myPartNameCVec_,
    & myPeriodCVec_,
    & myVolumeCVec_);

   storePartPgg (theTripALJOR);
   }

//------------------------------------------------------------------------------

void WitJ::PggHandler::getCoExecVolPip (
      Demand *             theDemand,
      int                  shipPer,
      PeggingTripleALJOR & theTripALJOR)
   {
   witGetDemandCoExecVolPipDbl (
      myWitRun_,
      theDemand->getPartName   (),
      theDemand->getDemandName (),
      shipPer,
    & nTriples_,
    & myOpnNameCVec_,
    & myBopIdxCVec_,
    & myPeriodCVec_,
    & myVolumeCVec_);

   storeBopEntPgg (theTripALJOR);
   }

//------------------------------------------------------------------------------

void WitJ::PggHandler::getExecVolPip (
      Demand *             theDemand,
      int                  shipPer,
      PeggingTripleALJOR & theTripALJOR)
   {
   witGetDemandExecVolPipDbl (
      myWitRun_,
      theDemand->getPartName   (),
      theDemand->getDemandName (),
      shipPer,
    & nTriples_,
    & myOpnNameCVec_,
    & myPeriodCVec_,
    & myVolumeCVec_);

   storeOpnPgg (theTripALJOR);
   }

//------------------------------------------------------------------------------

void WitJ::PggHandler::getProdVolPip (
      Demand *             theDemand,
      int                  shipPer,
      PeggingTripleALJOR & theTripALJOR)
   {
   witGetDemandProdVolPipDbl (
      myWitRun_,
      theDemand->getPartName   (),
      theDemand->getDemandName (),
      shipPer,
    & nTriples_,
    & myPartNameCVec_,
    & myPeriodCVec_,
    & myVolumeCVec_);

   storePartPgg (theTripALJOR);
   }

//------------------------------------------------------------------------------

void WitJ::PggHandler::getSideVolPip (
      Demand *             theDemand,
      int                  shipPer,
      PeggingTripleALJOR & theTripALJOR)
   {
   witGetDemandSideVolPipDbl (
      myWitRun_,
      theDemand->getPartName   (),
      theDemand->getDemandName (),
      shipPer,
    & nTriples_,
    & myPartNameCVec_,
    & myPeriodCVec_,
    & myVolumeCVec_);

   storePartPgg (theTripALJOR);
   }

//------------------------------------------------------------------------------

void WitJ::PggHandler::getSubVolPip (
      Demand *             theDemand,
      int                  shipPer,
      PeggingTripleALJOR & theTripALJOR)
   {
   witGetDemandSubVolPipDbl (
      myWitRun_,
      theDemand->getPartName   (),
      theDemand->getDemandName (),
      shipPer,
    & nTriples_,
    & myOpnNameCVec_,
    & myBomIdxCVec_,
    & mySubIdxCVec_,
    & myPeriodCVec_,
    & myVolumeCVec_);

   storeSubPgg (theTripALJOR);
   }

//------------------------------------------------------------------------------

void WitJ::PggHandler::getSupplyVolPip (
      Demand *             theDemand,
      int                  shipPer,
      PeggingTripleALJOR & theTripALJOR)
   {
   witGetDemandSupplyVolPipDbl (
      myWitRun_,
      theDemand->getPartName   (),
      theDemand->getDemandName (),
      shipPer,
    & nTriples_,
    & myPartNameCVec_,
    & myPeriodCVec_,
    & myVolumeCVec_);

   storePartPgg (theTripALJOR);
   }

//------------------------------------------------------------------------------

void WitJ::PggHandler::getExecVolCp (
      Demand *             theDemand,
      int                  shipPer,
      PeggingTripleALJOR & theTripALJOR)
   {
   witGetDemandExecVolPeggingDbl (
      myWitRun_,
      theDemand->getPartName   (),
      theDemand->getDemandName (),
      shipPer,
    & nTriples_,
    & myOpnNameCVec_,
    & myPeriodCVec_,
    & myVolumeCVec_);

   storeOpnPgg (theTripALJOR);
   }

//------------------------------------------------------------------------------

void WitJ::PggHandler::getSubVolCp (
      Demand *             theDemand,
      int                  shipPer,
      PeggingTripleALJOR & theTripALJOR)
   {
   witGetDemandSubVolPeggingDbl (
      myWitRun_,
      theDemand->getPartName   (),
      theDemand->getDemandName (),
      shipPer,
    & nTriples_,
    & myOpnNameCVec_,
    & myBomIdxCVec_,
    & mySubIdxCVec_,
    & myPeriodCVec_,
    & myVolumeCVec_);

   storeSubPgg (theTripALJOR);
   }

//------------------------------------------------------------------------------

void WitJ::PggHandler::getConsVolPip (
      Operation *          theOpn,
      int                  execPer,
      PeggingTripleALJOR & theTripALJOR)
   {
   witGetOperationConsVolPipDbl (
      myWitRun_,
      theOpn->getOperationName (),
      execPer,
    & nTriples_,
    & myPartNameCVec_,
    & myPeriodCVec_,
    & myVolumeCVec_);

   storePartPgg (theTripALJOR);
   }

//------------------------------------------------------------------------------

void WitJ::PggHandler::getCoExecVolPip (
      Operation *          theOpn,
      int                  execPer,
      PeggingTripleALJOR & theTripALJOR)
   {
   witGetOperationCoExecVolPipDbl (
      myWitRun_,
      theOpn->getOperationName (),
      execPer,
    & nTriples_,
    & myOpnNameCVec_,
    & myBopIdxCVec_,
    & myPeriodCVec_,
    & myVolumeCVec_);

   storeBopEntPgg (theTripALJOR);
   }

//------------------------------------------------------------------------------

void WitJ::PggHandler::getExecVolPip (
      Operation *          theOpn,
      int                  execPer,
      PeggingTripleALJOR & theTripALJOR)
   {
   witGetOperationExecVolPipDbl (
      myWitRun_,
      theOpn->getOperationName (),
      execPer,
    & nTriples_,
    & myOpnNameCVec_,
    & myPeriodCVec_,
    & myVolumeCVec_);

   storeOpnPgg (theTripALJOR);
   }

//------------------------------------------------------------------------------

void WitJ::PggHandler::getProdVolPip (
      Operation *          theOpn,
      int                  execPer,
      PeggingTripleALJOR & theTripALJOR)
   {
   witGetOperationProdVolPipDbl (
      myWitRun_,
      theOpn->getOperationName (),
      execPer,
    & nTriples_,
    & myPartNameCVec_,
    & myPeriodCVec_,
    & myVolumeCVec_);

   storePartPgg (theTripALJOR);
   }

//------------------------------------------------------------------------------

void WitJ::PggHandler::getSideVolPip (
      Operation *          theOpn,
      int                  execPer,
      PeggingTripleALJOR & theTripALJOR)
   {
   witGetOperationSideVolPipDbl (
      myWitRun_,
      theOpn->getOperationName (),
      execPer,
    & nTriples_,
    & myPartNameCVec_,
    & myPeriodCVec_,
    & myVolumeCVec_);

   storePartPgg (theTripALJOR);
   }

//------------------------------------------------------------------------------

void WitJ::PggHandler::getSubVolPip (
      Operation *          theOpn,
      int                  execPer,
      PeggingTripleALJOR & theTripALJOR)
   {
   witGetOperationSubVolPipDbl (
      myWitRun_,
      theOpn->getOperationName (),
      execPer,
    & nTriples_,
    & myOpnNameCVec_,
    & myBomIdxCVec_,
    & mySubIdxCVec_,
    & myPeriodCVec_,
    & myVolumeCVec_);

   storeSubPgg (theTripALJOR);
   }

//------------------------------------------------------------------------------

void WitJ::PggHandler::getSupplyVolPip (
      Operation *          theOpn,
      int                  execPer,
      PeggingTripleALJOR & theTripALJOR)
   {
   witGetOperationSupplyVolPipDbl (
      myWitRun_,
      theOpn->getOperationName (),
      execPer,
    & nTriples_,
    & myPartNameCVec_,
    & myPeriodCVec_,
    & myVolumeCVec_);

   storePartPgg (theTripALJOR);
   }

//------------------------------------------------------------------------------

void WitJ::PggHandler::storePartPgg (PeggingTripleALJOR & theTripALJOR)
   {
   int    theIdx;
   Part * thePart;

   theTripALJOR.clearAndAllocate (nTriples_);

   for (theIdx = 0; theIdx < nTriples_; theIdx ++)
      {
      thePart = myCoordinator_->findPart (myPartNameCVec_[theIdx]);

      theTripALJOR.add (thePart, myPeriodCVec_[theIdx], myVolumeCVec_[theIdx]);
      }

   freeWitCVectors ();
   }

//------------------------------------------------------------------------------

void WitJ::PggHandler::storeOpnPgg (PeggingTripleALJOR & theTripALJOR)
   {
   int         theIdx;
   Operation * theOpn;

   theTripALJOR.clearAndAllocate (nTriples_);

   for (theIdx = 0; theIdx < nTriples_; theIdx ++)
      {
      theOpn = myCoordinator_->findOperation (myOpnNameCVec_[theIdx]);

      theTripALJOR.add (theOpn, myPeriodCVec_[theIdx], myVolumeCVec_[theIdx]);
      }

   freeWitCVectors ();
   }

//------------------------------------------------------------------------------

void WitJ::PggHandler::storeSubPgg (PeggingTripleALJOR & theTripALJOR)
   {
   int          theIdx;
   Substitute * theSub;

   theTripALJOR.clearAndAllocate (nTriples_);

   for (theIdx = 0; theIdx < nTriples_; theIdx ++)
      {
      theSub =
         myCoordinator_->
            findSubstitute (
               myOpnNameCVec_[theIdx],
               myBomIdxCVec_ [theIdx],
               mySubIdxCVec_ [theIdx]);
               

      theTripALJOR.add (theSub, myPeriodCVec_[theIdx], myVolumeCVec_[theIdx]);
      }

   freeWitCVectors ();
   }

//------------------------------------------------------------------------------

void WitJ::PggHandler::storeBopEntPgg (PeggingTripleALJOR & theTripALJOR)
   {
   int        theIdx;
   BopEntry * theBopEnt;

   theTripALJOR.clearAndAllocate (nTriples_);

   for (theIdx = 0; theIdx < nTriples_; theIdx ++)
      {
      theBopEnt =
         myCoordinator_->
            findBopEntry (myOpnNameCVec_[theIdx], myBopIdxCVec_[theIdx]);

      theTripALJOR.
         add (theBopEnt, myPeriodCVec_[theIdx], myVolumeCVec_[theIdx]);
      }

   freeWitCVectors ();
   }

//------------------------------------------------------------------------------

void WitJ::PggHandler::freeWitCVectors ()
   {
   if (myPartNameCVec_ != NULL)
      freeWitStrVec (myPartNameCVec_, nTriples_);

   if (myOpnNameCVec_  != NULL)
      freeWitStrVec (myOpnNameCVec_,  nTriples_);

   witFree (myBopIdxCVec_);
   witFree (myBomIdxCVec_);
   witFree (mySubIdxCVec_);
   witFree (myPeriodCVec_);
   witFree (myVolumeCVec_);

   myPartNameCVec_ = NULL;
   myOpnNameCVec_  = NULL;
   myBopIdxCVec_   = NULL;
   myBomIdxCVec_   = NULL;
   mySubIdxCVec_   = NULL;
   myPeriodCVec_   = NULL;
   myVolumeCVec_   = NULL;

   nTriples_       = -1;
   }
