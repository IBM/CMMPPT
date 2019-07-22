#ifndef LgFrDataInterfaceImplementationFromSceFiles_h
#define LgFrDataInterfaceImplementationFromSceFiles_h

// RW2STL - inserted:
#include <string>
// End of RW2STL - inserted includes.


// This file contains the class: 
//    LgFrDataInterfaceFromSceFiles
//
// The following methods cause an abort when invoked:
//      rawParts()
//      productPart()
//      capacityParts()
//      partAttribute()
//      committedDemands();
//      newOpportunityDemands();
//      firmForecastDemands();
//      riskForecastDemands();
//      demandAttribute()
//
//  This class has a method "filename" which should be used by initial 
//  problem to obtain name of sce data file.

   
  //--------------------------------------------------------------------
  //  
//      

#include <wit/src/wit.h>
#include <scenario/src/difWitR.h>
#include <scenario/src/calendar.h>




// #include <wit/src/wit.h>
#include <scenario/src/dif.h>
#include <scenario/src/difImpl.h>

#include <sce/src/scrub.h>
// Forward references

class LgFrWitRunVeneer;
class LgFrDataInterfaceEssentials;

//------------------------------------------------------------------------------
// To register all global functions for this class
//------------------------------------------------------------------------------
void
registerSceFilesDifImpFunctions(LgFrDataInterfaceImplementation *dImpSce);

//------------------------------------------------------------------------------
// To create a LgFrDataInterfaceImplementationFromCmrpFiles object
//------------------------------------------------------------------------------
LgFrDataInterfaceImplementation *
createSceFilesDifImp(const std::string filename);


#if 0
//-------------------------------------------------------------------
//
//   Global functions to obtain the defined parts and capacities.
//
//-------------------------------------------------------------------

// difSceFGetRawPart global function (registering in the attribute table).
// Get raw parts for the Perfect_World ATP and insert them into set_of_part
// Return: a LgFrPartVector object (vector of raw parts).

