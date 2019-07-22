//------------------------------------------------------------------------------
// WIT-J C++ Header File ExceptionHandler.h.
//
// Contains the declaration of class ExceptionHandler.
//------------------------------------------------------------------------------

#ifndef ExceptionHandler_h
#define ExceptionHandler_h

#include <JniClient.h>

//------------------------------------------------------------------------------
// Class ExceptionHandler
//
// Responsible for working with C++ and Java exceptions.
//
// Class Hierarchy:
//
// JniClient
//    ExceptionHandler
//------------------------------------------------------------------------------

namespace WitJ
{
class ExceptionHandler: public JniClient
   {
   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   static void handleAnyException (JNIEnv * theJNIEnv);
      //
      // Handles any C++ exception thrown in WIT-J.
      // This function is designed to be called only from the catch clause of a
      // canonical try block that implements a Java native method.
      // It should not be called from anywhere else.

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   explicit ExceptionHandler (JNIEnv * theJNIEnv);
      //
      // Constructor.

   ~ExceptionHandler ();
      //
      // Destructor.

   void handleAnyException ();
      //
      // Handles any C++ exception that has been caught in a canonical try
      // block.

   void handleWitErrorExc (WitErrorExc & theWitErrorExc);
      //
      // Handles a WitErrorExc that has been caught as a C++ exception in a
      // canonical try block.

   void handleJThrowable (_jthrowable * theJThrow);
      //
      // Handles a _jthrowable that has been caught as a C++ exception in a
      // canonical try block.

   void raiseJavaException (
         const char * theClassName,
         const char * message = "");
      //
      // Causes JNI to:
      //    Construct a WIT-J/Java Exception whose class name is given by
      //       theClassName.
      //    Pass message to the Exception's ctor.
      //    Arrange for the Exception to be thrown when the currently running
      //       native method returns to Java.
      // Terminates execution if there is already a Java Exception pending.

   void raiseJavaExceptionForJThrowable (_jthrowable * causeJThrow);
      //
      // Causes JNI to:
      //    Construct a Java InternalErrorException indicating that a Java
      //       Throwable (referenced by causeJThrow) has been thrown during the
      //       execution of a JNI function called by the C++ side of WIT-J.
      //    Set the InternalErrorException's cause to the Java Throwable
      //       referenced by causeJThrow.
      //    Arrange for the InternalErrorException to be thrown when the
      //       currently running native method returns to Java.
      // Terminates execution if there is already a Java Exception pending.

   void abortOnJavaException ();
      //
      // Checks for any Java Throwable that's currently pending and if there is
      // one, aborts the program with an error message.

   ExceptionHandler              (const ExceptionHandler &);
   ExceptionHandler & operator = (const ExceptionHandler &);
      //
      // Not implemented; prevents accidental copying and assignment.
   };
};

//
// End of namespace WitJ.

#endif
