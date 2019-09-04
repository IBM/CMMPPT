//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef MaterialH
#define MaterialH

//------------------------------------------------------------------------------
// Header file: "Material.h"
//
// Contains the declaration of class Material.
//------------------------------------------------------------------------------

#include <Part.h>

//------------------------------------------------------------------------------
// Class Material.
//
// Class Hierarchy:
//
// ProbAssoc
//    Component
//       DelComp
//          Node
//             Part
//                Material
//
// Implemented in MatCap.C.
//------------------------------------------------------------------------------

class WitMaterial: public WitPart
   {
   public:

      //------------------------------------------------------------------------
      // Public member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitMaterial (const char * theName, WitCompMgr * theCompMgr);
         //
         // Part <<< theName.
         // Part <<< theCompMgr.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitMaterial ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitBoundSet *,             stockBounds)
      accessFunc (const WitFlexVec <int> &,  buildAheadUB)
      accessFunc (bool,                      baSelSplit)
      accessFunc (bool,                      asapPipOrder)
      accessFunc (const WitFlexVec <bool> &, mandEC)
      accessFunc (const WitDblFlexVec &,     stockVol)

      inline const WitDblFlexVec & stockCost () const
         {
         return stockCost_.curDblFlexVec ();
         }

      inline bool buildNstn ()
         {
         return (myBaDir_ == nstn);
         }

      inline bool buildAsap ()
         {
         return (myBaDir_ == asap);
         }

      //------------------------------------------------------------------------
      // Mapping functions
      //------------------------------------------------------------------------

      decMappingIndex (WitMaterial)

      static int nInstances (WitProblem *);

      //------------------------------------------------------------------------
      // Default value functions.
      //------------------------------------------------------------------------

      defaultValue (double defStockCost,    0.0)
      defaultValue (bool   defBaSelSplit,   false)
      defaultValue (bool   defAsapPipOrder, false)

      static int defBuildAheadUB (WitProblem * theProblem);

      //------------------------------------------------------------------------
      // "Set" functions.
      //------------------------------------------------------------------------

      void setStockCost    (const WitIVRealArg &);
      void setBuildNstn    (bool);
      void setBuildAsap    (bool);
      void setBuildAheadUB (const int *);
      void setBaSelSplit   (bool);
      void setAsapPipOrder (bool);

      //------------------------------------------------------------------------
      // store<Activity><Attr> functions.
      // Each of these functions sets the value of indicated attribute to the
      // value of the argument.
      // Each function is only valid when the indicated activity is being
      // performed.
      //------------------------------------------------------------------------

      void storePreMandEC (WitPeriod, bool);
         //
         // Valid only when preprocessing is being performed.

      void storePostStockVol (const WitTVec <double> &);
         //
         // Valid only when postprocessing is being performed.

      void storeOptStockVol     (WitPeriod, double);
         //
         // Valid only when the optimizing implosion subsystem is uploading the
         // solution.

      void storeStochStockCost (const WitDblFlexVec &);
      void storeStochStockVol  (const WitDblFlexVec &);
         //
         // Valid only when the stochastic implosion subsystem is uploading
         // data.

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      virtual void          writeMultiObjVecs (WitObjective *);
      virtual void          copySelfInto      (WitCompMgr * theCompMgr);
      virtual void          recount           ();
      virtual void          unpreprocess      ();
      virtual void          clearOptSoln      ();
      virtual void          clearStochSoln    ();
      virtual WitMaterial * thisMat           ();
         //
         // Overrides from class Component.

      virtual WitMsgFrag categoryFrag ();
      virtual WitAttr    apiCategory  ();
      virtual bool       canStock     (WitPeriod t);
      virtual void       display      ();
         //
         // Overrides from class Part.

      void copyAttrsFrom (WitMaterial * theMat);
         //
         // Copies the input attributes of theMat into this Material.

      bool nstnBaNeeded ();
         //
         // Returns true, iff NSTN build-ahead is appropriate for this Material.

      void rollBackScrap (
            WitTVec <double> & theStockVol,
            WitTVec <double> & theScrapVol);
         //
         // Moves scrapping of this Material backward in time so that any volume
         // of this Material that is scrapped is scrapped as early as possible.
         // Operates on theStockVol and theScrapVol instead of the actual
         // stockVol and scrapVol for the material, to allow the roll back
         // to be temporary, if need be.

   private:

      //------------------------------------------------------------------------
      // Private enum BaDir.
      //
      // Distinguishes between the two directions of build-ahead by Material:
      // nstn and asap and the absence of build-ahead: noBa.
      //------------------------------------------------------------------------

      enum WitBaDir
         {
         nstn,
         asap,
         noBa
         };

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitMaterial);

      virtual void writeDataAttrs ();
         //
         // Override from class Component.

      virtual WitBoundSet * myBoundSet ();
         //
         // Override from class Component.

      virtual const char * categoryKeyWord ();
         //
         // Override from class Part.

      virtual WitPeriod findExplosionPeriod (WitPeriod fillPer);
         //
         // Override from class Part.

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      int mappingIndex_;
         //
         // Mapping index: see Mapping.h.
         // == # Materials constructed before this Material.

      //------------------------------------------------------------------------
      // Input data.
      //------------------------------------------------------------------------

      WitObjVec stockCost_;
         //
         // Stores the cost coefficients of the stock variables for this
         // Material.
         // Range: (-inf, +inf).

      WitBoundSet * stockBounds_;
         //
         // Bounds on stock volumes.

      WitBaDir myBaDir_;
         //
         // Indicates the heuristic build-ahead style that has been requested 
         // for this Material.
         // This is a hidden input data attribute, which implements the
         // following officially documented input data attributes:
         //
         //    buildNstn
         //    buildAsap

      WitFlexVec <int> buildAheadUB_;
         //
         // Used by heuristic implosion/allocation.
         // Assuming one of the build-ahead techniques for Materials is being
         // used for this Material, buildAheadUB_[thePer] is the maximum 
         // number of periods that this Material can be built ahead of thePer.
         // Range: [0, nPeriods - 1].
         // Note that nPeriods - 1 implies no restriction.

      bool baSelSplit_;
         //
         // true, iff selection splitting for build-ahead is to performed at 
         // this Material.

      bool asapPipOrder_;
         //
         // If true,  PIP is to use ASAP pegging order on this Material;
         // if false, PIP is to use NSTN pegging order on this Material.

      //------------------------------------------------------------------------
      // Data set by general preprocessing
      //------------------------------------------------------------------------

      WitFlexVec <bool> mandEC_;
         //
         // mandEC_[thePer] is true iff this Material cannot be stocked in
         // thePer due to a mandatory engineering change.

      //------------------------------------------------------------------------
      // Data set by postprocessing.
      //------------------------------------------------------------------------

      WitDblFlexVec stockVol_;
         //
         // stockVol_[thePer] is the stock (inventory) level of this Material at
         // the end of thePer.
   };

#endif
