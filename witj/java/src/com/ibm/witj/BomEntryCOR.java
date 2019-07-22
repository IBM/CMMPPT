
//==============================================================================
// WIT-J Java Source File BomEntryCOR.java.
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;

//==============================================================================
// Class BomEntryCOR
//
// "BomEntry C++ Object Representation"
// A Java representation of a C++ BomEntry.
//
// Class Hierarchy:
//
// PreservedInternal
//    JavaAccObjCOR
//       ComponentCOR
//          BomEntryCOR
//==============================================================================

final class BomEntryCOR extends ComponentCOR
{
//==============================================================================
// Package-Internal Constructor
//==============================================================================

   BomEntryCOR (OperationCOR theOpnCOR, int theBomIdx, PartCOR thePartCOR)
      {
      createAndStoreCppBomEntry (theOpnCOR, theBomIdx, thePartCOR);
      }

//==============================================================================
// Package-Internal Native Methods
//==============================================================================

   native int getBomIndex ();

//==============================================================================
// Private Native Methods
//==============================================================================

//------------------------------------------------------------------------------
// createAndStoreCppBomEntry
// 
// Creates a new C++ BomEntry and stores its pointer in this BomEntryCOR.
//------------------------------------------------------------------------------

   private native void createAndStoreCppBomEntry (
      OperationCOR theOpnCOR,
      int          theBomIdx,
      PartCOR      thePartCOR);

//==============================================================================
}
