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

#ifndef MultiObjVecIRH
#define MultiObjVecIRH

//------------------------------------------------------------------------------
// Header file: "MultiObjVecIR.h"
//
// Contains the declaration of class MultiObjVecIR.
//------------------------------------------------------------------------------

#include <ObjVecIR.h>
#include <PtrMap.h>

//------------------------------------------------------------------------------
// Class MultiObjVecIR
//
// "Multiple-Objective Vector Internal Representation"
//
// Internal Representation of a vector of doubles that holds the value of an
// objective function attribute in multiple-objectives mode.
//
// Class hierarchy:
//
//    ObjVecIR
//      MultiObjVecIR
//
// Implemented in ObjVec.C.
//------------------------------------------------------------------------------

class WitMultiObjVecIR: public WitObjVecIR
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitMultiObjVecIR (WitMultiObjMgr *, double initValue);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitMultiObjVecIR ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      virtual void            copyInto       (WitObjVecIR *);
      virtual void            copyFrom       (WitMultiObjVecIR *);
      virtual void            writeDataMulti (const char *,
                                              WitDelComp *,
                                              WitObjective *);
      virtual void            display        (WitMsgID,
                                              WitProblem *);
      virtual WitDblFlexVec & curDblFlexVec  ();
         //
         // Overrides from class ObjVecIR.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitMultiObjVecIR);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitMultiObjMgr * const myMultiObjMgr_;
         //
         // The MultiObjMgr for this MultiObjVecIR.

      WitPtrMap <WitObjective, WitDblFlexVec> myDblFlexVecMap_;
         //
         // myDblFlexVecMap_.myElemAt (theObj) is the vector of doubles
         // represented by this ObjVecIR for theObj.
   };

#endif
