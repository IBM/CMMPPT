//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "DataWrit.C"
//
// Contains the implementation of the following classes and templates:
//
//    DataWriter
//    VecWriter <ArgVec, ArgScal>
//------------------------------------------------------------------------------

#include <DataWrit.h>
#include <VecWriter.h>
#include <DetOptImpMgr.h>
#include <Timing.h>
#include <Pre.h>
#include <CompMgr.h>
#include <GlobalComp.h>
#include <OptComp.h>
#include <MultiObjMgr.h>
#include <ObjVec.h>
#include <CpxParSpecMgr.h>
#include <PipMgr.h>
#include <StochImpMgr.h>
#include <FlexVec.h>
#include <TempMsgFile.h>
#include <MsgFrag.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class DataWriter.
//------------------------------------------------------------------------------

WitDataWriter::WitDataWriter (WitProblem * theProblem):

      WitProbAssoc    (theProblem),
      curComp_        (NULL),
      buffer_         (),
      keyWordWritten_ (false)
   {
   }

//------------------------------------------------------------------------------

WitDataWriter::~WitDataWriter ()
   {
   }

//------------------------------------------------------------------------------

void WitDataWriter::writeData (const char * fileName)
   {
   writeData (fileName, false);
   }

//------------------------------------------------------------------------------

void WitDataWriter::writeEndCommand ()
   {
   myMsgFac () ("endCommandWdMsg");
   }

//------------------------------------------------------------------------------

void WitDataWriter::writeFirstKeyWord (const char * theKeyWord)
   {
   if (keyWordWritten_)
      myMsgFac () ("firstKeyWordWdMsg",       theKeyWord);
   else
      {
      myMsgFac () ("firstInFileKeyWordWdMsg", theKeyWord);

      keyWordWritten_ = true;
      }
   }

//------------------------------------------------------------------------------

void WitDataWriter::writeKeyWord (const char * theKeyWord)
   {
   myMsgFac () ("keyWordWdMsg", theKeyWord);
   }

//------------------------------------------------------------------------------

void WitDataWriter::writeIntValue (int theInt)
   {
   myMsgFac () ("intWdMsg", theInt);
   }

//------------------------------------------------------------------------------

void WitDataWriter::writeStringValue (const WitString & theString)
   {
   writeCstringValue (theString.myCstring ());
   }

//------------------------------------------------------------------------------

void WitDataWriter::writeCstringValue (const char * theCstring)
   {
   myMsgFac () ("stringWdMsg", withSlashes (theCstring));
   }

//------------------------------------------------------------------------------

void WitDataWriter::writeVector (
      const char *              attrName,
      const WitFlexVec <bool> & theVector,
      bool                      defaultVal)
   {
   WitVecWriter <WitFlexVec <bool>, bool> theVecWriter (myProblem ());

   theVecWriter.
      writeVector (
         attrName,
         theVector,
         defaultVal,
         "boolSingleVectorWdMsg",
         "boolSparseVectorWdMsg",
          "boolDenseVectorWdMsg");
   }

//------------------------------------------------------------------------------

void WitDataWriter::writeVector (
      const char *          attrName,
      const WitTVec <int> & theVector,
      int                   defaultVal)
   {
   WitVecWriter <WitTVec <int>, int> theVecWriter (myProblem ());

   theVecWriter.
      writeVector (
         attrName,
         theVector,
         defaultVal,
         "intSingleVectorWdMsg",
         "intSparseVectorWdMsg",
          "intDenseVectorWdMsg");
   }

//------------------------------------------------------------------------------

void WitDataWriter::writeVector (
      const char *             attrName,
      const WitFlexVec <int> & theVector,
      int                      defaultVal)
   {
   WitVecWriter <WitFlexVec <int>, int> theVecWriter (myProblem ());

   theVecWriter.
      writeVector (
         attrName,
         theVector,
         defaultVal,
         "intSingleVectorWdMsg",
         "intSparseVectorWdMsg",
          "intDenseVectorWdMsg");
   }

//------------------------------------------------------------------------------

void WitDataWriter::writeVector (
      const char *          attrName,
      const WitDblFlexVec & theVector,
      double                defaultVal)
   {
   WitVecWriter <WitDblFlexVec, double> theVecWriter (myProblem ());

   if (myStochImpMgr ()->skipWriteData (attrName, curComp_))
      return;

   theVecWriter.writeDblVector (attrName, theVector, defaultVal);
   }

//------------------------------------------------------------------------------

void WitDataWriter::writeVector (
      const char *              attrName,
      const WitTVec <double> &  theVector,
      double                    defaultVal)
   {
   WitVecWriter <WitTVec <double>, double> theVecWriter (myProblem ());

   theVecWriter.writeDblVector (attrName, theVector, defaultVal);
   }

//------------------------------------------------------------------------------

