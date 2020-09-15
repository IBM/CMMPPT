//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "msgApi.C"
//
// Inner API functions relating to the message facility.
//------------------------------------------------------------------------------

#include <WitRun.h>
#include <ApiMgr.h>
#include <InputID.h>
#include <MsgFrag.h>
#include <MsgFac.h>
#include <Str.h>

//------------------------------------------------------------------------------

void WitRun::witGetMesgFile (FILE * * fileP)
   {
   myApiMgr_->startInnerFunc ();

   apiGet (fileP, "fileP", myMsgFac ().myFile ());

   myMsgFac () ("getAttrMsg", "mesgFile");
   }

//------------------------------------------------------------------------------

void WitRun::witSetMesgFileName (
      WitBoolean   quiet,
      const char * newFileName)
   {
   FILE * tempFile;
   int    fcloseResult;

   if (quiet)
      myApiMgr_->skipCallMsg       ();

   myApiMgr_->   allowPreInit      ();
   myApiMgr_->   allowExtOptActive ();
   myApiMgr_->   startInnerFunc    ();

   myInputID ()->setMyAttName ("mesgFileName");

      // open new message file
      //
   if (equalCstrings (newFileName, WitSTDOUT))
      {
      tempFile = stdout;
      }
   else
      tempFile =
         mySession_->
            openFile (newFileName, myMsgFac ().accessMode ().myCstring ());

   if (not quiet)
      myMsgFac () ("setStringAttrMsg",
         "mesgFileName",
         myMsgFac ().fileName (),
         newFileName);

      // close old message file
      //
   if (myMsgFac ().fileName () != WitSTDOUT)
      {
      fcloseResult = fclose (myMsgFac ().myFile ());

      if (fcloseResult != 0)
         myMsgFac () ("fcloseSmsg", myMsgFac ().fileName ());
      }

   myMsgFac ().setMyFile (tempFile, newFileName);

   myInputID ()->setMyAttName (NULL);
   }

//------------------------------------------------------------------------------

void WitRun::witGetMesgFileName (char * * mesgFileName)
   {
   myApiMgr_->startInnerFunc ();

   apiGet (mesgFileName, "mesgFileName", myMsgFac ().fileName ());

   myMsgFac () ("getAttrMsg", "mesgFileName");
   }

//------------------------------------------------------------------------------

void WitRun::witSetMesgFileAccessMode (
      WitBoolean   quiet,
      const char * newMode )
   {
   if (quiet)
      myApiMgr_->skipCallMsg       ();

   myApiMgr_->   allowPreInit      ();
   myApiMgr_->   allowExtOptActive ();
   myApiMgr_->   startInnerFunc    ();

   myInputID ()->setMyAttName ("mesgFileAccessMode");

   if (not quiet)
      myMsgFac () ("setStringAttrMsg",
         "mesgFileAccessMode",
         myMsgFac ().accessMode (),
         newMode);

   myMsgFac ().setAccessMode (newMode);

   myInputID ()->setMyAttName (NULL);
   }

//------------------------------------------------------------------------------

void WitRun::witGetMesgFileAccessMode (char * * mesgFileAccessMode)
   {
   myApiMgr_->startInnerFunc ();

   apiGet (mesgFileAccessMode, "mesgFileAccessMode", myMsgFac ().accessMode ());

   myMsgFac () ("getAttrMsg", "mesgFileAccessMode");
   }

//------------------------------------------------------------------------------

void WitRun::witSetMesgPrintNumber (
      WitBoolean quiet,
      int        messageNumber,
      WitBoolean mesgPrintMunber)
   {
   if (quiet)
      myApiMgr_->skipCallMsg       ();

   myApiMgr_->   allowPreInit      ();
   myApiMgr_->   allowExtOptActive ();
   myApiMgr_->   startInnerFunc    ();

   myInputID ()->setMyAttName ("mesgPrintNumber");

   if (myMsgFac ().isExtNumOfLevel (messageNumber))
      {
      if (not quiet)
         myMsgFac () ("setMsgTypeBoolAttrMsg",
            "mesgPrintNumber",
            myMsgFac ().myFrag (levelExtNumFragID (messageNumber)),
            mesgPrintMunber);

      myMsgFac ().setDisplayExternalNumberByLevel (
         messageNumber,
         asaBool (mesgPrintMunber));
      }
   else if (checkMsgExtNum (messageNumber))
      {
      if (not quiet)
         myMsgFac () (
            "setMsgBoolAttrMsg",
            "mesgPrintNumber",
            messageNumber,
            mesgPrintMunber);

      myMsgFac ().setDisplayExternalNumber (
         messageNumber,
         asaBool (mesgPrintMunber));
      }

   myInputID ()->setMyAttName (NULL);
   }

