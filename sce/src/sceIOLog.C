
// RW2STL - inserted:
#include <string>
#include <ostream>
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL - inserted includes.
#include <mcl/src/mcl.h>
#include <sce/src/sceIOLog.h>

//-----------------------------------------------------------------------------
// contains method definitions for LgFrSceIOLogMgr class
//
// *** SEE END OF FILE FOR MORE INFO ABOUT THE IO LOGS ***
//-----------------------------------------------------------------------------

//--------constructor/destructor

// Default Constructor
LgFrSceIOLogMgr::LgFrSceIOLogMgr()
  : InputLogName_("sceInput.log"), 
    OutputLogName_("sceOutput.log"),
    setOfParameters_(),
    inFileHead_(0),
    outFileHead_(0),
    numBadRecords_(0)
{
  // note: the names that we initialize these arrays with are NOT the names
  // we will use later...they are the names of the parameters corresponding
  // to the fileNames we want in each entry of these arrays
  // the actual names of files are put into these arrays during the 
  // 'copySetOfParams' method below.
  int i=0;
  while (!inFileArray_[i].empty()) {
    addNode(inFileArray_[i],IN_FILE);
    i++;
  }

  i=0;
  while (!outFileArray_[i].empty()) {
    addNode(outFileArray_[i],OUT_FILE);
    i++;
  }
}

// Constructor with log file names
LgFrSceIOLogMgr::LgFrSceIOLogMgr(const std::string inputLogName,
				 const std::string outputLogName)
  : InputLogName_(inputLogName), 
    OutputLogName_(outputLogName),
    setOfParameters_(),
    inFileHead_(0),
    outFileHead_(0),
    numBadRecords_(0)
{
  // note: the names that we initialize these arrays with are NOT the names
  // we will use later...they are the names of the parameters corresponding
  // to the fileNames we want in each entry of these arrays
  // the actual names of files are put into these arrays during the 
  // 'copySetOfParams' method below.
  int i=0;
  while (!inFileArray_[i].empty()) {
    addNode(inFileArray_[i],IN_FILE);
    i++;
  }

  i=0;
  while (!outFileArray_[i].empty()) {
    addNode(outFileArray_[i],OUT_FILE);
    i++;
  }
}

// Destructor
LgFrSceIOLogMgr::~LgFrSceIOLogMgr()
{
  clearAll();

  // just indicate at the end of the input log file whether or not there were
  // any rejected input records
  std::ofstream outLog(InputLogName_.c_str(), std::ofstream::app);

//  if (numBadRecords_ > 0)
    outLog << "There were " << numBadRecords_ << " input records rejected by SCE\n";
//  else
//    outLog << "There were no rejected input records.\n";
}

//**Note: this has never actually been tested, it should work though
// Assignment operator
LgFrSceIOLogMgr&
LgFrSceIOLogMgr::operator=(const LgFrSceIOLogMgr& rhs)
{
  if (this != &rhs) { // check for assignment to self

    InputLogName_ = rhs.InputLogName_;
    OutputLogName_ = rhs.OutputLogName_;
    setOfParameters_ = LgFrSceSetOfParameters(setOfParameters_);
    // use copy constructor
    
    // now must get all the fileNodes to have same data
    clearAll();
    fileNode * currNode = rhs.inFileHead_;
    // copy all of the input file nodes
    while (currNode != 0) {
      // create corresponding node
      addNode(currNode->fileName,IN_FILE);
      // just get a handle on new node so we can update other fields
      fileNode * temp = getNode(currNode->fileName,IN_FILE);

      if (temp != 0) { // just in case, make sure not null
	// give this new node all the same fields
	temp->parameterName = currNode->parameterName;
	temp->count = currNode->count;
	temp->good = currNode->good;
	temp->bad = currNode->bad;
	temp->comm = currNode->comm;
	temp->tot = currNode->tot;
	temp->next = 0;
      }

      // step to next fileNode
      currNode = currNode->next;
    }
    
    // copy all of the output file nodes
    currNode = rhs.outFileHead_;
    while (currNode != 0) {
      // create corresponding node
      addNode(currNode->fileName,OUT_FILE);
      // just get a handle on new node so we can update other fields
      fileNode * temp = getNode(currNode->fileName,OUT_FILE);

      if (temp != 0) {
	// give this new node all the same fields
	temp->parameterName = currNode->parameterName;
	temp->count = currNode->count;
	temp->good = currNode->good;
	temp->bad = currNode->bad;
	temp->comm = currNode->comm;
	temp->tot = currNode->tot;
	temp->next = 0;
      }      

      // step to next fileNode
      currNode = currNode->next;
    }
  }

  // after done, return self
  return *this;
}


//-------------------------------------------------------------------------
// The following methods are for maintaining the list of input and output
// files that we need to keep track of. They are currently being stored
// in a singly linked list, but this may later be changed to just about
// anything else, most likely a vector.
//-------------------------------------------------------------------------

void
LgFrSceIOLogMgr::addNode(const std::string theName, whichName typeName)
{
  fileNode * newNode = new fileNode;
  if (newNode == 0) {// out of memory
    exit(1);
  }

  // don't make any fileNodes with no name
  if (theName.empty())
    return;

  if ((typeName == IN_FILE) || (typeName == OUT_FILE)) {
    newNode->fileName = theName;
    newNode->parameterName = "";
  }
  else {
    newNode->fileName = "";
    newNode->parameterName = theName;
  }

  // this will always be the values for any new node
  newNode->count = 0;
  newNode->good = 0;
  newNode->bad = 0;
  newNode->comm = 0;
  newNode->tot = 0;
  newNode->next = 0;

  if ((typeName == IN_FILE) || (typeName == IN_PARAM)) {
    if (inFileHead_ == 0) {
      inFileHead_ = newNode;
    }
    else {
      newNode->next = inFileHead_;
      inFileHead_ = newNode;
    }
  }
  else {
    if (outFileHead_ == 0) {
      outFileHead_ = newNode;
    }
    else {
      newNode->next = outFileHead_; 
      outFileHead_ = newNode;
    }
  }
}


fileNode*
LgFrSceIOLogMgr::getNode(const std::string theName, 
			 whichName typeName)
const
{
  fileNode* start;

  // can't get a node with no name 
  if (theName.empty())
    return 0;

  if ((typeName == IN_FILE) || (typeName == IN_PARAM))
    start = inFileHead_;
  else
    start = outFileHead_;

  if ((typeName == IN_FILE) || (typeName == OUT_FILE)) {
    while (start != 0) {
      if (start->fileName == theName)
	return start;
      else
	start = start->next;
    }
    return 0;
  }
  else {
    while (start != 0) {
      if (start->parameterName == theName)
	return start;
      else
	start = start->next;
    }
    return 0;
  }
}


void
LgFrSceIOLogMgr::updateNode(const std::string fileName, 
			    long checkRec,
			    long goodRec,
			    long badRec,
			    long commRec,
			    long totRec,
			    whichName typeName)
{
  // get the node we are looking for
  fileNode * temp1 = getNode(fileName,typeName);

  // update the node's count
  if (temp1 != 0) {
    temp1->count += checkRec;
    temp1->good += goodRec;
    temp1->bad += badRec;
    temp1->comm += commRec;
    temp1->tot += totRec;
  }
}


void
LgFrSceIOLogMgr::clearAll()
{
  // this method deletes all nodes ... will probably only be used by
  // destructor
  while(inFileHead_ != 0) {
    fileNode * temp = inFileHead_->next;
    delete inFileHead_;
    inFileHead_ = temp;
  }

  while(outFileHead_ != 0) {
    fileNode * temp = outFileHead_->next;
    delete outFileHead_;
    outFileHead_ = temp;
  }
}


//--------other public methods

