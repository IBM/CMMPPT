//------------------------------------------------------------------------------
// WIT-J C++ Source File Part.C.
//
// Contains the implementation of class Part.
//------------------------------------------------------------------------------

#include        <Part.h>
#include <Coordinator.h>
#include   <CaseToken.h>
#include    <ArrayJOR.h>

//------------------------------------------------------------------------------

WitJ::Part::Part (
         Coordinator * theCoord,
         const char *  thePartNameCStr,
         CaseToken *   thePartCatCT):

      Component   (theCoord),
      myPartName_ (thePartNameCStr)
   {
   witAttr thePartCat;

   if (! extractInProgress ())
      {
      thePartCat = thePartCatCT->getWitAttr ();

      witAddPart (getWitRun (), getPartName (), thePartCat);
      }

   witSetPartAppData (getWitRun (), getPartName (), this);

   theCoord->recordCreatePartOpn ();
   }

//------------------------------------------------------------------------------

WitJ::Part::~Part ()
   {
   getCoordinator ()->recordDeletePartOpn ();
   }

//------------------------------------------------------------------------------

WitJ::Part * WitJ::Part::asPart ()
   {
   return this;
   }

//------------------------------------------------------------------------------

void WitJ::Part::getBelowList (CompALJOR <Part> & theBelowALJOR)
   {
   int      lenList;
   char * * partNameList;

   witGetPartBelowList (
      getWitRun (),
      getPartName (),
    & lenList,
    & partNameList);

   getCoordinator ()->populate (theBelowALJOR, partNameList, lenList);

   freeWitStrVec (partNameList, lenList);
   }

//------------------------------------------------------------------------------

void WitJ::Part::setBoundSet (
      const double * hardLBCVec,
      const double * softLBCVec,
      const double * hardUBCVec)
   {
   witSetPartStockBoundsDbl (
      getWitRun   (),
      getPartName (),
      hardLBCVec,
      softLBCVec,
      hardUBCVec);
   }

//------------------------------------------------------------------------------

void WitJ::Part::getBoundSet (
      DblArrayJOR & hardLBAJOR,
      DblArrayJOR & softLBAJOR,
      DblArrayJOR & hardUBAJOR)
   {
   double * hardLBCVec;
   double * softLBCVec;
   double * hardUBCVec;

   witGetPartStockBoundsDbl (
      getWitRun   (),
      getPartName (),
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

void WitJ::Part::copyComponentData (Component * origComp)
   {
   Part * origPart;

   origPart = origComp->asPart ();

   witCopyPartData (
                getWitRun   (),
                getPartName (),
      origPart->getWitRun   (),
      origPart->getPartName ());
   }

//------------------------------------------------------------------------------

void WitJ::Part::setPartName (const char * thePartNameCStr)
   {
   witSetPartPartName (
      getWitRun (),
      getPartName (),
      thePartNameCStr);

   myPartName_ = thePartNameCStr;
   }

//------------------------------------------------------------------------------

const char * WitJ::Part::getPartName ()
   {
   return myPartName_.c_str ();
   }
