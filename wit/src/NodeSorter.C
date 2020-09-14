//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "NodeSorter.C"
//
// Contains the implementation of class NodeSorter.
//------------------------------------------------------------------------------

#include <NodeSorter.h>
#include <BopEntry.h>
#include <Part.h>
#include <Operation.h>
#include <SubEntry.h>
#include <ObjStack.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class NodeSorter.
//------------------------------------------------------------------------------

WitNodeSorter::WitNodeSorter (WitProblem * theProblem):

      WitProbAssoc     (theProblem),
      allNodes_        (),
      successors_      (),
      sorted_          (),
      unsortedPredFor_ (),
      cycleFound_      (false),
      cycleParts_      (myProblem ()),
      cycleOpns_       (myProblem ())
   {
   }

//------------------------------------------------------------------------------

WitNodeSorter::~WitNodeSorter ()
   {
   }

//------------------------------------------------------------------------------

void WitNodeSorter::sortNodesIfNeeded ()
   {
   sortNodesIfNeededInt ();

   if (cycleFound_)
      reportCycle ();
   }

//------------------------------------------------------------------------------

void WitNodeSorter::getExpCycle (
      WitObjStack <WitPart> &      theParts,
      WitObjStack <WitOperation> & theOpns)
   {
   sortNodesIfNeededInt ();

   theParts.takeContentsFrom (cycleParts_);
   theOpns .takeContentsFrom (cycleOpns_);
   }

//------------------------------------------------------------------------------

void WitNodeSorter::sortNodesIfNeededInt ()
   {
   int            nBefore;
   WitPart *      thePart;
   WitOperation * theOpn;

   successors_.allocate (myProblem ());

   forEachPart (thePart, myProblem ())
      successors_ (thePart) = new WitList <WitNode>;

   forEachOperation (theOpn, myProblem ())
      successors_ (theOpn)  = new WitList <WitNode>;

   defineAllNodesSuccessors ();

   if (not nodesInOrder ())
      sortNodes ();

   forEachOperation (theOpn, myProblem ())
      delete successors_ (theOpn);

   forEachPart (thePart, myProblem ())
      delete successors_ (thePart);

   allNodes_  .clear ();
   successors_.clear ();

   if (cycleFound_)
      return;

   nBefore = 0;

   forEachPart (thePart, myProblem ())
      {
      thePart->storePreSortedPartIndex (nBefore);

      nBefore ++;
      }
   }

//------------------------------------------------------------------------------

void WitNodeSorter::defineAllNodesSuccessors ()
   {
   WitPart      * thePart;
   WitOperation * theOpn;
   WitBomEntry  * theBomEnt;
   WitSubEntry  * theSub;
   WitBopEntry  * theBopEnt;

   forEachPart (thePart, myProblem ())
      allNodes_.append (thePart);

   forEachOperation (theOpn, myProblem ())
      allNodes_.append (theOpn);

   forEachOperation (theOpn, myProblem ())
      {
      forEachEl (theBomEnt, theOpn->bom ())
         {
         successors_.myElemAt (theBomEnt->myPart ()).append (theOpn);

         forEachEl (theSub, theBomEnt->mySubEntries ())
            successors_.myElemAt (theSub->myPart ()).append (theOpn);
         }
      }

   forEachPart (thePart, myProblem ())
      forEachEl (theBopEnt, thePart->producingBopEntries ())
         if (theBopEnt->expAllowed ())
            successors_.myElemAt (theBopEnt->myOperation ()).append (thePart);
   }

//------------------------------------------------------------------------------
// nodesInOrder.
// Returns true, iff the Parts and Operations are already in order.
// Variables:
//
//    nBefore
//       nBefore (theNode) is the # of Nodes listed before theNode in allNodes_
//       This indicates the order of the Nodes in allNodes_.
//
//    nBeforeSuccs
//       nBeforeSuccs (theNode) is the minimum # of Nodes listed before
//       any successor to theNode.
//
// The Nodes are considered to be in order iff the Parts are in order and the
// Operations are in order. Since allNodes_ is just a temporary List, a Part
// is allowed to be out of order with respect to an Operation. Abstractly, this
// means that a Node should not be compared to its successors, which are of the
// opposite derived class (Part vs. Operation). Instead, a Node should be
// compared to the successors of its successors, which are of the same derived
// class. Specifically, the Nodes are out of order, iff there exists a Node,
// theNode and a successor to it, succNode, such that:
//
//    nBefore (theNode) >= nBeforeSuccs (succNode)
//------------------------------------------------------------------------------

bool WitNodeSorter::nodesInOrder ()
   {
   WitMapping <WitNode, int> nBefore      (myProblem (), 0);
   WitMapping <WitNode, int> nBeforeSuccs (myProblem (), 0);

   WitNode * theNode;
   WitNode * succNode;
   int nScanned;

   nScanned = 0;

   forEachEl (theNode, allNodes_)
      nBefore (theNode) = nScanned ++;

   forEachEl (theNode, allNodes_)
       {
       nBeforeSuccs (theNode) = myCompMgr ()->nNodes ();

       forEachEl (succNode, successors_.myElemAt (theNode))
          nBeforeSuccs (theNode) =
             min (nBeforeSuccs (theNode), nBefore (succNode));
       }

   forEachEl (theNode, allNodes_)
       forEachEl (succNode, successors_.myElemAt (theNode))
          if (nBefore (theNode) >= nBeforeSuccs (succNode))
             return false;

   return true;
   }

