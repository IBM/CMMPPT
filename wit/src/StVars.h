//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef StVarsH
#define StVarsH

//------------------------------------------------------------------------------
// Header file: "StVars.h"
//
// Contains the declaration the following classes:
//
//    StScrapVar
//    StNonSubVar
//    StSubVar
//    StShipVar
//    StBddVar
//    StExecVar
//    StStockVar
//    StCumShipVar
//    StSlbvVar
//------------------------------------------------------------------------------

#include <StochVar.h>

//------------------------------------------------------------------------------
// class StScrapVar
//
// "Stochastic Scrap Variable"
// A scrap variable in a stochastic implosion optimization problem.
//
// Class hierarchy:
//
//    OptVC
//       OptVar
//          StochVar
//             StScrapVar
//
// Implemented in StVars.C.
//------------------------------------------------------------------------------

class WitStScrapVar: public WitStochVar
   {
   public:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitStScrapVar (WitPart *, WitStochLoc *);

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      virtual ~WitStScrapVar ();

      //-----------------------------------------------------------------------
      // Overriding public virtual member functions.
      //-----------------------------------------------------------------------

      virtual double compObjCoeff ();
      virtual void   uploadSolnTo (WitScenario *);

      //-----------------------------------------------------------------------
      // Access functions.
      //-----------------------------------------------------------------------

      accessFunc (WitPart *, myPart)

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

      noCopyCtorAssign (WitStScrapVar);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitPart * const myPart_;
         //
         // The Part for this StScrapVar.
   };

//------------------------------------------------------------------------------
// class StNonSubVar
//
// "Stochastic Non-Substitution Variable"
// A non-substitution variable in a stochastic implosion optimization problem.
//
// Class hierarchy:
//
//    OptVC
//       OptVar
//          StochVar
//             StNonSubVar
//
// Implemented in StVars.C.
//------------------------------------------------------------------------------

class WitStNonSubVar: public WitStochVar
   {
   public:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitStNonSubVar (WitBomEntry *, WitStochLoc *);

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      virtual ~WitStNonSubVar ();

      //-----------------------------------------------------------------------
      // Overriding public virtual member functions.
      //-----------------------------------------------------------------------

      virtual double compObjCoeff ();
      virtual void   uploadSolnTo (WitScenario *);

      //-----------------------------------------------------------------------
      // Access functions.
      //-----------------------------------------------------------------------

      accessFunc (WitBomEntry *, myBomEnt)

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

      noCopyCtorAssign (WitStNonSubVar);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitBomEntry * const myBomEnt_;
         //
         // The BomEntry for this StNonSubVar.
   };

//------------------------------------------------------------------------------
// class StSubVar
//
// "Stochastic Substitution Variable"
// A substitution variable in a stochastic implosion optimization problem.
//
// Class hierarchy:
//
//    OptVC
//       OptVar
//          StochVar
//             StSubVar
//
// Implemented in StVars.C.
//------------------------------------------------------------------------------

class WitStSubVar: public WitStochVar
   {
   public:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitStSubVar (WitSubEntry *, WitStochLoc *);

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      virtual ~WitStSubVar ();

      //-----------------------------------------------------------------------
      // Overriding public virtual member functions.
      //-----------------------------------------------------------------------

      virtual double compObjCoeff ();
      virtual void   uploadSolnTo (WitScenario *);

      //-----------------------------------------------------------------------
      // Access functions.
      //-----------------------------------------------------------------------

      accessFunc (WitSubEntry *, mySub)

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

      noCopyCtorAssign (WitStSubVar);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitSubEntry * const mySub_;
         //
         // The SubEntry for this StSubVar.
   };

//------------------------------------------------------------------------------
// class StShipVar
//
// "Stochastic Shipment Variable"
// A shipment variable in a stochastic implosion optimization problem.
//
// Class hierarchy:
//
//    OptVC
//       OptVar
//          StochVar
//             StShipVar
//
// Implemented in StVars.C.
//------------------------------------------------------------------------------

class WitStShipVar: public WitStochVar
   {
   public:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitStShipVar (WitDemand *, WitStochLoc *);

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      virtual ~WitStShipVar ();

      //-----------------------------------------------------------------------
      // Overriding public virtual member functions.
      //-----------------------------------------------------------------------

      virtual double compObjCoeff ();
      virtual void   uploadSolnTo (WitScenario *);

      //-----------------------------------------------------------------------
      // Access functions.
      //-----------------------------------------------------------------------

      accessFunc (WitDemand *, myDemand)

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

      noCopyCtorAssign (WitStShipVar);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitDemand * const myDemand_;
         //
         // The Demand for this StShipVar.
   };

//------------------------------------------------------------------------------
// class StBddVar
//
// "Stochastic Bounded Variable"
// A variable that corresponds to a BoundSet in a stochastic implosion
// optimization problem.
//
// Class hierarchy:
//
//    OptVC
//       OptVar
//          StochVar
//             StBddVar
//
// Implemented in StVars.C.
//------------------------------------------------------------------------------

