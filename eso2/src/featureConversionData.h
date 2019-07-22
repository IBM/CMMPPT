#ifndef ESO2_featureConversionData_h
#define ESO2_featureConversionData_h

#include "scoFile.h"


class ESO2featureConversionData : public SCOcsvFormatFile {
public:

  // Fields
  getFieldH(plantLoc);
  getFloatFieldH(cost);
  getFloatFieldH(usageTime);
  
  // preferred constructor
  ESO2featureConversionData(std::string filename)
    : SCOcsvFormatFile(filename)
  {  
    fieldSeperator(',');
    strip(true);
    //insertField("feature");
    //insertField("plantLoc");
    //insertField("period");
    //insertField("supply");
    //insertField("wfmv");
    hasHeaderRecord(3);
    commentRecords(true);
  };

  // compute key for storing record in dictionary
  static std::string key( 
    const std::string & plantLoc ) 
  {
    std::string retVal = plantLoc;
    return retVal;
  };
  
  // destructor
  ~ESO2featureConversionData(){/*nothing to do here*/};
  
  // Self-test
  static void test()
  {
    ESO2featureConversionData file("../data/example1/featureConversionData.csv");
    
    // Test reading first record

    assert( file.plantLoc(0) == "pok" );
    assert( file.costAsFloat(0) == 5.5f );
    assert( file.usageTimeAsFloat(0) == 0.5f );

    
    assert( file.plantLoc(key("wat")) == "wat" );
    assert( file.costAsFloat(key("wat")) == 6.6f );
    assert( file.usageTimeAsFloat(key("wat")) == 0.0f );

  }
  
protected:
  
  // compute key for storing record in dictionary
  virtual std::string key( unsigned int recNum ) const
  {
    std::string retVal = key(
      plantLoc(recNum) );
    return retVal;
  };

private:
  
  // default constructor -- don't use!
  ESO2featureConversionData();
  
  // copy constructor  -- don't use!
  ESO2featureConversionData( const ESO2featureConversionData& source );
  
  // assignment operator -- don't use!
  ESO2featureConversionData&
  operator=(const ESO2featureConversionData& rhs);

  
};



class ESO2featureConversionDataIterator : public SCOfileIterator {
public:
    
  // Fields
  getFieldFileWithIteratorH(plantLoc);
  getFloatFieldFileWithIteratorH(cost);
  getFloatFieldFileWithIteratorH(usageTime);

  
  // preferred constructor
  ESO2featureConversionDataIterator(const ESO2featureConversionData & file)
    :SCOfileIterator(file)
  { /* Nothing to do here */};
  
  // destructor
  ~ESO2featureConversionDataIterator(){/* nothing to do here */};
  
  // Self-test
  static void test()
  {
    ESO2featureConversionData file("../data/example1/featureConversionData.csv");
    ESO2featureConversionDataIterator fileIter(file);
    
    // loop once for each record
    int recNo=0;
    for ( ; fileIter()!=NULL; ) {    
      if ( recNo == 0 ) {
        
        assert( fileIter.plantLoc() == "pok" );
        assert( fileIter.costAsFloat() == 5.5f );
        assert( fileIter.usageTimeAsFloat() == 0.5f );
          
      }
      recNo++;
    }
    
  }

private:
  
  
  // default constructor -- don't use!
  ESO2featureConversionDataIterator();
  
  // copy constructor  -- don't use!
  ESO2featureConversionDataIterator( const ESO2featureConversionDataIterator& source );
  
  // assignment operator -- don't use!
  ESO2featureConversionDataIterator&
  operator=(const ESO2featureConversionDataIterator& rhs);

  
};
#endif
