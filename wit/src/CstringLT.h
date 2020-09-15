//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef CstringLTH
#define CstringLTH

//------------------------------------------------------------------------------
// Header file: "CstringLT.h"
//
// Contains the declaration of class CstringLT.
//------------------------------------------------------------------------------

#include <Util.h>

//------------------------------------------------------------------------------
// Class CstringLT
//
// Comparison functor to be used for sorting c-strings.
//
// All implementation is inline.
//------------------------------------------------------------------------------

class WitCstringLT
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      inline WitCstringLT ()
         {
         }

      inline WitCstringLT (const WitCstringLT &)
         {
         }

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      inline ~WitCstringLT ()
         {
         }

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      inline bool operator () (
            const char * theCstring1,
            const char * theCstring2)
            const
         {
         return (strcmp (theCstring1, theCstring2) < 0);
         }
   };

#endif
