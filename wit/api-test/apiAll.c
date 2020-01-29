/*----------------------------------------------------------------------------*/
/* Program apiAll                                                             */
/*                                                                            */
/* This is a WIT application program that invokes each of WIT's API functions */
/* at least once. Its purpose is for testing WIT, especially WIT's API.       */
/*----------------------------------------------------------------------------*/
 
#include <wit.h>

#include <stdlib.h>
#include <stdio.h>

#if !defined(_MSC_VER)
#  include <unistd.h>  
#endif

#include <string.h>
#include <assert.h>
#include <math.h>

/*----------------------------------------------------------------------------*/

void testStandAlone ();

void setUpCoinEmbedded  (WitRun *);
void setUpCplexEmbedded (WitRun *);

void testCplexParSpecs      (WitRun *);
void testClearCplexParSpecs (WitRun *);
void testCplexParSpecAtts   (WitRun *);
void testGetIntCplexParSpec (WitRun *);
void testGetDblCplexParSpec (WitRun *);

void testWriteReadData (WitRun *, const char *);
void testOperationApis();
void testBopApis();
void testAddPart();
void testPartToArcApis ();
void testHeurAlloc ();
void testHeurAllocDbl ();
void testHeurAllocTwme ();
void testHeurAllocTwmeDbl ();
void testDeletion ();
void defProfDelTest (WitRun *);
void selItemsDelTest (WitRun *);
void copyWriteDelTest (WitRun *);
void checkSelDelTest (WitRun *);
void testExecVolPegging    (WitRun *, const char *, const char *, int);
void testExecVolPeggingDbl (WitRun *, const char *, const char *, int);
void testSubVolPegging     (WitRun *, const char *, const char *, int);
void testSubVolPeggingDbl  (WitRun *, const char *, const char *, int);

void testPip                (WitRun *);
void truncatePipSeq         (WitRun *);
void testExecVolPipToDems   (WitRun *, const char *, const char *, int);
void testExecVolPipToOpns   (WitRun *, const char *,               int);
void testCoExecVolPipToDems (WitRun *, const char *, const char *, int);
void testCoExecVolPipToOpns (WitRun *, const char *,               int);
void testSubVolPipToDems    (WitRun *, const char *, const char *, int);
void testSubVolPipToOpns    (WitRun *, const char *,               int);
void testSupplyVolPipToDems (WitRun *, const char *, const char *, int);
void testSupplyVolPipToOpns (WitRun *, const char *,               int);
void testSideVolPipToDems   (WitRun *, const char *, const char *, int);
void testSideVolPipToOpns   (WitRun *, const char *,               int);
void testProdVolPipToDems   (WitRun *, const char *, const char *, int);
void testProdVolPipToOpns   (WitRun *, const char *,               int);
void testConsVolPipToDems   (WitRun *, const char *, const char *, int);
void testConsVolPipToOpns   (WitRun *, const char *,               int);

void prtPartVolPip          (const char *, int, char * *, int *, float *);

void testPipDbl                (WitRun *);
void truncatePipSeqDbl         (WitRun *);
void testExecVolPipToDemsDbl   (WitRun *, const char *, const char *, int);
void testExecVolPipToOpnsDbl   (WitRun *, const char *,               int);
void testCoExecVolPipToDemsDbl (WitRun *, const char *, const char *, int);
void testCoExecVolPipToOpnsDbl (WitRun *, const char *,               int);
void testSubVolPipToDemsDbl    (WitRun *, const char *, const char *, int);
void testSubVolPipToOpnsDbl    (WitRun *, const char *,               int);
void testSupplyVolPipToDemsDbl (WitRun *, const char *, const char *, int);
void testSupplyVolPipToOpnsDbl (WitRun *, const char *,               int);
void testSideVolPipToDemsDbl   (WitRun *, const char *, const char *, int);
void testSideVolPipToOpnsDbl   (WitRun *, const char *,               int);
void testProdVolPipToDemsDbl   (WitRun *, const char *, const char *, int);
void testProdVolPipToOpnsDbl   (WitRun *, const char *,               int);
void testConsVolPipToDemsDbl   (WitRun *, const char *, const char *, int);
void testConsVolPipToOpnsDbl   (WitRun *, const char *,               int);

void prtPartVolPipDbl          (const char *, int, char * *, int *, double *);

void prtPipToDemsHeading (const char *, const char *, const char *, int);
void prtPipToOpnsHeading (const char *, const char *,               int);

void testPgdCritList  (WitRun *);
void printPgdCritList (WitRun *);

void testObjItr (WitRun *);
void testAccOptImp (witBoolean);
void optImpA1 (WitRun *, witBoolean);
void optImpB1 (WitRun *, witBoolean);
void optImpA2 (WitRun *);
void optImpB2 (WitRun *);
void prtFiles ();
void prtRemove (const char *);
void testCumShipVol ();

void testExtOpt             ();
void buildExtOptProblem     (WitRun *);
void testGetExtOptLpProb    (WitRun *);
void testGetExtOptLpProbDbl (WitRun *);
void testGetConIndices      (WitRun *);
void testGetVarIndices      (WitRun *);
void testGetSlbvVarIndices  (WitRun *);
void testGetSlbConIndices   (WitRun *);

void testCopyObjData1WR  ();
void testCopyObjData2WRs ();

void testShipLateAllowed ();

void testGetExpCycle     ();
void buildCycleProblem   (WitRun *);
void displayCycle        (WitRun *);

void testStageByObject   ();
void buildCoreProblemSBO (WitRun *);
void specifyStochDataSBO (WitRun *);
void checkStochData      (WitRun *);
void testStochWriteData  (WitRun *);
void testStochCopyData   (WitRun *);
void testStochImplode    (WitRun *);
void checkStochSoln      (WitRun *);
void testClearStochSoln  (WitRun *);

void testStageByPeriod   ();
void buildCoreProblemSBP (WitRun *);
void specifyStochDataSBP (WitRun *);

void testMultiObjMode  ();
void testObjListSpec   (WitRun *);
void testObjList       (WitRun *);
void testObjSeqNo      (WitRun *);
void testMultiObjTol   (WitRun *);
void populateMultiObj  (WitRun *);
void testSetObjVecs    (WitRun *);
void testGetObjVecs    (WitRun *);
void testMultiObjValue (WitRun *);

void testLeadTimeBounds ();

void          compareFltVecsOf2 (float *,        float,          float);
const float * fltVecOf1         (float);
const float * fltVecOf2         (float,          float);
witBoolean    equalStrings      (const char *,   const char *);
void          compareIntVecs    (const int *,    const int *,    int);
void          compareFltVecs    (const float *,  const float *,  int);
void          compareDblVecs    (const double *, const double *, int);
void          printFltVec       (const char *,   const float *,  int);
void          printDblVec       (const char *,   const double *, int);

const int    nPeriods        = 5;
const double dblEps          = .00001;

witBoolean   coinEmbedded    = WitFALSE;
witBoolean   cplexEmbedded   = WitFALSE;

/*----------------------------------------------------------------------------*/
/* Main Program                                                               */
/*----------------------------------------------------------------------------*/

int main ()
   {
   int   t;
   WitRun * theWitRun;
   WitRun * theMrpRun;
   WitRun * theObj1Run;

   char theAppData[] = "Contents of theAppData.";

   testStandAlone ();

   witNewRun (& theWitRun);
   witNewRun (& theObj1Run);

   /* Message Functions */
   {
   char *string;
   witBoolean bool;
   int intnum;

   witSetMesgThrowErrorExc (theWitRun, WitFALSE, 77, WitTRUE);

   witSetMesgFileAccessMode( theWitRun, WitFALSE, "w" );

   witSetMesgFileName( theWitRun, WitFALSE, "apiAll.msg" );

   witSetMesgPrintNumber( theWitRun, WitFALSE, 
        WitINFORMATIONAL_MESSAGES, WitFALSE);   

   witSetMesgFileName( theWitRun, WitTRUE, WitSTDOUT );

   remove ("apiAll.msg");

   witSetMesgPrintNumber( theWitRun, WitFALSE, 
       WitINFORMATIONAL_MESSAGES, WitTRUE);

   witSetMesgStopRunning( theWitRun, WitFALSE, WitFATAL_MESSAGES, WitFALSE);
   witSetMesgStopRunning( theWitRun, WitFALSE, WitFATAL_MESSAGES, WitTRUE); 

   witSetMesgTimesPrint( theWitRun, WitFALSE, WitWARNING_MESSAGES, 2 ); 
   witSetMesgTimesPrint( theWitRun, WitFALSE, WitWARNING_MESSAGES, UCHAR_MAX );
   }

   witInitialize ( theWitRun );

   witSetWit34Compatible( theWitRun, WitTRUE );

   setUpCoinEmbedded  (theWitRun);
   setUpCplexEmbedded (theWitRun);

   testCplexParSpecs (theWitRun);

   {
   FILE * tempFile;
   witGetMesgFile(     theWitRun, &tempFile );
   fprintf( tempFile, "Hello world\n");
   }

   /* get message functions */
   {
   char *string;
   witBoolean bool;
   int intnum;

   witGetMesgThrowErrorExc (theWitRun, 77, & bool);

   assert (bool);

   witGetMesgFileAccessMode( theWitRun, &string );
   assert( strcmp( string, "w" ) == 0 );

   witFree (string);

   witGetMesgFileName( theWitRun, &string );
   assert( strcmp( string, WitSTDOUT ) == 0 );

   witFree (string);

   witGetMesgPrintNumber( theWitRun, 97, &bool );
   assert( bool == WitTRUE );

   witGetMesgStopRunning( theWitRun, 543, &bool );
   assert( bool == WitTRUE );

   witGetMesgTimesPrint( theWitRun, 75, &intnum );
   assert( intnum == UCHAR_MAX );
   }


   /* witSetGlobalAttribute */
   {
   witSetAccAfterSoftLB( theWitRun, WitFALSE );
   witSetAccAfterSoftLB( theWitRun, WitTRUE );

   witSetAccAfterOptImp( theWitRun, WitTRUE );
   witSetAccAfterOptImp( theWitRun, WitFALSE );

   witSetAutoPriority( theWitRun, WitFALSE );
   witSetAutoPriority( theWitRun, WitTRUE );

   witSetBuildWhenEmpty( theWitRun, WitFALSE );
   witSetExecEmptyBom( theWitRun, WitTRUE );

   witSetRoundReqVols( theWitRun, WitFALSE );

   witSetComputeCriticalList( theWitRun, WitTRUE );

   witSetEquitability( theWitRun, 12 );
   witSetForcedMultiEq( theWitRun, WitTRUE );
   witSetHashTableSize( theWitRun, 5 );
   witSetIndependentOffsets (theWitRun, WitTRUE);
   witSetLocalBuildAhead( theWitRun, WitTRUE );
   witSetLocalMultiExec( theWitRun, WitTRUE );
   witSetLotSizeTolDbl( theWitRun, 5.0e-6 );
   witSetLotSizeTol( theWitRun, 1.0e-4 );
   witSetExpCutoffDbl( theWitRun, 1.0e-4 );
   witSetExpCutoff( theWitRun, 1.0e-3 );
   witSetNPeriods( theWitRun, nPeriods );
   witSetObjChoice( theWitRun, 1 );
   witSetSkipFailures (theWitRun, WitFALSE);
   witSetTitle( theWitRun, "Invoke Each API Function" );
   witSetWboundsDbl( theWitRun, 66000. );
   witSetWbounds( theWitRun, 44000. );

   witSetUseFocusHorizons( theWitRun, WitFALSE );

   witSetTruncOffsets( theWitRun, WitTRUE );

   witSetMultiRoute (theWitRun, WitTRUE);

   witSetPenExec (theWitRun, WitTRUE);

   witSetStockRealloc (theWitRun, WitFALSE);

   witSetStockReallocation (theWitRun, WitTRUE);

   witSetSrSelSplit (theWitRun, WitTRUE);

   witSetNonMrSelSplit (theWitRun, WitTRUE);

   witSetRespectStockSLBs (theWitRun, WitTRUE);

   witSetPrefHighStockSLBs (theWitRun, WitTRUE);

   witSetPerfPegging (theWitRun, WitTRUE);

   witSetCompPrices (theWitRun, WitTRUE);

   witSetUserHeurStart (theWitRun, WitTRUE);

   witSetTieBreakPropRt (theWitRun, WitTRUE);

   witSetNstnResidual (theWitRun, WitTRUE);

   witSetMinimalExcess (theWitRun, WitTRUE);
   }

   /* witGetGlobalAttribute */
   {
   witBoolean bool;
   int i;
   float theFlt;
   double theDbl;
   char * title;

   witGetAccAfterSoftLB( theWitRun, &bool );

   assert( bool );

   witGetAccAfterOptImp( theWitRun, &bool );

   assert( !bool );

   witGetAccelerated( theWitRun, &bool );

   assert( !bool );

   witGetDevMode (theWitRun, & bool);

   printf (
      "\n"
      "WIT was built in %s mode.\n",
      bool? "development": "non-development");

   witGetAutoPriority( theWitRun, &bool );
   assert( bool );
   witGetBuildWhenEmpty( theWitRun, &bool );
   assert( bool );
   witGetExecEmptyBom( theWitRun, &bool );
   assert( bool );
   witGetRoundReqVols( theWitRun, &bool );
   assert( ! bool );
   witGetComputeCriticalList( theWitRun, &bool );
   assert( bool );

   witSetComputeCriticalList( theWitRun, WitFALSE );

   witGetHashTableSize( theWitRun, &i );
   assert( i == 5 );

   witGetIndependentOffsets ( theWitRun, &bool );
   assert( bool );

   witSetIndependentOffsets (theWitRun, WitFALSE);

   witGetEquitability( theWitRun, &i );
   assert( i == 12 );

   witGetForcedMultiEq( theWitRun, &bool );
   assert( bool );

   witGetLocalBuildAhead( theWitRun, &bool );
   assert( bool );

   witGetLocalMultiExec( theWitRun, &bool );
   assert( bool );

   witGetLotSizeTol   ( theWitRun, &theFlt );
   witGetLotSizeTolDbl( theWitRun, &theDbl );

   assert( fabs(theFlt - 1.0e-4 ) < dblEps );
   assert( fabs(theDbl - 1.0e-4 ) < dblEps );

   witGetExpCutoff   ( theWitRun, &theFlt );
   witGetExpCutoffDbl( theWitRun, &theDbl );

   assert( fabs(theFlt - 1.0e-3 ) < dblEps );
   assert( fabs(theDbl - 1.0e-3 ) < dblEps );

   witSetMultiExec( theWitRun, WitTRUE );

   witGetLocalMultiExec( theWitRun, &bool );
   assert( ! bool );

   witGetMultiExec( theWitRun, &bool );
   assert( bool );

   witGetNPeriods( theWitRun, &i );
   assert( i == nPeriods );

   witGetObjChoice( theWitRun, &i );
   assert( i == 1 );

   witGetSkipFailures( theWitRun, &bool );
   assert( !bool );

   witGetTitle( theWitRun, &title );
   assert( strcmp( title, "Invoke Each API Function" ) == 0 );
   witFree ( title );
   
   witGetWbounds   ( theWitRun, &theFlt );
   witGetWboundsDbl( theWitRun, &theDbl );

   assert( fabs(theFlt - 44000. ) < dblEps );
   assert( fabs(theDbl - 44000. ) < dblEps );

   witGetPreprocessed( theWitRun, &bool );
   assert( !bool );

   witGetFeasible( theWitRun, &bool );
   assert( !bool );

   witGetUseFocusHorizons( theWitRun, &bool );
   assert( !bool );

   witGetTruncOffsets( theWitRun, &bool );
   assert( bool );

   witGetMultiRoute (theWitRun, &bool);
   assert (bool);

   witGetPenExec (theWitRun, &bool);
   assert (bool);

   witGetStockRealloc (theWitRun, &bool);
   assert (! bool);

   witGetStockReallocation (theWitRun, &bool);
   assert (bool);

   witGetSrSelSplit (theWitRun, &bool);
   assert (bool);

   witGetNonMrSelSplit (theWitRun, &bool);
   assert (bool);

   witSetSelSplit (theWitRun, WitTRUE);

   witGetSelSplit (theWitRun, &bool);
   assert (bool);

   witGetNonMrSelSplit (theWitRun, &bool);
   assert (! bool);

   witGetRespectStockSLBs (theWitRun, &bool);
   assert (bool);

   witGetPrefHighStockSLBs (theWitRun, &bool);
   assert (bool);

   witGetPerfPegging (theWitRun, &bool);
   assert (bool);

   witGetCompPrices (theWitRun, &bool);
   assert (bool);

   witGetUserHeurStart (theWitRun, & bool);
   assert (bool);

   witSetUserHeurStart (theWitRun, WitFALSE);

   witGetTieBreakPropRt (theWitRun, & bool);
   assert (bool);

   witGetNstnResidual (theWitRun, & bool);
   assert (bool);

   witGetMinimalExcess (theWitRun, & bool);
   assert (bool);
   }

   /* Global appData */
   {
   void * ptr;
   int i;

   witSetAppData( theWitRun, theAppData);
   witGetAppData( theWitRun, &ptr);
   assert( ptr==theAppData );
   }

   {
   int p;
   witGetOutputPrecision( theWitRun, &p );
   assert( p==3 );
   witSetOutputPrecision( theWitRun, 4 );
   witGetOutputPrecision( theWitRun, &p );
   assert( p==4 );
   }

   /* wit34Compatible */
   {
   witBoolean b;
   witSetWit34Compatible( theWitRun, WitFALSE );
   witGetWit34Compatible( theWitRun, &b );
   assert( !b );
   witSetWit34Compatible( theWitRun, WitTRUE );
   witGetWit34Compatible( theWitRun, &b );
   assert( b );
   }

   /* Part PS1 */
   {
   float      cycle             [] = {0.0, 0.1, 0.2, 0.3, 0.4};
   float      prodBoundsHL      [] = {1., 2.,  3., 4., 5.};
   float      prodBoundsSL      [] = {2., 2.2, 4., 5., 6.};
   float      stockBoundsSL     [] = {20., 20.2, 40., 50., 60.};
   double     stockBoundsSLDbl  [] = {10., 10.2, 30., 40., 50.};
   float      stockBoundsHU     [] = {20., 20.2, 40., 50., 60.};
   double     stockBoundsHUDbl  [] = {10., 10.2, 30., 40., 50.};
   float      defaultBoundsHU   [] = {-1.0, -1.0, -1.0, -1.0, -1.0};
   float      defaultBoundsHL   [] = {0.0, 0.0, 0.0, 0.0, 0.0};
   double     defaultBoundsHLDbl[] = {0.0, 0.0, 0.0, 0.0, 0.0};
   float      defaultBoundsSL   [] = {0.0, 0.0, 0.0, 0.0, 0.0};
   float      minLotSize        [] = {2., 2., 4., 5., 6.};
   float      incLotSize        [] = {1., 1., 3., 4., 3.};
   int        yield             [] = {90, 91, 92, 93, 94};
   float      theFlt;
   double     theDbl;
   float *    fv;
   int *      iv;
   witAttr    partCat;
   witBoolean buildNstn;
   witBoolean buildAsap;
   witBoolean scrapAllowed;
   witBoolean asapPipOrder;
   witBoolean baSelSplit;
   int        buildAheadLimit;
   int        buildAheadUB[] = {1, 2, 2, 1, 0};

   witAddPart( theWitRun, "PS1", WitPRODUCT );

   {  /* verify part stock bounds in double  */
   double *hlb;
   double *slb;
   double *hub;
   witSetPartStockBoundsDbl( theWitRun, "PS1", NULL, stockBoundsSLDbl,
                                                     stockBoundsHUDbl);
   witGetPartStockBoundsDbl( theWitRun, "PS1", &hlb, &slb, &hub);
   compareDblVecs( hlb, defaultBoundsHLDbl, nPeriods );
   compareDblVecs( slb, stockBoundsSLDbl,   nPeriods );
   compareDblVecs( hub, stockBoundsHUDbl,   nPeriods );
   witFree (hlb);
   witFree (slb);
   witFree (hub);
   }

   witSetPartCycleTime( theWitRun, "PS1", cycle );
   witSetPartProdBounds( theWitRun, "PS1", prodBoundsHL,prodBoundsSL,NULL );
   witSetPartStockBounds( theWitRun, "PS1", NULL, stockBoundsSL,stockBoundsHU);
   witSetPartYield( theWitRun, "PS1", yield );
   witSetPartMinLotSize( theWitRun, "PS1", minLotSize );
   witSetPartIncLotSize( theWitRun, "PS1", incLotSize );
   witSetPartBuildAsap( theWitRun, "PS1", WitTRUE);

   witSetPartScrapAllowed (theWitRun, "PS1", WitFALSE);
   witSetPartAsapPipOrder (theWitRun, "PS1", WitTRUE);

   {  /* verify get part product bounds */
   float *hlb;
   float *slb;
   float *hub;
   witGetPartProdBounds( theWitRun, "PS1", &hlb, &slb, &hub);
   compareFltVecs( hlb, prodBoundsHL,    nPeriods );
   compareFltVecs( slb, prodBoundsSL,    nPeriods );
   compareFltVecs( hub, defaultBoundsHU, nPeriods );
   witFree (hlb);
   witFree (slb);
   witFree (hub);
   }

   {  /* verify get part stock bounds */
   float *hlb;
   float *slb;
   float *hub;
   witGetPartStockBounds( theWitRun, "PS1", &hlb, &slb, &hub);
   compareFltVecs( hlb, defaultBoundsHL, nPeriods );
   compareFltVecs( slb, stockBoundsSL,   nPeriods );
   compareFltVecs( hub, stockBoundsHU,   nPeriods );
   witFree (hlb);
   witFree (slb);
   witFree (hub);
   }

   {  /* verify get part yield */
   int *my_yield;
   witGetPartYield( theWitRun, "PS1", &my_yield );
   compareIntVecs( my_yield, yield, nPeriods );
   witFree (my_yield);
   }

   witGetPartCategory( theWitRun, "PS1", &partCat );
   assert( partCat == WitPRODUCT );

   witGetPartMinLotSize( theWitRun, "PS1", &fv );
   compareFltVecs( fv, minLotSize, nPeriods );
   witFree (fv);

   witGetPartIncLotSize( theWitRun, "PS1", &fv );
   compareFltVecs( fv, incLotSize, nPeriods );
   witFree (fv);

   witGetPartCycleTime( theWitRun, "PS1", &fv );
   compareFltVecs( fv, cycle, nPeriods );
   witFree (fv);

   witGetPartBuildNstn( theWitRun, "PS1", & buildNstn);
   assert (! buildNstn);

   witGetPartBuildAsap( theWitRun, "PS1", & buildAsap);
   assert (buildAsap);

   witSetPartBuildNstn( theWitRun, "PS1", WitTRUE);

   witGetPartBuildNstn( theWitRun, "PS1", & buildNstn);
   assert (buildNstn);

   witGetPartBuildAsap( theWitRun, "PS1", & buildAsap);
   assert (! buildAsap);

   witGetPartScrapAllowed (theWitRun, "PS1", & scrapAllowed);

   assert (! scrapAllowed);

   witGetPartAsapPipOrder (theWitRun, "PS1", & asapPipOrder);

   assert (asapPipOrder);

   witSetPartAsapPipOrder (theWitRun, "PS1", WitFALSE);

   witSetPartBaSelSplit( theWitRun, "PS1", WitTRUE);

   witGetPartBaSelSplit( theWitRun, "PS1", & baSelSplit);
   assert (baSelSplit);

   witSetPartBaSelSplit( theWitRun, "PS1", WitFALSE);

   /* part buildAheadUB and buildAheadLimit */

   witSetPartBuildAheadLimit (theWitRun, "PS1", 2);

   witGetPartBuildAheadLimit (theWitRun, "PS1", & buildAheadLimit);

   assert (buildAheadLimit == 2);

   witGetPartBuildAheadUB (theWitRun, "PS1", & iv);

   assert (iv[3] == 2);

   witFree (iv);

   witSetPartBuildAheadUB (theWitRun, "PS1", buildAheadUB);

   witGetPartBuildAheadUB (theWitRun, "PS1", & iv);

   compareIntVecs (iv, buildAheadUB, nPeriods);

   witFree (iv);

   witGetPartBuildAheadLimit (theWitRun, "PS1", & buildAheadLimit);

   assert (buildAheadLimit == 1);

   /* part appData */
   {
   void * ptr;
   witSetPartAppData( theWitRun, "PS1", theAppData);
   witGetPartAppData( theWitRun, "PS1", &ptr);
   assert( ptr==theAppData );
   }


   }

   /* Part PS2 */
   {
   float cycle[] = {0.02, 0.12, 0.22, 0.32, 0.42};
   float prodBoundsHL[] = {1.2, 2.2,  3.2, 4.2, 5.2};
   float prodBoundsSL[] = {2.2, 2.22, 4.2, 5.2, 6.2};
   float stockBoundsHU[] = {20.2, 20.22, 40.2, 50.2, 60.2};
   float minLotSize[] = {12., 12., 14., 15., 16.};
   float incLotSize[] = {1., 2., 13., 14., 13.};
   int   yield[] = {94, 95, 96, 97, 98};
  
   witAttr partCat;

   witAddPart (theWitRun, "PS2", WitPRODUCT);

   witGetPartCategory( theWitRun, "PS2", &partCat );
   assert( partCat == WitPRODUCT );
   }

   /* Part MOBILE */
   {
   witAttr partCat;

   witAddPart (theWitRun, "MOBILE", WitPRODUCT);

   witGetPartCategory( theWitRun, "MOBILE", &partCat );
   assert( partCat == WitPRODUCT );
   }

   /* witGetParts */
   {
   int lenList;
   char ** partList;
   witGetParts( theWitRun, &lenList, &partList );
   assert( lenList == 3 );
   assert( strcmp(partList[0],"PS1") == 0 );
   assert( strcmp(partList[1],"PS2") == 0 );
   assert( strcmp(partList[2],"MOBILE") == 0 );
   witFree ( partList[0] );
   witFree ( partList[1] );
   witFree ( partList[2] );
   witFree ( partList );
   }



   /* Part CompA*/
   {
   float supplyVol[] = {2000., 2000., 2000., 2000., 2000.};
   float *fv;
   witAttr partCat;

   witAddPart( theWitRun, "CompA", WitRAW );
   witSetPartSupplyVol( theWitRun, "CompA", supplyVol );

   witGetPartSupplyVol( theWitRun, "CompA", &fv );
   compareFltVecs( fv, supplyVol, nPeriods );
   witFree (fv);

   witGetPartCategory( theWitRun, "CompA", &partCat );
   assert( partCat == WitRAW );
   }

   /* Part CompB*/
   {
   float  supplyVol[]    = {3500., 2500., 2500., 2500., 2500.};
   double supplyVolDbl[] = {3500., 2500., 2500., 2500., 2500.};
   float *fv;
   double * dv;
   witAttr partCat;

   witAddPart( theWitRun, "CompB", WitRAW );
   witSetPartSupplyVolDbl( theWitRun, "CompB", supplyVolDbl );

   witGetPartSupplyVol( theWitRun, "CompB", &fv );
   compareFltVecs( fv, supplyVol, nPeriods );
   witFree (fv);

   witGetPartSupplyVolDbl (theWitRun, "CompB", & dv);

   compareDblVecs (dv, supplyVolDbl, nPeriods);

   witFree (dv);

   witGetPartCategory( theWitRun, "CompB", &partCat );
   assert( partCat == WitRAW );
   }


   /* Part CompC*/
   {
   float supplyVol[] = {2000., 2000., 2000., 2000., 2000.};
   float *fv;
   witAttr partCat;

   witAddPart( theWitRun, "CompC", WitRAW );
   witSetPartSupplyVol( theWitRun, "CompC", supplyVol );

   witGetPartSupplyVol( theWitRun, "CompC", &fv );
   compareFltVecs( fv, supplyVol, nPeriods );
   witFree (fv);

   witGetPartCategory( theWitRun, "CompC", &partCat );
   assert( partCat == WitRAW );
   }

   /* Part TestCap*/
   {
   float supplyVol[] = {3500., 3500., 3500., 3500., 3500.};
   float *fv;
   witAttr partCat;

   witAddPart( theWitRun, "TestCap", WitCAPACITY );
   witSetPartSupplyVol( theWitRun, "TestCap", supplyVol );

   witGetPartSupplyVol( theWitRun, "TestCap", &fv );
   compareFltVecs( fv, supplyVol, nPeriods );
   witFree (fv);

   witGetPartCategory( theWitRun, "TestCap", &partCat );
   assert( partCat == WitCAPACITY );
   }

   {
   witBoolean exists;

   witGetPartExists( theWitRun, "TestCap", &exists );
   assert( exists );

   witGetPartExists( theWitRun, "xyzTestCap", &exists );
   assert( !exists );
   }
 
   /* Part PS1, Demand US */
   {
   int    priority          [] = { 3, 3, 3, 3, 3 };
   float  demandVol         [] = { 500., 500., 500., 500., 500. };
   double demandVolDbl      [] = { 500., 500., 500., 500., 500. };
   float  cumShipBoundsHL   [] = {1.2, 2.2,  3.2, 4.2, 5.2};
   double cumShipBoundsHLDbl[] = {1.6, 2.2,  3.2, 4.2, 5.2};
   float  cumShipBoundsSL   [] = {2.2, 2.22, 4.2, 5.2, 6.2};
   double cumShipBoundsSLDbl[] = {2.6, 2.22, 4.2, 5.2, 6.2};
   float  cumShipBoundsHU   [] = {-1., 200., -1., -1., -1.};
   double cumShipBoundsHUDbl[] = {-1., 300., -1., -1., -1.};
   float  fssShipVol        [] = { 500., 500., 450., 500., 500. };
   double fssShipVolDbl     [] = { 500., 500., 450., 500., 500. };

   int i;
   float theFlt;
   double theDbl;
   float *fv;
   double * dv;
   witBoolean bool;
   int *      iv;
   int        buildAheadLimit;
   int        buildAheadUB[] = {4, 2, 2, 1, 0};
   int        shipLateLimit;
   int        shipLateUB[]   = {3, 2, 3, 2, 1};
   
   witAddDemand( theWitRun, "PS1", "US" );
   witSetDemandPriority( theWitRun, "PS1", "US", priority );
   witSetDemandDemandVol( theWitRun, "PS1", "US", demandVol );
   witSetDemandDemandVolDbl( theWitRun, "PS1", "US", demandVolDbl );
   witSetDemandFssShipVol( theWitRun, "PS1", "US", fssShipVol );
   witSetDemandFssShipVolDbl( theWitRun, "PS1", "US", fssShipVolDbl );
   witSetDemandFocusHorizon( theWitRun, "PS1", "US", nPeriods-1 );
   witSetDemandShipLateLimit( theWitRun, "PS1", "US", 2 );
   witSetDemandPrefBuildAhead( theWitRun, "PS1", "US", WitTRUE );

   witSetDemandSearchInc    (theWitRun, "PS1", "US", 0.1);
   witSetDemandSearchIncDbl (theWitRun, "PS1", "US", 0.01);

   witGetDemandSearchInc    (theWitRun, "PS1", "US", & theFlt);
   witGetDemandSearchIncDbl (theWitRun, "PS1", "US", & theDbl);

   assert (fabs (theFlt - 0.01) < dblEps);
   assert (fabs (theDbl - 0.01) < dblEps);

   witSetDemandSearchInc    (theWitRun, "PS1", "US", 1);

   witGetDemandDemandVol( theWitRun, "PS1", "US", &fv );
   compareFltVecs( fv, demandVol, nPeriods );
   witFree (fv);

   witGetDemandDemandVolDbl( theWitRun, "PS1", "US", &dv );
   compareDblVecs( dv, demandVolDbl, nPeriods );
   witFree (dv);

   witGetDemandFssShipVol( theWitRun, "PS1", "US", &fv );
   compareFltVecs( fv, fssShipVol, nPeriods );
   witFree (fv);

   witGetDemandFssShipVolDbl( theWitRun, "PS1", "US", &dv );
   compareDblVecs( dv, fssShipVolDbl, nPeriods );
   witFree (dv);

   {
   witBoolean exists;

   witGetDemandExists (theWitRun, "PS1", "US", & exists);

   assert (exists);

   witGetDemandExists (theWitRun, "PS1", "UK", & exists);

   assert (! exists);

   witGetDemandExists (theWitRun, "PS9", "US", & exists);

   assert (! exists);
   }

   {
   double *hlb;
   double *slb;
   double *hub;

   witSetDemandCumShipBoundsDbl( theWitRun, "PS1", "US", 
      cumShipBoundsHLDbl, cumShipBoundsSLDbl, cumShipBoundsHUDbl);
   witGetDemandCumShipBoundsDbl( theWitRun, "PS1", "US", 
      &hlb, &slb, &hub);
   compareDblVecs( hlb, cumShipBoundsHLDbl, nPeriods );
   compareDblVecs( slb, cumShipBoundsSLDbl, nPeriods );
   compareDblVecs( hub, cumShipBoundsHUDbl, nPeriods );
   witFree (hlb);
   witFree (slb);
   witFree (hub);
   }

   {
   float *hlb;
   float *slb;
   float *hub;

   witSetDemandCumShipBounds( theWitRun, "PS1", "US", 
      cumShipBoundsHL, cumShipBoundsSL, cumShipBoundsHU);
   witGetDemandCumShipBounds( theWitRun, "PS1", "US", 
      &hlb, &slb, &hub);
   compareFltVecs( hlb, cumShipBoundsHL, nPeriods );
   compareFltVecs( slb, cumShipBoundsSL, nPeriods );
   compareFltVecs( hub, cumShipBoundsHU, nPeriods );
   witFree (hlb);
   witFree (slb);
   witFree (hub);
   }

   witGetDemandFocusHorizon( theWitRun, "PS1", "US", &i );
   assert( i == nPeriods-1 );

   /*---------------- demand buildAheadUB and buildAheadLimit ----------------*/

   witSetDemandBuildAheadLimit (theWitRun, "PS1", "US", 3);

   witGetDemandBuildAheadLimit (theWitRun, "PS1", "US", & buildAheadLimit);

   assert (buildAheadLimit == 3);

   witGetDemandBuildAheadUB (theWitRun, "PS1", "US", & iv);

   assert (iv[3] == 3);

   witFree (iv);

   witSetDemandBuildAheadUB (theWitRun, "PS1", "US", buildAheadUB);

   witGetDemandBuildAheadUB (theWitRun, "PS1", "US", & iv);

   compareIntVecs (iv, buildAheadUB, nPeriods);

   witFree (iv);

   witGetDemandBuildAheadLimit (theWitRun, "PS1", "US", & buildAheadLimit);

   assert (buildAheadLimit == 4);

   /*------------------ demand shipLateUB and shipLateLimit ------------------*/

   witSetDemandShipLateLimit (theWitRun, "PS1", "US", 1);

   witGetDemandShipLateLimit (theWitRun, "PS1", "US", & shipLateLimit);

   assert (shipLateLimit == 1);

   witGetDemandShipLateUB (theWitRun, "PS1", "US", & iv);

   assert (iv[2] == 1);

   witFree (iv);

   witSetDemandShipLateUB (theWitRun, "PS1", "US", shipLateUB);

   witGetDemandShipLateUB (theWitRun, "PS1", "US", & iv);

   compareIntVecs (iv, shipLateUB, nPeriods);

   witFree (iv);

   witGetDemandShipLateLimit (theWitRun, "PS1", "US", & shipLateLimit);

   assert (shipLateLimit == 3);

   /*-------------------------------------------------------------------------*/

   witGetDemandPrefBuildAhead( theWitRun, "PS1", "US", &bool );
   assert( bool );

   /* demand appData */
   {
   void * ptr;
   witSetDemandAppData( theWitRun, "PS1", "US",  theAppData);
   witGetDemandAppData( theWitRun, "PS1", "US",  &ptr);
   assert( ptr==theAppData );
   }

   }

   /* Part PS1, Demand AGP */
   {
   int priority[] = { 3, 2, 3, 2, 3 };
   float demandVol[] = { 500., 500., 500., 500., 500. };
   float cumShipBoundsHL[] = {1.2, 2.2,  3.2, 4.2, 5.2};
   float cumShipBoundsSL[] = {2.2, 2.22, 4.2, 5.2, 6.2};
   float cumShipBoundsHU[] = {-1., 200., -1., -1., -1.};

   int i;

   witAddDemand( theWitRun, "PS1", "AGP");
   witSetDemandFocusHorizon( theWitRun, "PS1", "AGP", nPeriods-2 );

   witGetDemandFocusHorizon( theWitRun, "PS1", "AGP", &i );
   assert( i == nPeriods-2 );
   }

   /*----------------------------------------------*/
   /* Changing the name of a part and of a demand. */
   /*----------------------------------------------*/
   {
   witBoolean selForDel;

   witGetDemandSelForDel   (theWitRun, "PS1",  "AGP", & selForDel);
   assert                                            (! selForDel);
   witSetPartPartName      (theWitRun, "PS1",           "TmpP");
   witSetDemandDemandName  (theWitRun, "TmpP", "AGP",   "TmpD");
   witSetDemandSelForDel   (theWitRun, "TmpP", "TmpD",  WitTRUE);
   witSetPartPartName      (theWitRun, "TmpP",          "PS1");
   witSetDemandDemandName  (theWitRun, "PS1",  "TmpD",  "AGP");
   witGetDemandSelForDel   (theWitRun, "PS1",  "AGP", & selForDel);
   assert                                              (selForDel);
   witSetDemandSelForDel   (theWitRun, "PS1",  "AGP",   WitFALSE);
   }

   /* witGetPartDemands */
   {
   int lenList;
   char **demandList;
   witGetPartDemands( theWitRun, "PS1", &lenList, &demandList );
   assert( lenList == 2 );
   assert( strcmp(demandList[0],"US") == 0 );
   assert( strcmp(demandList[1],"AGP") == 0 );
   witFree ( demandList[0] );
   witFree ( demandList[1] );
   witFree ( demandList );
   }


   /* Part PS2, Demand US */
   {
   int i;

   witAddDemand( theWitRun, "PS2", "US");
   witSetDemandFocusHorizon( theWitRun, "PS2", "US",  nPeriods-3 );

   witGetDemandFocusHorizon( theWitRun, "PS2", "US",  &i );
   assert( i == nPeriods-3 );
   }


   /* Part MOBILE, Demand US */
   {
   int priority[] = { 1, 2, 1, 1, 1 };
   float demandVol[] = { 1000., 1000.,1000.,1000.,1000. };

   int i;

   witAddDemand( theWitRun, "MOBILE", "US");
   witSetDemandFocusHorizon( theWitRun, "MOBILE", "US", nPeriods-4 );

   witGetDemandFocusHorizon( theWitRun, "MOBILE", "US", &i );
   assert( i == nPeriods-4 );
   }

   /* Part PS1 BOM */
   {
   float  offset         [] = {0.0, 0.5, 1.0, 0.0, 0.0};
   double offsetDbl      [] = {0.0, 0.5, 1.0, 0.0, 0.0};
   float  consRate       [] = {1.2, 2.3, 3.4, 4.5, 5.6};
   double consRateDbl    [] = {1.2, 2.3, 3.4, 4.5, 5.6};
   float  routingShare   [] = {5.0, 4.0, 3.0, 2.0, 1.0};
   double routingShareDbl[] = {5.0, 4.0, 3.0, 2.0, 1.0};

   witAddBomEntry( theWitRun, "PS1", "CompA");
   witSetBomEntryUsageTime( theWitRun, "PS1", 0, -0.1 );
   witSetBomEntryUsageRate( theWitRun, "PS1", 0, 1.1 );
   witSetBomEntryConsRate( theWitRun, "PS1", 0, consRate);
   witSetBomEntryConsRateDbl( theWitRun, "PS1", 0, consRateDbl);
   witSetBomEntryEarliestPeriod( theWitRun, "PS1", 0, 1 );
   witSetBomEntryLatestPeriod( theWitRun, "PS1", 0, 3 );
   witSetBomEntryMandEC( theWitRun, "PS1", 0, WitTRUE );

   witSetBomEntryExecPenaltyDbl  (theWitRun, "PS1", 0, 234.76);
   witSetBomEntryExecPenalty     (theWitRun, "PS1", 0,  34.76);
   witSetBomEntryRoutingShare (theWitRun, "PS1", 0, routingShare);
   witSetBomEntryRoutingShareDbl (theWitRun, "PS1", 0, routingShareDbl);

   witAddBomEntry(  theWitRun, "PS1", "CompB");
   witAddBomEntry( theWitRun, "PS1", "TestCap");

     {
     float * off;

     offset[1]=0.;
     offset[2]=0.;

     witSetBomEntryOffset( theWitRun, "PS1", 1, offset );

     witGetBomEntryOffset( theWitRun, "PS1", 1, &off );
     compareFltVecs( off, offset, nPeriods );

     witFree (off);
     }

     {
     double * off;

     offsetDbl[1]=0.;
     offsetDbl[2]=0.;

     witSetBomEntryOffsetDbl( theWitRun, "PS1", 1, offsetDbl );

     witGetBomEntryOffsetDbl( theWitRun, "PS1", 1, &off );
     compareDblVecs( off, offsetDbl, nPeriods );

     witFree (off);
     }

     {
     char *s;
     witGetBomEntryConsumedPart( theWitRun, "PS1", 2, &s );
     assert( equalStrings(s,"TestCap") );
     witFree (s);
     }
   
   }

   {
   int bomEntryEarliestPeriod;
   witGetBomEntryEarliestPeriod( theWitRun, "PS1", 0, &bomEntryEarliestPeriod );
   assert( bomEntryEarliestPeriod == 1 );
   }

   /* falloutRate */
   {
   float  theFalloutRate;
   double theFalloutRateDbl;
   int    theFallout;

   witSetBomEntryFalloutRateDbl (theWitRun, "PS1", 0, 0.043);
   witSetBomEntryFalloutRate    (theWitRun, "PS1", 0, 0.026);
   witGetBomEntryFalloutRate    (theWitRun, "PS1", 0, & theFalloutRate);
   witGetBomEntryFallout        (theWitRun, "PS1", 0, & theFallout);

   assert (fabs (theFalloutRate - 0.026) < dblEps);
   assert (theFallout == 3);

   witSetBomEntryFallout        (theWitRun, "PS1", 0, 4);
   witGetBomEntryFallout        (theWitRun, "PS1", 0, & theFallout);
   witGetBomEntryFalloutRate    (theWitRun, "PS1", 0, & theFalloutRate);
   witGetBomEntryFalloutRateDbl (theWitRun, "PS1", 0, & theFalloutRateDbl);

   assert (theFallout == 4);
   assert (fabs (theFalloutRate    - 0.04) < dblEps);
   assert (fabs (theFalloutRateDbl - 0.04) < dblEps);

   }

   {
   int bomEntryLatestPeriod;
   witGetBomEntryLatestPeriod( theWitRun, "PS1", 0, &bomEntryLatestPeriod );
   assert( bomEntryLatestPeriod == 3 );
   }

   {
   witBoolean bomEntryMandEC;
   witGetBomEntryMandEC( theWitRun, "PS1", 0, &bomEntryMandEC );
   assert( bomEntryMandEC == WitTRUE );
   }

   {
   float  theFlt;
   double theDbl;

   witGetBomEntryExecPenalty    (theWitRun, "PS1", 0, & theFlt);
   witGetBomEntryExecPenaltyDbl (theWitRun, "PS1", 0, & theDbl);

   assert (fabs (theFlt - 34.76) < dblEps);
   assert (fabs (theDbl - 34.76) < dblEps);
   }

   {
   float bomEntryUsageTime;
   witGetBomEntryUsageTime( theWitRun, "PS1", 0, &bomEntryUsageTime );
   assert( fabs( bomEntryUsageTime + 0.1 ) < dblEps );
   }

   {
   float   usageRate;
   float * consRate;
   float   goodConsRate[] = {1.2, 2.3, 3.4, 4.5, 5.6};

   witGetBomEntryUsageRate (theWitRun, "PS1", 0, & usageRate);

   assert (fabs (usageRate - 1.2) < dblEps);

   witGetBomEntryConsRate (theWitRun, "PS1", 0, & consRate);

   compareFltVecs (consRate, goodConsRate, nPeriods);

   witFree (consRate);
   }

   {
   double * consRateDbl;
   double   goodConsRateDbl[] = {1.2, 2.3, 3.4, 4.5, 5.6};

   witGetBomEntryConsRateDbl (theWitRun, "PS1", 0, & consRateDbl);

   compareDblVecs (consRateDbl, goodConsRateDbl, nPeriods);

   witFree (consRateDbl);
   }

   {
   float * routingShare;
   float   goodRoutingShare[] = {5.0, 4.0, 3.0, 2.0, 1.0};

   witGetBomEntryRoutingShare (theWitRun, "PS1", 0, & routingShare);

   compareFltVecs (routingShare, goodRoutingShare, nPeriods);

   witFree (routingShare);
   }

   {
   double * routingShareDbl;
   double   goodRoutingShareDbl[] = {5.0, 4.0, 3.0, 2.0, 1.0};

   witGetBomEntryRoutingShareDbl (theWitRun, "PS1", 0, & routingShareDbl);

   compareDblVecs (routingShareDbl, goodRoutingShareDbl, nPeriods);

   witFree (routingShareDbl);
   }

   {
   int nBomEntries;

   witGetPartNBomEntries( theWitRun, "PS1", &nBomEntries );
   assert( nBomEntries == 3 );

   witGetOperationNBomEntries( theWitRun, "PS1", &nBomEntries );
   assert( nBomEntries == 3 );

   witGetPartNBomEntries( theWitRun, "CompA", &nBomEntries );
   assert( nBomEntries == 0 );

   witGetPartNBomEntries( theWitRun, "TestCap", &nBomEntries );
   assert( nBomEntries == 0 );
   }

   /* bomEntry appData */
   {
   void * ptr;
   witSetBomEntryAppData( theWitRun, "PS1", 2, theAppData);
   witGetBomEntryAppData( theWitRun, "PS1", 2, &ptr);
   assert( ptr==theAppData );
   }

   /* impactPeriod */
   {
   int * impactPeriod;
   int   correctImpactPeriod[] = { -1, 1, 2, 3, -1};

   witGetBomEntryImpactPeriod (theWitRun, "PS1", 0, &impactPeriod);

   compareIntVecs (impactPeriod, correctImpactPeriod, nPeriods);

   witFree (impactPeriod);
   }

   /* Part PS2 BOM */
   {
   witAddBomEntry( theWitRun, "PS2", "CompA");
   witAddBomEntry( theWitRun, "PS2", "CompB");
   witAddBomEntry( theWitRun, "PS2", "CompC");
   witAddBomEntry( theWitRun, "PS2", "TestCap");
   }

   /* Part MOBILE BOM */
   {
   witAddBomEntry( theWitRun, "MOBILE", "CompB");
   witAddBomEntry( theWitRun, "MOBILE", "CompC");
   witAddBomEntry( theWitRun, "MOBILE", "TestCap");
   }

   /* Part PS1 BOM substitute */
   {
   float  consRate       [] = {6.5, 5.4, 4.3, 3.2, 2.1};
   double consRateDbl    [] = {6.5, 5.4, 4.3, 3.2, 2.1};
   float  routingShare   [] = {1.0, 3.0, 1.0, 3.0, 1.0};
   double routingShareDbl[] = {1.0, 3.0, 1.0, 3.0, 1.0};

   witAddSubsBomEntry( theWitRun, "PS1", 0, "CompB");
   witSetSubsBomEntryUsageRate( theWitRun, "PS1", 0, 0, 2.1 );
   witSetSubsBomEntryConsRate (theWitRun, "PS1", 0, 0, consRate);
   witSetSubsBomEntryConsRateDbl (theWitRun, "PS1", 0, 0, consRateDbl);
   witSetSubsBomEntryEarliestPeriod( theWitRun, "PS1", 0, 0, 0 );
   witSetSubsBomEntryLatestPeriod( theWitRun, "PS1", 0, 0, nPeriods-1 );

   witSetSubsBomEntryExpAllowed (theWitRun, "PS1", 0, 0, WitFALSE);
   witSetSubsBomEntryExpNetAversionDbl (theWitRun, "PS1", 0, 0, 3.9);
   witSetSubsBomEntryExpNetAversion    (theWitRun, "PS1", 0, 0, 1.2);
   witSetSubsBomEntryExecPenaltyDbl (theWitRun, "PS1", 0, 0, 329.87);
   witSetSubsBomEntryExecPenalty    (theWitRun, "PS1", 0, 0,  29.87);
   witSetSubsBomEntryNetAllowed (theWitRun, "PS1", 0, 0, WitFALSE);
   witSetSubsBomEntryRoutingShare (theWitRun, "PS1", 0, 0, routingShare);
   witSetSubsBomEntryRoutingShareDbl (theWitRun, "PS1", 0, 0, routingShareDbl);

   witAddSubsBomEntry(  theWitRun, "PS1", 1, "CompA");

   witSetSubsBomEntryNetAllowed    (theWitRun, "PS1", 1, 0, WitFALSE);
   witSetSubsBomEntryMrpNetAllowed (theWitRun, "PS1", 1, 0, WitTRUE);
   }

   /* consumedPartName */
   {
   char *s;
   witGetSubsBomEntryConsumedPart( theWitRun, "PS1", 0, 0, &s );
   assert( equalStrings(s,"CompB") );
   witFree (s);
   }

   {
   int subsBomEntryEarliestPeriod;
   witGetSubsBomEntryEarliestPeriod( theWitRun, "PS1", 0, 0, &subsBomEntryEarliestPeriod );
   assert( subsBomEntryEarliestPeriod == 0 );
   }

   /* falloutRate */
   {
   float  theFalloutRate;
   double theFalloutRateDbl;
   int    theFallout;

   witSetSubsBomEntryFalloutRateDbl (theWitRun, "PS1", 0, 0, 0.037);
   witSetSubsBomEntryFalloutRate    (theWitRun, "PS1", 0, 0, 0.054);
   witGetSubsBomEntryFalloutRate    (theWitRun, "PS1", 0, 0, & theFalloutRate);
   witGetSubsBomEntryFalloutRateDbl (theWitRun, "PS1", 0, 0, 
                                                & theFalloutRateDbl);
   witGetSubsBomEntryFallout        (theWitRun, "PS1", 0, 0, & theFallout);

   assert (fabs (theFalloutRate    - 0.054) < dblEps);
   assert (fabs (theFalloutRateDbl - 0.054) < dblEps);
   assert (theFallout == 5);

   witSetSubsBomEntryFallout     (theWitRun, "PS1", 0, 0, 17);
   witGetSubsBomEntryFallout     (theWitRun, "PS1", 0, 0, & theFallout);
   witGetSubsBomEntryFalloutRate (theWitRun, "PS1", 0, 0, & theFalloutRate);

   assert (theFallout == 17);
   assert (fabs (theFalloutRate - 0.17) < dblEps);
   }

   {
   int subsBomEntryLatestPeriod;
   witGetSubsBomEntryLatestPeriod( theWitRun, "PS1", 0, 0,
      &subsBomEntryLatestPeriod );
   assert( subsBomEntryLatestPeriod == nPeriods-1 );
   }

   {
   witBoolean subsBomEntryExpAllowed;

   witGetSubsBomEntryExpAllowed (
       theWitRun, "PS1", 0, 0, & subsBomEntryExpAllowed);

   assert (! subsBomEntryExpAllowed);
   }

   {
   witBoolean subsBomEntryNetAllowed;

   witGetSubsBomEntryNetAllowed (
       theWitRun, "PS1", 1, 0, & subsBomEntryNetAllowed);

   assert (! subsBomEntryNetAllowed);
   }

   {
   witBoolean subsBomEntryMrpNetAllowed;

   witGetSubsBomEntryMrpNetAllowed (
       theWitRun, "PS1", 1, 0, & subsBomEntryMrpNetAllowed);

   assert (subsBomEntryMrpNetAllowed);
   }

   {
   float  subsBomEntryExpNetAversion;
   double subsBomEntryExpNetAversionDbl;

   witGetSubsBomEntryExpNetAversion (
      theWitRun, "PS1", 0, 0, & subsBomEntryExpNetAversion);
   witGetSubsBomEntryExpNetAversionDbl (
      theWitRun, "PS1", 0, 0, & subsBomEntryExpNetAversionDbl);

   assert (fabs (subsBomEntryExpNetAversion    - 1.2) < dblEps);
   assert (fabs (subsBomEntryExpNetAversionDbl - 1.2) < dblEps);
   }

   {
   float   usageRate;
   float * consRate;
   float   goodConsRate[] = {6.5, 5.4, 4.3, 3.2, 2.1};

   witGetSubsBomEntryUsageRate (theWitRun, "PS1", 0, 0, & usageRate);

   assert (fabs (usageRate - 6.5) < dblEps);

   witGetSubsBomEntryConsRate (theWitRun, "PS1", 0, 0, & consRate);

   compareFltVecs (consRate, goodConsRate, nPeriods);

   witFree (consRate);
   }

   {
   double * consRateDbl;
   double   goodConsRateDbl[] = {6.5, 5.4, 4.3, 3.2, 2.1};

   witGetSubsBomEntryConsRateDbl (theWitRun, "PS1", 0, 0, & consRateDbl);

   compareDblVecs (consRateDbl, goodConsRateDbl, nPeriods);

   witFree (consRateDbl);
   }

   {
   float * routingShare;
   float   goodRoutingShare[] = {1.0, 3.0, 1.0, 3.0, 1.0};

   witGetSubsBomEntryRoutingShare (theWitRun, "PS1", 0, 0, & routingShare);

   compareFltVecs (routingShare, goodRoutingShare, nPeriods);

   witFree (routingShare);
   }

   {
   double * routingShareDbl;
   double   goodRoutingShareDbl[] = {1.0, 3.0, 1.0, 3.0, 1.0};

   witGetSubsBomEntryRoutingShareDbl (
      theWitRun, "PS1", 0, 0, & routingShareDbl);

   compareDblVecs (routingShareDbl, goodRoutingShareDbl, nPeriods);

   witFree (routingShareDbl);
   }

   {
   float  theFlt;
   double theDbl;

   witGetSubsBomEntryExecPenalty    (theWitRun, "PS1", 0, 0, & theFlt);
   witGetSubsBomEntryExecPenaltyDbl (theWitRun, "PS1", 0, 0, & theDbl);

   assert (fabs (theFlt - 29.87) < dblEps);
   assert (fabs (theDbl - 29.87) < dblEps);
   }

   /* subsBomEntry appData */
   {
   void * ptr;
   witSetSubsBomEntryAppData( theWitRun, "PS1", 0, 0, theAppData );
   witGetSubsBomEntryAppData( theWitRun, "PS1", 0, 0, &ptr);
   assert( ptr==theAppData );
   }

   /* impactPeriod */
   {
   int * impactPeriod;
   int   correctImpactPeriod[] = { -1, 1, 2, 3, -1};

   witGetSubsBomEntryImpactPeriod (theWitRun, "PS1", 0, 0, &impactPeriod);

   compareIntVecs (impactPeriod, correctImpactPeriod, nPeriods);

   witFree (impactPeriod);
   }

   {
   int nSubsBomEntries;
   witGetBomEntryNSubsBomEntries( theWitRun, "PS1", 1, &nSubsBomEntries );
   assert( nSubsBomEntries == 1 );
   }

   /*-------------------------------------------------------------------------*/
   /* belowList                                                               */
   /*                                                                         */
   /* This is being done at this point, instead of with the other Part        */
   /* attributes, so that the below list will not be empty.                   */
   /*-------------------------------------------------------------------------*/

   {
   int      lenList;
   char * * partNameList;
   int      i;

   witGetPartBelowList (theWitRun, "PS1", & lenList, & partNameList);

   printf ("\nBelow List:\n");

   for (i = 0; i < lenList; ++ i)
      printf ("   %s\n", partNameList[i]);

   for (i = 0; i < lenList; ++ i)
      witFree (partNameList[i]);

   witFree (partNameList);
   }

   /*-------------------------------------------------------------------------*/
   /* Object Iteration.                                                       */
   /*-------------------------------------------------------------------------*/

   testObjItr (theWitRun);

   /*-------------------------------------------------------------------------*/

   {
   witBoolean bool;
   witPreprocess( theWitRun );
   witOptPreprocess( theWitRun );
   witDisplayData( theWitRun, WitSTDOUT );
   witGetPreprocessed( theWitRun, &bool );
   assert( bool );
   }
   {
   witBoolean accAfterOptImp,accelerated;
   witGetAccAfterOptImp( theWitRun, &accAfterOptImp );
   witGetAccelerated( theWitRun, &accelerated );

   assert( !accAfterOptImp );
   assert( !accelerated );

   witSetAccAfterOptImp( theWitRun, WitTRUE );
   witGetAccAfterOptImp( theWitRun, &accAfterOptImp );

   assert( accAfterOptImp );

   }

   /* Opt implode */
   {
   float      prodVol    [] = {5000., 5000., 2000., 0000., 2000.};
   float      subVol1    [] = {4001., 4001., 1001., 501., 21.};
   float      subVol     [] = {4000., 4000., 1000., 500., 20.};
   double     subVolDbl  [] = {4000., 4000., 1000., 500., 20.};
   float      shipVol    [] = {6000., 6000., 3000., 0000., 3000.};
   double     shipVolDbl [] = {6000., 6000., 3000., 0000., 3000.};
   float      cumShipSLB [] = {10000., 10000., 10000., 10000., 10000.};
   float      cumShipHUB [] = {   -1.,    -1.,    -1.,    -1.,    -1.};
   float *    fv;
   double *   dv;
   char *     string;
   witBoolean theBool;

   witSetDemandCumShipBounds (theWitRun, "PS1", "US",
                              NULL,
                              cumShipSLB,
                              cumShipHUB);

   witSetSolverLogFileName (theWitRun, "solver-alt.log" );
   witGetSolverLogFileName (theWitRun, & string);

   assert (strcmp (string, "solver-alt.log") == 0);

   witFree (string);

   witSetPreferCoin (theWitRun,   WitTRUE);
   witGetPreferCoin (theWitRun, & theBool);

   assert (theBool);

   witSetPreferCoin (theWitRun,   WitFALSE);

   witSetPartProdVol( theWitRun, "PS1", prodVol );
   witGetPartProdVol( theWitRun, "PS1", &fv );
   compareFltVecs( fv, prodVol, nPeriods );
   witFree (fv);

   witSetDemandShipVol( theWitRun, "PS1", "US", shipVol );
   witGetDemandShipVol( theWitRun, "PS1", "US", &fv );
   compareFltVecs( fv, shipVol, nPeriods );
   witFree (fv);

   witSetDemandShipVolDbl( theWitRun, "PS1", "US", shipVolDbl );
   witGetDemandShipVolDbl( theWitRun, "PS1", "US", &dv );
   compareDblVecs( dv, shipVolDbl, nPeriods );
   witFree (dv);

   witSetSubsBomEntryProdVol( theWitRun, "PS1", 0, 0, subVol1);
   witGetSubsBomEntryProdVol( theWitRun, "PS1", 0, 0, &fv );
   compareFltVecs( fv, subVol1, nPeriods );
   witFree (fv);

   witSetSubsBomEntrySubVol( theWitRun, "PS1", 0, 0, subVol );
   witGetSubsBomEntrySubVol( theWitRun, "PS1", 0, 0, &fv );
   compareFltVecs( fv, subVol, nPeriods );
   witFree (fv);

   witSetSubsBomEntrySubVolDbl( theWitRun, "PS1", 0, 0, subVolDbl );
   witGetSubsBomEntrySubVolDbl( theWitRun, "PS1", 0, 0, &dv );
   compareDblVecs( dv, subVolDbl, nPeriods );
   witFree (dv);


   {
   witBoolean postprocessed;
   witGetPostprocessed( theWitRun, &postprocessed );
   assert( !postprocessed );
   witPostprocess(theWitRun);
   witGetPostprocessed( theWitRun, &postprocessed );
   assert( postprocessed );
   }


   {
   witAttr optInitMethod;
   witGetOptInitMethod( theWitRun, & optInitMethod );
   assert( optInitMethod == WitSCHED_OPT_INIT_METHOD );

   witSetOptInitMethod( theWitRun, WitCRASH_OPT_INIT_METHOD );
   witGetOptInitMethod( theWitRun, & optInitMethod );
   assert( optInitMethod == WitCRASH_OPT_INIT_METHOD );

   witSetOptInitMethod( theWitRun, WitHEUR_OPT_INIT_METHOD );
   witGetOptInitMethod( theWitRun, & optInitMethod );
   assert( optInitMethod == WitHEUR_OPT_INIT_METHOD );
   }

   if (cplexEmbedded)
      witOptImplode (theWitRun);
         /**/
         /* This test is somewhat CPLEX-specific */

   witHeurImplode (theWitRun);

   witWriteProdSched( theWitRun, WitSTDOUT, WitBSV );
   witWriteShipSched( theWitRun, WitSTDOUT, WitCSV );
   witWriteCriticalList( theWitRun, WitSTDOUT, WitBSV, 0 );

   witSetMesgFileAccessMode( theWitRun, WitFALSE, "w" );
   witWriteExecSched( theWitRun, "apiAll.sch", WitBSV );
   witSetMesgFileAccessMode( theWitRun, WitFALSE, "a" );
   witWriteShipSched( theWitRun, "apiAll.sch", WitBSV );
   witSetMesgFileAccessMode( theWitRun, WitFALSE, "w" );

   fflush (stdout);
   system ("cat apiAll.sch");
   remove ("apiAll.sch");
   }

   /* Get global solution attributes */

   if (cplexEmbedded )
      {
      float  objValue,  boundsValue,  rev,  inv,  serv,  sub;
      double objValueD, boundsValueD, revD, invD, servD, subD;
      witBoolean optSolved;

      witGetObjValue( theWitRun, &objValue );
      printf( "objValue = %f\n",objValue);

      witGetBoundsValue( theWitRun, &boundsValue );
      printf( "boundsValue = %f\n",boundsValue);

      objValue    = 0.0;
      boundsValue = 0.0;

      witGetObjValues( theWitRun, &objValue, &boundsValue );
      printf( "objValue = %f, boundsValue = %f\n",objValue,boundsValue);

      witGetBoundsValueDbl( theWitRun, &boundsValueD );
      printf( "boundsValue = %f\n",boundsValueD);

      objValueD    = 0.0;
      boundsValueD = 0.0;

      witGetObjValuesDbl( theWitRun, &objValueD, &boundsValueD );
      printf( "objValue = %f, boundsValue = %f\n",objValueD,boundsValueD);

      witGetPostprocessed( theWitRun, &optSolved );
      assert( optSolved );
      }

   witEvalObjectives(theWitRun);

   {
   witBoolean accAfterOptImp,accelerated;
   witGetAccAfterOptImp( theWitRun, &accAfterOptImp );
   witGetAccelerated( theWitRun, &accelerated );

   if (cplexEmbedded)
      {
      assert (accAfterOptImp);
      assert (accelerated);
      }

   witSetAccAfterOptImp( theWitRun, WitFALSE );
   witGetAccelerated( theWitRun, &accelerated );
   witGetAccAfterOptImp( theWitRun, &accAfterOptImp );

   if (cplexEmbedded)
      {
      assert (! accelerated);
      assert (! accAfterOptImp);

      remove ("solver-alt.log");
      }
   }

   /* Heuristic Implosion */
   {
   witBoolean postproc;

   witHeurImplode( theWitRun );

   witWriteExecSched( theWitRun, WitSTDOUT, WitCSV );
   witWriteShipSched( theWitRun, WitSTDOUT, WitBSV );
   witWriteCriticalList( theWitRun, WitSTDOUT, WitCSV, 0 );
   witGetPostprocessed( theWitRun, &postproc );
   assert( postproc );
   }
   {
   witBoolean bool;
   witGetFeasible( theWitRun, &bool );
/* assert( bool ); */
   }

   /* Get critical list */
   {
   int i;
   int lenCritList, *critPerList;
   char **critPartList;
   witGetCriticalList( theWitRun, &lenCritList, &critPartList, &critPerList);
   for ( i=0; i<lenCritList; i++ ) {
      printf(" Critical list item: %d  part: %s  period: %d\n",
             i, critPartList[i], critPerList[i] );
      witFree (critPartList[i]);
   }
   witFree (critPartList);
   witFree (critPerList);
   } 

   /* Get Focus Shortage Vol using fssShipVol */
   {
   int i;
   int lenList;
   char **partList;
   float **focShortVol;

   witSetUseFocusHorizons( theWitRun, WitFALSE );

   witGetFocusShortageVol( theWitRun, &lenList, &partList, &focShortVol);
   for ( i=0; i<lenList; i++ ) {
      printf(" Focus Shortage item: %d  part: %s\n", i, partList[i] );
      witFree (partList[i]);
      printFltVec ("focusedShortage", focShortVol[i], nPeriods);
      witFree (focShortVol[i]);
   }
   witFree (partList);
   witFree (focShortVol);
   } 

   /* Get Focus Shortage Vol using FocusHorizons */
   {
   int i;
   int lenList;
   char **partList;
   float **focShortVol;

   witSetUseFocusHorizons( theWitRun, WitTRUE );
   witGetDemandFocusHorizon( theWitRun, "PS1", "AGP", &i );
   assert( i == nPeriods-2 );

   witGetFocusShortageVol( theWitRun, &lenList, &partList, &focShortVol);
   for ( i=0; i<lenList; i++ ) {
      printf(" Focus Shortage item: %d  part: %s\n", i, partList[i] );
      witFree (partList[i]);
      printFltVec ("focusedShortage", focShortVol[i], nPeriods);
      witFree (focShortVol[i]);
   }
   witFree (partList);
   witFree (focShortVol);
   }


   /* Get Focus Shortage Vol using FocusHorizons: Double Precision */
   {
   int i;
   int lenList;
   char **partList;
   double **focShortVol;

   witGetFocusShortageVolDbl( theWitRun, &lenList, &partList, &focShortVol);
   for ( i=0; i<lenList; i++ ) {
      printf(" Focus Shortage item: %d  part: %s\n", i, partList[i] );
      witFree (partList[i]);
      printDblVec ("focusedShortage", focShortVol[i], nPeriods);
      witFree (focShortVol[i]);
   }
   witFree (partList);
   witFree (focShortVol);
   }


   /* Get part solution values */
   {
   float  * fv;
   double * dv;

   witGetPartConsVol( theWitRun, "CompA", &fv);
   printFltVec ("ConsVol", fv, nPeriods);
   witFree (fv);

   witGetPartConsVolDbl( theWitRun, "CompA", &dv);
   printDblVec ("ConsVol", dv, nPeriods);
   witFree (dv);

   witGetPartResidualVol( theWitRun, "CompA", &fv);
   printFltVec ("ResidualVol", fv, nPeriods);
   witFree (fv);

   witGetPartResidualVolDbl( theWitRun, "CompA", &dv);
   printDblVec ("ResidualVol", dv, nPeriods);
   witFree (dv);

   witGetPartExcessVol( theWitRun, "CompA", &fv);
   printFltVec ("ExcessVol", fv, nPeriods);
   witFree (fv);

   witGetPartExcessVolDbl( theWitRun, "CompA", &dv);
   printDblVec ("ExcessVol", dv, nPeriods);
   witFree (dv);

   witGetPartProdVol( theWitRun, "PS2", &fv);
   printFltVec ("ProdVol", fv, nPeriods);
   witFree (fv);

   witGetPartProdVolDbl (theWitRun, "PS2", & dv);
   printDblVec ("ProdVol", dv, nPeriods);
   witFree (dv);

   witGetPartScrapVol( theWitRun, "CompA", &fv);
   printFltVec ("ScrapVol", fv, nPeriods);
   witFree (fv);

   witGetPartScrapVolDbl( theWitRun, "CompA", &dv);
   printDblVec ("ScrapVol", dv, nPeriods);
   witFree (dv);

   witGetPartShadowPrice( theWitRun, "CompA", &fv);
   printFltVec ("ShadowPrice", fv, nPeriods);
   witFree (fv);

   witGetPartShadowPriceDbl (theWitRun, "CompA", & dv);
   printDblVec ("ShadowPrice", dv, nPeriods);
   witFree (dv);

   witGetPartFocusShortageVol( theWitRun, "CompB", &fv);
   printFltVec ("FocusShortageVol", fv, nPeriods);
   witFree (fv);

   witGetPartFocusShortageVolDbl( theWitRun, "CompB", &dv);
   printDblVec ("FocusShortageVol", dv, nPeriods);
   witFree (dv);

   witGetOperationFssExecVol( theWitRun, "PS1", &fv);
   printFltVec ("FssExecVol", fv, nPeriods);
   witFree (fv);

   witGetOperationFssExecVolDbl( theWitRun, "PS1", &dv);
   printDblVec ("FssExecVol", dv, nPeriods);
   witFree (dv);

   witGetSubsBomEntryFssSubVol( theWitRun, "PS1", 0, 0, &fv );
   printFltVec ("FssSubVol", fv, nPeriods);
   witFree (fv);

   witGetSubsBomEntryFssSubVolDbl( theWitRun, "PS1", 1, 0, &dv );
   printDblVec ("FssSubVol", dv, nPeriods);
   witFree (dv);

   witGetPartStockVol( theWitRun, "CompA", &fv);
   printFltVec ("StockVol", fv, nPeriods);
   witFree (fv);

   witGetPartStockVolDbl( theWitRun, "CompA", &dv);
   printDblVec ("StockVol", dv, nPeriods);
   witFree (dv);

   witGetPartSupplyVol( theWitRun, "CompA", &fv);
   printFltVec ("SupplyVol", fv, nPeriods);
   witFree (fv);
   }

   /* Get demand solution values */
   {
   float *fv;

   witGetDemandShipVol( theWitRun, "MOBILE", "US", &fv);
   printFltVec ("ShipVol", fv, nPeriods);
   witFree (fv);
   }

   /* Get substitute solution values */
   {
   float *fv;
   witGetSubsBomEntryProdVol( theWitRun, "PS1", 0, 0, &fv);
   printFltVec ("ProdVol", fv, nPeriods);
   witFree (fv);
   }

   /* Test Concurrent Pegging */

   {
   testExecVolPegging    (theWitRun, "PS1", "US", 1);
   testSubVolPegging     (theWitRun, "PS1", "US", 1);
   testExecVolPegging    (theWitRun, "PS1", "US", 2);
   testExecVolPeggingDbl (theWitRun, "PS1", "US", 2);
   testSubVolPegging     (theWitRun, "PS1", "US", 2);
   testSubVolPeggingDbl  (theWitRun, "PS1", "US", 2);

   witClearPegging       (theWitRun);

   testExecVolPegging    (theWitRun, "PS1", "US", 2);
   testSubVolPegging     (theWitRun, "PS1", "US", 2);
   }
   
   /* Write and Read data and switch to new witRun */
   {
   witWriteData( theWitRun, "apiAll.data" );

   witNewRun (& theMrpRun);

   witInitialize( theMrpRun );
   witSetWit34Compatible( theMrpRun, WitTRUE );
   witReadData( theMrpRun, "apiAll.data" );

   fflush (stdout);
   system ("cat apiAll.data");
   remove ("apiAll.data");
   }

   /* MRP */
   {
   float supp[] = {500, 500, 500, 500, 500};

   witSetPartSupplyVol (theMrpRun, "CompB", supp);

   witMrp( theMrpRun );
   witWriteReqSched( theMrpRun, WitSTDOUT, WitBSV );
   }

   /* Get part mrp solution values */
   {
   float  * fv;
   double * dv;

   witGetPartReqVol( theMrpRun, "CompB", &fv);
   printFltVec ("ReqVol", fv, nPeriods);
   witFree (fv);

   witGetPartReqVolDbl( theMrpRun, "CompB", &dv);
   printDblVec ("ReqVol", dv, nPeriods);
   witFree (dv);

   witGetPartMrpProdVol( theMrpRun, "PS2", &fv);
   printFltVec ("MrpProdVol", fv, nPeriods);
   witFree (fv);

   witGetPartMrpResidualVol( theMrpRun, "CompA", &fv);
   printFltVec ("MrpResidualVol", fv, nPeriods);
   witFree (fv);

   witGetPartMrpResidualVolDbl( theMrpRun, "CompA", &dv);
   printDblVec ("MrpResidualVol", dv, nPeriods);
   witFree (dv);

   witGetPartMrpExcessVol( theMrpRun, "CompA", &fv);
   printFltVec ("MrpExcessVol", fv, nPeriods);
   witFree (fv);

   witGetPartMrpExcessVolDbl( theMrpRun, "CompA", &dv);
   printDblVec ("MrpExcessVol", dv, nPeriods);
   witFree (dv);

   witGetPartMrpConsVol( theMrpRun, "CompB", &fv);
   printFltVec ("MrpConsVol", fv, nPeriods);
   witFree (fv);

   witGetPartMrpConsVolDbl( theMrpRun, "CompB", &dv);
   printDblVec ("MrpConsVol", dv, nPeriods);
   witFree (dv);

   witGetOperationMrpExecVol( theMrpRun, "PS2", &fv);
   printFltVec ("MrpExecVol", fv, nPeriods);
   witFree (fv);

   witGetOperationMrpExecVolDbl( theMrpRun, "PS2", &dv);
   printDblVec ("MrpExecVol", dv, nPeriods);
   witFree (dv);

   witGetSubsBomEntryMrpSubVol (theMrpRun, "PS1", 1, 0, & fv);
   printFltVec ("MrpSubVol", fv, nPeriods);
   witFree (fv);

   witGetSubsBomEntryMrpSubVolDbl (theMrpRun, "PS1", 1, 0, & dv);
   printDblVec ("MrpSubVol", dv, nPeriods);
   witFree (dv);
   }

   testPip    (theWitRun);
   testPipDbl (theWitRun);

   testPgdCritList (theWitRun);

   witDeleteRun (theWitRun);
   witDeleteRun (theMrpRun);

   {
   witInitialize( theObj1Run );
   witSetWit34Compatible( theObj1Run, WitTRUE );
   witSetHashTableSize( theObj1Run, 5 );
   witSetNPeriods( theObj1Run, nPeriods );
   witSetIndependentOffsets (theObj1Run, WitTRUE);
   witSetMultiRoute (theObj1Run, WitTRUE);
   }


   /* Parts */
   {
   float  supplyVolCompA [] = {2000., 2000., 2000., 2000., 2000.};
   float  supplyVolCompB [] = {3500., 2500., 2500., 2500., 2500.};
   float  supplyVolCompC [] = {2000., 2000., 2000., 2000., 2000.};
   float  supplyVolCap   [] = {3500., 3500., 3500., 3500., 3500.};

   float  obj1ProdCost   [] = {1.1,  1.2,  1.3,  1.4,  1.5};
   float  scrapCost      [] = {0.1,  0.2,  0.3,  0.4,  0.5};
   double scrapCostDbl   [] = {0.1,  0.2,  0.3,  0.4,  0.5};
   float  stockCost      [] = {0.01, 0.02, 0.03, 0.04, 0.05};
   double stockCostDbl   [] = {0.01, 0.02, 0.03, 0.04, 0.05};
   float  incLotSize     [] = { 500.0,  500.0,  500.0,  500.0,  500.0};
   float  minLotSize     [] = {2500.0, 2500.0, 2500.0, 2500.0, 2500.0};

   witBoolean   singleSource;
   witBoolean   propRtgSet [] = {WitTRUE, WitFALSE, WitTRUE, WitFALSE, WitTRUE};
   witBoolean * propRtgGet;
   witBoolean   propRoutingGet;

   witAddPart( theObj1Run, "PS1", WitPRODUCT );
   witAddPart (theObj1Run, "PS2", WitPRODUCT);
   witAddPart (theObj1Run, "MOBILE", WitPRODUCT);
   witAddPart( theObj1Run, "CompA", WitRAW );
   witSetPartSupplyVol( theObj1Run, "CompA", supplyVolCompB );

   witAddPart( theObj1Run, "CompB", WitRAW );
   witSetPartSupplyVol( theObj1Run, "CompB", supplyVolCompB );

   witAddPart( theObj1Run, "CompC", WitRAW );
   witSetPartSupplyVol( theObj1Run, "CompC", supplyVolCompC );

   witAddPart( theObj1Run, "TestCap", WitCAPACITY );
   witSetPartSupplyVol( theObj1Run, "TestCap", supplyVolCap );

   witSetPartObj1ProdCost     (theObj1Run, "PS1", obj1ProdCost);
   witSetPartObj1ScrapCost    (theObj1Run, "PS1", scrapCost);
   witSetPartObj1ScrapCostDbl (theObj1Run, "PS1", scrapCostDbl);
   witSetPartScrapCost        (theObj1Run, "PS1", scrapCost);
   witSetPartScrapCostDbl     (theObj1Run, "PS1", scrapCostDbl);
   witSetPartStockCost        (theObj1Run, "PS1", stockCost);
   witSetPartStockCostDbl     (theObj1Run, "PS1", stockCostDbl);
   witSetPartObj1StockCost    (theObj1Run, "PS1", stockCost);
   witSetPartObj1StockCostDbl (theObj1Run, "PS1", stockCostDbl);

   witSetPartSingleSource (theObj1Run, "PS1", WitTRUE);
   witSetPartPropRtg      (theObj1Run, "PS1", propRtgSet);
   witSetPartPropRouting  (theObj1Run, "PS2", WitTRUE);

   witSetOperationIncLotSize     (theObj1Run, "PS1", incLotSize);
   witSetOperationMinLotSize     (theObj1Run, "PS1", minLotSize);
   witSetOperationIncLotSize     (theObj1Run, "PS2", incLotSize);

   {  /* verify get obj1 product cost */
   float *cost;
   witGetPartObj1ProdCost( theObj1Run, "PS1", &cost );
   compareFltVecs( cost, obj1ProdCost, nPeriods );
   witFree (cost);
   }

   {  /* verify get scrap cost */
   float *cost;
   witGetPartScrapCost( theObj1Run, "PS1", &cost );
   compareFltVecs( cost, scrapCost, nPeriods );
   witFree (cost);
   }

   {  /* verify get scrap cost, double */
   double *cost;
   witGetPartScrapCostDbl( theObj1Run, "PS1", &cost );
   compareDblVecs( cost, scrapCostDbl, nPeriods );
   witFree (cost);
   }

   {  /* verify get obj1 scrap cost */
   float *cost;
   witGetPartObj1ScrapCost( theObj1Run, "PS1", &cost );
   compareFltVecs( cost, scrapCost, nPeriods );
   witFree (cost);
   }

   {  /* verify get obj1 scrap cost, double */
   double *cost;
   witGetPartObj1ScrapCostDbl( theObj1Run, "PS1", &cost );
   compareDblVecs( cost, scrapCostDbl, nPeriods );
   witFree (cost);
   }

   {  /* verify get stock cost */
   float *cost;
   witGetPartStockCost( theObj1Run, "PS1", &cost );
   compareFltVecs( cost, stockCost, nPeriods );
   witFree (cost);
   }

   {  /* verify get stock cost, double */
   double *cost;
   witGetPartStockCostDbl( theObj1Run, "PS1", &cost );
   compareDblVecs( cost, stockCostDbl, nPeriods );
   witFree (cost);
   }

   {  /* verify get obj1 stock cost */
   float *cost;
   witGetPartObj1StockCost( theObj1Run, "PS1", &cost );
   compareFltVecs( cost, stockCost, nPeriods );
   witFree (cost);
   }

   {  /* verify get obj1 stock cost, double */
   double *cost;
   witGetPartObj1StockCostDbl( theObj1Run, "PS1", &cost );
   compareDblVecs( cost, stockCostDbl, nPeriods );
   witFree (cost);
   }

   witGetPartSingleSource (theObj1Run, "PS1", & singleSource);

   assert (singleSource);

   witGetPartPropRtg (theObj1Run, "PS1", & propRtgGet);

   compareIntVecs (propRtgGet, propRtgSet, nPeriods);

   witFree (propRtgGet);

   witGetPartPropRouting (theObj1Run, "PS2", & propRoutingGet);

   assert (propRoutingGet);
   }

   /* Demands */
   {
   float  demandVolPS1US       [] = { 500., 500., 500., 500., 500. };
   float  demandVolPS1AGP      [] = { 500., 500., 500., 500., 500. };
   float  demandVolPS2US       [] = { 500., 500.,1000.,1500.,1500. };
   float  demandVolMOBILEUS    [] = { 1000., 1000.,1000.,1000.,1000. };

   float  cumShipRewardPS1US   [] = { 50., 50., 40., 50., 50. };
   double cumShipRewardDblPS1US[] = { 50., 50., 40., 50., 50. };
   float  cumShipRewardPS1AGP  [] = { 150., 150., 140., 150., 150. };

   float  shipRewardPS1US      [] = { 5., 5., 4., 5., 5. };
   double shipRewardDblPS1US   [] = { 5., 5., 4., 5., 5. };
   float  shipRewardPS1AGP     [] = { 15., 15., 14., 15., 15. };

   witAddDemand( theObj1Run, "PS1", "US" );

   witSetDemandObj1CumShipReward( theObj1Run, "PS1", "US", 
        cumShipRewardPS1US );
   witSetDemandObj1CumShipRewardDbl( theObj1Run, "PS1", "US", 
        cumShipRewardDblPS1US );
   witSetDemandCumShipReward( theObj1Run, "PS1", "US", 
        cumShipRewardPS1US );
   witSetDemandCumShipRewardDbl( theObj1Run, "PS1", "US", 
        cumShipRewardDblPS1US );
   witSetDemandObj1ShipReward( theObj1Run, "PS1", "US", 
        shipRewardPS1US );
   witSetDemandObj1ShipRewardDbl( theObj1Run, "PS1", "US", 
        shipRewardDblPS1US );
   witSetDemandShipReward( theObj1Run, "PS1", "US", 
        shipRewardPS1US );
   witSetDemandShipRewardDbl( theObj1Run, "PS1", "US", 
        shipRewardDblPS1US );

   witAddDemand( theObj1Run, "PS1", "AGP");
   witAddDemand( theObj1Run, "PS2", "US");
   witAddDemand( theObj1Run, "MOBILE", "US");
   
   witSetDemandDemandVol (theObj1Run, "PS1",    "US",  demandVolPS1US);
   witSetDemandDemandVol (theObj1Run, "PS1",    "AGP", demandVolPS1AGP);
   witSetDemandDemandVol (theObj1Run, "PS2",    "US",  demandVolPS2US);
   witSetDemandDemandVol (theObj1Run, "MOBILE", "US",  demandVolMOBILEUS);

   /* Verify demand settings */
   {  
   float  * fv;
   double * dv;

   witGetDemandShipReward( theObj1Run, "PS1", "US", &fv ); 
   compareFltVecs( fv, shipRewardPS1US, nPeriods );
   witFree (fv);

   witGetDemandShipRewardDbl( theObj1Run, "PS1", "US", &dv ); 
   compareDblVecs( dv, shipRewardDblPS1US, nPeriods );
   witFree (dv);

   witGetDemandObj1ShipReward( theObj1Run, "PS1", "US", &fv ); 
   compareFltVecs( fv, shipRewardPS1US, nPeriods );
   witFree (fv);

   witGetDemandObj1ShipRewardDbl( theObj1Run, "PS1", "US", &dv ); 
   compareDblVecs( dv, shipRewardDblPS1US, nPeriods );
   witFree (dv);

   witGetDemandCumShipReward( theObj1Run, "PS1", "US", &fv ); 
   compareFltVecs( fv, cumShipRewardPS1US, nPeriods );
   witFree (fv);

   witGetDemandCumShipRewardDbl( theObj1Run, "PS1", "US", &dv ); 
   compareDblVecs( dv, cumShipRewardDblPS1US, nPeriods );
   witFree (dv);

   witGetDemandObj1CumShipReward( theObj1Run, "PS1", "US", &fv ); 
   compareFltVecs( fv, cumShipRewardPS1US, nPeriods );
   witFree (fv);

   witGetDemandObj1CumShipRewardDbl( theObj1Run, "PS1", "US", &dv ); 
   compareDblVecs( dv, cumShipRewardDblPS1US, nPeriods );
   witFree (dv);
   }
   }

   /* BOM's */
   {
   witBoolean   singleSource;
   witBoolean   propRtgSet[] = {WitFALSE, WitTRUE, WitTRUE, WitTRUE, WitFALSE};
   witBoolean * propRtgGet;
   witBoolean   propRoutingGet;

   witAddBomEntry( theObj1Run, "PS1", "CompA");
   witAddBomEntry(  theObj1Run, "PS1", "CompB");
   witAddBomEntry( theObj1Run, "PS1", "TestCap");

   witAddBomEntry( theObj1Run, "PS2", "CompA");
   witAddBomEntry( theObj1Run, "PS2", "CompB");
   witAddBomEntry( theObj1Run, "PS2", "CompC");
   witAddBomEntry( theObj1Run, "PS2", "TestCap");

   witAddBomEntry( theObj1Run, "MOBILE", "CompB");
   witAddBomEntry( theObj1Run, "MOBILE", "CompC");
   witAddBomEntry( theObj1Run, "MOBILE", "TestCap");

   witSetBomEntrySingleSource (theObj1Run, "PS1", 0, WitTRUE);

   witGetBomEntrySingleSource (theObj1Run, "PS1", 0, & singleSource);

   assert (singleSource);

   witSetBomEntryPropRtg     (theObj1Run, "PS2", 1,   propRtgSet);
   witGetBomEntryPropRtg     (theObj1Run, "PS2", 1, & propRtgGet);

   compareIntVecs (propRtgGet, propRtgSet, nPeriods);

   witFree (propRtgGet);

   witSetBomEntryPropRouting (theObj1Run, "PS1", 1,   WitTRUE);
   witGetBomEntryPropRouting (theObj1Run, "PS1", 1, & propRoutingGet);

   assert (propRoutingGet);
   }

   /* Part PS1 BOM substitute */
   {
   float  subCostCompA   []={ 1., 2., 3., 4., 5.};
   double subCostDblCompA[]={ 1., 2., 3., 4., 5.};
   float  subCostCompB   []={ 10., 20., 30., 40., 50.};
   float  offsetCompA    []={ 2., 2., 3., 3., 3.};
   double offsetDblCompA []={ 2., 2., 3., 3., 3.};

   witAddSubsBomEntry( theObj1Run, "PS1", 0, "CompA");
   witSetSubsBomEntryObj1SubCost( theObj1Run, "PS1", 0, 0, subCostCompA);
   witSetSubsBomEntryObj1SubCostDbl (
      theObj1Run, "PS1", 0, 0, subCostDblCompA);
   witSetSubsBomEntrySubCost( theObj1Run, "PS1", 0, 0, subCostCompA);
   witSetSubsBomEntrySubCostDbl (
      theObj1Run, "PS1", 0, 0, subCostDblCompA);

   witSetSubsBomEntryOffset     ( theObj1Run, "PS1", 0, 0, offsetCompA);
   witSetSubsBomEntryOffsetDbl  ( theObj1Run, "PS1", 0, 0, offsetDblCompA);

   witAddSubsBomEntry(  theObj1Run, "PS1", 1, "CompB");

   {  
   float *fv;
   witGetSubsBomEntrySubCost( theObj1Run, "PS1", 0, 0, &fv ); 
   compareFltVecs( fv, subCostCompA, nPeriods );
   witFree (fv);
   }

   {  
   double *dv;
   witGetSubsBomEntrySubCostDbl( theObj1Run, "PS1", 0, 0, &dv ); 
   compareDblVecs( dv, subCostDblCompA, nPeriods );
   witFree (dv);
   }

   {  
   float *fv;
   witGetSubsBomEntryObj1SubCost( theObj1Run, "PS1", 0, 0, &fv ); 
   compareFltVecs( fv, subCostCompA, nPeriods );
   witFree (fv);
   }

   {  
   double *dv;
   witGetSubsBomEntryObj1SubCostDbl( theObj1Run, "PS1", 0, 0, &dv ); 
   compareDblVecs( dv, subCostDblCompA, nPeriods );
   witFree (dv);
   }

   {
   float *fv;
   witGetSubsBomEntryOffset( theObj1Run, "PS1", 0, 0, &fv );
   compareFltVecs( fv, offsetCompA, nPeriods );
   witFree (fv);
   }

   {
   double *dv;
   witGetSubsBomEntryOffsetDbl( theObj1Run, "PS1", 0, 0, &dv );
   compareDblVecs( dv, offsetDblCompA, nPeriods );
   witFree (dv);
   }
   }

   {
   witBoolean theBool;

   witSetMipMode                (theObj1Run,                WitTRUE);
   witGetMipMode                (theObj1Run,              & theBool);

   assert (theBool);

   witSetOperationIntExecVols   (theObj1Run, "PS1",         WitTRUE);
   witGetOperationIntExecVols   (theObj1Run, "PS1",       & theBool);

   assert (theBool);

   witSetSubsBomEntryIntSubVols (theObj1Run, "PS1", 0, 0,   WitTRUE);
   witGetSubsBomEntryIntSubVols (theObj1Run, "PS1", 0, 0, & theBool);

   assert (theBool);

   witSetDemandIntShipVols      (theObj1Run, "PS1", "US",    WitTRUE);
   witGetDemandIntShipVols      (theObj1Run, "PS1", "US",  & theBool);

   assert (theBool);
   }

   testWriteReadData (theObj1Run, "obj1-test.data");

   witSetPreferCoin (theObj1Run, WitTRUE);

   /* Do objective 1 implosion */
   {
   if (coinEmbedded || cplexEmbedded)
      {
      witOptImplode (theObj1Run);

      remove ("solver.log");
      }
   else
      witHeurImplode (theObj1Run);

   witSetPreferCoin (theObj1Run, WitFALSE);

   witWriteExecSched( theObj1Run, WitSTDOUT, WitBSV );
   witWriteShipSched( theObj1Run, WitSTDOUT, WitBSV );
   }

   /* Test opt implosion with CPLEX, if possible. */

   if (cplexEmbedded)
      {
      int    cplexStatusCode;
      char * cplexStatusText;
      float  cplexMipBound;
      double cplexMipBoundDbl;
      float  cplexMipRelGap;
      double cplexMipRelGapDbl;

      witOptImplode         (theObj1Run);
         /**/
         /* CPLEX-specific test */

      witGetCplexStatusCode (theObj1Run, & cplexStatusCode);

      assert (cplexStatusCode == 101);

      witGetCplexStatusText (theObj1Run, & cplexStatusText);

      assert (equalStrings (cplexStatusText, "integer optimal solution"));

      witFree (cplexStatusText);

      witGetCplexMipBound    (theObj1Run, & cplexMipBound);
      witGetCplexMipBoundDbl (theObj1Run, & cplexMipBoundDbl);

      assert (fabs (cplexMipBound    - 369150) < .001);
      assert (fabs (cplexMipBoundDbl - 369150) < .001);

      witGetCplexMipRelGap    (theObj1Run, & cplexMipRelGap);
      witGetCplexMipRelGapDbl (theObj1Run, & cplexMipRelGapDbl);

      assert (fabs (cplexMipRelGap)    < 1.0e-6);
      assert (fabs (cplexMipRelGapDbl) < 1.0e-6);

      remove ("solver.log");

      witWriteExecSched   (theObj1Run, WitSTDOUT,   WitBSV);
      witWriteShipSched   (theObj1Run, WitSTDOUT,   WitBSV);
      }

   /* Generate priorites */
   { 
   witGeneratePriorities( theObj1Run );
   witHeurImplode( theObj1Run );
   }


   /* Test WitRun copying */
   {
   int lhsInt, rhsInt;
   WitRun * lhs;

   witNewRun (& lhs);
   witCopyData( lhs, theObj1Run );

   witGetNPeriods( lhs,        &lhsInt );
   witGetNPeriods( theObj1Run, &rhsInt );
   assert( lhsInt == rhsInt );

   witGetHashTableSize( lhs,        &lhsInt );
   witGetHashTableSize( theObj1Run, &rhsInt );
   assert( lhsInt == rhsInt );

   witDeleteRun( lhs );

   witCopyData (theObj1Run, theObj1Run);               
      /*                                             */
      /* Test of copying a WitRun to itself (no-op). */
   }

   witDeleteRun( theObj1Run );

   testOperationApis();
   testBopApis();
   testAddPart();
   testPartToArcApis ();
   testHeurAlloc ();
   testHeurAllocDbl ();
   testHeurAllocTwme ();
   testHeurAllocTwmeDbl ();
   testDeletion ();

   if (cplexEmbedded)
      testAccOptImp (WitFALSE);

   if (coinEmbedded)
      testAccOptImp (WitTRUE);

   testCumShipVol ();

   /* witFree, witMalloc */
   {
   void * mem = NULL;
   witReturnCode rc = witMalloc(123,&mem);
   assert(mem!=NULL);
   assert(rc==WitINFORMATIONAL_RC);
   witFree(mem);
   }

   witDeleteRun (NULL);

   testExtOpt          ();
   testCopyObjData1WR  ();
   testCopyObjData2WRs ();
   testShipLateAllowed ();
   testGetExpCycle     ();
   testStageByObject   ();
   testStageByPeriod   ();
   testMultiObjMode    ();
   testLeadTimeBounds  ();

   printf (
      "\n"
      "Normal termination of apiAll.\n"
      "\n");

   exit (0);
   }
   /* End of Main Program */

