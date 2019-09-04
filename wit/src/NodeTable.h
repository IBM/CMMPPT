//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef NodeTableH
#define NodeTableH

//------------------------------------------------------------------------------
// Header file: "NodeTable.h"
//
// Contains the declaration of the following classes and templates:
//
//    NodeTable.
//    DerNodeTable <DerNode>.
//------------------------------------------------------------------------------

#include <Assoc.h>
#include <CstringLT.h>

#include <map>

//------------------------------------------------------------------------------

typedef std::map <const char *, WitNode *, WitCstringLT> WitNodeMap;

//------------------------------------------------------------------------------
// Class NodeTable
//
// A table of Nodes that maps the theNode->nameName () to theNode.
//
// Class Hierarchy:
//
// ProbAssoc
//    NodeTable
//       DerNodeTable <DerNode>
//
// Implemented in Misc.C
//------------------------------------------------------------------------------

class WitNodeTable: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Public member functions.
      //------------------------------------------------------------------------

      void remove (WitNode * theNode);
         //
         // Removes theNode from this NodeTable.
         // witAsserts that theNode is currently listed in this NodeTable.

      void purge ();
         //
         // Removes all selForDel Nodes from this NodeTable.

   protected:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitNodeTable (WitProblem *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitNodeTable ();

      //------------------------------------------------------------------------
      // Other protected member functions.
      //------------------------------------------------------------------------

      void insertNode (WitNode * theNode);
         //
         // Inserts a theNode into this NodeTable.
         // In debug mode, issues a fatal error, if the name of theNode matches
         // a name already in the table.

      WitNode * findNode (const char * theName);
         //
         // Returns the node whose name is partName, assuming it was
         // previously inserted into this NodeTable; otherwise returns NULL.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitNodeTable);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitNodeMap myNodeMap_;
         //
         // Maps from theNode->nameName () to theNode.

      WitNode * cachedNode_;
         //
         // The node most recently inserted into or found in this NodeTable.
   };

//------------------------------------------------------------------------------
// Class Template DerNodeTable <DerNode>
//
// A NodeTable whose Nodes are all instances of the class represented by
// DerNode, where the DerNode class is a derived class of Node.
//
// Class Hierarchy:
//
// ProbAssoc
//    NodeTable
//       DerNodeTable <DerNode>
//
// All implementation is inline.
//------------------------------------------------------------------------------

template <typename DerNode> 
      class WitDerNodeTable: public WitNodeTable
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      inline WitDerNodeTable (WitProblem * theProblem):

            WitNodeTable (theProblem)
         {
         }

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      inline ~WitDerNodeTable ()
         {
         }

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      inline void insert (DerNode * theDerNode)
         {
         insertNode (theDerNode);
         }

      DerNode * findDerNode (const char * theName)
         {
         return static_cast <DerNode *> (findNode (theName));
         }

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitDerNodeTable);
   };

#endif
