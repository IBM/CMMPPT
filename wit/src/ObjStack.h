//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef ObjStackH
#define ObjStackH

//------------------------------------------------------------------------------
// Header file: "ObjStack.h"
//
// Contains the declaration of the following classes and templates:
//
//    AbsObjStack
//    AbsObjStItr
//    ObjStack <ObjByPtr>
//    ObjStItr <ObjByPtr>
//------------------------------------------------------------------------------

#include <Stack.h>
#include <Link.h>

//------------------------------------------------------------------------------
// Class AbsObjStack
//
// A stack of individual objects of undefined type.
//
// Class Hierarchy:
//
// AbsStack
//    AbsObjStack
//       ObjStack <ObjByPtr>
//
// Implemented in Stack.C
//------------------------------------------------------------------------------

class WitAbsObjStack: public WitAbsStack
   {
   protected:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitAbsObjStack (WitProblem *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitAbsObjStack ();

      //------------------------------------------------------------------------
      // Other protected member functions.
      //------------------------------------------------------------------------

      void revCopyFromAbs (const WitAbsObjStack & theAbsObjStack);
         //
         // Copies the contents of theAbsObjStack into this AbsObjStack in 
         // reverse order.

      bool containsAbs (void * theObjByVP) const;
         //
         // Returns true, iff this AbsObjStack contains theObjByVP.

      void pushAbsObj (void * theObjByVP);
         //
         // Pushes theObjByVP onto the top of this AbsObjStack.

      bool popAbsObj (void * & theObjByVP);
         //
         // If this AbsObjStack is non-Empty, the first object it contains is
         // removed from it, theObjByVP is set to the removed object, and true
         // is returned;
         // otherwise, theObjByVP is set to NULL, and false is returned.

      void attachItrAbsObj (WitAbsObjStItr & theAbsObjStItr) const;
         //
         // Puts theAbsObjStItr into its initial state for iterating through
         // this AbsObjStack.

      void removeAbs (void * theObjByVP);
         //
         // Removes theObjByVP from this AbsObjStack (if it's there).

      void * firstObjByVP () const;
         //
         // Returns the first object in this AbsObjStack.
         // Not to be called if this AbsObjStack is empty.

      inline WitObjLink * firstObjLink () const
         {
         return reinterpret_cast <WitObjLink *> (firstAbsLink ());
         }

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitAbsObjStack);
   };

//------------------------------------------------------------------------------
// Class AbsObjStItr
//
// Responsible for iterating through the elements of an AbsObjStack.
//
// Class Hierarchy:
//
// AbsStItr
//    AbsObjStItr
//       ObjStItr <ObjByPtr>
//
// Implemented in Stack.C
//------------------------------------------------------------------------------

class WitAbsObjStItr: public WitAbsStItr       
   {
   friend class WitAbsObjStack;

   protected:

      //------------------------------------------------------------------------
      // Constructor functions.                                              
      //------------------------------------------------------------------------
                                                                             
      WitAbsObjStItr ();                                            
                                                                             
      //------------------------------------------------------------------------
      // Destructor function.                                                
      //------------------------------------------------------------------------
                                                                             
      ~WitAbsObjStItr ();                                           
                                                                             
      //------------------------------------------------------------------------
      // Other protected member functions.                                   
      //------------------------------------------------------------------------

      bool advanceAbsObj (void * & theObjByVP);
         //
         // See AbsStItr::advanceAbs.
         // If the advance is successful, theObjByVP is set to the next object;
         // otherwise                     theObjByVP is set to NULL.

   private:                                                                  
                                                                             
      //------------------------------------------------------------------------
      // Private member functions.                                           
      //------------------------------------------------------------------------
                                                                          
      noCopyCtorAssign (WitAbsObjStItr);                           
   };

//------------------------------------------------------------------------------
// Class template ObjStack <ObjByPtr>
//
// A stack of individual objects of type ObjByPtr.
//
// Class Hierarchy:
//
// AbsStack
//    AbsObjStack
//       ObjStack <ObjByPtr>
//
// All implementation is inline.
//------------------------------------------------------------------------------

template <typename ObjByPtr> 
      class WitObjStack: public WitAbsObjStack
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      inline WitObjStack (WitProblem * theProblem):
            WitAbsObjStack (theProblem)
         {
         }

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      inline ~WitObjStack ()
         {
         }

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      inline void revCopyFrom (const WitObjStack & theObjStack)
         {
         revCopyFromAbs (theObjStack);
         }

      inline bool contains (ObjByPtr * theObject) const
         {
         return containsAbs (theObject);
         }

      inline void push (ObjByPtr * theObject)
         {
         pushAbsObj (theObject);
         }

      inline bool pop (ObjByPtr * & theObject)
         {
         return popAbsObj (reinterpret_cast <void * &> (theObject));
         }

      inline void attachItr (WitObjStItr <ObjByPtr> & theObjStItr) const
         {
         attachItrAbsObj (theObjStItr);
         }

      inline void remove (ObjByPtr * theObject)
         {
         removeAbs (theObject);
         }

      inline void takeContentsFrom (WitObjStack & thatObjStack)
         {
         takeContentsFromAbs (thatObjStack);
         }

      inline ObjByPtr * firstObject () const
         {
         return reinterpret_cast <ObjByPtr *> (firstObjByVP ());
         }

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitObjStack);
   };

//------------------------------------------------------------------------------
// NonClass function template deleteContents.
//
// Deletes all of the objects stored by theObjStack and clears theObjStack.
//------------------------------------------------------------------------------

namespace WitNonClass
   {
   template <typename ObjByPtr> 
         void deleteContents (WitObjStack <ObjByPtr> & theObjStack)
      {
      ObjByPtr * theObject;
   
      while (theObjStack.pop (theObject))
         delete theObject;
      }
   }

//------------------------------------------------------------------------------
// NonClass function template revCopyInto.
//
// Makes theObjStack a reverse copy of theList.
//------------------------------------------------------------------------------

namespace WitNonClass
   {
   template <typename ObjByPtr> 
      void revCopyInto (
         WitObjStack <ObjByPtr> &   theObjStack,
         const WitList <ObjByPtr> & theList);
   }

//------------------------------------------------------------------------------
// Class template ObjStItr <ObjByPtr>
//
// Responsible for iterating through the elements of an ObjStack <ObjByPtr>.
//
// Class Hierarchy:
//
// AbsStItr
//    AbsObjStItr
//       ObjStItr <ObjByPtr>
//
// All implementation is inline.
//------------------------------------------------------------------------------

template <typename ObjByPtr> 
      class WitObjStItr: public WitAbsObjStItr
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      inline WitObjStItr ():
            WitAbsObjStItr ()
         {
         }

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      inline ~WitObjStItr ()
         {
         }

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      inline bool advance (ObjByPtr * & theObject)
         {
         return advanceAbsObj (reinterpret_cast <void * &> (theObject));
         }

      inline ObjByPtr * myObject () const
         {
         return reinterpret_cast <ObjByPtr *> (myObjByVP ());
         }

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      inline void * myObjByVP () const
         {
         return static_cast <const WitObjLink *> (curAbsLink ())->myObjByVP ();
         }

      noCopyCtorAssign (WitObjStItr);
   };

#endif
