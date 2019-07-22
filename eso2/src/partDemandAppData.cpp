#if defined(_MSC_VER)
#pragma warning(disable:4786)
#pragma warning(disable:4503)
#endif

#include <iostream>
#include "eso2Problem.h"
#include "partDemandAppData.h"

std::set<std::string> ESO2partDemandAppData::getAttributeNames() const
{
  //return attributeNames_;
  std::vector<std::string> retVal;
#if 0
  retVal.push_back("MtmAcquire");
  retVal.push_back("MtmMoveFrom");
  retVal.push_back("MtmMovedTo");
  retVal.push_back("MtmDisassemble");
  retVal.push_back("PartHarvestedFromMtm");
  retVal.push_back("FeatureAcquire");
  retVal.push_back("FeatureXrefFrom");
  retVal.push_back("PartXrefedTo");
  retVal.push_back("LoosePartAcquire");
  retVal.push_back("PartAltFrom");
  retVal.push_back("PartAltTo");
  retVal.push_back("PartMoveFrom");
  retVal.push_back("PartMovedTo"); 
#endif
#if 0
  std::set<std::string>::const_iterator it;
  for ( it=attributeNames_.begin(); it!=attributeNames_.end(); ++it ) {
    const std::string & an = *it;
    retVal.push_back(an);
  }
  return retVal;
#endif
  return attributeNames_;
}

void ESO2partDemandAppData::setPriority(int p )
{
  priority_=p;
}


int ESO2partDemandAppData::getPriority() const
{
  return priority_;
}
void ESO2partDemandAppData::setWac(int p, float w )
{
  wac_[p]=w;
}


float ESO2partDemandAppData::getWac(int period) const
{
  return wac_[period];
}
std::vector<float> ESO2partDemandAppData::getWac() const
{
  return wac_;
}

#if 0
void ESO2partDemandAppData::addPeggedItem(
  const std::string & attribName, int shipPeriod, 
  const std::string & item, const std::string & loc, int period, float quantity )
{
  addPeggedItem(attribName,shipPeriod,item,loc,period,0,quantity);
}
#endif

void ESO2partDemandAppData::addPeggedItem(
  const std::string & attribName, int shipPeriod, 
  const std::string & item, const std::string & loc, int period, int depth, float quantity )
{
  // if this attribName is new then add it to list of attribNames
  if ( attributeNames_.find(attribName) == attributeNames_.end() ) {
    attributeNames_.insert(attribName);
  }

#if 0
  ESO2mapIntToMapStringStringIntToFloat & mapOuter = peggedAttributes_[attribName];
  ESO2mapStringStringIntToFloat & mapInner = mapOuter[shipPeriod];
  ESO2stringStringIntTriple ilp(item,loc,period);
#endif
  
  ESO2mapIntToMapStringStringIntToIntFloatPair & mapOuter = peggedAttributes_[attribName];
  ESO2mapStringStringIntToIntFloatPair & mapInner = mapOuter[shipPeriod];
  ESO2stringStringIntTriple ilp(item,loc,period);

  std::pair<int,float> & depthQtyPair = mapInner[ilp];
  int curDepth = depthQtyPair.first;
  float qty = depthQtyPair.second;

  depthQtyPair.first = std::max(depth,curDepth);
  depthQtyPair.second += quantity;

}


/*
void ESO2partDemandAppData::getPeggedItems(
  const std::string & attribName,
  int shipPeriod,
  std::vector<std::string> &items,
  std::vector<std::string> &locs,
  std::vector<int> &periods,
  std::vector<float> &quantities ) const
{
  std::vector<int> depths;
  getPeggedItems(
    attribName,
    shipPeriod,
    items,
    locs,
    periods,
    depths,
    quantities 
    );
}
*/

void ESO2partDemandAppData::getPeggedItems(
  const std::string & attribName,
  int shipPeriod,
  std::vector<std::string> &items,
  std::vector<std::string> &locs,
  std::vector<int> &periods,
  std::vector<int> &depths,
  std::vector<float> &quantities ) const
{
  items.clear();
  locs.clear();
  periods.clear();
  depths.clear();
  quantities.clear();

  std::map<std::string,ESO2mapIntToMapStringStringIntToIntFloatPair>::const_iterator itAttrib = peggedAttributes_.find(attribName);
  if ( itAttrib != peggedAttributes_.end() ) {
    const ESO2mapIntToMapStringStringIntToIntFloatPair & mapOuter = (itAttrib->second);
    ESO2mapIntToMapStringStringIntToIntFloatPair::const_iterator mapOuterIt = mapOuter.find(shipPeriod);
    if ( mapOuterIt == mapOuter.end() ) return;

    const ESO2mapStringStringIntToIntFloatPair & mapInner = mapOuterIt->second;

    ESO2mapStringStringIntToIntFloatPairConstIterator it;
    //ESO2mapStringStringIntToFloat::const_iterator it;

    for (it=mapInner.begin(); it!=mapInner.end(); ++it ) {
      const ESO2stringStringIntTriple & ilpd = it->first;
      std::pair<int,float> depthQtyPair = it->second;
      int depth = depthQtyPair.first;
      float q = depthQtyPair.second;
      items.push_back(ilpd.getString1());
      locs.push_back(ilpd.getString2());
      periods.push_back(ilpd.getInt());
      depths.push_back(depth);
      quantities.push_back(q);
    }
  }
}


void ESO2partDemandAppData::clearPegging()
{
  peggedAttributes_.clear();
  attributeNames_.clear();
}

