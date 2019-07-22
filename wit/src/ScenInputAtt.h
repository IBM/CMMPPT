//==============================================================================
// IBM Confidential
//
// OCO Source Materials
//
// Watson Implosion Technology
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//
// The Source code for this program is not published or otherwise divested of
// its trade secrets, irrespective of what has been deposited with the U. S.
// Copyright office.
//==============================================================================

#ifndef ScenInputAttH
#define ScenInputAttH

//------------------------------------------------------------------------------
// Header file: "ScenInputAtt.h"
//
// Contains the declaration of the following classes and templates:
//
//    AbsScenInputAtt
//    ScenInputAtt <Owner>
//------------------------------------------------------------------------------

#include <StochAssoc.h>

//------------------------------------------------------------------------------
// Class AbsScenInputAtt
//
// "Abstract Scenario-Specific Input Attribute"
// Abstract interface to an object that stores and manages data for the
// stochastic version of an input attribute that exists in deterministic mode,
// but becomes Scenario-specific in stochastic mode.
//
// Class hierarchy:
//
//    ProbAssoc
//       StochAssoc
//          AbsScenInputAtt
//             ScenInputAtt <Owner>
//
// Implemented in ScenAtt.C
//------------------------------------------------------------------------------

class WitAbsScenInputAtt: public WitStochAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitAbsScenInputAtt ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      virtual void prepSet (WitDelComp * theDelComp) = 0;
         //
         // Takes action appropriate just before the value of the attribute is
         // to be set for theDelComp.

      virtual void prepSet (WitBoundSet * theBoundSet) = 0;
         //
         // Takes action appropriate just before the value of the attribute is
         // to be set for theBoundSet.

      virtual void downloadData (WitScenario * theScenario) = 0;
         //
         // Downloads the attribute value from each of the owners into this
         // AbsScenInputAtt for theScenario.

      virtual void uploadData (WitScenario * theScenario) = 0;
         //
         // Uploads the attribute value to each of the owners from this
         // AbsScenInputAtt for theScenario.
         // Valid only when the stochastic implosion subsystem is uploading
         // data.

      virtual void copyDataFrom (WitScenAttMgr * srcScenAttMgr) = 0;
         //
         // Copies into this AbsScenInputAtt the contents of the corresponding
         // AbsScenInputAtt that belongs to srcScenAttMgr.
         // It is assumed that srcScenAttMgr and this AbsScenInputAtt belong to
         // WitRuns whose Component set, # Scenarios, and # Periods match.

      virtual void writeData (
            WitComponent * theComp,
            WitScenario *  theScenario,
            bool           diffedSec,
            bool &         attWritten) = 0;
         //
         // Writes the section of the input data file for this AbsScenInputAtt
         // with respect to theComp and theScenario, if appropriate.
         // On entry, attWritten is true, iff an attribute has already been
         // written for theComp; if this function writes its attribute,
         // attWritten is set to true.
         // diffedSec indicates whether attributes are being written in the
         // differentiated section or the undifferentiated of the file.

      virtual void writeData (WitBoundSet *, WitScenario *, bool, bool &) = 0;
         //
         // Same as writeData (Component *, Scenario *, bool &), but for a
         // BoundSet.

      virtual bool ownedByBoundSet () = 0;
         //
         // Returns true, iff the attribute is owned by a BoundSet and not a
         // Component.

      virtual void validateData () = 0;
         //
         // Verifies that the values of the attribute are valid for a stochastic
         // implosion.

   protected:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitAbsScenInputAtt (WitScenAttMgr *);

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitAbsScenInputAtt);
   };

//------------------------------------------------------------------------------
// Class Template ScenInputAtt <Owner>
//
// "Scenario-Specific Input Attribute"
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
//          AbsScenInputAtt
//             ScenInputAtt <Owner>
//
// Implemented in ScenAtt.C
//------------------------------------------------------------------------------

