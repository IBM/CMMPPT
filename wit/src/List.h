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

#ifndef ListH
#define ListH

//------------------------------------------------------------------------------
// Header file: "List.h"
//
// Contains the declaration of the following classes and templates:
//
//    AbsList
//    ListItr
//    List <Elem>
//------------------------------------------------------------------------------

#include <Util.h>

#include <list>

//------------------------------------------------------------------------------
// class WitAbsList (Abstract List)
//
// An AbsList is a list whose elements are void pointers.
// It is implemented as a wrapper for std::list <void *>.
//
// AbsList
//    List <Elem>
//------------------------------------------------------------------------------

typedef const WitString & (* WitAbsListKeyFunc) (const void *);
   //
   // "Key" function for AbsList::findAbs.

class WitAbsList
   {
      friend class WitListItr;

   public:

      //------------------------------------------------------------------------
      // Public member functions.
      //------------------------------------------------------------------------

      void clear ();
         //
         // Removes all of the elements from this AbsList.

      inline bool isEmpty () const
            //
            // Returns true iff this AbsList is empty.
         {
         return (nElements_ == 0);
         }

      accessFunc (int, nElements)

   protected:

      //------------------------------------------------------------------------
      // Protected member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitAbsList ();
         //
         // Constructs an empty list.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitAbsList ();

      //------------------------------------------------------------------------
      // Other protected wrappers.
      //------------------------------------------------------------------------

      void copyAbs (const WitAbsList & theAbsList);
         //
         // Makes this AbsList a copy of theAbsList.

      void appendAbs (void * theObject);
         //
         // Adds theObject to the end of this AbsList.
         // Generates a severe error if the appending was unsuccessful, or
         // if theObject is NULL.

      void * firstAbs () const;
         //
         // Returns the first element in this AbsList.

      void * getAbs ();
         //
         // Returns and removes the first element in this AbsList.

      void * lastAbs () const;
         //
         // Returns the last element in this AbsList.

      void prependAbs (void * theObjByVP);
         //
         // Adds theObjByVP to the beginning of this AbsList.
         // The ObjByVP must not be NULL.

      void * atAbs (int theIndex) const;
         //
         // Returns a pointer to the element in this AbsList whose index ==
         // theIndex, where the indices are 0, 1, ..., nElements - 1.
         // If theIndex is outside this range, an RW error is issued.
         // Use this carefuly: this is a very expensive method to call,
         // and should not be used from a loop.

      void * findAbs (
            WitAbsListKeyFunc theFunc,
            const char *      theCstring)
            const;
         //
         // Return the first element of this AbsList, theElement, such that
         // theFunc (theElement) == theCstring, if any, else NULL.

      //------------------------------------------------------------------------
      // Non-wrapper member functions.
      //------------------------------------------------------------------------

      void appendUniquelyAbs (void * theObject);
         //
         // Same as append, but asserts that theObject is not already in this
         // AbsList.

      bool containsAbs (void * theObject) const;
         //
         // Returns true, iff theObject is in this AbsList.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitAbsList);

      std::list <void *> myListData_;
         //
         // The std::list <void *> for which this AbsList is a wrapper.

      int nElements_;
         //
         // The number of elements in this AbsList.
   };

//------------------------------------------------------------------------------
// Class ListItr
//
// "List Iterator"
//
// An iterator for an AbsList. Facilitates interating thru the elements of an
// AbsList, to which is it "attached" at contruction time. Implemented as
// a wrapper for class list::iterator <void *>. Since AbsList is only a base
// class for generic class List <Elem>, ListItrs are actually always attached
// List <Elem>s, but only interact with them using the AbsList interface.
// Class ListItr is not intended to be used directly, but instead, used only
// thru various macros:
//    forEachEl       in List.h
//    forEach<object> in Problem.h
//------------------------------------------------------------------------------

