
//==============================================================================
// WIT-J Java Source File OrdAttImp.java.
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;

import       java.util.*;

//==============================================================================
// Class OrdAttImp <V>
//
// "Ordinary Attribute Implementation."
// An AttImp <V> that operates in the usual, ordinary way.
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
//==============================================================================

abstract class OrdAttImp <V> extends AttImp <V>
{
//==============================================================================
// Package-Internal Methods
//==============================================================================

//------------------------------------------------------------------------------
// verifyValueType ()
//
// Override from class AttImp.
//------------------------------------------------------------------------------

   final void verifyValueType ()
      {
      String theTypeName;
      AttCOR theAttCOR;

      theTypeName = getAttribute ().getAttValType ().getName ();

      for (CompClassID theCompClassID: CompClassID.values ())
         {
         theAttCOR = myAttCORMap.get (theCompClassID);

         if (theAttCOR == null)
            continue;

         if (! theAttCOR.valueTypeMatches (theTypeName))
            throw new AssertionError ();
         }
      }

//------------------------------------------------------------------------------
// verifyAppliesTo ()
//
// Override from class AttImp.
//------------------------------------------------------------------------------

   final void verifyAppliesTo ()
      {
      boolean appliesInJava;
      boolean appliesInCpp;

      for (CompClassID theCompClassID: CompClassID.values ())
         {
         appliesInJava = getAttribute ().appliesTo (theCompClassID);

         appliesInCpp  = (myAttCORMap.get (theCompClassID) != null);

         if (appliesInJava != appliesInCpp)
            {
            System.out.printf (
               "\n"
             + "Error:\n"
             + "   A mismatch was found in the Attribute.appliesTo method.\n"
             + "\n"
             + "      Attribute:       %s\n"
             + "      Component Class: %s\n"
             + "      Java Value:      %s\n"
             + "      C++  Value:      %s\n"
             + "\n",
               getAttribute (),
               theCompClassID,
               appliesInJava,
               appliesInCpp);

            throw new AssertionError ();
            }
         }
      }

//------------------------------------------------------------------------------
// verifyIsModifiable ()
//
// Override from class AttImp.
//------------------------------------------------------------------------------

   final void verifyIsModifiable ()
      {
      boolean theAttIsModifiable;
      AttCOR  theAttCOR;

      theAttIsModifiable = getAttribute ().isModifiable ();

      for (CompClassID theCompClassID: CompClassID.values ())
         {
         theAttCOR = myAttCORMap.get (theCompClassID);

         if (theAttCOR == null)
            continue;

         assert (theAttCOR.isModifiable () == theAttIsModifiable);
         }
      }

//------------------------------------------------------------------------------
// getVector ()
//
// If the value type of the Attribute is an array type, this method returns the
// value of the WIT attribute represented by this OrdAttImp for the current
// Component.
// Otherwise, throws an exception.
//------------------------------------------------------------------------------

   final V getVector ()
      {
      V theArray;

      theArray = getAttribute ().getAttValType ().newArray (getNPeriods ());

      getVector (theArray);

      return theArray;
      }

//------------------------------------------------------------------------------
// deactivate ()
//
// Override from class PreservedInternal.
//------------------------------------------------------------------------------

   final void deactivate ()
      {
      AttCOR theAttCOR;

      for (CompClassID theCompClassID: CompClassID.values ())
         {
         theAttCOR = myAttCORMap.get (theCompClassID);

         if (theAttCOR != null)
            theAttCOR.deactivate ();
         }

      myAttCORMap.clear ();

      super.deactivate ();
      }

//------------------------------------------------------------------------------
// getCurAttCOR ()
//
// Returns the AttCOR that owns the C++ Att corresponding to this OrdAttImp and
// the Component class of the current Component, if any; otherwise null.
//------------------------------------------------------------------------------

   final AttCOR getCurAttCOR ()
      {
      CompClassID theCompClassID;

      theCompClassID = getCoordinator ().getCurComponent ().getCompClassID ();

      return myAttCORMap.get (theCompClassID);
      }

//------------------------------------------------------------------------------
// getCurCompCOR ()
//
// Returns the ComponentCOR for the current Component.
//------------------------------------------------------------------------------

   final ComponentCOR getCurCompCOR ()
      {
      return getCoordinator ().getCurComponent ().getComponentCOR ();
      }

//==============================================================================
// Private Constructor
//==============================================================================

   private OrdAttImp (Attribute <V> theAtt, Coordinator theCoord)
      {
      super (theAtt, theCoord);

      myAttCORMap = new EnumMap <CompClassID, AttCOR> (CompClassID.class);

      setUpAttCORMap ();
      }

//==============================================================================
// Private Methods
//==============================================================================

//------------------------------------------------------------------------------
// setUpAttCORMap ()
//
// Sets up myAttCORMap.
//------------------------------------------------------------------------------

   private void setUpAttCORMap ()
      {
      AttBldrCOR theAttBldrCOR;
      String     theAttCORName;
      AttCOR     theAttCOR;

      theAttBldrCOR = getCoordinator ().getAttBldrCOR ();

      for (CompClassID theCompClassID: CompClassID.values ())
         {
         theAttCORName = theCompClassID + "." + getAttribute ().getWitName ();

         if (theAttBldrCOR.hasAttFor (theAttCORName))
            {
            theAttCOR = new AttCOR (theAttBldrCOR, theAttCORName);

            myAttCORMap.put (theCompClassID, theAttCOR);
            }
         }
      }

//==============================================================================
// Private Fields
//==============================================================================
 
//------------------------------------------------------------------------------
// myAttCORMap
//
// myAttCORMap.get (theCompClassID) is the AttCOR that owns the C++ Att
// corresponding to this OrdAttImp and the Component class identified by
// theCompClassID, if any; otherwise null.
//------------------------------------------------------------------------------

