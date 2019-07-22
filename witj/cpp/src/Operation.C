//------------------------------------------------------------------------------
// WIT-J C++ Source File Operation.C.
//
// Contains the implementation of class Operation.
//------------------------------------------------------------------------------

#include   <Operation.h>
#include <Coordinator.h>
#include  <PggHandler.h>
#include   <CaseToken.h>
#include    <ArrayJOR.h>

#include         <wit.h>

//------------------------------------------------------------------------------

WitJ::Operation::Operation (
         Coordinator * theCoord,
         const char *  theOpnNameCStr):

      Component        (theCoord),
      myOperationName_ (theOpnNameCStr)
   {
   if (! extractInProgress ())
      witAddOperation (getWitRun (), getOperationName ());

   witSetOperationAppData (getWitRun (), getOperationName (), this);

   theCoord->recordCreatePartOpn ();
   }

//------------------------------------------------------------------------------

WitJ::Operation::~Operation ()
   {
   getCoordinator ()->recordDeletePartOpn ();
   }

//------------------------------------------------------------------------------

WitJ::Operation * WitJ::Operation::asOperation ()
   {
   return this;
   }

//------------------------------------------------------------------------------

void WitJ::Operation::setBoundSet (
      const double * hardLBCVec,
      const double * softLBCVec,
      const double * hardUBCVec)
   {
   witSetOperationExecBoundsDbl (
      getWitRun        (),
      getOperationName (),
      hardLBCVec,
      softLBCVec,
      hardUBCVec);
   }

//------------------------------------------------------------------------------

void WitJ::Operation::getBoundSet (
      DblArrayJOR & hardLBAJOR,
      DblArrayJOR & softLBAJOR,
      DblArrayJOR & hardUBAJOR)
   {
   double * hardLBCVec;
   double * softLBCVec;
   double * hardUBCVec;

   witGetOperationExecBoundsDbl (
      getWitRun        (),
      getOperationName (),
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

void WitJ::Operation::copyComponentData (Component * origComp)
   {
   Operation * origOpn;

   origOpn = origComp->asOperation ();

   witCopyOperationData (
               getWitRun        (),
               getOperationName (),
      origOpn->getWitRun        (),
      origOpn->getOperationName ());
   }

//------------------------------------------------------------------------------

void WitJ::Operation::setOperationName (const char * theOpnNameCStr)
   {
   witSetOperationOperationName (
      getWitRun        (),
      getOperationName (),
      theOpnNameCStr);

   myOperationName_ = theOpnNameCStr;
   }

//------------------------------------------------------------------------------

void WitJ::Operation::getPegging (
      CaseToken *          thePggCaseCT,
      int                  execPer,
      PeggingTripleALJOR & theTripALJOR)
   {
   PggHandler * thePggHandler;
   OpnPggFunc   theFunc;

   thePggHandler = getCoordinator ()->getPggHandler ();

   theFunc       = thePggCaseCT->getOpnPggFunc ();

   (thePggHandler->*theFunc) (this, execPer, theTripALJOR);
   }

//------------------------------------------------------------------------------

const char * WitJ::Operation::getOperationName ()
   {
   return myOperationName_.c_str ();
   }
