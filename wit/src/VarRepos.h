//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef VarReposH
#define VarReposH

//------------------------------------------------------------------------------
// Header file: "VarRepos.h"
//
// Contains the declaration of class template VarRepos <Variable>.
//------------------------------------------------------------------------------

#include <PairStack.h>

//------------------------------------------------------------------------------
// Class Template VarRepos <Variable>
//
// Saves the current values of variables of type Variable and restores them to
// the saved values upon request. 
//
// Implemented in Repos.C.
//------------------------------------------------------------------------------

template <typename Variable>
      class WitVarRepos: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitVarRepos (WitProblem *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitVarRepos ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void saveVal (Variable & theVar);
         //
         // Saves the current value of theVar.

      void restore ();
         //
         // Restores each variable saved in this TypeRepos to the value it had
         // the first time it was saved since the most recent call to restore or
         // clear. Empties the set of saved variables.

      void restoreAndSave (WitVarRepos * theVarRepos);
         //
         // Same as restore, but clears theVarRepos and then saves in it the
         // current values of all the variables that are being restored.

      void clear ();
         //
         // Empties the set of saved variables.

      inline bool isEmpty ()
         {
         return savedPairs_.isEmpty ();
         }

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitVarRepos);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitPairStack <Variable, Variable> savedPairs_;
         //
         // The set of variables and saved values currently stored by this 
         // VarRepos.
   };

#endif
