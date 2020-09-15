//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef SglObjVecIRH
#define SglObjVecIRH

//------------------------------------------------------------------------------
// Header file: "SglObjVecIR.h"
//
// Contains the declaration of class SglObjVecIR.
//------------------------------------------------------------------------------

#include <ObjVecIR.h>
#include <FlexVec.h>

//------------------------------------------------------------------------------
// Class SglObjVecIR
//
// "Single Objective Vector Internal Representation"
//
// Internal Representation of a vector of doubles that holds the value of an
// objective function attribute in single objective mode.
//
// Class hierarchy:
//
//    ObjVecIR
//      SglObjVecIR
//
// Implemented in ObjVec.C.
//------------------------------------------------------------------------------

class WitSglObjVecIR: public WitObjVecIR
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitSglObjVecIR (WitProblem *, double initValue);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitSglObjVecIR ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      virtual void            copyInto       (WitObjVecIR *);
      virtual void            copyFrom       (WitSglObjVecIR *);
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

      noCopyCtorAssign (WitSglObjVecIR);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitDblFlexVec myDblFlexVec_;
         //
         // The vector of doubles represented by this ObjVecIR.
   };

#endif
