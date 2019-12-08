//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "api.C"
//
// Contains the implementations of all of outer API functions (which are 
// C-callable global functions). These functions call the inner API functions
// (which are C++ WitRun member functions).
//
// Contains the implementation of class ErrorExc.
//------------------------------------------------------------------------------

#include <WitRun.h>
#include <ApiMgr.h>
#include <ISRealArg.h>
#include <OSRealArg.h>
#include <IVRealArg.h>
#include <OVRealArg.h>
#include <OTDARealArg.h>
#include <wit.h>

#include <stdlib.h>

//------------------------------------------------------------------------------
// Macro STANDARD_API_IMPL (theWitRun, innerFunc, argList)
//
// "Standard API Implementation"
// This macro expands to the standard implementation of an outer API function.
//
// Arguments:
//    theWitRun    The WitRun for which the inner API function is to be called.
//    innerFunc    The name of the inner API function to be called.
//                 It must also be the name of the outer API function within
//                 which this macro is being called.
//    argList      The list of arguments (in parentheses) to be passed to the
//                 inner API function.
//
// For most outer API functions, the implementation of the function consists
// entirely of a call to this macro or the macro STANDARD_DBL_API_IMPL.
// All other outer API functions are preceded by the following comment:
//
// Non-Standard API Function.
//------------------------------------------------------------------------------

#define STANDARD_API_IMPL(theWitRun, innerFunc, argList)                       \
   {                                                                           \
   try                                                                         \
      {                                                                        \
      WitApiMgr::startApiFunc (theWitRun, #innerFunc);                         \
                                                                               \
      theWitRun->innerFunc argList;                                            \
                                                                               \
      theWitRun->myApiMgr ()->finishApiFunc ();                                \
      }                                                                        \
                                                                               \
   catch (...)                                                                 \
      {                                                                        \
      theWitRun->myApiMgr ()->handleAnyExc ();                                 \
      }                                                                        \
                                                                               \
   return theWitRun->myApiMgr ()->apiRc ();                                    \
   }                                                                           \

//------------------------------------------------------------------------------
// Macro STANDARD_DBL_API_IMPL (theWitRun, innerFunc, argList)
//
// "Standard Double API Implementation"
//
// Same as STANDARD_API_IMPL, but appends "Dbl" to the end of the name of the
// inner API function to construct the name of the outer API function.
//------------------------------------------------------------------------------

#define STANDARD_DBL_API_IMPL(theWitRun, innerFunc, argList)                   \
   {                                                                           \
   try                                                                         \
      {                                                                        \
      WitApiMgr::startApiFunc (theWitRun, #innerFunc "Dbl");                   \
                                                                               \
      theWitRun->innerFunc argList;                                            \
                                                                               \
      theWitRun->myApiMgr ()->finishApiFunc ();                                \
      }                                                                        \
                                                                               \
   catch (...)                                                                 \
      {                                                                        \
      theWitRun->myApiMgr ()->handleAnyExc ();                                 \
      }                                                                        \
                                                                               \
   return theWitRun->myApiMgr ()->apiRc ();                                    \
   }                                                                           \

//------------------------------------------------------------------------------

extern "C"
   {

//------------------------------------------------------------------------------
// WIT API functions for the current release.
//------------------------------------------------------------------------------

witReturnCode witAddBomEntry (
      WitRun * const theWitRun,
      const char * const consumingOperationName,
      const char * const consumedPartName)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witAddBomEntry, (
         consumingOperationName,
         consumedPartName))
   }

//------------------------------------------------------------------------------

witReturnCode witAddBopEntry (
      WitRun * const theWitRun,
      const char * const producingOperationName,
      const char * const producedPartName)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witAddBopEntry, (
         producingOperationName,
         producedPartName))
   }

//------------------------------------------------------------------------------

witReturnCode witAddDblCplexParSpec (
      WitRun * const     theWitRun,
      const char * const theName,
      const float        theValue)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witAddDblCplexParSpec, (
         theName,
         theValue))
   }

//------------------------------------------------------------------------------

witReturnCode witAddDblCplexParSpecDbl (
      WitRun * const     theWitRun,
      const char * const theName,
      const double       theValue)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witAddDblCplexParSpec, (
         theName,
         theValue))
   }

//------------------------------------------------------------------------------

witReturnCode witAddDemand (
      WitRun * const theWitRun,
      const char * const demandedPartName,
      const char * const demandName)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witAddDemand, (
         demandedPartName,
         demandName))
   }
 
//------------------------------------------------------------------------------

witReturnCode witAddIntCplexParSpec (
      WitRun * const     theWitRun,
      const char * const theName,
      const int          theValue)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witAddIntCplexParSpec, (
         theName,
         theValue))
   }

//------------------------------------------------------------------------------

witReturnCode witAddOperation (
      WitRun * const theWitRun,
      const char * const operationName)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witAddOperation, (
         operationName))
   }

//------------------------------------------------------------------------------

witReturnCode witAddPart (
      WitRun * const theWitRun,
      const char * const partName,
      const witAttr partCategory)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witAddPart, (
         partName,
         partCategory))
   }
 
//------------------------------------------------------------------------------

witReturnCode witAddPartWithOperation (
      WitRun * const theWitRun,
      const char * const partAndOperationName)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witAddPartWithOperation, (
         partAndOperationName))
   }

//------------------------------------------------------------------------------

witReturnCode witAddSubsBomEntry (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const char * const consumedPartName)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witAddSubsBomEntry, (
         operationName,
         bomEntryIndex,
         consumedPartName))
   }
 
//------------------------------------------------------------------------------

witReturnCode witAdvanceObjItr (
      WitRun * const theWitRun)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witAdvanceObjItr, ())
   }

//------------------------------------------------------------------------------

witReturnCode witAppendToPipSeq (
      WitRun * const     theWitRun,
      const char * const partName,
      const char * const demandName,
      int                shipPeriod,
      float              incShipVol)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witAppendToPipSeq, (
         partName,
         demandName,
         shipPeriod,
         incShipVol))
   }

//------------------------------------------------------------------------------

witReturnCode witAppendToPipSeqDbl (
      WitRun * const     theWitRun,
      const char * const partName,
      const char * const demandName,
      int                shipPeriod,
      double             incShipVol)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witAppendToPipSeq, (
         partName,
         demandName,
         shipPeriod,
         incShipVol))
   }

//------------------------------------------------------------------------------

witReturnCode witBuildPip (
      WitRun * const theWitRun)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witBuildPip, ())
   }

//------------------------------------------------------------------------------

witReturnCode witClearCplexParSpecs (
      WitRun * const theWitRun)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witClearCplexParSpecs, ())
   }

//------------------------------------------------------------------------------

witReturnCode witClearPegging (
      WitRun * const theWitRun)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witClearPegging, ())
   }

//------------------------------------------------------------------------------

witReturnCode witClearPipSeq (
      WitRun * const theWitRun)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witClearPipSeq, ())
   }

//------------------------------------------------------------------------------

witReturnCode witClearStochSoln (
      WitRun * const theWitRun)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witClearStochSoln, ())
   }

//------------------------------------------------------------------------------

witReturnCode witCopyBomEntryData (
      WitRun * const     dupWitRun,
      const char * const dupOperationName,
      const int          dupBomEntryIndex,
      WitRun * const     origWitRun,
      const char * const origOperationName,
      const int          origBomEntryIndex)
   {
   STANDARD_API_IMPL (
      dupWitRun,
      witCopyBomEntryData, (
         dupOperationName,
         dupBomEntryIndex,
         origWitRun,
         origOperationName,
         origBomEntryIndex))
   }

//------------------------------------------------------------------------------

witReturnCode witCopyBopEntryData (
      WitRun * const     dupWitRun,
      const char * const dupOperationName,
      const int          dupBopEntryIndex,
      WitRun * const     origWitRun,
      const char * const origOperationName,
      const int          origBopEntryIndex)
   {
   STANDARD_API_IMPL (
      dupWitRun,
      witCopyBopEntryData, (
         dupOperationName,
         dupBopEntryIndex,
         origWitRun,
         origOperationName,
         origBopEntryIndex))
   }

//------------------------------------------------------------------------------

witReturnCode witCopyData (
      WitRun * const dupWitRun,
      WitRun * const origWitRun)
   {
   STANDARD_API_IMPL (
      dupWitRun,
      witCopyData, (
         origWitRun))
   }

//------------------------------------------------------------------------------

witReturnCode witCopyDemandData (
      WitRun * const     dupWitRun,
      const char * const dupPartName,
      const char * const dupDemandName,
      WitRun * const     origWitRun,
      const char * const origPartName,
      const char * const origDemandName)
   {
   STANDARD_API_IMPL (
      dupWitRun,
      witCopyDemandData, (
         dupPartName,
         dupDemandName,
         origWitRun,
         origPartName,
         origDemandName))
   }

//------------------------------------------------------------------------------

witReturnCode witCopyOperationData (
      WitRun * const     dupWitRun,
      const char * const dupOperationName,
      WitRun * const     origWitRun,
      const char * const origOperationName)
   {
   STANDARD_API_IMPL (
      dupWitRun,
      witCopyOperationData, (
         dupOperationName,
         origWitRun,
         origOperationName))
   }

//------------------------------------------------------------------------------

witReturnCode witCopyPartData (
      WitRun * const     dupWitRun,
      const char * const dupPartName,
      WitRun * const     origWitRun,
      const char * const origPartName)
   {
   STANDARD_API_IMPL (
      dupWitRun,
      witCopyPartData, (
         dupPartName,
         origWitRun,
         origPartName))
   }

//------------------------------------------------------------------------------

witReturnCode witCopySubsBomEntryData (
      WitRun * const     dupWitRun,
      const char * const dupOperationName,
      const int          dupBomEntryIndex,
      const int          dupSubsBomEntryIndex,
      WitRun * const     origWitRun,
      const char * const origOperationName,
      const int          origBomEntryIndex,
      const int          origSubsBomEntryIndex)
   {
   STANDARD_API_IMPL (
      dupWitRun,
      witCopySubsBomEntryData, (
         dupOperationName,
         dupBomEntryIndex,
         dupSubsBomEntryIndex,
         origWitRun,
         origOperationName,
         origBomEntryIndex,
         origSubsBomEntryIndex))
   }

//------------------------------------------------------------------------------

witReturnCode witDeleteRunInner (WitRun *);

//------------------------------------------------------------------------------
// Non-Standard API Function.
//------------------------------------------------------------------------------

witReturnCode witDeleteRun (
      WitRun * theWitRun)
   {
   witReturnCode theRC;

   if (theWitRun == NULL)
      return WitINFORMATIONAL_RC;

   theRC = witDeleteRunInner (theWitRun);

   if (theRC > WitWARNING_RC)
      return theRC;

   theWitRun->myApiMgr ()->lockMyWitRun ("witDeleteRun");

   delete theWitRun;

   return theRC;
   }

//------------------------------------------------------------------------------
// witDeleteRunInner
//
// Inner function for witDeleteRun.
// Not an actual API function.
//------------------------------------------------------------------------------

witReturnCode witDeleteRunInner (WitRun * theWitRun)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witDeleteRun, ())
   }

//------------------------------------------------------------------------------

witReturnCode witDisplayData (
      WitRun * const theWitRun,
      const char * const fileName)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witDisplayData, (
         fileName))
   }

//------------------------------------------------------------------------------

witReturnCode witEqHeurAlloc (
      WitRun * const theWitRun,
      const int lenLists,
      const char * const * const demandedPartNameList,
      const char * const * const demandNameList,
      const int * const shipPeriodList,
      const float * const desIncVolList,
      float * * incVolList)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witEqHeurAlloc, (
         lenLists,
         demandedPartNameList,
         demandNameList,
         shipPeriodList,
         desIncVolList,
         incVolList))
   }

//------------------------------------------------------------------------------

witReturnCode witEqHeurAllocDbl (
      WitRun * const theWitRun,
      const int lenLists,
      const char * const * const demandedPartNameList,
      const char * const * const demandNameList,
      const int * const shipPeriodList,
      const double * const desIncVolList,
      double * * incVolList)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witEqHeurAlloc, (
         lenLists,
         demandedPartNameList,
         demandNameList,
         shipPeriodList,
         desIncVolList,
         incVolList))
   }

//------------------------------------------------------------------------------

witReturnCode witEqHeurAllocTwme (
      WitRun * const theWitRun,
      const int lenLists,
      const char * const * const demandedPartNameList,
      const char * const * const demandNameList,
      const int * const shipPeriodList,
      const float * const desIncVolList,
      float * * incVolList,
      const witBoolean * const asapMultiExecList)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witEqHeurAllocTwme, (
         lenLists,
         demandedPartNameList,
         demandNameList,
         shipPeriodList,
         desIncVolList,
         incVolList,
         asapMultiExecList))
   }

//------------------------------------------------------------------------------

witReturnCode witEqHeurAllocTwmeDbl (
      WitRun * const theWitRun,
      const int lenLists,
      const char * const * const demandedPartNameList,
      const char * const * const demandNameList,
      const int * const shipPeriodList,
      const double * const desIncVolList,
      double * * incVolList,
      const witBoolean * const asapMultiExecList)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witEqHeurAllocTwme, (
         lenLists,
         demandedPartNameList,
         demandNameList,
         shipPeriodList,
         desIncVolList,
         incVolList,
         asapMultiExecList))
   }

//------------------------------------------------------------------------------

witReturnCode witEvalObjectives (
      WitRun * const theWitRun)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witEvalObjectives, ())
   }

//------------------------------------------------------------------------------

witReturnCode witFinishExtOpt (
      WitRun * const theWitRun)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witFinishExtOpt, ())
   }

//------------------------------------------------------------------------------

witReturnCode witFinishHeurAlloc (
      WitRun * const theWitRun)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witFinishHeurAlloc, ())
   }

//------------------------------------------------------------------------------
// Non-Standard API Function.
//------------------------------------------------------------------------------

witReturnCode witFree (
      void * mem)
   {
   free (mem);

   return WitINFORMATIONAL_RC;
   }

//------------------------------------------------------------------------------

witReturnCode witGeneratePriorities (
      WitRun * const theWitRun)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGeneratePriorities, ())
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetAccAfterSoftLB (
      WitRun * const theWitRun,
      witBoolean * accAfterSoftLB)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetAccAfterSoftLB, (
         accAfterSoftLB))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetAccAfterOptImp (
      WitRun * const theWitRun,
      witBoolean * accAfterOptImp)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetAccAfterOptImp, (
         accAfterOptImp))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetAccelerated (
      WitRun * const theWitRun,
      witBoolean * accelerated)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetAccelerated, (
         accelerated))
   }

//------------------------------------------------------------------------------

witReturnCode witGetAppData (
      WitRun * const theWitRun,
      void * * appData)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetAppData, (
         appData))
   }

//------------------------------------------------------------------------------

witReturnCode witGetAutoPriority (
      WitRun * const theWitRun,
      witBoolean * autoPriority)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetAutoPriority, (
         autoPriority))
   }

//------------------------------------------------------------------------------

witReturnCode witGetBomEntryAppData (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      void * * appData)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetBomEntryAppData, (
         operationName,
         bomEntryIndex,
         appData))
   }

//------------------------------------------------------------------------------

witReturnCode witGetBomEntryConsRate (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      float * * consRate)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetBomEntryConsRate, (
         operationName,
         bomEntryIndex,
         consRate))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetBomEntryConsRateDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      double * * consRate)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetBomEntryConsRate, (
         operationName,
         bomEntryIndex,
         consRate))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetBomEntryConsumedPart (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      char * * consumedPartName)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetBomEntryConsumedPart, (
         operationName,
         bomEntryIndex,
         consumedPartName))
   }

//------------------------------------------------------------------------------

witReturnCode witGetBomEntryEarliestPeriod (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      int * earliestPeriod)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetBomEntryEarliestPeriod, (
         operationName,
         bomEntryIndex,
         earliestPeriod))
   }

//------------------------------------------------------------------------------

witReturnCode witGetBomEntryExecPenalty (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      float * execPenalty)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetBomEntryExecPenalty, (
         operationName,
         bomEntryIndex,
         execPenalty))
   }

//------------------------------------------------------------------------------

witReturnCode witGetBomEntryExecPenaltyDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      double * execPenalty)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetBomEntryExecPenalty, (
         operationName,
         bomEntryIndex,
         execPenalty))
   }

//------------------------------------------------------------------------------

witReturnCode witGetBomEntryFalloutRate (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      float * falloutRate)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetBomEntryFalloutRate, (
         operationName,
         bomEntryIndex,
         falloutRate))
   }

//------------------------------------------------------------------------------

witReturnCode witGetBomEntryFalloutRateDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      double * falloutRate)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetBomEntryFalloutRate, (
         operationName,
         bomEntryIndex,
         falloutRate))
   }

//------------------------------------------------------------------------------

witReturnCode witGetBomEntryImpactPeriod (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      int * * impactPeriod)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetBomEntryImpactPeriod, (
         operationName,
         bomEntryIndex,
         impactPeriod))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetBomEntryLatestPeriod (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      int * latestPeriod)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetBomEntryLatestPeriod, (
         operationName,
         bomEntryIndex,
         latestPeriod))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetBomEntryMandEC (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      witBoolean * mandEC)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetBomEntryMandEC, (
         operationName,
         bomEntryIndex,
         mandEC))
   }

//------------------------------------------------------------------------------

witReturnCode witGetBomEntryNSubsBomEntries (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      int * nSubsBomEntries)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetBomEntryNSubsBomEntries, (
         operationName,
         bomEntryIndex,
         nSubsBomEntries))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetBomEntryNonSubVarIndex (
      WitRun * const     theWitRun,
      const char * const consumingOperationName,
      const int          bomEntryIndex,
      const int          thePer,
      int *              nonSubVarIndex)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetBomEntryNonSubVarIndex, (
         consumingOperationName,
         bomEntryIndex,
         thePer,
         nonSubVarIndex))
   }

//------------------------------------------------------------------------------

witReturnCode witGetBomEntryOffset (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      float * * offset)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetBomEntryOffset, (
         operationName,
         bomEntryIndex,
         offset))
   }

//------------------------------------------------------------------------------

witReturnCode witGetBomEntryOffsetDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      double * * offset)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetBomEntryOffset, (
         operationName,
         bomEntryIndex,
         offset))
   }

//------------------------------------------------------------------------------

witReturnCode witGetBomEntryPropRtg (
      WitRun * const     theWitRun,
      const char * const operationName,
      const int          bomEntryIndex,
      witBoolean * *     propRtg)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetBomEntryPropRtg, (
         operationName,
         bomEntryIndex,
         propRtg))
   }

//------------------------------------------------------------------------------

witReturnCode witGetBomEntryRoutingShare (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      float * * routingShare)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetBomEntryRoutingShare, (
         operationName,
         bomEntryIndex,
         routingShare))
   }

//------------------------------------------------------------------------------

witReturnCode witGetBomEntryRoutingShareDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      double * * routingShare)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetBomEntryRoutingShare, (
         operationName,
         bomEntryIndex,
         routingShare))
   }

//------------------------------------------------------------------------------

witReturnCode witGetBomEntrySelForDel (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      witBoolean * selForDel)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetBomEntrySelForDel, (
         operationName,
         bomEntryIndex,
         selForDel))
   }

//------------------------------------------------------------------------------

witReturnCode witGetBomEntrySingleSource (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      witBoolean * singleSource)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetBomEntrySingleSource, (
         operationName,
         bomEntryIndex,
         singleSource))
   }

//------------------------------------------------------------------------------

witReturnCode witGetBomEntrySubConIndex (
      WitRun * const     theWitRun,
      const char * const consumingOperationName,
      const int          bomEntryIndex,
      const int          thePer,
      int *              subConIndex)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetBomEntrySubConIndex, (
         consumingOperationName,
         bomEntryIndex,
         thePer,
         subConIndex))
   }

//------------------------------------------------------------------------------

witReturnCode witGetBopEntryAppData (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bopEntryIndex,
      void * * appData)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetBopEntryAppData, (
         operationName,
         bopEntryIndex,
         appData))
   }