//------------------------------------------------------------------------------

void WitNodeSorter::sortNodes ()
   {
   WitList <WitNode> sortedNodes;
      //
      // The sorted List of all Nodes.

   myMsgFac () ("sortNodesMsg");

   sorted_.allocate (myProblem (), false);

   sortNodes (sortedNodes);

   if (sortedNodes.nElements () != myCompMgr ()->nNodes ())
      constructCycle ();

   sorted_.clear ();

   if (not cycleFound_)
      myCompMgr ()->storePreSortedNodes (sortedNodes);
   }

//------------------------------------------------------------------------------
// sortNodes.
// Variables:
//
//    nUnsortedPreds
//       nUnsortedPreds (theNode) is the # of BillEntries to predecessors to
//       theNode that have not yet been sorted. When this value is 0,
//       theNode is eligible to be sorted.
//
//    eligibleNodes
//       The list of all Nodes currently eligible to be sorted: those with
//       no unsorted predecessors.
//------------------------------------------------------------------------------

void WitNodeSorter::sortNodes (WitList <WitNode> & sortedNodes)
   {
   WitMapping <WitNode, int> nUnsortedPreds (myProblem (), 0);

   WitList <WitNode> eligibleNodes;

   WitNode * theNode;
   WitNode * succNode;

   forEachEl (theNode, allNodes_)
      forEachEl (succNode, successors_.myElemAt (theNode))
         ++ nUnsortedPreds (succNode);

   forEachEl (theNode, allNodes_)
      if (nUnsortedPreds (theNode) == 0)
         eligibleNodes.append (theNode);

   while (not eligibleNodes.isEmpty ())
      {
      theNode = eligibleNodes.get ();

      sortedNodes.append (theNode);

      sorted_ (theNode) = true;

      forEachEl (succNode, successors_.myElemAt (theNode))
         {
         -- nUnsortedPreds (succNode);

         if (nUnsortedPreds (succNode) == 0)
             eligibleNodes.append (succNode);
         }
      }
   }

//------------------------------------------------------------------------------

void WitNodeSorter::constructCycle ()
   {
   WitNode * theNode;
   WitNode * succNode;
   WitPart * startingPart;

   unsortedPredFor_.allocate (myProblem ());

   forEachEl (theNode, allNodes_)
      if (not sorted_ (theNode))
         forEachEl (succNode, successors_.myElemAt (theNode))
            unsortedPredFor_ (succNode) = theNode;

   startingPart = findPartInCycle ();

   storeCycle (startingPart);

   unsortedPredFor_.clear ();

   cycleFound_ = true;
   }

//------------------------------------------------------------------------------
// findPartInCycle
// Local Variable:
//
//    scanned
//       scanned (theNode) is true, iff theNode has already been scanned in
//       the search for a cycle. A cycle has been found when a Node gets
//       scanned twice.
//------------------------------------------------------------------------------

WitPart * WitNodeSorter::findPartInCycle ()
   {
   WitMapping <WitNode, bool> scanned (myProblem (), false);
   WitNode *                  theNode;
   WitPart *                  thePart;

   forEachEl (theNode, allNodes_)
      if (not sorted_ (theNode))
         break;

   while (not scanned (theNode))
      {
      scanned (theNode) = true;

      theNode           = unsortedPredFor_ (theNode);
      }

   thePart = theNode->thisPart ();

   if (thePart != NULL)
      return thePart;

   thePart = unsortedPredFor_ (theNode)->thisPart ();

   stronglyAssert (thePart != NULL);

   return thePart;
   }

//------------------------------------------------------------------------------

void WitNodeSorter::storeCycle (WitPart * startingPart)
   {
   WitPart *      thePart;
   WitOperation * theOpn;

   thePart = startingPart;

   do
      {
      cycleParts_.push (thePart);

      theOpn  = unsortedPredFor_ (thePart)->thisOpn ();

      stronglyAssert (theOpn  != NULL);

      cycleOpns_.push (theOpn);

      thePart = unsortedPredFor_ (theOpn) ->thisPart ();

      stronglyAssert (thePart != NULL);
      }
      while (thePart != startingPart);

   cycleParts_.reverse ();
   cycleOpns_ .reverse ();
   }

//------------------------------------------------------------------------------

void WitNodeSorter::reportCycle ()
   {
   WitPart *      firstPart;
   WitPart *      thePart;
   WitOperation * theOpn;
   WitPart *      nextPart;

   myMsgFac () ("bomCycleHeadingSmsg");

   firstPart = cycleParts_.firstObject ();

   while (cycleParts_.pop (thePart))
      {
      cycleOpns_.pop (theOpn);

      nextPart =
         cycleParts_.isNonEmpty ()?
            cycleParts_.firstObject ():
            firstPart;

      myMsgFac () ("bomCycleNodesSmsg",
         thePart ->partName      (),
         theOpn  ->operationName (),
         nextPart->partName      ());
      }

   myMsgFac () ("bomCycleSmsg");
   }
