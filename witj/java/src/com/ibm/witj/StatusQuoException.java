
//==============================================================================
// WIT-J Java Source File StatusQuoException.java.
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;

//==============================================================================
// Class StatusQuoException
//
// One kind of unchecked Exception thrown by WIT-J.
//
// When WIT-J throws a StatusQuoException, this indicates that:
//
// * The Exception seems to be due to an error in the application program.
// * The state of WIT-J has not been changed since the public method call that
//   caused the Exception to be thrown.
//
// Class Hierarchy:
//
// RuntimeException (java.lang.RuntimeException)
//    WitjException
//       StatusQuoException
//==============================================================================

public final class StatusQuoException extends WitjException
{
//==============================================================================
// Package-Internal Constructor
//==============================================================================

   StatusQuoException (String message)
      {
      super (
         "\n"
       + "\n"
       + message + "\n"
       + "\n"
       + "   This indicates an error in the application program.\n"
       + "\n"
       + "   The state of WIT-J has not been changed.\n");
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
