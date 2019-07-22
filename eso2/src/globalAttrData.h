#ifndef ESO2_GLOBALATTRDATA_h
#define ESO2_GLOBALATTRDATA_h

#include "scoFile.h"


class ESO2globalAttrData : public SCOcsvFormatFile {
public:
  
  // fields  
  getFieldH(attrName);  
  getFieldH(attrValue);
  
  // preferred constructor
  ESO2globalAttrData(std::string filename)
    : SCOcsvFormatFile(filename)
  {  
    fieldSeperator(',');
    strip(true);
    hasHeaderRecord(2);
    commentRecords(true);
  };
  
  // destructor
  ~ESO2globalAttrData()  
  {
    // Nothing to do here
  }
  
  // Self-test
  static void test()
  {
    
    ESO2globalAttrData file("../data/example1/globalAttrData.csv");
    
    assert( file.attrValue("title") == "Sample Title" );
    assert( file.fieldAsString("title","attrValue") == "Sample Title" );
    assert( file.fieldAsInt("nPeriods","attrValue") == 4 );
    assert( file.fieldAsInt("nWfmvScaleFactors","attrValue") == 4 );
    assert( file.fieldAsFloat("wfmvScaleFactor1","attrValue") == 1. );
    assert( file.fieldAsFloat("wfmvScaleFactor2","attrValue") == 0.8f );
    assert( file.fieldAsFloat("wfmvScaleFactor3","attrValue") == 0.6f );
    assert( file.fieldAsFloat("wfmvScaleFactor4","attrValue") == 0.4f );

  }
  
protected:
  
  // compute key for storing record in dictionary
  virtual
    std::string
    key( unsigned recNum ) const
  {
    std::string retVal = fieldAsString(recNum,"attrName");
    return retVal;
  }
  
private:
  
  
  // default constructor -- don't use!
  ESO2globalAttrData();
  
  // copy constructor  -- don't use!
  ESO2globalAttrData( const ESO2globalAttrData& source );
  
  // assignment operator -- don't use!
  ESO2globalAttrData&
  operator=(const ESO2globalAttrData& rhs);

  
};


#if 0
class ESO2globalAttrDataIterator : public SCOfileIterator {
public:
  
  // fields
  //std::string locName() const;

  // preferred constructor
  ESO2globalAttrDataIterator(const ESO2globalAttrData &);
  
  // destructor
  ~ESO2globalAttrDataIterator();
  
  // Self-test
  static void test();

private:
  
  
  // default constructor -- don't use!
  ESO2globalAttrDataIterator();
  
  // copy constructor  -- don't use!
  ESO2globalAttrDataIterator( const ESO2globalAttrDataIterator& source );
  
  // assignment operator -- don't use!
  ESO2globalAttrDataIterator&
  operator=(const ESO2globalAttrDataIterator& rhs);

  
};
#endif
#endif