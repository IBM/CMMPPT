//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef DetVarsH
#define DetVarsH

//------------------------------------------------------------------------------
// Header file: "DetVars.h"
//
// Contains the declaration the following classes:
//
//    DetVar
//    ScrapVar
//    NonSubVar
//    SubVar
//    ShipVar
//    BoundedVar
//    ExecVar
//    StockVar
//    CumShipVar
//    SlbvVar
//    MultiObjVar
//    NtbvVar
//------------------------------------------------------------------------------

#include <OptVar.h>

//------------------------------------------------------------------------------
// Class DetVar
//
// "Deterministic Variable"
// An OptVar in a deterministic implosion optimization problem.
//
// Class hierarchy:
//
//    OptVC
//       OptVar
//          DetVar
//             ScrapVar
//             NonSubVar
//             SubVar
//             ShipVar
//             BoundedVar
//                ExecVar
//                StockVar
//                CumShipVar
//             SlbvVar
//             MultiObjVar
//             NtbvVar
//------------------------------------------------------------------------------

class WitDetVar: public WitOptVar
   {
   public:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitDetVar (WitDetImpOP *, WitPeriod);

      WitDetVar (WitDetImpOP *);

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      virtual ~WitDetVar ();
 
      //-----------------------------------------------------------------------
      // Overriding public virtual member functions.
      //-----------------------------------------------------------------------

      virtual WitOptProblem * myOptProblem ();
      virtual WitDetVar *     mandDetVar   ();

      //------------------------------------------------------------------------

      virtual void uploadSoln () = 0;
         //
         // Uploads the primal solution for this DetVar to the corresponding
         // attribute.

      virtual bool isSlbvVar ();
         //
         // Returns true, iff this DetVar is an SlbvVar.

      //-----------------------------------------------------------------------
      // Data access functions.
      //-----------------------------------------------------------------------

      accessFunc (WitDetImpOP *, myDetImpOP)

      inline bool hasPeriod ()
         {
         return (myPeriod_ != -1);
         }

      inline WitPeriod myPeriod ()
         {
         witAssert (hasPeriod ());

         return myPeriod_;
         }

   private:

      //-----------------------------------------------------------------------
      // Private member functions.
      //-----------------------------------------------------------------------

      noCopyCtorAssign (WitDetVar);

      //-----------------------------------------------------------------------
      // Overriding private virtual member functions.
      //-----------------------------------------------------------------------

      virtual void printSource ();

      //-----------------------------------------------------------------------
      // Private pure virtual member functions.
      //-----------------------------------------------------------------------

      virtual void printItem () = 0;
         //
         // Prints to the optProbFile text identifying the non-period
         // data from which this DetVar was generated.

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitDetImpOP * const myDetImpOP_;
         //
         // The DetImpOP that owns this DetVar.

      const WitPeriod myPeriod_;
         //
         // The Period for this DetVar, if it has one.
         // -1, if it doesn't have a Period.
   };

//------------------------------------------------------------------------------
// class ScrapVar
// A scrap variable in the optimization problem.
//
// Class hierarchy:
//
//    OptVC
//       OptVar
//          DetVar
//             ScrapVar
//------------------------------------------------------------------------------

class WitScrapVar: public WitDetVar
   {
   public:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitScrapVar (WitPart *, WitPeriod, WitDetImpOP *);

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      virtual ~WitScrapVar ();

      //-----------------------------------------------------------------------
      // Overriding public virtual member functions.
      //-----------------------------------------------------------------------

      virtual double compObjCoeff   ();
      virtual double indepInitValue ();
      virtual void   uploadSoln     ();

      //-----------------------------------------------------------------------
      // Access functions.
      //-----------------------------------------------------------------------

      accessFunc (WitPart *, myPart)

   private:

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

      noCopyCtorAssign (WitScrapVar);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitPart * const myPart_;
         //
         // The Part with which this ScrapVar is associated.
   };

//------------------------------------------------------------------------------
// class NonSubVar
// A non-substitution variable in the optimization problem.
//
// Class hierarchy:
//
//    OptVC
//       OptVar
//          DetVar
//             NonSubVar
//------------------------------------------------------------------------------

class WitNonSubVar: public WitDetVar
   {
   public:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitNonSubVar (WitBomEntry *, WitPeriod, WitDetImpOP *);

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      virtual ~WitNonSubVar ();

      //-----------------------------------------------------------------------
      // Overriding public virtual member functions.
      //-----------------------------------------------------------------------

      virtual double compObjCoeff   ();
      virtual double indepInitValue ();
      virtual void   uploadSoln     ();

      //-----------------------------------------------------------------------
      // Access functions.
      //-----------------------------------------------------------------------

      accessFunc (WitBomEntry *, myBomEnt)

   private:

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

      noCopyCtorAssign (WitNonSubVar);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitBomEntry * const myBomEnt_;
         //
         // The BomEntry with which this NonSubVar is associated.
   };

//------------------------------------------------------------------------------
// class SubVar
// A substitution variable in the optimization problem.
//
// Class hierarchy:
//
//    OptVC
//       OptVar
//          DetVar
//             SubVar
//------------------------------------------------------------------------------

