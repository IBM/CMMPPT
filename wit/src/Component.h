//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef ComponentH
#define ComponentH

//------------------------------------------------------------------------------
// Header file: "Component.h"
//
// Contains the declaration of class Component.
//------------------------------------------------------------------------------

#include <Assoc.h>

//------------------------------------------------------------------------------
// class Component.
//
// A component of a WIT problem.
// The collection of Components of a WIT problem define the problem.
//
// Class Hierarchy:
//
// ProbAssoc
//    Component
//       GlobalComp
//       OptComp
//       DelComp
//          Node
//             Part
//                Material
//                Capacity
//             Operation
//          BillEntry
//             ConsEntry
//                BomEntry
//                SubEntry
//             BopEntry
//          Demand
//------------------------------------------------------------------------------

class WitComponent: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitComponent ();
         //
         // This function can only be called from myCompMgr ().

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (void *, appData)

      //------------------------------------------------------------------------
      // "Set" functions.
      //------------------------------------------------------------------------

      void setAppData (void *);

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      virtual bool inserted () = 0;
         //
         // Returns true, iff this Component has been inserted into
         // myCompMgr ().

      void writeData ();
         //
         // Writes the section of the input data file for this Component.

      virtual void writeMultiObjVecs (WitObjective * theObj);
         //
         // Valid in multiple objectives mode only.
         // Writes to an input data file the ObjVecIRs owned by this Component.
         // For each ObjVecIR, the vector for theObj is written.
         // The default implementation does nothing.

      virtual void writeSolnData ();
         //
         // Writes the section of the solution data file for this Component.
         // This default version is a no-op.

      virtual void copySelfInto (WitCompMgr * theCompMgr) = 0;
         //
         // Creates a corresponding Component in theCompMgr, unless this
         // Component is the kind that automatically gets constructed by the
         // ctor of class Problem. Copies the input attributes of this Component
         // into the corresponding Component in theCompMgr.

      void purgeCompLists ();
         //
         // Removes all selForDel Components from this Component's Component
         // Lists.
         // Valid only during a purge.

      void reindexBoundSet ();
         //
         // Sets the mappingIndex of the BoundSet for this Component.
         // Valid only if this Component has a BoundSet, and myCompMgr () is
         // reindexing it.

      virtual void recount () = 0;
         //
         // Recomputes mappingIndex for this Component and tells myCompMgr ()
         // to recount this Component. This may be done for multiple levels of
         // derived classes.
         // Valid only if called by myCompMgr ().

      void clearDepLists ();
         //
         // Clears the dependent Lists of this Component.

      virtual WitBoundSet * myBoundSet ();
         //
         // Returns the BoundSet owned by this Component, if any, else NULL.
         // The base class implementation returns NULL.

      virtual void unpreprocess ();
         //
         // Puts this Component into its unpreprocessed state.

      virtual void clearOptSoln ();
         //
         // Clears this Component's portion of the solution.
         // Valid only when the optimizing implosion subsystem is clearing the
         // solution.

      virtual void clearStochSoln ();
         //
         // Clears this Component's portion of the stochastic solution.
         // Valid only when the stochastic implosion subsystem is clearing the
         // stochastic solution.

      //------------------------------------------------------------------------
      // This-derived-class-instance functions.
      //
      // In each case, if this Component is actually an instance of the
      // indicated derived class, the function returns that instance;
      // otherwise, it returns NULL.
      //------------------------------------------------------------------------

      virtual WitDelComp *   thisDelComp ();
      virtual WitNode *      thisNode    ();
      virtual WitPart *      thisPart    ();
      virtual WitMaterial *  thisMat     ();
      virtual WitCapacity *  thisCap     ();
      virtual WitOperation * thisOpn     ();
      virtual WitConsEntry * thisConsEnt ();
      virtual WitBomEntry *  thisBomEnt  ();
      virtual WitSubEntry *  thisSub     ();
      virtual WitBopEntry *  thisBopEnt  ();
      virtual WitDemand *    thisDemand  ();

   protected:

      //------------------------------------------------------------------------
      // Protected member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitComponent (WitCompMgr * theCompMgr);
         //
         // theCompMgr is the CompMgr that owns this Component.

      //------------------------------------------------------------------------
      // Other protected member functions.
      //------------------------------------------------------------------------

      void prepSetUnpreAttr ();
         //
         // Takes action appropriate just before the value of an attribute is to
         // be set, thereby necessitating unpreprocessing.
         // The attribute is not allowed to be set in stochastic mode.

      void prepSetUnpostAttr ();
         //
         // Takes action appropriate just before the value of an attribute is to
         // be set, thereby necessitating unpostprocessing.
         // The attribute is not allowed to be set in stochastic mode.

      void prepSetNonUnpostAttr ();
         //
         // Takes action appropriate just before the value of an attribute is to
         // be set without necessitating unpreprocessing or unpostprocessing.
         // The attribute is not allowed to be set in stochastic mode.

      virtual void writeDataID () = 0;
         //
         // Writes text identifying this Component to an input data file.

      virtual void writeDataAttrs () = 0;
         //
         // Writes the attributes of this Component to an input data file.

      void copyAttrsFrom (WitComponent * theComp);
         //
         // Copies the input attributes of theComp into this Component.

      virtual void purgeCompListsSpec ();
         //
         // Does aspects of purgeCompLists specific to the derived class.
         // The base class implementation is a no-op.

      virtual void clearDepListsSpec ();
         //
         // Does aspects of clearDepLists specific to the derived class.
         // The base class implementation is a no-op.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitComponent);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Input data.
      //------------------------------------------------------------------------

      void * appData_;
         //
         // The application data that has been attached to this Component, if
         // any.
   };

#endif
