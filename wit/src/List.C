//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "List.C"
//
// Contains the implementation the following classes:
//
//    WitAbsList
//    WitListItr
//------------------------------------------------------------------------------

#include <Str.h>
#include <List.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class WitAbsList.
//------------------------------------------------------------------------------

void WitAbsList::clear ()
   {
   myListData_.clear ();

   nElements_ = 0;
   }

//------------------------------------------------------------------------------

WitAbsList::WitAbsList ():

      myListData_ (),
      nElements_  (0)
   {
   }

//------------------------------------------------------------------------------

WitAbsList::~WitAbsList ()
   {
   }

//------------------------------------------------------------------------------

void WitAbsList::copyAbs (const WitAbsList & theAbsList)
   {
   myListData_ = theAbsList.myListData_;
   nElements_  = theAbsList.nElements_;
   }

//------------------------------------------------------------------------------

void WitAbsList::appendAbs (void * theObject)
   {
   myListData_.push_back (theObject);

   nElements_ ++;
   }

//------------------------------------------------------------------------------

void * WitAbsList::firstAbs () const
   {
   return myListData_.front ();
   }

//------------------------------------------------------------------------------

void * WitAbsList::getAbs ()
   {
   void * theObjByVP;

   theObjByVP = myListData_.front ();

   myListData_.pop_front ();

   if (nElements_ > 0)
      nElements_ --;

   return theObjByVP;
   }

//------------------------------------------------------------------------------

void * WitAbsList::lastAbs () const
   {
   return myListData_.back ();
   }

//------------------------------------------------------------------------------

void WitAbsList::prependAbs (void * theObjByVP)
   {
   witAssert (theObjByVP != NULL);

   myListData_.push_front (theObjByVP);

   nElements_ ++;
   }

//------------------------------------------------------------------------------

void * WitAbsList::atAbs (int theIndex) const
   {
   std::list <void *>::const_iterator theItr = myListData_.begin ();

   while (theIndex -- and theItr != myListData_.end ())
      theItr ++;

   return
      (theItr != myListData_.end ())?
         * theItr:
         NULL;
   }

//------------------------------------------------------------------------------

void * WitAbsList::findAbs (
      WitAbsListKeyFunc theFunc,
      const char *      theCstring)
      const
   {
   WitListItr theItr (* this);
   void *     theElement;

   while (theItr.advance (theElement, * this))
      if (theFunc (theElement) == theCstring)
         return theElement;

   return NULL;
   }

//------------------------------------------------------------------------------

void WitAbsList::appendUniquelyAbs (void * theObject)
   {
   witAssert (not containsAbs (theObject));

   appendAbs (theObject);
   }

//------------------------------------------------------------------------------

bool WitAbsList::containsAbs (void * theObject) const
   {
   std::list<void*>::const_iterator theItr = myListData_.begin ();

   while (theItr != myListData_.end ())
      {
      if (* theItr == theObject)
         return true;

      ++ theItr;
      } 

   return false;
   }

//------------------------------------------------------------------------------
// Implementation of class ListItr.
//------------------------------------------------------------------------------

WitListItr::WitListItr (const WitAbsList & theAbsList):
      myAbsList_ (theAbsList),
      iterating_ (false)
   {
   myListIterator_ = myAbsList_.myListData_.begin ();
   }

//------------------------------------------------------------------------------

WitListItr::~WitListItr ()
   {
   }

//------------------------------------------------------------------------------

void * WitListItr::key ()
   {
   return * myListIterator_;
   }

//------------------------------------------------------------------------------

bool WitListItr::advance ()
   {
   if (iterating_)
      myListIterator_++;
   else
      iterating_ = true;

   return (myListIterator_ != myAbsList_.myListData_.end ());
   }

//------------------------------------------------------------------------------

bool WitListItr::advance (
      void * &           theElement,
      const WitAbsList & theAbsList)
   {
   witAssert (& myAbsList_.myListData_ == & theAbsList.myListData_);
      //
      // Require that this Itr is attached to theAbsList.

   bool gotOne = advance ();

   if (gotOne)
      theElement = * myListIterator_;

   return gotOne;
   }
