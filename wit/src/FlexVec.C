//==============================================================================
// IBM Confidential
//
// OCO Source Materials
//
// Watson Implosion Technology
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//
// The Source code for this program is not published or otherwise divested of
// its trade secrets, irrespective of what has been deposited with the U. S.
// Copyright office.
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "FlexVec.C"
//
// Contains the following:
//
//    The implementation         of class template FlexVec   <Elem>.
//    The explicit instantiation of class template FlexVec   <Elem>.
//    The implementation         of class          DblFlexVec.
//    The implementation         of class          FlexVAsst.
//    The implementation of NonClass functions with arguments belonging to
//        specific Vector classes.
//------------------------------------------------------------------------------

#include <FlexVec.h>
#include <GlobalComp.h>
#include <IVRealArg.h>
#include <ApiMgr.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class template FlexVec <Elem>.
//------------------------------------------------------------------------------

template <typename Elem> 
      WitFlexVec <Elem>::WitFlexVec (WitProblem * theProblem, Elem initValue):

         curAsst_ (NULL)
   {
   curAsst_    = theProblem->scalarFlexVAsst ();

   myScalar () = initValue;
   }

//------------------------------------------------------------------------------

template <typename Elem> 
      WitFlexVec <Elem>::~WitFlexVec ()
   {
   beInScalarMode ();
   }

//------------------------------------------------------------------------------

template <typename Elem> 
      void WitFlexVec <Elem>::switchToVectorMode ()
   {
   Elem      theScalar;
   WitPeriod thePer;

   witAssert (scalarMode ());

   theScalar        = myScalar ();

   curAsst_         = curAsst_->myProblem ()->vectorFlexVAsst ();

   myCVecFromUnion_ = new Elem[nPeriods ()];

   for (thePer = nPeriods () - 1; thePer >= 0; thePer --)
      myCVec ()[thePer] = theScalar;
   }

//------------------------------------------------------------------------------

template <typename Elem> 
      void WitFlexVec <Elem>::switchToScalarMode ()
   {
   Elem theScalar;

   witAssert (vectorMode ());

   theScalar   = myCVec ()[0];

   delete[]      myCVec ();

   curAsst_    = curAsst_->myProblem ()->scalarFlexVAsst ();

   myScalar () = theScalar;
   }

//------------------------------------------------------------------------------

template <typename Elem> 
      void WitFlexVec <Elem>::operator = (const WitFlexVec & theFlexVec)
   {
   witAssert (nPeriods () == theFlexVec.nPeriods ());

   if (theFlexVec.vectorMode ())
      (* this) = theFlexVec.myCVec ();
   else
      (* this) = theFlexVec.myScalar ();
   }

//------------------------------------------------------------------------------

template <typename Elem> 
      void WitFlexVec <Elem>::operator = (Elem theScalar)
   {
   beInScalarMode ();

   myScalar () = theScalar;
   }

//------------------------------------------------------------------------------

template <typename Elem> 
      void WitFlexVec <Elem>::operator = (const WitTVec <Elem> & theTVec)
   {
   witAssert (nPeriods () == theTVec.length ());

   if (theTVec == theTVec[0])
      (* this) = theTVec[0];
   else
      {
      beInVectorMode ();

      theTVec.copyInto (myCVec ());
      }
   }

//------------------------------------------------------------------------------

template <typename Elem> 
      void WitFlexVec <Elem>::operator = (const Elem * theCVec)
   {
   WitPeriod thePer;

   if (hasSingleValue (theCVec, nPeriods ()))
      (* this) = theCVec[0];
   else
      {
      beInVectorMode ();

      for (thePer = nPeriods () - 1; thePer >= 0; thePer --)
         myCVec ()[thePer] = theCVec[thePer];
      }
   }

//------------------------------------------------------------------------------

template <typename Elem> 
      void WitFlexVec <Elem>::copyInto (WitTVec <Elem> & theTVec) const
   {
   if (vectorMode ())
      theTVec = myCVec ();
   else
      theTVec = myScalar ();
   }

//------------------------------------------------------------------------------

template <typename Elem> 
      void WitFlexVec <Elem>::copyInto (Elem * theCVec) const
   {
   WitPeriod thePer;

   witAssert (theCVec != NULL);

   for (thePer = nPeriods () - 1; thePer >= 0; thePer --)
      theCVec[thePer] = (* this)[thePer];
   }

//------------------------------------------------------------------------------

template <typename Elem> 
      bool WitFlexVec <Elem>::operator == (Elem theScalar) const
   {
   WitPeriod thePer;

   if (scalarMode ())
      return (myScalar () == theScalar);

   if (hasSingleValue (myCVec (), nPeriods ()))
      return (myCVec ()[0] == theScalar);
   else
      return false;
   }

//------------------------------------------------------------------------------

template <typename Elem> 
      bool WitFlexVec <Elem>::operator != (Elem theScalar) const
   {
   return not ((* this) == theScalar);
   }

//------------------------------------------------------------------------------

