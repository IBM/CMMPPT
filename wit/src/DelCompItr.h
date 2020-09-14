//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef DelCompItrH
#define DelCompItrH

//------------------------------------------------------------------------------
// Header file: "DelCompItr.h"
//
// Contains the declaration of class DelCompItr.
//------------------------------------------------------------------------------

#include <ObjStack.h>
#include <Assoc.h>

//------------------------------------------------------------------------------
// Class DelCompItr
//
// Responsible for iterating thru the DelComps of a Problem at the request of
// the application program.
//------------------------------------------------------------------------------

class WitDelCompItr: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitDelCompItr (WitProblem *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitDelCompItr ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void reset ();
         //
         // Resets DelComp iteration.

      void advance ();
         //
         // * If this DelCompItr is currently in its reset state, it is advanced
         //   to the the first DelComp.
         // * If this DelCompItr is currently at any but the last DelComp, it is
         //   advanced to the next DelComp.
         // * If this DelCompItr is currently at any the last DelComp, it is
         //   reset.

      WitAttr apiObjItrState ();
         //
         // Returns the API objItrState attribute value corresponding to the
         // current state of this DelCompItr.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessNonNull (WitDelComp *, curDelComp)

      inline bool active ()
         {
         return (curDelComp_ != NULL);
         }

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      void setup ();
         //
         // Sets up DelComp iteration.

      noCopyCtorAssign (WitDelCompItr);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitObjStack <WitDelComp> myDelCompStack_;
         //
         // In active mode, myDelCompStack_ is the Stack of DelComps being
         // iterated.
         // In reset mode, myDelCompStack_ is empty.

      WitObjStItr <WitDelComp> myStItr_;
         //
         // Itr for myDelCompStack_.

      WitDelComp * curDelComp_;
         //
         // The DelComp currently referenced by this DelCompItr, or NULL, if
         // this DelCompItr is in its reset state.
   };


#endif
