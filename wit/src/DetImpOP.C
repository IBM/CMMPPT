//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "DetImpOP.C"
//
// Implementation of class DetImpOP.
//------------------------------------------------------------------------------

#include <DetImpOP.h>
#include <ExecPerSch.h>
#include <DetOptImpMgr.h>
#include <DetVars.h>
#include <DetCons.h>
#include <Coeff.h>
#include <OptComp.h>
#include <GlobalComp.h>
#include <Material.h>
#include <Operation.h>
#include <BomEntry.h>
#include <SubEntry.h>
#include <BopEntry.h>
#include <Demand.h>
#include <BoundSet.h>
#include <MultiObjMgr.h>
#include <Objective.h>
#include <Timing.h>
#include <Session.h>
#include <MsgFac.h>

#include <float.h>

//------------------------------------------------------------------------------
// Implementation of class DetImpOP.
//------------------------------------------------------------------------------

const double WitDetImpOP::solnEpsilon = 0.0005;

//------------------------------------------------------------------------------

WitDetImpOP::WitDetImpOP (WitProblem * theProblem):

      WitOptProblem   (theProblem),
      myMultiObjVar_  (),
      myMultiObjCon_  (),
      myNtbvVar_      (NULL),
      myTbvCon_       (NULL),
      nSlbvVars_      (0),
      solnFile_       (NULL),
      myExecPerSched_ (NULL)
   {
   myExecPerSched_ = new WitExecPerSched <WitBillEntry> (myProblem ());

   if (multiObjMode ())
      {
      myMultiObjVar_.allocate (myProblem ());
      myMultiObjCon_.allocate (myProblem ());
      }
   }

//------------------------------------------------------------------------------

WitDetImpOP::~WitDetImpOP ()
   {
   delete myExecPerSched_;
   }

//------------------------------------------------------------------------------

bool WitDetImpOP::needDual ()
   {
   if (myOptComp ()->compPrices ())
      return true;

   if (myGlobalComp ()->computeCriticalList ())
      return true;

   return false;
   }

//------------------------------------------------------------------------------
// storeDerived functions.
//------------------------------------------------------------------------------

void WitDetImpOP::storeDerived (WitScrapVar * theScrapVar)
   {
   insertInto (myScrapVar_, theScrapVar->myPart (), theScrapVar);
   }

//------------------------------------------------------------------------------

void WitDetImpOP::storeDerived (WitNonSubVar * theNonSubVar)
   {
   insertInto (myNonSubVar_, theNonSubVar->myBomEnt (), theNonSubVar);
   }

//------------------------------------------------------------------------------

void WitDetImpOP::storeDerived (WitSubVar * theSubVar)
   {
   insertInto (mySubVar_, theSubVar->mySub (), theSubVar);
   }

//------------------------------------------------------------------------------

void WitDetImpOP::storeDerived (WitShipVar * theShipVar)
   {
   insertInto (myShipVar_, theShipVar->myDemand (), theShipVar);
   }

//------------------------------------------------------------------------------

void WitDetImpOP::storeDerived (WitSlbvVar * theSlbvVar)
   {
   insertInto (mySlbvVar_, theSlbvVar->myBoundSet (), theSlbvVar);
   }

//------------------------------------------------------------------------------

void WitDetImpOP::storeDerived (WitExecVar * theExecVar)
   {
   insertInto (myExecVar_, theExecVar->myOperation (), theExecVar);
   }

//------------------------------------------------------------------------------

void WitDetImpOP::storeDerived (WitStockVar * theStockVar)
   {
   insertInto (myStockVar_, theStockVar->myMaterial (), theStockVar);
   }

//------------------------------------------------------------------------------

void WitDetImpOP::storeDerived (WitCumShipVar * theCumShipVar)
   {
   insertInto (myCumShipVar_, theCumShipVar->myDemand (), theCumShipVar);
   }

//------------------------------------------------------------------------------