template <typename Elem> 
      bool WitFlexVec <Elem>::operator >= (Elem theScalar) const
   {
   WitPeriod thePer;

   if (scalarMode ())
      return (myScalar () >= theScalar);

   for (thePer = nPeriods () - 1; thePer >= 0; thePer --)
      if (myCVec ()[thePer] < theScalar)
         return false;

   return true;
   }

//------------------------------------------------------------------------------

template <typename Elem> 
      void WitFlexVec <Elem>::checkBounds (WitPeriod thePer) const
   { 
   if (thePer < 0)
      curAsst_->myProblem ()->myMsgFac () ("negativeIndexFmsg", thePer);

   if (thePer >= nPeriods ())
      curAsst_->myProblem ()->myMsgFac () ("indexTooLargeFmsg",
         thePer,
         nPeriods () - 1);
   }

//------------------------------------------------------------------------------
// Explicit instantiation of class template FlexVec <Elem>.
//------------------------------------------------------------------------------

template class WitFlexVec <bool>;
template class WitFlexVec <int>;
template class WitFlexVec <double>;
template class WitFlexVec <void *>;

//------------------------------------------------------------------------------
// Implementation of class DblFlexVec.
//------------------------------------------------------------------------------

WitDblFlexVec::WitDblFlexVec (WitProblem * theProblem, double initValue):

      WitFlexVec <double> (theProblem, initValue)
   {
   }

//------------------------------------------------------------------------------

WitDblFlexVec::~WitDblFlexVec ()
   {
   }

//------------------------------------------------------------------------------

void WitDblFlexVec::operator = (const WitDblFlexVec & theDblFlexVec)
   {
   thisFlexVec () = theDblFlexVec;
   }

//------------------------------------------------------------------------------

void WitDblFlexVec::operator = (double theScalar)
   {
   thisFlexVec () = theScalar;
   }

//------------------------------------------------------------------------------

void WitDblFlexVec::operator = (const WitTVec <double> & theDblTVec)
   {
   thisFlexVec () = theDblTVec;
   }

//------------------------------------------------------------------------------

void WitDblFlexVec::operator = (const double * theDblCVec)
   {
   thisFlexVec () = theDblCVec;
   }

//------------------------------------------------------------------------------

void WitDblFlexVec::operator = (const float * theFltCVec)
   {
   bool      scalMode;
   WitPeriod thePer;

   scalMode = hasSingleValue (theFltCVec, nPeriods ());

   if (scalMode)
      (* this) = static_cast <double> (theFltCVec[0]);
   else
      {
      beInVectorMode ();

      for (thePer = nPeriods () - 1; thePer >= 0; thePer --)
         myCVec ()[thePer] = static_cast <double> (theFltCVec[thePer]);
      }
   }

//------------------------------------------------------------------------------

void WitDblFlexVec::operator = (const WitIVRealArg & theIVRealArg)
   {
   theIVRealArg.copyInto (* this);
   }

//------------------------------------------------------------------------------

void WitDblFlexVec::copyInto (double * theDblCVec) const
   {
   thisFlexVec ().copyInto (theDblCVec);
   }

//------------------------------------------------------------------------------

void WitDblFlexVec::copyInto (float * theFltCVec) const
   {
   WitPeriod thePer;

   witAssert (theFltCVec != NULL);

   for (thePer = nPeriods () - 1; thePer >= 0; thePer --)
      theFltCVec[thePer] = static_cast <float> ((* this)[thePer]);
   }

//------------------------------------------------------------------------------
// Implementation of class FlexVAsst.
//------------------------------------------------------------------------------

WitFlexVAsst::WitFlexVAsst (WitProblem * theProblem, bool vectorModeVal):

      WitProbAssoc (theProblem),
      vectorMode_  (vectorModeVal),
      nPeriods_    (WitGlobalComp::defNPeriods ())
   {
   }

//------------------------------------------------------------------------------

WitFlexVAsst::~WitFlexVAsst ()
   {
   }

//------------------------------------------------------------------------------

void WitFlexVAsst::updateNPeriods ()
   {
   nPeriods_ = myGlobalComp ()->nPeriods ();
   }

//------------------------------------------------------------------------------
// Implementation of NonClass functions with arguments belonging to specific
// Vector classes.
//------------------------------------------------------------------------------

void WitNonClass::copy (
      WitFlexVec <bool> & theFlexVec,
      const WitBoolean *  theCVec)
   {
   WitPeriod thePer;

   if (hasSingleValue (theCVec, theFlexVec.nPeriods ()))
      theFlexVec = asaBool (theCVec[0]);
   else
      for (thePer = theFlexVec.nPeriods () - 1; thePer >= 0; thePer --)
         theFlexVec.elemRef (thePer) = asaBool (theCVec[thePer]);
   }

//------------------------------------------------------------------------------

void WitNonClass::copy (
      WitBoolean *              theCVec,
      const WitFlexVec <bool> & theFlexVec)
   {
   WitPeriod thePer;

   for (thePer = theFlexVec.nPeriods () - 1; thePer >= 0; thePer --)
      theCVec[thePer] = theFlexVec[thePer];
   }