//------------------------------------------------------------------------------

witReturnCode witGetBopEntryEarliestPeriod (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bopEntryIndex,
      int * earliestPeriod)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetBopEntryEarliestPeriod, (
         operationName,
         bopEntryIndex,
         earliestPeriod))
   }

//------------------------------------------------------------------------------

witReturnCode witGetBopEntryExpAllowed (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bopEntryIndex,
      witBoolean * expAllowed)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetBopEntryExpAllowed, (
         operationName,
         bopEntryIndex,
         expAllowed))
   }

//------------------------------------------------------------------------------

witReturnCode witGetBopEntryExpAversion (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bopEntryIndex,
      float * expAversion)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetBopEntryExpAversion, (
         operationName,
         bopEntryIndex,
         expAversion))
   }

//------------------------------------------------------------------------------

witReturnCode witGetBopEntryExpAversionDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bopEntryIndex,
      double * expAversion)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetBopEntryExpAversion, (
         operationName,
         bopEntryIndex,
         expAversion))
   }

//------------------------------------------------------------------------------

witReturnCode witGetBopEntryImpactPeriod (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bopEntryIndex,
      int * * impactPeriod)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetBopEntryImpactPeriod, (
         operationName,
         bopEntryIndex,
         impactPeriod))
   }

//------------------------------------------------------------------------------

witReturnCode witGetBopEntryLatestPeriod (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bopEntryIndex,
      int * latestPeriod)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetBopEntryLatestPeriod, (
         operationName,
         bopEntryIndex,
         latestPeriod))
   }

//------------------------------------------------------------------------------

witReturnCode witGetBopEntryOffset (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bopEntryIndex,
      float * * offset)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetBopEntryOffset, (
         operationName,
         bopEntryIndex,
         offset))
   }

//------------------------------------------------------------------------------

witReturnCode witGetBopEntryOffsetDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bopEntryIndex,
      double * * offset)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetBopEntryOffset, (
         operationName,
         bopEntryIndex,
         offset))
   }

//------------------------------------------------------------------------------

witReturnCode witGetBopEntryProductRate (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bopEntryIndex,
      float * * productRate)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetBopEntryProductRate, (
         operationName,
         bopEntryIndex,
         productRate))
   }

//------------------------------------------------------------------------------

witReturnCode witGetBopEntryProductRateDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bopEntryIndex,
      double * * productRate)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetBopEntryProductRate, (
         operationName,
         bopEntryIndex,
         productRate))
   }

//------------------------------------------------------------------------------

witReturnCode witGetBopEntryProducedPart (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bopEntryIndex,
      char * * producedPartName)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetBopEntryProducedPart, (
         operationName,
         bopEntryIndex,
         producedPartName))
   }

//------------------------------------------------------------------------------

witReturnCode witGetBopEntryRoutingShare (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bopEntryIndex,
      float * * routingShare)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetBopEntryRoutingShare, (
         operationName,
         bopEntryIndex,
         routingShare))
   }

//------------------------------------------------------------------------------

witReturnCode witGetBopEntryRoutingShareDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bopEntryIndex,
      double * * routingShare)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetBopEntryRoutingShare, (
         operationName,
         bopEntryIndex,
         routingShare))
   }

//------------------------------------------------------------------------------

witReturnCode witGetBopEntrySelForDel (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bopEntryIndex,
      witBoolean * selForDel)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetBopEntrySelForDel, (
         operationName,
         bopEntryIndex,
         selForDel))
   }

//------------------------------------------------------------------------------

witReturnCode witGetBoundsValue (
      WitRun * const theWitRun,
      float *        boundsValue)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetBoundsValue, (
         boundsValue))
   }

//------------------------------------------------------------------------------

witReturnCode witGetBoundsValueDbl (
      WitRun * const theWitRun,
      double *       boundsValue)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetBoundsValue, (
         boundsValue))
   }

//------------------------------------------------------------------------------

witReturnCode witGetCoinEmbedded (
      WitRun * const theWitRun,
      witBoolean *   coinEmbedded)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetCoinEmbedded, (
         coinEmbedded))
   }

//------------------------------------------------------------------------------

witReturnCode witGetCompPrices (
      WitRun * const theWitRun,
      witBoolean * compPrices)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetCompPrices, (
         compPrices))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetComputeCriticalList (
      WitRun * const theWitRun,
      witBoolean * computeCriticalList)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetComputeCriticalList, (
         computeCriticalList))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetCplexEmbedded (
      WitRun * const theWitRun,
      witBoolean *   cplexEmbedded)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetCplexEmbedded, (
         cplexEmbedded))
   }

//------------------------------------------------------------------------------

witReturnCode witGetCplexMipBound (
      WitRun * const theWitRun,
      float * cplexMipBound)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetCplexMipBound, (
         cplexMipBound))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetCplexMipBoundDbl (
      WitRun * const theWitRun,
      double * cplexMipBound)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetCplexMipBound, (
         cplexMipBound))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetCplexMipRelGap (
      WitRun * const theWitRun,
      float * cplexMipRelGap)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetCplexMipRelGap, (
         cplexMipRelGap))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetCplexMipRelGapDbl (
      WitRun * const theWitRun,
      double * cplexMipRelGap)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetCplexMipRelGap, (
         cplexMipRelGap))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetCplexParSpecDblVal (
      WitRun * const theWitRun,
      float * cplexParSpecDblVal)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetCplexParSpecDblVal, (
         cplexParSpecDblVal))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetCplexParSpecDblValDbl (
      WitRun * const theWitRun,
      double * cplexParSpecDblVal)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetCplexParSpecDblVal, (
         cplexParSpecDblVal))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetCplexParSpecIntVal (
      WitRun * const theWitRun,
      int * cplexParSpecIntVal)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetCplexParSpecIntVal, (
         cplexParSpecIntVal))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetCplexParSpecName (
      WitRun * const theWitRun,
      char * * cplexParSpecName)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetCplexParSpecName, (
         cplexParSpecName))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetCplexStatusCode (
      WitRun * const theWitRun,
      int *          cplexStatusCode)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetCplexStatusCode, (
         cplexStatusCode))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetCplexStatusText (
      WitRun * const theWitRun,
      char * *       cplexStatusText)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetCplexStatusText, (
         cplexStatusText))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetCriticalList (
      WitRun * const theWitRun,
      int * lenCritList,
      char * * * critPartList,
      int * * critPeriod)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetCriticalList, (
         lenCritList,
         critPartList,
         critPeriod))
   }
  
//------------------------------------------------------------------------------

witReturnCode witGetCurrentObjective (
      WitRun * const theWitRun,
      char * * theObjName)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetCurrentObjective, (
         theObjName))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetCurrentScenario (
      WitRun * const theWitRun,
      int * currentScenario)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetCurrentScenario, (
         currentScenario))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetDblCplexParSpec (
      WitRun * const     theWitRun,
      const char * const theName,
      witBoolean *       dblSpecExists,
      float *            theValue)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetDblCplexParSpec, (
         theName,
         dblSpecExists,
         theValue))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDblCplexParSpecDbl (
      WitRun * const     theWitRun,
      const char * const theName,
      witBoolean *       dblSpecExists,
      double *           theValue)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetDblCplexParSpec, (
         theName,
         dblSpecExists,
         theValue))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandAppData (
      WitRun * const theWitRun,
      const char * const demandedPartName,
      const char * const demandName,
      void * * appData)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetDemandAppData, (
         demandedPartName,
         demandName,
         appData))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandCoExecVolPip (
      WitRun * const     theWitRun,
      const char * const partName,
      const char * const demandName,
      const int          shipPeriod,
      int *              lenLists,
      char * * *         operationNameList,
      int * *            bopEntryIndexList,
      int * *            execPeriodList,
      float * *          peggedVolList)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetDemandCoExecVolPip, (
         partName,
         demandName,
         shipPeriod,
         lenLists,
         operationNameList,
         bopEntryIndexList,
         execPeriodList,
         peggedVolList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandCoExecVolPipDbl (
      WitRun * const     theWitRun,
      const char * const partName,
      const char * const demandName,
      const int          shipPeriod,
      int *              lenLists,
      char * * *         operationNameList,
      int * *            bopEntryIndexList,
      int * *            execPeriodList,
      double * *         peggedVolList)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetDemandCoExecVolPip, (
         partName,
         demandName,
         shipPeriod,
         lenLists,
         operationNameList,
         bopEntryIndexList,
         execPeriodList,
         peggedVolList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandConsVolPip (
      WitRun * const     theWitRun,
      const char * const partName,
      const char * const demandName,
      const int          shipPeriod,
      int *              lenLists,
      char * * *         partNameList,
      int * *            periodList,
      float * *          peggedVolList)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetDemandConsVolPip, (
         partName,
         demandName,
         shipPeriod,
         lenLists,
         partNameList,
         periodList,
         peggedVolList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandConsVolPipDbl (
      WitRun * const     theWitRun,
      const char * const partName,
      const char * const demandName,
      const int          shipPeriod,
      int *              lenLists,
      char * * *         partNameList,
      int * *            periodList,
      double * *         peggedVolList)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetDemandConsVolPip, (
         partName,
         demandName,
         shipPeriod,
         lenLists,
         partNameList,
         periodList,
         peggedVolList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandCumShipBounds (
      WitRun * const theWitRun,
      const char * const demandedPartName,
      const char * const demandName,
      float * * hardLower,
      float * * softLower,
      float * * hardUpper)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetDemandCumShipBounds, (
         demandedPartName,
         demandName,
         hardLower,
         softLower,
         hardUpper))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandCumShipBoundsDbl (
      WitRun * const theWitRun,
      const char * const demandedPartName,
      const char * const demandName,
      double * * hardLower,
      double * * softLower,
      double * * hardUpper)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetDemandCumShipBounds, (
         demandedPartName,
         demandName,
         hardLower,
         softLower,
         hardUpper))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandCumShipReward (
      WitRun * const theWitRun,
      const char * const demandedPartName,
      const char * const demandName,
      float * * cumShipReward)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetDemandCumShipReward, (
         demandedPartName,
         demandName,
         cumShipReward))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetDemandCumShipRewardDbl (
      WitRun * const theWitRun,
      const char * const demandedPartName,
      const char * const demandName,
      double * * cumShipReward)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetDemandCumShipReward, (
         demandedPartName,
         demandName,
         cumShipReward))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetDemandCumShipSlbConIndex (
      WitRun * const     theWitRun,
      const char * const partName,
      const char * const demandName,
      const int          thePer,
      int *              cumShipSlbConIndex)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetDemandCumShipSlbConIndex, (
         partName,
         demandName,
         thePer,
         cumShipSlbConIndex))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandCumShipSlbvVarIndex (
      WitRun * const     theWitRun,
      const char * const partName,
      const char * const demandName,
      const int          thePer,
      int *              cumShipSlbvVarIndex)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetDemandCumShipSlbvVarIndex, (
         partName,
         demandName,
         thePer,
         cumShipSlbvVarIndex))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandCumShipVarIndex (
      WitRun * const     theWitRun,
      const char * const partName,
      const char * const demandName,
      const int          thePer,
      int *              cumShipVarIndex)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetDemandCumShipVarIndex, (
         partName,
         demandName,
         thePer,
         cumShipVarIndex))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandDemandVol (
      WitRun * const theWitRun,
      const char * const demandedPartName,
      const char * const demandName,
      float * * demandVol)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetDemandDemandVol, (
         demandedPartName,
         demandName,
         demandVol))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetDemandDemandVolDbl (
      WitRun * const theWitRun,
      const char * const demandedPartName,
      const char * const demandName,
      double * * demandVol)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetDemandDemandVol, (
         demandedPartName,
         demandName,
         demandVol))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetDemandExecVolPegging (
      WitRun * const theWitRun,
      const char * const demandedPartName,
      const char * const demandName,
      const int shipPeriod,
      int * lenLists,
      char * * * operationNameList,
      int * * execPeriodList,
      float * * peggedExecVolList)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetDemandExecVolPegging, (
         demandedPartName,
         demandName,
         shipPeriod,
         lenLists,
         operationNameList,
         execPeriodList,
         peggedExecVolList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandExecVolPeggingDbl (
      WitRun * const theWitRun,
      const char * const demandedPartName,
      const char * const demandName,
      const int shipPeriod,
      int * lenLists,
      char * * * operationNameList,
      int * * execPeriodList,
      double * * peggedExecVolList)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetDemandExecVolPegging, (
         demandedPartName,
         demandName,
         shipPeriod,
         lenLists,
         operationNameList,
         execPeriodList,
         peggedExecVolList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandExecVolPip (
      WitRun * const     theWitRun,
      const char * const partName,
      const char * const demandName,
      const int          shipPeriod,
      int *              lenLists,
      char * * *         operationNameList,
      int * *            execPeriodList,
      float * *          peggedExecVolList)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetDemandExecVolPip, (
         partName,
         demandName,
         shipPeriod,
         lenLists,
         operationNameList,
         execPeriodList,
         peggedExecVolList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandExecVolPipDbl (
      WitRun * const     theWitRun,
      const char * const partName,
      const char * const demandName,
      const int          shipPeriod,
      int *              lenLists,
      char * * *         operationNameList,
      int * *            execPeriodList,
      double * *         peggedExecVolList)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetDemandExecVolPip, (
         partName,
         demandName,
         shipPeriod,
         lenLists,
         operationNameList,
         execPeriodList,
         peggedExecVolList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandExists (
      WitRun * const     theWitRun,
      const char * const partName,
      const char * const demandName,
      int *              exists)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetDemandExists, (
         partName,
         demandName,
         exists))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandFocusHorizon (
      WitRun * const theWitRun,
      const char * const demandedPartName,
      const char * const demandName,
      int * focusHorizon)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetDemandFocusHorizon, (
         demandedPartName,
         demandName,
         focusHorizon))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandFssShipVol (
      WitRun * const theWitRun,
      const char * const demandedPartName,
      const char * const demandName,
      float * * fssShipVol)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetDemandFssShipVol, (
         demandedPartName,
         demandName,
         fssShipVol))
   }
 
//------------------------------------------------------------------------------


witReturnCode witGetDemandFssShipVolDbl (
      WitRun * const theWitRun,
      const char * const demandedPartName,
      const char * const demandName,
      double * * fssShipVol)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetDemandFssShipVol, (
         demandedPartName,
         demandName,
         fssShipVol))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetDemandIntShipVols (
      WitRun * const theWitRun,
      const char * const partName,
      const char * const demandName,
      int * intShipVols)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetDemandIntShipVols, (
         partName,
         demandName,
         intShipVols))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandLeadTimeUB (
      WitRun * const theWitRun,
      const char * const demandedPartName,
      const char * const demandName,
      int * * leadTimeUB)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetDemandLeadTimeUB, (
         demandedPartName,
         demandName,
         leadTimeUB))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandPriority (
      WitRun * const theWitRun,
      const char * const demandedPartName,
      const char * const demandName,
      int * * priority)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetDemandPriority, (
         demandedPartName,
         demandName,
         priority))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandProdVolPip (
      WitRun * const     theWitRun,
      const char * const partName,
      const char * const demandName,
      const int          shipPeriod,
      int *              lenLists,
      char * * *         partNameList,
      int * *            periodList,
      float * *          peggedVolList)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetDemandProdVolPip, (
         partName,
         demandName,
         shipPeriod,
         lenLists,
         partNameList,
         periodList,
         peggedVolList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandProdVolPipDbl (
      WitRun * const     theWitRun,
      const char * const partName,
      const char * const demandName,
      const int          shipPeriod,
      int *              lenLists,
      char * * *         partNameList,
      int * *            periodList,
      double * *         peggedVolList)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetDemandProdVolPip, (
         partName,
         demandName,
         shipPeriod,
         lenLists,
         partNameList,
         periodList,
         peggedVolList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandSearchInc (
      WitRun * const theWitRun,
      const char * const demandedPartName,
      const char * const demandName,
      float * searchInc)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetDemandSearchInc, (
         demandedPartName,
         demandName,
         searchInc))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandSearchIncDbl (
      WitRun * const theWitRun,
      const char * const demandedPartName,
      const char * const demandName,
      double * searchInc)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetDemandSearchInc, (
         demandedPartName,
         demandName,
         searchInc))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandSelForDel (
      WitRun * const theWitRun,
      const char * const demandedPartName,
      const char * const demandName,
      int * selForDel)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetDemandSelForDel, (
         demandedPartName,
         demandName,
         selForDel))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandShipConIndex (
      WitRun * const     theWitRun,
      const char * const partName,
      const char * const demandName,
      const int          thePer,
      int *              shipConIndex)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetDemandShipConIndex, (
         partName,
         demandName,
         thePer,
         shipConIndex))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandShipVarIndex (
      WitRun * const     theWitRun,
      const char * const partName,
      const char * const demandName,
      const int          thePer,
      int *              shipVarIndex)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetDemandShipVarIndex, (
         partName,
         demandName,
         thePer,
         shipVarIndex))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandShipLateAllowed (
      WitRun * const theWitRun,
      const char * const partName,
      const char * const demandName,
      int * shipLateAllowed)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetDemandShipLateAllowed, (
         partName,
         demandName,
         shipLateAllowed))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandShipLateUB (
      WitRun * const theWitRun,
      const char * const demandedPartName,
      const char * const demandName,
      int * * shipLateUB)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetDemandShipLateUB, (
         demandedPartName,
         demandName,
         shipLateUB))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandShipReward (
      WitRun * const theWitRun,
      const char * const demandedPartName,
      const char * const demandName,
      float * * shipReward)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetDemandShipReward, (
         demandedPartName,
         demandName,
         shipReward))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandShipRewardDbl (
      WitRun * const theWitRun,
      const char * const demandedPartName,
      const char * const demandName,
      double * * shipReward)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetDemandShipReward, (
         demandedPartName,
         demandName,
         shipReward))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandShipVol (
      WitRun * const theWitRun,
      const char * const demandedPartName,
      const char * const demandName,
      float * * shipVol)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetDemandShipVol, (
         demandedPartName,
         demandName,
         shipVol))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetDemandShipVolDbl (
      WitRun * const theWitRun,
      const char * const demandedPartName,
      const char * const demandName,
      double * * shipVol)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetDemandShipVol, (
         demandedPartName,
         demandName,
         shipVol))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetDemandSideVolPip (
      WitRun * const     theWitRun,
      const char * const partName,
      const char * const demandName,
      const int          shipPeriod,
      int *              lenLists,
      char * * *         partNameList,
      int * *            periodList,
      float * *          peggedVolList)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetDemandSideVolPip, (
         partName,
         demandName,
         shipPeriod,
         lenLists,
         partNameList,
         periodList,
         peggedVolList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandSideVolPipDbl (
      WitRun * const     theWitRun,
      const char * const partName,
      const char * const demandName,
      const int          shipPeriod,
      int *              lenLists,
      char * * *         partNameList,
      int * *            periodList,
      double * *         peggedVolList)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetDemandSideVolPip, (
         partName,
         demandName,
         shipPeriod,
         lenLists,
         partNameList,
         periodList,
         peggedVolList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandSubVolPegging (
      WitRun * const theWitRun,
      const char * const demandedPartName,
      const char * const demandName,
      const int shipPeriod,
      int * lenLists,
      char * * * operationNameList,
      int * * execPeriodList,
      int * * bomEntryIndexList,
      int * * subsBomEntryIndexList,
      float * * peggedSubVolList)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetDemandSubVolPegging, (
         demandedPartName,
         demandName,
         shipPeriod,
         lenLists,
         operationNameList,
         execPeriodList,
         bomEntryIndexList,
         subsBomEntryIndexList,
         peggedSubVolList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandSubVolPeggingDbl (
      WitRun * const theWitRun,
      const char * const demandedPartName,
      const char * const demandName,
      const int shipPeriod,
      int * lenLists,
      char * * * operationNameList,
      int * * execPeriodList,
      int * * bomEntryIndexList,
      int * * subsBomEntryIndexList,
      double * * peggedSubVolList)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetDemandSubVolPegging, (
         demandedPartName,
         demandName,
         shipPeriod,
         lenLists,
         operationNameList,
         execPeriodList,
         bomEntryIndexList,
         subsBomEntryIndexList,
         peggedSubVolList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandSubVolPip (
      WitRun * const     theWitRun,
      const char * const partName,
      const char * const demandName,
      const int          shipPeriod,
      int *              lenLists,
      char * * *         operationNameList,
      int * *            bomEntryIndexList,
      int * *            subsBomEntryIndexList,
      int * *            execPeriodList,
      float * *          peggedSubVolList)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetDemandSubVolPip, (
         partName,
         demandName,
         shipPeriod,
         lenLists,
         operationNameList,
         bomEntryIndexList,
         subsBomEntryIndexList,
         execPeriodList,
         peggedSubVolList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandSubVolPipDbl (
      WitRun * const     theWitRun,
      const char * const partName,
      const char * const demandName,
      const int          shipPeriod,
      int *              lenLists,
      char * * *         operationNameList,
      int * *            bomEntryIndexList,
      int * *            subsBomEntryIndexList,
      int * *            execPeriodList,
      double * *         peggedSubVolList)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetDemandSubVolPip, (
         partName,
         demandName,
         shipPeriod,
         lenLists,
         operationNameList,
         bomEntryIndexList,
         subsBomEntryIndexList,
         execPeriodList,
         peggedSubVolList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandSupplyVolPip (
      WitRun * const     theWitRun,
      const char * const partName,
      const char * const demandName,
      const int          shipPeriod,
      int *              lenLists,
      char * * *         partNameList,
      int * *            periodList,
      float * *          peggedVolList)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetDemandSupplyVolPip, (
         partName,
         demandName,
         shipPeriod,
         lenLists,
         partNameList,
         periodList,
         peggedVolList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandSupplyVolPipDbl (
      WitRun * const     theWitRun,
      const char * const partName,
      const char * const demandName,
      const int          shipPeriod,
      int *              lenLists,
      char * * *         partNameList,
      int * *            periodList,
      double * *         peggedVolList)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetDemandSupplyVolPip, (
         partName,
         demandName,
         shipPeriod,
         lenLists,
         partNameList,
         periodList,
         peggedVolList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetEquitability (
      WitRun * const theWitRun,
      int * equitablity)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetEquitability, (
         equitablity))
   }

//------------------------------------------------------------------------------

witReturnCode witGetExecEmptyBom (
      WitRun * const theWitRun,
      witBoolean * execEmptyBom)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetExecEmptyBom, (
         execEmptyBom))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetExpCutoff (
      WitRun * const theWitRun,
      float * expCutoff)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetExpCutoff, (
         expCutoff))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetExpCutoffDbl (
      WitRun * const theWitRun,
      double * expCutoff)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetExpCutoff, (
         expCutoff))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetExtOptActive (
      WitRun * const theWitRun,
      witBoolean *   extOptActive)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetExtOptActive, (
         extOptActive))
   }

//------------------------------------------------------------------------------

witReturnCode witGetExpCycle (
      WitRun * const theWitRun,
      int *          lenLists,
      char * * *     partNameList,
      char * * *     operationNameList)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetExpCycle, (
         lenLists,
         partNameList,
         operationNameList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetExtOptIntVarIndices (
      WitRun * const theWitRun,
      int * *        indices,
      int *          len)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetExtOptIntVarIndices, (
         indices,
         len))
   }

