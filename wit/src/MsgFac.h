//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef MsgFacH
#define MsgFacH

//------------------------------------------------------------------------------
// Header file: "MsgFac.h"
//
// Contains the declaration of class MsgFacility.
//------------------------------------------------------------------------------

#include <MsgArg.h>

//------------------------------------------------------------------------------
// class MsgFacility
//
// Responsible for managing and issuing messages.
// This is the interface to the Message Subsystem, which is implemented using
// MCL.
//------------------------------------------------------------------------------

class WitMsgFacility
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitMsgFacility (WitSession * theSession);
         //
         // mySession_ <-- theSession.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitMsgFacility ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitSession *, mySession)

      FILE *            myFile     ();
      const WitString & fileName   ();
      const WitString & accessMode ();
      int               lineLength ();

      const char * boolText (bool theValue);
         //
         // Returns the text representing theValue.

      inline static bool stopRunningWasSet ()
         {
         return stopRunningWasSet_;
         }

      //------------------------------------------------------------------------
      // Access functions for Msgs.
      // Each of these functions returns the value of the indicated attribute
      // of the Msg whose external number is given by the arg.
      //------------------------------------------------------------------------

      bool displayExternalNumber (int);
      bool stopRunning           (int);
      bool throwErrorExc         (int);
      int  maxTimesIssued        (int);

      //------------------------------------------------------------------------
      // Set functions.
      //------------------------------------------------------------------------

      void setAccessMode (const char *);
      void setLineLength (int);

      void setMyFile (FILE * theFile, const char * theName);
         //
         // myFile   <-- theFile.
         // fileName <-- theName.

      void setBoolMsgFrags (
            WitMsgFrag theTrueFrag,
            WitMsgFrag theFalseFrag);
         //
         // Sets the MsgFrags for true and false to the arguments.

      //------------------------------------------------------------------------
      // Set functions for Msgs.
      // Each of these functions sets the indicated attribute of the Msg whose
      // external number is given by the first arg to the value of the second
      // arg.
      //------------------------------------------------------------------------

      void setDisplayExternalNumber (int, bool);
      void setStopRunning           (int, bool);
      void setThrowErrorExc         (int, bool);
      void setMaxTimesIssued        (int, int);

      //------------------------------------------------------------------------
      // Set functions for Severity Levels.
      // Each of these functions sets the indicated attribute of the Msg whose
      // Severity Level has an external number is given by the first arg to
      // the value of the second arg.
      //------------------------------------------------------------------------

      void setDisplayExternalNumberByLevel (int, bool);
      void setStopRunningByLevel           (int, bool);
      void setThrowErrorExcByLevel         (int, bool);
      void setMaxTimesIssuedByLevel        (int, int);

      //------------------------------------------------------------------------
      // Message issuing functions.
      // Each of the following functions issues the Msg with the given msgID
      // and the given MsgArgs.
      //------------------------------------------------------------------------

      void operator () (
           WitMsgID theMsgID
         );

      void operator () (
           WitMsgID theMsgID
         , const WitMsgArg &
         );

      void operator () (
           WitMsgID theMsgID
         , const WitMsgArg &
         , const WitMsgArg &
         );

      void operator () (
           WitMsgID theMsgID
         , const WitMsgArg &
         , const WitMsgArg &
         , const WitMsgArg &
         );

      void operator () (
           WitMsgID theMsgID
         , const WitMsgArg &
         , const WitMsgArg &
         , const WitMsgArg &
         , const WitMsgArg &
         );

      void operator () (
           WitMsgID theMsgID
         , const WitMsgArg &
         , const WitMsgArg &
         , const WitMsgArg &
         , const WitMsgArg &
         , const WitMsgArg &
         );


      void operator () (
           WitMsgID theMsgID
         , const WitMsgArg &
         , const WitMsgArg &
         , const WitMsgArg &
         , const WitMsgArg &
         , const WitMsgArg &
         , const WitMsgArg &

         , const WitMsgArg & theMsgArg7  = emptyArg ()
         , const WitMsgArg & theMsgArg8  = emptyArg ()
         , const WitMsgArg & theMsgArg9  = emptyArg ()
         , const WitMsgArg & theMsgArg10 = emptyArg ()
         , const WitMsgArg & theMsgArg11 = emptyArg ()
         , const WitMsgArg & theMsgArg12 = emptyArg ()
         , const WitMsgArg & theMsgArg13 = emptyArg ()
         , const WitMsgArg & theMsgArg14 = emptyArg ()
         , const WitMsgArg & theMsgArg15 = emptyArg ()
         );
         //
         // Handles 6-15 Args; expand as needed.

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      static void issueByException (
            WitMsgID          theMsgID,
            const WitMsgArg & theMsgArg1 = emptyArg (),
            const WitMsgArg & theMsgArg2 = emptyArg (),
            const WitMsgArg & theMsgArg3 = emptyArg ());
         //
         // Throws an exception that is caught by the current API function,
         // which then issues the Msg with the given msgID and the given
         // MsgArgs. This Msg must be an aborting Msg.

      void abortIfErrors ();
         //
         // Aborts, if any Msgs of Level >= error has been issued.

      WitMsgFrag myFrag (WitMsgFragID theFragID);
         //
         // Creates and returns a MsgFrag for the MclMsgFrag whose id_ matches
         // theFragID. Issues a fatal error if no such MclMsgFrag exists.

      const char * fragText (WitMsgFragID theFragID);
         //
         // Returns the message fragment text identified by theFragID.

      bool mayIssue (WitMsgID theMsgID);
         //
         // Returns true, iff the indicated Msg is currently eligible for being
         // issued. More presicely, if it is possible that the Msg would be
         // issued, the function certainly returns true; if it is not possible,
         // the function probably returns false, but there may be cases in which
         // it returns true anyway. This function allows client code to filter
         // out unneeded Msg issuing, to improve run-time speed.

      void setMaxTimesIssued (WitMsgID theMsgID, int theValue);
         //
         // Sets maxTimesIssued if the Msg whose ID is theMsgID to theValue.

      bool isExtNumOfMsg (int theExtNum);
         //
         // Returns true, iff theExtNum is the extNum of a Msg.

      bool isExtNumOfLevel (int theExtNum);
         //
         // Returns true, iff theExtNum is the extNum of an MclLevel.

      void listUnissuedMsgs ();
         //
         // Displays a list of a testable Msgs that have not been issued.

      bool displayStateChange (
            WitMsgID theMsgID,
            bool     currentMode,
            bool     newMode);
         //
         // To be called when the state of the WitRun is being changed.
         // currentMode is the current value of the attribute.
         // newMode     is the new     value of the attribute.
         // If the values are different, the indicated Msg is issued.
         // Returns the new state attribute value.

      void setCrashOnFatal (bool theValue);
         //
         // myMsgMsg->crashOnFatal_ <-- theValue

      void divide ();
         //
         // Issues a divider Msg of the appropriate length.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitMsgFacility);

      inline MclFacility & myMclFacility ();
         //
         // Returns myMsgMgr_->myMclFacility ()

      inline WitOutputDisplayer & myDisp ();
         //
         // Returns myMsgMgr_->myDisp ()

      WitMsg * extNumMsg (int theExtNum);
         //
         // Returns is the Msg whose externalNumber is theExtNum.
         // Asserts that one exists.

      const MclLevel & extNumLevel (int theExtNum);
         //
         // Returns the MclLevel matching theExtNum, which is require to exist.

      const MclLevel * extNumLevelPtr (int theExtNum);
         //
         // Returns a ptr to the MclLevel matching theExtNum, if any, else NULL.

      inline WitMsgFacility & myMsgFac ()
         {
         return * this;
         }
         //
         // Allows this MsgFacility to use itself in a manner identical to its
         // clients.

      inline static const WitMsgArg & emptyArg ()
         {
         return * emptyArgPtr_;
         }
         //
         // Returns a MsgArg that contains no actual argument.
         // Used as a default value for optional arguments.

      //------------------------------------------------------------------------
      // Static private member data.
      //------------------------------------------------------------------------

      static const WitMsgArg * emptyArgPtr_;
         //
         // Ptr to a MsgArg that contains no actual argument.
         // Used as a default value for optional arguments.

      static bool stopRunningWasSet_;
         //
         // True, iff the stopRunning attribute of any Msg has been set.

      //------------------------------------------------------------------------
      // Non-static private member data.
      //------------------------------------------------------------------------

      WitSession * const mySession_;
         //
         // The Session that owns this MsgFacility.

      WitMsgMgr * myMsgMgr_;
         //
         // The MsgMgr owned by this MsgFacility.

      bool usingTempFile_;
         //
         // true, iff this MsgFacility is currently using a temp file.

      FILE * nonTempFile_;
         //
         // The non-temporary file, when a temporary one is being used;
         // otherwise NULL.
   };

#endif
