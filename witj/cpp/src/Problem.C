//------------------------------------------------------------------------------
// WIT-J C++ Source File Problem.C.
//
// Contains the implementation of class Problem.
//------------------------------------------------------------------------------

#include          <Problem.h>
#include      <Coordinator.h>
#include           <Demand.h>
#include        <CaseToken.h>
#include        <StringVec.h>
#include <PreJavaException.h>
#include     <ComponentJOR.h>
#include        <StringJOR.h>
#include     <ArrayListJOR.h>

#include         <iostream>
#include           <string>
#include           <string.h>

//------------------------------------------------------------------------------

WitJ::Problem::Problem (Coordinator * theCoord):

      Component (theCoord),
      nPeriods_ (0)
   {
   clearData ();

   theCoord->store (this);
   }

//------------------------------------------------------------------------------

WitJ::Problem::~Problem ()
   {
   }

//------------------------------------------------------------------------------

WitJ::Problem * WitJ::Problem::asProblem ()
   {
   return this;
   }

//------------------------------------------------------------------------------

void WitJ::Problem::copyComponentData (Component *)
   {
   EXCEPTION_ASSERT (false);
   }

//------------------------------------------------------------------------------

void WitJ::Problem::callSimpleWitFunc (CaseToken * theFuncCT)
   {
   SimpleWitFunc theFunc;

   theFunc = theFuncCT->getSimpleWitFunc ();

   theFunc (getWitRun ());
   }

//------------------------------------------------------------------------------

void WitJ::Problem::writeSched (
      CaseToken *  theSchedIDCT,
      const char * fileName,
      CaseToken *  theFormatCT)
   {
   WriteSchedFunc theFunc;
   witFileFormat  theFormat;

   theFunc = theSchedIDCT->getWriteSchedFunc ();

   if (strlen (fileName) == 0)
      fileName = NULL;

   theFormat = theFormatCT->getWitFileFormat ();

   theFunc (getWitRun (), fileName, theFormat);
   }

//------------------------------------------------------------------------------

void WitJ::Problem::setNPeriods (int theInt)
   {
   nPeriods_ = theInt;

   if (! extractInProgress ())
      witSetNPeriods (getWitRun (), theInt);
   }

//------------------------------------------------------------------------------

int WitJ::Problem::getNPeriods ()
   {
   return nPeriods_;
   }

//------------------------------------------------------------------------------

void WitJ::Problem::clearData ()
   {
   nPeriods_ = 26;
   }

//------------------------------------------------------------------------------

void WitJ::Problem::performAction (const char * theCommand)
   {
   std::string theCmdStr;

   theCmdStr = theCommand;

   if (theCmdStr == "fail-c++-exception-assert")
      {
      copyComponentData (NULL);
      }

   else if (theCmdStr == "fail-c++-aborting-assert")
      {
      ABORTING_ASSERT (false);
      }

   else if (theCmdStr == "exhaust-memory-in-witj-c++")
      {
      exhaustMemory ();
      }

   else if (theCmdStr == "trigger-jni-exception")
      {
      getComponentJOR ()->findNonsense ();
      }

   else if (theCmdStr == "exhaust-memory-in-jni")
      {
      getComponentJOR ()->exhaustJniMemory ();
      }

   else if (theCmdStr == "exhaust-memory-in-wit")
      {
      witExhaustMemory (getWitRun ());
      }

   else if (theCmdStr == "trigger-wit-fatal-message")
      {
      witIssueFatalMesg (getWitRun ());
      }

   else
      {
      EXCEPTION_ASSERT (false);
      }
   }

//------------------------------------------------------------------------------

bool WitJ::Problem::throwUnexpException ()
   {
   throw 7;
   }

//------------------------------------------------------------------------------

void WitJ::Problem::addDblCplexParSpec (const char * theName, double theValue)
   {
   witAddDblCplexParSpecDbl (getWitRun (), theName, theValue);
   }

//------------------------------------------------------------------------------

void WitJ::Problem::addIntCplexParSpec (const char * theName, int theValue)
   {
   witAddIntCplexParSpec (getWitRun (), theName, theValue);
   }

//------------------------------------------------------------------------------

void WitJ::Problem::copyData (Problem * origProblem)
   {
   witCopyData (
                   getWitRun (),
      origProblem->getWitRun ());

   getCoordinator ()->extract (false);
   }

//------------------------------------------------------------------------------

void WitJ::Problem::displayData (const char * fileName)
   {
   if (strlen (fileName) == 0)
      fileName = NULL;

   witDisplayData (getWitRun (), fileName);
   }

//------------------------------------------------------------------------------