/*----------------------------------------------------------------------------*/
/* testStandAlone                                                             */
/*----------------------------------------------------------------------------*/

void testStandAlone ()
   {
   WitRun * theWitRun;

   witNewRun       (& theWitRun);
   witExecStandAlone (theWitRun, "diner.params");
   witDeleteRun      (theWitRun);
   }

/*----------------------------------------------------------------------------*/
/* setUpCoinEmbedded (WitRun * theWitRun)                                     */
/*                                                                            */
/* Sets coinEmbedded.                                                         */
/* Verifies that it has the required value indicated by the environment       */
/* variable API_ALL_COIN_REQ, if any.                                         */
/*----------------------------------------------------------------------------*/

void setUpCoinEmbedded (WitRun * theWitRun)
   {
   const char * coinReq;

   witGetCoinEmbedded (theWitRun, & coinEmbedded);

   printf ( "\n");

   if (coinEmbedded)
      printf ("This is a COIN-embedded build of WIT.\n");
   else
      printf ("This is a COIN-non-embedded build of WIT.\n");

   coinReq = getenv ("API_ALL_COIN_REQ");

   if (coinReq != NULL)
      {
      if (equalStrings (coinReq, "Embedded"))
         assert (coinEmbedded);

      if (equalStrings (coinReq, "Not Embedded"))
         assert (! coinEmbedded);
      }
   }

