//------------------------------------------------------------------------------
// WIT-J C++ Source File StringJOR.C.
//
// Contains the implementation of class StringJOR.
//------------------------------------------------------------------------------

#include <StringJOR.h>

//------------------------------------------------------------------------------

WitJ::StringJOR::StringJOR (JNIEnv * theJNIEnv):

      JniClient  (theJNIEnv),
      myJString_ (NULL)
   {
   }

//------------------------------------------------------------------------------

WitJ::StringJOR::~StringJOR ()
   {
   }

//------------------------------------------------------------------------------

void WitJ::StringJOR::makeJavaString (const char * theCString)
   {
   EXCEPTION_ASSERT (theCString != NULL);
   EXCEPTION_ASSERT (myJString_ == NULL);

   myJString_ = getJNIEnv ()->NewStringUTF (theCString);

   throwAnyJavaException ();
   }
