//------------------------------------------------------------------------------
// WIT-J C++ Source File ArrayListJOR.C.
//
// Contains the implementation of the following clases and templates:
//
//    ArrayListJOR
//    IntALJOR
//    DblALJOR
//    StringALJOR
//    CompALJOR <C>
//    DemandALJOR
//    PeggingTripleALJOR
//------------------------------------------------------------------------------

#include <ArrayListJOR.h>
#include <ComponentJOR.h>
#include  <Coordinator.h>
#include         <Part.h>
#include       <Demand.h>
#include    <Operation.h>
#include    <StringVec.h>

//------------------------------------------------------------------------------
// Implementation of class ArrayListJOR.
//------------------------------------------------------------------------------

_jmethodID * WitJ::ArrayListJOR::         clearJMeth_ = NULL;
_jmethodID * WitJ::ArrayListJOR::ensureCapacityJMeth_ = NULL;
_jmethodID * WitJ::ArrayListJOR::          sizeJMeth_ = NULL;
_jmethodID * WitJ::ArrayListJOR::           addJMeth_ = NULL;
_jmethodID * WitJ::ArrayListJOR::           getJMeth_ = NULL;

//------------------------------------------------------------------------------

void WitJ::ArrayListJOR::setUpJni (JNIEnv * theJNIEnv)
   {
   EXCEPTION_ASSERT (clearJMeth_ == NULL);

   clearJMeth_ =
      getJMethodID (
         "java/util/ArrayList",
         "clear",
         "()V",
         theJNIEnv);

   ensureCapacityJMeth_ =
      getJMethodID (
         "java/util/ArrayList",
         "ensureCapacity",
         "(I)V",
         theJNIEnv);

   sizeJMeth_ =
      getJMethodID (
         "java/util/ArrayList",
         "size",
         "()I",
         theJNIEnv);

   addJMeth_ =
      getJMethodID (
         "java/util/ArrayList",
         "add",
         "(Ljava/lang/Object;)Z",
         theJNIEnv);

   getJMeth_ =
      getJMethodID (
         "java/util/ArrayList",
         "get",
         "(I)Ljava/lang/Object;",
         theJNIEnv);
   }

//------------------------------------------------------------------------------

void WitJ::ArrayListJOR::clearAndAllocate (int minCapacity)
   {
   jint minCapJInt;

   getJNIEnv ()->CallVoidMethod (myArrayListJObj_, clearJMeth_);

   throwAnyJavaException ();

   minCapJInt = minCapacity;

   getJNIEnv ()->
      CallVoidMethod (myArrayListJObj_, ensureCapacityJMeth_, minCapJInt);

   throwAnyJavaException ();
   }

//------------------------------------------------------------------------------

WitJ::ArrayListJOR::ArrayListJOR (
         _jobject * theArrayListJObj,
         JNIEnv *   theJNIEnv):

      JniClient        (theJNIEnv),
      myArrayListJObj_ (theArrayListJObj)
   {
   }

//------------------------------------------------------------------------------

WitJ::ArrayListJOR::~ArrayListJOR ()
   {
   }

//------------------------------------------------------------------------------

int WitJ::ArrayListJOR::size ()
   {
   return getJNIEnv ()->CallIntMethod (myArrayListJObj_, sizeJMeth_);
   }

//------------------------------------------------------------------------------

void WitJ::ArrayListJOR::add (_jobject * theJObject)
   {
   getJNIEnv ()->CallBooleanMethod (myArrayListJObj_, addJMeth_, theJObject);

   throwAnyJavaException ();
   }

//------------------------------------------------------------------------------

_jobject * WitJ::ArrayListJOR::get (int theIdx)
   {
   jint       theIdxJInt;
   _jobject * theJObj;

   theIdxJInt = theIdx;

   theJObj =
      getJNIEnv ()->CallObjectMethod (myArrayListJObj_, getJMeth_, theIdxJInt);

   throwAnyJavaException ();

   EXCEPTION_ASSERT (theJObj != NULL);

   return theJObj;
   }

//------------------------------------------------------------------------------
// Implementation of class IntALJOR.
//------------------------------------------------------------------------------

_jmethodID * WitJ::IntALJOR::intValueJMeth_ = NULL;

//------------------------------------------------------------------------------

void WitJ::IntALJOR::setUpJni (JNIEnv * theJNIEnv)
   {
   intValueJMeth_ =
      getJMethodID (
         "java/lang/Integer",
         "intValue",
         "()I",
         theJNIEnv);
   }

//------------------------------------------------------------------------------

WitJ::IntALJOR::IntALJOR (_jobject * theIntArrayListJObj, JNIEnv * theJNIEnv):

      ArrayListJOR (theIntArrayListJObj, theJNIEnv)
   {
   }

//------------------------------------------------------------------------------

WitJ::IntALJOR::~IntALJOR ()
   {
   }

//------------------------------------------------------------------------------

