//------------------------------------------------------------------------------
// WIT-J C++ Source file ProblemCOR.C.
//
// Contains the C++ implementations of the native methods of Java class
// witj.ProblemCOR.
//------------------------------------------------------------------------------

#include     <NativeMacros.h>
#include <ExceptionHandler.h>
#include       <JniHandler.h>
#include          <Problem.h>
#include      <Coordinator.h>
#include     <ArrayListJOR.h>
#include        <StringJOR.h>

using namespace WitJ;

//------------------------------------------------------------------------------

NATIVE_METHOD (void, ProblemCOR, callSimpleWitFunc) (
      JNIEnv *   theJNIEnv,
      _jobject * theProblemCORJObj,
      _jobject * theFuncCTCORJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler  theJniHandler (theJNIEnv);
   Problem *   theProblem;
   CaseToken * theFuncCT;

   theProblem = theJniHandler.getJavaAccObj (theProblemCORJObj)->asProblem   ();
   theFuncCT  = theJniHandler.getJavaAccObj (theFuncCTCORJObj) ->asCaseToken ();

   theProblem->callSimpleWitFunc (theFuncCT);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, ProblemCOR, writeSched) (
      JNIEnv *   theJNIEnv,
      _jobject * theProblemCORJObj,
      _jobject * theWriteSchedCTCORJObj,
      _jstring * fileNameJStr,
      _jobject * theFormatCTCORJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler   theJniHandler (theJNIEnv);
   Problem *    theProblem;
   CaseToken *  theWriteSchedCT;
   const char * fileNameCStr;
   CaseToken *  theFormatCT;

   theProblem = theJniHandler.getJavaAccObj (theProblemCORJObj)->asProblem ();

   theWriteSchedCT =
      theJniHandler.getJavaAccObj (theWriteSchedCTCORJObj)->asCaseToken ();

   fileNameCStr = theJniHandler.getCString (fileNameJStr);

   theFormatCT =
      theJniHandler.getJavaAccObj (theFormatCTCORJObj)->asCaseToken ();

   theProblem->writeSched (theWriteSchedCT, fileNameCStr, theFormatCT);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, ProblemCOR, performAction) (
      JNIEnv *   theJNIEnv,
      _jobject * theProblemCORJObj,
      _jstring * theCommandJStr)
   {
   BEGIN_TRY_BLOCK

   JniHandler   theJniHandler (theJNIEnv);
   Problem *    theProblem;
   const char * theCommandCStr;

   theProblem = theJniHandler.getJavaAccObj (theProblemCORJObj)->asProblem ();

   theCommandCStr = theJniHandler.getCString (theCommandJStr);

   theProblem->performAction (theCommandCStr);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (jboolean, ProblemCOR, throwUnexpException) (
      JNIEnv *   theJNIEnv,
      _jobject * theProblemCORJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler theJniHandler (theJNIEnv);
   Problem *  theProblem;

   theProblem = theJniHandler.getJavaAccObj (theProblemCORJObj)->asProblem ();

   return theProblem->throwUnexpException ();

   END_TRY_BLOCK_VALUE_RETURN (theJNIEnv, false)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, ProblemCOR, addDblCplexParSpec) (
      JNIEnv *   theJNIEnv,
      _jobject * theProblemCORJObj,
      _jstring * theNameJStr,
      jdouble    theValue)
   {
   BEGIN_TRY_BLOCK

   JniHandler   theJniHandler (theJNIEnv);
   Problem *    theProblem;
   const char * theNameCStr;

   theProblem  = theJniHandler.getJavaAccObj (theProblemCORJObj)->asProblem ();
   theNameCStr = theJniHandler.getCString    (theNameJStr);

   theProblem->addDblCplexParSpec (theNameCStr, theValue);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, ProblemCOR, addIntCplexParSpec) (
      JNIEnv *   theJNIEnv,
      _jobject * theProblemCORJObj,
      _jstring * theNameJStr,
      jint       theValue)
   {
   BEGIN_TRY_BLOCK

   JniHandler   theJniHandler (theJNIEnv);
   Problem *    theProblem;
   const char * theNameCStr;

   theProblem  = theJniHandler.getJavaAccObj (theProblemCORJObj)->asProblem ();
   theNameCStr = theJniHandler.getCString    (theNameJStr);

   theProblem->addIntCplexParSpec (theNameCStr, theValue);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, ProblemCOR, copyData) (
      JNIEnv *   theJNIEnv,
      _jobject * dupProblemCORJObj,
      _jobject * origProblemCORJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler theJniHandler (theJNIEnv);
   Problem *   dupProblem;
   Problem *  origProblem;

    dupProblem = theJniHandler.getJavaAccObj  (dupProblemCORJObj)->asProblem ();
   origProblem = theJniHandler.getJavaAccObj (origProblemCORJObj)->asProblem ();

   dupProblem->copyData (origProblem);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, ProblemCOR, displayData) (
      JNIEnv *   theJNIEnv,
      _jobject * theProblemCORJObj,
      _jstring * fileNameJStr)
   {
   BEGIN_TRY_BLOCK

   JniHandler   theJniHandler (theJNIEnv);
   Problem *    theProblem;
   const char * fileNameCStr;

   theProblem   = theJniHandler.getJavaAccObj (theProblemCORJObj)->asProblem ();
   fileNameCStr = theJniHandler.getCString    (fileNameJStr);

   theProblem->displayData (fileNameCStr);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, ProblemCOR, eqHeurAlloc) (
      JNIEnv *   theJNIEnv,
      _jobject * theProblemCORJObj,
      _jobject * theDemandListJObj,
      _jobject * shipPeriodListJObj,
      _jobject * desIncVolListJObj,
      _jobject * incVolListJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler  theJniHandler                       (theJNIEnv);
   Problem *   theProblem;
   DemandALJOR theDemandALJOR  (theDemandListJObj,  theJNIEnv);
   IntALJOR    shipPeriodALJOR (shipPeriodListJObj, theJNIEnv);
   DblALJOR    desIncVolALJOR  (desIncVolListJObj,  theJNIEnv);
   DblALJOR    incVolALJOR     (incVolListJObj,     theJNIEnv);

   theProblem = theJniHandler.getJavaAccObj (theProblemCORJObj)->asProblem ();

   theProblem->
      eqHeurAlloc (
         theDemandALJOR,
         shipPeriodALJOR,
         desIncVolALJOR,
         incVolALJOR);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, ProblemCOR, getCriticalList) (
      JNIEnv *   theJNIEnv,
      _jobject * theProblemCORJObj,
      _jobject * critPartListJObj,
      _jobject * critPeriodListJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler       theJniHandler                        (theJNIEnv);
   Problem *        theProblem;
   CompALJOR <Part> critPartALJOR    (critPartListJObj,   theJNIEnv);
   IntALJOR         critPeriodALJOR  (critPeriodListJObj, theJNIEnv);

   theProblem = theJniHandler.getJavaAccObj (theProblemCORJObj)->asProblem ();

   theProblem->getCriticalList (critPartALJOR, critPeriodALJOR);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (_jobject *, ProblemCOR, getDblCplexParSpec) (
      JNIEnv *   theJNIEnv,
      _jobject * theProblemCORJObj,
      _jstring * theNameJStr)
   {
   BEGIN_TRY_BLOCK

   JniHandler   theJniHandler (theJNIEnv);
   Problem *    theProblem;
   const char * theNameCStr;
   bool         dblSpecExists;
   double       theValue;

   theProblem  = theJniHandler.getJavaAccObj (theProblemCORJObj)->asProblem ();
   theNameCStr = theJniHandler.getCString    (theNameJStr);

   theProblem->getDblCplexParSpec (theNameCStr, dblSpecExists, theValue);

   if (dblSpecExists)
      return JniClient::matchingDoubleJObj (theValue, theJNIEnv);
   else
      return NULL;

   END_TRY_BLOCK_VALUE_RETURN (theJNIEnv, NULL)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, ProblemCOR, getExpCycle) (
      JNIEnv *   theJNIEnv,
      _jobject * theProblemCORJObj,
      _jobject * thePartListJObj,
      _jobject * theOpnListJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler            theJniHandler                 (theJNIEnv);
   Problem *             theProblem;
   CompALJOR <Part>      thePartALJOR (thePartListJObj, theJNIEnv);
   CompALJOR <Operation> theOpnALJOR  (theOpnListJObj,  theJNIEnv);

   theProblem = theJniHandler.getJavaAccObj (theProblemCORJObj)->asProblem ();

   theProblem->getExpCycle (thePartALJOR, theOpnALJOR);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (_jobject *, ProblemCOR, getIntCplexParSpec) (
      JNIEnv *   theJNIEnv,
      _jobject * theProblemCORJObj,
      _jstring * theNameJStr)
   {
   BEGIN_TRY_BLOCK

   JniHandler   theJniHandler (theJNIEnv);
   Problem *    theProblem;
   const char * theNameCStr;
   bool         dblSpecExists;
   int          theValue;

   theProblem  = theJniHandler.getJavaAccObj (theProblemCORJObj)->asProblem ();
   theNameCStr = theJniHandler.getCString    (theNameJStr);

   theProblem->getIntCplexParSpec (theNameCStr, dblSpecExists, theValue);

   if (dblSpecExists)
      return JniClient::matchingIntegerJObj (theValue, theJNIEnv);
   else
      return NULL;

   END_TRY_BLOCK_VALUE_RETURN (theJNIEnv, NULL)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, ProblemCOR, getObjectiveList) (
      JNIEnv *   theJNIEnv,
      _jobject * theProblemCORJObj,
      _jobject * theObjNameListJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler  theJniHandler                        (theJNIEnv);
   Problem *   theProblem;
   StringALJOR theObjNameALJOR  (theObjNameListJObj, theJNIEnv);

   theProblem = theJniHandler.getJavaAccObj (theProblemCORJObj)->asProblem ();

   theProblem->getObjectiveList (theObjNameALJOR);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (_jstring *, ProblemCOR, getOptInitMethodName) (
      JNIEnv *   theJNIEnv,
      _jobject * theProblemCORJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler theJniHandler     (theJNIEnv);
   Problem *  theProblem;
   StringJOR  theMethodNameSJOR (theJNIEnv);

   theProblem = theJniHandler.getJavaAccObj (theProblemCORJObj)->asProblem ();

   theProblem->getOptInitMethodName (theMethodNameSJOR);

   return theMethodNameSJOR.getJString ();

   END_TRY_BLOCK_VALUE_RETURN (theJNIEnv, NULL)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, ProblemCOR, getPgdCritList) (
      JNIEnv *   theJNIEnv,
      _jobject * theProblemCORJObj,
      _jobject * critPartListJObj,
      _jobject * critPeriodListJObj,
      _jobject * theDemandListJObj,
      _jobject * shipPeriodListJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler         theJniHandler                       (theJNIEnv);
   Problem *          theProblem;
   CompALJOR <Part>   critPartALJOR   (critPartListJObj,   theJNIEnv);
   IntALJOR           critPeriodALJOR (critPeriodListJObj, theJNIEnv);
   CompALJOR <Demand> theDemandALJOR  (theDemandListJObj,  theJNIEnv);
   IntALJOR           shipPeriodALJOR (shipPeriodListJObj, theJNIEnv);

   theProblem = theJniHandler.getJavaAccObj (theProblemCORJObj)->asProblem ();

   theProblem->
      getPgdCritList (
         critPartALJOR,
         critPeriodALJOR,
         theDemandALJOR,
         shipPeriodALJOR);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, ProblemCOR, getPipSeq) (
      JNIEnv *   theJNIEnv,
      _jobject * theProblemCORJObj,
      _jobject * theDemandTripListJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler         theJniHandler                           (theJNIEnv);
   Problem *          theProblem;
   PeggingTripleALJOR theDemandATALJOR (theDemandTripListJObj, theJNIEnv);

   theProblem = theJniHandler.getJavaAccObj (theProblemCORJObj)->asProblem ();

   theProblem->getPipSeq (theDemandATALJOR);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, ProblemCOR, getSortedOperations) (
      JNIEnv *   theJNIEnv,
      _jobject * theProblemCORJObj,
      _jobject * theOpnListJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler            theJniHandler               (theJNIEnv);
   Problem *             theProblem;
   CompALJOR <Operation> theOpnALJOR (theOpnListJObj, theJNIEnv);

   theProblem = theJniHandler.getJavaAccObj (theProblemCORJObj)->asProblem ();

   theProblem->getSortedOperations (theOpnALJOR);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, ProblemCOR, getSortedParts) (
      JNIEnv *   theJNIEnv,
      _jobject * theProblemCORJObj,
      _jobject * thePartListJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler       theJniHandler                 (theJNIEnv);
   Problem *        theProblem;
   CompALJOR <Part> thePartALJOR (thePartListJObj, theJNIEnv);

   theProblem = theJniHandler.getJavaAccObj (theProblemCORJObj)->asProblem ();

   theProblem->getSortedParts (thePartALJOR);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, ProblemCOR, readData) (
      JNIEnv *   theJNIEnv,
      _jobject * theProblemCORJObj,
      _jstring * fileNameJStr)
   {
   BEGIN_TRY_BLOCK

   JniHandler   theJniHandler (theJNIEnv);
   Problem *    theProblem;
   const char * fileNameCStr;

   theProblem   = theJniHandler.getJavaAccObj (theProblemCORJObj)->asProblem ();
   fileNameCStr = theJniHandler.getCString    (fileNameJStr);

   theProblem->readData (fileNameCStr);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, ProblemCOR, setNPeriods) (
      JNIEnv *   theJNIEnv,
      _jobject * theProblemCORJObj,
      jint       theNPeriodsJInt)
   {
   BEGIN_TRY_BLOCK

   JniHandler theJniHandler (theJNIEnv);
   Problem *  theProblem;

   theProblem = theJniHandler.getJavaAccObj (theProblemCORJObj)->asProblem ();

   theProblem->setNPeriods (theNPeriodsJInt);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, ProblemCOR, setObjectiveList) (
      JNIEnv *   theJNIEnv,
      _jobject * theProblemCORJObj,
      _jobject * theObjNameListJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler  theJniHandler                        (theJNIEnv);
   Problem *   theProblem;
   StringALJOR theObjNameALJOR  (theObjNameListJObj, theJNIEnv);

   theProblem = theJniHandler.getJavaAccObj (theProblemCORJObj)->asProblem ();

   theProblem->setObjectiveList (theObjNameALJOR);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, ProblemCOR, setOptInitMethod) (
      JNIEnv *   theJNIEnv,
      _jobject * theProblemCORJObj,
      _jobject * theMethodCTCORJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler  theJniHandler (theJNIEnv);
   Problem *   theProblem;
   CaseToken * theMethodCT;

   theProblem  = theJniHandler.getJavaAccObj (theProblemCORJObj)->asProblem ();

   theMethodCT =
      theJniHandler.getJavaAccObj (theMethodCTCORJObj)->asCaseToken ();

   theProblem->setOptInitMethod (theMethodCT);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, ProblemCOR, writeCriticalList) (
      JNIEnv *   theJNIEnv,
      _jobject * theProblemCORJObj,
      _jstring * fileNameJStr,
      _jobject * theFormatCTCORJObj,
      jint       maxListLen)
   {
   BEGIN_TRY_BLOCK

   JniHandler   theJniHandler (theJNIEnv);
   Problem *    theProblem;
   const char * fileNameCStr;
   CaseToken *  theFormatCT;

   theProblem   = theJniHandler.getJavaAccObj (theProblemCORJObj)->asProblem ();
   fileNameCStr = theJniHandler.getCString    (fileNameJStr);

   theFormatCT  =
      theJniHandler.getJavaAccObj (theFormatCTCORJObj)->asCaseToken ();

   theProblem->writeCriticalList (fileNameCStr, theFormatCT, maxListLen);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, ProblemCOR, writeData) (
      JNIEnv *   theJNIEnv,
      _jobject * theProblemCORJObj,
      _jstring * fileNameJStr)
   {
   BEGIN_TRY_BLOCK

   JniHandler   theJniHandler (theJNIEnv);
   Problem *    theProblem;
   const char * fileNameCStr;

   theProblem   = theJniHandler.getJavaAccObj (theProblemCORJObj)->asProblem ();
   fileNameCStr = theJniHandler.getCString    (fileNameJStr);

   theProblem->writeData (fileNameCStr);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, ProblemCOR, createAndStoreCppProblem) (
      JNIEnv *   theJNIEnv,
      _jobject * theProblemCORJObj,
      _jobject * theCoordCORJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler    theJniHandler (theJNIEnv);
   Coordinator * theCoord;
   Problem *     theProblem;

   theCoord   = theJniHandler.getJavaAccObj (theCoordCORJObj)->asCoordinator ();

   theProblem = new Problem (theCoord);

   theJniHandler.storeInCOR (theProblemCORJObj, theProblem);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }
