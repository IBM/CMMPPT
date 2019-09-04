//==============================================================================
// Message Class Library
//
// (C) Copyright IBM Corp. 1997, 2005  All Rights Reserved
//==============================================================================

#ifndef mcl_set_h
#define mcl_set_h

//-----------------------------------------------------------------------------
// File set.h
//
// Contains the declaration of the following classes:
//
// MsgUnitSet
//-----------------------------------------------------------------------------

#include <map>

class MclMsgUnitSet
   {
   friend class MclMsgUnitItr;

   public:
      MclMsgUnitSet ();

     ~MclMsgUnitSet ();

     void insert (MclMsgUnit * theMsgUnit);
        //
        // theMsgUnit into this MsgUnitSet.

     MclMsgUnit * findValue (const char * msgId);
        //
        // Returns the key, or nil if no match was found

   private:

      std::map      <std::string, MclMsgUnit *> myMsgUnitsById_;
      std::multimap <int,         MclMsgUnit *> myMsgUnitsByNum_;
   };


#endif
