//------------------------------------------------------------------------------
// WIT-J C++ Source File JniClient.C.
//
// Contains the implementation of class JniClient.
//------------------------------------------------------------------------------

#include  <JniClient.h>
#include <JavaAccObj.h>

//------------------------------------------------------------------------------

_jclass *    WitJ::JniClient:: ourStringJCl_               = NULL;
_jclass *    WitJ::JniClient::ourIntegerJCl_               = NULL;
_jclass *    WitJ::JniClient:: ourDoubleJCl_               = NULL;

_jmethodID * WitJ::JniClient::valueOfIntegerJMeth_         = NULL;
_jmethodID * WitJ::JniClient:: valueOfDoubleJMeth_         = NULL;

_jfieldID *  WitJ::JniClient::ourMyCppJavaObjRepAsLongFid_ = NULL;

//------------------------------------------------------------------------------

void WitJ::JniClient::setUpJni (JNIEnv * theJNIEnv)
   {
   _jclass * theJClassForJavaAccObjCOR;

   ourStringJCl_  = newJClassGRef ("java/lang/String",  theJNIEnv);
   ourIntegerJCl_ = newJClassGRef ("java/lang/Integer", theJNIEnv);
   ourDoubleJCl_  = newJClassGRef ("java/lang/Double",  theJNIEnv);

   valueOfIntegerJMeth_ =
      getStaticJMethodID (
         ourIntegerJCl_,
         "valueOf",
         "(I)Ljava/lang/Integer;",
         theJNIEnv);

   valueOfDoubleJMeth_ =
      getStaticJMethodID (
         ourDoubleJCl_,
         "valueOf",
         "(D)Ljava/lang/Double;",
         theJNIEnv);

   theJClassForJavaAccObjCOR =
      findJClass ("com/ibm/witj/JavaAccObjCOR", theJNIEnv);

   ourMyCppJavaObjRepAsLongFid_ =
      theJNIEnv->
         GetFieldID (theJClassForJavaAccObjCOR, "myCppJavaObjRepAsLong", "J");

   throwAnyJavaException (theJNIEnv);

   theJNIEnv->DeleteLocalRef (theJClassForJavaAccObjCOR);
   }

//------------------------------------------------------------------------------

void WitJ::JniClient::storeInCOR (
      _jobject *   theJavaAccObjCORJObj,
      JavaAccObj * theJavaAccObj)
   {
   jlong theCppPtrAsJLong;

   theCppPtrAsJLong = reinterpret_cast <jlong> (theJavaAccObj);

   myJNIEnv_->
      SetLongField (
         theJavaAccObjCORJObj,
         ourMyCppJavaObjRepAsLongFid_,
         theCppPtrAsJLong);

   throwAnyJavaException ();
   }

//------------------------------------------------------------------------------

WitJ::JavaAccObj * WitJ::JniClient::getJavaAccObj (
      _jobject * theJavaAccObjCORJObj)
   {
   jlong        theCppPtrAsJLong;
   JavaAccObj * theJavaAccObj;

   theCppPtrAsJLong =
      myJNIEnv_->
         GetLongField (theJavaAccObjCORJObj, ourMyCppJavaObjRepAsLongFid_);

   throwAnyJavaException ();

   theJavaAccObj = reinterpret_cast <JavaAccObj *> (theCppPtrAsJLong);

   EXCEPTION_ASSERT (theJavaAccObj != NULL);

   EXCEPTION_ASSERT (theJavaAccObj->isValid ());

   return theJavaAccObj;
   }

//------------------------------------------------------------------------------

_jstring * WitJ::JniClient::newJString (
      const char * theCString,
      JNIEnv *     theJNIEnv)
   {
   _jstring * theJString;

   EXCEPTION_ASSERT (theCString != NULL);

   theJString = theJNIEnv->NewStringUTF (theCString);

   throwAnyJavaException (theJNIEnv);

   return theJString;
   }

//------------------------------------------------------------------------------

_jobject * WitJ::JniClient::matchingIntegerJObj (
      int      theInt,
      JNIEnv * theJNIEnv)
   {
   _jobject * theIntJObj;

   theIntJObj =
      theJNIEnv->
         CallStaticObjectMethod (ourIntegerJCl_, valueOfIntegerJMeth_, theInt);

   throwAnyJavaException (theJNIEnv);

   return theIntJObj;
   }

//------------------------------------------------------------------------------

