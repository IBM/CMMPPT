//------------------------------------------------------------------------------
// WIT-J C++ Header File Demand.h.
//
// Contains the declaration of class Demand.
//------------------------------------------------------------------------------

#ifndef Demand_h
#define Demand_h

#include <Component.h>

#include    <string>

//------------------------------------------------------------------------------
// Class Demand
//
// Represents a demand in an implosion problem.
//
// Class Hierarchy:
//
// JavaAccObj
//    Component
//       Demand
//------------------------------------------------------------------------------

namespace WitJ
{
class Demand: public Component
   {
   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   explicit Demand (Part * thePart, const char * theDemandNameCStr);
      //
      // Constructor.

   ~Demand ();
      //
      // Destructor.

   virtual Demand * asDemand ();
      //
      // Override from class JavaAccObj.

   virtual void setBoundSet (
         const double *,
         const double *,
         const double *);
      //
      // Override from class Component.

   virtual void getBoundSet (
         DblArrayJOR & hardLBAJOR,
         DblArrayJOR & softLBAJOR,
         DblArrayJOR & hardUBAJOR);
      //
      // Override from class Component.

   virtual void copyComponentData (Component *);
      //
      // Override from class Component.

   void appendToPipSeq (int theShipPer, double incShipVol);
      //
      // Appends the shipment triple (this, theShipPer, incShipVol) to the PIP
      // shipment sequence.

   void setDemandName (const char *);
      //
      // Sets the DemandName of this Demand within the Demand itself and in WIT.

   virtual void getPegging (CaseToken *, int, PeggingTripleALJOR &);
      //
      // Override from class Component.

   const char * getPartName ();
      //
      // Access Wrapper from myPart_.

   //---------------------------------------------------------------------------
   // Wrappers for WIT API Functions.
   //---------------------------------------------------------------------------

   double incHeurAlloc (int shipPer, double desIncVol);

   //---------------------------------------------------------------------------
   // Accessors.
   //---------------------------------------------------------------------------

   const char * getDemandName ();

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   Demand              (const Demand &);
   Demand & operator = (const Demand &);
      //
      // Not implemented; prevents accidental copying and assignment.

   //---------------------------------------------------------------------------
   // Private member data.
   //---------------------------------------------------------------------------

   Part * const myPart_;
      //
      // The demanded Part for this Demand.

   std::string myDemandName_;
      //
      // The DemandName of this Demand.
   };
};
//
// End of namespace WitJ.

#endif
