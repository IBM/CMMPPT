//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "Param.C"
//
// Contains the implementation of the following classes:
//
//    Param
//    StringParam
//    IntParam
//    DoubleParam
//    ChoiceParm
//------------------------------------------------------------------------------

#include <Param.h>
#include <ParamMgr.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class Param.
//------------------------------------------------------------------------------

WitParam::~WitParam ()
   {
   }

//------------------------------------------------------------------------------

const WitString & WitParam::itsName (const WitParam * theParam)
   {
   return theParam->myName ();
   }

//------------------------------------------------------------------------------

WitParam::WitParam (
         const char *  theName,
         const char *  theText,
         WitParamMgr * theParamMgr):

      myName_     (theName),
      myText_     (theText),
      myParamMgr_ (theParamMgr),
      myMsgFac_   (theParamMgr->myMsgFac ())
   {
   stronglyAssert (myName_.length () <= 15);
   stronglyAssert (myText_.length () <= 39);

   myParamMgr ()->store (this);
   }

//------------------------------------------------------------------------------
// Implementation of class StringParam.
//------------------------------------------------------------------------------

WitStringParam::WitStringParam (
         const char *  theName,
         const char *  theText,
         const char *  initValue,
         WitParamMgr * theParamMgr):

      WitParam (theName, theText, theParamMgr),

      myValue_ (initValue)
   {
   }

//------------------------------------------------------------------------------

WitStringParam::~WitStringParam ()
   {
   }

//------------------------------------------------------------------------------

void WitStringParam::parse (const WitString & theToken)
   {
   myValue_ = theToken;
   }

//------------------------------------------------------------------------------

void WitStringParam::print ()
   {
   myMsgFac () ("stringParamMsg", myName (), myText (), myValue ());
   }

//------------------------------------------------------------------------------

const WitString & WitStringParam::itsName (
      const WitStringParam * theStringParam)
   {
   return theStringParam->myName ();
   }

//------------------------------------------------------------------------------
// Implementation of class IntParam.
//------------------------------------------------------------------------------

WitIntParam::WitIntParam (
         const char *  theName,
         const char *  theText,
         int           theLB,
         int           theUB,
         int           initValue,
         WitParamMgr * theParamMgr):

      WitParam (theName, theText, theParamMgr),

      lb_      (theLB),
      ub_      (theUB),
      myValue_ (0)
   {
   stronglyAssert (lbSatisfied (initValue));
   stronglyAssert (ubSatisfied (initValue));

   myValue_ = initValue;
   }

//------------------------------------------------------------------------------

WitIntParam::~WitIntParam ()
   {
   }

//------------------------------------------------------------------------------

bool WitIntParam::lbSatisfied (int theValue)
   {
   return
      (lb_ < 0)?
         true:
         (theValue >= lb_);
   }

//------------------------------------------------------------------------------

bool WitIntParam::ubSatisfied (int theValue)
   {
   return
      (ub_ < 0)?
         true:
         (theValue <= ub_);
   }

//------------------------------------------------------------------------------

void WitIntParam::parse (const WitString & theToken)
   {
   int scanResult;
   int theValue;

   scanResult = sscanf (theToken.myCstring (), "%d", & theValue);

   if (scanResult != 1)
      myMsgFac () (
         "paramMustBeIntSmsg",
         myParamMgr ()->fileName (),
         myName (),
         theToken);

   if (not lbSatisfied (theValue))
      myMsgFac () (
         "intParamTooLowSmsg",
         myParamMgr ()->fileName (),
         myName (),
         theToken,
         lb_);

   if (not ubSatisfied (theValue))
      myMsgFac () (
         "intParamTooHighSmsg",
         myParamMgr ()->fileName (),
         myName (),
         theToken,
         ub_);

   myValue_ = theValue;
   }

//------------------------------------------------------------------------------

void WitIntParam::print ()
   {
   myMsgFac () ("intParamMsg", myName (), myText (), myValue ());
   }

//------------------------------------------------------------------------------