/*----------------------------------------------------------------------------*/
/* setUpCplexEmbedded (WitRun * theWitRun)                                    */
/*                                                                            */
/* Sets cplexEmbedded.                                                        */
/* Verifies that it has the required value indicated by the environment       */
/* variable API_ALL_CPLEX_REQ, if any.                                        */
/*----------------------------------------------------------------------------*/

void setUpCplexEmbedded (WitRun * theWitRun)
   {
   const char * cplexReq;

   witGetCplexEmbedded (theWitRun, & cplexEmbedded);

   printf ( "\n");

   if (cplexEmbedded)
      printf ("This is a CPLEX-embedded build of WIT.\n");
   else
      printf ("This is a CPLEX-non-embedded build of WIT.\n");

   cplexReq = getenv ("API_ALL_CPLEX_REQ");

   if (cplexReq != NULL)
      {
      if (equalStrings (cplexReq, "Embedded"))
         assert (cplexEmbedded);

      if (equalStrings (cplexReq, "Not Embedded"))
         assert (! cplexEmbedded);
      }
   }

/*----------------------------------------------------------------------------*/
/* testCplexParSpecs (theWitRun)                                              */
/*                                                                            */
/* Tests CPLEX parameter specifications.                                      */
/*----------------------------------------------------------------------------*/

void testCplexParSpecs (WitRun * theWitRun)
   {
   testClearCplexParSpecs   (theWitRun);

   testCplexParSpecAtts     (theWitRun);

   witAddIntCplexParSpec    (theWitRun, "SIMDISPLAY", 0);
   witAddIntCplexParSpec    (theWitRun, "ITLIM",      1000000);
   witAddDblCplexParSpec    (theWitRun, "EPRHS",      1.e-5);
   witAddDblCplexParSpecDbl (theWitRun, "EPOPT",      1.e-7);

   testGetIntCplexParSpec   (theWitRun);
   testGetDblCplexParSpec   (theWitRun);
   }

/*----------------------------------------------------------------------------*/
/* testClearCplexParSpecs (theWitRun)                                         */
/*                                                                            */
/* Tests witClearCplexParSpecs.                                               */
/*----------------------------------------------------------------------------*/

void testClearCplexParSpecs (WitRun * theWitRun)
   {
   int        theInt;
   witBoolean intSpecExists;

   witAddIntCplexParSpec    (theWitRun, "SIMDISPLAY",   2);

   witGetIntCplexParSpec    (theWitRun, "SIMDISPLAY", & intSpecExists,
                                                      & theInt);

   assert   (intSpecExists);
   assert   (theInt == 2);

   witClearCplexParSpecs    (theWitRun);
   
   witGetIntCplexParSpec    (theWitRun, "SIMDISPLAY", & intSpecExists,
                                                      & theInt);

   assert (! intSpecExists);
   }

/*----------------------------------------------------------------------------*/
/* testCplexParSpecAtts (theWitRun)                                           */
/*                                                                            */
/* Tests the attribute functions for CPLEX parameter specifications.          */
/*----------------------------------------------------------------------------*/

void testCplexParSpecAtts (WitRun * theWitRun)
   {
   char * theString;
   int    theInt;
   float  theFlt;
   double theDbl;

   witSetCplexParSpecName      (theWitRun, "LPMETHOD");
   witSetCplexParSpecIntVal    (theWitRun, 4);

   witSetCplexParSpecName      (theWitRun, "TILIM");
   witSetCplexParSpecDblVal    (theWitRun,  0.7);
   witSetCplexParSpecDblValDbl (theWitRun, 62.5);

   witGetCplexParSpecName      (theWitRun, & theString);
   witGetCplexParSpecIntVal    (theWitRun, & theInt);
   witGetCplexParSpecDblVal    (theWitRun, & theFlt);
   witGetCplexParSpecDblValDbl (theWitRun, & theDbl);

   assert (equalStrings (theString, "TILIM"));

   assert (theInt == 4);

   assert (fabs (theFlt - 62.5) < dblEps);
   assert (fabs (theDbl - 62.5) < dblEps);

   witFree (theString);
   }

/*----------------------------------------------------------------------------*/
/* testGetIntCplexParSpec (theWitRun)                                         */
/*                                                                            */
/* Tests witGetIntCplexParSpec.                                               */
/*----------------------------------------------------------------------------*/

void testGetIntCplexParSpec (WitRun * theWitRun)
   {
   int        theInt;
   witBoolean intSpecExists;

   /*-------------------------------------------------------------------------*/

   witGetIntCplexParSpec (theWitRun, "NONE",       & intSpecExists,
                                                   & theInt);

   assert (! intSpecExists);

   /*-------------------------------------------------------------------------*/

   witGetIntCplexParSpec (theWitRun, "EPOPT",      & intSpecExists,
                                                   & theInt);

   assert (! intSpecExists);

   /*-------------------------------------------------------------------------*/

   witGetIntCplexParSpec (theWitRun, "SIMDISPLAY", & intSpecExists,
                                                   & theInt);

   assert   (intSpecExists);

   assert   (theInt == 0);

   /*-------------------------------------------------------------------------*/

   witGetIntCplexParSpec (theWitRun, "ITLIM",      & intSpecExists,
                                                   & theInt);

   assert   (intSpecExists);

   assert   (theInt == 1000000);
   }

/*----------------------------------------------------------------------------*/
/* testGetDblCplexParSpec (theWitRun)                                         */
/*                                                                            */
/* Tests witGetDblCplexParSpec and witGetDblCplexParSpecDbl.                  */
/*----------------------------------------------------------------------------*/

void testGetDblCplexParSpec (WitRun * theWitRun)
   {
   float      theFlt;
   double     theDbl;
   witBoolean dblSpecExists;

   witGetDblCplexParSpec    (theWitRun, "NONE",       & dblSpecExists,
                                                      & theFlt);

   assert (! dblSpecExists);

   witGetDblCplexParSpec    (theWitRun, "SIMDISPLAY", & dblSpecExists,
                                                      & theFlt);

   assert (! dblSpecExists);

   witGetDblCplexParSpecDbl (theWitRun, "EPRHS",      & dblSpecExists,
                                                      & theDbl);

   assert   (dblSpecExists);

   assert   (fabs (theDbl - 1.e-5) < 1.e-9);
   }

/*----------------------------------------------------------------------------*/
/* testWriteReadData (theWitRun, fileName)                                    */
/*                                                                            */
/* Tests witWriteData and witReadData using a temporary WitRun.               */
/*----------------------------------------------------------------------------*/

void testWriteReadData (WitRun * theWitRun, const char * fileName)
   {
   WitRun * tempWitRun;

   witWriteData  (theWitRun, fileName);

   witNewRun   (& tempWitRun);

   witInitialize (tempWitRun);

   witReadData   (tempWitRun, fileName);

   witDeleteRun  (tempWitRun);

   remove                    (fileName);
   }

/*----------------------------------------------------------------------------*/

void testOperationApis ()
{
   int   t;
   WitRun * wr1;
   WitRun * wr2;

   char theAppData[] = "Contents of theAppData.";

   witNewRun (& wr1);
   witNewRun (& wr2);

   witInitialize( wr1 );
   witSetNPeriods( wr1, nPeriods );

   witInitialize( wr2 );
   witSetNPeriods( wr2, nPeriods );

   witAddOperation( wr1, "opn1" );
   witAddOperation( wr2, "opn1" );

   witAddPart (wr2, "part1", WitCAPACITY);
      /*                                             */
      /* Added to fulfill the >= 1 part requirement. */

   /* execBounds, double  */
   {
   double hlb[]={3., 4., 5., 4., 2.};
   double slb[]={4., 4., 5., 4., 3.};
   double hub[]={4., 4., 7., 4., 3.};
   double *hlb1, *slb1, *hub1;
   
   witSetOperationExecBoundsDbl( wr2, "opn1", hlb, slb, hub );
   witGetOperationExecBoundsDbl( wr2, "opn1", &hlb1, &slb1, &hub1 );

   compareDblVecs(hlb,hlb1, nPeriods);
   compareDblVecs(slb,slb1, nPeriods);
   compareDblVecs(hub,hub1, nPeriods);

   witFree (hlb1);
   witFree (slb1);
   witFree (hub1);
   }

   /* execBounds, float  */
   {
   float hlb[]={1., 2., 3., 2., 0.};
   float slb[]={2., 2., 3., 2., 1.};
   float hub[]={2., 2., 5., 2., 1.};
   float *hlb1, *slb1, *hub1;
   
   witSetOperationExecBounds( wr2, "opn1", hlb, slb, hub );
   witGetOperationExecBounds( wr2, "opn1", &hlb1, &slb1, &hub1 );

   compareFltVecs(hlb,hlb1, nPeriods);
   compareFltVecs(slb,slb1, nPeriods);
   compareFltVecs(hub,hub1, nPeriods);

   witFree (hlb1);
   witFree (slb1);
   witFree (hub1);
   }

   /* Changing the name of an operation. */

   {
   witSetOperationOperationName  (wr1, "opn1",      "Temp-Name");
   witSetOperationOperationName  (wr1, "Temp-Name", "opn1");
   }

   /* execPenalty */
   {
   float  theFlt;
   double theDbl;

   witSetOperationExecPenaltyDbl( wr2, "opn1", 272.89 );
   witGetOperationExecPenaltyDbl( wr2, "opn1", &theDbl );

   assert( fabs(theDbl - 272.89) < dblEps );

   witSetOperationExecPenalty( wr2, "opn1", 72.89 );
   witGetOperationExecPenalty( wr2, "opn1", &theFlt );

   assert( fabs(theFlt - 72.89) < dblEps );
   }

   /* execVol: double */
   {
   double dblVec[]={2., 2., 3., 2., 0.};
   double *dblVec1;

   witSetOperationExecVolDbl( wr2, "opn1", dblVec );
   witGetOperationExecVolDbl( wr2, "opn1", &dblVec1 );

   compareDblVecs(dblVec,dblVec1, nPeriods);

   witFree (dblVec1);
   }

   /* execVol: float */
   {
   float fltVec[]={2., 2., 3., 2., 0.};
   float *fltVec1;

   witSetOperationExecVol( wr2, "opn1", fltVec );
   witGetOperationExecVol( wr2, "opn1", &fltVec1 );

   compareFltVecs(fltVec,fltVec1, nPeriods);

   witFree (fltVec1);
   }

   /* twoLevelLotSizes */
   {
   witBoolean twoLevelLotSizes;

   witSetOperationTwoLevelLotSizes (wr2, "opn1",   WitTRUE);
   witGetOperationTwoLevelLotSizes (wr2, "opn1", & twoLevelLotSizes);

   assert (twoLevelLotSizes);
   }

   /* incLotSize */
   {
   float fltVec[]={2., 9., 3., 2., 1.};
   float *fltVec1;

   witSetOperationIncLotSize( wr2, "opn1", fltVec );
   witGetOperationIncLotSize( wr2, "opn1", &fltVec1 );

   compareFltVecs(fltVec,fltVec1, nPeriods);

   witFree (fltVec1);
   }


   /* incLotSize: double */
   {
   double   dblVec[]={2., 9., 3., 2., 1.};
   double * dblVec1;

   witSetOperationIncLotSizeDbl( wr2, "opn1", dblVec );
   witGetOperationIncLotSizeDbl( wr2, "opn1", &dblVec1 );

   compareDblVecs(dblVec,dblVec1, nPeriods);

   witFree (dblVec1);
   }


   /* minLotSize */
   {
   float fltVec[]={3., 9., 3., 2., 0.};
   float *fltVec1;

   witSetOperationMinLotSize( wr2, "opn1", fltVec );
   witGetOperationMinLotSize( wr2, "opn1", &fltVec1 );

   compareFltVecs(fltVec,fltVec1, nPeriods);

   witFree (fltVec1);
   }

   /* minLotSize: double */
   {
   double   dblVec[]={3., 9., 3., 2., 0.};
   double * dblVec1;

   witSetOperationMinLotSizeDbl( wr2, "opn1", dblVec );
   witGetOperationMinLotSizeDbl( wr2, "opn1", &dblVec1 );

   compareDblVecs(dblVec,dblVec1, nPeriods);

   witFree (dblVec1);
   }

   /* lotSize2Thresh */
   {
   float fltVec[]={13., 19., 13., 12., 10.};
   float *fltVec1;

   witSetOperationLotSize2Thresh( wr2, "opn1", fltVec );
   witGetOperationLotSize2Thresh( wr2, "opn1", &fltVec1 );

   compareFltVecs(fltVec,fltVec1, nPeriods);

   witFree (fltVec1);
   }

   /* lotSize2Thresh */
   {
   double   dblVec[]={13., 19., 13., 12., 10.};
   double * dblVec1;

   witSetOperationLotSize2ThreshDbl( wr2, "opn1", dblVec );
   witGetOperationLotSize2ThreshDbl( wr2, "opn1", &dblVec1 );

   compareDblVecs(dblVec,dblVec1, nPeriods);

   witFree (dblVec1);
   }

   /* minLotSize2 */
   {
   float fltVec[]={23., 29., 23., 22., 20.};
   float *fltVec1;

   witSetOperationMinLotSize2( wr2, "opn1", fltVec );
   witGetOperationMinLotSize2( wr2, "opn1", &fltVec1 );

   compareFltVecs(fltVec,fltVec1, nPeriods);

   witFree (fltVec1);
   }

   /* minLotSize2: double */
   {
   double   dblVec[]={23., 29., 23., 22., 20.};
   double * dblVec1;

   witSetOperationMinLotSize2Dbl( wr2, "opn1", dblVec );
   witGetOperationMinLotSize2Dbl( wr2, "opn1", &dblVec1 );

   compareDblVecs(dblVec,dblVec1, nPeriods);

   witFree (dblVec1);
   }

   /* incLotSize2 */
   {
   float fltVec[]={12., 19., 13., 12., 11.};
   float *fltVec1;

   witSetOperationIncLotSize2( wr2, "opn1", fltVec );
   witGetOperationIncLotSize2( wr2, "opn1", &fltVec1 );

   compareFltVecs(fltVec,fltVec1, nPeriods);

   witFree (fltVec1);
   }

   /* incLotSize2: double */
   {
   double   dblVec[]={12., 19., 13., 12., 11.};
   double * dblVec1;

   witSetOperationIncLotSize2Dbl( wr2, "opn1", dblVec );
   witGetOperationIncLotSize2Dbl( wr2, "opn1", &dblVec1 );

   compareDblVecs(dblVec,dblVec1, nPeriods);

   witFree (dblVec1);
   }

   /* execCost */
   {
   float fltVec[]={4., 9., 3., 2., 0.};
   float *fltVec1;

   witSetOperationObj1ExecCost (wr1, "opn1",   fltVec);
   witSetOperationExecCost     (wr1, "opn1",   fltVec);

   witGetOperationExecCost     (wr1, "opn1", & fltVec1);

   compareFltVecs (fltVec, fltVec1, nPeriods);

   witFree (fltVec1);

   witGetOperationObj1ExecCost (wr1, "opn1", & fltVec1);

   compareFltVecs (fltVec, fltVec1, nPeriods);

   witFree (fltVec1);
   }

   /* execCost */
   {
   double   dblVec[]={4., 9., 3., 2., 0.};
   double * dblVec1;

   witSetOperationObj1ExecCostDbl (wr1, "opn1",   dblVec);
   witSetOperationExecCostDbl     (wr1, "opn1",   dblVec);

   witGetOperationExecCostDbl     (wr1, "opn1", & dblVec1);

   compareDblVecs (dblVec, dblVec1, nPeriods);

   witFree (dblVec1);

   witGetOperationObj1ExecCostDbl (wr1, "opn1", & dblVec1);

   compareDblVecs (dblVec, dblVec1, nPeriods);

   witFree (dblVec1);
   }

   /* yieldRate */
   {
   float    trueYieldRate1   [] = {1.0, 0.876, 0.874, 0.01, 0.0};
   double   trueYieldRateDbl1[] = {1.0, 0.876, 0.874, 0.01, 0.0};
   int      trueYield1       [] = {100,   88,    87,     1,   0};

   int      trueYield2       [] = {   2,   0,    3,   0,   99};
   float    trueYieldRate2   [] = {0.02, 0.0, 0.03, 0.0, 0.99};

   float *  witYieldRate;
   double * witYieldRateDbl;
   int *    witYield;

   witSetOperationYieldRate    (wr2, "opn1",  trueYieldRate1);
   witSetOperationYieldRateDbl (wr2, "opn1",  trueYieldRateDbl1);
   witGetOperationYieldRate    (wr2, "opn1", & witYieldRate);
   witGetOperationYieldRateDbl (wr2, "opn1", & witYieldRateDbl);
   witGetOperationYield        (wr2, "opn1", & witYield);

   compareFltVecs (witYieldRate,    trueYieldRate1,    nPeriods);
   compareDblVecs (witYieldRateDbl, trueYieldRateDbl1, nPeriods);
   compareIntVecs (witYield,        trueYield1,        nPeriods);

   witFree (witYieldRate);
   witFree (witYieldRateDbl);
   witFree (witYield);

   witSetOperationYield        (wr2, "opn1",  trueYield2);
   witGetOperationYield        (wr2, "opn1", & witYield);
   witGetOperationYieldRate    (wr2, "opn1", & witYieldRate);

   compareIntVecs (witYield,        trueYield2,     nPeriods);
   compareFltVecs (witYieldRate,    trueYieldRate2, nPeriods);

   witFree (witYield);
   witFree (witYieldRate);
   }

   /* appData */
   {
   void * ptr;
   witSetOperationAppData( wr2, "opn1", theAppData);
   witGetOperationAppData( wr2, "opn1", &ptr);
   assert( ptr==theAppData );
   }

   /* executable */
   {
   witBoolean * executable;

   witBoolean   correctExecutable[] =
      {
      WitTRUE,
      WitFALSE,
      WitTRUE,
      WitFALSE,
      WitTRUE};

   witGetOperationExecutable (wr2, "opn1", &executable);

   compareIntVecs (executable, correctExecutable, nPeriods);

   witFree ( executable);
   }

   /* witGetOperations */
   {
   int lenList;
   char ** operationList;
   witGetOperations( wr2, &lenList, &operationList );
   assert( lenList == 1 );
   assert( equalStrings (operationList[0],"opn1"));
   witFree ( operationList[0] );
   witFree ( operationList );
   }

   {
   witBoolean exists;

   witGetOperationExists( wr2, "opn1", &exists );
   assert( exists );

   witGetOperationExists( wr2, "xyzzy", &exists );
   assert( !exists );
   }

   {
   WitRun *   copyWitRun;
   witBoolean theBool;

   witNewRun           (& copyWitRun);
   witCopyData           (copyWitRun, wr2);

   witSetHighPrecisionWD (copyWitRun, WitTRUE);
   witGetHighPrecisionWD (copyWitRun, & theBool);

   assert (theBool);

   witWriteData          (copyWitRun, "opnTest.data");
   witInitialize         (copyWitRun);
   witReadData           (copyWitRun, "opnTest.data");
   remove                (            "opnTest.data");
   witWriteData          (copyWitRun, WitSTDOUT);
   witDeleteRun          (copyWitRun);
   }

   witDeleteRun (wr1);
   witDeleteRun (wr2);
}

void testBopApis() 
{
   int   t;
   WitRun *wr1;

   char theAppData[] = "Contents of theAppData.";

   witNewRun (& wr1);

   witInitialize( wr1 );
   witSetWit34Compatible( wr1, WitTRUE );
   witSetNPeriods( wr1, nPeriods );

   witAddOperation( wr1, "opn1" );
   witAddPart( wr1, "part1", WitPRODUCT );
   witAddBopEntry( wr1, "opn1", "part1" );

   /* expAllowed and byProduct */
   {
   witBoolean bool;

   witSetBopEntryExpAllowed (wr1, "opn1", 0, WitFALSE);

   witGetBopEntryExpAllowed (wr1, "opn1", 0, & bool);
   assert (! bool);

   witGetBopEntryByProduct  (wr1, "opn1", 0, & bool);
   assert (bool);

   witSetBopEntryByProduct  (wr1, "opn1", 0, WitFALSE);

   witGetBopEntryByProduct  (wr1, "opn1", 0, & bool);
   assert (! bool);

   witGetBopEntryExpAllowed (wr1, "opn1", 0, & bool);
   assert (bool);
   }

   /* meSelSplit */
   {
   witBoolean bool;

   witSetBopEntryMeSelSplit (wr1, "opn1", 0, WitTRUE);

   witGetBopEntryMeSelSplit (wr1, "opn1", 0, & bool);
   assert (bool);
   }

   /* earliestPeriod */
   {
   int per;

   witSetBopEntryEarliestPeriod( wr1, "opn1", 0, 4 );
   witGetBopEntryEarliestPeriod( wr1, "opn1", 0, &per );
   assert( per==4 );

   witSetBopEntryEarliestPeriod( wr1, "opn1", 0, 1 );
   witGetBopEntryEarliestPeriod( wr1, "opn1", 0, &per );
   assert( per==1 );
   }

   /* latestPeriod */
   {
   int per;

   witSetBopEntryLatestPeriod( wr1, "opn1", 0, 1 );
   witGetBopEntryLatestPeriod( wr1, "opn1", 0, &per );
   assert( per==1 );

   witSetBopEntryLatestPeriod( wr1, "opn1", 0, 3 );
   witGetBopEntryLatestPeriod( wr1, "opn1", 0, &per );
   assert( per==3 );
   }

   /* offset */
   {
   float fltVec[]={4., 9., 0., 2., 0.};
   float *fltVec1;

   witSetBopEntryOffset( wr1, "opn1", 0, fltVec );
   witGetBopEntryOffset( wr1, "opn1", 0, &fltVec1 );

   compareFltVecs(fltVec,fltVec1, nPeriods);

   witFree (fltVec1);
   }

   /* offset: double */
   {
   double   dblVec[]={4., 9., 0., 2., 0.};
   double * dblVec1;

   witSetBopEntryOffsetDbl( wr1, "opn1", 0, dblVec );
   witGetBopEntryOffsetDbl( wr1, "opn1", 0, &dblVec1 );

   compareDblVecs(dblVec,dblVec1, nPeriods);

   witFree (dblVec1);
   }

   /* productRate & prodRate */
   {
   float   prodRateRet;
   float   productRateEnt[] = {2.0, 2.1, 2.2, 2.3, 2.4};
   float * productRateRet;

   witSetBopEntryProductRate (wr1, "opn1", 0,   productRateEnt);
   witGetBopEntryProdRate    (wr1, "opn1", 0, & prodRateRet);

   assert (fabs (prodRateRet - 2.0) < dblEps);

   witGetBopEntryProductRate (wr1, "opn1", 0, & productRateRet);

   compareFltVecs (productRateRet, productRateEnt, nPeriods);

   witFree (productRateRet);

   witSetBopEntryProdRate (wr1, "opn1", 0, 1.);
   witGetBopEntryProdRate (wr1, "opn1", 0, & prodRateRet);

   assert (fabs (prodRateRet - 1.0) < dblEps);

   witGetBopEntryProductRate (wr1, "opn1", 0, & productRateRet);

   assert (fabs (productRateRet[2] - 1.0) < dblEps);

   witFree (productRateRet);
   }

   /* productRate: double */
   {
   double   productRateDblEnt[] = {2.0, 2.1, 2.2, 2.3, 2.4};
   double * productRateDblRet;

   witSetBopEntryProductRateDbl (wr1, "opn1", 0,   productRateDblEnt);

   witGetBopEntryProductRateDbl (wr1, "opn1", 0, & productRateDblRet);

   compareDblVecs (productRateDblRet, productRateDblEnt, nPeriods);

   witFree (productRateDblRet);
   }

   /* expAversion */
   {
   float  aversion;
   double aversionDbl;

   witSetBopEntryExpAversion (wr1, "opn1", 0, -4.0);
   witGetBopEntryExpAversion (wr1, "opn1", 0, & aversion);

   assert (fabs (aversion + 4.0) < dblEps);

   witSetBopEntryExpAversionDbl (wr1, "opn1", 0, 53.0);
   witGetBopEntryExpAversionDbl (wr1, "opn1", 0, & aversionDbl);

   assert (fabs (aversionDbl - 53.0) < dblEps);

   witSetBopEntryExpAversion (wr1, "opn1", 0, 0.0);
   witGetBopEntryExpAversion (wr1, "opn1", 0, & aversion);

   assert (fabs (aversion - 0.0) < dblEps);
   }

   /* routingShare */
   {
   float   goodRoutingShare[] = {5.0, 5.0, 1.0, 5.0, 5.0};
   float * routingShare;

   witSetBopEntryRoutingShare (wr1, "opn1", 0, goodRoutingShare);

   witGetBopEntryRoutingShare (wr1, "opn1", 0, & routingShare);

   compareFltVecs (routingShare, goodRoutingShare, nPeriods);

   witFree (routingShare);
   }

   /* routingShare: double */
   {
   double   goodRoutingShareDbl[] = {5.0, 5.0, 1.0, 5.0, 5.0};
   double * routingShareDbl;

   witSetBopEntryRoutingShareDbl (wr1, "opn1", 0, goodRoutingShareDbl);

   witGetBopEntryRoutingShareDbl (wr1, "opn1", 0, & routingShareDbl);

   compareDblVecs (routingShareDbl, goodRoutingShareDbl, nPeriods);

   witFree (routingShareDbl);
   }

   /* producedPartName */
   {
   char *s;
   witGetBopEntryProducedPart( wr1, "opn1", 0, &s );
   assert( equalStrings(s,"part1") );
   witFree (s);
   }

   /* nBopEntries */
   {
   int nBopEntries;
   witGetOperationNBopEntries( wr1, "opn1", &nBopEntries );
   assert( nBopEntries == 1 );
   }

   /* bopEntry appData */
   {
   void * ptr;
   witSetBopEntryAppData( wr1, "opn1", 0, theAppData);
   witGetBopEntryAppData( wr1, "opn1", 0, &ptr);
   assert( ptr==theAppData );
   }

   /* impactPeriod */
   {
   int * impactPeriod;
   int   correctImpactPeriod[] = { -1, -1, 2, 1, -1};

   witGetBopEntryImpactPeriod (wr1, "opn1", 0, & impactPeriod);

   compareIntVecs (impactPeriod, correctImpactPeriod, nPeriods);

   witFree (impactPeriod);
   }

   witDeleteRun( wr1 );

}


void testAddPart()
{
   int   t;
   WitRun *wr1;

   witNewRun (& wr1);

   witInitialize( wr1 );
   witSetNPeriods( wr1, nPeriods );
   witSetWit34Compatible( wr1, WitTRUE );

   witAddPart( wr1, "partRaw",     WitRAW );
   witAddPart ( wr1, "partRawVa", WitRAW);

   witAddPart( wr1, "partCap",     WitCAPACITY );
   witAddPart( wr1, "partCapVa", WitCAPACITY);

   witAddPart( wr1, "partPrd",     WitPRODUCT );
   witAddPart( wr1, "partPrdVa", WitPRODUCT);
   {
   float obj1ProdCost[]  ={ 1., 2., 3., 4., 5.5 };
   float cycleTime[]={ 2., 3., 3., 4., 5.5 };
   int   yield[] = { 90, 91, 92, 93, 94 };
   witAddPart( wr1, "partPrdVa1", WitPRODUCT);
   }

   witAddPart( wr1, "partMat",     WitMATERIAL );
   witAddPart( wr1, "partMatVa", WitMATERIAL);

   witAddPartWithOperation( wr1, "partWithOperation" );

   witInitialize( wr1 );
   witSetNPeriods( wr1, nPeriods );
   witSetWit34Compatible( wr1, WitFALSE );

   witAddPart( wr1, "partCap",     WitCAPACITY );
   witAddPart( wr1, "partCapVa", WitCAPACITY);

   witAddPart( wr1, "partMat",     WitMATERIAL );
   witAddPart( wr1, "partMatVa", WitMATERIAL);

   witAddPartWithOperation( wr1, "partWithOperation" );

   witDeleteRun( wr1 );
}

