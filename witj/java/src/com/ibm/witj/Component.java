
//==============================================================================
// WIT-J Java Source File Component.java.
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;

import       java.util.*;

//==============================================================================
// Class Component
//
// Represents a component of a Problem, i.e., a "data object" such as an
// Operation or a Demand.
//
// Class Hierarchy:
//
// ThreadedObject
//    PreservedObject
//       Component
//          Problem
//          Part
//          Demand
//          Operation
//          BomEntry
//          Substitute
//          BopEntry
//==============================================================================

public abstract class Component extends PreservedObject
{
//==============================================================================
// Public Methods
//==============================================================================

//------------------------------------------------------------------------------
// set (theAtt, theValue)
//
// Sets the value of the WIT attribute represented by theAtt for this
// Component to theValue.
//------------------------------------------------------------------------------

   public final <V> void set (Attribute <V> theAtt, V theValue)
      {
      try
         {
         mustBeActive                                  ("Component.set");
         mustBeNonNull           (theAtt,   "theAtt",   "Component.set");
         mustApplyToThisComp     (theAtt,                         "set");
         theAtt.mustNotBeBSetAtt                       ("Component.set");
         mustBeModifiable        (theAtt);
         mustBeNonNull           (theValue, "theValue", "Component.set");
         checkArrayLength        (theAtt,    theValue,            "set");
         becomeCurComp           ();

         getAttImpFor (theAtt).set (theValue);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }

      finally
         {
         clearCurComp ();
         }
      }

//------------------------------------------------------------------------------
// setVectorToScalar (theAtt, theValue) methods
//
// Each of these methods sets each element of the value of the WIT vector
// attribute represented by theAtt for this Component to theValue.
//------------------------------------------------------------------------------