void WitJ::Problem::eqHeurAlloc (
      DemandALJOR & theDemandALJOR,
      IntALJOR &    shipPeriodALJOR,
      DblALJOR &    desIncVolALJOR,
      DblALJOR &    incVolALJOR)
   {
   std::vector <const char *> thePartNameVec;
   std::vector <const char *> theDemandNameVec;
   std::vector <int>          shipPeriodVec;
   std::vector <double>       desIncVolVec;
   double *                   incVolVec;

   theDemandALJOR .populateIDVecs (thePartNameVec, theDemandNameVec);
   shipPeriodALJOR.copyInto       (shipPeriodVec);
   desIncVolALJOR .copyInto       (desIncVolVec);

   witEqHeurAllocDbl (
      getWitRun (),
      thePartNameVec.size (),
    & thePartNameVec  [0],
    & theDemandNameVec[0],
    & shipPeriodVec   [0],
    & desIncVolVec    [0],
    & incVolVec);

   incVolALJOR.copyFrom (incVolVec, thePartNameVec.size ());

   witFree (incVolVec);
   }

//------------------------------------------------------------------------------

void WitJ::Problem::getCriticalList (
      CompALJOR <Part> & critPartALJOR,
      IntALJOR &         critPeriodALJOR)
   {
   int      lenCritList;
   char * * critPartList;
   int *    critPeriod;

   witGetCriticalList (
      getWitRun (),
    & lenCritList,
    & critPartList,
    & critPeriod);

   getCoordinator ()->populate (critPartALJOR, critPartList, lenCritList);

   critPeriodALJOR.   copyFrom                (critPeriod,   lenCritList);

   freeWitStrVec (critPartList, lenCritList);
   witFree       (critPeriod);
   }

//------------------------------------------------------------------------------

void WitJ::Problem::getDblCplexParSpec (
      const char * theName,
      bool &       dblSpecExists,
      double &     dblValue)
   {
   witBoolean theWitBool;

   witGetDblCplexParSpecDbl (
      getWitRun (),
      theName,
    & theWitBool,
    & dblValue);

   dblSpecExists = theWitBool;
   }

//------------------------------------------------------------------------------

void WitJ::Problem::getExpCycle (
      CompALJOR <Part> &      thePartALJOR,
      CompALJOR <Operation> & theOpnALJOR)
   {
   int      lenLists;
   char * * partNameList;
   char * * opnNameList;

   witGetExpCycle (getWitRun (), & lenLists, & partNameList, & opnNameList);

   getCoordinator ()->populate (thePartALJOR, partNameList, lenLists);
   getCoordinator ()->populate (theOpnALJOR,  opnNameList,  lenLists);

   freeWitStrVec (partNameList, lenLists);
   freeWitStrVec (opnNameList,  lenLists);
   }

//------------------------------------------------------------------------------

void WitJ::Problem::getIntCplexParSpec (
      const char * theName,
      bool &       intSpecExists,
      int &        intValue)
   {
   witBoolean theWitBool;

   witGetIntCplexParSpec (
      getWitRun (),
      theName,
    & theWitBool,
    & intValue);

   intSpecExists = theWitBool;
   }

//------------------------------------------------------------------------------

void WitJ::Problem::getObjectiveList (StringALJOR & theObjNameALJOR)
   {
   int      theLength;
   char * * theObjNameCVec;

   witGetObjectiveList (
      getWitRun (),
    & theLength,
    & theObjNameCVec);

   theObjNameALJOR.copyFrom (theObjNameCVec, theLength);

   freeWitStrVec (theObjNameCVec, theLength);
   }

//------------------------------------------------------------------------------

void WitJ::Problem::getOptInitMethodName (StringJOR & theMethodNameSJOR)
   {
   witAttr      theMethodAttr;
   const char * theMethodNameCStr;

   witGetOptInitMethod (getWitRun (), & theMethodAttr);

   theMethodNameCStr =
      (theMethodAttr ==   WitACC_OPT_INIT_METHOD)? "ACCELERATED":
      (theMethodAttr ==  WitHEUR_OPT_INIT_METHOD)?   "HEURISTIC":
      (theMethodAttr == WitSCHED_OPT_INIT_METHOD)?    "SCHEDULE":
      (theMethodAttr == WitCRASH_OPT_INIT_METHOD)?       "CRASH":
                                                       "INVALID";

   theMethodNameSJOR.makeJavaString (theMethodNameCStr);
   }

//------------------------------------------------------------------------------

