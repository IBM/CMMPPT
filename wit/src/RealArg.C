//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "RealArg.C"
//
// Contains the implementation of the following classes:
//
//    RealArg
//    ISRealArg
//    OSRealArg
//    IVRealArg
//    OVRealArg
//    OTDARealArg
//------------------------------------------------------------------------------

#include <ISRealArg.h>
#include <OSRealArg.h>
#include <IVRealArg.h>
#include <OVRealArg.h>
#include <OTDARealArg.h>
#include <Vector.h>
#include <WitRun.h>
#include <FlexVec.h>
#include <ApiCall.h>

//------------------------------------------------------------------------------
// Implementation of class RealArg.
//------------------------------------------------------------------------------

WitRealArg::WitRealArg (bool floatCaseVal):

      floatCase_ (floatCaseVal)
   {
   }

//------------------------------------------------------------------------------

WitRealArg::~WitRealArg ()
   {
   }

//------------------------------------------------------------------------------
// Implementation of class ISRealArg.
//------------------------------------------------------------------------------

WitISRealArg::WitISRealArg (float theFloat):

      WitRealArg (true),
      myFlt_     (theFloat),
      myDbl_     (0.0)
   {
   }

//------------------------------------------------------------------------------

WitISRealArg::WitISRealArg (double theDouble):

      WitRealArg (false),
      myFlt_     (0.0),
      myDbl_     (theDouble)
   {
   }

//------------------------------------------------------------------------------

WitISRealArg::~WitISRealArg ()
   {
   }

//------------------------------------------------------------------------------
// Implementation of class OSRealArg.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Inline member functions.
//------------------------------------------------------------------------------

inline float & WitOSRealArg::myFlt () const
   {
   nonMsgAssert (floatCase ());

   return (* myFltPtr_);
   }

//------------------------------------------------------------------------------

inline double & WitOSRealArg::myDbl () const
   {
   nonMsgAssert (doubleCase ());

   return (* myDblPtr_);
   }

//------------------------------------------------------------------------------
// Non-inline member functions.
//------------------------------------------------------------------------------

WitOSRealArg::WitOSRealArg (float * theFloatPtr):

      WitRealArg (true),
      myFltPtr_  (theFloatPtr),
      myDblPtr_  (NULL)
   {
   }

//------------------------------------------------------------------------------

WitOSRealArg::WitOSRealArg (double * theDoublePtr):

      WitRealArg (false),
      myFltPtr_  (NULL),
      myDblPtr_  (theDoublePtr)
   {
   }

//------------------------------------------------------------------------------

WitOSRealArg::~WitOSRealArg ()
   {
   }

//------------------------------------------------------------------------------

void WitOSRealArg::operator = (double theValue) const
   {
   if (floatCase ())
      myFlt () = static_cast <float> (theValue);
   else 
      myDbl () =                      theValue;
   }

//------------------------------------------------------------------------------

WitOSRealArg::operator void * () const
   {
   if (floatCase ())
      return myFltPtr_;
   else
      return myDblPtr_;
   }

//------------------------------------------------------------------------------

double WitOSRealArg::appVarVal () const
   {
   return 
      floatCase ()?
         static_cast <double> (myFlt ()):
                               myDbl ();
   }

//------------------------------------------------------------------------------
// Implementation of class IVRealArg.
//------------------------------------------------------------------------------

WitIVRealArg::WitIVRealArg (const float * theFltCVec):

      WitRealArg (true),
      myFltCVec_ (theFltCVec),
      myDblCVec_ (NULL)
   {
   }

//------------------------------------------------------------------------------

WitIVRealArg::WitIVRealArg (const double * theDblCVec):

      WitRealArg (false),
      myFltCVec_ (NULL),
      myDblCVec_ (theDblCVec)
   {
   }

//------------------------------------------------------------------------------

WitIVRealArg::~WitIVRealArg ()
   {
   }

//------------------------------------------------------------------------------

void WitIVRealArg::copyInto (WitDblFlexVec & theDblFlexVec) const
   {
   if (floatCase ())
      theDblFlexVec = myFltCVec ();
   else
      theDblFlexVec = myDblCVec ();
   }

//------------------------------------------------------------------------------

WitIVRealArg::operator const void * () const
   {
   if (floatCase ())
      return myFltCVec ();
   else
      return myDblCVec ();
   }

//------------------------------------------------------------------------------

bool WitIVRealArg::isNonNeg (int length) const
   {
   int theIdx;

   for (theIdx = 0; theIdx < length; theIdx ++)
      if ((* this)[theIdx] < 0.0)
         return false;

   return true;
   }

//------------------------------------------------------------------------------

