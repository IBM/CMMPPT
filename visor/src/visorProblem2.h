#ifndef VISORproblem2_h
#define VISORproblem2_h

//#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <cassert>

#include <wit.h>

//#include "partInterplant.h"

//typedef std::pair<std::string,int> ESO2stringIntPair;

extern "C" {typedef witReturnCode (STDCALL * witSetNameFloatStar) (
   WitRun * const theWitRun,
   const char * const name,
   const float * const floatVec);}

extern "C" {typedef witReturnCode (STDCALL * witGetNameFloatStarStar) (
   WitRun * const theWitRun,
   const char * const name,
   float ** floatVec);}

extern "C" {typedef witReturnCode (STDCALL * witSetName3FloatStar) (
   WitRun * const theWitRun,
   const char * const operationName,
   const float * const hardLower,
   const float * const softLower,
   const float * const hardUpper);}

extern "C" {typedef witReturnCode (STDCALL * witGetName3FloatStar) (
   WitRun * const theWitRun,
   const char * const operationName,
   float ** hardLower,
   float ** softLower,
   float ** hardUpper);}

extern "C" {typedef witReturnCode (STDCALL * witSetNameIndexFloatStar) (
   WitRun * const theWitRun,
   const char * const name,
   const int bopEntryIndex,
   const float * const floatVec);}

extern "C" {typedef witReturnCode (STDCALL * witSetNameIndexIndexFloatStar) (
   WitRun * const theWitRun,
   const char * const name,
   const int bopEntryIndex,
   const int subEntryIndex,
   const float * const floatVec);}

extern "C" {typedef witReturnCode (STDCALL * witGetNameIndexFloatStarStar) (
   WitRun * const theWitRun,
   const char * const name,
   const int bopEntryIndex,
   float ** floatVec);}

extern "C" {typedef witReturnCode (STDCALL * witGetNameIndexIntStarStar) (
   WitRun * const theWitRun,
   const char * const name,
   const int bopEntryIndex,
   int ** intVec);}

extern "C" {typedef witReturnCode (STDCALL * witGetNameIndexIndexFloatStarStar) (
   WitRun * const theWitRun,
   const char * const name,
   const int bopEntryIndex,
   const int subEntryIndex,
   float ** floatVec);}

extern "C" {typedef witReturnCode (STDCALL * witGetNameIndexIndexIntStarStar) (
   WitRun * const theWitRun,
   const char * const name,
   const int bopEntryIndex,
   const int subEntryIndex,
   int ** intVec);}

extern "C" {typedef witReturnCode (STDCALL * witGetNameIndexIndexIntStar) (
   WitRun * const theWitRun,
   const char * const consumingOperationName,
   const int bomEntryIndex,
   const int subsBomEntryIndex,
   int * earliestPeriod);}

extern "C" {typedef witReturnCode (STDCALL * witSetDblNameFloatStar) (
   WitRun * const theWitRun,
   const char * const partName,
   const char * const demandName,
   const float * const floatVec);}

extern "C" {typedef witReturnCode (STDCALL * witGetDblNameFloatStarStar) (
   WitRun * const theWitRun,
   const char * const partName,
   const char * const demandName,
   float ** floatVec);}

class VISORproblem2  {
public:

  //----------------------------------
  // Methods for global attributes.
  //----------------------------------
  // Set/Get String Describing Problem
  void setTitle(const std::string & title);
  std::string getTitle() const;

  // Set/Get Number of Periods in Problem
  void setNPeriods(int nP);
  inline int getNPeriods() const
  {
    int retVal = nPeriods_;
    return retVal;
  }

  // write wit data file
  void writeWitData( std::string filename );






  // Set OSL Logfile name
  void setSolverLogFileName(const std::string & name);

  //----------------------
  // Methods for Visor.
  //----------------------
  void addVisor( const std::string & name, const std::string & location);
  bool locationExists( const std::string & loc );



  //----------------------
  // Printer Hospital demands
  //----------------------




  //------------------------------------------
  // Visor name methods
  //-----------------------------------------
  std::string aggregateVisorName(const std::string & location );
  std::string aggregateOperName(const std::string & location );
  std::string noSupplyVisorName(const std::string & location );
  std::string visorPartName(const std::string & name, const std::string & location );
  std::string baseLocationName(const std::string & location );


 // String Utilities
  static std::string textBetween(const std::string & src, const std::string & before,const std::string & after);
  static std::string textAfter(const std::string & src, const std::string & before);
  static bool beginsWith(const std::string & src, const std::string & begString);
  static bool contains(const std::string & haystack, const std::string & needle);

  //--------------------
  // Solve Methods.
  //--------------------
  void solve();


  //----------------------------------------------------------------------

  void writeWitData( std::string filename ) const;

  //----------------------------------------------------------------------

  static float bigM() { return 1000000.0f; };

  //----------------------------------------------------------------------

  // default constructor
  VISORproblem2();

  // copy constructor
  VISORproblem2( const VISORproblem2& source );

  // assignment operator
  VISORproblem2&
  operator=(const VISORproblem2& rhs);

  // destructor
  ~VISORproblem2();


  // Self-test
  static void test();


  public:
  inline std::vector<float> witFloatStarToStlVec(const float * inputVec ) const
  {
    std::vector<float>  retVal;
    int np = getNPeriods();
    int i;
    for ( i=0; i<np; ++i ) retVal.push_back(inputVec[i]);
    return retVal;
  };

  inline std::vector<int> witIntStarToStlVec(const int * inputVec ) const
  {
    std::vector<int>  retVal;
    int np = getNPeriods();
    int i;
    for ( i=0; i<np; ++i ) retVal.push_back(inputVec[i]);
    return retVal;
  };

