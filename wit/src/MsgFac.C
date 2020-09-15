//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "MsgFac.C"
//
// Contains the implementation of the following classes:
//
//    MsgArg.
//    MsgFacility.
//    TempMsgFile.
//    IssueMsgExc.
//    AbortApiExc.
//
// Also contains the declaration and implementation of
// global operator << (MclArgList, WitMsgArg).
//------------------------------------------------------------------------------

#include <MsgMgr.h>
#include <MsgFrag.h>
#include <Session.h>
#include <TempMsgFile.h>
#include <IssueMsgExc.h>
#include <AbortApiExc.h>
#include <Msg.h>
#include <OutDisp.h>
#include <ISRealArg.h>
#include <wit.h>

//------------------------------------------------------------------------------
// Implementation of class MsgArg.
//------------------------------------------------------------------------------

WitMsgArg::WitMsgArg ():
      myMclArgPtr_ (NULL)
   {
   }

//------------------------------------------------------------------------------

WitMsgArg::WitMsgArg (bool rawArg):
      myMclArgPtr_ (new MclBoolArg (rawArg))
   {
   }

//------------------------------------------------------------------------------

WitMsgArg::WitMsgArg (int rawArg):
      myMclArgPtr_ (new MclIntArg (rawArg))
   {
   }

//------------------------------------------------------------------------------

WitMsgArg::WitMsgArg (size_t rawArg):
      myMclArgPtr_ (new MclIntArg (rawArg))
   {
   }

//------------------------------------------------------------------------------

WitMsgArg::WitMsgArg (long rawArg):
      myMclArgPtr_ (new MclIntArg (static_cast <int> (rawArg)))
   {
   }

//------------------------------------------------------------------------------

WitMsgArg::WitMsgArg (double rawArg):
      myMclArgPtr_ (new MclDoubleArg (rawArg))
   {
   }

//------------------------------------------------------------------------------

WitMsgArg::WitMsgArg (const WitISRealArg & rawArg):
      myMclArgPtr_ (new MclDoubleArg (rawArg.asaDbl ()))
   {
   }

//------------------------------------------------------------------------------

WitMsgArg::WitMsgArg (const char * rawArg):
      myMclArgPtr_ (new MclStringArg (rawArg))
   {
   }

//------------------------------------------------------------------------------

WitMsgArg::WitMsgArg (const WitString & rawArg):
      myMclArgPtr_ (new MclStringArg (rawArg.myCstring ()))
   {
   }

//------------------------------------------------------------------------------

WitMsgArg::WitMsgArg (const WitMsgFrag & rawArg):
      myMclArgPtr_ (new MclMsgFragArg (rawArg.myMclMsgFrag_))
   {
   }

//------------------------------------------------------------------------------

WitMsgArg::WitMsgArg (const int * rawArg, int length):
      myMclArgPtr_ (new MclIntVectorArg (rawArg, length))
   {
   }

//------------------------------------------------------------------------------

WitMsgArg::WitMsgArg (const float * rawArg, int length):
      myMclArgPtr_ (new MclFloatVectorArg (rawArg, length))
   {
   }

//------------------------------------------------------------------------------

WitMsgArg::WitMsgArg (const double * rawArg, int length):
      myMclArgPtr_ (new MclDoubleVectorArg (rawArg, length))
   {
   }

//------------------------------------------------------------------------------

WitMsgArg::WitMsgArg (const WitVector <bool> & rawArg):
      myMclArgPtr_ (
         new MclBoolVectorArg (rawArg.myCVec (), rawArg.length ()))
   {
   }

//------------------------------------------------------------------------------

WitMsgArg::WitMsgArg (const WitVector <int> & rawArg):
      myMclArgPtr_ (
         new MclIntVectorArg (rawArg.myCVec (), rawArg.length ()))
   {
   }

//------------------------------------------------------------------------------

WitMsgArg::WitMsgArg (const WitVector <double> & rawArg):
      myMclArgPtr_ (
         new MclDoubleVectorArg (rawArg.myCVec (), rawArg.length ()))
   {
   }

//------------------------------------------------------------------------------

WitMsgArg::~WitMsgArg ()
   {
   delete myMclArgPtr_;
   }

