#ifndef LGFRLASTPROBLEMSOLVED_H
#define LGFRLASTPROBLEMSOLVED_H
// RW2STL -- inserted:
#include <string>
// End of RW2STL -- inserted includes.
// Author: JP

#include <scenario/src/model.h>

#ifdef RS6K
// WIT does not support OptImplode in 64 bit mode
#ifndef __64BIT__
#ifndef DEMO
#define OPT_IMPLODE
#endif
#endif
#endif

#ifdef __OS2__
#ifndef DEMO
#endif
#endif

#ifdef WIN32
#ifndef DEMO
#endif
#endif

#ifdef __WINDOWS__
#ifndef DEMO
#endif
#endif

#include <wit/src/wit.h>
#include <scenario/src/timeVecs.h>
#include <scenario/src/witRunVn.h>
#include <scenario/src/sOfParts.h>
#include <scenario/src/sOfDemds.h>
#include <scenario/src/partSchd.h>
#include <scenario/src/demdSchd.h>

// Forward Reference
class LgFrWorkingImplosionProblem;
class LgFrWitRunVeneer;

// Represents the data stored in WIT and its associated solution if one exits.
//
// This object responsibilites are:
//   - Interface with WIT to:
//        - Get number of periods in problem
//        - Get WIT global attributes
//        - Cause WIT to do implosion or mrp
//        - Obtain a critical list
//        - Get WIT part supply
//        - Get WIT demand volume
//        - Get WIT solution vectors associated with parts and demands
//        - Get list of parts
//        - Get list of demands associated with a part
//   - Return pointer pointer to theWitRun associated with this object
//   - Create a WitRun object
//   - Apply WIT data changes stored in workingImplosionProblem object

extern "C" {typedef witReturnCode (STDCALL * apiFuncgetPartFloatVecAttribute)
       (WitRun* const,const char* const,float**);}

extern "C" {typedef witReturnCode (STDCALL * apiFuncwriteSchedule)
       (WitRun* const,const char* const,const witFileFormat);}

extern "C" {typedef witReturnCode (STDCALL * apiFuncsetPartFloatVecAttribute)
      (WitRun* const ,const char* const,const float * const);}

extern "C" {typedef witReturnCode (STDCALL * apiFuncgetDemandFloatVecAttribute)
    (WitRun* const,const char* const,const char* const,float**);}

extern "C" {typedef witReturnCode (STDCALL * apiFuncsetPartBoundsAttribute)
    (WitRun* const, const char* const, const float * const, 
         const float * const, const float * const );}

extern "C" {typedef witReturnCode (STDCALL * apiFuncgetDemandSoftLowerBoundAttribute)
    (WitRun* const, const char* const, const char* const, 
         float **, float **, float ** );}

extern "C" {typedef witReturnCode (STDCALL * apiFuncsetDemandBoundsAttribute)
    (WitRun* const, const char* const, const char* const,
         const float * const, const float * const, const float * const );}

extern "C" {typedef witReturnCode (STDCALL * apiFuncsetOperationFloatVecAttribute)
      (WitRun* const ,const char* const,const float * const);}

extern "C" {typedef witReturnCode (STDCALL * apiFuncsetOperationIntVecAttribute)
      (WitRun* const ,const char* const,const int * const);}

class LgFrLastProblemSolved : public LgFrSubscription {

private:
  static const std::string accessModeA;
  static const LgFrSchedulePrintStyle pmTEXT;
public:


  //-------------------------------------------------------------------------
  //
  // Build Ahead   
  // If true then all demand build ahead limits are set to nperiods,
  // by the lps update method.
  // If set to false then build ahead limits are set to 0.
  //
  //-------------------------------------------------------------------------

  // witGetBuildAhead
  virtual
     void
        buildAhead(
           bool );

  // witGetBuildAhead
  virtual
     bool
        buildAhead()
           const;

  //-------------------------------------------------------------------------
  //
  // Critical List
  //
  //-------------------------------------------------------------------------

  // Get Critical List with witGetCritcalList.
  virtual
     void
        criticalList(
           int * lenCritList,
           char * * * critPartList,
           int * * critPeriod)
           const;


  //-------------------------------------------------------------------------
  //
  // Wit Focus Shortage Horizon Functions
  //
  //-------------------------------------------------------------------------

