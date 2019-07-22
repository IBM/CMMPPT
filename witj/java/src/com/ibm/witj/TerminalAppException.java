
//==============================================================================
// WIT-J Java Source File TerminalAppException.java.
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;

//==============================================================================
// Class TerminalAppException
//
// One kind of unchecked Exception thrown by WIT-J.
//
// When WIT-J throws a TerminalAppException, this indicates that:
//
// * The Exception seems to be due to an error in the application program.
// * As a result of the throw, WIT-J is now in a non-functional state.
//   No further calls to WIT-J methods should be made for the rest of the
//   program process.
//
// Class Hierarchy:
//
// RuntimeException (java.lang.RuntimeException)
//    WitjException
//       TerminalException
//          TerminalAppException
//==============================================================================

public final class TerminalAppException extends TerminalException
{
//==============================================================================
// Package-Internal Methods
//==============================================================================

//------------------------------------------------------------------------------
// newInstanceForLoadLibraryFailure (theLinkErr)
//
// Constructs and returns a new TerminalAppException to be thrown when the
// call to System.loadLibrary ("witj") threw an UnsatisfiedLinkError.
//------------------------------------------------------------------------------

   static TerminalAppException newInstanceForLoadLibraryFailure (
         UnsatisfiedLinkError theLinkErr)
      {
      String               thePath;
      TerminalAppException theTermAppExc;

      thePath = System.getProperty ("java.library.path");

      theTermAppExc  =
         new TerminalAppException (
            "   An UnsatisfiedLinkError was thrown when the Java portion of\n"
          + "   WIT-J attempted to load the native library \"witj\" for the\n"
          + "   C++ portion of WIT-J. The error message was:\n"
          + "\n"
          + "      " + theLinkErr.getMessage () + "\n"
          + "\n"
          + "   This may indicate a problem with the native library path:\n"
          + "\n"
          + "      java.library.path = \"" + thePath + "\"");

      theTermAppExc.initCause (theLinkErr);

      return theTermAppExc;
      }

//------------------------------------------------------------------------------
// newInstanceForUnsatisfiedLink (theLinkErr)
//
// Constructs and returns a new TerminalAppException to be thrown when an
// UnsatisfiedLinkError was thrown other than by System.loadLibrary.
//------------------------------------------------------------------------------

   static TerminalAppException newInstanceForUnsatisfiedLink (
         UnsatisfiedLinkError theLinkErr)
      {
      String               thePath;
      TerminalAppException theTermAppExc;

      thePath = System.getProperty ("java.library.path");

      theTermAppExc  =
         new TerminalAppException (
            "   An UnsatisfiedLinkError was thrown within the Java portion of\n"
          + "   WIT-J. The error message was:\n"
          + "\n"
          + "      " + theLinkErr.getMessage () + "\n"
          + "\n"
          + "   This may indicate a problem linking to the native method\n"
          + "   implementations in the native library \"witj\" for the C++\n"
          + "   portion of WIT-J.\n"
          + "\n"
          + "      java.library.path = \"" + thePath + "\"");

      theTermAppExc.initCause (theLinkErr);

      return theTermAppExc;
      }

//==============================================================================
// Private Constructor
//
// To be called from C++.
//==============================================================================

   private TerminalAppException (String message)
      {
      super (
         message + "\n"
       + "\n"
       + "   This indicates an error in the application program.\n");
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
