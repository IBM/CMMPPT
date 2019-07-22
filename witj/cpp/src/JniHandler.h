//------------------------------------------------------------------------------
// WIT-J C++ Header File JniHandler.h.
//
// Contains the declaration of class JniHandler.
//------------------------------------------------------------------------------

#ifndef JniHandler_h
#define JniHandler_h

#include <JniClient.h>

//------------------------------------------------------------------------------
// Class JniHandler
//
// An instance of this class is intended to assist native method implementations
// in their interaction with JNI.
// The intended mode of use is to declare an instance as a local variable in a
// native method implementation and then call its methods to interact with JNI.
//
// Class Hierarchy:
//
// JniClient
//    JniHandler
//------------------------------------------------------------------------------

namespace WitJ
{
class JniHandler: public JniClient
   {
   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   explicit JniHandler (JNIEnv *);
      //
      // Constructor.

   ~JniHandler ();
      //
      // Destructor.

   const char * getCString (_jstring * theJString);
      //
      // Returns the theJString converted to a c-string.
      // This function may only be called once per JniHandler instance.
      // The returned c-string is owned by this JniHandler.

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   JniHandler              (const JniHandler &);
   JniHandler & operator = (const JniHandler &);
      //
      // Not implemented; prevents accidental copying and assignment.

   //---------------------------------------------------------------------------
   // Private member data.
   //---------------------------------------------------------------------------

   _jstring * myJString_;
      //          
      // If getCString (...) has been called, myJString_ is the jstring that was
      // passed; otherwise NULL.

   const char * myCString_;
      //
      // If getCString (...) has been called, myCString_ is the C-string that
      // was returned; otherwise NULL.
   };
};
//
// End of namespace WitJ.

#endif