class WitListItr
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitListItr (const WitAbsList & theAbsList);
         //
         // Constucts an ListItr attached to theAbsList.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitListItr ();

      //------------------------------------------------------------------------
      // Public member functions.
      //------------------------------------------------------------------------

      void * key ();
         //
         // Returns the element to which this ListItr points.

      bool advance ();
         //
         // Advances this Itr in its AbsList.
         // Returns true iff the advance did not move beyond the end of the
         // AbsList.

      bool advance (
            void * &           theElement, 
            const WitAbsList & theAbsList);
         //
         // Advances this Itr. If this Itr is still within theAbsList after the
         // advance, this function sets theElement to its key and returns
         // true. Otherwise this function sets theElement to NULL and
         // returns false. This Itr is required to be attached to theAbsList.

   private:

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      std::list <void *>::const_iterator myListIterator_;

      const WitAbsList & myAbsList_;

      bool iterating_;
   };

//------------------------------------------------------------------------------
// Class template List <Elem>
//
// A List <Elem> is a singly-linked list whose elements are instances of Elem.
//
// Class Hierarchy:
//
// AbsList
//    List <Elem>
//
// All implementation is inline.
//------------------------------------------------------------------------------

template <typename Elem> 
      class WitList: public WitAbsList
   {
   public:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      inline WitList ():
            WitAbsList ()
         {
         }

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      inline ~WitList ()
         {
         }

      //-----------------------------------------------------------------------
      // Wrappers around AbsList member functions.
      //-----------------------------------------------------------------------

      inline void operator = (const WitList & theList)
         {
         copyAbs (theList);
         }

      inline void append (Elem * theInstance)
         {
         appendAbs (theInstance);
         }

      inline Elem * first () const
         {
         return reinterpret_cast <Elem *> (firstAbs ());
         }

      inline Elem * get ()
         {
         return reinterpret_cast <Elem *> (getAbs ());
         }

      inline Elem * last () const
         {
         return reinterpret_cast <Elem *> (lastAbs ());
         }

      inline void prepend (Elem * theInstance)
         {
         prependAbs (theInstance);
         }

      inline Elem * at (int theIndex) const
         {
         return reinterpret_cast <Elem *> (atAbs (theIndex));
         }

      inline bool advance (WitListItr & theItr, Elem * & theElement) const
         {
         return
            theItr.advance (
                 reinterpret_cast <void * &> (theElement),
               * this);
         }

      inline void appendUniquely (Elem * theObject)
         {
         appendUniquelyAbs (theObject);
         }

      inline bool contains (Elem * theObject) const
         {
         return containsAbs (theObject);
         }

      inline Elem * find (
            const WitString & (* theFunc) (const Elem *),
            const char *         theCstring)
            const
         {
         return
            reinterpret_cast <Elem *> (
               findAbs (
                  reinterpret_cast <WitAbsListKeyFunc> (theFunc),
                  theCstring));
         }

   private:

      //-----------------------------------------------------------------------
      // Private member functions.
      //-----------------------------------------------------------------------

      noCopyCtor (WitList);
   };

//------------------------------------------------------------------------------
// macro forEachEl (theElement, theList)
//
// Expands to a header of a for loop that iterates thru each element in
// theList.
//
// Arguments:
//
//    theList:    Type: const List <Elem>, for some type, Elem.
//                The list being iterated.
//
//    theElement: Type: Elem *
//                A pointer that will be set to each element of theList, in
//                sequence. The body of the for loop should normally make use
//                of this pointer.
//
// Note: If theList is empty, the forEachEl loop does nothing (which is to be
// expected).
//
// Example:
//
// WitList <WitBomEntry> bom;
//
// ...Fill up bom with BomEntries...
//
// WitBomEntry * theBomEnt;
//
// forEachEl (theBomEnt, bom)
//    {
//    ...statements using theBomEnt...
//    }
//
// The implementation of this macro calls the macro forEachElItr (defined
// below), which expands to a name unique to the line of source code from which
// the forEachElItr macro was called. This name is used as the name of a
// ListItr, which is used to implement the forEachEl macro. It is necessary for
// the forEachEl macro to use a different name for its ListItr each time it is
// called, because it might get called more than once from within the same
// scope.
//------------------------------------------------------------------------------

#define forEachElItr name2 (forEachElItrOnLine, __LINE__)

//------------------------------------------------------------------------------

#define forEachEl(theElement, theList)                                         \
                                                                               \
   for (                                                                       \
      WitListItr forEachElItr (theList);                                       \
      (theList).advance (forEachElItr, theElement);                            \
      )                                                                        \

#endif
