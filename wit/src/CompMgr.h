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

#ifndef CompMgrH
#define CompMgrH

//------------------------------------------------------------------------------
// Header file: "CompMgr.h"
//
// Contains the declaration of class CompMgr.
//------------------------------------------------------------------------------

#include <List.h>

//------------------------------------------------------------------------------
// Class CompMgr
//
// "Component Manager"
// Responsible for maintaining structural relationships between the Components
// of a Problem, without regard to the content of the Components.
//------------------------------------------------------------------------------

class WitCompMgr
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitCompMgr (WitProblem * theProblem);
         //
         // myProblem_ <-- theProblem.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitCompMgr ();

      //------------------------------------------------------------------------
      // Component inserting functions.
      // Each of these functions inserts its argument Component into this
      // CompMgr.
      // Checks that the argument has not previously been inserted.
      //------------------------------------------------------------------------

      void insert (WitGlobalComp *);
      void insert (WitOptComp *);
      void insert (WitMaterial *);
      void insert (WitCapacity *);
      void insert (WitOperation *);
      void insert (WitBomEntry *);
      void insert (WitSubEntry *);
      void insert (WitBopEntry *);
      void insert (WitDemand *);

      //------------------------------------------------------------------------
      // Component matching functions.
      // Each of these functions finds and returns the Component in this CompMgr
      // that belongs to the same class and has the same ID as the argument
      // Component. Checks that such a Component exists.
      //------------------------------------------------------------------------

      WitPart *      myMatchFor (WitPart *);
      WitMaterial *  myMatchFor (WitMaterial *);
      WitDemand *    myMatchFor (WitDemand *);
      WitOperation * myMatchFor (WitOperation *);
      WitSubEntry *  myMatchFor (WitSubEntry *);
      WitBoundSet *  myMatchFor (WitBoundSet *);

      //------------------------------------------------------------------------
      // find<Component> functions.
      // Each of these functions corresponds to a Component class. The function
      // finds and returns the Component class instance in this CompMgr
      // identified by the arguments, or NULL if no such Component exists.
      // Asserts that the Component argument (if any) belongs to this CompMgr.
      //------------------------------------------------------------------------

      WitPart *      findPart      (const char *   theName) const;
      WitOperation * findOperation (const char *   theName) const;
      WitBomEntry *  findBomEntry  (WitOperation * theOpn,    int theIndex);
      WitSubEntry *  findSubEntry  (WitBomEntry *  theBomEnt, int theIndex);
      WitBopEntry *  findBopEntry  (WitOperation * theOpn,    int theIndex);
      WitDemand *    findDemand    (WitPart *      thePart, 
                                    const char *   theName);

      //------------------------------------------------------------------------
      // Component recount functions for concrete classes.
      // Each of these functions counts its argument at the end of a purge.
      // Valid only the this CompMgr is calling recount on the arguement.
      // and only valid once for each such call.
      //------------------------------------------------------------------------

      void recountGlobalComp (WitGlobalComp *);
      void recountOptComp    (WitOptComp    *);
      void recountMaterial   (WitMaterial   *);
      void recountCapacity   (WitCapacity   *);
      void recountOperation  (WitOperation  *);
      void recountBomEntry   (WitBomEntry   *);
      void recountSubEntry   (WitSubEntry   *);
      void recountBopEntry   (WitBopEntry   *);
      void recountDemand     (WitDemand     *);

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void setPartName      (WitPart *      thePart, const char * theName);
      void setOperationName (WitOperation * theOpn,  const char * theName);
         //
         // Sets the partName      of thePart to theName.
         // Sets the operationName of theOpn  to theName.

      inline bool hasSubEntries () const
         {
         return (nSubEntries () > 0);
         }

      int nBoundSetsSpecified () const;
         //
         // Counts and returns the # of BoundSets in this CompMgr that are
         // specified.

      void purgeData ();
         //
         // Deletes all Components that are selected for deletion.

      bool isReindexing (WitBoundSet * theBoundSet) const;
         //
         // Returns true, iff this CompMgr is currently reindexing theBoundSet.

      bool isDeleting (WitComponent * theComp) const;
         //
         // Returns true, iff this CompMgr is currently deleting theComp.

      void storePreSortedNodes (const WitList <WitNode> & theNodes);
         //
         // Replaces allParts_ and allOperations_ with the contents of theNodes,
         // which is to be a sorted List.
         // Valid only when preprocessing is being performed.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitProblem *,                   myProblem)
      accessFunc (WitMsgFacility &,               myMsgFac)
      accessFunc (WitGlobalComp *,                myGlobalComp)
      accessFunc (WitOptComp *,                   myOptComp)
      accessFunc (const WitList <WitPart> &,      allParts)
      accessFunc (const WitList <WitOperation> &, allOperations)
      accessFunc (const WitList <WitComponent> &, allComponents)
      accessFunc (int,                            nParts)
      accessFunc (int,                            nMaterials)
      accessFunc (int,                            nOperations)
      accessFunc (int,                            nBomEntries)
      accessFunc (int,                            nSubEntries)
      accessFunc (int,                            nBopEntries)
      accessFunc (int,                            nDemands)
      accessFunc (int,                            nBoundSets)
      accessFunc (bool,                           clearingDepLists)
      accessFunc (bool,                           settingNodeName)
      accessFunc (WitDelCompItr *,                myDelCompItr)
      accessFunc (bool,                           purging)

      inline int nNodes () const
         {
         return nParts () + nOperations ();
         }

      inline int nConsEntries () const
         {
         return nBomEntries () + nSubEntries ();
         }

      inline int nBillEntries () const
         {
         return nConsEntries () + nBopEntries ();
         }

      inline int nDelComps () const
         {
         return allComponents ().nElements () - 2;
         }

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitCompMgr);

      //------------------------------------------------------------------------
      // Component recount functions for base classes.
      //------------------------------------------------------------------------

      void recountComponent (WitComponent *);
      void recountPart      (WitPart      *);
      void recountBillEntry (WitBillEntry *);
      void recountConsEntry (WitConsEntry *);

      //------------------------------------------------------------------------
      // Other private member functions.
      //------------------------------------------------------------------------

      void insertPart (WitPart * thePart);
         //
         // Inserts thePart into this CompMgr.

      void insertComp (WitComponent * theComp);
         //
         // Inserts theComp into this CompMgr.

      void setNodeName (WitNode * theNode, const char * theName);
         //
         // Sets the nodeName of theNode to theName, except for NodeTable
         // maintenance.

      int nSelForDel () const;
         //
         // Computes and returns the # of DelComps owned by this CompMgr that
         // are selected for deletion.

      void purgeCompLists ();
         //
         // Removes all selForDel Components from this CompMgr's Derived
         // Component Lists.

      void clearCaches ();
         //
         // Sets all the Component caches of the CompMgr to NULL.

      void recountComps ();
         //
         // Counts the instances of the Component classes and re-computes thier
         // mappingIndexes at the end of a purge.

      void deleteComps ();
         //
         // Clears allComponents_ and deletes all the Components.
         // The Components are deletes in reverse order so that each Component
         // will be deleted before any Component that it refers to.

      void deleteComp (WitComponent * theComp);
         //
         // Deletes theComp.

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // General purpose associations.
      //------------------------------------------------------------------------

      WitProblem * const myProblem_;
         //
         // The Problem that owns this CompMgr.

      WitMsgFacility & myMsgFac_;
         //
         // The MsgFacility for this CompMgr.

      //------------------------------------------------------------------------
      // Component ownership.
      //------------------------------------------------------------------------

      WitGlobalComp * myGlobalComp_;
         //
         // The GlobalComp owned by this CompMgr.

      WitOptComp    * myOptComp_;
         //
         // The OptComp owned by this CompMgr.

      WitList <WitPart> allParts_;
         //
         // The List of all Parts owned by this CompMgr.

      WitList <WitOperation> allOperations_;
         //
         // The List of all Operations owned by this CompMgr.

      //------------------------------------------------------------------------
      // Associations with Components.
      //------------------------------------------------------------------------

      WitList <WitComponent> allComponents_;
         //
         // The List of all Components for this CompMgr.

      WitDerNodeTable <WitPart> *      myPartTable_;
      WitDerNodeTable <WitOperation> * myOpnTable_;
         //
         // The DerNodeTables for finding the Parts and Operations of this
         // CompMgr.

      //------------------------------------------------------------------------
      // Counts of the various Components.
      //------------------------------------------------------------------------

      int nParts_;
      int nMaterials_;
      int nOperations_;
      int nBomEntries_;
      int nSubEntries_;
      int nBopEntries_;
      int nDemands_;
      int nBoundSets_;

      //------------------------------------------------------------------------
      // Caches of Components.
      //------------------------------------------------------------------------

      WitDemand * cachedDemand_;
         //
         // The Demand most recently constructed or found via findDemand.

      WitBomEntry * cachedBomEntry_;
         //
         // The BomEntry most recently constructed or found via findBomEntry

      WitSubEntry * cachedSubEntry_;
         //
         // The SubEntry most recently constructed or found via findSubEntry

      WitBopEntry * cachedBopEntry_;
         //
         // The BopEntry most recently constructed or found via findBopEntry

      //------------------------------------------------------------------------
      // Other private member data.
      //------------------------------------------------------------------------

      WitDelCompItr * myDelCompItr_;
         //
         // The DelCompItr owned by this CompMgr.

      bool purging_;
         //
         // true, iff this CompMgr is currently doing a purgeData.

      WitComponent * recountingComp_;
         //
         // The Component currently being re-counted by this CompMgr, if any,
         // else NULL.

      WitComponent * deletingComp_;
         //
         // The Component currently being deleted by this CompMgr, if any,
         // else NULL.

      bool clearingDepLists_;
         //
         // true, iff the dependent Lists of the Components are being cleared.

      WitNode * nameChangeNode_;
         //
         // The Node whose nodeName is currently being changed if any;
         // otherwise NULL.

      bool settingNodeName_;
         //
         // True, iff this CompMgr is currently setting the nodeName of a Node.
   };

