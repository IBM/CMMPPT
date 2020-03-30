#include <numeric>
#include <set>

#include <cassert>
#include <iostream>
#include <cfloat>
#include <cstdio>
#include <climits>

#include <wit.h>

#include "VisorFloatEqual.h"
#include "onHandMaterial.h"
#include "printer.h"
#include "requestQuantity.h"
#include "visorProblem2.h"


//
//----------------------
// Visor Methods
//----------------------
void VISORproblem2::addVisor( const std::string & name, const std::string & location)
{
	std::string aggVisNm       = aggregateVisorName(location );
	std::string aggOperNm      = aggregateOperName(location );
	std::string noSupplyPartNm = noSupplyVisorName(location );
	std::string visorPartNm    = visorPartName(name, location );

	// If this is the first visor at this location, then add location constructs
	if ( !locationExists(location) )
	{
	   // Location does not exist so add it
	   // Add parts: Printer, NoSupply, and produced visor
       witAddPart(witRun(), aggVisNm.c_str(),      WitMATERIAL);
	   witAddPart(witRun(), noSupplyPartNm.c_str(), WitMATERIAL);

       // Add opertaion
	   witAddOperation(witRun(),aggOperNm.c_str());

	   // Add bom connecting operation to printer and noSupplyPart
	   witAddBomEntry(witRun(), aggOperNm.c_str(), noSupplyPartNm.c_str());

	   // Connect operation to produced visor
	   witAddBopEntry(witRun(), aggOperNm.c_str(), aggVisNm.c_str());

       std::string baseName = baseLocationName(location);
	   locationBaseNames_.insert(location);
	   //std::cout <<location+"\n";
    }

    // Add part and SubBom entry to part
    witAddPart(witRun(), visorPartNm.c_str(),      WitMATERIAL);
    witAddSubsBomEntry(witRun(),aggOperNm.c_str(),0,visorPartNm.c_str());

}

void VISORproblem2::setVisorSupplyVol( const std::string & name, const std::string & location, 
         const std::vector<float> & supplyVol)
{
    std::string visorPartNm    = visorPartName(name, location );
    witSetNameAttribute(witSetPartSupplyVol,visorPartNm,supplyVol);
}

bool VISORproblem2::locationExists( const std::string & loc )
{
  std::string aggOperNm      = aggregateOperName(loc );
  witBoolean exists;
  witGetOperationExists(mutableWitRun(),aggOperNm.c_str(),&exists);
  bool retVal = false;
  if( exists ) retVal = true;
  //std::cout <<aggOperNm <<" " <<retVal <<std::endl;
  return retVal;
}

//
//----------------------
// Visor Request (hospital demands) Methods
//----------------------
void VISORproblem2::addVisorRequest(
    const std::string & name,
    int period,
    int requestedQuantity)
{
	 // get list of all locations
    std::set<std::string> locs = getLocation();
    std::set<std::string>::const_iterator it;
    for ( it=locs.begin(); it!=locs.end(); ++it ) {
       
       std::string aggVisNm  = aggregateVisorName(*it);
       
       // Only add demand if it doesn't already exist
       if(!witGetDemandExists(aggVisNm,name))
           witAddDemand(witRun(),aggVisNm.c_str(),name.c_str());

       // Set demand to the requested number of visors       
       witSetDemandAttribute(witGetDemandDemandVol,witSetDemandDemandVol,
                      aggVisNm, name, period, requestedQuantity);    
    }
}

// Return true if demand exist
bool VISORproblem2::witGetDemandExists(const std::string & visorName, const std::string & demandName ) 
{
  bool retVal = false;
  
  witBoolean partExists;
  witGetPartExists( mutableWitRun(), visorName.c_str(), &partExists );
  if ( !partExists ) return retVal;   
  
  int nPartDemands;
  char ** demandList;
  witGetPartDemands( mutableWitRun(), visorName.c_str(), &nPartDemands, &demandList);
  for ( int d=0; d<nPartDemands; ++d ) {
    if ( demandList[d] == demandName ) {
      retVal = true;
    }    
    witFree(demandList[d]);
  }
  witFree(demandList);
  return retVal;
}

// -----------------------------
// solver methods
// ----------------------------
void VISORproblem2::solve()
{

	witHeurImplode(witRun());
}




