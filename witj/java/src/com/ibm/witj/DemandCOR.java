
//==============================================================================
// WIT-J Java Source File DemandCOR.java.
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;

//==============================================================================
// Class DemandCOR
//
// "Demand C++ Object Representation"
// A Java representation of a C++ Demand.
//
// Class Hierarchy:
//
// PreservedInternal
//    JavaAccObjCOR
//       ComponentCOR
//          DemandCOR
//==============================================================================

final class DemandCOR extends ComponentCOR
{
//==============================================================================
// Package-Internal Constructor
//==============================================================================

   DemandCOR (PartCOR thePartCOR, String theDemandName)
      {
      createAndStoreCppDemand (thePartCOR, theDemandName);
      }

//==============================================================================
// Package-Internal Methods
//==============================================================================

//------------------------------------------------------------------------------
// appendToPipSeq (theShipPer, incShipVol)
//
// Appends the shipment triple (my C++ Demand, theShipPer, incShipVol) to the
// PIP shipment sequence.
//------------------------------------------------------------------------------

   native void appendToPipSeq (int theShipPer, double incShipVol);

//------------------------------------------------------------------------------
// setDemandName
//
// Sets the DemandName of the C++ Demand.
//------------------------------------------------------------------------------

   native void setDemandName (String theDemandName);

//------------------------------------------------------------------------------
// Wrappers for WIT API Functions.
//------------------------------------------------------------------------------

   native double incHeurAlloc (int shipPer, double desIncVol);

//==============================================================================
// Private Native Methods
//==============================================================================

//------------------------------------------------------------------------------
// createAndStoreCppDemand
// 
// Creates a new C++ Demand and stores its pointer in this DemandCOR.
//------------------------------------------------------------------------------

   private native void createAndStoreCppDemand (
      PartCOR thePartCOR,
      String  theDemandName);

//==============================================================================
}