// default constructor
ESO2partDemandAppData::ESO2partDemandAppData()
:
ESO2appData(),peggedAttributes_(),attributeNames_(),priority_(0),wac_()
{}

ESO2partDemandAppData::ESO2partDemandAppData(int nPeriods)
:
ESO2appData(),peggedAttributes_(),attributeNames_(),priority_(0),wac_()
{
  setVector(nPeriods,0.f,wac_);
}

// destructor

ESO2partDemandAppData::~ESO2partDemandAppData()
{}

// copy constructor
ESO2partDemandAppData::ESO2partDemandAppData( const ESO2partDemandAppData& source ):
ESO2appData(source),
peggedAttributes_(source.peggedAttributes_),
attributeNames_(source.attributeNames_),
priority_(source.priority_),
wac_(source.wac_)
{};

// assignment operator
ESO2partDemandAppData&
ESO2partDemandAppData::operator=(const ESO2partDemandAppData& rhs)
{  
  if (this != &rhs) {        
    ESO2appData::operator=(rhs);
    peggedAttributes_ = rhs.peggedAttributes_;
    attributeNames_ = rhs.attributeNames_;
    priority_ = rhs.priority_;
    wac_=rhs.wac_;
  }
  return *this;
}

ESO2appData * ESO2partDemandAppData::clone() const
{ return (new ESO2partDemandAppData(*this));}

// Self-test
void ESO2partDemandAppData::test()
{

  ESO2partDemandAppData ad1;
  std::vector<std::string> mtms;
  std::vector<std::string> locs;
  std::vector<int> periods;
  std::vector<int> depths;
  std::vector<float> quantities;
 
  {
    ESO2partDemandAppData ad0(3);

    ad0.getPeggedItems("AcquireMtm",1,mtms,locs,periods,depths,quantities);
    assert(mtms.size()==0);
    //assert(ad0.getAttributeNames().size() == 0 );

    ad0.addPeggedItem("AcquireMtm",1,"mtm_a","loc1",1,11,2.f);
    ad0.getPeggedItems("AcquireMtm",1,mtms,locs,periods,depths,quantities);
    assert(mtms.size()==1);
    assert(mtms[0]=="mtm_a");
    assert(locs[0]=="loc1");
    assert(periods[0]==1);
    assert(depths[0]==11);
    assert(quantities[0]==2.f);
    //assert(ad0.getAttributeNames().size() == 1 );
    //assert(ad0.getAttributeNames()[0] == "AcquireMtm" );
    
    ad0.addPeggedItem("AcquireMtm",1,"mtm_a","loc2",2,202,2.f);
    ad0.getPeggedItems("AcquireMtm",1,mtms,locs,periods,depths,quantities);
    assert(mtms.size()==2);
    assert(mtms[0]=="mtm_a");
    
    assert(locs.size()==2);
    assert(locs[1]=="loc2");

    assert(periods[0]==1);
    assert(depths[0]==11);
    assert(quantities[0]==2.f);
    assert(mtms[1]=="mtm_a");
    assert(periods[1]==2);
    assert(depths[1]==202);
    assert(quantities[1]==2.f);
    //assert(ad0.getAttributeNames().size() == 1 );
    //assert(ad0.getAttributeNames()[0] == "AcquireMtm" );
    
    ad0.addPeggedItem("AcquireMtm",1,"mtm_b","loc1",2,500,2.f);
    ad0.getPeggedItems("AcquireMtm",1,mtms,locs,periods,depths,quantities);
    assert(mtms.size()==3);
    assert(mtms[0]=="mtm_a");
    assert(periods[0]==1);
    assert(depths[0]==11);
    assert(quantities[0]==2.f);
    assert(mtms[1]=="mtm_a");
    assert(periods[1]==2);
    assert(depths[1]==202);
    assert(quantities[1]==2.f);
    assert(mtms[2]=="mtm_b");
    assert(periods[2]==2);
    assert(depths[2]==500);
    assert(quantities[2]==2.f);
    //assert(ad0.getAttributeNames().size() == 1 );
    //assert(ad0.getAttributeNames()[0] == "AcquireMtm" );

    assert( ad0.getPriority()==0 );
    ad0.setPriority(5);
    assert( ad0.getPriority()==5 );

    assert( ad0.getWac(0)==0.f );
    ad0.setWac(0,2.f);
    ad0.setWac(1,4.f);
    assert( ad0.getWac(0)==2.f );
    assert( ad0.getWac(1)==4.f );
    
    ad1 = ad0;
    
  }

  assert( ad1.getPriority()==5 );
  assert( ad1.getWac()[0]==2.f );
  assert( ad1.getWac()[1]==4.f );
  
  //assert(ad1.getAttributeNames().size() == 1 );
  //assert(ad1.getAttributeNames()[0] == "AcquireMtm" );

   {
    ESO2partDemandAppData ad;

    ad.addPeggedItem("AcquireMtm",1,"mtm_a","loc1",1,11,2.f);
    ad.addPeggedItem("AcquireMtm",1,"mtm_a","loc1",1,12,5.f);
    ad.getPeggedItems("AcquireMtm",1,mtms,locs,periods,depths,quantities);
    assert(mtms.size()==1);
    assert(mtms[0]=="mtm_a");
    assert(locs[0]=="loc1");
    assert(periods[0]==1);
    assert(depths[0]==12);
    assert(quantities[0]==7.f);
   }

  
}
