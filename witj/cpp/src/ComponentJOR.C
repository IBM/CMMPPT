//------------------------------------------------------------------------------
// WIT-J C++ Source File ComponentJOR.C.
//
// Contains the implementation of class ComponentJOR.
//------------------------------------------------------------------------------

#include <ComponentJOR.h>
#include      <Problem.h>
#include         <Part.h>
#include    <Operation.h>
#include     <BomEntry.h>

#include     <iostream>

//------------------------------------------------------------------------------

_jclass *    WitJ::ComponentJOR::                ourPartJCl_ = NULL;
_jclass *    WitJ::ComponentJOR::              ourDemandJCl_ = NULL;
_jclass *    WitJ::ComponentJOR::           ourOperationJCl_ = NULL;
_jclass *    WitJ::ComponentJOR::            ourBomEntryJCl_ = NULL;
_jclass *    WitJ::ComponentJOR::          ourSubstituteJCl_ = NULL;
_jclass *    WitJ::ComponentJOR::            ourBopEntryJCl_ = NULL;

_jmethodID * WitJ::ComponentJOR::     makeInstancePartJMeth_ = NULL;
_jmethodID * WitJ::ComponentJOR::    newInstanceDemandJMeth_ = NULL;
_jmethodID * WitJ::ComponentJOR:: newInstanceOperationJMeth_ = NULL;
_jmethodID * WitJ::ComponentJOR::  newInstanceBomEntryJMeth_ = NULL;
_jmethodID * WitJ::ComponentJOR::newInstanceSubstituteJMeth_ = NULL;
_jmethodID * WitJ::ComponentJOR::  newInstanceBopEntryJMeth_ = NULL;
_jmethodID * WitJ::ComponentJOR::          setNPeriodsJMeth_ = NULL;

//------------------------------------------------------------------------------

WitJ::ComponentJOR::ComponentJOR (_jobject * theCompJObj, JNIEnv * theJNIEnv):

      JniClient        (theJNIEnv),
      myComponentJObj_ (NULL)
   {
   myComponentJObj_ = getJNIEnv ()->NewGlobalRef (theCompJObj);

   EXCEPTION_ASSERT (myComponentJObj_ != NULL);
   }

//------------------------------------------------------------------------------

WitJ::ComponentJOR::~ComponentJOR ()
   {
   getJNIEnv ()->DeleteGlobalRef (myComponentJObj_);
   }

//------------------------------------------------------------------------------

void WitJ::ComponentJOR::setUpJni (JNIEnv * theJNIEnv)
   {
   findAllJClasses   (theJNIEnv);
   findAllJMethodIDs (theJNIEnv);
   }

//------------------------------------------------------------------------------

void WitJ::ComponentJOR::makeJavaPart (
      Problem *    theProblem,
      const char * thePartNameCStr,
      bool         isMaterial)
   {
   ComponentJOR * theCompJOR;
   _jobject *     theProblemJObj;
   _jstring *     thePartNameJStr;

   theCompJOR      = theProblem->getComponentJOR ();
   theProblemJObj  = theProblem->getComponentJOR ()->myComponentJObj_;

   thePartNameJStr = theCompJOR->newJString (thePartNameCStr);

   theCompJOR->
      getJNIEnv ()->
         CallStaticObjectMethod (
            ourPartJCl_,
            makeInstancePartJMeth_,
            theProblemJObj,
            thePartNameJStr,
            isMaterial);

   theCompJOR->throwAnyJavaException ();

   theCompJOR->deleteLocalRef (thePartNameJStr);
   }

//------------------------------------------------------------------------------

void WitJ::ComponentJOR::makeJavaDemand (
      Part *       thePart,
      const char * theDemandNameCStr)
   {
   ComponentJOR * theCompJOR;
   _jobject *     thePartJObj;
   _jstring *     theDemandNameJStr;
   _jobject *     theDemandJObj;

   theCompJOR        = thePart->getComponentJOR ();
   thePartJObj       = thePart->getComponentJOR ()->myComponentJObj_;
   
   theDemandNameJStr = theCompJOR->newJString (theDemandNameCStr);

   theDemandJObj     =
      theCompJOR->
         getJNIEnv ()->
            CallStaticObjectMethod (
               ourDemandJCl_,
               newInstanceDemandJMeth_,
               thePartJObj,
               theDemandNameJStr);

   theCompJOR->throwAnyJavaException ();

   theCompJOR->deleteLocalRef (theDemandNameJStr);
   theCompJOR->deleteLocalRef (theDemandJObj);
   }

