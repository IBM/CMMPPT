//------------------------------------------------------------------------------
// WIT-J C++ Source file CaseTokenCOR.C.
//
// Contains the C++ implementations of the native methods of Java class
// witj.CaseTokenCOR.
//------------------------------------------------------------------------------

#include     <NativeMacros.h>
#include <ExceptionHandler.h>
#include       <JniHandler.h>
#include        <CaseToken.h>

using namespace WitJ;

//------------------------------------------------------------------------------

NATIVE_METHOD (void, CaseTokenCOR, createAndStoreCppCaseToken) (
      JNIEnv *   theJNIEnv,
      _jobject * theCaseTokenCORJObj,
      _jobject * theCoordCORJObj,
      _jstring * theNameJStr)
   { 
   BEGIN_TRY_BLOCK

   JniHandler    theJniHandler (theJNIEnv);
   Coordinator * theCoord;
   const char *  theNameCStr;
   CaseToken *   theCaseToken;

   theCoord = theJniHandler.getJavaAccObj (theCoordCORJObj)->asCoordinator ();

   theNameCStr  = theJniHandler.getCString (theNameJStr);

   theCaseToken = new CaseToken (theCoord, theNameCStr);

   theJniHandler.storeInCOR (theCaseTokenCORJObj, theCaseToken);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }
