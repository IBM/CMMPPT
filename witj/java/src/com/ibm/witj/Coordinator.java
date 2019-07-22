
//==============================================================================
// WIT-J Java Source File Coordinator.java.
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;

import       java.util.*;

//==============================================================================
// Class Coordinator
//
// Facilitates interactions between the objects of a Problem.
//
// Class Hierarchy:
//
// PreservedInternal
//    Coordinator
//==============================================================================

final class Coordinator extends PreservedInternal
{
//==============================================================================
// Package Internal Constructor
//==============================================================================

   Coordinator ()
      {
      myCoordinatorCOR  = new CoordinatorCOR ();

      myAttImpArr       = new AttImp <?>[Attribute.getAttributes ().size ()];

      SpecAttImp.buildInstances (this);

      buildOrdAttImps ();

      myCaseTokenCORMap = new HashMap <CaseToken, CaseTokenCOR> (100);

      buildCaseTokenCORs (SimpleWitFunc.values ());
      buildCaseTokenCORs (Part.Category.values ());
      buildCaseTokenCORs (OptInitMethod.values ());
      buildCaseTokenCORs (FileFormat   .values ());
      buildCaseTokenCORs (MessageGroup .values ());
      buildCaseTokenCORs (SchedID      .values ());
      buildCaseTokenCORs (PggCase      .values ());

      myCoordinatorCOR.setUpCaseTokens ();

      myMessageMgr      = new MessageMgr (this);

      myComponents      = new ArrayList <Component>  ();

      unmodComponents   = Collections.unmodifiableList (myComponents);

      myConcComps       =
         new EnumMap <CompClassID, ArrayList <? extends Component>> (
            CompClassID.class);

      unmodConcComps    =
         new EnumMap <CompClassID,      List <? extends Component>> (
            CompClassID.class);

      createConcCompList (Problem     .class);
      createConcCompList (Part        .class);
      createConcCompList (Demand      .class);
      createConcCompList (Operation   .class);
      createConcCompList (BomEntry    .class);
      createConcCompList (Substitute  .class);
      createConcCompList (BopEntry    .class);

      myPartMap          = new HashMap <String, Part>      ();
      myOpnMap           = new HashMap <String, Operation> ();

      nPartsCreated      = 0;
      nDemandsCreated    = 0;
      nOperationsCreated = 0;

      myProblem          = new Problem (this);

      recordConcComp (myProblem);

      allActiveCoordinators.get ().add (this);
      }

//==============================================================================
// Package-Internal Methods
//==============================================================================

//------------------------------------------------------------------------------
// store (theAttImp)
//
// Stores theAttImp in myAttImpArr.
//------------------------------------------------------------------------------

   void store (AttImp <?> theAttImp)
      {
      int theIdx;

      theIdx              = theAttImp.getAttribute ().getIndex ();

      myAttImpArr[theIdx] = theAttImp;
      }

//------------------------------------------------------------------------------
// getAttImpFor (theAtt)
//
// Returns the AttImp that belongs to this Coordinator corresponding to theAtt.
//------------------------------------------------------------------------------

  @SuppressWarnings ("unchecked")

   <V> AttImp <V> getAttImpFor (Attribute <V> theAtt)
      {
      AttImp <?> theAttImp;

      theAttImp = myAttImpArr[theAtt.getIndex ()];

      return (AttImp <V>) theAttImp;
      }

//------------------------------------------------------------------------------
// Returns the CaseTokenCOR owned by this Coordinator that corresponds to
// theCaseToken.
//------------------------------------------------------------------------------

   CaseTokenCOR getCaseTokenCOR (CaseToken theCaseToken)
      {
      return myCaseTokenCORMap.get (theCaseToken);
      }

//------------------------------------------------------------------------------
// recordPart (thePart)
//
// Records the fact that thePart has been created.
//------------------------------------------------------------------------------

   void recordPart (Part thePart)
      {
      recordConcComp (thePart);

      myPartMap.put (thePart.getName (), thePart);

      nPartsCreated ++;
      }

//------------------------------------------------------------------------------
// recordDemand (theDemand)
//
// Records the fact that theDemand has been created.
//------------------------------------------------------------------------------