class WitStBddVar: public WitStochVar
   {
   public:

      //-----------------------------------------------------------------------
      // Public member functions.
      //-----------------------------------------------------------------------

      virtual void genStSlbvVarIfNeeded ();
         //
         // Override from class StochVar.

      virtual WitBoundSet * myBoundSet () = 0;
         //
         // Returns the BoundSet that bounds this BoundedVar.

   protected:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitStBddVar (WitStochLoc *);

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      virtual ~WitStBddVar ();

      //-----------------------------------------------------------------------
      // Other protected member functions.
      //-----------------------------------------------------------------------

      double hardLB ();
      double hardUB ();

   private:

      //-----------------------------------------------------------------------
      // Private member functions.
      //-----------------------------------------------------------------------

      noCopyCtorAssign (WitStBddVar);
   };

//------------------------------------------------------------------------------
// class StExecVar
//
// "Stochastic Execution Variable"
// An execution variable in a stochastic implosion optimization problem.
//
// Class hierarchy:
//
//    OptVC
//       OptVar
//          StochVar
//             StBddVar
//                StExecVar
//
// Implemented in StVars.C.
//------------------------------------------------------------------------------

class WitStExecVar: public WitStBddVar
   {
   public:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitStExecVar (WitOperation *, WitStochLoc *);

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      virtual ~WitStExecVar ();

      //-----------------------------------------------------------------------
      // Overriding public virtual member functions.
      //-----------------------------------------------------------------------

      virtual double        compObjCoeff ();
      virtual void          uploadSolnTo (WitScenario *);
      virtual WitBoundSet * myBoundSet   ();

      //-----------------------------------------------------------------------
      // Access functions.
      //-----------------------------------------------------------------------

      accessFunc (WitOperation *, myOpn)

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

      noCopyCtorAssign (WitStExecVar);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitOperation * const myOpn_;
         //
         // The Operation for this StExecVar.
   };

//------------------------------------------------------------------------------
// class StStockVar
//
// "Stochastic Stock Variable"
// An stock variable in a stochastic implosion optimization problem.
//
// Class hierarchy:
//
//    OptVC
//       OptVar
//          StochVar
//             StBddVar
//                StStockVar
//
// Implemented in StVars.C.
//------------------------------------------------------------------------------

class WitStStockVar: public WitStBddVar
   {
   public:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitStStockVar (WitMaterial *, WitStochLoc *);

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      virtual ~WitStStockVar ();

      //-----------------------------------------------------------------------
      // Overriding public virtual member functions.
      //-----------------------------------------------------------------------

      virtual double        compObjCoeff ();
      virtual void          uploadSolnTo (WitScenario *);
      virtual WitBoundSet * myBoundSet   ();

      //-----------------------------------------------------------------------
      // Access functions.
      //-----------------------------------------------------------------------

      accessFunc (WitMaterial *, myMat)

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

      noCopyCtorAssign (WitStStockVar);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitMaterial * const myMat_;
         //
         // The Material for this StStockVar.
   };

//------------------------------------------------------------------------------
// class StCumShipVar
//
// "Stochastic Cumulative Shipment Variable"
// A cumulative shipment variable in a stochastic implosion optimization
// problem.
//
// Class hierarchy:
//
//    OptVC
//       OptVar
//          StochVar
//             StBddVar
//                StCumShipVar
//
// Implemented in StVars.C.
//------------------------------------------------------------------------------

class WitStCumShipVar: public WitStBddVar
   {
   public:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitStCumShipVar (WitDemand *, WitStochLoc *);

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      virtual ~WitStCumShipVar ();

      //-----------------------------------------------------------------------
      // Overriding public virtual member functions.
      //-----------------------------------------------------------------------

      virtual double        compObjCoeff ();
      virtual void          uploadSolnTo (WitScenario *);
      virtual WitBoundSet * myBoundSet   ();

      //-----------------------------------------------------------------------
      // Access functions.
      //-----------------------------------------------------------------------

      accessFunc (WitDemand *, myDemand)

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

      noCopyCtorAssign (WitStCumShipVar);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitDemand * const myDemand_;
         //
         // The Demand for this StCumShipVar.
   };

//------------------------------------------------------------------------------
// class StSlbvVar
//
// "Stochastic Soft Lower Bound Violation Variable"
// A soft lower bound violation variable in a stochastic implosion optimization
// problem.
//
// Class hierarchy:
//
//    OptVC
//       OptVar
//          StochVar
//             StSlbvVar
//
// Implemented in StVars.C.
//------------------------------------------------------------------------------

class WitStSlbvVar: public WitStochVar
   {
   public:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitStSlbvVar (WitStBddVar *);

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      virtual ~WitStSlbvVar ();

      //-----------------------------------------------------------------------
      // Overriding public virtual member functions.
      //-----------------------------------------------------------------------

      virtual double compObjCoeff        ();
      virtual void   genStSlbConIfNeeded ();
      virtual double stBoundsCoeffVal    ();
      virtual void   uploadSolnTo        (WitScenario *);
      virtual bool   isStSlbvVar         ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitStBddVar *, myBddVar)

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

      noCopyCtorAssign (WitStSlbvVar);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitStBddVar * const myBddVar_;
         //
         // The StBddVar for this StSlbvVar.
   };

#endif
