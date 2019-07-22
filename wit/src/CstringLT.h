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
