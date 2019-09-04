//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "Vecs.C"
//               
// Contains the implementations of the following:
//
//    Class String
//    Class Template TVec   <Elem>, with explicit instantiation.
//    Class Template Vector <Elem>, with explicit instantiation.
//    NonClass functions with arguments belonging to specific Vector classes.
//------------------------------------------------------------------------------

#include <Str.h>
#include <List.h>
#include <Problem.h>
#include <FlexVec.h>
#include <WitRun.h>
#include <GlobalComp.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class String.
//------------------------------------------------------------------------------

WitString::WitString ():
      myVector_ ()
   {
   operator = ("");
   }

//------------------------------------------------------------------------------

WitString::WitString (const char * theCstring):
      myVector_ ()
   {
   operator = (theCstring);
   }

//------------------------------------------------------------------------------

WitString::WitString (int nBlanks):
      myVector_ ()
   {
   resize (nBlanks);
   }

//------------------------------------------------------------------------------

WitString::WitString (const WitString & theString):
      myVector_ ()
   {
   operator = (theString.myCstring ());
   }

//------------------------------------------------------------------------------

WitString::~WitString ()
   {
   }

//------------------------------------------------------------------------------

void WitString::operator = (const char * theCstring)
   {
   myVector_.resize (strlen (theCstring) + 1);

   strcpy (myCstringForUpdate (), theCstring);
   }

//------------------------------------------------------------------------------

bool WitString::operator < (const char * theCstring) const
   {
   return (strcmp (myCstring (), theCstring) < 0);
   }

//------------------------------------------------------------------------------

void WitString::resize (int nChars)
   {
   myVector_.resize (nChars + 1, ' ');

   (* this)[static_cast <int> (nChars)] = '\0';
   }

//------------------------------------------------------------------------------

char * WitString::apiDupCstring () const
   {
   return dupCString (myCstring ());
   }

//------------------------------------------------------------------------------

void WitString::copyInto (char * theCstring) const
   {
   strcpy (theCstring, myCstring ());
   }

//------------------------------------------------------------------------------
// Implementation of class template TVec <Elem>
//------------------------------------------------------------------------------

template <typename Elem> 
      WitTVec <Elem>::WitTVec ():
         WitVector <Elem> ()
   {
   }

//------------------------------------------------------------------------------

template <typename Elem> 
      WitTVec <Elem>::WitTVec (WitProblem * theProblem, Elem initValue):
         WitVector <Elem> ()
   {
   allocate (theProblem, initValue);
   }

//------------------------------------------------------------------------------

template <typename Elem> 
      WitTVec <Elem>::~WitTVec ()
   {
   }

//------------------------------------------------------------------------------

template <typename Elem>
      void WitTVec <Elem>::operator = (
         const WitFlexVec <Elem> & theFlexVec)
   {
   theFlexVec.copyInto (* this);
   }

//------------------------------------------------------------------------------
// allocate (...)
//
// The use of "this->" seems to be required by the GNU 4.7 compiler.
// (It will probably be required by subsequent compilers as well.)
//------------------------------------------------------------------------------

template <typename Elem> 
      void WitTVec <Elem>::allocate (WitProblem * theProblem, Elem theScalar)
   {
   this->resize (theProblem->nPeriods (), theScalar);
   }

//------------------------------------------------------------------------------
// Explicit instantiation of class template TVec <Elem>
//------------------------------------------------------------------------------

template class WitTVec <bool>;
template class WitTVec <int>;
template class WitTVec <double>;
template class WitTVec <void *>;

//------------------------------------------------------------------------------
// Implementation of class template Vector <Elem>
//------------------------------------------------------------------------------

template <typename Elem> 
      WitVector <Elem>::WitVector ():
         myCVec_ (NULL),
         length_ (0)
   {
   }

//------------------------------------------------------------------------------

template <typename Elem> 
      WitVector <Elem>::WitVector (int nElems, Elem initValue):
         myCVec_ (NULL),
         length_ (0)
   {
   resize (nElems, initValue);
   }

//------------------------------------------------------------------------------

template <typename Elem> 
      WitVector <Elem>::~WitVector ()
   {
   delete[] myCVec_;
   }

//------------------------------------------------------------------------------

template <typename Elem> 
      void WitVector <Elem>::operator = (const WitVector & theVector)
   {
   witAssert (length_ == theVector.length_);

   copy (myCVec_, theVector.myCVec_, length_);
   }

//------------------------------------------------------------------------------