//-------------------------------------------------------------------------
// material Name Methods
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
// Visor and location Name Methods
//-------------------------------------------------------------------------
std::string VISORproblem2::aggregateVisorName(const std::string & location )
{
  return "Visor aggregation "+baseLocationName(location);
}
std::string VISORproblem2::aggregateOperName(const std::string & location )
{
  return "Aggregate  "+baseLocationName(location);
}
std::string VISORproblem2::noSupplyVisorName(const std::string & location )
{
  return "No supply part "+baseLocationName(location);
}
std::string VISORproblem2::visorPartName(const std::string & name, const std::string & location )
{
  return "Visor "+name+" made "+baseLocationName(location);
}
std::string VISORproblem2::baseLocationName(const std::string & location )
{
  return " at-> "+location;
}

std::set<std::string> VISORproblem2::getLocation() { return locationBaseNames_; }

//std::string VISORproblem2::printerFromPrinterName(const std::string & baseName)
//{
//  return textBetween(baseName,"Printer: "," at-> ");
//}
//std::string VISORproblem2::locationFromPrinterName(const std::string & baseName)
//{
//  return textAfter(baseName," at-> ");



//-------------------------------------------------------------------------
// text utilities Methods
//-------------------------------------------------------------------------
std::string VISORproblem2::textBetween(
                        const std::string & src,
                        const std::string & begMark,
                        const std::string & endMark)
{
  size_t begPos = src.find(begMark);
  assert( begPos != std::string::npos );
  begPos = begPos+begMark.size();

  size_t endPos = src.find(endMark);
  assert( endPos != std::string::npos );

  size_t len = endPos-begPos;
  std::string retVal = src.substr(begPos,len);
  return retVal;
}
std::string VISORproblem2::textAfter(
                        const std::string & src,
                        const std::string & begMark)
{
  size_t begPos = src.find(begMark);
  assert( begPos != std::string::npos );
  begPos = begPos+begMark.size();

  size_t endPos = src.size();

  size_t len = endPos-begPos;
  std::string retVal = src.substr(begPos,len);
  return retVal;
}
bool VISORproblem2::beginsWith(
                        const std::string & src,
                        const std::string & begString)
{
  size_t begPos = src.find(begString);
  if ( begPos==0 ) return true;
  else return false;
}
bool VISORproblem2::contains(
                        const std::string & haystack,
                        const std::string & needle)
{
  size_t pos = haystack.find(needle);
  if ( pos!=std::string::npos ) return true;
  else return false;
}



//-------------------------------------------------------------------------
// Utilities for invoking Wit Methods
//-------------------------------------------------------------------------
// Set element of Part/Operation float* attribute, given period and float
void VISORproblem2::witSetNameAttribute(
    witGetNameFloatStarStar witGetFunc,
    witSetNameFloatStar     witSetFunc,
    const std::string & name,
    int period,
    float flt )
{
  assert( period>= 0 );
  assert( period<getNPeriods() );
  float * floatStar;
  witGetFunc(mutableWitRun(),name.c_str(),&floatStar);
  floatStar[period] = flt;
  witSetFunc(witRun(),name.c_str(),floatStar);
  witFree(floatStar);
}

// Set Part/Operation float* attribute, given std::vector<float>
void VISORproblem2::witSetNameAttribute(
    witSetNameFloatStar     witSetFunc,
    const std::string & name,
    const std::vector<float> & value )
{
  float * fltStar = stlVecToFloatStar(value);
  witSetFunc(witRun(),name.c_str(),fltStar);
  delete [] fltStar;
}

// Set Part/Operation float* attribute, given float
void VISORproblem2::witSetNameAttribute(
    witSetNameFloatStar     witSetFunc,
    const std::string & name,
    float value )
{
  float * fltStar = floatToConstFloatStar(value);
  witSetFunc(witRun(),name.c_str(),fltStar);
  delete [] fltStar;
}

