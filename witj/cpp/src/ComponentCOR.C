//------------------------------------------------------------------------------
// WIT-J C++ Source File ComponentCOR.C.
//
// Contains the C++ implementations of the native methods of Java class
// witj.ComponentCOR.
//------------------------------------------------------------------------------

#include     <NativeMacros.h>
#include <ExceptionHandler.h>
#include       <JniHandler.h>
#include        <Component.h>
#include     <ComponentJOR.h>
#include      <CTVecSupply.h>
#include         <ArrayJOR.h>
#include     <ArrayListJOR.h>

using namespace WitJ;

//------------------------------------------------------------------------------

NATIVE_METHOD (void, ComponentCOR, setComponent) (
      JNIEnv *   theJNIEnv,
      _jobject * theCompCORJObj,
      _jobject * theCompJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler     theJniHandler (theJNIEnv);
   Component *    theComp;
   ComponentJOR * theCompJOR;

   theComp    = theJniHandler.getJavaAccObj (theCompCORJObj)->asComponent ();

   theCompJOR = new ComponentJOR (theCompJObj, theJNIEnv);

   theComp->setComponentJOR (theCompJOR);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, ComponentCOR, setBoundSet) (
      JNIEnv *        theJNIEnv,
      _jobject *      theCompCORJObj,
      _jdoubleArray * hardLBJArr,
      _jdoubleArray * softLBJArr,
      _jdoubleArray * hardUBJArr)
   {
   BEGIN_TRY_BLOCK

   JniHandler  theJniHandler                          (theJNIEnv);
   DblArrayJOR hardLBAJOR (hardLBJArr, theCompCORJObj, theJNIEnv);
   DblArrayJOR softLBAJOR (softLBJArr, theCompCORJObj, theJNIEnv);
   DblArrayJOR hardUBAJOR (hardUBJArr, theCompCORJObj, theJNIEnv);
   Component * theComp;

   theComp = theJniHandler.getJavaAccObj (theCompCORJObj)->asComponent ();

   theComp->
      setBoundSet (
         hardLBAJOR.getBoundCTVec (),
         softLBAJOR.getBoundCTVec (),
         hardUBAJOR.getBoundCTVec ());

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, ComponentCOR, setBoundSetToScalars) (
      JNIEnv *   theJNIEnv,
      _jobject * theCompCORJObj,
      jdouble    hardLBVal,
      jdouble    softLBVal,
      jdouble    hardUBVal)
   {
   BEGIN_TRY_BLOCK

   JniHandler  theJniHandler (theJNIEnv);
   Component * theComp;

   theComp = theJniHandler.getJavaAccObj (theCompCORJObj)->asComponent ();

   theComp->setBoundSetToScalars (hardLBVal, softLBVal, hardUBVal);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, ComponentCOR, getBoundSet) (
      JNIEnv *        theJNIEnv,
      _jobject *      theCompCORJObj,
      _jdoubleArray * hardLBJArr,
      _jdoubleArray * softLBJArr,
      _jdoubleArray * hardUBJArr)
   {
   BEGIN_TRY_BLOCK

   JniHandler  theJniHandler                          (theJNIEnv);
   DblArrayJOR hardLBAJOR (hardLBJArr, theCompCORJObj, theJNIEnv);
   DblArrayJOR softLBAJOR (softLBJArr, theCompCORJObj, theJNIEnv);
   DblArrayJOR hardUBAJOR (hardUBJArr, theCompCORJObj, theJNIEnv);
   Component * theComp;

   theComp = theJniHandler.getJavaAccObj (theCompCORJObj)->asComponent ();

   theComp->getBoundSet (hardLBAJOR, softLBAJOR, hardUBAJOR);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, ComponentCOR, copyComponentData) (
      JNIEnv *   theJNIEnv,
      _jobject * theCompCORJObj,
      _jobject * origCompCORJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler  theJniHandler (theJNIEnv);
   Component *  theComp;
   Component * origComp;

    theComp = theJniHandler.getJavaAccObj  (theCompCORJObj)->asComponent ();
   origComp = theJniHandler.getJavaAccObj (origCompCORJObj)->asComponent ();

   theComp->copyComponentData (origComp);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, ComponentCOR, setWasDeletedInWit) (
      JNIEnv *   theJNIEnv,
      _jobject * theCompCORJObj,
      jboolean   theJBool)
   {
   BEGIN_TRY_BLOCK

   JniHandler  theJniHandler (theJNIEnv);
   Component * theComp;

   theComp = theJniHandler.getJavaAccObj (theCompCORJObj)->asComponent ();

   theComp->setWasDeletedInWit (theJBool);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (jboolean, ComponentCOR, wasDeletedInWit) (
      JNIEnv *   theJNIEnv,
      _jobject * theCompCORJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler  theJniHandler (theJNIEnv);
   Component * theComp;

   theComp = theJniHandler.getJavaAccObj (theCompCORJObj)->asComponent ();

   return theComp->wasDeletedInWit ();

   END_TRY_BLOCK_VALUE_RETURN (theJNIEnv, false)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, ComponentCOR, getPegging) (
      JNIEnv *   theJNIEnv,
      _jobject * theCompCORJObj,
      _jobject * thePggCaseCTCORJObj,
      jint       pegPer,
      _jobject * theTripleListJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler         theJniHandler                   (theJNIEnv);
   Component *        theComp;
   CaseToken *        thePggCaseCT;
   PeggingTripleALJOR theTripALJOR (theTripleListJObj, theJNIEnv);

   theComp      = theJniHandler.getJavaAccObj (theCompCORJObj)->asComponent ();

   thePggCaseCT =
      theJniHandler.getJavaAccObj (thePggCaseCTCORJObj)->asCaseToken ();

   theComp->getPegging (thePggCaseCT, pegPer, theTripALJOR);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }
