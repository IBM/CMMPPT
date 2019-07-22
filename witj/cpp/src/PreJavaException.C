//------------------------------------------------------------------------------
// WIT-J C++ Source File PreJavaException.C.
//
// Contains the implementation of class PreJavaException.
//------------------------------------------------------------------------------

#include <PreJavaException.h>

//------------------------------------------------------------------------------

WitJ::PreJavaException::PreJavaException (
         const char * theJavaExcClassName,
         const char * theMessage):

      myJavaExcClassName_ (theJavaExcClassName),
      myMessage_          (theMessage)
   {
   }

//------------------------------------------------------------------------------

WitJ::PreJavaException::PreJavaException (
         const PreJavaException & thePreJavaException):

      myJavaExcClassName_ (thePreJavaException.myJavaExcClassName_),
      myMessage_          (thePreJavaException.myMessage_)
   {
   }

//------------------------------------------------------------------------------

WitJ::PreJavaException::~PreJavaException ()
   {
   }
