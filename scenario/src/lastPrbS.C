// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
#include <string>
// End of RW2STL -- inserted includes.
// File: LgFrLastProblemSolved.C
// Author: wally
// Created: 17 November 1993 7:20:21 am
// System: WITUSE

#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <limits.h>

#if !defined(WIN32) && !defined(__OS2__) && !defined(__WINDOWS__)
#include <unistd.h>
#endif


#include <iostream>
#include <math.h>
#include <limits.h>

#include <wit/src/wit.h>

#include <scenario/src/lastPrbS.h>
#include <scenario/src/demand.h>
#include <scenario/src/part.h>
#include <scenario/src/wrkgProb.h>
#include <scenario/src/implode.h>
#include <scenario/src/explode.h>
#include <scenario/src/machdep.h>


const std::string LgFrLastProblemSolved::accessModeA ("a");
const LgFrSchedulePrintStyle LgFrLastProblemSolved::pmTEXT (TEXT);
//-------------------------------------------------------------------------
//
// Build Ahead
//
//-------------------------------------------------------------------------

void
LgFrLastProblemSolved::buildAhead(
   bool b )
{
   buildAhead_ = b;
}

bool
LgFrLastProblemSolved::buildAhead()
const
{
   return buildAhead_;
}

//-------------------------------------------------------------------------
//
// Critical List
//
//-------------------------------------------------------------------------

// Get Critical List with witGetCritcalList.
void
LgFrLastProblemSolved::criticalList( 
   int * lenCritList,
   char * * * critPartList,
   int * * critPeriod) 
const
{
   witGetCriticalList( 
      witRunVeneer().constWitRun(), 
      lenCritList,
      critPartList,
      critPeriod);
}

//-------------------------------------------------------------------------
//
// Wit Focus Shortage Horizon Functions 
//
//-------------------------------------------------------------------------

// set wit global attribute useFocusHorizons
void
LgFrLastProblemSolved::useFocusHorizons(
   const bool flag )
{
   witSetUseFocusHorizons(
      witRunVeneer().mutableWitRun(),
      flag ? WitTRUE : WitFALSE );
}

// get wit global attribute useFocusHorizons
bool
LgFrLastProblemSolved::useFocusHorizons()
const
{
   witBoolean Bool;
   witGetUseFocusHorizons(
      witRunVeneer().constWitRun(), &Bool );
   if ( Bool ) return true;
   else        return false;
}


// Set a demand's fssShipVolume
void
LgFrLastProblemSolved::fssShipVolume(
   const LgFrDemand * const demand,
   const LgFrTimeVecFloat& fssShipVol )
{
   assert( numberOfPeriods() == fssShipVol.length() );
   witSetDemandFssShipVol(
            witRunVeneer().mutableWitRun(),
            demand->partPtr()->name().c_str(),
            demand->name().c_str(),
            fssShipVol.data() );
}

// Get demand's fssShipVolume
LgFrTimeVecFloat
LgFrLastProblemSolved::fssShipVolume(
        const LgFrDemand * const demand )
const
{
   return getDemandFloatVecAttribute( demand, witGetDemandFssShipVol );
}



// Set focus for specified demand and period
void
LgFrLastProblemSolved::focusHorizon(
   const LgFrDemand * const demand,
   const int period )
{
   
   witSetDemandFocusHorizon( 
            witRunVeneer().mutableWitRun(), 
            demand->partPtr()->name().c_str(),
            demand->name().c_str(),
            period );
   
}
// Get demand's focus horizon
int
LgFrLastProblemSolved::focusHorizon(
        const LgFrDemand * const demand )
const
{
   int retInt;
   witGetDemandFocusHorizon(
            witRunVeneer().constWitRun(),
            demand->partPtr()->name().c_str(),
            demand->name().c_str(),
            &retInt );
   return retInt;
}


// Get names of all parts who have nonzero shortages.  Amount of shortage
// is also returned.  
void
LgFrLastProblemSolved::focusShortageVolume(
   int * lenList,
   char *** partList,
   float *** shortageVolumeList )
const
{
   witGetFocusShortageVol( 
            witRunVeneer().constWitRun(), 
            lenList,
            partList,
            shortageVolumeList );
}

// Get the shortage quantity of a part with witGetPartFocusShortageVol.
LgFrTimeVecFloat 
LgFrLastProblemSolved::shortageVolume(
   const LgFrPart * const part )
const
{
   return getPartFloatVecAttribute( part, witGetPartFocusShortageVol );
}

// Get the fss production time vector float of a part with witGetOperationExecVol.
LgFrTimeVecFloat 
LgFrLastProblemSolved::fssProductionVolume(
   const LgFrPart * const part )
const
{
   return getPartFloatVecAttribute( part, witGetOperationFssExecVol );
}


//-------------------------------------------------------------------------
//
// Wit Global Attributes
//
//-------------------------------------------------------------------------

// Get number of time periods.
int
LgFrLastProblemSolved::numberOfPeriods()
const
{
   int nP;
   witGetNPeriods( witRunVeneer().constWitRun(), &nP );
   return nP;   
}



// witSetComputeCriticalList
void
LgFrLastProblemSolved::computeCriticalList(
   const bool flag )
{
   witSetComputeCriticalList( 
      witRunVeneer().mutableWitRun(),
      flag ? WitTRUE : WitFALSE );
}


// witGetComputeCriticalList
bool
LgFrLastProblemSolved::computeCriticalList()
const
{
   witBoolean Bool;
   witGetComputeCriticalList(
      witRunVeneer().constWitRun(), &Bool );
   if ( Bool ) return true;
   else        return false;
}


// witSetEquitability
void
LgFrLastProblemSolved::equitability(
   const int e )
{
   witSetEquitability( witRunVeneer().mutableWitRun(), e );
}

// witGetEquitability
int
LgFrLastProblemSolved::equitability()
const
{
   int e;
   witGetEquitability( witRunVeneer().constWitRun(), &e );
   return e;
}



// witSetWbounds
void
LgFrLastProblemSolved::weightBounds(
   const float w )
{
   witSetWbounds( witRunVeneer().mutableWitRun(), w );
}

// witGetWbounds
float
LgFrLastProblemSolved::weightBounds()
const
{
   float w;
   witGetWbounds( witRunVeneer().constWitRun(), &w );
   return w;
}

// witGetPostprocessed (used to be witGetImploded )
bool
LgFrLastProblemSolved::imploded()
const
{
   witBoolean s;
   witGetPostprocessed( witRunVeneer().constWitRun(), &s );
   return ( s ? WitTRUE : WitFALSE );
}

// Return true if there is an OptImplode solution.
bool
LgFrLastProblemSolved::optImploded()
const
{
   witBoolean s;
   witGetPostprocessed( witRunVeneer().constWitRun(), &s );
   return ( s && optImploded_ ? WitTRUE : WitFALSE );
}



// witGetObjValues
float
LgFrLastProblemSolved::objValue()
const
{
   float objValue,boundsValue;
   witGetObjValues( witRunVeneer().constWitRun(), &objValue, &boundsValue );
   return objValue;
}

// witGetObjValues
float
LgFrLastProblemSolved::boundsObjValue()
const
{
   float objValue,boundsValue;
   witGetObjValues( witRunVeneer().constWitRun(), &objValue, &boundsValue );
   return boundsValue;
}


//-------------------------------------------------------------------------
//
// Wit API Action functions
//
//-------------------------------------------------------------------------

