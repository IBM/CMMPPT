//==============================================================================
// IBM Confidential
//
// OCO Source Materials
//
// Watson Implosion Technology
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//
// The Source code for this program is not published or otherwise divested of
// its trade secrets, irrespective of what has been deposited with the U. S.
// Copyright office.
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "ObjVec.C"
//
// Contains the implementation of the following classes:
//
//    ObjVec
//    ObjVecIR
//    SglObjVecIR
//    MultiObjVecIR
//------------------------------------------------------------------------------

#include <ObjVec.h>
#include <SglObjVecIR.h>
#include <MultiObjVecIR.h>
#include <MultiObjMgr.h>
#include <Objective.h>
#include <Problem.h>
#include <OptComp.h>
#include <DelComp.h>
#include <DataWrit.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class ObjVec.
//------------------------------------------------------------------------------

WitObjVec::WitObjVec (WitProblem * theProblem, double initValue):

      myObjVecIR_ (newObjVecIR (theProblem, initValue))
   {
   }

//------------------------------------------------------------------------------

WitObjVec::~WitObjVec ()
   {
   delete myObjVecIR_;
   }

//------------------------------------------------------------------------------

void WitObjVec::operator = (const WitObjVec & theObjVec)
   {
   theObjVec.myObjVecIR_->copyInto (myObjVecIR_);
   }

//------------------------------------------------------------------------------

void WitObjVec::writeDataMulti (
      const char *   attrName,
      WitDelComp *   theDelComp,
      WitObjective * theObj)
      const
   {
   myObjVecIR_->writeDataMulti (attrName, theDelComp, theObj);
   }

//------------------------------------------------------------------------------

void WitObjVec::display (WitMsgID theMsgID, WitProblem * theProblem) const
   {
   myObjVecIR_->display (theMsgID, theProblem);
   }

//------------------------------------------------------------------------------

WitObjVecIR * WitObjVec::newObjVecIR (WitProblem * theProblem, double initValue)
   {
   if (theProblem->myOptComp ()->multiObjMode ())
      {
      return
         new WitMultiObjVecIR  (
            theProblem->myOptComp ()->myMultiObjMgr (),
            initValue);
      }
   else
      {
      return new WitSglObjVecIR (theProblem, initValue);
      }
   }

//------------------------------------------------------------------------------
// Implementation of class ObjVecIR.
//------------------------------------------------------------------------------

WitObjVecIR::~WitObjVecIR ()
   {
   }

//------------------------------------------------------------------------------

void WitObjVecIR::copyFrom (WitSglObjVecIR *)
   {
   stronglyAssert (false);
   }

//------------------------------------------------------------------------------

void WitObjVecIR::copyFrom (WitMultiObjVecIR *)
   {
   stronglyAssert (false);
   }

WitObjVecIR::WitObjVecIR ()
   {
   }

//------------------------------------------------------------------------------
// Implementation of class SglObjVecIR.
//------------------------------------------------------------------------------

WitSglObjVecIR::WitSglObjVecIR (WitProblem * theProblem, double initValue):

      WitObjVecIR   (),
      myDblFlexVec_ (theProblem, initValue)
   {
   }

//------------------------------------------------------------------------------

WitSglObjVecIR::~WitSglObjVecIR ()
   {
   }

//------------------------------------------------------------------------------

void WitSglObjVecIR::copyInto (WitObjVecIR * theObjVecIR)
   {
   theObjVecIR->copyFrom (this);
   }

//------------------------------------------------------------------------------

void WitSglObjVecIR::copyFrom (WitSglObjVecIR * theSglObjVecIR)
   {
   myDblFlexVec_ = theSglObjVecIR->myDblFlexVec_;
   }

//------------------------------------------------------------------------------

void WitSglObjVecIR::writeDataMulti (const char *, WitDelComp *, WitObjective *)
   {
   stronglyAssert (false);
   }
   
//------------------------------------------------------------------------------

void WitSglObjVecIR::display (WitMsgID theMsgID, WitProblem * theProblem)
   {
   theProblem->display (theMsgID, myDblFlexVec_);
   }

//------------------------------------------------------------------------------

WitDblFlexVec & WitSglObjVecIR::curDblFlexVec ()
   {
   return myDblFlexVec_;
   }

//------------------------------------------------------------------------------
// Implementation of class MultiObjVecIR.
//------------------------------------------------------------------------------

WitMultiObjVecIR::WitMultiObjVecIR (
         WitMultiObjMgr * theMultiObjMgr,
         double           initValue):

      WitObjVecIR      (),
      myMultiObjMgr_   (theMultiObjMgr),
      myDblFlexVecMap_ (theMultiObjMgr->myProblem ())
   {
   WitObjective * theObj;

   forEachEl (theObj, myMultiObjMgr_->myObjectives ())
      {
      myDblFlexVecMap_ (theObj) =
         new WitDblFlexVec (myMultiObjMgr_->myProblem (), initValue);
      }
   }

//------------------------------------------------------------------------------

WitMultiObjVecIR::~WitMultiObjVecIR ()
   {
   WitObjective * theObj;

   forEachEl (theObj, myMultiObjMgr_->myObjectives ())
      {
      delete myDblFlexVecMap_ (theObj);
      }
   }

//------------------------------------------------------------------------------

void WitMultiObjVecIR::copyInto (WitObjVecIR * theObjVecIR)
   {
   theObjVecIR->copyFrom (this);
   }

//------------------------------------------------------------------------------

void WitMultiObjVecIR::copyFrom (WitMultiObjVecIR * theMultiObjVecIR)
   {
   WitObjective * theObj;

   forEachEl (theObj, myMultiObjMgr_->myObjectives ())
      {
      myDblFlexVecMap_.myElemAt (theObj) =
         theMultiObjVecIR->
            myDblFlexVecMap_.myElemAt (theObj);
      }
   }

//------------------------------------------------------------------------------

void WitMultiObjVecIR::writeDataMulti (
      const char *   attrName,
      WitDelComp *   theDelComp,
      WitObjective * theObj)
   {
   if (myDblFlexVecMap_.myElemAt (theObj) == 0.0)
      return;

   theDelComp->writeSetClause ();

   myMultiObjMgr_->
      myProblem ()->
         myDataWriter ()->
            writeVector (attrName, myDblFlexVecMap_.myElemAt (theObj), 0.0);

   myMultiObjMgr_->
      myProblem ()->
         myDataWriter ()->writeEndCommand   ();
   }
   
//------------------------------------------------------------------------------

void WitMultiObjVecIR::display (WitMsgID theMsgID, WitProblem * theProblem)
   {
   WitObjective * theObj;

   forEachEl (theObj, myMultiObjMgr_->myObjectives ())
      {
      myMultiObjMgr_->myMsgFac () ("objNameDdMsg", theObj->myObjName ());

      theProblem->display (theMsgID, myDblFlexVecMap_.myElemAt (theObj));
      }
   }

//------------------------------------------------------------------------------

WitDblFlexVec & WitMultiObjVecIR::curDblFlexVec ()
   {
   return myDblFlexVecMap_.myElemAt (myMultiObjMgr_->currentObjective ());
   }