void *
difSceFGetRawParts(
    const LgFrDataInterfaceImplementation * dImp,
    const LgFrItemPtrVector*,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

// difSceFGetRawPart global function (registering in the attribute table).
// Get raw parts for the Perfect_World ATP and insert them into set_of_part
// Return: a LgFrPartVector object (vector of product parts).

void*
difSceFGetProductParts(
    const LgFrDataInterfaceImplementation * dImp,
    const LgFrItemPtrVector* ,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

// difSceFGetRawPart global function (registering in the attribute table).
// Get raw parts for the Perfect_World ATP and insert them into set_of_part
// Return: a LgFrPartVector object (vector of product parts).

void*
difSceFGetCapacityParts(
    const LgFrDataInterfaceImplementation * dImp,
    const LgFrItemPtrVector* ,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);
//-----------------------------------------------------------------------------
//
//  Global functions to obtain raw material attributes
//
//-----------------------------------------------------------------------------



// difSceFGetRawUnitCost global function (registered in the attribute table)
// it returns a LgFrFloatVector object.

void *
difSceFGetRawUnitCost(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrItemPtrVector* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = 0);

// difSceFGetRawProcCost global function (registered in the attribute table).
// It returns a LgFrFloatVector object.

void *
difSceFGetRawProcCost(
    const LgFrDataInterfaceImplementation * difImp,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

// difSceFGetRawConstrained global function (registered in the attribute
// table).  It returns a LgFrIntVector object.

void *
difSceFGetRawConstrained(
    const LgFrDataInterfaceImplementation * difImp,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

// difSceFGetRawProcLeadTime global function (registered in the attribute
// table).  It returns a LgFrIntVector object.

void *
difSceFGetRawProcLeadTime(
    const LgFrDataInterfaceImplementation * difImp,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

// difSceFGetRawObj1ScrapCost global function (registered in the attribute
// table).  It returns a LgFrIntVector object.

void *
difSceFGetRawObj1ScrapCost(
    const LgFrDataInterfaceImplementation * difImp,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

// difSceFGetRawObj1StockCost global function (registered in the attribute
// table).  It returns a LgFrIntVector object.

void *
difSceFGetRawObj1StockCost(
    const LgFrDataInterfaceImplementation * difImp,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

// difSceFGetRawSupplyVol global function (registered in the attribute
// table).  It returns a LgFrIntVector object.

void *
difSceFGetRawSupplyVol(
    const LgFrDataInterfaceImplementation * difImp,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//-----------------------------------------------------------------------------
//
//  Global functions to obtain producible part attributes
//
//-----------------------------------------------------------------------------
// difSceFGetProductUnitCost global function (registered in the attribute
// table).  It returns a LgFrIntVector object.

void *
difSceFGetProductUnitCost(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrItemPtrVector* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = 0);

// difSceFGetProductProcCost global function (registered in the attribute
// table).  It returns a LgFrIntVector object.

void *
difSceFGetProductProcCost(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrItemPtrVector* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = 0);
// difSceFGetProductConstrained global function (registered in the attribute
// table).  It returns a LgFrIntVector object.

void *
difSceFGetProductConstrained(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrItemPtrVector* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = 0);

// difSceFGetProducYieldt global function (registered in the attribute
// table).  It returns a LgFrIntVector object.

void *
difSceFGetProductYield(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrItemPtrVector* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = 0);

// difSceFGetProductProcLeadTime global function (registered in the attribute
// table).  It returns a LgFrIntVector object.

void *
difSceFGetProductProcLeadTime(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrItemPtrVector* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = 0);

// difSceFGetProductCycleTime global function (registered in the attribute
// table).  It returns a LgFrIntVector object.

void *
difSceFGetProductCycleTime(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrItemPtrVector* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = 0);

// difSceFGetObj1ProdCost global function (registered in the attribute
// table).  It returns a LgFrIntVector object.
void *
difSceFGetProductObj1ProdCost(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrItemPtrVector* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = 0);

// difSceFGetProductObj1ScrapCost global function (registered in the attribute

// table).  It returns a LgFrIntVector object.

void *
difSceFGetProductObj1ScrapCost(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrItemPtrVector* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = 0);

// difSceFGetProductObj1StockCost global function (registered in the attribute
// table).  It returns a LgFrIntVector object.

void *
difSceFGetProductObj1StockCost(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrItemPtrVector* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = 0);

// difSceFGetProductSupplyVol global function (registered in the attribute
// table).  It returns a LgFrIntVector object.

void *
difSceFGetProductSupplyVol(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrItemPtrVector* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = 0);

void *
difSceFGetProductMinLotSize(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrItemPtrVector* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = 0);


//-----------------------------------------------------------------------------

//
//  Global functions to obtain capacity attributes
//
//-----------------------------------------------------------------------------

// difSceFGetCapacityUnitCost global function (registered in the attribute
// table).  It returns a LgFrIntVector object.

void *
difSceFGetCapacityUnitCost(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrItemPtrVector* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = 0);

// difSceFGetCapacityConstrained global function (registered in the attribute
// table).  It returns a LgFrIntVector object.

void *
difSceFGetCapacityConstrained(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrItemPtrVector* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = 0);

// difSceFGetCapacityObj1ScrapCost global function (registered in the attribut
// table).  It returns a LgFrIntVector object.

void *
difSceFGetCapacityObj1ScrapCost(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrItemPtrVector* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = 0);

// difSceFGetCapacitySupplyVol global function (registered in the attribute
// table).  It returns a LgFrIntVector object.

void *
difSceFGetCapacitySupplyVol(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrItemPtrVector* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = 0);


//-----------------------------------------------------------------------------
//
//  Global functions  to obtain demands
//
//-----------------------------------------------------------------------------


// difSceFGetCommittedDemands global function (registering in the attribute
// table).  Get committed demands for the Perfect_World ATP and insert
// them into set_of_demand.  Return: a LgFrDemandVector object (vector
// of committed demands).

void *
difSceFGetCommittedDemands(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrItemPtrVector* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = 0);

// difSceFGetNewOpportunityDemands global function (registering in the attribu
// table).  Get committed demands for the Perfect_World ATP and insert
// them into set_of_demand.  Return: a LgFrDemandVector object (vector
// of committed demands).

void *
difSceFGetNewOpportunityDemands(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrItemPtrVector* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = 0);

// difSceFGetFirmForecastDemands global function (registering in the attribute

// table).  Get committed demands for the Perfect_World ATP and insert
// them into set_of_demand.  Return: a LgFrDemandVector object (vector
// of committed demands).

void *
difSceFGetFirmForecastDemands(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrItemPtrVector* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = 0);

// difSceFGetRiskForecastDemands global function (registering in the attribute

// table).  Get committed demands for the Perfect_World ATP and insert
// them into set_of_demand.  Return: a LgFrDemandVector object (vector
// of committed demands).

void *
difSceFGetRiskForecastDemands(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrItemPtrVector* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = 0);


//-----------------------------------------------------------------------------
//
//  Global functions to obtain demand attributes
//
//-----------------------------------------------------------------------------

// difSceFGetDemandGrossRev global function (registering in the attribute
// table).  Get committed demands for the Perfect_World ATP and insert
// them into set_of_demand.  Return: a LgFrDemandVector object (vector
// of committed demands).

void *
difSceFGetDemandGrossRev(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrItemPtrVector* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = 0);

// difSceFGetDemandVol global function (registering in the attribute
// table).  Get committed demands for the Perfect_World ATP and insert

// them into set_of_demand.  Return: a LgFrDemandVector object (vector
// of committed demands).

void *
difSceFGetDemandVol(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrItemPtrVector* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = 0);

// difSceFGetDemandObj1CumShipReward global function (registering in the attri
// table).  Get committed demands for the Perfect_World ATP and insert
// them into set_of_demand.  Return: a LgFrDemandVector object (vector
// of committed demands).

void *
difSceFGetDemandObj1CumShipReward(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrItemPtrVector* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = 0);

// difSceFGetDemandObj1ShipReward global function (registering in the attribut
// table).  Get committed demands for the Perfect_World ATP and insert
// them into set_of_demand.  Return: a LgFrDemandVector object (vector
// of committed demands).

void *
difSceFGetDemandObj1ShipReward(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrItemPtrVector* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = 0);


void *
difSceFGetDemandPriority(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrItemPtrVector* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------------------
// Define BOM
//---------------------------------------------------------------------
void*
difSceFDefineBom(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrItemPtrVector* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = 0);

#endif

//---------------------------------------------------------------------
//
//   Data interface from Wit Run
//
//---------------------------------------------------------------------

class LgFrDataInterfaceImplementationFromSceFiles :
  public LgFrDataInterfaceImplementation {

//private:

public:

  virtual int GetDIIType() const { return __LGFRDATAINTERFACEIMPLEMENTATIONFROMSCEFILES; }

  //--------------------------------------------------------------------
  //
  //   Methods for processing file in minor ways
  //
  //-------------------------------------------------------------------

  // Find out date in header
  static
    LgFrCalendar
      calendar(const std::string fileNameWithoutSuffix);


  //--------------------------------------------------------------------
  //  
  //   Dynamic type query methods
  //
 
 
  //-------------------------------------------------------------------
  virtual
  isAReturnType
  isA()
       const;

       
  
    
  //--------------------------------------------------------------------
  //  
  //   Standard class methods
  //
  //-------------------------------------------------------------------


  // Make a clone copy of this object
  virtual
    LgFrDataInterfaceImplementation *
        clone()
           const;
           
  // Preferred Constructor
  LgFrDataInterfaceImplementationFromSceFiles(
     std::string filename);

    LgFrDataInterfaceImplementationFromSceFiles(
     std::string filename, std::string scrubDir,
     bool scrubMode, bool scrubOnly); 
  
  // Destructor
  virtual
     ~LgFrDataInterfaceImplementationFromSceFiles();

  // Assignment operator.
  LgFrDataInterfaceImplementationFromSceFiles&
    operator=(
           const LgFrDataInterfaceImplementationFromSceFiles& rhs);


  // Copy constructor
  LgFrDataInterfaceImplementationFromSceFiles(
           const LgFrDataInterfaceImplementationFromSceFiles& source);


#if 0
  // Tests this class.  Only returns if all tests passed
  static
    void
      test ();

static
void
contextTest
  (
   LgFrDataInterface    	* ts1dif,	// testScenario1's
   LgFrLastProblemSolved        & lps,          // testScenario1's
   LgFrDataInterface    	* ts1dupDif,	// testScenario1dup's
   LgFrDataInterface    	* ts2dif,	// testScenario2's
   const LgFrScenario      * scenPtr);
#endif
  

//--------------
//   specific methods
//----------------

virtual
std::string filename()
const;
    
LgFrSceScrubber scrubber() const;
  
private:

  std::string  filename_;
  LgFrSceScrubber myScrubber_;
  
protected:


   // Default Constructor
  LgFrDataInterfaceImplementationFromSceFiles();
  

  
};


#endif
