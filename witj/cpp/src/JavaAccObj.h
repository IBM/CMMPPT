//------------------------------------------------------------------------------
// WIT-J C++ Header File JavaAccObj.h.
//
// Contains the declaration of class JavaAccObj.
//------------------------------------------------------------------------------

#ifndef JavaAccObj_h
#define JavaAccObj_h

#include <Includes.h>

//------------------------------------------------------------------------------
// Class JavaAccObj
//
// "Java Accessible Object"
// This class and the classes derived from it are set up to be accessible from
// the Java side of WIT-J.
//
// Class Hierarchy:
//
// JavaAccObj
//    Coordinator
//    AttBldr
//    Att
//       ProbScalarAtt    <Value>
//       GlobalScalarAtt  <Value>
//       PartScalarAtt    <Value>
//       DemandScalarAtt  <Value>
//       OpnScalarAtt     <Value>
//       BomScalarAtt     <Value>
//       SubScalarAtt     <Value>
//       BopScalarAtt     <Value>
//       ProbScalarAttString
//       ProbVectorAtt    <Elem>
//       PartVectorAtt    <Elem>
//       DemandVectorAtt  <Elem>
//       OpnVectorAtt     <Elem>
//       BomVectorAtt     <Elem>
//       SubVectorAtt     <Elem>
//       BopVectorAtt     <Elem>
//    MessageMgr
//    Component
//       Problem
//       Part
//       Demand
//       Operation
//       BomEntry
//       Substitute
//       BopEntry
//    CaseToken
//------------------------------------------------------------------------------

namespace WitJ
{
class JavaAccObj
   {
   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   virtual ~JavaAccObj ();

   inline bool isValid ()
      {
      return (validityMarker_ == validMark_);
      }

   //---------------------------------------------------------------------------
   // as{derived class} functions.
   //
   // In each case, if this JavaAccObj is actually an instance of the
   // indicated derived class, the function returns that instance;
   // Otherwise, an error is issued.
   //---------------------------------------------------------------------------

   virtual Coordinator * asCoordinator ();
   virtual Problem *     asProblem     ();
   virtual AttBldr *     asAttBldr     ();
   virtual Att *         asAtt         ();
   virtual MessageMgr *  asMessageMgr  ();
   virtual Component *   asComponent   ();
   virtual Part *        asPart        ();
   virtual Demand *      asDemand      ();
   virtual Operation *   asOperation   ();
   virtual BomEntry *    asBomEntry    ();
   virtual Substitute *  asSubstitute  ();
   virtual BopEntry *    asBopEntry    ();
   virtual CaseToken *   asCaseToken   ();

   //---------------------------------------------------------------------------
   // Pass thru to myCoordinator_:
   //---------------------------------------------------------------------------

   WitRun *      getWitRun         ();
   CTVecSupply * getCTVecSupply    ();
   bool          extractInProgress ();

   //---------------------------------------------------------------------------
   // Accessors.
   //---------------------------------------------------------------------------

   inline Coordinator * getCoordinator ()
      {
      return             myCoordinator_;
      }

   //---------------------------------------------------------------------------
   // Protected member functions.
   //---------------------------------------------------------------------------

   protected:

   explicit JavaAccObj (Coordinator *);
      //
      // Constructor.

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   JavaAccObj              (const JavaAccObj &);
   JavaAccObj & operator = (const JavaAccObj &);
      //
      // Not implemented; prevents accidental copying and assignment.

   //---------------------------------------------------------------------------
   // Private member data.
   //---------------------------------------------------------------------------

   Coordinator * const myCoordinator_;
      //
      // The Coordinator for this JavaAccObj.

   static const int validMark_;
      //
      // The value of the validityMarker_ data member of any valid JavaAccObj.

   int validityMarker_;
      //
      // This AccObject is currently valid (i.e., not destroyed), iff
      // validityMarker_ == validMark_.
   };
};
//
// End of namespace WitJ.

#endif
