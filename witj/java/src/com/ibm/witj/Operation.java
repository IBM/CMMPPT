
//==============================================================================
// WIT-J Java Source File Operation.java.
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;
import static com.ibm.witj.PggCase.*;

import           java.util.*;

//==============================================================================
// Class Operation
//
// Represents an operation in an implosion problem.
//==============================================================================

public final class Operation extends Component
{
//==============================================================================
// Public Methods
//
// Class Hierarchy:
//
// ThreadedObject
//    PreservedObject
//       Component
//          Operation
//==============================================================================

//------------------------------------------------------------------------------
// newInstance (theProblem, theOpnName)
//
// Factory method: Constructs and returns a new Operation for the arguments.
//------------------------------------------------------------------------------

   public static Operation newInstance (Problem theProblem, String theOpnName)
      {
      try
         {
         notAllowedWhenTerminated                ("Operation.newInstance");
         mustBeActive  (theProblem, "theProblem", "Operation.newInstance");
         mustBeNonNull (theOpnName, "theOpnName", "Operation.newInstance");

         return new Operation (theProblem.getCoordinator (), theOpnName);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getBomEntries ()
//
// Returns an unmodifiable List of all the BomEntries associated with this
// Operation, listed in the order of creation.
//------------------------------------------------------------------------------

   public List <BomEntry> getBomEntries ()
      {
      try
         {
         mustBeActive ("Operation.getBomEntries");

         return unmodBomEntries;
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getBopEntries ()
//
// Returns an unmodifiable List of all the BopEntries associated with this
// Operation, listed in the order of creation.
//------------------------------------------------------------------------------

   public List <BopEntry> getBopEntries ()
      {
      try
         {
         mustBeActive ("Operation.getBopEntries");

         return unmodBopEntries;
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getBomEntriesTo (thePart)
//
// Creates and returns a List of all BomEntries from this Operation to thePart,
// listed in order of creation.
//------------------------------------------------------------------------------

   public List <BomEntry> getBomEntriesTo (Part thePart)
      {
      try
         {
         mustBeActive ("Operation.getBomEntriesTo");

         return thePart.findAssociatesIn (myBomEntries);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getBopEntriesTo (thePart)
//
// Creates and returns a List of all BopEntries from this Operation to thePart,
// listed in order of creation.
//------------------------------------------------------------------------------

   public List <BopEntry> getBopEntriesTo (Part thePart)
      {
      try
         {
         mustBeActive ("Operation.getBopEntriesTo");

         return thePart.findAssociatesIn (myBopEntries);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getUniqueBomEntryTo (thePart)
//
// If there is exactly one BomEntry from this Operation to thePart, it is
//    returned.
// If there are none, null is returned.
// If there are more than one, a StatusQuoExeption is thrown.
//------------------------------------------------------------------------------

   public BomEntry getUniqueBomEntryTo (Part thePart)
      {
      try
         {
         mustBeActive ("Operation.getUniqueBomEntryTo");

         return
            thePart.
               findUniqueAssociateIn (
                  myBomEntries,
                  "   theOperation.getUniqueBomEntryTo (thePart) was\n"
                + "   called where theOperation and thePart are connected\n"
                + "   by more than one BomEntry:");
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getUniqueBopEntryTo (thePart)
//
// If there is exactly one BopEntry from this Operation to thePart, it is
//    returned.
// If there are none, null is returned.
// If there are more than one, a StatusQuoExeption is thrown.
//------------------------------------------------------------------------------

   public BopEntry getUniqueBopEntryTo (Part thePart)
      {
      try
         {
         mustBeActive ("Operation.getUniqueBopEntryTo");

         return
            thePart.
               findUniqueAssociateIn (
                  myBopEntries,
                  "   theOperation.getUniqueBopEntryTo (thePart) was\n"
                + "   called where theOperation and thePart are connected\n"
                + "   by more than one BopEntry:");
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getConsVolPip (execPeriod)
//
// Returns the consVol PIP for this Operation in execPeriod.
//------------------------------------------------------------------------------

   public ArrayList <PeggingTriple <Part>> getConsVolPip (int execPeriod)
      {
      try
         {
         mustBeActive ("Operation.getConsVolPip");

         return getPegging (OPN_CONS_PIP, Part.class, execPeriod);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getCoExecVolPip (execPeriod)
//
// Returns the coExecVol PIP for this Operation in execPeriod.
//------------------------------------------------------------------------------

   public ArrayList <PeggingTriple <BopEntry>> getCoExecVolPip (int execPeriod)
      {
      try
         {
         mustBeActive ("Operation.getCoExecVolPip");

         return getPegging (OPN_CO_EXEC_PIP, BopEntry.class, execPeriod);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getExecVolPip (execPeriod)
//
// Returns the execVol PIP for this Operation in execPeriod.
//------------------------------------------------------------------------------

   public ArrayList <PeggingTriple <Operation>> getExecVolPip (int execPeriod)
      {
      try
         {
         mustBeActive ("Operation.getExecVolPip");

         return getPegging (OPN_EXEC_PIP, Operation.class, execPeriod);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getProdVolPip (execPeriod)
//
// Returns the prodVol PIP for this Operation in execPeriod.
//------------------------------------------------------------------------------

   public ArrayList <PeggingTriple <Part>> getProdVolPip (int execPeriod)
      {
      try
         {
         mustBeActive ("Operation.getProdVolPip");

         return getPegging (OPN_PROD_PIP, Part.class, execPeriod);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getSideVolPip (execPeriod)
//
// Returns the sideVol PIP for this Operation in execPeriod.
//------------------------------------------------------------------------------

   public ArrayList <PeggingTriple <Part>> getSideVolPip (int execPeriod)
      {
      try
         {
         mustBeActive ("Operation.getSideVolPip");

         return getPegging (OPN_SIDE_PIP, Part.class, execPeriod);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getSubVolPip (execPeriod)
//
// Returns the subVol PIP for this Operation in execPeriod.
//------------------------------------------------------------------------------

   public ArrayList <PeggingTriple <Substitute>> getSubVolPip (int execPeriod)
      {
      try
         {
         mustBeActive ("Operation.getSubVolPip");

         return getPegging (OPN_SUB_PIP, Substitute.class, execPeriod);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getSupplyVolPip (execPeriod)
//
// Returns the supplyVol PIP for this Operation in execPeriod.
//------------------------------------------------------------------------------

   public ArrayList <PeggingTriple <Part>> getSupplyVolPip (int execPeriod)
      {
      try
         {
         mustBeActive ("Operation.getSupplyVolPip");

         return getPegging (OPN_SUPPLY_PIP, Part.class, execPeriod);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//==============================================================================
// Package-Internal Methods
//==============================================================================

//------------------------------------------------------------------------------
// restoreAfterPurge ()
//
// Override from class Component.
//------------------------------------------------------------------------------

   void restoreAfterPurge ()
      {
      removeInactive (myBomEntries);
      removeInactive (myBopEntries);
      }

//------------------------------------------------------------------------------
// toStringActive ().
//
// Override from class PreservedObject.
//------------------------------------------------------------------------------

   String toStringActive ()
      {
      return "Operation \"" + myName + "\"";
      }

//------------------------------------------------------------------------------
// setName ()
//
// Override from class Component.
//------------------------------------------------------------------------------

   void setName (String theName)
      {
      myOperationCOR.setOperationName (theName);

      myName = theName;
      }

//------------------------------------------------------------------------------
// getName ()
//
// Override from class Component.
//------------------------------------------------------------------------------

   String getName ()
      {
      return myName;
      }

//------------------------------------------------------------------------------
// getCompClassID
//
// Override from class Component.
//------------------------------------------------------------------------------

   CompClassID getCompClassID ()
      {
      return CompClassID.OPERATION;
      }

//------------------------------------------------------------------------------
// getComponentCOR
//
// Override from class Component.
//------------------------------------------------------------------------------

   ComponentCOR getComponentCOR ()
      {
      return myOperationCOR;
      }

//------------------------------------------------------------------------------
// deactivate
//
// Override from class PreservedObject.
//------------------------------------------------------------------------------

   void deactivate ()
      {
      myOperationCOR.deactivate ();
      super         .deactivate ();
      }

//------------------------------------------------------------------------------
// recordBomEntry (theBomEnt)
// recordBopEntry (theBopEnt)
//
// Each of these methods records the fact that the argument has been created.
//------------------------------------------------------------------------------

   void recordBomEntry (BomEntry theBomEnt)
      {
      assert (theBomEnt.getOperation () == this);

      myBomEntries.add (theBomEnt);
      }

   void recordBopEntry (BopEntry theBopEnt)
      {
      assert (theBopEnt.getOperation () == this);

      myBopEntries.add (theBopEnt);
      }

//------------------------------------------------------------------------------
// Accessors.
//------------------------------------------------------------------------------

   OperationCOR getOperationCOR ()
      {
      return myOperationCOR;
      }

//==============================================================================
// Private Constructor
//==============================================================================

   private Operation (Coordinator theCoord, String theOpnName)
      {
      super (theCoord);

      myName          = theOpnName;

      myOperationCOR  =
         new OperationCOR (theCoord.getCoordinatorCOR (), theOpnName);

      myBomEntries    = new ArrayList <BomEntry> (0);
      myBopEntries    = new ArrayList <BopEntry> (0);

      unmodBomEntries = Collections.unmodifiableList (myBomEntries);
      unmodBopEntries = Collections.unmodifiableList (myBopEntries);

      getCoordinator ().recordOperation (this);
      }

//==============================================================================
// Private Fields
//==============================================================================

//------------------------------------------------------------------------------
// myName;
//
// The OperationName of this Operation.
//------------------------------------------------------------------------------

   private String myName;

//------------------------------------------------------------------------------
// myOperationCOR.
//
// The OperationCOR that belongs to this Operation.
//------------------------------------------------------------------------------

   private final OperationCOR myOperationCOR;

//------------------------------------------------------------------------------
// myBomEntries
// myBopEntries
//
// A List of all BomEntries associated with this Operation.
// A List of all BopEntries associated with this Operation.
// The Components are listed in the order of creation.
//------------------------------------------------------------------------------

   private final ArrayList <BomEntry> myBomEntries;
   private final ArrayList <BopEntry> myBopEntries;

//------------------------------------------------------------------------------
// unmodBomEntries
// unmodBopEntries
//
// Unmodifiable views of:
//    myBomEntries
//    myBopEntries
//------------------------------------------------------------------------------

   private final List <BomEntry> unmodBomEntries;
   private final List <BopEntry> unmodBopEntries;

//==============================================================================
}
