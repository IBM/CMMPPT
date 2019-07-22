
// RW2STL - inserted:
#include <string>
#include <ostream>
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL - inserted includes.
// file: sceParam.C
// author: tom

#include <stdlib.h>
#include <assert.h>
#include <mcl/src/mcl.h>
#include <sce/src/sceParam.h>
#include <fstream>


#ifndef ELIMINATE_OLD_MAIN

// RW2STL - implement (RWGVector, LgFrSceParameter)

// RW2STL - implement (RWGOrderedVector, LgFrSceParameter)



  // Set Parameter
int
LgFrSceParameter::setStringVal(const std::string & stringVal,
			       whichVal whichOne=DEFAULT_VAL)
{
  if (whichOne == DEFAULT_VAL) {
    defStringVal_ = stringVal;
  }
  else {
    actStringVal_ = stringVal;
  }

  if (type_ == stringP)     return 0;
  else                      return 1;
}
  
int
LgFrSceParameter::setIntVal(int intVal,
			    whichVal whichOne=DEFAULT_VAL)
{  
  if (whichOne == DEFAULT_VAL) {
    defIntVal_ = intVal;
  }
  else {
    actIntVal_ = intVal;
  }

  if (type_ == intP)     return 0;
  else                   return 1;
}

int
LgFrSceParameter::setFloatVal(float floatVal,
			      whichVal whichOne=DEFAULT_VAL)
{  
  if (whichOne == DEFAULT_VAL) {
    defFloatVal_ = floatVal;
  }
  else {
    actFloatVal_ = floatVal;
  }

  if (type_ == floatP)     return 0;
  else                   return 1;
}

int
LgFrSceParameter::setBoolVal(bool boolVal,
			     whichVal whichOne=DEFAULT_VAL)
{  
  if (whichOne == DEFAULT_VAL) {
    defBoolVal_ = boolVal;
  }
  else {
    actBoolVal_ = boolVal;
  }

  if (type_ == boolP)     return 0;
  else                   return 1;
}

void
LgFrSceParameter::setMandatory(bool isMandatory)
{
  mandatory_ = isMandatory;
}
  
// Get Parameter

std::string
LgFrSceParameter::getStringVal(whichVal whichOne=ACTUAL_VAL) const
{
  if (whichOne == ACTUAL_VAL)
    return actStringVal_;
  else
    return defStringVal_;
}

int
LgFrSceParameter::getIntVal(whichVal whichOne=ACTUAL_VAL) const
{
  if (whichOne == ACTUAL_VAL)
    return actIntVal_;
  else
    return defIntVal_;
}

float
LgFrSceParameter::getFloatVal(whichVal whichOne=ACTUAL_VAL) const
{
  if (whichOne == ACTUAL_VAL)
    return actFloatVal_;
  else
    return defFloatVal_;
}

bool
LgFrSceParameter::getBoolVal(whichVal whichOne=ACTUAL_VAL) const
{
  if (whichOne == ACTUAL_VAL)
    return actBoolVal_;
  else
    return defBoolVal_;
}

bool
LgFrSceParameter::getMandatory() const
{
  return mandatory_;
}

  // Set/Get Current TYPE of parameter
void
LgFrSceParameter::setType(paramType type)
{ type_ = type;}

LgFrSceParameter::paramType
LgFrSceParameter::getType() const
{ return type_; }

std::string
LgFrSceParameter::getName()
const
{ return name_; }

int
LgFrSceParameter::getMethodSet()
const
{ return methodSet_; }



// -- constructors:  Use the type-explicit constructors
// string param  constructor
LgFrSceParameter::LgFrSceParameter(const std::string & name,
                                   const std::string & stringVal,
                                   paramType         type,
				   unsigned int methodSet,
				   bool isMandatory) // default = false
  : name_(name),
    defStringVal_(stringVal),
    actStringVal_(stringVal),
    defFloatVal_(0.0),
    actFloatVal_(0.0),
    defIntVal_(0),
    actIntVal_(0),
    defBoolVal_(true),
    actBoolVal_(true),
    type_(type),
    methodSet_(methodSet),
    mandatory_(isMandatory)
{
  assert(type == stringP);
}

  // float param  constructor
LgFrSceParameter::LgFrSceParameter(const std::string & name,
				   float             floatVal,
				   paramType         type,
				   unsigned int methodSet,
				   bool isMandatory) // default = false
  : name_(name),
    defStringVal_(""),
    actStringVal_(""),
    defFloatVal_(floatVal),
    actFloatVal_(floatVal),
    defIntVal_(0),
    actIntVal_(0),
    defBoolVal_(true),
    actBoolVal_(true),
    type_(type),
    methodSet_(methodSet),
    mandatory_(isMandatory)
{
  assert(type == floatP);
}

  // int param  constructor
LgFrSceParameter::LgFrSceParameter(const std::string & name,
				   int               intVal,
				   paramType         type,
				   unsigned int methodSet,
				   bool isMandatory) // default = false
  : name_(name),
    defStringVal_(""),
    actStringVal_(""),
    defFloatVal_(0.0),
    actFloatVal_(0.0),
    defIntVal_(0),
    actIntVal_(0),
    defBoolVal_(true),
    actBoolVal_(true),
    type_(type),
    methodSet_(methodSet),
    mandatory_(isMandatory)
{
  assert((type == intP) || (type == boolP));
  if (type_ == intP) {
    defIntVal_ = actIntVal_ = intVal;
  }
  else {
      defBoolVal_ = actBoolVal_ = intVal == 0 ? false : true;
  }
}


// fairly general constructor
LgFrSceParameter::LgFrSceParameter(const std::string & name)
  : name_(name),
    defStringVal_(""),
    actStringVal_(""),
    defFloatVal_(0.0),
    actFloatVal_(0.0),
    defIntVal_(0),
    actIntVal_(0),
    defBoolVal_(true),
    actBoolVal_(true),
    type_(boolP),
    methodSet_(0),
    mandatory_(false)
{
    // all the work is done in initializer
}

// default constructor:
LgFrSceParameter::LgFrSceParameter()
  : name_(""),
    defStringVal_(""),
    actStringVal_(""),
    defFloatVal_(0.0),
    actFloatVal_(0.0),
    defIntVal_(0),
    actIntVal_(0),
    defBoolVal_(true),
    actBoolVal_(true),
    type_(boolP),
    methodSet_(0),
    mandatory_(false)
{
    // all the work is done in initializer
}



 // copy constructor
LgFrSceParameter::LgFrSceParameter(const LgFrSceParameter& source)
  : name_(source.name_),
    defStringVal_(source.defStringVal_),
    actStringVal_(source.actStringVal_),
    defFloatVal_(source.defFloatVal_),
    actFloatVal_(source.actFloatVal_),
    defIntVal_(source.defIntVal_),
    actIntVal_(source.actIntVal_),
    defBoolVal_(source.defBoolVal_),
    actBoolVal_(source.actBoolVal_),
    type_(source.type_),
    methodSet_(source.methodSet_),  // for now, have this reflect original
    mandatory_(source.mandatory_)
{
  // nothing to do
}

  // == operator
bool
operator==(const LgFrSceParameter& lhs, const LgFrSceParameter& rhs)
{
    return sce::compareStringsCaseIns( lhs.getName(), rhs.getName() ) == 0;
}

// assignment operator
LgFrSceParameter&
LgFrSceParameter::operator=(const LgFrSceParameter& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    name_         = rhs.name_;
    defStringVal_ = rhs.defStringVal_;
    actStringVal_ = rhs.actStringVal_;
    defFloatVal_  = rhs.defFloatVal_;
    actFloatVal_  = rhs.actFloatVal_;
    defIntVal_    = rhs.defIntVal_;
    actIntVal_   = rhs.actIntVal_;
    defBoolVal_   = rhs.defBoolVal_;
    actBoolVal_   = rhs.actBoolVal_;
    type_      = rhs.type_;
    methodSet_ = rhs.methodSet_; // for now, just reflect original
    mandatory_ = rhs.mandatory_;
  }
  return *this;
}

// destructor
LgFrSceParameter::~LgFrSceParameter()
{
  // nothing to do
}


// ============== SET OF PARAMETERS CLASS =====================
  
// copy constructor
LgFrSceSetOfParameters::LgFrSceSetOfParameters(const LgFrSceSetOfParameters& source)
  : paramVec_(source.paramVec_),
  sceErrFacility_(source.sceErrFacility_)
{
  // nothing to do
}

