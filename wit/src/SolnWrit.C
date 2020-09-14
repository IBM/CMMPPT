//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "SolnWrit.C"
//
// Contains the implementation of class SolnWriter.
//------------------------------------------------------------------------------

#include <SolnWrit.h>
#include <Pre.h>
#include <GlobalComp.h>
#include <Part.h>
#include <Operation.h>
#include <BomEntry.h>
#include <SubEntry.h>
#include <Demand.h>
#include <ApiMgr.h>
#include <Timing.h>
#include <DetOptImpMgr.h>
#include <CompMgr.h>
#include <TempMsgFile.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------

#define REQ_EPSILON   0.0005  /* Values must exceed this amount in order     */
                              /* to appear in the requirements schedule.     */

#define PROD_EPSILON  0.0005  /* Values must exceed this amount in order     */
                              /* to appear in the production schedule.       */

#define SHIP_EPSILON  0.05    /* Values must exceed this amount in order     */
                              /* to appear in the shipment schedule.         */

#define LONG_MESSAGE_LINES 1000 /* Message length for witWriteProdSched,     */
                                /* witWriteShipSched, & witWriteReqSched.    */

//------------------------------------------------------------------------------

WitSolnWriter::WitSolnWriter (WitProblem * theProblem):
      WitProbAssoc (theProblem)
   {
   }

//------------------------------------------------------------------------------

WitSolnWriter::~WitSolnWriter ()
   {
   }

//------------------------------------------------------------------------------

void WitSolnWriter::writeCriticalList (
      const char *  filename,
      WitFileFormat fileFormat,
      int           maxListLength)
   {
   WitPartPerStack critList (myProblem ());
   const char *    theMsgID;
   WitPart *       thePart;
   WitPeriod       thePer;

   WitTimer::enterSection ("default");

   WitString quotedPartName (100);

   myMsgFac () (
      "witFileParam",
      (filename == NULL)?
         myMsgFac ().fileName ().myCstring ():
         filename);

   myMsgFac () ("fileFormatMsg", fileFormat );

   if      (fileFormat == WitBSV)
      theMsgID = "criticalListMsg";
   else if (fileFormat == WitCSV)
      theMsgID = "criticalListCsvMsg";
   else
      myMsgFac () ("unknownFileFormatSmsg", fileFormat);

   myPreprocessor ()->preprocess ();

   WitTempMsgFile theTemp (myMsgFac (), filename);

   critList.revCopyFrom (myGlobalComp ()->revCritList ());

   while (critList.pop (thePart, thePer))
      {
      myMsgFac () (theMsgID,
         quoteCstring (thePart->partName (), quotedPartName),
         thePer);
      }

   WitTimer::leaveSection ("default");
   }

//------------------------------------------------------------------------------

void WitSolnWriter::writeReqSched (
      const char *  filename,
      WitFileFormat fileFormat)
   {
   WitPart *    thePart;
   WitPeriod    t;
   int          saveMsgLen;
   const char * theMsgID;
   WitString    quotedPartName (100);

   WitTimer::enterSection ("default");

   myMsgFac () ("witFileParam",
      (filename == NULL)?
         myMsgFac ().fileName ().myCstring ():
         filename);

   myMsgFac () ("fileFormatMsg", fileFormat );

   if      (fileFormat == WitBSV)
      theMsgID = "reqSchedDataMsg";

   else if (fileFormat == WitCSV)
      theMsgID = "reqSchedDataCsvMsg";

   else
      myMsgFac () ("unknownFileFormatSmsg", fileFormat);

   myPreprocessor ()->preprocess ();

   WitTempMsgFile theTemp (myMsgFac (), filename);

   saveMsgLen = myMsgFac ().lineLength ();

   myMsgFac ().setLineLength (LONG_MESSAGE_LINES);

   forEachPart (thePart, myProblem ())
      forEachPeriod (t, myProblem ())
         if (fabs ((thePart->reqVol ())[t]) > REQ_EPSILON)
            myMsgFac () (theMsgID,
               quoteCstring (thePart->partName (), quotedPartName),
               t,
               (thePart->reqVol ())[t]);

   myMsgFac ().setLineLength (saveMsgLen);

   WitTimer::leaveSection ("default");
   }

//------------------------------------------------------------------------------

