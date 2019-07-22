#ifndef ESO2_altData_h
#define ESO2_altData_h

#include "scoFile.h"


class ESO2altData : public SCOcsvFormatFile {
public:

  // Fields
  getFieldH(part);
  getFieldH(altPart);
  getFieldH(loc);
  getFloatFieldH(altConvCost);
  getFloatFieldH(altConvTime);
  
  // preferred constructor
  ESO2altData(std::string filename)
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
    std::string retVal = part+"$+&"+altPart+"$+&"+loc;
    return retVal;
  };
  
  // destructor
  ~ESO2altData(){/*nothing to do here*/};
  
  // Self-test
  static void test()
  {
    ESO2altData file("../data/example1/altData.csv");
    
    // Test reading first record

    assert( file.part(0)=="p1");
    assert( file.altPart(0) == "altPart1" );
    assert( file.loc(0) == "WilyWonkaChocolateFactory" );
    assert( file.altConvCostAsFloat(0) == 1.0f );
    assert( file.altConvTimeAsFloat(0) == 2.0f );

    
    assert( file.altConvCostAsFloat(key("p1","altPart1","WilyWonkaChocolateFactory"))==1.0f);
   
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
  ESO2altData();
  
  // copy constructor  -- don't use!
  ESO2altData( const ESO2altData& source );
  
  // assignment operator -- don't use!
  ESO2altData&
  operator=(const ESO2altData& rhs);

  
};



class ESO2altDataIterator : public SCOfileIterator {
public:
    
  // Fields
  getFieldFileWithIteratorH(part);
  getFieldFileWithIteratorH(altPart);
  getFieldFileWithIteratorH(loc);
  getFloatFieldFileWithIteratorH(altConvCost);
  getFloatFieldFileWithIteratorH(altConvTime);
  
  
  // preferred constructor
  ESO2altDataIterator(const ESO2altData & file)
    :SCOfileIterator(file)
  { /* Nothing to do here */};
  
  // destructor
  ~ESO2altDataIterator(){/* nothing to do here */};
  
  // Self-test
  static void test()
  {
    ESO2altData file("../data/example1/altData.csv");
    ESO2altDataIterator fileIter(file);
    
    // loop once for each record
    int recNo=0;
    for ( ; fileIter()!=NULL; ) {    
      if ( recNo == 0 ) {
        
        assert( fileIter.part()=="p1");
        assert( fileIter.altPart() == "altPart1" );
        assert( fileIter.loc() == "WilyWonkaChocolateFactory" );
        assert( fileIter.altConvCostAsFloat() == 1.0f );
        assert( fileIter.altConvTimeAsFloat() == 2.0f );
          
      }
      recNo++;
    }
    
  }

private:
  
  
  // default constructor -- don't use!
  ESO2altDataIterator();
  
  // copy constructor  -- don't use!
  ESO2altDataIterator( const ESO2altDataIterator& source );
  
  // assignment operator -- don't use!
  ESO2altDataIterator&
  operator=(const ESO2altDataIterator& rhs);

  
};
#endif
