//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef AttReposH
#define AttReposH

//------------------------------------------------------------------------------
// Header file: "AttRepos.h"
//
// Contains the declaration of class template AttRepos <Object>.
//------------------------------------------------------------------------------

#include <TripStack.h>

//------------------------------------------------------------------------------
// Class Template AttRepos <Object>
//
// Save the current values of an attribute of type DblFlexVec owned by an object
// of type Object and restores them to the saved values upon request.
// The attribute values are retrieved and stored using member function pointers
// of class Object.
//
// Implemented in Repos.C.
//------------------------------------------------------------------------------

template <typename Object>
      class WitAttRepos: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitAttRepos (
         WitProblem *                     theProblem,
         const WitDblFlexVec & (Object::* theAccFunc) () const,
         void                  (Object::* theStoreFunc) (WitPeriod, double));

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitAttRepos ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void saveValAt (Object * theObject, WitPeriod thePer);
         //
         // Saves the value of the attribute for theObject in thePer.

      void restore ();
         //
         // Restores each attribute value saved in this AttRepos to the value it
         // had the first time it was saved since the most recent call to
         // restore or clear. Empties the set of saved attributes.

      void restoreAndSave (WitAttRepos * theAttRepos);
         //
         // Same as restore, but clears theAttRepos and then saves in it the
         // current values of all the attributes that are being restored.

      void clear ();
         //
         // Empties the set of saved attribute values.

      inline bool isEmpty ()
         {
         return savedTrips_.isEmpty ();
         }

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitAttRepos);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitTripStack <Object> savedTrips_;
         //
         // The set of saved values and corresponding Objects and Periods
         // currently stored by this AttRepos.

      const WitDblFlexVec & (Object::* const myAccFunc_) () const;
         //
         // The access function for the attribute.

      void (Object::* const myStoreFunc_) (WitPeriod, double);
         //
         // The member function of the Object class that sets the value of the
         // attribute in the indicated period to the indicated value.
   };

#endif
