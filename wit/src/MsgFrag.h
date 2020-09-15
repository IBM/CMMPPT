//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
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