  // Set wit global flag useFocusHorizons with witSetFocusHorizons
  virtual
     void
        useFocusHorizons(
           const bool useFocusHorizons ); 

  // Get wit global flag useFocusHorizons with witGetFocusHorizons
  virtual
     bool
        useFocusHorizons()
           const; 

  // Set a demand's fssShipVol with witSetDemandFssShipVol
  virtual
     void
        fssShipVolume(
           const LgFrDemand * const demand, 
           const LgFrTimeVecFloat& fssShipVol );

  // Get a demand's fssShipVol with witGetDemandFssShipVol.
  virtual
      LgFrTimeVecFloat
         fssShipVolume(
            const LgFrDemand * const demand )
               const;


  // Set focus for specified demand and period with witSetDemandFocusHorizon
  virtual
     void
        focusHorizon(
           const LgFrDemand * const demand,
           const int period );
  
  // Get a demands focus horizon with witGetDemandFocusHorizon
  virtual
     int
        focusHorizon(
           const LgFrDemand * const demand )
           const;

  // Set names of all parts whose shortage volume is not 0 
  // with witGetFocusShortageVol.
  virtual
     void
        focusShortageVolume(
           int * lenList,
           char *** partList,
           float *** shortageVolList )
              const;

  // Get the shortage quantity of a part with witGetPartFocusShortageVol.
  virtual
     LgFrTimeVecFloat
        shortageVolume(
           const LgFrPart * const part )
              const;

    // Get the fss production volume(timeVecFloat) of a part with witGetProcessExecVol.
  virtual
     LgFrTimeVecFloat
        fssProductionVolume(
           const LgFrPart * const part )
              const;

  //-------------------------------------------------------------------------
  //
  // Wit Global Attributes
  //
  //-------------------------------------------------------------------------

  // witSetComputeCriticalList
  virtual
     void
        computeCriticalList( 
           const bool flag );


  // witSetEquitability
  virtual
     void
        equitability(
           const int e );


  // witSetWbounds
  virtual
     void
        weightBounds(
           const float w );

  // Get the number of periods in problem with witGetNPeriods.
  virtual
     int
        numberOfPeriods()
           const;


  // witGetComputeCriticalList
  virtual
     bool
        computeCriticalList()
           const;

  // witGetEquitability   
  virtual
     int
        equitability()
           const;


  // witGetWbounds   
  virtual
     float
        weightBounds()
           const;

  // witGetImploded
  virtual
     bool
        imploded()
           const;

  // Return true if there is an OptImplode solution.
  virtual
     bool
        optImploded()
           const;

  // witGetObjValues
  virtual
     float
        objValue()
           const;

  // witGetObjValues
  virtual
     float
        boundsObjValue()
           const;



  //-------------------------------------------------------------------------
  //
  // Wit Action functions
  //
  //-------------------------------------------------------------------------

  // Update wit data structures with working implosion problem and then
  // implode using method stored in working implosion problem.
  virtual
     void
        implode(
           const LgFrWorkingImplosionProblem & wip );

  // witMrp
  virtual
     void
        mrp(
           const LgFrWorkingImplosionProblem & wip );

  //-------------------------------------------------------------------------
  //
  // Wit File Output functions
  //
  //-------------------------------------------------------------------------

  // Apply changes in wip and write wit data file to the file named fileName.
  virtual
     void
        writeWitDataFile(
           const std::string& fileName,
           const LgFrWorkingImplosionProblem & wip,
           const std::string & fopenAccessMode = accessModeA);

   // witWriteProdSched
   virtual
      bool
         writeProductionSchedule(
           const std::string& fileName,
           const std::string & fopenAccessMode = accessModeA,
	   const LgFrSchedulePrintStyle & ot = pmTEXT);

   // witWriteShipSched
   virtual
      bool
         writeShipmentSchedule(
           const std::string& fileName,
           const std::string & fopenAccessMode = accessModeA,
	   const LgFrSchedulePrintStyle & ot = pmTEXT);

   // witWriteReqSched
   virtual
      bool
         writeRequirementSchedule(
           const std::string& fileName,
           const std::string & fopenAccessMode = accessModeA,
	   const LgFrSchedulePrintStyle & ot = pmTEXT);

  //-------------------------------------------------------------------------
  //
  // Part functions
  //
  //-------------------------------------------------------------------------

  // Set the supply of a part with witSetPartSupplyVol.
  virtual
     void
        supplyVolume( 
           const LgFrPart * const part, 
           const LgFrTimeVecFloat& supply );