// --- Note.
// a quick note on the printInputLog(..) and printOutputLog(..) methods...
// the last argument in both is called 'checkRec', and the importance of
// this argument is that this is the number that is checked against the
// corresponding input/output file's checkRec for all pairs or sets of
// files that have such a correspondence. all the logic/pairing is taken
// care of in an ugly set of if-else statements in the totalInCount(..)
// method below.
// the reason the checkRec may not be the same as the goodRec argument is
// that the number of printed lines is not always going to be a valid
// measure of correctness .. for example, many of the flat file processing
// methods do not print zero vector records, so this would lead to an error
// when in fact there may be none.
// 
// when looking in the output log file, the format will be:
//     In file: <output fileName> ...
//           <#> records written.
//           <#> good records read from file: <input fileName>.
//            .
//            .
//           etc if multiple input files are reflected by this output file.
//
// it is important to note that the fact that the numbers do not add up here
// does NOT mean there is an error. if there may be an error, there will be
// a message in the sce error log file that says: 'there may have been an
// error ... blah blah'.
//
// in addition, not all the output files have corresponding input files, and
// if that is the case, then you will only see the number of records written
// by into the output file.
// --- 7.14.97


//method called after processing flat file, for writing a summary log;
//returns 1 if no errors in writing summary
//   "    0 if some error
int
LgFrSceIOLogMgr::printInputLog(
		    const std::string filein,    //records FROM "filein"
		    long totRec,      //total number of records
		    long goodRec,     //number of "good" records
		    long badRec,      //number of "bad" records
		    long commRec,     //number of comment/blank records
		    long checkRec)    //used for checking against output log
{
    // open file to write to; appends to end of file
    std::ofstream tout(InputLogName_.c_str());//, ios::out | ios::app);

    // write contents of log
    tout << "In input file: " << filein << " ...\n";
    tout << "\t" << totRec << " records read.\n";
    tout << "\t" << goodRec << " records accepted.\n";
    tout << "\t" << badRec << " records rejected.\n";       
    tout << "\t" << commRec << " lines commented or blank.\n"; 

    updateNode(filein,checkRec,goodRec,badRec,commRec,totRec,IN_FILE);

    // were there any badRecords?
    numBadRecords_ += badRec;

    // was there an error in counting records?
    if ((goodRec + badRec + commRec) == totRec) {
      tout.close();
      return 1;
    }
    else
    {
      tout << "There may have be an error here.\n";
      tout.close();
      return 0;
    }
}

int
LgFrSceIOLogMgr::printOutputLog(const std::string fileout,
				long totRec,
				long zeroRec,
				long checkRec)
{
    std::ofstream tout(OutputLogName_.c_str(),std::ofstream::app);

  // name of file?
  tout << "In file: " << fileout << "\n";

  long inCount = totalInCount(fileout, tout);
  // only print out stuff if the return value is >= 0
  // for now, return value = -1 means we don't want to print any message
  if (inCount >= 0) {
    // print out the number of zero vectors NOT printed in the output file...
    // we only care for this to be printed if non-zero
    // and if something else is being printed out.
    if (zeroRec != 0) {
      tout << "\t - " << zeroRec << " records with zeroVectors ommitted.\n";
    }

    tout << "\t  ----------------\n";

    if (inCount != checkRec)  { // this would be a miscount
      // just going to use the setOfParameters_'s error facility
      (*(setOfParameters_.getErrFacility()))("MiscountWarning",MclArgList() << fileout);

      tout << "There may have be an error here.\n";
    }
  }

  // make sure to clarify what is being written out.
  // if totRec+zeroRec != checkRec then this make sure to explain that this
  //  is because of the number of blocks printed != number of lines
  //  ie. the printed blocks are greater than a single line in some cases
  tout << "\t   " << checkRec-zeroRec << " data sets written.\n";

  // how many file written out?
  tout << "\t  (" << totRec << " total lines written)\n";

  tout.close();
  return 1;
}