   private final EnumMap <CompClassID, AttCOR> myAttCORMap;

//==============================================================================
// Package-Internal Static Nested Class OrdAttImp.BoolImp.
//
// The concrete class for OrdAttImp <Boolean>.
//==============================================================================

   static final class BoolImp extends OrdAttImp <Boolean>
   {
   BoolImp (Attribute <Boolean> theBoolAtt, Coordinator theCoord)
      {
      super (theBoolAtt, theCoord);
      }

   void set (Boolean theBool)
      {
      getCurAttCOR ().setBool (getCurCompCOR (), theBool);
      }

   Boolean get ()
      {
      return getCurAttCOR ().getBool (getCurCompCOR ());
      }
   }

//==============================================================================
// Package-Internal Static Nested Class OrdAttImp.IntImp.
//
// The concrete class for OrdAttImp <Integer>.
//==============================================================================

   static final class IntImp extends OrdAttImp <Integer>
   {
   IntImp (Attribute <Integer> theIntAtt, Coordinator theCoord)
      {
      super (theIntAtt, theCoord);
      }

   void set (Integer theInt)
      {
      getCurAttCOR ().setInt (getCurCompCOR (), theInt);
      }

   Integer get ()
      {
      return getCurAttCOR ().getInt (getCurCompCOR ());
      }
   }

//==============================================================================
// Package-Internal Static Nested Class OrdAttImp.DblImp.
//
// The concrete class for OrdAttImp <Double>.
//==============================================================================

   static final class DblImp extends OrdAttImp <Double>
   {
   DblImp (Attribute <Double> theDblAtt, Coordinator theCoord)
      {
      super (theDblAtt, theCoord);
      }

   void set (Double theDbl)
      {
      getCurAttCOR ().setDbl (getCurCompCOR (), theDbl);
      }

   Double get ()
      {
      return getCurAttCOR ().getDbl (getCurCompCOR ());
      }
   }

//==============================================================================
// Package-Internal Static Nested Class OrdAttImp.BoolVecImp.
//
// The concrete class for OrdAttImp <boolean[]>.
//==============================================================================

   static final class BoolVecImp extends OrdAttImp <boolean[]>
   {
   BoolVecImp (Attribute <boolean[]> theBoolVecAtt, Coordinator theCoord)
      {
      super (theBoolVecAtt, theCoord);
      }

   void set (boolean[] theBoolArr)
      {
      getCurAttCOR ().setBoolVec (getCurCompCOR (), theBoolArr);
      }

   void setVectorToElem0 (boolean[] theBoolArr)
      {
      getCurAttCOR ().setBoolVecToScalar (getCurCompCOR (), theBoolArr[0]);
      }

   boolean[] get ()
      {
      return getVector ();
      }

   void getVector (boolean[] theBoolArr)
      {
      getCurAttCOR ().getBoolVec (getCurCompCOR (), theBoolArr);
      }
   }

//==============================================================================
// Package-Internal Static Nested Class OrdAttImp.IntVecImp.
//
// The concrete class for OrdAttImp <int[]>.
//==============================================================================

   static final class IntVecImp extends OrdAttImp <int[]>
   {
   IntVecImp (Attribute <int[]> theIntVecAtt, Coordinator theCoord)
      {
      super (theIntVecAtt, theCoord);
      }

   void set (int[] theIntArr)
      {
      getCurAttCOR ().setIntVec (getCurCompCOR (), theIntArr);
      }

   void setVectorToElem0 (int[] theIntArr)
      {
      getCurAttCOR ().setIntVecToScalar (getCurCompCOR (), theIntArr[0]);
      }

   int[] get ()
      {
      return getVector ();
      }

   void getVector (int[] theIntArr)
      {
      getCurAttCOR ().getIntVec (getCurCompCOR (), theIntArr);
      }
   }

//==============================================================================
// Package-Internal Static Nested Class OrdAttImp.DblVecImp.
//
// The concrete class for OrdAttImp <double[]>.
//==============================================================================

   static final class DblVecImp extends OrdAttImp <double[]>
   {
   DblVecImp (Attribute <double[]> theDblVecAtt, Coordinator theCoord)
      {
      super (theDblVecAtt, theCoord);
      }

   void set (double[] theDblArr)
      {
      getCurAttCOR ().setDblVec (getCurCompCOR (), theDblArr);
      }

   void setVectorToElem0 (double[] theDblArr)
      {
      getCurAttCOR ().setDblVecToScalar (getCurCompCOR (), theDblArr[0]);
      }

   double[] get ()
      {
      return getVector ();
      }

   void getVector (double[] theDblArr)
      {
      getCurAttCOR ().getDblVec (getCurCompCOR (), theDblArr);
      }
   }

//==============================================================================
// Package-Internal Static Nested Class OrdAttImp.StringImp.
//
// The concrete class for OrdAttImp <String>.
//==============================================================================

   static final class StringImp extends OrdAttImp <String>
   {
   StringImp (Attribute <String> theStrAtt, Coordinator theCoord)
      {
      super (theStrAtt, theCoord);
      }

   void set (String theStr)
      {
      getCurAttCOR ().setString (getCurCompCOR (), theStr);
      }

   String get ()
      {
      return getCurAttCOR ().getString (getCurCompCOR ());
      }
   }

//==============================================================================
}
