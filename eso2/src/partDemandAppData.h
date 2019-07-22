#ifndef ESO2partDemandAppData_h
#define ESO2partDemandAppData_h

#include <utility>

#include "appData.h"
#include "stringStringIntTriple.h"


//#include "stringStringIntIntTuple.h"


#if 0
typedef std::map<ESO2stringStringIntTriple,float,ESO2stringStringIntTripleCompareFunctionObject> ESO2mapStringStringIntToFloat;
typedef std::map<ESO2stringStringIntTriple,float,ESO2stringStringIntTripleCompareFunctionObject>::const_iterator ESO2mapStringStringIntToFloatConstIterator;
typedef std::map<int,ESO2mapStringStringIntToFloat> ESO2mapIntToMapStringStringIntToFloat;
#endif

#if 0
typedef std::map<ESO2stringStringIntIntTuple,float,ESO2stringStringIntIntTupleCompareFunctionObject> ESO2mapStringStringIntIntToFloat;
typedef std::map<ESO2stringStringIntIntTuple,float,ESO2stringStringIntIntTupleCompareFunctionObject>::const_iterator ESO2mapStringStringIntIntToFloatConstIterator;
typedef std::map<int,ESO2mapStringStringIntIntToFloat> ESO2mapIntToMapStringStringIntIntToFloat;
#endif

typedef std::map<ESO2stringStringIntTriple,std::pair<int,float>,ESO2stringStringIntTripleCompareFunctionObject> ESO2mapStringStringIntToIntFloatPair;
typedef std::map<ESO2stringStringIntTriple,std::pair<int,float>,ESO2stringStringIntTripleCompareFunctionObject>::const_iterator ESO2mapStringStringIntToIntFloatPairConstIterator;
typedef std::map<int,ESO2mapStringStringIntToIntFloatPair> ESO2mapIntToMapStringStringIntToIntFloatPair;

class ESO2partDemandAppData : public ESO2appData {
public:
  
  std::set<std::string> getAttributeNames() const;
  
  /*
  void addPeggedItem(
    const std::string & attribName, int shipPeriod, 
    const std::string & item, 
    const std::string & loc,
    int period, 
    float quantity );
    
  void getPeggedItems(
    const std::string & attribName, int shipPeriod,
    std::vector<std::string> &items,
    std::vector<std::string> &locs,
    std::vector<int> &periods,
    std::vector<float> &quantities ) const;
    */

  void addPeggedItem(
    const std::string & attribName, int shipPeriod, 
    const std::string & item, 
    const std::string & loc,
    int period,
    int supplyChainDepth,
    float quantity );
  void getPeggedItems(
    const std::string & attribName, int shipPeriod,
    std::vector<std::string> &items,
    std::vector<std::string> &locs,
    std::vector<int> &periods,
    std::vector<int> &depths,
    std::vector<float> &quantities ) const;

  void setPriority(int);
  int getPriority() const;

  void setWac(int period, float wac);
  float getWac(int period) const;
  std::vector<float> getWac() const;

  void clearPegging();
    
private:
  // default constructor
  ESO2partDemandAppData();
public:
  ESO2partDemandAppData(int nPeriods);
  
  // destructor
  virtual ~ESO2partDemandAppData();
  
  // copy constructor
  ESO2partDemandAppData( const ESO2partDemandAppData& source );
  
  // assignment operator
  ESO2partDemandAppData&
    operator=(const ESO2partDemandAppData& rhs);

  virtual ESO2appData * clone() const;

  // Self-test
  static void test();

private:

  //std::map<std::string,ESO2mapIntToMapStringStringIntToFloat> peggedAttributes_;
  //std::map<std::string,ESO2mapIntToMapStringStringIntIntToFloat> peggedAttributes_;
  std::map<std::string,ESO2mapIntToMapStringStringIntToIntFloatPair> peggedAttributes_;
  std::set<std::string> attributeNames_;

  int priority_;
  std::vector<float> wac_;
  
};



#endif