/*----------------------------------------------------------------------------*/
/* testPartToArcApis                                                          */
/* Tests the API functions that find the arcs associated with a part.         */
/*----------------------------------------------------------------------------*/

void testPartToArcApis ()
   {
   WitRun * theWitRun;
   char *   theOperationName;
   int      nArcs;
   int      theBomEntryIndex;
   int      theSubEntryIndex;
   int      theBopEntryIndex;

   printf (
      "\n"
      "--- Testing the API functions that find the "
          "arcs associated with a part. ---\n"
      "\n");

   /*---------------------*/
   /* Set up the problem. */
   /*---------------------*/

   witNewRun   (& theWitRun);
   witInitialize (theWitRun);

   witAddPart (theWitRun, "PA", WitMATERIAL);
   witAddPart (theWitRun, "PB", WitMATERIAL);
   witAddPart (theWitRun, "PC", WitMATERIAL);

   witAddOperation (theWitRun, "OA");
   witAddOperation (theWitRun, "OB");

   witAddBomEntry (theWitRun, "OA", "PA");
   witAddBomEntry (theWitRun, "OB", "PA");

   witAddSubsBomEntry (theWitRun, "OA", 0, "PB");
   witAddSubsBomEntry (theWitRun, "OB", 0, "PB");
   witAddSubsBomEntry (theWitRun, "OA", 0, "PB");

   witAddBopEntry (theWitRun, "OA", "PC");
   witAddBopEntry (theWitRun, "OB", "PC");

   /*------------*/
   /* Run tests. */
   /*------------*/

   witGetPartNConsumingBomEntries (theWitRun, "PA", &nArcs);

   assert (nArcs == 2);

   witGetPartNConsumingSubsBomEntries (theWitRun, "PB", &nArcs);

   assert (nArcs == 3);

   witGetPartNProducingBopEntries (theWitRun, "PC", &nArcs);

   assert (nArcs == 2);

   witGetPartConsumingBomEntry (
      theWitRun,
      "PA",
      1,
      & theOperationName,
      & theBomEntryIndex);

   assert (equalStrings (theOperationName, "OB"));

   witFree (theOperationName);

   assert (theBomEntryIndex == 0);

   witGetPartConsumingSubsBomEntry (
      theWitRun,
      "PB",
      2,
      & theOperationName,
      & theBomEntryIndex,
      & theSubEntryIndex);

   assert (equalStrings (theOperationName, "OA"));

   witFree (theOperationName);

   assert (theBomEntryIndex == 0);

   assert (theSubEntryIndex == 1);

   witGetPartProducingBopEntry (
      theWitRun,
      "PC",
      1,
      & theOperationName,
      & theBopEntryIndex);

   assert (equalStrings (theOperationName, "OB"));

   witFree (theOperationName);

   assert (theBopEntryIndex == 0);

   witDeleteRun (theWitRun);
   }

/*----------------------------------------------------------------------------*/
/* testHeurAlloc                                                              */
/* Tests heuristic allocation and equitable allocation in float mode.         */
/*----------------------------------------------------------------------------*/

void testHeurAlloc ()
   {
   WitRun *   theWitRun;
   float      incVol;
   witBoolean active;
   witBoolean postproc;

   printf (
      "\n"
      "--- Testing Heuristic Allocation in Float Mode ---\n"
      "\n");

   witNewRun   (& theWitRun);
   witInitialize (theWitRun);

   witReadData (theWitRun, "diner.data");

   witSetEquitability (theWitRun, 100);

   witGetHeurAllocActive (theWitRun, & active);

   assert (! active);

   witStartHeurAlloc  (theWitRun);

   witGetHeurAllocActive (theWitRun, & active);

   assert (active);

   witSetAccAfterOptImp (theWitRun, WitFALSE);

   witGetHeurAllocActive (theWitRun, & active);

   assert (! active);

   witStartHeurAlloc  (theWitRun);

      {
      const char * const demandedPartNameList[] = {
         "Ham_and_Egg_Sandwich",
         "Ham_and_Cheese_Sandwich",
         "Ham_Sandwich"};

      const char * const demandNameList[] = {
         "demand",
         "demand",
         "demand"};

      const int shipPeriodList[] = {1, 0, 1};

      const float desIncVolList[] = {7.0, 6.0, 15.0};

      float * incVolList;

      witEqHeurAlloc (
         theWitRun,
         3,
         demandedPartNameList,
         demandNameList,
         shipPeriodList,
         desIncVolList,
         & incVolList);

      assert (incVolList[0] == 4.0);
      assert (incVolList[1] == 4.0);
      assert (incVolList[2] == 9.0);

      witFree (incVolList);
      }

   witIncHeurAlloc (
      theWitRun,
      "Grilled_Cheese_Sandwich",
      "demand",
      1,
      9.0,
      & incVol);

   assert (incVol == 1.0);

   witIncHeurAlloc (
      theWitRun,
      "Plain_Sandwich",
      "demand",
      0,
      10.0,
      & incVol);

   assert (incVol == 4.0);

   witFinishHeurAlloc (theWitRun);

   witGetHeurAllocActive (theWitRun, & active);

   assert (! active);

   witGetPostprocessed (theWitRun, & postproc);

   assert (postproc);

   witDeleteRun (theWitRun);
   }

/*----------------------------------------------------------------------------*/
/* testHeurAllocDbl                                                           */
/* Tests heuristic allocation and equitable allocation in double mode.        */
/*----------------------------------------------------------------------------*/

void testHeurAllocDbl ()
   {
   WitRun *   theWitRun;
   double     incVol;
   witBoolean active;
   witBoolean postproc;

   printf (
      "\n"
      "--- Testing Heuristic Allocation in Double Mode ---\n"
      "\n");

   witNewRun   (& theWitRun);
   witInitialize (theWitRun);

   witReadData (theWitRun, "diner.data");

   witSetEquitability (theWitRun, 100);

   witGetHeurAllocActive (theWitRun, & active);

   assert (! active);

   witStartHeurAlloc  (theWitRun);

   witGetHeurAllocActive (theWitRun, & active);

   assert (active);

   witSetAccAfterOptImp (theWitRun, WitFALSE);

   witGetHeurAllocActive (theWitRun, & active);

   assert (! active);

   witStartHeurAlloc  (theWitRun);

      {
      const char * const demandedPartNameList[] = {
         "Ham_and_Egg_Sandwich",
         "Ham_and_Cheese_Sandwich",
         "Ham_Sandwich"};

      const char * const demandNameList[] = {
         "demand",
         "demand",
         "demand"};

      const int shipPeriodList[] = {1, 0, 1};

      const double desIncVolList[] = {7.0, 6.0, 15.0};

      double * incVolList;

      witEqHeurAllocDbl (
         theWitRun,
         3,
         demandedPartNameList,
         demandNameList,
         shipPeriodList,
         desIncVolList,
         & incVolList);

      assert (incVolList[0] == 4.0);
      assert (incVolList[1] == 4.0);
      assert (incVolList[2] == 9.0);

      witFree (incVolList);
      }

   witIncHeurAllocDbl (
      theWitRun,
      "Grilled_Cheese_Sandwich",
      "demand",
      1,
      9.0,
      & incVol);

   assert (incVol == 1.0);

   witIncHeurAllocDbl (
      theWitRun,
      "Plain_Sandwich",
      "demand",
      0,
      10.0,
      & incVol);

   assert (incVol == 4.0);

   witFinishHeurAlloc (theWitRun);

   witGetHeurAllocActive (theWitRun, & active);

   assert (! active);

   witGetPostprocessed (theWitRun, & postproc);

   assert (postproc);

   witDeleteRun (theWitRun);
   }

/*----------------------------------------------------------------------------*/
/* testHeurAllocTwme                                                          */
/* Tests heuristic allocation and equitable allocation in float two-way       */
/* multi-exec mode.                                                           */
/*----------------------------------------------------------------------------*/

void testHeurAllocTwme ()
   {
   WitRun *   theWitRun;
   float      incVol;
   witBoolean active;
   witBoolean twoWayMultiExec;
   witBoolean multiExec;

   printf (
      "\n"
      "--- Testing Heuristic Allocation in Float Two-Way Multi-Exec Mode ---\n"
      "\n");

   witNewRun   (& theWitRun);
   witInitialize (theWitRun);

   witReadData (theWitRun, "diner.data");

   witSetEquitability (theWitRun, 100);

   witSetTwoWayMultiExec (theWitRun, WitTRUE);

   witGetTwoWayMultiExec (theWitRun, & twoWayMultiExec);

   assert (twoWayMultiExec);

   witGetMultiExec (theWitRun, & multiExec);

   assert (multiExec);

   witGetHeurAllocActive (theWitRun, & active);

   assert (! active);

   witStartHeurAlloc  (theWitRun);

      {
      const char * const demandedPartNameList[] = {
         "Ham_and_Egg_Sandwich",
         "Ham_and_Cheese_Sandwich",
         "Ham_Sandwich"};

      const char * const demandNameList[] = {
         "demand",
         "demand",
         "demand"};

      const int shipPeriodList[] = {1, 0, 1};

      const float desIncVolList[] = {7.0, 6.0, 15.0};

      float * incVolList;

      witBoolean asapMultiExecList[] = {WitTRUE, WitFALSE, WitTRUE};

      witEqHeurAllocTwme (
         theWitRun,
         3,
         demandedPartNameList,
         demandNameList,
         shipPeriodList,
         desIncVolList,
         & incVolList,
         asapMultiExecList);

      assert (incVolList[0] == 4.0);
      assert (incVolList[1] == 4.0);
      assert (incVolList[2] == 9.0);

      witFree (incVolList);
      }

   witIncHeurAllocTwme (
      theWitRun,
      "Grilled_Cheese_Sandwich",
      "demand",
      1,
      9.0,
      & incVol,
      WitTRUE);

   assert (incVol == 1.0);

   witIncHeurAllocTwme (
      theWitRun,
      "Plain_Sandwich",
      "demand",
      0,
      10.0,
      & incVol,
      WitFALSE);

   assert (incVol == 4.0);

   witFinishHeurAlloc (theWitRun);

   witGetHeurAllocActive (theWitRun, & active);

   assert (! active);

   witDeleteRun (theWitRun);
   }

/*----------------------------------------------------------------------------*/
/* testHeurAllocTwmeDbl                                                       */
/* Tests heuristic allocation and equitable allocation in double two-way      */
/* multi-exec mode.                                                           */
/*----------------------------------------------------------------------------*/

void testHeurAllocTwmeDbl ()
   {
   WitRun *   theWitRun;
   double     incVol;
   witBoolean active;
   witBoolean twoWayMultiExec;
   witBoolean multiExec;

   printf (
      "\n"
      "--- Testing Heuristic Allocation in Double Two-Way Multi-Exec Mode ---\n"
      "\n");

   witNewRun   (& theWitRun);
   witInitialize (theWitRun);

   witReadData (theWitRun, "diner.data");

   witSetEquitability (theWitRun, 100);

   witSetTwoWayMultiExec (theWitRun, WitTRUE);

   witGetTwoWayMultiExec (theWitRun, & twoWayMultiExec);

   assert (twoWayMultiExec);

   witGetMultiExec (theWitRun, & multiExec);

   assert (multiExec);

   witGetHeurAllocActive (theWitRun, & active);

   assert (! active);

   witStartHeurAlloc  (theWitRun);

      {
      const char * const demandedPartNameList[] = {
         "Ham_and_Egg_Sandwich",
         "Ham_and_Cheese_Sandwich",
         "Ham_Sandwich"};

      const char * const demandNameList[] = {
         "demand",
         "demand",
         "demand"};

      const int shipPeriodList[] = {1, 0, 1};

      const double desIncVolList[] = {7.0, 6.0, 15.0};

      double * incVolList;

      witBoolean asapMultiExecList[] = {WitTRUE, WitFALSE, WitTRUE};

      witEqHeurAllocTwmeDbl (
         theWitRun,
         3,
         demandedPartNameList,
         demandNameList,
         shipPeriodList,
         desIncVolList,
         & incVolList,
         asapMultiExecList);

      assert (incVolList[0] == 4.0);
      assert (incVolList[1] == 4.0);
      assert (incVolList[2] == 9.0);

      witFree (incVolList);
      }

   witIncHeurAllocTwmeDbl (
      theWitRun,
      "Grilled_Cheese_Sandwich",
      "demand",
      1,
      9.0,
      & incVol,
      WitTRUE);

   assert (incVol == 1.0);

   witIncHeurAllocTwmeDbl (
      theWitRun,
      "Plain_Sandwich",
      "demand",
      0,
      10.0,
      & incVol,
      WitFALSE);

   assert (incVol == 4.0);

   witFinishHeurAlloc (theWitRun);

   witGetHeurAllocActive (theWitRun, & active);

   assert (! active);

   witDeleteRun (theWitRun);
   }

/*----------------------------------------------------------------------------*/
/* testDeletion                                                               */
/* Tests item deletion.                                                       */
/*----------------------------------------------------------------------------*/

void testDeletion ()
   {
   WitRun * theWitRun;

   printf (
      "\n"
      "--- Testing Item Deletion ---\n"
      "\n");

   witNewRun (& theWitRun);

   witSetMesgPrintNumber (theWitRun,
                          WitTRUE,
                          WitINFORMATIONAL_MESSAGES,
                          WitFALSE);

   witInitialize         (theWitRun);
   witSetWit34Allowed    (theWitRun, WitFALSE);
   defProfDelTest        (theWitRun);
   selItemsDelTest       (theWitRun);
   copyWriteDelTest      (theWitRun);
   witDisplayData        (theWitRun, WitSTDOUT);
   checkSelDelTest       (theWitRun);
   witPurgeData          (theWitRun);
   witDisplayData        (theWitRun, WitSTDOUT);

   if (coinEmbedded || cplexEmbedded)
      {
      witSetPreferCoin (theWitRun, WitTRUE);

      witOptImplode    (theWitRun);

      witSetPreferCoin (theWitRun, WitFALSE);

      remove ("solver.log");
      }

   witWriteExecSched (theWitRun, WitSTDOUT, WitBSV);
   witWriteShipSched (theWitRun, WitSTDOUT, WitBSV);
   witDeleteRun      (theWitRun);
   }

/*----------------------------------------------------------------------------*/
/* defProfDelTest.                                                            */
/* Define the problem for a item deletion test.                               */
/*----------------------------------------------------------------------------*/

void defProfDelTest (WitRun * theWitRun)
   {
   float supplyVolA[] = {0,  5};
   float supplyVolB[] = {10, 0};
   float demandVolK[] = {20, 0};

   witSetTitle        (theWitRun, "Item Deletion Test");
   witSetNPeriods     (theWitRun, 2);

   witAddPart         (theWitRun, "A", WitCAPACITY);
   witAddPart         (theWitRun, "B", WitCAPACITY);
   witAddPart         (theWitRun, "E", WitMATERIAL);
   witAddPart         (theWitRun, "F", WitMATERIAL);
   witAddPart         (theWitRun, "I", WitMATERIAL);

   witAddOperation    (theWitRun, "C");
   witAddOperation    (theWitRun, "D");
   witAddOperation    (theWitRun, "H");

   witAddBomEntry     (theWitRun, "C", "A");
   witAddBomEntry     (theWitRun, "D", "B");
   witAddBomEntry     (theWitRun, "H", "B");
   witAddBomEntry     (theWitRun, "H", "E");
   witAddBomEntry     (theWitRun, "H", "F");

   witAddSubsBomEntry (theWitRun, "C", 0, "A");
   witAddSubsBomEntry (theWitRun, "C", 0, "B");
   witAddSubsBomEntry (theWitRun, "D", 0, "A");
   witAddSubsBomEntry (theWitRun, "H", 1, "F");
   witAddSubsBomEntry (theWitRun, "H", 2, "E");

   witAddBopEntry     (theWitRun, "C", "I");
   witAddBopEntry     (theWitRun, "C", "E");
   witAddBopEntry     (theWitRun, "C", "F");
   witAddBopEntry     (theWitRun, "D", "F");
   witAddBopEntry     (theWitRun, "H", "I");

   witAddDemand       (theWitRun, "E", "G");
   witAddDemand       (theWitRun, "I", "J");
   witAddDemand       (theWitRun, "I", "K");

   witSetPartSupplyVol   (theWitRun, "A",      supplyVolA);
   witSetPartSupplyVol   (theWitRun, "B",      supplyVolB);
   witSetDemandDemandVol (theWitRun, "I", "K", demandVolK);
   }

/*----------------------------------------------------------------------------*/
/* selItemsDelTest.                                                           */
/* Selects items for deletion.                                                */
/*----------------------------------------------------------------------------*/

void selItemsDelTest (WitRun * theWitRun)
   {
   witSetPartSelForDel         (theWitRun, "E",       WitTRUE);
   witSetOperationSelForDel    (theWitRun, "D",       WitTRUE);
   witSetOperationSelForDel    (theWitRun, "C",       WitTRUE);
   witSetBomEntrySelForDel     (theWitRun, "H", 0,    WitTRUE);
   witSetSubsBomEntrySelForDel (theWitRun, "C", 0, 0, WitTRUE);
   witSetBopEntrySelForDel     (theWitRun, "C", 0,    WitTRUE);
   witSetDemandSelForDel       (theWitRun, "I", "J",  WitTRUE);
   witSetOperationSelForDel    (theWitRun, "C",       WitFALSE);
   }

/*----------------------------------------------------------------------------*/
/* copyWriteDelTest                                                           */
/* Tests copying and writing the selForDel attributes.                        */
/*----------------------------------------------------------------------------*/

void copyWriteDelTest (WitRun * theWitRun)
   {
   WitRun * copyWitRun;

   witNewRun   (& copyWitRun);
   witCopyData   (copyWitRun, theWitRun);
   witInitialize ( theWitRun);
   witWriteData  (copyWitRun, "delTest.data");
   witDeleteRun  (copyWitRun);

   copyWitRun = NULL;

   witReadData   ( theWitRun, "delTest.data");
   remove        (            "delTest.data");
   }

/*----------------------------------------------------------------------------*/
/* checkSelDelTest                                                            */
/* Checks the selForDel attribute.                                            */
/*----------------------------------------------------------------------------*/

void checkSelDelTest (WitRun * theWitRun)
   {
   witBoolean selForDel;

   witGetPartSelForDel         (theWitRun, "E",       & selForDel);

   assert (selForDel);

   witGetOperationSelForDel    (theWitRun, "D",       & selForDel);

   assert (selForDel);

   witGetBomEntrySelForDel     (theWitRun, "H", 0,    & selForDel);

   assert (selForDel);

   witGetSubsBomEntrySelForDel (theWitRun, "C", 0, 0, & selForDel);

   assert (selForDel);

   witGetBopEntrySelForDel     (theWitRun, "C", 0,    & selForDel);

   assert (selForDel);

   witGetDemandSelForDel       (theWitRun, "I", "J",  & selForDel);

   assert (selForDel);


   witGetOperationSelForDel    (theWitRun, "C",       & selForDel);

   assert (!selForDel);

   witGetPartSelForDel         (theWitRun, "A",       & selForDel);

   assert (! selForDel);
   }

/*----------------------------------------------------------------------------*/
/* testExecVolPegging                                                         */
/*----------------------------------------------------------------------------*/

void testExecVolPegging (
      WitRun *     theWitRun, 
      const char * partName, 
      const char * demandName, 
      int          shipPer)
   {
   int      lenLists;
   char * * operationNameList;
   int *    execPeriodList;
   float *  peggedExecVolList;
   int      theIdx;

   witGetDemandExecVolPegging (
        theWitRun,
        partName,
        demandName,
        shipPer,
      & lenLists,
      & operationNameList,
      & execPeriodList,
      & peggedExecVolList);

   printf (
      "\n"
      "ExecVol Pegging for Part %s, Demand %s, Period %d:\n\n",
      partName,
      demandName,
      shipPer);

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      printf ("   Operation %s, Period %d, ExecVol: %.0f\n",
         operationNameList[theIdx],
         execPeriodList   [theIdx],
         peggedExecVolList[theIdx]);

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      witFree (operationNameList[theIdx]);

   witFree (operationNameList);
   witFree (execPeriodList);
   witFree (peggedExecVolList);
   }

/*----------------------------------------------------------------------------*/
/* testExecVolPeggingDbl                                                      */
/*----------------------------------------------------------------------------*/

void testExecVolPeggingDbl (
      WitRun *     theWitRun, 
      const char * partName, 
      const char * demandName, 
      int          shipPer)
   {
   int      lenLists;
   char * * operationNameList;
   int *    execPeriodList;
   double * peggedExecVolList;
   int      theIdx;

   witGetDemandExecVolPeggingDbl (
        theWitRun,
        partName,
        demandName,
        shipPer,
      & lenLists,
      & operationNameList,
      & execPeriodList,
      & peggedExecVolList);

   printf (
      "\n"
      "ExecVol Pegging (Double) for Part %s, Demand %s, Period %d:\n\n",
      partName,
      demandName,
      shipPer);

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      printf ("   Operation %s, Period %d, ExecVol: %.0f\n",
         operationNameList[theIdx],
         execPeriodList   [theIdx],
         peggedExecVolList[theIdx]);

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      witFree (operationNameList[theIdx]);

   witFree (operationNameList);
   witFree (execPeriodList);
   witFree (peggedExecVolList);
   }

/*----------------------------------------------------------------------------*/
/* testSubVolPegging                                                          */
/*----------------------------------------------------------------------------*/

void testSubVolPegging (
      WitRun *     theWitRun, 
      const char * partName, 
      const char * demandName, 
      int          shipPer)
   {
   int      lenLists;
   char * * operationNameList;
   int *    bomEntryIndexList;
   int *    subsBomEntryIndexList;
   int *    execPeriodList;
   float *  peggedSubVolList;
   int      theIdx;

   witGetDemandSubVolPegging (
        theWitRun,
        partName,
        demandName,
        shipPer,
      & lenLists,
      & operationNameList,
      & bomEntryIndexList,
      & subsBomEntryIndexList,
      & execPeriodList,
      & peggedSubVolList);

   printf (
      "\n"
      "SubVol Pegging for Part %s, Demand %s, Period %d:\n\n",
      partName,
      demandName,
      shipPer);

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      printf ("   Operation %s, BOM Ent %d, Sub %d, Period %d, SubVol: %.0f\n",
         operationNameList    [theIdx],
         bomEntryIndexList    [theIdx],
         subsBomEntryIndexList[theIdx],
         execPeriodList       [theIdx],
         peggedSubVolList     [theIdx]);

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      witFree (operationNameList[theIdx]);

   witFree (operationNameList);
   witFree (bomEntryIndexList);
   witFree (subsBomEntryIndexList);
   witFree (execPeriodList);
   witFree (peggedSubVolList);
   }

/*----------------------------------------------------------------------------*/
/* testSubVolPeggingDbl                                                       */
/*----------------------------------------------------------------------------*/

void testSubVolPeggingDbl (
      WitRun *     theWitRun, 
      const char * partName, 
      const char * demandName, 
      int          shipPer)
   {
   int      lenLists;
   char * * operationNameList;
   int *    bomEntryIndexList;
   int *    subsBomEntryIndexList;
   int *    execPeriodList;
   double * peggedSubVolList;
   int      theIdx;

   witGetDemandSubVolPeggingDbl (
        theWitRun,
        partName,
        demandName,
        shipPer,
      & lenLists,
      & operationNameList,
      & bomEntryIndexList,
      & subsBomEntryIndexList,
      & execPeriodList,
      & peggedSubVolList);

   printf (
      "\n"
      "SubVol Pegging (Double) for Part %s, Demand %s, Period %d:\n\n",
      partName,
      demandName,
      shipPer);

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      printf ("   Operation %s, BOM Ent %d, Sub %d, Period %d, SubVol: %.0f\n",
         operationNameList    [theIdx],
         bomEntryIndexList    [theIdx],
         subsBomEntryIndexList[theIdx],
         execPeriodList       [theIdx],
         peggedSubVolList     [theIdx]);

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      witFree (operationNameList[theIdx]);

   witFree (operationNameList);
   witFree (bomEntryIndexList);
   witFree (subsBomEntryIndexList);
   witFree (execPeriodList);
   witFree (peggedSubVolList);
   }

/*----------------------------------------------------------------------------*/
/* testPip                                                                    */
/*----------------------------------------------------------------------------*/

void testPip (WitRun * theWitRun)
   {
   witBoolean theBool;
   int        theInt;

   witSetPipSeqFromHeur   (theWitRun,   WitTRUE);
   witGetPipSeqFromHeur   (theWitRun, & theBool);

   assert                 (theBool);

   witSetNewPipPggOrder   (theWitRun,   WitTRUE);
   witGetNewPipPggOrder   (theWitRun, & theBool);

   assert                 (theBool);

   witGetPipExists        (theWitRun, & theBool);

   assert                 (! theBool);

   witSetOperationPipEnabled (theWitRun, "PS1",   WitTRUE);
   witGetOperationPipEnabled (theWitRun, "PS1", & theBool);

   assert                    (theBool);

   witSetOperationPipRank    (theWitRun, "PS1",   -3);
   witGetOperationPipRank    (theWitRun, "PS1", & theInt);

   assert                    (theInt == -3);

   witHeurImplode         (theWitRun);

   witBuildPip            (theWitRun);

   witGetPipExists        (theWitRun, & theBool);

   assert                 (theBool);

   testExecVolPipToDems   (theWitRun, "PS1", "US", 2);
   testExecVolPipToOpns   (theWitRun, "PS1",       2);
   testCoExecVolPipToDems (theWitRun, "PS1", "US", 2);
   testCoExecVolPipToOpns (theWitRun, "PS1",       2);
   testSubVolPipToDems    (theWitRun, "PS1", "US", 2);
   testSubVolPipToOpns    (theWitRun, "PS1",       2);
   testSupplyVolPipToDems (theWitRun, "PS1", "US", 2);
   testSupplyVolPipToOpns (theWitRun, "PS1",       2);
   testSideVolPipToDems   (theWitRun, "PS1", "US", 2);
   testSideVolPipToOpns   (theWitRun, "PS1",       2);
   testProdVolPipToDems   (theWitRun, "PS1", "US", 2);
   testProdVolPipToOpns   (theWitRun, "PS1",       2);
   testConsVolPipToDems   (theWitRun, "PS1", "US", 2);
   testConsVolPipToOpns   (theWitRun, "PS1",       2);

   truncatePipSeq         (theWitRun);

   witBuildPip            (theWitRun);
   }

/*----------------------------------------------------------------------------*/
/* truncatePipSeq                                                             */
/*                                                                            */
/* Reduces the PIP shipment sequence to its first and last elements.          */
/*----------------------------------------------------------------------------*/

void truncatePipSeq (WitRun * theWitRun)
   {
   int      lenLists;
   char * * partNameList;
   char * * demandNameList;
   int *    shipPerList;   
   float *  incShipVolList;
   int      theIdx;

   witGetPipSeq (
        theWitRun,
      & lenLists,
      & partNameList,
      & demandNameList,
      & shipPerList,
      & incShipVolList);

   witClearPipSeq (theWitRun);

   if (lenLists >= 2)
      {
      witAppendToPipSeq (
         theWitRun,
         partNameList  [0],
         demandNameList[0],
         shipPerList   [0],
         incShipVolList[0]);

      witAppendToPipSeq (
         theWitRun,
         partNameList  [lenLists - 1],
         demandNameList[lenLists - 1],
         shipPerList   [lenLists - 1],
         incShipVolList[lenLists - 1]);
      }

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      {
      witFree (partNameList  [theIdx]);
      witFree (demandNameList[theIdx]);
      }

   witFree (partNameList);
   witFree (demandNameList);
   witFree (shipPerList);
   witFree (incShipVolList);
   }

/*----------------------------------------------------------------------------*/
/* testExecVolPipToDems                                                       */
/*----------------------------------------------------------------------------*/

void testExecVolPipToDems (
      WitRun *     theWitRun, 
      const char * partName, 
      const char * demandName, 
      int          shipPer)
   {
   int      lenLists;
   char * * operationNameList;
   int *    execPeriodList;
   float *  peggedExecVolList;
   int      theIdx;

   witGetDemandExecVolPip (
        theWitRun,
        partName,
        demandName,
        shipPer,
      & lenLists,
      & operationNameList,
      & execPeriodList,
      & peggedExecVolList);

   prtPipToDemsHeading ("ExecVol", partName, demandName, shipPer);

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      printf ("   Operation %s, Period %d, ExecVol: %.0f\n",
         operationNameList[theIdx],
         execPeriodList   [theIdx],
         peggedExecVolList[theIdx]);

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      witFree (operationNameList[theIdx]);

   witFree (operationNameList);
   witFree (execPeriodList);
   witFree (peggedExecVolList);
   }

/*----------------------------------------------------------------------------*/
/* testExecVolPipToOpns                                                       */
/*----------------------------------------------------------------------------*/

void testExecVolPipToOpns (
      WitRun *     theWitRun, 
      const char * operationName, 
      int          execPer)
   {
   int      lenLists;
   char * * operationNameList;
   int *    execPeriodList;
   float *  peggedExecVolList;
   int      theIdx;

   witGetOperationExecVolPip (
        theWitRun,
        operationName,
        execPer,
      & lenLists,
      & operationNameList,
      & execPeriodList,
      & peggedExecVolList);

   prtPipToOpnsHeading ("ExecVol", operationName, execPer);

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      printf ("   Operation %s, Period %d, ExecVol: %.0f\n",
         operationNameList[theIdx],
         execPeriodList   [theIdx],
         peggedExecVolList[theIdx]);

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      witFree (operationNameList[theIdx]);

   witFree (operationNameList);
   witFree (execPeriodList);
   witFree (peggedExecVolList);
   }

/*----------------------------------------------------------------------------*/
/* testCoExecVolPipToDems                                                     */
/*----------------------------------------------------------------------------*/

