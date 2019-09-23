//------------------------------------------------------------------------------
// Source File: "msgAll.C".
//
// A WIT application program whose purpose is to test WIT's issuing of messages.
// This program makes whatever WIT API function calls are necessary
// to cause WIT to issue each of its messages for which the WIT internal
// attribute "WitMsg::testable" is TRUE, and then verifies that it has done so,
// by calling witListUnissuedMsgs, which lists the testable messages that
// weren't issued.
//
// Note: For convenience, this program intentionally leaks memory.
//------------------------------------------------------------------------------

#include <wit.h>

#include <stdlib.h>
#include <string.h>

//------------------------------------------------------------------------------

void           setUpMsgs         (WitRun *);
void           testMainMsgs      (WitRun *);
void           testPipMsgs       (WitRun *);
void           testExtOptMsgs    (WitRun *);
void           testCopyObjMsgs   (WitRun *);
void           testStochMsgs     (WitRun *);
void           testUhsMsgs       (WitRun *);
void           testMultiObjMsgs  (WitRun *);
void           checkUnissuedMsgs (WitRun *);

float *        vector            (float);
float *        vector            (float, float, float, float);
int *          vector            (int, int, int, int);
const char * * vector            (const char *);

//------------------------------------------------------------------------------

int main (int argc, char * argv[])
   {
   WitRun * theWitRun;

   witNewRun           (& theWitRun);

   setUpMsgs             (theWitRun);
   testMainMsgs          (theWitRun);
   testUhsMsgs           (theWitRun);
   testMultiObjMsgs      (theWitRun);
   checkUnissuedMsgs     (theWitRun);
   witDeleteRun          (theWitRun);
   }

//------------------------------------------------------------------------------
// testMainMsgs
//
// Sets up the messages.
// Verifies that the file name can be changed without crashing.
//------------------------------------------------------------------------------

void setUpMsgs (WitRun * theWitRun)
   {
   witSetMesgFileName    (theWitRun, WitFALSE, "msgAll-temp.log");
   witSetMesgFileName    (theWitRun, WitFALSE, WitSTDOUT);

   witSetMesgPrintNumber (theWitRun, WitFALSE, 702, WitFALSE);
   witSetMesgPrintNumber (theWitRun, WitFALSE, 703, WitFALSE);
   witSetMesgPrintNumber (theWitRun, WitFALSE, 704, WitFALSE);
   }

//------------------------------------------------------------------------------
// testMainMsgs
//
// Tests most of the messages.
//------------------------------------------------------------------------------

