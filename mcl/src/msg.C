//==============================================================================
// Message Class Library
//
// (C) Copyright IBM Corp. 1997, 2005  All Rights Reserved
//==============================================================================

//-----------------------------------------------------------------------------
// File msg.C
//
// Contains the implementation of the following classes:
//
// MsgUnit
//    Msg
//    MsgFrag
//-----------------------------------------------------------------------------

#include <mcl/src/msg.h>
#include <mcl/src/arg.h>
#include <mcl/src/facility.h>
#include <mcl/src/level.h>

#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//-----------------------------------------------------------------------------
// Implementation of class MsgUnit.
//-----------------------------------------------------------------------------

MclMsgUnit::~MclMsgUnit ()
   {
   if (! myFacility ().deletingMsgUnits ())
      MclFacility::calledByClientError ("MclMsgUnit::~MclMsgUnit");
   }

//-----------------------------------------------------------------------------

const char * MclMsgUnit::id () const
   {
   return id_.c_str();
   }

//-----------------------------------------------------------------------------

const char * MclMsgUnit::text () const
   {
   return text_.c_str();
   }

//-----------------------------------------------------------------------------

MclMsgUnit::MclMsgUnit (
         MclFacility & theFacility,
         const char *  idVal,
         const char *  textVal):

      myFacility_  (theFacility),
      isInserting_ (FALSE)
   {
   MclFacility::forbidNullArg (idVal,   "idVal",   "MclMsgUnit::MclMsgUnit");
   MclFacility::forbidNullArg (textVal, "textVal", "MclMsgUnit::MclMsgUnit");

   id_   = idVal;
   text_ = textVal;

   forbidBlankId ();
   forbidDupId   ();
   }

//-----------------------------------------------------------------------------

MclMsg * MclMsgUnit::asaMsg ()
   {
   return NULL;
   }

//-----------------------------------------------------------------------------

MclMsgFrag * MclMsgUnit::asaMsgFrag ()
   {
   return NULL;
   }

//-----------------------------------------------------------------------------

void MclMsgUnit::clientError (const char * text) const
   {
   std::cerr <<
      "\n\n"
      "MCL CLIENT ERROR:\n\n"
      "" << text << "\n\n"
      "   Message ID: \"" << id () << "\"\n";

   MclFacility::abortMcl ();
   }

//-----------------------------------------------------------------------------

void MclMsgUnit::insertSelf ()
   {
   isInserting_ = TRUE;

   myFacility ().insert (this);

   isInserting_ = FALSE;
   }

//-----------------------------------------------------------------------------

void MclMsgUnit::forbidBlankId () const
   {
   std::string emptyStr (id_.length (), ' ');

   if (id_ == emptyStr)
      clientError (
         "   Attempt to create a message unit with message ID all blanks.");
   }

//-----------------------------------------------------------------------------

void MclMsgUnit::forbidDupId () const
   {
   MclMsgUnit * dupIdMsgUnit = myFacility ().findMsgUnit (id ());

   if (dupIdMsgUnit != NULL)
      clientError (
         "   Attempt to create a message unit whose message ID duplicates "
            "an existing message unit.");
   }

//-----------------------------------------------------------------------------
// Implementation of class Msg.
//-----------------------------------------------------------------------------

MclMsg::MclMsg (
         MclFacility &    theFacility,
         const char *     msgId,
         const MclLevel & theLevel,
         const char *     textVal,
         int              externalNumber):

      MclMsgUnit             (theFacility, msgId, textVal),

      externalNumber_        (externalNumber),
      maxTimesIssued_        (-1),
      timesIssued_           (0),
      preceedingLineFeeds_   (0),
      trailingLineFeeds_     (1),
      vectorIndent_          (0),
      displayExternalNumber_ (TRUE),
      allowedToChange_       (TRUE),
      mark_                  (FALSE),
      myLevel_               (theLevel)
   {
   checkExtNumRange ();
   forbidDupExtNum ();

   insertSelf ();
   }

MclMsg::~MclMsg ()
   {
   }

void MclMsg::issue (const MclArgList& al)
   {
   timesIssued_ ++;
   mark_ = TRUE;
   
   if ((maxTimesIssued_ < 0) || (timesIssued_ <= maxTimesIssued_))
      {
      std::string theFinalText;
   
      getFinalText (theFinalText, al);

      myFacility ().output(theFinalText.c_str());

      if (myLevel () >= myFacility ().minErrOutLevel ())
         myFacility ().outputError(theFinalText.c_str());
      }

   postIssue ();
   }

