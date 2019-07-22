
//==============================================================================
// WIT-J Java Source File JavaAccObjCOR.java.
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;

//==============================================================================
// Class JavaAccObjCOR
//
// "JavaAccObj C++ Object Representation"
// A Java representation of a C++ JavaAccObj.
//
// An instance of this class has the following responsibilities:
//
// * Store an association to the represented C++ JavaAccObj when the
//   JavaAccObjCOR is being constructed. In most cases, the C++ JavaAccObj will
//   be created by the JavaAccObjCOR.
// * Provide an Java interface to the represented C++ JavaAccObj via native
//   methods.
// * Destroy the represented C++ JavaAccObj when the JavaAccObjCOR is
//   deactivated.
//
// Most concrete subclasses of this class have a private native method called
// "acquireNewCppObject" which creates the C++ JavaAccObj and stores the
// association to it. The acquireNewCppObject method is called by the subclass
// ctor.
//
// Class Hierarchy:
//
// PreservedInternal
//    JavaAccObjCOR
//       CoordinatorCOR
//       AttBldrCOR
//       AttCOR
//       MessageMgrCOR
//       ComponentCOR
//          ProblemCOR
//          PartCOR
//          DemandCOR
//          OperationCOR
//          BomEntryCOR
//          SubstituteCOR
//          BopEntryCOR
//==============================================================================

abstract class JavaAccObjCOR extends PreservedInternal
{
//==============================================================================
// Package-Internal Constructor
//==============================================================================

   JavaAccObjCOR ()
      {
      setUpCppPortion ();
      }

//==============================================================================
// Package-Internal Methods
//==============================================================================

//------------------------------------------------------------------------------
// deactivate
//
// Override from class PreservedInternal.
//------------------------------------------------------------------------------

   void deactivate ()
      {
      deleteCppObject ();

      super.deactivate ();
      }

//==============================================================================
// Private Methods
//==============================================================================

//------------------------------------------------------------------------------
// setUpCppPortion ()
//
// Sets up the C++ side of WIT-J.
//------------------------------------------------------------------------------

   private static synchronized void setUpCppPortion ()
      {
      if (setUpCalled)
         return;

      try
         {
         System.loadLibrary ("witj");
         }

      catch (UnsatisfiedLinkError theLinkErr)
         {
         throw
            TerminalAppException.
               newInstanceForLoadLibraryFailure (theLinkErr);
         }

      setUpJni ();

      setUpCalled = true;
      }

//------------------------------------------------------------------------------
// setUpJni ().
//
// Sets up the interaction between WIT-J/C++ and JNI.
// This method is to be called only from setUpCppPortion (), which is statically
// synchronized.
//------------------------------------------------------------------------------

   private static native void setUpJni ();

//------------------------------------------------------------------------------
// deleteCppObject ().
//
// Deletes the C++ JavaAccObj represented by this JavaAccObjCOR and sets the
// pointer to NULL.
//------------------------------------------------------------------------------

   private native void deleteCppObject ();

//==============================================================================
// Private Fields
//==============================================================================

//------------------------------------------------------------------------------
// setUpCalled
//
// True, iff setUpCppPortion () has been called at least once.
//------------------------------------------------------------------------------

   private static boolean setUpCalled = false;

//------------------------------------------------------------------------------
// myCppJavaObjRepAsLong.
//
// A pointer to the C++ JavaObjRep represented by this JavaAccObjCOR, cast as a
// long.
//------------------------------------------------------------------------------

   private long myCppJavaObjRepAsLong = 0;

//==============================================================================
}