template <typename Elem> 
      void WitVector <Elem>::operator = (Elem theScalar)
   {
   int theIdx;

   for (theIdx = 0; theIdx < length_; theIdx ++)
      myCVec_[theIdx] = theScalar;
   }  

//------------------------------------------------------------------------------

template <typename Elem> 
      void WitVector <Elem>::operator = (const Elem * theCVec)
   {
   copy (myCVec_, theCVec, length_);
   }

//------------------------------------------------------------------------------

template <typename Elem> 
      void WitVector <Elem>::copyInto (Elem * theCVec) const
   {
   copy (theCVec, myCVec_, length_);
   }

//------------------------------------------------------------------------------

template <typename Elem> 
      void WitVector <Elem>::clear ()
   {
   resize (0);
   }

//------------------------------------------------------------------------------

template <typename Elem> 
      void WitVector <Elem>::resize (int nElems, Elem theScalar)
   {
   resize (nElems);

   (* this) = theScalar;
   }

//------------------------------------------------------------------------------

template <typename Elem> 
       void WitVector <Elem>::resize (int nElems)
   {
   delete[] myCVec_;

   myCVec_ = new Elem[nElems];
   length_ = nElems;
   }

//------------------------------------------------------------------------------

template <typename Elem> 
      bool WitVector <Elem>::operator == (Elem theScalar) const
   {
   int theIdx;

   for (theIdx = 0; theIdx < length (); theIdx ++)
      if (myCVec_[theIdx] != theScalar)
         return false;

   return true;
   }

//------------------------------------------------------------------------------

template <typename Elem> 
      void WitVector <Elem>::checkBounds (int theIdx) const
   {
   if (theIdx < 0)
      WitMsgFacility::issueByException ("negativeIndexFmsg", theIdx);

   if (theIdx >= length_)
      WitMsgFacility::issueByException ("indexTooLargeFmsg",
         theIdx,
         length_ - 1);
   }

//------------------------------------------------------------------------------
// Explicit instantiation of class template Vector <Elem>
//------------------------------------------------------------------------------

template class WitVector <bool>;
template class WitVector <char>;
template class WitVector <int>;
template class WitVector <double>;
template class WitVector <void *>;
template class WitVector <const char *>;

//------------------------------------------------------------------------------
// Implementation of NonClass functions with arguments belonging to Vector
// classes and C-vector types.
//------------------------------------------------------------------------------

bool WitNonClass::operator >= (
      const WitVector <double> & theVector,
      double                     theScalar)
   {
   int theIdx;

   for (theIdx = 0; theIdx < theVector.length (); theIdx ++)
      if (theVector[theIdx] < theScalar)
         return false;

   return true;
   }

//------------------------------------------------------------------------------

template <typename Elem> 
      void WitNonClass::copy (
         Elem *       dstCVec, 
         const Elem * srcCVec, 
         int          nElems)
   {
   int theIdx;

   if (DEBUG_MODE)
      if (nElems > 0)
         {
         witAssert (dstCVec != NULL);
         witAssert (srcCVec != NULL);
         }

   for (theIdx = 0; theIdx < nElems; theIdx ++)
      dstCVec[theIdx] = srcCVec[theIdx];
   }

//------------------------------------------------------------------------------
// Explicit instantiation of function template NonClass::copy.
//------------------------------------------------------------------------------

template void WitNonClass::copy <bool>   (bool *,   const bool *,   int);
template void WitNonClass::copy <int>    (int *,    const int *,    int);
template void WitNonClass::copy <double> (double *, const double *, int);
template void WitNonClass::copy <void *> (void * *, void * const *, int);

//------------------------------------------------------------------------------

template <typename Elem> 
      bool WitNonClass::hasSingleValue (const Elem * theCVec, int nElems)
   {
   Elem valAt0;
   int  theIdx;

   witAssert (theCVec != NULL);

   valAt0 = theCVec[0];

   for (theIdx = 1; theIdx < nElems; theIdx ++)
      if (theCVec[theIdx] != valAt0)
         return false;

   return true;
   }

//------------------------------------------------------------------------------
// Explicit instantiation of function template NonClass::hasSingleValue.
//------------------------------------------------------------------------------

template bool WitNonClass::hasSingleValue <bool>   (const bool *,       int);
template bool WitNonClass::hasSingleValue <int>    (const int *,        int);
template bool WitNonClass::hasSingleValue <float>  (const float *,      int);
template bool WitNonClass::hasSingleValue <double> (const double *,     int);
template bool WitNonClass::hasSingleValue <void *> (void * const *,     int);
