//------------------------------------------------------------------------------
// WIT-J C++ Source file MessageMgrCOR.C.
//
// Contains the C++ implementations of the native methods of Java class
// witj.MessageMgrCOR.
//------------------------------------------------------------------------------

#include     <NativeMacros.h>
#include <ExceptionHandler.h>
#include       <JniHandler.h>
#include       <MessageMgr.h>
#include        <StringJOR.h>

using namespace WitJ;

//------------------------------------------------------------------------------

NATIVE_METHOD (void, MessageMgrCOR, flushFile) (
      JNIEnv *   theJNIEnv,
      _jobject * theMsgMgrCORJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler   theJniHandler (theJNIEnv);
   MessageMgr * theMsgMgr;

   theMsgMgr = theJniHandler.getJavaAccObj (theMsgMgrCORJObj)->asMessageMgr ();

   theMsgMgr->flushFile ();

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }
//------------------------------------------------------------------------------

NATIVE_METHOD (void, MessageMgrCOR, setQuiet) (
      JNIEnv *   theJNIEnv,
      _jobject * theMsgMgrCORJObj,
      jboolean   quietVal)
   {
   BEGIN_TRY_BLOCK

   JniHandler   theJniHandler (theJNIEnv);
   MessageMgr * theMsgMgr;

   theMsgMgr = theJniHandler.getJavaAccObj (theMsgMgrCORJObj)->asMessageMgr ();

   theMsgMgr->setQuiet (quietVal);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (jboolean, MessageMgrCOR, isQuiet) (
      JNIEnv *   theJNIEnv,
      _jobject * theMsgMgrCORJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler   theJniHandler (theJNIEnv);
   MessageMgr * theMsgMgr;

   theMsgMgr = theJniHandler.getJavaAccObj (theMsgMgrCORJObj)->asMessageMgr ();

   return theMsgMgr->isQuiet ();

   END_TRY_BLOCK_VALUE_RETURN (theJNIEnv, false)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, MessageMgrCOR, setMesgFileAccessMode) (
      JNIEnv *   theJNIEnv,
      _jobject * theMsgMgrCORJObj,
      _jstring * theAccModeJStr)
   {
   BEGIN_TRY_BLOCK

   JniHandler   theJniHandler (theJNIEnv);
   MessageMgr * theMsgMgr;
   const char * theAccModeCStr;

   theMsgMgr = theJniHandler.getJavaAccObj (theMsgMgrCORJObj)->asMessageMgr ();

   theAccModeCStr = theJniHandler.getCString (theAccModeJStr);

   theMsgMgr->setMesgFileAccessMode (theAccModeCStr);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (_jstring *, MessageMgrCOR, getMesgFileAccessMode) (
      JNIEnv *   theJNIEnv,
      _jobject * theMsgMgrCORJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler   theJniHandler  (theJNIEnv);
   MessageMgr * theMsgMgr;
   StringJOR    theAccModeSJOR (theJNIEnv);

   theMsgMgr = theJniHandler.getJavaAccObj (theMsgMgrCORJObj)->asMessageMgr ();

   theMsgMgr->getMesgFileAccessMode (theAccModeSJOR);

   return theAccModeSJOR.getJString ();

   END_TRY_BLOCK_VALUE_RETURN (theJNIEnv, NULL)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, MessageMgrCOR, setMesgFileName) (
      JNIEnv *   theJNIEnv,
      _jobject * theMsgMgrCORJObj,
      _jstring * theNameJStr)
   {
   BEGIN_TRY_BLOCK

   JniHandler   theJniHandler (theJNIEnv);
   MessageMgr * theMsgMgr;
   const char * theNameCStr;

   theMsgMgr = theJniHandler.getJavaAccObj (theMsgMgrCORJObj)->asMessageMgr ();

   theNameCStr = theJniHandler.getCString (theNameJStr);

   theMsgMgr->setMesgFileName (theNameCStr);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (_jstring *, MessageMgrCOR, getMesgFileName) (
      JNIEnv *   theJNIEnv,
      _jobject * theMsgMgrCORJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler   theJniHandler (theJNIEnv);
   MessageMgr * theMsgMgr;
   StringJOR    theNameSJOR   (theJNIEnv);

   theMsgMgr = theJniHandler.getJavaAccObj (theMsgMgrCORJObj)->asMessageMgr ();

   theMsgMgr->getMesgFileName (theNameSJOR);

   return theNameSJOR.getJString ();

   END_TRY_BLOCK_VALUE_RETURN (theJNIEnv, NULL)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, MessageMgrCOR, setMesgTimesPrint) (
      JNIEnv *   theJNIEnv,
      _jobject * theMsgMgrCORJObj,
      jint       theMsgNo,
      jint       theCount)
   {
   BEGIN_TRY_BLOCK

   JniHandler   theJniHandler (theJNIEnv);
   MessageMgr * theMsgMgr;

   theMsgMgr = theJniHandler.getJavaAccObj (theMsgMgrCORJObj)->asMessageMgr ();

   theMsgMgr->setMesgTimesPrint (theMsgNo, theCount);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, MessageMgrCOR, setMesgTimesPrintByGroup) (
      JNIEnv *   theJNIEnv,
      _jobject * theMsgMgrCORJObj,
      _jobject * theMsgGroupNoCTCORJObj,
      jint       theCount)
   {
   BEGIN_TRY_BLOCK

   JniHandler   theJniHandler (theJNIEnv);
   MessageMgr * theMsgMgr;
   CaseToken *  theMsgGroupNoCT;

   theMsgMgr = theJniHandler.getJavaAccObj (theMsgMgrCORJObj)->asMessageMgr ();

   theMsgGroupNoCT =
      theJniHandler.getJavaAccObj (theMsgGroupNoCTCORJObj)->asCaseToken ();

   theMsgMgr->setMesgTimesPrintByGroup (theMsgGroupNoCT, theCount);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (jint, MessageMgrCOR, getMesgTimesPrint) (
      JNIEnv *   theJNIEnv,
      _jobject * theMsgMgrCORJObj,
      jint       theMsgNo)
   {
   BEGIN_TRY_BLOCK

   JniHandler   theJniHandler (theJNIEnv);
   MessageMgr * theMsgMgr;
   int          theCount;

   theMsgMgr = theJniHandler.getJavaAccObj (theMsgMgrCORJObj)->asMessageMgr ();

   theMsgMgr->getMesgTimesPrint (theMsgNo, theCount);

   return theCount;

   END_TRY_BLOCK_VALUE_RETURN (theJNIEnv, 0)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, MessageMgrCOR, createAndStoreCppMessageMgr) (
      JNIEnv *   theJNIEnv,
      _jobject * theMsgMgrCORJObj,
      _jobject * theCoordCORJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler    theJniHandler (theJNIEnv);
   Coordinator * theCoord;
   MessageMgr *  theMsgMgr;

   theCoord  = theJniHandler.getJavaAccObj (theCoordCORJObj)->asCoordinator ();

   theMsgMgr = new MessageMgr (theCoord);

   theJniHandler.storeInCOR (theMsgMgrCORJObj, theMsgMgr);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }
