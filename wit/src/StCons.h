//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef StConsH
#define StConsH

//------------------------------------------------------------------------------
// Header file: "StCons.h"
//
// Contains the declaration the following classes:
//
//    StResCon
//    StSubCon
//    StShipCon
//    StSlbCon
//------------------------------------------------------------------------------

#include <StochCon.h>

//------------------------------------------------------------------------------
// class StResCon
//
// "Stochastic Resource Constraint"
// A resource constraint in a stochastic implosion optimization problem.
//
// Class hierarchy:
//
//    OptVC
//       OptVar
//          StochCon
//             StResCon
//
// Implemented in StCons.C.
//------------------------------------------------------------------------------

class WitStResCon: public WitStochCon
   {
   public:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitStResCon (WitPart *, WitStochLoc *);

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      virtual ~WitStResCon ();

      //-----------------------------------------------------------------------
      // Overriding public virtual member functions.
      //-----------------------------------------------------------------------

      virtual void generateCoeffs ();

   private:

      //-----------------------------------------------------------------------
      // Private member functions.
      //-----------------------------------------------------------------------

      //-----------------------------------------------------------------------
      // Overriding private virtual member functions.
      //-----------------------------------------------------------------------

      virtual double       upperBoundVal ();
      virtual double       lowerBoundVal ();
      virtual const char * classText     ();
      virtual void         printItem     ();

      //-----------------------------------------------------------------------
      // Other private member functions.
      //-----------------------------------------------------------------------

      void genPartCoeffs   ();
      void genMatCoeffs    ();
      void genDemandCoeffs ();
      void genBomEntCoeffs ();
      void genSubCoeffs    ();
      void genBopEntCoeffs ();
         //
         // Generates the Coeffs of this StResCon associated with:
         //    myPart_
         //    myPart_ as a Material, if it is one
         //    The Demands for myPart_
         //    The consuming BOM entries of myPart_
         //    The consuming substitutes of myPart_
         //    The producing BOP entries of myPart_

      void getExecPerRange (
            WitBillEntry * theBillEnt,
            WitPeriod &    execPerF,
            WitPeriod &    execPerL);
         //
         // Sets execPerF and execPerL to be the first and last execution
         // periods, execPer, for which
         // theBillEnt->impactPeriod ()[execPer] == myPer ().

      noCopyCtorAssign (WitStResCon);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitPart * const myPart_;
         //
         // The Part for this StResCon.
   };

//------------------------------------------------------------------------------
// class StSubCon
//
// "Stochastic Substitution Constraint"
// A substituion constraint in a stochastic implosion optimization problem.
//
// Class hierarchy:
//
//    OptVC
//       OptVar
//          StochCon
//             StSubCon
//
// Implemented in StCons.C.
//------------------------------------------------------------------------------

class WitStSubCon: public WitStochCon
   {
   public:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitStSubCon (WitBomEntry *, WitStochLoc *);

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      virtual ~WitStSubCon ();

      //-----------------------------------------------------------------------
      // Overriding public virtual member functions.
      //-----------------------------------------------------------------------

      virtual void generateCoeffs ();

   private:

      //-----------------------------------------------------------------------
      // Private member functions.
      //-----------------------------------------------------------------------

      //-----------------------------------------------------------------------
      // Overriding private virtual member functions.
      //-----------------------------------------------------------------------

      virtual double       upperBoundVal ();
      virtual double       lowerBoundVal ();
      virtual const char * classText     ();
      virtual void         printItem     ();

      //-----------------------------------------------------------------------
      // Other private member functions.
      //-----------------------------------------------------------------------

      noCopyCtorAssign (WitStSubCon);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitBomEntry * const myBomEnt_;
         //
         // The BomEntry for this StSubCon.
   };

//------------------------------------------------------------------------------
// class StShipCon
//
// "Stochastic Shipment Constraint"
// A shipment constraint in a stochastic implosion optimization problem.
//
// Class hierarchy:
//
//    OptVC
//       OptVar
//          StochCon
//             StShipCon
//
// Implemented in StCons.C.
//------------------------------------------------------------------------------

class WitStShipCon: public WitStochCon
   {
   public:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitStShipCon (WitDemand *, WitStochLoc *);

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      virtual ~WitStShipCon ();

      //-----------------------------------------------------------------------
      // Overriding public virtual member functions.
      //-----------------------------------------------------------------------

      virtual void generateCoeffs ();

   private:

      //-----------------------------------------------------------------------
      // Private member functions.
      //-----------------------------------------------------------------------

      //-----------------------------------------------------------------------
      // Overriding private virtual member functions.
      //-----------------------------------------------------------------------

      virtual double       upperBoundVal ();
      virtual double       lowerBoundVal ();
      virtual const char * classText     ();
      virtual void         printItem     ();

      //-----------------------------------------------------------------------
      // Other private member functions.
      //-----------------------------------------------------------------------

      noCopyCtorAssign (WitStShipCon);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitDemand * const myDemand_;
         //
         // The Demand for this StShipCon.
   };

//------------------------------------------------------------------------------
// class StSlbCon
//
// "Stochastic Soft Lower Bound Constraint"
// A soft lower bound constraint in a stochastic implosion optimization problem.
//
// Class hierarchy:
//
//    OptVC
//       OptVar
//          StochCon
//             StSlbCon
//
// Implemented in StCons.C.
//------------------------------------------------------------------------------

class WitStSlbCon: public WitStochCon
   {
   public:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitStSlbCon (WitStSlbvVar *);

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      virtual ~WitStSlbCon ();

      //-----------------------------------------------------------------------
      // Overriding public virtual member functions.
      //-----------------------------------------------------------------------

      virtual void generateCoeffs ();

   private:

      //-----------------------------------------------------------------------
      // Private member functions.
      //-----------------------------------------------------------------------

      //-----------------------------------------------------------------------
      // Overriding private virtual member functions.
      //-----------------------------------------------------------------------

      virtual double       upperBoundVal ();
      virtual double       lowerBoundVal ();
      virtual const char * classText     ();
      virtual void         printItem     ();

      //-----------------------------------------------------------------------
      // Other private member functions.
      //-----------------------------------------------------------------------

      WitStBddVar * myBddVar ();

      noCopyCtorAssign (WitStSlbCon);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitStSlbvVar * const mySlbvVar_;
         //
         // The StSlbvVar for this StSblCon.
   };

#endif