void WitDetImpOP::storeDerived (WitMultiObjVar * theMultiObjVar)
   {
   WitObjective * theObj;

   theObj = theMultiObjVar->myObj ();

   witAssert (myMultiObjVar_ (theObj) == NULL);

   myMultiObjVar_ (theObj) = theMultiObjVar;
   }

//------------------------------------------------------------------------------

void WitDetImpOP::storeDerived (WitNtbvVar * theNtbvVar)
   {
   witAssert (multiObjMode ());

   witAssert (myNtbvVar_ == NULL);

   myNtbvVar_ = theNtbvVar;
   }

//------------------------------------------------------------------------------

void WitDetImpOP::storeDerived (WitResourceCon * theResourceCon)
   {
   insertInto (myResourceCon_, theResourceCon->myPart (), theResourceCon);
   }

//------------------------------------------------------------------------------

void WitDetImpOP::storeDerived (WitSubCon * theSubCon)
   {
   insertInto (mySubCon_, theSubCon->myBomEnt (), theSubCon);
   }

//------------------------------------------------------------------------------

void WitDetImpOP::storeDerived (WitShipCon * theShipCon)
   {
   insertInto (myShipCon_, theShipCon->myDemand (), theShipCon);
   }

//------------------------------------------------------------------------------

void WitDetImpOP::storeDerived (WitSlbCon * theSlbCon)
   {
   insertInto (mySlbCon_, theSlbCon->myBoundSet (), theSlbCon);
   }

//------------------------------------------------------------------------------

void WitDetImpOP::storeDerived (WitMultiObjCon * theMultiObjCon)
   {
   WitObjective * theObj;

   theObj = theMultiObjCon->myObj ();

   witAssert (myMultiObjCon_ (theObj) == NULL);

   myMultiObjCon_ (theObj) = theMultiObjCon;
   }

//------------------------------------------------------------------------------

void WitDetImpOP::storeDerived (WitTbvCon * theTbvCon)
   {
   witAssert (multiObjMode ());

   witAssert (myTbvCon_ == NULL);

   myTbvCon_ = theTbvCon;
   }

//------------------------------------------------------------------------------
// OptVC look-up functions.
//------------------------------------------------------------------------------

WitScrapVar * WitDetImpOP::myScrapVar (WitPart * thePart, WitPeriod thePer)
   {
   return lookUp (myScrapVar_, thePart, thePer);
   }

//------------------------------------------------------------------------------

WitNonSubVar * WitDetImpOP::myNonSubVar (
      WitBomEntry * theBomEnt,
      WitPeriod     thePer)
   {
   return lookUp (myNonSubVar_, theBomEnt, thePer);
   }

//------------------------------------------------------------------------------

WitSubVar * WitDetImpOP::mySubVar (WitSubEntry * theSub, WitPeriod thePer)
   {
   return lookUp (mySubVar_, theSub, thePer);
   }

//------------------------------------------------------------------------------

WitShipVar * WitDetImpOP::myShipVar (WitDemand * theDemand, WitPeriod thePer)
   {
   return lookUp (myShipVar_, theDemand, thePer);
   }

//------------------------------------------------------------------------------

WitExecVar * WitDetImpOP::myExecVar (WitOperation * theOpn, WitPeriod thePer)
   {
   return lookUp (myExecVar_, theOpn, thePer);
   }

//------------------------------------------------------------------------------

WitStockVar * WitDetImpOP::myStockVar (WitMaterial * theMat, WitPeriod thePer)
   {
   return lookUp (myStockVar_, theMat, thePer);
   }

//------------------------------------------------------------------------------

WitCumShipVar * WitDetImpOP::myCumShipVar (
      WitDemand * theDemand,
      WitPeriod   thePer)
   {
   return lookUp (myCumShipVar_, theDemand, thePer);
   }

//------------------------------------------------------------------------------

WitResourceCon * WitDetImpOP::myResourceCon (WitPart * thePart, WitPeriod thePer)
   {
   return lookUp (myResourceCon_, thePart, thePer);
   }

//------------------------------------------------------------------------------

WitSubCon * WitDetImpOP::mySubCon (WitBomEntry * theBomEnt, WitPeriod thePer)
   {
   return lookUp (mySubCon_, theBomEnt, thePer);
   }