//------------------------------------------------------------------------------

witReturnCode witGetExtOptLpProb (
      WitRun * const theWitRun,
      int *          numcols,
      int *          numrows,
      int * *        start,
      int * *        index,
      float * *      value,
      float * *      collb,
      float * *      colub,
      float * *      obj,
      float * *      rowlb,
      float * *      rowub)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetExtOptLpProb, (
         numcols,
         numrows,
         start,
         index,
         value,
         collb,
         colub,
         obj,
         rowlb,
         rowub))
   }

//------------------------------------------------------------------------------

witReturnCode witGetExtOptLpProbDbl (
      WitRun * const theWitRun,
      int *          numcols,
      int *          numrows,
      int * *        start,
      int * *        index,
      double * *     value,
      double * *     collb,
      double * *     colub,
      double * *     obj,
      double * *     rowlb,
      double * *     rowub)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetExtOptLpProb, (
         numcols,
         numrows,
         start,
         index,
         value,
         collb,
         colub,
         obj,
         rowlb,
         rowub))
   }

//------------------------------------------------------------------------------

witReturnCode witGetFeasible (
      WitRun * const theWitRun,
      witBoolean * feasible)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetFeasible, (
         feasible))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetFocusShortageVol (
      WitRun * const theWitRun,
      int *          lenList,
      char * * *     partList,
      float * * *    shortageVolList )
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetFocusShortageVol, (
         lenList,
         partList,
         shortageVolList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetFocusShortageVolDbl (
      WitRun * const theWitRun,
      int *          lenList,
      char * * *     partList,
      double * * *   shortageVolList )
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetFocusShortageVol, (
         lenList,
         partList,
         shortageVolList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetForcedMultiEq (
      WitRun * const theWitRun,
      witBoolean * forcedMultiEq)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetForcedMultiEq, (
         forcedMultiEq))
   }

//------------------------------------------------------------------------------

witReturnCode witGetHeurAllocActive (
      WitRun * const theWitRun,
      witBoolean * heurAllocActive)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetHeurAllocActive, (
         heurAllocActive))
   }

//------------------------------------------------------------------------------

witReturnCode witGetHighPrecisionWD (
      WitRun * const theWitRun,
      witBoolean * highPrecisionWD)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetHighPrecisionWD, (
         highPrecisionWD))
   }

//------------------------------------------------------------------------------

witReturnCode witGetIndependentOffsets (
      WitRun * const theWitRun,
      witBoolean * independentOffsets)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetIndependentOffsets, (
         independentOffsets))
   }

//------------------------------------------------------------------------------

witReturnCode witGetIntCplexParSpec (
      WitRun * const     theWitRun,
      const char * const theName,
      witBoolean *       intSpecExists,
      int *              theValue)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetIntCplexParSpec, (
         theName,
         intSpecExists,
         theValue))
   }

//------------------------------------------------------------------------------

witReturnCode witGetLotSizeTol (
      WitRun * const theWitRun,
      float * lotSizeTol)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetLotSizeTol, (
         lotSizeTol))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetLotSizeTolDbl (
      WitRun * const theWitRun,
      double * lotSizeTol)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetLotSizeTol, (
         lotSizeTol))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetMesgFile (
      WitRun * const theWitRun,
      FILE * * mesgFile)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetMesgFile, (
         mesgFile))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetMesgFileAccessMode (
      WitRun * const theWitRun,
      char * * mesgFileAccessMode)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetMesgFileAccessMode, (
         mesgFileAccessMode))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetMesgFileName (
      WitRun * const theWitRun,
      char * * mesgFileName)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetMesgFileName, (
         mesgFileName))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetMesgPrintNumber (
      WitRun * const theWitRun,
      const int messageNumber,
      witBoolean * mesgPrintMunber)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetMesgPrintNumber, (
         messageNumber,
         mesgPrintMunber))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetMesgStopRunning (
      WitRun * const theWitRun,
      const int messageNumber,
      witBoolean * mesgStopRunning)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetMesgStopRunning, (
         messageNumber,
         mesgStopRunning))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetMesgThrowErrorExc (
      WitRun * const theWitRun,
      const int messageNumber,
      witBoolean * mesgThrowErrorExc)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetMesgThrowErrorExc, (
         messageNumber,
         mesgThrowErrorExc))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetMesgTimesPrint (
      WitRun * const theWitRun,
      const int messageNumber,
      int * mesgTimesPrint)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetMesgTimesPrint, (
         messageNumber,
         mesgTimesPrint))
   }

//------------------------------------------------------------------------------

witReturnCode witGetMinimalExcess (
      WitRun * const theWitRun,
      witBoolean * minimalExcess)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetMinimalExcess, (
         minimalExcess))
   }

//------------------------------------------------------------------------------

witReturnCode witGetMipMode (
      WitRun * const theWitRun,
      witBoolean * mipMode)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetMipMode, (
         mipMode))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetModHeurAlloc (
      WitRun * const theWitRun,
      witBoolean * modHeurAlloc)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetModHeurAlloc, (
         modHeurAlloc))
   }

//------------------------------------------------------------------------------

witReturnCode witGetMultiExec (
      WitRun * const theWitRun,
      witBoolean * multiExec)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetMultiExec, (
         multiExec))
   }

//------------------------------------------------------------------------------

witReturnCode witGetMultiObjMode (
      WitRun * const theWitRun,
      witBoolean * multiObjMode)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetMultiObjMode, (
         multiObjMode))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetMultiObjTol (
      WitRun * const theWitRun,
      float * multiObjTol)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetMultiObjTol, (
         multiObjTol))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetMultiObjTolDbl (
      WitRun * const theWitRun,
      double * multiObjTol)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetMultiObjTol, (
         multiObjTol))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetMultiRoute (
      WitRun * const theWitRun,
      witBoolean * multiRoute)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetMultiRoute, (
         multiRoute))
   }

//------------------------------------------------------------------------------

witReturnCode witGetNPeriods (
      WitRun * const theWitRun,
      int * nPeriods)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetNPeriods, (
         nPeriods))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetNScenarios (
      WitRun * const theWitRun,
      int * nScenarios)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetNScenarios, (
         nScenarios))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetNstnResidual (
      WitRun * const theWitRun,
      witBoolean * nstnResidual)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetNstnResidual, (
         nstnResidual))
   }

//------------------------------------------------------------------------------

witReturnCode witGetObjItrBomEntry (
      WitRun * const theWitRun,
      char * * consumingOperationName,
      int * bomEntryIndex)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetObjItrBomEntry, (
         consumingOperationName,
         bomEntryIndex))
   }

//------------------------------------------------------------------------------

witReturnCode witGetObjItrBopEntry (
      WitRun * const theWitRun,
      char * * producingOperationName,
      int * bopEntryIndex)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetObjItrBopEntry, (
         producingOperationName,
         bopEntryIndex))
   }

//------------------------------------------------------------------------------

witReturnCode witGetObjItrDemand (
      WitRun * const theWitRun,
      char * * demandedPartName,
      char * * demandName)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetObjItrDemand, (
         demandedPartName,
         demandName))
   }

//------------------------------------------------------------------------------

witReturnCode witGetObjItrOperation (
      WitRun * const theWitRun,
      char * * operationName)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetObjItrOperation, (
         operationName))
   }

//------------------------------------------------------------------------------

witReturnCode witGetObjItrPart (
      WitRun * const theWitRun,
      char * * partName)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetObjItrPart, (
         partName))
   }

//------------------------------------------------------------------------------

witReturnCode witGetObjItrState (
      WitRun * const theWitRun,
      witAttr * objItrState)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetObjItrState, (
         objItrState))
   }

//------------------------------------------------------------------------------

witReturnCode witGetObjItrSubsBomEntry (
      WitRun * const theWitRun,
      char * * consumingOperationName,
      int * bomEntryIndex,
      int * subsBomEntryIndex)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetObjItrSubsBomEntry, (
         consumingOperationName,
         bomEntryIndex,
         subsBomEntryIndex))
   }

//------------------------------------------------------------------------------

witReturnCode witGetObjValue (
      WitRun * const theWitRun,
      float *        objValue)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetObjValue, (
         objValue))
   }

//------------------------------------------------------------------------------

witReturnCode witGetObjValueDbl (
      WitRun * const theWitRun,
      double *       objValue)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetObjValue, (
         objValue))
   }

//------------------------------------------------------------------------------

witReturnCode witGetObjValues (
      WitRun * const theWitRun,
      float * objValue,
      float * boundsValue)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetObjValues, (
         objValue,
         boundsValue))
   }

//------------------------------------------------------------------------------

witReturnCode witGetObjValuesDbl (
      WitRun * const theWitRun,
      double * objValue,
      double * boundsValue)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetObjValues, (
         objValue,
         boundsValue))
   }

//------------------------------------------------------------------------------

witReturnCode witGetObjectiveList (
      WitRun * const theWitRun,
      int  *         lenList,
      char * * *     objectiveList)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetObjectiveList, (
         lenList,
         objectiveList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetObjectiveListSpec (
      WitRun * const theWitRun,
      char * * objectiveListSpec)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetObjectiveListSpec, (
         objectiveListSpec))
   }

//------------------------------------------------------------------------------

