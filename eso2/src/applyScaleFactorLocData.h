#ifndef ESO2_applyScaleFactorLocData_h
#define ESO2_applyScaleFactorLocData_h

#include "scoFile.h"


class ESO2applyScaleFactorLocData : public SCOcsvFormatFile {
public:

  // Fields
  getFieldH(plantLoc);
  getIntFieldH(partDemandFileNumber);
  
  // preferred constructor
  ESO2applyScaleFactorLocData(std::string filename)
    : SCOcsvFormatFile(filename)
  {  
    fieldSeperator(',');
    strip(true);
    //insertField("feature");
    //insertField("plantLoc");
    //insertField("period");
    //insertField("supply");
    //insertField("wfmv");
    hasHeaderRecord(2);
    commentRecords(true);
  };

  // compute key for storing record in dictionary
  static std::string key( 
    const std::string & plantLoc, 
    int partDemandFileNumber ) 
  {
    char fileNumber[100];
    sprintf( fileNumber, "%d", partDemandFileNumber );
    std::string retVal = plantLoc+"$+&"+fileNumber;
    return retVal;
  };
  
  // destructor
  ~ESO2applyScaleFactorLocData(){/*nothing to do here*/};
  
  // Self-test
  static void test()
  {
    ESO2applyScaleFactorLocData file("../data/example7/applyScaleFactorLocData.csv");
    
    // Test reading first record

    assert( file.plantLoc(0)=="GLA");
    assert( file.partDemandFileNumber(0) == "1" );
    assert( file.partDemandFileNumberAsInt(0) == 1 );
    assert( file.plantLoc(1)=="GLA");
    assert( file.partDemandFileNumber(1) == "2" );
    assert( file.partDemandFileNumberAsInt(1) == 2 );
    
    assert( file.plantLoc(key("GLA",1))=="GLA");
   
  }
  
protected:
  
  // compute key for storing record in dictionary
  virtual std::string key( unsigned int recNum ) const
  {
    std::string retVal = key(
      plantLoc(recNum),
      partDemandFileNumberAsInt(recNum) );
    return retVal;
  };

private:
  
  // default constructor -- don't use!
  ESO2applyScaleFactorLocData();
  
  // copy constructor  -- don't use!
  ESO2applyScaleFactorLocData( const ESO2applyScaleFactorLocData& source );
  
  // assignment operator -- don't use!
  ESO2applyScaleFactorLocData&
  operator=(const ESO2applyScaleFactorLocData& rhs);

  
};



class ESO2applyScaleFactorLocDataIterator : public SCOfileIterator {
public:
    
  // Fields
  getFieldFileWithIteratorH(plantLoc);
  getIntFieldFileWithIteratorH(partDemandFileNumber);  
  
  // preferred constructor
  ESO2applyScaleFactorLocDataIterator(const ESO2applyScaleFactorLocData & file)
    :SCOfileIterator(file)
  { /* Nothing to do here */};
  
  // destructor
  ~ESO2applyScaleFactorLocDataIterator(){/* nothing to do here */};
  
  // Self-test
  static void test()
  {
    ESO2applyScaleFactorLocData file("../data/example7/applyScaleFactorLocData.csv");
    ESO2applyScaleFactorLocDataIterator fileIter(file);

    // loop once for each record
    int recNo=0;
    for ( ; fileIter()!=NULL; ) {    
      if ( recNo == 0 ) {
        assert( fileIter.plantLoc()=="GLA");
        assert( fileIter.partDemandFileNumber() == "1" );
        assert( fileIter.partDemandFileNumberAsInt() == 1 );
      }
      recNo++;
    }
    
  }

private:
  
  
  // default constructor -- don't use!
  ESO2applyScaleFactorLocDataIterator();
  
  // copy constructor  -- don't use!
  ESO2applyScaleFactorLocDataIterator( const ESO2applyScaleFactorLocDataIterator& source );
  
  // assignment operator -- don't use!
  ESO2applyScaleFactorLocDataIterator&
  operator=(const ESO2applyScaleFactorLocDataIterator& rhs);

  
};
#endif