//------------------------------------------------------------------------------

WitShipCon * WitDetImpOP::myShipCon (WitDemand * theDemand, WitPeriod thePer)
   {
   return lookUp (myShipCon_, theDemand, thePer);
   }

//------------------------------------------------------------------------------

WitSlbvVar * WitDetImpOP::mySlbvVar (WitBoundedVar * theBoundedVar)
   {
   return
      lookUp (
         mySlbvVar_,
         theBoundedVar->myBoundSet (),
         theBoundedVar->myPeriod ());
   }

//------------------------------------------------------------------------------

WitSlbCon * WitDetImpOP::mySlbCon (WitBoundedVar * theBoundedVar)
   {
   return
      lookUp (
         mySlbCon_,
         theBoundedVar->myBoundSet (),
         theBoundedVar->myPeriod ());
   }

//------------------------------------------------------------------------------

WitMultiObjVar * WitDetImpOP::myMultiObjVar (WitObjective * theObj)
   {
   return myMultiObjVar_ (theObj);
   }

//------------------------------------------------------------------------------

void WitDetImpOP::printItem (WitNode * theNode)
   {
   fprintf (
      optProbFile (),
      itemFormat  (),
      theNode->nodeName ().myCstring (),
      "",
      "");
   }

//------------------------------------------------------------------------------

void WitDetImpOP::printItem (WitBomEntry * theBomEnt)
   {
   fprintf (optProbFile (), itemFormat (),
      theBomEnt->myOperationName ().myCstring (),
      theBomEnt->myPartName ().myCstring (),
      "");
   }

//------------------------------------------------------------------------------

void WitDetImpOP::printItem (WitSubEntry * theSub)
   {
   fprintf (optProbFile (), itemFormat (),
      theSub->myOperationName ().myCstring (),
      theSub->myBomEnt ()->myPartName ().myCstring (),
      theSub->myPartName ().myCstring ());
   }

//------------------------------------------------------------------------------

void WitDetImpOP::printItem (WitDemand * theDemand)
   {
   fprintf (optProbFile (), itemFormat (),
      theDemand->demandedPartName ().myCstring (),
      theDemand->demandName ().myCstring (),
      "");
   }

//------------------------------------------------------------------------------

void WitDetImpOP::printItem (WitBoundedVar * theBoundedVar)
   {
   fprintf (optProbFile (),
      " Var #%-7d BS #%-8d %-12s",
      theBoundedVar->index (),
      theBoundedVar->myBoundSet ()->mappingIndex (),
      "");
   }

//------------------------------------------------------------------------------

void WitDetImpOP::printItem (WitObjective * theObj)
   {
   fprintf (optProbFile (), itemLongFormat (),
      theObj->myObjName ().myCstring ());
   }

//------------------------------------------------------------------------------

void WitDetImpOP::printItem ()
   {
   fprintf (optProbFile (), itemLongFormat (), "");
   }

//------------------------------------------------------------------------------

void WitDetImpOP::storeBoundedVar (WitBoundedVar * theBoundedVar)
   {
   myBoundedVars_.append (theBoundedVar);
   }

//------------------------------------------------------------------------------

bool WitDetImpOP::slbvVarsExist ()
   {
   return (nSlbvVars_ > 0);
   }

//------------------------------------------------------------------------------

void WitDetImpOP::printPeriod (WitPeriod thePer)
   {
   fprintf (optProbFile (), periodDFormat (), thePer);
   }

//------------------------------------------------------------------------------

void WitDetImpOP::printNonPeriod ()
   {
   fprintf (optProbFile (), periodSFormat (), "");
   }

//------------------------------------------------------------------------------
// writeSoln prints out the primal solution from wit data structures.
// Optionally prints the dual solution for resource allocation constraints.
//------------------------------------------------------------------------------