#ifdef OPT_IMPLODE
// witOptImplode
void
LgFrLastProblemSolved::optImplode()
{
   // Perform implosion
   witOptImplode( 
          witRunVeneer().mutableWitRun() );
   optImploded_ = true;

   // Inform subscribers that a new implosion solution exists
   LgFrImplode implode(*this);
   LgFrSubscription::externalChange( &implode );
}
#endif

// witHeurImplode
void
LgFrLastProblemSolved::heurImplode()
{
   // Perform implosion
   witHeurImplode( 
          witRunVeneer().mutableWitRun() );
   optImploded_ = false;

   // Inform subscribers that a new implosion solution exists
   LgFrImplode implode(*this);
   LgFrSubscription::externalChange( &implode );
}

// Update wit data structures with working implosion problem and implode
// using method stored in working implosion problem
void
LgFrLastProblemSolved::implode(
   const LgFrWorkingImplosionProblem & wip )
{
  // Update problem data
  update(wip);

  // Get Implosion Method
  LgFrImplosionMethod impMeth = wip.implosionMethod();

  // Do implosion
  if ( impMeth==LgFrHEURISTIC )
     // witHeurImplode( witRunVeneer().mutableWitRun() );
     heurImplode();
#ifdef OPT_IMPLODE
  else if ( impMeth==LgFrOPTIMIZE )
     // witOptImplode( witRunVeneer().mutableWitRun() );
     optImplode();
#endif
}

// witMrp
void
LgFrLastProblemSolved::mrp(
    const LgFrWorkingImplosionProblem & wip)
{
   // Update problem data
   update(wip);

   // Perform MRP explosion
   witMrp( witRunVeneer().mutableWitRun() );

   // Inform subscribers that a new MRP explosion solution exists
   LgFrExplode explode(*this);
   LgFrSubscription::externalChange( &explode );
}

//-------------------------------------------------------------------------
//
// File output functions
//
//-------------------------------------------------------------------------

// witWriteData
// Update wit data with from a wip and then restore wit data.
void
LgFrLastProblemSolved::writeWitDataFile(
   const std::string& fileName,
   const LgFrWorkingImplosionProblem & wip,
   const std::string & fopenAccessMode  )
{

  // Make temporary copy of witRun
  LgFrWitRunVeneer savedWitRun(witRunVeneer());

  // Update problem data
  update(wip);

  // Set fopen access mode to append to an existing file
  witSetMesgFileAccessMode( 
     witRunVeneer().mutableWitRun(),
     WitTRUE,
     fopenAccessMode.c_str() );

  // Write file.  Use mutableWitRun because witWriteData does preprocessing
  // which changes theWitRun.  
  witWriteData( witRunVeneer().mutableWitRun(), fileName.c_str() );

  // Restore original witRun
  witRunVeneer() = savedWitRun;

}

// witWriteProdSched
bool
LgFrLastProblemSolved::writeProductionSchedule(
   const std::string& fileName,
   const std::string & fopenAccessMode,
   const LgFrSchedulePrintStyle  & ot  )
{
   return writeSchedule(
             witWriteProdSched,
             fileName,
             fopenAccessMode,
	     ot );
}

// witWriteShipSched
bool
LgFrLastProblemSolved::writeShipmentSchedule(
   const std::string& fileName,
   const std::string & fopenAccessMode,
   const LgFrSchedulePrintStyle  & ot  )
{
   return writeSchedule(
             witWriteShipSched,
             fileName,
             fopenAccessMode,
	     ot );
}

// witWriteReqSched
bool
LgFrLastProblemSolved::writeRequirementSchedule(
   const std::string & fileName,
   const std::string & fopenAccessMode,
   const LgFrSchedulePrintStyle  & ot  )
{
   return writeSchedule(
             witWriteReqSched,
             fileName,
             fopenAccessMode,
	     ot );
}


//-------------------------------------------------------------------------
//
// Part functions
//
//-------------------------------------------------------------------------

// Set the supply of a part with witSetPartSupplyVol.
void
LgFrLastProblemSolved::supplyVolume(
   const LgFrPart * const part,
   const LgFrTimeVecFloat& supply )
{
   assert( numberOfPeriods() == supply.length() );
   witSetPartSupplyVol( 
            witRunVeneer().mutableWitRun(), 
            part->name().c_str(),
            supply.data() );
}

// Set the cycleTime of a part with witSetPartCycleTime.
void
LgFrLastProblemSolved::cycleTime(
   const LgFrPart * const part,
   const LgFrTimeVecFloat& cTime )
{
   assert( numberOfPeriods() == cTime.length() );
   witSetPartCycleTime( 
            witRunVeneer().mutableWitRun(), 
            part->name().c_str(),
            cTime.data() );
}


// Set the lot size increment of a part with witSetPartIncLotSize
void
LgFrLastProblemSolved::incLotSize(
    const LgFrPart * const part, 
    const LgFrTimeVecFloat& lotInc )
{
    setPartFloatVecAttribute (part, lotInc, witSetPartIncLotSize);
}

// Set the minimum lot size of a part with witSetPartMinLotSize
void
LgFrLastProblemSolved::minLotSize(
           const LgFrPart * const part, 
           const LgFrTimeVecFloat& minSize )
{
    setPartFloatVecAttribute (part, minSize, witSetPartMinLotSize);
}


// Set the yield of a part with witSetPartYield
void
LgFrLastProblemSolved::yield(
           const LgFrPart * const part, 
           const LgFrTimeVecFloat& yield )
{
   witSetPartYield( 
            witRunVeneer().constWitRun(),
            part->name().c_str(),
            yield.intData() );
}

// Set production bounds with witSetProdBounds
// A null bound pointer leaves the bound unchanged
void
LgFrLastProblemSolved::productionBounds(
   const LgFrPart * const part,
   const LgFrTimeVecFloat* const hardLowerBound,
   const LgFrTimeVecFloat* const softLowerBound,
   const LgFrTimeVecFloat* const hardUpperBound )
{
   setPartBoundsAttribute( part,
                           hardLowerBound, softLowerBound, hardUpperBound,
                           witSetPartProdBounds
                           );
}

// Get a part's production soft lower bounds with witGetPartProdBounds.
LgFrTimeVecFloat
LgFrLastProblemSolved::prodSoftLowerBound( const LgFrPart * const p )
const
{
  float * hardLower;
  float * softLower; 
  float * hardUpper;
  witGetPartProdBounds (witRunVeneer().constWitRun(),
			p->name().c_str(),
			& hardLower, 
			& softLower, 
			& hardUpper );
  return LgFrTimeVecFloat ( this->numberOfPeriods(), softLower );
}



// Get name of all parts with witGetParts.
LgFrStringVector
LgFrLastProblemSolved::parts()
const
{
   int lenPartList;
   char ** partList;
   witGetParts( 
            witRunVeneer().constWitRun(), 
            &lenPartList,
            &partList );
   LgFrStringVector retVal(lenPartList);
   size_t i = 0; // Pulled out of the for below by RW2STL
   for( i=0; i<lenPartList; i++ ) {
     retVal[i]=std::string( partList[i] );
     portableFree( partList[i] );
   }
   portableFree( partList );
   return retVal;
}



// Get the supply of a part with witGetPartSupplyVol.
LgFrTimeVecFloat
LgFrLastProblemSolved::supplyVolume(
   const LgFrPart * const part )
const
{
   return getPartFloatVecAttribute( part, witGetPartSupplyVol );
}

// Get the cycleTime of a part with witGetPartCycleTime.
LgFrTimeVecFloat
LgFrLastProblemSolved::cycleTime(
   const LgFrPart * const part )
const
{
   return getPartFloatVecAttribute( part, witGetPartCycleTime );
}

