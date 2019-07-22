
//==============================================================================
// WIT-J Java Source File ThreadedObject.java.
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;

//==============================================================================
// Class ThreadedObject
//
// A ThreadedObject is a public object that's associated with a specific Thread,
// specifically, the Thread in which it was created.
// Use of a ThreadedObject is limited to its associated Thread;
// using a ThreadedObject from outside its associated Thread triggers an
// exception.
//
// Class Hierarchy:
//
// ThreadedObject
//    PreservedObject
//       MessageMgr
//       Component
//          Problem
//          Part
//          Demand
//          Operation
//          BomEntry
//          Substitute
//          BopEntry
//==============================================================================

public abstract class ThreadedObject
{
//==============================================================================
// Public Methods
//==============================================================================

//------------------------------------------------------------------------------
// Accessors.
//------------------------------------------------------------------------------

   public final synchronized Thread getThread ()
      {
      try
         {
         notAllowedWhenTerminated ("ThreadedObject.getThread");

         return myThread;
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//==============================================================================
// Package-Internal Constructor
//==============================================================================

   ThreadedObject ()
      {
      myThread = Thread.currentThread ();
      }

//==============================================================================
// Package-Internal Methods
//==============================================================================

//------------------------------------------------------------------------------
// checkThread (methName)
//
// Verifies that the current Thread is the Thread associated with this
// ThreadedObject.
// Verifies that WIT-J has not been terminated.
//
// Arguments:
//    methName: The name of the method.
//------------------------------------------------------------------------------

   final void checkThread (String methName)
      {
      notAllowedWhenTerminated (methName);

      if (Thread.currentThread () != myThread)
         throw
            new StatusQuoException (
               "   A method of a ThreadedObject was invoked from outside the\n"
             + "   ThreadedObject's associated Thread.\n"
             + "\n"
             + "      ThreadedObject:    " + this     + "\n"
             + "      Method:            " + methName + "\n"
             + "      Associated Thread: " + myThread + "\n"
             + "      Current    Thread: " + Thread.currentThread ());
      }

//------------------------------------------------------------------------------
// checkThread (theArg, argName, methName)
//
// Verifies that the current Thread is the Thread associated with a
// ThreadedObject argument to a method.
// Verifies the argument is non-null.
//
// Arguments:
//    theArg:   The argument being checked.
//    argName:  The name of the argument being checked.
//    methName: The name of the method.
//------------------------------------------------------------------------------

   static void checkThread (
         ThreadedObject theArg,
         String         argName,
         String         methName)
      {
      mustBeNonNull (theArg, argName, methName);

      if (Thread.currentThread () != theArg.myThread)
         throw
            new StatusQuoException (
               "   A ThreadedObject was passed as an argument to a method\n"
             + "   from outside the ThreadedObject's associated Thread.\n"
             + "\n"
             + "      Method:            " + methName        + "\n"
             + "      Argument:          " + argName         + "\n"
             + "      Argument Value:    " + theArg          + "\n"
             + "      Associated Thread: " + theArg.myThread + "\n"
             + "      Current    Thread: " + Thread.currentThread ());
      }

//==============================================================================
// Private Fields
//==============================================================================

//------------------------------------------------------------------------------
// myThread
//
// The Thread associated with this ThreadedObject, i.e., the Thread in which it
// was created.
//------------------------------------------------------------------------------

   private final Thread myThread;

//==============================================================================
}