void WitDataWriter::writeDouble (
      const char * attrName,
      double       theValue,
      double       defaultVal)
   {
   if (theValue != defaultVal)
      writeDouble (attrName, theValue);
   }

//------------------------------------------------------------------------------

void WitDataWriter::writeInt (
      const char * attrName,
      int          theValue,
      int          defaultVal)
   {
   if (theValue != defaultVal)
      writeInt (attrName, theValue);
   }

//------------------------------------------------------------------------------

void WitDataWriter::writeBool (
      const char * attrName,
      bool         theValue,
      bool         defaultVal)
   {
   if (theValue != defaultVal)
      writeBool (attrName, theValue);
   }

//------------------------------------------------------------------------------

void WitDataWriter::writeString (
      const char *      attrName,
      const WitString & theValue,
      const char *      defaultVal)
   {
   if (theValue != defaultVal)
      writeString (attrName, theValue);
   }

//------------------------------------------------------------------------------

void WitDataWriter::writeDouble (const char * attrName, double theValue)
   {
   WitMsgID theMsgID;

   writeFirstKeyWord (attrName);

   theMsgID =
      myProblem ()->myGlobalComp ()->highPrecisionWD ()?
         "doubleHpWdMsg":
         "doubleWdMsg";

   myMsgFac () (theMsgID, theValue);
   }

//------------------------------------------------------------------------------

void WitDataWriter::writeInt (const char * attrName, int theValue)
   {
   writeFirstKeyWord (attrName);

   myMsgFac () ("intWdMsg", theValue);
   }

//------------------------------------------------------------------------------

void WitDataWriter::writeBool (const char * attrName, bool theValue)
   {
   writeFirstKeyWord (attrName);

   myMsgFac () ("boolWdMsg", theValue);
   }

//------------------------------------------------------------------------------

void WitDataWriter::writeString (
      const char *      attrName,
      const WitString & theValue)
   {
   writeFirstKeyWord (attrName);
   writeStringValue  (theValue);
   }

//------------------------------------------------------------------------------

void WitDataWriter::writeObjVec (
      const char *      attrName,
      const WitObjVec & theObjVec,
      double            defaultVal)
   {
   if (not myOptComp ()->multiObjMode ())
      {
      writeVector (attrName, theObjVec.curDblFlexVec (), defaultVal);
      }
   }

//------------------------------------------------------------------------------

void WitDataWriter::writeSolnData (const char * fileName)
   {
   writeData (fileName, true);
   }

//------------------------------------------------------------------------------

void WitDataWriter::writeData (const char * fileName, bool solnData)
   {
   int saveMsgLen;

   myPreprocessor ()->preprocess ();

   WitTimer::enterSection ("extra");

   keyWordWritten_ = false;

   myMsgFac () ("witFileParam",
      (fileName == NULL)?
         myMsgFac ().fileName ().myCstring ():
         fileName);

   WitTempMsgFile theTemp (myMsgFac (), fileName);

   saveMsgLen = myMsgFac ().lineLength ();

   myMsgFac ().setLineLength (3000);
      //
      // Prevents line breaks.

   myMsgFac ().
      setBoolMsgFrags (
         myMsgFac ().myFrag ( "trueWdFrag"),
         myMsgFac ().myFrag ("falseWdFrag"));

   forEachEl (curComp_, myCompMgr ()->allComponents ())
      if (solnData)
         curComp_->writeSolnData ();
      else
         curComp_->writeData ();

   curComp_ = NULL;

   myOptComp ()->myCpxParSpecMgr ()->writeData ();

   myProblem ()->myPipMgr ()->writeShipSeqData ();

   myStochImpMgr ()->writeData ();

   if (myOptComp ()->multiObjMode ())
      {
      myOptComp ()->myMultiObjMgr ()->writeMultiObjVecs ();
      }

   myMsgFac ().
      setBoolMsgFrags (
         myMsgFac ().myFrag ( "trueFrag"),
         myMsgFac ().myFrag ("falseFrag"));

   myMsgFac ().setLineLength (saveMsgLen);

   WitTimer::leaveSection ("extra");
   }

//------------------------------------------------------------------------------

const char * WitDataWriter::withSlashes (const char * theCstring)
   {
   int  stringIndex;
   int  bufferIndex;
   int  stringLength = strlen (theCstring);
   bool specialFound = false;

   for (stringIndex = 0; stringIndex < stringLength; ++ stringIndex)
      if (isSpecial (theCstring[stringIndex]))
         {
         specialFound = true;

         break;
         }

   if (not specialFound)
      return theCstring;
         //
         // No special chars; therefore no "\"s added. (The usual case.)

   if (buffer_.length () < stringLength * 2)
      buffer_.resize (stringLength * 4);
         //
         // buffer_ may need to be twice the size of theCstring, but we resize
         // it to 4 times, to avoid excessive repeated resizing.

   bufferIndex = -1;

   for (stringIndex = 0; stringIndex < stringLength; ++ stringIndex)
      {
      if (isSpecial (theCstring[stringIndex]))
         buffer_[++ bufferIndex] = '\\';

      buffer_[++ bufferIndex] = theCstring[stringIndex];
      }

   buffer_[++ bufferIndex] = '\0';

   return buffer_.myCstring ();
   }

