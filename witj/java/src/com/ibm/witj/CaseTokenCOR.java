
//==============================================================================
// WIT-J Java Source File CaseTokenCOR.java.
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;

//==============================================================================
// Class CaseTokenCOR
//
// "CaseToken C++ Object Representation"
// A Java representation of a C++ CaseToken.
//
// Class Hierarchy:
//
// PreservedInternal
//    JavaAccObjCOR
//       CaseTokenCOR
//==============================================================================

final class CaseTokenCOR extends JavaAccObjCOR
{
//==============================================================================
// Package-Internal Constructor
//==============================================================================

   CaseTokenCOR (CoordinatorCOR theCoordCOR, String theName)
      {
      createAndStoreCppCaseToken (theCoordCOR, theName);
      }

//==============================================================================
// Private Methods
//==============================================================================

//------------------------------------------------------------------------------
// createAndStoreCppCaseToken
// 
// Creates a new C++ CaseToken and stores its pointer in this CaseTokenCOR.
//------------------------------------------------------------------------------

   private native void createAndStoreCppCaseToken (
      CoordinatorCOR theCoordCOR,
      String         theName);

//==============================================================================
}
