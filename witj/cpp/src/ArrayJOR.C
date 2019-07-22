//------------------------------------------------------------------------------
// WIT-J C++ Source File ArrayJOR.C.
//
// Contains the implementation of the following classes and templates:
//
//    ArrayJOR <E>
//    BoolArrayJOR
//    IntArrayJOR
//    DblArrayJOR
//------------------------------------------------------------------------------

#include    <ArrayJOR.h>
#include <CTVecSupply.h>
#include   <Component.h>
#include <Coordinator.h>

//------------------------------------------------------------------------------
// Implementation of class template ArrayJOR <E>.
//------------------------------------------------------------------------------

template <typename E>
      WitJ::ArrayJOR <E>::ArrayJOR (
            _jarray *  theJArray,
            _jobject * theJavaAccObjJObj,
            JNIEnv *   theJNIEnv):

         JniClient      (theJNIEnv),
         myJArray_      (theJArray),
         myLength_      (0),
         myCTVecSupply_ (NULL),
         myCTVector_    (NULL)
   {
   if (theJArray != NULL)
      myLength_ = theJNIEnv->GetArrayLength (myJArray_);
   else
      myLength_ = 0;

   myCTVecSupply_ =
      getJavaAccObj (theJavaAccObjJObj)->getCoordinator ()->getCTVecSupply ();
   }

//------------------------------------------------------------------------------

template <typename E>
      WitJ::ArrayJOR <E>::~ArrayJOR ()
   {
   if (myCTVector_ != NULL)
      {
      myCTVecSupply_->reclaimCTVec (myCTVector_);
      }
   }

//------------------------------------------------------------------------------

template <typename E>
      void * WitJ::ArrayJOR <E>::getVoidCVecCrit ()
   {
   void * theVoidCVec;

   theVoidCVec = getJNIEnv ()->GetPrimitiveArrayCritical (myJArray_, NULL);

   throwAnyJavaException ();

   return theVoidCVec;
   }

//------------------------------------------------------------------------------

template <typename E>
      E * WitJ::ArrayJOR <E>::acquiredCTVec ()
   {
   myCTVecSupply_->provideCTVec (myCTVector_);

   return myCTVector_;
   }

//------------------------------------------------------------------------------

template <typename E>
      void WitJ::ArrayJOR <E>::releaseJArrayCritAndCopy (void * theVoidCVec)
   {
   getJNIEnv ()->ReleasePrimitiveArrayCritical (myJArray_, theVoidCVec, 0);
   }

//------------------------------------------------------------------------------

template <typename E>
      void WitJ::ArrayJOR <E>::releaseJArrayCritDontCopy (void * theVoidCVec)
   {
   getJNIEnv ()->
      ReleasePrimitiveArrayCritical (myJArray_, theVoidCVec, JNI_ABORT);
   }

//------------------------------------------------------------------------------
// Implementation of class BoolArrayJOR.
//------------------------------------------------------------------------------

WitJ::BoolArrayJOR::BoolArrayJOR (
         _jbooleanArray * theJBoolArray,
         _jobject *       theJavaAccObjJObj,
         JNIEnv *         theJNIEnv):

      ArrayJOR <Boolean> (theJBoolArray, theJavaAccObjJObj, theJNIEnv)
   {
   }

//------------------------------------------------------------------------------

WitJ::BoolArrayJOR::~BoolArrayJOR ()
   {
   }

//------------------------------------------------------------------------------

void WitJ::BoolArrayJOR::copyFrom (const Boolean * theBoolCVec)
   {
   jboolean * theJBoolCVec;
   int        theIdx;

   theJBoolCVec = static_cast <jboolean *> (getVoidCVecCrit ());

   for (theIdx = 0; theIdx < getLength (); theIdx ++)
      {
      theJBoolCVec[theIdx] = theBoolCVec[theIdx];
      }

   releaseJArrayCritAndCopy (theJBoolCVec);
   }

//------------------------------------------------------------------------------

