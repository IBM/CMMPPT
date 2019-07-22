//------------------------------------------------------------------------------
// WIT-J C++ Source file PartCOR.C.
//
// Contains the C++ implementations of the native methods of Java class
// witj.PartCOR.
//------------------------------------------------------------------------------

#include     <NativeMacros.h>
#include <ExceptionHandler.h>
#include       <JniHandler.h>
#include             <Part.h>
#include      <Coordinator.h>
#include     <ArrayListJOR.h>

using namespace WitJ;

//------------------------------------------------------------------------------

NATIVE_METHOD (void, PartCOR, setPartName) (
      JNIEnv *   theJNIEnv,
      _jobject * thePartCORJObj,
      _jstring * thePartNameJStr)
   {
   BEGIN_TRY_BLOCK

   JniHandler   theJniHandler (theJNIEnv);
   Part *       thePart;
   const char * thePartNameCStr;

   thePart         = theJniHandler.getJavaAccObj (thePartCORJObj)->asPart ();
   thePartNameCStr = theJniHandler.getCString    (thePartNameJStr);

   thePart->setPartName (thePartNameCStr);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, PartCOR, getBelowList) (
      JNIEnv *   theJNIEnv,
      _jobject * thePartCORJObj,
      _jobject * theBelowListJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler       theJniHandler                   (theJNIEnv);
   Part *           thePart;
   CompALJOR <Part> theBelowALJOR (theBelowListJObj, theJNIEnv);

   thePart = theJniHandler.getJavaAccObj (thePartCORJObj)->asPart ();

   thePart->getBelowList (theBelowALJOR);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, PartCOR, createAndStoreCppPart) (
      JNIEnv *   theJNIEnv,
      _jobject * thePartCORJObj,
      _jobject * theCoordCORJObj,
      _jstring * thePartNameJStr,
      _jobject * thePartCatCTCORJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler    theJniHandler (theJNIEnv);
   Coordinator * theCoord;
   const char *  thePartNameCStr;
   CaseToken *   thePartCatCT;
   Part *        thePart;

   theCoord = theJniHandler.getJavaAccObj (theCoordCORJObj)->asCoordinator ();

   thePartNameCStr = theJniHandler.getCString (thePartNameJStr);

   thePartCatCT    =
      theJniHandler.getJavaAccObj (thePartCatCTCORJObj)->asCaseToken ();

   thePart         = new Part (theCoord, thePartNameCStr, thePartCatCT);

   theJniHandler.storeInCOR (thePartCORJObj, thePart);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }
