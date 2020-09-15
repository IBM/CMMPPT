//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef BoundPairH
#define BoundPairH

//------------------------------------------------------------------------------
// Header file: "BoundPair.h"
//
// Contains the declaration of class BoundPair.
//------------------------------------------------------------------------------

#include <Util.h>

//------------------------------------------------------------------------------
// class BoundPair
//
// Used for lower and upper bounds.
//
// Implemented in OptMisc.C.
//------------------------------------------------------------------------------

class WitBoundPair
   {
   public:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitBoundPair ();
         //
         // lower_ and upper_ will be set to 0.0.

      WitBoundPair (double lowerInit, double upperInit);
         //
         // lower_ will be set to lowerInit.
         // upper_ will be set to upperInit.

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      ~WitBoundPair ();

      //-----------------------------------------------------------------------
      // Data access functions.
      //-----------------------------------------------------------------------

      accessFunc (double, lower)
      accessFunc (double, upper)

      //-----------------------------------------------------------------------
      // Non-const access functions.
      //-----------------------------------------------------------------------

      inline double & lower ()
         {
         return lower_;
         }

      inline double & upper ()
         {
         return upper_;
         }

      //-----------------------------------------------------------------------
      // Other public member functions.
      //-----------------------------------------------------------------------

      void operator = (const WitBoundPair & theBoundPair);
         //
         // Copies memberwise.

   private:

      //-----------------------------------------------------------------------
      // Private member functions.
      //-----------------------------------------------------------------------

      noCopyCtor (WitBoundPair);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      double lower_;
      double upper_;
   };

#endif
