//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "MultiObj.C"
//
// Contains the implementation of the following classes:
//
//    MultiObjMgr
//    Objective
//------------------------------------------------------------------------------

#include <MultiObjMgr.h>
#include <Objective.h>
#include <OptComp.h>
#include <GlobalComp.h>
#include <CompMgr.h>
#include <DetOptImpMgr.h>
#include <DataWrit.h>
#include <Pre.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class MultiObjMgr.
//------------------------------------------------------------------------------

WitMultiObjMgr::WitMultiObjMgr (WitProblem * theProblem):

      WitProbAssoc      (theProblem),
      myObjectives_     (),
      currentObjective_ (NULL),
      multiObjTol_      (defMultiObjTol ())
   {
   WitObjective * theObj;

   theObj            = new WitObjective (this, "Default");

   myObjectives_.append (theObj);

   currentObjective_ = theObj;
   }

//------------------------------------------------------------------------------

WitMultiObjMgr::~WitMultiObjMgr ()
   {
   clearObjectives ();
   }

//------------------------------------------------------------------------------

void WitMultiObjMgr::copyAttrsFrom (WitMultiObjMgr * theMultiObjMgr)
   {
   WitObjective * srcObj;
   const char *   theObjName;
   WitObjective * dstObj;

   currentObjective_ = NULL;

   clearObjectives ();

   forEachEl (srcObj, theMultiObjMgr->myObjectives_)
      {
      theObjName = srcObj->myObjName ().myCstring ();

      dstObj     = new WitObjective (this, theObjName);

      myObjectives_.append (dstObj);

      dstObj->copyAttrsFrom (srcObj);

      if (theMultiObjMgr->currentObjective_ == srcObj)
         {
         currentObjective_ = dstObj;
         }
      }

   stronglyAssert (currentObjective_ != NULL);

   multiObjTol_ = theMultiObjMgr->multiObjTol_;
   }

//------------------------------------------------------------------------------

void WitMultiObjMgr::writeDataAttrs ()
   {
   WitString      theObjListSpec;
   WitObjective * theObj;

   getObjectiveListSpec (theObjListSpec);

   myDataWriter ()->writeString (
        "objectiveListSpec",
            theObjListSpec,
      defObjectiveListSpec ());

   myDataWriter ()->writeDouble (
        "multiObjTol",
         multiObjTol (),
      defMultiObjTol ());

   forEachEl (theObj, myObjectives_)
      {
      theObj->writeDataAttrs ();
      }
   }

//------------------------------------------------------------------------------

void WitMultiObjMgr::writeMultiObjVecs ()
   {
   WitObjective * theObj;
   WitComponent * theComp;

   forEachEl (theObj, myObjectives_)
      {
      theObj->writeObjNameAsCurrent ();

      forEachEl (theComp, myCompMgr ()->allComponents ())
         {
         theComp->writeMultiObjVecs (theObj);
         }
      }

   currentObjective_->writeObjNameAsCurrent ();
   }

//------------------------------------------------------------------------------

void WitMultiObjMgr::display ()
   {
   WitObjective * theObj;
   WitString      theObjListSpec;

   myMsgFac () ("objHeadingDdMsg");

   forEachEl (theObj, myObjectives_)
      {
      theObj->display ();
      }

   getObjectiveListSpec (theObjListSpec);

   myMsgFac () ("multiObjAttDdMsg",
      theObjListSpec,
      currentObjective_->myObjName (),
      multiObjTol_);
   }

//------------------------------------------------------------------------------

WitObjective * WitMultiObjMgr::findObjective (const char * theObjName)
   {
   WitObjective * theObj;

   forEachEl (theObj, myObjectives_)
      {
      if (theObj->myObjName () == theObjName)
         {
         return theObj;
         }
      }

   return NULL;
   }

//------------------------------------------------------------------------------

void WitMultiObjMgr::validateData ()
   {
   if (myGlobalComp ()->computeCriticalList ())
      {
      myMsgFac () ("globAttMultiObjOptImpSmsg", "computeCriticalList");
      }

   if (myOptComp ()->compPrices ())
      {
      myMsgFac () ("globAttMultiObjOptImpSmsg", "compPrices");
      }

   if (myOptComp ()->accAfterOptImp ())
      {
      myMsgFac () ("globAttMultiObjOptImpSmsg", "accAfterOptImp");
      }

   checkUniqueSeqNos ();
   }

//------------------------------------------------------------------------------

void WitMultiObjMgr::writeObjValues ()
   {
   WitObjective * theObj;

   myMsgFac () ("blankMsg");

   if (myOptComp ()->boundsValueValid ())
      {
      myMsgFac () ("boundsValueMsg", myOptComp ()->boundsValue ());
      }

   forEachEl (theObj, myObjectives_)
      {
      myMsgFac () ("multiObjValueMsg",
         theObj->myObjName (),
         theObj->objValue  ());
      }
   }

//------------------------------------------------------------------------------

void WitMultiObjMgr::setObjectiveList (
      const WitVector <const char *> & theObjNames)
   {
   int            theIdx;
   WitObjective * theObj;

   stronglyAssert (myCompMgr ()->nNodes () == 0);

   stronglyAssert (theObjNames.length () >= 1);

   myProblem ()->prepSetUnpreAttr ();

   clearObjectives ();

   for (theIdx = 0; theIdx < theObjNames.length (); theIdx ++)
      {
      theObj = new WitObjective (this, theObjNames[theIdx]);

      myObjectives_.append (theObj);
      }

   currentObjective_ = myObjectives_.first ();
   }

//------------------------------------------------------------------------------

void WitMultiObjMgr::setCurrentObjective (WitObjective * theObj)
   {
   witAssert (theObj->myProblem () == myProblem ());

   myProblem ()->prepSetNonUnpostAttr ();

   currentObjective_ = theObj;
   }

