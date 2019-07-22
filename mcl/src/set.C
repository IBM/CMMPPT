//==============================================================================
// IBM Confidential
//
// OCO Source Materials
//
// Message Class Library
//
// (C) Copyright IBM Corp. 1997, 2005  All Rights Reserved
//
// The Source code for this program is not published or otherwise divested of
// its trade secrets, irrespective of what has been deposited with the U. S.
// Copyright office.
//==============================================================================

//-----------------------------------------------------------------------------
// File set.C
//
// Contains the implementation of the following classes:
//
// MsgUnitSet
//-----------------------------------------------------------------------------

#include <assert.h>
#include <mcl/src/msg.h>
#include <mcl/src/set.h>

//-----------------------------------------------------------------------------
// Implementation of class MsgUnitSet
//-----------------------------------------------------------------------------

MclMsgUnitSet::MclMsgUnitSet ()
   {
   }

//-----------------------------------------------------------------------------

MclMsgUnitSet::~MclMsgUnitSet ()
   {
   std::map <std::string, MclMsgUnit *>:: iterator theItr;

   myMsgUnitsByNum_.clear ();

   theItr = myMsgUnitsById_.begin ();

   while (theItr != myMsgUnitsById_.end ())
      {
      delete theItr->second;

      theItr->second = NULL;

      theItr ++;
      }
   }

//-----------------------------------------------------------------------------

void MclMsgUnitSet::insert (MclMsgUnit * theMsgUnit)
   {
   int theExtNum;

   myMsgUnitsById_[theMsgUnit->id ()] = theMsgUnit;

   theExtNum =
      theMsgUnit->asaMsg ()?
         theMsgUnit->asaMsg ()->externalNumber ():
         -1;

   myMsgUnitsByNum_.insert (std::make_pair (theExtNum, theMsgUnit));
   }

//-----------------------------------------------------------------------------

MclMsgUnit * MclMsgUnitSet::findValue (const char * msgId)
   {
   std::map <std::string, MclMsgUnit *>::iterator theItr;

   theItr = myMsgUnitsById_.find (msgId);

   return
      (theItr == myMsgUnitsById_.end ())?
         NULL:
         theItr->second;
   }