void WitDetImpOP::writeSoln (const char * fName)
   {
   WitTimer::enterSection ("extra");

   myMsgFac () ("writeOptSolnMsg");

   solnFile_ = openFile (fName, "w");

   myProblem ()->mySession ()->writeHeading (solnFile ());

   writePartSoln       ();
   writeOperationSoln  ();
   writeSubEntrySoln   ();
   writeDemandSoln     ();
   writeBoundedVarSoln ();

   fclose (solnFile ());

   solnFile_ = NULL;

   WitTimer::leaveSection ("extra");
   }

//------------------------------------------------------------------------------

void WitDetImpOP::generateVariables ()
   {
   WitPart *       thePart;
   WitMaterial *   theMat;
   WitOperation *  theOpn;
   WitBomEntry *   theBomEnt;
   WitSubEntry *   theSub;
   WitDemand *     theDemand;
   WitBoundedVar * theBoundedVar;
   WitPeriod       thePer;
   WitObjective *  theObj;

   forEachMaterial (theMat, myProblem ())
      forEachPeriod (thePer, myProblem ())
         if (theMat->canStock (thePer))
            new WitStockVar (theMat, thePer, this);

   forEachPart (thePart, myProblem ())
      if (thePart->scrapAllowed ())
         forEachPeriod (thePer, myProblem ())
            new WitScrapVar (thePart, thePer, this);

   forEachOperation (theOpn, myProblem ())
      forEachPeriod (thePer, myProblem ())
         if (theOpn->executable ()[thePer])
            new WitExecVar (theOpn, thePer, this);

   forEachBomEntry (theBomEnt, myProblem ())
      forEachPeriod (thePer, myProblem ())
         if (theBomEnt->hasSubsInEffect ()[thePer])
            new WitNonSubVar (theBomEnt, thePer, this);

   forEachSubEntry (theSub, myProblem ())
      forEachPeriod (thePer, myProblem ())
         if (theSub->inEffect (thePer))
            new WitSubVar (theSub, thePer, this);

   forEachDemand (theDemand, myProblem ())
      forEachPeriod (thePer, myProblem ())
         new WitShipVar (theDemand, thePer, this);

   forEachDemand (theDemand, myProblem ())
      if (theDemand->shipLateAllowed ())
         forEachPeriod (thePer, myProblem ())
            new WitCumShipVar (theDemand, thePer, this);

   forEachEl (theBoundedVar, myBoundedVars_)
      if (theBoundedVar->needsAnSlbvVar ())
         {
         new WitSlbvVar (theBoundedVar, this);

         ++ nSlbvVars_;
         }

   if (multiObjMode ())
      {
      forEachEl (theObj, myMultiObjMgr ()->myObjectives ())
         {
         new WitMultiObjVar (theObj, this);
         }

      if (nSlbvVars_ > 0)
         {
         new WitNtbvVar (this);
         }
      }
   }

//------------------------------------------------------------------------------

void WitDetImpOP::generateConstraints ()
   {
   WitPart *       thePart;
   WitBomEntry *   theBomEnt;
   WitDemand *     theDemand;
   WitPeriod       thePer;
   WitBoundedVar * theBoundedVar;
   WitObjective *  theObj;

   forEachPart (thePart, myProblem ())
      forEachPeriod (thePer, myProblem ())
         new WitResourceCon (thePart, thePer, this);

   if (myCompMgr ()->hasSubEntries ())
      forEachBomEntry (theBomEnt, myProblem ())
         forEachPeriod (thePer, myProblem ())
            if (theBomEnt->hasSubsInEffect ()[thePer])
               new WitSubCon (theBomEnt, thePer, this);

   forEachDemand (theDemand, myProblem ())
      if (theDemand->shipLateAllowed ())
         forEachPeriod (thePer, myProblem ())
            new WitShipCon (theDemand, thePer, this);

   forEachEl (theBoundedVar, myBoundedVars_)
      if (mySlbvVar (theBoundedVar) != NULL)
         new WitSlbCon (theBoundedVar, this);

   if (multiObjMode ())
      {
      forEachEl (theObj, myMultiObjMgr ()->myObjectives ())
         {
         new WitMultiObjCon (theObj, this);
         }

      if (myNtbvVar_ != NULL)
         {
         new WitTbvCon (this);
         }
      }
   }