class WitSubVar: public WitDetVar
   {
   public:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitSubVar (WitSubEntry *, WitPeriod, WitDetImpOP *);

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      virtual ~WitSubVar ();

      //-----------------------------------------------------------------------
      // Overriding public virtual member functions.
      //-----------------------------------------------------------------------

      virtual double compObjCoeff   ();
      virtual double indepInitValue ();
      virtual bool   isAnIntVar     ();
      virtual void   uploadSoln     ();

      //-----------------------------------------------------------------------
      // Access functions.
      //-----------------------------------------------------------------------

      accessFunc (WitSubEntry *, mySub)

   private:

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

      noCopyCtorAssign (WitSubVar);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitSubEntry * const mySub_;
         //
         // The SubEntry with which this SubVar is associated.
   };

//------------------------------------------------------------------------------
// class ShipVar
// A shipment variable in the optimization problem.
//
// Class hierarchy:
//
//    OptVC
//       OptVar
//          DetVar
//             ShipVar
//------------------------------------------------------------------------------

class WitShipVar: public WitDetVar
   {
   public:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitShipVar (WitDemand *, WitPeriod, WitDetImpOP *);

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      virtual ~WitShipVar ();

      //-----------------------------------------------------------------------
      // Overriding public virtual member functions.
      //-----------------------------------------------------------------------

      virtual double compObjCoeff   ();
      virtual double indepInitValue ();
      virtual bool   isAnIntVar     ();
      virtual void   uploadSoln     ();

      //-----------------------------------------------------------------------
      // Access functions.
      //-----------------------------------------------------------------------

      accessFunc (WitDemand *, myDemand)

   private:

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

      noCopyCtorAssign (WitShipVar);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitDemand * const myDemand_;
         //
         // The Demand with which this ShipVar is associated.
   };

//------------------------------------------------------------------------------
// class BoundedVar
// A variable with user-specified bounds in the optimization problem.
//
// Class hierarchy:
//
//    OptVC
//       OptVar
//          DetVar
//             BoundedVar
//                ExecVar
//                StockVar
//                CumShipVar
//------------------------------------------------------------------------------

class WitBoundedVar: public WitDetVar
   {
   public:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitBoundedVar (WitDetImpOP *, WitPeriod);

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      virtual ~WitBoundedVar ();

      //-----------------------------------------------------------------------
      // Public pure virtual function members.
      //-----------------------------------------------------------------------

      virtual WitBoundSet * myBoundSet () = 0;
         //
         // Returns the BoundSet that bounds this BoundedVar.

      //-----------------------------------------------------------------------
      // Other public member functions.
      //-----------------------------------------------------------------------

      double hardLB ();
         //
         // Hard lower bound:         0.0 <= hardLB ()  <  +infinity.

      double softLB ();
         //
         // Soft lower bound:         0.0 <= softLB ()  <  +infinity.

      double hardUB ();
         //
         // Hard upper bound:         0.0 <= hardUB ()  <= +infinity.

      bool needsAnSlbvVar ();
         //
         // Returns true, iff an SlbvVar should be generated for this
         // BoundedVar.

      void printBoundInfo ();
         //
         // Prints bound information of this BoundedVar to optProbFile.

      void printSoln ();
         //
         // Prints the optimization solution associated with this BoundedVar to
         // solnFile.

   private:

      //-----------------------------------------------------------------------
      // Private member functions.
      //-----------------------------------------------------------------------

      noCopyCtorAssign (WitBoundedVar);
   };

//------------------------------------------------------------------------------
// class ExecVar
// An execution variable in the optimization problem.
//
// Class hierarchy:
//
//    OptVC
//       OptVar
//          DetVar
//             BoundedVar
//                ExecVar
//------------------------------------------------------------------------------

class WitExecVar: public WitBoundedVar
   {
   public:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitExecVar (WitOperation *, WitPeriod, WitDetImpOP *);

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      virtual ~WitExecVar ();

      //-----------------------------------------------------------------------
      // Overriding public virtual member functions.
      //-----------------------------------------------------------------------

      virtual double        compObjCoeff   ();
      virtual double        indepInitValue ();
      virtual bool          isAnIntVar     ();
      virtual void          uploadSoln     ();
      virtual WitBoundSet * myBoundSet     ();

      //-----------------------------------------------------------------------
      // Access functions.
      //-----------------------------------------------------------------------

      accessFunc (WitOperation *, myOperation)

   private:

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

      noCopyCtorAssign (WitExecVar);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitOperation * const myOperation_;
         //
         // The Operation with which this ExecVar is associated.
   };

//------------------------------------------------------------------------------
// class StockVar
// A stock variable in the optimization problem.
//
// Class hierarchy:
//
//    OptVC
//       OptVar
//          DetVar
//             BoundedVar
//                StockVar
//------------------------------------------------------------------------------

