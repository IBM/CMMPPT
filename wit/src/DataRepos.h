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

#ifndef DataReposH
#define DataReposH

//------------------------------------------------------------------------------
// Header file: "DataRepos.h"
//
// Contains the declaration of class DataRepos.
//------------------------------------------------------------------------------

#include <Assoc.h>

template <typename Variable> class WitVarRepos;
template <typename Object>   class WitAttRepos;

//------------------------------------------------------------------------------
// Class DataRepos
//
// Saves the current values of various variables and attributes and restores
// them to the saved values upon request. 
// Designed for use by heuristic allocation.
//
// Implemented in Repos.C.
//------------------------------------------------------------------------------

class WitDataRepos: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitDataRepos (WitProblem *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitDataRepos ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void saveVal (int    & theVar);
      void saveVal (bool   & theVar);
      void saveVal (double & theVar);
         //
         // Saves the current value of theVar.

      template <typename ObjByPtr>
            inline void saveVal (ObjByPtr * & thePtr)
         {
         savePtrVal (reinterpret_cast <void * &> (thePtr));
         }

      void saveExecVol (WitOperation * theOpn, WitPeriod thePer);
         //
         // Saves the execVol for theOpn in thePer.

      void saveSubVol (WitSubEntry * theSub, WitPeriod thePer);
         //
         // Saves the subVol for theSub in thePer.

      void restore ();
         //
         // Restores each variable saved in this DataRepos to the value it had
         // the first time it was saved since the most recent call to restore or
         // clear. Empties the set of saved variables.

      void restoreAndSave (WitDataRepos * theDataRepos);
         //
         // Same as restore, but clears theDataRepos and then saves in it the
         // current values of all the variables that are being restored.

      void clear ();
         //
         // Empties the set of saved variables.

      bool isEmpty ();
         //
         // Returns true, iff the set of saved variables is empty.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      void savePtrVal (void * & theVoidPtr);
         //
         // Saves the current value of theVoidPtr.

      noCopyCtorAssign (WitDataRepos);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitVarRepos <int>    * myIntRepos_;
      WitVarRepos <bool>   * myBoolRepos_;
      WitVarRepos <double> * myDblRepos_;
      WitVarRepos <void *> * myPtrRepos_;
         //
         // The VarReposes owned by this DataRepos.

      WitAttRepos <WitOperation> * execVolRepos_;
      WitAttRepos <WitSubEntry> *  subVolRepos_;
         //
         // The AttReposes owned by this DataRepos.
   };

#endif
