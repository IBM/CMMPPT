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

#ifndef ObjVecH
#define ObjVecH

//------------------------------------------------------------------------------
// Header file: "ObjVec.h"
//
// Contains the declaration of class ObjVec.
//------------------------------------------------------------------------------

#include <ObjVecIR.h>

//------------------------------------------------------------------------------
// Class ObjVec
//
// "Objective Vector"
//
// Represents a vector of doubles that holds the value of an objective function
// attribute.
//------------------------------------------------------------------------------

class WitObjVec
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitObjVec (WitProblem *, double initValue);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitObjVec ();

      //------------------------------------------------------------------------
      // Wrapper methods for myObjVecIR_.
      //------------------------------------------------------------------------

      void operator = (const WitObjVec & theObjVec);

      void writeDataMulti (
         const char *   attrName,
         WitDelComp *   theDelComp,
         WitObjective * theObj)
         const;

      void display (WitMsgID, WitProblem *) const;

      inline const WitDblFlexVec & curDblFlexVec () const
         {
         return myObjVecIR_->curDblFlexVec ();
         }

      inline WitDblFlexVec & curDblFlexVec ()
         {
         return myObjVecIR_->curDblFlexVec ();
         }

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      static WitObjVecIR * newObjVecIR (
            WitProblem * theProblem,
            double       initValue);
         //
         // Creates and returns a new ObjVecIR of the appropriate derived class
         // for theProblem.

      noCopyCtor (WitObjVec);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitObjVecIR * const myObjVecIR_;
         //
         // The ObjVecIR owned by this ObjVec that constitutes its internal
         // representation.
   };

#endif
