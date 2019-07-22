//------------------------------------------------------------------------------
// WIT-J C++ Source File BopEntry.C.
//
// Contains the implementation of class BopEntry.
//------------------------------------------------------------------------------

#include    <BopEntry.h>
#include        <Part.h>
#include   <Operation.h>
#include <Coordinator.h>

#include         <wit.h>

//------------------------------------------------------------------------------

WitJ::BopEntry::BopEntry (
         Operation * theOpn, 
         int         theBopIdx,
         Part *      thePart):

      Component   (theOpn->getCoordinator ()),
      myOpn_      (theOpn),
      myBopIndex_ (theBopIdx)
   {
   if (! extractInProgress ())
      witAddBopEntry (
         getWitRun (),
         getOperationName (),
         thePart->getPartName ());

   witSetBopEntryAppData (getWitRun (), getOperationName (), myBopIndex_, this);
   }

//------------------------------------------------------------------------------

WitJ::BopEntry::~BopEntry ()
   {
   }

//------------------------------------------------------------------------------

WitJ::BopEntry * WitJ::BopEntry::asBopEntry ()
   {
   return this;
   }

//------------------------------------------------------------------------------

void WitJ::BopEntry::copyComponentData (Component * origComp)
   {
   BopEntry * origBopEnt;

   origBopEnt = origComp->asBopEntry ();

   witCopyBopEntryData (
                  getWitRun        (),
                  getOperationName (),
                  myBopIndex_,
      origBopEnt->getWitRun        (),
      origBopEnt->getOperationName (),
      origBopEnt->myBopIndex_);
   }

//------------------------------------------------------------------------------

const char * WitJ::BopEntry::getOperationName ()
   {
   return myOpn_->getOperationName ();
   }

//------------------------------------------------------------------------------

void WitJ::BopEntry::setBopIndex (int theBopIdx)
   {
   myBopIndex_ = theBopIdx;
   }