MclMsg * MclMsg::maxTimesIssued (int theValue)
   {
   return set (maxTimesIssued_, "maxTimesIssued", theValue);
   }
  
MclMsg * MclMsg::preceedingLineFeeds (int theValue)
   {
   return setNonNeg (preceedingLineFeeds_, "preceedingLineFeeds", theValue);
   }

MclMsg * MclMsg::trailingLineFeeds (int theValue)
   {
   return setNonNeg (trailingLineFeeds_, "trailingLineFeeds", theValue);
   }

MclMsg * MclMsg::vectorIndent (int theValue)
   {
   return setNonNeg (vectorIndent_, "vectorIndent", theValue);
   }

MclMsg * MclMsg::displayExternalNumber (MclBoolean theValue)
   {
   return set (displayExternalNumber_, "displayExternalNumber", theValue);
   }

MclMsg * MclMsg::allowedToChange (MclBoolean theValue)
   {
   return set (allowedToChange_, "allowedToChange", theValue);
   }

MclMsg * MclMsg::asaMsg ()
   {
   return this;
   }

void MclMsg::postIssue ()
   {
   if (myLevel () < MclLevel::error ())
      return;

   std::cerr <<
      "\n\n"
      "The above message indicates an error.\n"
      "Program aborted." << std::endl;

   exit (myLevel ().severity ());
   }

void MclMsg::checkExtNumRange () const
   {
   if ((externalNumber_ >= 0) && (externalNumber_ < 10000))
      return;

   std::cerr <<
      "\n\n"
      "MCL CLIENT ERROR:\n\n"
      "   The externalNumber of a message is out of range.\n\n"
      "   Message ID:     \"" << id () << "\"\n"
      "   externalNumber: " << externalNumber_ << "\n"
      "   Valid range:    0 <= externalNumber < 10000\n";

   MclFacility::abortMcl ();
   }

MclMsg * MclMsg::set (
      int &        theAttr,
      const char * attrName,
      int          theValue)
   {
   if (! allowedToChange_)
      {
      std::cerr <<
         "\n\n"
         "MCL CLIENT ERROR:\n\n"
         "   An attempt was made to change an attribute of a message whose\n"
         "   allowToChange attribute is FALSE.\n\n"
         "   Message ID:     \"" << id ()           << "\"\n"
         "   externalNumber: "   << externalNumber_ << "\n"
         "   Attribute:      "   << attrName        << "\n";

      MclFacility::abortMcl ();
      }

   theAttr = theValue;

   return this;
   }

MclMsg * MclMsg::setNonNeg (
      int &        theAttr,
      const char * attrName,
      int          theValue)
   {
   if (! (theValue >= 0))
      {
      std::cerr <<
         "\n\n"
         "MCL CLIENT ERROR:\n\n"
         "   An attempt was made to assign a negative value to a nonnegative\n"
         "   message attribute.\n\n"
         "   Message ID:     \"" << id ()           << "\"\n"
         "   externalNumber: "   << externalNumber_ << "\n"
         "   Attribute:      "   << attrName        << "\n"
         "   Value:          "   << theValue        << "\n";

      MclFacility::abortMcl ();
      }

   return set (theAttr, attrName, theValue);
   }

void MclMsg::forbidDupExtNum () const
   {
   MclMsg * dupExtNumMsg = myFacility ().findMsg (externalNumber_);

   if (dupExtNumMsg == NULL)
      return;

   std::cerr <<
      "\n\n"
      "MCL CLIENT ERROR:\n\n"
      "   Attempt to create a message whose external number duplicates\n"
      "   an existing message.\n\n"
      "   Current  Message ID: \"" << id ()               << "\"\n"
      "   Existing Message ID: \"" << dupExtNumMsg->id () << "\"\n"
      "   External Number:     "   << externalNumber_     << "\n";

   MclFacility::abortMcl ();
   }