// Get the excess of a part with witGetPartExcessVol.
LgFrTimeVecFloat
LgFrLastProblemSolved::excessVolume(
   const LgFrPart * const part )
const
{
   return getPartFloatVecAttribute( part, witGetPartExcessVol );
}

// Get the scrap of a part with witGetPartScrapVol.
LgFrTimeVecFloat
LgFrLastProblemSolved::scrapVolume(
   const LgFrPart * const part )
const
{
   return getPartFloatVecAttribute( part, witGetPartScrapVol );
}

// Get the stock of a part with witGetPartStockVol.
LgFrTimeVecFloat
LgFrLastProblemSolved::stockVolume(
   const LgFrPart * const part )
const
{
   return getPartFloatVecAttribute( part, witGetPartStockVol );
}



// Get the production quantity of a part with witGetPartProdVol.
LgFrTimeVecFloat 
LgFrLastProblemSolved::productionVolume(
   const LgFrPart * const part )
const
{
 LgFrTimeVecFloat prodVol = getPartFloatVecAttribute( part, witGetPartProdVol );

 // In Wit V3.4:
 //   prodVol was post yield.
 // In Wit V4.0:
 //   execVol is pre yield.
 // In Wit V4.0 when wit34Compatible is true:
 //   prodVol is pre yield.

 // If wit34Compatible is true then modify prodVol by yield to give same result
 // as Wit V3.4
 witBoolean wit34Compatible;
 witGetWit34Compatible( witRunVeneer().constWitRun(), &wit34Compatible );

 if( wit34Compatible )
   prodVol = prodVol * yield( part ) / 100.;

 return prodVol;
}

// Get the requirement volume of a part with witGetPartReqVol.
LgFrTimeVecFloat 
LgFrLastProblemSolved::requirementVolume(
   const LgFrPart * const part )
const
{
   return getPartFloatVecAttribute( part, witGetPartReqVol );
}

// Get the requirement volume of a part with witGetMrpPartReqVol.
LgFrTimeVecFloat
LgFrLastProblemSolved::mrpProductionVolume(
   const LgFrPart * const part )
const
{
   return getPartFloatVecAttribute( part, witGetPartMrpProdVol );
}


// Get the mrpCons volume of a part with witGetPartMrpConsVol.
LgFrTimeVecFloat
LgFrLastProblemSolved::mrpConsVolume(
   const LgFrPart * const part )
const
{
   return getPartFloatVecAttribute( part, witGetPartMrpConsVol );
}


// Get the lot size increment of a part with witGetPartIncLotSize
LgFrTimeVecFloat
LgFrLastProblemSolved::incLotSize(
    const LgFrPart * const part )
const
{
   return getPartFloatVecAttribute( part, witGetPartIncLotSize );
}

// Get the minimum lot size of a part with witGetPartMinLotSize
LgFrTimeVecFloat
LgFrLastProblemSolved::minLotSize(
    const LgFrPart * const part )
const
{
   return getPartFloatVecAttribute( part, witGetPartMinLotSize );
}


// Get the yield of a part with witGetPartYield
LgFrTimeVecFloat
LgFrLastProblemSolved::yield(
    const LgFrPart * const part )
const
{
   int nP = numberOfPeriods();
   int * cVec;

   witGetPartYield( 
            witRunVeneer().constWitRun(),
            part->name().c_str(),
            &cVec );

   LgFrTimeVecFloat fVec (nP);

   size_t t = 0; // Pulled out of the for below by RW2STL
   for ( t=0; t<nP; t++ ) fVec[t] = (float)cVec[t];

   portableFree(cVec);

   return fVec;
}

// Get the part category of a part with witGetPartCategory
LgFrPartCategory
LgFrLastProblemSolved::category(
const LgFrPart * const part )
const 
{
   // Get part category from wit
   witAttr partCat;
   witGetPartCategory( witRunVeneer().constWitRun(), 
            part->name().c_str(),
            &partCat );

   // Return part category in terms used by the Logistics frame work
   if      ( std::string(partCat) == std::string(WitPRODUCT) ) return LgFrPRODUCT;
   else if ( std::string(partCat) == std::string(WitRAW    ) ) return LgFrRAW;
   else if ( std::string(partCat) == std::string(WitCAPACITY)) return LgFrCAPACITY;
   assert( 1 == 0 );
   return LgFrPRODUCT;    // <-- unreachable line of code
}


// Get a part's bomEntry count with witGetPartNBomEntries
int
LgFrLastProblemSolved::nBomEntries(
   const LgFrPart * const part )
const
{
   int retVal = 0;
   if ( category(part) == LgFrPRODUCT ) {
      witGetPartNBomEntries(
            witRunVeneer().constWitRun(),
            part->name().c_str(),
            &retVal );
   }
   return retVal;
}

// Get the the consumed part name that is at index i of part p.  The
// caller of this method is responsible for freeing the char * (part-name)
// storage.
char *
LgFrLastProblemSolved::bomEntryConsumedPart (
					     const LgFrPart & p,
					     const int i)
const
{
  char * consumedPartName;
  consumedPartName = 0;  // Not necessary, but it eliminates a compiler
                            // warning
  witGetBomEntryConsumedPart( 
			     this->witRunVeneer().constWitRun(),
			     p.name().c_str(),
			     i,
			     &consumedPartName);
  return consumedPartName;
}




//-------------------------------------------------------------------------
//
// Demand functions
//
//-------------------------------------------------------------------------

// Set a demand's volume
void
LgFrLastProblemSolved::demandVolume(
   const LgFrDemand * const demand,
   const LgFrTimeVecFloat& quantity )
{
   assert( numberOfPeriods() == quantity.length() );
   witSetDemandDemandVol(
            witRunVeneer().mutableWitRun(), 
            demand->partPtr()->name().c_str(),
            demand->name().c_str(),
            quantity.data() );
}


   

// Set a demand's priority with witSetDemandPriority
// Priority is represented as an LgFrTimeVecFloat even though WIT represents
// it as an integer.
void
LgFrLastProblemSolved::priority(
   const LgFrDemand * const demand,
   const LgFrTimeVecFloat priority )
{
   witSetDemandPriority( 
            witRunVeneer().constWitRun(), 
            demand->partPtr()->name().c_str(),
            demand->name().c_str(),
            priority.intData() );
}


// Set a demand's obj1ShipReward
void
LgFrLastProblemSolved::obj1ShipReward(
   const LgFrDemand * const demand,
   const LgFrTimeVecFloat& obj1ShipReward )
{
   assert( numberOfPeriods() == obj1ShipReward.length() );

   int objChoice;
   witGetObjChoice( witRunVeneer().constWitRun(), &objChoice );

   if ( objChoice != 1 )
      std::cerr <<"Warning: LgFrLastProblemSolved::obj1ShipReward" << std::endl
           <<"         can not set obj1ShipReward because"    << std::endl
           <<"         objChoice is not 1" << std::endl;
   else
      witSetDemandObj1ShipReward(
               witRunVeneer().mutableWitRun(),
               demand->partPtr()->name().c_str(),
               demand->name().c_str(),
               obj1ShipReward.data() );
}


