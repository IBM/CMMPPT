//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "PipReqMgr.C"
//
// Contains the implementation of class PipReqMgr.
//------------------------------------------------------------------------------

#include <PipReqMgr.h>
#include <PipReqSpec.h>
#include <PipReqBasis.h>
#include <PipReqClient.h>
#include <PipMgr.h>
#include <TripStack.h>
#include <GlobalComp.h>
#include <Material.h>
#include <Operation.h>
#include <BopEntry.h>
#include <Demand.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class PipReqMgr.
//------------------------------------------------------------------------------

WitPipReqMgr::WitPipReqMgr (WitProblem * theProblem):

      WitProbAssoc    (theProblem),
      myDemClients_   (),
      myOpnClients_   (),
      myBases_        (theProblem),
      myPartReqs_     (),
      myBopEntReqs_   (),
      myReqSpecVec_   (),
      nActReqSpecs_   (0),
      maxActReqSpecs_ (0),
      curReqList_     (myProblem ()),
      myReqListItr_   (),
      curBasis_       (NULL),
      incReqVolCur_   (0.0)
   {
   WitPart *     thePart;
   WitPeriod     fillPer;
   WitBopEntry * theBopEnt;
   WitPeriod     execPer;

   buildDemClients ();
   buildOpnClients ();

   myPartReqs_.allocate (myProblem ());

   forEachPart (thePart, myProblem ())
      forEachPeriod (fillPer, myProblem ())
         myPartReqs_.myPtrAt (thePart, fillPer) =
            new WitPipReqList (myProblem ());

   myBopEntReqs_.allocate (myProblem ());

   forEachBopEntry (theBopEnt, myProblem ())
      forEachPeriod (execPer, myProblem ())
         myBopEntReqs_.myPtrAt (theBopEnt, execPer) =
            new WitPipReqList (myProblem ());
   }

//------------------------------------------------------------------------------

WitPipReqMgr::~WitPipReqMgr ()
   {
   int            theIdx;
   WitPart *      thePart;
   WitPeriod      fillPer;
   WitBopEntry *  theBopEnt;
   WitPeriod      execPer;
   WitOperation * theOpn;
   int            theSeqNo;

   for (theIdx = 0; theIdx < myReqSpecVec_.length (); theIdx ++)
      delete myReqSpecVec_[theIdx];

   myReqSpecVec_.clear ();

   forEachBopEntry (theBopEnt, myProblem ())
      forEachPeriod (execPer, myProblem ())
         delete myBopEntReqs_.myPtrAt (theBopEnt, execPer);

   myBopEntReqs_.clear ();

   forEachPart (thePart, myProblem ())
      forEachPeriod (fillPer, myProblem ())
         delete myPartReqs_.myPtrAt (thePart, fillPer);

   myPartReqs_.clear ();

   deleteContents (myBases_);

   forEachOperation (theOpn, myProblem ())
      {
      if (not theOpn->pipEnabled ())
         continue;

      forEachPeriod (execPer, myProblem ())
         {
         delete myOpnClients_.myPtrAt (theOpn, execPer);
         }
      }

   myOpnClients_.clear ();

   for (theSeqNo = 0; theSeqNo < myDemClients_.length (); theSeqNo ++)
      delete myDemClients_[theSeqNo];

   myDemClients_.clear ();
   }

//------------------------------------------------------------------------------

bool WitPipReqMgr::hasReqsFor (WitOperation * theOpn, WitPeriod execPer)
   {
   WitBopEntry * theBopEnt;

   forEachEl (theBopEnt, theOpn->bop ())
      if (myBopEntReqs_.myElemAt (theBopEnt, execPer).isNonEmpty ())
         return true;

   return false;
   }

//------------------------------------------------------------------------------

WitPipReqBasis * WitPipReqMgr::newBasisForSeqNo (int theSeqNo)
   {
   WitPipReqBasis * theBasis;

   theBasis = new WitPipReqBasis (myDemClients_[theSeqNo], myProblem ());

   myBases_.push (theBasis);

   return theBasis;
   }

//------------------------------------------------------------------------------

void WitPipReqMgr::addReq (
      WitPart *        thePart,
      WitPeriod        fillPer,
      WitPipReqBasis * theBasis,
      double           incReqVol)
   {
   witAssert (incReqVol > NET_TOL);

   myPartReqs_.myElemAt (thePart, fillPer).push (theBasis, incReqVol);
   }

