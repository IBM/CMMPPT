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

#ifndef ReqPtH
#define ReqPtH

//------------------------------------------------------------------------------
// Header file: "ReqPt.h"
//
// Contains the declaration of class ReqPt.
//------------------------------------------------------------------------------

#include <ObjStack.h>

//------------------------------------------------------------------------------
// Class ReqPt
//
// "Requirement Point"
// Represents a Part and period at which requirements may be placed.
//
// Implemented in Selection.C.
//------------------------------------------------------------------------------

class WitReqPt
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitReqPt (WitProblem *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitReqPt ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void set (WitPart *, WitPeriod);
         //
         // Sets myPart_ and myPer_.

      bool hasAlt ();
         //
         // Returns true, iff this ReqPt is associated with one or more
         // potential alterations to the configuration.

      void setSource (WitReqPt * theSource);
         //
         // source_ <-- theSource, if appropriate.

      void putDirectAlt (WitAltPt * theAltPt);
         //
         // Puts theAltPt into the list of direct alterations for this ReqPt,
         // if appropriate.

      void copyStateOf (WitReqPt * theReqPt);
         //
         // Copies the state of theReqPt into this ReqPt.

      void reset ();
         //
         // Puts this ReqPt in its initial state.

      bool getDirectAlt (WitAltPt * & theAltPt);
         //
         // If this ReqPt has any direct alts, one of them is
         // removed, theAltPt is set to it, and true is returned;
         // otherwise false is returned.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitReqPt *, source)
      accessFunc (bool,       hasResAlt)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitReqPt);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitPart * myPart_;
         //
         // The Part represented by this ReqPt.
         // Used only for debugging.

      WitPeriod myPer_;
         //
         // The Period represented by this ReqPt.
         // Used only for debugging.

      WitObjStack <WitAltPt> directAlts_;
         //
         // The AltPts at which the configuration would be altered as a
         // direct result of failure to meet requirements at this ReqPt.

      WitReqPt * source_;
         //
         // One of the of sources of requirements for this ReqPt, or NULL.

      bool hasResAlt_;
         //
         // true, iff this ReqPt has a resolving alteration.
   };

#endif
