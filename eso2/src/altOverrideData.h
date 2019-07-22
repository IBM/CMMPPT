#ifndef ESO2_altOverrideData_h
#define ESO2_altOverrideData_h

#include "scoFile.h"


class ESO2altOverrideData : public SCOcsvFormatFile {
public:

  // Fields
  getFieldH(part);
  getFieldH(altPart);
  getFieldH(loc);
  getFloatFieldH(altConvCost);
  getFloatFieldH(altConvTime);
  
  // preferred constructor
  ESO2altOverrideData(std::string filename)
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
    const std::string & altPart, 
    const std::string & loc ) 
  {
    //char periodchar[100];
    //sprintf( periodchar, "%d", period );
    std::string retVal = part+"$+&"+loc+"$+&"+altPart;
    return retVal;
  };
  
  // destructor
  ~ESO2altOverrideData(){/*nothing to do here*/};
  
  // Self-test
  static void test()
  {
    ESO2altOverrideData file("../data/example1/altOverrideData.csv");
    
    // Test reading first record

    assert( file.part(0)=="p1");
    assert( file.altPart(0) == "altPart1" );
    assert( file.loc(0) == "pok" );
    assert( file.altConvCostAsFloat(0) == 1.0f );
    assert( file.altConvTimeAsFloat(0) == 2.0f );

    
    assert( file.altConvCostAsFloat(key("p1","altPart1","pok"))==1.0f);
   
  }
  
protected:
  
  // compute key for storing record in dictionary
  virtual std::string key( unsigned int recNum ) const
  {
    std::string retVal = key(
      part(recNum),
      altPart(recNum),
      loc(recNum) );
    return retVal;
  };

private:
  
  // default constructor -- don't use!
  ESO2altOverrideData();
  
  // copy constructor  -- don't use!
  ESO2altOverrideData( const ESO2altOverrideData& source );
  
  // assignment operator -- don't use!
  ESO2altOverrideData&
  operator=(const ESO2altOverrideData& rhs);

  
};



class ESO2altOverrideDataIterator : public SCOfileIterator {
public:
    
  // Fields
  getFieldFileWithIteratorH(part);
  getFieldFileWithIteratorH(altPart);
  getFieldFileWithIteratorH(loc);
  getFloatFieldFileWithIteratorH(altConvCost);
  getFloatFieldFileWithIteratorH(altConvTime);
  
  
  // preferred constructor
  ESO2altOverrideDataIterator(const ESO2altOverrideData & file)
    :SCOfileIterator(file)
  { /* Nothing to do here */};
  
  // destructor
  ~ESO2altOverrideDataIterator(){/* nothing to do here */};
  
  // Self-test
  static void test()
  {
    ESO2altOverrideData file("../data/example1/altOverrideData.csv");
    ESO2altOverrideDataIterator fileIter(file);
    
    // loop once for each record
    int recNo=0;
    for ( ; fileIter()!=NULL; ) {    
      if ( recNo == 0 ) {
        
        assert( fileIter.part()=="p1");
        assert( fileIter.altPart() == "altPart1" );
        assert( fileIter.loc() == "pok" );
        assert( fileIter.altConvCostAsFloat() == 1.0f );
        assert( fileIter.altConvTimeAsFloat() == 2.0f );
          
      }
      recNo++;
    }
    
  }

private:
  
  
  // default constructor -- don't use!
  ESO2altOverrideDataIterator();
  
  // copy constructor  -- don't use!
  ESO2altOverrideDataIterator( const ESO2altOverrideDataIterator& source );
  
  // assignment operator -- don't use!
  ESO2altOverrideDataIterator&
  operator=(const ESO2altOverrideDataIterator& rhs);

  
};
#endif
