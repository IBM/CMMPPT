#include <numeric>
#include <set>

#include <cassert>
#include <iostream>
#include <cfloat>
#include <cstdio>
#include <climits>

#include <wit.h>

#include "CoinFloatEqual.h"
#include "onHandMaterial.h"
#include "printer.h"
#include "requestQuantity.h"
#include "visorProblem.h"


// 
//----------------------
// Material Methods
//----------------------
void VISORproblem::addMaterial    (const std::string & location, const std::string & nozSize, const std::string & plasticType, float quantity, int sharePercent )
{
	std::string ownMatName = ownMaterialName(location, nozSize, plasticType );
	std::string shrMatName = shrMaterialName(location, nozSize, plasticType );
	
	witAddPart(witRun(), ownMatName.c_str(), WitMATERIAL);
	witAddPart(witRun(), shrMatName.c_str(), WitCAPACITY);
	
	float shrQty = quantity*(float)sharePercent/100.0f;
	float ownQty = quantity-shrQty;
	witSetNameAttribute(&witGetPartSupplyVol,witSetPartSupplyVol,ownMatName,0,ownQty);
	witSetNameAttribute(&witGetPartSupplyVol,witSetPartSupplyVol,shrMatName,0,shrQty);
	
	std::string baseName = baseMaterialName(location, nozSize, plasticType);
	materialBaseNames_.insert(baseName);
}

void VISORproblem::getMaterials(
    std::vector<std::string> & locs,
    std::vector<std::string> & nozSizes,
    std::vector<std::string> & pTypes ) 
{
  locs.clear();
  nozSizes.clear();
  pTypes.clear();
#if 0
  int nMtms = materialBaseNames_.size();
  int m;
  for ( m=0; m<nMtms; m++ ) {
    locs.push_back( locationFromMaterialName(materialBaseNames_[m]) );
    nozSizes.push_back( nozSizeFromMaterialName(materialBaseNames_[m]) );
    pTypes.push_back( plasticTypeFromMaterialName(materialBaseNames_[m]) );
  }
#else
  std::set<std::string>::const_iterator it;
  for ( it=materialBaseNames_.begin(); it!=materialBaseNames_.end(); ++it ) {
    locs.push_back( locationFromMaterialName(*it) );
    nozSizes.push_back( nozSizeFromMaterialName(*it) );
    pTypes.push_back( plasticTypeFromMaterailName(*it) );
  }
#endif  
}

std::vector<float> VISORproblem::getOwnSupply(
             const std::string & loc, 
             const std::string & nSize,
             const std::string & pType )
{
  std::vector<float> retVal;
  std::string name = ownMaterialName(loc,nSize,pType);
  retVal = witGetNameAttribute(witGetPartSupplyVol,name);
  return retVal;
}

std::vector<float> VISORproblem::getSharedSupply(
             const std::string & loc, 
             const std::string & nSize,
             const std::string & pType )
{
  std::vector<float> retVal;
  std::string name = shrMaterialName(loc,nSize,pType);
  retVal = witGetNameAttribute(witGetPartSupplyVol,name);
  return retVal;
}


