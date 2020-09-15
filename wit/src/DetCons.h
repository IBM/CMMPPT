//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef DetConsH
#define DetConsH

//------------------------------------------------------------------------------
// Header file: "DetCons.h"
//
// Contains the declaration the following classes:
//
//    DetCon
//    ResourceCon
//    SubCon
//    ShipCon
//    SlbCon
//    MultiObjCon
//    TbvCon
//------------------------------------------------------------------------------

#include <OptCon.h>

//------------------------------------------------------------------------------
// class DetCon
//
// "Deterministic Constraint"
// An OptCon in a deterministic implosion optimization problem.
//
// Class hierarchy:
//
//    OptVC
//       OptCon
//          DetCon
//             ResourceCon
//             SubCon
//             ShipCon
//             SlbCon
//             MultiObjCon
//             TbvCon
//------------------------------------------------------------------------------

class WitDetCon: public WitOptCon
   {
   public:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitDetCon (WitDetImpOP *, WitPeriod);
      WitDetCon (WitDetImpOP *);

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      virtual ~WitDetCon ();

      //-----------------------------------------------------------------------
      // Overriding public virtual member functions.
      //-----------------------------------------------------------------------

      virtual WitOptProblem * myOptProblem ();

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

      noCopyCtorAssign (WitDetCon);

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
         // data from which this DetCon was generated.

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitDetImpOP * const myDetImpOP_;
         //
         // The DetImpOP that owns this DetCon.

      const WitPeriod myPeriod_;
         //
         // The Period for this DetCon, if it has one.
         // -1, if it doesn't have a Period.
   };

//------------------------------------------------------------------------------
// class ResourceCon
// A resource allocation constraint in the optimization problem.
//
// Class hierarchy:
//
//    OptVC
//       OptCon
//          DetCon
//             ResourceCon
//------------------------------------------------------------------------------

class WitResourceCon: public WitDetCon
   {
   public:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitResourceCon (WitPart *, WitPeriod, WitDetImpOP *);

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      virtual ~WitResourceCon ();

      //-----------------------------------------------------------------------
      // Overriding public virtual member functions.
      //-----------------------------------------------------------------------

      virtual void generateCoeffs ();

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

      void genBillEntCoeffs ();
         //
         // Generates the coefficients for this ResourceCon that are based on
         // BillEntries.

      void getExecPerRange (
            WitBillEntry * theBillEnt,
            WitPeriod &    execPerF,
            WitPeriod &    execPerL);
         //
         // Sets execPerF and execPerL to be the first and last execution
         // periods, execPer, for which
         // theBillEnt->impactPeriod ()[execPer] == myPeriod ().

      noCopyCtorAssign (WitResourceCon);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitPart * const myPart_;
         //
         // The Part with which this ResourceCon is associated.
   };

//------------------------------------------------------------------------------
// class SubCon
// A substitution constraint in the optimization problem.
//
// Class hierarchy:
//
//    OptVC
//       OptCon
//          DetCon
//             SubCon
//------------------------------------------------------------------------------

class WitSubCon: public WitDetCon
   {
   public:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitSubCon (WitBomEntry *, WitPeriod, WitDetImpOP *);

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      virtual ~WitSubCon ();

      //-----------------------------------------------------------------------
      // Overriding public virtual member functions.
      //-----------------------------------------------------------------------

      virtual void generateCoeffs ();

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

      noCopyCtorAssign (WitSubCon);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitBomEntry * const myBomEnt_;
         //
         // The BomEntry with which this SubCon is associated.
   };

//------------------------------------------------------------------------------
// class ShipCon
// A shipment constraint in the optimization problem.
//
// Class hierarchy:
//
//    OptVC
//       OptCon
//          DetCon
//             ShipCon
//------------------------------------------------------------------------------

class WitShipCon: public WitDetCon
   {
   public:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitShipCon (WitDemand *, WitPeriod, WitDetImpOP *);

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      virtual ~WitShipCon ();

      //-----------------------------------------------------------------------
      // Overriding public virtual member functions.
      //-----------------------------------------------------------------------

      virtual void generateCoeffs ();

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

      noCopyCtorAssign (WitShipCon);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitDemand * const myDemand_;
         //
         // The Demand with which this ShipCon is associated.
   };

//------------------------------------------------------------------------------
// class SlbCon
// A soft lower bound constraint in the optimization problem.
//
// Class hierarchy:
//
//    OptVC
//       OptCon
//          DetCon
//             SlbCon
//------------------------------------------------------------------------------

class WitSlbCon: public WitDetCon
   {
   public:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitSlbCon (WitBoundedVar *, WitDetImpOP *);

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      virtual ~WitSlbCon ();

      //-----------------------------------------------------------------------
      // Overriding public virtual member functions.
      //-----------------------------------------------------------------------

      virtual void generateCoeffs ();

      //-----------------------------------------------------------------------
      // Other public member functions.
      //-----------------------------------------------------------------------

      WitBoundSet * myBoundSet ();

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

      noCopyCtorAssign (WitSlbCon);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitBoundedVar * const myBoundedVar_;
         //
         // The BoundedVar to which this SlbCon applies.
   };

//------------------------------------------------------------------------------
// class MultiObjCon
// A multiple objectives constraint in the optimization problem.
//
// Class hierarchy:
//
//    OptVC
//       OptCon
//          DetCon
//             MultiObjCon
//------------------------------------------------------------------------------

class WitMultiObjCon: public WitDetCon
   {
   public:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitMultiObjCon (WitObjective *, WitDetImpOP *);

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      virtual ~WitMultiObjCon ();

      //-----------------------------------------------------------------------
      // Overriding public virtual member functions.
      //-----------------------------------------------------------------------

      virtual void generateCoeffs ();

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

      noCopyCtorAssign (WitMultiObjCon);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitObjective * const myObj_;
         //
         // The Objective with which this MultiObjCon is associated.
   };

//------------------------------------------------------------------------------
// class TbvCon
// A total bound violation constraint in the optimization problem.
//
// Class hierarchy:
//
//    OptVC
//       OptCon
//          DetCon
//             TbvCon
//------------------------------------------------------------------------------

class WitTbvCon: public WitDetCon
   {
   public:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitTbvCon (WitDetImpOP *);

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      virtual ~WitTbvCon ();

      //-----------------------------------------------------------------------
      // Overriding public virtual member functions.
      //-----------------------------------------------------------------------

      virtual void generateCoeffs ();

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

      noCopyCtorAssign (WitTbvCon);
   };

#endif
