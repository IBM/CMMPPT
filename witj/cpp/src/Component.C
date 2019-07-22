//------------------------------------------------------------------------------
// WIT-J C++ Source File Component.C.
//
// Contains the implementation of class Component.
//------------------------------------------------------------------------------

#include    <Component.h>
#include <ComponentJOR.h>
#include  <Coordinator.h>
#include      <Problem.h>
#include    <CaseToken.h>
#include  <CTVecSupply.h>

//------------------------------------------------------------------------------

WitJ::Component::~Component ()
   {
   delete myComponentJOR_;
   }

//------------------------------------------------------------------------------

WitJ::Component * WitJ::Component::asComponent ()
   {
   return this;
   }

//------------------------------------------------------------------------------

void WitJ::Component::setComponentJOR (ComponentJOR * theComponentJOR)
   {
   EXCEPTION_ASSERT (myComponentJOR_ == NULL);

   myComponentJOR_ = theComponentJOR;
   }

//------------------------------------------------------------------------------

void WitJ::Component::setBoundSet (
      const double *,
      const double *,
      const double *)
   {
   EXCEPTION_ASSERT (false);
   }

//------------------------------------------------------------------------------

void WitJ::Component::setBoundSetToScalars (
      double hardLBVal,
      double softLBVal,
      double hardUBVal)
   {
   double * hardLBCVec;
   double * softLBCVec;
   double * hardUBCVec;
   int      nPers;
   int      thePer;

   getCTVecSupply ()->provideCTVec (hardLBCVec);
   getCTVecSupply ()->provideCTVec (softLBCVec);
   getCTVecSupply ()->provideCTVec (hardUBCVec);

   nPers = getCoordinator ()->getProblem ()->getNPeriods ();

   for (thePer = 0; thePer < nPers; thePer ++)
      {
      hardLBCVec[thePer] = hardLBVal;
      softLBCVec[thePer] = softLBVal;
      hardUBCVec[thePer] = hardUBVal;
      }

   setBoundSet (hardLBCVec, softLBCVec, hardUBCVec);

   getCTVecSupply ()->reclaimCTVec (hardLBCVec);
   getCTVecSupply ()->reclaimCTVec (softLBCVec);
   getCTVecSupply ()->reclaimCTVec (hardUBCVec);
   }

//------------------------------------------------------------------------------

void WitJ::Component::getBoundSet (
      DblArrayJOR & hardLBAJOR,
      DblArrayJOR & softLBAJOR,
      DblArrayJOR & hardUBAJOR)
   {
   }

//------------------------------------------------------------------------------

void WitJ::Component::setWasDeletedInWit (bool theBool)
   {
   wasDeletedInWit_ = theBool;
   }

//------------------------------------------------------------------------------

void WitJ::Component::getPegging (CaseToken *, int, PeggingTripleALJOR &)
   {
   EXCEPTION_ASSERT (false);
   }

//------------------------------------------------------------------------------

WitJ::Component::Component (Coordinator * theCoord):

      JavaAccObj       (theCoord),
      myComponentJOR_  (NULL),
      wasDeletedInWit_ (false)
   {
   }
