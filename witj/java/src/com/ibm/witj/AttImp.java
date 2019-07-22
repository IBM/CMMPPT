
//==============================================================================
// WIT-J Java Source File AttImp.java.
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;

//==============================================================================
// Class AttImp <V>
//
// "Attribute Implementation"
// Fulfills the main responsibilities of an Attribute <V>
// Each AttImp corresponds to one Problem and one Attribute <V>.
//
// Class Hierarchy:
//
// PreservedInternal
//    AttImp <V>
//       OrdAttImp <V>
//          OrdAttImp.BoolImp
//          OrdAttImp.IntImp
//          OrdAttImp.DblImp
//          OrdAttImp.BoolVecImp
//          OrdAttImp.IntVecImp
//          OrdAttImp.DblVecImp
//          OrdAttImp.StringImp
//       SpecAttImp <V>
//          SpecAttImp.NPeriodsImp
//          SpecAttImp.NameImp
//          SpecAttImp.IndexImp
//          SpecAttImp.BoundSetImp
//==============================================================================

abstract class AttImp <V> extends PreservedInternal
{
//==============================================================================
// Package-Internal Constructor
//==============================================================================

   AttImp (Attribute <V> theAtt, Coordinator theCoord)
      {
      myAttribute   = theAtt;
      myCoordinator = theCoord;

      theCoord.store (this);
      }

//==============================================================================
// Package-Internal Methods
//==============================================================================

//------------------------------------------------------------------------------
// verifyValueType ()
//
// Verifies that the value type of the Java Attribute for this AttImp matches
// that of all corresponding C++ Atts.
// The verification is done only for OrdAttImps.
//------------------------------------------------------------------------------

   void verifyValueType ()
      {
      }

//------------------------------------------------------------------------------
// verifyAppliesTo ()
//
// Verifies that myAttribute.appliesTo (theCompClassID) returns true, iff
// there is a corrsponding Att in the C++ side.
// The verification is done only for OrdAttImps.
//------------------------------------------------------------------------------

   void verifyAppliesTo ()
      {
      }

//------------------------------------------------------------------------------
// verifyIsModifiable ().
//
// Verifies that the value returned by myAttribute.isModifiable () matches the
// value returned by theAttCOR.isModifiable () for each cooresponding AttCOR.
// The verification is done only for OrdAttImps.
//------------------------------------------------------------------------------

   void verifyIsModifiable ()
      {
      }

//------------------------------------------------------------------------------
// set (theValue)
//
// Sets the value of the WIT attribute represented by this AttImp for the
// current Component to theValue.
//------------------------------------------------------------------------------

   void set (V theValue)
      {
      throw new AssertionError ();
      }

//------------------------------------------------------------------------------
// setVectorToElem0 (theV)
//
// The value type V must be an array type.
// Sets each element of the value of the WIT vector attribute represented by
// this AttImp for the current Component to element #0 of theV.
//------------------------------------------------------------------------------

   void setVectorToElem0 (V theV)
      {
      throw new AssertionError ();
      }

//------------------------------------------------------------------------------
// get ()
//
// Returns the value of the WIT attribute represented by this AttImp for the
// current Component.
//------------------------------------------------------------------------------

   V get ()
      {
      throw new AssertionError ();
      }

//------------------------------------------------------------------------------
// getVector (theArray)
//
// The value type V must be an array type.
// Set theArray to the current value of the WIT attribute represented by this
// AttImp for the current Component.
//------------------------------------------------------------------------------

   void getVector (V theArray)
      {
      throw new AssertionError ();
      }

//------------------------------------------------------------------------------
// getNPeriods ()
//------------------------------------------------------------------------------

   final int getNPeriods ()
      {
      return myCoordinator.getProblem ().getNPeriods ();
      }

//------------------------------------------------------------------------------
// getCurComponent ()
//------------------------------------------------------------------------------

   final Component getCurComponent ()
      {
      return myCoordinator.getCurComponent ();
      }

//------------------------------------------------------------------------------
// Accessors.
//------------------------------------------------------------------------------

   final Attribute <V> getAttribute ()
      {
      return myAttribute;
      }

   final Coordinator getCoordinator ()
      {
      return myCoordinator;
      }

//==============================================================================
// Private Fields
//==============================================================================

//------------------------------------------------------------------------------
// myAttribute
//
// The Attribute for this AttImp.
//------------------------------------------------------------------------------
 
   private final Attribute <V> myAttribute;

//------------------------------------------------------------------------------
// myCoordinator
//
// The Coordinator for this AttImp.
//------------------------------------------------------------------------------

   private final Coordinator myCoordinator;

//==============================================================================
}
