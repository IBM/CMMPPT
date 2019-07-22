//------------------------------------------------------------------------------
// WIT-J C++ Source file AttBldrCOR.C.
//
// Contains the C++ implementations of the native methods of Java class
// witj.AttBldrCOR.
//------------------------------------------------------------------------------

#include     <NativeMacros.h>
#include <ExceptionHandler.h>
#include       <JniHandler.h>
#include          <AttBldr.h>
#include        <StringJOR.h>

using namespace WitJ;

//------------------------------------------------------------------------------

NATIVE_METHOD (jboolean, AttBldrCOR, hasAttFor) (
      JNIEnv *   theJNIEnv,
      _jobject * theAttBldrCORJObj,
      _jstring * scopedAttNameJStr)
   {
   BEGIN_TRY_BLOCK

   JniHandler   theJniHandler (theJNIEnv);
   AttBldr *    theAttBldr;
   const char * scopedAttNameCStr;

   theAttBldr = theJniHandler.getJavaAccObj (theAttBldrCORJObj)->asAttBldr ();

   scopedAttNameCStr = theJniHandler.getCString (scopedAttNameJStr);

   return theAttBldr->hasAttFor (scopedAttNameCStr);

   END_TRY_BLOCK_VALUE_RETURN (theJNIEnv, false)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (_jstring *, AttBldrCOR, getAllAttNames) (
      JNIEnv *   theJNIEnv,
      _jobject * theAttBldrCORJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler theJniHandler (theJNIEnv);
   AttBldr *  theAttBldr;
   StringJOR  theStringJOR  (theJNIEnv);

   theAttBldr = theJniHandler.getJavaAccObj (theAttBldrCORJObj)->asAttBldr ();

   theAttBldr->getAllAttNames (theStringJOR);

   return theStringJOR.getJString ();

   END_TRY_BLOCK_VALUE_RETURN (theJNIEnv, NULL)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, AttBldrCOR, createAndStoreCppAttBldr) (
      JNIEnv *   theJNIEnv,
      _jobject * theAttBldrCORJObj,
      _jobject * theCoordCORJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler    theJniHandler (theJNIEnv);
   Coordinator * theCoord;
   AttBldr *     theAttBldr;

   theCoord   = theJniHandler.getJavaAccObj (theCoordCORJObj)->asCoordinator ();

   theAttBldr = new AttBldr (theCoord);

   theJniHandler.storeInCOR (theAttBldrCORJObj, theAttBldr);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }
