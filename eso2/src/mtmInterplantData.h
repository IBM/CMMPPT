#ifndef ESO2_mtmInterplantData_h
#define ESO2_mtmInterplantData_h

#include "scoFile.h"


class ESO2mtmInterplantData : public SCOcsvFormatFile {
public:

  // Fields
  //getFieldH(mtm);
  getFieldH(srcLoc);
  getFieldH(destLoc);
  getFloatFieldH(defaultMoveCost);
  getFloatFieldH(defaultMoveTime);
  
  // preferred constructor
  ESO2mtmInterplantData(std::string filename)
    : SCOcsvFormatFile(filename)
  {  
    fieldSeperator(',');
    strip(true);
    //insertField("mtm");
    //insertField("plantLoc");
    //insertField("period");
    //insertField("supply");
    //insertField("wfmv");
    hasHeaderRecord(4);
    commentRecords(true);
  };
  
  float defaultMoveCostAsFloat(  
    const std::string & srcLoc, 
    const std::string & destLoc) const
  {
    return defaultMoveCostAsFloat(key(srcLoc,destLoc));
  }

  float defaultMoveTimeAsFloat(  
    const std::string & srcLoc, 
    const std::string & destLoc) const
  {
    return defaultMoveTimeAsFloat(key(srcLoc,destLoc));
  }

  bool keyExists(  
    const std::string & srcLoc, 
    const std::string & destLoc) const
  {
    return SCOfile::keyExists(key(srcLoc,destLoc));
  }

  // compute key for storing record in dictionary
  static std::string key( 
    //const std::string & mtm, 
    const std::string & srcLoc,
    const std::string & destLoc) 
  {
    std::string retVal = srcLoc+"$+&"+destLoc;
    return retVal;
  };
  
  // destructor
  ~ESO2mtmInterplantData(){/*nothing to do here*/};
  
  // Self-test
  static void test()
  {
    ESO2mtmInterplantData file("../data/example1/mtmInterplantData.csv");
    
    // Test reading first record

    //std::string mtm = file.mtm(0);
    //assert( mtm=="m1");
    assert( file.srcLoc(0) == "pok" );
    assert( file.destLoc(0) == "wat" );
    //assert( file.periodAsInt(0) == 0 );
    assert( file.defaultMoveCostAsFloat(0) == 5.0f );
    assert( file.defaultMoveTimeAsFloat(0) == 1.0f );

    //assert( file.mtm(1) =="m1");
    assert( file.srcLoc(1) == "pok" );
    assert( file.destLoc(1) == "wat" );
    //assert( file.periodAsInt(1) == 1 );
    assert( file.defaultMoveCostAsFloat(1) == 5.1f );
    assert( file.defaultMoveTimeAsFloat(1) == 1.0f );

    // Test reading record by key    
    assert( file.defaultMoveCostAsFloat(file.key("pok","wat")) == 5.2f );

    //assert( file.numRecords() == 16 );

  }
  
protected:
  
  // compute key for storing record in dictionary
  virtual std::string key( unsigned int recNum ) const
  {
    std::string retVal = key(
      //mtm(recNum),
      srcLoc(recNum),
      destLoc(recNum) );
    return retVal;
  };

private:
  
  // default constructor -- don't use!
  ESO2mtmInterplantData();
  
  // copy constructor  -- don't use!
  ESO2mtmInterplantData( const ESO2mtmInterplantData& source );
  
  // assignment operator -- don't use!
  ESO2mtmInterplantData&
  operator=(const ESO2mtmInterplantData& rhs);

  
};



class ESO2mtmInterplantDataIterator : public SCOfileIterator {
public:
  
  // fields 
  //getFieldFileWithIteratorH(mtm);
  getFieldFileWithIteratorH(srcLoc);
  getFieldFileWithIteratorH(destLoc);
  //getIntFieldFileWithIteratorH(period);
  getFloatFieldFileWithIteratorH(defaultMoveCost);
  getFloatFieldFileWithIteratorH(defaultMoveTime);
  
  // preferred constructor
  ESO2mtmInterplantDataIterator(const ESO2mtmInterplantData & file)
    :SCOfileIterator(file)
  { /* Nothing to do here */};
  
  // destructor
  ~ESO2mtmInterplantDataIterator(){/* nothing to do here */};
  
  // Self-test
  static void test()
  {
    ESO2mtmInterplantData file("../data/example1/mtmInterplantData.csv");
    ESO2mtmInterplantDataIterator fileIter(file);
    
    // loop once for each record
    int recNo=0;
    for ( ; fileIter()!=NULL; ) {    
      if ( recNo == 0 ) {
        
        //assert( fileIter.mtm() == "m1" );
        assert( fileIter.srcLoc() == "pok" );
        assert( fileIter.destLoc() == "wat" );
        //assert( fileIter.periodAsInt() == 0 );
        assert( fileIter.defaultMoveCostAsFloat() == 5.0f );
        assert( fileIter.defaultMoveTimeAsFloat() == 1.0f );

        //assert( fileIter.recNum() == 11 );
          
      }
      recNo++;
    }
    
    // number of records including comments
    assert( fileIter.recNum()==27);
  }

private:
  
  
  // default constructor -- don't use!
  ESO2mtmInterplantDataIterator();
  
  // copy constructor  -- don't use!
  ESO2mtmInterplantDataIterator( const ESO2mtmInterplantDataIterator& source );
  
  // assignment operator -- don't use!
  ESO2mtmInterplantDataIterator&
  operator=(const ESO2mtmInterplantDataIterator& rhs);

  
};
#endif
