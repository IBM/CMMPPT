//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef StackH
#define StackH

//------------------------------------------------------------------------------
// Header file: "Stack.h"
//
// Contains the declaration of the following classes:
//
//    AbsStack
//    AbsStItr
//------------------------------------------------------------------------------

#include <Util.h>

//------------------------------------------------------------------------------
// Class AbsStack
//
// A stack of undefined elements.
//
// Class Hierarchy:
//
// AbsStack
//    AbsObjStack
//       ObjStack <ObjByPtr>
//    PairStack   <ObjByPtr, ObjByVal>
//
// Implemented in Stack.C
//------------------------------------------------------------------------------

class WitAbsStack
   {
   public:

      //------------------------------------------------------------------------
      // Public member functions.
      //------------------------------------------------------------------------

      inline bool isNonEmpty () const
         {
         return not isEmpty ();
         }

      inline bool isEmpty () const
         {
         return (firstAbsLink_ == NULL);
         }

      void clear ();
         //
         // Removes all of the elements from this AbsStack.

      int nElements () const;
         //
         // Computes (by traversal) and returns the # of elements in this
         // AbsStack.

      void reverse ();
         //
         // Reverses the order of the elements in this AbsStack.

      WitProblem * myProblem () const;
         //
         // Returns the Problem associated with this AbsStack.

      inline const WitAbsLink * firstConstLink () const
         {
         return firstAbsLink_;
         }

   protected:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitAbsStack (WitAbsLinkMgr *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitAbsStack ();

      //------------------------------------------------------------------------
      // Other protected functions.
      //------------------------------------------------------------------------

      void pushAbs ();
         //
         // Pushes a new element onto the top of this AbsStack.

      void popAbs ();
         //
         // This AbsStack must be is non-Empty.
         // Removes the first element from this AbsStack.

      void takeContentsFromAbs (WitAbsStack & thatAbsStack);
         //
         // Assumes this AbsStack is empty.
         // Takes the contents of thatAbsStack and moves them into this
         // AbsStack.
         // This is done in constant time.

      void attachItrAbs (WitAbsStItr & theAbsStItr) const;
         //                                                                 
         // Puts theAbsStItr into its initial state for iterating through this
         // AbsStack.                                        

      accessFunc (WitAbsLink *, firstAbsLink)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitAbsStack);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitAbsLinkMgr * const myAbsLinkMgr_;
         //
         // The AbsLinkMgr to be used by this AbsStack.

      WitAbsLink * firstAbsLink_;
         //
         // The first AbsLink in this AbsStack.
   };

//------------------------------------------------------------------------------
// Class AbsStItr
//
// Responsible for iterating through the elements of an AbsStack.
//
// Class Hierarchy:
//
// AbsStItr
//    AbsObjStItr
//       ObjStItr  <ObjByPtr>
//    PairStItr <ObjByPtr, ObjByVal>
//
// Implemented in Stack.C
//------------------------------------------------------------------------------

class WitAbsStItr                                
   {                                                                         
   friend class WitAbsStack;

   protected:

      //------------------------------------------------------------------------
      // Constructor functions.                                              
      //------------------------------------------------------------------------
                                                                             
      WitAbsStItr ();                                            
                                                                             
      //------------------------------------------------------------------------
      // Destructor function.                                                
      //------------------------------------------------------------------------
                                                                             
      ~WitAbsStItr ();                                           
                                                                             
      //------------------------------------------------------------------------
      // Other protected member functions.                                   
      //------------------------------------------------------------------------
                                                                            
      void advanceAbs ();
         //                                                                 
         // If this AbsStItr is in its initial state, this              
         // function advances it to the first element of the AbsStack;  
         // otherwise it advances it to the next element.                   

      accessFunc (const WitAbsLink *, curAbsLink)

   private:                                                                  
                                                                             
      //------------------------------------------------------------------------
      // Private member functions.                                           
      //------------------------------------------------------------------------
                                                                          
      noCopyCtorAssign (WitAbsStItr);                           
                                                                          
      //------------------------------------------------------------------------
      // Private member data.                                                
      //------------------------------------------------------------------------

      const WitAbsStack * myAbsStack_;
         //
         // The AbsStack to which this AbsStItr is currently attached, if any;
         // otherwise NULL.
                                                                             
      const WitAbsLink * curAbsLink_;
         //                                                                  
         // The AbsLink currently associated with this AbsStItr,
         // or NULL, if this AbsStItr is in its initial state.
   };

#endif