//------------------------------------------------------------------------------

void WitMsgArg::operator = (const WitMsgArg & theMsgArg)
   {
   delete myMclArgPtr_;

   myMclArgPtr_ =
      theMsgArg.hasArg ()?
         theMsgArg.myMclArg ().clone ():
         NULL;
   }

//------------------------------------------------------------------------------
// Global operator << (MclArgList, WitMsgArg).
//
// Conditional argument appending.
//------------------------------------------------------------------------------

inline MclArgList & operator << (
      MclArgList &      theArgList,
      const WitMsgArg & theMsgArg)
   {
   if (theMsgArg.hasArg ())
      theArgList << theMsgArg.myMclArg ();

   return theArgList;
   }

//------------------------------------------------------------------------------
// Implementation of class MsgFacility.
//------------------------------------------------------------------------------

const WitMsgArg * WitMsgFacility::emptyArgPtr_       = NULL;
bool              WitMsgFacility::stopRunningWasSet_ = false;

//------------------------------------------------------------------------------

inline MclFacility & WitMsgFacility::myMclFacility ()
   {
   return myMsgMgr_->myMclFacility ();
   }

//------------------------------------------------------------------------------

inline WitOutputDisplayer & WitMsgFacility::myDisp ()
   {
   return myMsgMgr_->myDisp ();
   }

//------------------------------------------------------------------------------

WitMsgFacility::WitMsgFacility (WitSession * theSession):
      mySession_     (theSession),
      myMsgMgr_      (NULL),
      usingTempFile_ (false),
      nonTempFile_   (NULL)
   {
   if (emptyArgPtr_ == NULL)
      emptyArgPtr_ = new WitMsgArg;

   myMsgMgr_ = new WitMsgMgr (* this);
   }

//------------------------------------------------------------------------------

WitMsgFacility::~WitMsgFacility ()
   {
   delete myMsgMgr_;
   }

//------------------------------------------------------------------------------

FILE * WitMsgFacility::myFile ()
   {
   return myDisp ().myFile ();
   }

//------------------------------------------------------------------------------

const WitString & WitMsgFacility::fileName ()
   {
   return myDisp ().fileName ();
   }

//------------------------------------------------------------------------------

const WitString & WitMsgFacility::accessMode ()
   {
   return myDisp ().accessMode ();
   }

//------------------------------------------------------------------------------

int WitMsgFacility::lineLength ()
   {
   return myMclFacility ().lineLength ();
   }

//------------------------------------------------------------------------------

const char * WitMsgFacility::boolText (bool theValue)
   {
   return myMclFacility ().booleanText (theValue);
   }

//------------------------------------------------------------------------------

bool WitMsgFacility::displayExternalNumber (int theExtNum)
   {
   return extNumMsg (theExtNum)->displayExternalNumber ();
   }

//------------------------------------------------------------------------------

bool WitMsgFacility::stopRunning (int theExtNum)
   {
   return extNumMsg (theExtNum)->stopRunning ();
   }

//------------------------------------------------------------------------------

bool WitMsgFacility::throwErrorExc (int theExtNum)
   {
   return extNumMsg (theExtNum)->throwErrorExc ();
   }

//------------------------------------------------------------------------------

int WitMsgFacility::maxTimesIssued (int theExtNum)
   {
   return extNumMsg (theExtNum)->maxTimesIssued ();
   }

//------------------------------------------------------------------------------

void WitMsgFacility::setAccessMode (const char * theValue)
   {
   myDisp ().setAccessMode (theValue);
   }

//------------------------------------------------------------------------------

void WitMsgFacility::setLineLength (int theValue)
   {
   myMclFacility ().lineLength (theValue);
   }

//------------------------------------------------------------------------------

void WitMsgFacility::setMyFile (FILE * theFile, const char * theName)
   {
   myDisp ().setMyFile (theFile, theName);
   }

//------------------------------------------------------------------------------

void WitMsgFacility::setBoolMsgFrags (
      WitMsgFrag theTrueFrag,
      WitMsgFrag theFalseFrag)
   {
   myMclFacility ().
      booleanMsgFrags (
         theTrueFrag .myMclMsgFrag_,
         theFalseFrag.myMclMsgFrag_);
   }

