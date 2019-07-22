//------------------------------------------------------------------------------
// WIT-J C++ Source file AttCOR.C.
//
// Contains the C++ implementations of the native methods of Java class
// witj.AttCOR.
//------------------------------------------------------------------------------

#include     <NativeMacros.h>
#include <ExceptionHandler.h>
#include       <JniHandler.h>
#include              <Att.h>
#include          <AttBldr.h>
#include        <Component.h>
#include      <CTVecSupply.h>
#include         <ArrayJOR.h>
#include        <StringJOR.h>

using namespace WitJ;

//------------------------------------------------------------------------------

NATIVE_METHOD (void, AttCOR, setBool) (
      JNIEnv *   theJNIEnv,
      _jobject * theAttCORJObj,
      _jobject * theCompCORJObj,
      jboolean   theJBool)
   {
   BEGIN_TRY_BLOCK

   JniHandler  theJniHandler (theJNIEnv);
   Att *       theAtt;
   Component * theComp;
   Boolean     theBool;

   theAtt  = theJniHandler.getJavaAccObj (theAttCORJObj) ->asAtt ();
   theComp = theJniHandler.getJavaAccObj (theCompCORJObj)->asComponent ();

   theBool = theJBool;

   theAtt->set (theComp, theBool);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, AttCOR, setInt) (
      JNIEnv *   theJNIEnv,
      _jobject * theAttCORJObj,
      _jobject * theCompCORJObj,
      jint       theJInt)
   {
   BEGIN_TRY_BLOCK

   JniHandler  theJniHandler (theJNIEnv);
   Att *       theAtt;
   Component * theComp;
   int         theInt;

   theAtt  = theJniHandler.getJavaAccObj (theAttCORJObj) ->asAtt ();
   theComp = theJniHandler.getJavaAccObj (theCompCORJObj)->asComponent ();

   theInt  = theJInt;

   theAtt->set (theComp, theInt);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, AttCOR, setDbl) (
      JNIEnv *   theJNIEnv,
      _jobject * theAttCORJObj,
      _jobject * theCompCORJObj,
      jdouble    theJDbl)
   {
   BEGIN_TRY_BLOCK

   JniHandler  theJniHandler (theJNIEnv);
   Att *       theAtt;
   Component * theComp;
   double      theDbl;

   theAtt  = theJniHandler.getJavaAccObj (theAttCORJObj) ->asAtt ();
   theComp = theJniHandler.getJavaAccObj (theCompCORJObj)->asComponent ();

   theDbl  = theJDbl;

   theAtt->set (theComp, theDbl);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, AttCOR, setString) (
      JNIEnv *   theJNIEnv,
      _jobject * theAttCORJObj,
      _jobject * theCompCORJObj,
      _jstring * theJString)
   {
   BEGIN_TRY_BLOCK

   JniHandler   theJniHandler (theJNIEnv);
   Att *        theAtt;
   Component *  theComp;
   const char * theCString;

   theAtt     = theJniHandler.getJavaAccObj (theAttCORJObj) ->asAtt ();
   theComp    = theJniHandler.getJavaAccObj (theCompCORJObj)->asComponent ();
   theCString = theJniHandler.getCString    (theJString);

   theAtt->set (theComp, theCString);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, AttCOR, setBoolVec) (
      JNIEnv *         theJNIEnv,
      _jobject *       theAttCORJObj,
      _jobject *       theCompCORJObj,
      _jbooleanArray * theJBoolArr)
   {
   BEGIN_TRY_BLOCK

   JniHandler  theJniHandler                         (theJNIEnv);
   BoolArrayJOR theBoolAJOR (theJBoolArr, theAttCORJObj, theJNIEnv);
   Att *       theAtt;
   Component * theComp;

   theAtt  = theJniHandler.getJavaAccObj (theAttCORJObj) ->asAtt ();
   theComp = theJniHandler.getJavaAccObj (theCompCORJObj)->asComponent ();

   theAtt->set (theComp, theBoolAJOR.getCTVec ());

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, AttCOR, setIntVec) (
      JNIEnv *     theJNIEnv,
      _jobject *   theAttCORJObj,
      _jobject *   theCompCORJObj,
      _jintArray * theJIntArr)
   {
   BEGIN_TRY_BLOCK

   JniHandler  theJniHandler                         (theJNIEnv);
   IntArrayJOR theIntAJOR (theJIntArr, theAttCORJObj, theJNIEnv);
   Att *       theAtt;
   Component * theComp;

   theAtt  = theJniHandler.getJavaAccObj (theAttCORJObj) ->asAtt ();
   theComp = theJniHandler.getJavaAccObj (theCompCORJObj)->asComponent ();

   theAtt->set (theComp, theIntAJOR.getCTVec ());

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, AttCOR, setDblVec) (
      JNIEnv *        theJNIEnv,
      _jobject *      theAttCORJObj,
      _jobject *      theCompCORJObj,
      _jdoubleArray * theJDblArr)
   {
   BEGIN_TRY_BLOCK

   JniHandler  theJniHandler                         (theJNIEnv);
   DblArrayJOR theDblAJOR (theJDblArr, theAttCORJObj, theJNIEnv);
   Att *       theAtt;
   Component * theComp;

   theAtt  = theJniHandler.getJavaAccObj (theAttCORJObj) ->asAtt ();
   theComp = theJniHandler.getJavaAccObj (theCompCORJObj)->asComponent ();

   theAtt->set (theComp, theDblAJOR.getCTVec ());

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, AttCOR, setBoolVecToScalar) (
      JNIEnv *   theJNIEnv,
      _jobject * theAttCORJObj,
      _jobject * theCompCORJObj,
      jboolean   theJBool)
   {
   BEGIN_TRY_BLOCK

   JniHandler  theJniHandler (theJNIEnv);
   Att *       theAtt;
   Component * theComp;
   Boolean     theBool;

   theAtt  = theJniHandler.getJavaAccObj (theAttCORJObj) ->asAtt ();
   theComp = theJniHandler.getJavaAccObj (theCompCORJObj)->asComponent ();

   theBool = theJBool;

   theAtt->setVectorToScalar (theComp, theBool);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, AttCOR, setIntVecToScalar) (
      JNIEnv *   theJNIEnv,
      _jobject * theAttCORJObj,
      _jobject * theCompCORJObj,
      jint       theJInt)
   {
   BEGIN_TRY_BLOCK

   JniHandler  theJniHandler (theJNIEnv);
   Att *       theAtt;
   Component * theComp;
   int         theInt;

   theAtt  = theJniHandler.getJavaAccObj (theAttCORJObj) ->asAtt ();
   theComp = theJniHandler.getJavaAccObj (theCompCORJObj)->asComponent ();

   theInt  = theJInt;

   theAtt->setVectorToScalar (theComp, theInt);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, AttCOR, setDblVecToScalar) (
      JNIEnv *   theJNIEnv,
      _jobject * theAttCORJObj,
      _jobject * theCompCORJObj,
      jdouble    theJDbl)
   {
   BEGIN_TRY_BLOCK

   JniHandler  theJniHandler (theJNIEnv);
   Att *       theAtt;
   Component * theComp;
   double      theDbl;

   theAtt  = theJniHandler.getJavaAccObj (theAttCORJObj) ->asAtt ();
   theComp = theJniHandler.getJavaAccObj (theCompCORJObj)->asComponent ();

   theDbl  = theJDbl;

   theAtt->setVectorToScalar (theComp, theDbl);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (jboolean, AttCOR, getBool) (
      JNIEnv *   theJNIEnv,
      _jobject * theAttCORJObj,
      _jobject * theCompCORJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler  theJniHandler (theJNIEnv);
   Att *       theAtt;
   Component * theComp;
   Boolean     theBool;

   theAtt  = theJniHandler.getJavaAccObj (theAttCORJObj) ->asAtt ();
   theComp = theJniHandler.getJavaAccObj (theCompCORJObj)->asComponent ();

   theAtt->get (theComp, theBool);

   EXCEPTION_ASSERT ((theBool == 0) || (theBool == 1));

   return theBool;

   END_TRY_BLOCK_VALUE_RETURN (theJNIEnv, false)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (jint, AttCOR, getInt) (
      JNIEnv *   theJNIEnv,
      _jobject * theAttCORJObj,
      _jobject * theCompCORJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler  theJniHandler (theJNIEnv);
   Att *       theAtt;
   Component * theComp;
   int         theInt;

   theAtt  = theJniHandler.getJavaAccObj (theAttCORJObj) ->asAtt ();
   theComp = theJniHandler.getJavaAccObj (theCompCORJObj)->asComponent ();

   theAtt->get (theComp, theInt);

   return theInt;

   END_TRY_BLOCK_VALUE_RETURN (theJNIEnv, 0)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (jdouble, AttCOR, getDbl) (
      JNIEnv *   theJNIEnv,
      _jobject * theAttCORJObj,
      _jobject * theCompCORJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler  theJniHandler (theJNIEnv);
   Att *       theAtt;
   Component * theComp;
   double      theDbl;

   theAtt  = theJniHandler.getJavaAccObj (theAttCORJObj) ->asAtt ();
   theComp = theJniHandler.getJavaAccObj (theCompCORJObj)->asComponent ();

   theAtt->get (theComp, theDbl);

   return theDbl;

   END_TRY_BLOCK_VALUE_RETURN (theJNIEnv, 0.0)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (_jstring *, AttCOR, getString) (
      JNIEnv *   theJNIEnv,
      _jobject * theAttCORJObj,
      _jobject * theCompCORJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler  theJniHandler (theJNIEnv);
   Att *       theAtt;
   Component * theComp;
   StringJOR   theStringJOR  (theJNIEnv);

   theAtt  = theJniHandler.getJavaAccObj (theAttCORJObj) ->asAtt ();
   theComp = theJniHandler.getJavaAccObj (theCompCORJObj)->asComponent ();

   theAtt->get (theComp, theStringJOR);

   return theStringJOR.getJString ();

   END_TRY_BLOCK_VALUE_RETURN (theJNIEnv, NULL)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, AttCOR, getBoolVec) (
      JNIEnv *         theJNIEnv,
      _jobject *       theAttCORJObj,
      _jobject *       theCompCORJObj,
      _jbooleanArray * theJBoolArr)
   {
   BEGIN_TRY_BLOCK

   JniHandler   theJniHandler                           (theJNIEnv);
   BoolArrayJOR theBoolAJOR (theJBoolArr, theAttCORJObj, theJNIEnv);
   Att *        theAtt;
   Component *  theComp;

   theAtt  = theJniHandler.getJavaAccObj (theAttCORJObj) ->asAtt ();
   theComp = theJniHandler.getJavaAccObj (theCompCORJObj)->asComponent ();

   theAtt->get (theComp, theBoolAJOR);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, AttCOR, getIntVec) (
      JNIEnv *     theJNIEnv,
      _jobject *   theAttCORJObj,
      _jobject *   theCompCORJObj,
      _jintArray * theJIntArr)
   {
   BEGIN_TRY_BLOCK

   JniHandler  theJniHandler                         (theJNIEnv);
   IntArrayJOR theIntAJOR (theJIntArr, theAttCORJObj, theJNIEnv);
   Att *       theAtt;
   Component * theComp;

   theAtt  = theJniHandler.getJavaAccObj (theAttCORJObj) ->asAtt ();
   theComp = theJniHandler.getJavaAccObj (theCompCORJObj)->asComponent ();

   theAtt->get (theComp, theIntAJOR);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, AttCOR, getDblVec) (
      JNIEnv *        theJNIEnv,
      _jobject *      theAttCORJObj,
      _jobject *      theCompCORJObj,
      _jdoubleArray * theJDblArr)
   {
   BEGIN_TRY_BLOCK

   JniHandler  theJniHandler                         (theJNIEnv);
   DblArrayJOR theDblAJOR (theJDblArr, theAttCORJObj, theJNIEnv);
   Att *       theAtt;
   Component * theComp;

   theAtt  = theJniHandler.getJavaAccObj (theAttCORJObj) ->asAtt ();
   theComp = theJniHandler.getJavaAccObj (theCompCORJObj)->asComponent ();

   theAtt->get (theComp, theDblAJOR);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (jboolean, AttCOR, isModifiable) (
      JNIEnv *   theJNIEnv,
      _jobject * theAttCORJObj)
   {
   BEGIN_TRY_BLOCK

   JniHandler theJniHandler (theJNIEnv);
   Att *      theAtt;

   theAtt  = theJniHandler.getJavaAccObj (theAttCORJObj)->asAtt ();

   return theAtt->isModifiable ();

   END_TRY_BLOCK_VALUE_RETURN (theJNIEnv, false)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (jboolean, AttCOR, valueTypeMatches) (
      JNIEnv *   theJNIEnv,
      _jobject * theAttCORJObj,
      _jstring * theTypeNameJStr)
   {
   BEGIN_TRY_BLOCK

   JniHandler  theJniHandler (theJNIEnv);
   Att *       theAtt;
   std::string theTypeName;

   theAtt      = theJniHandler.getJavaAccObj (theAttCORJObj)  ->asAtt ();
   theTypeName = theJniHandler.getCString    (theTypeNameJStr);

   return theAtt->valueTypeMatches (theTypeName);

   END_TRY_BLOCK_VALUE_RETURN (theJNIEnv, false)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (jboolean, AttCOR, appliesTo) (
      JNIEnv *   theJNIEnv,
      _jobject * theAttCORJObj,
      _jstring * theClassNameJStr)
   {
   BEGIN_TRY_BLOCK

   JniHandler   theJniHandler (theJNIEnv);
   Att *        theAtt;
   const char * theClassNameCStr;

   theAtt           = theJniHandler.getJavaAccObj (theAttCORJObj)   ->asAtt ();
   theClassNameCStr = theJniHandler.getCString    (theClassNameJStr);

   return (theAtt->getCompClassName () == theClassNameCStr);

   END_TRY_BLOCK_VALUE_RETURN (theJNIEnv, false)
   }

//------------------------------------------------------------------------------

NATIVE_METHOD (void, AttCOR, storeCppAtt) (
      JNIEnv *   theJNIEnv,
      _jobject * theAttCORJObj,
      _jobject * theAttBldrCORJObj,
      _jstring * scopedAttNameJStr)
   {
   BEGIN_TRY_BLOCK

   JniHandler   theJniHandler (theJNIEnv);
   AttBldr *    theAttBldr;
   const char * scopedAttNameCStr;
   Att *        theAtt;

   theAttBldr = theJniHandler.getJavaAccObj (theAttBldrCORJObj)->asAttBldr ();

   scopedAttNameCStr = theJniHandler.getCString (scopedAttNameJStr);

   theAtt = theAttBldr->handOverAttFor (scopedAttNameCStr);

   theJniHandler.storeInCOR (theAttCORJObj, theAtt);

   END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
   }

