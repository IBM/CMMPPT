//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef MsgBuilderH
#define MsgBuilderH

//------------------------------------------------------------------------------
// Header file: "MsgBuilder.h"
//
// Contains the declaration of class MsgBuilder.
//------------------------------------------------------------------------------

#include <typedefs.h>

class MclLevel;

//------------------------------------------------------------------------------
// Class MsgBuilder
//
// Responsible for creating the Msgs and MsgFrags of WIT.
//------------------------------------------------------------------------------

class WitMsgBuilder
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitMsgBuilder (WitMsgMgr * theMsgMgr);
         //
         // myMsgMgr_ <-- theMsgMgr.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitMsgBuilder ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void buildMsgFrags ();
         //
         // Creates all of the MsgFrags for myMsgMgr_.

      void buildMsgs ();
         //
         // Creates all of the Msgs for myMsgMgr_.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitMsgBuilder);

      void makeMsgFrag (WitMsgFragID theFragID, const char * textVal);
         //
         // Creates a new MsgFrag with the indicated arguments.

      void makeMsg (
            WitMsgID         theMsgID,
            int              theExtNum,
            const MclLevel & theLevel,
            const char *     textVal);
         //
         // Creates a new Msg with the indicated arguments.
         // Note that the args are in a different order than in the Msg ctor.

      void checkExtNumOrder (int theExtNum);
         //
         // Verifies that theExtNum > currentMsg->externalNumber ().

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitMsgMgr * const myMsgMgr_;
         //
         // The MsgMgr for this MsgBuilder.

      const MclLevel & info_;
      const MclLevel & warning_;
      const MclLevel & severe_;
      const MclLevel & fatal_;

      WitMsg * currentMsg_;
         //
         // The Msg that was most recently built by makeMsg, if any;
         // otherwise NULL.
   };

#endif
