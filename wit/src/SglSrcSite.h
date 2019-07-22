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

#ifndef SglSrcSiteH
#define SglSrcSiteH

//------------------------------------------------------------------------------
// Header file: "SglSrcSite.h"
//
// Contains the declaration of class SglSrcSite.
//------------------------------------------------------------------------------

#include <ObjStack.h>
#include <Assoc.h>

//------------------------------------------------------------------------------
// Class SglSrcSite
//
// "Single-Source Site"
//
// Represents an RtSite for which the single-source technique has been
// requested.
//------------------------------------------------------------------------------

class WitSglSrcSite: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitSglSrcSite (WitRtSite *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitSglSrcSite ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      bool isPreferredTo (WitSglSrcSite * othSite);
         //
         // Returns true, iff this SglSrcSite is preferred to othSite for
         // sgl-src consideration.

      bool nextCandIsSglSrc ();
         //
         // Returns true, iff incumbUniqueRtCand_ exists and the next RtCand
         // for sgl-src matches it.

      void selectNext ();
         //
         // Selects the next RtCand for sgl-src.

      void applySel ();
         //
         // Applies the current sgl-src selection, if any, to the multi-route
         // subsystem.

      void resumeMS ();
         //
         // Puts this SglSrcSite back into multi-source mode.

      void initCmqFixed ();
         //
         // Takes actions appropriate at the beginning for SglSrcMgr::cmqFixed.

      void recordVisit (WitRtCand * theRtCand, int thePcIdx);
         //
         // Records the fact that this SglSrcSite is being visited, using
         // theRtCand as its selection, with thePcIdx as the current 
         // perm-commit index. If theRtCand == NULL, this indicates that
         // a candidate ineligible for sgl-src was used as the selection.

      void resetIncumb ();
         //
         // Resets data data associated with the incumbent cmqFixed.

      void saveCurAsIncumb ();
         //
         // Saves data associated with the current cmqFixed as data associated
         // with the incumbent cmqFixed.

      bool expProhibited (WitPeriod thePer);
         //
         // Returns true, iff explosion thru this SglSrcSite is prohibited in
         // thePer.

      void reset ();
         //
         // Puts this SglSrcSite into its initial state.

      void prtSel (const char * text);
         //
         // Does printing for some aspect of attempting a selection at this
         // SglSrcSite, as indicated by text.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      inline bool sglSrcMode ()
         {
         return (selRtCand_ != NULL);
         }
         //
         // true  --> this SglSrcSite is sgl-src   mode.
         // false --> this SglSrcSite is multi-src mode.

      inline bool hasUntried ()
         {
         return (nextRtCand_ != NULL);
         }

      accessFunc (bool, altered)
      accessFunc (int,  curPcIdx)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitSglSrcSite);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitRtSite * const myRtSite_;
         //
         // The RtSite represented by this SglSrcSite.

      WitSelector * const mySelector_;
         //
         // The Selector associated with this SglSrcSite.

      WitObjStack <WitRtCand> myRtCands_;
         //
         // The ordered list of RtCands considered by this SglSrcSite.

      WitRtCand * selRtCand_;
         //
         // The RtCand currently selected for sgl-src routing from this
         // SglSrcSite, if any; otherwise NULL.

      WitRtCand * nextRtCand_;
         //
         // The next RtCand to consider for sgl-src routing from this
         // SglSrcSite, if any; otherwise NULL.

      WitObjStItr <WitRtCand> nextRtCandItr_;
         //
         // The Itr corresponding to nextRtCand_.

      bool altered_;
         //
         // true, iff the selection for this SglSrcSite has been altered since
         // the most recent reset.

      int curPcIdx_;
         //
         // The perm-commit index of the last time this SglSrcSite was visited
         // during the current cmqFixed, or -1, if it was not visited during
         // the current cmqFixed.

      int incumbPcIdx_;
         //
         // Same as curPcIdx_, but for the incumbent cmqFixed.

      bool curVisited_;
         //
         // true, iff this SglSrcSite has been visited during the current
         // cmqFixed in perm or temp mode,

      WitRtCand * curUniqueRtCand_;
         //
         // If this SglSrcSite was visited during the current cmqFixed with flow
         // to only one RtCand in perm or temp mode, curUniqueRtCand_ is that
         // RtCand; otherwise NULL.

      WitRtCand * incumbUniqueRtCand_;
         //
         // Same as curUniqueRtCand_, but for the incumbent cmqFixed.
   };

#endif