// Set Part/Operation bound attribute, given three std::vector<float> vectors
void VISORproblem2::witSetNameBoundAttribute(
    witSetName3FloatStar     witSetFunc,
    const std::string & name,
    const std::vector<float> & hlb,
    const std::vector<float> & slb,
    const std::vector<float> & hub )
{
  float * hlbFltStar = stlVecToFloatStar(hlb);
  float * slbFltStar = stlVecToFloatStar(slb);
  float * hubFltStar = stlVecToFloatStar(hub);
  witSetFunc(witRun(),name.c_str(),hlbFltStar, slbFltStar, hubFltStar);
  delete [] hlbFltStar;
  delete [] slbFltStar;
  delete [] hubFltStar;
}
// Get Part/Operation bound attributes, given three std::vector<float> vectors
void VISORproblem2::witGetNameBoundAttribute(
    witGetName3FloatStar     witGetFunc,
    const std::string & name,
    std::vector<float> & hlb,
    std::vector<float> & slb,
    std::vector<float> & hub )const
{
  float * hlbFltStar;
  float * slbFltStar;
  float * hubFltStar;
  witGetFunc(mutableWitRun(),name.c_str(),&hlbFltStar, &slbFltStar, &hubFltStar);
  hlb=witFloatStarToStlVec(hlbFltStar);
  slb=witFloatStarToStlVec(slbFltStar);
  hub=witFloatStarToStlVec(hubFltStar);
  witFree(hlbFltStar);
  witFree(slbFltStar);
  witFree(hubFltStar);
}



// Set element of Bop/Bop Entry float* attribute, given period and float
void VISORproblem2::witSetArcAttribute(
    witGetNameIndexFloatStarStar witGetFunc,
    witSetNameIndexFloatStar     witSetFunc,
    const std::string & opName,
    int index,
    int period,
    float flt )
{
  assert( period>= 0 );
  assert( period<getNPeriods() );
  //int bei = getBopEntryIndex(opName,partName);
  //assert( bei!=-1 );
  float * floatStar;
  witGetFunc(mutableWitRun(),opName.c_str(),index,&floatStar);
  floatStar[period] = flt;
  witSetFunc(witRun(),opName.c_str(),index,floatStar);
  witFree(floatStar);
}

// Set element of Bop/Bop Entry float* attribute, given std::vector<float>
void VISORproblem2::witSetArcAttribute(
    witSetNameIndexFloatStar     witSetFunc,
    const std::string & opName,
    int index,
    const std::vector<float> & value )
{
  float * fltStar = stlVecToFloatStar(value);
  witSetFunc(witRun(),opName.c_str(),index,fltStar);
  delete []  fltStar;
}

// Set element of Bop/Bop Entry float* attribute, given float
void VISORproblem2::witSetArcAttribute(
    witSetNameIndexFloatStar     witSetFunc,
    const std::string & opName,
    int index,
    float value )
{
  float * fltStar = floatToConstFloatStar(value);
  witSetFunc(witRun(),opName.c_str(),index,fltStar);
  delete []  fltStar;
}



// Set element on demand float* attribute, given period and fltValue
void VISORproblem2::witSetDemandAttribute(
    witGetDblNameFloatStarStar      witGetFunc,
    witSetDblNameFloatStar          witSetFunc,
    const std::string & partName,
    const std::string & demandName,
    int period,
    float flt )
{
  assert( period>= 0 );
  assert( period<getNPeriods() );
  float * floatStar;
  witGetFunc(mutableWitRun(),partName.c_str(),demandName.c_str(),&floatStar);
  floatStar[period] = flt;
  witSetFunc(witRun(),partName.c_str(),demandName.c_str(),floatStar);
  witFree(floatStar);
}

// Set element on demand float* attribute, given stl::vector<float>
void VISORproblem2::witSetDemandAttribute(
    witSetDblNameFloatStar          witSetFunc,
    const std::string & partName,
    const std::string & demandName,
    const std::vector<float> & fltVec )
{
  float * floatStar = stlVecToFloatStar(fltVec);
  witSetFunc(witRun(),partName.c_str(),demandName.c_str(),floatStar);
  delete [] floatStar;
}

// Get vector of wit part/Operation float* attribute
std::vector<float> VISORproblem2::witGetNameAttribute(
    witGetNameFloatStarStar witGetFunc,
    const std::string & name ) const
{
  float * attr;
  witGetFunc(mutableWitRun(),name.c_str(),&attr);
  std::vector<float> retVal = witFloatStarToStlVec(attr);
  witFree(attr);
  return retVal;
}

// Get vector of bom/bop float* attribute
std::vector<float> VISORproblem2::witGetArcAttribute(
                                     witGetNameIndexFloatStarStar     witGetFunc,
                                     const std::string & opName,
                                     int index )const
{
  float * attr;
  witGetFunc(mutableWitRun(),opName.c_str(),index,&attr);
  std::vector<float> retVal = witFloatStarToStlVec(attr);
  witFree(attr);
  return retVal;
}

// Get vector of bom/bop int* attribute
std::vector<int> VISORproblem2::witGetArcAttribute(
                                     witGetNameIndexIntStarStar     witGetFunc,
                                     const std::string & opName,
                                     int index )const
{
  int * attr;
  witGetFunc(mutableWitRun(),opName.c_str(),index,&attr);
  std::vector<int> retVal = witIntStarToStlVec(attr);
  witFree(attr);
  return retVal;
}



// Get vector of sub float* attribute
std::vector<float> VISORproblem2::witGetSubArcAttribute(
                                     witGetNameIndexIndexFloatStarStar     witGetFunc,
                                     const std::string & opName,
                                     int index, int subIndex )const
{
  float * attr;
  witGetFunc(mutableWitRun(),opName.c_str(),index,subIndex,&attr);
  std::vector<float> retVal = witFloatStarToStlVec(attr);
  witFree(attr);
  return retVal;
}

// Get vector of sub int* attribute
std::vector<int> VISORproblem2::witGetSubArcAttribute(
                                     witGetNameIndexIndexIntStarStar     witGetFunc,
                                     const std::string & opName,
                                     int index, int subIndex )const
{
  int * attr;
  witGetFunc(mutableWitRun(),opName.c_str(),index,subIndex,&attr);
  std::vector<int> retVal = witIntStarToStlVec(attr);
  witFree(attr);
  return retVal;
}

// Get vector of sub int attribute
int VISORproblem2::witGetSubArcAttribute(
                                     witGetNameIndexIndexIntStar     witGetFunc,
                                     const std::string & opName,
                                     int index, int subIndex )const
{
  int retVal;
  witGetFunc(mutableWitRun(),opName.c_str(),index,subIndex,&retVal);
  return retVal;
}

// Set sub float* attribute, given period and fltValue
void VISORproblem2::witSetSubArcAttribute(
    witGetNameIndexIndexFloatStarStar witGetFunc,
    witSetNameIndexIndexFloatStar     witSetFunc,
    const std::string & opName,
    int index, int subIndex,
    int period,
    float flt )
{
  assert( period>= 0 );
  assert( period<getNPeriods() );
  float * floatStar;
  witGetFunc(mutableWitRun(),opName.c_str(),index,subIndex,&floatStar);
  floatStar[period] = flt;
  witSetFunc(witRun(),opName.c_str(),index,subIndex,floatStar);
  witFree(floatStar);
}





// Get vector of wit demand float* attribute
std::vector<float> VISORproblem2::witGetDemandAttribute(
    witGetDblNameFloatStarStar witGetFunc,
    const std::string & partName,
    const std::string & demandName )const
{
  float * attr;
  witGetFunc(mutableWitRun(),partName.c_str(),demandName.c_str(),&attr);
  std::vector<float> retVal = witFloatStarToStlVec(attr);
  witFree(attr);
  return retVal;
}






//--------------------------------------------------------

void VISORproblem2::setTitle( const std::string & title)
{
  witSetTitle(witRun(),title.c_str());
}

std::string VISORproblem2::getTitle() const
{
  char * t;
  witGetTitle(mutableWitRun(),&t);
  std::string retVal(t);
  witFree(t);
  return retVal;
}

void VISORproblem2::setNPeriods( int nP )
{
  witSetNPeriods(witRun(),nP);
  nPeriods_ = nP;
}
// Set OSL Logfile name
void VISORproblem2::setSolverLogFileName(const std::string & name)
{
  witSetSolverLogFileName(witRun(),name.c_str());
}

void VISORproblem2::writeWitData( std::string filename )
{
  witSetMesgFileAccessMode(mutableWitRun(),WitFALSE,"w");
  witWriteData(mutableWitRun(), filename.c_str() );
}












WitRun * VISORproblem2::mutableWitRun() const { return wr_; }
WitRun * VISORproblem2::witRun() { return wr_; }



