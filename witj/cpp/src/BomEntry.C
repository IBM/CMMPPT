//------------------------------------------------------------------------------
// WIT-J C++ Source File BomEntry.C.
//
// Contains the implementation of class BomEntry.
//------------------------------------------------------------------------------

#include    <BomEntry.h>
#include        <Part.h>
#include   <Operation.h>
#include <Coordinator.h>

#include        <wit.h>

//------------------------------------------------------------------------------

WitJ::BomEntry::BomEntry (
         Operation * theOpn, 
         int         theBomIdx,
         Part *      thePart):

      Component   (theOpn->getCoordinator ()),
      myOpn_      (theOpn),
      myBomIndex_ (theBomIdx)
   {
   if (! extractInProgress ())
      witAddBomEntry (
         getWitRun (),
         getOperationName (),
         thePart->getPartName ());

   witSetBomEntryAppData (getWitRun (), getOperationName (), myBomIndex_, this);
   }

//------------------------------------------------------------------------------

WitJ::BomEntry::~BomEntry ()
   {
   }

//------------------------------------------------------------------------------

WitJ::BomEntry * WitJ::BomEntry::asBomEntry ()
   {
   return this;
   }

//------------------------------------------------------------------------------

void WitJ::BomEntry::copyComponentData (Component * origComp)
   {
   BomEntry * origBomEnt;

   origBomEnt = origComp->asBomEntry ();

   witCopyBomEntryData (
                  getWitRun        (),
                  getOperationName (),
                  myBomIndex_,
      origBomEnt->getWitRun        (),
      origBomEnt->getOperationName (),
      origBomEnt->myBomIndex_);
   }

//------------------------------------------------------------------------------

const char * WitJ::BomEntry::getOperationName ()
   {
   return myOpn_->getOperationName ();
   }

//------------------------------------------------------------------------------

void WitJ::BomEntry::setBomIndex (int theBomIdx)
   {
   myBomIndex_ = theBomIdx;
   }