#if 0
void VISORproblem::addMtm(const std::string & mtmName, const std::string & mtmLoc,
                         float demanCostS, float demanTimeS)
{
  std::string igfMtmWitName = igfMtmName(mtmName, mtmLoc );
  std::string acquireMtmWitName = acquireMtmName(mtmName, mtmLoc, mtmLoc );
  std::string serverMtmWitName = serverMtmName(mtmName, mtmLoc, mtmLoc );
  std::string demanMtmWitName = demanMtmName(mtmName, mtmLoc, mtmLoc );

  int np = getNPeriods();

  // Add igf owned mtm, acquire operation, and connect them
  witAddPart(witRun(), igfMtmWitName.c_str(), WitCAPACITY);
  witAddOperation(witRun(), acquireMtmWitName.c_str() );
  witAddBomEntry(witRun(), acquireMtmWitName.c_str(), igfMtmWitName.c_str() );

  // Add substitute boms to allow period 0 supply to be consumed in latter periods.
  addSubsForPeriod1SupplyPerishPeriod(acquireMtmWitName,igfMtmWitName,0.0f);

  // Add server owned mtm, and connect to acquire operation
  addNonScrapableCapacity(serverMtmWitName.c_str());
  witAddBopEntry(witRun(), acquireMtmWitName.c_str(), serverMtmWitName.c_str() );

  // Add demanufacturing operation and connect to server owned mtm
  witAddOperation(witRun(), demanMtmWitName.c_str() );
  witAddBomEntry(witRun(), demanMtmWitName.c_str(), serverMtmWitName.c_str() );

  // Set default demanCost
  float * demanCost = floatToConstFloatStar( (float) demanCostS );
  witSetOperationObj1ExecCost(witRun(), demanMtmWitName.c_str(), demanCost );
  delete [] demanCost;

  // Set default demanTime
  float * demanTime = floatToConstFloatStar( (float) demanTimeS );
  witSetBomEntryOffset(witRun(), demanMtmWitName.c_str(), 0, demanTime );
  delete [] demanTime;

  // Set default app data to hold wfmv
  ESO2mtmAppData * appData = new ESO2mtmAppData(np);
  std::vector<float> moveCost = floatToStlVec(0.0f);
  appData->setMoveCost(moveCost);
  witSetOperationAppData (mutableWitRun(), acquireMtmWitName.c_str(), appData);
  
  std::string baseName = baseMtmName(mtmName, mtmLoc );
  materialBaseNames_.insert(baseName);
  //addMtmAcquireLoc(baseName,mtmLoc);
}


bool VISORproblem::mtmExists(const std::string & mtmName, const std::string & mtmLoc )const
{
  std::string igfMtmWitName = igfMtmName(mtmName, mtmLoc );
  witBoolean partExists;
  witGetPartExists(mutableWitRun(),igfMtmWitName.c_str(),&partExists);
  bool retVal = false;
  if( partExists) retVal = true;
  return retVal;
}



//-------------------------------------------------
// Optimization methods
//-------------------------------------------------
void VISORproblem::solve()
{

  //writeWitData("eso2.wit");

 

  witBoolean postProcessed; 
  witGetPostprocessed(mutableWitRun(),&postProcessed);
  assert(postProcessed);

}

void VISORproblem::solveWithPegging() {
  std::cout <<"entered: VISORproblem::solveWithPegging()" <<std::endl;
  solve();
  doPegging();
}
#endif



//-------------------------------------------------------------------------
// material Name Methods
//-------------------------------------------------------------------------
std::string VISORproblem::ownMaterialName(const std::string & location, const std::string & nozSize, const std::string & plasticType )
{
  return "ownSupply: "+baseMaterialName(location, nozSize, plasticType);
}
std::string VISORproblem::shrMaterialName(const std::string & location, const std::string & nozSize, const std::string & plasticType )
{
  return "shareableSupply: "+baseMaterialName(location, nozSize, plasticType);
}
std::string VISORproblem::baseMaterialName(const std::string & location, const std::string & nozSize, const std::string & plasticType )
{
  return " Size "+nozSize+" Type "+plasticType+" at "+location;
}
std::string VISORproblem::locationFromMaterialName(const std::string & matName)
{  
  return textAfter(matName," at ");
}
std::string VISORproblem::nozSizeFromMaterialName(const std::string & matName)
{ 
  return textBetween(matName," Size "," Type ");
}
std::string VISORproblem::plasticTypeFromMaterailName(const std::string & matName)
{  
  return textBetween(matName," Type "," at ");
}