//------------------------------------------------------------------------------

void WitJ::ComponentJOR::makeJavaOperation (
      Problem *    theProblem,
      const char * theOpnNameCStr)
   {
   ComponentJOR * theCompJOR;
   _jobject *     theProblemJObj;
   _jstring *     theOpnNameJStr;
   _jobject *     theOpnJObj;

   theCompJOR       = theProblem->getComponentJOR ();
   theProblemJObj   = theProblem->getComponentJOR ()->myComponentJObj_;
   
   theOpnNameJStr   = theCompJOR->newJString (theOpnNameCStr);

   theOpnJObj =
      theCompJOR->
         getJNIEnv ()->
            CallStaticObjectMethod (
               ourOperationJCl_,
               newInstanceOperationJMeth_,
               theProblemJObj,
               theOpnNameJStr);

   theCompJOR->throwAnyJavaException ();

   theCompJOR->deleteLocalRef (theOpnNameJStr);
   theCompJOR->deleteLocalRef (theOpnJObj);
   }

//------------------------------------------------------------------------------

void WitJ::ComponentJOR::makeJavaBomEntry (Operation * theOpn, Part * thePart)
   {
   ComponentJOR * theCompJOR;
   _jobject *     theOpnJObj;
   _jobject *     thePartJObj;
   _jobject *     theBomEntJObj;

   theCompJOR    = theOpn ->getComponentJOR ();
   theOpnJObj    = theOpn ->getComponentJOR ()->myComponentJObj_;
   thePartJObj   = thePart->getComponentJOR ()->myComponentJObj_;
   
   theBomEntJObj =
      theCompJOR->
         getJNIEnv ()->
            CallStaticObjectMethod (
               ourBomEntryJCl_,
               newInstanceBomEntryJMeth_,
               theOpnJObj,
               thePartJObj);

   theCompJOR->throwAnyJavaException ();

   theCompJOR->deleteLocalRef (theBomEntJObj);
   }

//------------------------------------------------------------------------------

void WitJ::ComponentJOR::makeJavaSubstitute (
      BomEntry * theBomEnt,
      Part *     thePart)
   {   
   ComponentJOR * theCompJOR;
   _jobject *     theBomEntJObj;
   _jobject *     thePartJObj;
   _jobject *     theSubJObj;

   theCompJOR    = theBomEnt->getComponentJOR ();
   theBomEntJObj = theBomEnt->getComponentJOR ()->myComponentJObj_;
   thePartJObj   = thePart  ->getComponentJOR ()->myComponentJObj_;

   theSubJObj    =
      theCompJOR->
         getJNIEnv ()->
            CallStaticObjectMethod (
               ourSubstituteJCl_,
               newInstanceSubstituteJMeth_,
               theBomEntJObj,
               thePartJObj);

   theCompJOR->throwAnyJavaException ();

   theCompJOR->deleteLocalRef (theSubJObj);
   }

//------------------------------------------------------------------------------

void WitJ::ComponentJOR::makeJavaBopEntry (Operation * theOpn, Part * thePart)
   {
   ComponentJOR * theCompJOR;
   _jobject *     theOpnJObj;
   _jobject *     thePartJObj;
   _jobject *     theBopEntJObj;

   theCompJOR    = theOpn ->getComponentJOR ();
   theOpnJObj    = theOpn ->getComponentJOR ()->myComponentJObj_;
   thePartJObj   = thePart->getComponentJOR ()->myComponentJObj_;
   
   theBopEntJObj =
      theCompJOR->
         getJNIEnv ()->
            CallStaticObjectMethod (
               ourBopEntryJCl_,
               newInstanceBopEntryJMeth_,
               theOpnJObj,
               thePartJObj);

   theCompJOR->throwAnyJavaException ();

   theCompJOR->deleteLocalRef (theBopEntJObj);
   }

//------------------------------------------------------------------------------

