//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef ScenAttH
#define ScenAttH

//------------------------------------------------------------------------------
// Header file: "ScenAtt.h"
//
// Contains the declaration of class template ScenAtt <Owner>.
//------------------------------------------------------------------------------

#include <StochAssoc.h>
#include <PtrMap.h>

//------------------------------------------------------------------------------
// Class Template ScenAtt <Owner>
//
// "Scenario-Specific Attribute"
// Stores and manages data for the stochastic version of an attribute that
// exists in deterministic mode, but becomes Scenario-specific in stochastic
// mode.
//
// Template argument:
//    Owner: The class whose instances own the deterministic version of the
//           attribute.
//
// Class hierarchy:
//
//    ProbAssoc
//       StochAssoc
//          ScenAtt <Owner>
//------------------------------------------------------------------------------

template <typename Owner>
      class WitScenAtt: public WitStochAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitScenAtt (WitScenAttMgr * theScenAttMgr);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitScenAtt ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      const WitList <Owner> & allOwners ();
         //
         // Returns the List of all Owner instances.
         // Implemented using template specialization.

      void allocateFor (Owner * theOwner);
         //
         // Allocates storage for this ScenAtt for theOwner in all Scenarios.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      inline WitDblFlexVec & myValueFor (
            Owner *       theOwner,
            WitScenario * theScenario)
         {
         return myValue_.myElemAt (theOwner).myElemAt (theScenario);
         }

      inline bool isAllocatedFor (Owner * theOwner)
         {
         return (myValue_ (theOwner) != NULL);
         }

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitScenAtt);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitScenAttMgr * const myScenAttMgr_;
         //
         // The ScenAttMgr for this ScenAtt.

      WitPtrMap <Owner, WitPtrMap <WitScenario, WitDblFlexVec> > myValue_;
         //
         // myValue_.myElemAt (theOwner).myElemAt (theScenario) is the value of
         // the attribute stored by this ScenAtt for theOwner in theScenario.
   };

#endif
