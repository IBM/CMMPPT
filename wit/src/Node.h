//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef NodeH
#define NodeH

//------------------------------------------------------------------------------
// Header file: "Node.h"
//
// Contains the declaration of class Node.
//------------------------------------------------------------------------------

#include <DelComp.h>
#include <Str.h>

//------------------------------------------------------------------------------
// Class Node.
//
// A Node in the BOM graph.
//
// Class Hierarchy:
//
// ProbAssoc
//    Component
//       Node
//          Part
//             Material
//             Capacity
//          Operation
//
// Implemented in Component.C.
//------------------------------------------------------------------------------

class WitNode: public WitDelComp
   {
   public:

      //------------------------------------------------------------------------
      // Public member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitNode ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (const WitString &, nodeName)

      //------------------------------------------------------------------------
      // Mapping functions
      //------------------------------------------------------------------------

      inline int mappingIndex () const
         {                                                       
         return nodeIndex ();
         }                                                       
         //
         // Mapping index: See Mapping.h.

      inline static void verifyMappingIndex (WitNode * &)
         {
         }
         //
         // See decMappingIndex in macs.h.

      static int nInstances (WitProblem *);

      //------------------------------------------------------------------------
      // store<Activity><Attr> functions.
      // Each of these functions sets the value of indicated attribute to the
      // value of the argument.
      // Each function is only valid when the indicated activity is being
      // performed.
      //------------------------------------------------------------------------

      void storeCompMgrNodeName (const char *);
         //
         // Valid only when myCompMgr is setting a nodeName.

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      virtual WitNode * thisNode ();
         //
         // Overrides from class Component.

      static const WitString & nodeName (const WitNode * theNode);
         //
         // Returns theNode->nodeName ();

      virtual int nodeIndex () const = 0;
         //
         // Base class function.
         // Mapping index for Nodes.
         // Note the value returned depends on the # of Parts in myProblem ().

   protected:

      //------------------------------------------------------------------------
      // Protected member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitNode (const char * theName, WitCompMgr * theCompMgr);
         //
         // nodeName_ <-- theName.
         // Component <<< theCompMgr.

      //------------------------------------------------------------------------
      // Other protected member functions.
      //------------------------------------------------------------------------

      virtual void writeDataAttrs ();
         //
         // Override from class Component.

      void copyAttrsFrom (WitNode * theNode);
         //
         // Copies the input attributes of theNode into this Node.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitNode);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Data set by the constructor.
      //------------------------------------------------------------------------

      WitString nodeName_;
         //
         // The string that uniquely identifies this Node, among Parts or
         // Operations (a Part may have the same name as an Operation.)
   };

#endif
