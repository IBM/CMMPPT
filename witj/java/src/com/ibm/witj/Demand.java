
//==============================================================================
// WIT-J Java Source File Demand.java.
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;
import static com.ibm.witj.PggCase.*;

import       java.util.*;

//==============================================================================
// Class Demand
//
// Represents a demand in an implosion problem.
//
// Class Hierarchy:
//
// ThreadedObject
//    PreservedObject
//       Component
//          Demand
//==============================================================================

public final class Demand extends Component
{
//==============================================================================
// Public Methods
//==============================================================================

//------------------------------------------------------------------------------
// newInstance (thePart, theDemandName)
//
// Factory method: Constructs and returns a new Demand for the arguments.
//------------------------------------------------------------------------------

   public static Demand newInstance (Part thePart, String theDemandName)
      {
      try
         {
         notAllowedWhenTerminated ("Demand.newInstance");

         mustBeActive  (thePart,       "thePart",       "Demand.newInstance");
         mustBeNonNull (theDemandName, "theDemandName", "Demand.newInstance");

         return new Demand (thePart, theDemandName);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// incHeurAlloc (shipPeriod, desIncVol)
//
// Wrapper for witIncHeurAlloc.
//------------------------------------------------------------------------------

   public double incHeurAlloc (int shipPeriod, double desIncVol)
      {
      try
         {
         mustBeActive ("Demand.incHeurAlloc");

         return myDemandCOR.incHeurAlloc (shipPeriod, desIncVol);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// appendToPipSeq (theShipPer, incShipVol)
//
// Appends the shipment triple (this Demand, theShipPer, incShipVol) to the
// PIP shipment sequence.
//------------------------------------------------------------------------------

   public void appendToPipSeq (int theShipPer, double incShipVol)
      {
      try
         {
         mustBeActive ("Demand.appendToPipSeq");

         myDemandCOR.appendToPipSeq (theShipPer, incShipVol);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getConsVolPip (shipPeriod)
//
// Returns the consVol PIP for this Demand in shipPeriod.
//------------------------------------------------------------------------------

   public ArrayList <PeggingTriple <Part>> getConsVolPip (int shipPeriod)
      {
      try
         {
         mustBeActive ("Demand.getConsVolPip");

         return getPegging (DEM_CONS_PIP, Part.class, shipPeriod);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getCoExecVolPip (shipPeriod)
//
// Returns the coExecVol PIP for this Demand in shipPeriod.
//------------------------------------------------------------------------------

   public ArrayList <PeggingTriple <BopEntry>> getCoExecVolPip (int shipPeriod)
      {
      try
         {
         mustBeActive ("Demand.getCoExecVolPip");

         return getPegging (DEM_CO_EXEC_PIP, BopEntry.class, shipPeriod);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getExecVolPip (shipPeriod)
//
// Returns the execVol PIP for this Demand in shipPeriod.
//------------------------------------------------------------------------------

   public ArrayList <PeggingTriple <Operation>> getExecVolPip (int shipPeriod)
      {
      try
         {
         mustBeActive ("Demand.getExecVolPip");

         return getPegging (DEM_EXEC_PIP, Operation.class, shipPeriod);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getProdVolPip (shipPeriod)
//
// Returns the prodVol PIP for this Demand in shipPeriod.
//------------------------------------------------------------------------------

   public ArrayList <PeggingTriple <Part>> getProdVolPip (int shipPeriod)
      {
      try
         {
         mustBeActive ("Demand.getProdVolPip");

         return getPegging (DEM_PROD_PIP, Part.class, shipPeriod);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getSideVolPip (shipPeriod)
//
// Returns the sideVol PIP for this Demand in shipPeriod.
//------------------------------------------------------------------------------

   public ArrayList <PeggingTriple <Part>> getSideVolPip (int shipPeriod)
      {
      try
         {
         mustBeActive ("Demand.getSideVolPip");

         return getPegging (DEM_SIDE_PIP, Part.class, shipPeriod);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getSubVolPip (shipPeriod)
//
// Returns the subVol PIP for this Demand in shipPeriod.
//------------------------------------------------------------------------------

   public ArrayList <PeggingTriple <Substitute>> getSubVolPip (int shipPeriod)
      {
      try
         {
         mustBeActive ("Demand.getSubVolPip");

         return getPegging (DEM_SUB_PIP, Substitute.class, shipPeriod);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getSupplyVolPip (shipPeriod)
//
// Returns the supplyVol PIP for this Demand in shipPeriod.
//------------------------------------------------------------------------------

   public ArrayList <PeggingTriple <Part>> getSupplyVolPip (int shipPeriod)
      {
      try
         {
         mustBeActive ("Demand.getSupplyVolPip");

         return getPegging (DEM_SUPPLY_PIP, Part.class, shipPeriod);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getExecVolPegging (shipPeriod)
//
// Returns the execVol concurrent pegging for this Demand in shipPeriod.
//------------------------------------------------------------------------------

   public ArrayList <PeggingTriple <Operation>> getExecVolPegging (
         int shipPeriod)
      {
      try
         {
         mustBeActive ("Demand.getExecVolPegging");

         return getPegging (DEM_EXEC_CP, Operation.class, shipPeriod);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getSubVolPegging (shipPeriod)
//
// Returns the subVol concurrent pegging for this Demand in shipPeriod.
//------------------------------------------------------------------------------

   public ArrayList <PeggingTriple <Substitute>> getSubVolPegging (
         int shipPeriod)
      {
      try
         {
         mustBeActive ("Demand.getSubVolPegging");

         return getPegging (DEM_SUB_CP, Substitute.class, shipPeriod);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// Accessors.
//------------------------------------------------------------------------------

   public Part getPart ()
      {
      try
         {
         mustBeActive ("Demand.getPart");

         return myPart;
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
// toStringActive ().
//
// Override from class PreservedObject.
//------------------------------------------------------------------------------

   String toStringActive ()
      {
      return "Demand \"" + myName + "\" for " + myPart;
      }

//------------------------------------------------------------------------------
// deactivate
//
// Override from class PreservedObject.
//------------------------------------------------------------------------------

   void deactivate ()
      {
      myDemandCOR.deactivate ();
      super      .deactivate ();
      }

//------------------------------------------------------------------------------
// restoreAfterPurge ()
//
// Override from class Component.
//------------------------------------------------------------------------------

   void restoreAfterPurge ()
      {
      }

//------------------------------------------------------------------------------
// setName ()
//
// Override from class Component.
//------------------------------------------------------------------------------

   void setName (String theName)
      {
      myDemandCOR.setDemandName (theName);

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
      return CompClassID.DEMAND;
      }

//------------------------------------------------------------------------------
// getComponentCOR
//
// Override from class Component.
//------------------------------------------------------------------------------

   ComponentCOR getComponentCOR ()
      {
      return myDemandCOR;
      }

//------------------------------------------------------------------------------
// Accessors.
//------------------------------------------------------------------------------

   DemandCOR getDemandCOR ()
      {
      return myDemandCOR;
      }

//==============================================================================
// Private Constructor
//
// Arguments:
//    thePart:       The demanded Part for the Demand.
//    theDemandName: The DemandName of the Demand.
//==============================================================================

   private Demand (Part thePart, String theDemandName)
      {
      super (thePart.getCoordinator ());

      myPart      = thePart;

      myName      = theDemandName;

      myDemandCOR = new DemandCOR (thePart.getPartCOR (), theDemandName);

      getCoordinator ().recordDemand (this);
      myPart           .recordDemand (this);
      }

//==============================================================================
// Private Fields
//==============================================================================

//------------------------------------------------------------------------------
// myPart.
//
// The demanded Part for this Demand.
//------------------------------------------------------------------------------

   private final Part myPart;

//------------------------------------------------------------------------------
// myName.
//
// The name of this Demand.
//------------------------------------------------------------------------------

   private String myName;

//------------------------------------------------------------------------------
// myDemandCOR.
//
// The DemandCOR that belongs to this Demand.
//------------------------------------------------------------------------------

   private final DemandCOR myDemandCOR;

//==============================================================================
}
