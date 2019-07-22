
//==============================================================================
// WIT-J Java Source File AttValType.java.
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;

//==============================================================================
// Class AttValType <V>
//
// Represents the value type of an Attribute <V>.
// Defines (through its subclasses) the corresponding value-type specific
// behavior for an Attribute <V>.
// This class and its subclasses are immutable.
//
// Class Hierarchy:
//
// AttValType <V>
//    AttValType.BoolType
//    AttValType.IntType
//    AttValType.DblType
//    AttValType.BoolVecType
//    AttValType.IntVecType
//    AttValType.DblVecType
//    AttValType.StringType
//    AttValType.BoundSetType
//==============================================================================

abstract class AttValType <V>
{
//==============================================================================
// Package-Internal Methods
//==============================================================================

//------------------------------------------------------------------------------
// makeOrdAttImp (theAtt, theCoord)
//
// Creates a new OrdAttImp for theAtt and theCoord.
// Not allowed for value type BoundSet.
//------------------------------------------------------------------------------

   abstract void makeOrdAttImp (Attribute <V> theAtt, Coordinator theCoord);

//------------------------------------------------------------------------------
// isArrayType ()
//
// Returns true, iff the value type is an array type.
//------------------------------------------------------------------------------

   boolean isArrayType ()
      {
      return false;
      }

//------------------------------------------------------------------------------
// newArray (theLength)
//
// If the value type is an array type, this method creates a value type instance
// of length theLength and returns it.
// Otherwise, throws an exception.
//------------------------------------------------------------------------------

   V newArray (int theLength)
      {
      throw new AssertionError ();
      }

//------------------------------------------------------------------------------
// arrayLength (theArray)
//
// If the value type is an array type, returns the length of theArray.
// Otherwise, throws an exception.
//------------------------------------------------------------------------------

   int arrayLength (V theArray)
      {
      throw new AssertionError ();
      }

//------------------------------------------------------------------------------
// setArrayToElem0 (theArray1, theArray2)
//
// If the value type is an array type, sets element #theIdx of theArray1 to
// element #0 of theArray2.
// Otherwise, throws an exception.
//------------------------------------------------------------------------------

   void setElemToElem0 (V theArray1, int theIdx, V theArray2)
      {
      throw new AssertionError ();
      }

//------------------------------------------------------------------------------
// getLastPeriod (Problem theProblem)
//
// If the value type is Integer, returns nPeriods - 1 for theProblem.
// If the value type is int[],   returns an array of length nPeriods for
//                                       theProblem filled with nPeriods - 1.
// Otherwise, throws an exception.
//------------------------------------------------------------------------------

   V getLastPeriod (Problem theProblem)
      {
      throw new AssertionError ();
      }

//------------------------------------------------------------------------------
// getName ()
//
// Returns the name of the value type.
//------------------------------------------------------------------------------

   final String getName ()
      {
      return myValueType.getSimpleName ();
      }

//------------------------------------------------------------------------------
// Accessors.
//------------------------------------------------------------------------------

   final Class <V> getValueType ()
      {
      return myValueType;
      }

//==============================================================================
// Package-Internal static constant fields.
//
// These are all of the AttValTypes for WIT-J.
//==============================================================================

   static final AttValType <Boolean>   BOOL      = new BoolType     ();
   static final AttValType <Integer>   INT       = new IntType      ();
   static final AttValType <Double>    DBL       = new DblType      ();
   static final AttValType <boolean[]> BOOL_VEC  = new BoolVecType  ();
   static final AttValType <int[]>     INT_VEC   = new IntVecType   ();
   static final AttValType <double[]>  DBL_VEC   = new DblVecType   ();
   static final AttValType <String>    STRING    = new StringType   ();
   static final AttValType <BoundSet>  BOUND_SET = new BoundSetType ();

//==============================================================================
// Private Constructor
//==============================================================================

   private AttValType (Class <V> theValueType)
      {
      myValueType = theValueType;
      }

//==============================================================================
// Private Fields
//==============================================================================

//------------------------------------------------------------------------------
// myValueType
//
// Indicates the value type represented by this AttValType.
//------------------------------------------------------------------------------

   private final Class <V> myValueType;

//==============================================================================
// Private Static Nested Class AttValType.BoolType.
//
// The concrete class for AttValType <Boolean>.
//==============================================================================

   private static final class BoolType extends AttValType <Boolean>
   {
   BoolType ()
      {
      super (Boolean.class);
      }

   void makeOrdAttImp (Attribute <Boolean> theBoolAtt, Coordinator theCoord)
      {
      new OrdAttImp.BoolImp (theBoolAtt, theCoord);
      }
   }

//==============================================================================
// Private Static Nested Class AttValType.IntType.
//
// The concrete class for AttValType <Integer>.
//==============================================================================

