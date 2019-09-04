//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef NodeSorterH
#define NodeSorterH

//------------------------------------------------------------------------------
// Header file: "NodeSorter.h"
//
// Contains the declaration of class NodeSorter.
//------------------------------------------------------------------------------

#include <Mapping.h>
#include <PtrMap.h>
#include <ObjStack.h>
#include <Assoc.h>

//------------------------------------------------------------------------------
// Class NodeSorter
//
// Responsible for sorting the Parts and Operations of a Problem, as part of
// preprocessing.
//
// Implemented in PreHelp.C.
//------------------------------------------------------------------------------

class WitNodeSorter: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitNodeSorter (WitProblem *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitNodeSorter ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void sortNodesIfNeeded ();
         //
         // Does a topological sort of the Nodes of theProblem, unless they
         // are already in order.
         // If an explodeable cycle is found, it is flagged as an error.

      void getExpCycle (
            WitObjStack <WitPart> &      theParts,
            WitObjStack <WitOperation> & theOpns);
         //
         // If the Problem contains at least one explodeable cycle, this
         // function retrieves one of them;
         // If not, this function leaves the argument Lists unaltered.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      void sortNodesIfNeededInt ();
         //
         // Does a topological sort of the Nodes of theProblem, unless they
         // are already in order.

      void defineAllNodesSuccessors ();
         //
         // Sets up allNodes and successorsVector.

      bool nodesInOrder ();
         //
         // Returns true, iff the Parts and Operations are already in order.

      void sortNodes ();
         //
         // Sorts allNodes, checks for cycles, and stores the result in
         // myProblem ().

      void sortNodes (WitList <WitNode> & sortedNodes);
         //
         // Sorts allNodes and stores the result in sortedNodes.

      void constructCycle ();
         //
         // Constructs and reports a cycle in the BOM structure.
         // Assumes that the set of unsorted Nodes is non-empty and that each
         // node in this set has a predecessor in the BOM in this set.
         // This is precisely the condition that occurs when the topological
         // sort (sortNodes) fails. Under this condition, a cycle can be found
         // by following a path of unsorted predecessor Nodes until a repeat is
         // found.

      WitPart * findPartInCycle ();
         //
         // Finds and returns a Part in a cycle of unsorted predecessor Nodes.

      void storeCycle (WitPart * startingPart);
         //
         // Stores a cycle in the BOM structure starting at startingPart in
         // cycleParts_ and cycleOpns_.

      void reportCycle ();
         //
         // Issues the severe error messages for a cycle in the BOM structure.

      noCopyCtorAssign (WitNodeSorter);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitList <WitNode> allNodes_;
         //
         // All the Nodes in myProblem.

      WitPtrMap <WitNode, WitList <WitNode> > successors_;
         //
         // successors_.myElemAt (theNode) is a List of all Nodes, otherNode,
         // such that there is an BillEntry from theNode to otherNode, so that
         // otherNode must be higher in the BOM than theNode. otherNode will be
         // listed once in successors_.myElemAt (theNode) for each BillEntry
         // connecting theNode to otherNode.

      WitMapping <WitNode, bool> sorted_;
         //
         // sorted_ (theNode) is true, iff theNode has been sorted.

      WitPtrMap <WitNode, WitNode> unsortedPredFor_;
         //
         // During constructCycle, unsortedPredFor_ (theNode)
         // is an unsorted predecessor of theNode. By assumption, this exists
         // for each unsorted Node during constructCycle.

      bool cycleFound_;
         //
         // True, iff an explodeable cycle has been found.

      WitObjStack <WitPart>      cycleParts_;
      WitObjStack <WitOperation> cycleOpns_;
         //
         // If an explodeable cycle has been found, it is stored in these
         // Stacks.
   };

#endif