// Set a demand's obj1CumShipReward
void
LgFrLastProblemSolved::obj1CumShipReward(
   const LgFrDemand * const demand,
   const LgFrTimeVecFloat& obj1CumShipReward )
{
   assert( numberOfPeriods() == obj1CumShipReward.length() );

   int objChoice;
   witGetObjChoice( witRunVeneer().constWitRun(), &objChoice );

   if ( objChoice != 1 )
      std::cerr <<"Warning: LgFrLastProblemSolved::obj1CumShipReward" << std::endl
           <<"         can not set obj1CumShipReward because"    << std::endl
           <<"         objChoice is not 1" << std::endl;
   else
      witSetDemandObj1CumShipReward(
               witRunVeneer().mutableWitRun(),
               demand->partPtr()->name().c_str(),
               demand->name().c_str(),
               obj1CumShipReward.data() );
}


// Set a demand's buildAheadLimit with witSetBuildAheadLimit
void
LgFrLastProblemSolved::buildAheadLimit(
   const LgFrDemand * const demand,
   const int buildAheadLimit )
{
   witSetDemandBuildAheadLimit( 
            witRunVeneer().mutableWitRun(), 
            demand->partPtr()->name().c_str(),
            demand->name().c_str(),
            buildAheadLimit );
}

// Set a demand's shipLateLimit with witSetShipLateLimit
void
LgFrLastProblemSolved::shipLateLimit(
   const LgFrDemand * const demand,
   const int shipLateLimit )
{
   witSetDemandShipLateLimit( 
            witRunVeneer().mutableWitRun(), 
            demand->partPtr()->name().c_str(),
            demand->name().c_str(),
            shipLateLimit );
}
 
// Set cumulative shipment bounds with witSetCumShipBounds
// A null bound pointer leaves the bound unchanged
void
LgFrLastProblemSolved::cumShipBounds(
   const LgFrDemand * const demand,
   const LgFrTimeVecFloat* const hardLowerBound,
   const LgFrTimeVecFloat* const softLowerBound,
   const LgFrTimeVecFloat* const hardUpperBound )
{
   setDemandBoundsAttribute( demand,
                           hardLowerBound, softLowerBound, hardUpperBound,
                           witSetDemandCumShipBounds
                           );
}
  
   
// Get name of all demands for a part with witGetPartDemands.
void
LgFrLastProblemSolved::demands(
   const LgFrPart * const part,
   int * lenDemandList,
   char *** demandList )
const
{
   witGetPartDemands( 
            witRunVeneer().constWitRun(), 
            part->name().c_str(),
            lenDemandList,
            demandList );
}


// Get demand's quantity with witGetDemandDemandVol
LgFrTimeVecFloat
LgFrLastProblemSolved::demandVolume(
        const LgFrDemand * const demand ) 
const
{
   return getDemandFloatVecAttribute( demand, witGetDemandDemandVol );
}

// Get quantity to be shipped to a demand witGetDemandShipVol.
LgFrTimeVecFloat 
LgFrLastProblemSolved::shipmentVolume(
        const LgFrDemand * const demand )
const
{
   return getDemandFloatVecAttribute( demand, witGetDemandShipVol );
}

// Get a demands backlog.
LgFrTimeVecFloat
LgFrLastProblemSolved::backlogVolume(
        const LgFrDemand * const demand )
const
{
   return
      demandVolume(demand).scan(LgFrAdd)      // Cumulative demandVolume
      -                                       // Minus
      shipmentVolume(demand).scan(LgFrAdd);   // Cumulative shipmentVolume
}

   

// Get a demand's priority with witGetDemandPriority
// Priority is returned as an LgFrTimeVecFloat even though WIT represents
// it as an integer.
LgFrTimeVecFloat
LgFrLastProblemSolved::priority(
   const LgFrDemand * const demand )
const
{
   int nP = numberOfPeriods();
   int * cVec;

   witGetDemandPriority( 
            witRunVeneer().constWitRun(), 
            demand->partPtr()->name().c_str(),
            demand->name().c_str(),
            &cVec );

   LgFrTimeVecFloat fVec (nP);

   size_t t = 0; // Pulled out of the for below by RW2STL
   for ( t=0; t<nP; t++ ) fVec[t] = (float)cVec[t];

   portableFree(cVec);

   return fVec;
}



// Get demand's obj1ShipReward
LgFrTimeVecFloat
LgFrLastProblemSolved::obj1ShipReward(
        const LgFrDemand * const demand )
const
{
   LgFrTimeVecFloat retVal;

   int objChoice;
   witGetObjChoice( witRunVeneer().constWitRun(), &objChoice );

   if ( objChoice != 1 )
      retVal = LgFrTimeVecFloat( numberOfPeriods(), 0.0 );
   else 
      retVal = getDemandFloatVecAttribute( demand, witGetDemandObj1ShipReward );

   return retVal;
}


// Get demand's obj1CumShipReward
LgFrTimeVecFloat
LgFrLastProblemSolved::obj1CumShipReward(
        const LgFrDemand * const demand )
const
{
   LgFrTimeVecFloat retVal;

   int objChoice;
   witGetObjChoice( witRunVeneer().constWitRun(), &objChoice );

   if ( objChoice != 1 )
      retVal = LgFrTimeVecFloat( numberOfPeriods(), 0.0 );
   else
      retVal = 
         getDemandFloatVecAttribute( demand, witGetDemandObj1CumShipReward );

   return retVal;
}

// Get demand's cumShip soft lower bound witGetDemandCumShipBounds
LgFrTimeVecFloat
LgFrLastProblemSolved::cumShipSoftLowerBound(
        const LgFrDemand * const demand ) 
const
{
   return getDemandSoftLowerBoundAttribute(
                                           demand,
                                           witGetDemandCumShipBounds
                                          );
}


//-------------------------------------------------------------------------
//
// WIT Message Functions
//
//-------------------------------------------------------------------------

// Turn on or off the printing of a WIT message.
// msgNumber : is the message to change.
// msgOn : if true then printing the message is turned on.
void
LgFrLastProblemSolved::printMessage( 
  int msgNumber, 
  bool msgOn )
{
  witSetMesgTimesPrint(
    witRunVeneer().mutableWitRun(),
    WitTRUE,
    msgNumber,
    msgOn ? UCHAR_MAX : 0 );
}


//-------------------------------------------------------------------------
//
// Miscellaneous
//
//-------------------------------------------------------------------------

// Return a pointer to LastSolvedProblem's WitRun.
// The returned value is used as the first parameter to WIT API functions.
WitRun*
LgFrLastProblemSolved::witRun()
{
   return witRunVeneer().mutableWitRun();
}

// Return a pointer to LastSolvedProblem's WitRunVeneer.
LgFrWitRunVeneer &
LgFrLastProblemSolved::witRunVeneer()
{
   return witRunVeneer_;
}

const LgFrWitRunVeneer &
LgFrLastProblemSolved::witRunVeneer()
const
{
   return witRunVeneer_;
} 

