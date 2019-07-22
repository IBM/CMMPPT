//------------------------------------------------------------------------------
// WIT-J C++ Source File ExceptionHandler.C.
//
// Contains the implementation of class ExceptionHandler.
//------------------------------------------------------------------------------

#include <ExceptionHandler.h>
#include <PreJavaException.h>

#include              <wit.h>

#include           <string>

//------------------------------------------------------------------------------

void WitJ::ExceptionHandler::handleAnyException (JNIEnv * theJNIEnv)
   {
   ExceptionHandler theHandler (theJNIEnv);

   theHandler.handleAnyException ();
   }

//------------------------------------------------------------------------------

WitJ::ExceptionHandler::ExceptionHandler (JNIEnv * theJNIEnv):

      JniClient (theJNIEnv)
   {
   }

//------------------------------------------------------------------------------

WitJ::ExceptionHandler::~ExceptionHandler ()
   {
   }

//------------------------------------------------------------------------------

void WitJ::ExceptionHandler::handleAnyException ()
   {
   try
      {
      throw;
      }

   catch (WitErrorExc & theWitErrorExc)
      {
      handleWitErrorExc (theWitErrorExc);
      }

   catch (PreJavaException & thePreJavaException)
      {
      raiseJavaException (
         thePreJavaException.getJavaExcClassName (),
         thePreJavaException.getMessage          ());
      }

   catch (_jthrowable * theJThrow)
      {
      handleJThrowable (theJThrow);
      }

   catch (std::bad_alloc &)
      {
      raiseJavaException ("OutOfMemoryException", "WIT-J's C++ code");
      }

   catch (...)
      {
      raiseJavaException (
         "InternalErrorException", 
         "   A C++ exception of an unexpected type "
            "was thrown from the C++ side of WIT-J.");
      }
   }

//------------------------------------------------------------------------------

void WitJ::ExceptionHandler::handleWitErrorExc (WitErrorExc & theWitErrorExc)
   {
   if (theWitErrorExc.retCode () == WitSEVERE_RC)
      {
      if (theWitErrorExc.mesgNum () == 404)
         {
         raiseJavaException ("OutOfMemoryException", "WIT");
         }
      else
         {
         raiseJavaException (
            "TerminalAppException",
            "   WIT has issued a severe error message.");
         }
      }
   else if (theWitErrorExc.retCode () == WitFATAL_RC)
      {
      raiseJavaException (
         "InternalErrorException", 
         "   WIT has issued a fatal error message.");
      }
   else
      {
      ABORTING_ASSERT (false);
      }
   }

//------------------------------------------------------------------------------

void WitJ::ExceptionHandler::handleJThrowable (_jthrowable * theJThrow)
   {
   _jclass * theOutOfMemoryErrorJCl;
   bool      memoryCase;

   theOutOfMemoryErrorJCl =
      getJNIEnv ()->FindClass ("java/lang/OutOfMemoryError");

   abortOnJavaException ();

   memoryCase = getJNIEnv ()->IsInstanceOf (theJThrow, theOutOfMemoryErrorJCl);

   if (memoryCase)
      {
      raiseJavaException ("OutOfMemoryException", "JNI");
      }
   else
      {
      raiseJavaExceptionForJThrowable (theJThrow);
      }

   deleteLocalRef (theOutOfMemoryErrorJCl);
   }

//------------------------------------------------------------------------------

void WitJ::ExceptionHandler::raiseJavaException (
      const char * theClassName,
      const char * message)
   {
   std::string theClassDesc;
   _jclass *   theJClass;
   jint        throwResult;

   theClassDesc  = "com/ibm/witj/";

   theClassDesc += theClassName;

   theJClass     = getJNIEnv ()->FindClass (theClassDesc.c_str ());

   abortOnJavaException ();

   throwResult   = getJNIEnv ()->ThrowNew (theJClass, message);

   ABORTING_ASSERT (throwResult == 0);

   deleteLocalRef (theJClass);
   }

//------------------------------------------------------------------------------

void WitJ::ExceptionHandler::raiseJavaExceptionForJThrowable (
      _jthrowable * causeJThrow)
   {
   _jclass *     theInternalErrorExcJCl;
   _jmethodID *  newInstanceForThrowableFromJniID;
   _jobject *    theInternalErrorExcJObj;
   _jthrowable * theInternalErrorExcJThrow;
   jint          throwResult;

   theInternalErrorExcJCl =
      getJNIEnv ()->FindClass ("com/ibm/witj/InternalErrorException");

   abortOnJavaException ();

   newInstanceForThrowableFromJniID =
      getJNIEnv ()->
         GetStaticMethodID (
            theInternalErrorExcJCl,
            "newInstanceForThrowableFromJni",
            "(Ljava/lang/Throwable;)Lcom/ibm/witj/InternalErrorException;");

   abortOnJavaException ();

   theInternalErrorExcJObj =
      getJNIEnv ()->
         CallStaticObjectMethod (
            theInternalErrorExcJCl,
            newInstanceForThrowableFromJniID,
            causeJThrow);

   abortOnJavaException ();

   theInternalErrorExcJThrow =
      static_cast <_jthrowable *> (theInternalErrorExcJObj);

   throwResult = getJNIEnv ()->Throw (theInternalErrorExcJThrow);

   ABORTING_ASSERT (throwResult == 0);

   deleteLocalRef (theInternalErrorExcJCl);
   deleteLocalRef (theInternalErrorExcJObj);
   }

//------------------------------------------------------------------------------

void WitJ::ExceptionHandler::abortOnJavaException ()
   {
   if (getJNIEnv ()->ExceptionCheck ())
      {
      getJNIEnv ()->ExceptionDescribe ();

      ABORTING_ASSERT (false);
      }
   }
