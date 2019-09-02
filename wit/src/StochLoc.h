//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef StochLocH
#define StochLocH

//------------------------------------------------------------------------------
// Header file: "StochLoc.h"
//
// Contains the declaration of class StochLoc.
//------------------------------------------------------------------------------

#include <ScenGroup.h>
#include <PtrMap.h>

//------------------------------------------------------------------------------
// Class StochLoc
//
// "Stochastic Location"
// Represents a pair consisting of a ScenGroup and a Period.
// Instances of this class are used to locate a StochVar or StochCon in the
// StochImpOP.
//
// Class hierarchy:
//
//    ProbAssoc
//       StochAssoc
//          StochLoc
//
// Implemented in StochOpt.C.
//------------------------------------------------------------------------------

class WitStochLoc: public WitStochAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitStochLoc (WitScenGroup *, WitPeriod);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitStochLoc ();

      //------------------------------------------------------------------------
      // Other public member functions
      //------------------------------------------------------------------------

      void generateVariables ();
         //
         // Generates the StochVars associated with this StochLoc.

      void generateConstraints ();
         //
         // Generates the StochCons associated with this StochLoc.

      void clearStochVars ();
         //
         // Releases memory for PtrMaps to StochVars.

      void print ();
         //
         // Prints this StochLoc to the OptProblem file.

      //------------------------------------------------------------------------
      // StochVar look-up functions.
      //------------------------------------------------------------------------

      WitStScrapVar   * myScrapVarFor   (WitPart      *);
      WitStStockVar   * myStockVarFor   (WitMaterial  *);
      WitStExecVar    * myExecVarFor    (WitOperation *);
      WitStShipVar    * myShipVarFor    (WitDemand    *);
      WitStCumShipVar * myCumShipVarFor (WitDemand    *);
      WitStNonSubVar  * myNonSubVarFor  (WitBomEntry  *);
      WitStSubVar     * mySubVarFor     (WitSubEntry  *);

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc    (WitPeriod,       myPer)
      accessFunc    (WitScenGroup *,  myScenGroup)

      accessWrapper (WitStochImpOP *, myStochImpOP, myScenGroup_)
      accessWrapper (WitStage *,      myStage,      myScenGroup_)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      void genScrapVars   ();
      void genStockVars   ();
      void genShipVars    ();
      void genCumShipVars ();
      void genExecVars    ();
      void genNonSubVars  ();
      void genSubVars     ();
         //
         // Each of these functions generates the StochVars associated with this
         // StochLoc for a specific class derived from StochVar.

      void genResCons  ();
      void genShipCons ();
      void genSubCons  ();
         //
         // Each of these functions generates the StochCons associated with this
         // StochLoc for a specific class derived from StochCon.

      noCopyCtorAssign (WitStochLoc);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitScenGroup * const myScenGroup_;
         //
         // The ScenGroup that owns this StochLoc.

      const WitPeriod myPer_;
         //
         // The Period for this StochLoc.

      //-----------------------------------------------------------------------
      // PtrMaps to StochVars
      //
      // E.g.,myScrapVarFor (thePart) is the StScrapVar for myScenGroup_,
      // myPer_, and thePart, if any; otherwise NULL.
      //-----------------------------------------------------------------------

      WitPtrMap <WitPart,      WitStScrapVar>   myScrapVarFor_;
      WitPtrMap <WitMaterial,  WitStStockVar>   myStockVarFor_;
      WitPtrMap <WitOperation, WitStExecVar>    myExecVarFor_;
      WitPtrMap <WitDemand,    WitStShipVar>    myShipVarFor_;
      WitPtrMap <WitDemand,    WitStCumShipVar> myCumShipVarFor_;
      WitPtrMap <WitBomEntry,  WitStNonSubVar>  myNonSubVarFor_;
      WitPtrMap <WitSubEntry,  WitStSubVar>     mySubVarFor_;
   };

#endif
