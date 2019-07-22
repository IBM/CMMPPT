//------------------------------------------------------------------------------
// WIT-J C++ Source File MessageMgr.C.
//
// Contains the implementation of class MessageMgr.
//------------------------------------------------------------------------------

#include <MessageMgr.h>
#include  <StringJOR.h>
#include  <CaseToken.h>

#include        <wit.h>

#include     <string>

//------------------------------------------------------------------------------

WitJ::MessageMgr::MessageMgr (Coordinator * theCoord):

      JavaAccObj (theCoord),

      quiet_     (true)
   {
   }

//------------------------------------------------------------------------------

WitJ::MessageMgr::~MessageMgr ()
   {
   }

//------------------------------------------------------------------------------

WitJ::MessageMgr * WitJ::MessageMgr::asMessageMgr ()
   {
   return this;
   }

//------------------------------------------------------------------------------

void WitJ::MessageMgr::flushFile ()
   {
   FILE * theFile;

   witGetMesgFile (getWitRun (), & theFile);

   fflush (theFile);
   }

//------------------------------------------------------------------------------

void WitJ::MessageMgr::setQuiet (bool quietVal)
   {
   quiet_ = quietVal;
   }

//------------------------------------------------------------------------------

bool WitJ::MessageMgr::isQuiet ()
   {
   return quiet_;
   }

//------------------------------------------------------------------------------

void WitJ::MessageMgr::setMesgFileAccessMode (const char * theCStr)
   {
   witSetMesgFileAccessMode (getWitRun (), quiet_, theCStr);
   }

//------------------------------------------------------------------------------

void WitJ::MessageMgr::getMesgFileAccessMode (StringJOR & theStringJOR)
   {
   char * theCString;

   witGetMesgFileAccessMode (getWitRun (), & theCString);

   theStringJOR.makeJavaString (theCString);

   witFree (theCString);
   }

//------------------------------------------------------------------------------

void WitJ::MessageMgr::setMesgFileName (const char * theCStr)
   {
   witSetMesgFileName (getWitRun (), quiet_, theCStr);
   }

//------------------------------------------------------------------------------

void WitJ::MessageMgr::getMesgFileName (StringJOR & theStringJOR)
   {
   char * theCString;

   witGetMesgFileName (getWitRun (), & theCString);

   theStringJOR.makeJavaString (theCString);

   witFree (theCString);
   }

//------------------------------------------------------------------------------

void WitJ::MessageMgr::setMesgTimesPrint (int theMsgNo, int theCount)
   {
   if (theCount < 0)
       theCount = UCHAR_MAX;

   witSetMesgTimesPrint (getWitRun (), quiet_, theMsgNo, theCount);
   }

//------------------------------------------------------------------------------

void WitJ::MessageMgr::setMesgTimesPrintByGroup (
      CaseToken * theGroupCT,
      int         theCount)
   {
   int theMsgGroupNo;

   theMsgGroupNo = theGroupCT->getMsgGroupNo ();

   if (theCount < 0)
       theCount = UCHAR_MAX;

   witSetMesgTimesPrint (getWitRun (), quiet_, theMsgGroupNo, theCount);
   }

//------------------------------------------------------------------------------

void WitJ::MessageMgr::getMesgTimesPrint (int theMsgNo, int & theCount)
   {
   theCount = 0;

   witGetMesgTimesPrint (getWitRun (), theMsgNo, & theCount);

   if (theCount == UCHAR_MAX)
       theCount = -1;
   }
