#ifndef ESO2_altExcludeLocData_h
#define ESO2_altExcludeLocData_h

#include "scoFile.h"


class ESO2altExcludeLocData : public SCOcsvFormatFile {
public:

  // Fields
  getFieldH(part);
  getFieldH(altPart);
  getFieldH(excludeLoc);
  
  // preferred constructor
  ESO2altExcludeLocData(std::string filename)
    : SCOcsvFormatFile(filename)
  {  
    fieldSeperator(',');
    strip(true);
    //insertField("feature");
    //insertField("plantLoc");
    //insertField("period");
    //insertField("supply");
    //insertField("wfmv");
    hasHeaderRecord(3);
    commentRecords(true);
  };

  // compute key for storing record in dictionary
  static std::string key( 
    const std::string & part, 
    const std::string & altPart, 
    const std::string & excludeLoc ) 
  {
    //char periodchar[100];
    //sprintf( periodchar, "%d", period );
    std::string retVal = part+"$+&"+altPart+"$+&"+excludeLoc;
    return retVal;
  };
  
  // destructor
  ~ESO2altExcludeLocData(){/*nothing to do here*/};
  
  // Self-test
  static void test()
  {
    ESO2altExcludeLocData file("../data/example1/altExcludeLocData.csv");
    
    // Test reading first record

    assert( file.part(0)=="p1");
    assert( file.altPart(0) == "altPart1" );
    assert( file.excludeLoc(0) == "roch" );
    
    assert( file.excludeLoc(key("p1","altPart1","roch"))=="roch");
   
  }
  
protected:
  
  // compute key for storing record in dictionary
  virtual std::string key( unsigned int recNum ) const
  {
    std::string retVal = key(
      part(recNum),
      altPart(recNum),
      excludeLoc(recNum));
    return retVal;
  };

private:
  
  // default constructor -- don't use!
  ESO2altExcludeLocData();
  
  // copy constructor  -- don't use!
  ESO2altExcludeLocData( const ESO2altExcludeLocData& source );
  
  // assignment operator -- don't use!
  ESO2altExcludeLocData&
  operator=(const ESO2altExcludeLocData& rhs);

  
};



class ESO2altExcludeLocDataIterator : public SCOfileIterator {
public:
    
  // Fields
  getFieldFileWithIteratorH(part);
  getFieldFileWithIteratorH(altPart);
  getFieldFileWithIteratorH(excludeLoc);
  
  
  // preferred constructor
  ESO2altExcludeLocDataIterator(const ESO2altExcludeLocData & file)
    :SCOfileIterator(file)
  { /* Nothing to do here */};
  
  // destructor
  ~ESO2altExcludeLocDataIterator(){/* nothing to do here */};
  
  // Self-test
  static void test()
  {
    ESO2altExcludeLocData file("../data/example1/altExcludeLocData.csv");
    ESO2altExcludeLocDataIterator fileIter(file);
    
    // loop once for each record
    int recNo=0;
    for ( ; fileIter()!=NULL; ) {    
      if ( recNo == 0 ) {
        
        assert( fileIter.part()=="p1");
        assert( fileIter.altPart() == "altPart1" );
        assert( fileIter.excludeLoc() == "roch" );
      }
      recNo++;
    }
    
  }

private:
  
  
  // default constructor -- don't use!
  ESO2altExcludeLocDataIterator();
  
  // copy constructor  -- don't use!
  ESO2altExcludeLocDataIterator( const ESO2altExcludeLocDataIterator& source );
  
  // assignment operator -- don't use!
  ESO2altExcludeLocDataIterator&
  operator=(const ESO2altExcludeLocDataIterator& rhs);

  
};
#endif