// Update wit data structures with working implosion problem
void
LgFrLastProblemSolved::update(
   const LgFrWorkingImplosionProblem & wip )
{
   int i;

   // Update global parms stored in WIP
   equitability( wip.equitability() );
   computeCriticalList( wip.computeCriticalList() );

   // Update supply of all parts whose supply volumes have changed

   // Get a reference to a schedule containing all such parts and their
   // timeVecs
   const LgFrSortingPartScheduleFloat & chgPartVols
     = wip.supplyVolume(LgFrLATEST);

   int chgPartsEntries = chgPartVols.size();
   for ( i=0; i< chgPartsEntries; i++ ) {
      const LgFrItemTimeVecPairFloat & itv = chgPartVols[i];
      supplyVolume( &( itv.part() ),	      // Update supply of part
		    itv.timeVecFloat()    );
   }

   // Update supply of all parts whose cycle times have changed

   // Get all such parts
   LgFrPartVector chgPartsCT = wip.changedPartsCycleTime();

   int chgPartsCTEntries = chgPartsCT.size();
   for ( i=0; i< chgPartsCTEntries; i++ ) {

      // Update supply of part i
      cycleTime( &chgPartsCT[i], wip.cycleTime( chgPartsCT[i] ) );
   }

   // Update all changed demand volumes

   // Get all changed demands
   LgFrDemandVector chgDemands = wip.changedDemands();

   // Loop once for each changed demand
   int chgDemandsEntries = chgDemands.size();
   for ( i=0; i< chgDemandsEntries; i++ ) {

      // Update demand volume for demand i
      demandVolume( &chgDemands[i], wip.timeVec( chgDemands[i] ) );

   }
   
   // Update all changed demand priorities

   // Get all changed demands
   LgFrDemandVector chgPriority = wip.changedDemandsPriority();

   // Loop once for each changed demand
   int chgPriorityEntries = chgPriority.size();
   for ( i=0; i< chgPriorityEntries; i++ ) {

      // Update demand volume for demand i
      priority( &chgPriority[i], wip.priority( chgPriority [i] ) );

   }

   // Update all changed demand obj1ShipRewards

   // Get all changed demands
   LgFrDemandVector chgObj1ShipReward 
      = wip.changedDemandsObj1ShipReward();

   // Loop once for each changed demand
   int chgObj1ShipRewardEntries = chgObj1ShipReward.size();
   for ( i=0; i< chgObj1ShipRewardEntries; i++ ) {

      // Update demand volume for demand i
      obj1ShipReward( &chgObj1ShipReward[i], 
                      wip.obj1ShipReward( chgObj1ShipReward [i] ) );

   }


   // Update all changed demand cumShipSoftLowerBounds

   // Get all changed demands
   LgFrDemandVector chgCumShipSoftLowerBound 
      = wip.changedDemandsCumShipSoftLowerBound();

   // Loop once for each changed demand
   int chgCumShipSoftLowerBoundEntries = chgCumShipSoftLowerBound.size();
   for ( i=0; i< chgCumShipSoftLowerBoundEntries; i++ ) {
      // Update attribute value for demand i
      LgFrDemand demand = chgCumShipSoftLowerBound[i];
      LgFrTimeVecFloat softLowerBound = wip.cumShipSoftLowerBound( demand );
      cumShipBounds( &demand,  0, &softLowerBound, 0 );
   }
   
   // Set build ahead in lps to value in wip
   buildAhead( wip.buildAhead() );

   // Now set build ahead limit for every demand
   {
      // get list of parts
      int lenPartList;
      char **partList;
      witGetParts( witRunVeneer().constWitRun(), 
                   &lenPartList, &partList );
      
      // Loop once for each part
      size_t p = 0; // Pulled out of the for below by RW2STL
      for( p=0; p<lenPartList; p++) {
   
         // Get demands for part
         int lenDemList;
         char **demList;
         witGetPartDemands( witRunVeneer().constWitRun(),
                            partList[p], &lenDemList, &demList );

         // Loop once for each demand
         size_t d = 0; // Pulled out of the for below by RW2STL
         for( d=0; d<lenDemList; d++) {

            witSetDemandBuildAheadLimit( witRunVeneer().mutableWitRun(),
                             partList[p], demList[d], 
                             buildAhead_ ? numberOfPeriods()-1 : 0 );

            portableFree( demList[d] );
         }
         portableFree( demList );
         portableFree( partList[p] );
      }
      portableFree( partList );
   }
   
}

//-------------------------------------------------------------------------
//
// Subscribes-to methods
//
//-------------------------------------------------------------------------

// Informed object m when the implosion solution or MRP solution
// has changed.  The object added must have a member function
// updateFrom( LgFrModel * p, void * d ).
// When the implosion solution
// is changed then m.updateFrom( this, LgFrImplode() ) is invoked.
// When the MRP solution
// is changed then m.updateFrom( this, LgFrExplode() ) is invoked.
//
// See LgFrModel and LgFrModelClient.
void 
LgFrLastProblemSolved::addDependent(
   LgFrModelClient * m)
{
   LgFrSubscription::addDependent( m );
}

   // No longer inform object m when implosion or MRP solution has changed.
void 
LgFrLastProblemSolved::removeDependent(
   LgFrModelClient * m )
{
   LgFrSubscription::removeDependent( m );
}

// Update From should not be invoked because lps does not subscribe to
// any other objects.
void 
LgFrLastProblemSolved::localUpdate(
   LgFrModel *,
   void * )
{
   std::cerr <<"LgFrLastProblemSolved::updateFrom()" << std::endl
        <<"This method should not be invoked because lps does not" << std::endl
        <<"subscribe to any other objects" << std::endl;
   abort();
}


//-------------------------------------------------------------------------
//
// Destructor, assignment, and constructors.
//
//-------------------------------------------------------------------------

// Destructor
LgFrLastProblemSolved::~LgFrLastProblemSolved()
{
  // This is not really necessary: objects that still depend on this one
  // will never just never get notified again.  But it may help find bugs.
  assert (this->numberDependents() == 0);
}


// Assignment operator.
// Note that this method does not invoke any assignment operators in its
// ancestor classes.  This is not neccessarily consistent with good
// operator= semantics, but it is important for the correct operation of the
// LgFrScenario class, which assumes that the dependency list of *this
// will not be changed.
LgFrLastProblemSolved&
LgFrLastProblemSolved::operator=(
            const LgFrLastProblemSolved& rhs)
{
  // See Scott Meyers, "Effective C++", item 16
  if (this!=&rhs)
     { 
        // assign to all data members
        witRunVeneer_   = rhs.witRunVeneer_;
        optImploded_      = rhs.optImploded_;
        buildAhead_       = rhs.buildAhead_;
     }
  return *this;
}


// Default Copy constructor
LgFrLastProblemSolved::LgFrLastProblemSolved(
            const LgFrLastProblemSolved& source)
 : witRunVeneer_(source.witRunVeneer_),
   optImploded_(source.optImploded_),
   buildAhead_(source.buildAhead_)
{
  assert (this->numberDependents() == 0);
}

// Default Constructor
LgFrLastProblemSolved::LgFrLastProblemSolved()
 : witRunVeneer_(),
   optImploded_(false),
   buildAhead_(false)
{
  assert (this->numberDependents() == 0);
}

//-------------------------------------------------------------------------
//
// Private member functions
//
//-------------------------------------------------------------------------

// Get a float time vector part attribute
LgFrTimeVecFloat 
LgFrLastProblemSolved::getPartFloatVecAttribute(
        const LgFrPart * const part,
        apiFuncgetPartFloatVecAttribute apiFunc)
const
{
   int nP = numberOfPeriods();
   float * cVec;

   apiFunc( witRunVeneer().constWitRun(), 
            part->name().c_str(),
            &cVec );

   LgFrTimeVecFloat fVec(nP,cVec);
   portableFree(cVec);

   return fVec;
}

// Set a float time vector part attribute
void
LgFrLastProblemSolved::setPartFloatVecAttribute(
        const LgFrPart * const part,
	const LgFrTimeVecFloat& timeVec,
        apiFuncsetPartFloatVecAttribute apiFunc
        )
{
   int nP = numberOfPeriods();
   assert( nP == timeVec.length() );

   apiFunc( witRunVeneer().mutableWitRun(), 
            part->name().c_str(),
            timeVec.data() );
}