//------------------------------------------------------------------------------

void WitPipReqMgr::addReq (
      WitBopEntry *    theBopEnt,
      WitPeriod        execPer,
      WitPipReqBasis * theBasis,
      double           incExecVol)
   {
   witAssert (incExecVol > NET_TOL);

   myBopEntReqs_.myElemAt (theBopEnt, execPer).push (theBasis, incExecVol);
   }

//------------------------------------------------------------------------------

void WitPipReqMgr::getCurReqList (WitPipReqList & theReqList)
   {
   theReqList.takeContentsFrom (curReqList_);
   }

//------------------------------------------------------------------------------

void WitPipReqMgr::loadCurReqList (WitPart * thePart, WitPeriod fillPer)
   {
   witAssert (curReqList_.isEmpty ());

   curReqList_.takeContentsFrom (myPartReqs_.myElemAt (thePart, fillPer));

   if (curReqList_.isNonEmpty ())
      {
      if (myGlobalComp ()->printDebug ())
         fprintf (msgFile (), 
            "\n"
            "Part %s, Period %d:\n",
            thePart->partName ().myCstring (),
            fillPer);

      sortReqList ();
      }
   }

//------------------------------------------------------------------------------

void WitPipReqMgr::loadCurReqList (WitOperation * theOpn, WitPeriod execPer)
   {
   witAssert (hasReqsFor (theOpn, execPer));

   witAssert (curReqList_.isEmpty ());

   if (hasMultiExp (theOpn, execPer))
      mergeReqs (theOpn, execPer);
   else
      loadReqsFromOne (theOpn, execPer);
   }

//------------------------------------------------------------------------------

void WitPipReqMgr::clearCurReqList ()
   {  
   curReqList_.clear ();
   }

//------------------------------------------------------------------------------

void WitPipReqMgr::initReqListItr ()
   {
   curReqList_.attachItr (myReqListItr_);

   if (not myReqListItr_.advance (curBasis_, incReqVolCur_))
      curBasis_ = NULL;
   }

//------------------------------------------------------------------------------

void WitPipReqMgr::advanceReqListItr (
      double &           netReqVol,
      WitPipReqBasis * & theBasis,
      double &           incReqVol)
   {
   witAssert (not finishedReqListItr ());

   witAssert (netReqVol > NET_TOL);

   theBasis       = curBasis_;

   incReqVol      = min (incReqVolCur_, netReqVol);

   incReqVolCur_ -= incReqVol;
   netReqVol     -= incReqVol;

   if (incReqVolCur_ > NET_TOL)
      return;

   if (not myReqListItr_.advance (curBasis_, incReqVolCur_))
      curBasis_ = NULL;
   }

//------------------------------------------------------------------------------

void WitPipReqMgr::advanceReqListItr (
      WitPipReqBasis * & theBasis,
      double &           incReqVol)
   {
   witAssert (not finishedReqListItr ());

   theBasis  = curBasis_;
   incReqVol = incReqVolCur_;

   if (not myReqListItr_.advance (curBasis_, incReqVolCur_))
      curBasis_ = NULL;
   }

//------------------------------------------------------------------------------

bool WitPipReqMgr::finishedReqListItr ()
   {
   return (curBasis_ == NULL);
   }

//------------------------------------------------------------------------------

void WitPipReqMgr::addOpnClientToCurReqs (
      WitOperation * theOpn,
      WitPeriod      execPer)
   {
   WitPipReqClient <WitOperation> * theOpnClient;
   WitPipReqBasis *                 theBasis;
   double                           incReqVol;
   WitPipReqBasis *                 newReqBasis;
   WitPipReqList                    newReqList (myProblem ());

   theOpnClient = myOpnClients_.myPtrAt (theOpn, execPer);

   while (curReqList_.pop (theBasis, incReqVol))
      {
      newReqBasis = new WitPipReqBasis (theBasis, theOpnClient);

      myBases_.push (newReqBasis);

      newReqList.push (newReqBasis, incReqVol);
      }

   newReqList.reverse ();

   curReqList_.takeContentsFrom (newReqList);

   if (myGlobalComp ()->printDebug ())
      prtCurReqListAfterAddOpn ();
   }

//------------------------------------------------------------------------------

void WitPipReqMgr::prtSizes ()
   {
   myMsgFac () ("pipSizesMsg",
      maxActReqSpecs_,
      myReqSpecVec_.length ());
   }

