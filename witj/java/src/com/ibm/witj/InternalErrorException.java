
//==============================================================================
// WIT-J Java Source File InternalErrorException.java
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;

//==============================================================================
// Class InternalErrorException
//
// One kind of unchecked Exception thrown by WIT-J.
//
// When WIT-J throws a InternalErrorException, this indicates that:
//
// * A bug in WIT-J has been detected.
// * As a result of the throw, WIT-J is now in a non-functional state.
//   No further calls to WIT-J methods should be made for the rest of the
//   program process.
//
// Class Hierarchy:
//
// RuntimeException (java.lang.RuntimeException)
//    WitjException
//       TerminalException
//          InternalErrorException
//==============================================================================

public final class InternalErrorException extends TerminalException
{
//==============================================================================
// Package-Internal Methods
//==============================================================================

//------------------------------------------------------------------------------
// newInstanceForJavaAssertionError (theAssertErr)
//
// Constructs and returns a new InternalErrorException to be thrown when a Java
// AssertionError, theAssertErr, has been thrown. Sets theAssertErr as the
// cause of the InternalErrorException.
//------------------------------------------------------------------------------

   static InternalErrorException newInstanceForJavaAssertionError (
         AssertionError theAssertErr)
      {
      InternalErrorException theInternalErrorExc;

      theInternalErrorExc =
         new InternalErrorException (
            "   An AssertionError was thrown from the Java side of WIT-J.");

      theInternalErrorExc.initCause (theAssertErr);

      return theInternalErrorExc;
      }

//------------------------------------------------------------------------------
// newInstanceForUnexpJavaThrowable (theThrowable)
//
// Constructs and returns a new InternalErrorException to be thrown when a
// Throwable (theThrowable) of an unexpected class has been thrown from the Java
// side of WIT-J. Sets theThrowable as the cause of the InternalErrorException.
//------------------------------------------------------------------------------

   static InternalErrorException newInstanceForUnexpJavaThrowable (
         Throwable theThrowable)
      {
      InternalErrorException theInternalErrorExc;

      theInternalErrorExc =
         new InternalErrorException (
            "   An instance of an unexpected Throwable class was thrown from\n"
          + "   the Java side of WIT-J:\n"
          + "\n"
          + "      " + theThrowable.getClass ());

      theInternalErrorExc.initCause (theThrowable);

      return theInternalErrorExc;
      }

//==============================================================================
// Private Constructor
//
// Called from this class and from C++.
//==============================================================================

   private InternalErrorException (String message)
      {
      super (
             message + "\n"
       +    "\n"
       + "   This indicates an internal error (bug) in WIT-J.\n");
      }

//==============================================================================
// Private Methods
//==============================================================================

//------------------------------------------------------------------------------
// newInstanceForThrowableFromJni (theThrowable)
//
// Constructs and returns a new InternalErrorException to be thrown when a Java
// Throwable (theThrowable) has been thrown during the execution of a JNI
// function called by the C++ side of WIT-J. Sets theThrowable as the cause of
// the InternalErrorException.
//------------------------------------------------------------------------------

   private static InternalErrorException newInstanceForThrowableFromJni (
         Throwable theThrowable)
      {
      InternalErrorException theInternalErrorExc;

      theInternalErrorExc =
         new InternalErrorException (
            "   A Java Throwable was thrown during the execution of a JNI\n"
          + "   function called by the C++ side of WIT-J:\n"
          + "\n"
          + "      " + theThrowable.getClass ());

      theInternalErrorExc.initCause (theThrowable);

      return theInternalErrorExc;
      }

//==============================================================================
// Private Fields
//==============================================================================

//------------------------------------------------------------------------------
// serialVersionUID
//
// This field is required in order to avoid a compiler warning whenever an
// instance is constructed, because this class inherits from class Throwable,
// which implements the interface Serializable.
//------------------------------------------------------------------------------

   private static final long serialVersionUID = 1L;

//==============================================================================
}