void WitJ::ComponentJOR::setJavaNPeriods (Problem * theProblem, int theValue)
   {
   ComponentJOR * theCompJOR;
   _jobject *     theProblemJObj;

   theCompJOR     = theProblem->getComponentJOR ();
   theProblemJObj = theProblem->getComponentJOR ()->myComponentJObj_;

   theCompJOR->
      getJNIEnv ()->
         CallVoidMethod (
            theProblemJObj,
            setNPeriodsJMeth_,
            theValue);

   theCompJOR->throwAnyJavaException ();
   }

//------------------------------------------------------------------------------

void WitJ::ComponentJOR::exhaustJniMemory ()
   {
   std::string  theString;
   const char * theCString;
   int          totDiscarded;
   _jstring *   theJString;
   _jobject *   theJGlobalRef;

   theString.append (10485760, ' ');

   theCString = theString.c_str ();

   for (totDiscarded = 10; totDiscarded <= 10000; totDiscarded += 10)
      {
      theJString    = newJString (theCString);

      theJGlobalRef = getJNIEnv ()->NewGlobalRef (theJString);

      EXCEPTION_ASSERT (theJGlobalRef != NULL);

      std::cout << totDiscarded << "MB of memory have been allocated by JNI.\n";

      std::cout.flush ();

      deleteLocalRef (theJString);
      }

   EXCEPTION_ASSERT (false);
   }

//------------------------------------------------------------------------------

void WitJ::ComponentJOR::findAllJClasses (JNIEnv * theJNIEnv)
   {
   ourPartJCl_       = newJClassGRef ("com/ibm/witj/Part",       theJNIEnv);
   ourDemandJCl_     = newJClassGRef ("com/ibm/witj/Demand",     theJNIEnv);
   ourOperationJCl_  = newJClassGRef ("com/ibm/witj/Operation",  theJNIEnv);
   ourBomEntryJCl_   = newJClassGRef ("com/ibm/witj/BomEntry",   theJNIEnv);
   ourSubstituteJCl_ = newJClassGRef ("com/ibm/witj/Substitute", theJNIEnv);
   ourBopEntryJCl_   = newJClassGRef ("com/ibm/witj/BopEntry",   theJNIEnv);
   }

//------------------------------------------------------------------------------

void WitJ::ComponentJOR::findAllJMethodIDs (JNIEnv * theJNIEnv)
   {
   makeInstancePartJMeth_ =
      getStaticJMethodID (
         ourPartJCl_,
         "makeInstance",
         "(Lcom/ibm/witj/Problem;Ljava/lang/String;Z)V",
         theJNIEnv);

   newInstanceDemandJMeth_ =
      getStaticJMethodID (
         ourDemandJCl_,
         "newInstance",
         "(Lcom/ibm/witj/Part;Ljava/lang/String;)Lcom/ibm/witj/Demand;",
         theJNIEnv);

   newInstanceOperationJMeth_ =
      getStaticJMethodID (
         ourOperationJCl_,
         "newInstance",
         "(Lcom/ibm/witj/Problem;Ljava/lang/String;)Lcom/ibm/witj/Operation;",
         theJNIEnv);

   newInstanceBomEntryJMeth_ =
      getStaticJMethodID (
         ourBomEntryJCl_,
         "newInstance",
         "(Lcom/ibm/witj/Operation;Lcom/ibm/witj/Part;)"
            "Lcom/ibm/witj/BomEntry;",
         theJNIEnv);

   newInstanceSubstituteJMeth_ =
      getStaticJMethodID (
         ourSubstituteJCl_,
         "newInstance",
         "(Lcom/ibm/witj/BomEntry;Lcom/ibm/witj/Part;)"
            "Lcom/ibm/witj/Substitute;",
         theJNIEnv);

   newInstanceBopEntryJMeth_ =
      getStaticJMethodID (
         ourBopEntryJCl_,
         "newInstance",
         "(Lcom/ibm/witj/Operation;Lcom/ibm/witj/Part;)"
            "Lcom/ibm/witj/BopEntry;",
         theJNIEnv);

   setNPeriodsJMeth_ =
      getJMethodID (
         "com/ibm/witj/Problem",
         "setNPeriods",
         "(I)V",
         theJNIEnv);
   }
