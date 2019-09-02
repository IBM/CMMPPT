//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef MeDirH
#define MeDirH

//------------------------------------------------------------------------------
// Header file: "MeDir.h"
//
// Contains the declaration of the following classes:
//
//    MeDir.
//    NstnMeDir.
//    AsapMeDir.
//------------------------------------------------------------------------------

#include <Assoc.h>

//------------------------------------------------------------------------------
// Class MeDir
//
// "Multi-Exec Direction"
// Responsible for the behavior that distinguishes between NSTN Multi-Exec and
// ASAP Multi-Exec.
//
// Class Hierarchy:
//
// MeDir
//    NstnMeDir
//    AsapMeDir
//
// Implemented in MultiEx.C.
//------------------------------------------------------------------------------

class WitMeDir: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitMeDir ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      virtual WitMeDir * oppMeDir () = 0;
         //
         // Returns the MeDir owned by myMeMgr_ that represents the multi-exec
         // direction opposite from this MeDir.

      virtual WitMePt * myMePt (WitMeSitePt * theMeSitePt) = 0;
         //
         // Returns the MePt owned by theMeSitePt and associated with this
         // MeDir.

      virtual WitPeriod initSelExecPer (WitMeSitePt * theMeSitePt) = 0;
         //
         // Returns the initial execution period to be selected for theMeSitePt
         // in the direction modeled by this MeDir.

      virtual WitPeriod nextExecPer (
            WitPeriod     execPer,
            WitMeSitePt * theMeSitePt) = 0;
         //
         // Returns the next execution period for theMeSitePt after execPer,
         // if any; otherwise -1.

      virtual const char * myID () = 0;
         //
         // Returns a string identifying this MeDir.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitMeMgr *, myMeMgr)

   protected:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitMeDir (WitMeMgr *);

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitMeDir);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitMeMgr * const myMeMgr_;
         //
         // The MeMgr that owns this MrDir.
   };

//------------------------------------------------------------------------------
// Class NstnMeDir
//
// "NSTN Multi-Exec Direction"
// Responsible for the behavior of NSTN Multi-Exec that distinguishes it from
// ASAP Multi-Exec.
//
// Class Hierarchy:
//
// MeDir
//    NstnMeDir
//
// Implemented in MultiEx.C.
//------------------------------------------------------------------------------

class WitNstnMeDir: public WitMeDir
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitNstnMeDir (WitMeMgr *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitNstnMeDir ();

      //------------------------------------------------------------------------
      // Overrides from class MeDir.
      //------------------------------------------------------------------------

      virtual WitMeDir *   oppMeDir       ();
      virtual WitMePt *    myMePt         (WitMeSitePt *);
      virtual WitPeriod    initSelExecPer (WitMeSitePt *);
      virtual WitPeriod    nextExecPer    (WitPeriod, WitMeSitePt *);
      virtual const char * myID           ();

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitNstnMeDir);
   };

//------------------------------------------------------------------------------
// Class AsapMeDir
//
// "ASAP Multi-Exec Direction"
// Responsible for the behavior of ASAP Multi-Exec that distinguishes it from
// NSTN Multi-Exec.
//
// Class Hierarchy:
//
// MeDir
//    AsapMeDir
//
// Implemented in MultiEx.C.
//------------------------------------------------------------------------------

class WitAsapMeDir: public WitMeDir
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitAsapMeDir (WitMeMgr *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitAsapMeDir ();

      //------------------------------------------------------------------------
      // Overrides from class MeDir.
      //------------------------------------------------------------------------

      virtual WitMeDir *   oppMeDir       ();
      virtual WitMePt *    myMePt         (WitMeSitePt *);
      virtual WitPeriod    initSelExecPer (WitMeSitePt *);
      virtual WitPeriod    nextExecPer    (WitPeriod, WitMeSitePt *);
      virtual const char * myID           ();

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitAsapMeDir);
   };

#endif
