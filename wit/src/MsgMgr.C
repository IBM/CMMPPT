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

//------------------------------------------------------------------------------
// Source file: "MsgMgr.C"
//
// Contains the implementation of the following classes:
//
//    OutputDisplayer.
//    Msg.
//    MsgMgr.
//------------------------------------------------------------------------------

#include <MsgMgr.h>
#include <Msg.h>
#include <OutDisp.h>
#include <MsgBuilder.h>
#include <DataRead.h>
#include <Session.h>
#include <ApiMgr.h>
#include <AbortApiExc.h>
#include <InputID.h>
#include <SaeMgr.h>
#include <Problem.h>
#include <GlobalComp.h>

//------------------------------------------------------------------------------
// Implementation of class OutputDisplayer.
//------------------------------------------------------------------------------

WitOutputDisplayer::WitOutputDisplayer (WitMsgMgr * theMsgMgr):
      MclOutputDisplayer (),

      myMsgMgr_          (theMsgMgr),
      myFile_            (stdout),
      fileName_          ("stdout"),
      accessMode_        ("w")
   {
   }

//------------------------------------------------------------------------------

WitOutputDisplayer::~WitOutputDisplayer ()
   {
   bool closeResult;

   if (myFile_ == stdout)
      fflush (stdout);
   else
      {
      closeResult = fclose (myFile_);

      if (closeResult != 0)
         {
         startNonMsgSevereError ();

         fprintf (stderr,
            "An error was encountered when closing the message file.\n"
            "\n"
            "   File Name: %s\n",
            fileName_.myCstring ());

         finishNonMsgSevereError ();
         }
      }
   }

//------------------------------------------------------------------------------

void WitOutputDisplayer::setAccessMode (const char * theValue)
   {
   accessMode_ = theValue;
   }

//------------------------------------------------------------------------------

void WitOutputDisplayer::setMyFile (FILE * theFile, const char * theName)
   {
   myFile_   = theFile;
   fileName_ = theName;
   }

//------------------------------------------------------------------------------

void WitOutputDisplayer::output (const char * finalText)
   {
   if (     (myMsgMgr_->issuedMsg ()->myLevel () == MclLevel::info ())
         or (myFile_ != stdout))
      {
      fprintf (myFile_, "%s", finalText);
      }

   if (WitSaeMgr::standAloneMode ())
      if (myMsgMgr_->issuedMsg ()->dupToStdout ())
         if (myFile_ != stdout)
            {
            printf ("%s", finalText);
            }
   }

//------------------------------------------------------------------------------

void WitOutputDisplayer::outputError (const char * finalText)
   {
   fflush (myFile ());

   fprintf (stderr, "%s", finalText);

   fflush (stderr);
   }

//------------------------------------------------------------------------------
// Implementation of class Msg.
//------------------------------------------------------------------------------

WitMsg::WitMsg (
         WitMsgMgr *      theMsgMgr,
         WitMsgID         theMsgID,
         const MclLevel & theLevel,
         const char *     textVal,
         int              extNum):

      MclMsg (
         theMsgMgr->myMclFacility (),
         theMsgID,
         theLevel,
         textVal,
         extNum),

      myMsgMgr_       (theMsgMgr),
      delayedSevere_  (false),
      stopRunning_    (true),
      throwErrorExc_  (false),
      dupToStdout_    (false),
      issueDuringRD_  (theLevel >  MclLevel::info ()),
      idInputOnIssue_ (false),
      testable_       (theLevel <= MclLevel::warning ())
   {
   vectorIndent (6);

   myMsgMgr_->record (this);
   }

//------------------------------------------------------------------------------

WitMsg::~WitMsg ()
   {
   }

//------------------------------------------------------------------------------

void WitMsg::delayedSevere (bool theValue)
   {
   int dummyVar;

   if (theValue)
      nonMsgAssert (myLevel () == MclLevel::severe ());

   set (dummyVar, "delayedSevere", theValue);

   delayedSevere_ = theValue;
   }

//------------------------------------------------------------------------------

void WitMsg::stopRunning (bool theValue)
   {
   int dummyVar;

   if (theValue)
      throwErrorExc_ = false;

   set (dummyVar, "stopRunning", theValue);

   stopRunning_ = theValue;
   }

//------------------------------------------------------------------------------

void WitMsg::throwErrorExc (bool theValue)
   {
   int dummyVar;

   if (theValue)
      stopRunning_ = false;

   set (dummyVar, "throwErrorExc", theValue);

   throwErrorExc_ = theValue;
   }

//------------------------------------------------------------------------------

void WitMsg::dupToStdout (bool theValue)
   {
   int dummyVar;

   nonMsgAssert (myLevel () == MclLevel::info ());

   set (dummyVar, "dupToStdout", theValue);

   dupToStdout_ = theValue;
   }

//------------------------------------------------------------------------------

void WitMsg::issueDuringRD (bool theValue)
   {
   int dummyVar;

   set (dummyVar, "issueDuringRD", theValue);

   issueDuringRD_ = theValue;
   }

//------------------------------------------------------------------------------

void WitMsg::idInputOnIssue (bool theValue)
   {
   int dummyVar;

   set (dummyVar, "idInputOnIssue", theValue);

   idInputOnIssue_ = theValue;
   }

//------------------------------------------------------------------------------