void testCoExecVolPipToDems (
      WitRun *     theWitRun, 
      const char * partName, 
      const char * demandName, 
      int          shipPer)
   {
   int      lenLists;
   char * * operationNameList;
   int *    bopEntryIndexList;
   int *    execPeriodList;
   float *  peggedCoExecVolList;
   int      theIdx;

   witGetDemandCoExecVolPip (
        theWitRun,
        partName,
        demandName,
        shipPer,
      & lenLists,
      & operationNameList,
      & bopEntryIndexList,
      & execPeriodList,
      & peggedCoExecVolList);

   prtPipToDemsHeading ("CoExecVol", partName, demandName, shipPer);

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      printf ("   Operation %s, BOP Entry %d, Period %d, CoExecVol: %.0f\n",
         operationNameList    [theIdx],
         bopEntryIndexList    [theIdx],
         execPeriodList       [theIdx],
         peggedCoExecVolList  [theIdx]);

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      witFree (operationNameList[theIdx]);

   witFree (operationNameList);
   witFree (bopEntryIndexList);
   witFree (execPeriodList);
   witFree (peggedCoExecVolList);
   }

/*----------------------------------------------------------------------------*/
/* testCoExecVolPipToOpns                                                     */
/*----------------------------------------------------------------------------*/

void testCoExecVolPipToOpns (
      WitRun *     theWitRun, 
      const char * operationName, 
      int          execPer)
   {
   int      lenLists;
   char * * operationNameList;
   int *    bopEntryIndexList;
   int *    execPeriodList;
   float *  peggedCoExecVolList;
   int      theIdx;

   witGetOperationCoExecVolPip (
        theWitRun,
        operationName,
        execPer,
      & lenLists,
      & operationNameList,
      & bopEntryIndexList,
      & execPeriodList,
      & peggedCoExecVolList);

   prtPipToOpnsHeading ("CoExecVol", operationName, execPer);

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      printf ("   Operation %s, BOP Entry %d, Period %d, CoExecVol: %.0f\n",
         operationNameList    [theIdx],
         bopEntryIndexList    [theIdx],
         execPeriodList       [theIdx],
         peggedCoExecVolList  [theIdx]);

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      witFree (operationNameList[theIdx]);

   witFree (operationNameList);
   witFree (bopEntryIndexList);
   witFree (execPeriodList);
   witFree (peggedCoExecVolList);
   }

/*----------------------------------------------------------------------------*/
/* testSubVolPipToDems                                                        */
/*----------------------------------------------------------------------------*/

void testSubVolPipToDems (
      WitRun *     theWitRun, 
      const char * partName, 
      const char * demandName, 
      int          shipPer)
   {
   int      lenLists;
   char * * operationNameList;
   int *    bomEntryIndexList;
   int *    subsBomEntryIndexList;
   int *    execPeriodList;
   float *  peggedSubVolList;
   int      theIdx;

   witGetDemandSubVolPip (
        theWitRun,
        partName,
        demandName,
        shipPer,
      & lenLists,
      & operationNameList,
      & bomEntryIndexList,
      & subsBomEntryIndexList,
      & execPeriodList,
      & peggedSubVolList);

   prtPipToDemsHeading ("SubVol", partName, demandName, shipPer);

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      printf (
         "   Operation %s, BOM Entry %d, Sub %d, Period %d, SubVol: %.0f\n",
         operationNameList    [theIdx],
         bomEntryIndexList    [theIdx],
         subsBomEntryIndexList[theIdx],
         execPeriodList       [theIdx],
         peggedSubVolList     [theIdx]);

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      witFree (operationNameList[theIdx]);

   witFree (operationNameList);
   witFree (bomEntryIndexList);
   witFree (subsBomEntryIndexList);
   witFree (execPeriodList);
   witFree (peggedSubVolList);
   }

/*----------------------------------------------------------------------------*/
/* testSubVolPipToOpns                                                        */
/*----------------------------------------------------------------------------*/

void testSubVolPipToOpns (
      WitRun *     theWitRun, 
      const char * operationName, 
      int          execPer)
   {
   int      lenLists;
   char * * operationNameList;
   int *    bomEntryIndexList;
   int *    subsBomEntryIndexList;
   int *    execPeriodList;
   float *  peggedSubVolList;
   int      theIdx;

   witGetOperationSubVolPip (
        theWitRun,
        operationName,
        execPer,
      & lenLists,
      & operationNameList,
      & bomEntryIndexList,
      & subsBomEntryIndexList,
      & execPeriodList,
      & peggedSubVolList);

   prtPipToOpnsHeading ("SubVol", operationName, execPer);

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      printf (
         "   Operation %s, BOM Entry %d, Sub %d, Period %d, SubVol: %.0f\n",
         operationNameList    [theIdx],
         bomEntryIndexList    [theIdx],
         subsBomEntryIndexList[theIdx],
         execPeriodList       [theIdx],
         peggedSubVolList     [theIdx]);

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      witFree (operationNameList[theIdx]);

   witFree (operationNameList);
   witFree (bomEntryIndexList);
   witFree (subsBomEntryIndexList);
   witFree (execPeriodList);
   witFree (peggedSubVolList);
   }

/*----------------------------------------------------------------------------*/
/* testSupplyVolPipToDems                                                     */
/*----------------------------------------------------------------------------*/

void testSupplyVolPipToDems (
      WitRun *     theWitRun, 
      const char * partName, 
      const char * demandName, 
      int          shipPer)
   {
   int      lenLists;
   char * * partNameList;
   int *    periodList;
   float *  peggedSupplyVolList;

   witGetDemandSupplyVolPip (
        theWitRun,
        partName,
        demandName,
        shipPer,
      & lenLists,
      & partNameList,
      & periodList,
      & peggedSupplyVolList);

   prtPipToDemsHeading ("SupplyVol", partName, demandName, shipPer);

   prtPartVolPip (
      "SupplyVol",
      lenLists,
      partNameList,
      periodList,
      peggedSupplyVolList);
   }

/*----------------------------------------------------------------------------*/
/* testSupplyVolPipToOpns                                                     */
/*----------------------------------------------------------------------------*/

void testSupplyVolPipToOpns (
      WitRun *     theWitRun, 
      const char * operationName, 
      int          execPer)
   {
   int      lenLists;
   char * * partNameList;
   int *    periodList;
   float *  peggedSupplyVolList;

   witGetOperationSupplyVolPip (
        theWitRun,
        operationName,
        execPer,
      & lenLists,
      & partNameList,
      & periodList,
      & peggedSupplyVolList);

   prtPipToOpnsHeading ("SupplyVol", operationName, execPer);

   prtPartVolPip (
      "SupplyVol",
      lenLists,
      partNameList,
      periodList,
      peggedSupplyVolList);
   }

/*----------------------------------------------------------------------------*/
/* testSideVolPipToDems                                                       */
/*----------------------------------------------------------------------------*/

void testSideVolPipToDems (
      WitRun *     theWitRun, 
      const char * partName, 
      const char * demandName, 
      int          shipPer)
   {
   int      lenLists;
   char * * partNameList;
   int *    periodList;
   float *  peggedSideVolList;

   witGetDemandSideVolPip (
        theWitRun,
        partName,
        demandName,
        shipPer,
      & lenLists,
      & partNameList,
      & periodList,
      & peggedSideVolList);

   prtPipToDemsHeading ("SideVol", partName, demandName, shipPer);

   prtPartVolPip (
      "SideVol",
      lenLists,
      partNameList,
      periodList,
      peggedSideVolList);
   }

/*----------------------------------------------------------------------------*/
/* testSideVolPipToOpns                                                       */
/*----------------------------------------------------------------------------*/

void testSideVolPipToOpns (
      WitRun *     theWitRun, 
      const char * operationName, 
      int          execPer)
   {
   int      lenLists;
   char * * partNameList;
   int *    periodList;
   float *  peggedSideVolList;

   witGetOperationSideVolPip (
        theWitRun,
        operationName,
        execPer,
      & lenLists,
      & partNameList,
      & periodList,
      & peggedSideVolList);

   prtPipToOpnsHeading ("SideVol", operationName, execPer);

   prtPartVolPip (
      "SideVol",
      lenLists,
      partNameList,
      periodList,
      peggedSideVolList);
   }

/*----------------------------------------------------------------------------*/
/* testProdVolPipToDems                                                       */
/*----------------------------------------------------------------------------*/

void testProdVolPipToDems (
      WitRun *     theWitRun, 
      const char * partName, 
      const char * demandName, 
      int          shipPer)
   {
   int      lenLists;
   char * * partNameList;
   int *    periodList;
   float *  peggedProdVolList;

   witGetDemandProdVolPip (
        theWitRun,
        partName,
        demandName,
        shipPer,
      & lenLists,
      & partNameList,
      & periodList,
      & peggedProdVolList);

   prtPipToDemsHeading ("ProdVol", partName, demandName, shipPer);

   prtPartVolPip (
      "ProdVol",
      lenLists,
      partNameList,
      periodList,
      peggedProdVolList);
   }

/*----------------------------------------------------------------------------*/
/* testProdVolPipToOpns                                                       */
/*----------------------------------------------------------------------------*/

void testProdVolPipToOpns (
      WitRun *     theWitRun, 
      const char * operationName, 
      int          execPer)
   {
   int      lenLists;
   char * * partNameList;
   int *    periodList;
   float *  peggedProdVolList;

   witGetOperationProdVolPip (
        theWitRun,
        operationName,
        execPer,
      & lenLists,
      & partNameList,
      & periodList,
      & peggedProdVolList);

   prtPipToOpnsHeading ("ProdVol", operationName, execPer);

   prtPartVolPip (
      "ProdVol",
      lenLists,
      partNameList,
      periodList,
      peggedProdVolList);
   }

/*----------------------------------------------------------------------------*/
/* testConsVolPipToDems                                                       */
/*----------------------------------------------------------------------------*/

void testConsVolPipToDems (
      WitRun *     theWitRun, 
      const char * partName, 
      const char * demandName, 
      int          shipPer)
   {
   int      lenLists;
   char * * partNameList;
   int *    periodList;
   float *  peggedConsVolList;

   witGetDemandConsVolPip (
        theWitRun,
        partName,
        demandName,
        shipPer,
      & lenLists,
      & partNameList,
      & periodList,
      & peggedConsVolList);

   prtPipToDemsHeading ("ConsVol", partName, demandName, shipPer);

   prtPartVolPip (
      "ConsVol",
      lenLists,
      partNameList,
      periodList,
      peggedConsVolList);
   }

/*----------------------------------------------------------------------------*/
/* testConsVolPipToOpns                                                       */
/*----------------------------------------------------------------------------*/

void testConsVolPipToOpns (
      WitRun *     theWitRun, 
      const char * operationName, 
      int          execPer)
   {
   int      lenLists;
   char * * partNameList;
   int *    periodList;
   float *  peggedConsVolList;

   witGetOperationConsVolPip (
        theWitRun,
        operationName,
        execPer,
      & lenLists,
      & partNameList,
      & periodList,
      & peggedConsVolList);

   prtPipToOpnsHeading ("ConsVol", operationName, execPer);

   prtPartVolPip (
      "ConsVol",
      lenLists,
      partNameList,
      periodList,
      peggedConsVolList);
   }

/*----------------------------------------------------------------------------*/
/* prtPartVolPip                                                              */
/*----------------------------------------------------------------------------*/

void prtPartVolPip (
      const char * attrName,
      int          lenLists,
      char * *     partNameList,
      int *        periodList,
      float *      peggedVolList)
   {
   int theIdx;

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      printf ("   Part %s, Period %d, %s: %.0f\n",
         partNameList [theIdx],
         periodList   [theIdx],
         attrName,
         peggedVolList[theIdx]);

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      witFree (partNameList[theIdx]);

   witFree (partNameList);
   witFree (periodList);
   witFree (peggedVolList);
   }

/*----------------------------------------------------------------------------*/
/* testPipDbl                                                                 */
/*----------------------------------------------------------------------------*/

void testPipDbl (WitRun * theWitRun)
   {
   witBoolean theBool;

   witSetNewPipPggOrder      (theWitRun,   WitTRUE);
   witGetNewPipPggOrder      (theWitRun, & theBool);

   assert                    (theBool);

   witSetPipSeqFromHeur      (theWitRun,   WitTRUE);
   witGetPipSeqFromHeur      (theWitRun, & theBool);

   assert                    (theBool);

   witGetPipExists           (theWitRun, & theBool);

   assert                    (! theBool);

   witHeurImplode            (theWitRun);

   witBuildPip               (theWitRun);

   witGetPipExists           (theWitRun, & theBool);

   assert                    (theBool);

   testExecVolPipToDemsDbl   (theWitRun, "PS1", "US", 2);
   testExecVolPipToOpnsDbl   (theWitRun, "PS1",       2);
   testCoExecVolPipToDemsDbl (theWitRun, "PS1", "US", 2);
   testCoExecVolPipToOpnsDbl (theWitRun, "PS1",       2);
   testSubVolPipToDemsDbl    (theWitRun, "PS1", "US", 2);
   testSubVolPipToOpnsDbl    (theWitRun, "PS1",       2);
   testSupplyVolPipToDemsDbl (theWitRun, "PS1", "US", 2);
   testSupplyVolPipToOpnsDbl (theWitRun, "PS1",       2);
   testSideVolPipToDemsDbl   (theWitRun, "PS1", "US", 2);
   testSideVolPipToOpnsDbl   (theWitRun, "PS1",       2);
   testProdVolPipToDemsDbl   (theWitRun, "PS1", "US", 2);
   testProdVolPipToOpnsDbl   (theWitRun, "PS1",       2);
   testConsVolPipToDemsDbl   (theWitRun, "PS1", "US", 2);
   testConsVolPipToOpnsDbl   (theWitRun, "PS1",       2);

   truncatePipSeqDbl         (theWitRun);

   witBuildPip               (theWitRun);
   }

/*----------------------------------------------------------------------------*/
/* truncatePipSeqDbl                                                          */
/*                                                                            */
/* Reduces the PIP shipment sequence to its first and last elements.          */
/*----------------------------------------------------------------------------*/

void truncatePipSeqDbl (WitRun * theWitRun)
   {
   int      lenLists;
   char * * partNameList;
   char * * demandNameList;
   int *    shipPerList;   
   double * incShipVolList;
   int      theIdx;

   witGetPipSeqDbl (
        theWitRun,
      & lenLists,
      & partNameList,
      & demandNameList,
      & shipPerList,
      & incShipVolList);

   witClearPipSeq (theWitRun);

   witAppendToPipSeqDbl (
      theWitRun,
      partNameList  [0],
      demandNameList[0],
      shipPerList   [0],
      incShipVolList[0]);

   witAppendToPipSeqDbl (
      theWitRun,
      partNameList  [lenLists - 1],
      demandNameList[lenLists - 1],
      shipPerList   [lenLists - 1],
      incShipVolList[lenLists - 1]);

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      {
      witFree (partNameList  [theIdx]);
      witFree (demandNameList[theIdx]);
      }

   witFree (partNameList);
   witFree (demandNameList);
   witFree (shipPerList);
   witFree (incShipVolList);
   }

/*----------------------------------------------------------------------------*/
/* testExecVolPipToDemsDbl                                                    */
/*----------------------------------------------------------------------------*/

void testExecVolPipToDemsDbl (
      WitRun *     theWitRun, 
      const char * partName, 
      const char * demandName, 
      int          shipPer)
   {
   int      lenLists;
   char * * operationNameList;
   int *    execPeriodList;
   double * peggedExecVolList;
   int      theIdx;

   witGetDemandExecVolPipDbl (
        theWitRun,
        partName,
        demandName,
        shipPer,
      & lenLists,
      & operationNameList,
      & execPeriodList,
      & peggedExecVolList);

   prtPipToDemsHeading ("ExecVol", partName, demandName, shipPer);

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      printf ("   Operation %s, Period %d, ExecVol: %.0f\n",
         operationNameList[theIdx],
         execPeriodList   [theIdx],
         peggedExecVolList[theIdx]);

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      witFree (operationNameList[theIdx]);

   witFree (operationNameList);
   witFree (execPeriodList);
   witFree (peggedExecVolList);
   }

/*----------------------------------------------------------------------------*/
/* testExecVolPipToOpnsDbl                                                    */
/*----------------------------------------------------------------------------*/

void testExecVolPipToOpnsDbl (
      WitRun *     theWitRun, 
      const char * operationName, 
      int          execPer)
   {
   int      lenLists;
   char * * operationNameList;
   int *    execPeriodList;
   double * peggedExecVolList;
   int      theIdx;

   witGetOperationExecVolPipDbl (
        theWitRun,
        operationName,
        execPer,
      & lenLists,
      & operationNameList,
      & execPeriodList,
      & peggedExecVolList);

   prtPipToOpnsHeading ("ExecVol", operationName, execPer);

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      printf ("   Operation %s, Period %d, ExecVol: %.0f\n",
         operationNameList[theIdx],
         execPeriodList   [theIdx],
         peggedExecVolList[theIdx]);

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      witFree (operationNameList[theIdx]);

   witFree (operationNameList);
   witFree (execPeriodList);
   witFree (peggedExecVolList);
   }

/*----------------------------------------------------------------------------*/
/* testCoExecVolPipToDemsDbl                                                  */
/*----------------------------------------------------------------------------*/

void testCoExecVolPipToDemsDbl (
      WitRun *     theWitRun, 
      const char * partName, 
      const char * demandName, 
      int          shipPer)
   {
   int      lenLists;
   char * * operationNameList;
   int *    bopEntryIndexList;
   int *    execPeriodList;
   double * peggedCoExecVolList;
   int      theIdx;

   witGetDemandCoExecVolPipDbl (
        theWitRun,
        partName,
        demandName,
        shipPer,
      & lenLists,
      & operationNameList,
      & bopEntryIndexList,
      & execPeriodList,
      & peggedCoExecVolList);

   prtPipToDemsHeading ("CoExecVol", partName, demandName, shipPer);

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      printf ("   Operation %s, BOP Entry %d, Period %d, CoExecVol: %.0f\n",
         operationNameList    [theIdx],
         bopEntryIndexList    [theIdx],
         execPeriodList       [theIdx],
         peggedCoExecVolList  [theIdx]);

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      witFree (operationNameList[theIdx]);

   witFree (operationNameList);
   witFree (bopEntryIndexList);
   witFree (execPeriodList);
   witFree (peggedCoExecVolList);
   }

/*----------------------------------------------------------------------------*/
/* testCoExecVolPipToOpnsDbl                                                  */
/*----------------------------------------------------------------------------*/

void testCoExecVolPipToOpnsDbl (
      WitRun *     theWitRun, 
      const char * operationName, 
      int          execPer)
   {
   int      lenLists;
   char * * operationNameList;
   int *    bopEntryIndexList;
   int *    execPeriodList;
   double * peggedCoExecVolList;
   int      theIdx;

   witGetOperationCoExecVolPipDbl (
        theWitRun,
        operationName,
        execPer,
      & lenLists,
      & operationNameList,
      & bopEntryIndexList,
      & execPeriodList,
      & peggedCoExecVolList);

   prtPipToOpnsHeading ("CoExecVol", operationName, execPer);

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      printf ("   Operation %s, BOP Entry %d, Period %d, CoExecVol: %.0f\n",
         operationNameList    [theIdx],
         bopEntryIndexList    [theIdx],
         execPeriodList       [theIdx],
         peggedCoExecVolList  [theIdx]);

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      witFree (operationNameList[theIdx]);

   witFree (operationNameList);
   witFree (bopEntryIndexList);
   witFree (execPeriodList);
   witFree (peggedCoExecVolList);
   }

/*----------------------------------------------------------------------------*/
/* testSubVolPipToDemsDbl                                                     */
/*----------------------------------------------------------------------------*/

void testSubVolPipToDemsDbl (
      WitRun *     theWitRun, 
      const char * partName, 
      const char * demandName, 
      int          shipPer)
   {
   int      lenLists;
   char * * operationNameList;
   int *    bomEntryIndexList;
   int *    subsBomEntryIndexList;
   int *    execPeriodList;
   double * peggedSubVolList;
   int      theIdx;

   witGetDemandSubVolPipDbl (
        theWitRun,
        partName,
        demandName,
        shipPer,
      & lenLists,
      & operationNameList,
      & bomEntryIndexList,
      & subsBomEntryIndexList,
      & execPeriodList,
      & peggedSubVolList);

   prtPipToDemsHeading ("SubVol", partName, demandName, shipPer);

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      printf (
         "   Operation %s, BOM Entry %d, Sub %d, Period %d, SubVol: %.0f\n",
         operationNameList    [theIdx],
         bomEntryIndexList    [theIdx],
         subsBomEntryIndexList[theIdx],
         execPeriodList       [theIdx],
         peggedSubVolList     [theIdx]);

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      witFree (operationNameList[theIdx]);

   witFree (operationNameList);
   witFree (bomEntryIndexList);
   witFree (subsBomEntryIndexList);
   witFree (execPeriodList);
   witFree (peggedSubVolList);
   }

/*----------------------------------------------------------------------------*/
/* testSubVolPipToOpnsDbl                                                     */
/*----------------------------------------------------------------------------*/

void testSubVolPipToOpnsDbl (
      WitRun *     theWitRun, 
      const char * operationName, 
      int          execPer)
   {
   int      lenLists;
   char * * operationNameList;
   int *    bomEntryIndexList;
   int *    subsBomEntryIndexList;
   int *    execPeriodList;
   double * peggedSubVolList;
   int      theIdx;

   witGetOperationSubVolPipDbl (
        theWitRun,
        operationName,
        execPer,
      & lenLists,
      & operationNameList,
      & bomEntryIndexList,
      & subsBomEntryIndexList,
      & execPeriodList,
      & peggedSubVolList);

   prtPipToOpnsHeading ("SubVol", operationName, execPer);

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      printf (
         "   Operation %s, BOM Entry %d, Sub %d, Period %d, SubVol: %.0f\n",
         operationNameList    [theIdx],
         bomEntryIndexList    [theIdx],
         subsBomEntryIndexList[theIdx],
         execPeriodList       [theIdx],
         peggedSubVolList     [theIdx]);

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      witFree (operationNameList[theIdx]);

   witFree (operationNameList);
   witFree (bomEntryIndexList);
   witFree (subsBomEntryIndexList);
   witFree (execPeriodList);
   witFree (peggedSubVolList);
   }

/*----------------------------------------------------------------------------*/
/* testSupplyVolPipToDemsDbl                                                  */
/*----------------------------------------------------------------------------*/

void testSupplyVolPipToDemsDbl (
      WitRun *     theWitRun,
      const char * partName,
      const char * demandName,
      int          shipPer)
   {
   int      lenLists;
   char * * partNameList;
   int *    periodList;
   double * peggedSupplyVolList;

   witGetDemandSupplyVolPipDbl (
        theWitRun,
        partName,
        demandName,
        shipPer,
      & lenLists,
      & partNameList,
      & periodList,
      & peggedSupplyVolList);

   prtPipToDemsHeading ("SupplyVol", partName, demandName, shipPer);

   prtPartVolPipDbl (
      "SupplyVol",
      lenLists,
      partNameList,
      periodList,
      peggedSupplyVolList);
   }

/*----------------------------------------------------------------------------*/
/* testSupplyVolPipToOpnsDbl                                                  */
/*----------------------------------------------------------------------------*/

void testSupplyVolPipToOpnsDbl (
      WitRun *     theWitRun, 
      const char * operationName, 
      int          execPer)
   {
   int      lenLists;
   char * * partNameList;
   int *    periodList;
   double * peggedSupplyVolList;

   witGetOperationSupplyVolPipDbl (
        theWitRun,
        operationName,
        execPer,
      & lenLists,
      & partNameList,
      & periodList,
      & peggedSupplyVolList);

   prtPipToOpnsHeading ("SupplyVol", operationName, execPer);

   prtPartVolPipDbl (
      "SupplyVol",
      lenLists,
      partNameList,
      periodList,
      peggedSupplyVolList);
   }

/*----------------------------------------------------------------------------*/
/* testSideVolPipToDemsDbl                                                    */
/*----------------------------------------------------------------------------*/

void testSideVolPipToDemsDbl (
      WitRun *     theWitRun, 
      const char * partName, 
      const char * demandName, 
      int          shipPer)
   {
   int      lenLists;
   char * * partNameList;
   int *    periodList;
   double * peggedSideVolList;

   witGetDemandSideVolPipDbl (
        theWitRun,
        partName,
        demandName,
        shipPer,
      & lenLists,
      & partNameList,
      & periodList,
      & peggedSideVolList);

   prtPipToDemsHeading ("SideVol", partName, demandName, shipPer);

   prtPartVolPipDbl (
      "SideVol",
      lenLists,
      partNameList,
      periodList,
      peggedSideVolList);
   }

/*----------------------------------------------------------------------------*/
/* testSideVolPipToOpnsDbl                                                    */
/*----------------------------------------------------------------------------*/

void testSideVolPipToOpnsDbl (
      WitRun *     theWitRun, 
      const char * operationName, 
      int          execPer)
   {
   int      lenLists;
   char * * partNameList;
   int *    periodList;
   double * peggedSideVolList;

   witGetOperationSideVolPipDbl (
        theWitRun,
        operationName,
        execPer,
      & lenLists,
      & partNameList,
      & periodList,
      & peggedSideVolList);

   prtPipToOpnsHeading ("SideVol", operationName, execPer);

   prtPartVolPipDbl (
      "SideVol",
      lenLists,
      partNameList,
      periodList,
      peggedSideVolList);
   }

/*----------------------------------------------------------------------------*/
/* testProdVolPipToDemsDbl                                                    */
/*----------------------------------------------------------------------------*/

void testProdVolPipToDemsDbl (
      WitRun *     theWitRun, 
      const char * partName, 
      const char * demandName, 
      int          shipPer)
   {
   int      lenLists;
   char * * partNameList;
   int *    periodList;
   double * peggedProdVolList;

   witGetDemandProdVolPipDbl (
        theWitRun,
        partName,
        demandName,
        shipPer,
      & lenLists,
      & partNameList,
      & periodList,
      & peggedProdVolList);

   prtPipToDemsHeading ("ProdVol", partName, demandName, shipPer);

   prtPartVolPipDbl (
      "ProdVol",
      lenLists,
      partNameList,
      periodList,
      peggedProdVolList);
   }

/*----------------------------------------------------------------------------*/
/* testProdVolPipToOpnsDbl                                                    */
/*----------------------------------------------------------------------------*/

void testProdVolPipToOpnsDbl (
      WitRun *     theWitRun, 
      const char * operationName, 
      int          execPer)
   {
   int      lenLists;
   char * * partNameList;
   int *    periodList;
   double * peggedProdVolList;

   witGetOperationProdVolPipDbl (
        theWitRun,
        operationName,
        execPer,
      & lenLists,
      & partNameList,
      & periodList,
      & peggedProdVolList);

   prtPipToOpnsHeading ("ProdVol", operationName, execPer);

   prtPartVolPipDbl (
      "ProdVol",
      lenLists,
      partNameList,
      periodList,
      peggedProdVolList);
   }

/*----------------------------------------------------------------------------*/
/* testConsVolPipToDemsDbl                                                    */
/*----------------------------------------------------------------------------*/

void testConsVolPipToDemsDbl (
      WitRun *     theWitRun, 
      const char * partName, 
      const char * demandName, 
      int          shipPer)
   {
   int      lenLists;
   char * * partNameList;
   int *    periodList;
   double * peggedConsVolList;

   witGetDemandConsVolPipDbl (
        theWitRun,
        partName,
        demandName,
        shipPer,
      & lenLists,
      & partNameList,
      & periodList,
      & peggedConsVolList);

   prtPipToDemsHeading ("ConsVol", partName, demandName, shipPer);

   prtPartVolPipDbl (
      "ConsVol",
      lenLists,
      partNameList,
      periodList,
      peggedConsVolList);
   }

/*----------------------------------------------------------------------------*/
/* testConsVolPipToOpnsDbl                                                    */
/*----------------------------------------------------------------------------*/

void testConsVolPipToOpnsDbl (
      WitRun *     theWitRun, 
      const char * operationName, 
      int          execPer)
   {
   int      lenLists;
   char * * partNameList;
   int *    periodList;
   double * peggedConsVolList;

   witGetOperationConsVolPipDbl (
        theWitRun,
        operationName,
        execPer,
      & lenLists,
      & partNameList,
      & periodList,
      & peggedConsVolList);

   prtPipToOpnsHeading ("ConsVol", operationName, execPer);

   prtPartVolPipDbl (
      "ConsVol",
      lenLists,
      partNameList,
      periodList,
      peggedConsVolList);
   }

/*----------------------------------------------------------------------------*/
/* prtPartVolPipDbl                                                           */
/*----------------------------------------------------------------------------*/

void prtPartVolPipDbl (
      const char * attrName,
      int          lenLists,
      char * *     partNameList,
      int *        periodList,
      double *     peggedVolList)
   {
   int theIdx;

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      printf ("   Part %s, Period %d, %s: %.0f\n",
         partNameList [theIdx],
         periodList   [theIdx],
         attrName,
         peggedVolList[theIdx]);

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      witFree (partNameList[theIdx]);

   witFree (partNameList);
   witFree (periodList);
   witFree (peggedVolList);
   }

/*----------------------------------------------------------------------------*/
/* prtPipToDemsHeading                                                        */
/*----------------------------------------------------------------------------*/

void prtPipToDemsHeading (
      const char * attrName,
      const char * partName,
      const char * demandName,
      int          shipPer)
   {
   printf (
      "\n"
      "%s PIP for Part %s, Demand %s, Period %d:\n\n",
      attrName,
      partName,
      demandName,
      shipPer);
   }

/*----------------------------------------------------------------------------*/
/* prtPipToOpnsHeading                                                        */
/*----------------------------------------------------------------------------*/

void prtPipToOpnsHeading (
      const char * attrName,
      const char * operationName,
      int          execPer)
   {
   printf (
      "\n"
      "%s PIP for Operation %s, Period %d:\n\n",
      attrName,
      operationName,
      execPer);
   }

/*----------------------------------------------------------------------------*/
/* Function testPgdCritList.                                                  */
/*                                                                            */
/* Tests pegged critical list.                                                */
/*----------------------------------------------------------------------------*/

void testPgdCritList (WitRun * theWitRun)
   {
   witBoolean theBool;

   witSetSelSplit        (theWitRun,   WitFALSE);
   witSetSrSelSplit      (theWitRun,   WitFALSE);
   witSetSkipFailures    (theWitRun,   WitTRUE);

   witSetPgdCritListMode (theWitRun,   WitTRUE);

   witGetPgdCritListMode (theWitRun, & theBool);

   assert (theBool);

   witHeurImplode        (theWitRun);

   printPgdCritList      (theWitRun);

   witSetPgdCritListMode (theWitRun,   WitFALSE);

   printPgdCritList      (theWitRun);
   }

