//------------------------------------------------------------------------------
// WIT-J C++ Header File CaseToken.h.
//
// Contains the declaration of class CaseToken.
//------------------------------------------------------------------------------

#ifndef CaseToken_h
#define CaseToken_h

#include <JavaAccObj.h>

#include        <wit.h>

//------------------------------------------------------------------------------
// Typedef SimpleWitFunc
// "Simple WIT API Function"
//
// A pointer to a WIT API function whose only argument is the WitRun *.
//------------------------------------------------------------------------------

namespace WitJ
{
typedef witReturnCode (* SimpleWitFunc) (WitRun * const);
}

//------------------------------------------------------------------------------
// Typedef WriteSchedFunc
// "Write Schedule Function"
//
// A pointer to a WIT API function for writing a schedule.
//------------------------------------------------------------------------------

namespace WitJ
{
typedef witReturnCode (* WriteSchedFunc) (
   WitRun * const,
   const char * const,
   const witFileFormat);
}

//------------------------------------------------------------------------------
// Class CaseToken
//
// Represents a particular alternative that might be chosen.
//
// Class Hierarchy:
//
// JavaAccObj
//    CaseToken
//------------------------------------------------------------------------------

namespace WitJ
{
class CaseToken: public JavaAccObj
   {
   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   explicit CaseToken (Coordinator *, const char *);
      //
      // Constructor.

   ~CaseToken ();
      //
      // Destructor.
   
   virtual CaseToken * asCaseToken ();
      //
      // Override from class JavaAccObj.

   //---------------------------------------------------------------------------
   // "Set" functions for each kind of alternative that might be represented by
   // this CaseToken.
   // Exactly one of these functions should be called exactly one for each
   // CaseToken.
   //---------------------------------------------------------------------------

   void setSimpleWitFunc  (SimpleWitFunc);
   void setWitAttr        (witAttr);
   void setWitFileFormat  (witFileFormat);
   void setMsgGroupNo     (int);
   void setWriteSchedFunc (WriteSchedFunc);
   void setDemPggFunc     (DemPggFunc);
   void setOpnPggFunc     (OpnPggFunc);

   //---------------------------------------------------------------------------
   // Accessors.
   //---------------------------------------------------------------------------

   inline SimpleWitFunc getSimpleWitFunc ()
      {
      EXCEPTION_ASSERT (myCaseKind_ == SIMPLE_WIT_FUNC);

      return mySimpleWitFunc_;
      }

   inline witAttr getWitAttr ()
      {
      EXCEPTION_ASSERT (myCaseKind_ == WIT_ATTR);

      return myWitAttr_;
      }

   inline witFileFormat getWitFileFormat ()
      {
      EXCEPTION_ASSERT (myCaseKind_ == WIT_FILE_FORMAT);

      return myWitFileFormat_;
      }

   inline int getMsgGroupNo ()
      {
      EXCEPTION_ASSERT (myCaseKind_ == MSG_GROUP_NO);

      return myMsgGroupNo_;
      }

   inline WriteSchedFunc getWriteSchedFunc ()
      {
      EXCEPTION_ASSERT (myCaseKind_ == WRITE_SCHED_FUNC);

      return myWriteSchedFunc_;
      }

   inline DemPggFunc getDemPggFunc ()
      {
      EXCEPTION_ASSERT (myCaseKind_ == DEM_PGG_CASE);

      return myDemPggFunc_;
      }

   inline OpnPggFunc getOpnPggFunc ()
      {
      EXCEPTION_ASSERT (myCaseKind_ == OPN_PGG_CASE);

      return myOpnPggFunc_;
      }

   private:

   //---------------------------------------------------------------------------
   // Private Nested Enum CaseKind
   //
   // Identifies which kind of alternative is represented by a CaseToken.
   //---------------------------------------------------------------------------

   enum CaseKind
      {
      SIMPLE_WIT_FUNC,
      WIT_ATTR,
      WIT_FILE_FORMAT,
      MSG_GROUP_NO,
      WRITE_SCHED_FUNC,
      DEM_PGG_CASE,
      OPN_PGG_CASE,
      NO_CASE,
      };

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   CaseToken              (const CaseToken &);
   CaseToken & operator = (const CaseToken &);
      //
      // Not implemented; prevents accidental copying and assignment.

   //---------------------------------------------------------------------------
   // Private member data.
   //---------------------------------------------------------------------------

   CaseKind myCaseKind_;
      //
      // Identifies which kind of alternative is represented by this CaseToken.

   union
      {
      SimpleWitFunc mySimpleWitFunc_;
         //
         // The SimpleWitFunc represented by this CaseToken,
         // if myCaseKind == SIMPLE_WIT_FUNC.

      witAttr myWitAttr_;
         //
         // The witAttr represented by this CaseToken,
         // if myCaseKind == WIT_ATTR.

      witFileFormat myWitFileFormat_;
         //
         // The witFileFormat represented by this CaseToken,
         // if myCaseKind == WIT_FILE_FORMAT.

      int myMsgGroupNo_;
         //
         // The message group # represented by this CaseToken,
         // if myCaseKind == MSG_GROUP_NO.

      WriteSchedFunc myWriteSchedFunc_;
         //
         // The WriteSchedFunc represented by this CaseToken,
         // if myCaseKind == WRITE_SCHED_FUNC.

      DemPggFunc myDemPggFunc_;
         //
         // The DemPggFunc represented by this CaseToken,
         // if myCaseKind == DEM_PGG_CASE.

      OpnPggFunc myOpnPggFunc_;
         //
         // The OpnPggFunc represented by this CaseToken,
         // if myCaseKind == OPN_PGG_CASE.
      };
   };
};
//
// End of namespace WitJ.

#endif
