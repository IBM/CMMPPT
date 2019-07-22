#ifndef ESO2_partDemandData_h
#define ESO2_partDemandData_h

#include "scoFile.h"


class ESO2partDemandData : public SCOcsvFormatFile {
public:

  // Fields
  getFieldH(part);
  getFieldH(custLoc);
  getFieldH(plantLoc);
  getIntFieldH(period);
  getFloatFieldH(demandVol);
  getFloatFieldH(wac);
  getIntFieldH(etnFlag);
  
  // preferred constructor
  ESO2partDemandData(std::string filename)
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
    const std::string & part, 
    const std::string & custLoc, 
    const std::string & plantLoc, 
    int period ) 
  {
    char periodchar[100];
    sprintf( periodchar, "%d", period );
    std::string retVal = part+"$+&"+custLoc+"$+&"+plantLoc+"$+&"+periodchar;
    return retVal;
  };
  
  // destructor
  ~ESO2partDemandData(){/*nothing to do here*/};
  
  // Self-test
  static void test()
  {
    ESO2partDemandData file("../data/example1/partDemandData.csv");
    
    // Test reading first record

    assert( file.part(0)=="p2");
    assert( file.custLoc(0) == "pok" );
    assert( file.plantLoc(0) == "pok" );
    assert( file.periodAsInt(0) == 1 );
    assert( file.demandVolAsFloat(0) == 1.5f );
    assert( file.wacAsFloat(0) == 25.5f );
    assert( file.etnFlagAsInt(0) == 1 );

    std::string k= file.key("p2","pok","wat",1);
    assert( file.part(k)=="p2");
    assert( file.custLoc(k) == "pok" );
    assert( file.plantLoc(k) == "wat" );
    assert( file.periodAsInt(k) == 1 );
    assert( file.demandVolAsFloat(k) == 2.1f );
    assert( file.wacAsFloat(k) == 20.5f );
    assert( file.etnFlagAsInt(k) == 1 );

  }
  
protected:
  
  // compute key for storing record in dictionary
  virtual std::string key( unsigned int recNum ) const
  {
    std::string retVal = key(
      part(recNum),
      custLoc(recNum),
      plantLoc(recNum),
      periodAsInt(recNum) );
    return retVal;
  };

private:
  
  // default constructor -- don't use!
  ESO2partDemandData();
  
  // copy constructor  -- don't use!
  ESO2partDemandData( const ESO2partDemandData& source );
  
  // assignment operator -- don't use!
  ESO2partDemandData&
  operator=(const ESO2partDemandData& rhs);

  
};



class ESO2partDemandDataIterator : public SCOfileIterator {
public:
    
  // Fields
  getFieldFileWithIteratorH(part);
  getFieldFileWithIteratorH(custLoc);
  getFieldFileWithIteratorH(plantLoc);
  getIntFieldFileWithIteratorH(period);
  getFloatFieldFileWithIteratorH(demandVol);
  getFloatFieldFileWithIteratorH(wac);
  getIntFieldFileWithIteratorH(etnFlag);

  
  // preferred constructor
  ESO2partDemandDataIterator(const ESO2partDemandData & file)
    :SCOfileIterator(file)
  { /* Nothing to do here */};
  
  // destructor
  ~ESO2partDemandDataIterator(){/* nothing to do here */};
  
  // Self-test
  static void test()
  {
    ESO2partDemandData file("../data/example1/partDemandData.csv");
    ESO2partDemandDataIterator fileIter(file);
    
    // loop once for each record
    int recNo=0;
    for ( ; fileIter()!=NULL; ) {    
      if ( recNo == 0 ) {
        
        assert( fileIter.part()=="p2");
        assert( fileIter.custLoc() == "pok" );
        assert( fileIter.plantLoc() == "pok" );
        assert( fileIter.periodAsInt() == 1 );
        assert( fileIter.demandVolAsFloat() == 1.5f );
        assert( fileIter.wacAsFloat() == 25.5f );
        assert( fileIter.etnFlagAsInt() == 1 );
          
      }
      recNo++;
    }
    
  }

private:
  
  
  // default constructor -- don't use!
  ESO2partDemandDataIterator();
  
  // copy constructor  -- don't use!
  ESO2partDemandDataIterator( const ESO2partDemandDataIterator& source );
  
  // assignment operator -- don't use!
  ESO2partDemandDataIterator&
  operator=(const ESO2partDemandDataIterator& rhs);

  
};
#endif