//------------------------------------------------------------------------------
// Component iteration macros.
//
// Each of the following macros is associated with some Component class and
// facilitates iterating thru all of the class's instances belonging to a
// Problem.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Iterator name macros.
// Each of these macros expands to the name of a local AbsItr variable whose
// name is unique to the line on which the macro is invoked.
//------------------------------------------------------------------------------

#define localItr1  name2 (localItr1OnLine, __LINE__)
#define localItr2  name2 (localItr2OnLine, __LINE__)

//------------------------------------------------------------------------------
// forEachBomEntry
// Expands to the header of a double nested "for" loop causing theBomEnt to
// iterate thru all the BomEntries in theProblem.
//------------------------------------------------------------------------------

#define forEachBomEntry(theBomEnt, theProblem)                                 \
                                                                               \
   for  (WitListItr localItr1 ((theProblem)->myCompMgr ()->allOperations ());  \
         localItr1.advance ();)                                                \
      forEachEl (theBomEnt, WitOperation::key (localItr1)->bom ())             \

//------------------------------------------------------------------------------
// forEachBopEntry
// Expands to the header of a double nested "for" loop causing theBopEnt to
// iterate thru all the BopEntries in theProblem
//------------------------------------------------------------------------------

#define forEachBopEntry(theBopEnt, theProblem)                                 \
                                                                               \
   for  (WitListItr localItr1 ((theProblem)->myCompMgr ()->allOperations ());  \
         localItr1.advance ();)                                                \
      forEachEl (theBopEnt, WitOperation::key (localItr1)->bop ())             \