//------------------------------------------------------------------------------

void WitDetImpOP::generateLexOptVarSeq (WitPtrVec <WitOptVar> & theOptVarSeq)
   {
   int            seqLength;
   WitObjective * theObj;
   int            theIdx;

   stronglyAssert (multiObjMode ());

   if (myNtbvVar_ == NULL)
      {
      seqLength = myMultiObjMgr ()->myObjectives ().nElements ();

      theOptVarSeq.resize (seqLength);

      forEachEl (theObj, myMultiObjMgr ()->myObjectives ())
         {
         theIdx = theObj->objectiveSeqNo () - 1;

         theOptVarSeq[theIdx] = myMultiObjVar_ (theObj);
         }
      }
   else
      {
      seqLength = myMultiObjMgr ()->myObjectives ().nElements () + 1;

      theOptVarSeq.resize (seqLength);

      theOptVarSeq[0] = myNtbvVar_;

      forEachEl (theObj, myMultiObjMgr ()->myObjectives ())
         {
         theIdx = theObj->objectiveSeqNo ();

         theOptVarSeq[theIdx] = myMultiObjVar_ (theObj);
         }
      }
   }

//------------------------------------------------------------------------------

void WitDetImpOP::postGenFixed ()
   {
   delete myExecPerSched_;

   myExecPerSched_ = NULL;
   }

//------------------------------------------------------------------------------

void WitDetImpOP::postGenFlexible ()
   {
   }

//------------------------------------------------------------------------------

void WitDetImpOP::printOptVCHeading ()
   {
   fprintf (optProbFile (), "Index    Class       ");

   fprintf (optProbFile (), itemFormat (),    "Item", "Item", "Item");
   fprintf (optProbFile (), periodSFormat (), "Per");
   }

//------------------------------------------------------------------------------

void WitDetImpOP::displayDerived ()
   {
   int nBSSpec;

   nBSSpec = myCompMgr ()->nBoundSetsSpecified ();

   if (nBSSpec > 0)
      myMsgFac () ("boundCountsMsg",
         nBSSpec,
         myBoundedVars_.nElements (),
         nSlbvVars_);
   }

//------------------------------------------------------------------------------

void WitDetImpOP::printDerived ()
   {
   printBoundedVars ();
   }

//------------------------------------------------------------------------------

void WitDetImpOP::initDepVars ()
   {
   initSlbvVars ();

   if (multiObjMode ())
      initMultiObjVars ();

   if (myNtbvVar_ != NULL)
      initNtbvVar ();
   }

//------------------------------------------------------------------------------

void WitDetImpOP::doScreening ()
   {
   if (myOptComp ()->printOptProblem ())
      saveBounds ();

   screenImpCons ();

   relaxCumShipNonNeg ();

   screenSlbOptVCs ();
   }

//------------------------------------------------------------------------------

void WitDetImpOP::writePeSoln (WitOptVC * theOptVC)
   {
   double valJ;

   valJ =
      (theOptVC == NULL)?
         0.0:
         theOptVC->solnValue ();

   if (fabs (valJ) > solnEpsilon)
      fprintf (solnFile (), "%13.3f  ", valJ);
   else
      fprintf (solnFile (), "               ");
   }

//------------------------------------------------------------------------------

void WitDetImpOP::writePartSoln ()
   {
   WitPart *     thePart;
   WitMaterial * theMat;
   WitPeriod     t;

   forEachPart (thePart, myProblem ())
      {
      fprintf (
         solnFile (),
         "\nPart %s:\n",
         thePart->partName ().myCstring ());

      fprintf (solnFile (),
          "Period      Inventory          Scrap");

      if (needDual ())
         fprintf (solnFile (), "   Shadow Price");

      fprintf (solnFile (), "\n");

      forEachPeriod (t, myProblem ())
         {
         fprintf (solnFile (), "%6d  ", t);

         theMat = thePart->thisMat ();

         writePeSoln (
            (theMat != NULL)?
               myStockVar (theMat, t):
               NULL);

         writePeSoln (myScrapVar (thePart, t));

         if (needDual ())
            writePeSoln (myResourceCon (thePart, t));

         fprintf (solnFile (), "\n");
         }
      }
   }

