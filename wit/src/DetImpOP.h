//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef DetImpOPH
#define DetImpOPH

//------------------------------------------------------------------------------
// Header file: "DetImpOP.h"
//
// Contains the declaration of class DetImpOP.
//------------------------------------------------------------------------------

#include <OptProblem.h>
#include <PtrSched.h>
#include <PtrMap.h>

//------------------------------------------------------------------------------
// class DetImpOP
//
// "Deterministic Implosion Optimization Problem"
//
// Class hierarchy:
//
//    ProbAssoc
//       OptProblem
//          DetImpOP
//------------------------------------------------------------------------------

class WitDetImpOP: public WitOptProblem
   {
   public:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitDetImpOP (WitProblem *);

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      virtual ~WitDetImpOP ();

      //-----------------------------------------------------------------------
      // Overriding public virtual member functions.
      //-----------------------------------------------------------------------

      virtual bool needDual ();

      //-----------------------------------------------------------------------
      // Access functions.
      //-----------------------------------------------------------------------

      accessFunc    (const WitList <WitBoundedVar> &,  myBoundedVars)
      accessFunc    (WitNtbvVar *,                     myNtbvVar)
      accessFunc    (FILE *,                           solnFile)
      accessNonNull (WitExecPerSched <WitBillEntry> *, myExecPerSched)

      //-----------------------------------------------------------------------
      // Derived OptVC storing functions.
      // Each of these functions store its argument (a newly constructed
      // instance of a derived class of OptVC) in the approriate place in
      // this DetImpOP.
      //-----------------------------------------------------------------------

      void storeDerived (WitScrapVar *);
      void storeDerived (WitNonSubVar *);
      void storeDerived (WitSubVar *);
      void storeDerived (WitShipVar *);
      void storeDerived (WitSlbvVar *);
      void storeDerived (WitExecVar *);
      void storeDerived (WitStockVar *);
      void storeDerived (WitCumShipVar *);
      void storeDerived (WitMultiObjVar *);
      void storeDerived (WitNtbvVar *);
      void storeDerived (WitResourceCon *);
      void storeDerived (WitSubCon *);
      void storeDerived (WitShipCon *);
      void storeDerived (WitSlbCon *);
      void storeDerived (WitMultiObjCon *);
      void storeDerived (WitTbvCon *);

      //-----------------------------------------------------------------------
      // OptVC look-up functions.
      //
      // Each of these functions returns the OptVC in this DetImpOP
      // corresponding to its arguments, if one exists; otherwise NULL.
      //-----------------------------------------------------------------------

      WitScrapVar *    myScrapVar    (WitPart *,      WitPeriod);
      WitNonSubVar *   myNonSubVar   (WitBomEntry *,  WitPeriod);
      WitSubVar *      mySubVar      (WitSubEntry *,  WitPeriod);
      WitShipVar *     myShipVar     (WitDemand *,    WitPeriod);
      WitExecVar *     myExecVar     (WitOperation *, WitPeriod);
      WitStockVar *    myStockVar    (WitMaterial *,  WitPeriod);
      WitCumShipVar *  myCumShipVar  (WitDemand *,    WitPeriod);

      WitResourceCon * myResourceCon (WitPart *,      WitPeriod);
      WitSubCon *      mySubCon      (WitBomEntry *,  WitPeriod);
      WitShipCon *     myShipCon     (WitDemand *,    WitPeriod);

      WitSlbvVar *     mySlbvVar     (WitBoundedVar *);
      WitSlbCon *      mySlbCon      (WitBoundedVar *);

      WitMultiObjVar * myMultiObjVar (WitObjective *);

      //-----------------------------------------------------------------------
      // OptVC item printing functions.
      //
      // Each of these functions prints its argument to optProbFile ().
      //-----------------------------------------------------------------------

      void printItem (WitNode *);
      void printItem (WitBomEntry *);
      void printItem (WitSubEntry *);
      void printItem (WitDemand *);
      void printItem (WitBoundedVar *);
      void printItem (WitObjective *);
      void printItem ();

      //-----------------------------------------------------------------------
      // Other public member functions.
      //-----------------------------------------------------------------------

      void storeBoundedVar (WitBoundedVar * theBoundedVar);
         //
         // Stores theBoundedVar.

      bool slbvVarsExist ();
         //
         // Returns true iff there are any slbvVars in this DetImpOP.

      void printPeriod (WitPeriod thePer);
         //
         // Prints thePer to optProbFile ().

      void printNonPeriod ();
         //
         // Prints blank space to optProbFile () in place of a Period.

      void writeSoln (const char * fName);
         //
         // Prints out the primal solution from WIT data structures.
         // Writes to a file whose name is given by fName.

   private:

      //-----------------------------------------------------------------------
      // Private member functions.
      //-----------------------------------------------------------------------

      virtual void generateVariables    ();
      virtual void generateConstraints  ();
      virtual void generateLexOptVarSeq (WitPtrVec <WitOptVar> &);
      virtual void postGenFixed         ();
      virtual void postGenFlexible      ();
      virtual void printOptVCHeading    ();
      virtual void displayDerived       ();
      virtual void printDerived         ();
      virtual void initDepVars          ();
      virtual void doScreening          ();
         //
         // Overrides from class OptProblem

      //-----------------------------------------------------------------------
      // Private member functions relating to output of the solution to the
      // optimization problem.
      //-----------------------------------------------------------------------

      void writePeSoln (WitOptVC * theOptVC);
         //
         // Writes the solution value of theOptVC, or blank if it is NULL,
         // or the value is nearly zero.
         // If the OptVC is a variable, the primal solution value is written;
         // if it is a constraint, the dual value is written.

      void writePartSoln ();
         //
         // Prints out the aspects of the primal solution associated with
         // parts. Optionally prints the dual solution for material balance
         // constraints.

      void writeOperationSoln ();
         //
         // Prints out the aspects of the primal solution associated with
         // Operations.

      void writeSubEntrySoln ();
         //
         // Prints out the aspects of the primal solution associated with
         // SubEntries.

      void writeDemandSoln ();
         //
         // Prints out the aspects of the primal solution associated with
         // demands.

      void writeBoundedVarSoln ();
         //
         // Prints out the aspects of the primal solution associated with
         // BoundedVars.

      //-----------------------------------------------------------------------
      // Other private member functions.
      //-----------------------------------------------------------------------

      noCopyCtorAssign (WitDetImpOP);

         // The following are formats used in printing an OptVC.
         //
      inline static const char * itemFormat     ();
      inline static const char * itemLongFormat ();
      inline static const char * periodDFormat  ();
      inline static const char * periodSFormat  ();

      template <typename Item, typename DerOptVC>
            void insertInto (
               WitPtrSched <Item, DerOptVC> & thePtrSched,
               const Item *                   theItem,
               DerOptVC *                     theDerOptVC);
         //
         // Inserts theDerOptVC into thePtrSched at theItem and
         // theDerOptVC->myPeriod (), allocating as necessary.

      template <typename Item, typename DerOptVC>
            DerOptVC * lookUp (
               WitPtrSched <Item, DerOptVC> & thePtrSched,
               const Item *                   theItem,
               WitPeriod                      thePer);
         //
         // Returns the DerOptVC in thePtrSched for theItem in thePer,
         // if any, else NULL.

      void initSlbvVars ();
         //
         // Initializes the SlbvVars.

      void initMultiObjVars ();
         //
         // Initializes the MultiObjVars.

      void initNtbvVar ();
         //
         // Initializes myNtbvVar_.

      void screenImpCons ();
         //
         // Performs generic screening of the constraints of this DetImpOP.
         // Constraints are considered in an order that facilitates
         // cascading of screening, i.e., we try to screen in an order such
         // that if screening constraint A permits constraint B to be screened,
         // then A is screened before B.

      void relaxCumShipNonNeg ();
         //
         // Relaxes redundant nonnegativity bounds on cumulative shipment
         // variables.

      void screenSlbOptVCs ();
         //
         // Screens out the variables and constraints for soft lower bounds
         // that are not soft.

      bool positiveHardLBsExist ();
         //
         // Returns true iff myProblem  () has at least one BoundSet with a
         // positive hardLB.

      bool disallowedScrapExists ();
         //
         // Returns true iff myProblem  () has at least one Part with
         // scrapAllowed == false.

      void printBoundedVars ();
         //
         // Prints information about the BoundedVars of this DetImpOP to
         // optProbFile ().

      bool             multiObjMode  ();
      WitMultiObjMgr * myMultiObjMgr ();
         //
         // Pass-thru to myOptComp ().

      //-----------------------------------------------------------------------
      // Static constant private member data.
      //-----------------------------------------------------------------------

      static const double solnEpsilon;
         //
         // Values must exceed this amount in order to appear in the solution
         // output.

      //-----------------------------------------------------------------------
      // The OptVC Schedules for this DetImpOP.
      //-----------------------------------------------------------------------

      WitPtrSched <WitPart,      WitScrapVar   > myScrapVar_;
      WitPtrSched <WitBomEntry,  WitNonSubVar  > myNonSubVar_;
      WitPtrSched <WitSubEntry,  WitSubVar     > mySubVar_;
      WitPtrSched <WitDemand,    WitShipVar    > myShipVar_;
      WitPtrSched <WitBoundSet,  WitSlbvVar    > mySlbvVar_;
      WitPtrSched <WitOperation, WitExecVar    > myExecVar_;
      WitPtrSched <WitMaterial,  WitStockVar   > myStockVar_;
      WitPtrSched <WitDemand,    WitCumShipVar > myCumShipVar_;

      WitPtrSched <WitPart,      WitResourceCon> myResourceCon_;
      WitPtrSched <WitBomEntry,  WitSubCon     > mySubCon_;
      WitPtrSched <WitDemand,    WitShipCon    > myShipCon_;
      WitPtrSched <WitBoundSet,  WitSlbCon     > mySlbCon_;

      //-----------------------------------------------------------------------
      // Other private member data.
      //-----------------------------------------------------------------------

      WitPtrMap <WitObjective, WitMultiObjVar> myMultiObjVar_;
         //
         // In multiple objectives mode:
         //    myMultiObjVar_ (theObj) is the MultiObjVar owned by this DetImpOP
         //    for theObj.
         // In single objective mode:
         //    myMultiObjVar_ is empty.

      WitPtrMap <WitObjective, WitMultiObjCon> myMultiObjCon_;
         //
         // Same as myMultiObjVar_, but for MultiObjCons.

      WitList <WitBoundedVar> myBoundedVars_;
         //
         // The list of all BoundedVars in this DetImpOP.

      WitNtbvVar * myNtbvVar_;
      WitTbvCon  * myTbvCon_;
         //
         // In multiple objectives mode,
         //    If there are soft lower bounds:
         //       myNtbvVar_ is the NtbvVar owned by this DetImpOP.
         //       myTbvCon_  is the TbvCon  owned by this DetImpOP.
         // Otherwise:
         //    myNtbvVar_ is NULL.
         //    myTbvCon_  is NULL.

      int nSlbvVars_;
         //
         // # soft lower bound violation variables.

      FILE * solnFile_;
         //
         // Solution output file.

      WitExecPerSched <WitBillEntry> * myExecPerSched_;
         //
         // The ExecPerSched owned by this DetImpOP, if any; otherwise NULL.
   };

#endif