void WitMsg::testable (bool theValue)
   {
   int dummyVar;

   set (dummyVar, "testable", theValue);

   testable_ = theValue;
   }

//------------------------------------------------------------------------------

void WitMsg::postIssue ()
   {
   }

//------------------------------------------------------------------------------
// Implementation of class MsgMgr.
//------------------------------------------------------------------------------

WitMsgMgr::WitMsgMgr (WitMsgFacility & theMsgFac):
      myMsgFac_         (theMsgFac),
      myDispPtr_        (NULL),
      myMclFacilityPtr_ (NULL),
      myApiMgr_         (theMsgFac.mySession ()->myApiMgr ()),
      extNumMsg_        (10000),
      errorsIssued_     (false),
      issuedMsg_        (NULL),
      crashOnFatal_     (false)
   {
   myDispPtr_        = new WitOutputDisplayer (this);
   myMclFacilityPtr_ = new MclFacility        ("WIT", myDisp ());

   WitMsgBuilder theBuilder (this);

   theBuilder.buildMsgFrags ();

   theBuilder.buildMsgs ();

   myMclFacility ().booleanMsgFrags (
      myMclFacility ().findMsgFrag ("trueFrag"),
      myMclFacility ().findMsgFrag ("falseFrag"));

   resizeExtNumMsg ();
   }

//------------------------------------------------------------------------------

WitMsgMgr::~WitMsgMgr ()
   {
   delete myMclFacilityPtr_;
   delete myDispPtr_;
   }

//------------------------------------------------------------------------------

void WitMsgMgr::record (WitMsg * theMsg)
   {
   nonMsgAssert (extNumMsg (theMsg->externalNumber ()) == NULL);

   extNumMsg_[theMsg->externalNumber ()] = theMsg;
   }

//------------------------------------------------------------------------------

void WitMsgMgr::issue (WitMsgID theMsgID, const MclArgList & theArgList)
   {
   MclMsg * theMclMsg;
   WitMsg * theMsg;
   WitMsg * prevIssuedMsg;

   theMclMsg = myMclFacility ().findMsg (theMsgID);

   if (theMclMsg == NULL)
      myMsgFac () ("invalidMsgIDFmsg", theMsgID);

   theMsg = myMsg (theMclMsg);

   if (not theMsg->delayedSevere ())
      myApiMgr ()->updateApiRc (theMsg->myLevel ().severity ());

   if (theMsg->myLevel () >= MclLevel::error ())
      errorsIssued_ = true;

   prevIssuedMsg = issuedMsg_;
   issuedMsg_    = theMsg;

   if (issueAllowed (theMsg))
      theMsg->issue (theArgList);

   issuedMsg_    = prevIssuedMsg;

   if (theMsg->myLevel () == MclLevel::fatal ())
      {
      myMsgFac () ("explainFatalMessageMsg");

      if (crashOnFatal_)
         crashForFatal ();
      }

   if (theMsg->idInputOnIssue ())
      myApiMgr ()->myInputID ()->idInput ();

   if (theMsg->myLevel () >= MclLevel::error ())
      if (not theMsg->delayedSevere ())
         throw WitAbortApiExc (theMsg);
   }

//------------------------------------------------------------------------------

WitMsg * WitMsgMgr::myMsg (MclMsg * theMclMsg) const
   {
   WitMsg * theMsg;

   theMsg = extNumMsg (theMclMsg->externalNumber ());

   nonMsgAssert (theMsg != NULL);

   return theMsg;
   }

//------------------------------------------------------------------------------

WitMsg * WitMsgMgr::extNumMsg (int theExtNum) const
   {
   if (theExtNum < 0)
      return NULL;

   if (theExtNum >= extNumMsg_.length ())
      return NULL;

   return extNumMsg_[theExtNum];
   }

//------------------------------------------------------------------------------

bool WitMsgMgr::issueAllowed (WitMsg * theMsg) const
   {
   if (not mySession ()->active ())
      return true;

   if (not mySession ()->myProblem ()->myDataReader ()->active ())
      return true;

   if (mySession ()->myProblem ()->myGlobalComp ()->readDataMsgs ())
      return true;

   return (theMsg->issueDuringRD ());
   }

//------------------------------------------------------------------------------

void WitMsgMgr::setCrashOnFatal (bool theValue)
   {
   crashOnFatal_ = theValue;
   }

//------------------------------------------------------------------------------

void WitMsgMgr::resizeExtNumMsg ()
   {
   int maxExtNum;
   int theExtNum;

   WitPtrVec <WitMsg> tempExtNumMsg;


   maxExtNum = -1;

   for (theExtNum = 0; theExtNum < 10000; ++ theExtNum)
      if (extNumMsg (theExtNum) != NULL)
         maxExtNum = theExtNum;

   tempExtNumMsg.resize (maxExtNum + 1);

   for (theExtNum = 0; theExtNum <= maxExtNum; ++ theExtNum)
      tempExtNumMsg[theExtNum] = extNumMsg_[theExtNum];

   extNumMsg_.resize (maxExtNum + 1);

   extNumMsg_ = tempExtNumMsg;
   }

//------------------------------------------------------------------------------

void WitMsgMgr::crashForFatal ()
   {
   void (* theFunc) ();

   theFunc = NULL;

   myMsgFac () ("crashMsg");

   fflush (stdout);

   theFunc ();
      //
      // Causes a segmentation fault on Linux.
   }
