//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef PartH
#define PartH

//------------------------------------------------------------------------------
// Header file: "Part.h"
//
// Contains the declaration of the following classes:
//
//    Part
//------------------------------------------------------------------------------

#include <Node.h>
#include <FlexVec.h>
#include <ObjVec.h>
#include <PtrTVec.h>
#include <PtrVec.h>

//------------------------------------------------------------------------------
// Class Part.
//
// Class Hierarchy:
//
// ProbAssoc
//    Component
//       DelComp
//          Node
//             Part
//                Material
//                Capacity
//------------------------------------------------------------------------------

class WitPart: public WitNode
   {
   public:

      //------------------------------------------------------------------------
      // Public member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitPart ();

      //------------------------------------------------------------------------
      // Mapping functions
      //------------------------------------------------------------------------

      decMappingIndex (WitPart)

      static int nInstances (WitProblem *);

      //------------------------------------------------------------------------
      // Default value functions.
      //------------------------------------------------------------------------

      defaultValue (double defSupplyVol,        0.0)
      defaultValue (bool   defSingleSource,     false)
      defaultValue (bool   defPropRtg,          false)
      defaultValue (bool   defBoundedLeadTimes, false)
      defaultValue (bool   defScrapAllowed,     true)
      defaultValue (double defScrapCost,        0.0)

      //------------------------------------------------------------------------
      // "Set" functions.
      //------------------------------------------------------------------------

      void setPartName         (const char *);
      void setSupplyVol        (const WitIVRealArg &);
      void setSingleSource     (bool);
      void setPropRtg          (const WitBoolean *);
      void setBoundedLeadTimes (bool);
      void setScrapAllowed     (bool);
      void setScrapCost        (const WitIVRealArg &);

      //------------------------------------------------------------------------
      // store<Activity><Attr> functions.
      // Each of these functions sets the value of indicated attribute to the
      // value of the argument.
      // Each function is only valid when the indicated activity is being
      // performed.
      //------------------------------------------------------------------------

      void storePreSortedPartIndex     (int);
      void storePreBelowList           (const WitPtrVec <WitPart> &);
      void storePreExpPerGaps          (bool);
      void storePreProducingBopEntries (const WitList <WitBopEntry> &);
      void storePreExpBopEntry         (const WitPtrTVec <WitBopEntry> &);
         //
         // Valid only when preprocessing is being performed.

      void storePostConsVol     (const WitTVec <double> &);
      void storePostProdVol     (const WitTVec <double> &);
      void storePostScrapVol    (const WitTVec <double> &);
      void storePostResidualVol (const WitTVec <double> &);
         //
         // Valid only when postprocessing is being performed.

      void storeOptScrapVol    (WitPeriod, double);
      void storeOptShadowPrice (WitPeriod, double);
         //
         // Valid only when the optimizing implosion subsystem is uploading the
         // solution.

      void storeStochSupplyVol (const WitDblFlexVec &);
      void storeStochScrapCost (const WitDblFlexVec &);
      void storeStochScrapVol  (const WitDblFlexVec &);
         //
         // Valid only when the stochastic implosion subsystem is uploading
         // data.

      void storeMrpReqVol         (const WitTVec <double> &);
      void storeMrpMrpConsVol     (const WitTVec <double> &);
      void storeMrpMrpResidualVol (const WitTVec <double> &);
         //
         // Valid only when the MRP subsystem is uploading the MRP solution.

      void storeFssFocShortageVol (const WitTVec <double> &);
         //
         // Valid only when the FSS subsystem is uploading the FSS solution.

      //------------------------------------------------------------------------
      // Insert functions.
      // Each of these functions does an appendUniquely on the argument for the
      // appropriate List.
      //------------------------------------------------------------------------

      void insert (WitBomEntry *);
      void insert (WitSubEntry *);
      void insert (WitBopEntry *);
      void insert (WitDemand *);

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      virtual bool      inserted          ();
      virtual void      writeMultiObjVecs (WitObjective *);
      virtual void      unpreprocess      ();
      virtual void      clearOptSoln      ();
      virtual void      clearStochSoln    ();
      virtual WitPart * thisPart          ();
         //
         // Overrides from class Component.

      virtual void       identifyInstance ();
      virtual void       writeSetClause   ();
      virtual WitMsgFrag objTypeMsgFrag   ();
      virtual WitAttr    apiObjItrState   ();
         //
         // Overrides from class DelComp.

      virtual WitMsgFrag categoryFrag () = 0;
         //
         // Returns a message fragment indicating category of this Part.

      virtual WitAttr apiCategory () = 0;
         //
         // Returns the category of this Part in api terms.

      virtual bool canStock (WitPeriod thePer) = 0;
         //
         // Returns true iff part this Part can be stocked in period thePer.

      virtual int nodeIndex () const;
         //
         // Override from class Node.

      virtual void display ();
         //
         // Displays the input and preprocessing data for this Part.

      inline static WitPart * key (WitListItr & theItr)
         {
         return reinterpret_cast <WitPart *> (theItr.key ());
         }
         //
         // Used by forEach<object> macros.

      inline WitPeriod explosionPeriod (WitPeriod fillPer)
         {
         return 
            expPerGaps_?
               findExplosionPeriod (fillPer):
               fillPer;
         }
         //
         // Returns the Period in which to explode this Part for filling
         // requirements in fillPer. 
         
      bool explodeable (WitPeriod thePer);
         //
         // Returns true iff this Part can be exploded in period thePer.
         // Note that this implies that the relevent effProdRate is 
         // significantly positive.

      double net (
            WitPeriod          reqPer,
            double             reqQty,
            WitTVec <double> & supply);
         //
         // Net the supply vector by reqQty and return 0 if reqQty is filled
         // or the amount remaining if the supply is exhausted.

      void writeSoln ();
         //
         // Prints the comprehensive implosion solution for this Part.

      void writeDualSoln ();
         //
         // Prints the dual optimization solution for this Part.

      void writeMrpSoln ();
         //
         // Prints the comprehensive MRP solution for this Part.

      void resetShadowPrice ();
         //
         // Sets shadowPrice_ to 0.0.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (const WitList <WitDemand> &,   myDemands)
      accessFunc (const WitList <WitBomEntry> &, consumingBomEntries)
      accessFunc (const WitList <WitSubEntry> &, consumingSubEntries)
      accessFunc (const WitList <WitBopEntry> &, producingBopEntries)
      accessFunc (const WitDblFlexVec &,         supplyVol)
      accessFunc (bool,                          singleSource)
      accessFunc (const WitFlexVec <bool> &,     propRtg)
      accessFunc (bool,                          boundedLeadTimes)
      accessFunc (bool,                          scrapAllowed)
      accessFunc (int,                           sortedPartIndex)
      accessFunc (const WitPtrVec <WitPart> &,   belowList)
      accessFunc (const WitDblFlexVec &,         shadowPrice)
      accessFunc (const WitDblFlexVec &,         consVol)
      accessFunc (const WitDblFlexVec &,         prodVol)
      accessFunc (const WitDblFlexVec &,         scrapVol)
      accessFunc (const WitDblFlexVec &,         residualVol)
      accessFunc (const WitDblFlexVec &,         mrpConsVol)
      accessFunc (const WitDblFlexVec &,         mrpResidualVol)
      accessFunc (const WitDblFlexVec &,         reqVol)
      accessFunc (const WitDblFlexVec &,         focShortageVol)

      inline const WitDblFlexVec & scrapCost () const
         {
         return scrapCost_.curDblFlexVec ();
         }

      WitBopEntry * expBopEntry (WitPeriod thePer);

      inline const WitString & partName ()
         {
         return nodeName ();
         }

   protected:

      //------------------------------------------------------------------------
      // Protected member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitPart (const char * theName, WitCompMgr * theCompMgr);
         //
         // Node <<< theName.
         // Node <<< theCompMgr.

      //------------------------------------------------------------------------
      // Other protected member functions.
      //------------------------------------------------------------------------

      void recountPart ();
         //
         // Recounts this Part.

      virtual const char * categoryKeyWord () = 0;
         //
         // Returns the key word for this Part's category for the input file
         // format.

      virtual void writeDataAttrs ();
         //
         // Override from class Component.

      void copyAttrsFrom (WitPart * thePart);
         //
         // Copies the input attributes of thePart into this Part.

      virtual WitPeriod findExplosionPeriod (WitPeriod fillPer) = 0;
         //
         // Computes and returns the Period in which to explode this Part for
         // filling requirements in fillPer. 
         
   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitPart);

      virtual void writeDataID        ();
      virtual void purgeCompListsSpec ();
      virtual void clearDepListsSpec  ();
         //
         // Overrides from class Component.

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Data maintained by myCompMgr ().
      //------------------------------------------------------------------------

      int mappingIndex_;
         //
         // Mapping index: see Mapping.h.
         // == # Parts constructed before this Part.

      WitList <WitDemand> myDemands_;
         //
         // The list of demands for this Part.

      WitList <WitBomEntry> consumingBomEntries_;
         //
         // A list of all BomEntries, theBomEnt, such that
         // theBomEnt->myPart_ == this Part.

      WitList <WitSubEntry> consumingSubEntries_;
         //
         // A list of all SubEntries,
         // theSub, such that theSub->myPart_ == this Part.

      WitList <WitBopEntry> producingBopEntries_;
         //
         // A list of all BopEntries, theBopEnt, such that
         // theBopEnt->myPart_ == this Part.
         // When myProblem () is preprocessed, this list is sorted in order of
         // of increasing expAversion.

      //------------------------------------------------------------------------
      // Input data.
      //------------------------------------------------------------------------

      WitDblFlexVec supplyVol_;
         //
         // Non-cumulative external supply volume in each period.
         // Range: [0.0, +inf).

      bool singleSource_;
         //
         // The single-source technique is to be applied to this Part, iff
         // singleSource_ is true.

      WitFlexVec <bool> propRtg_;
         //
         // The proportionate routing technique is to be applied to this Part
         // in thePer, iff propRtg_[thePer] is true.

      bool boundedLeadTimes_;
         //
         // true, iff lead time bounds apply to this Part.

      bool scrapAllowed_;
         //
         // Applies to optimizing implosion only.
         // If false, scrapVol is constrained to be zero in a feasible
         // optimizing implosion solution.

      WitObjVec scrapCost_;
         //
         // Stores the cost coefficients of the scrap variables for this Part.
         // Range: (-inf, +inf).

      //------------------------------------------------------------------------
      // Data set by preprocessing.
      //------------------------------------------------------------------------

      int sortedPartIndex_;
         //
         // # Parts before this Part in the sorted list of Parts.

      WitPtrTVec <WitBopEntry> expBopEntry_;
         //
         // expBopEntry_[thePer] is the preferred BopEntry to be used
         // for explosions in order to produce this Part in period thePer.
         // expBopEntry_[thePer] != NULL iff this Part can be exploded in 
         // thePer.

      WitPtrVec <WitPart> belowList_;
         //
         // The list of all parts in the multilevel BOM extending downward from
         // this Part, including this Part itself. The list is in
         // downward order, so this Part is always its first element.
         // Considers all BopEntries that produce this Part for which expAllowed
         // is true. The purpose of this data is for its use in heuristic
         // implosion/allocation.

      bool expPerGaps_;
         //
         // true, iff there is at least one period, fillPer, such that:
         // * this Part is not explodeable in fillPer, and
         // * there is an earlier period, expPer in which this Part is 
         //   explodeable and the result can be stocked thru fillPer.
         // explosionPeriod () will search for an earlier explosion period iff
         // expPerGaps_ is true.

      //------------------------------------------------------------------------
      // Data set by optimizing implosion.
      //------------------------------------------------------------------------

      WitDblFlexVec shadowPrice_;
         //
         // shadowPrice_[thePer] is the shadow price (dual variable value) for
         // the resource allocation constraint in this Part in thePer.
         // Valid only if myOptComp ()->compPrices () is true.

      //------------------------------------------------------------------------
      // Data set by postprocessing.
      //------------------------------------------------------------------------

      WitDblFlexVec consVol_;
         //
         // consVol[thePer] is the volume of this Part that was consumed in
         // thePer.

      WitDblFlexVec prodVol_;
         //
         // prodVol[thePer] is total volume of this Part that is produced in
         // thePer by all Operations.

      WitDblFlexVec scrapVol_;
         //
         // scrapVol_[thePer] is the volume of this part that was scrapped in
         // thePer.

      WitDblFlexVec residualVol_;
         //
         // residualVol_[thePer] is the amount by which consumption of this Part
         // could be increased in thePer, without making the current implosion
         // solution infeasible.

      //------------------------------------------------------------------------
      // Data set by MRP.
      //------------------------------------------------------------------------

      WitDblFlexVec reqVol_;
         //
         // reqVol[thePer] is the required volume of the part in thePer.
         // This field is the main output of mrp.

      WitDblFlexVec mrpConsVol_;
         //
         // Same as consVol, but for MRP.

      WitDblFlexVec mrpResidualVol_;
         //
         // Same as residualVol, but for MRP.

      //------------------------------------------------------------------------
      // Data set by FSS.
      //------------------------------------------------------------------------

      WitDblFlexVec focShortageVol_;
         //
         // The Focussed Shortage Volume for this Part.
   };

#endif
