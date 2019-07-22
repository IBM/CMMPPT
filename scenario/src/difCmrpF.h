#ifndef LgFrDataInterfaceImplementationFromCmrpFiles_h
#define LgFrDataInterfaceImplementationFromCmrpFiles_h
// RW2STL -- inserted:
#include <fstream>
#include <map>
#include <string>
// End of RW2STL -- inserted includes.


// This file contains the class: 
//    LgFrDataInterfaceImplementationFromCmrpFiles

/*---------------------------------------------------------------------

Cmrp File Interface Format
 
 
Overview of the Files
 

There are five required files with suffix .raw, .prd, .cap, .dem and
.bom.  They are:
 
1.   raw - This file contains the data associated with raw parts. A
     raw part is a part that cannot be built. Any quantity of a raw
     part that is not used in one period remains available in the
     next period. Since it cannot be built, quantities of a raw part
     can only be obtained as external supply.
 
2.   prd - This file contains the data associated with product parts.
     A product part is a part that can be built. Any quantity of a
     product part that is not used in one period remains available in
     the next period. Since it can be built, quantities of a product
     can be obtained by building them. In addition, quantities of a
     product part can be obtained as an external supply.
 
3.   cap - This file contains the data associated with capacities. A
     capacity represents some limitation on the quantity of one or
     more products that can be produced during one period. Any
     quantity of a capacity that is not used in one period is lost
     and is not available in the next period.
 
4.   dem - This file contains the data associated with demands. Each
     part has zero or more demands associated with it. A demand
     represents an external customer who places demand for the part.
     ("External customer" means external to the system being modeled
     by PRM. So it is possible for this external customer to be
     internal to VW). More than one demand is permitted for the same
     part, to represent the fact that some demand for a part may be
     more urgent than another demand for the same part.
 
5.  bom - This file contains the data associated with
    bill-of-manufacturing (BOM) arcs. A BOM arc is the association
    between a particular produced part and one particular consumed
    part (or capacity) in its BOM. Each entry represents the
    consumption of some volume of the consumed part (or capacity) in
    order to produce one unit of product.

There are optional files with suffix .rev, .mls, .ils, .yld, .cal.
If any of these optional files is absent then default values are used.
The optioanal files are:

1.  rev - This file contain revenue associated with shipping to a
    demand.  If this file does not exist, then default values are used.
    Revenue data is only needed for econonmic based implosion.
 
2.  mls - This file contains the minimum lot size when producing a product.
    This data is only used by the priority based implosion.
 
3.  ils - This file contains the increment lot size when producing a product.
    This data is only used by the priority based implosion.
 
4.  yld - This file contains product yield.

5.  cal - This file allows one to specify the starting date of each period.
 
 
 
General comments pertaining to all files
 
   All character data is case-sensitive. The data is read as a series
   of tokens where each token is separated by one or more blank
   spaces. This means that character data can not have embedded
   blanks.
 
 
Details of each files format
 
1.raw
 
     Record 1 Token 1 - nPeriods
            Integer>0
            Number of time periods in the planning horizon.
 
     Record 1 Token 2 - startDay
            1<=Integer<=31
            Day of month of first period.
 
     Record 1 Token 3 - startMonth
            1<=Integer<=12
            Month of year of first period.
 
     Record 1 Token 4 - startYear
            1990<=Integer<=2100
            Year of first period.
 
     Record 1 Token 5 - periodSize
            character string.  One of the following:  hour day week month
            Size of periods.  Tokens 3 through 5 are used to obtain the
            correct column headings on the graphical display.
 
     All records that follow have the same format.  There is one record
     per raw part.
 
     Token 1 - rawPartName
               character string
               Every rawPartName must be unique.
 
     Token 2 - unitCost
               float>=0.0
               This is the cost to procure the part.  
               In CMRP this value is used when computing the financial 
               impact of using implosion instead of explosion when 
               ordering parts.
 
     Token 3 - constrainedFlag
               character string.  One of the following:  constrained
               unconstrained
               This flag is used to identify if this part is constrained
               or unconstrained.  An unconstrained part is a part that is
               readily available outside of the procurement lead time.
               A constrained part is a part which is not easily obtained.
 
     Token 4 - procurementLeadTime
               float>=0.0
               This is the time it takes to procure the part.  The
               procurement time is the time from when an order is placed
               to the time that the part is available to be used in the
               manufacturing process.  For example, if it takes three
               and a half weeks from the time a part is ordered to the
               time it is available to be used and the periods are
               weekly, then the procurementLeadTime is 3.5.
 
     Tokens 5,6 ...4+nPeriods - supplyVolume
               float>=0.0
               This is a vector of tokens, one for each period.  These
               tokens represent the external supply for each time period.
               Also, if there is any initial inventory for this part it
               should be included in the volume of the initial period
               (token 5).
 
     Example:
     3 10 4 1995 day
     raw1  1.0  constrained  .5  2.0  4.0  5.0
     raw2  2.0  unconstrained  1.0  2.0  2.0  2.0
 
     This problem has 3 time periods.  The first period is April 10,
     1995.  Each period is one day long.  There are two raw products named
     raw1 and raw2.  Raw1 has a unitCost of 1.0, is constrained, has a
     procurementLeadTime of 0.5 days, and has an expected supply of 2.0
     units on April 4, 4.0 units on April 5, and 5.0 units on April 6.
 
 
 
2. prd
 
     The first record of this file is the same as the first record of the
     raw file.  All records that follow have the same format.  There is
     one record per product part.
 
     Token 1 - productPartName
               character string
               Every productPartName and rawPartName must be unique.
 
     Token 2 - unitCost
               float>=0.0
               This is the cost to produce the part including the cost
               of the parts consumed.
               In CMRP this value is used when computing the financial 
               impact of using implosion instead of explosion when 
               ordering parts.
 
     Token 3 - cycleTime
               float>=0.0
               This is the time it takes to produce the part.  The unit
               of time is the same as the units of the periods.  For
               example, if it takes 1.5 days to produce a product and
               the periods are daily, then the cycleTime is 1.5
 
     Tokens 4,5,...3+nPeriods - supply Volume
               float>=0.0
               This is a vector of tokens, one of each period.  These
               tokens represent the external supply for each time period
               which will most likely be zero because this is a produced
               part.  If there is any initial inventory for this part
               it should be included in the volume of the initial period
               (token 4).

     Example:
     3 10 4 1995 day
     prod1  1.3  .5  2.0  0.0  0.0
     sub1  2.3  0.25  2.0  0.0  0.0
 
     This file has 2 products, prod1 and sub1.  Prod1 has a unitCost of
     1.3, a cycleTime of 0.5 days, and an initial inventory of 2.0 units
     on April 4.
 
 
 
3. cap
 
     The first record of this file is the same as the first record of the
     raw file.  All records that follow have the same format.  There is
     one record per capacity.
 
     Token 1 - capacityPartName
               character string
               Every capacityPartName, productPartName and rawPartName
               must be unique.
 
     Tokens 2,3,...1+nPeriods - supply Volume
               float>=0.0
               This is the amount of available capacity in each period.
 
     Example:
     3 10 4 1995 day
     cap1  8.0 8.0 16.0
 
     This file defines one capacity named cap1.  There are 8.0 units of
     this capacity available on April 4 and April 5.  There are 16.0
     units available on April 6.  the units might be hours with April 6
     working a double shift.
 
 
 
4. dem
 
     The first record of this file is the same as the first record of
     the raw file.  All records that follow have the same format.  There
     is one record per product demand.
 
     Token 1 - demandName
               character string
               This is a string to identify the demand.  It might be the
               customer name or type of demand.
 
     Token 2 - demandedPartName
               character string
               partName of the demanded part.  Each demandName - demanded
               PartName pair must be unique.  The demandedPartName must
               be defined in the prd or raw files.
 
     Token 3 - priority
               1<=integer<=1000
               This is the importance of demand.  A demand priority of 1
               has the highest importance.
 
     Tokens 4,5,...3+nPeriods - demandVolume
               float>=0.0
               This is a vector of tokens, one for each period.  These
               tokens represent the demand volume for a period.
 
     Example:
     3 10 4 1995 day
     customer1 prod1  5  2.0  1.0  2.0
     customer2 prod1  4  1.0  1.0  1.0
 
     This file has a two demands named customer1 and customer2.
     Customer1 wants the product named prod1 and has a demand priority
     of 5.  Customer1 wants 2 units of prod1 on April 4 and 6, and 1
     unit on April 5.  Customer2 is more important than customer1
     because customers2 has priority 4.
 
 
 
5. bom
 
     The first record of this file is the same as the first record of the
     raw file.  All records that follow have the same format.  There is
     one record per BOM entry.
 
     Token 1 - producedPartName
               character string
               The name of the part being produced in the BOM entry.  The
               producedPartName must be defined in the prd files.
 
     Token 2 - consumedPartName
               The name of the part or capacity being consumed in the
               BOM entry.  The consumedPartName must be defined in the
               prd or raw files.
 
     Token 3 - usageRate
               float>=0.0
               The number of units of the consumed part used to make one
               unit of the produced part.
 
     Example:
     3 10 4 1995 day
     prod1 sub1  1.0
     prod1 raw2  2.0
     prod1 cap1  .25
     sub1 raw1  3.0
 
     This file has 4 BOM entries.  1.0 unites of sub1 is consumed to
     produce 1 unit of prod1 2.0 units of raw2 is consumed to produce
     one unit of prod1.  0.25 units of cap1 is consumed to make one unit
     of prod1.  3.0 units of raw1 is consumed to produce one unit of
     sub1.



Optional Files:

1. rev

     The first record of this file is the same as the first record of
     the raw file.  All records that follow have the same format.  There
     is one record per product demand with a revenue value..

     Token 1 - demandName
               character string
               This is a string to identify the demand.  It might be the
               customer name or type of demand.

     Token 2 - demandedPartName
               character string
               partName of the demanded part.  Each demandName - demanded
               PartName pair must have be defined in the dem file.

     Token 3 - revenue
               float
               The revenue associated with shipping one unit of the
               the part named demandedPartName to the demandName demand.

     Example:
     3 10 4 1995 day
     customer1 prod1  35.0
     customer2 prod1  40.0

     This file indicates that there is $35 revenue when shipping one
     unit of prod1 to customer1 and $40 revenue when shipping to customer2.


2. mls

     The first record of this file is the same as the first record of the
     raw file.  All records that follow have the same format.  There is
     one record per product part with a minimum lot size.

     Token 1 - productPartName
               character string
               Every productPartName and rawPartName must be unique.

     Token 2 - minLotSize
               >= 0.0
               This is the minimum amount that should be made when this
               product is made.  Note that this is used by the heuristic
               and ignored by the LP.

     Example:
     3 10 4 1995 day
     prod1  1.5
     sub1  .5

     This file indicates that when producing prod1 the minimum productions
     quanitity is 1.5.  The minimum production quantity for sub1 is 0.5.

3. ils

     The first record of this file is the same as the first record of the
     raw file.  All records that follow have the same format.  There is
     one record per product part with an increment lot size.

     Token 1 - productPartName
               character string
               Every productPartName and rawPartName must be unique.

     Token 2 - incLotSize
               >= 0.0
               This is the incremental amount that should be made when this
               product is made.  Note that this is used by the heuristic
               and ignored by the LP.

     Example:
     3 10 4 1995 day
     prod1  .5
     sub1  1. 

     This file indicates that when producing prod1 the incremental production
     quanitity is 0.5.  The incremental production quantity for sub1 is 1.0.
     
4. yld

     The first record of this file is the same as the first record of the
     raw file.  All records that follow have the same format.  There is
     one record per product part with an increment lot size.

     Token 1 - productPartName
               character string
               Every productPartName and rawPartName must be unique.

     Token 2 - yield
               >= 0
               This is the percentage yield when making productPartName.
               This is an integer quantity. Floats are rounded.

     Example:
     3 10 4 1995 day
     prod1  89
     sub1   95 

     This file indicates that when producing prod1 the production yield is 89%.
     The production yield for sub1 is 95%.

5. cal

     This file contains nPeriods + 1 records.  Each record has a period
     start date and has the same format. The last record contains the
     the start date of the period which follows the last period.

     Token 1 - periodNumber
               >=0
               The first record must have 0,
               the second record must have 1,
               the third record must have 2, ...

     Token 2 - startDay
            1<=Integer<=31
            Day of month period which is start of this period.
 
     Token 3 - startMonth
            1<=Integer<=12
            Month of year which is the start of this period.
 
     Token 4 - startYear
            1990<=Integer<=2100
            Year whic is the start of this period.

     Example:
     0 10 4 1995
     1 18 4 1995
     2 24 4 1995
     3  1 5 1995

     This is a 3 period problem.
     The first period begins on April 4, 1995.
     The second period begins on April 18, 1995.
     The third period begins on April 24,, 1995.
     If there was a forth period it would begin on May 5, 1995.


  -------------------------------------------------------------------*/


