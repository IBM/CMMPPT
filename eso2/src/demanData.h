#ifndef ESO2_demanData_h
#define ESO2_demanData_h

#include "scoFile.h"


class ESO2demanData : public SCOcsvFormatFile {
public:

  // Fields
  getFieldH(srcLoc);
  getFloatFieldH(demanCost);
  getFloatFieldH(demanTime);
  
  // preferred constructor
  ESO2demanData(std::string filename)
    : SCOcsvFormatFile(filename)
  {  
    fieldSeperator(',');
    strip(true);
    //insertField("mtm");
    //insertField("plantLoc");
    //insertField("period");
    //insertField("supply");
    //insertField("wfmv");
    hasHeaderRecord(3);
    commentRecords(true);
  };

  // compute key for storing record in dictionary
  static std::string key( 
    const std::string & srcLoc ) 
  {
    std::string retVal = srcLoc;
    return retVal;
  };
  
  // destructor
  ~ESO2demanData(){/*nothing to do here*/};
  
  // Self-test
  static void test()
  {
    ESO2demanData file("../data/example1/demanData.csv");
    
    // Test reading first record

    assert( file.srcLoc(0) == "pok" );
    assert( file.demanCostAsFloat(0) == 12.0f );
    assert( file.demanTimeAsFloat(0) == 0.0f );

    assert( file.srcLoc(1) == "pok" );
    assert( file.demanCostAsFloat(1) == 13.0f );
    assert( file.demanTimeAsFloat(1) == 1.0f );
    
    // Test reading record by key    
    assert( file.demanCostAsFloat(file.key("pok")) == 13.0f );

  }
  
protected:
  
  // compute key for storing record in dictionary
  virtual std::string key( unsigned int recNum ) const
  {
    std::string retVal = key(
      srcLoc(recNum) );
    return retVal;
  };

private:
  
  // default constructor -- don't use!
  ESO2demanData();
  
  // copy constructor  -- don't use!
  ESO2demanData( const ESO2demanData& source );
  
  // assignment operator -- don't use!
  ESO2demanData&
  operator=(const ESO2demanData& rhs);

  
};



class ESO2demanDataIterator : public SCOfileIterator {
public:
  
  // fields 
  getFieldFileWithIteratorH(srcLoc);
  getFloatFieldFileWithIteratorH(demanCost);
  getFloatFieldFileWithIteratorH(demanTime);

  
  // preferred constructor
  ESO2demanDataIterator(const ESO2demanData & file)
    :SCOfileIterator(file)
  { /* Nothing to do here */};
  
  // destructor
  ~ESO2demanDataIterator(){/* nothing to do here */};
  
  // Self-test
  static void test()
  {
    ESO2demanData file("../data/example1/demanData.csv");
    ESO2demanDataIterator fileIter(file);
    
    // loop once for each record
    int recNo=0;
    for ( ; fileIter()!=NULL; ) {    
      if ( recNo == 0 ) {
        
        assert( fileIter.srcLoc() == "pok" );
        assert( fileIter.demanCostAsFloat() == 12.0f );
        assert( fileIter.demanTimeAsFloat() == 0.0f );

      }
      recNo++;
    }
    
  }

private:
  
  
  // default constructor -- don't use!
  ESO2demanDataIterator();
  
  // copy constructor  -- don't use!
  ESO2demanDataIterator( const ESO2demanDataIterator& source );
  
  // assignment operator -- don't use!
  ESO2demanDataIterator&
  operator=(const ESO2demanDataIterator& rhs);

  
};
#endif