void testMainMsgs (WitRun * theWitRun)
   {
   witBoolean theBool;
   int        theInt;
   float      theFloat;
   double     theDouble;
   float *    theFloatSt;
   char * *   theCharStSt;
   float * *  theFloatStSt;
   int        lenLists;
   char * *   operationNameList;
   int *      execPeriodList;
   float *    peggedExecVolList;
   witAttr    objItrState;

   witSetMesgTimesPrint  (theWitRun, WitFALSE, 106,                UCHAR_MAX);
   witSetMesgTimesPrint  (theWitRun, WitFALSE,  82,                UCHAR_MAX);
   witSetMesgTimesPrint  (theWitRun, WitFALSE, WitSEVERE_MESSAGES, 1);

   witSetMesgPrintNumber (theWitRun, WitFALSE, WitSEVERE_MESSAGES, WitTRUE);

   witSetMesgStopRunning (theWitRun, WitFALSE, WitSEVERE_MESSAGES, WitTRUE);

   witInitialize                 (theWitRun);
   witSetTitle                   (theWitRun, "msgAll Data");
   witSetNPeriods                (theWitRun, 4);
   witSetWit34Compatible         (theWitRun, WitTRUE);
   witSetComputeCriticalList     (theWitRun, WitTRUE);
   witSetAccAfterSoftLB          (theWitRun, WitTRUE);
   witSetBuildWhenEmpty          (theWitRun, WitTRUE);
   witSetLocalBuildAhead         (theWitRun, WitTRUE);
   witSetLocalMultiExec          (theWitRun, WitTRUE);
   witSetTwoWayMultiExec         (theWitRun, WitTRUE);
   witSetMultiRoute              (theWitRun, WitTRUE);
   witSetPenExec                 (theWitRun, WitTRUE);
   witSetStockRealloc            (theWitRun, WitTRUE);
   witSetSkipFailures            (theWitRun, WitFALSE);
   witSetSelectionRecovery       (theWitRun, WitTRUE);
   witSetRespectStockSLBs        (theWitRun, WitTRUE);
   witSetExpCutoff               (theWitRun, 0.001);
   witSetPerfPegging             (theWitRun, WitTRUE);

   witGetAccelerated             (theWitRun, & theBool);
   witGetBuildWhenEmpty          (theWitRun, & theBool);
   witGetOutputPrecision         (theWitRun, & theInt);
   witGetExpCutoff               (theWitRun, & theFloat);
   witGetExpCutoffDbl            (theWitRun, & theDouble);

   witAddPart                    (theWitRun, "Mat", WitMATERIAL);
   witAddPart                    (theWitRun, "Cap", WitCAPACITY);
   witAddOperation               (theWitRun, "Opn");
   witAddBomEntry                (theWitRun, "Opn", "Cap");
   witAddSubsBomEntry            (theWitRun, "Opn", 0, "Cap");
   witAddBopEntry                (theWitRun, "Opn", "Mat");
   witAddBopEntry                (theWitRun, "Opn", "Mat");
   witAddDemand                  (theWitRun, "Mat", "Dem");
   witAddDemand                  (theWitRun, "Mat", "Dm2");
   witAddDemand                  (theWitRun, "Mat", "Dm3");
   witAddOperation               (theWitRun, "1234567890123");
   witAddPart                    (theWitRun, "Prd", WitPRODUCT);
   witAddPart                    (theWitRun, "Raw", WitRAW);

   witSetPartSupplyVol           (theWitRun, "Cap",        vector (2.0));
   witSetPartSupplyVol           (theWitRun, "Mat",        vector (1.0f, 0.0f,
                                                                   0.0f, 0.0f));
   witSetPartSupplyVol           (theWitRun, "Raw",        vector (1.0f, 0.0f,
                                                                   1.0f, 0.0f));
   witSetDemandDemandVol         (theWitRun, "Mat", "Dem", vector (30.0));
   witSetExpCutoff               (theWitRun,                        0.001);
   witSetPartBuildAsap           (theWitRun, "Mat",        WitTRUE);
   witSetPartBuildAheadLimit     (theWitRun, "Mat",        1);
   witSetPartBoundedLeadTimes    (theWitRun, "Mat",        WitTRUE);
   witSetPartStockBounds         (theWitRun, "Mat",        vector (0.0),
                                                           vector (10.0),
                                                           vector (10.0));
   witSetOperationExecBounds     (theWitRun, "Opn",        vector (0.0),
                                                           vector (0.0),
                                                           vector (100.0));
   witSetDemandCumShipBounds     (theWitRun, "Mat", "Dem", vector (0.0),
                                                           vector (0.0),
                                                           vector (100.0));
   witSetDemandPriority          (theWitRun, "Mat", "Dem", vector (5, 5, 5, 5));
   witSetDemandPriority          (theWitRun, "Mat", "Dm2", vector (1, 0, 0, 0));
   witSetDemandPriority          (theWitRun, "Mat", "Dm3", vector (1, 1, 0, 0));
   witSetSubsBomEntryExpAllowed  (theWitRun, "Opn", 0, 0,  WitTRUE);
   witSetBopEntryExpAllowed      (theWitRun, "Opn", 0,     WitTRUE);
   witSetBopEntryProdRate        (theWitRun, "Opn", 0,     0.007);
   witSetPartObj1ScrapCost       (theWitRun, "Mat",        vector (-1.0));
   witSetOperationExecCost       (theWitRun, "Opn",        vector (-1.0));
   witSetSubsBomEntrySubCost     (theWitRun, "Opn", 0, 0,  vector (-1.0));
   witSetWit34Compatible         (theWitRun,               WitTRUE);

   witSetOperationTwoLevelLotSizes
                                 (theWitRun, "Opn",        WitTRUE);
   witSetOperationMinLotSize     (theWitRun, "Opn",        vector (20.0));
   witSetOperationIncLotSize     (theWitRun, "Opn",        vector (1.0));
   witSetOperationLotSize2Thresh (theWitRun, "Opn",        vector (50.0));
   witSetOperationMinLotSize2    (theWitRun, "Opn",        vector (100.0));
   witSetOperationIncLotSize2    (theWitRun, "Opn",        vector (20.0));

   witSetOperationYieldRate      (theWitRun, "Opn",        
                                  vector (1.0f, 1.0f, .05f, 1.0f));
   witSetPartCycleTime           (theWitRun, "Prd",        vector (10.0));
   witSetBomEntryUsageTime       (theWitRun, "Opn", 0,     0.0);
   witSetBomEntryUsageRate       (theWitRun, "Opn", 0,    -1.0);
   witSetBomEntryUsageRate       (theWitRun, "Opn", 0,     1.0);
   witSetBomEntryFallout         (theWitRun, "Opn", 0,     2);
   witSetPartSelForDel           (theWitRun, "Mat",        WitTRUE);
   witSetOperationSelForDel      (theWitRun, "Opn",        WitTRUE);

   witSetPartPropRtg (
      theWitRun,
      "Mat",
      vector (WitFALSE, WitFALSE, WitTRUE, WitFALSE));

   witSetPartPropRtg (
      theWitRun,
      "Cap",
      vector (WitTRUE, WitFALSE, WitFALSE, WitTRUE));

   witSetBomEntryPropRtg (
      theWitRun,
      "Opn",
      0,
      vector (WitTRUE, WitTRUE, WitTRUE, WitTRUE));

   witSetCplexParSpecName    (theWitRun, "LPMETHOD");
   witSetCplexParSpecIntVal  (theWitRun, 4);

   witSetCplexParSpecName    (theWitRun, "TILIM");
   witSetCplexParSpecDblVal  (theWitRun, 1.5);

   witGetPartNBomEntries     (theWitRun, "Cap",    & theInt);
   witGetPartBuildAheadLimit (theWitRun, "Mat",    & theInt);
   witGetPartCycleTime       (theWitRun, "Prd",    & theFloatSt);
   witGetBomEntryUsageTime   (theWitRun, "Opn", 0, & theFloat);
   witGetBomEntryFallout     (theWitRun, "Opn", 0, & theInt);
   witGetPartObj1ScrapCost   (theWitRun, "Mat",    & theFloatSt);

   while (WitTRUE)
      {
      witAdvanceObjItr  (theWitRun);

      witGetObjItrState (theWitRun, & objItrState);

      if (objItrState == WitINACTIVE)
         break;
      }

   witGetDemandExecVolPegging (
      theWitRun,
      "Mat",
      "Dem",
      2,
      & lenLists,
      & operationNameList,
      & execPeriodList,
      & peggedExecVolList);

   witStartHeurAlloc         (theWitRun);
   witIncHeurAllocTwme       (theWitRun, "Mat", "Dem", 0, 1.0, & theFloat, 
                                         WitTRUE);

   witEqHeurAllocTwme        (theWitRun,
      4,
      vector ("Mat"),
      vector ("Dem"),
      vector (2, 3, 1, 0),
      vector (2.0f, 3.0f, 2.0f, 3.0f),
      & theFloatSt,
      vector (WitTRUE, WitFALSE, WitFALSE, WitTRUE));

   witFinishHeurAlloc        (theWitRun);

   witGetFocusShortageVol    (theWitRun,
      & theInt,
      & theCharStSt,
      & theFloatStSt);

   witOptImplode             (theWitRun);

   remove                    ("solver.log");

   witMrp                    (theWitRun);

   witSetOperationExecVol    (theWitRun, "Opn",        vector (10.0));
   witSetSubsBomEntrySubVol  (theWitRun, "Opn", 0, 0,  vector (22.7));
   witSetDemandShipVol       (theWitRun, "Mat", "Dem", vector (10.0));
   witSetHighPrecisionWD     (theWitRun, WitTRUE);
   witWriteData              (theWitRun, WitSTDOUT);
   witSetHighPrecisionWD     (theWitRun, WitFALSE);
   witWriteData              (theWitRun, WitSTDOUT);
   witDisplayData            (theWitRun, WitSTDOUT);
   witWriteExecSched         (theWitRun, WitSTDOUT, WitBSV);
   witWriteExecSched         (theWitRun, WitSTDOUT, WitCSV);
   witWriteProdSched         (theWitRun, WitSTDOUT, WitBSV);
   witWriteShipSched         (theWitRun, WitSTDOUT, WitBSV);
   witWriteShipSched         (theWitRun, WitSTDOUT, WitCSV);
   witWriteReqSched          (theWitRun, WitSTDOUT, WitBSV);
   witWriteReqSched          (theWitRun, WitSTDOUT, WitCSV);
   witWriteCriticalList      (theWitRun, WitSTDOUT, WitBSV, 10);
   witWriteCriticalList      (theWitRun, WitSTDOUT, WitCSV, 10);
   witDisplayData            (theWitRun, WitSTDOUT);
   witSetAccAfterOptImp      (theWitRun, WitTRUE);

   witOptImplode             (theWitRun);
   witOptImplode             (theWitRun);

   witSetAccAfterOptImp      (theWitRun, WitFALSE);

   remove                    ("solver.log");

   witSetAccAfterOptImp      (theWitRun, WitTRUE);

   witSetOptInitMethod       (theWitRun, WitHEUR_OPT_INIT_METHOD);

   witOptImplode             (theWitRun);

   witSetDemandShipVol       (theWitRun, "Mat", "Dem", vector (20.0));

   witEvalObjectives         (theWitRun);

   witSetAccAfterOptImp      (theWitRun, WitFALSE);
   remove                    ("solver.log");

   witSetComputeCriticalList (theWitRun,        WitFALSE);
   witSetMipMode             (theWitRun,        WitTRUE);
   witSetOperationIntExecVols(theWitRun, "Opn", WitTRUE); 

   witOptImplode             (theWitRun);

   witSetMipMode             (theWitRun,        WitFALSE);

   witSetMultiRoute          (theWitRun,        WitFALSE);
   witSetSelSplit            (theWitRun,        WitTRUE);
   witSetStockRealloc        (theWitRun,        WitFALSE);
   witSetStockReallocation   (theWitRun,        WitTRUE);
   witSetPartSingleSource    (theWitRun, "Mat", WitTRUE);
   witSetPartPropRouting     (theWitRun, "Cap", WitTRUE);
   witSetPipSeqFromHeur      (theWitRun,        WitTRUE);

   witHeurImplode            (theWitRun);

   testPipMsgs               (theWitRun);

   witSetMultiRoute          (theWitRun,        WitTRUE);
   witSetPenExec             (theWitRun,        WitFALSE);

   witHeurImplode            (theWitRun);

   witSetPartSingleSource    (theWitRun, "Mat", WitFALSE);
   witSetSelSplit            (theWitRun,        WitFALSE);
   witSetPenExec             (theWitRun,        WitTRUE);
   witSetPgdCritListMode     (theWitRun,        WitTRUE);

   witSetUserHeurStart       (theWitRun,        WitTRUE);

   witSetOperationMinLotSize (theWitRun, "Opn", vector (0.0));
   witSetOperationExecVol    (theWitRun, "Opn", vector (1.0));

   witHeurImplode            (theWitRun);

   witSetUserHeurStart       (theWitRun,        WitFALSE);

   witSetTieBreakPropRt      (theWitRun,        WitTRUE);

   witHeurImplode            (theWitRun);

   witPurgeData              (theWitRun);

   witSetMesgPrintNumber     (theWitRun,
                              WitFALSE,
                              WitINFORMATIONAL_MESSAGES,
                              WitFALSE);

   witPurgeData              (theWitRun);

   witSetMesgPrintNumber     (theWitRun,
                              WitFALSE,
                              WitINFORMATIONAL_MESSAGES,
                              WitTRUE);

   testExtOptMsgs            (theWitRun);

   testCopyObjMsgs           (theWitRun);

   testStochMsgs             (theWitRun);
   }

