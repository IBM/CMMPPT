
//==============================================================================
// WIT-J Java Source File Problem.java.
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;
import static com.ibm.witj.CompClassID.*;

import       java.util.*;

//==============================================================================
// Class Problem
//
// The governing interface class of WIT-J.
// Each instance of class Problem corresponds to a WitRun.
//
// Class Hierarchy:
//
// ThreadedObject
//    PreservedObject
//       Component
//          Problem
//==============================================================================

public final class Problem extends Component
{
//==============================================================================
// Public Methods
//==============================================================================

//------------------------------------------------------------------------------
// newInstance ()
//
// Factory method:
//    Constructs a new Problem and returns it.
//------------------------------------------------------------------------------

   public static Problem newInstance ()
      {
      try
         {
         Coordinator theCoord;

         notAllowedWhenTerminated ("Problem.newInstance");

         theCoord = new Coordinator ();

         return theCoord.getProblem ();
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// clearData ()
//
// Restores this Problem to a base state:
//    Invokes witInitialize on the underlying WitRun.
//    All Components owned by this Problem except itself are removed from this
//       Problem and deactivatived.
//    All global Attributes are restored to their default values.
//    The state of the message system is left unchanged.
//------------------------------------------------------------------------------

   public void clearData ()
      {
      try
         {
         mustBeActive ("Problem.clearData");

         getCoordinator ().clearData ();

         nPeriods = 26;
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// performAction (theCommand)
//
// Performs the action indicated by theCommand.
//------------------------------------------------------------------------------

   public void performAction (String theCommand)
      {
      try
         {
         mustBeActive ("Problem.performAction");

         if (isForCpp (theCommand))
            {
            myProblemCOR.performAction (theCommand);
            }

         else if (theCommand.equals ("self-test"))
            {
            Coordinator.selfTest ();
            }

         else if (theCommand.equals ("fail-java-assert"))
            {
            assert (false);
            }

         else if (theCommand.equals ("exhaust-memory-in-witj-java"))
            {
            exhaustMemory ();
            }

         else if (theCommand.equals ("throw-unexpected-java-throwable"))
            {
            Coordinator theCoord;

            theCoord = null;

            theCoord.getPart ("No Part");
            }

         else if (theCommand.equals ("throw-unexpected-c++-exception"))
            {
            myProblemCOR.throwUnexpException ();
            }

         else if (theCommand.equals ("call-unimplemented-native-method"))
            {
            myProblemCOR.notImplemented ();
            }

         else
            {
            throw
               new StatusQuoException (
                  "   Problem.performAction (theCommand) was called\n"
                + "   with an unrecoginized command string.\n"
                + "\n"
                + "      Command: \"" + theCommand + "\"");
            }
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// Wrappers for WIT API Functions:
//------------------------------------------------------------------------------

   public void addDblCplexParSpec (String theName, double theValue)
      {
      try
         {
         mustBeActive ("Problem.addDblCplexParSpec");

         myProblemCOR.addDblCplexParSpec (theName, theValue);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public void addIntCplexParSpec (String theName, int theValue)
      {
      try
         {
         mustBeActive ("Problem.addIntCplexParSpec");

         myProblemCOR.addIntCplexParSpec (theName, theValue);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public void buildPip ()
      {
      try
         {
         mustBeActive ("Problem.buildPip");

         call (SimpleWitFunc.BuildPip);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public void clearCplexParSpecs ()
      {
      try
         {
         mustBeActive ("Problem.clearCplexParSpecs");

         call (SimpleWitFunc.ClearCplexParSpecs);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public void clearPegging ()
      {
      try
         {
         mustBeActive ("Problem.clearPegging");

         call (SimpleWitFunc.ClearPegging);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public void clearPipSeq ()
      {
      try
         {
         mustBeActive ("Problem.clearPipSeq");

         call (SimpleWitFunc.ClearPipSeq);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public void clearStochSoln ()
      {
      try
         {
         mustBeActive ("Problem.clearStochSoln");

         call (SimpleWitFunc.ClearStochSoln);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public void copyData (Problem origProblem)
      {
      try
         {
         ProblemCOR origProblemCOR;

         mustBeActive                             ("Problem.copyData");
         mustBeActive (origProblem, "origProblem", "Problem.copyData");

         if (origProblem == this)
            return;

         getCoordinator ().clearData ();

         origProblemCOR = origProblem.myProblemCOR;

         myProblemCOR.copyData (origProblemCOR);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public void displayData (String fileName)
      {
      try
         {
         mustBeActive                        ("Problem.displayData");
         mustBeNonNull (fileName, "fileName", "Problem.displayData");

         myProblemCOR.displayData (fileName);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public void eqHeurAlloc (
         ArrayList <Demand>  theDemandList,
         ArrayList <Integer> shipPeriodList,
         ArrayList <Double>  desIncVolList,
         ArrayList <Double>  incVolList)
      {
      try
         {
         mustBeActive ("Problem.eqHeurAlloc");

         checkEqHeurAlloc (
            theDemandList,
            shipPeriodList,
            desIncVolList,
            incVolList);

         myProblemCOR.
            eqHeurAlloc (
               theDemandList,
               shipPeriodList,
               desIncVolList,
               incVolList);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public void evalObjectives ()
      {
      try
         {
         mustBeActive ("Problem.evalObjectives");

         call (SimpleWitFunc.EvalObjectives);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public void finishHeurAlloc ()
      {
      try
         {
         mustBeActive ("Problem.finishHeurAlloc");

         call (SimpleWitFunc.FinishHeurAlloc);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public void generatePriorities ()
      {
      try
         {
         mustBeActive ("Problem.generatePriorities");

         call (SimpleWitFunc.GeneratePriorities);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public void getCriticalList (
         ArrayList <Part>    critPartList,
         ArrayList <Integer> critPeriodList)
      {
      try
         {
         mustBeActive                        ("Problem.getCriticalList");

         mustBeNonNull (
            critPartList,   "critPartList",   "Problem.getCriticalList");

         mustBeNonNull (
            critPeriodList, "critPeriodList", "Problem.getCriticalList");

         myProblemCOR.getCriticalList (critPartList, critPeriodList);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public Double getDblCplexParSpec (String theName)
      {
      try
         {
         mustBeActive                      ("Problem.getDblCplexParSpec");
         mustBeNonNull (theName, "theName", "Problem.getDblCplexParSpec");

         return myProblemCOR.getDblCplexParSpec (theName);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public void getExpCycle (
         ArrayList <Part>      thePartList,
         ArrayList <Operation> theOpnList)
      {
      try
         {
         mustBeActive                  ("Problem.getExpCycle");

         mustBeNonNull (
            thePartList, "thePartList", "Problem.getExpCycle");

         mustBeNonNull (
            theOpnList,  "theOpnList",  "Problem.getExpCycle");

         myProblemCOR.getExpCycle (thePartList, theOpnList);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public Integer getIntCplexParSpec (String theName)
      {
      try
         {
         mustBeActive                      ("Problem.getIntCplexParSpec");
         mustBeNonNull (theName, "theName", "Problem.getIntCplexParSpec");

         return myProblemCOR.getIntCplexParSpec (theName);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public ArrayList <String> getObjectiveList ()
      {
      try
         {
         ArrayList <String> theObjNameList;

         mustBeActive ("Problem.getObjectiveList");

         theObjNameList = new ArrayList <String> (0);

         myProblemCOR.getObjectiveList (theObjNameList);

         return theObjNameList;
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public OptInitMethod getOptInitMethod ()
      {
      try
         {
         String theMethodName;

         mustBeActive ("Problem.getOptInitMethod");

         theMethodName = myProblemCOR.getOptInitMethodName ();

         return OptInitMethod.valueOf (theMethodName);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public void getPgdCritList (
         ArrayList <Part>    critPartList,
         ArrayList <Integer> critPeriodList,
         ArrayList <Demand>  theDemandList,
         ArrayList <Integer> shipPeriodList)
      {
      try
         {
         mustBeActive                        ("Problem.getPgdCritList");

         mustBeNonNull (
            critPartList,   "critPartList",   "Problem.getPgdCritList");

         mustBeNonNull (
            critPeriodList, "critPeriodList", "Problem.getPgdCritList");

         mustBeNonNull (
            theDemandList,  "theDemandList",  "Problem.getPgdCritList");

         mustBeNonNull (
            shipPeriodList, "shipPeriodList", "Problem.getPgdCritList");

         myProblemCOR.
            getPgdCritList (
               critPartList,
               critPeriodList,
               theDemandList,
               shipPeriodList);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public ArrayList <PeggingTriple <Demand>> getPipSeq ()
      {
      try
         {
         ArrayList <PeggingTriple <Demand>> theDemandTripList;

         mustBeActive ("Problem.getPipSeq");

         theDemandTripList = new ArrayList <PeggingTriple <Demand>> (0);

         myProblemCOR.getPipSeq (theDemandTripList);

         return theDemandTripList;
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getSortedOperations ()
//
// Returns a list of all Operations in this Problem, sorted (by WIT) in BOM
// order.
//------------------------------------------------------------------------------

   public ArrayList <Operation> getSortedOperations ()
      {
      try
         {
         ArrayList <Operation> theOpnList;

         mustBeActive ("Problem.getSortedOperations");

         theOpnList = new ArrayList <Operation> (0);

         myProblemCOR.getSortedOperations (theOpnList);

         return theOpnList;
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getSortedParts ()
//
// Returns a list of all Parts in this Problem, sorted (by WIT) in BOM order.
//------------------------------------------------------------------------------

   public ArrayList <Part> getSortedParts ()
      {
      try
         {
         ArrayList <Part> thePartList;

         mustBeActive ("Problem.getSortedParts");

         thePartList = new ArrayList <Part> (0);

         myProblemCOR.getSortedParts (thePartList);

         return thePartList;
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public void heurImplode ()
      {
      try
         {
         mustBeActive ("Problem.heurImplode");

         call (SimpleWitFunc.HeurImplode);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public void mrp ()
      {
      try
         {
         mustBeActive ("Problem.mrp");

         call (SimpleWitFunc.Mrp);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public void optImplode ()
      {
      try
         {
         mustBeActive ("Problem.optImplode");

         call (SimpleWitFunc.OptImplode);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public void postprocess ()
      {
      try
         {
         mustBeActive ("Problem.postprocess");

         call (SimpleWitFunc.Postprocess);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public void preprocess ()
      {
      try
         {
         mustBeActive ("Problem.preprocess");

         call (SimpleWitFunc.Preprocess);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public void purgeData ()
      {
      try
         {
         mustBeActive ("Problem.purgeData");

         call (SimpleWitFunc.PurgeData);

         getCoordinator ().restoreAfterPurge ();
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// readData (fileName)
//
// Calls witReadData on fileName and copies the resulting structure into this
// Problem and the corresponding C++ Problem.
// The call to the underlying native method is globally synchronized with all
// other such calls. This prevents asynchronous calls to witReadData, which are
// not allowed.
//------------------------------------------------------------------------------

   public void readData (String fileName)
      {
      try
         {
         mustBeActive                        ("Problem.readData");
         mustBeNonNull (fileName, "fileName", "Problem.readData");

         if (getCoordinator ().hasPartsOrOperations ())
            {
            throw
               new StatusQuoException (
                  "   Problem.readData was called with a Problem\n"
                + "   that has one or more Parts or Operations.");
            }

         synchronized (readDataLock)
            {
            myProblemCOR.readData (fileName);
            }
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public void setObjectiveList (String... theObjNames)
      {
      try
         {
         ArrayList <String> theObjNameList;

         theObjNameList = new ArrayList <String> (Arrays.asList (theObjNames));

         setObjectiveList (theObjNameList);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public void setObjectiveList (ArrayList <String> theObjNameList)
      {
      try
         {
         checkArrayListArg (
            theObjNameList,
           "theObjNameList",
           "Problem.setObjectiveList");

         myProblemCOR.setObjectiveList (theObjNameList);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public void setOptInitMethod (OptInitMethod theMethod)
      {
      try
         {
         CaseTokenCOR theMethodCTCOR;

         mustBeActive ("Problem.setOptInitMethod");

         theMethodCTCOR = getCoordinator ().getCaseTokenCOR (theMethod);

         myProblemCOR.setOptInitMethod (theMethodCTCOR);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public void shutDownHeurAlloc ()
      {
      try
         {
         mustBeActive ("Problem.shutDownHeurAlloc");

         call (SimpleWitFunc.ShutDownHeurAlloc);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public void startHeurAlloc ()
      {
      try
         {
         mustBeActive ("Problem.startHeurAlloc");

         call (SimpleWitFunc.StartHeurAlloc);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public void stochImplode ()
      {
      try
         {
         mustBeActive ("Problem.stochImplode");

         call (SimpleWitFunc.StochImplode);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public void writeCriticalList (
         String     fileName,
         FileFormat theFormat,
         int        maxListLen)
      {
      try
         {
         CaseTokenCOR theFormatCTCOR;

         mustBeActive                          ("Problem.writeCriticalList");
         mustBeNonNull (fileName,  "fileName",  "Problem.writeCriticalList");
         mustBeNonNull (theFormat, "theFormat", "Problem.writeCriticalList");

         theFormatCTCOR = getCoordinator ().getCaseTokenCOR (theFormat);

         myProblemCOR.writeCriticalList (fileName, theFormatCTCOR, maxListLen);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public void writeData (String fileName)
      {
      try
         {
         mustBeActive                        ("Problem.writeData");
         mustBeNonNull (fileName, "fileName", "Problem.writeData");

         myProblemCOR.writeData (fileName);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public void writeExecSched (String fileName, FileFormat theFormat)
      {
      try
         {
         mustBeActive                          ("Problem.writeExecSched");
         mustBeNonNull (fileName,  "fileName",  "Problem.writeExecSched");
         mustBeNonNull (theFormat, "theFormat", "Problem.writeExecSched");

         writeSched (SchedID.EXEC_SCHED, fileName, theFormat);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public void writeReqSched (String fileName, FileFormat theFormat)
      {
      try
         {
         mustBeActive                          ("Problem.writeReqSched");
         mustBeNonNull (fileName,  "fileName",  "Problem.writeReqSched");
         mustBeNonNull (theFormat, "theFormat", "Problem.writeReqSched");

         writeSched (SchedID.REQ_SCHED, fileName, theFormat);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public void writeShipSched (String fileName, FileFormat theFormat)
      {
      try
         {
         mustBeActive                          ("Problem.writeShipSched");
         mustBeNonNull (fileName,  "fileName",  "Problem.writeShipSched");
         mustBeNonNull (theFormat, "theFormat", "Problem.writeShipSched");

         writeSched (SchedID.SHIP_SCHED, fileName, theFormat);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getComponents ()
//
// Returns an unmodifiable List of all Components that belong to this Problem,
// listed in creation order.
//------------------------------------------------------------------------------

   public List <Component> getComponents ()
      {
      try
         {
         mustBeActive ("Problem.getComponents");

         return getCoordinator ().getComponents ();
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getParts       ()
// getDemands     ()
// getOperations  ()
// getBomEntries  ()
// getSubstitutes ()
// getBopEntries  ()
//
// Each of these methods returns an unmodifiable List of all the instances of
// the indicated Component class that belong to this Problem, listed in creation
// order.
//------------------------------------------------------------------------------

   public List <Part> getParts ()
      {
      try
         {
         mustBeActive ("Problem.getParts");

         return getCoordinator ().getInstances (Part.class, PART);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public List <Demand> getDemands ()
      {
      try
         {
         mustBeActive ("Problem.getDemands");

         return getCoordinator ().getInstances (Demand.class, DEMAND);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public List <Operation> getOperations ()
      {
      try
         {
         mustBeActive ("Problem.getOperations");

         return getCoordinator ().getInstances (Operation.class, OPERATION);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public List <BomEntry> getBomEntries ()
      {
      try
         {
         mustBeActive ("Problem.getBomEntries");

         return getCoordinator ().getInstances (BomEntry.class, BOM_ENTRY);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public List <Substitute> getSubstitutes ()
      {
      try
         {
         mustBeActive ("Problem.getSubstitutes");

         return getCoordinator ().getInstances (Substitute.class, SUBSTITUTE);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public List <BopEntry> getBopEntries ()
      {
      try
         {
         mustBeActive ("Problem.getBopEntries");

         return getCoordinator ().getInstances (BopEntry.class, BOP_ENTRY);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getPart (thePartName)
//                                  
// Returns the Part for this Problem whose PartName matches thePartName, if any;
// otherwise null.
//------------------------------------------------------------------------------

   public Part getPart (String thePartName)
      {
      try
         {
         mustBeActive ("Problem.getPart");

         return getCoordinator ().getPart (thePartName);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getOperation (theOpnName)
//                                  
// Returns the Operation for this Problem whose OperationName matches
// theOpnName, if any; otherwise null.
//------------------------------------------------------------------------------

   public Operation getOperation (String theOpnName)
      {
      try
         {
         mustBeActive ("Problem.getOperation");

         return getCoordinator ().getOperation (theOpnName);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getNPartsCreated ()
// 
// Returns the # of Parts created for this Problem regardless of whether or not
// any of them have been deactivated.
//------------------------------------------------------------------------------

   public int getNPartsCreated ()
      {
      try
         {
         mustBeActive ("Problem.nPartsCreated");

         return getCoordinator ().getNPartsCreated ();
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getNDemandsCreated ()
// 
// Returns the # of Demands created for this Problem regardless of whether or
// not any of them have been deactivated.
//------------------------------------------------------------------------------

   public int getNDemandsCreated ()
      {
      try
         {
         mustBeActive ("Problem.nDemandsCreated");

         return getCoordinator ().getNDemandsCreated ();
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getNOperationsCreated ()
// 
// Returns the # of Operations created for this Problem regardless of whether or
// not any of them have been deactivated.
//------------------------------------------------------------------------------

   public int getNOperationsCreated ()
      {
      try
         {
         mustBeActive ("Problem.nOperationsCreated");

         return getCoordinator ().getNOperationsCreated ();
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// deactivate ()
//
// Puts this Problem and the PreservedObjects that it owns into an inactive
// state. This Problem and the PreservedObjects that it owns cannot be
// garbage-collected until this method is called.
// Override from class PreservedObject.
//------------------------------------------------------------------------------

   public void deactivate ()
      {
      try
         {
         mustBeActive ("Problem.deactivate");

         myProblemCOR     .deactivate ();
         getCoordinator ().deactivate ();
         super            .deactivate ();
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// Accessors.
//------------------------------------------------------------------------------

   public MessageMgr getMessageMgr ()
      {
      try
         {
         mustBeActive ("Problem.getMessageMgr");

         return getCoordinator ().getMessageMgr ();
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//==============================================================================
// Package-Internal Constructor
//==============================================================================

   Problem (Coordinator theCoord)
      {
      super (theCoord);

      nPeriods     = 26;

      myProblemCOR = new ProblemCOR (theCoord.getCoordinatorCOR ());
      }

//==============================================================================
// Package-Internal Methods
//==============================================================================

//------------------------------------------------------------------------------
// toStringActive ().
//
// Override from class PreservedObject.
//------------------------------------------------------------------------------

   String toStringActive ()
      {
      return "Problem";
      }

//------------------------------------------------------------------------------
// getCompClassID ()
//
// Override from class Component.
//------------------------------------------------------------------------------

   CompClassID getCompClassID ()
      {
      return CompClassID.PROBLEM;
      }

//------------------------------------------------------------------------------
// getComponentCOR ().
//
// Override from class Component.
//------------------------------------------------------------------------------

   ComponentCOR getComponentCOR ()
      {
      return myProblemCOR;
      }

//------------------------------------------------------------------------------
// setNPeriods (theValue)
//
// Setter for nPeriods.
//------------------------------------------------------------------------------

   void setNPeriods (int theValue)
      {
      assert (! getCoordinator ().hasPartsOrOperations ());

      myProblemCOR.setNPeriods (theValue);

      nPeriods = theValue;
      }

//------------------------------------------------------------------------------
// getNPeriods ()
//
// Accessor.
//------------------------------------------------------------------------------

   int getNPeriods ()
      {
      return nPeriods;
      }

//------------------------------------------------------------------------------
// Accessors.
//------------------------------------------------------------------------------

   ProblemCOR getProblemCOR ()
      {
      return myProblemCOR;
      }

//==============================================================================
// Private Methods
//==============================================================================

//------------------------------------------------------------------------------
// call (theFunc)
//
// Calls the simple WIT API function indicated by theFunc.
//------------------------------------------------------------------------------

   private void call (SimpleWitFunc theFunc)
      {
      CaseTokenCOR theFuncCTCOR;

      theFuncCTCOR = getCoordinator ().getCaseTokenCOR (theFunc);

      myProblemCOR.callSimpleWitFunc (theFuncCTCOR);
      }

//------------------------------------------------------------------------------
// writeSched (theSchedID, fileName, theFormat)
//
// Writes a schedule indicated by theSchedID to a file whose name is given by
// fileName, using the file format indicated by theFormat.
//------------------------------------------------------------------------------

   private void writeSched (
         SchedID    theSchedID,
         String     fileName,
         FileFormat theFormat)
      {
      CaseTokenCOR theSchedIDCTCOR;
      CaseTokenCOR theFormatCTCOR;

      theSchedIDCTCOR = getCoordinator ().getCaseTokenCOR (theSchedID);
      theFormatCTCOR  = getCoordinator ().getCaseTokenCOR (theFormat);

      myProblemCOR.writeSched (theSchedIDCTCOR, fileName, theFormatCTCOR);
      }

//------------------------------------------------------------------------------
// isForCpp (theCommand)
//
// Returns true, iff theCommand is to be passed through to the C++ side of
// WIT-J.
//------------------------------------------------------------------------------

   private boolean isForCpp (String theCommand)
      {
      return
         theCommand.equals ("fail-c++-exception-assert")  ||
         theCommand.equals ("fail-c++-aborting-assert")   ||
         theCommand.equals ("exhaust-memory-in-witj-c++") ||
         theCommand.equals ("trigger-jni-exception")      ||
         theCommand.equals ("exhaust-memory-in-jni")      ||
         theCommand.equals ("exhaust-memory-in-wit")      ||
         theCommand.equals ("trigger-wit-fatal-message");
      }

//------------------------------------------------------------------------------
// exhaustMemory ()
//
// Repeatedly allocates memory in 100MB increments until memory is exhausted.
// Used for testing.
//------------------------------------------------------------------------------

   private void exhaustMemory ()
      {
      LinkedList <byte[]> the100MBChunks;
      int                 totDiscarded;
      byte[]              the100MB;

      the100MBChunks = new LinkedList <byte[]> ();

      for (totDiscarded = 100; true; totDiscarded += 100)
         {
         the100MB = new byte[104857600];

         the100MBChunks.addLast (the100MB);

         println (
            totDiscarded + "MB of memory have been allocated by WIT-J/Java.");
         }
      }

//------------------------------------------------------------------------------
// checkEqHeurAlloc
//
// Checks the arguments to eqHeurAlloc..
//------------------------------------------------------------------------------

   private void checkEqHeurAlloc (
         ArrayList <Demand>  theDemandList,
         ArrayList <Integer> shipPeriodList,
         ArrayList <Double>  desIncVolList,
         ArrayList <Double>  incVolList)
      {
      checkDemandListArg (
         theDemandList,
        "Problem.eqHeurAlloc");

      checkArrayListArg (
         shipPeriodList,
        "shipPeriodList",
        "Problem.eqHeurAlloc");

      sizeMustMatch (
         shipPeriodList,
        "shipPeriodList",
         theDemandList,
        "theDemandList",
        "Problem.eqHeurAlloc");

      checkArrayListArg (
         desIncVolList,
        "desIncVolList",
        "Problem.eqHeurAlloc");

      sizeMustMatch (
         desIncVolList,
        "desIncVolList",
         theDemandList,
        "theDemandList",
        "Problem.eqHeurAlloc");

      mustBeNonNull (
         incVolList,
        "incVolList",
        "Problem.eqHeurAlloc");
      }

//------------------------------------------------------------------------------
// checkDemandListArg (theDemandList, methName)
//
// Performs error checking for an ArrayList <Demand> argument to a public
// method.
//    theDemandList: The ArrayList <Demand> argument
//    methName:      The name of the method
//------------------------------------------------------------------------------

   private void checkDemandListArg (
         ArrayList <Demand> theDemandList, 
         String             methName)
      {     
      int    theIdx;
      Demand theDemand;

      checkArrayListArg (theDemandList, "theDemandList", methName);

      for (theIdx = 0; theIdx < theDemandList.size (); theIdx ++)
         {
         theDemand = theDemandList.get (theIdx);

         if (! theDemand.isActive ())
            throw
               new StatusQuoException (
                  "   A method was called with an ArrayList <Demand> argument\n"
                + "   that contained an inactive Demand.\n"
                + "\n"
                + "      Method:     " + methName + "\n"
                + "      Argument:   theDemandList\n"
                + "      List Index: " + theIdx);

          if (theDemand.getCoordinator () != getCoordinator ())
            throw
               new StatusQuoException (
                  "   A method was called with an ArrayList <Demand> argument\n"
                + "   that contained a Demand that did not belong to\n"
                + "   the Problem on which the method was called.\n"
                + "\n"
                + "      Method:     " + methName + "\n"
                + "      Argument:   theDemandList\n"
                + "      List Index: " + theIdx   + "\n"
                + "      Demand:     " + theDemand);
         }
      }

//------------------------------------------------------------------------------
// checkArrayListArg (theArrayList, argName, methName)
//
// Performs error checking for an ArrayList argument to a public method.
//    theArrayList: The ArrayList argument
//    argName:      The name of ArrayList argument
//    methName:     The name of the method
//------------------------------------------------------------------------------

   private void checkArrayListArg (
         ArrayList <?> theArrayList,
         String        argName,
         String        methName)
      {     
      int    theIdx;
      Object theElem;

      mustBeNonNull (theArrayList, argName, methName);

      for (theIdx = 0; theIdx < theArrayList.size (); theIdx ++)
         {
         theElem = theArrayList.get (theIdx);

         if (theElem == null)
            throw
               new StatusQuoException (
                  "   A method was called with an ArrayList argument\n"
                + "   that contained a null reference.\n"
                + "\n"
                + "      Method:     " + methName + "\n"
                + "      Argument:   " + argName  + "\n"
                + "      List Index: " + theIdx);
         }
      }

//------------------------------------------------------------------------------
// sizeMustMatch (theArrayList2, arg2Name, theArrayList1, arg1Name, methName)
//
// Verifies that two ArrayList arguments are the same size.
//------------------------------------------------------------------------------

   private void sizeMustMatch (
         ArrayList <?> theArrayList2,
         String        arg2Name,
         ArrayList <?> theArrayList1,
         String        arg1Name,
         String        methName)
      {
      if (theArrayList2.size () != theArrayList1.size ())
         throw
            new StatusQuoException (
               "   A method was called with two ArrayList arguments "
             +    "whose sizes do not match.\n"
             + "\n"
             + "      Method:           " + methName              + "\n"
             + "      First  ArrayList: " + arg1Name              + "\n"
             + "         Size:          " + theArrayList1.size () + "\n"
             + "      Second ArrayList: " + arg2Name              + "\n"
             + "         Size:          " + theArrayList2.size ());

      }
      
//==============================================================================
// Private Fields
//==============================================================================

//------------------------------------------------------------------------------
// readDataLock
//
// The synchronization lock for this Object is used to globally synchronize all
// calls to readData.
//------------------------------------------------------------------------------

   private static final Object readDataLock = new Object ();

//------------------------------------------------------------------------------
// nPeriods
//
// Cached copy of the value stored in WIT.
//------------------------------------------------------------------------------

   private int nPeriods;

//------------------------------------------------------------------------------
// myProblemCOR.
//
// The ProblemCOR that belongs to this Problem.
//------------------------------------------------------------------------------

   private final ProblemCOR myProblemCOR;

//==============================================================================
}