// assignment operator
LgFrSceSetOfParameters&
LgFrSceSetOfParameters::operator=(const LgFrSceSetOfParameters& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    paramVec_  = rhs.paramVec_;
    sceErrFacility_ = rhs.sceErrFacility_;
  }
  return *this;
}

// destructor
LgFrSceSetOfParameters::~LgFrSceSetOfParameters()
  {
    printParamLog(getString("parameterLogFileName"));
  }

  // default constructor
LgFrSceSetOfParameters::LgFrSceSetOfParameters()
  : paramVec_(),
    sceErrFacility_(0)
{
  // There were some problems with RW's 'auto'-resizing of this GOrderedVector
  // we're using this as a temporary fix ... if the number of parameters ever
  // exceeds 256... it's currently well under 200, in the 150's
  paramVec_.reserve(512);

  // Another fix, though this may be a less desirable solution, is to undefine
  // and re-define the macro DEFAULT_VECTOR_CAPACITY to a larger value. This will
  // however effect other ordered vectors used in SCE and require much more
  // overhead for their use.
}


// For sharing error facility for messaging purposes
void
LgFrSceSetOfParameters::setErrFacility(MclFacility * existingFacility)
{
  sceErrFacility_ = existingFacility;
}

MclFacility*
LgFrSceSetOfParameters::getErrFacility()
{
  return sceErrFacility_;
}


// allows for explicit indexing of parameters in SOP given an int index
LgFrSceParameter&
LgFrSceSetOfParameters::operator()(size_t index)
{
  return paramVec_[index];
}

// returns the number of parameters currently in set of parameters
size_t
LgFrSceSetOfParameters::numParameters()
const
{
  return paramVec_.size();
}




static size_t findInVector( const LgFrSceParameter& p, const std::vector<LgFrSceParameter>& v )
{
    std::vector<LgFrSceParameter>::const_iterator it;
    size_t i = 0;
    for( it = v.begin(); it != v.end(); it++, i++ )
         {
            if( *it == p )
                return i;
         }
    return sce::badIndex;
}

// returns: -1=doesn't exist; (int)paramType otherwise {0,1,2,3}
int
LgFrSceSetOfParameters::exists(const std::string name)
const
{
  // temporary param
  LgFrSceParameter param(name);
  
  size_t index = findInVector( param, paramVec_ );
  if (index == sce::badIndex) // doesn't exist
    return -1;
  else
    return (int)(paramVec_[index].getType());
}




// Set Parameter: One for each type.
// Return 0 if added a new one, 1 if changed an existing param
int
LgFrSceSetOfParameters::setString(std::string name,
                                  std::string stringVal,
				  unsigned int methodSet,
				  bool isMandatory)
{
  // create a paramter with the desired properties
  LgFrSceParameter param(name,stringVal,LgFrSceParameter::stringP,methodSet,isMandatory);

  // see if that parameter already exists in the list
  size_t index = findInVector( param, paramVec_ );
  // if the param doesn't already exist in list
  // then add it to the list.  (this is how we manage uniqueness)
  if (index == sce::badIndex) {
    paramVec_.push_back(param);
  }
  // otherwise, a param already exists so we change it
  else {
    // get the one in the list
    LgFrSceParameter existingParam = paramVec_[index];
    // You're not allowed to change the type of a parameter!!
    assert(existingParam.getType() == LgFrSceParameter::stringP);

    // check if this is supposed to replace old default value
    if (!(methodSet == 0)) { // if not, then we must keep default value!
      // FIRST, set default string val of new param to that of old param!
      param.setStringVal(existingParam.getStringVal(DEFAULT_VAL),DEFAULT_VAL);
    }

    // remove the copy in the list and add the new one
    paramVec_.erase(paramVec_.begin() + index);
    paramVec_.push_back(param);
  }

  return (index == sce::badIndex) ? 0 : 1;
}

int
LgFrSceSetOfParameters::setInt(std::string name,
                               int intVal,
			       unsigned int methodSet,
			       bool isMandatory)
{
  // create a paramter with the desired properties
  LgFrSceParameter param(name,intVal,LgFrSceParameter::intP,methodSet,isMandatory);

  // see if that parameter already exists in the list
  size_t index = findInVector( param, paramVec_ );
  // if the param doesn't already exist in list
  // then add it to the list.  (this is how we manage uniqueness)
  if (index == sce::badIndex) {
    paramVec_.push_back(param);
  }
  // otherwise, a param already exists so we change it
  else {
    // get the one in the list
    LgFrSceParameter existingParam = paramVec_[index];
    // You're not allowed to change the type of a parameter!!
    assert(existingParam.getType() == LgFrSceParameter::intP);

    // check if this is supposed to replace old default value
    if (!(methodSet == 0)) { // if not, then we must keep default value!
      // FIRST, set default string val of new param to that of old param!
      param.setIntVal(existingParam.getIntVal(DEFAULT_VAL),DEFAULT_VAL);
    }

    // remove the copy in the list and add the new one
    paramVec_.erase(paramVec_.begin() + index);
    paramVec_.push_back(param);
  }

  return (index == sce::badIndex) ? 0 : 1;

}

int
LgFrSceSetOfParameters::setFloat(std::string name,
                                 float floatVal,
				 unsigned int methodSet,
				 bool isMandatory)
{
  // create a paramter with the desired properties
  LgFrSceParameter param(name,floatVal,LgFrSceParameter::floatP,methodSet,isMandatory);

  // see if that parameter already exists in the list
  size_t index = findInVector( param, paramVec_ );
  // if the param doesn't already exist in list
  // then add it to the list.  (this is how we manage uniqueness)
  if (index == sce::badIndex) {
    paramVec_.push_back(param);
  }
  // otherwise, a param already exists so we change it
  else {
    // get the one in the list
    LgFrSceParameter existingParam = paramVec_[index];
    // You're not allowed to change the type of a parameter!!
    assert(existingParam.getType() == LgFrSceParameter::floatP);

    // check if this is supposed to replace old default value
    if (!(methodSet == 0)) { // if not, then we must keep default value!
      // FIRST, set default string val of new param to that of old param!
      param.setFloatVal(existingParam.getFloatVal(DEFAULT_VAL),DEFAULT_VAL);
    }

    // remove the copy in the list and add the new one
    paramVec_.erase(paramVec_.begin() + index);
    paramVec_.push_back(param);
  }

  return (index == sce::badIndex) ? 0 : 1;

}


int
LgFrSceSetOfParameters::setBool(std::string name,
                                bool boolVal,
				unsigned int methodSet,
				bool isMandatory)
{
  // create a paramter with the desired properties
  LgFrSceParameter param(name,(int)boolVal,LgFrSceParameter::boolP,methodSet,isMandatory);

  // see if that parameter already exists in the list
  size_t index = findInVector( param, paramVec_ );
  // if the param doesn't already exist in list
  // then add it to the list.  (this is how we manage uniqueness)
  if (index == sce::badIndex) {
    paramVec_.push_back(param);
  }
  // otherwise, a param already exists so we change it
  else {
    // get the one in the list
    LgFrSceParameter existingParam = paramVec_[index];
    // You're not allowed to change the type of a parameter!!
    assert(existingParam.getType() == LgFrSceParameter::boolP);

    // check if this is supposed to replace old default value
    if (!(methodSet == 0)) { // if not, then we must keep default value!
      // FIRST, set default string val of new param to that of old param!
      param.setBoolVal(existingParam.getBoolVal(DEFAULT_VAL),DEFAULT_VAL);
    }

    // remove the copy in the list and add the new one
    paramVec_.erase(paramVec_.begin() + index);
    paramVec_.push_back(param);
  }

  return (index == sce::badIndex) ? 0 : 1;

}

  
// Get Parameter