//------------------------------------------------------------------------------
// Implementation of class template VecWriter <ArgVec, ArgScal>.
//------------------------------------------------------------------------------

template <typename ArgVec, typename ArgScal>
      WitVecWriter <ArgVec, ArgScal>::WitVecWriter (WitProblem * theProblem):

      WitProbAssoc (theProblem)
   {
   }

//------------------------------------------------------------------------------

template <typename ArgVec, typename ArgScal>
      WitVecWriter <ArgVec, ArgScal>::~WitVecWriter ()
   {
   }

//------------------------------------------------------------------------------

template <typename ArgVec, typename ArgScal>
      void WitVecWriter <ArgVec, ArgScal>::writeDblVector (
         const char *   attrName,
         const ArgVec & theArgVec,
         ArgScal        defaultVal)
   {
   if (myProblem ()->myGlobalComp ()->highPrecisionWD ())
      writeVector (
         attrName,
         theArgVec,
         defaultVal,
         "doubleSingleVectorHpWdMsg",
         "doubleSparseVectorHpWdMsg",
          "doubleDenseVectorHpWdMsg");
   else
      writeVector (
         attrName,
         theArgVec,
         defaultVal,
         "doubleSingleVectorWdMsg",
         "doubleSparseVectorWdMsg",
          "doubleDenseVectorWdMsg");
   }

//------------------------------------------------------------------------------

template <typename ArgVec, typename ArgScal>
      void WitVecWriter <ArgVec, ArgScal>::writeVector (
         const char *   attrName,
         const ArgVec & theArgVec,
         ArgScal        defaultVal,
         WitMsgID       singleMsgID,
         WitMsgID       sparseMsgID,
         WitMsgID       denseMsgID)
   {
   bool vecWritten;

   if (theArgVec == defaultVal)
      return;

   myProblem ()->myDataWriter ()->writeFirstKeyWord (attrName);

   writeSingle (theArgVec, singleMsgID, vecWritten);

   if (vecWritten)
      return;

   writeSparse (theArgVec, defaultVal, sparseMsgID, vecWritten);

   if (vecWritten)
      return;

   writeDense (theArgVec, denseMsgID);
   }

//------------------------------------------------------------------------------

template <typename ArgVec, typename ArgScal>
      void WitVecWriter <ArgVec, ArgScal>::writeSingle (
         const ArgVec & theArgVec,
         WitMsgID       theMsgID,
         bool &         vecWritten)
   {
   WitPeriod thePer;

   vecWritten = false;

   forEachPeriod (thePer, myProblem ())
      if (theArgVec[thePer] != theArgVec[0])
         return;

   myMsgFac () (theMsgID, theArgVec[0]);

   vecWritten = true;
   }

//------------------------------------------------------------------------------

template <typename ArgVec, typename ArgScal>
      void WitVecWriter <ArgVec, ArgScal>::writeSparse (
            const ArgVec & theArgVec,
            ArgScal        defaultVal,
            WitMsgID       theMsgID,
            bool &         vecWritten)
   {
   WitPeriod thePer;
   int       nNonDefaults; // # elements not at default value.
   bool      firstTime;

   vecWritten   = false;
   nNonDefaults = 0;

   forEachPeriod (thePer, myProblem ())
      if (theArgVec[thePer] != defaultVal)
         {
         nNonDefaults ++;

         if (nNonDefaults >= nPeriods () / 2)
            return;
         }

   myMsgFac () ("startVectorWdMsg", "sparse");

   firstTime = true;

   forEachPeriod (thePer, myProblem ())
      if (theArgVec[thePer] != defaultVal)
         {
         myMsgFac () (theMsgID,
            (firstTime? "": " "),
            thePer,
            theArgVec[thePer]);

         firstTime = false;
         }

   myMsgFac () ("endVectorWdMsg");

   vecWritten = true;
   }

//------------------------------------------------------------------------------

template <typename ArgVec, typename ArgScal>
      void WitVecWriter <ArgVec, ArgScal>::writeDense (
         const ArgVec & theArgVec,
         WitMsgID       theMsgID)
   {
   WitPeriod thePer;

   myMsgFac () ("startVectorWdMsg", "dense");

   forEachPeriod (thePer, myProblem ())
      myMsgFac () (theMsgID,
         ((thePer == 0)? "": " "),
         theArgVec[thePer]);

   myMsgFac () ("endVectorWdMsg");
   }