void MclMsg::getFinalText (std::string & finalText, const MclArgList & al) const
   {
   int       maxArgNum = 0;
   std::string formattedArg;
   size_t idx, start = 0;
   finalText = text ();

   while ((idx = finalText.find('%', start)) != std::string::npos)
     {
      if (finalText[idx+1] == '%')
        {
          finalText.erase(idx+1, 1);
          start = idx + 1;
          continue;
        }
      // The old validateCurArgFmt used to simply validate the format text.
      // The new validateCurArgFmt does that, but it also sets 
      // posPercent, number, posv and posDollar.
      /*validateCurArgFmt(finalText, start);  
      int argNum = 0;

      size_t posPercent = indexInCurArgFmt("%", finalText, start);
      size_t posDollar = indexInCurArgFmt("$", finalText, start);
      
      sscanf (& finalText[posPercent + 1], "%d", & argNum);*/
      int argNum = 0;
      size_t posPercent, posv, posDollar, posFmt;
      posPercent = posv = posDollar = posFmt = std::string::npos;

      validateCurArgFmt(finalText, start, posPercent, argNum, posv, posDollar, posFmt);

      validateArgNum (argNum, al);
      if (argNum > maxArgNum)
         maxArgNum = argNum;

      MclBoolean reqVector = (posv != std::string::npos);

      if (posFmt == std::string::npos)
         missingFormatFlag (&finalText[ posPercent ]);

	  int fmtLen = posFmt - posDollar;

	  std::string argFmt = finalText.substr(posDollar + 1, fmtLen);

      std::string fmt("%");

      fmt += argFmt;

      al[argNum].format (formattedArg, fmt, reqVector, argNum, this);

      int lenReplaced = posFmt - posPercent + 1;

      finalText.replace(posPercent, lenReplaced, formattedArg);

      start=posPercent + formattedArg.length();
      }

   validateMaxArgNum (maxArgNum, al);
   procForAttrs (finalText);
   procLine(finalText);
   }

void MclMsg::validateArgNum (
      size_t             argNum,
      const MclArgList & theArgList)
      const
   {
   if (argNum < 1)
     {
     std::cerr <<
        "\n\n"
        "MCL CLIENT ERROR:\n\n"
        "   A message was issued whose text contains an invalid argument "
           "number.\n\n"
        "   Message ID: \"" << id ()  << "\"\n"
        "   Argument #: "   << argNum << "\n";

     MclFacility::abortMcl ();
     }

   if (argNum > theArgList.length ())
      {
      std::cerr <<
         "\n\n"
         "MCL CLIENT ERROR:\n\n"
         "   A message was issued with too few arguments.\n\n"
         "   Message ID:         \"" << id ()                << "\"\n"
         "   Argument # in text: "   << argNum               << "\n"
         "   # arguments passed: "   << theArgList.length () << "\n";

      MclFacility::abortMcl ();
      }
   }

void MclMsg::validateMaxArgNum (
      int                maxArgNum,
      const MclArgList & theArgList)
      const
   {
   int nArgs = theArgList.length ();

   if (maxArgNum < nArgs)
     {
     std::cerr <<
        "\n\n"
        "MCL CLIENT ERROR:\n\n"
        "   A message was issued with too many arguments.\n\n"
        "   Message ID:                \"" << id ()     << "\"\n"
        "   Maximum argument # in text: "  << maxArgNum << "\n"
        "   # arguments passed:         "  << nArgs     << "\n";

     MclFacility::abortMcl ();
     }
   }

void MclMsg::missingFormatFlag (const char * argSpec ) const
   {
   std::cerr <<
      "\n\n"
      "MCL CLIENT ERROR:\n\n"
      "   A message was issued whose text includes an argument specification\n"
      "   without a format flag.\n\n"
      "   Message ID:             \"" << id ()   << "\"\n"
      "   Argument specification: \"" << argSpec << "\"\n";

   MclFacility::abortMcl ();
   }

