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

#ifndef StageMgrH
#define StageMgrH

//------------------------------------------------------------------------------
// Header file: "StageMgr.h"
//
// Contains the declaration of class StageMgr.
//------------------------------------------------------------------------------

#include <StochAssoc.h>
#include <PtrVec.h>

//------------------------------------------------------------------------------
// Class StageMgr
//
// "Object Stage Manager"
// Manages the stage data for stochastic implosion in either mode.
//
// Class hierarchy:
//
//    ProbAssoc
//       StageMgr
//          ObjStageMgr
//          PerStageMgr
//
// Implemented in Stage.C.
//------------------------------------------------------------------------------

class WitStageMgr: public WitStochAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitStageMgr ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      static WitStageMgr * newStageMgr (WitStochModeMgr *);
         //
         // Constructs a new StageMgr and returns of the appropriate derived
         // class.

      virtual void setObjectStageIdx (WitNode * theNode, int theStageIdx) = 0;
         //
         // In stage-by-object mode, sets the objectStage of theNode to the
         //                          Stage whose index is theStageIdx.
         // In stage-by-period mode, issues a fatal error.

      virtual void setPeriodStageIdx (WitPeriod thePer, int theStageIdx) = 0;
         //
         // In stage-by-object mode, issues a fatal error.
         // In stage-by-period mode, sets the periodStage of thePer to the Stage
         //                          whose index is theStageIdx.

      virtual WitStage * objectStage (WitNode * theNode) = 0;
         //
         // In stage-by-object mode, returns the objectStage of theNode.
         // In stage-by-period mode, issues a fatal error.

      virtual WitStage * periodStage (WitPeriod thePer) = 0;
         //
         // In stage-by-object mode, issues a fatal error.
         // In stage-by-period mode, returns the periodStage of thePer.

      virtual WitStage * myStageOf (WitNode * theNode, WitPeriod thePer) = 0;
         //
         // Returns the stage of either theNode or thePer,
         // depending on the stage-by mode.

      virtual bool includesNode (WitStage * theStage, WitNode * theNode) = 0;
         //
         // Stage-by-object mode: returns true, iff theNode belongs to
         //                       theStage.
         // Stage-by-period mode: returns true, unconditionally.

      virtual bool includesPeriod (WitStage * theStage, WitPeriod thePer) = 0;
         //
         // Stage-by-object mode: returns true, unconditionally.
         // Stage-by-period mode: returns true, iff thePer belongs to
         //                       theStage.

      virtual void writeData () = 0;
         //
         // Writes the section of the input data file for the stage data owned
         // by this ObjStageMgr.

      virtual void displayData () = 0;
         //
         // Does displayData for the stage data owned by this ObjStageMgr.

      virtual void copyAttrsFrom (WitStageMgr * origStageMgr) = 0;
         //
         // Copies the input attributes of origStageMgr into this StochModeMgr.

      virtual void validateData () = 0;
         //
         // Verifies that the stage attributes have valid values for a
         // stochastic implosion.

      virtual void issueAttMisMatchHeadMsg () = 0;
         //
         // Issues the heading msg for an attribute mis-match.

      inline WitStage * myStageFor (int theIdx)
         {
         return myStageVec_[theIdx];
         }

   protected:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitStageMgr (WitStochModeMgr *);

      //------------------------------------------------------------------------
      // Default value functions.
      //------------------------------------------------------------------------

      inline WitStage * defStage ()
         {
         return myStageFor (0);
         }

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitStageMgr);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitPtrVec <WitStage> myStageVec_;
         //
         // myStageVec_[theIdx] is the Stage owned by this StageMgr whose index
         // is theIdx.
   };

#endif
