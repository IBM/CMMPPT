//------------------------------------------------------------------------------
// WIT-J C++ Source File CoordinatorCOR.C.
//------------------------------------------------------------------------------

#include     <NativeMacros.h>
#include <ExceptionHandler.h>
#include       <JniHandler.h>
#include      <Coordinator.h>

using namespace WitJ;

//------------------------------------------------------------------------------

NATIVE_METHOD (void, CoordinatorCOR, setUpCaseTokens) (
      JNIEnv *   theJNIEnv,
      _jobject * theCoordCORJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler    theJniHandler (theJNIEnv);
   Coordinator * theCoord;

   theCoord = theJniHandler.getJavaAccObj (theCoordCORJObj)->asCoordinator ();

   theCoord->setUpCaseTokens ();

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, CoordinatorCOR, restoreAfterPurge) (
      JNIEnv *   theJNIEnv,
      _jobject * theCoordCORJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler    theJniHandler (theJNIEnv);
   Coordinator * theCoord;

   theCoord = theJniHandler.getJavaAccObj (theCoordCORJObj)->asCoordinator ();

   theCoord->restoreAfterPurge ();

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, CoordinatorCOR, clearData) (
      JNIEnv *   theJNIEnv,
      _jobject * theCoordCORJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler    theJniHandler (theJNIEnv);
   Coordinator * theCoord;

   theCoord = theJniHandler.getJavaAccObj (theCoordCORJObj)->asCoordinator ();

   theCoord->clearData ();

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, CoordinatorCOR, createAndStoreCppCoordinator) (
      JNIEnv *   theJNIEnv,
      _jobject * theCoordCORJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler    theJniHandler (theJNIEnv);
   Coordinator * theCoord;

   theCoord = new Coordinator;

   theJniHandler.storeInCOR (theCoordCORJObj, theCoord);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }
