#ifndef ESO2_demanOverrideData_h
#define ESO2_demanOverrideData_h

#include "scoFile.h"


class ESO2demanOverrideData : public SCOcsvFormatFile {
public:

  // Fields
  getFieldH(mtm);
  getFieldH(srcLoc);
  getIntFieldH(period);
  getFloatFieldH(demanCost);
  getFloatFieldH(demanTime);
  
  // preferred constructor
  ESO2demanOverrideData(std::string filename)
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
    const std::string & srcLoc, 
    int period ) 
  {
    char periodchar[100];
    sprintf( periodchar, "%d", period );
    std::string retVal = mtm+"$+&"+srcLoc+"$+&"+periodchar;
    return retVal;
  };
  
  // destructor
  ~ESO2demanOverrideData(){/*nothing to do here*/};
  
  // Self-test
  static void test()
  {
    ESO2demanOverrideData file("../data/example1/demanOverrideData.csv");
    
    // Test reading first record

    std::string mtm = file.mtm(0);
    assert( mtm=="m1");
    assert( file.srcLoc(0) == "pok" );
    assert( file.periodAsInt(0) == 1 );
    assert( file.demanCostAsFloat(0) == 12.0f );
    assert( file.demanTimeAsFloat(0) == 0.0f );

    assert( file.mtm(1) == "m1" );
    assert( file.srcLoc(1) == "pok" );
    assert( file.periodAsInt(1) == 2 );
    assert( file.demanCostAsFloat(1) == 13.0f );
    assert( file.demanTimeAsFloat(1) == 1.0f );
    
    // Test reading record by key    
    assert( file.demanCostAsFloat(file.key("m2","wat",3)) == 13.0f );

  }
  
protected:
  
  // compute key for storing record in dictionary
  virtual std::string key( unsigned int recNum ) const
  {
    std::string retVal = key(
      mtm(recNum),
      srcLoc(recNum),
      periodAsInt(recNum) );
    return retVal;
  };

private:
  
  // default constructor -- don't use!
  ESO2demanOverrideData();
  
  // copy constructor  -- don't use!
  ESO2demanOverrideData( const ESO2demanOverrideData& source );
  
  // assignment operator -- don't use!
  ESO2demanOverrideData&
  operator=(const ESO2demanOverrideData& rhs);

  
};



class ESO2demanOverrideDataIterator : public SCOfileIterator {
public:
  
  // fields 
  getFieldFileWithIteratorH(mtm);
  getFieldFileWithIteratorH(srcLoc);
  getIntFieldFileWithIteratorH(period);
  getFloatFieldFileWithIteratorH(demanCost);
  getFloatFieldFileWithIteratorH(demanTime);

  
  // preferred constructor
  ESO2demanOverrideDataIterator(const ESO2demanOverrideData & file)
    :SCOfileIterator(file)
  { /* Nothing to do here */};
  
  // destructor
  ~ESO2demanOverrideDataIterator(){/* nothing to do here */};
  
  // Self-test
  static void test()
  {
    ESO2demanOverrideData file("../data/example1/demanOverrideData.csv");
    ESO2demanOverrideDataIterator fileIter(file);
    
    // loop once for each record
    int recNo=0;
    for ( ; fileIter()!=NULL; ) {    
      if ( recNo == 0 ) {
        
        assert( fileIter.mtm() == "m1" );
        assert( fileIter.srcLoc() == "pok" );
        assert( fileIter.periodAsInt() == 1 );
        assert( fileIter.demanCostAsFloat() == 12.0f );
        assert( fileIter.demanTimeAsFloat() == 0.0f );

        //assert( fileIter.recNum() == 11 );
          
      }
      recNo++;
    }
    
    // number of records including comments
    //assert( fileIter.recNum()==27);
  }

private:
  
  
  // default constructor -- don't use!
  ESO2demanOverrideDataIterator();
  
  // copy constructor  -- don't use!
  ESO2demanOverrideDataIterator( const ESO2demanOverrideDataIterator& source );
  
  // assignment operator -- don't use!
  ESO2demanOverrideDataIterator&
  operator=(const ESO2demanOverrideDataIterator& rhs);

  
};
#endif