  inline float * stlVecToFloatStar(const std::vector<float> & inputVector ) const
  {
    size_t len = inputVector.size();
    float * retVal = NULL;
    if ( len > 0 ) {
      //retVal = (float *) malloc(len*sizeof(float));
      retVal = new float[len];
      size_t i;
      for ( i=0; i<len; ++i ) retVal[i] = inputVector[i];
    }
    return retVal;
  };

  inline float * floatToConstFloatStar(float input ) const
  {
    int len = getNPeriods();
    //float * retVal = (float *) malloc(len*sizeof(float));
    float * retVal = new float[len];
    int i;
    for ( i=0; i<len; ++i ) retVal[i] = input;
    return retVal;
  };

  inline std::vector<float> floatToStlVec(float input ) const
  {
    int len = getNPeriods();
    std::vector<float> retVal;
    int i;
    for ( i=0; i<len; ++i ) retVal.push_back(input);
    return retVal;
  };
#if 0
  inline std::vector<int> intToStlVec(int input ) const
  {
    int len = getNPeriods();
    std::vector<int> retVal;
    int i;
    for ( i=0; i<len; ++i ) retVal.push_back(input);
    return retVal;
  };
#endif

private:
  // Set element of Part/Operation float* attribute, given period and fltValue
  void witSetNameAttribute(
    witGetNameFloatStarStar witGetFunc,
    witSetNameFloatStar     witSetFunc,
    const std::string & name,
    int period,
    float supply );

  // Set Part/Operation float* attribute, given std::vector<float>
  void witSetNameAttribute(
    witSetNameFloatStar     witSetFunc,
    const std::string & name,
    const std::vector<float> & value );

  // Set Part/Operation float* attribute, given float
  void witSetNameAttribute(
    witSetNameFloatStar     witSetFunc,
    const std::string & name,
    float value );

  // Set Part/Operation bound attribute, given three std::vector<float> vectors
  void witSetNameBoundAttribute(
    witSetName3FloatStar     witSetFunc,
    const std::string & name,
    const std::vector<float> & hlb,
    const std::vector<float> & slb,
    const std::vector<float> & hub );


  std::vector<std::string> witGetOperProducts(const std::string & opName) const;


  // Set element on bom/bop float* attribute, given period and fltValue
  void witSetArcAttribute(
    witGetNameIndexFloatStarStar witGetFunc,
    witSetNameIndexFloatStar     witSetFunc,
    const std::string & opName,
    int index,
    int period,
    float flt );

  // Set element on bom/bop float* attribute, given std::vector<float>
  void witSetArcAttribute(
    witSetNameIndexFloatStar     witSetFunc,
    const std::string & opName,
    int index,
    const std::vector<float> & value );

  // Set element on bom/bop float* attribute, given float
  void witSetArcAttribute(
    witSetNameIndexFloatStar     witSetFunc,
    const std::string & opName,
    int index,
    float value );

  // Set element on demand float* attribute, given period and fltValue
  void witSetDemandAttribute(
    witGetDblNameFloatStarStar      witGetFunc,
    witSetDblNameFloatStar          witSetFunc,
    const std::string & partName,
    const std::string & demandName,
    int period,
    float flt );

  // Set element on demand float* attribute, given std::vector<float>
  void witSetDemandAttribute(
    witSetDblNameFloatStar          witSetFunc,
    const std::string & partName,
    const std::string & demandName,
    const std::vector<float> & fltVec );

  // Get vector of wit part/Operation float* attribute
  std::vector<float> witGetNameAttribute(
    witGetNameFloatStarStar witGetFunc,
    const std::string & name )const;

  // Get vector of bom/bop float* attribute
  std::vector<float> witGetArcAttribute(
    witGetNameIndexFloatStarStar     witGetFunc,
    const std::string & opName,
    int index )const;
  // Get vector of bom/bop int* attribute
  std::vector<int> witGetArcAttribute(
    witGetNameIndexIntStarStar     witGetFunc,
    const std::string & opName,
    int index )const;

 // Get vector of sub float* attribute
  std::vector<float> witGetSubArcAttribute(
    witGetNameIndexIndexFloatStarStar     witGetFunc,
    const std::string & opName,
    int index, int subIndex )const;
  // Get vector of sub int* attribute
  std::vector<int> witGetSubArcAttribute(
    witGetNameIndexIndexIntStarStar     witGetFunc,
    const std::string & opName,
    int index, int subIndex )const;
  // Get vector of sub int attribute
  int witGetSubArcAttribute(
    witGetNameIndexIndexIntStar     witGetFunc,
    const std::string & opName,
    int index, int subIndex )const;

  // Set sub float* attribute, given period and fltValue
  void witSetSubArcAttribute(
    witGetNameIndexIndexFloatStarStar witGetFunc,
    witSetNameIndexIndexFloatStar     witSetFunc,
    const std::string & opName,
    int index, int subIndex,
    int period,
    float flt );

  // Get vector of wit demand float* attribute
  std::vector<float> witGetDemandAttribute(
    witGetDblNameFloatStarStar witGetFunc,
    const std::string & partName,
    const std::string & demandName )const;


  // Get Part/Operation bound attributes, given three std::vector<float> vectors
  void witGetNameBoundAttribute(
    witGetName3FloatStar     witGetFunc,
    const std::string & name,
    std::vector<float> & hlb,
    std::vector<float> & slb,
    std::vector<float> & hub )const;



  void gutsOfCopy( const VISORproblem2& source );
  void gutsOfDestructor();


  WitRun * mutableWitRun() const;
  WitRun * witRun();

  mutable WitRun * wr_;

  int nPeriods_; // cached for performance

  std::set<std::string> locationBaseNames_;
  //std::set<std::string> printerBaseNames_;

};


#endif