//------------------------------------------------------------------------------

void WitMsgFacility::setDisplayExternalNumber (int theExtNum, bool theValue)
   {
   extNumMsg (theExtNum)->displayExternalNumber (theValue);
   }

//------------------------------------------------------------------------------

void WitMsgFacility::setStopRunning (int theExtNum, bool theValue)
   {
   stopRunningWasSet_ = true;

   extNumMsg (theExtNum)->stopRunning (theValue);
   }

//------------------------------------------------------------------------------

void WitMsgFacility::setThrowErrorExc (int theExtNum, bool theValue)
   {
   stopRunningWasSet_ = true;

   extNumMsg (theExtNum)->throwErrorExc (theValue);
   }

//------------------------------------------------------------------------------

void WitMsgFacility::setMaxTimesIssued (int theExtNum, int theValue)
   {
   extNumMsg (theExtNum)->maxTimesIssued (theValue);
   }

//------------------------------------------------------------------------------

void WitMsgFacility::setDisplayExternalNumberByLevel (
      int  theExtNum,
      bool theValue)
   {
   myMclFacility ().displayExternalNumber (extNumLevel (theExtNum), theValue);
   }

//------------------------------------------------------------------------------

void WitMsgFacility::setStopRunningByLevel (int theExtNum, bool theValue)
   {
   const MclLevel & theLevel = extNumLevel (theExtNum);

   MclMsgUnitItr theItr (myMclFacility ());
   MclMsg *      theMclMsg;

   stopRunningWasSet_ = true;

   theMclMsg = theItr.nextMsgForChange (theLevel);

   while (theMclMsg != NULL)
      {
      myMsgMgr_->myMsg (theMclMsg)->stopRunning (theValue);

      theMclMsg = theItr.nextMsgForChange (theLevel);
      }
   }

//------------------------------------------------------------------------------

void WitMsgFacility::setThrowErrorExcByLevel (int theExtNum, bool theValue)
   {
   const MclLevel & theLevel = extNumLevel (theExtNum);

   MclMsgUnitItr theItr (myMclFacility ());
   MclMsg *      theMclMsg;

   stopRunningWasSet_ = true;

   theMclMsg = theItr.nextMsgForChange (theLevel);

   while (theMclMsg != NULL)
      {
      myMsgMgr_->myMsg (theMclMsg)->throwErrorExc (theValue);

      theMclMsg = theItr.nextMsgForChange (theLevel);
      }
   }

//------------------------------------------------------------------------------

void WitMsgFacility::setMaxTimesIssuedByLevel (int theExtNum, int theValue)
   {
   myMclFacility ().maxTimesIssued (extNumLevel (theExtNum), theValue);
   }

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

void WitMsgFacility::operator () (
        WitMsgID          theMsgID
      )
   {
   MclArgList theArgList;

   myMsgMgr_->issue (theMsgID, theArgList);
   }

//------------------------------------------------------------------------------

void WitMsgFacility::operator () (
        WitMsgID          theMsgID
      , const WitMsgArg & theMsgArg1
      )
   {
   MclArgList theArgList;

   theArgList
      << theMsgArg1
      ;

   myMsgMgr_->issue (theMsgID, theArgList);
   }

//------------------------------------------------------------------------------

void WitMsgFacility::operator () (
        WitMsgID          theMsgID
      , const WitMsgArg & theMsgArg1
      , const WitMsgArg & theMsgArg2
      )
   {
   MclArgList theArgList;

   theArgList
      << theMsgArg1
      << theMsgArg2
      ;

   myMsgMgr_->issue (theMsgID, theArgList);
   }

//------------------------------------------------------------------------------

void WitMsgFacility::operator () (
        WitMsgID          theMsgID
      , const WitMsgArg & theMsgArg1
      , const WitMsgArg & theMsgArg2
      , const WitMsgArg & theMsgArg3
      )
   {
   MclArgList theArgList;

   theArgList
      << theMsgArg1
      << theMsgArg2
      << theMsgArg3
      ;

   myMsgMgr_->issue (theMsgID, theArgList);
   }