const WitJ::Boolean * WitJ::BoolArrayJOR::getCTVec ()
   {
   jboolean * theJBoolCVec;
   Boolean *  theBoolCVec;
   int        theIdx;

   theJBoolCVec = static_cast <jboolean *> (getVoidCVecCrit ());

   theBoolCVec  = acquiredCTVec ();

   for (theIdx = 0; theIdx < getLength (); theIdx ++)
      {
      theBoolCVec[theIdx] = theJBoolCVec[theIdx];
      }

   releaseJArrayCritDontCopy (theJBoolCVec);

   return theBoolCVec;
   }

//------------------------------------------------------------------------------
// Implementation of class IntArrayJOR.
//------------------------------------------------------------------------------

WitJ::IntArrayJOR::IntArrayJOR (
         _jintArray * theJIntArray,
         _jobject *   theJavaAccObjJObj,
         JNIEnv *     theJNIEnv):

      ArrayJOR <int> (theJIntArray, theJavaAccObjJObj, theJNIEnv)
   {
   }

//------------------------------------------------------------------------------

WitJ::IntArrayJOR::~IntArrayJOR ()
   {
   }

//------------------------------------------------------------------------------

void WitJ::IntArrayJOR::copyFrom (const int * theIntCVec)
   {
   jint * theJIntCVec;
   int    theIdx;

   theJIntCVec = static_cast <jint *> (getVoidCVecCrit ());

   for (theIdx = 0; theIdx < getLength (); theIdx ++)
      {
      theJIntCVec[theIdx] = theIntCVec[theIdx];
      }

   releaseJArrayCritAndCopy (theJIntCVec);
   }

//------------------------------------------------------------------------------

const int * WitJ::IntArrayJOR::getCTVec ()
   {
   jint * theJIntCVec;
   int *  theIntCVec;
   int    theIdx;

   theJIntCVec = static_cast <jint *> (getVoidCVecCrit ());

   theIntCVec  = acquiredCTVec ();

   for (theIdx = 0; theIdx < getLength (); theIdx ++)
      {
      theIntCVec[theIdx] = theJIntCVec[theIdx];
      }

   releaseJArrayCritDontCopy (theJIntCVec);

   return theIntCVec;
   }

//------------------------------------------------------------------------------
// Implementation of class DblArrayJOR.
//------------------------------------------------------------------------------

WitJ::DblArrayJOR::DblArrayJOR (
         _jdoubleArray * theJDblArray,
         _jobject *      theJavaAccObjJObj,
         JNIEnv *        theJNIEnv):

      ArrayJOR <double> (theJDblArray, theJavaAccObjJObj, theJNIEnv)
   {
   }

//------------------------------------------------------------------------------

WitJ::DblArrayJOR::~DblArrayJOR ()
   {
   }

//------------------------------------------------------------------------------

const double * WitJ::DblArrayJOR::getBoundCTVec ()
   {
   if (getLength () == 0)
      return NULL;
   else
      return getCTVec ();
   }

//------------------------------------------------------------------------------

void WitJ::DblArrayJOR::copyFrom (const double * theDblCVec)
   {
   jdouble * theJDblCVec;
   int       theIdx;

   theJDblCVec = static_cast <jdouble *> (getVoidCVecCrit ());

   for (theIdx = 0; theIdx < getLength (); theIdx ++)
      {
      theJDblCVec[theIdx] = theDblCVec[theIdx];
      }

   releaseJArrayCritAndCopy (theJDblCVec);
   }

//------------------------------------------------------------------------------

const double * WitJ::DblArrayJOR::getCTVec ()
   {
   jdouble * theJDblCVec;
   double *  theDblCVec;
   int       theIdx;

   theJDblCVec = static_cast <jdouble *> (getVoidCVecCrit ());

   theDblCVec  = acquiredCTVec ();

   for (theIdx = 0; theIdx < getLength (); theIdx ++)
      {
      theDblCVec[theIdx] = theJDblCVec[theIdx];
      }

   releaseJArrayCritDontCopy (theJDblCVec);

   return theDblCVec;
   }
