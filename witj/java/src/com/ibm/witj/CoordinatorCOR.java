
//==============================================================================
// WIT-J Java Source File CoordinatorCOR.java.
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;

//==============================================================================
// Class CoordinatorCOR
//
// "Coordinator C++ Object Representation"
// A Java representation of a C++ Coordinator.
//
// Class Hierarchy:
//
// PreservedInternal
//    JavaAccObjCOR
//       CoordinatorCOR
//==============================================================================

final class CoordinatorCOR extends JavaAccObjCOR
{
//==============================================================================
// Package-Internal Constructor
//==============================================================================

    CoordinatorCOR ()
      {
      createAndStoreCppCoordinator ();
      }

//==============================================================================
// Package-Internal Native Methods
//==============================================================================

   native void setUpCaseTokens   ();
   native void restoreAfterPurge ();
   native void clearData         ();

//==============================================================================
// Private Native Methods
//==============================================================================

//------------------------------------------------------------------------------
// createAndStoreCppCoordinator ()
// 
// Creates a new C++ Coordinator and stores its pointer in this CoordinatorCOR.
//------------------------------------------------------------------------------

   private native void createAndStoreCppCoordinator ();

//==============================================================================
}