witReturnCode witGetObjectiveSeqNo (
      WitRun * const theWitRun,
      int * objectiveSeqNo)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetObjectiveSeqNo, (
         objectiveSeqNo))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetOperationAppData (
      WitRun * const theWitRun,
      const char * const operationName,
      void * * appData)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetOperationAppData, (
         operationName,
         appData))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationCoExecVolPip (
      WitRun * const     theWitRun,
      const char * const operationName,
      const int          execPeriod,
      int *              lenLists,
      char * * *         operationNameList,
      int * *            bopEntryIndexList,
      int * *            execPeriodList,
      float * *          peggedVolList)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetOperationCoExecVolPip, (
         operationName,
         execPeriod,
         lenLists,
         operationNameList,
         bopEntryIndexList,
         execPeriodList,
         peggedVolList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationCoExecVolPipDbl (
      WitRun * const     theWitRun,
      const char * const operationName,
      const int          execPeriod,
      int *              lenLists,
      char * * *         operationNameList,
      int * *            bopEntryIndexList,
      int * *            execPeriodList,
      double * *         peggedVolList)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetOperationCoExecVolPip, (
         operationName,
         execPeriod,
         lenLists,
         operationNameList,
         bopEntryIndexList,
         execPeriodList,
         peggedVolList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationConsVolPip (
      WitRun * const     theWitRun,
      const char * const operationName,
      const int          execPeriod,
      int *              lenLists,
      char * * *         partNameList,
      int * *            periodList,
      float * *          peggedVolList)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetOperationConsVolPip, (
         operationName,
         execPeriod,
         lenLists,
         partNameList,
         periodList,
         peggedVolList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationConsVolPipDbl (
      WitRun * const     theWitRun,
      const char * const operationName,
      const int          execPeriod,
      int *              lenLists,
      char * * *         partNameList,
      int * *            periodList,
      double * *         peggedVolList)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetOperationConsVolPip, (
         operationName,
         execPeriod,
         lenLists,
         partNameList,
         periodList,
         peggedVolList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationExecBounds (
      WitRun * const theWitRun,
      const char * const operationName,
      float * * hardLower,
      float * * softLower,
      float * * hardUpper)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetOperationExecBounds, (
         operationName,
         hardLower,
         softLower,
         hardUpper))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationExecBoundsDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      double * * hardLower,
      double * * softLower,
      double * * hardUpper)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetOperationExecBounds, (
         operationName,
         hardLower,
         softLower,
         hardUpper))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationExecCost (
      WitRun * const theWitRun,
      const char * const operationName,
      float * * execCost)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetOperationExecCost, (
         operationName,
         execCost))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationExecCostDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      double * * execCost)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetOperationExecCost, (
         operationName,
         execCost))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationExecPenalty (
      WitRun * const theWitRun,
      const char * const operationName,
      float * execPenalty)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetOperationExecPenalty, (
         operationName,
         execPenalty))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationExecPenaltyDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      double * execPenalty)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetOperationExecPenalty, (
         operationName,
         execPenalty))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationExecSlbConIndex (
      WitRun * const     theWitRun,
      const char * const operationName,
      const int          thePer,
      int *              execSlbConIndex)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetOperationExecSlbConIndex, (
         operationName,
         thePer,
         execSlbConIndex))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationExecSlbvVarIndex (
      WitRun * const     theWitRun,
      const char * const operationName,
      const int          thePer,
      int *              execSlbvVarIndex)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetOperationExecSlbvVarIndex, (
         operationName,
         thePer,
         execSlbvVarIndex))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationExecVarIndex (
      WitRun * const     theWitRun,
      const char * const operationName,
      const int          thePer,
      int *              execVarIndex)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetOperationExecVarIndex, (
         operationName,
         thePer,
         execVarIndex))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationExecVol (
      WitRun * const theWitRun,
      const char * const operationName,
      float * * execVol)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetOperationExecVol, (
         operationName,
         execVol))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationExecVolDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      double * * execVol)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetOperationExecVol, (
         operationName,
         execVol))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationExecVolPip (
      WitRun * const     theWitRun,
      const char * const operationName,
      const int          execPeriod,
      int *              lenLists,
      char * * *         operationNameList,
      int * *            execPeriodList,
      float * *          peggedExecVolList)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetOperationExecVolPip, (
         operationName,
         execPeriod,
         lenLists,
         operationNameList,
         execPeriodList,
         peggedExecVolList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationExecVolPipDbl (
      WitRun * const     theWitRun,
      const char * const operationName,
      const int          execPeriod,
      int *              lenLists,
      char * * *         operationNameList,
      int * *            execPeriodList,
      double * *         peggedExecVolList)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetOperationExecVolPip, (
         operationName,
         execPeriod,
         lenLists,
         operationNameList,
         execPeriodList,
         peggedExecVolList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationExecutable (
      WitRun * const theWitRun,
      const char * const operationName,
      witBoolean * * execVol)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetOperationExecutable, (
         operationName,
         execVol))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationExists (
      WitRun * const theWitRun,
      const char * const operationName,
      witBoolean * exists)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetOperationExists, (
         operationName,
         exists))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationFssExecVol (
      WitRun * const theWitRun,
      const char * const operationName,
      float * * fssExecVol)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetOperationFssExecVol, (
         operationName,
         fssExecVol))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationFssExecVolDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      double * * fssExecVol)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetOperationFssExecVol, (
         operationName,
         fssExecVol))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationIncLotSize (
      WitRun * const theWitRun,
      const char * const operationName,
      float * * incLotSize)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetOperationIncLotSize, (
         operationName,
         incLotSize))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationIncLotSizeDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      double * * incLotSize)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetOperationIncLotSize, (
         operationName,
         incLotSize))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationIncLotSize2 (
      WitRun * const theWitRun,
      const char * const operationName,
      float * * incLotSize2)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetOperationIncLotSize2, (
         operationName,
         incLotSize2))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationIncLotSize2Dbl (
      WitRun * const theWitRun,
      const char * const operationName,
      double * * incLotSize2)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetOperationIncLotSize2, (
         operationName,
         incLotSize2))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationIntExecVols (
      WitRun * const theWitRun,
      const char * const operationName,
      witBoolean * intExecVols)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetOperationIntExecVols, (
         operationName,
         intExecVols))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationLotSize2Thresh (
      WitRun * const theWitRun,
      const char * const operationName,
      float * * lotSize2Thresh)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetOperationLotSize2Thresh, (
         operationName,
         lotSize2Thresh))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationLotSize2ThreshDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      double * * lotSize2Thresh)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetOperationLotSize2Thresh, (
         operationName,
         lotSize2Thresh))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationMinLotSize (
      WitRun * const theWitRun,
      const char * const operationName,
      float * * minLotSize)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetOperationMinLotSize, (
         operationName,
         minLotSize))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationMinLotSizeDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      double * * minLotSize)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetOperationMinLotSize, (
         operationName,
         minLotSize))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationMinLotSize2 (
      WitRun * const theWitRun,
      const char * const operationName,
      float * * minLotSize2)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetOperationMinLotSize2, (
         operationName,
         minLotSize2))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationMinLotSize2Dbl (
      WitRun * const theWitRun,
      const char * const operationName,
      double * * minLotSize2)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetOperationMinLotSize2, (
         operationName,
         minLotSize2))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationMrpExecVol (
      WitRun * const theWitRun,
      const char * const operationName,
      float * * mrpExecVol)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetOperationMrpExecVol, (
         operationName,
         mrpExecVol))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationMrpExecVolDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      double * * mrpExecVol)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetOperationMrpExecVol, (
         operationName,
         mrpExecVol))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationNBomEntries (
      WitRun * const theWitRun,
      const char * const operationName,
      int * nBomEntries)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetOperationNBomEntries, (
         operationName,
         nBomEntries))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationNBopEntries (
      WitRun * const theWitRun,
      const char * const operationName,
      int * nBopEntries)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetOperationNBopEntries, (
         operationName,
         nBopEntries))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationObjectStage (
      WitRun * const theWitRun,
      const char * const operationName,
      int * objectStage)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetOperationObjectStage, (
         operationName,
         objectStage))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationPipEnabled (
      WitRun * const     theWitRun,
      const char * const operationName,
      witBoolean *       pipEnabled)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetOperationPipEnabled, (
         operationName,
         pipEnabled))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationPipRank (
      WitRun * const     theWitRun,
      const char * const operationName,
      int *              pipRank)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetOperationPipRank, (
         operationName,
         pipRank))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationProdVolPip (
      WitRun * const     theWitRun,
      const char * const operationName,
      const int          execPeriod,
      int *              lenLists,
      char * * *         partNameList,
      int * *            periodList,
      float * *          peggedVolList)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetOperationProdVolPip, (
         operationName,
         execPeriod,
         lenLists,
         partNameList,
         periodList,
         peggedVolList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationProdVolPipDbl (
      WitRun * const     theWitRun,
      const char * const operationName,
      const int          execPeriod,
      int *              lenLists,
      char * * *         partNameList,
      int * *            periodList,
      double * *         peggedVolList)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetOperationProdVolPip, (
         operationName,
         execPeriod,
         lenLists,
         partNameList,
         periodList,
         peggedVolList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationSelForDel (
      WitRun * const theWitRun,
      const char * const operationName,
      witBoolean * selForDel)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetOperationSelForDel, (
         operationName,
         selForDel))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationSideVolPip (
      WitRun * const     theWitRun,
      const char * const operationName,
      const int          execPeriod,
      int *              lenLists,
      char * * *         partNameList,
      int * *            periodList,
      float * *          peggedVolList)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetOperationSideVolPip, (
         operationName,
         execPeriod,
         lenLists,
         partNameList,
         periodList,
         peggedVolList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationSideVolPipDbl (
      WitRun * const     theWitRun,
      const char * const operationName,
      const int          execPeriod,
      int *              lenLists,
      char * * *         partNameList,
      int * *            periodList,
      double * *         peggedVolList)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetOperationSideVolPip, (
         operationName,
         execPeriod,
         lenLists,
         partNameList,
         periodList,
         peggedVolList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationSubVolPip (
      WitRun * const     theWitRun,
      const char * const operationName,
      const int          execPeriod,
      int *              lenLists,
      char * * *         operationNameList,
      int * *            bomEntryIndexList,
      int * *            subsBomEntryIndexList,
      int * *            execPeriodList,
      float * *          peggedSubVolList)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetOperationSubVolPip, (
         operationName,
         execPeriod,
         lenLists,
         operationNameList,
         bomEntryIndexList,
         subsBomEntryIndexList,
         execPeriodList,
         peggedSubVolList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationSubVolPipDbl (
      WitRun * const     theWitRun,
      const char * const operationName,
      const int          execPeriod,
      int *              lenLists,
      char * * *         operationNameList,
      int * *            bomEntryIndexList,
      int * *            subsBomEntryIndexList,
      int * *            execPeriodList,
      double * *         peggedSubVolList)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetOperationSubVolPip, (
         operationName,
         execPeriod,
         lenLists,
         operationNameList,
         bomEntryIndexList,
         subsBomEntryIndexList,
         execPeriodList,
         peggedSubVolList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationSupplyVolPip (
      WitRun * const     theWitRun,
      const char * const operationName,
      const int          execPeriod,
      int *              lenLists,
      char * * *         partNameList,
      int * *            periodList,
      float * *          peggedVolList)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetOperationSupplyVolPip, (
         operationName,
         execPeriod,
         lenLists,
         partNameList,
         periodList,
         peggedVolList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationSupplyVolPipDbl (
      WitRun * const     theWitRun,
      const char * const operationName,
      const int          execPeriod,
      int *              lenLists,
      char * * *         partNameList,
      int * *            periodList,
      double * *         peggedVolList)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetOperationSupplyVolPip, (
         operationName,
         execPeriod,
         lenLists,
         partNameList,
         periodList,
         peggedVolList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperations (
      WitRun * const theWitRun,
      int * lenOperationList,
      char * * * operationList)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetOperations, (
         lenOperationList,
         operationList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationTwoLevelLotSizes (
      WitRun * const theWitRun,
      const char * const operationName,
      witBoolean * twoLevelLotSizes)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetOperationTwoLevelLotSizes, (
         operationName,
         twoLevelLotSizes))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationYieldRate (
      WitRun * const theWitRun,
      const char * const operationName,
      float * * yieldRate)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetOperationYieldRate, (
         operationName,
         yieldRate))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationYieldRateDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      double * * yieldRate)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetOperationYieldRate, (
         operationName,
         yieldRate))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOptInitMethod (
      WitRun * const theWitRun,
      witAttr * optInitMethod)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetOptInitMethod, (
         optInitMethod))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetOutputPrecision (
      WitRun * const theWitRun,
      int * outputPrecision)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetOutputPrecision, (
         outputPrecision))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetPostprocessed (
      WitRun * const theWitRun,
      witBoolean * postprocessed)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPostprocessed, (
         postprocessed))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartAppData (
      WitRun * const theWitRun,
      const char * const partName,
      void * * appData)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartAppData, (
         partName,
         appData))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetPartAsapPipOrder (
      WitRun * const theWitRun,
      const char * const partName,
      witBoolean * asapPipOrder)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartAsapPipOrder, (
         partName,
         asapPipOrder))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartBelowList (
      WitRun * const theWitRun,
      const char * const partName,
      int * lenList,
      char * * * partNameList)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartBelowList, (
         partName,
         lenList,
         partNameList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartBoundedLeadTimes (
      WitRun * const theWitRun,
      const char * const partName,
      witBoolean * boundedLeadTimes)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartBoundedLeadTimes, (
         partName,
         boundedLeadTimes))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartBuildAheadUB (
      WitRun * const theWitRun,
      const char * const partName,
      int * * buildAheadUB)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartBuildAheadUB, (
         partName,
         buildAheadUB))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartBuildAsap (
      WitRun * const theWitRun,
      const char * const partName,
      witBoolean * buildAsap)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartBuildAsap, (
         partName,
         buildAsap))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartBuildNstn (
      WitRun * const theWitRun,
      const char * const partName,
      witBoolean * buildNstn)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartBuildNstn, (
         partName,
         buildNstn))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartConsVol (
      WitRun * const theWitRun,
      const char * const partName,
      float * * consVol)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartConsVol, (
         partName,
         consVol))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartConsVolDbl (
      WitRun * const theWitRun,
      const char * const partName,
      double * * consVol)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetPartConsVol, (
         partName,
         consVol))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartConsumingBomEntry (
      WitRun * const theWitRun,
      const char * const partName,
      const int consIndex,
      char * * consumingOperationName,
      int * bomEntryIndex)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartConsumingBomEntry, (
         partName,
         consIndex,
         consumingOperationName,
         bomEntryIndex))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartConsumingSubsBomEntry (
      WitRun * const theWitRun,
      const char * const partName,
      const int consIndex,
      char * * consumingOperationName,
      int * bomEntryIndex,
      int * subsBomEntryIndex)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartConsumingSubsBomEntry, (
         partName,
         consIndex,
         consumingOperationName,
         bomEntryIndex,
         subsBomEntryIndex))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartDemands (
      WitRun * const theWitRun,
      const char * const partName,
      int * lenDemandList,
      char * * * demandList)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartDemands, (
         partName,
         lenDemandList,
         demandList))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetPartCategory (
      WitRun * const theWitRun,
      const char * const partName,
      witAttr * category)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartCategory, (
         partName,
         category))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetPartExcessVol (
      WitRun * const theWitRun,
      const char * const partName,
      float * * excessVol)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartExcessVol, (
         partName,
         excessVol))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartExcessVolDbl (
      WitRun * const theWitRun,
      const char * const partName,
      double * * excessVol)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetPartExcessVol, (
         partName,
         excessVol))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartExists (
      WitRun * const theWitRun,
      const char * const partName,
      witBoolean * exists)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartExists, (
         partName,
         exists))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartFocusShortageVol (
      WitRun * const theWitRun,
      const char * const partName,
      float * * shortageVol)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartFocusShortageVol, (
         partName,
         shortageVol))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartFocusShortageVolDbl (
      WitRun * const theWitRun,
      const char * const partName,
      double * * shortageVol)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetPartFocusShortageVol, (
         partName,
         shortageVol))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartMrpConsVol (
      WitRun * const theWitRun,
      const char * const partName,
      float * * mrpConsVol)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartMrpConsVol, (
         partName,
         mrpConsVol))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartMrpConsVolDbl (
      WitRun * const theWitRun,
      const char * const partName,
      double * * mrpConsVol)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetPartMrpConsVol, (
         partName,
         mrpConsVol))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartMrpExcessVol (
      WitRun * const theWitRun,
      const char * const partName,
      float * * mrpExcessVol)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartMrpExcessVol, (
         partName,
         mrpExcessVol))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartMrpExcessVolDbl (
      WitRun * const theWitRun,
      const char * const partName,
      double * * mrpExcessVol)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetPartMrpExcessVol, (
         partName,
         mrpExcessVol))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartMrpResidualVol (
      WitRun * const theWitRun,
      const char * const partName,
      float * * mrpResidualVol)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartMrpResidualVol, (
         partName,
         mrpResidualVol))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartMrpResidualVolDbl (
      WitRun * const theWitRun,
      const char * const partName,
      double * * mrpResidualVol)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetPartMrpResidualVol, (
         partName,
         mrpResidualVol))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartNConsumingBomEntries (
      WitRun * const theWitRun,
      const char * const partName,
      int * nConsumingBomEntries)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartNConsumingBomEntries, (
         partName,
         nConsumingBomEntries))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartNConsumingSubsBomEntries (
      WitRun * const theWitRun,
      const char * const partName,
      int * nConsumingSubsBomEntries)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartNConsumingSubsBomEntries, (
         partName,
         nConsumingSubsBomEntries))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartNProducingBopEntries (
      WitRun * const theWitRun,
      const char * const partName,
      int * nProducingBopEntries)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartNProducingBopEntries, (
         partName,
         nProducingBopEntries))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartObjectStage (
      WitRun * const     theWitRun,
      const char * const partName,
      int *              objectStage)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartObjectStage, (
         partName,
         objectStage))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartProdVol (
      WitRun * const theWitRun,
      const char * const partName,
      float * * prodVol)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartProdVol, (
         partName,
         prodVol))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartProdVolDbl (
      WitRun * const theWitRun,
      const char * const partName,
      double * * prodVol)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetPartProdVol, (
         partName,
         prodVol))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartProducingBopEntry (
      WitRun * const theWitRun,
      const char * const partName,
      const int prodIndex,
      char * * producingOperationName,
      int * bopEntryIndex)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartProducingBopEntry, (
         partName,
         prodIndex,
         producingOperationName,
         bopEntryIndex))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartPropRtg (
      WitRun * const     theWitRun,
      const char * const partName,
      witBoolean * *     propRtg)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartPropRtg, (
         partName,
         propRtg))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartReqVol (
      WitRun * const theWitRun,
      const char * const partName,
      float * * reqVol)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartReqVol, (
         partName,
         reqVol))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetPartReqVolDbl (
      WitRun * const theWitRun,
      const char * const partName,
      double * * reqVol)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetPartReqVol, (
         partName,
         reqVol))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetPartResidualVol (
      WitRun * const theWitRun,
      const char * const partName,
      float * * residualVol)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartResidualVol, (
         partName,
         residualVol))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartResidualVolDbl (
      WitRun * const theWitRun,
      const char * const partName,
      double * * residualVol)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetPartResidualVol, (
         partName,
         residualVol))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartResourceConIndex (
      WitRun * const     theWitRun,
      const char * const partName,
      const int          thePer,
      int *              resourceConIndex)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartResourceConIndex, (
         partName,
         thePer,
         resourceConIndex))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartScrapAllowed (
      WitRun * const theWitRun,
      const char * const partName,
      witBoolean * scrapAllowed)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartScrapAllowed, (
         partName,
         scrapAllowed))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetPartScrapCost (
      WitRun * const theWitRun,
      const char * const partName,
      float * * scrapCost)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartScrapCost, (
         partName,
         scrapCost))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetPartScrapCostDbl (
      WitRun * const theWitRun,
      const char * const partName,
      double * * scrapCost)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetPartScrapCost, (
         partName,
         scrapCost))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetPartScrapVarIndex (
      WitRun * const     theWitRun,
      const char * const partName,
      const int          thePer,
      int *              scrapVarIndex)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartScrapVarIndex, (
         partName,
         thePer,
         scrapVarIndex))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartScrapVol (
      WitRun * const theWitRun,
      const char * const partName,
      float * * scrapVol)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartScrapVol, (
         partName,
         scrapVol))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetPartScrapVolDbl (
      WitRun * const theWitRun,
      const char * const partName,
      double * * scrapVol)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetPartScrapVol, (
         partName,
         scrapVol))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetPartSelForDel (
      WitRun * const theWitRun,
      const char * const partName,
      witBoolean * selForDel)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartSelForDel, (
         partName,
         selForDel))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartShadowPrice (
      WitRun * const theWitRun,
      const char * const partName,
      float * * shadowPrice)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartShadowPrice, (
         partName,
         shadowPrice))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetPartShadowPriceDbl (
      WitRun * const theWitRun,
      const char * const partName,
      double * * shadowPrice)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetPartShadowPrice, (
         partName,
         shadowPrice))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetPartSingleSource (
      WitRun * const theWitRun,
      const char * const partName,
      witBoolean * singleSource)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartSingleSource, (
         partName,
         singleSource))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetPartStockBounds (
      WitRun * const theWitRun,
      const char * const partName,
      float * * hardLower,
      float * * softLower,
      float * * hardUpper)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartStockBounds, (
         partName,
         hardLower,
         softLower,
         hardUpper))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetPartStockBoundsDbl (
      WitRun * const theWitRun,
      const char * const partName,
      double * * hardLower,
      double * * softLower,
      double * * hardUpper)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetPartStockBounds, (
         partName,
         hardLower,
         softLower,
         hardUpper))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetPartStockCost (
      WitRun * const theWitRun,
      const char * const partName,
      float * * stockCost)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartStockCost, (
         partName,
         stockCost))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartStockCostDbl (
      WitRun * const theWitRun,
      const char * const partName,
      double * * stockCost)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetPartStockCost, (
         partName,
         stockCost))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartStockSlbConIndex (
      WitRun * const     theWitRun,
      const char * const partName,
      const int          thePer,
      int *              stockSlbConIndex)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartStockSlbConIndex, (
         partName,
         thePer,
         stockSlbConIndex))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartStockSlbvVarIndex (
      WitRun * const     theWitRun,
      const char * const partName,
      const int          thePer,
      int *              stockSlbvVarIndex)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartStockSlbvVarIndex, (
         partName,
         thePer,
         stockSlbvVarIndex))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartStockVarIndex (
      WitRun * const     theWitRun,
      const char * const partName,
      const int          thePer,
      int *              stockVarIndex)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartStockVarIndex, (
         partName,
         thePer,
         stockVarIndex))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartStockVol (
      WitRun * const theWitRun,
      const char * const partName,
      float * * stockVol)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartStockVol, (
         partName,
         stockVol))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartStockVolDbl (
      WitRun * const theWitRun,
      const char * const partName,
      double * * stockVol)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetPartStockVol, (
         partName,
         stockVol))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartSupplyVol (
      WitRun * const theWitRun,
      const char * const partName,
      float * * supplyVol)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartSupplyVol, (
         partName,
         supplyVol))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartSupplyVolDbl (
      WitRun * const theWitRun,
      const char * const partName,
      double * * supplyVol)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetPartSupplyVol, (
         partName,
         supplyVol))
   }

//------------------------------------------------------------------------------

witReturnCode witGetParts (
      WitRun * const theWitRun,
      int * lenPartList,
      char * * * partList)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetParts, (
         lenPartList,
         partList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPenExec (
      WitRun * const theWitRun,
      witBoolean * penExec)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPenExec, (
         penExec))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPerfPegging (
      WitRun * const theWitRun,
      witBoolean * perfPegging)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPerfPegging, (
         perfPegging))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPeriodStage (
      WitRun * const theWitRun,
      int * *        periodStage)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPeriodStage, (
         periodStage))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPipExists (
      WitRun * const theWitRun,
      witBoolean * pipExists)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPipExists, (
         pipExists))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPipSeq (
      WitRun * const theWitRun,
      int *          lenLists,
      char * * *     partNameList,
      char * * *     demandNameList,
      int * *        shipPerList,
      float * *      incShipVolList)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPipSeq, (
         lenLists,
         partNameList,
         demandNameList,
         shipPerList,
         incShipVolList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPipSeqDbl (
      WitRun * const theWitRun,
      int *          lenLists,
      char * * *     partNameList,
      char * * *     demandNameList,
      int * *        shipPerList,
      double * *     incShipVolList)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetPipSeq, (
         lenLists,
         partNameList,
         demandNameList,
         shipPerList,
         incShipVolList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPipSeqFromHeur (
      WitRun * const theWitRun,
      witBoolean * pipSeqFromHeur)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPipSeqFromHeur, (
         pipSeqFromHeur))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPgdCritList (
      WitRun * const theWitRun,
      int *          lenLists,
      char * * *     critPartNameList,
      int * *        critPerList,
      char * * *     demPartNameList,
      char * * *     demandNameList,
      int * *        shipPerList)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPgdCritList, (
         lenLists,
         critPartNameList,
         critPerList,
         demPartNameList,
         demandNameList,
         shipPerList))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPgdCritListMode (
      WitRun * const theWitRun,
      witBoolean * pgdCritListMode)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPgdCritListMode, (
         pgdCritListMode))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetPrefHighStockSLBs (
      WitRun * const theWitRun,
      witBoolean * prefHighStockSLBs)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPrefHighStockSLBs, (
         prefHighStockSLBs))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPreferCoin (
      WitRun * const theWitRun,
      witBoolean * preferCoin)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPreferCoin, (
         preferCoin))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetPreprocessed (
      WitRun * const theWitRun,
      witBoolean * preprocessed)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPreprocessed, (
         preprocessed))
   }

//------------------------------------------------------------------------------

witReturnCode witGetProbability (
      WitRun * const theWitRun,
      float * probability)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetProbability, (
         probability))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetProbabilityDbl (
      WitRun * const theWitRun,
      double * probability)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetProbability, (
         probability))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetRespectStockSLBs (
      WitRun * const theWitRun,
      witBoolean * respectStockSLBs)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetRespectStockSLBs, (
         respectStockSLBs))
   }

//------------------------------------------------------------------------------

witReturnCode witGetRoundReqVols (
      WitRun * const theWitRun,
      witBoolean * roundReqVols)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetRoundReqVols, (
         roundReqVols))
   }

//------------------------------------------------------------------------------

witReturnCode witGetSelSplit (
      WitRun * const theWitRun,
      witBoolean * selSplit)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetSelSplit, (
         selSplit))
   }

//------------------------------------------------------------------------------

witReturnCode witGetSelectionRecovery (
      WitRun * const theWitRun,
      witBoolean * selectionRecovery)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetSelectionRecovery, (
         selectionRecovery))
   }

//------------------------------------------------------------------------------

witReturnCode witGetSkipFailures (
      WitRun * const theWitRun,
      witBoolean * skipFailures)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetSkipFailures, (
         skipFailures))
   }

//------------------------------------------------------------------------------

witReturnCode witGetSolverLogFileName (
      WitRun * const theWitRun,
      char * * solverLogFileName)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetSolverLogFileName, (
         solverLogFileName))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetStageByObject (
      WitRun * const theWitRun,
      witBoolean *   stageByObject)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetStageByObject, (
         stageByObject))
   }

