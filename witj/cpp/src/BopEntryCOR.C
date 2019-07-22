//------------------------------------------------------------------------------
// WIT-J C++ Source file BopEntryCOR.C.
//
// Contains the C++ implementations of the native methods of Java class
// witj.BopEntryCOR.
//------------------------------------------------------------------------------

#include     <NativeMacros.h>
#include <ExceptionHandler.h>
#include       <JniHandler.h>
#include         <BopEntry.h>

using namespace WitJ;

//------------------------------------------------------------------------------

NATIVE_METHOD (void, BopEntryCOR, createAndStoreCppBopEntry) (
      JNIEnv *   theJNIEnv,
      _jobject * theBopEntCORJObj,
      _jobject * theOpnCORJObj,
      jint       theBopIdx,
      _jobject * thePartCORJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler  theJniHandler (theJNIEnv);
   Operation * theOpn;
   Part *      thePart;
   BopEntry *  theBopEnt;

   theOpn    = theJniHandler.getJavaAccObj (theOpnCORJObj) ->asOperation ();
   thePart   = theJniHandler.getJavaAccObj (thePartCORJObj)->asPart ();

   theBopEnt = new BopEntry (theOpn, theBopIdx, thePart);

   theJniHandler.storeInCOR (theBopEntCORJObj, theBopEnt);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (jint, BopEntryCOR, getBopIndex) (
      JNIEnv *   theJNIEnv,
      _jobject * theBopEntCORJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler theJniHandler (theJNIEnv);
   BopEntry * theBopEnt;

   theBopEnt = theJniHandler.getJavaAccObj (theBopEntCORJObj)->asBopEntry ();

   return theBopEnt->getBopIndex ();

   END_TRY_BLOCK_VALUE_RETURN (theJNIEnv, 0)
   }
