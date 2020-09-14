//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "Repos.C"
//
// Contains the implementation of the following classes and templates:
//
//    DataRepos
//    VarRepos <Variable>
//    AttRepos <Object>
//------------------------------------------------------------------------------

#include <DataRepos.h>
#include <VarRepos.h>
#include <AttRepos.h>
#include <Operation.h>
#include <SubEntry.h>

//------------------------------------------------------------------------------
// Implementation of class DataRepos.
//------------------------------------------------------------------------------

WitDataRepos::WitDataRepos (WitProblem * theProblem):

      WitProbAssoc  (theProblem),
      myIntRepos_   (NULL),
      myBoolRepos_  (NULL),
      myDblRepos_   (NULL),
      myPtrRepos_   (NULL),
      execVolRepos_ (NULL),
      subVolRepos_  (NULL)
   {
   myIntRepos_   = new WitVarRepos <int>    (theProblem);
   myBoolRepos_  = new WitVarRepos <bool>   (theProblem);
   myDblRepos_   = new WitVarRepos <double> (theProblem);
   myPtrRepos_   = new WitVarRepos <void *> (theProblem);

   execVolRepos_ =
      new WitAttRepos <WitOperation> (theProblem,
                     & WitOperation:: execVol,
                     & WitOperation:: storeHeurExecVol);

   subVolRepos_ =
      new WitAttRepos  <WitSubEntry> (theProblem,
                      & WitSubEntry:: subVol,
                      & WitSubEntry:: storeHeurSubVol);
   }

//------------------------------------------------------------------------------

WitDataRepos::~WitDataRepos ()
   {
   delete subVolRepos_;
   delete execVolRepos_;
   delete myPtrRepos_;
   delete myDblRepos_;
   delete myBoolRepos_;
   delete myIntRepos_;
   }

//------------------------------------------------------------------------------

void WitDataRepos::saveVal (int & theVar)
   {
   myIntRepos_->saveVal (theVar);
   }

//------------------------------------------------------------------------------

void WitDataRepos::saveVal (bool & theVar)
   {
   myBoolRepos_->saveVal (theVar);
   }

//------------------------------------------------------------------------------

void WitDataRepos::saveVal (double & theVar)
   {
   myDblRepos_->saveVal (theVar);
   }

//------------------------------------------------------------------------------

void WitDataRepos::saveExecVol (WitOperation * theOpn, WitPeriod thePer)
   {
   execVolRepos_->saveValAt (theOpn, thePer);
   }

//------------------------------------------------------------------------------

void WitDataRepos::saveSubVol (WitSubEntry * theSub, WitPeriod thePer)
   {
   subVolRepos_->saveValAt (theSub, thePer);
   }

//------------------------------------------------------------------------------

void WitDataRepos::restore ()
   {
   myIntRepos_  ->restore ();
   myBoolRepos_ ->restore ();
   myDblRepos_  ->restore ();
   myPtrRepos_  ->restore ();
   execVolRepos_->restore ();
   subVolRepos_ ->restore ();
   }

//------------------------------------------------------------------------------

void WitDataRepos::restoreAndSave (WitDataRepos * theDataRepos)
   {
   myIntRepos_  ->restoreAndSave (theDataRepos->myIntRepos_);
   myBoolRepos_ ->restoreAndSave (theDataRepos->myBoolRepos_);
   myDblRepos_  ->restoreAndSave (theDataRepos->myDblRepos_);
   myPtrRepos_  ->restoreAndSave (theDataRepos->myPtrRepos_);
   execVolRepos_->restoreAndSave (theDataRepos->execVolRepos_);
   subVolRepos_ ->restoreAndSave (theDataRepos->subVolRepos_);
   }

//------------------------------------------------------------------------------

void WitDataRepos::clear ()
   {
   myIntRepos_  ->clear ();
   myBoolRepos_ ->clear ();
   myDblRepos_  ->clear ();
   myPtrRepos_  ->clear ();
   execVolRepos_->clear ();
   subVolRepos_ ->clear ();
   }

//------------------------------------------------------------------------------

bool WitDataRepos::isEmpty ()
   {
   return
          myIntRepos_ ->isEmpty ()
      and myBoolRepos_->isEmpty ()
      and myDblRepos_ ->isEmpty ()
      and myPtrRepos_ ->isEmpty ();
   }

//------------------------------------------------------------------------------

void WitDataRepos::savePtrVal (void * & theVoidPtr)
   {
   myPtrRepos_->saveVal (theVoidPtr);
   }

//------------------------------------------------------------------------------
// Implementation of class template VarRepos <Variable>.
//------------------------------------------------------------------------------

template <typename Variable>
      WitVarRepos <Variable>::WitVarRepos (WitProblem * theProblem):

         WitProbAssoc (theProblem),
         savedPairs_  (theProblem)
   {
   }

//------------------------------------------------------------------------------

template <typename Variable>
      WitVarRepos <Variable>::~WitVarRepos ()
   {
   }

//------------------------------------------------------------------------------

template <typename Variable>
      void WitVarRepos <Variable>::saveVal (Variable & theVar)
   {
   savedPairs_.push (& theVar, theVar);
   }

//------------------------------------------------------------------------------
    
template <typename Variable>
      void WitVarRepos <Variable>::restore ()
   {
   Variable * theVarPtr;
   Variable   theVal;

   while (savedPairs_.pop (theVarPtr, theVal))
      * theVarPtr = theVal;
   }

//------------------------------------------------------------------------------
    
template <typename Variable>
      void WitVarRepos <Variable>::restoreAndSave (WitVarRepos * theVarRepos)
   {
   Variable * theVarPtr;
   Variable   theVal;

   theVarRepos->clear ();

   while (savedPairs_.pop (theVarPtr, theVal))
      {
      theVarRepos->saveVal (* theVarPtr);

      * theVarPtr = theVal;
      }
   }

//------------------------------------------------------------------------------

template <typename Variable>
      void WitVarRepos <Variable>::clear ()
   {
   savedPairs_.clear ();
   }

//------------------------------------------------------------------------------
// Implementation of class template AttRepos <Object>.
//------------------------------------------------------------------------------

template <typename Object>
      WitAttRepos <Object>::WitAttRepos (
            WitProblem *                     theProblem,
            const WitDblFlexVec & (Object::* theAccFunc)   () const,
            void                  (Object::* theStoreFunc) (WitPeriod, double)):

         WitProbAssoc (theProblem),
         savedTrips_  (theProblem),
         myAccFunc_   (theAccFunc),
         myStoreFunc_ (theStoreFunc)
   {
   }

//------------------------------------------------------------------------------

template <typename Object>
      WitAttRepos <Object>::~WitAttRepos ()
   {
   }

//------------------------------------------------------------------------------

template <typename Object>
      void WitAttRepos <Object>::saveValAt (
         Object *  theObject,
         WitPeriod thePer)
   {
   double theValue;

   theValue = (theObject->*myAccFunc_) ()[thePer]; 

   savedTrips_.push (theObject, thePer, theValue);
   }

//------------------------------------------------------------------------------

template <typename Object>
      void WitAttRepos <Object>::restore ()
   {
   Object *  theObject;
   WitPeriod thePer;
   double    theValue;

   while (savedTrips_.pop (theObject, thePer, theValue))
      (theObject->*myStoreFunc_) (thePer, theValue);
   }

//------------------------------------------------------------------------------

template <typename Object>
      void WitAttRepos <Object>::restoreAndSave (WitAttRepos * theAttRepos)
   {
   Object *  theObject;
   WitPeriod thePer;
   double    theValue;

   witAssert (theAttRepos->myAccFunc_   == myAccFunc_);
   witAssert (theAttRepos->myStoreFunc_ == myStoreFunc_);

   theAttRepos->clear ();

   while (savedTrips_.pop (theObject, thePer, theValue))
      {
      theAttRepos->saveValAt (theObject, thePer);

      (theObject->*myStoreFunc_) (thePer, theValue);
      }
   }

//------------------------------------------------------------------------------

template <typename Object>
      void WitAttRepos <Object>::clear ()
   {
   savedTrips_.clear ();
   }