//------------------------------------------------------------------------------

witReturnCode witGetStochMode (
      WitRun * const theWitRun,
      witBoolean * stochMode)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetStochMode, (
         stochMode))
   }

//------------------------------------------------------------------------------

witReturnCode witGetStochSolnMode (
      WitRun * const theWitRun,
      witBoolean * stochSolnMode)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetStochSolnMode, (
         stochSolnMode))
   }

//------------------------------------------------------------------------------

witReturnCode witGetStockReallocation (
      WitRun * const theWitRun,
      witBoolean * stockReallocation)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetStockReallocation, (
         stockReallocation))
   }

//------------------------------------------------------------------------------

witReturnCode witGetSubsBomEntryAppData (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      void * * appData)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetSubsBomEntryAppData, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         appData))
   }

//------------------------------------------------------------------------------

witReturnCode witGetSubsBomEntryConsRate (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      float * * consRate)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetSubsBomEntryConsRate, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         consRate))
   }

//------------------------------------------------------------------------------

witReturnCode witGetSubsBomEntryConsRateDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      double * * consRate)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetSubsBomEntryConsRate, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         consRate))
   }

//------------------------------------------------------------------------------

witReturnCode witGetSubsBomEntryConsumedPart (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      char * * consumedPart)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetSubsBomEntryConsumedPart, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         consumedPart))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetSubsBomEntryEarliestPeriod (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      int * earliestPeriod)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetSubsBomEntryEarliestPeriod, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         earliestPeriod))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetSubsBomEntryExecPenalty (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      float * execPenalty)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetSubsBomEntryExecPenalty, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         execPenalty))
   }

//------------------------------------------------------------------------------

witReturnCode witGetSubsBomEntryExecPenaltyDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      double * execPenalty)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetSubsBomEntryExecPenalty, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         execPenalty))
   }

//------------------------------------------------------------------------------

witReturnCode witGetSubsBomEntryExpAllowed (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      witBoolean * expAllowed)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetSubsBomEntryExpAllowed, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         expAllowed))
   }

//------------------------------------------------------------------------------

witReturnCode witGetSubsBomEntryExpNetAversion (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      float * expNetAversion)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetSubsBomEntryExpNetAversion, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         expNetAversion))
   }

//------------------------------------------------------------------------------

witReturnCode witGetSubsBomEntryExpNetAversionDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      double * expNetAversion)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetSubsBomEntryExpNetAversion, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         expNetAversion))
   }

//------------------------------------------------------------------------------

witReturnCode witGetSubsBomEntryFalloutRate (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      float * falloutRate)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetSubsBomEntryFalloutRate, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         falloutRate))
   }

//------------------------------------------------------------------------------

witReturnCode witGetSubsBomEntryFalloutRateDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      double * falloutRate)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetSubsBomEntryFalloutRate, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         falloutRate))
   }

//------------------------------------------------------------------------------

witReturnCode witGetSubsBomEntryFssSubVol (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      float * * fssSubVol)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetSubsBomEntryFssSubVol, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         fssSubVol))
   }

//------------------------------------------------------------------------------

witReturnCode witGetSubsBomEntryFssSubVolDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      double * * fssSubVol)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetSubsBomEntryFssSubVol, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         fssSubVol))
   }

//------------------------------------------------------------------------------

witReturnCode witGetSubsBomEntryImpactPeriod (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      int * * impactPeriod)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetSubsBomEntryImpactPeriod, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         impactPeriod))
   }

//------------------------------------------------------------------------------

witReturnCode witGetSubsBomEntryIntSubVols (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      witBoolean * intSubVols)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetSubsBomEntryIntSubVols, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         intSubVols))
   }

//------------------------------------------------------------------------------

witReturnCode witGetSubsBomEntryLatestPeriod (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      int * latestPeriod)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetSubsBomEntryLatestPeriod, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         latestPeriod))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetSubsBomEntryMrpNetAllowed (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      witBoolean * mrpNetAllowed)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetSubsBomEntryMrpNetAllowed, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         mrpNetAllowed))
   }

//------------------------------------------------------------------------------

witReturnCode witGetSubsBomEntryMrpSubVol (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      float * * mrpSubVol)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetSubsBomEntryMrpSubVol, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         mrpSubVol))
   }

//------------------------------------------------------------------------------

witReturnCode witGetSubsBomEntryMrpSubVolDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      double * * mrpSubVol)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetSubsBomEntryMrpSubVol, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         mrpSubVol))
   }

//------------------------------------------------------------------------------

witReturnCode witGetSubsBomEntryNetAllowed (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      witBoolean * netAllowed)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetSubsBomEntryNetAllowed, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         netAllowed))
   }

//------------------------------------------------------------------------------

witReturnCode witGetSubsBomEntryObj1SubCost (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      float * * obj1SubCost)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetSubsBomEntryObj1SubCost, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         obj1SubCost))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetSubsBomEntryObj1SubCostDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      double * * obj1SubCost)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetSubsBomEntryObj1SubCost, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         obj1SubCost))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetSubsBomEntryOffset (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      float * * offset)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetSubsBomEntryOffset, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         offset))
   }

//------------------------------------------------------------------------------

witReturnCode witGetSubsBomEntryOffsetDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      double * * offset)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetSubsBomEntryOffset, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         offset))
   }

//------------------------------------------------------------------------------

witReturnCode witGetSubsBomEntryRoutingShare (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      float * * routingShare)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetSubsBomEntryRoutingShare, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         routingShare))
   }

//------------------------------------------------------------------------------

witReturnCode witGetSubsBomEntryRoutingShareDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      double * * routingShare)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetSubsBomEntryRoutingShare, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         routingShare))
   }

//------------------------------------------------------------------------------

witReturnCode witGetSubsBomEntrySelForDel (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      witBoolean * selForDel)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetSubsBomEntrySelForDel, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         selForDel))
   }

//------------------------------------------------------------------------------

witReturnCode witGetSubsBomEntrySubCost (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      float * * subCost)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetSubsBomEntrySubCost, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         subCost))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetSubsBomEntrySubCostDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      double * * subCost)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetSubsBomEntrySubCost, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         subCost))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetSubsBomEntrySubVarIndex (
      WitRun * const     theWitRun,
      const char * const consumingOperationName,
      const int          bomEntryIndex,
      const int          subsBomEntryIndex,
      const int          thePer,
      int *              subVarIndex)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetSubsBomEntrySubVarIndex, (
         consumingOperationName,
         bomEntryIndex,
         subsBomEntryIndex,
         thePer,
         subVarIndex))
   }

//------------------------------------------------------------------------------

witReturnCode witGetSubsBomEntrySubVol (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      float * * subVol)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetSubsBomEntrySubVol, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         subVol))
   }

//------------------------------------------------------------------------------

witReturnCode witGetSubsBomEntrySubVolDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      double * * subVol)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetSubsBomEntrySubVol, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         subVol))
   }

//------------------------------------------------------------------------------

witReturnCode witGetTieBreakPropRt (
      WitRun * const theWitRun,
      witBoolean * tieBreakPropRt)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetTieBreakPropRt, (
         tieBreakPropRt))
   }

//------------------------------------------------------------------------------

witReturnCode witGetTitle (
      WitRun * const theWitRun,
      char * * title)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetTitle, (
         title))
   }

//------------------------------------------------------------------------------

witReturnCode witGetTruncOffsets (
      WitRun * const theWitRun,
      witBoolean * truncOffsets)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetTruncOffsets, (
         truncOffsets))
   }

//------------------------------------------------------------------------------

witReturnCode witGetTwoWayMultiExec (
      WitRun * const theWitRun,
      witBoolean * twoWayMultiExec)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetTwoWayMultiExec, (
         twoWayMultiExec))
   }

//------------------------------------------------------------------------------

witReturnCode witGetUseFocusHorizons (
      WitRun * const theWitRun,
      witBoolean * useFocusHorizons)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetUseFocusHorizons, (
         useFocusHorizons))
   }

//------------------------------------------------------------------------------

witReturnCode witGetUserHeurStart (
      WitRun * const theWitRun,
      witBoolean * userHeurStart)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetUserHeurStart, (
         userHeurStart))
   }

//------------------------------------------------------------------------------

witReturnCode witGetWbounds (
      WitRun * const theWitRun,
      float * wbounds)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetWbounds, (
         wbounds))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetWboundsDbl (
      WitRun * const theWitRun,
      double * wbounds)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetWbounds, (
         wbounds))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetWit34Compatible (
      WitRun * const theWitRun,
      witBoolean * wit34Compatible)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetWit34Compatible, (
         wit34Compatible))
   }

//------------------------------------------------------------------------------

witReturnCode witHeurImplode (
      WitRun * const theWitRun)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witHeurImplode, ())
   }
 
//------------------------------------------------------------------------------

witReturnCode witIncHeurAlloc (
      WitRun * const theWitRun,
      const char * const demandedPartName,
      const char * const demandName,
      const int shipPeriod,
      const float desIncVol,
      float * incVol)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witIncHeurAlloc, (
         demandedPartName,
         demandName,
         shipPeriod,
         desIncVol,
         incVol))
   }

//------------------------------------------------------------------------------

witReturnCode witIncHeurAllocDbl (
      WitRun * const theWitRun,
      const char * const demandedPartName,
      const char * const demandName,
      const int shipPeriod,
      const double desIncVol,
      double * incVol)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witIncHeurAlloc, (
         demandedPartName,
         demandName,
         shipPeriod,
         desIncVol,
         incVol))
   }

//------------------------------------------------------------------------------

witReturnCode witIncHeurAllocTwme (
      WitRun * const theWitRun,
      const char * const demandedPartName,
      const char * const demandName,
      const int shipPeriod,
      const float desIncVol,
      float * incVol,
      witBoolean asapMultiExec)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witIncHeurAllocTwme, (
         demandedPartName,
         demandName,
         shipPeriod,
         desIncVol,
         incVol,
         asapMultiExec))
   }

//------------------------------------------------------------------------------

witReturnCode witIncHeurAllocTwmeDbl (
      WitRun * const theWitRun,
      const char * const demandedPartName,
      const char * const demandName,
      const int shipPeriod,
      const double desIncVol,
      double * incVol,
      witBoolean asapMultiExec)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witIncHeurAllocTwme, (
         demandedPartName,
         demandName,
         shipPeriod,
         desIncVol,
         incVol,
         asapMultiExec))
   }

//------------------------------------------------------------------------------

witReturnCode witInitialize (
      WitRun * const theWitRun)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witInitialize, ())
   }

//------------------------------------------------------------------------------

witReturnCode witMrp (
      WitRun * const theWitRun)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witMrp, ())
   }
 
//------------------------------------------------------------------------------
// Non-Standard API Function.
//------------------------------------------------------------------------------

witReturnCode witNewRun (
      WitRun * * theWitRunPtr)
   {
   forbidNullArg (theWitRunPtr, "theWitRunPtr", "witNewRun");

   * theWitRunPtr = WitRun::newInstance ();

   STANDARD_API_IMPL (
      (* theWitRunPtr),
      witNewRun, ())
   }

//------------------------------------------------------------------------------

witReturnCode witOptImplode (
      WitRun * const theWitRun)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witOptImplode, ())
   }

//------------------------------------------------------------------------------

witReturnCode witPostprocess (
      WitRun * const theWitRun)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witPostprocess, ())
   }
 
//------------------------------------------------------------------------------

witReturnCode witPreprocess (
      WitRun * const theWitRun)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witPreprocess, ())
   }
 
//------------------------------------------------------------------------------

witReturnCode witPurgeData (
      WitRun * const theWitRun)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witPurgeData, ())
   }

//------------------------------------------------------------------------------

witReturnCode witReadData (
      WitRun * const theWitRun,
      const char * const fileName)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witReadData, (
         fileName))
   }
 
//------------------------------------------------------------------------------

witReturnCode witResetObjItr (
      WitRun * const theWitRun)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witResetObjItr, ())
   }

//------------------------------------------------------------------------------

witReturnCode witSetAccAfterSoftLB (
      WitRun * const theWitRun,
      const witBoolean accAfterSoftLB)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetAccAfterSoftLB, (
         accAfterSoftLB))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetAccAfterOptImp (
      WitRun * const theWitRun,
      const witBoolean accAfterOptImp)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetAccAfterOptImp, (
         accAfterOptImp))
   }

//------------------------------------------------------------------------------

witReturnCode witSetAppData (
      WitRun * const theWitRun,
      void * const appData)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetAppData, (
         appData))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetAutoPriority (
      WitRun * const theWitRun,
      const witBoolean autoPriority)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetAutoPriority, (
         autoPriority))
   }
  
//------------------------------------------------------------------------------

witReturnCode witSetBomEntryAppData (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      void * const appData)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetBomEntryAppData, (
         operationName,
         bomEntryIndex,
         appData))
   }

//------------------------------------------------------------------------------

witReturnCode witSetBomEntryConsRate (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const float * consRate)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetBomEntryConsRate, (
         operationName,
         bomEntryIndex,
         consRate))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetBomEntryConsRateDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const double * consRate)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetBomEntryConsRate, (
         operationName,
         bomEntryIndex,
         consRate))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetBomEntryEarliestPeriod (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int earliestPeriod)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetBomEntryEarliestPeriod, (
         operationName,
         bomEntryIndex,
         earliestPeriod))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetBomEntryExecPenalty (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const float execPenalty)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetBomEntryExecPenalty, (
         operationName,
         bomEntryIndex,
         execPenalty))
   }

//------------------------------------------------------------------------------

witReturnCode witSetBomEntryExecPenaltyDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const double execPenalty)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetBomEntryExecPenalty, (
         operationName,
         bomEntryIndex,
         execPenalty))
   }

//------------------------------------------------------------------------------

witReturnCode witSetBomEntryFalloutRate (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const float falloutRate)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetBomEntryFalloutRate, (
         operationName,
         bomEntryIndex,
         falloutRate))
   }

//------------------------------------------------------------------------------

witReturnCode witSetBomEntryFalloutRateDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const double falloutRate)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetBomEntryFalloutRate, (
         operationName,
         bomEntryIndex,
         falloutRate))
   }

//------------------------------------------------------------------------------

witReturnCode witSetBomEntryLatestPeriod (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int latestPeriod)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetBomEntryLatestPeriod, (
         operationName,
         bomEntryIndex,
         latestPeriod))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetBomEntryMandEC (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const witBoolean mandEC)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetBomEntryMandEC, (
         operationName,
         bomEntryIndex,
         mandEC))
   }

//------------------------------------------------------------------------------

witReturnCode witSetBomEntryOffset (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const float * const offset)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetBomEntryOffset, (
         operationName,
         bomEntryIndex,
         offset))
   }

//------------------------------------------------------------------------------

witReturnCode witSetBomEntryOffsetDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const double * const offset)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetBomEntryOffset, (
         operationName,
         bomEntryIndex,
         offset))
   }

//------------------------------------------------------------------------------

witReturnCode witSetBomEntryPropRtg (
      WitRun * const           theWitRun,
      const char * const       operationName,
      const int                bomEntryIndex,
      const witBoolean * const propRtg)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetBomEntryPropRtg, (
         operationName,
         bomEntryIndex,
         propRtg))
   }

//------------------------------------------------------------------------------

witReturnCode witSetBomEntryRoutingShare (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const float * routingShare)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetBomEntryRoutingShare, (
         operationName,
         bomEntryIndex,
         routingShare))
   }

//------------------------------------------------------------------------------

witReturnCode witSetBomEntryRoutingShareDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const double * routingShare)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetBomEntryRoutingShare, (
         operationName,
         bomEntryIndex,
         routingShare))
   }

//------------------------------------------------------------------------------

witReturnCode witSetBomEntrySelForDel (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const witBoolean selForDel)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetBomEntrySelForDel, (
         operationName,
         bomEntryIndex,
         selForDel))
   }

//------------------------------------------------------------------------------

witReturnCode witSetBomEntrySingleSource (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const witBoolean singleSource)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetBomEntrySingleSource, (
         operationName,
         bomEntryIndex,
         singleSource))
   }

//------------------------------------------------------------------------------

witReturnCode witSetBopEntryAppData (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bopEntryIndex,
      void * const appData)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetBopEntryAppData, (
         operationName,
         bopEntryIndex,
         appData))
   }

//------------------------------------------------------------------------------

witReturnCode witSetBopEntryEarliestPeriod (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bopEntryIndex,
      const int earliestPeriod)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetBopEntryEarliestPeriod, (
         operationName,
         bopEntryIndex,
         earliestPeriod))
   }

//------------------------------------------------------------------------------

witReturnCode witSetBopEntryExpAllowed (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bopEntryIndex,
      const witBoolean expAllowed)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetBopEntryExpAllowed, (
         operationName,
         bopEntryIndex,
         expAllowed))
   }

//------------------------------------------------------------------------------

witReturnCode witSetBopEntryExpAversion (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bopEntryIndex,
      const float expAversion)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetBopEntryExpAversion, (
         operationName,
         bopEntryIndex,
         expAversion))
   }

//------------------------------------------------------------------------------

witReturnCode witSetBopEntryExpAversionDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bopEntryIndex,
      const double expAversion)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetBopEntryExpAversion, (
         operationName,
         bopEntryIndex,
         expAversion))
   }

//------------------------------------------------------------------------------

witReturnCode witSetBopEntryLatestPeriod (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bopEntryIndex,
      const int latestPeriod)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetBopEntryLatestPeriod, (
         operationName,
         bopEntryIndex,
         latestPeriod))
   }

//------------------------------------------------------------------------------

witReturnCode witSetBopEntryOffset (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bopEntryIndex,
      const float * const offset)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetBopEntryOffset, (
         operationName,
         bopEntryIndex,
         offset))
   }

//------------------------------------------------------------------------------

witReturnCode witSetBopEntryOffsetDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bopEntryIndex,
      const double * const offset)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetBopEntryOffset, (
         operationName,
         bopEntryIndex,
         offset))
   }

//------------------------------------------------------------------------------

witReturnCode witSetBopEntryProductRate (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bopEntryIndex,
      const float * productRate)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetBopEntryProductRate, (
         operationName,
         bopEntryIndex,
         productRate))
   }

//------------------------------------------------------------------------------

witReturnCode witSetBopEntryProductRateDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bopEntryIndex,
      const double * productRate)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetBopEntryProductRate, (
         operationName,
         bopEntryIndex,
         productRate))
   }

//------------------------------------------------------------------------------

witReturnCode witSetBopEntryRoutingShare (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bopEntryIndex,
      const float * routingShare)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetBopEntryRoutingShare, (
         operationName,
         bopEntryIndex,
         routingShare))
   }

//------------------------------------------------------------------------------

witReturnCode witSetBopEntryRoutingShareDbl ( 
      WitRun * const theWitRun,
      const char * const operationName,
      const int bopEntryIndex,
      const double * routingShare)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetBopEntryRoutingShare, (
         operationName,
         bopEntryIndex,
         routingShare))
   }

//------------------------------------------------------------------------------

witReturnCode witSetBopEntrySelForDel (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bopEntryIndex,
      const witBoolean selForDel)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetBopEntrySelForDel, (
         operationName,
         bopEntryIndex,
         selForDel))
   }

//------------------------------------------------------------------------------

witReturnCode witSetCompPrices (
      WitRun * const theWitRun,
      const witBoolean compPrices)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetCompPrices, (
         compPrices))
   }

//------------------------------------------------------------------------------

witReturnCode witSetComputeCriticalList (
      WitRun * const theWitRun,
      const witBoolean computeCriticalList)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetComputeCriticalList, (
         computeCriticalList))
   }
  
//------------------------------------------------------------------------------

witReturnCode witSetCplexParSpecDblVal (
      WitRun * const theWitRun,
      const float cplexParSpecDblVal)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetCplexParSpecDblVal, (
         cplexParSpecDblVal))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetCplexParSpecDblValDbl (
      WitRun * const theWitRun,
      const double cplexParSpecDblVal)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetCplexParSpecDblVal, (
         cplexParSpecDblVal))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetCplexParSpecIntVal (
      WitRun * const theWitRun,
      const int cplexParSpecIntVal)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetCplexParSpecIntVal, (
         cplexParSpecIntVal))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetCplexParSpecName (
      WitRun * const theWitRun,
      const char * const cplexParSpecName)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetCplexParSpecName, (
         cplexParSpecName))
   }
  