void MclMsg::procForAttrs (std::string & finalText) const
{
  if (displayExternalNumber_)
    {
      std::string prefix(myFacility().prefix());
      char s[] = "0000";
      int i = 3;
      int ext = externalNumber_;
      while (ext)
        {
          s[i--] = ext % 10 + '0';
          ext /= 10;
        }
      prefix.append(s);
      std::string levelFlag;
	  levelFlag += myLevel().flag();
      levelFlag += ' ';
      prefix.append(levelFlag);
      finalText = prefix + finalText;

      size_t start = 0;
      size_t idx = finalText.find("\n", start);
      while(idx != std::string::npos && idx != finalText.length() - 1)
        {
          finalText.insert(idx+1, "         ");
          start = idx + 10;
          idx = finalText.find("\n", start);
        }
    }
  if (preceedingLineFeeds_)
    {
      std::string plf(preceedingLineFeeds_, '\n');
      finalText = plf + finalText;
    }
  if (trailingLineFeeds_)
    {
      std::string tlf( trailingLineFeeds_, '\n');
      finalText = finalText + tlf;
    }
}
// Note that the number argument in real life should be a size_t,
// but this provides some warning validation in the caller
void  MclMsg::validateCurArgFmt (const std::string & finalText, size_t start, size_t& posPercent, int& number, size_t& posv, size_t& posDollar, size_t& posFmt ) const
{
    posPercent = posDollar = posv = posFmt = std::string::npos;

	size_t i = posPercent = finalText.find('%', start);
    bool validArgFmt = false;
	if( i < finalText.length() ) // if % was found
        if( isdigit( finalText[++i] ) ) // next MUST be a digit
        {
	  const char * p = finalText.c_str() + i;
	    i++;
            sscanf( p, "%d", &number );
            while( isdigit( finalText[i] ) ) // more digits OK.
                i++;
            if( finalText[i] =='v' ) // v is opt.
                posv = i++;
            if( finalText[i] =='$' ) // MUST be a $
            {
                posDollar = i;
                validArgFmt = true;
            }
            posFmt = finalText.find_first_of( "abcdefghijklmnopqrstuvwxy", i+1 );
        }

  if (!validArgFmt)
    {
    std::cerr <<
       "\n\n"
       "MCL CLIENT ERROR:\n\n"
       "   A message was issued whose text includes "
          "an invalid argument specification.\n"
       "   The argument specification must be a '%', "
           "followed by a sequence of digits,\n"
       "   followed by an optional 'v' followed by a '$'.\n\n"
       "   Message ID:   \"" << id ()   << "\"\n"
       "   Message Text:\n\n"
       "\"" << text () << "\"\n";
 
    MclFacility::abortMcl ();
    }
}

void MclMsg::procLine (std::string & finalText) const
{
  size_t lineLength = myFacility().lineLength();
  MclBoolean flagBreak = FALSE;
  size_t start = 0;
  size_t idx;
  size_t txtLen = finalText.length();
  MclBoolean addedLF = FALSE;

  if (txtLen == 0)
     return;

  if (finalText[txtLen - 1] != '\n')
     {
     finalText.append("\n");

     addedLF = TRUE;
     }

  while ((idx = finalText.find("\n", start)) != std::string::npos )
    {
      size_t len = idx - start;
      size_t blankIdx;
       
      if (len > lineLength)
        {
          std::string str = finalText.substr(start, lineLength);
          blankIdx = str.find_last_of(' ');

          std::string lineStart ("\n");

          if (displayExternalNumber_)
              lineStart += "         ";

          MclBoolean breakWord =
             blankIdx == std::string::npos ||
             (displayExternalNumber_ && blankIdx < 9);

          if (breakWord)
             str.insert (lineLength, lineStart);
          else
             {
             str.erase (blankIdx, 1);
             str.insert (blankIdx, lineStart);
             }

          finalText.replace(start, lineLength, str);
          flagBreak = TRUE;
        }
      if (flagBreak)
        {
          flagBreak = FALSE;
          start = start + blankIdx + 2;
        }
      else
        start = idx + 1;
    }

  if (addedLF)
     {
     assert (finalText [finalText.length () - 1] == '\n');

     finalText.erase (finalText.length () - 1);
     }
}

//-----------------------------------------------------------------------------
// Implementation of class MsgFrag.
//-----------------------------------------------------------------------------

MclMsgFrag::MclMsgFrag (
         MclFacility & theFacility,
         const char *  idVal,
         const char *  textVal):

      MclMsgUnit (theFacility, idVal, textVal)
   {
   insertSelf ();
   }

//-----------------------------------------------------------------------------

MclMsgFrag::~MclMsgFrag ()
   {
   }

//-----------------------------------------------------------------------------

MclMsgFrag * MclMsgFrag::asaMsgFrag ()
   {
   return this;
   }