//------------------------------------------------------------------------------
// forEachSubEntry
// Expands to the header of a triple nested "for" loop causing theSub to
// iterate thru all the SubEntries in theProblem.
//------------------------------------------------------------------------------

#define forEachSubEntry(theSub, theProblem)                                    \
                                                                               \
   if ((theProblem)->myCompMgr ()->hasSubEntries ())                           \
      for (WitListItr localItr1 ((theProblem)->myCompMgr ()->allOperations ());\
            localItr1.advance ();)                                             \
         for  (WitListItr localItr2 (WitOperation::key (localItr1)->bom ());   \
               localItr2.advance ();)                                          \
            forEachEl (theSub, WitBomEntry::key (localItr2)->mySubEntries ())  \

//------------------------------------------------------------------------------
// forEachDemand
// Expands to the header of a double nested "for" loop causing theDemand to
// iterate thru all the Demands in theProblem.
//------------------------------------------------------------------------------

#define forEachDemand(theDemand, theProblem)                                   \
                                                                               \
   for  (WitListItr localItr1 ((theProblem)->myCompMgr ()->allParts ());       \
         localItr1.advance ();)                                                \
      forEachEl (theDemand, WitPart::key (localItr1)->myDemands ())            \

//------------------------------------------------------------------------------
// forEachOperation
// Expands to the header of a "for" loop causing theOpn to
// iterate thru all the Operations in theProblem.
//------------------------------------------------------------------------------

#define forEachOperation(theOpn, theProblem)                                   \
                                                                               \
   forEachEl (theOpn, (theProblem)->myCompMgr ()->allOperations ())            \

//------------------------------------------------------------------------------
// forEachPart
// Expands to the header of a "for" loop causing thePart to
// iterate thru all the Parts in theProblem.
//------------------------------------------------------------------------------

#define forEachPart(thePart, theProblem)                                       \
                                                                               \
   forEachEl (thePart, (theProblem)->myCompMgr ()->allParts ())                \

//------------------------------------------------------------------------------
// forEachMaterial
// Expands to the header of a "for" loop causing theMat to
// iterate thru all the Materials in theProblem.
//------------------------------------------------------------------------------

#define forEachMaterial(theMat, theProblem)                                    \
                                                                               \
   for  (WitListItr localItr1 ((theProblem)->myCompMgr ()->allParts ());       \
         localItr1.advance ();)                                                \
      if (theMat = WitPart::key (localItr1)->thisMat (), (theMat != NULL))     \

#endif
