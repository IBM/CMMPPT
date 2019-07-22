//------------------------------------------------------------------------------
// WIT-J C++ Header File JniClient.h.
//
// Contains the declaration of class JniClient.
//------------------------------------------------------------------------------

#ifndef JniClient_h
#define JniClient_h

#include <Includes.h>

#include      <jni.h>

//------------------------------------------------------------------------------
// Class JniClient
//
// This class and the classes derived from it are responsible for all
// interaction with JNI.
//
// Class Hierarchy:
//
// JniClient
//    JniHandler
//    StringJOR
//    ArrayJOR <Boolean>
//       BoolArrayJOR
//    ArrayJOR <int>
//       IntArrayJOR
//    ArrayJOR <double>
//       DblArrayJOR
//    ArrayListJOR
//       IntALJOR
//       DblALJOR
//       StringALJOR
//       CompALJOR <C>
//       DemandALJOR
//       PeggingTripleALJOR
//    ComponentJOR
//    ExceptionHandler
//------------------------------------------------------------------------------

namespace WitJ
{
class JniClient
   {
   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   static void setUpJni (JNIEnv *);
      //
      // Sets up the interaction between this class and JNI.

   void storeInCOR (
         _jobject *   theJavaAccObjCORJObj,
         JavaAccObj * theJavaAccObj);
      //
      // Stores theJavaAccObj in the Java JavaAccObjCOR represented by
      // theJavaAccObjCORJObj, transferring ownership of it.
      // If theJavaAccObj is NULL, the Java JavaAccObjCOR represented by
      // theJavaAccObjCORJObj releases ownership of its JavaAccObj.

   JavaAccObj * getJavaAccObj (_jobject * theJavaAccObjCORJObj);
      //
      // Returns the JavaAccObj owned by the JavaAccObjCOR referenced by
      // theJavaAccObjCORJObj.
      // theJavaAccObjCORJObj and the JavaAccObj must not be NULL.

   static _jstring * newJString (const char * theCString, JNIEnv * theJNIEnv);
      //
      // Creates a Java string that's a copy of theCString and creates and
      // returns a local reference to it.

   static _jobject * matchingIntegerJObj (int    theInt, JNIEnv * theJNIEnv);
   static _jobject * matchingDoubleJObj  (double theDbl, JNIEnv * theJNIEnv);
      //
      // Creates and returns a local reference to a Java Integer whose value
      //    matches theInt.
      // Creates and returns a local reference to a Java Double  whose value
      //    matches theDbl.

   bool isaUTF8String (const char * theCString);
      //
      // Returns true, iff theCString is in UTF-8 format.
      // This is determined by converting theCString from UFT-8 format into a
      // Java String and then back again and verifying that its content is
      // preserved.

   static void throwAnyJavaException (JNIEnv * theJNIEnv);
      //
      // Checks for any Java Throwable that's currently pending and if there is
      // one, clears it and then throws the corresponding _jthrowable pointer as
      // a C++ exception.

   void findNonsense ();
      //
      // Searches for a Java class called "Nonsense".
      // Used for testing.

   //---------------------------------------------------------------------------
   // Protected member functions.
   //---------------------------------------------------------------------------

   protected:

   explicit JniClient (JNIEnv *);
      //
      // Constructor.

   explicit JniClient (const JniClient &);
      //
      // Copy Constructor.

   ~JniClient ();
      //
      // Destructor.

   static _jclass * newJClassGRef (
         const char * theClassName,
         JNIEnv *     theJNIEnv);
      //
      // Finds the JNI class object for the Java class whose name is given by
      // theClassName and creates and returns a global reference to it.

   static _jmethodID * getJMethodID (
         const char * theClassName,
         const char * theMethName,
         const char * theMethDesc,
         JNIEnv *     theJNIEnv);
      //
      // Finds and returns the _jmethodID of a non-static Java method.
      // Arguments:
      //    theClassName: The name of the class to which the method belongs.
      //    theMethName:  The name of the method.
      //    theMethDesc:  The JNI method descriptor of the method.
      //    theJNIEnv:    The JNI environment to be used.

   static _jmethodID * getStaticJMethodID (
         _jclass *    theJClass,
         const char * theMethName,
         const char * theMethDesc,
         JNIEnv *     theJNIEnv);
      //
      // Returns the _jmethodID for a static Java method corresponding to the
      // arguments.

   _jstring * newJString (const char * theCString);
      //
      // Creates a Java string that's a copy of theCString and creates and
      // returns a local reference to it.

   static _jclass * findJClass (const char * theClassName, JNIEnv * theJNIEnv);
      //
      // Finds the Java class whose name is given by theClassName and creates
      // and returns a JNI local reference to it.

   void throwAnyJavaException ();
      //
      // Checks for any Java Throwable that's currently pending and if there is
      // one, clears it and then throws the corresponding _jthrowable pointer as
      // a C++ exception.

   void deleteLocalRef (_jobject * theLocalRef);
      //
      // Deletes the local reference theLocalRef.

   inline JNIEnv * getJNIEnv ()
      {
      return myJNIEnv_;
      }

   inline JNIEnv * getJNIEnv (JniClient & theJniClient)
      {
      return theJniClient.myJNIEnv_;
      }

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   JniClient & operator = (const JniClient &);
      //
      // Not implemented; prevents accidental assignment.

   //---------------------------------------------------------------------------
   // Private member data.
   //---------------------------------------------------------------------------

   JNIEnv * const myJNIEnv_;
      //
      // The JNI environment for this JniClient.

   static _jclass *  ourStringJCl_;
   static _jclass * ourIntegerJCl_;
   static _jclass *  ourDoubleJCl_;
      //
      // JNI Global references to each of the following Java classes:
      //    String
      //    Integer
      //    Double

   static _jmethodID * valueOfIntegerJMeth_;
   static _jmethodID *  valueOfDoubleJMeth_;
      //
      // The JNI method IDs for each of the following Java methods:
      //    Class Integer static method valueOf
      //    Class Double  static method valueOf

   static _jfieldID * ourMyCppJavaObjRepAsLongFid_;
      //
      // The JNI field ID for witj.JavaAccObjCOR.myCppJavaObjRepAsLong.
   };
};
//
// End of namespace WitJ.

#endif