_jobject * WitJ::JniClient::matchingDoubleJObj (
      double   theDbl,
      JNIEnv * theJNIEnv)
   {
   _jobject * theDblJObj;

   theDblJObj =
      theJNIEnv->
         CallStaticObjectMethod (ourDoubleJCl_, valueOfDoubleJMeth_, theDbl);

   throwAnyJavaException (theJNIEnv);

   return theDblJObj;
   }

//------------------------------------------------------------------------------

bool WitJ::JniClient::isaUTF8String (const char * theCString)
   {
   _jstring *   theJString;
   const char * theCString2;
   bool         isUFT8;

   theJString  = newJString (theCString);

   theCString2 = myJNIEnv_->GetStringUTFChars (theJString, NULL);

   throwAnyJavaException ();

   isUFT8      = equal (theCString2, theCString);

   myJNIEnv_->ReleaseStringUTFChars (theJString, theCString2);

   deleteLocalRef (theJString);

   return isUFT8;
   }

//------------------------------------------------------------------------------

void WitJ::JniClient::throwAnyJavaException (JNIEnv * theJNIEnv)
   {
   _jthrowable * theJThrow;

   theJThrow = theJNIEnv->ExceptionOccurred ();

   if (theJThrow != NULL)
      {
      theJNIEnv->ExceptionClear ();

      throw theJThrow;
      }
   }

//------------------------------------------------------------------------------

void WitJ::JniClient::findNonsense ()
   {
   findJClass ("Nonsense", myJNIEnv_);
   }

//------------------------------------------------------------------------------

WitJ::JniClient::JniClient (JNIEnv * theJNIEnv):

      myJNIEnv_ (theJNIEnv)
   {
   }

//------------------------------------------------------------------------------

WitJ::JniClient::JniClient (const JniClient & theJniClient):

      myJNIEnv_ (theJniClient.myJNIEnv_)
   {
   }

//------------------------------------------------------------------------------

WitJ::JniClient::~JniClient ()
   {
   }

//------------------------------------------------------------------------------

_jclass * WitJ::JniClient::newJClassGRef (
      const char * theClassName,
      JNIEnv *     theJNIEnv)
   {
   _jclass *  theJClLRef;
   _jobject * theJClGRef;

   theJClLRef = findJClass (theClassName, theJNIEnv);

   theJClGRef = theJNIEnv->NewGlobalRef (theJClLRef);

   EXCEPTION_ASSERT (theJClGRef != NULL);

   theJNIEnv->DeleteLocalRef (theJClLRef);

   return static_cast <_jclass *> (theJClGRef);
   }

//------------------------------------------------------------------------------

_jmethodID * WitJ::JniClient::getJMethodID (
      const char * theClassName,
      const char * theMethName,
      const char * theMethDesc,
      JNIEnv *     theJNIEnv)
   {
   _jclass *    theJClass;
   _jmethodID * theJMethodID;

   theJClass    = findJClass (theClassName, theJNIEnv);

   theJMethodID = theJNIEnv->GetMethodID (theJClass, theMethName, theMethDesc);

   throwAnyJavaException (theJNIEnv);

   theJNIEnv->DeleteLocalRef (theJClass);

   return theJMethodID;
   }

//------------------------------------------------------------------------------

_jmethodID * WitJ::JniClient::getStaticJMethodID (
      _jclass *    theJClass,
      const char * theMethName,
      const char * theMethDesc,
      JNIEnv *     theJNIEnv)
   {
   _jmethodID * theJMethodID;

   theJMethodID =
      theJNIEnv->GetStaticMethodID (theJClass, theMethName, theMethDesc);

   throwAnyJavaException (theJNIEnv);

   return theJMethodID;
   }

//------------------------------------------------------------------------------

_jstring * WitJ::JniClient::newJString (const char * theCString)
   {
   return newJString (theCString, myJNIEnv_);
   }

//------------------------------------------------------------------------------

_jclass * WitJ::JniClient::findJClass (
      const char * theClassName,
      JNIEnv *     theJNIEnv)
   {
   _jclass * theJClass;

   theJClass = theJNIEnv->FindClass (theClassName);

   throwAnyJavaException (theJNIEnv);

   return theJClass;
   }

//------------------------------------------------------------------------------

void WitJ::JniClient::throwAnyJavaException ()
   {
   throwAnyJavaException (myJNIEnv_);
   }

//------------------------------------------------------------------------------

void WitJ::JniClient::deleteLocalRef (_jobject * theLocalRef)
   {
   myJNIEnv_->DeleteLocalRef (theLocalRef);
   }
