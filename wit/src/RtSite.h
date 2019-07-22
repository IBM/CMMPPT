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

#ifndef RtSiteH
#define RtSiteH

//------------------------------------------------------------------------------
// Header file: "RtSite.h"
//
// Contains the declaration of class RtSite.
//------------------------------------------------------------------------------

#include <Assoc.h>
#include <List.h>

//------------------------------------------------------------------------------
// Class RtSite
//
// "Routing Site"
//
// Represents either a Part or a BomEntry in its role as a "site" at which a
// routing decision must be made. 
//
// In comments, the Part or BomEntry represented by this RtSite will be referred
// to as "the represented site".
//
// Class Hierarchy:
//
// ProbAssoc
//    RtSite
//       ProdRtSite
//       ConsRtSite
//
// Implemented in Routing.C.
//------------------------------------------------------------------------------

class WitRtSite: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitRtSite ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void findRtCands ();
         //
         // Builds myRtCands_.

      void clearRtCands ();
         //
         // Clears myRtCands_.

      void printPer (WitPeriod thePer, int labelCol, int dataCol);
         //
         // Prints thePer.
         // The label will be printed in column labelCol.
         // If dataCol > 0, the data will be printed in that column, if
         // possible.

      virtual bool isForProd () = 0;
         //
         // Returns true,  if this RtSite is for production,  i.e. it represents
         //    a Part.
         // Returns false, if this RtSite is for consumption, i.e. it represents
         //    a BomEntry.

      virtual void printID (int labelCol, int dataCol) = 0;
         //
         // Prints info identifying the represented site.
         // The label will be printed in column labelCol.
         // If dataCol > 0, the data will be printed in that column, if
         // possible.

      virtual bool explodeable (WitPeriod thePer) = 0;
         //
         // Returns true, iff the represented site is eligible for explosion in
         // thePer.

      virtual const WitFlexVec <bool> & propRtg () = 0;
         //
         // Returns propRtg () for the represented site.

      virtual bool singleSource () = 0;
         //
         // Returns the value of the singleSource attribute for the represented
         // site.

      virtual WitPart * mrSortPart () = 0;
         //
         // Returns the Part associated an MrSite associated with this RtSite, 
         // for SSPE sorting.

      virtual WitRtCand * mrDefaultRtCand (WitPeriod thePer) = 0;
         //
         // Returns the RtCand corresponding to the default MrCand to be used by
         // the MrSite corresponding to this RtCand in thePer, if any,
         // otherwise NULL.

      virtual int heightIdx () = 0;
         //
         // Returns an index indicating the height of the represented site in
         // the BOM structure.

      //------------------------------------------------------------------------
      // Mapping functions
      //------------------------------------------------------------------------

      decMappingIndex (WitRtSite)

      static int nInstances (WitProblem *);

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (const WitList <WitRtCand> &, myRtCands)
      accessFunc (WitRtMgr *,                  myRtMgr)

   protected:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitRtSite (WitRtMgr *);

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      virtual void findRtCands (WitList <WitRtCand> & theRtCands) = 0;
         //
         // Sets theRtCands to be the set of RtCands for this RtSite.

      virtual const char * perCaseID () = 0;
         //
         // Returns a string indicating which kind of period is associated with
         // this RtSite.

      noCopyCtorAssign (WitRtSite);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      const int mappingIndex_;
         //
         // Mapping index.

      WitList <WitRtCand> myRtCands_;
         //
         // The RtCands for this RtSite.

      WitRtMgr * const myRtMgr_;
         //
         // The RtMgr that owns this RtSite.
   };

#endif