   public final void setVectorToScalar (
         Attribute <boolean[]> theAtt,
         boolean               theValue)
      {
      try
         {
         setVectorToElem0 (theAtt, new boolean[]{theValue});
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public final void setVectorToScalar (
         Attribute <int[]> theAtt,
         int               theValue)
      {
      try
         {
         setVectorToElem0 (theAtt, new int[]{theValue});
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public final void setVectorToScalar (
         Attribute <double[]> theAtt,
         double               theValue)
      {
      try
         {
         setVectorToElem0 (theAtt, new double[]{theValue});
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// setBoundSet (theAtt, hardLBArr, softLBArr, hardUBArr)
//
// Sets the value of theAtt to the arrays.
// A null array reference for a bound indicates that the bound is not to be
// altered.
//------------------------------------------------------------------------------

   public final void setBoundSet (
         Attribute <BoundSet> theAtt,
         double[]             hardLBArr,
         double[]             softLBArr,
         double[]             hardUBArr)
      {
      try
         {
         mustBeActive                                ("Component.setBoundSet");
         mustBeNonNull         (theAtt, "theAtt",     "Component.setBoundSet");
         mustApplyToThisComp   (theAtt,                         "setBoundSet");
         checkBoundArrayLength (theAtt, hardLBArr, "hardLBArr", "setBoundSet");
         checkBoundArrayLength (theAtt, softLBArr, "softLBArr", "setBoundSet");
         checkBoundArrayLength (theAtt, hardUBArr, "hardUBArr", "setBoundSet");

         getComponentCOR ().setBoundSet (hardLBArr, softLBArr, hardUBArr);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// setBoundSetToScalars (theAtt, hardLBVal, softLBVal, hardUBVal)
//
// Sets the value of the BoundSet Attribute to the arguments in all periods.
//------------------------------------------------------------------------------

   public final void setBoundSetToScalars (
         Attribute <BoundSet> theAtt,
         double               hardLBVal,
         double               softLBVal,
         double               hardUBVal)
      {
      try
         {
         mustBeActive                ("Component.setBoundSetToScalars");
         mustBeNonNull       (theAtt, "theAtt", 
                                      "Component.setBoundSetToScalars");
         mustApplyToThisComp (theAtt,           "setBoundSetToScalars");

         getComponentCOR ().
            setBoundSetToScalars (hardLBVal, softLBVal, hardUBVal);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// get (theAtt)
//
// Returns the current value of the WIT attribute represented by theAtt for this
// Component.
//------------------------------------------------------------------------------

   public final <V> V get (Attribute <V> theAtt)
      {
      try
         {
         mustBeActive                              ("Component.get");
         mustBeNonNull           (theAtt, "theAtt", "Component.get");
         mustApplyToThisComp     (theAtt,                     "get");
         theAtt.mustNotBeBSetAtt                   ("Component.get");
         becomeCurComp           ();

         return getAttImpFor (theAtt).get ();
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }

      finally
         {
         clearCurComp ();
         }
      }

//------------------------------------------------------------------------------
// getVector (theAtt, theArray)
//
// The value type V must be an array type.
// Sets theArray to the current value of the WIT attribute represented by theAtt
// for this Component.
//------------------------------------------------------------------------------

   public final <V> void getVector (Attribute <V> theAtt, V theArray)
      {
      try
         {
         mustBeActive                              ("Component.getVector");
         mustBeNonNull       (theAtt,   "theAtt",   "Component.getVector");
         mustApplyToThisComp (theAtt,                         "getVector");
         mustBeArrayValType  (theAtt);
         mustBeNonNull       (theArray, "theArray", "Component.getVector");
         checkArrayLength    (theAtt,    theArray,            "getVector");
         becomeCurComp       ();

         getAttImpFor (theAtt).getVector (theArray);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }

      finally
         {
         clearCurComp ();
         }
      }

//------------------------------------------------------------------------------
// getBoundSet (theAtt, hardLBArr, softLBArr, hardUBArr)
//
// Retrieves the value of theAtt, storing it in the arrays.
//------------------------------------------------------------------------------

   public final void getBoundSet (
         Attribute <BoundSet> theAtt,
         double[]             hardLBArr,
         double[]             softLBArr,
         double[]             hardUBArr)
      {
      try
         {
         mustBeActive                                ("Component.getBoundSet");
         mustBeNonNull       (theAtt,    "theAtt",    "Component.getBoundSet");
         mustApplyToThisComp (theAtt,                           "getBoundSet");
         mustBeNonNull       (hardLBArr, "hardLBArr", "Component.getBoundSet");
         mustBeNonNull       (softLBArr, "softLBArr", "Component.getBoundSet");
         mustBeNonNull       (hardUBArr, "hardUBArr", "Component.getBoundSet");

         checkBoundArrayLength (theAtt, hardLBArr, "hardLBArr", "getBoundSet");
         checkBoundArrayLength (theAtt, softLBArr, "softLBArr", "getBoundSet");
         checkBoundArrayLength (theAtt, hardUBArr, "hardUBArr", "getBoundSet");

         getComponentCOR ().getBoundSet (hardLBArr, softLBArr, hardUBArr);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// copyComponentData (Component origComp)
//
// Copies input data from origComp into this Component.
// This Component must not be an instance of class Problem.
// origComp must be an instance of the same Component class as this Component.
//------------------------------------------------------------------------------

   public void copyComponentData (Component origComp)
      {
      try
         {
         checkForCopyComp (origComp);

         if (origComp == this)
            return;

         getComponentCOR ().copyComponentData (origComp.getComponentCOR ());
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getProblem ()
//
// Returns the Problem for this Component.
//------------------------------------------------------------------------------

   public final Problem getProblem ()
      {
      try
         {
         mustBeActive ("Component.getProblem");

         return getCoordinator ().getProblem ();
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//==============================================================================
// Package-Internal Constructor
//
// Constructs a Component that will belong to theCoord.
//==============================================================================

   Component (Coordinator theCoord)
      {
      super (theCoord);
      }

//==============================================================================
// Package-Internal Methods
//==============================================================================

//------------------------------------------------------------------------------
// reqMatchingProblems (...)
//
// Throws a StatusQuoException, if theComp1 and this theComp2 belong to
// different Problems.
//------------------------------------------------------------------------------

   static final void reqMatchingProblems (
         Component theComp1,
         String    theArgName1,
         Component theComp2,
         String    theArgName2,
         String    theMethName)
      {
      mustBeActive (theComp1, theArgName1, theMethName);
      mustBeActive (theComp2, theArgName2, theMethName);

      if (theComp1.getCoordinator () != theComp2.getCoordinator ())
         throw
            new StatusQuoException (
               "   A method was called with two Component arguments that\n"
             + "   belong to different Problems where this is not allowed.\n"
             + "\n"
             + "      Method:                    " + theMethName + "\n"
             + "      First  Component Argument: " + theArgName1 + "\n"
             + "      First  Component ID:       " + theComp1 + "\n"
             + "      Second Component Argument: " + theArgName2 + "\n"
             + "      Second Component ID:       " + theComp2);
      }

//------------------------------------------------------------------------------
// restoreAfterPurge ()
//
// Updates the state of this Component as appropriate just after a call to
// witPurgeData.
//------------------------------------------------------------------------------

   void restoreAfterPurge ()
      {
      }

//------------------------------------------------------------------------------
// getPegging (thePggCase, theCompClass, pegPer)
//
// Returns the pegging indicated by thePggCase for this Demand in pegPer, where
// theCompClass indicates the Component class of the pegging.
//------------------------------------------------------------------------------

   <C extends Component> ArrayList <PeggingTriple <C>> getPegging (
         PggCase   thePggCase,
         Class <C> theCompClass,
         int       pegPer)
      {
      CaseTokenCOR                  thePggCaseCTCOR;
      ArrayList <PeggingTriple <C>> theTripleList;

      thePggCaseCTCOR = getCoordinator ().getCaseTokenCOR (thePggCase);

      theTripleList   = new ArrayList <PeggingTriple <C>> (0);

      getComponentCOR ().getPegging (thePggCaseCTCOR, pegPer, theTripleList);

      return theTripleList;
      }

//------------------------------------------------------------------------------
// setName ()
//
// If this Component is a Part, Demand, or Operation, this method sets the
// PartName, DemandName, or OperationName to theName, as appropriate.
// Otherwise, this method throws an exception.
//------------------------------------------------------------------------------

   void setName (String theName)
      {
      throw new AssertionError ();
      }

//------------------------------------------------------------------------------
// getName ()
//
// If this Component is a Part, Demand, or Operation, this method returns the
// PartName, DemandName, or OperationName, as appropriate.
// Otherwise, this method throws an exception.
//------------------------------------------------------------------------------

   String getName ()
      {
      throw new AssertionError ();
      }

//------------------------------------------------------------------------------
// getIndex ()
//
// If this Component is a BomEntry, Substitute, or BopEntry, this method
// returns the BomIndex, SubIndex, or BopIndex, as appropriate.
// Otherwise, this method throws an exception.
//------------------------------------------------------------------------------

   int getIndex ()
      {
      throw new AssertionError ();
      }

//------------------------------------------------------------------------------
// Casting methods
//------------------------------------------------------------------------------

   final Problem asProblem ()
      {
      return (Problem) this;
      }

   final Part asPart ()
      {
      return (Part) this;
      }

   final Demand asDemand ()
      {
      return (Demand) this;
      }

   final Operation asOperation ()
      {
      return (Operation) this;
      }

   final BomEntry asBomEntry ()
      {
      return (BomEntry) this;
      }

   final Substitute asSubstitute ()
      {
      return (Substitute) this;
      }

   final BopEntry asBopEntry ()
      {
      return (BopEntry) this;
      }

//------------------------------------------------------------------------------
// getCompClassID ()
//
// Returns the CompClassID for the Component class to which this Component
// belongs.
//------------------------------------------------------------------------------

   abstract CompClassID getCompClassID ();

//------------------------------------------------------------------------------
// getComponentCOR ().
//
// Returns the ComponentCOR that belongs to this Component.
//------------------------------------------------------------------------------

   abstract ComponentCOR getComponentCOR ();

//------------------------------------------------------------------------------
// Access wrappers.
//------------------------------------------------------------------------------

   int getNPeriods ()
      {
      return getCoordinator ().getProblem ().getNPeriods ();
      }

//==============================================================================
// Private Methods
//==============================================================================

//------------------------------------------------------------------------------
// setVectorToElem0 (theAtt, theArray)
//
// The value type V must be an array type.
// Sets each element of the value of the WIT vector attribute represented by
// theAtt for this Component to element #0 of theArray.
// To be called by the setVectorToScalar methods.
//------------------------------------------------------------------------------

   private final <V> void setVectorToElem0 (Attribute <V> theAtt, V theArray)
      {
      try
         {
         mustBeActive                          ("Component.setVectorToScalar");
         mustBeNonNull       (theAtt, "theAtt", "Component.setVectorToScalar");
         mustApplyToThisComp (theAtt,                     "setVectorToScalar");
         mustBeModifiable    (theAtt);
         becomeCurComp       ();

         getAttImpFor (theAtt).setVectorToElem0 (theArray);
         }

      finally
         {
         clearCurComp ();
         }
      }

//------------------------------------------------------------------------------
// mustApplyToThisComp (theAtt, methodName)
//
// Verifies that theAtt applies to this Component.
// methodName is the name of the calling method.
//------------------------------------------------------------------------------

   private void mustApplyToThisComp (Attribute <?> theAtt, String methodName)
      {
      if (! theAtt.appliesTo (getCompClassID ()))
         throw
            new StatusQuoException (
               "   Component." + methodName
             +   " was called for an instance of a Component class\n"
             + "   to which the Attribute does not apply.\n"
             + "\n"
             + "      Component: " + this   + "\n"
             + "      Attribute: " + theAtt);
      }

//------------------------------------------------------------------------------
// mustBeModifiable (theAtt)
//
// Verifies that theAtt is modifiable.
//------------------------------------------------------------------------------

   private void mustBeModifiable (Attribute <?> theAtt)
      {
      if (! theAtt.isModifiable ())
         throw
            new StatusQuoException (
               "   Component.set was called with an unmodifiable Attribute.\n"
             + "\n"
             + "      Component: " + this + "\n"
             + "      Attribute: " + theAtt);
      }

//------------------------------------------------------------------------------
// mustBeArrayValType (theAtt)
//
// Verifies that the value type of theAtt is an array type.
//------------------------------------------------------------------------------

   final <V> void mustBeArrayValType (Attribute <V> theAtt)
      {
      if (! theAtt.getAttValType ().isArrayType ())
         throw
            new StatusQuoException (
               "   Component.getVector was called with an Attribute whose\n"
             + "   value type is not an array type.\n"
             + "\n"
             + "      Component:  " + this   + "\n"
             + "      Attribute:  " + theAtt + "\n"
             + "      Value Type: " + theAtt.getAttValType ().getName () + "\n"
               );
      }

//------------------------------------------------------------------------------
// void checkArrayLength (theAtt, theArray, methodName)
//
// If the value type of theAtt is an array type, this method verifies that the
// array length of theArray matches nPeriods.
// Otherwise, does nothing.
//------------------------------------------------------------------------------

   final <V> void checkArrayLength (
         Attribute <V> theAtt,
         V             theArray, 
         String        methodName)
      {
      if (theAtt.getAttValType ().isArrayType ())
         {
         int theLength;

         theLength = theAtt.getAttValType ().arrayLength (theArray);

         if (theLength != getNPeriods ())
            throw
               new StatusQuoException (
                  "   Component." + methodName + " was called with an array\n"
                + "   whose length does not match nPeriods for the Problem.\n"
                + "\n"
                + "      Component:    " + this      + "\n"
                + "      Attribute:    " + theAtt    + "\n"
                + "      Array Length: " + theLength + "\n"
                + "      nPeriods:     " + getNPeriods ());
         }
      }

//------------------------------------------------------------------------------
// checkBoundArrayLength (theAtt, theBoundArr, methodName)
//
// Does checkArrayLength (...) for theBoundArr, if it is not null.
//------------------------------------------------------------------------------

   private void checkBoundArrayLength (
         Attribute <BoundSet> theAtt,
         double[]             theBoundArr,
         String               argName,
         String               methodName)
      {
      if (theBoundArr == null)
         return;

      if (theBoundArr.length != getNPeriods ())
         throw
            new StatusQuoException (
               "   Component." + methodName + " was called with an array\n"
             + "   whose length does not match nPeriods for the Problem.\n"
             + "\n"
             + "      Component:      " + this               + "\n"
             + "      Attribute:      " + theAtt             + "\n"
             + "      Array Argument: " + argName            + "\n"
             + "      Array Length:   " + theBoundArr.length + "\n"
             + "      nPeriods:       " + getNPeriods ());
      }

//------------------------------------------------------------------------------
// checkForCopyComp (origComp)
//
// Does error checking for copyComponentData (theComp).
//------------------------------------------------------------------------------

   private void checkForCopyComp (Component origComp)
      {
      mustBeActive                       ("Component.copyComponentData");
      mustBeActive (origComp, "origComp", "Component.copyComponentData");

      if (getCompClassID () == CompClassID.PROBLEM)
         throw
            new StatusQuoException (
               "   Component.copyComponentData was called for an instance of "
                + "class Problem.");

      if (origComp.getCompClassID () != getCompClassID ())
         throw
            new StatusQuoException (
               "   Component.copyComponentData was called with an original\n"
             + "   Component and a duplicate Component that belong to\n"
             + "   different classes.\n"
             + "\n"
             + "       Original Component: " + origComp + "\n"
             + "      Duplicate Component: " + this);
      }

//------------------------------------------------------------------------------
// getAttImpFor (theAtt)
//
// Returns the AttImp corresponding to theAtt.
//------------------------------------------------------------------------------

   private <V> AttImp <V> getAttImpFor (Attribute <V> theAtt)
      {
      return getCoordinator ().getAttImpFor (theAtt);
      }

//------------------------------------------------------------------------------
// becomeCurComp ()
//
// Makes this Component the current Component.
//------------------------------------------------------------------------------

   private void becomeCurComp ()
      {
      getCoordinator ().setCurComponent (this);
      }

//------------------------------------------------------------------------------
// clearCurComp ()
//
// Clears the current Component.
//------------------------------------------------------------------------------

   private void clearCurComp ()
      {
      getCoordinator ().clearCurComponent ();
      }

//==============================================================================
}