void WitIVRealArg::issueSetDblVecMsg (
      const char * theAttName,
      WitApiCall & theApiCall)
      const
   {
   if (floatCase ())
      theApiCall.issueSetDblVecMsg (theAttName, myFltCVec ());
   else
      theApiCall.issueSetDblVecMsg (theAttName, myDblCVec ());
   }

//------------------------------------------------------------------------------

void WitIVRealArg::issueBoundVecMsg (
      WitBoundSet * theBoundSet,
      WitMsgFragID  theFragID,
      WitApiCall &  theApiCall)
      const
   {
   if (floatCase ())
      theApiCall.issueBoundVecMsg (theBoundSet, myFltCVec (), theFragID);
   else
      theApiCall.issueBoundVecMsg (theBoundSet, myDblCVec (), theFragID);
   }

//------------------------------------------------------------------------------
// Implementation of class OVRealArg.
//------------------------------------------------------------------------------

WitOVRealArg::WitOVRealArg (float * * theFltCVecPtr):

      WitRealArg    (true),
      myFltCVecPtr_ (theFltCVecPtr),
      myDblCVecPtr_ (NULL)
   {
   }

//------------------------------------------------------------------------------

WitOVRealArg::WitOVRealArg (double * * theDblCVecPtr):

      WitRealArg    (false),
      myFltCVecPtr_ (NULL),
      myDblCVecPtr_ (theDblCVecPtr)
   {
   }

//------------------------------------------------------------------------------

WitOVRealArg::~WitOVRealArg ()
   {
   }

//------------------------------------------------------------------------------

void WitOVRealArg::apiAlloc (int nElems) const
   {
   if (floatCase ())
      WitNonClass::apiAlloc (myFltCVecPtr_, nElems);
   else
      WitNonClass::apiAlloc (myDblCVecPtr_, nElems);
   }

//------------------------------------------------------------------------------

void WitOVRealArg::operator = (const WitDblFlexVec & theDblFlexVec) const
   {
   if (floatCase ())
      theDblFlexVec.copyInto (myFltCVec ());
   else
      theDblFlexVec.copyInto (myDblCVec ());
   }

//------------------------------------------------------------------------------

void WitOVRealArg::operator = (const WitVector <double> & theDblVec) const
   {
   int theIdx;

   if (floatCase ())
      for (theIdx = 0; theIdx < theDblVec.length (); theIdx ++)
         myFltCVec ()[theIdx] = static_cast <float> (theDblVec[theIdx]);
   else
      theDblVec.copyInto (myDblCVec ());
   }

//------------------------------------------------------------------------------

WitOVRealArg::operator void * () const
   {
   if (floatCase ())
      return myFltCVecPtr_;
   else
      return myDblCVecPtr_;
   }

//------------------------------------------------------------------------------
// Implementation of class OTDARealArg.
//------------------------------------------------------------------------------

WitOTDARealArg::WitOTDARealArg (float * * * theFltArPtr):

      WitRealArg  (true),
      myFltArPtr_ (theFltArPtr),
      myDblArPtr_ (NULL)
   {
   }

//------------------------------------------------------------------------------

WitOTDARealArg::WitOTDARealArg (double * * * theDblArPtr):

      WitRealArg  (false),
      myFltArPtr_ (NULL),
      myDblArPtr_ (theDblArPtr)
   {
   }

//------------------------------------------------------------------------------

WitOTDARealArg::~WitOTDARealArg ()
   {
   }

//------------------------------------------------------------------------------

void WitOTDARealArg::apiAlloc (int nRows, int nCols) const
   {
   int theIdx;

   if (floatCase ())
      WitNonClass::apiAlloc (myFltArPtr_, nRows);
   else
      WitNonClass::apiAlloc (myDblArPtr_, nRows);

   for (theIdx = 0; theIdx < nRows; theIdx ++)
      if (floatCase ())
         WitNonClass::apiAlloc (& ((* myFltArPtr_)[theIdx]), nCols);
      else
         WitNonClass::apiAlloc (& ((* myDblArPtr_)[theIdx]), nCols);
   }

//------------------------------------------------------------------------------

void WitOTDARealArg::copyIntoRow (
      int                   theIdx,
      const WitDblFlexVec & theDblFlexVec)
      const
   {
   if (floatCase ())
      theDblFlexVec.copyInto ((* myFltArPtr_)[theIdx]);
   else
      theDblFlexVec.copyInto ((* myDblArPtr_)[theIdx]);
   }

//------------------------------------------------------------------------------

WitOTDARealArg::operator void * () const
   {
   if (floatCase ())
      return myFltArPtr_;
   else
      return myDblArPtr_;
   }