void
LgFrSceIOLogMgr::resetInputLog()
{
  // this will clear any file with same name or create one if none exists
  std::ofstream temp(InputLogName_.c_str());
  temp.close();
}

void
LgFrSceIOLogMgr::resetOutputLog()
{
  // this will clear any file with same name or create one if none exists
  std::ofstream temp(OutputLogName_.c_str());
  temp.close();
}

// should return the number of good input records read by sce
// corresponding to the output file of the given name
// ...also takes the open std::ofstream buffer (output log file) which should
// be open, and prints relevant info into it
long 
LgFrSceIOLogMgr::totalInCount(const std::string fileOut, std::ofstream& outBuffer)
{
  // for now i can't think of a better way than just a lookup type
  // implementation...

  // this is to get the parameter name corresponding to this fileName
  // but we also need to check that there is a node corresponding to this file
  fileNode * temp = getNode(fileOut,OUT_FILE);
  if (temp == 0) // this means no node for this name
    return -1; 
  // we will check this later on, because we don't want to make this
  // produce a 'miscount' error message, because it is not. we'll just
  // let a miscount slide if the return value is -1.

  // if there is a node with fileName.c_str() == fileOut.c_str() then get the parameter name
  std::string outParamName = temp->parameterName;

  // there are bound to be some of these that are null strings ... if so,
  // just return a zero count
  if (outParamName.empty())
    return 0;

  // Note: for future reference, the below is just a large set of 'if-else'
  //       statements.
  //       what we are trying to check is if the 'fileOut' passed as an arg
  //       matches one of the output file names which have a corresponding
  //       input file(s) against which it can be checked.
  //       the way we check is by seeing if the number of output lines
  //       printed are valid with respect to the input files on which it
  //       should depend.
  //       to add another such correspondence, add the name of the parameter
  //       that refers to the output file you want, by appending another
  //       'else if ...' statement at the end here and just follow the 
  //       pattern that the rest follow. in addition, you will want to
  //       add the relevant file names to the list of input and output file
  //       names that is a static data member of this class (see end of file).
  if (outParamName == "supplyResponseFileName") {
    fileNode * tempNode = getNode("demandVolFile",IN_PARAM);
    if (tempNode == 0) return 0;
    long tempCount = tempNode->count;
    outBuffer << "\t   " << tempCount << " good records from input file: " 
	      << tempNode->fileName << "\n";
    return tempCount;
  }
  else if (outParamName == "backlogResponseFileName") {
    fileNode * tempNode = getNode("demandVolFile",IN_PARAM);
    if (tempNode == 0) return 0;
    long tempCount = tempNode->count;
    outBuffer << "\t   " << tempCount << " good records from input file: "
	      << tempNode->fileName << "\n";
    return tempCount;
  }
  else if (outParamName == "subUsageFileName") {
    fileNode * tempNode = getNode("subBomFile",IN_PARAM);
    if (tempNode == 0) return 0;
    long tempCount = tempNode->count;
    outBuffer << "\t   " << tempCount << " good records from input file: "
	      << tempNode->fileName << "\n";
    return tempCount;
  }
  else if (outParamName == "interplantOrdersFileName") {
    fileNode * tempNode = getNode("interPlantFile",IN_PARAM);
    if (tempNode == 0) return 0;
    long tempCount = tempNode->count;
    outBuffer << "\t   " << tempCount << " good records from input file: "
	      << tempNode->fileName << "\n";
    return tempCount;
  }
  else if (outParamName == "totalDemandReportFileName") 
    {
      fileNode * tempNode1 = getNode("partFile",IN_PARAM);
      fileNode * tempNode2 = getNode("capFile",IN_PARAM);
      long tempCount1 = 0;
      long tempCount2 = 0;
      
      if (tempNode1 != 0) {
	tempCount1 = tempNode1->count;
	outBuffer << "\t   " << tempCount1 << " good records from input file: "
		  << tempNode1->fileName << "\n";
      }
      
      if (tempNode2 != 0) {
	tempCount2 = tempNode2->count;
	outBuffer << "\t + " << tempCount2 << " good records from input file: "
		  << tempNode2->fileName << "\n";
      }

      return tempCount1+tempCount2;
    }    
  else if ((outParamName == "partDBFileName") &&
	   (setOfParameters_.getBool("printPartDBFile")))
    {
      fileNode * tempNode1 = getNode("partFile",IN_PARAM);
      fileNode * tempNode2 = getNode("capFile",IN_PARAM);
      long tempCount1 = 0;
      long tempCount2 = 0;
      
      if (tempNode1 != 0) {
	tempCount1 = tempNode1->count;
	outBuffer << "\t   " << tempCount1 << " good records from input file: "
		  << tempNode1->fileName << "\n";
      }

      if  (setOfParameters_.getBool("combineCapacityIntoPartDB")) {
	if (tempNode2 != 0) {
	  tempCount2 = tempNode2->count;
	  outBuffer << "\t + " << tempCount2 << " good records from input file: "
		    << tempNode2->fileName << "\n";
	}
      }

      return tempCount1+tempCount2;
    }    

  else if ((outParamName == "capacityDBFileName") &&
	   (setOfParameters_.getBool("printCapacityDBFile")))
  {
    fileNode * tempNode = getNode ("capFile",IN_PARAM);
    if (tempNode == 0) return 0;
    long tempCount = tempNode->count;
    outBuffer << "\t   " << tempCount << " good records from input file: "
	      << tempNode->fileName << "\n";
    return tempCount;
  }
  else if (outParamName == "opExecutionVolumeFileName") {
    fileNode * tempNode1 = getNode("opFile",IN_PARAM);
    fileNode * tempNode2 = getNode("partFile",IN_PARAM);
    long tempCount1 = 0;
    long tempCount2 = 0;

    if (tempNode1 != 0) {
      tempCount1 = tempNode1->count;
      outBuffer << "\t   " << tempCount1 << " good records from input file: "
		<< tempNode1->fileName << "\n";
    }

    if (tempNode2 != 0) {
      tempCount2 = tempNode2->count;
      outBuffer << "\t + " << tempCount2 << " good records from input file: "
		<< tempNode2->fileName << "\n";
    }

    return tempCount1+tempCount2;
  }
  else if ((outParamName == "FSSGenReportFileName") ||
	   (outParamName == "FSSSingReportFileName")) {	   
    // we need these parameters to know what to print...if anything
    std::string theScheme = setOfParameters_.getString("FSSScheme");
    bool printFlag = setOfParameters_.getBool("FSSFlag");

    if (!printFlag) // this means we shouldn't produce either report
      return -1; // we return -1 so this doesn't produce an error message

    long totalCount = 0;

    if ((theScheme == "B" || theScheme == "b" || 
	 theScheme == "s" || theScheme == "S") &&
	(outParamName == "FSSSingReportFileName"))
      {
	fileNode * tempNode = getNode("demandVolFile",IN_PARAM);
	if (tempNode != 0) {
	  long tempCount = tempNode->count;
	  outBuffer << "\t   " << tempCount << " good records from file: "
		    << tempNode->fileName << "\n";
	  totalCount += tempCount;

	  // this is particular to FSS Singleton reports, some output skipped
	  //	  if (tempNode->bad != 0) {
	  //  outBuffer << "\t - " << tempNode->bad << " records ommitted. (See error log for explanation)\n";
	    // there are some records for which fss is not run.
	    
	    // this should be decremented in order for count to work out right
	  //  totalCount -= (tempNode->bad);
	  //}
	}
      }

    if ((theScheme == "B" || theScheme == "b" ||
	 theScheme == "g" || theScheme == "G")  &&
	(outParamName == "FSSGenReportFileName"))
      // these are the only three cases where we want to return theCount.
      {
	fileNode * tempNode = getNode("FSSFile",IN_PARAM);
	if (tempNode != 0) {
	  long tempCount = tempNode->count;
	  outBuffer << "\t   " << tempCount << " good records from file: "
		    << tempNode->fileName << "\n";
	  totalCount += tempCount;
	}
      }
    
    return totalCount;
  }
  else if (outParamName == "optionalFeatRatioFileName") {
    fileNode * tempNode = getNode("topLevelToOptionFile",IN_PARAM);
    if (tempNode == 0) return 0;
    long tempCount = tempNode->count;
    outBuffer << "\t   " << tempCount << " good records from file: "
	      << tempNode->fileName << "\n";
    return tempCount;
  }
  else if (outParamName == "selFeatBBratioFileName") {
    fileNode * tempNode = getNode("topLevelToBBratioFile",IN_PARAM);
    if (tempNode == 0) return 0;
    long tempCount = tempNode->count;
    outBuffer << "\t   " << tempCount << " good records from file: "
	      << tempNode->fileName << "\n";
    return tempCount;
  }
  else if (outParamName == "geoSupplyByPdfFileName") {
    fileNode * tempNode = getNode("geoSourceFile",IN_PARAM);
    if (tempNode == 0) return 0;
    long tempCount = tempNode->count;
    outBuffer << "\t   " << tempCount << " good records from file: "
	      << tempNode->fileName << "\n";
    return tempCount;
  }
  else 
   return 0;
}


