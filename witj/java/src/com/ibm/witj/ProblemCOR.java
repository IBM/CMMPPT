
//==============================================================================
// WIT-J Java Source File ProblemCOR.java.
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;

import       java.util.*;

//==============================================================================
// Class ProblemCOR
//
// "Problem C++ Object Representation"
// A Java representation of a C++ Problem.
//
// Class Hierarchy:
//
// PreservedInternal
//    JavaAccObjCOR
//       ComponentCOR
//          ProblemCOR
//==============================================================================

final class ProblemCOR extends ComponentCOR
{
//==============================================================================
// Package-Internal Constructor
//==============================================================================

   ProblemCOR (CoordinatorCOR theCoordCOR)
      {
      createAndStoreCppProblem (theCoordCOR);
      }

//==============================================================================
// Package-Internal Methods
//==============================================================================

//------------------------------------------------------------------------------
// Wrappers for corresponding C++ Problem member functions.
//------------------------------------------------------------------------------

   native void    callSimpleWitFunc    (CaseTokenCOR          theFuncCTCOR);
   native void    performAction        (String                theCommand);
   native boolean throwUnexpException  ();
   native void    writeSched           (CaseTokenCOR          theSchedIDCTCOR,
                                        String                fileName, 
                                        CaseTokenCOR          theFormatCTCOR);
   native void    addDblCplexParSpec   (String                theName,
                                        double                theValue);
   native void    addIntCplexParSpec   (String                theName,
                                        int                   theValue);
   native void    copyData             (ProblemCOR            origProblemCOR);
   native void    displayData          (String                fileName);
   native void    eqHeurAlloc          (ArrayList <Demand>    theDemandList,
                                        ArrayList <Integer>   shipPeriodList,
                                        ArrayList <Double>    desIncVolList,
                                        ArrayList <Double>    incVolList);
   native void    getCriticalList      (ArrayList <Part>      critPartList,
                                        ArrayList <Integer>   critPeriodList);
   native Double  getDblCplexParSpec   (String                theName);
   native void    getExpCycle          (ArrayList <Part>      thePartList,
                                        ArrayList <Operation> theOpnList);
   native Integer getIntCplexParSpec   (String                theName);
   native void    getObjectiveList     (ArrayList <String>    theObjNameList);
   native String  getOptInitMethodName ();
   native void    getPgdCritList (      ArrayList <Part>      critPartList,
                                        ArrayList <Integer>   critPeriodList,
                                        ArrayList <Demand>    theDemandList,
                                        ArrayList <Integer>   shipPeriodList);
   native void    getPipSeq            (ArrayList <PeggingTriple <Demand>>
                                        theDemandTripList);
   native void    getSortedOperations  (ArrayList <Operation> theOpnList);
   native void    getSortedParts       (ArrayList <Part>      thePartList);
   native void    readData             (String                fileName);
   native void    setNPeriods          (int                   theInt);
   native void    setObjectiveList     (ArrayList <String>    theObjNameList);
   native void    setOptInitMethod     (CaseTokenCOR          theMethodCTCOR);
   native void    writeCriticalList    (String                fileName, 
                                        CaseTokenCOR          theFormatCTCOR,
                                        int                   maxListLen);
   native void    writeData            (String                fileName);

//------------------------------------------------------------------------------
// notImplemented ()
//
// This native method is not implemented in the C++ portion.
// It is intended for use in error testing.
//------------------------------------------------------------------------------

   native void notImplemented ();

//==============================================================================
// Private Native Methods
//==============================================================================

//------------------------------------------------------------------------------
// createAndStoreCppProblem (theCoordCOR)
// 
// Creates a new C++ Problem and stores its pointer in this ProblemCOR.
//------------------------------------------------------------------------------

   private native void createAndStoreCppProblem (CoordinatorCOR theCoordCOR);

//==============================================================================
}