void WitJ::IntALJOR::copyFrom (const int * theIntCVec, int theLength)
   {
   int         theIdx;
    _jobject * theIntegerJObj;

   clearAndAllocate (theLength);

   for (theIdx = 0; theIdx < theLength; theIdx ++)
      {
      theIntegerJObj = matchingIntegerJObj (theIntCVec[theIdx], getJNIEnv ());

      add            (theIntegerJObj);

      deleteLocalRef (theIntegerJObj);
      }
   }

//------------------------------------------------------------------------------

void WitJ::IntALJOR::copyInto (std::vector <int> & theIntVec)
   {
   int        theLength;
   int        theIdx;
   _jobject * theIntegerJObj;
   int        theInt;

   theLength = size ();

   theIntVec.clear ();

   theIntVec.resize (theLength);

   for (theIdx = 0; theIdx < theLength; theIdx ++)
      {
      theIntegerJObj = get (theIdx);

      theInt = getJNIEnv ()->CallIntMethod (theIntegerJObj, intValueJMeth_);

      throwAnyJavaException ();

      theIntVec[theIdx] = theInt;

      deleteLocalRef (theIntegerJObj);
      }
   }

//------------------------------------------------------------------------------
// Implementation of class DblALJOR.
//------------------------------------------------------------------------------

_jmethodID * WitJ::DblALJOR::doubleValueJMeth_ = NULL;

//------------------------------------------------------------------------------

void WitJ::DblALJOR::setUpJni (JNIEnv * theJNIEnv)
   {
   doubleValueJMeth_ =
      getJMethodID (
         "java/lang/Double",
         "doubleValue",
         "()D",
         theJNIEnv);
   }

//------------------------------------------------------------------------------

WitJ::DblALJOR::DblALJOR (_jobject * theDblArrayListJObj, JNIEnv * theJNIEnv):

      ArrayListJOR (theDblArrayListJObj, theJNIEnv)
   {
   }

//------------------------------------------------------------------------------

WitJ::DblALJOR::~DblALJOR ()
   {
   }

//------------------------------------------------------------------------------

void WitJ::DblALJOR::copyFrom (const double * theDblCVec, int theLength)
   {
   int         theIdx;
    _jobject * theDoubleJObj;

   clearAndAllocate (theLength);

   for (theIdx = 0; theIdx < theLength; theIdx ++)
      {
      theDoubleJObj = matchingDoubleJObj (theDblCVec[theIdx], getJNIEnv ());

      add            (theDoubleJObj);

      deleteLocalRef (theDoubleJObj);
      }
   }

//------------------------------------------------------------------------------

void WitJ::DblALJOR::copyInto (std::vector <double> & theDblVec)
   {
   int        theLength;
   int        theIdx;
   _jobject * theDoubleJObj;
   double     theDbl;

   theLength = size ();

   theDblVec.clear ();

   theDblVec.resize (theLength);

   for (theIdx = 0; theIdx < theLength; theIdx ++)
      {
      theDoubleJObj = get (theIdx);

      theDbl =
         getJNIEnv ()->CallDoubleMethod (theDoubleJObj, doubleValueJMeth_);

      throwAnyJavaException ();

      theDblVec[theIdx] = theDbl;

      deleteLocalRef (theDoubleJObj);
      }
   }

//------------------------------------------------------------------------------
// Implementation of class StringALJOR.
//------------------------------------------------------------------------------

WitJ::StringALJOR::StringALJOR (
         _jobject * theStringArrayListJObj,
         JNIEnv *   theJNIEnv):

      ArrayListJOR (theStringArrayListJObj, theJNIEnv)
   {
   }

//------------------------------------------------------------------------------

WitJ::StringALJOR::~StringALJOR ()
   {
   }

//------------------------------------------------------------------------------

void WitJ::StringALJOR::copyFrom (
      const char * const * theCStrCVec,
      int                  theLength)
   {
   int        theIdx;
   _jstring * theJString;

   clearAndAllocate (theLength);

   for (theIdx = 0; theIdx < theLength; theIdx ++)
      {
      theJString = newJString (theCStrCVec[theIdx]);

      add            (theJString);

      deleteLocalRef (theJString);
      }
   }

//------------------------------------------------------------------------------

void WitJ::StringALJOR::copyInto (StringVec & theStringVec)
   {
   int        theLength;
   int        theIdx;
   _jobject * theStringJObj;
   _jstring * theJString;
   int        theJStringLen;
   int        theCStringLen;

   theLength = size ();

   theStringVec.resize (theLength);

   for (theIdx = 0; theIdx < theLength; theIdx ++)
      {
      theStringJObj = get (theIdx);

      theJString    = static_cast <_jstring *> (theStringJObj);

      theJStringLen = getJNIEnv ()->GetStringLength    (theJString);
      theCStringLen = getJNIEnv ()->GetStringUTFLength (theJString);

      theStringVec.resizeAt (theIdx, theCStringLen);

      getJNIEnv ()->
         GetStringUTFRegion (
            theJString,
            0,
            theJStringLen,
            theStringVec[theIdx]);

      throwAnyJavaException ();

      theStringVec[theIdx][theCStringLen] = '\0';

      deleteLocalRef (theStringJObj);
      }
   }

//------------------------------------------------------------------------------
// Implementation of class template CompALJOR <C>.
//------------------------------------------------------------------------------

