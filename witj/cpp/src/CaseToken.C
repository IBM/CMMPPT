//------------------------------------------------------------------------------
// WIT-J C++ Source File CaseToken.C.
//
// Contains the implementation of class CaseToken.
//------------------------------------------------------------------------------

#include   <CaseToken.h>
#include <Coordinator.h>

//------------------------------------------------------------------------------

WitJ::CaseToken::CaseToken (Coordinator * theCoord, const char * theName):

      JavaAccObj  (theCoord),
      myCaseKind_ (NO_CASE)
   {
   mySimpleWitFunc_ = NULL;

   theCoord->mapNameToCaseToken (theName, this);
   }

//------------------------------------------------------------------------------

WitJ::CaseToken::~CaseToken ()
   {
   }

//------------------------------------------------------------------------------

WitJ::CaseToken * WitJ::CaseToken::asCaseToken ()
   {
   return this;
   }

//------------------------------------------------------------------------------

void WitJ::CaseToken::setSimpleWitFunc (SimpleWitFunc theFunc)
   {
   EXCEPTION_ASSERT (myCaseKind_ == NO_CASE);

   myCaseKind_      = SIMPLE_WIT_FUNC;

   mySimpleWitFunc_ = theFunc;
   }

//------------------------------------------------------------------------------

void WitJ::CaseToken::setWitAttr (witAttr theWitAttr)
   {
   EXCEPTION_ASSERT (myCaseKind_ == NO_CASE);

   myCaseKind_ = WIT_ATTR;

   myWitAttr_  = theWitAttr;
   }

//------------------------------------------------------------------------------

void WitJ::CaseToken::setWitFileFormat (witFileFormat theWitFileFormat)
   {
   EXCEPTION_ASSERT (myCaseKind_ == NO_CASE);

   myCaseKind_      = WIT_FILE_FORMAT;

   myWitFileFormat_ = theWitFileFormat;
   }

//------------------------------------------------------------------------------

void WitJ::CaseToken::setMsgGroupNo (int theMsgGroupNo)
   {
   EXCEPTION_ASSERT (myCaseKind_ == NO_CASE);

   myCaseKind_   = MSG_GROUP_NO;

   myMsgGroupNo_ = theMsgGroupNo;
   }

//------------------------------------------------------------------------------

void WitJ::CaseToken::setWriteSchedFunc (WriteSchedFunc theFunc)
   {
   EXCEPTION_ASSERT (myCaseKind_ == NO_CASE);

   myCaseKind_       = WRITE_SCHED_FUNC;

   myWriteSchedFunc_ = theFunc;
   }

//------------------------------------------------------------------------------

void WitJ::CaseToken::setDemPggFunc (DemPggFunc theFunc)
   {
   EXCEPTION_ASSERT (myCaseKind_ == NO_CASE);

   myCaseKind_   = DEM_PGG_CASE;

   myDemPggFunc_ = theFunc;
   }

//------------------------------------------------------------------------------

void WitJ::CaseToken::setOpnPggFunc (OpnPggFunc theFunc)
   {
   EXCEPTION_ASSERT (myCaseKind_ == NO_CASE);

   myCaseKind_   = OPN_PGG_CASE;

   myOpnPggFunc_ = theFunc;
   }