//------------------------------------------------------------------------------

void WitRun::witGetMesgPrintNumber (
      int          messageNumber,
      WitBoolean * mesgPrintMunber)
   {
   myApiMgr_->startInnerFunc ();

   myMsgFac () ("getAttrMsg", "mesgPrintNumber");

   if (checkMsgExtNum (messageNumber))
      apiGet (
         mesgPrintMunber,
         "mesgPrintMunber",
         myMsgFac ().displayExternalNumber (messageNumber));
   }

//------------------------------------------------------------------------------

void WitRun::witSetMesgStopRunning (
      WitBoolean quiet,
      int        messageNumber,
      WitBoolean mesgStopRunning)
   {
   if (quiet)
      myApiMgr_->skipCallMsg       ();

   myApiMgr_->   allowPreInit      ();
   myApiMgr_->   allowExtOptActive ();
   myApiMgr_->   startInnerFunc    ();

   myInputID ()->setMyAttName ("mesgStopRunning");

   if (    messageNumber != WitINFORMATIONAL_MESSAGES
       and messageNumber != WitWARNING_MESSAGES)
      {
      if (myMsgFac ().isExtNumOfLevel (messageNumber))
         {
         if (not quiet)
            {
            myMsgFac () ("setMsgTypeBoolAttrMsg",
               "mesgStopRunning",
               myMsgFac ().myFrag (levelExtNumFragID (messageNumber)),
               mesgStopRunning);

            if (mesgStopRunning)
               myMsgFac () ("setMsgType2ndBoolAttrMsg",
                  "mesgThrowErrorExc",
                  myMsgFac ().myFrag (levelExtNumFragID (messageNumber)),
                  "mesgStopRunning");
            }

         myMsgFac ().setStopRunningByLevel (
            messageNumber,
            asaBool (mesgStopRunning));
         }
      else if (checkMsgExtNum (messageNumber))
         {
         if (not quiet)
            {
            myMsgFac () (
               "setMsgBoolAttrMsg",
               "mesgStopRunning",
               messageNumber,
               mesgStopRunning);

            if (mesgStopRunning)
               if (myMsgFac ().throwErrorExc (messageNumber))
                  myMsgFac () ("setMsg2ndBoolAttrMsg",
                     "mesgThrowErrorExc",
                     messageNumber,
                     "mesgStopRunning");
            }

         myMsgFac ().setStopRunning (messageNumber, asaBool (mesgStopRunning));
         }
      }

   myInputID ()->setMyAttName (NULL);
   }

//------------------------------------------------------------------------------

void WitRun::witGetMesgStopRunning (
      int          messageNumber,
      WitBoolean * mesgStopRunning)
   {
   myApiMgr_->startInnerFunc ();

   myMsgFac () ("getAttrMsg", "mesgStopRunning");

   if (checkMsgExtNum (messageNumber))
      apiGet (
         mesgStopRunning,
          "mesgStopRunning",
          myMsgFac ().stopRunning (messageNumber));
   }

//------------------------------------------------------------------------------

void WitRun::witSetMesgThrowErrorExc (
      WitBoolean quiet,
      int        messageNumber,
      WitBoolean mesgThrowErrorExc)
   {
   if (quiet)
      myApiMgr_->skipCallMsg       ();

   myApiMgr_->   allowPreInit      ();
   myApiMgr_->   allowExtOptActive ();
   myApiMgr_->   startInnerFunc    ();

   myInputID ()->setMyAttName ("mesgThrowErrorExc");

   if (    messageNumber != WitINFORMATIONAL_MESSAGES
       and messageNumber != WitWARNING_MESSAGES)
      {
      if (myMsgFac ().isExtNumOfLevel (messageNumber))
         {
         if (not quiet)
            {
            myMsgFac () ("setMsgTypeBoolAttrMsg",
               "mesgThrowErrorExc",
               myMsgFac ().myFrag (levelExtNumFragID (messageNumber)),
               mesgThrowErrorExc);

            if (mesgThrowErrorExc)
               myMsgFac () ("setMsgType2ndBoolAttrMsg",
                  "mesgStopRunning",
                  myMsgFac ().myFrag (levelExtNumFragID (messageNumber)),
                  "mesgThrowErrorExc");
            }

         myMsgFac ().setThrowErrorExcByLevel (
            messageNumber,
            asaBool (mesgThrowErrorExc));
         }
      else if (checkMsgExtNum (messageNumber))
         {
         if (not quiet)
            {
            myMsgFac () ("setMsgBoolAttrMsg",
               "mesgThrowErrorExc",
               messageNumber,
               mesgThrowErrorExc);

            if (mesgThrowErrorExc)
               if (myMsgFac ().stopRunning (messageNumber))
                  myMsgFac () ("setMsg2ndBoolAttrMsg",
                     "mesgStopRunning",
                     messageNumber,
                     "mesgThrowErrorExc");
            }

         myMsgFac ().
            setThrowErrorExc (messageNumber, asaBool (mesgThrowErrorExc));
         }
      }

   myInputID ()->setMyAttName (NULL);
   }

