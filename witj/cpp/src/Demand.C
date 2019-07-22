//------------------------------------------------------------------------------
// WIT-J C++ Source File Demand.C.
//
// Contains the implementation of class Demand.
//------------------------------------------------------------------------------

#include       <Demand.h>
#include         <Part.h>
#include    <Operation.h>
#include   <Substitute.h>
#include     <BopEntry.h>
#include  <Coordinator.h>
#include    <CaseToken.h>
#include   <PggHandler.h>
#include     <ArrayJOR.h>
#include <ArrayListJOR.h>

#include          <wit.h>

//------------------------------------------------------------------------------
// Non-Inline member functions.
//------------------------------------------------------------------------------

WitJ::Demand::Demand (Part * thePart, const char * theDemandNameCStr):

      Component     (thePart->getCoordinator ()),
      myPart_       (thePart),
      myDemandName_ (theDemandNameCStr)
   {
   if (! extractInProgress ())
      witAddDemand (getWitRun (), getPartName (), getDemandName ());

   witSetDemandAppData (getWitRun (), getPartName (), getDemandName (), this);
   }

//------------------------------------------------------------------------------

WitJ::Demand::~Demand ()
   {
   }

//------------------------------------------------------------------------------

WitJ::Demand * WitJ::Demand::asDemand ()
   {
   return this;
   }

//------------------------------------------------------------------------------

void WitJ::Demand::setBoundSet (
      const double * hardLBCVec,
      const double * softLBCVec,
      const double * hardUBCVec)
   {
   witSetDemandCumShipBoundsDbl (
      getWitRun     (),
      getPartName   (),
      getDemandName (),
      hardLBCVec,
      softLBCVec,
      hardUBCVec);
   }

//------------------------------------------------------------------------------

void WitJ::Demand::getBoundSet (
      DblArrayJOR & hardLBAJOR,
      DblArrayJOR & softLBAJOR,
      DblArrayJOR & hardUBAJOR)
   {
   double * hardLBCVec;
   double * softLBCVec;
   double * hardUBCVec;

   witGetDemandCumShipBoundsDbl (
      getWitRun     (),
      getPartName   (),
      getDemandName (),
    & hardLBCVec,
    & softLBCVec,
    & hardUBCVec);

   hardLBAJOR.copyFrom (hardLBCVec);
   softLBAJOR.copyFrom (softLBCVec);
   hardUBAJOR.copyFrom (hardUBCVec);

   witFree (hardLBCVec);
   witFree (softLBCVec);
   witFree (hardUBCVec);
   }

//------------------------------------------------------------------------------

void WitJ::Demand::copyComponentData (Component * origComp)
   {
   Demand * origDemand;

   origDemand = origComp->asDemand ();

   witCopyDemandData (
                  getWitRun     (),
                  getPartName   (),
                  getDemandName (),
      origDemand->getWitRun     (),
      origDemand->getPartName   (),
      origDemand->getDemandName ());
   }

//------------------------------------------------------------------------------

void WitJ::Demand::appendToPipSeq (int theShipPer, double incShipVol)
   {
   witAppendToPipSeqDbl (
      getWitRun     (),
      getPartName   (),
      getDemandName (),
      theShipPer,
      incShipVol);
   }

//------------------------------------------------------------------------------

void WitJ::Demand::setDemandName (const char * theDemandNameCStr)
   {
   witSetDemandDemandName (
      getWitRun     (),
      getPartName   (),
      getDemandName (),
      theDemandNameCStr);

   myDemandName_ = theDemandNameCStr;
   }

//------------------------------------------------------------------------------

void WitJ::Demand::getPegging (
      CaseToken *          thePggCaseCT,
      int                  shipPer,
      PeggingTripleALJOR & theTripALJOR)
   {
   PggHandler * thePggHandler;
   DemPggFunc   theFunc;

   thePggHandler = getCoordinator ()->getPggHandler ();

   theFunc       = thePggCaseCT->getDemPggFunc ();

   (thePggHandler->*theFunc) (this, shipPer, theTripALJOR);
   }

//------------------------------------------------------------------------------

const char * WitJ::Demand::getPartName ()
   {
   return myPart_->getPartName ();
   }

//------------------------------------------------------------------------------

double WitJ::Demand::incHeurAlloc (int shipPer, double desIncVol)
   {
   double incVol;

   witIncHeurAllocDbl (
      getWitRun     (),
      getPartName   (),
      getDemandName (),
      shipPer,
      desIncVol,
    & incVol);

   return incVol;
   }

//------------------------------------------------------------------------------

const char * WitJ::Demand::getDemandName ()
   {
   return myDemandName_.c_str ();
   }
