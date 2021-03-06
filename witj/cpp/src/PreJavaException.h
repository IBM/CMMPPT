//------------------------------------------------------------------------------
// WIT-J C++ Header File PreJavaException.h.
//
// Contains the declaration of class PreJavaException.
//------------------------------------------------------------------------------

#ifndef PreJavaException_h
#define PreJavaException_h

#include <Includes.h>

#include   <string>

//------------------------------------------------------------------------------
// Class PreJavaException
//
// A C++ exception to be thrown in order to cause a Java exception to be thrown
// when the execution returns to Java.
//------------------------------------------------------------------------------

namespace WitJ
{
class PreJavaException
   {
   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   explicit PreJavaException (
         const char * theJavaExcClassName,
         const char * theMessage);
      //
      // Constructor.

   PreJavaException (const PreJavaException &);
      //
      // Copy Constructor.

   ~PreJavaException ();
      //
      // Destructor.

   //---------------------------------------------------------------------------
   // Accessors.
   //---------------------------------------------------------------------------

   inline const char * getJavaExcClassName ()
      {
      return myJavaExcClassName_.c_str ();
      }

   inline const char * getMessage ()
      {
      return myMessage_.c_str ();
      }

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   PreJavaException & operator = (const PreJavaException &);
      //
      // Not implemented; prevents accidental assignment.

   //---------------------------------------------------------------------------
   // Private member data.
   //---------------------------------------------------------------------------

   const std::string myJavaExcClassName_;
      //
      // The class name of the Java exception to be thrown.

   const std::string myMessage_;
      //
      // The detail message for the Java exception to be thrown.
   
   };
};
//
// End of namespace WitJ.

#endif