//------------------------------------------------------------------------------

void WitPipReqMgr::buildDemClients ()
   {
   WitDemTripStack theShipSeq (myProblem ());
   int             theSeqNo;
   WitDemand *     theDemand;
   WitPeriod       shipPer;
   double          incShipVol;

   myProblem ()->myPipMgr ()->getShipSeq (theShipSeq);

   myDemClients_.resize (theShipSeq.nElements () + 1);

   theSeqNo = -1;

   while (theShipSeq.pop (theDemand, shipPer, incShipVol))
      {
      theSeqNo ++;

      myDemClients_[theSeqNo] =
         new WitPipReqClient <WitDemand> (theDemand, shipPer, theSeqNo);
      }
   }

//------------------------------------------------------------------------------

void WitPipReqMgr::buildOpnClients ()
   {
   int                         nPipOpns;
   WitOperation *              theOpn;
   WitPtrVec <WitOperation>    thePipOpns;
   int                         theIdx;
   int                         theSeqNo;
   WitPtrVecItr <WitOperation> theItr;
   WitPeriod                   execPer;

   nPipOpns = 0;
   
   forEachOperation (theOpn, myProblem ())
      if (theOpn->pipEnabled ())
         nPipOpns ++;

   thePipOpns.resize (nPipOpns);

   theIdx = -1;

   forEachOperation (theOpn, myProblem ())
      if (theOpn->pipEnabled ())
         {
         theIdx ++;

         thePipOpns[theIdx] = theOpn;
         }

   thePipOpns.sort (compareOpnsForReqClient);

   myOpnClients_.allocate1D (myProblem ());

   theSeqNo = -1;

   thePipOpns.attachItr (theItr);

   while (theItr.advance (theOpn))
      {
      myOpnClients_.myPtrTVecAt (theOpn).allocate (myProblem ());

      for (execPer = lastPeriod (); execPer >= 0; execPer --)
         {
         theSeqNo ++;

         myOpnClients_.myPtrAt (theOpn, execPer) =
            new WitPipReqClient <WitOperation> (theOpn, execPer, theSeqNo);
         }
      }
   }

//------------------------------------------------------------------------------

bool WitPipReqMgr::compareOpnsForReqClient (
      WitOperation * theOpn1,
      WitOperation * theOpn2)
   {
   return
      (theOpn1->pipRank      () > theOpn2->pipRank      ())? true:
      (theOpn1->pipRank      () < theOpn2->pipRank      ())? false:
      (theOpn1->mappingIndex () < theOpn2->mappingIndex ())? true:
                                                             false;
   }

//------------------------------------------------------------------------------

void WitPipReqMgr::loadReqsFromOne (WitOperation * theOpn, WitPeriod execPer)
   {
   WitBopEntry * expBopEnt;
   WitBopEntry * theBopEnt;

   expBopEnt = NULL;

   forEachEl (theBopEnt, theOpn->bop ())
      if (expEligible (theBopEnt, execPer))
         {
         expBopEnt = theBopEnt;

         break;
         }

   witAssert (expBopEnt != NULL);

   curReqList_.takeContentsFrom (myBopEntReqs_.myElemAt (expBopEnt, execPer));

   curReqList_.reverse ();

   if (myGlobalComp ()->printDebug ())
      prtCurReqList (theOpn, execPer);
   }

//------------------------------------------------------------------------------

void WitPipReqMgr::mergeReqs (WitOperation * theOpn, WitPeriod execPer)
   {
   WitBopEntry *       theBopEnt;
   double              incExecVol;
   WitPipReqBasis *    theBasis;
   WitPipReqBasisStack theBases (myProblem ());

   witAssert (theOpn->bop ().nElements () > 1);

   forEachEl (theBopEnt, theOpn->bop ())
      myBopEntReqs_.myElemAt (theBopEnt, execPer).reverse ();

   if (myGlobalComp ()->printDebug ())
      prtBopEntReqs (theOpn, execPer);

   while (true)
      {
      incExecVol = minIncReqVol (theOpn, execPer);

      if (incExecVol < 0.0)
         break;

      extractReqBases (theOpn, execPer, incExecVol, theBases);

      theBasis = new WitPipReqBasis (theBases, myProblem ());

      myBases_.push (theBasis);
      
      curReqList_.push (theBasis, incExecVol);
      }

   sortReqList ();
   }

