//==============================================================================
// Message Class Library
//
// (C) Copyright IBM Corp. 1997, 2005  All Rights Reserved
//==============================================================================

//-----------------------------------------------------------------------------
// File facility.C
//
// Contains the implementation of the following classes:
//
// Facility
// MsgUnitItr
//-----------------------------------------------------------------------------

#include <mcl/src/facility.h>
#include <mcl/src/arg.h>
#include <mcl/src/level.h>
#include <mcl/src/msg.h>
#include <mcl/src/output.h>
#include <mcl/src/set.h>

#include <assert.h>

#include <stdlib.h>

#include <iostream>

//-----------------------------------------------------------------------------

const char * const MclFacility::proprietaryStmt_ =
   "\n"
   "=======================================================\n"
   "Licensed Materials - Property of IBM\n"
   "\n"
   "Message Class Library\n"
   "\n"
   "(C) Copyright IBM Corp. 1997, 2005  All Rights Reserved\n"
   "\n"
   "US Government Users Restricted Rights -\n"
   "Use, duplication or disclosure restricted by\n"
   "GSA ADP Schedule Contract with IBM Corp.\n"
   "=======================================================\n";

//-----------------------------------------------------------------------------

MclFacility::MclFacility (
         const char* prefix,
         MclOutputDisplayer& theOutputDisplayer):
  
      lineLength_        (80),
      myMsgUnitSet_      (* new MclMsgUnitSet),
      myOutDisp_         (theOutputDisplayer),
      trueText_          ("TRUE"),
      falseText_         ("FALSE"),
      minErrOutLevelPtr_ (NULL),
      deletingMsgUnits_  (FALSE)
   {
   MclArg  ::setUpClass ();
   MclLevel::setUpClass ();

   forbidNullArg (prefix, "prefix", "MclFacility::MclFacility");

   prefix_ = prefix;

   if (prefix_.length () != 3)
      {
      printf("\n\nPrefix to a msg should be 3 characters.\n\n");

      abortMcl ();
      }

   // This was performed by the toUpper method of std::string.
   // It was used only here, so the best thing was to code it 
   // with a std::string;
   for( size_t i = 0; i < prefix_.length(); i++ )
	   prefix_[i] = toupper(prefix_[i]);

   minErrOutLevel (MclLevel::warning ());
   }

MclFacility::~MclFacility()
   {
   deletingMsgUnits_ = TRUE;

   delete & myMsgUnitSet_;

   deletingMsgUnits_ = FALSE;
   }

void MclFacility::insert (MclMsgUnit * msgUnit)
   {
   if (! msgUnit->isInserting ())
      calledByClientError ("MclFacility::insert");

   assert (& (msgUnit->myFacility ()) == this);

   myMsgUnitSet_.insert (msgUnit);
   }

void MclFacility::operator() (const char * msgId, const MclArgList& al)
   {
   MclMsg * theMsg = findMsg (msgId);

   if (theMsg == NULL)
      {
      std::cerr <<
         "\n\n"
         "MCL CLIENT ERROR:\n\n"
         "   MclFacility::operator() was called for a message ID\n"
         "   that does not match any message in the facility.\n\n"
         "   Message ID: \"" << msgId << "\"\n";

      abortMcl ();
      }

   theMsg->issue (al);
   }

void MclFacility::operator () (const char * msgId)
   {
   MclArgList emptyArgList;

   (* this) (msgId, emptyArgList);
   }

const char * MclFacility::prefix () const
   {
   return prefix_.c_str();
   }

int MclFacility::lineLength() const
   {
   return lineLength_;
   }

void MclFacility::minErrOutLevel (const MclLevel & theLevel)
   {
   minErrOutLevelPtr_ = & theLevel;
   }

MclBoolean MclFacility::deletingMsgUnits () const
   {
   return deletingMsgUnits_;
   }

const char * MclFacility::booleanText (MclBoolean theValue) const
   {
   return
      theValue?
         trueText_.c_str():
         falseText_.c_str();
   }

MclMsgUnit * MclFacility::findMsgUnit (const char * theId)
   {
   forbidNullArg (theId, "theId", "MclFacility::findMsgUnit");

   return myMsgUnitSet_.findValue (theId);
   }

MclMsg * MclFacility::findMsg (const char * theId)
   {
   MclMsgUnit * theMsgUnit = findMsgUnit (theId);

   return
      (theMsgUnit == NULL)?
         NULL:
         theMsgUnit->asaMsg ();
   }

MclMsg * MclFacility::findMsg (int theExtNumber)
   {
   MclMsgUnitItr theItr (* this);
   MclMsg *      theMsg;

   while (theMsg = theItr.nextMsg ())
      if (theMsg->externalNumber () == theExtNumber)
         return theMsg;

   return NULL;
   }

MclMsgFrag * MclFacility::findMsgFrag (const char * theId)
   {
   MclMsgUnit * theMsgUnit = findMsgUnit (theId);

   return
      (theMsgUnit == NULL)?
         NULL:
         theMsgUnit->asaMsgFrag ();
   }

