
//==============================================================================
// WIT-J Java Source File PartCOR.java.
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;

import           java.util.*;

//==============================================================================
// Class PartCOR
//
// "Part C++ Object Representation"
// A Java representation of a C++ Part.
//
// Class Hierarchy:
//
// PreservedInternal
//    JavaAccObjCOR
//       ComponentCOR
//          PartCOR
//==============================================================================

final class PartCOR extends ComponentCOR
{
//==============================================================================
// Package-Internal Constructor
//==============================================================================

   PartCOR (
         CoordinatorCOR theCoordCOR,
         String         thePartName,
         CaseTokenCOR   thePartCatCTCOR)
      {
      createAndStoreCppPart (theCoordCOR, thePartName, thePartCatCTCOR);
      }

//==============================================================================
// Package-Internal Native Methods
//==============================================================================

//------------------------------------------------------------------------------
// setPartName
//
// Sets the PartName of the C++ Part.
//------------------------------------------------------------------------------

   native void setPartName (String thePartName);

//------------------------------------------------------------------------------

   native void getBelowList (ArrayList <Part> theBelowList);

//==============================================================================
// Private Native Methods
//==============================================================================

//------------------------------------------------------------------------------
// createAndStoreCppPart
// 
// Creates a new C++ Part and stores its pointer in this PartCOR.
//------------------------------------------------------------------------------

   private native void createAndStoreCppPart (
      CoordinatorCOR theCoordCOR,
      String         thePartName,
      CaseTokenCOR   thePartCatCTCOR);

//==============================================================================
}