//------------------------------------------------------------------------------

void WitMsgFacility::operator () (
        WitMsgID          theMsgID
      , const WitMsgArg & theMsgArg1
      , const WitMsgArg & theMsgArg2
      , const WitMsgArg & theMsgArg3
      , const WitMsgArg & theMsgArg4
      )
   {
   MclArgList theArgList;

   theArgList
      << theMsgArg1
      << theMsgArg2
      << theMsgArg3
      << theMsgArg4
      ;

   myMsgMgr_->issue (theMsgID, theArgList);
   }

//------------------------------------------------------------------------------

void WitMsgFacility::operator () (
        WitMsgID          theMsgID
      , const WitMsgArg & theMsgArg1
      , const WitMsgArg & theMsgArg2
      , const WitMsgArg & theMsgArg3
      , const WitMsgArg & theMsgArg4
      , const WitMsgArg & theMsgArg5
      )
   {
   MclArgList theArgList;

   theArgList
      << theMsgArg1
      << theMsgArg2
      << theMsgArg3
      << theMsgArg4
      << theMsgArg5
      ;

   myMsgMgr_->issue (theMsgID, theArgList);
   }

//------------------------------------------------------------------------------

void WitMsgFacility::operator () (
        WitMsgID          theMsgID
      , const WitMsgArg & theMsgArg1
      , const WitMsgArg & theMsgArg2
      , const WitMsgArg & theMsgArg3
      , const WitMsgArg & theMsgArg4
      , const WitMsgArg & theMsgArg5
      , const WitMsgArg & theMsgArg6
      , const WitMsgArg & theMsgArg7
      , const WitMsgArg & theMsgArg8
      , const WitMsgArg & theMsgArg9
      , const WitMsgArg & theMsgArg10
      , const WitMsgArg & theMsgArg11
      , const WitMsgArg & theMsgArg12
      , const WitMsgArg & theMsgArg13
      , const WitMsgArg & theMsgArg14
      , const WitMsgArg & theMsgArg15
      )
   {
   MclArgList theArgList;

   theArgList
      << theMsgArg1
      << theMsgArg2
      << theMsgArg3
      << theMsgArg4
      << theMsgArg5
      << theMsgArg6
      << theMsgArg7
      << theMsgArg8
      << theMsgArg9
      << theMsgArg10
      << theMsgArg11
      << theMsgArg12
      << theMsgArg13
      << theMsgArg14
      << theMsgArg15
      ;

   myMsgMgr_->issue (theMsgID, theArgList);
   }

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

void WitMsgFacility::issueByException (
      WitMsgID          theMsgID,
      const WitMsgArg & theMsgArg1,
      const WitMsgArg & theMsgArg2,
      const WitMsgArg & theMsgArg3)
   {
   WitIssueMsgExc theIssueMsgExc (
      theMsgID,
      theMsgArg1,
      theMsgArg2,
      theMsgArg3);

   throw theIssueMsgExc;
   }

//------------------------------------------------------------------------------

void WitMsgFacility::abortIfErrors ()
   {
   if (myMsgMgr_->errorsIssued ())
      (* this) ("inputErrorRepSmsg");
   }

//------------------------------------------------------------------------------

WitMsgFrag WitMsgFacility::myFrag (WitMsgFragID theFragID)
   {
   MclMsgFrag * theMclMsgFrag;

   theMclMsgFrag = myMclFacility ().findMsgFrag (theFragID);

   if (theMclMsgFrag == NULL)
      myMsgFac () ("invalidMsgFragIDFmsg", theFragID);

   return WitMsgFrag (theMclMsgFrag);
   }

//------------------------------------------------------------------------------

const char * WitMsgFacility::fragText (WitMsgFragID theFragID)
   {
   return myFrag (theFragID).myMclMsgFrag_->text ();
   }

//------------------------------------------------------------------------------