// Set a bound set part attribute
void
LgFrLastProblemSolved::setPartBoundsAttribute(
   const LgFrPart * const part,
   const LgFrTimeVecFloat* const hardLowerBound,
   const LgFrTimeVecFloat* const softLowerBound,
   const LgFrTimeVecFloat* const hardUpperBound,
   apiFuncsetPartBoundsAttribute apiFunc)
{

   if (hardLowerBound != 0) 
      assert( numberOfPeriods() == hardLowerBound->length() );

   if (softLowerBound != 0) {
      assert( numberOfPeriods() == softLowerBound->length() );
      if ( hardLowerBound != 0 ) 
         assert( *hardLowerBound <= *softLowerBound );
   }
   
   if (hardUpperBound != 0) {
      assert( numberOfPeriods() == hardUpperBound->length() );
      if ( hardLowerBound != 0 ) 
         assert( *hardLowerBound <= *hardUpperBound );
      if ( softLowerBound != 0 ) 
         assert( *softLowerBound <= *hardUpperBound );
   }

   apiFunc( 
            witRunVeneer().mutableWitRun(), 
            part->name().c_str(),
            hardLowerBound == 0 ? 0 : hardLowerBound->data(),
            softLowerBound == 0 ? 0 : softLowerBound->data(),
            hardUpperBound == 0 ? 0 : hardUpperBound->data() );
}


// Get a float time vector demand attribute
LgFrTimeVecFloat
LgFrLastProblemSolved::getDemandFloatVecAttribute(
        const LgFrDemand * const demand,
        apiFuncgetDemandFloatVecAttribute apiFunc )
const
{
   int nP = numberOfPeriods();
   float * cVec;

   apiFunc( witRunVeneer().constWitRun(), 
            demand->partPtr()->name().c_str(),
            demand->name().c_str(),
            &cVec );

   LgFrTimeVecFloat fVec (nP,cVec);
   portableFree(cVec);

   return fVec;
}

// Get a soft lower bound demand attribute
LgFrTimeVecFloat
LgFrLastProblemSolved::getDemandSoftLowerBoundAttribute(
        const LgFrDemand * const demand,
        apiFuncgetDemandSoftLowerBoundAttribute apiFunc)
const
{
   int nP = numberOfPeriods();
   float * hlb;
   float * slb;
   float * hub;

   apiFunc( witRunVeneer().constWitRun(), 
            demand->partPtr()->name().c_str(),
            demand->name().c_str(),
            &hlb, &slb, &hub );

   LgFrTimeVecFloat fVec (nP,slb);
   
   portableFree(hlb);
   portableFree(slb);
   portableFree(hub);

   return fVec;
}


// Set a bound set demand attribute
void
LgFrLastProblemSolved::setDemandBoundsAttribute(
   const LgFrDemand * const demand,
   const LgFrTimeVecFloat* const hardLowerBound,
   const LgFrTimeVecFloat* const softLowerBound,
   const LgFrTimeVecFloat* const hardUpperBound,
   apiFuncsetDemandBoundsAttribute apiFunc)
{

   if (hardLowerBound != 0) 
      assert( numberOfPeriods() == hardLowerBound->length() );

   if (softLowerBound != 0) {
      assert( numberOfPeriods() == softLowerBound->length() );
      if ( hardLowerBound != 0 ) 
         assert( *hardLowerBound <= *softLowerBound );
   }
   
   if (hardUpperBound != 0) {
      assert( numberOfPeriods() == hardUpperBound->length() );
      if ( hardLowerBound != 0 ) 
         assert( *hardLowerBound <= *hardUpperBound );
      if ( softLowerBound != 0 ) 
         assert( *softLowerBound <= *hardUpperBound );
   }

   apiFunc( 
            witRunVeneer().mutableWitRun(),
            demand->partPtr()->name().c_str(),
            demand->name().c_str(),
            hardLowerBound == 0 ? 0 : hardLowerBound->data(),
            softLowerBound == 0 ? 0 : softLowerBound->data(),
            hardUpperBound == 0 ? 0 : hardUpperBound->data() );
}

// Invoke witWrite*Sched functions
bool
LgFrLastProblemSolved::writeSchedule(
   apiFuncwriteSchedule apiFunc,
   const std::string& fileName,
   const std::string& fopenAccessMode,
   const LgFrSchedulePrintStyle  & ot )
{

  // Make sure printing of schedule messages is turned on
  printMessage( 154, true );      // Production schedule
  printMessage( 253, true );
  printMessage( 153, true );      // Shipment schedule
  printMessage( 286, true );      // Requirement schedule

  // Set fopen access mode
  witSetMesgFileAccessMode( 
     witRunVeneer().mutableWitRun(),
     WitTRUE,
     fopenAccessMode.c_str() );

  // Set WIT to return to caller after severe error
  witSetMesgStopRunning(
     witRunVeneer().mutableWitRun(),
     WitTRUE,
     WitSEVERE_MESSAGES,
     WitFALSE );

  // Write file
  witReturnCode rc = apiFunc( 
     witRunVeneer().mutableWitRun(), 
     fileName.c_str(), 
     (ot == CSV  ?  WitCSV  :  WitBSV)   );

  // Restore MesgStopRunning flag
  witSetMesgStopRunning(
     witRunVeneer().mutableWitRun(),
     WitTRUE,
     WitSEVERE_MESSAGES,
     WitTRUE );

  if ( rc > WitWARNING_RC ) return false;
  return true;
     
}

//-------------------------------------------------------------------------
//
// Unit Test
//
//-------------------------------------------------------------------------

#ifdef NDEBUG
#undef NDEBUG
#endif
// Test this class
void
LgFrLastProblemSolved::test()
{
  // Test default constructors constructors
  LgFrLastProblemSolved defInst;
  const LgFrLastProblemSolved constDefInst;
}