//------------------------------------------------------------------------------

witReturnCode witSetCurrentObjective (
      WitRun * const theWitRun,
      const char * const theObjName)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetCurrentObjective, (
         theObjName))
   }
  
//------------------------------------------------------------------------------

witReturnCode witSetCurrentScenario (
      WitRun * const theWitRun,
      const int currentScenario)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetCurrentScenario, (
         currentScenario))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetDemandAppData (
      WitRun * const theWitRun,
      const char * const partName,
      const char * const demandName,
      void * const appData)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetDemandAppData, (
         partName,
         demandName,
         appData))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetDemandCumShipBounds (
      WitRun * const theWitRun,
      const char * const partName,
      const char * const demandName,
      const float * const hardLower,
      const float * const softLower,
      const float * const hardUpper)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetDemandCumShipBounds, (
         partName,
         demandName,
         hardLower,
         softLower,
         hardUpper))
   }

//------------------------------------------------------------------------------

witReturnCode witSetDemandCumShipBoundsDbl ( 
      WitRun * const theWitRun,
      const char * const partName,
      const char * const demandName,
      const double * const hardLower,
      const double * const softLower,
      const double * const hardUpper)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetDemandCumShipBounds, (
         partName,
         demandName,
         hardLower,
         softLower,
         hardUpper))
   }

//------------------------------------------------------------------------------

witReturnCode witSetDemandCumShipReward (
      WitRun * const theWitRun,
      const char * const partName,
      const char * const demandName,
      const float * const cumShipReward)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetDemandCumShipReward, (
         partName,
         demandName,
         cumShipReward))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetDemandCumShipRewardDbl ( 
      WitRun * const theWitRun,
      const char * const partName,
      const char * const demandName,
      const double * const cumShipReward)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetDemandCumShipReward, (
         partName,
         demandName,
         cumShipReward))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetDemandDemandName (
      WitRun * const     theWitRun,
      const char * const partName,
      const char * const demandName,
      const char * const newName)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetDemandDemandName, (
         partName,
         demandName,
         newName))
   }

//------------------------------------------------------------------------------

witReturnCode witSetDemandDemandVol (
      WitRun * const theWitRun,
      const char * const partName,
      const char * const demandName,
      const float * const demandVol)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetDemandDemandVol, (
         partName,
         demandName,
         demandVol))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetDemandDemandVolDbl (
      WitRun * const theWitRun,
      const char * const partName,
      const char * const demandName,
      const double * const demandVol)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetDemandDemandVol, (
         partName,
         demandName,
         demandVol))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetDemandFocusHorizon (
      WitRun * const theWitRun,
      const char * const partName,
      const char * const demandName,
      const int focusHorizon)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetDemandFocusHorizon, (
         partName,
         demandName,
         focusHorizon))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetDemandFssShipVol (
      WitRun * const theWitRun,
      const char * const partName,
      const char * const demandName,
      const float * const fssShipVol)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetDemandFssShipVol, (
         partName,
         demandName,
         fssShipVol))
   }

//------------------------------------------------------------------------------

witReturnCode witSetDemandFssShipVolDbl (
      WitRun * const theWitRun,
      const char * const partName,
      const char * const demandName,
      const double * const fssShipVol)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetDemandFssShipVol, (
         partName,
         demandName,
         fssShipVol))
   }

//------------------------------------------------------------------------------

witReturnCode witSetDemandIntShipVols (
      WitRun * const theWitRun,
      const char * const partName,
      const char * const demandName,
      const int intShipVols)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetDemandIntShipVols, (
         partName,
         demandName,
         intShipVols))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetDemandLeadTimeUB (
      WitRun * const theWitRun,
      const char * const partName,
      const char * const demandName,
      const int * leadTimeUB)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetDemandLeadTimeUB, (
         partName,
         demandName,
         leadTimeUB))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetDemandPriority (
      WitRun * const theWitRun,
      const char * const partName,
      const char * const demandName,
      const int * const priority)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetDemandPriority, (
         partName,
         demandName,
         priority))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetDemandSearchInc (
      WitRun * const theWitRun,
      const char * const partName,
      const char * const demandName,
      const float searchInc)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetDemandSearchInc, (
         partName,
         demandName,
         searchInc))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetDemandSearchIncDbl (
      WitRun * const theWitRun,
      const char * const partName,
      const char * const demandName,
      const double searchInc)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetDemandSearchInc, (
         partName,
         demandName,
         searchInc))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetDemandSelForDel (
      WitRun * const theWitRun,
      const char * const partName,
      const char * const demandName,
      const witBoolean selForDel)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetDemandSelForDel, (
         partName,
         demandName,
         selForDel))
   }

//------------------------------------------------------------------------------

witReturnCode witSetDemandShipLateAllowed (
      WitRun * const theWitRun,
      const char * const partName,
      const char * const demandName,
      const int shipLateAllowed)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetDemandShipLateAllowed, (
         partName,
         demandName,
         shipLateAllowed))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetDemandShipLateUB (
      WitRun * const theWitRun,
      const char * const partName,
      const char * const demandName,
      const int * shipLateUB)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetDemandShipLateUB, (
         partName,
         demandName,
         shipLateUB))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetDemandShipReward (
      WitRun * const theWitRun,
      const char * const partName,
      const char * const demandName,
      const float * const shipReward)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetDemandShipReward, (
         partName,
         demandName,
         shipReward))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetDemandShipRewardDbl (
      WitRun * const theWitRun,
      const char * const partName,
      const char * const demandName,
      const double * const shipReward)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetDemandShipReward, (
         partName,
         demandName,
         shipReward))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetDemandShipVol (
      WitRun * const theWitRun,
      const char * const partName,
      const char * const demandName,
      const float * const shipVol)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetDemandShipVol, (
         partName,
         demandName,
         shipVol))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetDemandShipVolDbl (
      WitRun * const theWitRun,
      const char * const partName,
      const char * const demandName,
      const double * const shipVol)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetDemandShipVol, (
         partName,
         demandName,
         shipVol))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetEquitability (
      WitRun * const theWitRun,
      const int equitablity)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetEquitability, (
         equitablity))
   }

//------------------------------------------------------------------------------

witReturnCode witSetExecEmptyBom (
      WitRun * const theWitRun,
      const witBoolean execEmptyBom)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetExecEmptyBom, (
         execEmptyBom))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetExpCutoff (
      WitRun * const theWitRun,
      const float expCutoff)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetExpCutoff, (
         expCutoff))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetExpCutoffDbl (
      WitRun * const theWitRun,
      const double expCutoff)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetExpCutoff, (
         expCutoff))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetExtOptSoln (
      WitRun * const      theWitRun,
      const float * const colsol)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetExtOptSoln, (
         colsol))
   }

//------------------------------------------------------------------------------

witReturnCode witSetExtOptSolnDbl (
      WitRun * const       theWitRun,
      const double * const colsol)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetExtOptSoln, (
         colsol))
   }

//------------------------------------------------------------------------------

witReturnCode witSetForcedMultiEq (
      WitRun * const theWitRun,
      const witBoolean forcedMultiEq)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetForcedMultiEq, (
         forcedMultiEq))
   }

//------------------------------------------------------------------------------

witReturnCode witSetHighPrecisionWD (
      WitRun * const theWitRun,
      const witBoolean highPrecisionWD)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetHighPrecisionWD, (
         highPrecisionWD))
   }

//------------------------------------------------------------------------------

witReturnCode witSetIndependentOffsets (
      WitRun * const theWitRun,
      const witBoolean independentOffsets)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetIndependentOffsets, (
         independentOffsets))
   }

//------------------------------------------------------------------------------

witReturnCode witSetLotSizeTol (
      WitRun * const theWitRun,
      const float lotSizeTol)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetLotSizeTol, (
         lotSizeTol))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetLotSizeTolDbl (
      WitRun * const theWitRun,
      const double lotSizeTol)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetLotSizeTol, (
         lotSizeTol))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetMesgFileAccessMode (
      WitRun * const theWitRun,
      const witBoolean quiet,
      const char * const mesgFileAccessMode)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetMesgFileAccessMode, (
         quiet,
         mesgFileAccessMode))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetMesgFileName (
      WitRun * const theWitRun,
      const witBoolean quiet,
      const char * const mesgFileName)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetMesgFileName, (
         quiet,
         mesgFileName))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetMesgPrintNumber (
      WitRun * const theWitRun,
      const witBoolean quiet,
      const int messageNumber,
      const witBoolean mesgPrintMunber)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetMesgPrintNumber, (
         quiet,
         messageNumber,
         mesgPrintMunber))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetMesgStopRunning (
      WitRun * const theWitRun,
      const witBoolean quiet,
      const int messageNumber,
      const witBoolean mesgStopRunning)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetMesgStopRunning, (
         quiet,
         messageNumber,
         mesgStopRunning))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetMesgThrowErrorExc (
      WitRun * const theWitRun,
      const witBoolean quiet,
      const int messageNumber,
      const witBoolean mesgThrowErrorExc)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetMesgThrowErrorExc, (
         quiet,
         messageNumber,
         mesgThrowErrorExc))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetMesgTimesPrint (
      WitRun * const theWitRun,
      const witBoolean quiet,
      const int messageNumber,
      const int mesgTimesPrint)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetMesgTimesPrint, (
         quiet,
         messageNumber,
         mesgTimesPrint))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetMinimalExcess (
      WitRun * const theWitRun,
      const witBoolean minimalExcess)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetMinimalExcess, (
         minimalExcess))
   }

//------------------------------------------------------------------------------

witReturnCode witSetMipMode (
      WitRun * const theWitRun,
      const witBoolean mipMode)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetMipMode, (
         mipMode))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetModHeurAlloc (
      WitRun * const theWitRun,
      const witBoolean modHeurAlloc)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetModHeurAlloc, (
         modHeurAlloc))
   }

//------------------------------------------------------------------------------

witReturnCode witSetMultiExec (
      WitRun * const theWitRun,
      const witBoolean multiExec)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetMultiExec, (
         multiExec))
   }

//------------------------------------------------------------------------------

witReturnCode witSetMultiObjMode (
      WitRun * const theWitRun,
      const witBoolean multiObjMode)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetMultiObjMode, (
         multiObjMode))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetMultiObjTol (
      WitRun * const theWitRun,
      const float multiObjTol)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetMultiObjTol, (
         multiObjTol))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetMultiObjTolDbl (
      WitRun * const theWitRun,
      const double multiObjTol)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetMultiObjTol, (
         multiObjTol))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetMultiRoute (
      WitRun * const theWitRun,
      const witBoolean multiRoute)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetMultiRoute, (
         multiRoute))
   }

//------------------------------------------------------------------------------

witReturnCode witSetNPeriods (
      WitRun * const theWitRun,
      const int nPeriods)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetNPeriods, (
         nPeriods))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetNScenarios (
      WitRun * const theWitRun,
      const int nScenarios)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetNScenarios, (
         nScenarios))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetNstnResidual (
      WitRun * const theWitRun,
      const witBoolean nstnResidual)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetNstnResidual, (
         nstnResidual))
   }

//------------------------------------------------------------------------------

witReturnCode witSetObjectiveList (
      WitRun * const             theWitRun,
      const int                  lenList,
      const char * const * const objectiveList)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetObjectiveList, (
         lenList,
         objectiveList))
   }

//------------------------------------------------------------------------------

witReturnCode witSetObjectiveListSpec (
      WitRun * const     theWitRun,
      const char * const objectiveListSpec)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetObjectiveListSpec, (
         objectiveListSpec))
   }

//------------------------------------------------------------------------------

witReturnCode witSetObjectiveSeqNo (
      WitRun * const theWitRun,
      const int objectiveSeqNo)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetObjectiveSeqNo, (
         objectiveSeqNo))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetOperationAppData (
      WitRun * const theWitRun,
      const char * const operationName,
      void * const appData)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetOperationAppData, (
         operationName,
         appData))
   }

//------------------------------------------------------------------------------

witReturnCode witSetOperationExecBounds (
      WitRun * const theWitRun,
      const char * const operationName,
      const float * const hardLower,
      const float * const softLower,
      const float * const hardUpper)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetOperationExecBounds, (
         operationName,
         hardLower,
         softLower,
         hardUpper))
   }

//------------------------------------------------------------------------------

witReturnCode witSetOperationExecBoundsDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const double * const hardLower,
      const double * const softLower,
      const double * const hardUpper)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetOperationExecBounds, (
         operationName,
         hardLower,
         softLower,
         hardUpper))
   }

//------------------------------------------------------------------------------

witReturnCode witSetOperationExecCost (
      WitRun * const theWitRun,
      const char * const operationName,
      const float * const execCost)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetOperationExecCost, (
         operationName,
         execCost))
   }

//------------------------------------------------------------------------------

witReturnCode witSetOperationExecCostDbl ( 
      WitRun * const theWitRun,
      const char * const operationName,
      const double * const execCost)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetOperationExecCost, (
         operationName,
         execCost))
   }

//------------------------------------------------------------------------------

witReturnCode witSetOperationExecPenalty (
      WitRun * const theWitRun,
      const char * const operationName,
      const float execPenalty)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetOperationExecPenalty, (
         operationName,
         execPenalty))
   }

//------------------------------------------------------------------------------

witReturnCode witSetOperationExecPenaltyDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const double execPenalty)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetOperationExecPenalty, (
         operationName,
         execPenalty))
   }

//------------------------------------------------------------------------------

witReturnCode witSetOperationExecVol (
      WitRun * const theWitRun,
      const char * const operationName,
      const float * const execVol)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetOperationExecVol, (
         operationName,
         execVol))
   }

//------------------------------------------------------------------------------

witReturnCode witSetOperationExecVolDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const double * const execVol)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetOperationExecVol, (
         operationName,
         execVol))
   }

//------------------------------------------------------------------------------

witReturnCode witSetOperationIncLotSize (
      WitRun * const theWitRun,
      const char * const operationName,
      const float * const incLotSize)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetOperationIncLotSize, (
         operationName,
         incLotSize))
   }

//------------------------------------------------------------------------------

witReturnCode witSetOperationIncLotSizeDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const double * const incLotSize)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetOperationIncLotSize, (
         operationName,
         incLotSize))
   }

//------------------------------------------------------------------------------

witReturnCode witSetOperationIncLotSize2 (
      WitRun * const theWitRun,
      const char * const operationName,
      const float * const incLotSize2)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetOperationIncLotSize2, (
         operationName,
         incLotSize2))
   }

//------------------------------------------------------------------------------

witReturnCode witSetOperationIncLotSize2Dbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const double * const incLotSize2)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetOperationIncLotSize2, (
         operationName,
         incLotSize2))
   }

//------------------------------------------------------------------------------

witReturnCode witSetOperationIntExecVols (
      WitRun * const theWitRun,
      const char * const operationName,
      const witBoolean intExecVols)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetOperationIntExecVols, (
         operationName,
         intExecVols))
   }

//------------------------------------------------------------------------------

witReturnCode witSetOperationLotSize2Thresh (
      WitRun * const theWitRun,
      const char * const operationName,
      const float * const lotSize2Thresh)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetOperationLotSize2Thresh, (
         operationName,
         lotSize2Thresh))
   }

//------------------------------------------------------------------------------

witReturnCode witSetOperationLotSize2ThreshDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const double * const lotSize2Thresh)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetOperationLotSize2Thresh, (
         operationName,
         lotSize2Thresh))
   }

//------------------------------------------------------------------------------

witReturnCode witSetOperationMinLotSize (
      WitRun * const theWitRun,
      const char * const operationName,
      const float * const minLotSize)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetOperationMinLotSize, (
         operationName,
         minLotSize))
   }

//------------------------------------------------------------------------------

witReturnCode witSetOperationMinLotSizeDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const double * const minLotSize)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetOperationMinLotSize, (
         operationName,
         minLotSize))
   }

//------------------------------------------------------------------------------

witReturnCode witSetOperationMinLotSize2 (
      WitRun * const theWitRun,
      const char * const operationName,
      const float * const minLotSize2)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetOperationMinLotSize2, (
         operationName,
         minLotSize2))
   }

//------------------------------------------------------------------------------

witReturnCode witSetOperationMinLotSize2Dbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const double * const minLotSize2)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetOperationMinLotSize2, (
         operationName,
         minLotSize2))
   }

//------------------------------------------------------------------------------

witReturnCode witSetOperationObjectStage (
      WitRun * const     theWitRun,
      const char * const operationName,
      const int          objectStage)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetOperationObjectStage, (
         operationName,
         objectStage))
   }

//------------------------------------------------------------------------------

witReturnCode witSetOperationOperationName (
      WitRun * const     theWitRun,
      const char * const operationName,
      const char * const newName)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetOperationOperationName, (
         operationName,
         newName))
   }

//------------------------------------------------------------------------------

witReturnCode witSetOperationPipEnabled (
      WitRun * const     theWitRun,
      const char * const operationName,
      const witBoolean   pipEnabled)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetOperationPipEnabled, (
         operationName,
         pipEnabled))
   }

//------------------------------------------------------------------------------

witReturnCode witSetOperationPipRank (
      WitRun * const     theWitRun,
      const char * const operationName,
      const int          pipRank)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetOperationPipRank, (
         operationName,
         pipRank))
   }

//------------------------------------------------------------------------------

witReturnCode witSetOperationSelForDel (
      WitRun * const theWitRun,
      const char * const operationName,
      const witBoolean selForDel)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetOperationSelForDel, (
         operationName,
         selForDel))
   }

//------------------------------------------------------------------------------

witReturnCode witSetOperationTwoLevelLotSizes (
      WitRun * const theWitRun,
      const char * const operationName,
      const witBoolean twoLevelLotSizes)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetOperationTwoLevelLotSizes, (
         operationName,
         twoLevelLotSizes))
   }

//------------------------------------------------------------------------------

witReturnCode witSetOperationYieldRate (
      WitRun * const theWitRun,
      const char * const operationName,
      const float * const yieldRate)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetOperationYieldRate, (
         operationName,
         yieldRate))
   }

//------------------------------------------------------------------------------

witReturnCode witSetOperationYieldRateDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const double * const yieldRate)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetOperationYieldRate, (
         operationName,
         yieldRate))
   }

//------------------------------------------------------------------------------

witReturnCode witSetOptInitMethod (
      WitRun * const theWitRun,
      const witAttr optInitMethod)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetOptInitMethod, (
         optInitMethod))
   }

//------------------------------------------------------------------------------

witReturnCode witSetOutputPrecision (
      WitRun * const theWitRun,
      const int outputPrecision)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetOutputPrecision, (
         outputPrecision))
   }

//------------------------------------------------------------------------------

witReturnCode witSetPartAppData (
      WitRun * const theWitRun,
      const char * const partName,
      void * const appData)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetPartAppData, (
         partName,
         appData))
   }

//------------------------------------------------------------------------------

witReturnCode witSetPartAsapPipOrder (
      WitRun * const theWitRun,
      const char * const partName,
      const witBoolean asapPipOrder)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetPartAsapPipOrder, (
         partName,
         asapPipOrder))
   }

//------------------------------------------------------------------------------

witReturnCode witSetPartBoundedLeadTimes (
      WitRun * const theWitRun,
      const char * const partName,
      const witBoolean boundedLeadTimes)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetPartBoundedLeadTimes, (
         partName,
         boundedLeadTimes))
   }

//------------------------------------------------------------------------------

witReturnCode witSetPartBuildAheadUB (
      WitRun * const theWitRun,
      const char * const partName,
      const int * const buildAheadUB)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetPartBuildAheadUB, (
         partName,
         buildAheadUB))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetPartBuildAsap (
      WitRun * const theWitRun,
      const char * const partName,
      const witBoolean buildAsap)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetPartBuildAsap, (
         partName,
         buildAsap))
   }

