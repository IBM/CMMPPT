//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef OptStarterH
#define OptStarterH

//------------------------------------------------------------------------------
// Header file: "OptStarter.h"
//
// Contains the declaration of class OptStarter.
//------------------------------------------------------------------------------

#include <Str.h>

//------------------------------------------------------------------------------
// class OptStarter
//
// Responsible for determining the means by which an initial solution for opt
// implosion is to be generated.
//
// Implemented in Misc.C.
//------------------------------------------------------------------------------

class WitOptStarter
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitOptStarter (
            WitOptComp * theOptComp,
            const char * theParamValue,
            WitAttr      theApiAttr,
            WitMsgID     theMsgID,
            bool         externalVal);
         //
         // paramValue     <-- theParamValue.
         // myApiAttr_     <-- theApiAttr.
         // myMsgID_       <-- theMsgID.
         // external_      <-- externalVal.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitOptStarter ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitOptComp *,      myOptComp)
      accessFunc (const WitString &, paramValue)
      accessFunc (WitAttr,           myApiAttr)
      accessFunc (bool,              external)

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void beChosen ();
         //
         // Sets myProblem ()->myOptComp ()->optInitMethod_ to this
         // OptStarter.

      bool isChosen ();
         //
         // Returns true, iff
         // this == myProblem ()->myOptComp ()->optInitMethod_ ().

      static const WitString & paramValue (const WitOptStarter * theOptStarter);
         //
         // Returns theOptStarter->paramValue ();

      void display ();
         //
         // Issues myMsgID_.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitOptStarter);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitOptComp * const myOptComp_;
         //
         // The OptComp that owns this OptStarter.

      const WitString paramValue_;
         //
         // String identifying this OptStarter as a stand-alone control
         // parameter value.

      const WitAttr myApiAttr_;
         //
         // The API attribute for this OptStarter.

      WitMsgID const myMsgID_;
         //
         // The Msg ID of the informational message identifying this OptStarter.
         // This Msg will be issued at the beginning of opt implosion.

      const bool external_;
         //
         // true, iff the initial solution for opt implosion is to generated
         // externally (i.e. from implosion schedules).
   };

#endif