//------------------------------------------------------------------------------
// testPipMsgs
//
// Tests messages for PIP.
//------------------------------------------------------------------------------

void testPipMsgs (WitRun * theWitRun)
   {
   int      lenLists;
   char * * partNameList;
   char * * demandNameList;
   int *    shipPerList;   
   float *  incShipVolList;
   int      theIdx;
   char * * operationNameList;
   int *    execPeriodList;
   float *  peggedExecVolList;

   witGetPipSeq (
        theWitRun,
      & lenLists,
      & partNameList,
      & demandNameList,
      & shipPerList,
      & incShipVolList);

   witClearPipSeq (theWitRun);

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      witAppendToPipSeq (
         theWitRun,
         partNameList  [theIdx],
         demandNameList[theIdx],
         shipPerList   [theIdx],
         incShipVolList[theIdx] * 2);

   witSetNewPipPggOrder      (theWitRun,        WitFALSE);
   witWriteData              (theWitRun,        WitSTDOUT);
   witDisplayData            (theWitRun,        WitSTDOUT);
   witBuildPip               (theWitRun);
   witSetNewPipPggOrder      (theWitRun,        WitTRUE);
   witSetOperationPipEnabled (theWitRun, "Opn", WitTRUE);
   witPostprocess            (theWitRun);
   witBuildPip               (theWitRun);

   witGetDemandExecVolPip (
      theWitRun,
      "Mat",
      "Dem",
      2,
      & lenLists,
      & operationNameList,
      & execPeriodList,
      & peggedExecVolList);

   witGetOperationExecVolPip (
      theWitRun,
      "Opn",
      1,
      & lenLists,
      & operationNameList,
      & execPeriodList,
      & peggedExecVolList);
   }