//------------------------------------------------------------------------------

double WitPipReqMgr::minIncReqVol (WitOperation * theOpn, WitPeriod execPer)
   {
   double        minVol;
   WitBopEntry * theBopEnt;
   double        incReqVol;

   witAssert (theOpn->bop ().nElements () > 1);

   minVol = -1.0;

   forEachEl (theBopEnt, theOpn->bop ())
      if (myBopEntReqs_.myElemAt (theBopEnt, execPer).isNonEmpty ())
         {
         incReqVol =
            myBopEntReqs_.myElemAt (theBopEnt, execPer).firstValue ();

         if (minVol < 0.0)
            minVol = incReqVol;
         else
            setToMin (minVol, incReqVol);
         }

   return minVol;
   }

//------------------------------------------------------------------------------

void WitPipReqMgr::extractReqBases (
      WitOperation *        theOpn,
      WitPeriod             execPer,
      double                deltaIncReqVol,
      WitPipReqBasisStack & theBases)
   {
   WitBopEntry *    theBopEnt;
   WitPipReqBasis * theBasis;
   double           incReqVol;

   witAssert (deltaIncReqVol > NET_TOL);

   theBases.clear ();

   forEachEl (theBopEnt, theOpn->bop ())
      if (myBopEntReqs_.myElemAt (theBopEnt, execPer).isNonEmpty ())
         {
         myBopEntReqs_.myElemAt (theBopEnt, execPer).
            pop (theBasis, incReqVol);

         if (incReqVol - deltaIncReqVol > NET_TOL)
            myBopEntReqs_.myElemAt (theBopEnt, execPer).
               push (theBasis, incReqVol - deltaIncReqVol);

         theBases.push (theBasis);
         }
   }

//------------------------------------------------------------------------------

void WitPipReqMgr::sortReqList ()
   {
   if (myGlobalComp ()->printDebug ())
      prtCurReqListBeforeSort ();

   loadReqSpecVec ();

   myReqSpecVec_.sort (compareReqSpecs, nActReqSpecs_);

   unloadReqSpecVec ();

   if (myGlobalComp ()->printDebug ())
      prtCurReqListAfterSort ();
   }

//------------------------------------------------------------------------------

void WitPipReqMgr::loadReqSpecVec ()
   {
   int              theIdx;
   WitPipReqBasis * theBasis;
   double           incReqVol;

   witAssert (nActReqSpecs_ == 0);

   nActReqSpecs_ = curReqList_.nElements ();

   setToMax (maxActReqSpecs_, nActReqSpecs_);

   if (nActReqSpecs_ > myReqSpecVec_.length ())
      expandReqSpecVec ();

   theIdx = nActReqSpecs_;

   while (curReqList_.pop (theBasis, incReqVol))
      {
      theIdx --;

      myReqSpecVec_[theIdx]->setData (theBasis, incReqVol);
      }
   }

//------------------------------------------------------------------------------

void WitPipReqMgr::expandReqSpecVec ()
   {
   int                         oldVecLength;
   WitObjStack <WitPipReqSpec> theReqSpecs (myProblem ());
   int                         theIdx;

   witAssert (nActReqSpecs_ > myReqSpecVec_.length ());

   oldVecLength = myReqSpecVec_.length ();

   for (theIdx = 0; theIdx < oldVecLength; theIdx ++)
      theReqSpecs.push (myReqSpecVec_[theIdx]);

   myReqSpecVec_.resize (2 * nActReqSpecs_);

   for (theIdx = 0; theIdx < oldVecLength; theIdx ++)
      theReqSpecs.pop  (myReqSpecVec_[theIdx]);

   for (theIdx = oldVecLength; theIdx < myReqSpecVec_.length (); theIdx ++)
      myReqSpecVec_[theIdx] = new WitPipReqSpec;
   }

//------------------------------------------------------------------------------

bool WitPipReqMgr::compareReqSpecs (
      WitPipReqSpec * theReqSpec1,
      WitPipReqSpec * theReqSpec2)
   {
   WitPipReqBasis * theBasisl;
   WitPipReqBasis * theBasis2;

   theBasisl = theReqSpec1->myBasis ();
   theBasis2 = theReqSpec2->myBasis ();

   return theBasisl->belongsBefore (theBasis2);
   }

//------------------------------------------------------------------------------