template <typename C>
      WitJ::CompALJOR <C>::CompALJOR (
            _jobject * theCompArrayListJObj,
            JNIEnv *   theJNIEnv):

         ArrayListJOR (theCompArrayListJObj, theJNIEnv)
   {
   }

//------------------------------------------------------------------------------

template <typename C>
      WitJ::CompALJOR <C>::~CompALJOR ()
   {
   }

//------------------------------------------------------------------------------

template <typename C>
      void WitJ::CompALJOR <C>::add (C * theC)
   {
   _jobject * theCompJObj;

   theCompJObj = theC->getComponentJOR ()->getComponentJObj ();

   ArrayListJOR::add (theCompJObj);
   }

//------------------------------------------------------------------------------
// Explicit instantiation of class template CompALJOR <C>.
//------------------------------------------------------------------------------

namespace WitJ
{
template class CompALJOR <Part>;
template class CompALJOR <Demand>;
template class CompALJOR <Operation>;
}

//------------------------------------------------------------------------------
// Implementation of class DemandALJOR.
//------------------------------------------------------------------------------

_jmethodID * WitJ::DemandALJOR::getDemandCORJMeth_ = NULL;

//------------------------------------------------------------------------------

void WitJ::DemandALJOR::setUpJni (JNIEnv * theJNIEnv)
   {
   getDemandCORJMeth_ =
      getJMethodID (
         "com/ibm/witj/Demand",
         "getDemandCOR",
         "()Lcom/ibm/witj/DemandCOR;",
         theJNIEnv);
   }

//------------------------------------------------------------------------------

WitJ::DemandALJOR::DemandALJOR (
         _jobject * theDemandArrayListJObj,
         JNIEnv *   theJNIEnv):

      ArrayListJOR (theDemandArrayListJObj, theJNIEnv)
   {
   }

//------------------------------------------------------------------------------

WitJ::DemandALJOR::~DemandALJOR ()
   {
   }

//------------------------------------------------------------------------------

void WitJ::DemandALJOR::populateIDVecs (
      std::vector <const char *> & thePartNameVec,
      std::vector <const char *> & theDemandNameVec)
   {
   int        theLength;
   int        theIdx;
   _jobject * theDemandCORJObj;
   _jobject * theDemandJObj;
   Demand *   theDemand;

   theLength = size ();

   thePartNameVec  .clear  ();
   theDemandNameVec.clear  ();

   thePartNameVec  .resize (theLength);
   theDemandNameVec.resize (theLength);

   for (theIdx = 0; theIdx < theLength; theIdx ++)
      {
      theDemandJObj = get (theIdx);

      theDemandCORJObj =
         getJNIEnv ()->CallObjectMethod (theDemandJObj, getDemandCORJMeth_);

      throwAnyJavaException ();

      theDemand = getJavaAccObj (theDemandCORJObj)->asDemand ();

      thePartNameVec  [theIdx] = theDemand->getPartName   ();
      theDemandNameVec[theIdx] = theDemand->getDemandName ();

      deleteLocalRef (theDemandJObj);
      deleteLocalRef (theDemandCORJObj);
      }
   }

//------------------------------------------------------------------------------
// Implementation of class PeggingTripleALJOR.
//------------------------------------------------------------------------------

_jclass *    WitJ::PeggingTripleALJOR::ourPeggingTripleJCl_    = NULL;

_jmethodID * WitJ::PeggingTripleALJOR::ctorPeggingTripleJMeth_ = NULL;

//------------------------------------------------------------------------------

void WitJ::PeggingTripleALJOR::setUpJni (JNIEnv * theJNIEnv)
   {
   ourPeggingTripleJCl_ =
      newJClassGRef ("com/ibm/witj/PeggingTriple", theJNIEnv);

   ctorPeggingTripleJMeth_ =
      getJMethodID (
         "com/ibm/witj/PeggingTriple",
         "<init>",
         "(Lcom/ibm/witj/Component;ID)V",
         theJNIEnv);
   }

//------------------------------------------------------------------------------

WitJ::PeggingTripleALJOR::PeggingTripleALJOR (
         _jobject * theTripleListJObj,
         JNIEnv *   theJNIEnv):

      ArrayListJOR (theTripleListJObj, theJNIEnv)
   {
   }

//------------------------------------------------------------------------------

WitJ::PeggingTripleALJOR::~PeggingTripleALJOR ()
   {
   }

//------------------------------------------------------------------------------

void WitJ::PeggingTripleALJOR::add (
      Component * theComp,
      int         thePeriod,
      double      theVolume)
   {
   _jobject * theCompJObj;
   _jobject * thePeggingTripleJObj;

   theCompJObj = theComp->getComponentJOR ()->getComponentJObj ();

   thePeggingTripleJObj =
      getJNIEnv ()->
         NewObject (
            ourPeggingTripleJCl_,
            ctorPeggingTripleJMeth_,
            theCompJObj,
            thePeriod,
            theVolume);
               
   throwAnyJavaException ();

   ArrayListJOR::add (thePeggingTripleJObj);

   deleteLocalRef    (thePeggingTripleJObj);
   }