// Note of Caution:
// don't use this as a general method of copying SOP ... that's not what
// this method is for.
// the problem is that this method also tries to initialize the fileNodes
// with the correct fields, and it is only valid to do this if this has
// not yet been done.
void
LgFrSceIOLogMgr::copySetOfParams(LgFrSceSetOfParameters & theSetOfParams)
{
  // use copy constructor -- just in case...we don't want to change it
  setOfParameters_ = LgFrSceSetOfParameters(theSetOfParams);

  // update log file names... get names from SOP
  InputLogName_ = setOfParameters_.getString("inputLogFileName");
  OutputLogName_ = setOfParameters_.getString("outputLogFileName");

  // this is to replace the parameter names for these files with the
  // actual names ... we need to do this because, we don't know what
  // the actual names will be for any given run of sce, but the param
  // names will always be the same for these files
  // however, we will keep both the filename and the parameter name
  // around because the parameter names establish the correspondences
  // but we need the file names to update the nodes
  fileNode* temp;
  int i=0;
  while(!inFileArray_[i].empty()) {
    temp = getNode(inFileArray_[i],IN_FILE);
    if (temp != 0) {
      //      if (!(temp->fileName).empty())
	temp->parameterName = temp->fileName;
	//else
	//temp->parameterName = "NO_NAME";

      std::string currFileName = setOfParameters_.getString(temp->fileName);
      //if (!currFileName.empty())
	temp->fileName = currFileName;
	//      else
	//temp->fileName = "NO_NAME";
    }
    i++;
  }

  i=0;
  while(!outFileArray_[i].empty()) {
    temp = getNode(outFileArray_[i],OUT_FILE);
    if (temp != 0) {
      //      if (!(temp->fileName).empty())
	temp->parameterName = temp->fileName;
	//      else
	//temp->parameterName = "NO_NAME";

      std::string currFileName = setOfParameters_.getString(temp->fileName);
      //if (!currFileName.empty())
	temp->fileName = currFileName;
	//else
	//temp->parameterName = "NO_NAME";
    }
    i++;
  }
}


