
//==============================================================================
// WIT-J Java Source File PreservedObject.java.
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;

import       java.util.*;

//==============================================================================
// Class PreservedObject
//
// A PreservedObject is a public object that is withheld from
// garbage-collection until it is deactivated.
//
// At any point in time, a PreservedObject is in an "active" state or an
// "inactive" state:
//
// * Initially, a PreservedObject is "active" (just after construction).
// * When its deactivate method is called, it enters into its "inactive" state.
// * From that point, on it remains in its "inactive" state.
//
// Most of the public methods of any PreservedObject subclass are allowed to be
// called only when the PreservedObject is in an active state. The exceptions
// to this rule are the following methods, which may be called regardless of
// whether or not the PreservedObject is active:
//
// * isActive ()
// * All methods inherited from class Object
//
// If  any other public method is called when the PreservedObject is inactive,
// a StatusQuoException is thrown.
// 
// When a PreservedObject is in an inactive state, the following conditions
// hold:
// 
// * It no longer owns any C++ objects.
// * All PreservedObjects that it owns are inactive.
// * There are no references to it from active PreservedObjects.
// * There are no static references to it from WIT-J.
//
// The deactivate method is responsible for putting the PreservedObject into
// this state.
//
// When a PreservedObject is active, WIT-J maintains a static reference to it
// (perhaps indirectly). This prevents the PreservedObject from being garbage
// collected while it is active, which, in turn, prevents the C++ objects
// that it owns from becoming memory leaks. The deactivate method removes this
// static reference and deletes all C++ objects that the PreservedObject owns.
// Thus when a PreservedObject is inactive and all references to it from the
// application program are removed, it is ready for safe garbage collection.
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

public abstract class PreservedObject extends ThreadedObject
{
//==============================================================================
// Public Methods
//==============================================================================

//------------------------------------------------------------------------------
// toString ().
//
// Override from class Object.
//------------------------------------------------------------------------------

   public final synchronized String toString ()
      {
      try
         {
         notAllowedWhenTerminated ("PreservedObject.toString");

         return
            active?
               toStringActive ():
               "Inactive " + getClass ().getSimpleName ();
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// Accessors.
//------------------------------------------------------------------------------

   public final boolean isActive ()
      {
      try
         {
         checkThread ("PreservedObject.isActive");

         return active;
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//==============================================================================
// Protected Methods
//==============================================================================

//------------------------------------------------------------------------------
// finalize ()
//
// Override from class Object.
//------------------------------------------------------------------------------

   protected final void finalize () throws Throwable
      {
      assert (! active);

      super.finalize ();
      }

//==============================================================================
// Package-Internal Constructor
//==============================================================================

   PreservedObject (Coordinator theCoord)
      {
      myCoordinator = theCoord;
      }

//==============================================================================
// Package-Internal Methods
//==============================================================================

//------------------------------------------------------------------------------
// toStringActive ().
//
// Returns the value for toString () for this PreservedObject when it is active.
//------------------------------------------------------------------------------

   abstract String toStringActive ();

//------------------------------------------------------------------------------
// deactivate ().
//
// "Deactivates" this PreservedObject, i.e., puts it into an inactive state.
// See the comment at the top of this class declaration.
//------------------------------------------------------------------------------

   void deactivate ()
      {
      assert (active);

      active = false;
      }

//------------------------------------------------------------------------------
// mustBeActive (methName)
//
// Verifies that this PreservedObject is currently active.
// Performs a Thread-check on this PreservedObject.
//
// Arguments:
//    methName: The name of the method.
//------------------------------------------------------------------------------

   final void mustBeActive (String methName)
      {
      checkThread (methName);

      if (! active)
         throw
            new StatusQuoException (
               "   A method that requires an active PreservedObject was\n"
             + "   called with an inactive PreservedObject.\n"
             + "\n"
             + "      Method: " + methName + "\n"
             + "      Object: " + this);
      }

//------------------------------------------------------------------------------
// mustBeActive (theArg, argName, methName)
//
// Verifies that a PreservedObject argument to a method is non-null and active.
// Performs a Thread-check on theArg.
//
// Arguments:
//    theArg:   The argument being checked.
//    argName:  The name of the argument being checked.
//    methName: The name of the method.
//------------------------------------------------------------------------------

   static void mustBeActive (
         PreservedObject theArg,
         String          argName,
         String          methName)
      {
      checkThread (theArg, argName, methName);

      if (! theArg.active)
         throw
            new StatusQuoException (
               "   An inactive PreservedObject was passed as an argument to\n"
             + "   a method that requires an active PreservedObject.\n"
             + "\n"
             + "      Method:         " + methName + "\n"
             + "      Argument:       " + argName  + "\n"
             + "      Argument Value: " + theArg);
      }

//------------------------------------------------------------------------------
// removeInactive (theArrayList)
//
// Removes all inactive PreservedObjects from theArrayList.
// Reduces the capacity of theArrayList to its new size.
//------------------------------------------------------------------------------

   static
      <P extends PreservedObject>
         void removeInactive (ArrayList <P> theArrayList)
      {
      ArrayList <P> activeObjects;

      activeObjects = new ArrayList <P> (theArrayList.size ());

      for (P theP: theArrayList)
         {
         if (theP.isPIActive ())
            activeObjects.add (theP);
         }

      theArrayList.clear          ();
      theArrayList.trimToSize     ();
      theArrayList.ensureCapacity (activeObjects.size ());
      
      for (P theP: activeObjects)
         {
         theArrayList.add (theP);
         }
      }

//------------------------------------------------------------------------------
// Accessors.
//------------------------------------------------------------------------------

   final boolean isPIActive ()
      {
      return active;
      }

   final Coordinator getCoordinator ()
      {
      return myCoordinator;
      }

//==============================================================================
// Private Fields
//==============================================================================

//------------------------------------------------------------------------------
// active.
//
// True, iff this PreservedObject is currently active
//------------------------------------------------------------------------------

   private boolean active = true;

//------------------------------------------------------------------------------
// myCoordinator
//
// The Coordinator for this PreservedObject.
//------------------------------------------------------------------------------

   private final Coordinator myCoordinator;

//==============================================================================
}