std::string
LgFrSceSetOfParameters::getString(std::string  name)
const
{
  // create a paramter with the desired properties
  LgFrSceParameter param(name);

  // see if that parameter  exists in the list
  size_t index = findInVector( param, paramVec_ );
  // if the param doesn't already exist in list
  // then its a system error
  // NEVER try to access a parameter that has not been added to the list
  // -- or a parameter whose value is a null string.
  if (index == sce::badIndex ||
      (paramVec_[index].getStringVal().empty()))
    {
    // there is a special case here for output filenames that are constructed
    // internally...if these are not specified in the parameter file, they 
    // will not be constructed, but we can guess a default name as follows:
    size_t nameLength = name.length();
    if (nameLength < 8)
        return std::string("");
    size_t quoteFileNameIndex = sce::find( name, "FileName" );
    if ( quoteFileNameIndex == nameLength - 8 ) 
    {
      name.resize(quoteFileNameIndex+4);  // cut "Name" from "..FileName"
      if (exists("outputFilePrefix") != -1) // if output file prefix exists
      {
        if (exists(name+"Extension") != -1) // "..FileExtension" ?
          return (getString("outputFilePrefix") + getString(name+"Extension"));
	    // Note of Caution: if the following clause is revised, keep in mind
	    // that the remove method actually *removes* part of the original
	    // string...so below, 'name' is actually altered by this call.
	    else
        {
          name.resize(quoteFileNameIndex);  // cut "FileName"
          if (exists(name+"Extension") != -1)
	        return (getString("outputFilePrefix") + getString(name+"Extension"));
        }
      }
    }

    // This warning message is gone for now...
    // (*sceErrFacility_)("MissingParameterRequested",MclArgList() << name);
    //** some calls expect this to return a null string .. so don't exit;

    return std::string(""); // null string
  }
  // otherwise, get the param and return its value
  param = paramVec_[index];
  assert(param.getType() == LgFrSceParameter::stringP);
  return param.getStringVal();
}


int
LgFrSceSetOfParameters::getInt(std::string name)
const
{
  // create a paramter with the desired properties
  LgFrSceParameter param(name);

  // see if that parameter  exists in the list
  size_t index = findInVector( param, paramVec_ );

  // if the param doesn't already exist in list
  // then its a system error
  // NEVER try to access a parameter that has not been added to the list
  if (index == sce::badIndex) {
    (*sceErrFacility_)("MissingParameter",MclArgList() << name);
  }
  // otherwise, get the param and return its value
  param = paramVec_[index];
  assert(param.getType() == LgFrSceParameter::intP);
  return param.getIntVal();
}

float
LgFrSceSetOfParameters::getFloat(std::string name)
const
{
  // create a paramter with the desired properties
  LgFrSceParameter param(name);

  // see if that parameter  exists in the list
  size_t index = findInVector( param, paramVec_ );

  // if the param doesn't already exist in list
  // then its a system error
  // NEVER try to access a parameter that has not been added to the list
  if (index == sce::badIndex) {
    (*sceErrFacility_)("MissingParameter",MclArgList() << name);
  }
  // otherwise, get the param and return its value
  param = paramVec_[index];
  assert(param.getType() == LgFrSceParameter::floatP);
  return param.getFloatVal();
}


bool
LgFrSceSetOfParameters::getBool(std::string name)
const
{
  // create a paramter with the desired properties
  LgFrSceParameter param(name);

  // see if that parameter  exists in the list
  size_t index = findInVector( param, paramVec_ );

  // if the param doesn't already exist in list
  // then its a system error
  // NEVER try to access a parameter that has not been added to the list
  if (index == sce::badIndex) {
    (*sceErrFacility_)("MissingParameter",MclArgList() << name);
  }
  // otherwise, get the param and return its value
  param = paramVec_[index];
  assert(param.getType() == LgFrSceParameter::boolP);
  return param.getBoolVal();
}


