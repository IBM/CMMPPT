#ifndef ESO2_mtmInterplantOverrideData_h
#define ESO2_mtmInterplantOverrideData_h

#include "scoFile.h"


class ESO2mtmInterplantOverrideData : public SCOcsvFormatFile {
public:

  // Fields
  getFieldH(mtm);
  getFieldH(srcLoc);
  getFieldH(destLoc);
  getIntFieldH(period);
  getFloatFieldH(moveCost);
  getFloatFieldH(moveTime);
  
  // preferred constructor
  ESO2mtmInterplantOverrideData(std::string filename)
    : SCOcsvFormatFile(filename)
  {  
    fieldSeperator(',');
    strip(true);
    //insertField("mtm");
    //insertField("plantLoc");
    //insertField("period");
    //insertField("supply");
    //insertField("wfmv");
    hasHeaderRecord(6);
    commentRecords(true);
  };

  // compute key for storing record in dictionary
  static std::string key( 
    const std::string & mtm, 
    const std::string & srcLoc,
    const std::string & destLoc, 
    int period ) 
  {
    char periodchar[100];
    sprintf( periodchar, "%d", period );
    std::string retVal = mtm+"$+&"+srcLoc+"$+&"+destLoc+"$+&"+periodchar;
    return retVal;
  };
  
  // destructor
  ~ESO2mtmInterplantOverrideData(){/*nothing to do here*/};
  
  // Self-test
  static void test()
  {
    ESO2mtmInterplantOverrideData file("../data/example1/mtmData.csv");
    
    // Test reading first record

    std::string mtm = file.mtm(0);
    assert( mtm=="m1");
    assert( file.srcLoc(0) == "pok" );
    assert( file.destLoc(0) == "wat" );
    assert( file.periodAsInt(0) == 0 );
    assert( file.moveCostAsFloat(0) == 5.0f );
    assert( file.moveTimeAsFloat(0) == 1.0f );

    assert( file.mtm(1) =="m1");
    assert( file.srcLoc(1) == "pok" );
    assert( file.destLoc(1) == "wat" );
    assert( file.periodAsInt(1) == 1 );
    assert( file.moveCostAsFloat(1) == 5.1f );
    assert( file.moveTimeAsFloat(1) == 1.0f );

    // Test reading record by key    
    assert( file.moveCostAsFloat(file.key("m1","pok","wat",2)) == 5.2f );

    //assert( file.numRecords() == 16 );

  }
  
protected:
  
  // compute key for storing record in dictionary
  virtual std::string key( unsigned int recNum ) const
  {
    std::string retVal = key(
      mtm(recNum),
      srcLoc(recNum),
      destLoc(recNum),
      periodAsInt(recNum) );
    return retVal;
  };

private:
  
  // default constructor -- don't use!
  ESO2mtmInterplantOverrideData();
  
  // copy constructor  -- don't use!
  ESO2mtmInterplantOverrideData( const ESO2mtmInterplantOverrideData& source );
  
  // assignment operator -- don't use!
  ESO2mtmInterplantOverrideData&
  operator=(const ESO2mtmInterplantOverrideData& rhs);

  
};



class ESO2mtmInterplantOverrideDataIterator : public SCOfileIterator {
public:
  
  // fields 
  getFieldFileWithIteratorH(mtm);
  getFieldFileWithIteratorH(srcLoc);
  getFieldFileWithIteratorH(destLoc);
  getIntFieldFileWithIteratorH(period);
  getFloatFieldFileWithIteratorH(moveCost);
  getFloatFieldFileWithIteratorH(moveTime);
  
  // preferred constructor
  ESO2mtmInterplantOverrideDataIterator(const ESO2mtmInterplantOverrideData & file)
    :SCOfileIterator(file)
  { /* Nothing to do here */};
  
  // destructor
  ~ESO2mtmInterplantOverrideDataIterator(){/* nothing to do here */};
  
  // Self-test
  static void test()
  {
    ESO2mtmInterplantOverrideData file("../data/example1/mtmData.csv");
    ESO2mtmInterplantOverrideDataIterator fileIter(file);
    
    // loop once for each record
    int recNo=0;
    for ( ; fileIter()!=NULL; ) {    
      if ( recNo == 0 ) {
        
        assert( fileIter.mtm() == "m1" );
        assert( fileIter.srcLoc() == "pok" );
        assert( fileIter.destLoc() == "wat" );
        assert( fileIter.periodAsInt() == 0 );
        assert( fileIter.moveCostAsFloat() == 5.0f );
        assert( fileIter.moveTimeAsFloat() == 1.0f );

        //assert( fileIter.recNum() == 11 );
          
      }
      recNo++;
    }
    
    // number of records including comments
    assert( fileIter.recNum()==27);
  }

private:
  
  
  // default constructor -- don't use!
  ESO2mtmInterplantOverrideDataIterator();
  
  // copy constructor  -- don't use!
  ESO2mtmInterplantOverrideDataIterator( const ESO2mtmInterplantOverrideDataIterator& source );
  
  // assignment operator -- don't use!
  ESO2mtmInterplantOverrideDataIterator&
  operator=(const ESO2mtmInterplantOverrideDataIterator& rhs);

  
};
#endif
