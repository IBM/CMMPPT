//------------------------------------------------------------------------------
// WIT-J C++ Source File CTVecSupply.C.
//
// Contains the implementation of class CTVecSupply.
//------------------------------------------------------------------------------

#include <CTVecSupply.h>

//------------------------------------------------------------------------------
// Definition of class CTVecSupply.
//------------------------------------------------------------------------------

WitJ::CTVecSupply::CTVecSupply (int nPeriodsVal):

      nPeriods_    (nPeriodsVal),
      myIntCTVecs_ (),
      myDblCTVecs_ ()
   {
   }

//------------------------------------------------------------------------------

WitJ::CTVecSupply::~CTVecSupply ()
   {
   while (not myIntCTVecs_.empty ())
      {
      delete myIntCTVecs_.back ();

      myIntCTVecs_.pop_back ();
      }

   while (not myDblCTVecs_.empty ())
      {
      delete myDblCTVecs_.back ();

      myDblCTVecs_.pop_back ();
      }
   }

//------------------------------------------------------------------------------

void WitJ::CTVecSupply::provideCTVec (int * & theIntCTVec)
   {
   if (myIntCTVecs_.empty ())
      {
      theIntCTVec = new int[nPeriods_];
      }
   else
      {
      theIntCTVec = myIntCTVecs_.back ();

      myIntCTVecs_.pop_back ();
      }
   }

//------------------------------------------------------------------------------

void WitJ::CTVecSupply::provideCTVec (double * & theDblCTVec)
   {
   if (myDblCTVecs_.empty ())
      {
      theDblCTVec = new double[nPeriods_];
      }
   else
      {
      theDblCTVec = myDblCTVecs_.back ();

      myDblCTVecs_.pop_back ();
      }
   }

//------------------------------------------------------------------------------

void WitJ::CTVecSupply::reclaimCTVec (int * & theIntCTVec)
   {
   myIntCTVecs_.push_back (theIntCTVec);

   theIntCTVec = NULL;
   }

//------------------------------------------------------------------------------

void WitJ::CTVecSupply::reclaimCTVec (double * & theDblCTVec)
   {
   myDblCTVecs_.push_back (theDblCTVec);

   theDblCTVec = NULL;
   }
