//------------------------------------------------------------------------------
// WIT-J C++ Source File Substitute.C.
//
// Contains the implementation of class Substitute.
//------------------------------------------------------------------------------

#include  <Substitute.h>
#include    <BomEntry.h>
#include        <Part.h>
#include <Coordinator.h>

#include         <wit.h>

//------------------------------------------------------------------------------

WitJ::Substitute::Substitute (
         BomEntry *   theBomEnt,
         int          theSubIdx,
         Part *       thePart):

      Component   (theBomEnt->getCoordinator ()),
      myBomEnt_   (theBomEnt),
      mySubIndex_ (theSubIdx)
   {
   if (! extractInProgress ())
      witAddSubsBomEntry (
         getWitRun (),
         getOperationName (),
         getBomIndex (),
         thePart->getPartName ());

   witSetSubsBomEntryAppData (
      getWitRun (),
      getOperationName (),
      getBomIndex (),
      mySubIndex_,
      this);
   }

//------------------------------------------------------------------------------

WitJ::Substitute::~Substitute ()
   {
   }

//------------------------------------------------------------------------------

WitJ::Substitute * WitJ::Substitute::asSubstitute ()
   {
   return this;
   }

//------------------------------------------------------------------------------

void WitJ::Substitute::copyComponentData (Component * origComp)
   {
   Substitute * origSub;

   origSub = origComp->asSubstitute ();

   witCopySubsBomEntryData (
               getWitRun        (),
               getOperationName (),
               getBomIndex      (),
               mySubIndex_,
      origSub->getWitRun        (),
      origSub->getOperationName (),
      origSub->getBomIndex      (),
      origSub->mySubIndex_);
   }

//------------------------------------------------------------------------------

const char * WitJ::Substitute::getOperationName ()
   {
   return myBomEnt_->getOperationName ();
   }

//------------------------------------------------------------------------------

int WitJ::Substitute::getBomIndex ()
   {
   return myBomEnt_->getBomIndex ();
   }

//------------------------------------------------------------------------------

void WitJ::Substitute::setSubIndex (int theSubIdx)
   {
   mySubIndex_ = theSubIdx;
   }
