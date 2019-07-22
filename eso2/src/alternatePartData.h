#ifndef ESO2_alternatePartData_h
#define ESO2_alternatePartData_h

#include "scoFile.h"


class ESO2alternatePartData : public SCOcsvFormatFile {
public:

  // Fields
  getFieldH(upLevelPart);
  getFieldH(downLevelPart);
  getFieldH(plantLoc);
  getIntFieldH(period);
  getFloatFieldH(prodRate);
  getFloatFieldH(cost);
  getFloatFieldH(usageTime);
  
  // preferred constructor
  ESO2alternatePartData(std::string filename)
    : SCOcsvFormatFile(filename)
  {  
    fieldSeperator(',');
    strip(true);
    //insertField("feature");
    //insertField("plantLoc");
    //insertField("period");
    //insertField("supply");
    //insertField("wfmv");
    hasHeaderRecord(7);
    commentRecords(true);
  };

  // compute key for storing record in dictionary
  static std::string key( 
    const std::string & upLevelPart, 
    const std::string & downLevelPart, 
    const std::string & plantLoc, 
    int period ) 
  {
    char periodchar[100];
    sprintf( periodchar, "%d", period );
    std::string retVal = upLevelPart+"$+&"+downLevelPart+"$+&"+plantLoc+"$+&"+periodchar;
    return retVal;
  };
  
  // destructor
  ~ESO2alternatePartData(){/*nothing to do here*/};
  
  // Self-test
  static void test()
  {
    ESO2alternatePartData file("../data/example1/alternatePartData.csv");
    
    // Test reading first record

    assert( file.upLevelPart(0)=="u1");
    assert( file.downLevelPart(0) == "p1" );
    assert( file.plantLoc(0) == "pok" );
    assert( file.periodAsInt(0) == 1 );
    assert( file.prodRateAsFloat(0) == 1.5f );
    assert( file.costAsFloat(0) == 5.5f );
    assert( file.usageTimeAsFloat(0) == 0.5f );

    
    assert( file.upLevelPart(key("u2","p2","wat",2))=="u2");
    assert( file.downLevelPart(key("u2","p2","wat",2)) == "p2" );
    assert( file.plantLoc(key("u2","p2","wat",2)) == "wat" );
    assert( file.periodAsInt(key("u2","p2","wat",2)) == 2 );
    assert( file.prodRateAsFloat(key("u2","p2","wat",2)) == 2.1f );
    assert( file.costAsFloat(key("u2","p2","wat",2)) == 6.5f );
    assert( file.usageTimeAsFloat(key("u2","p2","wat",2)) == 0.0f );

  }
  
protected:
  
  // compute key for storing record in dictionary
  virtual std::string key( unsigned int recNum ) const
  {
    std::string retVal = key(
      upLevelPart(recNum),
      downLevelPart(recNum),
      plantLoc(recNum),
      periodAsInt(recNum) );
    return retVal;
  };

private:
  
  // default constructor -- don't use!
  ESO2alternatePartData();
  
  // copy constructor  -- don't use!
  ESO2alternatePartData( const ESO2alternatePartData& source );
  
  // assignment operator -- don't use!
  ESO2alternatePartData&
  operator=(const ESO2alternatePartData& rhs);

  
};



class ESO2alternatePartDataIterator : public SCOfileIterator {
public:
    
  // Fields
  getFieldFileWithIteratorH(upLevelPart);
  getFieldFileWithIteratorH(downLevelPart);
  getFieldFileWithIteratorH(plantLoc);
  getIntFieldFileWithIteratorH(period);
  getFloatFieldFileWithIteratorH(prodRate);
  getFloatFieldFileWithIteratorH(cost);
  getFloatFieldFileWithIteratorH(usageTime);

  
  // preferred constructor
  ESO2alternatePartDataIterator(const ESO2alternatePartData & file)
    :SCOfileIterator(file)
  { /* Nothing to do here */};
  
  // destructor
  ~ESO2alternatePartDataIterator(){/* nothing to do here */};
  
  // Self-test
  static void test()
  {
    ESO2alternatePartData file("../data/example1/alternatePartData.csv");
    ESO2alternatePartDataIterator fileIter(file);
    
    // loop once for each record
    int recNo=0;
    for ( ; fileIter()!=NULL; ) {    
      if ( recNo == 0 ) {
        
        assert( fileIter.upLevelPart()=="u1");
        assert( fileIter.downLevelPart() == "p1" );
        assert( fileIter.plantLoc() == "pok" );
        assert( fileIter.periodAsInt() == 1 );
        assert( fileIter.prodRateAsFloat() == 1.5f );
        assert( fileIter.costAsFloat() == 5.5f );
        assert( fileIter.usageTimeAsFloat() == 0.5f );
          
      }
      recNo++;
    }
    
  }

private:
  
  
  // default constructor -- don't use!
  ESO2alternatePartDataIterator();
  
  // copy constructor  -- don't use!
  ESO2alternatePartDataIterator( const ESO2alternatePartDataIterator& source );
  
  // assignment operator -- don't use!
  ESO2alternatePartDataIterator&
  operator=(const ESO2alternatePartDataIterator& rhs);

  
};
#endif