std::string
LgFrSceIOLogMgr::inFileArray_[] = {
  "demandVolFile",
  "subBomFile",
  "partFile",
  "capFile",
  "interPlantFile",
  "opFile",
  "FSSFile",
  "topLevelToBBratioFile",
  "topLevelToOptionFile",
  "geoSourceFile",
  "partCondFile", 
  "" // null string to signify end of array
};

std::string
LgFrSceIOLogMgr::outFileArray_[] = {
  "supplyResponseFileName",
  "subUsageFileName",
  "interplantOrdersFileName",
  "totalDemandReportFileName",
  "partDBFileName",
  "capacityDBFileName",
  "optionalFeatRatioFileName",
  "selFeatBBratioFileName",
  "" // remember to keep this null string as last element of array
};

//------------------------------   README   -----------------------------------

//I.	INPUT FILE LOG ENTRIES
//
//	If a problem should arise in reconciling the number of input file
//	records (ie. # read in = # rejected + # accepted + # comment/blank),
//	the problem will most likely be found in the file ipSce.C.
//
//	In ipSce.C are most of the flat file processing methods. Most likely
//	when you look at one of these methods which produce an input file log
//	entry, you will see several counters like: goodRec,badRec,commentRec,
//	or some other pretty self explanatory (type=long) variable. In
//	addition, usually, if there is a variable 'lineCount' or something
//	else that indicates that the number of lines processed is being kept
//	track of, this is often the number used when we need to indicate the
//	total line count in reconciliation.
//
//	If there is some problem, which hopefully there will not be, with
//	making the count come out right, then go find the method in ipSce.C
//	that corresponds to the file for which the count isn't coming out.
//	Then look for any points in the code where there are 'continue's
//	or other places where a counter should have been incremented but was
//	not. Basically, every time one of these methods reads a line from a
//	file, there should be two counters incremented...the totalRec counter
//	and one of the other ones. Other problems may occur if any of the 
//	methods of the flat file manager are changed because the file
//	processing methods often rely on their return codes to determine
//	which kind of line was read in. Lastly, there may be some error in
//	the values passed to the sceIOLogMgr method 'printInputLog(..)'. Make
//	sure the values passed to that method are in the correct order. The
//	syntax can be found in sceIOLog.h.
//
//	The only exceptions to all of the above should be the input file log
//	entries for the FSS input file (if one exists) ... this file is
//	processed differently and this occurs in the file sceFssMg.C. Similar
//	counters will be found there. The other exception is the parameter
//	file log entry, and this file is processed by the parameter manager
//	in file paramMgr.C. Note, this file only produced at the very end of
//	an SCE run, this is because it was the only predictable place to 
//	print this information. If SCE halts execution in mid-run, this log
//	entry will not be available, nor will the parameter log file which
//	gives information about the values of each parameter.
//
//	* If you're planning on adding another input file that needs to have
//	  an entry in the input file log...
//
//	- Well, obviously there should already be a method somewhere that
//	  'processes' this input file, however it does so. The first step is
//	  just to add the relevant counters to that method to keep track of
//	  the total number of records as well as the number that are accepted,
//	  rejected, and commented/blank. In addition, if you later will be 
//	  reconciling an output file log entry using this input file, you will
//	  want to pass the number that should be used in this checking later.
//	  (you may want to read the below section before going on, if you have
//	   not already done so)
//
//	- Make sure the counters work the way you expect and that the count
//	  works out.
//
//	- Make sure to add the parameter name corresponding to the input file's
//	  filename, in the statically defined array in sceIOLog.C, that lists
//	  the input files of interest (these are the ones for which we will be
//	  producing input log entries).
//
//	- Also make sure that whatever class owns this method that processes
//	  the file has access to the IO log manager. If not, then find some
//	  way to give this class access to it. Note: if it has access to the
//	  scenario or the flat file manager, it will have access to the IO log
//	  manager as well... it should be accessible by using 'get()' methods.
//
//	- After the file has been processed, call the 'printInputLog(..)' of
//	  the IO log manager following the prescribed syntax.
//
//
//II.	OUTPUT FILE LOG ENTRIES
//
//	These are a bit more complicated because they require a check between
//	the number of records outputted (written to an output file) and the
//	number of 'good' or accepted records read in by one or multiple input
//	files. Therefore, there are a lot of places to check if this count is
//	found to be incorrect at some point.
//
//	A brief explanation of how this reconciliation is currently done:
//
//	At the beginning of SCE, when all the scenario is created, a flat file
//	manager is intantiated and also as a data member, the IO log manager
//	is instantiated as well.
//	
//	In the constructor of the log manager, we create a linked list of 
//	structures* that hold a name of a file (actually the Parameter Name --
//	not the actual file name), and have room to store information about 
//	the records of that file, namely the number of records. There are two 
//	singly linked list structures, one for the input files, and one for 
//	the output files. (*NOTE: these structures are only created for files 
//	specified by parameter name in two static arrays of the sceIOLogMgr) 
//
//	As soon as the set of parameters object is accessible to the log
//	manager, it looks up the 'real' names of the files corresponding to
//	the parameters that are given to it initially. It keeps track of both
//	the parameter name and the actual name of each file.
//
//	When any input file is processed for which an input log entry is 
//	produced, the log manager checks if this is one of the files that we
//	are interested in (ie. among those name in the static array mentioned
//	above). If so, then it stores, in the corresponding node of the input
//	file linked list, the number of good,bad,comment,and total records as
//	well as the number that should be used to check against later. This
//	is often the number of good/accepted records, but may be different
//	because sometimes that is not the number we want (eg. we may only want
//	the number of unique sets of input data instead of all).
//
//	Similarly, when we get to a method (most are members of the class
//	LgFrScenarioForSce - defined in scenSce.C) that produces an output
//	file, we keep track of the number of records printed out, and
//	the number of zero-vectors (which are sometimes not printed out) if
//	relevant, and also the count that we want to check against the input
//	files or whatever we wanted to check against. Again, this is often
//	going to be the number of total lines printed out but may be something
//	like the number of unique sets of data printed out. 
//
//	Note: there is also one exception among the output files. the output 
//	log entry for the FSS run is produced by the FSS manager in sceFssMg.C.
//
//	The final checking is done by the sceIOLogMgr in the 'totalIn(..)'
//	method. This method takes several arguments including an output file
//	name and searches for that name by means of a long and ugly 
//	'if-else if'-structure. If it is one of the files of interest (again,
//	this just implies it has a node in the output file linked list), then
//	a value is returned that should be the same as the number that is
//	given to the 'printOutputLog(..)' method used for checking.
//
//	So where should you check if there is some reconciliation problem for
//	the output files? Step through all of the above mentioned steps and
//	make sure everything happens as expected. It may help to use the
//	xldb (debugger) for keeping track of counters as you go through.
//
//
//	* If you're planning on adding another output file that needs to be
//	  reconciled with input files....
//
//	- First, make sure it is feasible to reconcile it. And then determine
//	  which input files it must be checked against.
//
//	- If it is feasible and you know which input files to check, then 
//	  make sure these input files are producing (or capable of producing)
//	  input log entries.. ie. make sure SCE is counting the number of
//	  records (good,bad,etc) of these input files, and that this input file
//	  has an entry in the log manager's input file linked list.
//	  (see the section on INPUT FILE LOGs above for more detail)
//
//	- Make sure to add the parameter name corresponding to this output
//	  file's filename in the statically defined array in sceIOLog.C, so
//	  that a node in the output file linked list is created for it.
//
//	- Go and add another 'else if (...)' block to the list of them in
//	  the definition for the log manager method 'totalIn(..)' -- just 
//	  try to follow the same sort of format as the others. Note: a return
//	  value of -1 means that nothing should be printed by the output log
//	  manager about the input files; otherwise a non-negative return value
//	  translates to what the log manager will check against the number of
//	  records(or whatever else it may be) that are written to the output
//	  file.
//
//	- Finally, make sure that whatever class owns the method that produces
//	  this output file has access to the IO log manager. If not, find some
//	  way to give it access to it, otherwise this file will be unable to
//	  conform with the others. Note: if the class has access to either the
//	  scenario object or the flat file manager, then it will also have 
//	  access to the IO log manager through means of the 'get()' methods.
//
//	- Call the 'printOutputLog(..)' method of the IO log manager following
//	  the prescribed syntax.
