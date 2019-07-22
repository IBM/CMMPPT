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