int MclFacility::maxSeverityOfMsgsIssued()
{
  MclMsgUnitItr theItr (* this);
  MclMsg *      theMsg;
  int max = 0;

   while (theMsg = theItr.nextMsg ())
      if (theMsg->mark())
        {
          int level = (theMsg->myLevel ()).severity ();
          if (level > max)
            {
              max = level;
            }
        }
  return max;
}

void MclFacility::output (const char * finalText)
   {
   myOutDisp_.output (finalText);
   }

void MclFacility::outputError (const char * finalText)
   {
   myOutDisp_.outputError (finalText);
   }

void MclFacility::forbidNullArg (
      const void * thePtr,
      const char * argName,
      const char * funcName)
   {
   if (thePtr != NULL)
      return;

   std::cerr <<
      "\n\n"
      "MCL CLIENT ERROR:\n\n"
      "   A null pointer was passed as a function argument.\n\n"
      "   Function:  " << funcName << "\n"
      "   Argument:  " << argName  << "\n";

   abortMcl ();
   }

void MclFacility::abortMcl ()
   {
   std::cerr <<
      "\n"
      "Program aborted by MCL.\n\n";

   exit (1);
   }

void MclFacility::calledByClientError (const char * funcName)
   {
   std::cerr <<
      "\n\n"
      "MCL CLIENT ERROR:\n\n"
      "   The function " << funcName << " was called by the client.\n";

   MclFacility::abortMcl ();
   }

void MclFacility::booleanMsgFrags (
      const MclMsgFrag *  trueFrag,
      const MclMsgFrag * falseFrag)
   {
   forbidNullArg ( trueFrag,  "trueFrag", "MclFacility::booleanMsgFrags");
   forbidNullArg (falseFrag, "falseFrag", "MclFacility::booleanMsgFrags");

    trueText_ =  trueFrag->text ();
   falseText_ = falseFrag->text ();
   }

void MclFacility::lineLength (int theValue)
   {
   if (theValue < 20)
      {
      std::cerr <<
         "\n\n"
         "MCL CLIENT ERROR:\n\n"
         "   Attempt to set lineLength to a value < 20.\n"
         "   Value:  " << theValue << "\n";

      abortMcl ();
      }

   lineLength_ = theValue;
   }

void MclFacility::maxTimesIssued (const MclLevel & theLevel, int theValue)
   {
   MclMsgUnitItr theItr (* this);
   MclMsg *      theMsg;

   while (theMsg = theItr.nextMsgForChange (theLevel))
      theMsg->maxTimesIssued (theValue);
   }

void MclFacility::displayExternalNumber (
      const MclLevel & theLevel,
      MclBoolean       theValue)
   {
   MclMsgUnitItr theItr (* this);
   MclMsg *      theMsg;

   while (theMsg = theItr.nextMsgForChange (theLevel))
      theMsg->displayExternalNumber (theValue);
   
   }

//---------------------------------------------------------------------------
// Implementation of class MsgUnitItr.
//---------------------------------------------------------------------------

MclMsgUnitItr::MclMsgUnitItr (MclFacility & theFacility) : 

      myItr_         (),
      myMsgUnitSet_  (theFacility.myMsgUnitSet_),
      iteratorValid_ (false)
   {
   }

//---------------------------------------------------------------------------

MclMsgUnitItr::~MclMsgUnitItr ()
   {
   }

//---------------------------------------------------------------------------

MclMsg * MclMsgUnitItr::nextMsgForChange (const MclLevel & theLevel)
   {
   MclMsg * theMsg;

   while (theMsg = nextMsg ())
      if (theMsg->myLevel () == theLevel)
         if (theMsg->allowedToChange ())
            return theMsg;

   return NULL;
   }

//---------------------------------------------------------------------------

MclMsg * MclMsgUnitItr::nextMsg ()
   {
   MclMsgUnit * theMsgUnit;
   MclMsg     * theMsg;

   while (theMsgUnit = nextMsgUnit ())
      {
      theMsg = theMsgUnit->asaMsg ();

      if (theMsg != NULL)
         return theMsg;
      }

   return NULL;
   }

//---------------------------------------------------------------------------

MclMsgFrag * MclMsgUnitItr::nextMsgFrag ()
   {
   MclMsgUnit * theMsgUnit;
   MclMsgFrag * theMsgFrag;

   while (theMsgUnit = nextMsgUnit ())
      {
      theMsgFrag = theMsgUnit->asaMsgFrag ();

      if (theMsgFrag != NULL)
         return theMsgFrag;
      }

   return NULL;
   }

//---------------------------------------------------------------------------

MclMsgUnit * MclMsgUnitItr::nextMsgUnit ()
   {
   if (! iteratorValid_)
      {
      myItr_         = myMsgUnitSet_.myMsgUnitsByNum_.begin ();

      iteratorValid_ = true;
      }
   else
      myItr_ ++;

   return
      (myItr_ != myMsgUnitSet_.myMsgUnitsByNum_.end ())?
         myItr_->second:
         NULL;
   }

//---------------------------------------------------------------------------

void MclMsgUnitItr::reset ()
   {
   iteratorValid_ = false; 
      //
      // this will trigger getting a fresh iterator next time next gets called
   }
