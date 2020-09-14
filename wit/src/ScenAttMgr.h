//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef ScenAttMgrH
#define ScenAttMgrH

//------------------------------------------------------------------------------
// Header file: "ScenAttMgr.h"
//
// Contains the declaration of class ScenAttMgr.
//------------------------------------------------------------------------------

#include <StochAssoc.h>
#include <List.h>

//------------------------------------------------------------------------------
// Class ScenAttMgr
//
// "Scenario Attribute Manager"
// Owns and maintains the ScenInputAtts and ScenSolnAtts for stochastic
// implosion.
//
// Class hierarchy:
//
//    ProbAssoc
//       StochAssoc
//          ScenAttMgr
//
// Implemented in ScenAtt.C.
//------------------------------------------------------------------------------

class WitScenAttMgr: public WitStochAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitScenAttMgr (WitStochModeMgr *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitScenAttMgr ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void downloadInput ();
         //
         // For each scenario-specific input attribute, this function downloads
         // the value of the attribute from each of the owners into the
         // corresponding ScenSolnAtt for the current Scenario.

      void uploadInput ();
         //
         // For each scenario-specific input attribute, this function uploads
         // the value of the attribute for the current Scenario to each of the
         // owners.
         // Valid only when the stochastic implosion subsystem is uploading
         // data.

      void copyInputFrom (WitScenAttMgr * srcScenAttMgr);
         //
         // Copies the input attributes stored by srcScenAttMgr into this
         // ScenAttMgr.

      void writeCompData (WitScenario * theScenario, bool diffedSec);
         //
         // Writes the section of the input data file for a subset of the
         // scenario-specific input attributes/Components for theScenario.
         // diffedSec indicates whether attributes are being written in the
         // differentiated section or the undifferentiated of the file.
 
      void writeBoundSetData (WitScenario * theScenario, bool diffedSec);
         //
         // Writes the section of the input data file for a subset of the
         // scenario-specific input attributes/BoundSets for theScenario.
         // diffedSec indicates whether attributes are being written in the
         // differentiated section or the undifferentiated of the file.
 
      void validateData ();
         //
         // Verifies that the values of all scenario-specific the attribute are
         // valid for a stochastic implosion.

      void uploadSoln ();
         //
         // For each scenario-specific solution attribute, this function uploads
         // the value of the attribute for the current Scenario to each of the
         // owners.
         // Valid only when the stochastic implosion subsystem is uploading
         // data.

      void clearSoln ();
         //
         // Frees the storage space used by the vectors of all ScenSolnAtts.

      void store (WitAbsScenInputAtt * theInputAtt);
         //
         // Stores theInputAtt in this Scenario.
         // Valid only when this Scenario is building its ScenInputAtts.

      void store (WitAbsScenSolnAtt * theSolnAtt);
         //
         // Stores theSolnAtt in this Scenario.
         // Valid only when this Scenario is building its ScenSolnAtts.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitScenInputAtt <WitPart>      *, supplyVol)
      accessFunc (WitScenInputAtt <WitPart>      *, scrapCost)
      accessFunc (WitScenInputAtt <WitMaterial>  *, stockCost)
      accessFunc (WitScenInputAtt <WitDemand>    *, demandVol)
      accessFunc (WitScenInputAtt <WitDemand>    *, shipReward)
      accessFunc (WitScenInputAtt <WitDemand>    *, cumShipReward)
      accessFunc (WitScenInputAtt <WitOperation> *, execCost)
      accessFunc (WitScenInputAtt <WitSubEntry>  *, subCost)
      accessFunc (WitScenInputAtt <WitBoundSet>  *, hardLB)
      accessFunc (WitScenInputAtt <WitBoundSet>  *, softLB)
      accessFunc (WitScenInputAtt <WitBoundSet>  *, hardUB)
      accessFunc (WitScenSolnAtt  <WitPart>      *, scrapVol)
      accessFunc (WitScenSolnAtt  <WitMaterial>  *, stockVol)
      accessFunc (WitScenSolnAtt  <WitDemand>    *, shipVol)
      accessFunc (WitScenSolnAtt  <WitDemand>    *, cumShipVol)
      accessFunc (WitScenSolnAtt  <WitOperation> *, execVol)
      accessFunc (WitScenSolnAtt  <WitSubEntry>  *, subVol)
      accessFunc (const WitList   <WitMaterial>  &, allMats)
      accessFunc (const WitList   <WitDemand>    &, allDemands)
      accessFunc (const WitList   <WitSubEntry>  &, allSubs)
      accessFunc (const WitList   <WitBoundSet>  &, allBoundSets)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      void listOwners ();
         //
         // Builds each of the Lists of Owner objects.

      void buildScenInputAtts ();
      void buildScenSolnAtts  ();
         //
         // Builds the ScenInputAtts owned by this ScenAtrMgr.
         // Builds the ScenSolnAtts  owned by this ScenAtrMgr.

      noCopyCtorAssign (WitScenAttMgr);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitList <WitAbsScenInputAtt> myAbsScenInputAtts_;
      WitList <WitAbsScenSolnAtt>  myAbsScenSolnAtts_;
         //
         // The List of all AbsScenInputAtts owned by this ScenAttMgr.
         // The List of all AbsScenSolnAtts  owned by this ScenAttMgr.

      bool buildingInputAtts_;
         //
         // True, iff this Scenario is currently building its ScenInputAtts.

      bool buildingSolnAtts_;
         //
         // True, iff this Scenario is currently building its ScenSolnAtts.

      //------------------------------------------------------------------------
      // The ScenInputAtts owned by this Scenario.
      //------------------------------------------------------------------------

      WitScenInputAtt <WitPart>      * supplyVol_;
      WitScenInputAtt <WitPart>      * scrapCost_;
      WitScenInputAtt <WitMaterial>  * stockCost_;
      WitScenInputAtt <WitDemand>    * demandVol_;
      WitScenInputAtt <WitDemand>    * shipReward_;
      WitScenInputAtt <WitDemand>    * cumShipReward_;
      WitScenInputAtt <WitOperation> * execCost_;
      WitScenInputAtt <WitSubEntry>  * subCost_;
      WitScenInputAtt <WitBoundSet>  * hardLB_;
      WitScenInputAtt <WitBoundSet>  * softLB_;
      WitScenInputAtt <WitBoundSet>  * hardUB_;

      //------------------------------------------------------------------------
      // The ScenSolnAtts owned by this ScenAttMgr.
      //------------------------------------------------------------------------

      WitScenSolnAtt <WitPart>      * scrapVol_;
      WitScenSolnAtt <WitMaterial>  * stockVol_;
      WitScenSolnAtt <WitDemand>    * shipVol_;
      WitScenSolnAtt <WitDemand>    * cumShipVol_;
      WitScenSolnAtt <WitOperation> * execVol_;
      WitScenSolnAtt <WitSubEntry>  * subVol_;

      //------------------------------------------------------------------------
      // Lists of all instances of various classes that will function as Owners
      // for the ScenAtts.
      //------------------------------------------------------------------------

      WitList <WitMaterial> allMats_;
      WitList <WitDemand>   allDemands_;
      WitList <WitSubEntry> allSubs_;
      WitList <WitBoundSet> allBoundSets_;
   };

#endif
