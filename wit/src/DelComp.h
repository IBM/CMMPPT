//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef DelCompH
#define DelCompH

//------------------------------------------------------------------------------
// Header file: "DelComp.h"
//
// Contains the declaration of class
//------------------------------------------------------------------------------

#include <Component.h>

//------------------------------------------------------------------------------
// Class DelComp.
//
// A Deleteable Component.
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
//             Operation
//          BillEntry
//             ConsEntry
//                BomEntry
//                SubEntry
//             BopEntry
//          Demand
//
// Implemented in Component.C.
//------------------------------------------------------------------------------

class WitDelComp: public WitComponent
   {
   public:

      //------------------------------------------------------------------------
      // Public member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitDelComp ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (bool, selForDel)

      //------------------------------------------------------------------------
      // Default value functions.
      //------------------------------------------------------------------------

      defaultValue (bool defSelForDel, false)

      //------------------------------------------------------------------------
      // "Set" functions.
      //------------------------------------------------------------------------

      void setSelForDel (bool);

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void identifyOrAutoSel ();
         //
         // If this DelComp is selected for deletion, it is identified as being
         // selected by the user. Otherwise does automatic selection for
         // deletion, if required by the prerequisites for this DelComp.
         // Valid only during a purge.

      void identify ();
         //
         // Issues a Msg identifying this DelComp.

      virtual void identifyInstance () = 0;
         //
         // Issues a Msg distinguishing this DelComp from other instances of
         // its class.

      virtual WitDelComp * thisDelComp ();
         //
         // Override from class Component.

      virtual void writeSetClause () = 0;
         //
         // Writes a "set" clause for this DelComp to an input data file.

      void display ();
         //
         // Displays the input and preprocessing data for this DelComp.

      void prepSetScenSpecAttr (const char * theAttName);
         //
         // Takes action appropriate just before the value of an attribute is to
         // be set, thereby necessitating unpostprocessing.
         // The attribute is allowed to be set in stochastic mode.
         // theAttName is the name of the attribute.

      virtual WitMsgFrag objTypeMsgFrag () = 0;
         //
         // Returns a message fragment indicating the object type for this
         // DelComp.

      virtual WitAttr apiObjItrState () = 0;
         //
         // Base class function.
         // Returns the API objItrState attribute value that applies when the
         // DelCompItr is located at this DelComp.

      //------------------------------------------------------------------------
      // purgeSelComps functions.
      //
      // Each of the following functions removes the elements from the argument
      // List that are selected for deletion.
      //------------------------------------------------------------------------

      static void purgeSelComps (WitList <WitPart> &);
      static void purgeSelComps (WitList <WitOperation> &);
      static void purgeSelComps (WitList <WitBomEntry> &);
      static void purgeSelComps (WitList <WitSubEntry> &);
      static void purgeSelComps (WitList <WitBopEntry> &);
      static void purgeSelComps (WitList <WitDemand> &);

   protected:

      //------------------------------------------------------------------------
      // Protected member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitDelComp (WitCompMgr * theCompMgr);
         //
         // Component <<< theCompMgr.

      //------------------------------------------------------------------------
      // Other protected member functions.
      //------------------------------------------------------------------------

      virtual void writeDataAttrs ();
         //
         // Override from class Component.

      void copyAttrsFrom (WitDelComp * theDelComp);
         //
         // Copies the input attributes of theDelComp into this DelComp.

      virtual void findPrereqs (WitDelComp * & prereq1, WitDelComp * & prereq2);
         //
         // prereq1 <-- The first  prerequisite for this DelComp, if any,
         //             else NULL.
         // prereq2 <-- The second prerequisite for this DelComp, if any,
         //             else NULL.
         // This default version sets them to NULL.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitDelComp);

      void autoSelForDel (WitDelComp * prereq);
         //
         // If prereq is not NULL and
         //    prereq is selected for deletion and
         //    this DelComp is not selected for deletion,
         // selects this DelComp for deletion.

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Input data.
      //------------------------------------------------------------------------

      bool selForDel_;
         //
         // true, iff this Component is to be deleted at the next purge.
   };

#endif