//------------------------------------------------------------------------------

void WitRun::witGetMesgThrowErrorExc (
      int          messageNumber,
      WitBoolean * mesgThrowErrorExc)
   {
   myApiMgr_->startInnerFunc ();

   myMsgFac () ("getAttrMsg", "mesgThrowErrorExc");

   if (checkMsgExtNum (messageNumber))
      apiGet (
         mesgThrowErrorExc,
          "mesgThrowErrorExc",
          myMsgFac ().throwErrorExc (messageNumber));
   }

//------------------------------------------------------------------------------

void WitRun::witSetMesgTimesPrint (
      WitBoolean quiet,
      int        messageNumber,
      int        mesgTimesPrint)
   {
   if (quiet)
      myApiMgr_->skipCallMsg       ();

   myApiMgr_->   allowPreInit      ();
   myApiMgr_->   allowExtOptActive ();
   myApiMgr_->   startInnerFunc    ();

   myInputID ()->setMyAttName ("mesgTimesPrint");

   int maxTimesIssued;
      //
      // Same as mesgTimesPrint, but negative == infinity.

      // Check range of mesgTimesPrint
      //
   if (mesgTimesPrint < 0 or mesgTimesPrint > UCHAR_MAX)
      myMsgFac () ("intLowEqUpEqImmSmsg", mesgTimesPrint, 0, UCHAR_MAX);

   maxTimesIssued =
      (mesgTimesPrint == UCHAR_MAX)?
         -1:
         mesgTimesPrint;

   if (myMsgFac ().isExtNumOfLevel (messageNumber))
      {
      if (not quiet)
         myMsgFac () ("chgMsgsTimesPrintSm",
            myMsgFac ().myFrag (levelExtNumFragID (messageNumber)),
            mesgTimesPrint);

      myMsgFac ().setMaxTimesIssuedByLevel (messageNumber, maxTimesIssued);
      }
   else if (checkMsgExtNum (messageNumber))
      {
      if (not quiet)
         myMsgFac () ("chgMsgTimesPrintSm", messageNumber, mesgTimesPrint);

      myMsgFac ().setMaxTimesIssued (messageNumber, maxTimesIssued);
      }

   myInputID ()->setMyAttName (NULL);
   }

//------------------------------------------------------------------------------

void WitRun::witGetMesgTimesPrint (
      int   messageNumber,
      int * mesgTimesPrint)
   {
   int maxTimesIssued;
      //
      // Same as mesgTimesPrint, but negative == infinity.

   myApiMgr_->startInnerFunc ();

   myMsgFac () ("getAttrMsg", "mesgTimesPrint");

   if (checkMsgExtNum (messageNumber))
      {
      maxTimesIssued = myMsgFac ().maxTimesIssued (messageNumber);

      apiGet (
         mesgTimesPrint,
         "mesgTimesPrint",
         (maxTimesIssued < 0)?
            UCHAR_MAX:
            maxTimesIssued);
      }
   }

//------------------------------------------------------------------------------

void WitRun::witListUnissuedMsgs ()
   {
   myApiMgr_->startInnerFunc ();

   if (not devMode ())
      myMsgFac () ("funcNotAvailSmsg", "witListUnissuedMsgs");

   myMsgFac ().listUnissuedMsgs ();
   }

//------------------------------------------------------------------------------

const char * WitRun::levelExtNumFragID (int theExtNum)
   {
   WitMsgID theMsgID;

   theMsgID =
      (theExtNum == WitINFORMATIONAL_MESSAGES)? "informationalMsgFrag":
      (theExtNum == WitWARNING_MESSAGES)?       "warningMsgFrag":
      (theExtNum == WitSEVERE_MESSAGES)?        "severeMsgFrag":
      (theExtNum == WitFATAL_MESSAGES)?         "fatalMsgFrag":
                                                NULL;

   witAssert (theMsgID != NULL);

   return theMsgID;
   }

//------------------------------------------------------------------------------

bool WitRun::checkMsgExtNum (int theExtNum)
   {
   bool theExtNumIsValid;

   theExtNumIsValid = myMsgFac ().isExtNumOfMsg (theExtNum);

   if (not theExtNumIsValid)
      myMsgFac () ("invalidExtNumWmsg", apiFuncName (), theExtNum);

   return theExtNumIsValid;
   }
