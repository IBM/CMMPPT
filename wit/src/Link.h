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

#ifndef LinkH
#define LinkH

//------------------------------------------------------------------------------
// Header file: "Link.h"
//
// Contains the declaration of the following classes and templates:
//
//    AbsLink
//    ObjLink
//    PairLink <ObjByVal>
//    TripLink
//------------------------------------------------------------------------------

#include <Util.h>

//------------------------------------------------------------------------------
// Class AbsLink
//
// A link in an dynamic stack.
//
// Class Hierarchy:
//
// AbsLink
//    ObjLink
//    PairLink <ObjByVal>
//    TripLink
//
// Implemented in Stack.C
//------------------------------------------------------------------------------

class WitAbsLink
   {
   public:

      //------------------------------------------------------------------------
      // Public member functions.
      //------------------------------------------------------------------------

      void linkTo (WitAbsLink * theAbsLink);
         //
         // Sets this AbsLink to link to theAbsLink, while leaving the object
         // link unaltered.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitAbsLink *, nextAbsLink)

   protected:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitAbsLink ();

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitAbsLink ();

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitAbsLink);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitAbsLink * nextAbsLink_;
         //
         // The next AbsLink after this AbsLink in the Stack.
   };

//------------------------------------------------------------------------------
// Class ObjLink
//
// A link in a dynamic stack of individual objects.
//
// Class Hierarchy:
//
// AbsLink
//    ObjLink
//
// Implemented in Stack.C
//------------------------------------------------------------------------------

class WitObjLink: public WitAbsLink
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitObjLink ();

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitObjLink ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (void *, myObjByVP)

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void setDataTo (void *);
         //
         // Sets the data for this ObjLink to the arguments.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitObjLink);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      void * myObjByVP_;
         //
         // The object associated with this ObjLink.
   };

//------------------------------------------------------------------------------
// Class Template PairLink <ObjByVal>
//
// A link in a dynamic stack of pairs, (Object, ObjByVal).
//
// Class Hierarchy:
//
// AbsLink
//    PairLink <ObjByVal>
//
// Implemented in Stack.C.
//------------------------------------------------------------------------------

template <typename ObjByVal> 
      class WitPairLink: public WitAbsLink
   {                                                                           
   public:                                                                     
                                                                               
      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------
                                                                               
      WitPairLink ();                                                
                                                                               
      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------
                                                                               
      ~WitPairLink ();                                               
                                                                               
      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------
                                                                               
      accessFunc (void *,   myObjByVP)                                         
      accessFunc (ObjByVal, myObjByVal)
                                                                               
      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------
                                                                               
      void setDataTo (void *, ObjByVal);                                    
         //
         // Sets the data for this PairLink to the arguments.
                                                                               
   private:                                                                    
                                                                               
      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitPairLink);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------
                                                                               
      void * myObjByVP_;
         //
         // The object associated with this PairLink.
                                                                               
      ObjByVal myObjByVal_;
         //
         // The value associated with this PairLink.
   };                                                                          

//------------------------------------------------------------------------------
// Class TripLink
//
// A link in a dynamic stack of triples, (Object, Period, Double).
//
// Class Hierarchy:
//
// AbsLink
//    TripLink
//
// Implemented in Stack.C
//------------------------------------------------------------------------------

class WitTripLink: public WitAbsLink
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitTripLink ();

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitTripLink ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void setDataTo (void *, WitPeriod, double);
         //
         // Sets the data for this TripLink to the arguments.

      void getData (void * &, WitPeriod &, double &) const;
         //
         // Sets the args to the data for this TripLink.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitTripLink);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      void * myObjByVP_;
         //
         // The object associated with this TripLink.

      WitPeriod myPer_;
         //
         // The period associated with this TripLink.

      double myDbl_;
         //
         // The double associated with this TripLink.
   };

#endif
