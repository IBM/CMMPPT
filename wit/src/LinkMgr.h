//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef LinkMgrH
#define LinkMgrH

//------------------------------------------------------------------------------
// Header file: "LinkMgr.h"
//
// Contains the declaration of the following classes and templates:
//
//    AbsLinkMgr
//    ObjLinkMgr
//    PairLinkMgr <ObjByVal>
//    TripLinkMgr
//------------------------------------------------------------------------------

#include <Link.h>
#include <Assoc.h>

//------------------------------------------------------------------------------
// Class AbsLinkMgr
//
// A manager of abstract dynamic stack links.
// Responsible for owning and recycling the AbsLinks of a Problem.
//
// Class Hierarchy:
//
// AbsLinkMgr
//    ObjLinkMgr
//    PairLinkMgr <ObjByVal>
//    TripLinkMgr
//
// Implemented in Stack.C
//------------------------------------------------------------------------------

class WitAbsLinkMgr: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Public member functions.
      //------------------------------------------------------------------------

      static void deleteInstance (WitAbsLinkMgr * theAbsLinkMgr);
         //
         // Heap-destructs theAbsLinkMgr.

      WitAbsLink * freshAbsLink (WitAbsLink * theAbsLink);
         //
         // Attaches an available AbsLink to theAbsLink and returns it.

      void recycleAbsLink (WitAbsLink * theAbsLink);
         //
         // Stores theAbsLink for future re-use.

   protected:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitAbsLinkMgr (WitProblem * theProblem);
         //
         // ProbAssoc <<< theProblem.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitAbsLinkMgr ();

      //------------------------------------------------------------------------
      // Other protected member functions.
      //------------------------------------------------------------------------

      virtual WitAbsLink * newAbsLink () = 0;
         //
         // Heap-constructs and returns new instance of a class derived from 
         // AbsLink.
         // Base class function.

      virtual void deleteAbsLink (WitAbsLink * theAbsLink) = 0;
         //
         // Heap-destructs the derived class version of theAbsLink.
         // Base class function.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitAbsLinkMgr);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitAbsLink * firstUnusedAbsLink_;
         //
         // First unused AbsLink stored by this AbsLinkMgr.
   };

//------------------------------------------------------------------------------
// Class ObjLinkMgr
//
// A manager of links for dynamic stacks of individual objects.
// Responsible for owning and recycling the ObjLinks of a Problem.
//
// Class Hierarchy:
//
// AbsLinkMgr
//    ObjLinkMgr
//
// Implemented in Stack.C
//------------------------------------------------------------------------------

class WitObjLinkMgr: public WitAbsLinkMgr
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitObjLinkMgr (WitProblem * theProblem);
         //
         // AbsLinkMgr <<< theProblem.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitObjLinkMgr);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitObjLinkMgr ();
         //
         // Override from class AbsLinkMgr.                                 

      //------------------------------------------------------------------------
      // Other private member functions.
      //------------------------------------------------------------------------

      virtual WitAbsLink * newAbsLink ();
         //
         // Override from class AbsLinkMgr.

      virtual void deleteAbsLink (WitAbsLink *);
         //
         // Override from class AbsLinkMgr.
   };

//------------------------------------------------------------------------------
// Class Template PairLinkMgr <ObjByVal>
//
// A manager of dynamic stack links for pairs of the form: (Object, ObjByVal).
// Responsible for owning and recycling the PairLinks of a Problem.
//
// Class Hierarchy:
//
// AbsLinkMgr
//    PairLinkMgr <ObjByVal>
//
// Implemented in Stack.C.
//------------------------------------------------------------------------------

template <typename ObjByVal> 
      class WitPairLinkMgr: public WitAbsLinkMgr
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitPairLinkMgr (WitProblem *);

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      static WitPairLinkMgr * theInstanceFor (WitProblem * theProblem);
         //
         // Returns the PairLinkMgr <ObjByVal> owned by theProblem.
         // Implemented non-parametrically in Stack.C.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitPairLinkMgr ();
         //
         // Override from class AbsLinkMgr.

      //------------------------------------------------------------------------
      // Other private member functions.
      //------------------------------------------------------------------------

      virtual WitAbsLink * newAbsLink    ();
      virtual void         deleteAbsLink (WitAbsLink *);
         //
         // Overrides from class AbsLinkMgr.

      noCopyCtorAssign (WitPairLinkMgr);
   };

//------------------------------------------------------------------------------
// Class TripLinkMgr
//
// A manager of links for dynamic stacks of triples.
// Responsible for owning and recycling the TripLinks of a Problem.
//
// Class Hierarchy:
//
// AbsLinkMgr
//    TripLinkMgr
//
// Implemented in Stack.C
//------------------------------------------------------------------------------

class WitTripLinkMgr: public WitAbsLinkMgr
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitTripLinkMgr (WitProblem *);

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitTripLinkMgr);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitTripLinkMgr ();
         //
         // Override from class AbsLinkMgr.                                 

      //------------------------------------------------------------------------
      // Other private member functions.
      //------------------------------------------------------------------------

      virtual WitAbsLink * newAbsLink ();
         //
         // Override from class AbsLinkMgr.

      virtual void deleteAbsLink (WitAbsLink *);
         //
         // Override from class AbsLinkMgr.
   };

#endif
