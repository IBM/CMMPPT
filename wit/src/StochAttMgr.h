//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef StochAttMgrH
#define StochAttMgrH

//------------------------------------------------------------------------------
// Header file: "StochAttMgr.h"
//
// Contains the declaration of class StochAttMgr.
//------------------------------------------------------------------------------

#include <CstringLT.h>
#include <StochAssoc.h>

#include <map>

//------------------------------------------------------------------------------

typedef std::map <const char *, WitStochAtt *, WitCstringLT> WitStochAttMap;

//------------------------------------------------------------------------------
// Class StochAttMgr
//
// "Stochastic Attribute Manager"
// Owns and provides access to the StochAtts of the stochastic implosion
// subsystem.
//
// Class hierarchy:
//
//    ProbAssoc
//       StochAssoc
//          StochAttMgr
//------------------------------------------------------------------------------

class WitStochAttMgr: public WitStochAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitStochAttMgr (WitStochModeMgr *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitStochAttMgr ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void makeScenInputStochAtt (
            const char *         theName,
            WitAbsScenInputAtt * theAbsScenInputAtt);
         //
         // Builds a StochAtt whose name is given by theName and that
         // corresponds to a theScenInputAtt.

      void makeScenSolnStochAtt (const char * theName);
         //
         // Builds a StochAtt corresponding to a ScenSolnAtt whose name is
         // given by theName.

      bool valueOf (
            bool (WitStochAtt::* theAccFunc) () const, 
            const char *         theAttName);
         //
         // Returns the value of theAccFunc for the StochAtt whose name is given
         // by theName.

      void prepSetStochModeAttr (const char * theAttName);
         //
         // Takes action appropriate just before the value of the attribute
         // whose name is given by theName is to be set.
         // The attribute is allowed to be set in stochastic mode.
         // The attribute must not be a scenario-specific attribute associated
         // with either a DelComp or a BoundSet.

      void prepSetScenSpecAttr (
            const char * theAttName,
            WitDelComp * theDelComp);
         //
         // Takes action appropriate just before the value of the
         // scenario-specific attribute whose name is given by theName is to be
         // set for theDelComp.

      void prepSetBoundSet (WitBoundSet * theBoundSet);
         //
         // Takes action appropriate just before the value of theBoundSet is to
         // be set.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      void prepSetBoundSetAttr (
            const char *  theAttName,
            WitBoundSet * theBoundSet);
         //
         // Takes action appropriate just before the value of the attribute
         // whose name is given by theName is to be set for theBoundSet.
         // The attribute is allowed to be set in stochastic mode.

      void makeStochAtts ();
         //
         // Creates and stores all the StochAtts owned by this StochAttMgr.

      void makeStochAtt (const char * theName);
         //
         // Creates and stores a new StochAtt whose name is given by theName.

      void makeStochAtt (
            const char *         theName,
            WitAbsScenInputAtt * theAbsScenInputAtt);
         //
         // Creates and stores a new StochAtt whose name is given by theName
         // and that corresponds to a theScenInputAtt, if non-NULL.

      WitStochAtt * findStochAtt (const char * theName);
         //
         // Returns the StochAtt owned by this StochAttMgr whose name matches
         // theName, if any; otherwise returns NULL.

      noCopyCtorAssign (WitStochAttMgr);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitStochAttMap myStochAttMap_;
         //
         // Maps the name of each stochastic attribute to the corresponding
         // StochAtt owned by this StochAttMgr.

      WitStochAtt * cachedStochAtt_;
         //
         // The StochAtt most recently looked up by findStochAtt (), if any;
         // otherwise NULL.
   };

#endif
