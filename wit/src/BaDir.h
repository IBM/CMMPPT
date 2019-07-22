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

#ifndef BaDirH
#define BaDirH

//------------------------------------------------------------------------------
// Header file: "BaDir.h"
//
// Contains the declaration of the following classes:
//
//    BaDir.
//    NstnDir.
//    AsapDir.
//------------------------------------------------------------------------------

#include <Assoc.h>

//------------------------------------------------------------------------------
// Class BaDir
//
// "Build-Ahead Direction"
// Responsible for the behavior that distinguishes between NSTN Build-Ahead and
// ASAP Build-Ahead.
//
// BaDir
//    NstnBaDir
//    AsapBaDir
//
// Implemented in BuildAhd.C.
//------------------------------------------------------------------------------

class WitBaDir: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitBaDir ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      virtual void findInitExpPers (
            WitMaterial *         theMat,
            WitTVec <WitPeriod> & expPerVec) = 0;
         //
         // Base class function.
         // On return, expPerVec[fillPer] is the initial explosion period to be
         // used when filling requirements for theMat in fillPer.

      virtual WitPeriod findInitExpPer (
            WitMaterial * theMat,
            WitPeriod     fillPer) = 0;
         //
         // Base class function.
         // Returns the initial explosion period to be used when filling
         // requirements for theMat in fillPer.

      virtual void advanceExpPer (
            WitMaterial * theMat,
            WitPeriod     fillPer, 
            WitPeriod &   expPer) = 0;
         //
         // Base class function.
         // Moves expPer one period in the appropriate direction, as appropriate
         // for theMat. If the resulting expPer is not a valid explosion period
         // for fillPer, it is set to -1.

   protected:

      //------------------------------------------------------------------------
      // Protected member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitBaDir (WitBaMgr *);

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitBaMgr *, myBaMgr)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitBaDir);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitBaMgr * const myBaMgr_;
         //
         // The BaMgr that owns this BaDir.

   };

//------------------------------------------------------------------------------
// Class NstnBaDir
//
// "No-Sooner-Than-Necessary Build-Ahead Direction"
// Responsible for the behavior of NSTN Build-Ahead that distinguishes it from
// ASAP Build-Ahead.
//
// BaDir
//    NstnBaDir
//
// Implemented in BuildAhd.C.
//------------------------------------------------------------------------------

class WitNstnBaDir: public WitBaDir
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitNstnBaDir (WitBaMgr *);

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitNstnBaDir ();

      //------------------------------------------------------------------------
      // Other private member functions.
      //------------------------------------------------------------------------

      virtual void      findInitExpPers (WitMaterial *, WitTVec <WitPeriod> &);
      virtual WitPeriod findInitExpPer  (WitMaterial *, WitPeriod);
      virtual void      advanceExpPer   (WitMaterial *, WitPeriod, WitPeriod &);
         //
         // Overrides from class BaDir.

      noCopyCtorAssign (WitNstnBaDir);
   };

//------------------------------------------------------------------------------
// Class AsapBaDir
//
// "As-Soon-As-Possible Build-Ahead Direction"
// Responsible for the behavior of ASAP Build-Ahead that distinguishes it from
// NSTN Build-Ahead.
//
// BaDir
//    AsapBaDir
//
// Implemented in BuildAhd.C.
//------------------------------------------------------------------------------

class WitAsapBaDir: public WitBaDir
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitAsapBaDir (WitBaMgr *);

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitAsapBaDir ();

      //------------------------------------------------------------------------
      // Other private member functions.
      //------------------------------------------------------------------------

      virtual void      findInitExpPers (WitMaterial *, WitTVec <WitPeriod> &);
      virtual WitPeriod findInitExpPer  (WitMaterial *, WitPeriod);
      virtual void      advanceExpPer   (WitMaterial *, WitPeriod, WitPeriod &);
         //
         // Overrides from class BaDir.

      noCopyCtorAssign (WitAsapBaDir);
   };

#endif
