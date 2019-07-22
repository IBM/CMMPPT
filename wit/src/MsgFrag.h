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

#ifndef MsgFragH
#define MsgFragH

//------------------------------------------------------------------------------
// Header file: "MsgFrag.h"
//
// Contains the declaration of class MsgFrag.
//------------------------------------------------------------------------------

#include <Util.h>

//------------------------------------------------------------------------------

class MclMsgFrag;

//------------------------------------------------------------------------------
// Class MsgFrag
//
// Wrapper for an MclMsgFrag.
//
// Implemented inline.
//------------------------------------------------------------------------------

class WitMsgFrag
   {
      //------------------------------------------------------------------------
      // Friends.
      //------------------------------------------------------------------------

      friend class WitMsgArg;
      friend class WitMsgFacility;

   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      inline WitMsgFrag (MclMsgFrag * theMclMsgFrag):
            myMclMsgFrag_ (theMclMsgFrag)
         {
         }

      inline WitMsgFrag (const WitMsgFrag & theMsgFrag):
            myMclMsgFrag_ (theMsgFrag.myMclMsgFrag_)
         {
         }
            //
            // Copy ctor.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      inline ~WitMsgFrag ()
         {
         }

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      WitMsgFrag & operator = (const WitMsgFrag &);
         //
         // Not implemented; prevents unintentional assignment.

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      MclMsgFrag * const myMclMsgFrag_;
         //
         // The MclMsgFrag for which this MsgFrag is a wrapper.
   };

#endif