  // Set the cycleTime of a part with witSetPartCycleTime.
  virtual
     void
        cycleTime( 
           const LgFrPart * const part, 
           const LgFrTimeVecFloat& supply );


  // Set the lot size increment of a part with witSetPartIncLotSize
  virtual
     void
        incLotSize(
           const LgFrPart * const part, 
           const LgFrTimeVecFloat& lotSizeInc );

    // Set the minimum lot size of a part with witSetPartMinLotSize
  virtual
     void
        minLotSize(
           const LgFrPart * const part, 
           const LgFrTimeVecFloat& minSize );

    // Set the minimum lot size of a part with witSetPartYield
  virtual
     void
        yield(
           const LgFrPart * const part, 
           const LgFrTimeVecFloat& yield );

  // witSetPartProdBounds
  // A null bound pointer leaves the bound unchanged
  virtual
     void
        productionBounds(
           const LgFrPart * const part,
           const LgFrTimeVecFloat* const hardLowerBound,
           const LgFrTimeVecFloat* const softLowerBound,
           const LgFrTimeVecFloat* const hardUpperBound );

  // Get name of all parts with witGetParts.
  virtual
     LgFrStringVector
        parts()
           const;

  // Get the supply of a part with witGetPartSupplyVol.
  virtual
     LgFrTimeVecFloat
        supplyVolume(
           const LgFrPart * const part )
              const;

  // Get the cycleTime of a part with witGetPartCycleTime.
  virtual
     LgFrTimeVecFloat
        cycleTime(
           const LgFrPart * const part )
              const;

  // Get the excess of a part with witGetPartExcessVol.
  virtual
     LgFrTimeVecFloat
        excessVolume(
           const LgFrPart * const part )
              const;

  // Get the scrapVol of a part with witGetPartScrapVol.
  virtual
     LgFrTimeVecFloat
        scrapVolume(
           const LgFrPart * const part )
              const;

  // Get the stockVol of a part with witGetPartStockVol.
  virtual
     LgFrTimeVecFloat
        stockVolume(
           const LgFrPart * const part )
              const;

  // Get the production quantity of a part with witGetPartProdVol.
  virtual
     LgFrTimeVecFloat
        productionVolume(
           const LgFrPart * const part ) 
              const;

  // Get the requirement volume of a part with witGetPartReqVol.
  virtual
     LgFrTimeVecFloat
        requirementVolume(
           const LgFrPart * const part ) 
              const;


  // Get the requirement volume of a part with witGetPartMrpProdVol.
  virtual
     LgFrTimeVecFloat
        mrpProductionVolume(
           const LgFrPart * const part )
              const;


  // Get the requirement volume of a part with witGetPartMrpConsVol.
  virtual
     LgFrTimeVecFloat
        mrpConsVolume(
           const LgFrPart * const part )
              const;

  // Get the lot size increment of a part with witGetPartIncLotSize
  virtual
     LgFrTimeVecFloat
        incLotSize(
           const LgFrPart * const part )
              const;

  // Get the minimum lot size of a part with witGetPartMinLotSize
  virtual
     LgFrTimeVecFloat
        minLotSize(
           const LgFrPart * const part )
              const;

  // Get the yield of a part with witGetPartYield
  virtual
     LgFrTimeVecFloat
        yield(
           const LgFrPart * const part )
              const;

  // Get the category of a part with witGetPartCategory
  virtual
     LgFrPartCategory
        category(
           const LgFrPart * const part ) 
              const;


  // Get the number of BOM entries on a part.
  // Zero returned if part is not a product.
  virtual
     int
        nBomEntries(
           const LgFrPart * const part )
              const;

  // Get the the consumed part name that is at index i of part p.  The
  // caller of this method is responsible for freeing the char * (part-name)
  // storage.
  virtual
     char *
        bomEntryConsumedPart (
			      const LgFrPart & p,
			      const int i)
            const;

  // Get a part's production soft lower bounds with witGetPartProdBounds.
  virtual
    LgFrTimeVecFloat
      prodSoftLowerBound( const LgFrPart * const p )
	const;

  //-------------------------------------------------------------------------
  //
  // Demand functions
  //
  //-------------------------------------------------------------------------