bool WitMsgFacility::mayIssue (WitMsgID theMsgID)
   {
   MclMsg * theMclMsg;
   WitMsg * theMsg;

   theMclMsg = myMclFacility ().findMsg (theMsgID);

   nonMsgAssert (theMclMsg != NULL);

   theMsg = myMsgMgr_->myMsg (theMclMsg);

   if (myMsgMgr_->issueAllowed (theMsg))
      {
      if (theMsg->maxTimesIssued () < 0)
         return true;

      if (theMsg->timesIssued () < theMsg->maxTimesIssued ())
         return true;
      }

   return false;
   }

//------------------------------------------------------------------------------

void WitMsgFacility::setMaxTimesIssued (WitMsgID theMsgID, int theValue)
   {
   MclMsg * theMclMsg;

   theMclMsg = myMclFacility ().findMsg (theMsgID);

   nonMsgAssert (theMclMsg != NULL);

   theMclMsg->maxTimesIssued (theValue);
   }

//------------------------------------------------------------------------------

bool WitMsgFacility::isExtNumOfMsg (int theExtNum)
   {
   return (myMsgMgr_->extNumMsg (theExtNum) != NULL);
   }

//------------------------------------------------------------------------------

bool WitMsgFacility::isExtNumOfLevel (int theExtNum)
   {
   return (extNumLevelPtr (theExtNum) != NULL);
   }

//------------------------------------------------------------------------------

void WitMsgFacility::listUnissuedMsgs ()
   {
   MclMsgUnitItr theItr (myMclFacility ());
   MclMsg *      theMclMsg;
   WitMsg *      theMsg;
   int           nUnissued;

   nUnissued = 0;

   theMclMsg = theItr.nextMsg ();

   while (theMclMsg != NULL)
      {
      theMsg = myMsgMgr_->myMsg (theMclMsg);

      if (theMsg->testable ())
         if (theMsg->timesIssued () == 0)
            ++ nUnissued;

      theMclMsg = theItr.nextMsg ();
      }

   if (nUnissued == 0)
      {
      myMsgFac () ("allIssuedMsg");

      return;
      }

   myMsgFac () ("nUnissuedMsg", nUnissued);

   theItr.reset ();

   theMclMsg = theItr.nextMsg ();

   while (theMclMsg != NULL)
      {
      theMsg = myMsgMgr_->myMsg (theMclMsg);

      if (theMsg->testable ())
         if (theMsg->timesIssued () == 0)
            myMsgFac () ("unissuedMsgMsg",
               theMsg->externalNumber (),
               theMsg->id ());

      theMclMsg = theItr.nextMsg ();
      }
   }

//------------------------------------------------------------------------------

bool WitMsgFacility::displayStateChange (
      WitMsgID theMsgID,
      bool     currentMode,
      bool     newMode)
   {
   if (newMode != currentMode)
      myMsgFac () (theMsgID);

   return newMode;
   }

//------------------------------------------------------------------------------

void WitMsgFacility::setCrashOnFatal (bool theValue)
   {
   myMsgMgr_->setCrashOnFatal (theValue);
   }

//------------------------------------------------------------------------------

void WitMsgFacility::divide ()
   {
   bool shortLine;

   shortLine =
      myMclFacility ().findMsg ("shortDividerMsg")->displayExternalNumber ();

   if (shortLine)
      {
      if (mayIssue ("shortDividerMsg"))
         myMsgFac () ("shortDividerMsg");
      }
   else
      {
      if (mayIssue ("longDividerMsg"))
         myMsgFac () ("longDividerMsg");
      }
   }

//------------------------------------------------------------------------------

WitMsg * WitMsgFacility::extNumMsg (int theExtNum)
   {
   WitMsg * theMsg;

   theMsg = myMsgMgr_->extNumMsg (theExtNum);

   nonMsgAssert (theMsg != NULL);

   return theMsg;
   }

//------------------------------------------------------------------------------

const MclLevel & WitMsgFacility::extNumLevel (int theExtNum)
   {
   const MclLevel * theLevelPtr;

   theLevelPtr = extNumLevelPtr (theExtNum);

   nonMsgAssert (theLevelPtr != NULL);

   return * theLevelPtr;
   }

//------------------------------------------------------------------------------