//------------------------------------------------------------------------------
// testExtOptMsgs
//
// Tests messages for ext opt implosion.
//------------------------------------------------------------------------------

void testExtOptMsgs (WitRun * theWitRun)
   {
   int      numcols;
   int      numrows;
   int *    start;
   int *    index;
   double * value;
   double * collb;
   double * colub;
   double * obj;
   double * rowlb;
   double * rowub;
   int      scrapVarIndex;

   witStartExtOpt          (theWitRun);

   witGetExtOptLpProbDbl   (theWitRun,
      & numcols,
      & numrows,
      & start,
      & index,
      & value,
      & collb,
      & colub,
      & obj,
      & rowlb,
      & rowub);

   collb[2] -= 4.7;

   witGetPartScrapVarIndex (theWitRun, "Cap", 2, & scrapVarIndex);
   witSetExtOptSolnDbl     (theWitRun, collb);
   witFinishExtOpt         (theWitRun);
   witStartExtOpt          (theWitRun);
   witShutDownExtOpt       (theWitRun);
   }

//------------------------------------------------------------------------------
// testCopyObjMsgs
//
// Tests messages for witCopy<Object>Data.
//------------------------------------------------------------------------------

void testCopyObjMsgs (WitRun * theWitRun)
   {
   WitRun * theWitRun2;

   witCopyPartData       (theWitRun, "Raw",theWitRun, "Prd");

   witNewRun           (& theWitRun2);
   witInitialize         (theWitRun2);
   witSetNPeriods        (theWitRun2, 4);
   witSetWit34Compatible (theWitRun2, WitTRUE);
   witAddPart            (theWitRun2, "A", WitMATERIAL);

   witCopyPartData       (theWitRun, "Prd", theWitRun2, "A");

   witDeleteRun          (theWitRun2);
   }