void
LgFrSceSetOfParameters::printParamLog(std::string logFileName)
const
{
  size_t numEntries = numParameters();

  long numUserDefined = 0;
  long numDefaultDefined = 0;

  // create buffer to write log into -- default name = param.log
  std::ofstream outFile(logFileName.c_str());//, ios::out);
  std::left( outFile );



  outFile << "sce::badIndex is ... [" << sce::badIndex << "]\n\n";


  outFile << "In the parameter file: " << getString("parameterFileName")
	  << ", the end state of the parameters were as follows:\n";

  outFile << "NOTE: Names exceeding 40 chars and Values exceeding 25 chars will be cut short.\n\n";

  outFile.width(40);
  outFile << "Parameter Name";
  outFile.width(25); 
  outFile << "Default Value";
  outFile.width(25);
  outFile << "Actual/Used Value";
  outFile.width(10);
  outFile << "Method Set\n\n";

  std::left( outFile );

  int i = 0; // Pulled out of the for below by RW2STL
  for (i=0; i<numEntries; i++) {
    LgFrSceParameter currParam = paramVec_[i];

    // NAME
    outFile.width(40);
    outFile << currParam.getName();

    // DEFAULT and ACTUAL values
    switch(currParam.getType()) {
    case LgFrSceParameter::intP: 
      outFile.width(25);
      outFile << currParam.getIntVal(DEFAULT_VAL);
      outFile.width(25);
      outFile << currParam.getIntVal(ACTUAL_VAL);
      break;
    case LgFrSceParameter::floatP: 
      outFile.width(25);
      outFile << currParam.getFloatVal(DEFAULT_VAL);
      outFile.width(25);
      outFile << currParam.getFloatVal(ACTUAL_VAL);
      break;
    case LgFrSceParameter::stringP: 
      outFile.width(25);
      outFile << "\"" + currParam.getStringVal(DEFAULT_VAL) + "\"";
      outFile.width(25);
      outFile << "\"" + currParam.getStringVal(ACTUAL_VAL) + "\"";
      break;
    case LgFrSceParameter::boolP: 
      outFile.width(25);
      outFile << currParam.getBoolVal(DEFAULT_VAL);
      outFile.width(25);
      outFile << currParam.getBoolVal(ACTUAL_VAL);
      break;
    default: 
      outFile.width(25);
      outFile << "<UNKNOWN TYPE>";
      outFile.width(25);
      outFile << "<UNKNOWN TYPE>";
      break;
    }

    // SET FLAG
    outFile.width(5);
    if (currParam.getMethodSet() == 1) {
      outFile << "USER-DEFINED\n";
      numUserDefined++;
    }
    else {
      outFile << "DEFAULT\n";
      numDefaultDefined++;
    }
  }

  outFile << "\nNumber of User-Defined Parameters: " << numUserDefined;
  outFile << "\nNumber of Default Parameters: " << numDefaultDefined;
  outFile << "\nTotal Number of Parameters: " << numUserDefined+numDefaultDefined << "\n";

  outFile.close();
}



void
LgFrSceSetOfParameters::printScrubbedParamFile(std::string scrubParamFileName)
const
{
  size_t numEntries = numParameters();

  long numUserDefined = 0;
  long numDefaultDefined = 0;

  std::ofstream outFile(scrubParamFileName.c_str());
  std::showpoint( outFile );

  int i = 0; 
  for (i=0; i<numEntries; i++) {
    LgFrSceParameter currParam = paramVec_[i];

    // NAME
    std::string paramName = currParam.getName();


    // skip string  parameters that have null values 
    if (currParam.getType() == LgFrSceParameter::stringP) {
      if (this->getString(currParam.getName()).empty())
	continue;
    }

    // skip some parameters that are internal
    if (paramName == "parameterFileName")
      continue;
    if (paramName == "outputLogFileName")
      continue;
    if (paramName == "inputLogFileName")
      continue;
    if (paramName == "nStandaloneFeatureArcs")
      continue;
    if (paramName == "parameterLogFileName")
      continue;
 


    outFile << paramName << ",";
    //    outFile << "\"" << paramName << "\",";

    switch(currParam.getType()) {
    case LgFrSceParameter::intP: 
      outFile << currParam.getIntVal() << "\n";
      break;
    case LgFrSceParameter::floatP: 
      outFile << currParam.getFloatVal() << "\n";
      break;
    case LgFrSceParameter::stringP: 
      if (! (this->getString(currParam.getName()).empty()))
	outFile << this->getString(currParam.getName()) << "\n";
      break;
    case LgFrSceParameter::boolP: 
      outFile << currParam.getBoolVal() << "\n";
      break;
    default: 
      outFile << "<UNKNOWN TYPE>\n";
      break;
    }

  }


  outFile.close();
}



int
LgFrSceSetOfParameters::getMethodSet(const std::string name) const
{
  // create a paramter with the desired properties
  LgFrSceParameter param(name);

  // see if that parameter already exists in the list
  // if the param doesn't exist, return -1
  size_t index = findInVector( param, paramVec_ );
  if (index == sce::badIndex) {
    return -1;
  }
  // otherwise, a param already exists so we get the method used to set it
  else {
  return (paramVec_[index].getMethodSet());
  }
}




#ifdef NDEBUG
#undef NDEBUG
#endif

  // self-test
void
LgFrSceSetOfParameters::test()
{
  
}


// self-test
void
LgFrSceParameter::test()
{
  
 }

#endif