const MclLevel * WitMsgFacility::extNumLevelPtr (int theExtNum)
   {
   return
      (theExtNum == WitINFORMATIONAL_MESSAGES)? & MclLevel::info    ():
      (theExtNum ==       WitWARNING_MESSAGES)? & MclLevel::warning ():
      (theExtNum ==        WitSEVERE_MESSAGES)? & MclLevel::severe  ():
      (theExtNum ==         WitFATAL_MESSAGES)? & MclLevel::fatal   ():
                                                NULL;
   }

//------------------------------------------------------------------------------
// Implementation of class TempMsgFile.
//------------------------------------------------------------------------------

WitTempMsgFile::WitTempMsgFile (
         WitMsgFacility & theMsgFac,
         const char *     theFileName):

      myMsgFac_     (theMsgFac),
      active_       (false),
      prevFile_     (NULL),
      prevFileName_ ()
   {
   FILE * tempFile;

   if (theFileName == NULL)
      return;
         //
         // No filename given; no temporary assignment is needed.

   if (theFileName == myMsgFac ().fileName ())
      return;
         //
         // no temporary assignment is needed.

   active_   = true;
   prevFile_ = myMsgFac ().myFile ();

   prevFileName_ = myMsgFac ().fileName ();

   if (equalCstrings (theFileName, WitSTDOUT))
      {
      myMsgFac ().setMyFile (stdout, theFileName);
      }
   else
      {
      tempFile =
         myMsgFac ().
            mySession ()->
               openFile (theFileName, myMsgFac ().accessMode ().myCstring ());

      myMsgFac ().setMyFile (tempFile, theFileName);
      }
   }

//------------------------------------------------------------------------------

WitTempMsgFile::~WitTempMsgFile ()
   {
   if (active_)
      {
      if (myMsgFac ().myFile () != NULL)
         fclose (myMsgFac ().myFile ());

      myMsgFac ().setMyFile (prevFile_, prevFileName_.myCstring ());
      }
   }

//------------------------------------------------------------------------------
// Implementation of class WitIssueMsgExc.
//------------------------------------------------------------------------------

WitIssueMsgExc::WitIssueMsgExc (
         WitMsgID          theMsgID,
         const WitMsgArg & theMsgArg1,
         const WitMsgArg & theMsgArg2,
         const WitMsgArg & theMsgArg3):

      myMsgID_   (theMsgID),
      myMsgArg1_ (),
      myMsgArg2_ (),
      myMsgArg3_ ()
   {
   myMsgArg1_ = theMsgArg1;
   myMsgArg2_ = theMsgArg2;
   myMsgArg3_ = theMsgArg3;
   }

//------------------------------------------------------------------------------

WitIssueMsgExc::WitIssueMsgExc (const WitIssueMsgExc & theIssueMsgExc):

      myMsgID_   (theIssueMsgExc.myMsgID_),
      myMsgArg1_ (),
      myMsgArg2_ (),
      myMsgArg3_ ()
   {
   myMsgArg1_ = theIssueMsgExc.myMsgArg1_;
   myMsgArg2_ = theIssueMsgExc.myMsgArg2_;
   myMsgArg3_ = theIssueMsgExc.myMsgArg3_;
   }

//------------------------------------------------------------------------------

WitIssueMsgExc::~WitIssueMsgExc ()
   {
   }

//------------------------------------------------------------------------------
// Implementation of class AbortApiExc.
//------------------------------------------------------------------------------

WitAbortApiExc::WitAbortApiExc (WitMsg * theMsg):

      myMsg_ (theMsg)
   {
   }

//------------------------------------------------------------------------------

WitAbortApiExc::WitAbortApiExc (const WitAbortApiExc & theAbortApiExc):

      myMsg_ (theAbortApiExc.myMsg_)
   {
   }

//------------------------------------------------------------------------------

WitAbortApiExc::~WitAbortApiExc ()
   {
   }

//------------------------------------------------------------------------------

int WitAbortApiExc::myMsgNum ()
   {
   return myMsg_->externalNumber ();
   }

//------------------------------------------------------------------------------

bool WitAbortApiExc::stopRunning ()
   {
   return myMsg_->stopRunning ();
   }

//------------------------------------------------------------------------------

bool WitAbortApiExc::throwErrorExc ()
   {
   return myMsg_->throwErrorExc ();
   }
