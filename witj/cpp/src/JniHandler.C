//------------------------------------------------------------------------------
// WIT-J C++ Source File JniHandler.C.
//
// Contains the implementation of class JniHandler.
//------------------------------------------------------------------------------

#include <JniHandler.h>

//------------------------------------------------------------------------------

WitJ::JniHandler::JniHandler (JNIEnv * theJNIEnv):

      JniClient  (theJNIEnv),
      myJString_ (NULL),
      myCString_ (NULL)
   {
   }

//------------------------------------------------------------------------------

WitJ::JniHandler::~JniHandler ()
   {
   if (myJString_ != NULL)
      getJNIEnv ()->ReleaseStringUTFChars (myJString_, myCString_);
   }

//------------------------------------------------------------------------------

const char * WitJ::JniHandler::getCString (_jstring * theJString)
   {
   EXCEPTION_ASSERT (theJString != NULL);
   EXCEPTION_ASSERT (myJString_ == NULL);

   myJString_ = theJString;

   myCString_ = getJNIEnv ()->GetStringUTFChars (myJString_, NULL);
      
   throwAnyJavaException ();

   return myCString_;
   }