#include <wit/src/wit.h>
#include <scenario/src/lastPrbS.h>
#include <scenario/src/difWitR.h>
#include <scenario/src/calendar.h>


//Forward declarations
class ifstream;
class LgFrDataInterfaceEssentials;

//------------------------------------------------------------------------------
// To register all global functions for this class
//------------------------------------------------------------------------------
void
registerCmrpFilesDifImpFunctions(LgFrDataInterfaceImplementation *dImpCMRP);

//------------------------------------------------------------------------------
// To create a LgFrDataInterfaceImplementationFromCmrpFiles object
//------------------------------------------------------------------------------
LgFrDataInterfaceImplementation *
createCmrpFilesDifImp(const std::string& filename);

//------------------------------------------------------------------------------
//
// Global functions to get raw part attributes.
//
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// difCmrpFGetRawConstrained global function (registered in the attribute table).
// It returns a LgFrIntVector object.
//------------------------------------------------------------------------------
void *
difCmrpFGetRawConstrained(
    const LgFrDataInterfaceImplementation * difImp,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0 );

//------------------------------------------------------------------------------
// difCmrpFGetRawProcLeadTime global function (registered in the attribute table).
// It returns a LgFrTimeVecFloatVector object.
//------------------------------------------------------------------------------
void *
difCmrpFGetRawProcLeadTime(
    const LgFrDataInterfaceImplementation * difImp,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr = 0 );


