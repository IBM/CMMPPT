//------------------------------------------------------------------------------
// WIT-J C++ Source file OperationCOR.C.
//
// Contains the C++ implementations of the native methods of Java class
// witj.OperationCOR.
//------------------------------------------------------------------------------

#include     <NativeMacros.h>
#include <ExceptionHandler.h>
#include       <JniHandler.h>
#include        <Operation.h>

using namespace WitJ;

//------------------------------------------------------------------------------

NATIVE_METHOD (void, OperationCOR, setOperationName) (
      JNIEnv *   theJNIEnv,
      _jobject * theOpnCORJObj,
      _jstring * theOpnNameJStr)
   {
   BEGIN_TRY_BLOCK

   JniHandler   theJniHandler (theJNIEnv);
   Operation *  theOpn;
   const char * theOpnNameCStr;

   theOpn         = theJniHandler.getJavaAccObj (theOpnCORJObj)->asOperation ();
   theOpnNameCStr = theJniHandler.getCString    (theOpnNameJStr);

   theOpn->setOperationName (theOpnNameCStr);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, OperationCOR, createAndStoreCppOperation) (
      JNIEnv *   theJNIEnv,
      _jobject * theOpnCORJObj,
      _jobject * theCoordCORJObj,
      _jstring * theOpnNameJStr)
   { 
   BEGIN_TRY_BLOCK

   JniHandler    theJniHandler (theJNIEnv);
   Coordinator * theCoord;
   const char *  theOpnNameCStr;
   Operation *   theOpn;

   theCoord = theJniHandler.getJavaAccObj (theCoordCORJObj)->asCoordinator ();

   theOpnNameCStr = theJniHandler.getCString (theOpnNameJStr);

   theOpn = new Operation (theCoord, theOpnNameCStr);

   theJniHandler.storeInCOR (theOpnCORJObj, theOpn);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }
