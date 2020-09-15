//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef CapacityH
#define CapacityH

//------------------------------------------------------------------------------
// Header file: "Capacity.h"
//
// Contains the declaration of class Capacity.
//------------------------------------------------------------------------------

#include <Part.h>

//------------------------------------------------------------------------------
// Class Capacity.
//
// Class Hierarchy:
//
// ProbAssoc
//    Component
//       DelComp
//          Node
//             Part
//                Capacity
//
// Implemented in MatCap.C.
//------------------------------------------------------------------------------

class WitCapacity: public WitPart
   {
   public:

      //------------------------------------------------------------------------
      // Public member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitCapacity (const char * theName, WitCompMgr * theCompMgr);
         //
         // Part <<< theName.
         // Part <<< theCompMgr.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitCapacity ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      virtual void          copySelfInto (WitCompMgr * theCompMgr);
      virtual void          recount      ();
      virtual WitCapacity * thisCap      ();
         //
         // Overrides from class Component.

      virtual WitMsgFrag categoryFrag ();
      virtual WitAttr    apiCategory  ();
      virtual bool       canStock     (WitPeriod t);
         //
         // Override from class Part.

      void copyAttrsFrom (WitCapacity * theCap);
         //
         // Copies the input attributes of theCap into this Capacity.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitCapacity);

      virtual WitPeriod findExplosionPeriod (WitPeriod fillPer);
         //
         // Override from class Part.

      virtual const char * categoryKeyWord ();
         //
         // Override from class Part.

      virtual void writeDataAttrs ();
         //
         // Override from class Component.
   };

#endif