   void recordDemand (Demand theDemand)
      {
      recordConcComp (theDemand);

      nDemandsCreated ++;
      }

//------------------------------------------------------------------------------
// recordOperation (theOpn)
//
// Records the fact that theOpn has been created.
//------------------------------------------------------------------------------

   void recordOperation (Operation theOpn)
      {
      recordConcComp (theOpn);

      myOpnMap.put (theOpn.getName (), theOpn);

      nOperationsCreated ++;
      }

//------------------------------------------------------------------------------
// recordConcComp (theConcComp)
//
// Records the fact that theConcComp has been created.
//------------------------------------------------------------------------------

  @SuppressWarnings ("unchecked")

   <C extends Component> void recordConcComp (C theConcComp)
      {
      ArrayList <C> theConcCompList;

      assert (theConcComp.getCoordinator () == this);

      myComponents.add (theConcComp);

      theConcCompList =
         (ArrayList <C>) myConcComps.get (theConcComp.getCompClassID ());

      theConcCompList.add (theConcComp);

      theConcComp.getComponentCOR ().setComponent (theConcComp);
      }

//------------------------------------------------------------------------------
// getPart (thePartName)
//
// Returns the Part that belongs to this Coordinator whose PartName matches
// thePartName, if any; otherwise null.
//------------------------------------------------------------------------------

   Part getPart (String thePartName)
      {
      return myPartMap.get (thePartName);
      }

//------------------------------------------------------------------------------
// getOperation (theOpnName)
//
// Returns the Operation that belongs to this Coordinator whose OperationName
// matches theOpnName, if any; otherwise null.
//------------------------------------------------------------------------------

   Operation getOperation (String theOpnName)
      {
      return myOpnMap.get (theOpnName);
      }

//------------------------------------------------------------------------------
// selfTest ()
//
// Performs various tests of WIT-J that cannot be done by calling public
// methods.
//------------------------------------------------------------------------------

   static void selfTest ()
      {
      Coordinator theCoord;

      println (
           "\n"
         + "----------------------------------------"
         + "----------------------------------------\n"
         + "Performing a self-test.\n"
         + "----------------------------------------"
         + "----------------------------------------");

      Attribute.selfTest ();

      theCoord = new Coordinator ();

      theCoord.selfTestAttImps ();

      theCoord.myProblem.deactivate ();

      println (
           "\n"
         + "----------------------------------------"
         + "----------------------------------------\n"
         + "Self-test complete.\n"
         + "----------------------------------------"
         + "----------------------------------------");
      }

//------------------------------------------------------------------------------
// restoreAfterPurge ()
//
// Updates the state of WIT-J to match that of WIT just after a call to
// witPurgeData.
//------------------------------------------------------------------------------

   void restoreAfterPurge ()
      {
      for (Component theComp: myComponents)
         if (theComp != myProblem)
            theComp.getComponentCOR ().setWasDeletedInWit (true);

      myCoordinatorCOR.restoreAfterPurge ();

      deactivateComponentsDeletedInWit ();

      PreservedObject.removeInactive (myComponents);

      for (CompClassID theCompClassID: CompClassID.values ())
         {
         PreservedObject.removeInactive (myConcComps.get (theCompClassID));
         }

      myPartMap = activeOnly (myPartMap);
      myOpnMap  = activeOnly (myOpnMap);

      for (Component theComp: myComponents)
         {
         theComp.restoreAfterPurge ();
         }
      }

//------------------------------------------------------------------------------
// clearData ()
//
// Restores this Coordinator to its default state.
//------------------------------------------------------------------------------

   void clearData ()
      {
      int       theIdx;
      Component theComp;

      myPartMap = new HashMap <String, Part>      ();
      myOpnMap  = new HashMap <String, Operation> ();

      for (CompClassID theCompClassID: CompClassID.values ())
         if (theCompClassID != CompClassID.PROBLEM)
            {
            myConcComps.get (theCompClassID).clear      ();
            myConcComps.get (theCompClassID).trimToSize ();
            }

      for (theIdx = myComponents.size () - 1; theIdx >= 0; theIdx --)
         {
         theComp = myComponents.get (theIdx);

         if (theComp != myProblem)
            theComp.deactivate ();
         }

      myComponents.clear      ();
      myComponents.add        (myProblem);
      myComponents.trimToSize ();

      myCoordinatorCOR.clearData ();
      }

//------------------------------------------------------------------------------
// hasPartsOrOperations ()
//
// Returns true iff there is at least one Part or Operation in myProblem.
//------------------------------------------------------------------------------

