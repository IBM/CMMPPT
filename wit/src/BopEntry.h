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

#ifndef BopEntryH
#define BopEntryH

//------------------------------------------------------------------------------
// Header file: "BopEntry.h"
//
// Contains the declaration of the class BopEntry
// See also the "class hierarchy" comment in Component.h.
//------------------------------------------------------------------------------

#include <BillEntry.h>

//------------------------------------------------------------------------------
// class BopEntry: "Bill-Of-Products Entry"
// A BillEntry representing the production of a Material in the execution of a
// Operation.
//
// Class Hierarchy:
//
// ProbAssoc
//    Component
//       DelComp
//          BillEntry
//             BopEntry
//------------------------------------------------------------------------------

class WitBopEntry: public WitBillEntry
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitBopEntry (WitOperation * theOpn, WitPart * thePart);
         //
         // myOperation_ <-- theOpn.
         // BillEntry    <<< thePart.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitBopEntry ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitOperation *,              myOperation)
      accessFunc (const WitDblFlexVec &,       productRate)
      accessFunc (bool,                        expAllowed)
      accessFunc (double,                      expAversion)
      accessFunc (bool,                        meSelSplit)
      accessFunc (const WitDblFlexVec &,       effProdRate)
      accessFunc (const WitTVec <WitPeriod> &, expExecPeriod)

      //------------------------------------------------------------------------
      // Mapping functions
      //------------------------------------------------------------------------

      decMappingIndex (WitBopEntry)

      static int nInstances (WitProblem *);

      //------------------------------------------------------------------------
      // Default value functions.
      //------------------------------------------------------------------------

      defaultValue (double defProductRate, 1.0)
      defaultValue (bool   defExpAllowed,  true)
      defaultValue (double defExpAversion, 0.0)
      defaultValue (double defPipShare,    1.0)
      defaultValue (bool   defMeSelSplit,  false)

      //------------------------------------------------------------------------
      // "Set" functions.
      //------------------------------------------------------------------------

      void setProductRate (const WitIVRealArg &);
      void setExpAllowed  (bool);
      void setExpAversion (double);
      void setMeSelSplit  (bool);

      //------------------------------------------------------------------------
      // store<Activity><Attr> functions.
      // Each of these functions sets the value of indicated attribute to the
      // value of the argument.
      // Each function is only valid when the indicated activity is being
      // performed.
      //------------------------------------------------------------------------

      void storePreEffProdRate   (const WitTVec <double> &);
      void storePreExpExecPeriod (const WitTVec <WitPeriod> &);
         //
         // Valid only when preprocessing is being performed.

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      virtual bool          inserted         ();
      virtual void          identifyInstance ();
      virtual void          writeSetClause   ();
      virtual void          copySelfInto     (WitCompMgr * theCompMgr);
      virtual void          recount          ();
      virtual void          unpreprocess     ();
      virtual WitBopEntry * thisBopEnt       ();
         //
         // Overrides from class Component.

      virtual WitMsgFrag objTypeMsgFrag ();
      virtual WitAttr    apiObjItrState ();
         //
         // Overrides from class DelComp.

      void display ();
         //
         // Displays the input and preprocessing data for this BopEntry.

      void copyAttrsFrom (WitBopEntry * theBopEnt);
         //
         // Copies the input attributes of theBopEnt into this BopEntry.

      bool expEligible (WitPeriod expPer)
         {
         return (expExecPeriod_[expPer] >= 0);
         }
         //
         // Returns true, iff this BopEntry is eligible for explosion in
         // period expPer.

      bool execPerOKForExp (WitPeriod execPer);
         //
         // Returns true, iff this BopEntry is eligible for explosion in
         // execution period execPer.

      static void getAllInstances (
            WitProblem *                theProblem,
            WitObjStack <WitBopEntry> & theBopEntries);
         //
         // Sets theBopEntries to be the set of all BOP entries for theProblem.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitBopEntry);

      virtual WitOperation * derivedOperation ();
         //
         // Override from class BillEntry.

      virtual void writeDataID ();
         //
         // Override from class Component.

      virtual void writeDataAttrs ();
         //
         // Override from class Component.

      virtual void findPrereqs (WitDelComp * & prereq1, WitDelComp * & prereq2);
         //
         // Override from class DelComp.

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Data set by the constructor.
      //------------------------------------------------------------------------

      int mappingIndex_;
         //
         // Mapping index: see Mapping.h.
         // == # BopEntries constructed before this BopEntry.

      WitOperation * const myOperation_;
         //
         // The Operation to which this BopEntry belongs.

      //------------------------------------------------------------------------
      // Input data.
      //------------------------------------------------------------------------

      WitDblFlexVec productRate_;
         //
         // Production rate. Each unit of execution of myOperation in thePer
         // nominally produces productRate ()[thePer] units of myPart ().
         // Range: [0.0, +inf).

      bool expAllowed_;
         //
         // This BopEntry is allowed to be used for explosion, iff expAllowed_ 
         // is true.

      double expAversion_;
         //
         // "Explosion Aversion":
         // BopEntries with lower expAversion are preferred for explosion over
         // those with higher expAversion.
         // Range: (-inf, +inf).

      bool meSelSplit_;
         //
         // true, iff selection splitting for multi-exec is to performed at this
         // BopEntry.

      //------------------------------------------------------------------------
      // Data set by preprocessing.
      //------------------------------------------------------------------------

      WitDblFlexVec effProdRate_;
         //
         // productRate, scaled down by yieldRate. Value == 0.0 when
         // inEffect (t) is false. effProdRate_[t] units of myPart () are
         // produced when 1 unit of myOperation () is executed in completion
         // period t. This data should be used in place of productRate for all
         // implosion and explosion calculations.

      WitTVec <WitPeriod> expExecPeriod_;
         //
         // expExecPeriod_[expPer] is the preferred period in which
         // myOperation () is to be executed in order to meet demand for 
         // myPart () in period expPer for explosion purposes.
   };

#endif