//------------------------------------------------------------------------------

void WitDetImpOP::writeOperationSoln ()
   {
   WitOperation * theOpn;
   WitPeriod      thePer;

   forEachOperation (theOpn, myProblem ())
      {
      fprintf (solnFile (),
         "\nOperation %s:\n",
         theOpn->operationName ().myCstring ());

      fprintf (solnFile (), "Period      Execution\n");

      forEachPeriod (thePer, myProblem ())
         {
         fprintf (solnFile (), "%6d  ", thePer);

         writePeSoln (myExecVar (theOpn, thePer));

         fprintf (solnFile (), "\n");
         }
      }
   }

//------------------------------------------------------------------------------

void WitDetImpOP::writeSubEntrySoln ()
   {
   WitBomEntry * theBomEnt;
   WitSubEntry * theSub;
   WitPeriod t;

   forEachBomEntry (theBomEnt, myProblem ())
      if (not theBomEnt->mySubEntries ().isEmpty ())
         {
         fprintf (solnFile (),
            "\n"
            "Consuming Operation: %s,  Part Replaced: %s,  BOM Entry #%d:\n"
            "\n",
            theBomEnt->myOperationName ().myCstring (),
            theBomEnt->myPartName ().myCstring (),
            theBomEnt->localIndex ());

         fprintf (solnFile (), "Period        Non-Sub\n");

         forEachPeriod (t, myProblem ())
            if (theBomEnt->inEffect (t))
               {
               fprintf (solnFile (), "   %3d  ", t);

               writePeSoln (myNonSubVar (theBomEnt, t));

               fprintf (solnFile (), "\n");
               }

         fprintf (solnFile (),
            "\n"
            "Period  Substitute       Sub #   Substitution\n");

         forEachPeriod (t, myProblem ())
            forEachEl (theSub, theBomEnt->mySubEntries ())
               if (theSub->inEffect (t))
                  {
                  fprintf (solnFile (),
                     "   %3d  %-12s  %8d  ",
                     t,
                     theSub->myPartName ().myCstring (),
                     theSub->localIndex ());

                  writePeSoln (mySubVar (theSub, t));

                  fprintf (solnFile (), "\n");
                  }
         }
   }

//------------------------------------------------------------------------------

void WitDetImpOP::writeDemandSoln ()
   {
   WitDemand * theDemand;
   WitPeriod t;

   forEachDemand (theDemand, myProblem ())
      {
      fprintf (solnFile (),
         "\nPart %s,  Demand %s:\n",
         theDemand->demandedPartName ().myCstring (),
         theDemand->demandName ().myCstring ());

      fprintf (solnFile (),
          "Period       Shipment       Cum Ship\n");

      forEachPeriod (t, myProblem ())
         {
         fprintf (solnFile (), "%6d  ", t);

         writePeSoln (myShipVar    (theDemand, t));
         writePeSoln (myCumShipVar (theDemand, t));

         fprintf (solnFile (), "\n");
         }
      }
   }

//------------------------------------------------------------------------------

void WitDetImpOP::writeBoundedVarSoln ()
   {
   WitBoundedVar * theBoundedVar;

   fprintf (solnFile (),
      "\n\nVar Index"
      "   Hard LB"
      "   Soft LB"
      " Violation"
      "     Value"
      "   Hard UB"
      "\n");

   forEachEl (theBoundedVar, myBoundedVars_)
      theBoundedVar->printSoln ();
   }

//------------------------------------------------------------------------------

const char * WitDetImpOP::itemFormat ()
   {
   return " %-12s %-12s %-12s";
   }

//------------------------------------------------------------------------------

const char * WitDetImpOP::itemLongFormat ()
   {
   return " %-38s";
   }

//------------------------------------------------------------------------------