//
void
LgFrLastProblemSolved::contextTest(
   LgFrWorkingImplosionProblem &wip,
   LgFrLastProblemSolved       &lps,
   LgFrSetOfParts              &sop,
   LgFrSetOfDemands            &sod)
{
  // The data for this test should have come from
  // "/u/implode/wit/data/brenda/denise/wit.data"

  // char*  testFileName = "/u/implode/wit/data/brenda/denise/wit.data";
  const float fltEps = .0001f;
  int i;

  // Get objects from scenario
  LgFrLastProblemSolved & inst = lps;
  LgFrSetOfParts &sp = sop;
  LgFrSetOfDemands &sd = sod;

  // Test the assignment method.
  LgFrLastProblemSolved assignedLps = inst;
  assert ( assignedLps.optImploded() == inst.optImploded() );

  LgFrLastProblemSolved copyInst (inst);
  assert ( copyInst.optImploded() == inst.optImploded() );

  // Test parts method
  int numParts, numDemands, demandCnt=0;
  char **demandList;
  LgFrStringVector partList = inst.parts();
  numParts = partList.size();  
  assert( numParts == 153 );
  assert( std::string(partList[0]) == std::string("0000011F0598") );
  assert( std::string(partList[numParts-1]) == std::string("0000093X0774") );
  // LgFrPart parts[200];
  // LgFrDemand demands[200];;
  for ( i=0; i<numParts; i++ )
  {
     // parts[i] = sp.newPart( partList[i] );

     // Get any demands associated with this part and 
     // add to set of demands
     inst.demands( &(sp[i]), &numDemands, &demandList );
     int j = 0; // Pulled out of the for below by RW2STL
     for ( j=0; j<numDemands; j++ ) {
        // sd.insert( LgFrDemand( demandList[j], demandCnt, &(sp[i]) ) );
        // demands[demandCnt] = sd.newDemand( demandList[j], &(parts[i]) );
        demandCnt++;
        portableFree(demandList[j]);
     }
     portableFree(demandList);
  }

  assert( demandCnt == 190 );
  
  //Invoke methods
  inst.numberOfPeriods();

  // BOM testing.  For part 0000093X0774, only child is 0000021F4863.
  {	
    std::string parentName("0000093X0774");
    const LgFrPart * p = sp.findValue( parentName );
    assert (  inst.nBomEntries(p)	== 1);
    char * child = inst.bomEntryConsumedPart ( *p, 0 );
    assert (  std::string (child) == std::string ("0000021F4863")  );
    free (child);
  }

  inst.computeCriticalList(false);
  assert( !inst.computeCriticalList() );
  inst.computeCriticalList(true);
  assert( inst.computeCriticalList() );
  inst.equitability(10);
  inst.weightBounds(45.6f);
  assert( !inst.optImploded() );
  assert( !inst.imploded() );

  inst.heurImplode();
  assert( !inst.optImploded() );
  assert( inst.imploded() );
#ifdef OPT_IMPLODE
  inst.mrp(wip);	// Executing this resets the critical list
  inst.optImplode();

  assert( inst.optImploded() );
  assert( inst.imploded() );
  inst.objValue();
  inst.boundsObjValue();
#endif


  // Get critical list
  {
  int lenCritList;
  char ** critPartList;
  int * critPeriod;
  inst.criticalList( 
    & lenCritList,
    & critPartList,
    & critPeriod);
#ifdef OPT_IMPLODE
  assert( critPartList[0] == std::string("0000055F7320") );
  assert( critPeriod[0] == 0 );
#else
  assert( lenCritList > 0 );
  assert( critPartList[0] == std::string("0000055F9806") );
  assert( critPeriod[0] == 1 );
#endif
   int i = 0; // Pulled out of the for below by RW2STL
   for ( i=0; i<lenCritList; i++ ) portableFree( critPartList[i] );
   portableFree( critPartList );
   portableFree( critPeriod );
   }


  // Test update
  // Change supply of first 10 parts in WIP and then update LPS
  // Loop once for each changed part - optimize so you don't
  //  always update everythings when wip changes.
  int spEntries = sp.size();
  wip.beginSeriesOfChanges();
  for ( i=0; i< spEntries - 1; i++ )
  {
     wip.timeVec( sp[i], 
                  LgFrTimeVecFloat(inst.numberOfPeriods(),float(i)) ); 
     if ( i>=10 ) break;
  }
  int entries = (spEntries > 10) ? 10 : spEntries;
  wip.endSeriesOfChanges();
  wip.timeVec( sp[entries],  
               LgFrTimeVecFloat(inst.numberOfPeriods(),float(entries) ) );  
  inst.update(wip);
  inst.heurImplode();
 
  // Test implode method
  inst.implode(wip);

  LgFrTimeVecFloat inputFlVec(inst.numberOfPeriods());
  LgFrTimeVecFloat outputFlVec(inst.numberOfPeriods());
  LgFrTimeVecFloat oneFlVec(inst.numberOfPeriods(),1.0);
  LgFrTimeVecFloat twoFlVec(inst.numberOfPeriods(),2.0);
  inputFlVec = 3.1414926f;

  // Test supply volume -- change supply in LPS only
  LgFrPart part0 ( sp[0] );
  LgFrPart part25( sp[25] );
  inst.supplyVolume(&part0, inputFlVec);
  outputFlVec = inst.supplyVolume(&part0);
  assert ( inputFlVec == outputFlVec );
 
  // test cycleTime
  const LgFrPart * partCTptr = sp.findValue("0000055F5966");
  outputFlVec = inst.cycleTime(partCTptr);
  assert (outputFlVec[0] -10.0 < fltEps);

  // Test requirementVolume
  outputFlVec = inst.requirementVolume(&part0);
  for ( i=0; i<outputFlVec.length(); i++ )  
     assert ( outputFlVec[i] >= 0.0 );

  // Test productionVolume
  outputFlVec = inst.productionVolume(&part25);
  for ( i=1; i<outputFlVec.length(); i++ )  
     assert ( outputFlVec[i] > 0.0 );


  // Test prodBounds
  outputFlVec[0] = 2.;
  inst.productionBounds(&part25,0,0,0);
  inst.productionBounds(&part25,0,0,&outputFlVec);
  inst.productionBounds(&part25,&oneFlVec,&twoFlVec,0);
  inst.productionBounds(&part25,&oneFlVec,&twoFlVec,&outputFlVec);

  // Test demandVolume -- change demand in LPS only
  LgFrDemand demand0 ( sd[0] );
  inst.demandVolume(&demand0, inputFlVec);
  outputFlVec = inst.demandVolume(&sd[0]);
  assert ( inputFlVec == outputFlVec );

  // Test shipmentVolume
  outputFlVec = inst.shipmentVolume(&sd[0]);
  for ( i=0; i<outputFlVec.length(); i++ )  
     assert ( outputFlVec[i] >= 0.0 );


  // Test priority
  LgFrTimeVecFloat prio(inst.numberOfPeriods());
  for ( i=0; i<prio.length(); i++ )  prio[i] =(float)(i*2 - 1);
  inst.priority(&sd[0],prio);
  outputFlVec = inst.priority(&sd[0]);
  assert( prio == outputFlVec );

  // Test buildAheadLimit
  inst.buildAheadLimit(&sd[0],inst.numberOfPeriods()-1);
  assert( !inst.buildAhead() );
  inst.buildAhead( true );
  assert( inst.buildAhead() );

  // Test shipLateLimit
  inst.shipLateLimit(&sd[0],0);

  // Test cumShipBounds
  {
    LgFrTimeVecFloat csSLB = inst.cumShipSoftLowerBound( &sd[0] );
    LgFrTimeVecFloat newCsSLB = csSLB + 49;
    inst.cumShipBounds( &sd[0], 0, &newCsSLB, 0 );
    assert ( inst.cumShipSoftLowerBound( &sd[0] ) == newCsSLB );
    inst.cumShipBounds( &sd[0], 0, &csSLB, 0 );
    assert ( inst.cumShipSoftLowerBound( &sd[0] ) == csSLB );
  }
    


  // Test setting of demands focus horizon
  {
    int numPer = inst.numberOfPeriods();
    for ( i=0; i<sd.size(); i++ )
    {
      // Set every other demand
      if ( i%2 == 0 ) continue;
      // Set horizon period to mod(i,numPer)
      inst.focusHorizon( &sd[i], i%numPer );
    }
  }
  // Make sure focus Horizon is set.
  {
    int numPer = inst.numberOfPeriods();
    for ( i=0; i<sd.size(); i++ )
    {
      // Set every other demand
      if ( i%2 == 0 ) continue;
      // Get focus horizon period
      assert( inst.focusHorizon( &sd[i] ) == i%numPer );
    }
  }


  // Test focusShortageVolume()
  {
    int numPer = inst.numberOfPeriods();
    int lenList;
    char **partList;
    float **shortVol;
    inst.heurImplode();
    // Make sure focus Horizon is still set.
    {
      int numPer = inst.numberOfPeriods();
      for ( i=0; i<sd.size(); i++ )
      {
        // Set every other demand
        if ( i%2 == 0 ) continue;
        // Get focus horizon period
        assert( inst.focusHorizon( &sd[i] ) == i%numPer );
      }
    }


    inst.focusShortageVolume( &lenList, &partList, &shortVol );

    assert ( lenList>=2 );
    assert ( shortVol[0][2] > 0.0 );
    assert ( shortVol[0][3] > 0.0 );

    assert ( shortVol[1][1] > 0.0 );
    assert ( shortVol[1][2] > 0.0 );
    assert ( shortVol[1][3] > 0.0 );
    assert ( shortVol[1][4] > 0.0 );
   
    int i = 0; // Pulled out of the for below by RW2STL
    for ( i=0; i<lenList; i++ )
    {
       portableFree(partList[i]);
       portableFree(shortVol[i]);
    }
    portableFree(partList);
    portableFree(shortVol);
  }

  // Test shortageVolume()
  {
    LgFrTimeVecFloat shortVol0 = inst.shortageVolume( &part0 );
    assert ( shortVol0[2] > 0.0 );
    assert ( shortVol0[3] > 0.0 );
    LgFrTimeVecFloat shortVol5 = inst.shortageVolume( &sp[5] );
    assert ( shortVol5[1] > 0.0 );
    assert ( shortVol5[2] > 0.0 );
    assert ( shortVol5[3] > 0.0 );
    assert ( shortVol5[4] > 0.0 );

    // Test fssProductionVolume()
  {
    LgFrTimeVecFloat prodVol25 = inst.fssProductionVolume( &part25 );
    //cout << "\nPrint out the production volume for part 25" <<  std::endl;
    //cout << "Part 25" << ": " << part25.name() << prodVol25.format() <<  std::endl;

    assert ( prodVol25[0] ==  0.0 );
    assert ( prodVol25[1] == 47.0 );
    assert ( prodVol25[2] == 34.0 );
    assert ( prodVol25[3] == 28.0 );
    assert ( prodVol25[4] == 32.0 );

  }
  
  // Test getting of demand's focus horizon
  {
    int numPer = inst.numberOfPeriods();
    for ( i=0; i<sd.size(); i++ )
    {
      // Set every other demand
      if ( i%2 == 0 ) continue;
      // Set horizon period to mod(i,numPer)
      assert( (i%numPer) == (inst.focusHorizon( &sd[i] )) );
    }
  }

  // Test useFocusHorizons methods
  {
    assert( inst.useFocusHorizons() );
    inst.useFocusHorizons( false );
    assert( !inst.useFocusHorizons() );
    inst.useFocusHorizons( true );
    assert( inst.useFocusHorizons() );
  }

  // Test fssShipVol methods
  {
    LgFrTimeVecFloat demandVol2 = inst.demandVolume( &sd[2] );
    LgFrTimeVecFloat shipVol2 = inst.shipmentVolume( &sd[2] );
    assert( inst.fssShipVolume( &sd[2] ) == shipVol2 );
    inst.fssShipVolume( &sd[2], demandVol2+shipVol2+3000. );
    assert( inst.fssShipVolume( &sd[2] ) == demandVol2+shipVol2+3000. );
  }
  }


  // Test writing wit data file
  inst.update(wip);
  // Create a temporary file name
  std::string tmpFileName( LgFrTempFileName() );
  inst.writeWitDataFile(tmpFileName,wip);

  // Get supply of a part
  size_t lastPart( sp.size()-1 );
  LgFrPart aPart( sp[lastPart] );
  LgFrTimeVecFloat aPartSupply( wip.timeVec( aPart ) );
  // Increase part supply
  wip.timeVec( aPart, aPartSupply + 3.4f );
  // write wit data file
  inst.writeWitDataFile(tmpFileName,wip,"w");
  // Make sure part supply in inst is unchanged
  LgFrTimeVecFloat origPartSupply( inst.supplyVolume( &aPart ) );
  assert( aPartSupply == origPartSupply );


  // Make sure part supply in wit.data file is correct 
  {
  WitRun * tmpWitRun;
  witNewRun( &tmpWitRun );
  witSetMesgTimesPrint( tmpWitRun, WitTRUE, 
                        WitINFORMATIONAL_MESSAGES, 0 );
  witInitialize( tmpWitRun );

  witSetMesgTimesPrint( tmpWitRun, WitTRUE, 538, 0 );
  witSetWit34Compatible( tmpWitRun, WitTRUE );

  witReadData( tmpWitRun, tmpFileName.c_str() );
  float * changedPartSupply;
  witGetPartSupplyVol( tmpWitRun, aPart.name().c_str(), &changedPartSupply );
  int nPeriods;
  witGetNPeriods( tmpWitRun, &nPeriods );
  LgFrTimeVecFloat chgPartSupply( nPeriods, changedPartSupply );
  portableFree( changedPartSupply );
  assert( aPartSupply + 3.4f == chgPartSupply );
  witDeleteRun( tmpWitRun );
  }
  assert (unlink (tmpFileName.c_str()) == 0);

  // Test writing production schedule
  std::string prodFile = LgFrTempFileName();

  inst.implode(wip);
  assert( inst.writeProductionSchedule( prodFile, "w" ) );
  // Test writing to file with invalid file name
  inst.printMessage( 75, false );
#if !defined(_MSC_VER)
  // MSVC++ runtime library fails to return when fopen is called with
  // a filename of "".  So this test is skipped when in the MSVC++ environment.

  // cygwinGcc is also failing on this test, so it is being removed.
  //assert( !inst.writeProductionSchedule( std::string() ) );
#endif
  inst.printMessage( 75, true );
  // Test appending to an existing file
  assert( inst.writeProductionSchedule( prodFile ) );
  assert (unlink (prodFile.c_str()) == 0);

  // Test writing shipment and requirement schedule
  std::string outFile = LgFrTempFileName();
  assert( inst.writeShipmentSchedule( outFile, "w" ) );
  inst.mrp(wip);
  assert( inst.writeRequirementSchedule( outFile, "a" ) );
  assert (unlink (outFile.c_str()) == 0);

  // Test backlogVolume
  {
  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<size_tMin(20,sd.size()); i++ ) {
     assert ( inst.backlogVolume( &sd[i] ) ==
              ( inst.demandVolume(&sd[i]).scan(LgFrAdd) - 
                inst.shipmentVolume(&sd[i]).scan(LgFrAdd) )  ); 
     }
  }

  // Test lot size increment
  {
      LgFrTimeVecFloat inc = inst.incLotSize( &aPart );
      LgFrTimeVecFloat newInc = inc + 5.0;
      inst.incLotSize ( &aPart, newInc );
      LgFrTimeVecFloat resultInc = inst.incLotSize( &aPart );
      assert (resultInc == newInc);
  }

  // Test minimum lot size
  {
      LgFrTimeVecFloat size = inst.minLotSize( &aPart );
      LgFrTimeVecFloat newSize = size + 5.0;
      inst.minLotSize ( &aPart, newSize );
      LgFrTimeVecFloat resultSize = inst.minLotSize( &aPart );
      assert (resultSize == newSize);
  }
  
  // Test increment lot size
  {
      LgFrTimeVecFloat val = inst.incLotSize( &aPart );
      LgFrTimeVecFloat newVal = val + 4.0;
      inst.incLotSize( &aPart, newVal );
      LgFrTimeVecFloat resultVal = inst.incLotSize( &aPart );
      assert (resultVal == newVal);
  }

  // Test Yield
  {
      LgFrTimeVecFloat val = inst.yield( &aPart );
      LgFrTimeVecFloat newVal(val);
      newVal[0] = 89.;
      newVal[1] = 91.;
      LgFrTimeVecFloat newValDithered(newVal);
      newValDithered[0] = newValDithered[0] - .3f;
      newValDithered[1] = newValDithered[1] + .3f;
      inst.yield ( &aPart, newValDithered );
      LgFrTimeVecFloat resultVal = inst.yield( &aPart );
      assert (resultVal == newVal);
  }

}