   private static final class IntType extends AttValType <Integer>
   {
   IntType ()
      {
      super (Integer.class);
      }

   void makeOrdAttImp (Attribute <Integer> theIntAtt, Coordinator theCoord)
      {
      new OrdAttImp.IntImp (theIntAtt, theCoord);
      }

   Integer getLastPeriod (Problem theProblem)
      {
      return theProblem.getNPeriods () - 1;
      }
   }

//==============================================================================
// Private Static Nested Class AttValType.DblType.
//
// The concrete class for AttValType <Double>.
//==============================================================================

   private static final class DblType extends AttValType <Double>
   {
   DblType ()
      {
      super (Double.class);
      }

   void makeOrdAttImp (Attribute <Double> theDblAtt, Coordinator theCoord)
      {
      new OrdAttImp.DblImp (theDblAtt, theCoord);
      }
   }

//==============================================================================
// Private Static Nested Class AttValType.BoolVecType.
//
// The concrete class for AttValType <boolean[]>.
//==============================================================================

   private static final class BoolVecType extends AttValType <boolean[]>
   {
   BoolVecType ()
      {
      super (boolean[].class);
      }

   void makeOrdAttImp (
         Attribute <boolean[]> theBoolVecAtt,
         Coordinator           theCoord)
      {
      new OrdAttImp.BoolVecImp (theBoolVecAtt, theCoord);
      }

   boolean isArrayType ()
      {
      return true;
      }

   boolean[] newArray (int theLength)
      {
      return new boolean[theLength];
      }

   int arrayLength (boolean[] theBoolArr)
      {
      return theBoolArr.length;
      }

   void setElemToElem0 (boolean[] theBooArr1, int theIdx, boolean[] theBooArr2)
      {
      theBooArr1[theIdx] = theBooArr2[0];
      }
   }

//==============================================================================
// Private Static Nested Class AttValType.IntVecType.
//
// The concrete class for AttValType <int[]>.
//==============================================================================

   private static final class IntVecType extends AttValType <int[]>
   {
   IntVecType ()
      {
      super (int[].class);
      }

   void makeOrdAttImp (Attribute <int[]> theIntVecAtt, Coordinator theCoord)
      {
      new OrdAttImp.IntVecImp (theIntVecAtt, theCoord);
      }

   boolean isArrayType ()
      {
      return true;
      }

   int[] newArray (int theLength)
      {
      return new int[theLength];
      }

   int arrayLength (int[] theIntArr)
      {
      return theIntArr.length;
      }

   void setElemToElem0 (int[] theIntArr1, int theIdx, int[] theIntArr2)
      {
      theIntArr1[theIdx] = theIntArr2[0];
      }

   int[] getLastPeriod (Problem theProblem)
      {
      int   nPers;
      int[] theIntArr;
      int   theIdx;

      nPers     = theProblem.getNPeriods ();

      theIntArr = new int[nPers];

      for (theIdx = 0; theIdx < nPers; theIdx ++)
         {
         theIntArr[theIdx] = nPers - 1;
         }

      return theIntArr;
      }
   }

//==============================================================================
// Private Static Nested Class AttValType.DblVecType.
//
// The concrete class for AttValType <double[]>.
//==============================================================================

   private static final class DblVecType extends AttValType <double[]>
   {
   DblVecType ()
      {
      super (double[].class);
      }

   void makeOrdAttImp (Attribute <double[]> theDblVecAtt, Coordinator theCoord)
      {
      new OrdAttImp.DblVecImp (theDblVecAtt, theCoord);
      }

   boolean isArrayType ()
      {
      return true;
      }

   double[] newArray (int theLength)
      {
      return new double[theLength];
      }

   int arrayLength (double[] theDblArr)
      {
      return theDblArr.length;
      }

   void setElemToElem0 (double[] theDblArr1, int theIdx, double[] theDblArr2)
      {
      theDblArr1[theIdx] = theDblArr2[0];
      }
   }

//==============================================================================
// Private Static Nested Class AttValType.StringType.
//
// The concrete class for AttValType <String>.
//==============================================================================

   private static final class StringType extends AttValType <String>
   {
   StringType ()
      {
      super (String.class);
      }

   void makeOrdAttImp (Attribute <String> theStrAtt, Coordinator theCoord)
      {
      new OrdAttImp.StringImp (theStrAtt, theCoord);
      }
   }

//==============================================================================
// Private Static Nested Class AttValType.BoundSetType.
//
// The concrete class for AttValType <BoundSet>.
//==============================================================================

   private static final class BoundSetType extends AttValType <BoundSet>
   {
   BoundSetType ()
      {
      super (BoundSet.class);
      }

   void makeOrdAttImp (Attribute <BoundSet> theBSetAtt, Coordinator theCoord)
      {
      throw new AssertionError ();
      }
   }

//==============================================================================
}
