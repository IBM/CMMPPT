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
// Source file: "CpxParSpec.C"
//
// Contains the implementation of the following classes: 
//
//    CpxParSpecMgr
//    CpxParSpec
//------------------------------------------------------------------------------

#include <CpxParSpecMgr.h>
#include <CpxParSpec.h>
#include <DataWrit.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class CpxParSpecMgr.
//------------------------------------------------------------------------------

WitCpxParSpecMgr::WitCpxParSpecMgr (WitProblem * theProblem):

      WitProbAssoc        (theProblem),
      myCpxParSpecs_      (),
      cplexParSpecName_   (defCplexParSpecName   ()),
      cplexParSpecIntVal_ (defCplexParSpecIntVal ()),
      cplexParSpecDblVal_ (defCplexParSpecDblVal ()),
      addingSpecsByVal_   (false)
   {
   }

//------------------------------------------------------------------------------

WitCpxParSpecMgr::~WitCpxParSpecMgr ()
   {
   clearCpxParSpecs ();
   }

//------------------------------------------------------------------------------

void WitCpxParSpecMgr::setCplexParSpecName (const char * theValue)
   {
   witAssert (theValue != NULL);

   myProblem ()->prepSetNonUnpostAttr ();

   cplexParSpecName_ = theValue;

   addingSpecsByVal_ = (cplexParSpecName_ != "NO_PARAM");
   }

//------------------------------------------------------------------------------

void WitCpxParSpecMgr::setCplexParSpecIntVal (int theValue)
   {
   myProblem ()->prepSetNonUnpostAttr ();

   cplexParSpecIntVal_ = theValue;

   if (addingSpecsByVal_)
      addIntCpxParSpec (cplexParSpecName_.myCstring (), theValue);
   }

//------------------------------------------------------------------------------

void WitCpxParSpecMgr::setCplexParSpecDblVal (double theValue)
   {
   myProblem ()->prepSetNonUnpostAttr ();

   cplexParSpecDblVal_ = theValue;

   if (addingSpecsByVal_)
      addDblCpxParSpec (cplexParSpecName_.myCstring (), theValue);
   }

//------------------------------------------------------------------------------

void WitCpxParSpecMgr::addIntCpxParSpec (const char * theName, int theValue)
   {
   WitCpxParSpec * theSpec;

   witAssert (not equalCstrings (theName, "NO_PARAM"));

   deleteMatchingCpxParSpec (theName);

   theSpec = new WitCpxParSpec (myProblem (), theName, theValue);

   myCpxParSpecs_.append (theSpec);

   myMsgFac () ("addIntCpxParSpecMsg", theName, theValue);
   }

//------------------------------------------------------------------------------

void WitCpxParSpecMgr::addDblCpxParSpec (const char * theName, double theValue)
   {
   WitCpxParSpec * theSpec;

   witAssert (not equalCstrings (theName, "NO_PARAM"));

   deleteMatchingCpxParSpec (theName);

   theSpec = new WitCpxParSpec (myProblem (), theName, theValue);

   myCpxParSpecs_.append (theSpec);

   myMsgFac () ("addDblCpxParSpecMsg", theName, theValue);
   }

//------------------------------------------------------------------------------

WitCpxParSpec * WitCpxParSpecMgr::findCpxParSpec (const char * theName)
   {
   WitCpxParSpec * theSpec;

   forEachEl (theSpec, myCpxParSpecs_)
      if (theSpec->myName () == theName)
         return theSpec;

   return NULL;
   }

//------------------------------------------------------------------------------

void WitCpxParSpecMgr::clearCpxParSpecs ()
   {
   while (not myCpxParSpecs_.isEmpty ())
      {
      delete myCpxParSpecs_.get ();
      }
   }

//------------------------------------------------------------------------------

void WitCpxParSpecMgr::display ()
   {
   WitCpxParSpec * theSpec;

   myMsgFac () ("cpxParSpecAttsDdMsg",
      cplexParSpecName_,
      cplexParSpecIntVal_,
      cplexParSpecDblVal_);

   if (myCpxParSpecs_.isEmpty ())
      return;

   myMsgFac () ("cpxParSpecsHeadDdMsg");

   forEachEl (theSpec, myCpxParSpecs_)
      theSpec->display ();
   }

//------------------------------------------------------------------------------

void WitCpxParSpecMgr::copyAttrsFrom (WitCpxParSpecMgr * theCpxParSpecMgr)
   {
   WitCpxParSpec * theSpec;
   WitCpxParSpec * dupSpec;

   cplexParSpecName_   = theCpxParSpecMgr->cplexParSpecName_;
   cplexParSpecIntVal_ = theCpxParSpecMgr->cplexParSpecIntVal_;
   cplexParSpecDblVal_ = theCpxParSpecMgr->cplexParSpecDblVal_;

   clearCpxParSpecs ();

   forEachEl (theSpec, myCpxParSpecs_)
      {
      dupSpec = new WitCpxParSpec (myProblem (), theSpec);

      myCpxParSpecs_.append (dupSpec);
      }
   }

