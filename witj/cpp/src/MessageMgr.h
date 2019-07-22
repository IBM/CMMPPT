//------------------------------------------------------------------------------
// WIT-J C++ Header File MessageMgr.h.
//
// Contains the declaration of class MessageMgr.
//------------------------------------------------------------------------------

#ifndef MessageMgr_h
#define MessageMgr_h

#include <JavaAccObj.h>

//------------------------------------------------------------------------------
// Class MessageMgr
//
// "Message Manager"
// Provides an interface WIT's message functions for a Problem.
//
// Class Hierarchy:
//
// JavaAccObj
//    MessageMgr
//------------------------------------------------------------------------------

namespace WitJ
{
class MessageMgr: public JavaAccObj
   {
   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   explicit MessageMgr (Coordinator *);
      //
      // Constructor.

   ~MessageMgr ();
      //
      // Destructor.
   
   virtual MessageMgr * asMessageMgr ();
      //
      // Override from class JavaAccObj.

   void flushFile ();
      //
      // Flushes WIT's message file.

   void setQuiet (bool quietVal);
      //
      // Setter.

   bool isQuiet ();
      //
      // Accessor.

   //---------------------------------------------------------------------------
   // Wrappers for WIT API Functions.
   //---------------------------------------------------------------------------

   void setMesgFileAccessMode    (const char * theCStr);
   void getMesgFileAccessMode    (StringJOR &  theStringJOR);
   void setMesgFileName          (const char * theCStr);
   void getMesgFileName          (StringJOR &  theStringJOR);

   void setMesgTimesPrint        (int         theMsgNo,   int   theCount);
   void setMesgTimesPrintByGroup (CaseToken * theGroupCT, int   theCount);
   void getMesgTimesPrint        (int         theMsgNo,   int & theCount);

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   MessageMgr              (const MessageMgr &);
   MessageMgr & operator = (const MessageMgr &);
      //
      // Not implemented; prevents accidental copying and assignment.

   //---------------------------------------------------------------------------
   // Private member data.
   //---------------------------------------------------------------------------

   bool quiet_;
      //
      // True, iff WIT informational messages are to be suppressed when
      // executing "set" methods.
   };
};
//
// End of namespace WitJ.

#endif
