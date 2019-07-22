
//==============================================================================
// WIT-J Java Source File BopEntryCOR.java.
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;

//==============================================================================
// Class BopEntryCOR
//
// "BopEntry C++ Object Representation"
// A Java representation of a C++ BopEntry.
//
// Class Hierarchy:
//
// PreservedInternal
//    JavaAccObjCOR
//       ComponentCOR
//          BopEntryCOR
//==============================================================================

final class BopEntryCOR extends ComponentCOR
{
//==============================================================================
// Package-Internal Constructor
//==============================================================================

   BopEntryCOR (OperationCOR theOpnCOR, int theBopIdx, PartCOR thePartCOR)
      {
      createAndStoreCppBopEntry (theOpnCOR, theBopIdx, thePartCOR);
      }

//==============================================================================
// Package-Internal Native Methods
//==============================================================================

   native int getBopIndex ();

//==============================================================================
// Private Methods
//==============================================================================

//------------------------------------------------------------------------------
// createAndStoreCppBopEntry
// 
// Creates a new C++ BopEntry and stores its pointer in this BopEntryCOR.
//------------------------------------------------------------------------------

   private native void createAndStoreCppBopEntry (
      OperationCOR theOpnCOR,
      int          theBopIdx,
      PartCOR      thePartCOR);

//==============================================================================
}
