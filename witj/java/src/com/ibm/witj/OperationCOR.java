
//==============================================================================
// WIT-J Java Source File OperationCOR.java.
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;

//==============================================================================
// Class OperationCOR
//
// "Operation C++ Object Representation"
// A Java representation of a C++ Operation.
//
// Class Hierarchy:
//
// PreservedInternal
//    JavaAccObjCOR
//       ComponentCOR
//          OperationCOR
//==============================================================================

final class OperationCOR extends ComponentCOR
{
//==============================================================================
// Package-Internal Constructor
//==============================================================================

   OperationCOR (CoordinatorCOR theCoordCOR, String theOpnName)
      {
      createAndStoreCppOperation (theCoordCOR, theOpnName);
      }

//==============================================================================
// Package-Internal Native Methods
//==============================================================================

//------------------------------------------------------------------------------
// setOperationName
//
// Sets the OperationName of the C++ Operation.
//------------------------------------------------------------------------------

   native void setOperationName (String theOpnName);

//==============================================================================
// Private Methods
//==============================================================================

//------------------------------------------------------------------------------
// createAndStoreCppOperation
// 
// Creates a new C++ Operation and stores its pointer in this OperationCOR.
//------------------------------------------------------------------------------

   private native void createAndStoreCppOperation (
      CoordinatorCOR theCoordCOR,
      String         theOpnName);

//==============================================================================
}
