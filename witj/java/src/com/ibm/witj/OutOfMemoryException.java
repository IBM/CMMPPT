
//==============================================================================
// WIT-J Java Source File OutOfMemoryException.java.
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;

//==============================================================================
// Class OutOfMemoryException
//
// One kind of unchecked Exception thrown by WIT-J.
//
// When WIT-J throws an OutOfMemoryException, this indicates that:
//
// * WIT-J has run out of memory.
// * As a result of the throw, WIT-J is now in a non-functional state.
//   No further calls to WIT-J methods should be made for the rest of the
//   program process.
//
// Class Hierarchy:
//
// RuntimeException (java.lang.RuntimeException)
//    WitjException
//       TerminalException
//          OutOfMemoryException
//==============================================================================

public final class OutOfMemoryException extends TerminalException
{
//==============================================================================
// Package-Internal Constructors
//==============================================================================

   OutOfMemoryException (OutOfMemoryError theMemoryErr)
      {
      super (
         "   WIT-J has run out of memory.\n"
       +    "\n"
       + "   This occurred in WIT-J's Java code.\n");

      initCause (theMemoryErr);
      }

//------------------------------------------------------------------------------

   OutOfMemoryException (String message)
      {
      super (
         "   WIT-J has run out of memory.\n"
       +    "\n"
       + "   This occurred in " + message + ".\n");
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