//-------------------------------------------------------------------------
// text utilities Methods
//-------------------------------------------------------------------------
std::string VISORproblem::textBetween(
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
std::string VISORproblem::textAfter(
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
bool VISORproblem::beginsWith(
                        const std::string & src, 
                        const std::string & begString)
{
  size_t begPos = src.find(begString);
  if ( begPos==0 ) return true;
  else return false;
}
bool VISORproblem::contains(
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
void VISORproblem::witSetNameAttribute(
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
void VISORproblem::witSetNameAttribute(
    witSetNameFloatStar     witSetFunc,
    const std::string & name,
    const std::vector<float> & value )
{
  float * fltStar = stlVecToFloatStar(value);
  witSetFunc(witRun(),name.c_str(),fltStar);
  delete [] fltStar;
}

// Set Part/Operation float* attribute, given float
void VISORproblem::witSetNameAttribute(
    witSetNameFloatStar     witSetFunc,
    const std::string & name,
    float value )
{
  float * fltStar = floatToConstFloatStar(value);
  witSetFunc(witRun(),name.c_str(),fltStar);
  delete [] fltStar;
}

// Set Part/Operation bound attribute, given three std::vector<float> vectors
void VISORproblem::witSetNameBoundAttribute(
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
void VISORproblem::witGetNameBoundAttribute(
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
void VISORproblem::witSetArcAttribute(
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
void VISORproblem::witSetArcAttribute(
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
void VISORproblem::witSetArcAttribute(
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
void VISORproblem::witSetDemandAttribute(
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
void VISORproblem::witSetDemandAttribute(
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
std::vector<float> VISORproblem::witGetNameAttribute(
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
std::vector<float> VISORproblem::witGetArcAttribute(
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
std::vector<int> VISORproblem::witGetArcAttribute(
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
std::vector<float> VISORproblem::witGetSubArcAttribute(
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
std::vector<int> VISORproblem::witGetSubArcAttribute(
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
int VISORproblem::witGetSubArcAttribute(
                                     witGetNameIndexIndexIntStar     witGetFunc,
                                     const std::string & opName,
                                     int index, int subIndex )const
{
  int retVal;
  witGetFunc(mutableWitRun(),opName.c_str(),index,subIndex,&retVal);
  return retVal;
}

// Set sub float* attribute, given period and fltValue
void VISORproblem::witSetSubArcAttribute(
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
std::vector<float> VISORproblem::witGetDemandAttribute(
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

void VISORproblem::setTitle( const std::string & title)
{
  witSetTitle(witRun(),title.c_str());
}

std::string VISORproblem::getTitle() const
{
  char * t;
  witGetTitle(mutableWitRun(),&t);
  std::string retVal(t);
  witFree(t);
  return retVal;
}

void VISORproblem::setNPeriods( int nP )
{
  witSetNPeriods(witRun(),nP);
  nPeriods_ = nP;
}












WitRun * VISORproblem::mutableWitRun() const { return wr_; }
WitRun * VISORproblem::witRun() { return wr_; }



// default constructor
VISORproblem::VISORproblem()
:
wr_(NULL),
nPeriods_(30),
materialBaseNames_()
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
VISORproblem::~VISORproblem()
{
  gutsOfDestructor();
}

// copy constructor. Not yet suported
VISORproblem::VISORproblem( const VISORproblem& source ):
wr_(NULL),
nPeriods_(source.nPeriods_)
{
  gutsOfCopy(source);
}

// assignment operator. Not yet supported
VISORproblem&
VISORproblem::operator=(const VISORproblem& rhs)
{  
  if (this != &rhs) { 
    if ( wr_ != NULL ) gutsOfDestructor();
    wr_ = NULL;
    gutsOfCopy(rhs);
  }
  return *this;
}

// copy object
void VISORproblem::gutsOfCopy( const VISORproblem& source )
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
 
#if 0 
  // Copy global app data
  {      
    //ESO2probAppData * srcAppData;  
    //ESO2probAppData * sinkAppData;
    //witGetAppData(source.mutableWitRun(), (void**)&srcAppData);
    //sinkAppData = new ESO2probAppData(*srcAppData);    
    //witSetAppData(witRun(), sinkAppData);
  }
    
  // copy partDemand appData
  {
    std::vector<std::string> parts;
    std::vector<std::string> custLocs;
    std::vector<std::string> plantLocs;
    getPartDemands( parts, custLocs, plantLocs);
    int p;
    for (p=0; p<parts.size(); ++p ) { 
      ESO2partDemandAppData * srcAppData;  
      ESO2partDemandAppData * sinkAppData;
      std::string dName = partDemandName(parts[p],custLocs[p],plantLocs[p]);
      witGetDemandAppData(source.mutableWitRun(),dName.c_str(),dName.c_str(),(void**)&srcAppData);
      sinkAppData = new ESO2partDemandAppData(*srcAppData);    
      witSetDemandAppData(witRun(),dName.c_str(),dName.c_str(),sinkAppData);
    }
  }
  // copy looseParts appData
  {
    std::vector<std::string> parts;
    std::vector<std::string> locs;
    getLooseParts( parts, locs);
    int p;
    for (p=0; p<parts.size(); ++p ) { 
      ESO2loosePartAppData * srcAppData;  
      ESO2loosePartAppData * sinkAppData;
      std::string opName = acquireLoosePartName(parts[p],locs[p]);
      witGetOperationAppData(source.mutableWitRun(),opName.c_str(),(void**)&srcAppData);
      sinkAppData = new ESO2loosePartAppData(*srcAppData);    
      witSetOperationAppData(witRun(),opName.c_str(),sinkAppData);
    }
  }
   
  // copy features appData
  {
    std::vector<std::string> features;
    std::vector<std::string> locs;
    getFeatures( features, locs);
    int p;
    for (p=0; p<features.size(); ++p ) {
      ESO2featureAppData * srcAppData;  
      ESO2featureAppData * sinkAppData;
      std::string opName = acquireFeatureName(features[p],locs[p]);
      witGetOperationAppData(source.mutableWitRun(),opName.c_str(),(void**)&srcAppData);
      sinkAppData = new ESO2featureAppData(*srcAppData);    
      witSetOperationAppData(witRun(),opName.c_str(),sinkAppData);
    }
  }
  
#endif

}

// destructor
void VISORproblem::gutsOfDestructor()
{

  // Delete global app data
  {  
    //ESO2probAppData * probAppData;  
    //witGetAppData(mutableWitRun(),  (void**)&probAppData);
    //delete probAppData;
  }
  
  // delete partDemand appData
  {
    //std::vector<std::string> parts;
    //std::vector<std::string> custLocs;
    //std::vector<std::string> plantLocs;
    //getPartDemands( parts, custLocs, plantLocs);
    //int p;
    //for (p=0; p<parts.size(); ++p ) { 
      //ESO2partDemandAppData * srcAppData;  
      //std::string dName = partDemandName(parts[p],custLocs[p],plantLocs[p]);
      //witGetDemandAppData(mutableWitRun(),dName.c_str(),dName.c_str(),(void**)&srcAppData);
      //delete srcAppData;
    //}
  }

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
VISORproblem::test()
{
  //OsiRelFltEq eq(1e-05);
  CoinRelFltEq eq(1.e-05);

  {
    VISORproblem prob;
    
    prob.setNPeriods(12);
    assert( prob.getNPeriods() == 12 );
    
    prob.setTitle("wittitle");
    assert( prob.getTitle() == "wittitle");

  }


         
  // Test materials 
  {
    VISORproblem prob;
    assert(prob.getNPeriods()==30);
    prob.setNPeriods(25);    
    assert(prob.getNPeriods()==25);
    
    prob.addMaterial("Briarcliff","N175","PLA",200.0,75);
    
    std::vector<std::string> location, nozSize, plasticType;
    prob.getMaterials( location, nozSize, plasticType );
    assert( location.size()==1 );
    assert( nozSize.size()==1 );
    assert( plasticType.size()==1 );    
    assert( location[0]=="Briarcliff" );
    assert( nozSize[0]=="N175" );
    assert( plasticType[0]=="PLA" ); 
    
    std::vector<float> sv=prob.getOwnSupply("Briarcliff","N175","PLA");
    assert( eq(sv[0],50.) );
    assert( eq(sv[1],0.0) );
    sv=prob.getSharedSupply("Briarcliff","N175","PLA");
    assert( eq(sv[0],150.) );
    assert( eq(sv[3],0.0) );
    assert( sv.size()==25 );
    
    
    prob.addMaterial("Amawalk",   "N175","ABS",100.0,33);    
    prob.getMaterials( location, nozSize, plasticType );
    assert( location.size()==2 );
    sv=prob.getOwnSupply("Amawalk",   "N175","ABS");
    assert( eq(sv[0],67.) );
    assert( eq(sv[24],0.0) );
    sv=prob.getSharedSupply("Amawalk",   "N175","ABS");
    assert( eq(sv[0],33.) );
    assert( eq(sv[20],0.0) );
    
    
    //std::vector<float> vol = p1.getPartDemandShipVol("0000000P1413","980","980");
    //assert(eq(vol[0],0.0));
   }
    
}