const WitString & WitIntParam::itsName (const WitIntParam * theIntParam)
   {
   return theIntParam->myName ();
   }

//------------------------------------------------------------------------------
// Implementation of class DoubleParam.
//------------------------------------------------------------------------------

WitDoubleParam::WitDoubleParam (
         const char *  theName,
         const char *  theText,
         double        theLB,
         double        initValue,
         WitParamMgr * theParamMgr):

      WitParam (theName, theText, theParamMgr),

      lb_      (theLB),
      myValue_ (0.0)
   {
   stronglyAssert (lbSatisfied (initValue));

   myValue_ = initValue;
   }

//------------------------------------------------------------------------------

WitDoubleParam::~WitDoubleParam ()
   {
   }

//------------------------------------------------------------------------------

bool WitDoubleParam::lbSatisfied (double theValue)
   {
   return
      (lb_ < 0.0)?
         true:
         (theValue >= lb_ * .999);
   }

//------------------------------------------------------------------------------

void WitDoubleParam::parse (const WitString & theToken)
   {
   int    scanResult;
   double theValue;

   scanResult = sscanf (theToken.myCstring (), "%lf", & theValue);

   if (scanResult != 1)
      myMsgFac () (
         "paramMustBeDblSmsg",
         myParamMgr ()->fileName (),
         myName (),
         theToken);

   if (not lbSatisfied (theValue))
      myMsgFac () (
         "dblParamTooLowSmsg",
         myParamMgr ()->fileName (),
         myName (),
         theToken,
         lb_);

   myValue_ = theValue;
   }

//------------------------------------------------------------------------------

void WitDoubleParam::print ()
   {
   myMsgFac () ("dblParamMsg", myName (), myText (), myValue ());
   }

//------------------------------------------------------------------------------

const WitString & WitDoubleParam::itsName (
      const WitDoubleParam * theDoubleParam)
   {
   return theDoubleParam->myName ();
   }

//------------------------------------------------------------------------------
// Implementation of class ChoiceParam.
//------------------------------------------------------------------------------

WitChoiceParam::WitChoiceParam (
         const char *  theName,
         const char *  theText,
         WitParamMgr * theParamMgr):

      WitParam (theName, theText, theParamMgr),

      allChoices_ (),
      myValue_    (NULL)
   {
   }

//------------------------------------------------------------------------------

WitChoiceParam::~WitChoiceParam ()
   {
   while (not allChoices_.isEmpty ())
      delete allChoices_.get ();
   }

//------------------------------------------------------------------------------

void WitChoiceParam::parse (const WitString & theToken)
   {
   const WitString * theChoice;
   WitString *       validChoice;

   theChoice =
      allChoices_.find (& WitString::thisString, theToken.myCstring ());

   if (theChoice == NULL)
      {
      myMsgFac () (
         "paramReadErrorInvalidChoiceMain",
         myParamMgr ()->fileName (),
         myName (),
         theToken);

      forEachEl (validChoice, allChoices_)
         myMsgFac () ("paramReadErrorInvalidChoiceAlt", (* validChoice));

      myMsgFac () ("paramReadErrorInvalidChoiceEnd");
      }

   myValue_ = theChoice;
   }

//------------------------------------------------------------------------------

void WitChoiceParam::print ()
   {
   myMsgFac () ("stringParamMsg", myName (), myText (), myValue ());
   }

//------------------------------------------------------------------------------

void WitChoiceParam::addChoice (const char * theChoice)
   {
   allChoices_.append (new WitString (theChoice));
   }

//------------------------------------------------------------------------------

void WitChoiceParam::choose (const char * theCstring)
   {
   const WitString * theChoice;

   theChoice = allChoices_.find (& WitString::thisString, theCstring);

   stronglyAssert (theChoice != NULL);

   myValue_ = theChoice;
   }
//------------------------------------------------------------------------------

const WitString & WitChoiceParam::itsName (
      const WitChoiceParam * theChoiceParam)
   {
   return theChoiceParam->myName ();
   }
