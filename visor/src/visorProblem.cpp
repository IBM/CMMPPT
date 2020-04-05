

#include <cassert>
#include <iostream>
#include <cfloat>
#include <climits>

#include "VisorFloatEqual.h"
#include "visorProblem.h"







//----------------------------------
// Set/Get Wit Global Attributes.
//----------------------------------

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
// Set OSL Logfile name
void VISORproblem::setSolverLogFileName(const std::string & name)
{
  witSetSolverLogFileName(witRun(),name.c_str());
}

void VISORproblem::writeWitData( const std::string& filename )
{
  witSetMesgFileAccessMode(mutableWitRun(),WitFALSE,"w");
  witWriteData(mutableWitRun(), filename.c_str() );
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
  return begPos == 0;
}
bool VISORproblem::contains(
    const std::string & haystack,
    const std::string & needle)
{
  size_t pos = haystack.find(needle);
  return pos != std::string::npos;
}



//------------------------------------------------
// Wrappers around WIT functions that allow C++ datatypes
// to be used.
// These are implemented by passing a WIT functon as a paramenter
//-------------------------------------------------

//------------------------------------------------
//  Part/Operation Set/Get Wrappers
//-------------------------------------------------
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
bool VISORproblem::witGetNameAttributeBool(
    witGetNameWitBooleanStar  witGetFunc,
    const std::string &name) const
{
  witBoolean witRetVal;
  witGetFunc(mutableWitRun(), name.c_str(), &witRetVal);
  bool retVal = false;
  if (witRetVal) retVal = true;
  return retVal;
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

std::vector<std::string> VISORproblem::witGetOperProducts(const std::string &opName) const {
  std::vector<std::string> retVal;

  int nBopEntries;
  witGetOperationNBopEntries(mutableWitRun(), opName.c_str(), &nBopEntries);

  int b;
  for (b = 0; b < nBopEntries; b++) {
    char *prodPart;
    witGetBopEntryProducedPart(mutableWitRun(), opName.c_str(), b, &prodPart);
    retVal.push_back(prodPart);
    witFree(prodPart);
  }
  return retVal;
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
















WitRun * VISORproblem::mutableWitRun() const { return wr_; }
WitRun * VISORproblem::witRun() { return wr_; }



// default constructor
VISORproblem::VISORproblem()
    :
    wr_(nullptr),
    nPeriods_(0)
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




  //witSetIndependentOffsets( witRun(), WitTRUE );
  //witSetNPeriods(witRun(),30);
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
    wr_(nullptr),
    nPeriods_(source.nPeriods_)
{
  gutsOfCopy(source);
}

// assignment operator. Not yet supported
VISORproblem&
VISORproblem::operator=(const VISORproblem& rhs)
{
  if (this != &rhs) {
    if ( wr_ != nullptr ) gutsOfDestructor();
    wr_ = nullptr;
    gutsOfCopy(rhs);
  }
  return *this;
}

// copy object
void VISORproblem::gutsOfCopy( const VISORproblem& source )
{
  // Copy Wit Run
  assert(wr_==nullptr);
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
  VisorRelFltEq eq(1.e-05);

  {
    VISORproblem prob;

    prob.setNPeriods(12);
    assert( prob.getNPeriods() == 12 );

    prob.setTitle("wittitle");
    assert( prob.getTitle() == "wittitle");

  }



  {
    // Test materials
    VISORproblem prob;
    assert(prob.getNPeriods()==0);
    prob.setNPeriods(25);
    assert(prob.getNPeriods()==25);

  }


  //std::vector<float> vol = p1.getPartDemandShipVol("0000000P1413","980","980");
  //assert(eq(vol[0],0.0));
}