void WitJ::Problem::getPgdCritList (
      CompALJOR <Part> &   critPartALJOR,
      IntALJOR &           critPeriodALJOR,
      CompALJOR <Demand> & theDemandALJOR,
      IntALJOR &           shipPeriodALJOR)
   {
   int      lenLists;
   char * * critPartNameList;
   int *    critPerList;
   char * * demPartNameList;
   char * * demandNameList;
   int *    shipPerList;

   witGetPgdCritList (
      getWitRun (),
    & lenLists,
    & critPartNameList,
    & critPerList,
    & demPartNameList,
    & demandNameList,
    & shipPerList);

   getCoordinator ()->populate (critPartALJOR,  critPartNameList, lenLists);

   critPeriodALJOR.   copyFrom                 (critPerList,      lenLists);

   getCoordinator ()->populate (theDemandALJOR, demPartNameList,
                                                demandNameList,   lenLists);

   shipPeriodALJOR.   copyFrom                 (shipPerList,      lenLists);

   freeWitStrVec (critPartNameList, lenLists);
   witFree       (critPerList);
   freeWitStrVec (demPartNameList,  lenLists);
   freeWitStrVec (demandNameList,   lenLists);
   witFree       (shipPerList);
   }

//------------------------------------------------------------------------------

void WitJ::Problem::getPipSeq (PeggingTripleALJOR & theDemandPTALJOR)
   {
   int      nTriples;
   char * * partNameCVec;
   char * * demandNameCVec;
   int *    thePeriodCVec;
   double * theVolumeCVec;
   int      theIdx;
   Demand * theDemand;

   witGetPipSeqDbl (
      getWitRun (),
    & nTriples,
    & partNameCVec,
    & demandNameCVec,
    & thePeriodCVec,
    & theVolumeCVec);

   theDemandPTALJOR.clearAndAllocate (nTriples);

   for (theIdx = 0; theIdx < nTriples; theIdx ++)
      {
      theDemand =
         getCoordinator ()->
            findDemand (
               partNameCVec  [theIdx],
               demandNameCVec[theIdx]);

      theDemandPTALJOR.
         add (
            theDemand,
            thePeriodCVec[theIdx],
            theVolumeCVec[theIdx]);
      }

   freeWitStrVec (partNameCVec,   nTriples);
   freeWitStrVec (demandNameCVec, nTriples);

   witFree       (thePeriodCVec);
   witFree       (theVolumeCVec);
   }

//------------------------------------------------------------------------------

void WitJ::Problem::getSortedOperations (CompALJOR <Operation> & theOpnALJOR)
   {
   int      lenList;
   char * * opnNameList;

   witPreprocess (getWitRun ());

   witGetOperations (getWitRun (), & lenList, & opnNameList);

   getCoordinator ()->populate (theOpnALJOR, opnNameList, lenList);

   freeWitStrVec (opnNameList, lenList);
   }

//------------------------------------------------------------------------------

void WitJ::Problem::getSortedParts (CompALJOR <Part> & thePartALJOR)
   {
   int      lenList;
   char * * partNameList;

   witPreprocess (getWitRun ());

   witGetParts (getWitRun (), & lenList, & partNameList);

   getCoordinator ()->populate (thePartALJOR, partNameList, lenList);

   freeWitStrVec (partNameList, lenList);
   }

//------------------------------------------------------------------------------

void WitJ::Problem::readData (const char * fileName)
   {
   try
      {
      witReadData (getWitRun (), fileName);
      }

   catch (WitErrorExc & theWitErrorExc)
      {
      throw PreJavaException ("ReadDataException", "");
      }

   getCoordinator ()->extract (true);
   }

//------------------------------------------------------------------------------

void WitJ::Problem::setObjectiveList (StringALJOR & theObjNameALJOR)
   {
   StringVec theObjNameVec;

   theObjNameALJOR.copyInto (theObjNameVec);

   witSetObjectiveList (
      getWitRun                    (),
      theObjNameVec.getLength      (),
      theObjNameVec.getCStringCVec ());
   }

//------------------------------------------------------------------------------

void WitJ::Problem::setOptInitMethod (CaseToken * theMethodCT)
   {
   witAttr theMethod;

   theMethod = theMethodCT->getWitAttr ();

   witSetOptInitMethod (getWitRun (), theMethod);
   }

//------------------------------------------------------------------------------

void WitJ::Problem::writeCriticalList (
      const char * fileName,
      CaseToken *  theFormatCT,
      int          maxListLen)
   {
   witFileFormat theFormat;

   if (strlen (fileName) == 0)
      fileName = NULL;

   theFormat = theFormatCT->getWitFileFormat ();

   witWriteCriticalList (getWitRun (), fileName, theFormat, maxListLen);
   }

//------------------------------------------------------------------------------

void WitJ::Problem::writeData (const char * fileName)
   {
   if (strlen (fileName) == 0)
      fileName = NULL;

   witWriteData (getWitRun (), fileName);
   }

//------------------------------------------------------------------------------

void WitJ::Problem::exhaustMemory ()
   {
   int totDiscarded;

   for (totDiscarded = 100; true; totDiscarded += 100)
      {
      new char[104857600];

      std::cout
         << totDiscarded << "MB of memory have been allocated by WIT-J/C++.\n";

      std::cout.flush ();
      }
   }
