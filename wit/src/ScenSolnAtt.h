//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef ScenSolnAttH
#define ScenSolnAttH

//------------------------------------------------------------------------------
// Header file: "ScenSolnAtt.h"
//
// Contains the declaration of the following classes and templates:
//
//    AbsScenSolnAtt
//    ScenSolnAtt <Owner>
//------------------------------------------------------------------------------

#include <StochAssoc.h>

//------------------------------------------------------------------------------
// Class AbsScenSolnAtt
//
// "Abstract Scenario-Specific Solution Attribute"
// Abstract interface to an object that stores and manages data for the
// stochastic version of an solution attribute that exists in deterministic
// mode, but becomes Scenario-specific in stochastic mode.
//
// Class hierarchy:
//
//    ProbAssoc
//       StochAssoc
//          AbsScenSolnAtt
//             ScenSolnAtt <Owner>
//
// Implemented in ScenAtt.C
//------------------------------------------------------------------------------

class WitAbsScenSolnAtt: public WitStochAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitAbsScenSolnAtt ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      virtual void uploadData (WitScenario * theScenario) = 0;
         //
         // Uploads the attribute value to each of the owners from this
         // AbsScenSolnAtt for theScenario.
         // Valid only when the stochastic implosion subsystem is uploading
         // data.

      virtual void clearData () = 0;
         //
         // Frees the storage space used by the vectors of this AbsScenSolnAtt.

   protected:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitAbsScenSolnAtt (WitScenAttMgr *);

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitAbsScenSolnAtt);
   };

//------------------------------------------------------------------------------
// Class Template ScenSolnAtt <Owner>
//
// "Scenario-Specific Solution Attribute"
// Stores and manages data for the stochastic version of an input attribute that
// exists in deterministic mode, but becomes Scenario-specific in stochastic
// mode.
//
// Template argument:
//    Owner: The class whose instances own the deterinistic version of the
//           attribute.
//
// Class hierarchy:
//
//    ProbAssoc
//       StochAssoc
//          AbsScenSolnAtt
//             ScenSolnAtt <Owner>
//
// Implemented in Scenario.C
//------------------------------------------------------------------------------

template <typename Owner>
      class WitScenSolnAtt: private WitAbsScenSolnAtt
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitScenSolnAtt (
         WitScenAttMgr * theScenAttMgr,
         const char *    theAttName,
         void (Owner::*  theStoreFunc) (const WitDblFlexVec &));

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void storeValueFor (
            Owner *       theOwner,
            WitScenario * theScenario,
            WitPeriod     thePer,
            double        theValue);
         //
         // Sets the value of the attribute for theOwner in theScenario in
         // thePer to theValue.
         // Valid only when the StochOptMgr is uploading the stochastic
         // implosion solution.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitScenSolnAtt ();

      //-----------------------------------------------------------------------
      // Overriding private virtual member functions.
      //-----------------------------------------------------------------------

      virtual void uploadData (WitScenario *);
      virtual void clearData  ();

      //-----------------------------------------------------------------------

      noCopyCtorAssign (WitScenSolnAtt);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitScenAtt <Owner> * myScenAtt_;
         //
         // The ScenAtt owned by this ScenSolnAtt.

      void (Owner::* const myStoreFunc_) (const WitDblFlexVec &);
         //
         // The member function of the Owner class that sets the value of the
         // Owner's version of the attribute represented by this ScenSolnAtt to
         // the value of the argument.
         // The function is expected to be valid only when the stochastic
         // implosion subsystem is uploading data.
   };

#endif
