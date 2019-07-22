
//==============================================================================
// WIT-J Java Source File MessageMgrCOR.java.
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;

//==============================================================================
// Class MessageMgrCOR
//
// "MessageMgr C++ Object Representation"
// A Java representation of a C++ MessageMgr.
//
// Class Hierarchy:
//
// PreservedInternal
//    JavaAccObjCOR
//       MessageMgrCOR
//==============================================================================

final class MessageMgrCOR extends JavaAccObjCOR
{
//==============================================================================
// Package-Internal Constructor
//==============================================================================

   MessageMgrCOR (CoordinatorCOR theCoordCOR)
      {
      createAndStoreCppMessageMgr (theCoordCOR);
      }

//==============================================================================
// Package-Internal Methods
//==============================================================================

//------------------------------------------------------------------------------
// flushFile
//
// Flushes WIT's message file.
//------------------------------------------------------------------------------

   native void flushFile ();

//------------------------------------------------------------------------------
// setQuiet
//
// Setter.
//------------------------------------------------------------------------------

   native void setQuiet (boolean quietVal);

//------------------------------------------------------------------------------
// isQuiet
//
// Accessor.
//------------------------------------------------------------------------------

   native boolean isQuiet ();

//------------------------------------------------------------------------------
// Wrappers for corresponding WIT API functions.
//------------------------------------------------------------------------------

   native void   setMesgFileAccessMode (String theValue);
   native String getMesgFileAccessMode ();
   native void   setMesgFileName       (String theValue);
   native String getMesgFileName       ();

   native void   setMesgTimesPrint        (int          theMsgNo,
                                           int          theCount);
   native void   setMesgTimesPrintByGroup (CaseTokenCOR theMsgGroupNoCTCOR,
                                           int          theCount);
   native int    getMesgTimesPrint        (int          theMsgNo);

//==============================================================================
// Private Methods
//==============================================================================

//------------------------------------------------------------------------------
// createAndStoreCppMessageMgr (theCoordCOR)
// 
// Creates a new C++ MessageMgr and stores its pointer in this MessageMgrCOR.
//------------------------------------------------------------------------------

   private native void createAndStoreCppMessageMgr (CoordinatorCOR theCoordCOR);

//==============================================================================
}
