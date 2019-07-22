#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sce/src/sceParam.h>

//----------------------------------------------------------------------------
//  Class: LgFrSceIOLogMgr
//
//  What is it: It is simply a class that is responsible for creating the
//              input and output logs for sce. It reports the number of lines
//              that are read in from each input flat file and whether each
//              line is good,bad,or comment/blank. It reports the number of
//              lines written into each output file, and also for certain
//              output files, whether or not there may be an error after 
//              doing a simple check against a corresponding input file or 
//              set of input files.
//
//  This could have easily been implemented as part of another class, but
//  for the sake of readability and to make debugging easier, it seemed more
//  logical to make another class.
//
//  For the time being, this IOLogManager is part of the FlatFileManager, but
//  the only important issue is that there should be an IOLogManager for each
//  scenario.
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
#ifndef LGFRSCEIOLOGMGR_H
#define LGFRSCEIOLOGMGR_H

// RW2STL - inserted:
#include <string>
// End of RW2STL - inserted includes.

struct fileNode {
  std::string fileName;
  std::string parameterName;
  long count;
  long good;
  long bad;
  long comm;
  long tot;
  struct fileNode * next;
};

enum whichName {IN_FILE, OUT_FILE, IN_PARAM, OUT_PARAM};
// there are times when we want to search through different sets of names
// ...these are just flags used by the 'getNode' method

//----------------------------------------------------------------------------



// class that takes care of IO Log printing for SCE
class LgFrSceIOLogMgr {

public:

  fileNode* getNode(const std::string theName, whichName typeName) const;
  void addNode(const std::string theName, whichName typeName);

  void resetInputLog();
  void resetOutputLog();

  void updateNode(const std::string fileName,
		  long checkRec,
		  long goodRec,
		  long badRec,
		  long commRec,
		  long totRec,
		  whichName typeName);
  void clearAll();

  // should return the number of good input records read by sce
  // corresponding to the output file of the given name
  // ...it also prints into the output log file
  long totalInCount(const std::string fileOut, std::ofstream& outBuffer);

  void copySetOfParams(LgFrSceSetOfParameters & theSetOfParams);

  int
  printInputLog(  const std::string filein,    //records FROM "filein"
		  long totRec,      //total number of records
		  long goodRec,     //number of "good" records
		  long badRec,      //number of "bad" records
		  long commRec,     //number of comment/blank records
		  long checkRec);   //used for checking against output
  int
  printOutputLog(const std::string fileout,    //records going TO "fileout"
		 long totRec,             //total number of records written
		 long zeroRec,            //number of ommitted zero Vector recs
		 long checkRec);          //used for checking against input


  LgFrSceIOLogMgr();  // Default Constructor
		   
  LgFrSceIOLogMgr(const std::string inputLogName, const std::string outputLogName);
  
  ~LgFrSceIOLogMgr();

  // Assignment operator
  LgFrSceIOLogMgr& operator=(const LgFrSceIOLogMgr& rhs);

private:

  std::string InputLogName_;
  std::string OutputLogName_;

  // this holds number of rejected records from input files
  long numBadRecords_;

  LgFrSceSetOfParameters setOfParameters_;

  fileNode* inFileHead_;
  fileNode* outFileHead_;

  // going to use as a look-up table -- these will hold *parameter* names
  // of files we may want to know about
  static std::string inFileArray_[];
  static std::string outFileArray_[];
};


#endif