  // Set a demand's volume with witSetDemandDemandVol
  virtual
     void
        demandVolume( 
           const LgFrDemand * const demand, 
           const LgFrTimeVecFloat& quantity );


  // Set a demand's priority with witSetDemandPriority
  // The vector of float priority is rounded to nearest integer value.
  virtual
     void
        priority( 
           const LgFrDemand * const demand, 
           const LgFrTimeVecFloat priority );

  // Set a demand's obj1ShipReward with witSetObj1ShipReward
  virtual
     void
        obj1ShipReward( 
           const LgFrDemand * const demand, 
           const LgFrTimeVecFloat& obj1ShipReward );

  // Set a demand's obj1CumShipReward with witSetObj1CumShipReward
  virtual
     void
        obj1CumShipReward( 
           const LgFrDemand * const demand, 
           const LgFrTimeVecFloat& obj1CumShipReward );

  // witSetDemandBuildAheadLimit
  virtual
     void
        buildAheadLimit( 
           const LgFrDemand * const demand, 
           const int buildAheadLimit );

  // witSetDemandShipLateLimit
  virtual
     void
        shipLateLimit( 
           const LgFrDemand * const demand, 
           const int shipLateLimit );


  // witSetDemandCumShipBounds
  // A null bound pointer leaves the bound unchanged
  virtual
     void
        cumShipBounds(
           const LgFrDemand * const demand,
           const LgFrTimeVecFloat* const hardLowerBound,
           const LgFrTimeVecFloat* const softLowerBound,
           const LgFrTimeVecFloat* const hardUpperBound );

  

  
  // Get name of all demands for a part with witGetPartDemands.
  virtual
     void
        demands(
           const LgFrPart * const part,
           int * lenDemandList,
           char *** demandList )
              const;

  // Get a demand's quantity with witGetDemandDemandVol.
  virtual
      LgFrTimeVecFloat
         demandVolume(
            const LgFrDemand * const demand )
               const;

  // Get quantity to be shipped to a demand witGetDemandDemandShipVol.
  virtual
     LgFrTimeVecFloat 
        shipmentVolume(
           const LgFrDemand * const demand )
              const;

  // Get a demand's backlog.  This is computed from demandVolume and
  // shipmentVolume. 
  // ( backlog = cumulative(demandVolume) - cumulative(shipmentVolume) )
  virtual
     LgFrTimeVecFloat 
        backlogVolume(
           const LgFrDemand * const demand )
              const;


  // Get a demand's priority with witGetDemandPriority
  // Priority is returned as an LgFrTimeVecFloat even though WIT represents
  // it as an integer.
  virtual
     LgFrTimeVecFloat
        priority( 
           const LgFrDemand * const demand )
              const; 

  // Get a demand's obj1ShipReward with witGetObj1ShipReward
  virtual
     LgFrTimeVecFloat
        obj1ShipReward( 
           const LgFrDemand * const demand )
              const; 

  // Get a demand's obj1CumShipReward with witGetObj1CumShipReward
  virtual
     LgFrTimeVecFloat
        obj1CumShipReward( 
           const LgFrDemand * const demand )
              const; 

  // Get demand's cumShip soft lower bound witGetDemandCumShipBounds
  virtual
     LgFrTimeVecFloat
        cumShipSoftLowerBound(
           const LgFrDemand * const demand ) 
              const;

  //-------------------------------------------------------------------------
  //
  // WIT Message Functions
  //
  //-------------------------------------------------------------------------

  // Turn on or off the printing of a WIT message.
  // msgNumber : is the message to change.
  //             Use WitINFORMATIONAL_MESSAGES to specify all
  //             informational messages.
  // msgOn : if true then printing the message is turned on.
  virtual
     void
        printMessage( 
          int msgNumber, 
          bool msgOn );

  //-------------------------------------------------------------------------
  //
  // Non-Wit API functions
  //
  //-------------------------------------------------------------------------

  // Return a pointer to LastSolvedProblem's WitRun.
  // The returned value is used as the first parameter to WIT API functions.
  virtual
     WitRun*
        witRun();
  
  // Return a pointer to LastSolvedProblem's WitRunVeneer.
  virtual
     const
        LgFrWitRunVeneer &
           witRunVeneer()
              const;
  virtual
     LgFrWitRunVeneer &
        witRunVeneer();
  
  // Update wit data structures with working implosion problem
  virtual
     void
        update(
           const LgFrWorkingImplosionProblem & wip );