   boolean hasPartsOrOperations ()
      {
      return
            (myConcComps.get (CompClassID.PART)     .size () > 0)
         || (myConcComps.get (CompClassID.OPERATION).size () > 0);
      }

//------------------------------------------------------------------------------
// getInstances (theCompClass, theCompClassID)
//
// Verifies that theCompClass and theCompClassID correspond to the same class.
// Let C be that Component class. Returns an unmodifiable List of all instances
// of class C that belong to this Coordinator, listed in creation order.
//------------------------------------------------------------------------------

  @SuppressWarnings ("unchecked")

   <C extends Component> List <C> getInstances (
         Class <C>   theCompClass,
         CompClassID theCompClassID)
      {
      assert (theCompClassID.getCompClass () == theCompClass);

      return (List <C>) unmodConcComps.get (theCompClassID);
      }

//------------------------------------------------------------------------------
// setCurComponent (theComp)
//
// Sets the current Component for this Coordinator to theComp.
// Asserts that the current Component does not already exist.
//------------------------------------------------------------------------------

   void setCurComponent (Component theComp)
      {
      assert (curComponent == null);

      curComponent = theComp;
      }

//------------------------------------------------------------------------------
// clearCurComponent ()
//
// Sets the current Component for this Coordinator to null.
//------------------------------------------------------------------------------

   void clearCurComponent ()
      {
      curComponent = null;
      }

//------------------------------------------------------------------------------
// getCurComponent ()
//
// Returns the current Component for this Coordinator.
// Asserts that the current Component exists.
//------------------------------------------------------------------------------

   Component getCurComponent ()
      {
      assert (curComponent != null);

      return curComponent;
      }

//------------------------------------------------------------------------------
// Accessors.
//------------------------------------------------------------------------------

   CoordinatorCOR getCoordinatorCOR ()
      {
      return myCoordinatorCOR;
      }

   MessageMgr getMessageMgr ()
      {
      return myMessageMgr;
      }

   List <Component> getComponents ()
      {
      return unmodComponents;
      }

   Problem getProblem ()
      {
      return myProblem;
      }

   int getNPartsCreated ()
      {
      return nPartsCreated;
      }

   int getNDemandsCreated ()
      {
      return nDemandsCreated;
      }

   int getNOperationsCreated ()
      {
      return nOperationsCreated;
      }

   AttBldrCOR getAttBldrCOR ()
      {
      return myAttBldrCOR;
      }

//------------------------------------------------------------------------------
// deactivate ().
//
// Override from class PreservedInternal.
//------------------------------------------------------------------------------

   void deactivate ()
      {
      int       theIdx;
      Component theComp;

      for (theIdx = myComponents.size () - 1; theIdx >= 0; theIdx --)
         {
         theComp = myComponents.get (theIdx);

         if (theComp != myProblem)
            theComp.deactivate ();
         }

      myMessageMgr.deactivate ();

      for (CaseToken theCaseToken: myCaseTokenCORMap.keySet ())
         {
         myCaseTokenCORMap.get (theCaseToken).deactivate ();
         }

      myCoordinatorCOR.deactivate ();
      super           .deactivate ();

      assert (allActiveCoordinators.get ().contains (this));

      allActiveCoordinators.get ().remove (this);
      }

//==============================================================================
// Private Methods
//==============================================================================

//------------------------------------------------------------------------------
// buildOrdAttImps ()
//
// Creates and stores the OrdAttImps that are to belong to this Coordinator.
//------------------------------------------------------------------------------

