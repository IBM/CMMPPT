
//==============================================================================
// WIT-J Java Source File ComponentCOR.java.
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;

import       java.util.*;

//==============================================================================
// Class ComponentCOR
//
// "Component C++ Object Representation"
// A Java representation of a C++ Component.
//
// Class Hierarchy:
//
// PreservedInternal
//    JavaAccObjCOR
//       ComponentCOR
//          ProblemCOR
//          PartCOR
//          DemandCOR
//          OperationCOR
//          BomEntryCOR
//          SubstituteCOR
//          BopEntryCOR
//==============================================================================

abstract class ComponentCOR extends JavaAccObjCOR
{
//==============================================================================
// Package-Internal Constructor
//==============================================================================

   ComponentCOR ()
      {
      }

//==============================================================================
// Package-Internal Native Methods
//==============================================================================

//------------------------------------------------------------------------------
// setComponent (theComp)
//
// Sets theComp as the Java Component corresponding to the represented C++
// Component.
//------------------------------------------------------------------------------

   final native void setComponent (Component theComp);

//------------------------------------------------------------------------------
// setBoundSet (hardLBArr, softLBArr, hardUBArr)
//
// If the C++ Component is an instance of a class that has a BoundSet, this
// function sets the value of the BoundSet to the arguments;
// otherwise an assertion exception is thrown.
//------------------------------------------------------------------------------

   final native void setBoundSet (
      double[] hardLBArr,
      double[] softLBArr,
      double[] hardUBArr);

//------------------------------------------------------------------------------
// setBoundSetToScalars (hardLBVal, softLBVal, hardUBVal)
//
// If the C++ Component is an instance of a class that has a BoundSet, this
// function sets the value of the BoundSet to the arguments in all periods;
// otherwise an assertion exception is thrown.
//------------------------------------------------------------------------------

   final native void setBoundSetToScalars (
      double hardLBVal,
      double softLBVal,
      double hardUBVal);

//------------------------------------------------------------------------------
// getBoundSet (hardLBArr, softLBArr, hardUBArr)
//
// If the C++ Component is an instance of a class that has a BoundSet, this
// function sets the value of the arguments to the BoundSet;
// otherwise an assertion exception is thrown.
//------------------------------------------------------------------------------

   final native void getBoundSet (
      double[] hardLBArr,
      double[] softLBArr,
      double[] hardUBArr);

//------------------------------------------------------------------------------

   final native void    copyComponentData  (ComponentCOR origCompCOR);
   final native void    setWasDeletedInWit (boolean      theBool);
   final native boolean    wasDeletedInWit ();

//------------------------------------------------------------------------------
// getPegging (thePggCaseCTCOR, pegPer, theTripleList)
//
// Retrieves the pegging indicated by the pegging case represented by
// thePggCaseCTCOR for the C++ Component in pegPer, storing it in theTripleList.
//------------------------------------------------------------------------------

   native <C extends Component> void getPegging (
      CaseTokenCOR                  thePggCaseCTCOR,
      int                           pegPer,
      ArrayList <PeggingTriple <C>> theTripleList);

//==============================================================================
}
