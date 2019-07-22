//------------------------------------------------------------------------------
// WIT-J C++ Source file DemandCOR.C.
//
// Contains the C++ implementations of the native methods of Java class
// witj.DemandCOR.
//------------------------------------------------------------------------------

#include     <NativeMacros.h>
#include <ExceptionHandler.h>
#include       <JniHandler.h>
#include             <Part.h>
#include           <Demand.h>

using namespace WitJ;

//------------------------------------------------------------------------------

NATIVE_METHOD (void, DemandCOR, appendToPipSeq) (
      JNIEnv *   theJNIEnv,
      _jobject * theDemandCORJObj,
      jint       theShipPer,
      jdouble    incShipVol)
   {
   BEGIN_TRY_BLOCK

   JniHandler theJniHandler (theJNIEnv);
   Demand *   theDemand;

   theDemand = theJniHandler.getJavaAccObj (theDemandCORJObj)->asDemand ();

   theDemand->appendToPipSeq (theShipPer, incShipVol);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, DemandCOR, setDemandName) (
      JNIEnv *   theJNIEnv,
      _jobject * theDemandCORJObj,
      _jstring * theDemandNameJStr)
   {
   BEGIN_TRY_BLOCK

   JniHandler   theJniHandler (theJNIEnv);
   Demand *     theDemand;
   const char * theDemandNameCStr;

   theDemand = theJniHandler.getJavaAccObj (theDemandCORJObj)->asDemand ();

   theDemandNameCStr = theJniHandler.getCString (theDemandNameJStr);

   theDemand->setDemandName (theDemandNameCStr);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (jdouble, DemandCOR, incHeurAlloc) (
      JNIEnv *   theJNIEnv,
      _jobject * theDemandCORJObj,
      jint       shipPer,
      jdouble    desIncVol)
   {
   BEGIN_TRY_BLOCK

   JniHandler theJniHandler (theJNIEnv);
   Demand *   theDemand;
   double     incVol;

   theDemand = theJniHandler.getJavaAccObj (theDemandCORJObj)->asDemand ();

   incVol    = theDemand->incHeurAlloc (shipPer, desIncVol);

   return incVol;

   END_TRY_BLOCK_VALUE_RETURN (theJNIEnv, 0.0)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, DemandCOR, createAndStoreCppDemand) (
      JNIEnv *   theJNIEnv,
      _jobject * theDemandCORJObj,
      _jobject * thePartCORJObj,
      _jstring * theDemandNameJStr)
   {
   BEGIN_TRY_BLOCK

   JniHandler   theJniHandler (theJNIEnv);
   Part *       thePart;
   const char * theDemandNameCStr;
   Demand *     theDemand;

   thePart           = theJniHandler.getJavaAccObj (thePartCORJObj)->asPart ();
   theDemandNameCStr = theJniHandler.getCString    (theDemandNameJStr);

   theDemand         = new Demand (thePart, theDemandNameCStr);

   theJniHandler.storeInCOR (theDemandCORJObj, theDemand);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }
