//------------------------------------------------------------------------------
// WIT-J C++ Source file BomEntryCOR.C.
//
// Contains the C++ implementations of the native methods of Java class
// witj.BomEntryCOR.
//------------------------------------------------------------------------------

#include     <NativeMacros.h>
#include <ExceptionHandler.h>
#include       <JniHandler.h>
#include         <BomEntry.h>

using namespace WitJ;

//------------------------------------------------------------------------------

NATIVE_METHOD (void, BomEntryCOR, createAndStoreCppBomEntry) (
      JNIEnv *   theJNIEnv,
      _jobject * theBomEntCORJObj,
      _jobject * theOpnCORJObj,
      jint       theBomIdx,
      _jobject * thePartCORJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler  theJniHandler (theJNIEnv);
   Operation * theOpn;
   Part *      thePart;
   BomEntry *  theBomEnt;

   theOpn    = theJniHandler.getJavaAccObj (theOpnCORJObj) ->asOperation ();
   thePart   = theJniHandler.getJavaAccObj (thePartCORJObj)->asPart ();

   theBomEnt = new BomEntry (theOpn, theBomIdx, thePart);

   theJniHandler.storeInCOR (theBomEntCORJObj, theBomEnt);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (jint, BomEntryCOR, getBomIndex) (
      JNIEnv *   theJNIEnv,
      _jobject * theBomEntCORJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler theJniHandler (theJNIEnv);
   BomEntry * theBomEnt;

   theBomEnt = theJniHandler.getJavaAccObj (theBomEntCORJObj)->asBomEntry ();

   return theBomEnt->getBomIndex ();

   END_TRY_BLOCK_VALUE_RETURN (theJNIEnv, 0)
   }