   //-------------------------------------------------------------------------
   //
   // Subscribe methods
   //
   //-------------------------------------------------------------------------

   // Inform object m when the implosion solution or MRP solution
   // has changed.  The object added must have a member function
   // updateFrom( LgFrModel * p, void * d ).  
   // When the implosion solution
   // is changed then m.updateFrom( this, LgFrImplode() ) is invoked.
   // When the MRP solution
   // is changed then m.updateFrom( this, LgFrExplode() ) is invoked.
   //
   // See LgFrModel and LgFrModelClient.
   void 
      addDependent( 
         LgFrModelClient * m);

   // No longer inform object m when implosion or MRP solution has changed.
   void 
      removeDependent(
         LgFrModelClient * m );

   // Not used since lps does not subscribe to any other objects.
   void
      localUpdate(
         LgFrModel *,
               void * );


  //-------------------------------------------------------------------------
  //
  // Destructor, assignment, and constructors.
  //
  //-------------------------------------------------------------------------

  // Destructor
  ~LgFrLastProblemSolved();

  // Assignment operator.
  LgFrLastProblemSolved&
     operator=(
        const LgFrLastProblemSolved& rhs);

  // Default Copy constructor
  LgFrLastProblemSolved(
     const LgFrLastProblemSolved& source);

  // Default Constructor
  LgFrLastProblemSolved( );


//-------------------------------------------------------------------------
//
// Unit Test
//
//-------------------------------------------------------------------------


  // Tests this class.  Only returns if all tests passed
  static
    void
      test ();

  // Test this class in the context of it's owner(s).
  // The data for this test should have come from
  // "/u/implode/wit/data/brenda/denise/wit.data"
  static void contextTest(
      LgFrWorkingImplosionProblem &wip,
      LgFrLastProblemSolved       &lps,
      LgFrSetOfParts              &sop, 
      LgFrSetOfDemands            &sod);


private:
	// The WitRunVeneer that this Last Problem Solved is using to do
	// implosion.
	LgFrWitRunVeneer witRunVeneer_;	// 1

        // True if most recent implosion was an OptImplode
        bool optImploded_;

        // True if buld ahead limit is set to nPeriods by update method
        bool buildAhead_;

#ifdef OPT_IMPLODE
  // witOptImplode
  virtual
     void
        optImplode();
#endif

  // witHeurImplode
  virtual
     void
        heurImplode();

   // Get a float time vector part attribute
   LgFrTimeVecFloat
      getPartFloatVecAttribute(
         const LgFrPart * const part,
         apiFuncgetPartFloatVecAttribute apiFunc )
               const;

    // Set a float time vector part attribute
    void
       setPartFloatVecAttribute(
          const LgFrPart * const part,
  	  const LgFrTimeVecFloat& timeVec,
          apiFuncsetPartFloatVecAttribute  apiFunc);

   // Set a bound set part attribute
   void
      setPartBoundsAttribute(
         const LgFrPart * const part,
         const LgFrTimeVecFloat* const hardLowerBound,
         const LgFrTimeVecFloat* const softLowerBound,
         const LgFrTimeVecFloat* const hardUpperBound,
         apiFuncsetPartBoundsAttribute apiFunc);

   // Get a float time vector demand attribute
   LgFrTimeVecFloat
      getDemandFloatVecAttribute(
         const LgFrDemand * const demand,
         apiFuncgetDemandFloatVecAttribute apiFunc  )
               const;

  // Get a soft lower bound demand attribute
  LgFrTimeVecFloat
      getDemandSoftLowerBoundAttribute(
         const LgFrDemand * const demand,
         apiFuncgetDemandSoftLowerBoundAttribute apiFunc )
      const;
  
   // Set a bound set demand attribute
   void
      setDemandBoundsAttribute(
         const LgFrDemand * const demand,
         const LgFrTimeVecFloat* const hardLowerBound,
         const LgFrTimeVecFloat* const softLowerBound,
         const LgFrTimeVecFloat* const hardUpperBound,
         apiFuncsetDemandBoundsAttribute apiFunc );

   // Invoke witWrite*Sched functions
   bool
   writeSchedule(
      apiFuncwriteSchedule apiFunc,
      const std::string& fileName,
      const std::string& fopenAccessMode,
      const LgFrSchedulePrintStyle & ot = pmTEXT);

};

#endif
