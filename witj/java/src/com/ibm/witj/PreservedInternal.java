
//==============================================================================
// WIT-J Java Source File PreservedInternal.java.
//==============================================================================

package com.ibm.witj;

//==============================================================================
// Class PreservedInternal
//
// Similar to PreservedObject, but package-internal.
//
// Class Hierarchy:
//
// PreservedInternal
//    Coordinator
//    AttImp <V>
//       OrdAttImp <V>
//          {Static Nested Classes}
//       SpecAttImp <V>
//          {Static Nested Classes}
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

abstract class PreservedInternal
{
//==============================================================================
// Protected Methods
//==============================================================================

//------------------------------------------------------------------------------
// finalize ()
//
// Override from class Object.
//------------------------------------------------------------------------------

   protected final void finalize () throws Throwable
      {
      assert (! active);

      super.finalize ();
      }

//==============================================================================
// Package-Internal Constructor
//==============================================================================

   PreservedInternal ()
      {
      }

//==============================================================================
// Package-Internal Methods
//==============================================================================

//------------------------------------------------------------------------------
// deactivate ().
//
// "Deactivates" this PreservedInternal, i.e., puts it into an inactive state.
// See the comment at the top of the PreservedObject class declaration.
//------------------------------------------------------------------------------

   void deactivate ()
      {
      assert (active);

      active = false;
      }

//==============================================================================
// Private Fields
//==============================================================================

//------------------------------------------------------------------------------
// active.
//
// True, iff this PreservedInternal is currently active
//------------------------------------------------------------------------------

   private boolean active = true;

//==============================================================================
}
