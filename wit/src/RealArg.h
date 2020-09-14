//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef RealArgH
#define RealArgH

//------------------------------------------------------------------------------
// Header file: "RealArg.h"
//
// Contains the declaration of class RealArg.
//------------------------------------------------------------------------------

#include <Util.h>

//------------------------------------------------------------------------------
// Class RealArg
//
// A base class for for classes whose instances are arguments to API functions
// where the underlying type of the argument is a type derived either from float
// or from double.
//
// Class Hierarchy:
//
// RealArg
//    ISRealArg
//    OSRealArg
//    IVRealArg
//    OVRealArg
//    OTDARealArg
//------------------------------------------------------------------------------

class WitRealArg
   {
   protected:

      //------------------------------------------------------------------------
      // Protected member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitRealArg (bool);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitRealArg ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (bool, floatCase)

      inline bool doubleCase () const
         {
         return (not floatCase_);
         }

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitRealArg);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      const bool floatCase_;
         //
         // If true,  the underlying argument is of a type based on float.
         // If false, the underlying argument is of a type based on double.
   };

#endif
