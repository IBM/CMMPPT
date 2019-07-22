
//==============================================================================
// WIT-J Java Source File AttCOR.java.
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;

//==============================================================================
// Class AttCOR
//
// "Att C++ Object Representation"
// A Java representation of a C++ Att.
// In the comments below, the WIT attribute represented by the C++ Att that
// belongs to this AttCOR is called "the represented attribute".
//
// Class Hierarchy:
//
// PreservedInternal
//    JavaAccObjCOR
//       AttCOR
//==============================================================================

final class AttCOR extends JavaAccObjCOR
{
//==============================================================================
// Package-Internal Constructor
// 
// Creates an AttCOR that will own the C++ Att whose scoped name matches
// scopedAttName and that was built by the C++ AttBldr that belongs to
// theAttBldrCOR.
//==============================================================================

   AttCOR (AttBldrCOR theAttBldrCOR, String scopedAttName)
      {
      storeCppAtt (theAttBldrCOR, scopedAttName);
      }

//==============================================================================
// Package-Internal Methods
//==============================================================================

//------------------------------------------------------------------------------
// set... (theCompCOR, theValue) methods
//
// Each of these methods sets the represented attribute's value associated with 
// the C++ Component that belongs to theCompCOR to theValue.
//
// Throws a C++ assert exception if any of the following conditions occur:
//
//    The represented attribute is not an input attribute.
//    The type of the represented attribute does not match the scalar type
//       indicated by the method name.
//    The Component class of the represented attribute does not match the C++
//       Component that belongs to theCompCOR.
//------------------------------------------------------------------------------

   native void setBool   (ComponentCOR theCompCOR, boolean theValue);
   native void setInt    (ComponentCOR theCompCOR, int     theValue);
   native void setDbl    (ComponentCOR theCompCOR, double  theValue);
   native void setString (ComponentCOR theCompCOR, String  theValue);

//------------------------------------------------------------------------------
// set...Vec (theCompCOR, theArray) methods
//
// Each of these methods sets the represented attribute's value associated with 
// the C++ Component that belongs to theCompCOR to theArray.
//
// Throws a C++ assert exception if any of the following conditions occur:
//
//    The represented attribute is not an input attribute.
//    The type of the represented attribute does not match the vector type
//       indicated by the method name.
//    The Component class of the represented attribute does not match the C++
//       Component that belongs to theCompCOR.
//    The length of theArray != nPeriods.
//------------------------------------------------------------------------------

   native void setBoolVec (ComponentCOR theCompCOR, boolean[] theArray);
   native void setIntVec  (ComponentCOR theCompCOR, int[]     theArray);
   native void setDblVec  (ComponentCOR theCompCOR, double[]  theArray);

//------------------------------------------------------------------------------
// set...VecToScalar (theCompCOR, theValue) methods
//
// Each of these methods sets each element of the represented attribute's value
// associated with the C++ Component that belongs to theCompCOR to theValue.
//
// Throws a C++ assert exception if any of the following conditions occur:
//
//    The represented attribute is not an input attribute.
//    The type of the represented attribute does not match the vector type
//       indicated by the method name.
//    The Component class of the represented attribute does not match the C++
//       Component that belongs to theCompCOR.
//------------------------------------------------------------------------------

   native void setBoolVecToScalar (ComponentCOR theCompCOR, boolean theValue);
   native void setIntVecToScalar  (ComponentCOR theCompCOR, int     theValue);
   native void setDblVecToScalar  (ComponentCOR theCompCOR, double  theValue);

//------------------------------------------------------------------------------
// get... (theCompCOR) methods
//
// Returns the represented attribute's value associated with the C++ Component
// that belongs to theCompCOR.
//
// Throws a C++ assert exception if either of the following conditions occur:
//
//    The type of the represented attribute does not match the scalar type
//       indicated by the method name.
//    The Component class of the represented attribute does not match the C++
//       Component that belongs to theCompCOR.
//------------------------------------------------------------------------------

   native boolean getBool   (ComponentCOR theCompCOR);
   native int     getInt    (ComponentCOR theCompCOR);
   native double  getDbl    (ComponentCOR theCompCOR);
   native String  getString (ComponentCOR theCompCOR);

//------------------------------------------------------------------------------
// get...Vec (theCompCOR, theArray) methods
//
// Sets theArray to the represented attribute's value associated with the
// the C++ Component that belongs to theCompCOR.
//
// Throws a C++ assert exception if any of the following conditions occur:
//
//    The type of the represented attribute does not match the vector type
//       indicated by the method name.
//    The Component class of the represented attribute does not match the C++
//       Component that belongs to theCompCOR.
//    The length of theArray != nPeriods.
//------------------------------------------------------------------------------

   native void getBoolVec (ComponentCOR theCompCOR, boolean[] theArray);
   native void getIntVec  (ComponentCOR theCompCOR, int[]     theArray);
   native void getDblVec  (ComponentCOR theCompCOR, double[]  theArray);

//------------------------------------------------------------------------------
// isModifiable
//
// Returns true, iff the C++ Att supports a "set" member function.
//------------------------------------------------------------------------------

   native boolean isModifiable ();

//------------------------------------------------------------------------------
// valueTypeMatches (theTypeName)
//
// Returns true, iff the value type of the represented attribute matches
// theTypeName.
//------------------------------------------------------------------------------

   native boolean valueTypeMatches (String theTypeName);

//------------------------------------------------------------------------------
// appliesTo (theClassName)
//
// Returns true, iff the represented attribute applies to the Component class
// whose name is given by theClassName.
//------------------------------------------------------------------------------

   native boolean appliesTo (String theClassName);

//==============================================================================
// Private Methods
//==============================================================================

//------------------------------------------------------------------------------
// storeCppAtt (theAttBldrCOR, scopedAttName)
// 
// Finds the C++ Att whose scoped name matches scopedAttName and that was built
// by the C++ AttBldr represented by theAttBldrCOR and stores its pointer in
// this AttCOR.
//------------------------------------------------------------------------------

   private native void storeCppAtt (
      AttBldrCOR theAttBldrCOR,
      String     scopedAttName);

//==============================================================================
}