//------------------------------------------------------------------------------

witReturnCode witSetPartBuildNstn (
      WitRun * const theWitRun,
      const char * const partName,
      const witBoolean buildNstn)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetPartBuildNstn, (
         partName,
         buildNstn))
   }

//------------------------------------------------------------------------------

witReturnCode witSetPartObjectStage (
      WitRun * const     theWitRun,
      const char * const partName,
      const int          objectStage)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetPartObjectStage, (
         partName,
         objectStage))
   }

//------------------------------------------------------------------------------

witReturnCode witSetPartPartName (
      WitRun * const     theWitRun,
      const char * const partName,
      const char * const newName)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetPartPartName, (
         partName,
         newName))
   }

//------------------------------------------------------------------------------

witReturnCode witSetPartPropRtg (
      WitRun * const theWitRun,
      const char * const partName,
      const witBoolean * const propRtg)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetPartPropRtg, (
         partName,
         propRtg))
   }

//------------------------------------------------------------------------------

witReturnCode witSetPartScrapAllowed (
      WitRun * const theWitRun,
      const char * const partName,
      const witBoolean scrapAllowed)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetPartScrapAllowed, (
         partName,
         scrapAllowed))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetPartScrapCost (
      WitRun * const theWitRun,
      const char * const partName,
      const float * const scrapCost)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetPartScrapCost, (
         partName,
         scrapCost))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetPartScrapCostDbl (
      WitRun * const theWitRun,
      const char * const partName,
      const double * const scrapCost)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetPartScrapCost, (
         partName,
         scrapCost
         ))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetPartSelForDel (
      WitRun * const theWitRun,
      const char * const partName,
      const witBoolean selForDel)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetPartSelForDel, (
         partName,
         selForDel))
   }

//------------------------------------------------------------------------------

witReturnCode witSetPartSingleSource (
      WitRun * const theWitRun,
      const char * const partName,
      const witBoolean singleSource)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetPartSingleSource, (
         partName,
         singleSource))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetPartStockBounds (
      WitRun * const theWitRun,
      const char * const partName,
      const float * const hardLower,
      const float * const softLower,
      const float * const hardUpper)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetPartStockBounds, (
         partName,
         hardLower,
         softLower,
         hardUpper))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetPartStockBoundsDbl (
      WitRun * const theWitRun,
      const char * const partName,
      const double * const hardLower,
      const double * const softLower,
      const double * const hardUpper)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetPartStockBounds, (
         partName,
         hardLower,
         softLower,
         hardUpper))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetPartStockCost (
      WitRun * const theWitRun,
      const char * const partName,
      const float * const stockCost)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetPartStockCost, (
         partName,
         stockCost))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetPartStockCostDbl (
      WitRun * const theWitRun,
      const char * const partName,
      const double * const stockCost)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetPartStockCost, (
         partName,
         stockCost))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetPartSupplyVol (
      WitRun * const theWitRun,
      const char * const partName,
      const float * const supplyVol)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetPartSupplyVol, (
         partName,
         supplyVol))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetPartSupplyVolDbl ( 
      WitRun * const theWitRun,
      const char * const partName,
      const double * const supplyVol)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetPartSupplyVol, (
         partName,
         supplyVol))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetPenExec (
      WitRun * const theWitRun,
      const witBoolean penExec)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetPenExec, (
         penExec))
   }

//------------------------------------------------------------------------------

witReturnCode witSetPerfPegging (
      WitRun * const theWitRun,
      const witBoolean perfPegging)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetPerfPegging, (
         perfPegging))
   }

//------------------------------------------------------------------------------

witReturnCode witSetPeriodStage (
      WitRun * const theWitRun,
      const int *    periodStage)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetPeriodStage, (
         periodStage))
   }

//------------------------------------------------------------------------------

witReturnCode witSetPgdCritListMode (
      WitRun * const theWitRun,
      const witBoolean pgdCritListMode)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetPgdCritListMode, (
         pgdCritListMode))
   }
  
//------------------------------------------------------------------------------

witReturnCode witSetPipSeqFromHeur (
      WitRun * const theWitRun,
      const witBoolean pipSeqFromHeur)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetPipSeqFromHeur, (
         pipSeqFromHeur))
   }

//------------------------------------------------------------------------------

witReturnCode witSetPrefHighStockSLBs (
      WitRun * const theWitRun,
      const witBoolean prefHighStockSLBs)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetPrefHighStockSLBs, (
         prefHighStockSLBs))
   }

//------------------------------------------------------------------------------

witReturnCode witSetPreferCoin (
      WitRun * const theWitRun,
      const witBoolean preferCoin)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetPreferCoin, (
         preferCoin))
   }

//------------------------------------------------------------------------------

witReturnCode witSetProbability (
      WitRun * const theWitRun,
      const float probability)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetProbability, (
         probability))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetProbabilityDbl (
      WitRun * const theWitRun,
      const double probability)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetProbability, (
         probability))
   }

 //------------------------------------------------------------------------------

witReturnCode witSetRespectStockSLBs (
      WitRun * const theWitRun,
      const witBoolean respectStockSLBs)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetRespectStockSLBs, (
         respectStockSLBs))
   }

//------------------------------------------------------------------------------

witReturnCode witSetRoundReqVols (
      WitRun * const theWitRun,
      const witBoolean roundReqVols)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetRoundReqVols, (
         roundReqVols))
   }

//------------------------------------------------------------------------------

witReturnCode witSetSelSplit (
      WitRun * const theWitRun,
      const witBoolean selSplit)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetSelSplit, (
         selSplit))
   }

//------------------------------------------------------------------------------

witReturnCode witSetSelectionRecovery (
      WitRun * const theWitRun,
      const witBoolean selectionRecovery)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetSelectionRecovery, (
         selectionRecovery))
   }

//------------------------------------------------------------------------------

witReturnCode witSetSkipFailures (
      WitRun * const theWitRun,
      const witBoolean skipFailures)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetSkipFailures, (
         skipFailures))
   }

//------------------------------------------------------------------------------

witReturnCode witSetSolverLogFileName (
      WitRun * const theWitRun,
      const char * const solverLogFileName)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetSolverLogFileName, (
         solverLogFileName))
   }
  
//------------------------------------------------------------------------------

witReturnCode witSetStageByObject (
      WitRun * const theWitRun,
      const witBoolean stageByObject)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetStageByObject, (
         stageByObject))
   }

//------------------------------------------------------------------------------

witReturnCode witSetStochMode (
      WitRun * const theWitRun,
      const witBoolean stochMode)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetStochMode, (
         stochMode))
   }

//------------------------------------------------------------------------------

witReturnCode witSetStockReallocation (
      WitRun * const theWitRun,
      const witBoolean stockReallocation)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetStockReallocation, (
         stockReallocation))
   }

//------------------------------------------------------------------------------

witReturnCode witSetSubsBomEntryAppData (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      void * const appData)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetSubsBomEntryAppData, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         appData))
   }

//------------------------------------------------------------------------------

witReturnCode witSetSubsBomEntryConsRate (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      const float * consRate)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetSubsBomEntryConsRate, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         consRate))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetSubsBomEntryConsRateDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      const double * consRate)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetSubsBomEntryConsRate, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         consRate))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetSubsBomEntryEarliestPeriod (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      const int earliestPeriod)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetSubsBomEntryEarliestPeriod, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         earliestPeriod))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetSubsBomEntryExecPenalty (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      const float execPenalty)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetSubsBomEntryExecPenalty, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         execPenalty))
   }

//------------------------------------------------------------------------------

witReturnCode witSetSubsBomEntryExecPenaltyDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      const double execPenalty)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetSubsBomEntryExecPenalty, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         execPenalty))
   }

//------------------------------------------------------------------------------

witReturnCode witSetSubsBomEntryExpAllowed (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      const witBoolean expAllowed)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetSubsBomEntryExpAllowed, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         expAllowed))
   }

//------------------------------------------------------------------------------

witReturnCode witSetSubsBomEntryExpNetAversion (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      const float expNetAversion)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetSubsBomEntryExpNetAversion, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         expNetAversion))
   }

//------------------------------------------------------------------------------

witReturnCode witSetSubsBomEntryExpNetAversionDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      const double expNetAversion)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetSubsBomEntryExpNetAversion, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         expNetAversion))
   }

//------------------------------------------------------------------------------

witReturnCode witSetSubsBomEntryFalloutRate (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      const float falloutRate)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetSubsBomEntryFalloutRate, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         falloutRate))
   }

//------------------------------------------------------------------------------

witReturnCode witSetSubsBomEntryFalloutRateDbl ( 
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      const double falloutRate)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetSubsBomEntryFalloutRate, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         falloutRate))
   }

//------------------------------------------------------------------------------

witReturnCode witSetSubsBomEntryIntSubVols (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      const witBoolean intSubVols)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetSubsBomEntryIntSubVols, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         intSubVols))
   }

//------------------------------------------------------------------------------

witReturnCode witSetSubsBomEntryLatestPeriod (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      const int latestPeriod)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetSubsBomEntryLatestPeriod, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         latestPeriod))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetSubsBomEntryMrpNetAllowed (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      const witBoolean mrpNetAllowed)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetSubsBomEntryMrpNetAllowed, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         mrpNetAllowed))
   }

//------------------------------------------------------------------------------

witReturnCode witSetSubsBomEntryNetAllowed (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      const witBoolean netAllowed)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetSubsBomEntryNetAllowed, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         netAllowed))
   }

//------------------------------------------------------------------------------

witReturnCode witSetSubsBomEntryObj1SubCost (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      const float * const obj1SubCost)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetSubsBomEntryObj1SubCost, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         obj1SubCost))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetSubsBomEntryObj1SubCostDbl ( 
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      const double * const obj1SubCost)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetSubsBomEntryObj1SubCost, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         obj1SubCost))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetSubsBomEntryOffset (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      const float * const offset)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetSubsBomEntryOffset, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         offset))
   }

//------------------------------------------------------------------------------

witReturnCode witSetSubsBomEntryOffsetDbl ( 
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      const double * const offset)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetSubsBomEntryOffset, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         offset))
   }

//------------------------------------------------------------------------------

witReturnCode witSetSubsBomEntryRoutingShare (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      const float * routingShare)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetSubsBomEntryRoutingShare, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         routingShare))
   }

//------------------------------------------------------------------------------

witReturnCode witSetSubsBomEntryRoutingShareDbl ( 
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      const double * routingShare)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetSubsBomEntryRoutingShare, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         routingShare))
   }

//------------------------------------------------------------------------------

witReturnCode witSetSubsBomEntrySelForDel (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      const witBoolean selForDel)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetSubsBomEntrySelForDel, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         selForDel))
   }

//------------------------------------------------------------------------------

witReturnCode witSetSubsBomEntrySubCost (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      const float * const subCost)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetSubsBomEntrySubCost, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         subCost))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetSubsBomEntrySubCostDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      const double * const subCost)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetSubsBomEntrySubCost, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         subCost))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetSubsBomEntrySubVol (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      const float * const subVol)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetSubsBomEntrySubVol, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         subVol))
   }

//------------------------------------------------------------------------------

witReturnCode witSetSubsBomEntrySubVolDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      const double * const subVol)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetSubsBomEntrySubVol, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         subVol))
   }

//------------------------------------------------------------------------------

witReturnCode witSetTieBreakPropRt (
      WitRun * const theWitRun,
      const witBoolean tieBreakPropRt)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetTieBreakPropRt, (
         tieBreakPropRt))
   }

//------------------------------------------------------------------------------

witReturnCode witSetTitle (
      WitRun * const theWitRun,
      const char * const title)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetTitle, (
         title))
   }

//------------------------------------------------------------------------------

witReturnCode witSetTruncOffsets (
      WitRun * const theWitRun,
      const witBoolean truncOffsets)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetTruncOffsets, (
         truncOffsets))
   }

//------------------------------------------------------------------------------

witReturnCode witSetTwoWayMultiExec (
      WitRun * const theWitRun,
      const witBoolean twoWayMultiExec)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetTwoWayMultiExec, (
         twoWayMultiExec))
   }

//------------------------------------------------------------------------------

witReturnCode witSetUseFocusHorizons (
      WitRun * const theWitRun,
      const witBoolean useFocusHorizons)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetUseFocusHorizons, (
         useFocusHorizons))
   }

//------------------------------------------------------------------------------

witReturnCode witSetUserHeurStart (
      WitRun * const theWitRun,
      const witBoolean userHeurStart)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetUserHeurStart, (
         userHeurStart))
   }

//------------------------------------------------------------------------------

witReturnCode witSetWbounds (
      WitRun * const theWitRun,
      const float wbounds)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetWbounds, (
         wbounds))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetWboundsDbl (
      WitRun * const theWitRun,
      const double wbounds)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetWbounds, (
         wbounds))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetWit34Compatible (
      WitRun * const theWitRun,
      const witBoolean wit34Compatible)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetWit34Compatible, (
         wit34Compatible))
   }

//------------------------------------------------------------------------------

witReturnCode witShutDownExtOpt (
      WitRun * const theWitRun)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witShutDownExtOpt, ())
   }

//------------------------------------------------------------------------------

witReturnCode witShutDownHeurAlloc (
      WitRun * const theWitRun)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witShutDownHeurAlloc, ())
   }

//------------------------------------------------------------------------------

witReturnCode witStartExtOpt (
      WitRun * const theWitRun)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witStartExtOpt, ())
   }

//------------------------------------------------------------------------------

witReturnCode witStartHeurAlloc (
      WitRun * const theWitRun)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witStartHeurAlloc, ())
   }

//------------------------------------------------------------------------------

witReturnCode witStochImplode (
      WitRun * const theWitRun)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witStochImplode, ())
   }

//------------------------------------------------------------------------------

witReturnCode witWriteCriticalList (
      WitRun * const theWitRun,
      const char * const fileName,
      const witFileFormat fileFormat,
      const int maxListLen)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witWriteCriticalList, (
         fileName,
         fileFormat,
         maxListLen))
   }

//------------------------------------------------------------------------------

witReturnCode witWriteData (
      WitRun * const theWitRun,
      const char * const fileName)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witWriteData, (
         fileName))
   }
 
//------------------------------------------------------------------------------

witReturnCode witWriteExecSched (
      WitRun * const theWitRun,
      const char * const fileName,
      const witFileFormat fileFormat)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witWriteExecSched, (
         fileName,
         fileFormat))
   }
 
//------------------------------------------------------------------------------

witReturnCode witWriteReqSched (
      WitRun * const theWitRun,
      const char * const fileName,
      const witFileFormat fileFormat)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witWriteReqSched, (
         fileName,
         fileFormat))
   }
 
//------------------------------------------------------------------------------

witReturnCode witWriteShipSched (
      WitRun * const theWitRun,
      const char * const fileName,
      const witFileFormat fileFormat)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witWriteShipSched, (
         fileName,
         fileFormat))
   }

//------------------------------------------------------------------------------
// The following functions that were formerly part of WIT's documented API,
// but have since been removed from the documentation.
// They are included here for upward compatibility.
//------------------------------------------------------------------------------

witReturnCode witGetBomEntryFallout (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      int * fallout)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetBomEntryFallout, (
         operationName,
         bomEntryIndex,
         fallout))
   }

//------------------------------------------------------------------------------

witReturnCode witGetBomEntryPropRouting (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      witBoolean * propRouting)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetBomEntryPropRouting, (
         operationName,
         bomEntryIndex,
         propRouting))
   }

//------------------------------------------------------------------------------

witReturnCode witGetBomEntryUsageRate (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      float * usageRate)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetBomEntryUsageRate, (
         operationName,
         bomEntryIndex,
         usageRate))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetBomEntryUsageTime (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      float * time)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetBomEntryUsageTime, (
         operationName,
         bomEntryIndex,
         time))
   }

//------------------------------------------------------------------------------

witReturnCode witGetBopEntryByProduct (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bopEntryIndex,
      witBoolean * byProduct)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetBopEntryByProduct, (
         operationName,
         bopEntryIndex,
         byProduct))
   }

//------------------------------------------------------------------------------

witReturnCode witGetBopEntryProdRate (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bopEntryIndex,
      float * prodRate)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetBopEntryProdRate, (
         operationName,
         bopEntryIndex,
         prodRate))
   }

//------------------------------------------------------------------------------

witReturnCode witGetBuildWhenEmpty (
      WitRun * const theWitRun,
      witBoolean * buildWhenEmpty)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetBuildWhenEmpty, (
         buildWhenEmpty))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetDemandBuildAheadLimit (
      WitRun * const theWitRun,
      const char * const partName,
      const char * const demandName,
      int * buildAheadLimit)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetDemandBuildAheadLimit, (
         partName,
         demandName,
         buildAheadLimit))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandBuildAheadUB (
      WitRun * const theWitRun,
      const char * const partName,
      const char * const demandName,
      int * * buildAheadUB)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetDemandBuildAheadUB, (
         partName,
         demandName,
         buildAheadUB))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandObj1CumShipReward (
      WitRun * const theWitRun,
      const char * const demandedPartName,
      const char * const demandName,
      float * * obj1CumShipReward)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetDemandObj1CumShipReward, (
         demandedPartName,
         demandName,
         obj1CumShipReward))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetDemandObj1CumShipRewardDbl (
      WitRun * const theWitRun,
      const char * const demandedPartName,
      const char * const demandName,
      double * * obj1CumShipReward)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetDemandObj1CumShipReward, (
         demandedPartName,
         demandName,
         obj1CumShipReward))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetDemandObj1ShipReward (
      WitRun * const theWitRun,
      const char * const demandedPartName,
      const char * const demandName,
      float * * obj1ShipReward)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetDemandObj1ShipReward, (
         demandedPartName,
         demandName,
         obj1ShipReward))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandObj1ShipRewardDbl (
      WitRun * const theWitRun,
      const char * const demandedPartName,
      const char * const demandName,
      double * * obj1ShipReward)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetDemandObj1ShipReward, (
         demandedPartName,
         demandName,
         obj1ShipReward))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandPrefBuildAhead (
      WitRun * const theWitRun,
      const char * const partName,
      const char * const demandName,
      int * prefBuildAhead)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetDemandPrefBuildAhead, (
         partName,
         demandName,
         prefBuildAhead))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDemandShipLateLimit (
      WitRun * const theWitRun,
      const char * const partName,
      const char * const demandName,
      int * shipLateLimit)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetDemandShipLateLimit, (
         partName,
         demandName,
         shipLateLimit))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetHashTableSize (
      WitRun * const theWitRun,
      int * hashTableSize)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetHashTableSize, (
         hashTableSize))
   }

//------------------------------------------------------------------------------

witReturnCode witGetLocalBuildAhead (
      WitRun * const theWitRun,
      witBoolean * localBuildAhead)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetLocalBuildAhead, (
         localBuildAhead))
   }

//------------------------------------------------------------------------------

witReturnCode witGetLocalMultiExec (
      WitRun * const theWitRun,
      witBoolean * localMultiExec)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetLocalMultiExec, (
         localMultiExec))
   }

//------------------------------------------------------------------------------

witReturnCode witGetObjChoice (
      WitRun * const theWitRun,
      int * objChoice)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetObjChoice, (
         objChoice))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationObj1ExecCost (
      WitRun * const theWitRun,
      const char * const operationName,
      float * * obj1ExecCost)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetOperationObj1ExecCost, (
         operationName,
         obj1ExecCost))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationObj1ExecCostDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      double * * obj1ExecCost)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetOperationObj1ExecCost, (
         operationName,
         obj1ExecCost))
   }

//------------------------------------------------------------------------------