   private void buildOrdAttImps ()
      {
      String unmatchedAttNames;

      myAttBldrCOR = new AttBldrCOR (myCoordinatorCOR);

      for (Attribute <?> theAtt: Attribute.getAttributes ())
         {
         if (getAttImpFor (theAtt) == null)
            {
            theAtt.makeOrdAttImp (this);
            }
         }

      unmatchedAttNames = myAttBldrCOR.getAllAttNames ();

      if (! unmatchedAttNames.isEmpty ())
         {
         throw
            new AssertionError (
               "\n\n"
             + "           "
             + "The following C++ Atts did not match any Java OrdAttImp:\n"
             + "              " + unmatchedAttNames + "\n");
         }

      myAttBldrCOR.deactivate ();

      myAttBldrCOR = null;
      }

//------------------------------------------------------------------------------
// buildCaseTokenCORs (theCaseTokens)
//
// Creates and stores a CaseTokenCOR for each listed theCaseToken.
//------------------------------------------------------------------------------

   private void buildCaseTokenCORs (CaseToken[] theCaseTokens)
      {
      CaseTokenCOR theCaseTokenCOR;

      for (CaseToken theCaseToken: theCaseTokens)
         {
         theCaseTokenCOR =
            new CaseTokenCOR (
               myCoordinatorCOR,
               theCaseToken.toString ());

         myCaseTokenCORMap.put (theCaseToken, theCaseTokenCOR);
         }
      }

//------------------------------------------------------------------------------
// createConcCompList (theClass)
//
// Creates and stores the ArrayList that is to be used as the List of all
// instances of the class represented by theClass that are owned by this
// Coordinator.
// Creates and stores an unmodifiable view of the List.
//------------------------------------------------------------------------------

   private <C extends Component> void createConcCompList (Class <C> theClass)
      {
      CompClassID   theCompClassID;
      ArrayList <C> theConcCompList;
      List <C>      unmodConcCompList;

      theCompClassID    = CompClassID.getInstanceFor (theClass);

      theConcCompList   = new ArrayList <C> ();

      unmodConcCompList = Collections.unmodifiableList (theConcCompList);

      myConcComps   .put (theCompClassID, theConcCompList);
      unmodConcComps.put (theCompClassID, unmodConcCompList);
      }

//------------------------------------------------------------------------------
// deactivateComponentsDeletedInWit ()
//
// Shuts down all Components corresponding to elements in WIT that have been
// deleted by witPurgeData.
//------------------------------------------------------------------------------

   private void deactivateComponentsDeletedInWit ()
      {
      int       theIdx;
      Component theComp;

      for (theIdx = myComponents.size () - 1; theIdx >= 0; theIdx --)
         {
         theComp = myComponents.get (theIdx);

         if (theComp != myProblem)
            if (theComp.getComponentCOR ().wasDeletedInWit ())
               theComp.deactivate ();
         }
      }

//------------------------------------------------------------------------------
// activeOnly (theHashMap)
//
// Creates and returns a copy of theHashMap with all mappings to inactive
// PreservedObjects removed.
//------------------------------------------------------------------------------

   static
      <P extends PreservedObject>
         HashMap <String, P>
            activeOnly (HashMap <String, P> theHashMap)
      {
      int                 nActive;
      int                 theCap;
      HashMap <String, P> newHashMap;
      P                   theValue;

      nActive = 0;

      for (P theP: theHashMap.values ())
         if (theP.isPIActive ())
            nActive ++;

      theCap     = (int) Math.ceil (nActive / 0.75);

      newHashMap = new HashMap <String, P> (theCap);

      for (Map.Entry <String, P> theEntry: theHashMap.entrySet ())
         {
         if (theEntry.getValue ().isPIActive ())
            {
            newHashMap.put (theEntry.getKey (), theEntry.getValue ());
            }
         }

      return newHashMap;
      }

//------------------------------------------------------------------------------
// selfTestAttImps ()
//
// Performs a self-test on the AttImps that belong to this Coordinator.
//------------------------------------------------------------------------------

   private void selfTestAttImps ()
      {
      AttImp <?> theAttImp;

      for (Attribute <?> theAtt: Attribute.getAttributes ())
         {
         theAttImp = getAttImpFor (theAtt);

         theAttImp.verifyValueType    ();
         theAttImp.verifyAppliesTo    ();
         theAttImp.verifyIsModifiable ();
         }
      }

//==============================================================================
// Private Fields
//==============================================================================
 
//------------------------------------------------------------------------------
// allActiveCoordinators
//
// allActiveCoordinators.get () is the set of all currently active Coordinators
// that were created in the current thread. Their presence in this Set prevents
// them from being garbage-collected until they are deactivated.
//------------------------------------------------------------------------------