const char * WitDetImpOP::periodDFormat ()
   {
   return " %4d";
   }

//------------------------------------------------------------------------------

const char * WitDetImpOP::periodSFormat ()
   {
   return " %4s";
   }

//------------------------------------------------------------------------------

template <typename Item, typename DerOptVC>
      void WitDetImpOP::insertInto (
         WitPtrSched <Item, DerOptVC> & thePtrSched,
         const Item *                   theItem,
         DerOptVC *                     theDerOptVC)
   {
   if (thePtrSched.domainSize () == 0)
      thePtrSched.allocate1D (myProblem ());

   if (thePtrSched.myPtrTVecAt (theItem).length () == 0)
       thePtrSched.myPtrTVecAt (theItem).allocate (myProblem ());

   witAssert (
      thePtrSched.myPtrAt (theItem, theDerOptVC->myPeriod ()) == NULL);

   thePtrSched.myPtrAt (theItem, theDerOptVC->myPeriod ()) = theDerOptVC;
   }

//------------------------------------------------------------------------------

template <typename Item, typename DerOptVC>
      DerOptVC * WitDetImpOP::lookUp (
         WitPtrSched <Item, DerOptVC> & thePtrSched,
         const Item *                   theItem,
         WitPeriod                      thePer)
   {
   if (thePtrSched.domainSize () == 0)
      return NULL;

   if (thePtrSched.myPtrTVecAt (theItem).length () == 0)
      return NULL;

   return thePtrSched.myPtrAt (theItem, thePer);
   }

//------------------------------------------------------------------------------

void WitDetImpOP::initSlbvVars ()
   {
   WitSlbvVar *    theSlbvVar;
   WitBoundedVar * theBoundedVar;
   double          thePrimalVal;

   forEachEl (theBoundedVar, myBoundedVars_)
      {
      theSlbvVar = mySlbvVar (theBoundedVar);

      if (theSlbvVar != NULL)
         {
         thePrimalVal = 
            positivePart (
               theBoundedVar->softLB () - theBoundedVar->primalValue ());

         theSlbvVar->setPrimalValue (thePrimalVal);
         }
      }
   }

//------------------------------------------------------------------------------

void WitDetImpOP::initMultiObjVars ()
   {
   WitObjective *   theObj;
   WitMultiObjVar * theMultiObjVar;
   double           thePrimalVal;
   WitCoeffItr      theCoeffItr;
   WitCoeff *       theCoeff;

   witAssert (multiObjMode ());

   forEachEl (theObj, myMultiObjMgr ()->myObjectives ())
      {
      theMultiObjVar = myMultiObjVar_ (theObj);

      thePrimalVal   = 0.0;

      myMultiObjCon_ (theObj)->attachCoeffItr (theCoeffItr);

      while (theCoeffItr.advance (theCoeff))
         {
         if (theCoeff->myVar () == theMultiObjVar)
            continue;

         thePrimalVal +=
            theCoeff->myValue () * theCoeff->myVar ()->primalValue ();
         }

      theMultiObjVar->setPrimalValue (thePrimalVal);
      }
   }

//------------------------------------------------------------------------------

void WitDetImpOP::initNtbvVar ()
   {
   WitSlbvVar *    theSlbvVar;
   WitBoundedVar * theBoundedVar;
   double          thePrimalVal;

   witAssert (myNtbvVar_ != NULL);

   thePrimalVal = 0.0;

   forEachEl (theBoundedVar, myBoundedVars_)
      {
      theSlbvVar = mySlbvVar (theBoundedVar);

      if (theSlbvVar != NULL)
         {
         thePrimalVal -= theSlbvVar->primalValue ();
         }
      }

   myNtbvVar_->setPrimalValue (thePrimalVal);
   }

//------------------------------------------------------------------------------

