
//==============================================================================
// WIT-J Java Source File BadCharacterException.java.
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;

//==============================================================================
// Class BadCharacterException
//
// One kind of unchecked Exception thrown by WIT-J.
//
// When WIT-J throws a BadCharacterException, this indicates that:
//
// * The readData method of class Problem found a string in the data file
//   containing a non-UTF-8 character.
//
// * As a result of the throw, WIT-J is now in a non-functional state.
//   No further calls to WIT-J methods should be made for the rest of the
//   program process.
//
// Class Hierarchy:
//
// RuntimeException (java.lang.RuntimeException)
//    WitjException
//       TerminalException
//          BadCharacterException
//==============================================================================

public final class BadCharacterException extends TerminalException
{
//==============================================================================
// Public Methods
//==============================================================================

//------------------------------------------------------------------------------
// Accessor
//------------------------------------------------------------------------------

   public String getOffendingString ()
      {
      return myOffendingString;
      }

//==============================================================================
// Private Constructor
//
// To be called from C++.
//==============================================================================

   private BadCharacterException (String theOffendingString)
      {
      super (
         "   The readData method of class Problem found a string in the data\n"
       + "   file containing a non-UTF-8 character.\n"
       + "\n"
       + "      Offending string (as a Java String): "
       +        theOffendingString + "\n"
       + "\n"
       + "   This indicates an error in the data file.\n");

      myOffendingString = theOffendingString;
      }

//==============================================================================
// Private Fields
//==============================================================================

//------------------------------------------------------------------------------
// myOffendingString
//
// The offending c-string that caused this BadCharacterException to be thrown,
// converted to a Java String.
//------------------------------------------------------------------------------

   final String myOffendingString;

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
