#ifndef ESO2_featureConversionOverrideData_h
#define ESO2_featureConversionOverrideData_h

#include "scoFile.h"


class ESO2featureConversionOverrideData : public SCOcsvFormatFile {
public:

  // Fields
  getFieldH(part);
  getFieldH(feature);
  getFieldH(plantLoc);
  getIntFieldH(period);
  getFloatFieldH(cost);
  getFloatFieldH(usageTime);
  
  // preferred constructor
  ESO2featureConversionOverrideData(std::string filename)
    : SCOcsvFormatFile(filename)
  {  
    fieldSeperator(',');
    strip(true);
    //insertField("feature");
    //insertField("plantLoc");
    //insertField("period");
    //insertField("supply");
    //insertField("wfmv");
    hasHeaderRecord(6);
    commentRecords(true);
  };

  // compute key for storing record in dictionary
  static std::string key( 
    const std::string & part, 
    const std::string & feature, 
    const std::string & plantLoc, 
    int period ) 
  {
    char periodchar[100];
    sprintf( periodchar, "%d", period );
    std::string retVal = part+"$+&"+feature+"$+&"+plantLoc+"$+&"+periodchar;
    return retVal;
  };
  
  // destructor
  ~ESO2featureConversionOverrideData(){/*nothing to do here*/};
  
  // Self-test
  static void test()
  {
    ESO2featureConversionOverrideData file("../data/example1/featureConversionOverrideData.csv");
    
    // Test reading first record
/*
"u3","f1","wat",1,7.7,1.0
"u3","f1","wat",2,7.6,1.0
"u3","f1","wat",3,7.5,1.0
"u3","f1","wat",4,7.4,1.0
*/
    assert( file.part(0)=="u3");
    assert( file.feature(0) == "f1" );
    assert( file.plantLoc(0) == "wat" );
    assert( file.periodAsInt(0) == 1 );
    assert( file.costAsFloat(0) == 7.7f );
    assert( file.usageTimeAsFloat(0) == 1.0f );

    std::string k = key("u3","f1","wat",2);
    assert( file.part(k)=="u3");
    assert( file.feature(k) == "f1" );
    assert( file.plantLoc(k) == "wat" );
    assert( file.periodAsInt(k) == 2 );
    assert( file.costAsFloat(k) == 7.6f );
    assert( file.usageTimeAsFloat(k) == 1.0f );

  }
  
protected:
  
  // compute key for storing record in dictionary
  virtual std::string key( unsigned int recNum ) const
  {
    std::string retVal = key(
      part(recNum),
      feature(recNum),
      plantLoc(recNum),
      periodAsInt(recNum) );
    return retVal;
  };

private:
  
  // default constructor -- don't use!
  ESO2featureConversionOverrideData();
  
  // copy constructor  -- don't use!
  ESO2featureConversionOverrideData( const ESO2featureConversionOverrideData& source );
  
  // assignment operator -- don't use!
  ESO2featureConversionOverrideData&
  operator=(const ESO2featureConversionOverrideData& rhs);

  
};



class ESO2featureConversionOverrideDataIterator : public SCOfileIterator {
public:
    
  // Fields
  getFieldFileWithIteratorH(part);
  getFieldFileWithIteratorH(feature);
  getFieldFileWithIteratorH(plantLoc);
  getIntFieldFileWithIteratorH(period);
  getFloatFieldFileWithIteratorH(cost);
  getFloatFieldFileWithIteratorH(usageTime);

  
  // preferred constructor
  ESO2featureConversionOverrideDataIterator(const ESO2featureConversionOverrideData & file)
    :SCOfileIterator(file)
  { /* Nothing to do here */};
  
  // destructor
  ~ESO2featureConversionOverrideDataIterator(){/* nothing to do here */};
  
  // Self-test
  static void test()
  {
    ESO2featureConversionOverrideData file("../data/example1/featureConversionOverrideData.csv");
    ESO2featureConversionOverrideDataIterator fileIter(file);
    
    // loop once for each record
    int recNo=0;
    for ( ; fileIter()!=NULL; ) {    
      if ( recNo == 0 ) {
        
        assert( fileIter.part()=="u3");
        assert( fileIter.feature() == "f1" );
        assert( fileIter.plantLoc() == "wat" );
        assert( fileIter.periodAsInt() == 1 );
        assert( fileIter.costAsFloat() == 7.7f );
        assert( fileIter.usageTimeAsFloat() == 1.0f );
          
      }
      recNo++;
    }
    
  }

private:
  
  
  // default constructor -- don't use!
  ESO2featureConversionOverrideDataIterator();
  
  // copy constructor  -- don't use!
  ESO2featureConversionOverrideDataIterator( const ESO2featureConversionOverrideDataIterator& source );
  
  // assignment operator -- don't use!
  ESO2featureConversionOverrideDataIterator&
  operator=(const ESO2featureConversionOverrideDataIterator& rhs);

  
};
#endif