   private
      static final ThreadLocal <HashSet <Coordinator>> allActiveCoordinators;

//------------------------------------------------------------------------------
// myCoordinatorCOR.
//
// The CoordinatorCOR that belongs to this Coordinator.
//------------------------------------------------------------------------------

   private final CoordinatorCOR myCoordinatorCOR;

//------------------------------------------------------------------------------
// myProblem.
//
// The Problem that owns this Coordinator.
//------------------------------------------------------------------------------

   private final Problem myProblem;

//------------------------------------------------------------------------------
// myAttImpArr
//
// myAttImpArr[theAtt.getIndex ()] is the AttImp that belongs to this
// Coordinator corresponding to theAtt.
//------------------------------------------------------------------------------

   private final AttImp <?>[] myAttImpArr;

//------------------------------------------------------------------------------
// myCaseTokenCORMap
//
// Maps from all the of CaseTokens defined in WIT-J/Java to the CaseTokenCORs
// that represent the matching WIT-J/C++ CaseTokens.
//------------------------------------------------------------------------------

   private final HashMap <CaseToken, CaseTokenCOR> myCaseTokenCORMap;

//------------------------------------------------------------------------------
// myMessageMgr.
//
// The MessageMgr that belongs to this Problem.
//------------------------------------------------------------------------------

   private final MessageMgr myMessageMgr;

//------------------------------------------------------------------------------
// myComponents.
//
// The list of all Components that belong to this Coordinator.
//------------------------------------------------------------------------------

   private final ArrayList <Component> myComponents;

//------------------------------------------------------------------------------
// unmodComponents.
//
// Unmodifiable view of myComponents.
//------------------------------------------------------------------------------

   private final List <Component> unmodComponents;

//------------------------------------------------------------------------------
// myConcComps.
// "My Concrete Components"
//
// myConcComps.get (theCompClassID) is a List of all instances of the Class
// corresponding to theCompClassID that belong to this Coordinator, listed in
// creation order.
//------------------------------------------------------------------------------

   private
      final
         EnumMap <CompClassID, ArrayList <? extends Component>> myConcComps;

//------------------------------------------------------------------------------
// unmodConcComps.
//
// "Unmodified Concrete Components"
// unmodConcComps.get (theCompClassID) is an unmodifiable view of
// myConcComps   .get (theCompClassID).
//------------------------------------------------------------------------------

   private
      final
         EnumMap <CompClassID, List <? extends Component>> unmodConcComps;

//------------------------------------------------------------------------------
// myPartMap
// myOpnMap
//
// myPartMap.get (thePartName) is the Part that belongs to this Coordinator
// whose PartName matches thePartName, if any; otherwise null.
//
// myOpnMap.get (theOpnName) is the Operation that belongs to this Coordinator
// whose OperationName matches theOpnName, if any; otherwise null.
//------------------------------------------------------------------------------

   private HashMap <String, Part>      myPartMap;
   private HashMap <String, Operation> myOpnMap;

//------------------------------------------------------------------------------
// nPartsCreated
// nDemandsCreated
// nOperationsCreated
//
// The # of Parts, Demands, and Operations created for this Coordinator,
// regardless of whether or not any of them have been deactivated.
//------------------------------------------------------------------------------

   private int nPartsCreated;
   private int nDemandsCreated;
   private int nOperationsCreated;

//------------------------------------------------------------------------------
// myAttBldrCOR
//
// During the execution of buildOrdAttImps (), myAttBldrCOR is the AttBldrCOR
// used by this method; otherwise, null.
//------------------------------------------------------------------------------

   private AttBldrCOR myAttBldrCOR = null;

//------------------------------------------------------------------------------
// curComponent
//
// The Component for which a "set" or "get" method is currently executing, if
// any; otherwise null.
//------------------------------------------------------------------------------

   private Component curComponent = null;

//==============================================================================
// Static initialization block
//==============================================================================

   static
      {
      allActiveCoordinators =
         new ThreadLocal <HashSet <Coordinator>> ()
            {
            protected HashSet <Coordinator> initialValue ()
               {
               return new HashSet <Coordinator> (10);
               }
            };
      }

//==============================================================================
}
