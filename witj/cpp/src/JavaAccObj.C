//------------------------------------------------------------------------------
// WIT-J C++ Source File JavaAccObj.C.
//
// Contains the implementation of class JavaAccObj.
//------------------------------------------------------------------------------

#include <Coordinator.h>

//------------------------------------------------------------------------------

const int WitJ::JavaAccObj::validMark_ = 1234567890;

//------------------------------------------------------------------------------

WitJ::JavaAccObj::~JavaAccObj ()
   {
   validityMarker_ = 0;
   }

//------------------------------------------------------------------------------

WitJ::Coordinator * WitJ::JavaAccObj::asCoordinator ()
   {
   EXCEPTION_ASSERT (false);

   return NULL;
   }

//------------------------------------------------------------------------------

WitJ::Problem * WitJ::JavaAccObj::asProblem ()
   {
   EXCEPTION_ASSERT (false);

   return NULL;
   }

//------------------------------------------------------------------------------

WitJ::AttBldr * WitJ::JavaAccObj::asAttBldr ()
   {
   EXCEPTION_ASSERT (false);

   return NULL;
   }

//------------------------------------------------------------------------------

WitJ::Att * WitJ::JavaAccObj::asAtt ()
   {
   EXCEPTION_ASSERT (false);

   return NULL;
   }

//------------------------------------------------------------------------------

WitJ::MessageMgr * WitJ::JavaAccObj::asMessageMgr ()
   {
   EXCEPTION_ASSERT (false);

   return NULL;
   }

//------------------------------------------------------------------------------

WitJ::Component * WitJ::JavaAccObj::asComponent ()
   {
   EXCEPTION_ASSERT (false);

   return NULL;
   }

//------------------------------------------------------------------------------

WitJ::Part * WitJ::JavaAccObj::asPart ()
   {
   EXCEPTION_ASSERT (false);

   return NULL;
   }

//------------------------------------------------------------------------------

WitJ::Demand * WitJ::JavaAccObj::asDemand ()
   {
   EXCEPTION_ASSERT (false);

   return NULL;
   }

//------------------------------------------------------------------------------

WitJ::Operation * WitJ::JavaAccObj::asOperation ()
   {
   EXCEPTION_ASSERT (false);

   return NULL;
   }

//------------------------------------------------------------------------------

WitJ::BomEntry * WitJ::JavaAccObj::asBomEntry ()
   {
   EXCEPTION_ASSERT (false);

   return NULL;
   }

//------------------------------------------------------------------------------

WitJ::Substitute * WitJ::JavaAccObj::asSubstitute ()
   {
   EXCEPTION_ASSERT (false);

   return NULL;
   }

//------------------------------------------------------------------------------

WitJ::BopEntry * WitJ::JavaAccObj::asBopEntry ()
   {
   EXCEPTION_ASSERT (false);

   return NULL;
   }

//------------------------------------------------------------------------------

WitJ::CaseToken * WitJ::JavaAccObj::asCaseToken ()
   {
   EXCEPTION_ASSERT (false);

   return NULL;
   }

//------------------------------------------------------------------------------

WitRun * WitJ::JavaAccObj::getWitRun ()
   {
   return myCoordinator_->getWitRun ();
   }

//------------------------------------------------------------------------------

WitJ::CTVecSupply * WitJ::JavaAccObj::getCTVecSupply ()
   {
   return myCoordinator_->getCTVecSupply ();
   }

//------------------------------------------------------------------------------

bool WitJ::JavaAccObj::extractInProgress ()
   {
   return getCoordinator ()->extractInProgress ();
   }

//------------------------------------------------------------------------------

WitJ::JavaAccObj::JavaAccObj (Coordinator * theCoord):

      myCoordinator_  (theCoord),
      validityMarker_ (validMark_)
   {
   EXCEPTION_ASSERT (theCoord != NULL);
   }
