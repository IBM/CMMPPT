//------------------------------------------------------------------------------
// WIT-J C++ Source file JavaAccObjCOR.C.
//
// Contains the C++ implementations of the native methods of Java class
// witj.JavaAccObjCOR.
//------------------------------------------------------------------------------

#include     <NativeMacros.h>
#include <ExceptionHandler.h>
#include       <JniHandler.h>
#include     <ComponentJOR.h>
#include       <JavaAccObj.h>
#include     <ArrayListJOR.h>

using namespace WitJ;

//------------------------------------------------------------------------------
// setUpJni ()
//
// This function is to be called only from within the execution of the Java
// method JavaAccObjCOR.setUpCppPortion (), which is statically synchronized.
//------------------------------------------------------------------------------

NATIVE_METHOD (void, JavaAccObjCOR, setUpJni) (
      JNIEnv * theJNIEnv,
      _jclass *)
   {
   BEGIN_TRY_BLOCK

   JniClient         ::setUpJni (theJNIEnv);
   ComponentJOR      ::setUpJni (theJNIEnv);
   DemandALJOR       ::setUpJni (theJNIEnv);
   ArrayListJOR      ::setUpJni (theJNIEnv);
   IntALJOR          ::setUpJni (theJNIEnv);
   DblALJOR          ::setUpJni (theJNIEnv);
   PeggingTripleALJOR::setUpJni (theJNIEnv);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, JavaAccObjCOR, deleteCppObject) (
      JNIEnv *   theJNIEnv,
      _jobject * theJavaAccObjCORJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler   theJniHandler (theJNIEnv);
   JavaAccObj * theJavaAccObj;

   theJavaAccObj = theJniHandler.getJavaAccObj (theJavaAccObjCORJObj);

   theJniHandler.storeInCOR (theJavaAccObjCORJObj, NULL);

   delete theJavaAccObj;

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }
