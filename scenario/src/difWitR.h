#ifndef LgFrDataInterfaceImplementationFromWitRun_h
#define LgFrDataInterfaceImplementationFromWitRun_h
// RW2STL -- inserted:
#include <string>
// End of RW2STL -- inserted includes.

// This file contains the class: 
// LgFrDataInterfaceImplementationFromWitRun

#include <wit/src/wit.h>

#include <scenario/src/dif.h>
#include <scenario/src/difImpl.h>

//---------------------------------------------------------------------------------------
//
// Forward references
//
//---------------------------------------------------------------------------------------
class LgFrWitRunVeneer;
class LgFrDataInterfaceEssentials;           

//---------------------------------------------------------------------------------------
//
// All gobal functions declarations
//
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// To register all global functions for this class
//---------------------------------------------------------------------------------------
void
registerWitRunDifImpFunctions(LgFrDataInterfaceImplementation *dImpWR);

//---------------------------------------------------------------------------------------
// To create a LgFrDataInterfaceImplementationFromWitRun object
//---------------------------------------------------------------------------------------
LgFrDataInterfaceImplementation * 
createWitRunDifImp(LgFrWitRunVeneer * witRunVeneerPtr);

//---------------------------------------------------------------------------------------
//
// Global functions to get "parts"
//
//---------------------------------------------------------------------------------------
// Return: a LgFrPartVector object (vector of parts in general)
// used by difWitRunGetRawPart, difWitRunGetProductParts, difWitRunGetCapacityParts to get different
// kinds of parts from WIT.
//---------------------------------------------------------------------------------------
void *
difWitRunGetParts(
    const LgFrDataInterfaceImplementation *dImp, 
    const LgFrDataInterfaceEssentials *difEssenPtr,
    witAttr ma, 
    LgFrError& status);

