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

#ifndef IssueMsgExcH
#define IssueMsgExcH

//------------------------------------------------------------------------------
// Header file: "IssueMsgExc.h"
//
// Contains the declaration of class IssueMsgExc.
//------------------------------------------------------------------------------

#include <MsgArg.h>

//------------------------------------------------------------------------------
// Class IssueMsgExc
//
// "Issue Message Exception"
// An exception of this type is thrown, when the code needs to issue a message,
// but cannot locally determine a MsgFacility to use. The exception is caught by
// the API, which issues the message. An IssueMsgExc stores a MsgID and three
// MsgArgs that are to be used to issue the message.
//
// Implemented in MsgFac.C.
//------------------------------------------------------------------------------

class WitIssueMsgExc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitIssueMsgExc (
            WitMsgID          theMsgID,
            const WitMsgArg & theMsgArg1,
            const WitMsgArg & theMsgArg2,
            const WitMsgArg & theMsgArg3);

      WitIssueMsgExc (const WitIssueMsgExc &);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitIssueMsgExc ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitMsgID,          myMsgID)
      accessFunc (const WitMsgArg &, myMsgArg1)
      accessFunc (const WitMsgArg &, myMsgArg2)
      accessFunc (const WitMsgArg &, myMsgArg3)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      WitMsgArg & operator = (const WitMsgArg &);
         //
         // Not implemented.

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitMsgID myMsgID_;
         //
         // The MsgID of the Msg to be issued.

      WitMsgArg myMsgArg1_;
      WitMsgArg myMsgArg2_;
      WitMsgArg myMsgArg3_;
         //
         // The MsgArgs to be used in issuing the Msg.
   };

#endif