class WitStockVar: public WitBoundedVar
   {
   public:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitStockVar (WitMaterial *, WitPeriod, WitDetImpOP *);

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      virtual ~WitStockVar ();

      //-----------------------------------------------------------------------
      // Overriding public virtual member functions.
      //-----------------------------------------------------------------------

      virtual double        compObjCoeff   ();
      virtual double        indepInitValue ();
      virtual void          uploadSoln     ();
      virtual WitBoundSet * myBoundSet     ();

      //-----------------------------------------------------------------------
      // Access functions.
      //-----------------------------------------------------------------------

      accessFunc (WitMaterial *, myMaterial)

   private:

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

      noCopyCtorAssign (WitStockVar);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitMaterial * const myMaterial_;
         //
         // The Material with which this StockVar is associated.
   };

//------------------------------------------------------------------------------
// class CumShipVar
// A cumulative shipment variable in the optimization problem.
//
// Class hierarchy:
//
//    OptVC
//       OptVar
//          DetVar
//             BoundedVar
//                CumShipVar
//------------------------------------------------------------------------------

class WitCumShipVar: public WitBoundedVar
   {
   public:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitCumShipVar (WitDemand *, WitPeriod, WitDetImpOP *);

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      virtual ~WitCumShipVar ();

      //-----------------------------------------------------------------------
      // Overriding public virtual member functions.
      //-----------------------------------------------------------------------

      virtual double        compObjCoeff   ();
      virtual double        indepInitValue ();
      virtual void          uploadSoln     ();
      virtual WitBoundSet * myBoundSet     ();

      //-----------------------------------------------------------------------
      // Access functions.
      //-----------------------------------------------------------------------

      accessFunc (WitDemand *, myDemand)

   private:

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

      noCopyCtorAssign (WitCumShipVar);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitDemand * const myDemand_;
         //
         // The Demand with which this CumShipVar is associated.
   };

//------------------------------------------------------------------------------
// class SlbvVar
// A soft lower bound violation variable in the optimization problem.
//
// Class hierarchy:
//
//    OptVC
//       OptVar
//          DetVar
//             SlbvVar
//------------------------------------------------------------------------------

class WitSlbvVar: public WitDetVar
   {
   public:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitSlbvVar (WitBoundedVar *, WitDetImpOP *);

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      virtual ~WitSlbvVar ();

      //-----------------------------------------------------------------------
      // Overriding public virtual member functions.
      //-----------------------------------------------------------------------

      virtual double compObjCoeff ();
      virtual void   uploadSoln   ();
      virtual bool   isSlbvVar    ();

      //-----------------------------------------------------------------------
      // Other public member functions.
      //-----------------------------------------------------------------------

      inline WitBoundSet * myBoundSet ();
         //
         // Returns myBoundedVar_->myBoundSet ().
         // Implemented in this file just below class BoundedVar.

   private:

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

      noCopyCtorAssign (WitSlbvVar);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitBoundedVar * const myBoundedVar_;
         //
         // The BoundedVar to which this SlbvVar applies.
   };

//------------------------------------------------------------------------------
// class MultiObjVar
// A multiple objectives variable in the optimization problem: a variable whose
// value is constrained to match the value of one of the Objectives in multiple
// objectives mode.
//
// Class hierarchy:
//
//    OptVC
//       OptVar
//          DetVar
//             MultiObjVar
//------------------------------------------------------------------------------

class WitMultiObjVar: public WitDetVar
   {
   public:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitMultiObjVar (WitObjective *, WitDetImpOP *);

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      virtual ~WitMultiObjVar ();

      //-----------------------------------------------------------------------
      // Overriding public virtual member functions.
      //-----------------------------------------------------------------------

      virtual double       compObjCoeff       ();
      virtual void         uploadSoln         ();
      virtual const char * lexObjElemName     ();
      virtual void         reportLexUnbounded ();

      //-----------------------------------------------------------------------
      // Access functions.
      //-----------------------------------------------------------------------

      accessFunc (WitObjective *, myObj)

   private:

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

      noCopyCtorAssign (WitMultiObjVar);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitObjective * const myObj_;
         //
         // The Objective with which this MultiObjVar is associated.
   };

//------------------------------------------------------------------------------
// class NtbvVar
// A negative total bound violation variable in the optimization problem:
// a variable whose value is constrained to be negative of the sum of the
// violations of soft lower bounds in the solution.
//
// Class hierarchy:
//
//    OptVC
//       OptVar
//          DetVar
//             NtbvVar
//------------------------------------------------------------------------------

class WitNtbvVar: public WitDetVar
   {
   public:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitNtbvVar (WitDetImpOP *);

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      virtual ~WitNtbvVar ();

      //-----------------------------------------------------------------------
      // Overriding public virtual member functions.
      //-----------------------------------------------------------------------

      virtual double       compObjCoeff       ();
      virtual void         uploadSoln         ();
      virtual const char * lexObjElemName     ();
      virtual void         reportLexUnbounded ();

   private:

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

      noCopyCtorAssign (WitNtbvVar);
   };

//------------------------------------------------------------------------------
// Implementation of inline functions of class SlbvVar.
//------------------------------------------------------------------------------

inline WitBoundSet * WitSlbvVar::myBoundSet ()
   {
   return myBoundedVar_->myBoundSet ();
   }

#endif