// default constructor
VISORproblem2::VISORproblem2()
:
wr_(NULL),
nPeriods_(30),
locationBaseNames_()
//printerBaseNames_()
{
  witNewRun( &wr_ );
  witInitialize( witRun() );
  //witSetOslMesgFileName(witRun(),WitSTDOUT);

  // Turn off WIT informational messages
  witSetMesgTimesPrint( witRun(), WitTRUE, WitINFORMATIONAL_MESSAGES, 0);
  // Turn off warning msg about long names.
  witSetMesgTimesPrint( witRun(), WitTRUE, 338, 0);

  // Turn on some interesting informational messages
  witSetMesgTimesPrint( witRun(), WitTRUE, 729, UCHAR_MAX);
  witSetMesgTimesPrint( witRun(), WitTRUE, 730, UCHAR_MAX);

  //WIT0729I Deleting selected objects.
  //         Number of objects selected by the user:  158900
  //         Number of objects selected by WIT:       160629
  //         Number of objects selected total:        319529

  //WIT0730I Object purge complete.
  //         Number of objects before purge:  332170
  //         Number of objects deleted:       319529
  //         Number of objects after purge:    12641

  /* Turn off warning msg:
  WIT0749W One or more BOP entries have been rejected for possible explosion,
         because they had positive effective production rates (= yieldRate *
         productRate) that were below the value of expCutoff. The largest
         effective production rate that was rejected and the corresponding BOP
         entry and execution period are given below:

            expCutoff:             0.01
            Largest Rejected Rate: 0.008
            Operation:             "disassemble_26353AU"
            BOP Entry Index:       4
            Exec. Period:          0
  */
  //witSetMesgTimesPrint( witRun(), WitTRUE, 749, 0);




    witSetIndependentOffsets( witRun(), WitTRUE );
    witSetNPeriods(witRun(),30);
  //witSetObjChoice( witRun(), 1 );

  //witSetUseFocusHorizons( witRun(), WitFALSE );

  //witSetExecEmptyBom( witRun(), WitFALSE );
  //witSetMultiRoute( witRun(), WitTRUE );
  //witSetIndependentOffsets( witRun(), WitTRUE );

  //witSetAccAfterOptImp(witRun(),WitTRUE );
  //witSetAccAfterSoftLB(witRun(),WitTRUE );

  //witSetPostSolveRecovery(witRun(),WitTRUE);

  //witSetGroupPipMode(witRun(),WitTRUE);

  //ESO2probAppData * problemAppData = new ESO2probAppData;
  //witSetAppData(witRun(),problemAppData);

}

// destructor
VISORproblem2::~VISORproblem2()
{
  gutsOfDestructor();
}

// copy constructor. Not yet suported
VISORproblem2::VISORproblem2( const VISORproblem2& source ):
wr_(NULL),
nPeriods_(source.nPeriods_)
{
  gutsOfCopy(source);
}

// assignment operator. Not yet supported
VISORproblem2&
VISORproblem2::operator=(const VISORproblem2& rhs)
{
  if (this != &rhs) {
    if ( wr_ != NULL ) gutsOfDestructor();
    wr_ = NULL;
    gutsOfCopy(rhs);
  }
  return *this;
}

// copy object
void VISORproblem2::gutsOfCopy( const VISORproblem2& source )
{
  // Copy Wit Run
  assert(wr_==NULL);
  witNewRun( &wr_ );

  // Turn off WIT informational messages
  witSetMesgTimesPrint( wr_, WitTRUE, WitINFORMATIONAL_MESSAGES, 0);
  // Turn off warning msg about long names.
  witSetMesgTimesPrint( wr_, WitTRUE, 338, 0);

  witCopyData(wr_,source.wr_);
  nPeriods_ = source.nPeriods_;



}

// destructor
void VISORproblem2::gutsOfDestructor()
{



  // delete the witRun
  witDeleteRun(witRun());
}



#ifdef NDEBUG
#undef NDEBUG
#endif
//------------------------------------------------------------------------------
// Test function
//------------------------------------------------------------------------------
void
VISORproblem2::test()
{
  //OsiRelFltEq eq(1e-05);
  VisorRelFltEq eq(1.e-05);

  {
    VISORproblem2 prob;

    prob.setNPeriods(12);
    assert( prob.getNPeriods() == 12 );

    prob.setTitle("wittitle");
    assert( prob.getTitle() == "wittitle");

  }



  {
  	 // Test materials
    VISORproblem2 prob;
    assert(prob.getNPeriods()==30);
    prob.setNPeriods(25);
    assert(prob.getNPeriods()==25);



  }



}


