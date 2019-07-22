
//==============================================================================
// WIT-J Java Source File SubstituteCOR.java.
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;

//==============================================================================
// Class SubstituteCOR
//
// "Substitute C++ Object Representation"
// A Java representation of a C++ Substitute.
//
// Class Hierarchy:
//
// PreservedInternal
//    JavaAccObjCOR
//       ComponentCOR
//          SubstituteCOR
//==============================================================================

final class SubstituteCOR extends ComponentCOR
{
//==============================================================================
// Package-Internal Constructor
//==============================================================================

   SubstituteCOR (BomEntryCOR theBomEntCOR, int theSubIdx, PartCOR thePartCOR)
      {
      createAndStoreCppSubstitute (theBomEntCOR, theSubIdx, thePartCOR);
      }

//==============================================================================
// Package-Internal Native Methods
//==============================================================================

   native int getSubIndex ();

//==============================================================================
// Private Methods
//==============================================================================

//------------------------------------------------------------------------------
// createAndStoreCppSubstitute
// 
// Creates a new C++ Substitute and stores its pointer in this SubstituteCOR.
//------------------------------------------------------------------------------

   private native void createAndStoreCppSubstitute (
      BomEntryCOR theBomEntCOR,
      int         theSubIdx,
      PartCOR     thePartCOR);

//==============================================================================
}