/*----------------------------------------------------------------------------*/
/* Function printPgdCritList.                                                 */
/*                                                                            */
/* Prints pegged critical list.                                               */
/*----------------------------------------------------------------------------*/

void printPgdCritList (WitRun * theWitRun)
   {
   int      lenLists;
   char * * critPartNameList;
   int *    critPerList;
   char * * demPartNameList;
   char * * demandNameList;
   int *    shipPerList;
   int      theIdx;

   witGetPgdCritList (
        theWitRun,
      & lenLists,
      & critPartNameList,
      & critPerList,
      & demPartNameList,
      & demandNameList,
      & shipPerList);

   printf (
      "\n"
      "-------------------------------------\n"
      "Pegged Critical List\n"
      "-------------------------------------\n"
      "\n"
      "Idx  Crit     Crit  Dem     Dem  Ship\n"
      "     Part      Per  Part          Per\n"
      );

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      printf (
         "%3d  %-7s  %4d  %-6s  %-3s  %4d\n",
         theIdx,
         critPartNameList[theIdx],
         critPerList     [theIdx],
         demPartNameList [theIdx],
         demandNameList  [theIdx],
         shipPerList     [theIdx]);

   printf (
      "\n"
      "-------------------------------------\n"
      "\n");

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      {
      witFree (critPartNameList[theIdx]);
      witFree (demPartNameList [theIdx]);
      witFree (demandNameList  [theIdx]);
      }

   witFree (critPartNameList);
   witFree (critPerList);
   witFree (demPartNameList);
   witFree (demandNameList);
   witFree (shipPerList);
   }

/*----------------------------------------------------------------------------*/
/* testObjItr                                                                 */
/*----------------------------------------------------------------------------*/

void testObjItr (WitRun * theWitRun)
   {
   witAttr objItrState;
   char *  partName;
   char *  opnName;
   char *  demandName;
   int     bomEntIdx;
   int     subIdx;
   int     bopEntIdx;

   witGetObjItrState (theWitRun, & objItrState);

   assert (objItrState == WitINACTIVE);

   witResetObjItr (theWitRun);

   while (WitTRUE)
      {
      witAdvanceObjItr  (theWitRun);

      witGetObjItrState (theWitRun, & objItrState);

      if      (objItrState == WitAT_PART)
         {
         witGetObjItrPart (theWitRun, & partName);

         printf (
            "\n"
            "Object iteration is currently located at the following part:\n"
            "\n"
            "   Part \"%s\"\n",
            partName);

         witFree (partName);
         }
      else if (objItrState == WitAT_DEMAND)
         {
         witGetObjItrDemand (theWitRun, & partName, & demandName);

         printf (
            "\n"
            "Object iteration is currently located at the following demand:\n"
            "\n"
            "   Part   \"%s\"\n"
            "   Demand \"%s\"\n",
            partName,
            demandName);

         witFree (partName);
         witFree (demandName);
         }
      else if (objItrState == WitAT_OPERATION)
         {
         witGetObjItrOperation (theWitRun, & opnName);

         printf (
            "\n"
            "Object iteration is currently located at the following operation:"
            "\n"
            "\n"
            "   Operation \"%s\"\n",
            opnName);

         witFree (opnName);
         }
      else if (objItrState == WitAT_BOM_ENTRY)
         {
         witGetObjItrBomEntry (theWitRun, & opnName, & bomEntIdx);

         printf (
            "\n"
            "Object iteration is currently located at the following BOM entry:"
            "\n"
            "\n"
            "   Operation       \"%s\"\n"
            "   BOM Entry Index %d\n",
            opnName,
            bomEntIdx);

         witFree (opnName);
         }
      else if (objItrState == WitAT_SUB_ENTRY)
         {
         witGetObjItrSubsBomEntry (theWitRun, & opnName, & bomEntIdx, & subIdx);

         printf (
            "\n"
            "Object iteration is currently located at the following substitute:"
            "\n"
            "\n"
            "   Operation       \"%s\"\n"
            "   BOM Entry Index %d\n"
            "   Sub Index       %d\n",
            opnName,
            bomEntIdx,
            subIdx);

         witFree (opnName);
         }
      else if (objItrState == WitAT_BOP_ENTRY)
         {
         witGetObjItrBopEntry (theWitRun, & opnName, & bopEntIdx);

         printf (
            "\n"
            "Object iteration is currently located at the following BOP entry:"
            "\n"
            "\n"
            "   Operation       \"%s\"\n"
            "   BOP Entry Index %d\n",
            opnName,
            bopEntIdx);

         witFree (opnName);
         }
      else if (objItrState == WitINACTIVE)
         return;
      else
         assert (WitFALSE);
      }
   }

/*----------------------------------------------------------------------------*/
/* testAccOptImp                                                              */
/*                                                                            */
/* Tests accelerated optimizing implosion.                                    */
/*----------------------------------------------------------------------------*/

void testAccOptImp (witBoolean preferCoin)
   {
   WitRun * theWitRunA;
   WitRun * theWitRunB;

   printf (
      "\n"
      "--- Testing Accelerated Optimizing Implosion ---\n"
      "\n"
      "Preferred Solver: %s\n",
      preferCoin? "COIN": "CPLEX");

   witNewRun  (& theWitRunA);
   witNewRun  (& theWitRunB);

   optImpA1     (theWitRunA, preferCoin);
   optImpB1     (theWitRunB, preferCoin);
   optImpA2     (theWitRunA);
   optImpB2     (theWitRunB);

   witDeleteRun (theWitRunA);
   witDeleteRun (theWitRunB);

   prtFiles ();
   }

/*----------------------------------------------------------------------------*/
/* optImpA1                                                                   */
/*----------------------------------------------------------------------------*/

void optImpA1 (WitRun * theWitRunA, witBoolean preferCoin)
   {
   witSetMesgFileName         (theWitRunA, WitTRUE, "wit-a.log");

   witInitialize              (theWitRunA);

   witReadData                (theWitRunA, "std-obj1.data");

   witSetAccAfterOptImp       (theWitRunA, WitTRUE);

   witSetSolverLogFileName    (theWitRunA, "solver-a.log");

   witSetPreferCoin           (theWitRunA, preferCoin);

   witOptImplode              (theWitRunA);

   witWriteExecSched          (theWitRunA, "exec-a1.out", WitBSV);
   }

/*----------------------------------------------------------------------------*/
/* optImpB1                                                                   */
/*----------------------------------------------------------------------------*/

void optImpB1 (WitRun * theWitRunB, witBoolean preferCoin)
   {
   float hardLB[] = {0, 0, 0, 0, 0, 0, 0, 0};

   witSetMesgFileName         (theWitRunB, WitTRUE, "wit-b.log");

   witInitialize              (theWitRunB);

   witReadData                (theWitRunB, "diner.data");

   witSetAccAfterOptImp       (theWitRunB, WitTRUE);

   witSetSolverLogFileName    (theWitRunB, "solver-b.log");

   witSetOperationExecBounds  (theWitRunB,
                               "Ham_Sandwich",
                               hardLB,
                               NULL,
                               NULL);

   witSetPreferCoin           (theWitRunB, preferCoin);

   witOptImplode              (theWitRunB);

   witWriteExecSched          (theWitRunB, "exec-b1.out", WitBSV);
   }

/*----------------------------------------------------------------------------*/
/* optImpA2                                                                   */
/*----------------------------------------------------------------------------*/

void optImpA2 (WitRun * theWitRunA)
   {
   float supplyVol[] = {0, 10, 10, 10, 10, 10, 10, 10};

   witSetPartSupplyVol (theWitRunA, "COMP2", supplyVol);

   witOptImplode       (theWitRunA);

   witWriteExecSched   (theWitRunA, "exec-a2.out", WitBSV);
   }

/*----------------------------------------------------------------------------*/
/* optImpB2                                                                   */
/*----------------------------------------------------------------------------*/

void optImpB2 (WitRun * theWitRunB)
   {
   float supplyVol[] = {200, 200, 200, 200, 200, 200, 200, 200};

   witSetPartSupplyVol (theWitRunB, "Bread", supplyVol);

   witOptImplode       (theWitRunB);

   witWriteExecSched   (theWitRunB, "exec-b2.out", WitBSV);
   }

/*----------------------------------------------------------------------------*/
/* prtFiles                                                                   */
/*----------------------------------------------------------------------------*/

void prtFiles ()
   {
   prtRemove ("wit-a.log");
   prtRemove ("exec-a1.out");
   prtRemove ("exec-a2.out");
   remove    ("solver-a.log");

   prtRemove ("wit-b.log");
   prtRemove ("exec-b1.out");
   prtRemove ("exec-b2.out");
   remove    ("solver-b.log");
   }

/*----------------------------------------------------------------------------*/
/* prtRemove                                                                  */
/*----------------------------------------------------------------------------*/

void prtRemove (const char * filename)
   {
   char cmd[100];

   printf (
      "\n"
      "File \"%s\":\n"
      "\n",
      filename);

   fflush (stdout);

   strcpy(cmd, "cat ");

   strcat (cmd, filename);

   system (cmd);

   remove (filename);
   }

/*----------------------------------------------------------------------------*/
/* testCumShipVol                                                             */
/*                                                                            */
/* Verifies that heuristic allocation satisfies cumShipVol <= cumDemandVol.   */
/*----------------------------------------------------------------------------*/

void testCumShipVol ()
   {
   WitRun *   theWitRun;
   float      supplyVol[] = {100.0,  0.0,  0.0};
   float      demandVol[] = { 30.0, 10.0, 10.0};
   float      incVol;
   witBoolean postproc;

   witNewRun           (& theWitRun);

   witInitialize         (theWitRun);

   witSetNPeriods        (theWitRun, 3);

   witAddPart            (theWitRun, "A",      WitMATERIAL);

   witAddDemand          (theWitRun, "A", "B");

   witSetPartSupplyVol   (theWitRun, "A",      supplyVol);

   witSetDemandDemandVol (theWitRun, "A", "B", demandVol);

   witStartHeurAlloc     (theWitRun);

   witIncHeurAlloc       (theWitRun, "A", "B", 1, 29, & incVol);
   witIncHeurAlloc       (theWitRun, "A", "B", 2, 13, & incVol);
   witIncHeurAlloc       (theWitRun, "A", "B", 0, 14, & incVol);

   assert (fabs (incVol - 8.0) < 0.00001);

   witShutDownHeurAlloc  (theWitRun);

   witGetPostprocessed   (theWitRun, & postproc);

   assert (! postproc);

   witDeleteRun          (theWitRun);
   }

/*----------------------------------------------------------------------------*/
/* Function testExtOpt.                                                       */
/*                                                                            */
/* Tests external optimizing implosion.                                       */
/*----------------------------------------------------------------------------*/

void testExtOpt ()
   {
   WitRun *   theWitRun;
   witBoolean extOptActive;

   witNewRun            (& theWitRun);

   buildExtOptProblem     (theWitRun);

   witGetExtOptActive     (theWitRun, & extOptActive);
   assert                            (! extOptActive);

   witStartExtOpt         (theWitRun);

   witGetExtOptActive     (theWitRun, & extOptActive);
   assert                              (extOptActive);

   testGetExtOptLpProb    (theWitRun);
   testGetExtOptLpProbDbl (theWitRun);
   testGetConIndices      (theWitRun);
   testGetVarIndices      (theWitRun);
   testGetSlbvVarIndices  (theWitRun);
   testGetSlbConIndices   (theWitRun);

   witFinishExtOpt        (theWitRun);

   witGetExtOptActive     (theWitRun, & extOptActive);
   assert                            (! extOptActive);

   witStartExtOpt         (theWitRun);
   witShutDownExtOpt      (theWitRun);

   witGetExtOptActive     (theWitRun, & extOptActive);
   assert                            (! extOptActive);

   witDeleteRun           (theWitRun);
   }

/*----------------------------------------------------------------------------*/
/* buildExtOptProblem                                                         */
/*----------------------------------------------------------------------------*/

void buildExtOptProblem (WitRun * theWitRun)
   {
   float offset[]     = {2.0, 2.0, 2.0};
   float execSlb[]    = {3.0, 4.0, 5.0};
   float stockSlb[]   = {2.0, 0.0, 3.0};
   float cumShipSlb[] = {1.0, 3.0, 0.0};

   witInitialize             (theWitRun);

   witSetNPeriods            (theWitRun,             3);

   witAddPart                (theWitRun,   "A",      WitMATERIAL);
   witAddPart                (theWitRun,   "B",      WitMATERIAL);
   witAddOperation           (theWitRun,   "C");
   witAddBomEntry            (theWitRun,   "C", "A");
   witAddBopEntry            (theWitRun,   "C", "B");
   witAddSubsBomEntry        (theWitRun,   "C",  0,  "A");
   witAddDemand              (theWitRun,   "B", "D");

   witSetBomEntryOffset      (theWitRun,   "C",  0,  offset);
   witSetOperationExecBounds (theWitRun,   "C",      NULL, execSlb,    NULL);
   witSetPartStockBounds     (theWitRun,   "B",      NULL, stockSlb,   NULL);
   witSetDemandCumShipBounds (theWitRun,   "B", "D", NULL, cumShipSlb, NULL);
   }

/*----------------------------------------------------------------------------*/
/* testGetExtOptLpProb                                                        */
/*----------------------------------------------------------------------------*/

