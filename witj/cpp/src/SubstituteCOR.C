//------------------------------------------------------------------------------
// WIT-J C++ Source File SubstituteCOR.C.
//
// Contains the C++ implementations of the native methods of Java class
// witj.SubstituteCOR.
//------------------------------------------------------------------------------

#include     <NativeMacros.h>
#include <ExceptionHandler.h>
#include       <JniHandler.h>
#include       <Substitute.h>

using namespace WitJ;

//------------------------------------------------------------------------------

NATIVE_METHOD (void, SubstituteCOR, createAndStoreCppSubstitute) (
      JNIEnv *   theJNIEnv,
      _jobject * theSubCORJObj,
      _jobject * theBomEntCORJObj,
      jint       theSubIdx,
      _jobject * thePartCORJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler   theJniHandler (theJNIEnv);
   BomEntry *   theBomEnt;
   Part *       thePart;
   Substitute * theSub;

   theBomEnt = theJniHandler.getJavaAccObj (theBomEntCORJObj)->asBomEntry ();
   thePart   = theJniHandler.getJavaAccObj (thePartCORJObj)  ->asPart ();

   theSub    = new Substitute (theBomEnt, theSubIdx, thePart);

   theJniHandler.storeInCOR (theSubCORJObj, theSub);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (jint, SubstituteCOR, getSubIndex) (
      JNIEnv *   theJNIEnv,
      _jobject * theSubCORJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler   theJniHandler (theJNIEnv);
   Substitute * theSub;

   theSub = theJniHandler.getJavaAccObj (theSubCORJObj)->asSubstitute ();

   return theSub->getSubIndex ();

   END_TRY_BLOCK_VALUE_RETURN (theJNIEnv, 0)
   }