//---------------------------------------------------------------------------------------
// difWitRunGetRawPart global function (registering in the attribute table).
// Get raw parts from WIT and insert them into set_of_part
// Return: a LgFrPartVector object (vector of raw parts).
//---------------------------------------------------------------------------------------
void *
difWitRunGetRawParts(
    const LgFrDataInterfaceImplementation * dImp,
    const LgFrItemPtrVector* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------------------------------------
// difWitRunProductPart global function (registering in the attribute table).
// Get product parts from WIT and insert it into set_of_part
// Return: a LgFrPartVector object (vector of producible parts)
//---------------------------------------------------------------------------------------
void*
difWitRunGetProductParts(
    const LgFrDataInterfaceImplementation * dImp,
    const LgFrItemPtrVector* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------------------------------------
// difWitRunGetCapacityParts global function (registering in the attribute table).
// Get capacity parts from WIT and insert it into set_of_part
// Return: a LgFrPartVector object (vector of capacities).
//---------------------------------------------------------------------------------------
void*
difWitRunGetCapacityParts(
    const LgFrDataInterfaceImplementation * dImp,
    const LgFrItemPtrVector* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0); 

//---------------------------------------------------------------------------------------
//
// Global functions to get part attributes.
//
//---------------------------------------------------------------------------------------
// difWitRunGetPartUnitCost global function (registered in the attribute table)
// it returns a LgFrFloatVector object.
//---------------------------------------------------------------------------------------
void *
difWitRunGetPartUnitCost(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrItemPtrVector* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------------------------------------
// difWitRunGetPartProcCost global function (registered in the attribute table).
// It returns a LgFrFloatVector object.
//---------------------------------------------------------------------------------------
void *
difWitRunGetPartProcCost(
    const LgFrDataInterfaceImplementation * difImp,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------------------------------------
// difWitRunGetPartConstrained global function (registered in the attribute table).
// It returns a LgFrIntVector object.
//---------------------------------------------------------------------------------------
void *
difWitRunGetPartConstrained(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);               

//---------------------------------------------------------------------------------------
// difWitRunGetPartProcLeadTime global function (registered in the attribute table).
// It returns a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------------------
void *
difWitRunGetPartProcLeadTime(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------------------------------------
// Global function to get some part attributes, such as,
// Obj1ScrapCost, Obj1StockCost, and SupplyVol
// (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------------------
extern "C" {typedef witReturnCode (STDCALL * apiFuncGetPartAttrTimeVecFloat) 
          (WitRun * const, const char * const, float * * );}

extern "C" {typedef witReturnCode (STDCALL * apiFuncGetDemandAttrTimeVecFloat) 
       (WitRun * const, const char * const, const char * const, float * *);}

extern "C" {typedef witReturnCode (STDCALL * apiFuncGetDemandAttrTimeVecInt)
       (WitRun * const, const char * const, const char * const, int * *);}

void *
difWitRunGetDemandAttrTimeVecInt(
    const LgFrDataInterfaceImplementation * dImp,
      const LgFrItemPtrVector* itemPtrOrdVecPtr,
      apiFuncGetDemandAttrTimeVecInt apiFunc,
             LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

void *
difWitRunGetPartAttrTimeVecFloat(
    const LgFrDataInterfaceImplementation * dImp,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    apiFuncGetPartAttrTimeVecFloat apiFunc,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------------------------------------
// difWitRunGetPartObj1ScrapCost global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------------------
void *
difWitRunGetPartObj1ScrapCost(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------------------------------------
// difWitRunGetPartObj1StockCost global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------------------
void *
difWitRunGetPartObj1StockCost(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------------------------------------
// difWitRunGetPartsupplyVol global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------------------
void *
difWitRunGetPartSupplyVol(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//------------------------------------------------------------------------------
// difWitRunGetProductYield global function (registered in the attribute table).
// Return: a LgFrIntVector object
//------------------------------------------------------------------------------
void *
difWitRunGetProductYield(
    const LgFrDataInterfaceImplementation * dImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//------------------------------------------------------------------------------
// difWitRunGetProductYield global function (registered in the attribute table).
// Return: a LgFrTimeVectorFloat object
//------------------------------------------------------------------------------
void *
difWitRunGetProductYieldFloat(
    const LgFrDataInterfaceImplementation * dImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------------------------------
// difWitRunGetProductCycleTime global function
// (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------------
void *
difWitRunGetProductCycleTime(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------------------------------------
// difWitRunGetProductObj1ProdCost global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------------------
void *
difWitRunGetProductObj1ProdCost(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------------------------------
// difWitRunGetProductMinLotSize global function
// (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------------
void *
difWitRunGetProductMinLotSize(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------------------------------
// difWitRunGetProductIncLotSize global function
// (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------------
void *
difWitRunGetProductIncLotSize(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);


//--------------------------------------------------------------------
//  
//   Global functions to obtain demands
//
//-------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// difWitRunGetCommittedDemands global function (registering in the attribute table).
// Get committed demands from WIT and insert them into set_of_demand
// Return: a LgFrDemandVector object (vector of committed demands).
//---------------------------------------------------------------------------------------
void *
difWitRunGetCommittedDemands(
  const LgFrDataInterfaceImplementation * dImp,
  const LgFrItemPtrVector* itemPtrOrdVecPtr,
  LgFrError & status,
  const LgFrDataInterfaceEssentials * difEssenPtr = 0); 

//---------------------------------------------------------------------------------------
// difWitRunGetNewOpportunityDemands global function (registering in the attribute table).
// Get new opportunity demands from WIT and insert them into set_of_demand
// Return: a LgFrDemandVector object (vector of new opportunity demands).
//---------------------------------------------------------------------------------------
void *
difWitRunGetNewOpportunityDemands(
    const LgFrDataInterfaceImplementation * dImp,
    const LgFrItemPtrVector* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0); 

//---------------------------------------------------------------------------------------
// difWitRunGetFirmForecastDemands global function (registering in the attribute table).
// Get firm forcast demands from WIT and insert them into set_of_demand
// Return: a LgFrDemandVector object (vector of firm forcast demands).
//---------------------------------------------------------------------------------------
void *
difWitRunGetFirmForecastDemands(
    const LgFrDataInterfaceImplementation * dImp,
    const LgFrItemPtrVector* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------------------------------------
// difWitRunGetRiskForecastDemands global function (registering in the attribute table).
// Get risk forcast demands from WIT and insert them into set_of_demand
// Return: a LgFrDemandVector object (vector of risk forcast demands).
//---------------------------------------------------------------------------------------
void *
difWitRunGetRiskForecastDemands(
    const LgFrDataInterfaceImplementation * dImp,
    const LgFrItemPtrVector* /* itemPtrOrdVecPtr, not used */,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

// ------------------------------------------------------------------------
//
// Global functions to obtain demands attributes
//
// ------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// difWitRunGetDemandGrossRev global function (registering in the attribute table).
// Compute total demand gross revenue with data from WIT.
// Return: a LgFrFloatVector object (vector of demand gross revenues).
//---------------------------------------------------------------------------------------
void *
difWitRunGetDemandGrossRev(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------------------------------------
// Global function to get some demand attributes, such as,
// demandVol, getObj1CumShipReward, getObj1ShipReward
// (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------------------
void *
difWitRunGetDemandAttrTimeVecFloat(
    const LgFrDataInterfaceImplementation * dImp,
      const LgFrItemPtrVector* itemPtrOrdVecPtr,
      apiFuncGetDemandAttrTimeVecFloat apiFunc,
             LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------------------------------------
// difWitRunGetDemandVol global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------------------
void *
difWitRunGetDemandVol(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------------------------------------
// difWitRunDemanObj1CumShipReward global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------------------
void *
difWitRunGetDemandObj1CumShipReward(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------------------------------------
// difWitRunDemanObj1ShipReward global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------------------
void *
difWitRunGetDemandObj1ShipReward(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//---------------------------------------------------------------------------------------
// difWitRunGetDemanPriority global function (registered in the attribute table).
// Return: a LgFrTimeVecFloatVector object.
//
// Note that priority should be a TimeVecInt, and not  TimeVecFloat.
// should return LgFrVectorTimeVecInt jmt 7/21/95, need to implement TimeVectInt class.
// Now, simply returns a LgFrTimeVecFloatVector object.
//---------------------------------------------------------------------------------------
void *
difWitRunGetDemandPriority(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

//--------------------------------------------------------------------
//  
//   Global function to define BOM
//
//-------------------------------------------------------------------
void*
difWitRunDefineBom(
    const LgFrDataInterfaceImplementation * difImp, 
    const LgFrItemPtrVector*, //  itemPtrOrdVecPtr,
    LgFrError &, // status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0);

// end of global functions declarations
 
//---------------------------------------------------------------------
//   Data interface from Wit Run
//
//   LgFrDataInterfaceImplementationFromWitRun class
//
//---------------------------------------------------------------------

class LgFrDataInterfaceImplementationFromWitRun 
: public LgFrDataInterfaceImplementation {

    virtual int GetDIIType() const { return __LGFRDATAINTERFACEIMPLEMENTATIONFROMWITRUN; }
//private:
protected:  
  static const std::string defaultRiskForecast;
  static const std::string defaultFirmForecast;
  static const std::string defaultNewOps;
  static const std::string defaultCommitted;

public:

  //-------------------------------------------------------------------
  //  
  //   Methods to obtain global attributes
  //
  //-------------------------------------------------------------------

  // Return problem title
  // This should be the name of the underlying dataset.
  virtual
    std::string
      title(const LgFrDataInterfaceEssentials * = 0 )
        const;

  // Make a problem title
  // This should be the name of the underlying dataset.
  virtual
    std::string
      makeTitle(const LgFrDataInterfaceEssentials * = 0 )
        const;

  //--------------------------------------------------------------------
  //  
  //   Methods to query demands
  //
  //-------------------------------------------------------------------

  // return true if demand name matches compareExpression
  virtual
    bool isADemand(
      std::string demandedPartName,
      std::string demandName,
      std::string compareExprssion) const;                      

  virtual
    bool isANewOpDemand(
      std::string demandedPartName,
      std::string demandName) const;

  virtual
    bool isAFirmForecastDemand(
      std::string demandedPartName,
      std::string demandName) const;

 virtual 
   bool isARiskForecastDemand(
     std::string demandedPartName,
     std::string demandName) const;

  virtual
    bool isACommittedDemand(
      std::string demandedPartName,
      std::string demandName) const;

  //--------------------------------------------------------------------
  //
  //   Methods to manage resources
  //
  //-------------------------------------------------------------------

  // Free up extra resources
  virtual
    void
      deleteExtraResources();


  //--------------------------------------------------------------------
  //  
  //   Standard class methods
  //
  //-------------------------------------------------------------------

  // Make clone copy of this object
  virtual
    LgFrDataInterfaceImplementation *
      clone()
        const;

  // Assignment operator.
  LgFrDataInterfaceImplementationFromWitRun&
    operator=(
      const LgFrDataInterfaceImplementationFromWitRun& rhs);

  // Copy constructor
  LgFrDataInterfaceImplementationFromWitRun(
    const LgFrDataInterfaceImplementationFromWitRun& source);

  // Preferred Constructor
  LgFrDataInterfaceImplementationFromWitRun(
     WitRun * witRun,
     const std::string & riskForecastDemandNamePattern = defaultRiskForecast,
     const std::string & firmForecastDemandNamePattern = defaultFirmForecast,
     const std::string & newOpDemandNamePattern        = defaultNewOps);
  
  // Destructor
  virtual
     ~LgFrDataInterfaceImplementationFromWitRun();

  // Tests this class.  Only returns if all tests passed
  static
    void
      test ();

  // Context test
  static
    void
      contextTest
        ( LgFrDataInterface        * tdif1Ptr,        // testScenario1's
          LgFrLastProblemSolved    & lps,  	      // testScenario1's
          LgFrDataInterface        * ts1dupDifPtr,    // testScenario1dup's
          LgFrDataInterface        * tdif2Ptr,        // testScenario2's
          const LgFrScenario       * scen);	      // testScenario1's

// protected:

  virtual
    /* const */ WitRun *
      constWitRun(const LgFrDataInterfaceEssentials* = 0) 
        const;

protected:

  virtual
    WitRun *
      mutableWitRun(const LgFrDataInterfaceEssentials* = 0);

  // Returns true if witRunVeneer exists.
  // Returns false if witRun needs to be populated with data.
  virtual
    bool
      validWitRun()
        const;

  // Free the memory resources associated with the witRun
  virtual
    void
      deleteWitRun();

  // Default Constructor
  LgFrDataInterfaceImplementationFromWitRun();

  std::string riskForecastRE_;
  std::string firmForecastRE_;
  std::string newOpsRE_;

private:

  LgFrWitRunVeneer * witRunVeneerPtr_;


  // Set problem title (not allowed since the title is the dataset name and can not be set
  // arbitrarily).
  virtual
    void
      title(std::string);

};


// demand in general
// Return: a LgFrDemandVector object (vector of demands in general)
// used by commited, newOps, firmForecast, riskForecast to get different
// kinds of demands from WIT.
void*
  difWitRunGetDemands(
    const LgFrDataInterfaceImplementation * dImp,
    LgFrDataInterfaceEssentials * difEssenPtr,
    bool
     ( LgFrDataInterfaceImplementationFromWitRun::*isA)
        (std::string, std::string)const,
    LgFrError & status);


#endif

