#ifndef ESO2_partInterplantData_h
#define ESO2_partInterplantData_h

#include "scoFile.h"


class ESO2partInterplantData : public SCOcsvFormatFile {
public:

  // Fields;
  getFieldH(srcLoc);
  getFieldH(destLoc);
  getFloatFieldH(defaultMoveCost);
  getFloatFieldH(defaultMoveTime);
  
  // preferred constructor
  ESO2partInterplantData(std::string filename)
    : SCOcsvFormatFile(filename)
  {  
    fieldSeperator(',');
    strip(true);
    //insertField("feature");
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
    const std::string & srcLoc, 
    const std::string & destLoc ) 
  {
    std::string retVal = 
      srcLoc+"$+&"
      +destLoc+"$+&";
    return retVal;
  };
  
  // destructor
  ~ESO2partInterplantData(){/*nothing to do here*/};
  
  // Self-test
  static void test()
  {
    ESO2partInterplantData file("../data/example1/partInterplantData.csv");
    
    // Test reading first record

    assert( file.srcLoc(0) == "pok" );
    assert( file.destLoc(0) == "wat" );
    assert( file.defaultMoveCostAsFloat(0) == 3.0f );
    assert( file.defaultMoveTimeAsFloat(0) == 2.0f );

    std::string k = file.key("wat","pok");
    assert( file.srcLoc(k) == "wat" );
    assert( file.destLoc(k) == "pok" );
    assert( file.defaultMoveCostAsFloat(k) == 3.0f );
    assert( file.defaultMoveTimeAsFloat(k) == 2.0f );

  }
  
protected:
  
  // compute key for storing record in dictionary
  virtual std::string key( unsigned int recNum ) const
  {
    std::string retVal = key(
      srcLoc(recNum),
      destLoc(recNum) );
    return retVal;
  };

private:
  
  // default constructor -- don't use!
  ESO2partInterplantData();
  
  // copy constructor  -- don't use!
  ESO2partInterplantData( const ESO2partInterplantData& source );
  
  // assignment operator -- don't use!
  ESO2partInterplantData&
  operator=(const ESO2partInterplantData& rhs);

  
};



class ESO2partInterplantDataIterator : public SCOfileIterator {
public:
    
  // Fields
  getFieldFileWithIteratorH(srcLoc);
  getFieldFileWithIteratorH(destLoc);
  getFloatFieldFileWithIteratorH(defaultMoveCost);
  getFloatFieldFileWithIteratorH(defaultMoveTime);

  
  // preferred constructor
  ESO2partInterplantDataIterator(const ESO2partInterplantData & file)
    :SCOfileIterator(file)
  { /* Nothing to do here */};
  
  // destructor
  ~ESO2partInterplantDataIterator(){/* nothing to do here */};
  
  // Self-test
  static void test()
  {
    ESO2partInterplantData file("../data/example1/partInterplantData.csv");
    ESO2partInterplantDataIterator fileIter(file);
    
    // loop once for each record
    int recNo=0;
    for ( ; fileIter()!=NULL; ) {    
      if ( recNo == 0 ) {
        
        assert( fileIter.srcLoc() == "pok" );
        assert( fileIter.destLoc() == "wat" );
        assert( fileIter.defaultMoveCostAsFloat() == 3.0f );
        assert( fileIter.defaultMoveTimeAsFloat() == 2.0f );
          
      }
      recNo++;
    }
    
  }

private:
  
  
  // default constructor -- don't use!
  ESO2partInterplantDataIterator();
  
  // copy constructor  -- don't use!
  ESO2partInterplantDataIterator( const ESO2partInterplantDataIterator& source );
  
  // assignment operator -- don't use!
  ESO2partInterplantDataIterator&
  operator=(const ESO2partInterplantDataIterator& rhs);

  
};
#endif
