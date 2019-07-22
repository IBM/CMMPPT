#ifndef ESO2_LoosePartData_h
#define ESO2_LoosePartData_h

#include "scoFile.h"


class ESO2loosePartData : public SCOcsvFormatFile {
public:

  // Fields
  getFieldH(part);
  getFieldH(plantLoc);
  getIntFieldH(period);
  getFloatFieldH(supply);
  getFloatFieldH(wfmv);
  
  // preferred constructor
  ESO2loosePartData(std::string filename)
    : SCOcsvFormatFile(filename)
  {  
    fieldSeperator(',');
    strip(true);
    //insertField("loosePart");
    //insertField("plantLoc");
    //insertField("period");
    //insertField("supply");
    //insertField("wfmv");
    hasHeaderRecord(5);
    commentRecords(true);
  };

  // compute key for storing record in dictionary
  static std::string key( 
    const std::string & loosePart, 
    const std::string & plantLoc, 
    int period ) 
  {
    char periodchar[100];
    sprintf( periodchar, "%d", period );
    std::string retVal = loosePart+"$+&"+plantLoc+"$+&"+periodchar;
    return retVal;
  };
  
  // destructor
  ~ESO2loosePartData(){/*nothing to do here*/};
  
  // Self-test
  static void test()
  {
    ESO2loosePartData file("../data/example1/loosePartData.csv");
    
    // Test reading first record

    std::string loosePart = file.part(0);
    assert( loosePart=="f1");
    assert( file.plantLoc(0) == "wat" );
    assert( file.periodAsInt(0) == 1 );
    assert( file.supplyAsFloat(0) == 2.0f );
    assert( file.wfmvAsFloat(0) == 2.0f );

    assert( file.part(1) == "f1" );
    assert( file.plantLoc(1) == "wat" );
    assert( file.periodAsInt(1) == 2 );
    assert( file.supplyAsFloat(1) == 1.0f );
    assert( file.wfmvAsFloat(1) == 1.0f );
    
    // Test reading record by key    
    assert( file.wfmvAsFloat(file.key("f1","wat",3)) == 0.0f );
    assert( file.supplyAsFloat(file.key("f1","wat",3)) == 0.0f );


  }
  
protected:
  
  // compute key for storing record in dictionary
  virtual std::string key( unsigned int recNum ) const
  {
    std::string retVal = key(
      part(recNum),
      plantLoc(recNum),
      periodAsInt(recNum) );
    return retVal;
  };

private:
  
  // default constructor -- don't use!
  ESO2loosePartData();
  
  // copy constructor  -- don't use!
  ESO2loosePartData( const ESO2loosePartData& source );
  
  // assignment operator -- don't use!
  ESO2loosePartData&
  operator=(const ESO2loosePartData& rhs);

  
};



class ESO2loosePartDataIterator : public SCOfileIterator {
public:
  
  // fields 
  getFieldFileWithIteratorH(part);
  getFieldFileWithIteratorH(plantLoc);
  getIntFieldFileWithIteratorH(period);
  getFloatFieldFileWithIteratorH(supply);
  getFloatFieldFileWithIteratorH(wfmv);

  
  // preferred constructor
  ESO2loosePartDataIterator(const ESO2loosePartData & file)
    :SCOfileIterator(file)
  { /* Nothing to do here */};
  
  // destructor
  ~ESO2loosePartDataIterator(){/* nothing to do here */};
  
  // Self-test
  static void test()
  {
    ESO2loosePartData file("../data/example1/loosePartData.csv");
    ESO2loosePartDataIterator fileIter(file);
    
    // loop once for each record
    int recNo=0;
    for ( ; fileIter()!=NULL; ) {    
      if ( recNo == 0 ) {
        
        assert( fileIter.part() == "f1" );
        assert( fileIter.plantLoc() == "wat" );
        assert( fileIter.periodAsInt() == 1 );
        assert( fileIter.supplyAsFloat() == 2.0f );
        assert( fileIter.wfmvAsFloat() == 2.0f );

        //assert( fileIter.recNum() == 9 );
          
      }
      recNo++;
    }
    
    // number of records including comments
    //assert( fileIter.recNum()==25);
  }

private:
  
  
  // default constructor -- don't use!
  ESO2loosePartDataIterator();
  
  // copy constructor  -- don't use!
  ESO2loosePartDataIterator( const ESO2loosePartDataIterator& source );
  
  // assignment operator -- don't use!
  ESO2loosePartDataIterator&
  operator=(const ESO2loosePartDataIterator& rhs);

  
};
#endif