witReturnCode witGetOperationYield (
      WitRun * const theWitRun,
      const char * const operationName,
      int * * yield)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetOperationYield, (
         operationName,
         yield))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartBuildAheadLimit (
      WitRun * const theWitRun,
      const char * const partName,
      int * buildAheadLimit)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartBuildAheadLimit, (
         partName,
         buildAheadLimit))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartCycleTime (
      WitRun * const theWitRun,
      const char * const partName,
      float * * cycleTime)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartCycleTime, (
         partName,
         cycleTime))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartIncLotSize (
      WitRun * const theWitRun,
      const char * const partName,
      float * * incLotSize)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartIncLotSize, (
         partName,
         incLotSize))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartMinLotSize (
      WitRun * const theWitRun,
      const char * const partName,
      float * * minLotSize)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartMinLotSize, (
         partName,
         minLotSize))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartMrpProdVol (
      WitRun * const theWitRun,
      const char * const partName,
      float * * mrpProdVol)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartMrpProdVol, (
         partName,
         mrpProdVol))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartNBomEntries (
      WitRun * const theWitRun,
      const char * const partName,
      int * nBomEntries)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartNBomEntries, (
         partName,
         nBomEntries))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartObj1ProdCost (
      WitRun * const theWitRun,
      const char * const partName,
      float * * obj1ProdCost)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartObj1ProdCost, (
         partName,
         obj1ProdCost))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartObj1ScrapCost (
      WitRun * const theWitRun,
      const char * const partName,
      float * * obj1ScrapCost)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartObj1ScrapCost, (
         partName,
         obj1ScrapCost))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetPartObj1ScrapCostDbl (
      WitRun * const theWitRun,
      const char * const partName,
      double * * obj1ScrapCost)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetPartObj1ScrapCost, (
         partName,
         obj1ScrapCost))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetPartObj1StockCost (
      WitRun * const theWitRun,
      const char * const partName,
      float * * obj1StockCost)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartObj1StockCost, (
         partName,
         obj1StockCost))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartObj1StockCostDbl (
      WitRun * const theWitRun,
      const char * const partName,
      double * * obj1StockCost)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witGetPartObj1StockCost, (
         partName,
         obj1StockCost))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartProdBounds (
      WitRun * const theWitRun,
      const char * const partName,
      float * * hardLower,
      float * * softLower,
      float * * hardUpper)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartProdBounds, (
         partName,
         hardLower,
         softLower,
         hardUpper))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartPropRouting (
      WitRun * const theWitRun,
      const char * const partName,
      witBoolean * propRouting)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartPropRouting, (
         partName,
         propRouting))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetPartYield (
      WitRun * const theWitRun,
      const char * const partName,
      int * * yield)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartYield, (
         partName,
         yield))
   }

//------------------------------------------------------------------------------

witReturnCode witGetStockRealloc (
      WitRun * const theWitRun,
      witBoolean * stockRealloc)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetStockRealloc, (
         stockRealloc))
   }

//------------------------------------------------------------------------------

witReturnCode witGetSubsBomEntryFallout (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      int * fallout)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetSubsBomEntryFallout, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         fallout))
   }

//------------------------------------------------------------------------------

witReturnCode witGetSubsBomEntryProdVol (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      float * * prodVol)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetSubsBomEntryProdVol, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         prodVol))
   }
 
//------------------------------------------------------------------------------

witReturnCode witGetSubsBomEntryUsageRate (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      float * usageRate)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetSubsBomEntryUsageRate, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         usageRate))
   }

//------------------------------------------------------------------------------

witReturnCode witOptPreprocess (
      WitRun * const theWitRun)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witOptPreprocess, ())
   }

//------------------------------------------------------------------------------

witReturnCode witSetBomEntryFallout (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int fallout)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetBomEntryFallout, (
         operationName,
         bomEntryIndex,
         fallout))
   }

//------------------------------------------------------------------------------

witReturnCode witSetBomEntryPropRouting (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const witBoolean propRouting)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetBomEntryPropRouting, (
         operationName,
         bomEntryIndex,
         propRouting))
   }

//------------------------------------------------------------------------------

witReturnCode witSetBomEntryUsageRate (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const float usageRate)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetBomEntryUsageRate, (
         operationName,
         bomEntryIndex,
         usageRate))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetBomEntryUsageTime (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const float time)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetBomEntryUsageTime, (
         operationName,
         bomEntryIndex,
         time))
   }

//------------------------------------------------------------------------------

witReturnCode witSetBopEntryByProduct (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bopEntryIndex,
      const witBoolean byProduct)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetBopEntryByProduct, (
         operationName,
         bopEntryIndex,
         byProduct))
   }

//------------------------------------------------------------------------------

witReturnCode witSetBopEntryProdRate (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bopEntryIndex,
      const float prodRate)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetBopEntryProdRate, (
         operationName,
         bopEntryIndex,
         prodRate))
   }

//------------------------------------------------------------------------------

witReturnCode witSetBuildWhenEmpty (
      WitRun * const theWitRun,
      const witBoolean buildWhenEmpty)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetBuildWhenEmpty, (
         buildWhenEmpty))
   }

//------------------------------------------------------------------------------

witReturnCode witSetDemandBuildAheadLimit (
      WitRun * const theWitRun,
      const char * const partName,
      const char * const demandName,
      const int buildAheadLimit)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetDemandBuildAheadLimit, (
         partName,
         demandName,
         buildAheadLimit))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetDemandBuildAheadUB (
      WitRun * const theWitRun,
      const char * const partName,
      const char * const demandName,
      const int * buildAheadUB)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetDemandBuildAheadUB, (
         partName,
         demandName,
         buildAheadUB))
   }

//------------------------------------------------------------------------------

witReturnCode witSetDemandObj1CumShipReward (
      WitRun * const theWitRun,
      const char * const partName,
      const char * const demandName,
      const float * const obj1CumShipReward)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetDemandObj1CumShipReward, (
         partName,
         demandName,
         obj1CumShipReward))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetDemandObj1CumShipRewardDbl (
      WitRun * const theWitRun,
      const char * const partName,
      const char * const demandName,
      const double * const obj1CumShipReward)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetDemandObj1CumShipReward, (
         partName,
         demandName,
         obj1CumShipReward))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetDemandObj1ShipReward (
      WitRun * const theWitRun,
      const char * const partName,
      const char * const demandName,
      const float * const obj1ShipReward)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetDemandObj1ShipReward, (
         partName,
         demandName,
         obj1ShipReward))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetDemandObj1ShipRewardDbl (
      WitRun * const theWitRun,
      const char * const partName,
      const char * const demandName,
      const double * const obj1ShipReward)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetDemandObj1ShipReward, (
         partName,
         demandName,
         obj1ShipReward))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetDemandPrefBuildAhead (
      WitRun * const theWitRun,
      const char * const partName,
      const char * const demandName,
      const int prefBuildAhead)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetDemandPrefBuildAhead, (
         partName,
         demandName,
         prefBuildAhead))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetDemandShipLateLimit (
      WitRun * const theWitRun,
      const char * const partName,
      const char * const demandName,
      const int shipLateLimit)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetDemandShipLateLimit, (
         partName,
         demandName,
         shipLateLimit))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetHashTableSize (
      WitRun * const theWitRun,
      const int hashTableSize)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetHashTableSize, (
         hashTableSize))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetLocalBuildAhead (
      WitRun * const theWitRun,
      const witBoolean localBuildAhead)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetLocalBuildAhead, (
         localBuildAhead))
   }

//------------------------------------------------------------------------------

witReturnCode witSetLocalMultiExec (
      WitRun * const theWitRun,
      const witBoolean localMultiExec)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetLocalMultiExec, (
         localMultiExec))
   }

//------------------------------------------------------------------------------

witReturnCode witSetObjChoice (
      WitRun * const theWitRun,
      const int objChoice)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetObjChoice, (
         objChoice))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetOperationObj1ExecCost (
      WitRun * const theWitRun,
      const char * const operationName,
      const float * const obj1ExecCost)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetOperationObj1ExecCost, (
         operationName,
         obj1ExecCost))
   }

//------------------------------------------------------------------------------

witReturnCode witSetOperationObj1ExecCostDbl (
      WitRun * const theWitRun,
      const char * const operationName,
      const double * const obj1ExecCost)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetOperationObj1ExecCost, (
         operationName,
         obj1ExecCost))
   }

//------------------------------------------------------------------------------

witReturnCode witSetOperationYield (
      WitRun * const theWitRun,
      const char * const operationName,
      const int * const yield)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetOperationYield, (
         operationName,
         yield))
   }

//------------------------------------------------------------------------------

witReturnCode witSetPartBuildAheadLimit (
      WitRun * const theWitRun,
      const char * const partName,
      const int buildAheadLimit)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetPartBuildAheadLimit, (
         partName,
         buildAheadLimit))
   }

//------------------------------------------------------------------------------

witReturnCode witSetPartCycleTime (
      WitRun * const theWitRun,
      const char * const partName,
      const float * const cycleTime)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetPartCycleTime, (
         partName,
         cycleTime))
   }

//------------------------------------------------------------------------------

witReturnCode witSetPartIncLotSize (
      WitRun * const theWitRun,
      const char * const partName,
      const float * const incLotSize)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetPartIncLotSize, (
         partName,
         incLotSize))
   }

//------------------------------------------------------------------------------

witReturnCode witSetPartMinLotSize (
      WitRun * const theWitRun,
      const char * const partName,
      const float * const minLotSize)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetPartMinLotSize, (
         partName,
         minLotSize))
   }

//------------------------------------------------------------------------------

witReturnCode witSetPartObj1ProdCost (
      WitRun * const theWitRun,
      const char * const partName,
      const float * const obj1ProdCost)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetPartObj1ProdCost, (
         partName,
         obj1ProdCost))
   }

//------------------------------------------------------------------------------

witReturnCode witSetPartObj1ScrapCost (
      WitRun * const theWitRun,
      const char * const partName,
      const float * const obj1ScrapCost)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetPartObj1ScrapCost, (
         partName,
         obj1ScrapCost
         ))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetPartObj1ScrapCostDbl (
      WitRun * const theWitRun,
      const char * const partName,
      const double * const obj1ScrapCost)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetPartObj1ScrapCost, (
         partName,
         obj1ScrapCost
         ))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetPartObj1StockCost (
      WitRun * const theWitRun,
      const char * const partName,
      const float * const obj1StockCost)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetPartObj1StockCost, (
         partName,
         obj1StockCost))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetPartObj1StockCostDbl (
      WitRun * const theWitRun,
      const char * const partName,
      const double * const obj1StockCost)
   {
   STANDARD_DBL_API_IMPL (
      theWitRun,
      witSetPartObj1StockCost, (
         partName,
         obj1StockCost))
   }
 
//------------------------------------------------------------------------------

witReturnCode witSetPartProdBounds (
      WitRun * const theWitRun,
      const char * const partName,
      const float * const hardLower,
      const float * const softLower,
      const float * const hardUpper)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetPartProdBounds, (
         partName,
         hardLower,
         softLower,
         hardUpper))
   }

//------------------------------------------------------------------------------

witReturnCode witSetPartProdVol (
      WitRun * const theWitRun,
      const char * const partName,
      const float * const prodVol)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetPartProdVol, (
         partName,
         prodVol))
   }

//------------------------------------------------------------------------------

witReturnCode witSetPartPropRouting (
      WitRun * const theWitRun,
      const char * const partName,
      const witBoolean propRouting)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetPartPropRouting, (
         partName,
         propRouting))
   }

//------------------------------------------------------------------------------

witReturnCode witSetPartYield (
      WitRun * const theWitRun,
      const char * const partName,
      const int * const yield)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetPartYield, (
         partName,
         yield))
   }

//------------------------------------------------------------------------------

witReturnCode witSetStockRealloc (
      WitRun * const theWitRun,
      const witBoolean stockRealloc)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetStockRealloc, (
         stockRealloc))
   }

//------------------------------------------------------------------------------

witReturnCode witSetSubsBomEntryFallout (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      const int fallout)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetSubsBomEntryFallout, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         fallout))
   }

//------------------------------------------------------------------------------

witReturnCode witSetSubsBomEntryProdVol (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      const float * const prodVol)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetSubsBomEntryProdVol, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         prodVol))
   }

//------------------------------------------------------------------------------

witReturnCode witSetSubsBomEntryUsageRate (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bomEntryIndex,
      const int subsBomEntryIndex,
      const float usageRate)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetSubsBomEntryUsageRate, (
         operationName,
         bomEntryIndex,
         subsBomEntryIndex,
         usageRate))
   }
 
//------------------------------------------------------------------------------

witReturnCode witWriteProdSched (
      WitRun * const theWitRun,
      const char * const fileName,
      const witFileFormat fileFormat)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witWriteProdSched, (
         fileName,
         fileFormat))
   }

//------------------------------------------------------------------------------
// Miscellaneous undocumented functions.
//------------------------------------------------------------------------------

witReturnCode witExecStandAlone (
      WitRun * const     theWitRun,
      const char * const theFileName)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witExecStandAlone, (
         theFileName))
   }

//------------------------------------------------------------------------------

witReturnCode witExhaustMemory (
      WitRun * const theWitRun)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witExhaustMemory, ())
   }

//------------------------------------------------------------------------------

witReturnCode witGetBopEntryMeSelSplit (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bopEntryIndex,
      witBoolean * meSelSplit)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetBopEntryMeSelSplit, (
         operationName,
         bopEntryIndex,
         meSelSplit))
   }

//------------------------------------------------------------------------------

witReturnCode witGetDevMode (
      WitRun * const theWitRun,
      witBoolean *   devMode)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetDevMode, (
         devMode))
   }

//------------------------------------------------------------------------------

witReturnCode witGetNewPipPggOrder (
      WitRun * const theWitRun,
      witBoolean * newPipPggOrder)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetNewPipPggOrder, (
         newPipPggOrder))
   }

//------------------------------------------------------------------------------

witReturnCode witGetNonMrSelSplit (
      WitRun * const theWitRun,
      witBoolean * nonMrSelSplit)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetNonMrSelSplit, (
         nonMrSelSplit))
   }

//------------------------------------------------------------------------------

witReturnCode witGetPartBaSelSplit (
      WitRun * const theWitRun,
      const char * const partName,
      witBoolean * baSelSplit)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetPartBaSelSplit, (
         partName,
         baSelSplit))
   }

//------------------------------------------------------------------------------

witReturnCode witGetSrSelSplit (
      WitRun * const theWitRun,
      witBoolean * srSelSplit)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witGetSrSelSplit, (
         srSelSplit))
   }

//------------------------------------------------------------------------------

witReturnCode witInterpretDevCmd (
      WitRun * const     theWitRun,
      const char * const theDevCmd)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witInterpretDevCmd, (
         theDevCmd))
   }

//------------------------------------------------------------------------------

witReturnCode witIssueFatalMesg (
      WitRun * const theWitRun)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witIssueFatalMesg, ())
   }

//------------------------------------------------------------------------------

witReturnCode witListUnissuedMsgs (
      WitRun * const theWitRun)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witListUnissuedMsgs, ())
   }

//------------------------------------------------------------------------------
// Non-Standard API Function.
//------------------------------------------------------------------------------

witReturnCode witMalloc (
      size_t size,
      void * * mem)
   {
   * mem = malloc (size);

   return
      (* mem != NULL)?
         WitINFORMATIONAL_RC: 
         WitWARNING_RC;
   }

//------------------------------------------------------------------------------

witReturnCode witNoDependantWitRunVeneersDecr (
      WitRun * const theWitRun)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witNoDependantWitRunVeneersDecr, ())
   }

//------------------------------------------------------------------------------

witReturnCode witNoDependantWitRunVeneersIncr (
      WitRun * const theWitRun)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witNoDependantWitRunVeneersIncr, ())
   }

//------------------------------------------------------------------------------

witReturnCode witNoDependantWitRunVeneersIsOne (
      WitRun * const theWitRun,
      int * isOne)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witNoDependantWitRunVeneersIsOne, (
         isOne))
   }

//------------------------------------------------------------------------------

witReturnCode witSetBopEntryMeSelSplit (
      WitRun * const theWitRun,
      const char * const operationName,
      const int bopEntryIndex,
      const witBoolean meSelSplit)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetBopEntryMeSelSplit, (
         operationName,
         bopEntryIndex,
         meSelSplit))
   }

//------------------------------------------------------------------------------

witReturnCode witSetNewPipPggOrder (
      WitRun * const theWitRun,
      const witBoolean newPipPggOrder)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetNewPipPggOrder, (
         newPipPggOrder))
   }

//------------------------------------------------------------------------------

witReturnCode witSetNonMrSelSplit (
      WitRun * const theWitRun,
      const witBoolean nonMrSelSplit)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetNonMrSelSplit, (
         nonMrSelSplit))
   }

//------------------------------------------------------------------------------

witReturnCode witSetPartBaSelSplit (
      WitRun * const theWitRun,
      const char * const partName,
      const witBoolean baSelSplit)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetPartBaSelSplit, (
         partName,
         baSelSplit))
   }

//------------------------------------------------------------------------------

witReturnCode witSetSrSelSplit (
      WitRun * const theWitRun,
      const witBoolean srSelSplit)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetSrSelSplit, (
         srSelSplit))
   }

//------------------------------------------------------------------------------

witReturnCode witSetWit34Allowed (
      WitRun * const theWitRun,
      const witBoolean wit34Allowed)
   {
   STANDARD_API_IMPL (
      theWitRun,
      witSetWit34Allowed, (
         wit34Allowed))
   }

//------------------------------------------------------------------------------

const witAttr       WitSTDOUT                 = "stdout";
const witAttr       WitSTDIN                  = "stdin";

const witAttr       WitRAW                    = "RAW";
const witAttr       WitPRODUCT                = "PRODUCT";
const witAttr       WitCAPACITY               = "CAPACITY";
const witAttr       WitMATERIAL               = "MATERIAL";

const witAttr       WitINACTIVE               = "inactive";
const witAttr       WitAT_PART                = "atPart";
const witAttr       WitAT_DEMAND              = "atDemand";
const witAttr       WitAT_OPERATION           = "atOperation";
const witAttr       WitAT_BOM_ENTRY           = "atBomEntry";
const witAttr       WitAT_SUB_ENTRY           = "atSubEntry";
const witAttr       WitAT_BOP_ENTRY           = "atBopEntry";

const witFileFormat WitBSV                    = "bsv";
const witFileFormat WitCSV                    = "csv";

const witAttr       WitACC_OPT_INIT_METHOD    = "accOptInitMethod";
const witAttr       WitHEUR_OPT_INIT_METHOD   = "heurOptInitMethod";
const witAttr       WitSCHED_OPT_INIT_METHOD  = "schedOptInitMethod";
const witAttr       WitCRASH_OPT_INIT_METHOD  = "crashOptInitMethod";


const int           WitINFORMATIONAL_MESSAGES = -1;
const int           WitWARNING_MESSAGES       = -2;
const int           WitSEVERE_MESSAGES        = -3;
const int           WitFATAL_MESSAGES         = -4;

const witReturnCode WitINFORMATIONAL_RC       =  0;
const witReturnCode WitWARNING_RC             =  1;
const witReturnCode WitSEVERE_RC              =  3;
const witReturnCode WitFATAL_RC               =  4;

//------------------------------------------------------------------------------

   }
   //
   // End of extern "C"

//------------------------------------------------------------------------------
// Implementation of class ErrorExc.
//------------------------------------------------------------------------------

WitErrorExc::WitErrorExc (
         const char *  funcNameVal,
         int           mesgNumVal,
         witReturnCode retCodeVal):

      mesgNum_ (mesgNumVal),
      retCode_ (retCodeVal)
   {
   strncpy (funcName_, funcNameVal, 50);

   funcName_[50] = '\0';
   }

//------------------------------------------------------------------------------

WitErrorExc::WitErrorExc (const WitErrorExc & theErrorExc):

      mesgNum_ (theErrorExc.mesgNum_),
      retCode_ (theErrorExc.retCode_)
   {
   strcpy (funcName_, theErrorExc.funcName_);
   }

//------------------------------------------------------------------------------

WitErrorExc::~WitErrorExc ()
   {
   }

//------------------------------------------------------------------------------

const char * WitErrorExc::funcName () const
   {
   return funcName_;
   }

//------------------------------------------------------------------------------

int WitErrorExc::mesgNum () const
   {
   return mesgNum_;
   }

//------------------------------------------------------------------------------

witReturnCode WitErrorExc::retCode () const
   {
   return retCode_;
   }
