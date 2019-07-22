#ifndef ESO2_mtmDemanBomData_h
#define ESO2_mtmDemanBomData_h

#include "scoFile.h"


class ESO2mtmDemanBomData : public SCOcsvFormatFile {
public:

  // Fields
  getFieldH(mtm);
  getFieldH(part);
  getFieldH(srcLoc);
  getIntFieldH(period);
  getFloatFieldH(attachRate);
  //getIntFieldH(etnFlag);
  //getFloatFieldH(usageTime);
  
  // preferred constructor
  ESO2mtmDemanBomData(std::string filename)
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
    const std::string & mtm, 
    const std::string & part, 
    const std::string & plantLoc, 
    int period ) 
  {
    char periodchar[100];
    sprintf( periodchar, "%d", period );
    std::string retVal = mtm+"$+&"+part+"$+&"+plantLoc+"$+&"+periodchar;
    return retVal;
  };
  
  // destructor
  ~ESO2mtmDemanBomData(){/*nothing to do here*/};
  
  // Self-test
  static void test()
  {
    ESO2mtmDemanBomData file("../data/example1/mtmDemanBomData.csv");
    
    // Test reading first record

    assert( file.mtm(0)=="m1");
    assert( file.part(0) == "p1" );
    assert( file.srcLoc(0) == "pok" );
    assert( file.periodAsInt(0) == 1 );
    assert( file.attachRateAsFloat(0) == 1.5f );
    //assert( file.etnFlagAsInt(0) == 1 );
    //assert( file.usageTimeAsFloat(0) == 5.5f );

    
    assert( file.mtm(key("m2","p2","wat",2))=="m2");
    assert( file.part(key("m2","p2","wat",2)) == "p2" );
    assert( file.srcLoc(key("m2","p2","wat",2)) == "wat" );
    assert( file.periodAsInt(key("m2","p2","wat",2)) == 2 );
    assert( file.attachRateAsFloat(key("m2","p2","wat",2)) == 2.1f );
    //assert( file.etnFlagAsInt(key("m2","p2","wat",2)) == 0 );
    //assert( file.usageTimeAsFloat(key("m2","p2","wat",1)) == 6.5f );

  }
  
protected:
  
  // compute key for storing record in dictionary
  virtual std::string key( unsigned int recNum ) const
  {
    std::string retVal = key(
      mtm(recNum),
      part(recNum),
      srcLoc(recNum),
      periodAsInt(recNum) );
    return retVal;
  };

private:
  
  // default constructor -- don't use!
  ESO2mtmDemanBomData();
  
  // copy constructor  -- don't use!
  ESO2mtmDemanBomData( const ESO2mtmDemanBomData& source );
  
  // assignment operator -- don't use!
  ESO2mtmDemanBomData&
  operator=(const ESO2mtmDemanBomData& rhs);

  
};



class ESO2mtmDemanBomDataIterator : public SCOfileIterator {
public:
    
  // Fields
  getFieldFileWithIteratorH(mtm);
  getFieldFileWithIteratorH(part);
  getFieldFileWithIteratorH(srcLoc);
  getIntFieldFileWithIteratorH(period);
  getFloatFieldFileWithIteratorH(attachRate);
  //getIntFieldFileWithIteratorH(etnFlag);
  //getFloatFieldFileWithIteratorH(usageTime);

  
  // preferred constructor
  ESO2mtmDemanBomDataIterator(const ESO2mtmDemanBomData & file)
    :SCOfileIterator(file)
  { /* Nothing to do here */};
  
  // destructor
  ~ESO2mtmDemanBomDataIterator(){/* nothing to do here */};
  
  // Self-test
  static void test()
  {
    ESO2mtmDemanBomData file("../data/example1/mtmDemanBomData.csv");
    ESO2mtmDemanBomDataIterator fileIter(file);
    
    // loop once for each record
    int recNo=0;
    for ( ; fileIter()!=NULL; ) {    
      if ( recNo == 0 ) {
        
        assert( fileIter.mtm()=="m1");
        assert( fileIter.part() == "p1" );
        assert( fileIter.srcLoc() == "pok" );
        assert( fileIter.periodAsInt() == 1 );
        assert( fileIter.attachRateAsFloat() == 1.5f );
        //assert( fileIter.etnFlagAsInt() == 1 );
        //assert( fileIter.usageTimeAsFloat() == 5.5f );
          
      }
      recNo++;
    }
    
  }

private:
  
  
  // default constructor -- don't use!
  ESO2mtmDemanBomDataIterator();
  
  // copy constructor  -- don't use!
  ESO2mtmDemanBomDataIterator( const ESO2mtmDemanBomDataIterator& source );
  
  // assignment operator -- don't use!
  ESO2mtmDemanBomDataIterator&
  operator=(const ESO2mtmDemanBomDataIterator& rhs);

  
};
#endif