void WitDetImpOP::screenImpCons ()
   {
   WitPart *   thePart;
   WitDemand * theDemand;
   WitPeriod   t;
   int         nVarsFixed = 0; // # variables fixed.

   forEachPart (thePart, myProblem ())
      forEachPeriod (t, myProblem ())
         nVarsFixed += screen (myResourceCon (thePart, t));

   forEachDemand (theDemand, myProblem ())
      forEachPeriod (t, myProblem ())
         nVarsFixed += screen (myShipCon (theDemand, t));

   if (devMode ())
      myMsgFac () ("nVarsFixedMsg", nVarsFixed);
   }

//------------------------------------------------------------------------------

void WitDetImpOP::relaxCumShipNonNeg ()
   {
   WitDemand *     theDemand;
   WitPeriod       thePer;
   WitCumShipVar * theCumShipVar;
   int             nRelaxed;

   nRelaxed = 0;

   forEachDemand (theDemand, myProblem ())
      if (theDemand->shipLateAllowed ())
         forEachPeriod (thePer, myProblem ())
            {
            theCumShipVar = myCumShipVar (theDemand, thePer);

            if (theCumShipVar->bounds ().upper () > 0.0)
               if (theCumShipVar->bounds ().lower () == 0.0)
                  {
                  theCumShipVar->bounds ().lower () = - DBL_MAX;

                  ++ nRelaxed;
                  }
            }

   if (devMode ())
      myMsgFac () ("nCshipRelaxedMsg", nRelaxed);
   }

//------------------------------------------------------------------------------

void WitDetImpOP::screenSlbOptVCs ()
   {
   WitBoundedVar * theBoundedVar;
   WitSlbCon *     theSlbCon;
   int             nScreened = 0;

   forEachEl (theBoundedVar, myBoundedVars_)
      {
      theSlbCon = mySlbCon (theBoundedVar);

      if (theSlbCon != NULL)
         if (theBoundedVar->softLB () <=
             theBoundedVar->hardLB () + FLOAT_EPSILON)
            {
            theSlbCon->bounds ().lower () = - DBL_MAX;

            mySlbvVar (theBoundedVar)->bounds ().upper () = 0.0;

            ++ nScreened;
            }
      }

   if (devMode ())
      myMsgFac () ("nSlbScreenedMsg", nScreened);
   }

//------------------------------------------------------------------------------

bool WitDetImpOP::positiveHardLBsExist ()
   {
   WitMaterial *  theMat;
   WitDemand *    theDemand;
   WitOperation * theOpn;

   forEachMaterial (theMat, myProblem ())
      if (theMat->stockBounds ()->hasPositiveHardLB ())
         return true;

   forEachDemand (theDemand, myProblem ())
      if (theDemand->cumShipBounds ()->hasPositiveHardLB ())
         return true;

   forEachOperation (theOpn, myProblem ())
      if (theOpn->execBounds ()->hasPositiveHardLB ())
         return true;

   return false;
   }

//------------------------------------------------------------------------------

bool WitDetImpOP::disallowedScrapExists ()
   {
   WitPart * thePart;

   forEachPart (thePart, myProblem ())
      if (not thePart->scrapAllowed ())
         return true;

   return false;
   }

//------------------------------------------------------------------------------

void WitDetImpOP::printBoundedVars ()
   {
   WitBoundedVar * theBoundedVar;

   fprintf (optProbFile (),
      "\n\n"
      "Bound Data\n"
      "===== ====\n\n\n");

   fprintf (optProbFile (),
      "# Bounded Variables: %8d\n",   myBoundedVars_.nElements ());

   fprintf (optProbFile (), "# Slbv Variables:    %8d\n\n", nSlbvVars_);

   fprintf (optProbFile (),
        "Variable"
      "   Hard LB"
      "   Soft LB"
      "   Hard UB"
      "\n");

   forEachEl (theBoundedVar, myBoundedVars_)
      theBoundedVar->printBoundInfo ();
   }

//------------------------------------------------------------------------------

bool WitDetImpOP::multiObjMode ()
   {
   return myOptComp ()->multiObjMode ();
   }

//------------------------------------------------------------------------------

WitMultiObjMgr * WitDetImpOP::myMultiObjMgr ()
   {
   return myOptComp ()->myMultiObjMgr ();
   }