void  WitSolnWriter::writeExecSched (
   const char * const filename,
   const WitFileFormat fileFormat )
   {
   WitOperation * theOpn;
   WitPeriod      t;
   WitBomEntry *  theBomEnt;
   WitSubEntry *  theSub;
   int            saveMsgLen;
   const char *   theMsgID;
   char           formattedProdVol[100];
   char           formattedSubVol [100];
   WitString      quotedOperationName    (100);
   WitString      quotedReplacedPartName (100);
   WitString      quotedConsumedPartName (100);

   WitTimer::enterSection ("default");

   myMsgFac () ("witFileParam",
      (filename == NULL)?
         myMsgFac ().fileName ().myCstring ():
         filename);

   myMsgFac () ("fileFormatMsg", fileFormat );

   if      (fileFormat == WitBSV)
      theMsgID = "prodSchedDataMsg";
   else if (fileFormat == WitCSV)
      theMsgID = "prodSchedDataCsvMsg";
   else
      myMsgFac () ("unknownFileFormatSmsg", fileFormat);

   myPreprocessor ()->preprocess ();

   WitTempMsgFile theTemp (myMsgFac (), filename);

   saveMsgLen = myMsgFac ().lineLength ();

   myMsgFac ().setLineLength (LONG_MESSAGE_LINES);

   forEachOperation (theOpn, myProblem ())
      forEachPeriod (t, myProblem ())
         if (fabs (theOpn->execVol ()[t]) > PROD_EPSILON)
            myMsgFac ()(theMsgID,
               quoteCstring (
                  theOpn->operationName (),
                  quotedOperationName),
               t,
               formattedDouble (theOpn->execVol ()[t], formattedProdVol));

   if (myCompMgr ()->hasSubEntries ())
      {
      if (fileFormat == WitBSV)
         theMsgID = "subSchedDataMsg";
      else
         theMsgID = "subSchedDataCsvMsg";

      myMsgFac () ("subSchedHeadingMsg");

      forEachBomEntry (theBomEnt, myProblem ())
         if (not theBomEnt->mySubEntries ().isEmpty ())
            forEachPeriod (t, myProblem ())
               forEachEl (theSub, theBomEnt->mySubEntries ())
                  if (theSub->inEffect (t))
                     if (fabs (theSub->subVol ()[t]) > PROD_EPSILON)
                        {
                        myMsgFac () (theMsgID,
                           quoteCstring (
                              theSub->myOperationName (),
                              quotedOperationName),
                           quoteCstring (
                              theBomEnt->myPartName (),
                              quotedReplacedPartName),
                           theBomEnt->localIndex (),
                           t,
                           quoteCstring (
                              theSub->myPartName (),
                              quotedConsumedPartName),
                           theSub->localIndex (),
                           formattedDouble (
                              theSub->subVol ()[t],
                              formattedSubVol));
                        }
      }

   myMsgFac ().setLineLength (saveMsgLen);

   WitTimer::leaveSection ("default");
   }

//------------------------------------------------------------------------------

void WitSolnWriter::writeShipSched (
   const char * const filename,
   const WitFileFormat fileFormat )
   {
   WitDemand *    theDemand;
   WitPeriod      t;
   int            saveMsgLen;
   const char *   theMsgID;
   char           formattedShipVol[100];
   WitString      quotedPartName   (100);
   WitString      quotedDemandName (100);

   WitTimer::enterSection ("default");

   myMsgFac () ("witFileParam",
      (filename == NULL)?
         myMsgFac ().fileName ().myCstring ():
         filename);

   myMsgFac () ("fileFormatMsg", fileFormat);

   if      (fileFormat == WitBSV)
      theMsgID = "shipSchedDataMsg";

   else if (fileFormat == WitCSV)
      theMsgID = "shipSchedDataCsvMsg";

   else
      myMsgFac () ("unknownFileFormatSmsg", fileFormat);

   myPreprocessor ()->preprocess ();

   WitTempMsgFile theTemp (myMsgFac (), filename);

   saveMsgLen = myMsgFac ().lineLength ();

   myMsgFac ().setLineLength (LONG_MESSAGE_LINES);

   forEachDemand (theDemand, myProblem ())
      forEachPeriod (t, myProblem ())
         if (fabs (theDemand->shipVol ()[t]) > SHIP_EPSILON)
            {
            myMsgFac () (theMsgID,
               quoteCstring (theDemand->demandedPartName (), quotedPartName),
               quoteCstring (theDemand->demandName (),       quotedDemandName),
               t,
               formattedDouble (theDemand->shipVol ()[t], formattedShipVol));
            }

   myMsgFac ().setLineLength (saveMsgLen);

   WitTimer::leaveSection ("default");
   }

//------------------------------------------------------------------------------

const char * WitSolnWriter::quoteCstring (
      const WitString & inputString,
      WitString &       quotedString)
   {
   int inputStringLen;

   inputStringLen = inputString.length ();

   if (inputStringLen + 2 > quotedString.maxLength ())
      quotedString.resize (inputStringLen + 100);

   quotedString[0] = '"';

   inputString.copyInto (& quotedString[1]);

   quotedString[inputStringLen+1] = '"';
   quotedString[inputStringLen+2] = '\0';

   return quotedString.myCstring ();
   }

//------------------------------------------------------------------------------

char * WitSolnWriter::formattedDouble (double theDbl, char * theCstring)
   {
   sprintf (theCstring, "%.*f", myGlobalComp ()->outputPrecision (), theDbl);

   return theCstring;
   }