//------------------------------------------------------------------------------

void WitCpxParSpecMgr::writeData ()
   {
   WitCpxParSpec * theSpec;
   WitString       noParam;

   if (myCpxParSpecs_.isEmpty ())
      if       (cplexParSpecName_   == defCplexParSpecName   ())
         if    (cplexParSpecIntVal_ == defCplexParSpecIntVal ())
            if (cplexParSpecDblVal_ == defCplexParSpecDblVal ())
               return;

   forEachEl (theSpec, myCpxParSpecs_)
      theSpec->writeData ();

   myDataWriter ()->writeFirstKeyWord ("set");
   myDataWriter ()->writeKeyWord      ("problem");

   noParam = "NO_PARAM";

   myDataWriter ()->writeString ("cplexParSpecName",   noParam);
   myDataWriter ()->writeInt    ("cplexParSpecIntVal", cplexParSpecIntVal_);
   myDataWriter ()->writeDouble ("cplexParSpecDblVal", cplexParSpecDblVal_);

   myDataWriter ()->writeString ("cplexParSpecName",   cplexParSpecName_,
                                                       "NO_PARAM");

   myDataWriter ()->writeEndCommand ();
   }

//------------------------------------------------------------------------------

void WitCpxParSpecMgr::deleteMatchingCpxParSpec (const char * theName)
   {
   WitCpxParSpec *         matchSpec;
   WitCpxParSpec *         theSpec;
   WitList <WitCpxParSpec> unmatchSpecs;

   matchSpec = findCpxParSpec (theName);

   if (matchSpec == NULL)
      return;

   forEachEl (theSpec, myCpxParSpecs_)
      if (theSpec != matchSpec)
         {
         unmatchSpecs.append (theSpec);
         }

   myCpxParSpecs_ = unmatchSpecs;

   delete matchSpec;
   }

//------------------------------------------------------------------------------
// Implementation of class CpxParSpec.
//------------------------------------------------------------------------------

WitCpxParSpec::WitCpxParSpec (
         WitProblem * theProblem,
         const char * theName,
         int          theIntVal):

      WitProbAssoc  (theProblem),
      myName_       (theName),
      valTypeIsInt_ (true),
      myIntVal_     (theIntVal),
      myDblVal_     (0.0)
   {
   }

//------------------------------------------------------------------------------

WitCpxParSpec::WitCpxParSpec (
         WitProblem * theProblem,
         const char * theName,
         double       theDblVal):

      WitProbAssoc  (theProblem),
      myName_       (theName),
      valTypeIsInt_ (false),
      myIntVal_     (0),
      myDblVal_     (theDblVal)
   {
   }

//------------------------------------------------------------------------------

WitCpxParSpec::WitCpxParSpec (
      WitProblem *    theProblem,
      WitCpxParSpec * theSpec):

      WitProbAssoc  (theProblem),
      myName_       (theSpec->myName_),
      valTypeIsInt_ (theSpec->valTypeIsInt_),
      myIntVal_     (theSpec->myIntVal_),
      myDblVal_     (theSpec->myDblVal_)
   {
   }

//------------------------------------------------------------------------------

WitCpxParSpec::~WitCpxParSpec ()
   {
   }

//------------------------------------------------------------------------------

int WitCpxParSpec::myIntVal () const
   {
   witAssert (valTypeIsInt_);

   return myIntVal_;
   }

//------------------------------------------------------------------------------

double WitCpxParSpec::myDblVal () const
   {
   witAssert (not valTypeIsInt_);

   return myDblVal_;
   }

//------------------------------------------------------------------------------

void WitCpxParSpec::display ()
   {
   if (valTypeIsInt_)
      myMsgFac () ("intCpxParSpecDdMsg", myName_, myIntVal_);
   else
      myMsgFac () ("dblCpxParSpecDdMsg", myName_, myDblVal_);
   }

//------------------------------------------------------------------------------

void WitCpxParSpec::writeData ()
   {
   myDataWriter    ()->writeFirstKeyWord ("set");
   myDataWriter    ()->writeKeyWord      ("problem");

   myDataWriter    ()->writeString       ("cplexParSpecName", myName_);

   if (valTypeIsInt_)
      myDataWriter ()->writeInt          ("cplexParSpecIntVal", myIntVal_);
   else
      myDataWriter ()->writeDouble       ("cplexParSpecDblVal", myDblVal_);

   myDataWriter    ()->writeEndCommand   ();
   }