//------------------------------------------------------------------------------
// testStochMsgs
//
// Tests messages for stochastic implosion.
//------------------------------------------------------------------------------

void testStochMsgs (WitRun * theWitRun)
   {
   witBoolean devMode;

   witGetDevMode              (theWitRun, & devMode);

   if (! devMode)
      return;

   witSetWit34Compatible      (theWitRun, WitFALSE);
   witSetAccAfterSoftLB       (theWitRun, WitFALSE);

   witSetStageByObject        (theWitRun, WitTRUE);
   witSetStochMode            (theWitRun, WitTRUE);

   witSetPartObjectStage      (theWitRun, "Cap",           1);
   witSetPartObjectStage      (theWitRun, "Raw",           1);
   witSetPartObjectStage      (theWitRun, "Prd",           1);
   witSetOperationObjectStage (theWitRun, "1234567890123", 1);
   witSetOperationObjectStage (theWitRun, "Prd",           1);

   witSetProbability          (theWitRun, 1.0);
   witWriteData               (theWitRun, WitSTDOUT);
   witDisplayData             (theWitRun, WitSTDOUT);
   witStochImplode            (theWitRun);
   witSetStochMode            (theWitRun, WitFALSE);

   witSetStageByObject        (theWitRun, WitFALSE);
   witSetStochMode            (theWitRun, WitTRUE);
   witSetPeriodStage          (theWitRun, vector (0, 0, 1, 1));
   witDisplayData             (theWitRun, WitSTDOUT);
   witSetStochMode            (theWitRun, WitFALSE);
   }

