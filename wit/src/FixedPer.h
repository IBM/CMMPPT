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

#ifndef FixedPerH
#define FixedPerH

//------------------------------------------------------------------------------
// Header file: "FixedPer.h"
//
// Contains the declaration and inline implementation of class FixedPer.
//------------------------------------------------------------------------------

#include <Util.h>

//------------------------------------------------------------------------------
// Class FixedPer
//
// Represents an unchanging Period.
//------------------------------------------------------------------------------

class WitFixedPer
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      inline WitFixedPer (WitPeriod thePer):
            myPer_ (thePer)
         {
         }

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      inline ~WitFixedPer ()
         {
         }

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitPeriod, myPer)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitFixedPer);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      const WitPeriod myPer_;
   };

#endif