template <typename Owner>
      class WitScenInputAtt: private WitAbsScenInputAtt
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitScenInputAtt (
         WitScenAttMgr *                     theScenAttMgr,
         const char *                        theAttName,
         double                              theDefaultVal,
         const WitDblFlexVec & (Owner::*     theAccFunc) () const,
         void (Owner::*                      theStoreFunc) (
                                                const WitDblFlexVec &),
         WitScenInputAtt * (WitScenAttMgr::* theInstFunc) () const);

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      const WitDblFlexVec & myValueFor (Owner *, WitScenario *);

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitScenInputAtt ();

      //-----------------------------------------------------------------------
      // Overriding private virtual member functions.
      //-----------------------------------------------------------------------

      virtual void prepSet      (WitDelComp *);
      virtual void prepSet      (WitBoundSet *);
      virtual void downloadData (WitScenario *);
      virtual void uploadData   (WitScenario *);
      virtual void copyDataFrom (WitScenAttMgr *);
      virtual void writeData    (WitComponent *, WitScenario *, bool, bool &);
      virtual void writeData    (WitBoundSet *,  WitScenario *, bool, bool &);

      virtual bool ownedByBoundSet ();
      virtual void validateData    ();

      //------------------------------------------------------------------------
      // Other private member functions.
      //------------------------------------------------------------------------

      void prepSetForOwner (Owner * theOwner);
         //
         // Takes action appropriate just before the value of the attribute is
         // to be set for theOwner.

      static Owner * theOwnerFor (WitComponent * theComp);
         //
         // Returns the Owner corresponding to theComp.
         // Implemented using template specialization.
         // Not allowed if Owner is BoundSet.

      void writeDataForOwner (
            Owner *        theOwner,
            WitScenario *  theScenario,
            bool           diffedSec,
            bool &         attWritten);
         //
         // Writes the section of the input data file for this ScenInputAtt
         // with respect to theOwner and theScenario, if appropriate.
         // On entry, attWritten is true, iff an attribute has already been
         // written for theComp; if this function writes its attribute,
         // attWritten is set to true.
         // diffedSec indicates whether attributes are being written in the
         // differentiated section or the undifferentiated of the file.

      WitNode * stageNode (Owner * theOwner);
         //
         // Returns the Node whose stage determines the stage for theOwner.

      void checkSameInAllScenarios (Owner * theOwner, WitPeriod thePer);
         //
         // Verifies that the value of the attribute for theOwner in thePer is
         // the same in all Scenarios.

      void issueMisMatchOwnerMsg (Owner * theOwner);
         //
         // Issues a msg identifying theOwner for a stage 0 attribute mismatch
         // situation.
         // Implemented using template specialization.

      bool isDiffedAt (Owner * theOwner);
         //
         // Returns true, iff the attribute is considered to be differentiated
         // at theOwner.

      noCopyCtorAssign (WitScenInputAtt);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitScenAtt <Owner> * myScenAtt_;
         //
         // The ScenAtt owned by this ScenInputAtt when the attribute is
         // differentiated; otherwise NULL.
         // The attribute is considered to be differentiated, iff its value
         // for at least one Owner has been set at least once since stochactic
         // mode was entered.

      const char * const myAttName_;
         //
         // The name of the attribute.

      const double myDefaultVal_;
         //
         // The default value of the attribute.

      const WitDblFlexVec & (Owner::* const myAccFunc_) () const;
         //
         // The access function for the attribute.

      void (Owner::* const myStoreFunc_) (const WitDblFlexVec &);
         //
         // The member function of the Owner class that sets the value of the
         // Owner's version of the attribute represented by this ScenInputAtt to
         // the value of the argument.
         // The function is expected to be valid only when the stochastic
         // implosion subsystem is uploading data.

      WitScenInputAtt * (WitScenAttMgr::* const myInstFunc_) () const;
         //
         // The member function of class ScenAttMgr that returns the
         // ScenInputAtt that belongs to the ScenAttMgr and corresponds to this
         // ScenInputAtt.
   };

#endif