//------------------------------------------------------------------------------

void WitMultiObjMgr::setMultiObjTol (double theValue)
   {
   stronglyAssert (theValue >= 0.0);

   stronglyAssert (theValue <= 0.001001);

   multiObjTol_ = theValue;
   }

//------------------------------------------------------------------------------

void WitMultiObjMgr::getObjectiveListSpec (WitString & theObjListSpec)
   {
   int            theStrLength;
   WitObjective * theObj;
   char *         curPos;

   theStrLength = -1;

   forEachEl (theObj, myObjectives_)
      {
      theStrLength += theObj->myObjName ().length () + 1;
      }

   theObjListSpec.resize (theStrLength);
   
   curPos = theObjListSpec.myCstringForUpdate ();

   forEachEl (theObj, myObjectives_)
      {
      if (theObj != myObjectives_.first ())
         {
         * curPos = '|';

         curPos ++;
         }

      theObj->myObjName ().copyInto (curPos);

      curPos += theObj->myObjName ().length ();
      }

   stronglyAssert (curPos == theObjListSpec.myCstring () + theStrLength);
   }

//------------------------------------------------------------------------------

void WitMultiObjMgr::clearObjectives ()
   {
   currentObjective_ = NULL;

   while (not myObjectives_.isEmpty ())
      {
      delete myObjectives_.get ();
      }
   }

//------------------------------------------------------------------------------

void WitMultiObjMgr::checkUniqueSeqNos ()
   {
   WitPtrVec <WitObjective> theObjsBySeqNo;
   WitObjective *           theObj;
   int                      theSeqNo;

   theObjsBySeqNo.resize (myObjectives_.nElements () + 1);

   forEachEl (theObj, myObjectives_)
      {
      theSeqNo = theObj->objectiveSeqNo ();

      if (theObjsBySeqNo[theSeqNo] != NULL)
         {
         myMsgFac () ("dupObjSeqNoSmsg",
            theObjsBySeqNo[theSeqNo]->myObjName (),
            theObj                  ->myObjName (),
            theSeqNo);
         }

      theObjsBySeqNo[theSeqNo] = theObj;
      }
   }

//------------------------------------------------------------------------------
// Implementation of class Objective.
//------------------------------------------------------------------------------

WitObjective::WitObjective (
      WitMultiObjMgr * theMultiObjMgr,
      const char *     theObjName):

      WitProbAssoc    (theMultiObjMgr),
      myMultiObjMgr_  (theMultiObjMgr),
      mappingIndex_   (-1),
      myObjName_      (theObjName),
      objectiveSeqNo_ (),
      objValue_       (0.0)
   {
   stronglyAssert (not myPreprocessor ()->preprocessed ());

   stronglyAssert (myMultiObjMgr_->findObjective (theObjName) == NULL);

   stronglyAssert (not containsVBar (theObjName));

   mappingIndex_   = theMultiObjMgr->myObjectives ().nElements ();

   objectiveSeqNo_ = defObjectiveSeqNo ();
   }

//------------------------------------------------------------------------------

WitObjective::~WitObjective ()
   {
   }

//------------------------------------------------------------------------------

void WitObjective::copyAttrsFrom (WitObjective * theObjective)
   {
   objectiveSeqNo_ = theObjective->objectiveSeqNo_;
   }

//------------------------------------------------------------------------------

void WitObjective::writeDataAttrs ()
   {
   myMsgFac () ("blankMsg");

   myDataWriter ()->writeString ("currentObjective", myObjName_, "");

   myDataWriter ()->writeInt (
        "objectiveSeqNo",
         objectiveSeqNo (),
      defObjectiveSeqNo ());
   }

//------------------------------------------------------------------------------

void WitObjective::writeObjNameAsCurrent ()
   {
   myDataWriter ()->writeFirstKeyWord ("set");
   myDataWriter ()->writeKeyWord      ("problem");
   myDataWriter ()->writeString       ("currentObjective", myObjName_);
   myDataWriter ()->writeEndCommand   ();
   }

//------------------------------------------------------------------------------

void WitObjective::display ()
   {
   myMsgFac () ("objectiveDdMsg", objectiveSeqNo_, myObjName_);
   }

//------------------------------------------------------------------------------

bool WitObjective::containsVBar (const char * theCstring)
   {
   return (strpbrk (theCstring, "|") != NULL);
   }

//------------------------------------------------------------------------------

void WitObjective::storeOptObjValue (double theValue)
   {
   witAssert (myDetOptImpMgr ()->uploadingSoln ());

   objValue_ = theValue;
   }

//------------------------------------------------------------------------------

bool WitObjective::hasNegativeCosts ()
   {
   WitObjective * savedObj;
   bool           hasNegCosts;

   savedObj    = myMultiObjMgr_->currentObjective ();

   myMultiObjMgr_->setCurrentObjective (this);

   hasNegCosts = myOptComp ()->negativeCostsExist ();

   myMultiObjMgr_->setCurrentObjective (savedObj);

   return hasNegCosts;
   }

//------------------------------------------------------------------------------

int WitObjective::nInstances (WitProblem * theProblem)
   {
   return
      theProblem->myOptComp ()->myMultiObjMgr ()->myObjectives ().nElements ();
   }

//------------------------------------------------------------------------------

int WitObjective::defObjectiveSeqNo ()
   {
   return mappingIndex_ + 1;
   }

//------------------------------------------------------------------------------

void WitObjective::setObjectiveSeqNo (int theSeqNo)
   {
   stronglyAssert (theSeqNo >= 1);
   stronglyAssert (theSeqNo <= myMultiObjMgr_->myObjectives ().nElements ());

   objectiveSeqNo_ = theSeqNo;
   }