//---------------------------------------------------------------------
//
//   Data interface from Wit Run
//
//---------------------------------------------------------------------

class LgFrDataInterfaceImplementationFromCmrpFiles : 
    public LgFrDataInterfaceImplementationFromWitRun {

    virtual int GetDIIType() const { return __LGFRDATAINTERFACEIMPLEMENTATIONFROMCMRPFILES; }

friend 
void *
difCmrpFGetRawConstrained(
    const LgFrDataInterfaceImplementation * difImp,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr);

friend
void *
difCmrpFGetRawProcLeadTime(
    const LgFrDataInterfaceImplementation * difImp,
    const LgFrItemPtrVector* itemPtrOrdVecPtr,
    LgFrError & status,
    const LgFrDataInterfaceEssentials * difEssenPtr);

public:

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
  //   Methods for processing file in minor ways
  //
  //-------------------------------------------------------------------

  // Find out date in header
  static
    LgFrCalendar
      calendar(const std::string& fileNameWithoutSuffix);

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
  LgFrDataInterfaceImplementationFromCmrpFiles(
     std::string fileNameWithoutSuffix);
  
  // Destructor
  virtual
     ~LgFrDataInterfaceImplementationFromCmrpFiles();

  // Assignment operator.
  LgFrDataInterfaceImplementationFromCmrpFiles&
    operator=(
	const LgFrDataInterfaceImplementationFromCmrpFiles& rhs);


  // Copy constructor
  LgFrDataInterfaceImplementationFromCmrpFiles(
      const LgFrDataInterfaceImplementationFromCmrpFiles& source);

  // ContextTest(...)
  static
    void
      contextTest
      (
	LgFrDataInterface    	* tdif1Ptr,	// testScenario1's
	LgFrLastProblemSolved   & lps,          // testScenario1's
	LgFrDataInterface    	* ts1dupDifPtr,	// testScenario1dup's
        LgFrDataInterface    	* tdif2Ptr,	// testScenario2
	const LgFrScenario      * scenPtr); 
  

//--------------
//   specific methods
//----------------
// protected:

    virtual
      /* const */ WitRun *
        constWitRun(const LgFrDataInterfaceEssentials * = 0)
          const;

protected:

    virtual
      WitRun *
        mutableWitRun(const LgFrDataInterfaceEssentials * = 0) ;

    virtual
      void
        defineWitRun(const LgFrDataInterfaceEssentials * = 0)
          const;

    virtual
      void
        readOperationAttribute(
          apiFuncsetOperationFloatVecAttribute witSetOperation,
          std::string fileExtension,
          std::string attributeName,
          const LgFrDataInterfaceEssentials * difEssenPtr)
            const;
  
    virtual
      void
        readOperationAttribute(
          apiFuncsetOperationIntVecAttribute witSetOperation,
          std::string fileExtension,
          std::string attributeName,
          const LgFrDataInterfaceEssentials * difEssenPtr)
            const;
 	  
   // Default Constructor
  LgFrDataInterfaceImplementationFromCmrpFiles();     
       

private:

  std::string baseFileName_;

  typedef std::pair<std::string,bool> ConstrainedDictPair;
  typedef std::map<std::string,bool> ConstrainedDictionary;
  ConstrainedDictionary constrainedDict_;

  typedef std::pair<std::string,float> ProcLeadTimePair;
  typedef std::map<std::string,float> ProcLeadTimeDictionary;
  ProcLeadTimeDictionary procLeadTimeDict_;
 
  // If dictionary is added then constructor code which initializes the
  // dictPtrVec_ needs to be updated.

  //RWHashDictionary* dictPtrVec_[3];


  // Methods used in implementing public methods.
  void
    freeDictKeysAndValues();

  void
    deepCopyDictKeysAndValues(
       const  LgFrDataInterfaceImplementationFromCmrpFiles& );

  void
    readFirstRecord(
	std::ifstream & inputFile,
          const std::string& fileSuffix )
             const;

 
};


#endif
