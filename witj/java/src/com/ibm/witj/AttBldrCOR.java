
//==============================================================================
// WIT-J Java Source File AttBldrCOR.java.
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;

//==============================================================================
// Class AttBldrCOR
//
// "AttBldr C++ Object Representation"
// A Java representation of a C++ AttBldr.
//
// Class Hierarchy:
//
// PreservedInternal
//    JavaAccObjCOR
//       AttBldrCOR
//==============================================================================

final class AttBldrCOR extends JavaAccObjCOR
{
//==============================================================================
// Package-Internal Constructor
//==============================================================================

   AttBldrCOR (CoordinatorCOR theCoordCOR)
      {
      createAndStoreCppAttBldr (theCoordCOR);
      }

//==============================================================================
// Package-Internal Methods
//==============================================================================

//------------------------------------------------------------------------------
// hasAttFor (scopedAttName)
//
// Returns true, iff the C++ AttBldr that belongs to this AttBldrCOR currently
// owns a C++ Att whose scoped name matches scopedAttName.
//------------------------------------------------------------------------------

   native boolean hasAttFor (String scopedAttName);

//------------------------------------------------------------------------------
// getAllAttNames
//
// Returns a String listing the names of all C++ Atts owned by the C++ AttBldr.
//------------------------------------------------------------------------------

   native String getAllAttNames ();

//==============================================================================
// Private Methods
//==============================================================================

//------------------------------------------------------------------------------
// createAndStoreCppAttBldr (theCoordCOR).
// 
// Creates a new C++ AttBldr and stores its pointer in this AttBldrCOR.
//------------------------------------------------------------------------------

   private native void createAndStoreCppAttBldr (CoordinatorCOR theCoordCOR);

//==============================================================================
}
