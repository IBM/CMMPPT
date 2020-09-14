//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef ObjVecIRH
#define ObjVecIRH

//------------------------------------------------------------------------------
// Header file: "ObjVecIR.h"
//
// Contains the declaration of class ObjVecIR.
//------------------------------------------------------------------------------

#include <Util.h>

//------------------------------------------------------------------------------
// Class ObjVecIR
//
// "Objective Vector Internal Representation"
//
// Internal Representation of a vector of doubles that holds the value of an
// objective function attribute.
//
// Class hierarchy:
//
//    ObjVecIR
//      SglObjVecIR
//      MultiObjVecIR
//
// Implemented in ObjVec.C.
//------------------------------------------------------------------------------

class WitObjVecIR
   {
   public:

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitObjVecIR ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      virtual void copyInto (WitObjVecIR * theObjVecIR) = 0;
         //
         // Copies the contents of this ObjVecIR into theObjVecIR, if this
         // ObjVecIR is of the same type as theObjVecIR;
         // otherwise issues a fatal error.

      virtual void copyFrom (WitSglObjVecIR *);
      virtual void copyFrom (WitMultiObjVecIR *);
         //
         // Copies the contents of the argument into this ObjVecIR, if this
         // ObjVecIR is of the same type as the argument;
         // otherwise issues a fatal error.

      virtual void writeDataMulti (
            const char *   attrName,
            WitDelComp *   theDelComp,
            WitObjective * theObj) = 0;
         //
         // Writes this ObjVecIR to the input data output file, as appropriate
         // for multiple objectives mode:
         //    In single objective mode, a fatal error is issued.
         //    In multiple objectives mode, the DblFlexVec for theObj is written
         //       including the set clause for theDelComp.
         //       The writing is only done if the vector is not 0.

      virtual void display (WitMsgID theMsgID, WitProblem *) = 0;
         //
         // Displays this ObjVecIR, issuing the indicated Msg.

      virtual WitDblFlexVec & curDblFlexVec () = 0;
         //
         // Returns the current vector of doubles represented by this ObjVecIR.

   protected:

      //------------------------------------------------------------------------
      // Protected constructor functions.
      //------------------------------------------------------------------------

      WitObjVecIR ();

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitObjVecIR);
   };

#endif