void testGetExtOptLpProb (WitRun * theWitRun)
   {
   int     numcols;
   int     numrows;
   int *   start;
   int *   index;
   float * value;
   float * collb;
   float * colub;
   float * obj;
   float * rowlb;
   float * rowub;

   witGetExtOptLpProb (
        theWitRun,
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

   witSetExtOptSoln (theWitRun, collb);

   witFree (start);
   witFree (index);
   witFree (value);
   witFree (collb);
   witFree (colub);
   witFree (obj);
   witFree (rowlb);
   witFree (rowub);
   }

/*----------------------------------------------------------------------------*/
/* testGetExtOptLpProbDbl                                                     */
/*----------------------------------------------------------------------------*/

void testGetExtOptLpProbDbl (WitRun * theWitRun)
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
   int *    indices;
   int      nIntVars;

   witGetExtOptLpProbDbl (
        theWitRun,
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

   witGetExtOptIntVarIndices (
        theWitRun,
      & indices,
      & nIntVars);

   witSetExtOptSolnDbl (theWitRun, collb);

   witFree (start);
   witFree (index);
   witFree (value);
   witFree (collb);
   witFree (colub);
   witFree (obj);
   witFree (rowlb);
   witFree (rowub);
   witFree (indices);
   }

/*----------------------------------------------------------------------------*/
/* testGetConIndices                                                          */
/*----------------------------------------------------------------------------*/

void testGetConIndices (WitRun * theWitRun)
   {
   int conIndex;

   witGetPartResourceConIndex (theWitRun, "A", 2, & conIndex);

   printf (
      "\n"
      "Resource Allocation Constraint Index:\n"
      "   Part:   A\n"
      "   Period: 2\n"
      "   Index: %2d\n",
      conIndex);

   witGetDemandShipConIndex (theWitRun, "B", "D", 1, & conIndex);

   printf (
      "\n"
      "Shipment Cumulation Constraint Index:\n"
      "   Part:   B\n"
      "   Demand: D\n"
      "   Period: 1\n"
      "   Index: %2d\n",
      conIndex);

   witGetBomEntrySubConIndex (theWitRun, "C", 0, 1, & conIndex);

   printf (
      "\n"
      "Substitution Constraint Index:\n"
      "   Operation: C\n"
      "   BOM Entry: 0\n"
      "   Period:    1\n"
      "   Index:    %2d\n",
      conIndex);
   }

/*----------------------------------------------------------------------------*/
/* testGetVarIndices                                                          */
/*----------------------------------------------------------------------------*/

void testGetVarIndices (WitRun * theWitRun)
   {
   int varIndex;

   witGetOperationExecVarIndex (theWitRun, "C", 1, & varIndex);

   printf (
      "\n"
      "Execution Variable Index:\n"
      "   Operation: C\n"
      "   Period:    1\n"
      "   Index:    %2d\n",
      varIndex);

   witGetPartScrapVarIndex (theWitRun, "A", 2, & varIndex);

   printf (
      "\n"
      "Scrap Variable Index:\n"
      "   Part:   A\n"
      "   Period: 2\n"
      "   Index: %2d\n",
      varIndex);

   witGetPartStockVarIndex (theWitRun, "B", 0, & varIndex);

   printf (
      "\n"
      "Stock Variable Index:\n"
      "   Part:   B\n"
      "   Period: 0\n"
      "   Index: %2d\n",
      varIndex);

   witGetDemandShipVarIndex (theWitRun, "B", "D", 0, & varIndex);

   printf (
      "\n"
      "Shipment Variable Index:\n"
      "   Part:   B\n"
      "   Demand: D\n"
      "   Period: 0\n"
      "   Index: %2d\n",
      varIndex);

   witGetDemandCumShipVarIndex (theWitRun, "B", "D", 2, & varIndex);

   printf (
      "\n"
      "Cumulative Shipment Variable Index:\n"
      "   Part:   B\n"
      "   Demand: D\n"
      "   Period: 2\n"
      "   Index: %2d\n",
      varIndex);

   witGetBomEntryNonSubVarIndex (theWitRun, "C", 0, 0, & varIndex);

   printf (
      "\n"
      "Non-Substitution Variable Index:\n"
      "   Operation: C\n"
      "   BOM Entry: 0\n"
      "   Period:    0\n"
      "   Index:    %2d\n",
      varIndex);

   witGetSubsBomEntrySubVarIndex (theWitRun, "C", 0, 0, 2, & varIndex);

   printf (
      "\n"
      "Substitution Variable Index:\n"
      "   Operation:  C\n"
      "   BOM Entry:  0\n"
      "   Substitute: 0\n"
      "   Period:     2\n"
      "   Index:     %2d\n",
      varIndex);
   }

/*----------------------------------------------------------------------------*/
/* testGetSlbvVarIndices                                                      */
/*----------------------------------------------------------------------------*/

void testGetSlbvVarIndices (WitRun * theWitRun)
   {
   int varIndex;

   witGetOperationExecSlbvVarIndex (theWitRun, "C", 2, & varIndex);

   printf (
      "\n"
      "Execution SLBV Variable Index:\n"
      "   Operation: C\n"
      "   Period:    2\n"
      "   Index:    %2d\n",
      varIndex);

   witGetPartStockSlbvVarIndex (theWitRun, "B", 0, & varIndex);

   printf (
      "\n"
      "Stock SLBV Variable Index:\n"
      "   Part:   B\n"
      "   Period: 0\n"
      "   Index: %2d\n",
      varIndex);

   witGetDemandCumShipSlbvVarIndex (theWitRun, "B", "D", 1, & varIndex);

   printf (
      "\n"
      "Cumulative Shipment SLBV Variable Index:\n"
      "   Part:   B\n"
      "   Demand: D\n"
      "   Period: 2\n"
      "   Index: %2d\n",
      varIndex);
   }

/*----------------------------------------------------------------------------*/
/* testGetSlbConIndices                                                       */
/*----------------------------------------------------------------------------*/

void testGetSlbConIndices (WitRun * theWitRun)
   {
   int conIndex;

   witGetOperationExecSlbConIndex (theWitRun, "C", 2, & conIndex);

   printf (
      "\n"
      "Execution SLB Constraint Index:\n"
      "   Operation: C\n"
      "   Period:    2\n"
      "   Index:    %2d\n",
      conIndex);

   witGetPartStockSlbConIndex (theWitRun, "B", 2, & conIndex);

   printf (
      "\n"
      "Stock SLB Constraint Index:\n"
      "   Part:   B\n"
      "   Period: 2\n"
      "   Index: %2d\n",
      conIndex);

   witGetDemandCumShipSlbConIndex (theWitRun, "B", "D", 0, & conIndex);

   printf (
      "\n"
      "Cumulative Shipment SLB Constraint Index:\n"
      "   Part:   B\n"
      "   Demand: D\n"
      "   Period: 0\n"
      "   Index: %2d\n",
      conIndex);
   }

/*----------------------------------------------------------------------------*/
/* testCopyObjData1WR                                                         */
/*                                                                            */
/* Tests witCopy<Object>Data using one WitRun.                                */
/*----------------------------------------------------------------------------*/

void testCopyObjData1WR ()
   {
   WitRun *   theWitRun;
   witBoolean theBool;

   witNewRun                 (& theWitRun);
   witInitialize               (theWitRun);

   witAddPart                  (theWitRun, "A", WitMATERIAL);
   witAddPart                  (theWitRun, "B", WitMATERIAL);
   witAddPart                  (theWitRun, "C", WitMATERIAL);
   witAddOperation             (theWitRun, "D");
   witAddOperation             (theWitRun, "E");
   witAddBomEntry              (theWitRun, "D", "A");
   witAddBomEntry              (theWitRun, "E", "B");
   witAddBopEntry              (theWitRun, "D", "B");
   witAddBopEntry              (theWitRun, "E", "C");
   witAddSubsBomEntry          (theWitRun, "D",  0, "A");
   witAddSubsBomEntry          (theWitRun, "E",  0, "A");
   witAddDemand                (theWitRun, "C", "F");
   witAddDemand                (theWitRun, "C", "G");

   witSetPartSelForDel         (theWitRun, "A",         WitTRUE);
   witSetOperationSelForDel    (theWitRun, "D",         WitTRUE);
   witSetBomEntrySelForDel     (theWitRun, "D", 0,      WitTRUE);
   witSetBopEntrySelForDel     (theWitRun, "D", 0,      WitTRUE);
   witSetSubsBomEntrySelForDel (theWitRun, "D", 0, 0,   WitTRUE);
   witSetDemandSelForDel       (theWitRun, "C", "F",    WitTRUE);

   witCopyPartData             (theWitRun, "B",         theWitRun, "A");
   witCopyOperationData        (theWitRun, "E",         theWitRun, "D");
   witCopyBomEntryData         (theWitRun, "E", 0,      theWitRun, "D", 0);
   witCopyBopEntryData         (theWitRun, "E", 0,      theWitRun, "D", 0);
   witCopySubsBomEntryData     (theWitRun, "E", 0, 0,   theWitRun, "D", 0, 0);
   witCopyDemandData           (theWitRun, "C", "G",    theWitRun, "C", "F");

   witGetPartSelForDel         (theWitRun, "B",       & theBool);
   assert                                              (theBool);
   witGetOperationSelForDel    (theWitRun, "E",       & theBool);
   assert                                              (theBool);
   witGetBomEntrySelForDel     (theWitRun, "E", 0,    & theBool);
   assert                                              (theBool);
   witGetBopEntrySelForDel     (theWitRun, "E", 0,    & theBool);
   assert                                              (theBool);
   witGetSubsBomEntrySelForDel (theWitRun, "E", 0, 0, & theBool);
   assert                                              (theBool);
   witGetDemandSelForDel       (theWitRun, "C", "G",  & theBool);
   assert                                              (theBool);

   witDeleteRun                (theWitRun);
   }

/*----------------------------------------------------------------------------*/
/* testCopyObjData2WRs                                                        */
/*                                                                            */
/* Tests witCopy<Object>Data using two WitRuns.                               */
/*----------------------------------------------------------------------------*/

void testCopyObjData2WRs ()
   {
   WitRun *   origWitRun;
   WitRun *    dupWitRun;
   witBoolean theBool;

   witNewRun                 (& origWitRun);
   witInitialize               (origWitRun);

   witAddPart                  (origWitRun, "A", WitMATERIAL);
   witAddPart                  (origWitRun, "B", WitMATERIAL);
   witAddOperation             (origWitRun, "C");
   witAddBomEntry              (origWitRun, "C", "A");
   witAddBopEntry              (origWitRun, "C", "B");
   witAddSubsBomEntry          (origWitRun, "C",  0, "A");
   witAddDemand                (origWitRun, "B", "D");

   witSetPartSelForDel         (origWitRun, "A",         WitTRUE);
   witSetOperationSelForDel    (origWitRun, "C",         WitTRUE);
   witSetBomEntrySelForDel     (origWitRun, "C", 0,      WitTRUE);
   witSetBopEntrySelForDel     (origWitRun, "C", 0,      WitTRUE);
   witSetSubsBomEntrySelForDel (origWitRun, "C", 0, 0,   WitTRUE);
   witSetDemandSelForDel       (origWitRun, "B", "D",    WitTRUE);

   witNewRun                  (& dupWitRun);
   witInitialize                (dupWitRun);

   witAddPart                   (dupWitRun, "A", WitMATERIAL);
   witAddPart                   (dupWitRun, "B", WitMATERIAL);
   witAddOperation              (dupWitRun, "C");
   witAddBomEntry               (dupWitRun, "C", "A");
   witAddBopEntry               (dupWitRun, "C", "B");
   witAddSubsBomEntry           (dupWitRun, "C",  0, "A");
   witAddDemand                 (dupWitRun, "B", "D");

   witCopyPartData              (dupWitRun, "A",         origWitRun, "A");
   witCopyOperationData         (dupWitRun, "C",         origWitRun, "C");
   witCopyBomEntryData          (dupWitRun, "C", 0,      origWitRun, "C", 0);
   witCopyBopEntryData          (dupWitRun, "C", 0,      origWitRun, "C", 0);
   witCopySubsBomEntryData      (dupWitRun, "C", 0, 0,   origWitRun, "C", 0, 0);
   witCopyDemandData            (dupWitRun, "B", "D",    origWitRun, "B", "D");

   witGetPartSelForDel          (dupWitRun, "A",       & theBool);
   assert                                               (theBool);
   witGetOperationSelForDel     (dupWitRun, "C",       & theBool);
   assert                                               (theBool);
   witGetBomEntrySelForDel      (dupWitRun, "C", 0,    & theBool);
   assert                                               (theBool);
   witGetBopEntrySelForDel      (dupWitRun, "C", 0,    & theBool);
   assert                                               (theBool);
   witGetSubsBomEntrySelForDel  (dupWitRun, "C", 0, 0, & theBool);
   assert                                               (theBool);
   witGetDemandSelForDel        (dupWitRun, "B", "D",  & theBool);
   assert                                               (theBool);

   witDeleteRun                 (dupWitRun);
   witDeleteRun                (origWitRun);
   }

/*----------------------------------------------------------------------------*/
/* testShipLateAllowed                                                        */
/*                                                                            */
/* Test of WIT with shipLateAllowed = FALSE                                   */
/*----------------------------------------------------------------------------*/

void testShipLateAllowed ()
   {
   WitRun *   theWitRun;
   float      supplyVolA [] = {100.0, 400.0};
   float      demandVolB [] = {200.0, 200.0};
   witBoolean shipLateAllowedB;
   float *    shipVolB;
   float      fssShipVolB[] = {700.0, 700.0};
   float *    shortVolA;

   witNewRun                 (& theWitRun);
   witInitialize               (theWitRun);

   witSetNPeriods              (theWitRun, 2);

   witAddPart                  (theWitRun, "A",        WitCAPACITY );
   witAddDemand                (theWitRun, "A", "B");

   witSetPartSupplyVol         (theWitRun, "A",        supplyVolA);
   witSetDemandDemandVol       (theWitRun, "A", "B",   demandVolB);
   witSetDemandShipLateAllowed (theWitRun, "A", "B",   WitFALSE);
   witGetDemandShipLateAllowed (theWitRun, "A", "B", & shipLateAllowedB);
   assert                                           (! shipLateAllowedB);

   witHeurImplode              (theWitRun);

   witGetDemandShipVol         (theWitRun, "A", "B", & shipVolB);

   printFltVec                 ("B.shipVol",           shipVolB,  2);
   witFree                                            (shipVolB);

   witSetUseFocusHorizons      (theWitRun,             WitFALSE);
   witSetDemandFssShipVol      (theWitRun, "A", "B",   fssShipVolB);
   witGetPartFocusShortageVol  (theWitRun, "A",      & shortVolA);

   printFltVec                 ("A.shortVol",          shortVolA, 2);
   witFree                                            (shortVolA);

   witDeleteRun                (theWitRun);
   }

/*----------------------------------------------------------------------------*/
/* testGetExpCycle                                                            */
/*                                                                            */
/* Test of witGetExpCycle                                                     */
/*----------------------------------------------------------------------------*/

void testGetExpCycle ()
   {
   WitRun * theWitRun;

   witNewRun       (& theWitRun);
   buildCycleProblem (theWitRun);
   displayCycle      (theWitRun);
   witDeleteRun      (theWitRun);
   }

/*----------------------------------------------------------------------------*/

void buildCycleProblem (WitRun * theWitRun)
   {
   witInitialize   (theWitRun);

   witAddPart      (theWitRun, "A", WitMATERIAL);
   witAddOperation (theWitRun, "B");
   witAddPart      (theWitRun, "C", WitMATERIAL);
   witAddOperation (theWitRun, "D");
   witAddBomEntry  (theWitRun, "B", "A");
   witAddBopEntry  (theWitRun, "B", "C");
   witAddBomEntry  (theWitRun, "D", "C");
   witAddBopEntry  (theWitRun, "D", "A");
   }

/*----------------------------------------------------------------------------*/

void displayCycle (WitRun * theWitRun)
   {
   int      lenLists;
   char * * partNameList;
   char * * opnNameList;
   int      theIdx;

   witGetExpCycle (
        theWitRun,
      & lenLists,
      & partNameList,
      & opnNameList);

   assert (lenLists > 0);

   printf (
      "\n"
      "The following explodeable cycle was found:\n");

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      printf (
         "   Part      %s\n"
         "   Operation %s\n",
         partNameList[theIdx],
         opnNameList [theIdx]);
      
   printf (
      "   Part      %s\n",
      partNameList[0]);

   for (theIdx = 0; theIdx < lenLists; theIdx ++)
      {
      witFree (partNameList[theIdx]);
      witFree (opnNameList [theIdx]);
      }

   witFree (partNameList);
   witFree (opnNameList);
   }

/*----------------------------------------------------------------------------*/
/* testStageByObject                                                          */
/*                                                                            */
/* Test of API functions for stochastic implosion in stage-by-object mode.    */
/*----------------------------------------------------------------------------*/

void testStageByObject ()
   {
   WitRun * theWitRun;

   if (! coinEmbedded)
      if (! cplexEmbedded)
         return;

   witNewRun         (& theWitRun);
   buildCoreProblemSBO (theWitRun);
   specifyStochDataSBO (theWitRun);
   checkStochData      (theWitRun);
   witDisplayData      (theWitRun, WitSTDOUT);
   printf                         ("\n");
   testStochWriteData  (theWitRun);
   testStochCopyData   (theWitRun);
   testStochImplode    (theWitRun);
   checkStochSoln      (theWitRun);
   testClearStochSoln  (theWitRun);

   witDeleteRun        (theWitRun);
   }

/*----------------------------------------------------------------------------*/
/* buildCoreProblemSBO                                                        */
/*                                                                            */
/* Builds the core problem for the stochastic implosion stage-by-object test. */
/*----------------------------------------------------------------------------*/

void buildCoreProblemSBO (WitRun * theWitRun)
   {
   float supplyVolA [] = {500.0};
   float demandVolD [] = {300.0};
   float execCostC  [] =   {1.0};
   float shipRewardD[] =   {5.0};

   witSetMesgPrintNumber     (theWitRun, WitTRUE,
                                         WitINFORMATIONAL_MESSAGES,
                                         WitFALSE);   

   witInitialize             (theWitRun);

   witSetNPeriods            (theWitRun, 1);
   witSetWbounds             (theWitRun, 0.0);
   witSetOptInitMethod       (theWitRun, WitCRASH_OPT_INIT_METHOD);
   witSetPreferCoin          (theWitRun, WitTRUE);

   witAddPart                (theWitRun, "A",      WitMATERIAL);
   witAddPart                (theWitRun, "B",      WitMATERIAL);
   witAddOperation           (theWitRun, "C");
   witAddBomEntry            (theWitRun, "C", "A");
   witAddBopEntry            (theWitRun, "C", "B");
   witAddDemand              (theWitRun, "B", "D");

   witSetPartSupplyVol       (theWitRun, "A",      supplyVolA);
   witSetDemandDemandVol     (theWitRun, "B", "D", demandVolD);
   witSetDemandCumShipBounds (theWitRun, "B", "D", NULL,
                                                   demandVolD,
                                                   NULL);
   witSetOperationExecCost   (theWitRun, "C",      execCostC);
   witSetDemandShipReward    (theWitRun, "B", "D", shipRewardD);
   }

/*----------------------------------------------------------------------------*/
/* specifyStochDataSBO                                                        */
/*                                                                            */
/* Specifies the stochastic data for the stochastic implosion test.           */
/*----------------------------------------------------------------------------*/

void specifyStochDataSBO (WitRun * theWitRun)
   {
   float demandVolD0[] = {100.0};
   float demandVolD1[] = {200.0};

   witSetStageByObject        (theWitRun,           WitTRUE);
   witSetNScenarios           (theWitRun,           3);
   witSetStochMode            (theWitRun,           WitTRUE);
   witSetPartObjectStage      (theWitRun, "A",      0);
   witSetPartObjectStage      (theWitRun, "B",      1);
   witSetOperationObjectStage (theWitRun, "C",      0);
   witSetPartAppData          (theWitRun, "A",      theWitRun);

   witSetCurrentScenario      (theWitRun,           0);
   witSetProbability          (theWitRun,           0.6);
   witSetDemandDemandVol      (theWitRun, "B", "D", demandVolD0);
   witSetDemandCumShipBounds  (theWitRun, "B", "D", NULL,
                                                    demandVolD0,
                                                    NULL);

   witSetCurrentScenario      (theWitRun,           1);
   witSetProbabilityDbl       (theWitRun,           0.3);
   witSetDemandDemandVol      (theWitRun, "B", "D", demandVolD1);
   witSetDemandCumShipBounds  (theWitRun, "B", "D", NULL,
                                                    demandVolD1,
                                                    NULL);

   witSetCurrentScenario      (theWitRun,           2);
   witSetProbability          (theWitRun,           0.1);

   witSetCurrentScenario      (theWitRun,           1);
   }

/*----------------------------------------------------------------------------*/
/* checkStochData                                                             */
/*                                                                            */
/* Verifies the stochastic data for the stochastic implosion test.            */
/*----------------------------------------------------------------------------*/

void checkStochData (WitRun * theWitRun)
   {
   witBoolean theBool;
   int        theInt;
   float      theFlt;
   double     theDbl;

   witGetStageByObject        (theWitRun,      & theBool);
   assert                                       (theBool);

   witGetNScenarios           (theWitRun,      & theInt);
   assert                                       (theInt == 3);

   witGetStochMode            (theWitRun,      & theBool);
   assert                                       (theBool);

   witGetCurrentScenario      (theWitRun,      & theInt);
   assert                                       (theInt == 1);

   witGetOperationObjectStage (theWitRun, "C", & theInt);
   assert                                       (theInt == 0);

   witGetPartObjectStage      (theWitRun, "B", & theInt);
   assert                                       (theInt == 1);

   /* Check data for scenario #0. */

   witSetCurrentScenario (theWitRun,   0);
   witGetProbabilityDbl  (theWitRun, & theDbl);

   assert (fabs (theDbl - 0.6) < dblEps);

   /* Check data for scenario #1. */

   witSetCurrentScenario (theWitRun,   1);
   witGetProbability     (theWitRun, & theFlt);

   assert (fabs (theFlt - 0.3) < dblEps);

   /* Check data for scenario #2. */

   witSetCurrentScenario (theWitRun,   2);
   witGetProbabilityDbl  (theWitRun, & theDbl);

   assert (fabs (theDbl - 0.1) < dblEps);

   witSetCurrentScenario (theWitRun,   1);
   }

/*----------------------------------------------------------------------------*/
/* testStochWriteData                                                         */
/*                                                                            */
/* Tests witWriteData in stochastic implosion mode.                           */
/*----------------------------------------------------------------------------*/

void testStochWriteData (WitRun * theWitRun)
   {
   WitRun * theWitRun2;

   witDisplayData           (theWitRun, "disp1.out");
   witWriteData             (theWitRun, "stoch.data");

   witNewRun              (& theWitRun2);
   witSetMesgPrintNumber    (theWitRun2, WitTRUE,
                                         WitINFORMATIONAL_MESSAGES,
                                         WitFALSE);   
   witInitialize            (theWitRun2);
   witReadData              (theWitRun2, "stoch.data");
   witDisplayData           (theWitRun2, "disp2.out");
   witDeleteRun             (theWitRun2);

   remove ("stoch.data");

   printf ("\n"
           "-----------\n"
           "Diff Output\n"
           "-----------\n"
           "\n");

   fflush  (stdout);

   system ("diff -I \"^Run Started At: \" disp1.out disp2.out");

   printf ("\n"
           "------------------\n"
           "End of Diff Output\n"
           "------------------\n"
           "\n");

   fflush  (stdout);

   remove ("disp1.out");
   remove ("disp2.out");
   }

/*----------------------------------------------------------------------------*/
/* testStochCopyData                                                          */
/*                                                                            */
/* Tests witCopyData in stochastic implosion mode.                            */
/*----------------------------------------------------------------------------*/

void testStochCopyData (WitRun * theWitRun)
   {
   WitRun * theWitRun2;

   witDisplayData           (theWitRun, "disp1.out");

   witNewRun              (& theWitRun2);
   witSetMesgPrintNumber    (theWitRun2, WitTRUE,
                                         WitINFORMATIONAL_MESSAGES,
                                         WitFALSE);   
   witInitialize            (theWitRun2);
   witCopyData              (theWitRun2, theWitRun);
   witPreprocess            (theWitRun2);
   witDisplayData           (theWitRun2, "disp2.out");
   witDeleteRun             (theWitRun2);

   remove ("stoch.data");

   printf ("\n"
           "-----------\n"
           "Diff Output\n"
           "-----------\n"
           "\n");

   fflush  (stdout);

   system ("diff -I \"^Run Started At: \" disp1.out disp2.out");

   printf ("\n"
           "------------------\n"
           "End of Diff Output\n"
           "------------------\n"
           "\n");

   fflush  (stdout);

   remove ("disp1.out");
   remove ("disp2.out");
   }

/*----------------------------------------------------------------------------*/
/* testStochImplode                                                           */
/*                                                                            */
/* Tests witStochImplode.                                                     */
/*----------------------------------------------------------------------------*/

void testStochImplode (WitRun * theWitRun)
   {
   witBoolean theBool;

   witGetStochSolnMode (theWitRun, & theBool);
   assert                         (! theBool);

   witStochImplode     (theWitRun);

   witGetStochSolnMode (theWitRun, & theBool);
   assert                           (theBool);
   }

/*----------------------------------------------------------------------------*/
/* checkStochSoln                                                             */
/*                                                                            */
/* Verifies the stochastic implosion solution for the stochastic implosion    */
/* test.                                                                      */
/*----------------------------------------------------------------------------*/

void checkStochSoln (WitRun * theWitRun)
   {
   float   objValue;
   float   boundsValue;
   float * theFltVec;
   float   execVolC[]      = {200.};
   float   residualVolB0[] = {100.};
   float   zeroVec[]       = {  0.};

   /*-------------------------------------------------------------------------*/

   witGetObjValues        (theWitRun, & objValue, & boundsValue );

   printf (
      "\n"
      "Stochastic objective function values:\n"
      "   objValue    = %3.0f\n"
      "   boundsValue = %3.0f\n",
      objValue,
      boundsValue);

   assert (fabs (objValue    - 500.0) < dblEps);
   assert (fabs (boundsValue -  10.0) < dblEps);

   /*-------------------------------------------------------------------------*/

   witSetCurrentScenario  (theWitRun, 0);

   witGetOperationExecVol (theWitRun, "C", & theFltVec);
   compareFltVecs                           (theFltVec, execVolC,      1);
   witFree                                  (theFltVec);

   witGetPartResidualVol  (theWitRun, "B", & theFltVec);
   compareFltVecs                           (theFltVec, residualVolB0, 1);
   witFree                                  (theFltVec);

   /*-------------------------------------------------------------------------*/

   witSetCurrentScenario  (theWitRun, 2);

   witGetOperationExecVol (theWitRun, "C", & theFltVec);
   compareFltVecs                           (theFltVec, execVolC,      1);
   witFree                                  (theFltVec);

   witGetPartResidualVol  (theWitRun, "B", & theFltVec);
   compareFltVecs                           (theFltVec, zeroVec,       1);
   witFree                                  (theFltVec);
   }

/*----------------------------------------------------------------------------*/
/* testClearStochSoln                                                         */
/*                                                                            */
/* Tests witClearStochSoln.                                                   */
/*----------------------------------------------------------------------------*/

void testClearStochSoln (WitRun * theWitRun)
   {
   float * theFltVec;
   float   zeroVec[]    = {  0.};
   float   supplyVolB[] = {107.};

   /*-------------------------------------------------------------------------*/

   witClearStochSoln   (theWitRun);

   witGetPartExcessVol (theWitRun, "A", & theFltVec);
   compareFltVecs                        (theFltVec, zeroVec, 1);
   witFree                               (theFltVec);

   witSetPartSupplyVol (theWitRun, "B",   supplyVolB);
   }

/*----------------------------------------------------------------------------*/
/* testStageByPeriod                                                          */
/*                                                                            */
/* Test of stochastic implosion in stage-by-period mode.                      */
/*----------------------------------------------------------------------------*/

void testStageByPeriod ()
   {
   WitRun * theWitRun;

   if (! coinEmbedded)
      if (! cplexEmbedded)
         return;

   witNewRun         (& theWitRun);
   buildCoreProblemSBP (theWitRun);
   specifyStochDataSBP (theWitRun);
   witDisplayData      (theWitRun, WitSTDOUT);
   witStochImplode     (theWitRun);
   witDeleteRun        (theWitRun);
   }

/*----------------------------------------------------------------------------*/
/* buildCoreProblemSBP                                                        */
/*                                                                            */
/* Builds the core problem for the stochastic implosion stage-by-period test. */
/*----------------------------------------------------------------------------*/

void buildCoreProblemSBP (WitRun * theWitRun)
   {
   float execCostB  [] = { 4.0,  4.0};
   float execCostC  [] = { 7.0,  7.0};
   float shipRewardD[] = {10.0, 10.0};
   float offsetB    [] = {-1,   -1};
   float offsetC    [] = { 0,    0};

   witInitialize           (theWitRun);

   witSetNPeriods          (theWitRun, 2);
   witSetOptInitMethod     (theWitRun, WitCRASH_OPT_INIT_METHOD);
   witSetPreferCoin        (theWitRun, WitTRUE);

   witAddPart              (theWitRun, "A", WitCAPACITY);
   witAddOperation         (theWitRun, "B");
   witAddOperation         (theWitRun, "C");
   witAddBopEntry          (theWitRun, "B", "A");
   witAddBopEntry          (theWitRun, "C", "A");
   witAddDemand            (theWitRun, "A", "D");

   witSetOperationExecCost (theWitRun, "B",      execCostB);
   witSetOperationExecCost (theWitRun, "C",      execCostC);
   witSetDemandShipReward  (theWitRun, "A", "D", shipRewardD);

   witSetBopEntryOffset    (theWitRun, "B",  0,  offsetB);
   witSetBopEntryOffset    (theWitRun, "C",  0,  offsetC);
   }

/*----------------------------------------------------------------------------*/
/* specifyStochDataSBP                                                        */
/*                                                                            */
/* Specifies the stochastic data for the stochastic implosion stage-by-period */
/* test.                                                                      */
/*----------------------------------------------------------------------------*/

void specifyStochDataSBP (WitRun * theWitRun)
   {
   int   periodStage[] = {0,    1};
   float demandVolD0[] = {0.0, 20.0};
   float demandVolD1[] = {0.0, 30.0};

   witSetStageByObject   (theWitRun, WitFALSE);
   witSetNScenarios      (theWitRun, 2);
   witSetStochMode       (theWitRun, WitTRUE);
   witSetPeriodStage     (theWitRun, periodStage);

   witSetCurrentScenario (theWitRun,           0);
   witSetDemandDemandVol (theWitRun, "A", "D", demandVolD0);

   witSetCurrentScenario (theWitRun,           1);
   witSetDemandDemandVol (theWitRun, "A", "D", demandVolD1);
   }

/*----------------------------------------------------------------------------*/
/* Function testMultiObjMode                                                  */
/*                                                                            */
/* Tests multiple objectives mode.                                            */
/*----------------------------------------------------------------------------*/

void testMultiObjMode ()
   {
   WitRun *   theWitRun;
   WitRun *   cpyWitRun;
   witBoolean theBool;

   if (! cplexEmbedded)
      return;

   witNewRun        (& theWitRun);

   witInitialize      (theWitRun);

   witSetNPeriods     (theWitRun,   2);

   witSetMultiObjMode (theWitRun,   WitTRUE);
   witGetMultiObjMode (theWitRun, & theBool);
   assert                          (theBool);

   testObjListSpec    (theWitRun);
   testObjList        (theWitRun);
   testObjSeqNo       (theWitRun);
   testMultiObjTol    (theWitRun);
   populateMultiObj   (theWitRun);
   testSetObjVecs     (theWitRun);

   witNewRun        (& cpyWitRun);

   witCopyData        (cpyWitRun, theWitRun);

   witDeleteRun       (theWitRun);

   testGetObjVecs     (cpyWitRun);

   witOptImplode      (cpyWitRun);
       /**/
      /* Not ready for COIN yet: multiObjMode */

   testMultiObjValue  (cpyWitRun);

   witDeleteRun       (cpyWitRun);
   }

/*----------------------------------------------------------------------------*/
/* Function testObjListSpec                                                   */
/*                                                                            */
/* Tests witSetObjectiveListSpec and related functions.                       */
/*----------------------------------------------------------------------------*/

void testObjListSpec (WitRun * theWitRun)
   {
   char *   theSpec;
   int      lenList;
   char * * theList;

   witSetObjectiveListSpec (theWitRun, "temp1|temp2|temp3");

   witGetObjectiveListSpec (theWitRun, & theSpec);

   assert (equalStrings (theSpec, "temp1|temp2|temp3"));

   witFree (theSpec);

   witGetObjectiveList     (theWitRun, & lenList, & theList);

   assert (lenList == 3);

   assert (equalStrings (theList[0], "temp1"));
   assert (equalStrings (theList[1], "temp2"));
   assert (equalStrings (theList[2], "temp3"));

   witFree (theList[0]);
   witFree (theList[1]);
   witFree (theList[2]);
   witFree (theList);
   }

/*----------------------------------------------------------------------------*/
/* Function testObjList                                                       */
/*                                                                            */
/* Tests witSetObjectiveList and related functions.                           */
/*----------------------------------------------------------------------------*/

void testObjList (WitRun * theWitRun)
   {
   const char * theObjList[] = {"LOW", "HIGH"};
   char * *     outObjList;
   int          lenList;
   char *       theSpec;

   witSetObjectiveList (theWitRun,   2,         theObjList);
   witGetObjectiveList (theWitRun, & lenList, & outObjList);

   assert (lenList == 2);

   assert (equalStrings (outObjList[0], "LOW"));
   assert (equalStrings (outObjList[1], "HIGH"));

   witFree (outObjList[0]);
   witFree (outObjList[1]);
   witFree (outObjList);

   witGetObjectiveListSpec (theWitRun, & theSpec);

   assert (equalStrings (theSpec, "LOW|HIGH"));

   witFree (theSpec);
   }

/*----------------------------------------------------------------------------*/
/* testObjSeqNo                                                               */
/*                                                                            */
/* Tests functions for objectiveSeqNo and currentObjective.                   */
/*----------------------------------------------------------------------------*/

void testObjSeqNo (WitRun * theWitRun)
   {
   char * theObjName;
   int    theSeqNo;

   witSetCurrentObjective (theWitRun, "LOW");
   witSetObjectiveSeqNo   (theWitRun, 2);

   witSetCurrentObjective (theWitRun, "HIGH");
   witSetObjectiveSeqNo   (theWitRun, 1);

   witGetCurrentObjective (theWitRun, & theObjName);
   witGetObjectiveSeqNo   (theWitRun, & theSeqNo);

   assert (equalStrings (theObjName, "HIGH"));

   assert (theSeqNo == 1);

   witFree (theObjName);
   }

/*----------------------------------------------------------------------------*/
/* testMultiObjTol                                                            */
/*                                                                            */
/* Tests the multiObjTol attribute.                                           */
/*----------------------------------------------------------------------------*/

void testMultiObjTol (WitRun * theWitRun)
   {
   float  theFlt;
   double theDbl;

   witSetMultiObjTol    (theWitRun,   1.e-6);
   witSetMultiObjTolDbl (theWitRun,   1.e-5);
   witGetMultiObjTol    (theWitRun, & theFlt);
   witGetMultiObjTolDbl (theWitRun, & theDbl);

   assert (fabs (theFlt - 1.e-5) < 1.e-10);
   assert (fabs (theDbl - 1.e-5) < 1.e-10);
   }

/*----------------------------------------------------------------------------*/
/* populateMultiObj                                                           */
/*                                                                            */
/* Creates all data objects for the WitRun for the multiple objectives mode   */
/* test.                                                                      */
/*----------------------------------------------------------------------------*/

void populateMultiObj (WitRun * theWitRun)
   {
   witAddPart            (theWitRun, "A", WitCAPACITY);
   witAddPart            (theWitRun, "B", WitMATERIAL);
   witAddOperation       (theWitRun, "C");
   witAddBomEntry        (theWitRun, "C", "A");
   witAddSubsBomEntry    (theWitRun, "C",  0,  "A");
   witAddBopEntry        (theWitRun, "C", "B");
   witAddDemand          (theWitRun, "B", "D");

   witSetPartSupplyVol   (theWitRun, "A",      fltVecOf2 (2, 0));
   witSetDemandDemandVol (theWitRun, "B", "D", fltVecOf2 (0, 1));
   }

/*----------------------------------------------------------------------------*/
/* Function testSetObjVecs                                                    */
/*                                                                            */
/* Tests functions for setting objective function vectors in multiple         */
/* objectives mode.                                                           */
/*----------------------------------------------------------------------------*/

void testSetObjVecs (WitRun * theWitRun)
   {
   witSetCurrentObjective    (theWitRun, "LOW");

   witSetPartScrapCost       (theWitRun, "A",       fltVecOf2 ( 1,  2));
   witSetPartStockCost       (theWitRun, "B",       fltVecOf2 ( 5,  6));
   witSetOperationExecCost   (theWitRun, "C",       fltVecOf2 ( 9, 10));
   witSetSubsBomEntrySubCost (theWitRun, "C", 0, 0, fltVecOf2 (13, 14));
   witSetDemandShipReward    (theWitRun, "B", "D",  fltVecOf2 (17, 18));
   witSetDemandCumShipReward (theWitRun, "B", "D",  fltVecOf2 (21, 22));

   witSetCurrentObjective    (theWitRun, "HIGH");

   witSetPartScrapCost       (theWitRun, "A",       fltVecOf2 ( 3,  4));
   witSetPartStockCost       (theWitRun, "B",       fltVecOf2 ( 7,  8));
   witSetOperationExecCost   (theWitRun, "C",       fltVecOf2 (11, 12));
   witSetSubsBomEntrySubCost (theWitRun, "C", 0, 0, fltVecOf2 (15, 16));
   witSetDemandShipReward    (theWitRun, "B", "D",  fltVecOf2 (19, 20));
   witSetDemandCumShipReward (theWitRun, "B", "D",  fltVecOf2 (23, 24));
   }

/*----------------------------------------------------------------------------*/
/* Function testGetObjVecs                                                    */
/*                                                                            */
/* Tests functions for retrieving objective function vectors in               */
/* multiple objectives mode.                                                  */
/*----------------------------------------------------------------------------*/

void testGetObjVecs (WitRun * theWitRun)
   {
   float   goodVec[2];
   float * outVec;

   witSetCurrentObjective    (theWitRun, "LOW");

   witGetPartScrapCost       (theWitRun, "A",       & outVec);
   compareFltVecsOf2                                 (outVec,  1,  2);
   witFree                                           (outVec);

   witGetPartStockCost       (theWitRun, "B",       & outVec);
   compareFltVecsOf2                                 (outVec,  5,  6);
   witFree                                           (outVec);

   witGetOperationExecCost   (theWitRun, "C",       & outVec);
   compareFltVecsOf2                                 (outVec,  9, 10);
   witFree                                           (outVec);

   witGetSubsBomEntrySubCost (theWitRun, "C", 0, 0, & outVec);
   compareFltVecsOf2                                 (outVec, 13, 14);
   witFree                                           (outVec);

   witGetDemandShipReward    (theWitRun, "B", "D",  & outVec);
   compareFltVecsOf2                                 (outVec, 17, 18);
   witFree                                           (outVec);

   witGetDemandCumShipReward (theWitRun, "B", "D",  & outVec);
   compareFltVecsOf2                                 (outVec, 21, 22);
   witFree                                           (outVec);


   witSetCurrentObjective    (theWitRun, "HIGH");

   witGetPartScrapCost       (theWitRun, "A",       & outVec);
   compareFltVecsOf2                                 (outVec,  3,  4);
   witFree                                           (outVec);

   witGetPartStockCost       (theWitRun, "B",       & outVec);
   compareFltVecsOf2                                 (outVec,  7,  8);
   witFree                                           (outVec);

   witGetOperationExecCost   (theWitRun, "C",       & outVec);
   compareFltVecsOf2                                 (outVec, 11, 12);
   witFree                                           (outVec);

   witGetSubsBomEntrySubCost (theWitRun, "C", 0, 0, & outVec);
   compareFltVecsOf2                                 (outVec, 15, 16);
   witFree                                           (outVec);

   witGetDemandShipReward    (theWitRun, "B", "D",  & outVec);
   compareFltVecsOf2                                 (outVec, 19, 20);
   witFree                                           (outVec);

   witGetDemandCumShipReward (theWitRun, "B", "D",  & outVec);
   compareFltVecsOf2                                 (outVec, 23, 24);
   witFree                                           (outVec);
   }

/*----------------------------------------------------------------------------*/
/* testMultiObjValue                                                          */
/*                                                                            */
/* Tests witGetObjValue in multiple objectives mode.                          */
/*----------------------------------------------------------------------------*/

void testMultiObjValue (WitRun * theWitRun)
   {
   float theFloat;

   witSetCurrentObjective (theWitRun, "LOW");

   witGetObjValue         (theWitRun, & theFloat);
   
   assert (fabs (theFloat - 25.0) < dblEps);


   witSetCurrentObjective (theWitRun, "HIGH");

   witGetObjValue         (theWitRun, & theFloat);
   
   assert (fabs (theFloat - 23.0) < dblEps);
   }

/*----------------------------------------------------------------------------*/
/* testLeadTimeBounds ()                                                      */
/*                                                                            */
/* Tests lead time bounds and selection recovery.                             */
/*----------------------------------------------------------------------------*/

void testLeadTimeBounds ()
   {
   WitRun *   theWitRun;
   witBoolean theBool;
   float      supplyVol[] = {100,  0,  0};
   float      demandVol[] = {  0,  0, 10};
   int        the1Vec  [] = {  1,  1,  1};
   int        the2Vec  [] = {  2,  2,  2};
   int *      theIntVec;

   witNewRun                (& theWitRun);

   witInitialize              (theWitRun);

   witSetNPeriods             (theWitRun,               3);
   witSetSelectionRecovery    (theWitRun,               WitTRUE);
   witGetSelectionRecovery    (theWitRun,             & theBool);
   assert                                              (theBool);
   witSetSkipFailures         (theWitRun,               WitFALSE);

   witAddPart                 (theWitRun,   "A",        WitCAPACITY);
   witAddPart                 (theWitRun,   "B",        WitMATERIAL);
   witAddOperation            (theWitRun,   "C");
   witAddBomEntry             (theWitRun,   "C", "A");
   witAddBopEntry             (theWitRun,   "C", "B");
   witAddDemand               (theWitRun,   "B", "D");
   witAddDemand               (theWitRun,   "B", "E");

   witSetPartSupplyVol        (theWitRun,   "A",        supplyVol);
   witSetPartBuildNstn        (theWitRun,   "B",        WitTRUE);
   witSetPartBoundedLeadTimes (theWitRun,   "B",        WitTRUE);
   witGetPartBoundedLeadTimes (theWitRun,   "B",      & theBool);
   assert                                              (theBool);

   witSetDemandDemandVol      (theWitRun,   "B", "D",   demandVol);
   witSetDemandDemandVol      (theWitRun,   "B", "E",   demandVol);

   witSetDemandLeadTimeUB     (theWitRun,   "B", "D",   the2Vec);
   witSetDemandLeadTimeUB     (theWitRun,   "B", "E",   the1Vec);
   witGetDemandLeadTimeUB     (theWitRun,   "B", "E", & theIntVec);
   assert                                              (theIntVec[2] == 1);

   witSetDemandPriority       (theWitRun,   "B", "D",   the1Vec);
   witSetDemandPriority       (theWitRun,   "B", "E",   the2Vec);

   witHeurImplode             (theWitRun);

   witWriteShipSched          (theWitRun,               WitSTDOUT, WitBSV);

   witDeleteRun               (theWitRun);
   }

/*----------------------------------------------------------------------------*/
/* compareFltVecsOf2                                                          */
/*                                                                            */
/* Asserts that theFltVec2 == (theFlt0, theFlt1)                              */
/*----------------------------------------------------------------------------*/

void compareFltVecsOf2 (float * theFltVec, float theFlt0, float theFlt1)
   {
   compareFltVecs (
      theFltVec,
      fltVecOf2 (theFlt0, theFlt1),
      2);
   }

/*----------------------------------------------------------------------------*/
/* Function fltVecOf1 (theFlt)                                                */
/*                                                                            */
/* Populates a statically declared vector of length 1 with the argument and   */
/* returns it.                                                                */
/*----------------------------------------------------------------------------*/

const float * fltVecOf1 (float theFlt)
   {
   static float theFltVec[1];

   theFltVec[0] = theFlt;

   return theFltVec;
   }

/*----------------------------------------------------------------------------*/
/* Function fltVecOf2 (theFlt0, theFlt1)                                      */
/*                                                                            */
/* Populates a statically declared vector of length 2 with the arguments and  */
/* returns it.                                                                */
/*----------------------------------------------------------------------------*/

const float * fltVecOf2 (float theFlt0, float theFlt1)
   {
   static float theFltVec[2];

   theFltVec[0] = theFlt0;
   theFltVec[1] = theFlt1;

   return theFltVec;
   }

/*----------------------------------------------------------------------------*/
/* Function equalStrings.                                                     */
/*                                                                            */
/* Returns true iff str1 == str2 as strings.                                  */
/*----------------------------------------------------------------------------*/

witBoolean equalStrings (const char * str1, const char * str2)
   {
   return (strcmp (str1, str2) == 0);
   }

/*----------------------------------------------------------------------------*/
/* Function compareIntVecs.                                                   */
/*                                                                            */
/* Asserts that theIntVec1 == theIntVec2.                                     */
/*----------------------------------------------------------------------------*/

void compareIntVecs (const int * theIntVec1, const int * theIntVec2, int length)
   {
   int theIdx;

   for (theIdx = 0; theIdx < length; theIdx ++) 
      assert (theIntVec1[theIdx] == theIntVec2[theIdx]);
   }

/*----------------------------------------------------------------------------*/
/* Function compareFltVecs.                                                   */
/*                                                                            */
/* Asserts that theFltVec1 == theFltVec2.                                     */
/*----------------------------------------------------------------------------*/

void compareFltVecs (
      const float * theFltVec1,
      const float * theFltVec2,
      int           length)
   {
   int theIdx;

   for (theIdx = 0; theIdx < length; theIdx ++) 
      assert (fabs (theFltVec1[theIdx] - theFltVec2[theIdx]) < dblEps);
   }

/*----------------------------------------------------------------------------*/
/* Function compareDblVecs.                                                   */
/*                                                                            */
/* Asserts that theDblVec1 == theDblVec2.                                     */
/*----------------------------------------------------------------------------*/

void compareDblVecs (
      const double * theDblVec1,
      const double * theDblVec2,
      int            length)
   {
   int theIdx;

   for (theIdx = 0; theIdx < nPeriods; theIdx ++) 
      assert (fabs (theDblVec1[theIdx] - theDblVec2[theIdx]) < dblEps);
   }

/*----------------------------------------------------------------------------*/
/* Function printFltVec.                                                      */
/*----------------------------------------------------------------------------*/

void printFltVec (const char * attrName, const float * theFltVec, int length)
   {
   int theIdx;

   printf ("%s = ", attrName);

   for (theIdx = 0; theIdx < length; theIdx ++)
      printf (" %f", theFltVec[theIdx]);

   printf ("\n");
   }

/*----------------------------------------------------------------------------*/
/* Function printDblVec.                                                      */
/*----------------------------------------------------------------------------*/

void printDblVec (const char * attrName, const double * theDblVec, int length)
   {
   int theIdx;

   printf ("%s (double) = ", attrName);

   for (theIdx = 0; theIdx < length; theIdx ++)
      printf (" %f", theDblVec[theIdx]);

   printf ("\n");
   }
