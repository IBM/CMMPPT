#ifndef ESO2_MtmData_h
#define ESO2_MtmData_h

#include "scoFile.h"


class ESO2mtmData : public SCOcsvFormatFile {
public:

  // Fields
  getFieldH(mtm);
  getFieldH(plantLoc);
  getIntFieldH(period);
  getFloatFieldH(supply);
  getFloatFieldH(wfmv);
  
  // preferred constructor
  ESO2mtmData(std::string filename)
    : SCOcsvFormatFile(filename)
  {  
    fieldSeperator(',');
    strip(true);
    //insertField("mtm");
    //insertField("plantLoc");
    //insertField("period");
    //insertField("supply");
    //insertField("wfmv");
    hasHeaderRecord(5);
    commentRecords(true);
  };

  // compute key for storing record in dictionary
  static std::string key( 
    const std::string & mtm, 
    const std::string & plantLoc, 
    int period ) 
  {
    char periodchar[100];
    sprintf( periodchar, "%d", period );
    std::string retVal = mtm+"$+&"+plantLoc+"$+&"+periodchar;
    return retVal;
  };
  
  // destructor
  ~ESO2mtmData(){/*nothing to do here*/};
  
  // Self-test
  static void test()
  {
    ESO2mtmData file("../data/example1/mtmData.csv");
    
    // Test reading first record

    std::string mtm = file.mtm(0);
    assert( mtm=="m1");
    assert( file.plantLoc(0) == "pok" );
    assert( file.periodAsInt(0) == 1 );
    assert( file.supplyAsFloat(0) == 20.0f );
    assert( file.wfmvAsFloat(0) == 5.4f );

    assert( file.mtm(1) == "m1" );
    assert( file.plantLoc(1) == "pok" );
    assert( file.periodAsInt(1) == 2 );
    assert( file.supplyAsFloat(1) == /*21.0f*/ 0.0f );
    assert( file.wfmvAsFloat(1) == 5.4f );
    
    // Test reading record by key    
    assert( file.wfmvAsFloat(file.key("m2","wat",3)) == 5.0f );
    assert( file.supplyAsFloat(file.key("m2","wat",3)) == /*30.0f*/ 0.0f );

    assert( file.numRecords() == 16 );

  }
  
protected:
  
  // compute key for storing record in dictionary
  virtual std::string key( unsigned int recNum ) const
  {
    std::string retVal = key(
      mtm(recNum),
      plantLoc(recNum),
      periodAsInt(recNum) );
    return retVal;
  };

private:
  
  // default constructor -- don't use!
  ESO2mtmData();
  
  // copy constructor  -- don't use!
  ESO2mtmData( const ESO2mtmData& source );
  
  // assignment operator -- don't use!
  ESO2mtmData&
  operator=(const ESO2mtmData& rhs);

  
};



class ESO2mtmDataIterator : public SCOfileIterator {
public:
  
  // fields 
  getFieldFileWithIteratorH(mtm);
  getFieldFileWithIteratorH(plantLoc);
  getIntFieldFileWithIteratorH(period);
  getFloatFieldFileWithIteratorH(supply);
  getFloatFieldFileWithIteratorH(wfmv);
  
  // preferred constructor
  ESO2mtmDataIterator(const ESO2mtmData & file)
    :SCOfileIterator(file)
  { /* Nothing to do here */};
  
  // destructor
  ~ESO2mtmDataIterator(){/* nothing to do here */};
  
  // Self-test
  static void test()
  {
    ESO2mtmData file("../data/example1/mtmData.csv");
    ESO2mtmDataIterator fileIter(file);
    
    // loop once for each record
    int recNo=0;
    for ( ; fileIter()!=NULL; ) {    
      if ( recNo == 0 ) {
        
        assert( fileIter.mtm() == "m1" );
        assert( fileIter.plantLoc() == "pok" );
        assert( fileIter.periodAsInt() == 1 );
        assert( fileIter.supplyAsFloat() == 20.0f );
        assert( fileIter.wfmvAsFloat() == 5.4f );

        //assert( fileIter.recNum() == 11 );
          
      }
      recNo++;
    }
    
    // number of records including comments
    //assert( fileIter.recNum()==27);
  }

private:
  
  
  // default constructor -- don't use!
  ESO2mtmDataIterator();
  
  // copy constructor  -- don't use!
  ESO2mtmDataIterator( const ESO2mtmDataIterator& source );
  
  // assignment operator -- don't use!
  ESO2mtmDataIterator&
  operator=(const ESO2mtmDataIterator& rhs);

  
};
#endif
