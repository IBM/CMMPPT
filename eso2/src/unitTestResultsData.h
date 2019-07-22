#ifndef ESO2_unitTestResultsData_h
#define ESO2_unitTestResultsData_h

#include "scoFile.h"


class ESO2unitTestResultsData : public SCOcsvFormatFile {
public:

  // Fields
  getIntFieldH(tc);
  getIntFieldH(perishLimit);
  getIntFieldH(p);
  getIntFieldH(sf);
  getIntFieldH(allowInventoryHoldingAtDemandLocation);
  getIntFieldH(removeCycles);
  getFloatFieldH(objValue);
  
  // preferred constructor
  ESO2unitTestResultsData(std::string filename)
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
    int tc,
    int perishLimit,
    int p,
    int sf,
    int allowInventoryHoldingAtDemandLocation,
    int removeCycles) 
  {
    char tcchar[100];
    sprintf( tcchar, "%d", tc );
    char perishLimitChar[100];
    sprintf( perishLimitChar, "%d", perishLimit );
    char pchar[100];
    sprintf( pchar, "%d", p);
    char sfchar[100];
    sprintf( sfchar, "%d", sf );
    char allowInvAtDemLocChar[100];
    sprintf( allowInvAtDemLocChar, "%d", allowInventoryHoldingAtDemandLocation );
    char removeCyclesChar[100];
    sprintf( removeCyclesChar, "%d", removeCycles );
    std::string retVal = std::string(tcchar)+"$+&"+perishLimitChar+"$+&"+pchar+"$+&"+sfchar+"$+&"+allowInvAtDemLocChar+"$+&"+removeCyclesChar;
    return retVal;
  };
  
  // destructor
  ~ESO2unitTestResultsData(){/*nothing to do here*/};
  
  // Self-test
  static void test()
  {
#if 0
    ESO2unitTestResultsData file("../data/example1/unitTestResultsData.csv");
    
    // Test reading first record

    assert( file.upLevelPart(0)=="u1");
    assert( file.downLevelPart(0) == "p1" );
    assert( file.plantLoc(0) == "pok" );
    assert( file.periodAsInt(0) == 0 );
    assert( file.prodRateAsFloat(0) == 1.5f );
    assert( file.costAsFloat(0) == 5.5f );
    assert( file.usageTimeAsFloat(0) == 0.5f );

    
    assert( file.upLevelPart(key("u2","p2","wat",1))=="u2");
    assert( file.downLevelPart(key("u2","p2","wat",1)) == "p2" );
    assert( file.plantLoc(key("u2","p2","wat",1)) == "wat" );
    assert( file.periodAsInt(key("u2","p2","wat",1)) == 1 );
    assert( file.prodRateAsFloat(key("u2","p2","wat",1)) == 2.1f );
    assert( file.costAsFloat(key("u2","p2","wat",1)) == 6.5f );
    assert( file.usageTimeAsFloat(key("u2","p2","wat",1)) == 0.0f );
#endif

  }
  
protected:
  
  // compute key for storing record in dictionary
  virtual std::string key( unsigned int recNum ) const
  {
    std::string retVal = key(
      tcAsInt(recNum),
      perishLimitAsInt(recNum),
      pAsInt(recNum),
      sfAsInt(recNum),
      allowInventoryHoldingAtDemandLocationAsInt(recNum),
      removeCyclesAsInt(recNum));
    return retVal;
  };

private:
  
  // default constructor -- don't use!
  ESO2unitTestResultsData();
  
  // copy constructor  -- don't use!
  ESO2unitTestResultsData( const ESO2unitTestResultsData& source );
  
  // assignment operator -- don't use!
  ESO2unitTestResultsData&
  operator=(const ESO2unitTestResultsData& rhs);

  
};



class ESO2unitTestResultsDataIterator : public SCOfileIterator {
public:
    
  // Fields
  getIntFieldFileWithIteratorH(tc);
  getIntFieldFileWithIteratorH(perishLimit);
  getIntFieldFileWithIteratorH(p);
  getIntFieldFileWithIteratorH(sf);
  getIntFieldFileWithIteratorH(allowInventoryHoldingAtDemandLocation);
  getFloatFieldFileWithIteratorH(objValue);

  
  // preferred constructor
  ESO2unitTestResultsDataIterator(const ESO2unitTestResultsData & file)
    :SCOfileIterator(file)
  { /* Nothing to do here */};
  
  // destructor
  ~ESO2unitTestResultsDataIterator(){/* nothing to do here */};
  
  // Self-test
  static void test()
  {
#if 0
    ESO2unitTestResultsData file("../data/example1/unitTestResultsData.csv");
    ESO2unitTestResultsDataIterator fileIter(file);
    
    // loop once for each record
    int recNo=0;
    for ( ; fileIter()!=NULL; ) {    
      if ( recNo == 0 ) {
        
        assert( fileIter.upLevelPart()=="u1");
        assert( fileIter.downLevelPart() == "p1" );
        assert( fileIter.plantLoc() == "pok" );
        assert( fileIter.periodAsInt() == 0 );
        assert( fileIter.prodRateAsFloat() == 1.5f );
        assert( fileIter.costAsFloat() == 5.5f );
        assert( fileIter.usageTimeAsFloat() == 0.5f );
          
      }
      recNo++;
    }
#endif
  }

private:
  
  
  // default constructor -- don't use!
  ESO2unitTestResultsDataIterator();
  
  // copy constructor  -- don't use!
  ESO2unitTestResultsDataIterator( const ESO2unitTestResultsDataIterator& source );
  
  // assignment operator -- don't use!
  ESO2unitTestResultsDataIterator&
  operator=(const ESO2unitTestResultsDataIterator& rhs);

  
};
#endif
