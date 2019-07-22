#ifndef ESO2_featureXrefData_h
#define ESO2_featureXrefData_h

#include "scoFile.h"


class ESO2featureXrefData : public SCOcsvFormatFile {
public:

  // Fields
  getFieldH(part);
  getFieldH(feature);
  getFieldH(plantLoc);
  getIntFieldH(period);
  getFloatFieldH(prodRate);
  
  // preferred constructor
  ESO2featureXrefData(std::string filename)
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
  ~ESO2featureXrefData(){/*nothing to do here*/};
  
  // Self-test
  static void test()
  {
    ESO2featureXrefData file("../data/example1/featureXrefData.csv");
    
    // Test reading first record
/*  
"u3","f1","wat",1,3.1
"u3","f1","wat",2,3.1
"u3","f1","wat",3,3.1
"u3","f1","wat",4,3.1
*/

    assert( file.part(0)=="u3");
    assert( file.feature(0) == "f1" );
    assert( file.plantLoc(0) == "wat" );
    assert( file.periodAsInt(0) == 1 );
    assert( file.prodRateAsFloat(0) == 3.1f );

    
    assert( file.part(key("u3","f1","wat",2))=="u3");
    assert( file.feature(key("u3","f1","wat",2)) == "f1" );
    assert( file.plantLoc(key("u3","f1","wat",2)) == "wat" );
    assert( file.periodAsInt(key("u3","f1","wat",2)) == 2 );
    assert( file.prodRateAsFloat(key("u3","f1","wat",2)) == 3.1f );
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
  ESO2featureXrefData();
  
  // copy constructor  -- don't use!
  ESO2featureXrefData( const ESO2featureXrefData& source );
  
  // assignment operator -- don't use!
  ESO2featureXrefData&
  operator=(const ESO2featureXrefData& rhs);

  
};



class ESO2featureXrefDataIterator : public SCOfileIterator {
public:
    
  // Fields
  getFieldFileWithIteratorH(part);
  getFieldFileWithIteratorH(feature);
  getFieldFileWithIteratorH(plantLoc);
  getIntFieldFileWithIteratorH(period);
  getFloatFieldFileWithIteratorH(prodRate);

  
  // preferred constructor
  ESO2featureXrefDataIterator(const ESO2featureXrefData & file)
    :SCOfileIterator(file)
  { /* Nothing to do here */};
  
  // destructor
  ~ESO2featureXrefDataIterator(){/* nothing to do here */};
  
  // Self-test
  static void test()
  {
    ESO2featureXrefData file("../data/example1/featureXrefData.csv");
    ESO2featureXrefDataIterator fileIter(file);
    
    // loop once for each record
    int recNo=0;
    for ( ; fileIter()!=NULL; ) {    
      if ( recNo == 0 ) {
        
        assert( fileIter.part()=="u3");
        assert( fileIter.feature() == "f1" );
        assert( fileIter.plantLoc() == "wat" );
        assert( fileIter.periodAsInt() == 1 );
        assert( fileIter.prodRateAsFloat() == 3.1f );
          
      }
      recNo++;
    }
    
  }

private:
  
  
  // default constructor -- don't use!
  ESO2featureXrefDataIterator();
  
  // copy constructor  -- don't use!
  ESO2featureXrefDataIterator( const ESO2featureXrefDataIterator& source );
  
  // assignment operator -- don't use!
  ESO2featureXrefDataIterator&
  operator=(const ESO2featureXrefDataIterator& rhs);

  
};
#endif