//------------------------------------------------------------------------------
// testUhsMsgs
//
// Tests messages for UHS.
//------------------------------------------------------------------------------

void testUhsMsgs (WitRun * theWitRun)
   {
   witInitialize             (theWitRun);
   witSetNPeriods            (theWitRun, 1);
   witSetUserHeurStart       (theWitRun, true);

   witAddPart                (theWitRun, "A", WitMATERIAL);
   witAddPart                (theWitRun, "B", WitMATERIAL);
   witAddOperation           (theWitRun, "C");
   witAddBomEntry            (theWitRun, "C","A");
   witAddSubsBomEntry        (theWitRun, "C", 0, "B");
   witAddPart                (theWitRun, "D", WitMATERIAL);
   witAddBopEntry            (theWitRun, "C", "D");
   witAddDemand              (theWitRun, "D", "E");

   witSetOperationIncLotSize (theWitRun, "C", vector (100));

   witSetOperationExecBounds (theWitRun, "C", NULL, NULL, vector (17));

   witSetPartSupplyVol       (theWitRun, "B", vector (20));

   witSetDemandDemandVol     (theWitRun, "D", "E", vector (30));

   witSetOperationExecVol    (theWitRun, "C", vector (20));

   witSetSubsBomEntrySubVol  (theWitRun, "C", 0,  0, vector (27));

   witSetDemandShipVol       (theWitRun, "D", "E", vector (45));

   witHeurImplode            (theWitRun);
   }

//------------------------------------------------------------------------------
// testMultiObjMsgs
//
// Tests messages for multiple objectives mode.
//------------------------------------------------------------------------------

void testMultiObjMsgs (WitRun * theWitRun)
   {
   float softLB[] = {10.0};

   witInitialize         (theWitRun);
   witSetNPeriods        (theWitRun,      1);
   witSetMultiObjMode    (theWitRun,      WitTRUE);
   witAddPart            (theWitRun, "A", WitMATERIAL);
   witSetPartStockBounds (theWitRun, "A", NULL, softLB, NULL);
   witDisplayData        (theWitRun,      WitSTDOUT);
   witOptImplode         (theWitRun);
   }

//------------------------------------------------------------------------------
// checkUnissuedMsgs
// In development mode, lists unissued messages.
//------------------------------------------------------------------------------

void checkUnissuedMsgs (WitRun * theWitRun)
   {
   witBoolean devMode;

   witInitialize (theWitRun);

   witGetDevMode (theWitRun, & devMode);

   if (! devMode)
      return;

   witSetMesgTimesPrint (theWitRun, WitFALSE, 98,  0);

   printf (
      "\n"
      "----------------------------------------"
      "----------------------------------------"
      "\n");

   witListUnissuedMsgs (theWitRun);
   }

//------------------------------------------------------------------------------
// Utility Functions
//------------------------------------------------------------------------------

float * vector (float theValue)
   {
   float * v = new float [4];

   v[0] = theValue;
   v[1] = theValue;
   v[2] = theValue;
   v[3] = theValue;

   return v;
   }

//------------------------------------------------------------------------------

float * vector (float v0, float v1, float v2, float v3)
   {
   float * v = new float [4];

   v[0] = v0;
   v[1] = v1;
   v[2] = v2;
   v[3] = v3;

   return v;
   }

//------------------------------------------------------------------------------

int * vector (int v0, int v1, int v2, int v3)
   {
   int * v = new int [4];

   v[0] = v0;
   v[1] = v1;
   v[2] = v2;
   v[3] = v3;

   return v;
   }

//------------------------------------------------------------------------------

const char * * vector (const char * theString)
   {
   const char * * theStrVec = new const char * [4];

   theStrVec[0] = theString;
   theStrVec[1] = theString;
   theStrVec[2] = theString;
   theStrVec[3] = theString;

   return theStrVec;
   }
