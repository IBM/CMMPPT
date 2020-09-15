//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef StochAttH
#define StochAttH

//------------------------------------------------------------------------------
// Header file: "StochAtt.h"
//
// Contains the declaration of class StochAtt.
//------------------------------------------------------------------------------

#include <Str.h>

//------------------------------------------------------------------------------
// Class StochAtt
//
// "Stochastic Attribute"
// A StochAtt represents a stochastic attribute: any attribute that is to be
// given special treatment in stochastic mode. The data of a StochAtt consists
// primarily of "meta-attributes", which specify which kinds of special
// treatment apply to the represented attribute.
//------------------------------------------------------------------------------

class WitStochAtt
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitStochAtt (const char *, WitAbsScenInputAtt *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitStochAtt ();

      //------------------------------------------------------------------------
      // Meta-attribute setting functions.
      //------------------------------------------------------------------------

      void setIsScenSpec        (bool);
      void setCanSetInStochMode (bool);
      void setCanSetInSolnMode  (bool);

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (const WitString &,    myName)
      accessFunc (bool,                 isScenSpec)
      accessFunc (bool,                 canSetInStochMode)
      accessFunc (bool,                 canSetInSolnMode)
      accessFunc (WitAbsScenInputAtt *, myAbsScenInputAtt)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitStochAtt);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitString myName_;

      //------------------------------------------------------------------------
      // Meta-Attributes.
      //------------------------------------------------------------------------

      bool isScenSpec_;
         //
         // True, iff the attribute is scenario-specific.

      bool canSetInStochMode_;
         //
         // True, iff the attribute is an attribute that can be set in
         // stochastic mode.

      bool canSetInSolnMode_;
         //
         // True, iff the attribute is an attribute that can be set in
         // stochastic solution mode.

      //------------------------------------------------------------------------
      // Other private member data.
      //------------------------------------------------------------------------

      WitAbsScenInputAtt * const myAbsScenInputAtt_;
         //
         // The AbsScenInputAtt corresponding to this StochAtt, if any;
         // otherwise NULL.
   };

#endif
