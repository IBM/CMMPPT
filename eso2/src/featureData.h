#ifndef ESO2_FeatureData_h
#define ESO2_FeatureData_h

#include "scoFile.h"


class ESO2featureData : public SCOcsvFormatFile {
public:

  // Fields
  getFieldH(feature);
  getFieldH(plantLoc);
  getIntFieldH(period);
  getFloatFieldH(supply);
  getFloatFieldH(wfmv);
  
  // preferred constructor
  ESO2featureData(std::string filename)
    : SCOcsvFormatFile(filename)
  {  
    fieldSeperator(',');
    strip(true);
    //insertField("feature");
    //insertField("plantLoc");
    //insertField("period");
    //insertField("supply");
    //insertField("wfmv");
    hasHeaderRecord(5);
    commentRecords(true);
  };

  // compute key for storing record in dictionary
  static std::string key( 
    const std::string & feature, 
    const std::string & plantLoc, 
    int period ) 
  {
    char periodchar[100];
    sprintf( periodchar, "%d", period );
    std::string retVal = feature+"$+&"+plantLoc+"$+&"+periodchar;
    return retVal;
  };
  
  // destructor
  ~ESO2featureData(){/*nothing to do here*/};
  
  // Self-test
  static void test()
  {
    ESO2featureData file("../data/example1/featureData.csv");
    
    // Test reading first record

    std::string feature = file.feature(0);
    assert( feature=="f1");
    assert( file.plantLoc(0) == "pok" );
    assert( file.periodAsInt(0) == 1 );
    assert( file.supplyAsFloat(0) == 20.0f );
    assert( file.wfmvAsFloat(0) == 5.0f );

    assert( file.feature(1) == "f1" );
    assert( file.plantLoc(1) == "pok" );
    assert( file.periodAsInt(1) == 2 );
    assert( file.supplyAsFloat(1) == 21.0f );
    assert( file.wfmvAsFloat(1) == 5.4f );
    
    // Test reading record by key    
    assert( file.wfmvAsFloat(file.key("f2","wat",3)) == 5.0f );
    assert( file.supplyAsFloat(file.key("f2","wat",3)) == 30.0f );

    assert( file.numRecords() == 16 );

  }
  
protected:
  
  // compute key for storing record in dictionary
  virtual std::string key( unsigned int recNum ) const
  {
    std::string retVal = key(
      feature(recNum),
      plantLoc(recNum),
      periodAsInt(recNum) );
    return retVal;
  };

private:
  
  // default constructor -- don't use!
  ESO2featureData();
  
  // copy constructor  -- don't use!
  ESO2featureData( const ESO2featureData& source );
  
  // assignment operator -- don't use!
  ESO2featureData&
  operator=(const ESO2featureData& rhs);

  
};



class ESO2featureDataIterator : public SCOfileIterator {
public:
  
  // fields 
  getFieldFileWithIteratorH(feature);
  getFieldFileWithIteratorH(plantLoc);
  getIntFieldFileWithIteratorH(period);
  getFloatFieldFileWithIteratorH(supply);
  getFloatFieldFileWithIteratorH(wfmv);

  
  // preferred constructor
  ESO2featureDataIterator(const ESO2featureData & file)
    :SCOfileIterator(file)
  { /* Nothing to do here */};
  
  // destructor
  ~ESO2featureDataIterator(){/* nothing to do here */};
  
  // Self-test
  static void test()
  {
    ESO2featureData file("../data/example1/featureData.csv");
    ESO2featureDataIterator fileIter(file);
    
    // loop once for each record
    int recNo=0;
    for ( ; fileIter()!=NULL; ) {    
      if ( recNo == 0 ) {
        
        assert( fileIter.feature() == "f1" );
        assert( fileIter.plantLoc() == "pok" );
        assert( fileIter.periodAsInt() == 1 );
        assert( fileIter.supplyAsFloat() == 20.0f );
        assert( fileIter.wfmvAsFloat() == 5.0f );

        assert( fileIter.recNum() == 9 );
          
      }
      recNo++;
    }
    
    // number of records including comments
    assert( fileIter.recNum()==25);
  }

private:
  
  
  // default constructor -- don't use!
  ESO2featureDataIterator();
  
  // copy constructor  -- don't use!
  ESO2featureDataIterator( const ESO2featureDataIterator& source );
  
  // assignment operator -- don't use!
  ESO2featureDataIterator&
  operator=(const ESO2featureDataIterator& rhs);

  
};
#endif