void WitPipReqMgr::unloadReqSpecVec ()
   {
   int              theIdx;
   WitPipReqBasis * theBasis;
   double           incReqVol;
   WitPipReqBasis * prevBasis;
   double           prevIncReqVol;

   witAssert (nActReqSpecs_ > 0);

   for (theIdx = nActReqSpecs_ - 1; theIdx >= 0; theIdx --)
      {
      myReqSpecVec_[theIdx]->getData (theBasis, incReqVol);
      myReqSpecVec_[theIdx]->setData (NULL,     0.0);

      if (curReqList_.isNonEmpty ())
         if (curReqList_.firstObject ()->isEquivalentTo (theBasis))
            {
            curReqList_.pop (prevBasis, prevIncReqVol);

            incReqVol += prevIncReqVol;
            }

      curReqList_.push (theBasis, incReqVol);
      }

   nActReqSpecs_ = 0;
   }

//------------------------------------------------------------------------------

bool WitPipReqMgr::hasMultiExp (WitOperation * theOpn, WitPeriod execPer)
   {
   int           nEligible;
   WitBopEntry * theBopEnt;

   nEligible = 0;

   forEachEl (theBopEnt, theOpn->bop ())
      if (expEligible (theBopEnt, execPer))
         nEligible ++;

   return (nEligible > 1);
   }

//------------------------------------------------------------------------------

bool WitPipReqMgr::expEligible (WitBopEntry * theBopEnt, WitPeriod execPer)
   {
   if (theBopEnt->expAllowed ())
      if (theBopEnt->execPerOKForExp (execPer))
         return true;

   return false;
   }

//------------------------------------------------------------------------------

void WitPipReqMgr::prtCurReqListBeforeSort ()
   {
   fprintf (msgFile (), 
      "\n"
      "Requirements before sorting and consolidation:\n");

   prtReqList (curReqList_);
   }

//------------------------------------------------------------------------------

void WitPipReqMgr::prtCurReqListAfterSort ()
   {
   fprintf (msgFile (), 
      "\n"
      "Requirements after sorting and consolidation:\n");

   prtReqList (curReqList_);
   }

//------------------------------------------------------------------------------

void WitPipReqMgr::prtCurReqList (WitOperation * theOpn, WitPeriod execPer)
   {
   fprintf (msgFile (), 
      "\n"
      "Requirements for Operation %s in Period %d:\n",
      theOpn->operationName ().myCstring (),
      execPer);

   prtReqList (curReqList_);
   }

//------------------------------------------------------------------------------

void WitPipReqMgr::prtCurReqListAfterAddOpn ()
   {
   fprintf (msgFile (), 
      "\n"
      "Requirements after adding the current Operation pair:\n");

   prtReqList (curReqList_);
   }

//------------------------------------------------------------------------------

void WitPipReqMgr::prtBopEntReqs (WitOperation * theOpn, WitPeriod execPer)
   {
   WitBopEntry *    theBopEnt;
   WitPipReqListItr theItr;
   WitPipReqBasis * theBasis;
   double           incReqVol;
   
   witAssert (myGlobalComp ()->printDebug ());

   fprintf (msgFile (),
      "\n"
      "Merging requirements for Operation %s in Period %d:\n",
      theOpn->operationName ().myCstring (),
      execPer);

   forEachEl (theBopEnt, theOpn->bop ())
      {
      fprintf (msgFile (),
         "\n"
         "   BOP Entry #%d:\n",
         theBopEnt->localIndex ());

      myBopEntReqs_.myElemAt (theBopEnt, execPer).attachItr (theItr);

      while (theItr.advance (theBasis, incReqVol))
         {
         fprintf (msgFile (),
            "\n"
            "      IncReqVol: %3.0f\n",
            incReqVol);

         theBasis->print (msgFile (), "      ");
         }
      }
   }

//------------------------------------------------------------------------------

void WitPipReqMgr::prtReqList (const WitPipReqList & theReqList)
   {
   WitPipReqListItr theItr;
   WitPipReqBasis * theBasis;
   double           incReqVol;

   witAssert (myGlobalComp ()->printDebug ());

   theReqList.attachItr (theItr);

   while (theItr.advance (theBasis, incReqVol))
      {
      fprintf (msgFile (),
         "\n"
         "   IncReqVol: %3.0f\n",
         incReqVol);

      theBasis->print (msgFile (), "   ");
      }
   }
