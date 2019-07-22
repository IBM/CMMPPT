// file: sceParam.h
// author: tom

#ifndef  LGFRSCESETOFPARAMETERS_H
#define  LGFRSCESETOFPARAMETERS_H

// RW2STL - inserted:
#include <vector>
#include <string>
// End of RW2STL - inserted includes.

#include <sce/src/sceDefin.h>
#include <mcl/src/mcl.h>


enum whichVal {DEFAULT_VAL, ACTUAL_VAL};


//  SCE parameter class 
class LgFrSceParameter {
  // friend class LgFrSceSetOfParameters;

public:

enum paramType {intP, floatP, stringP, boolP};  

  // self-test
static void test();

// string param  constructor
LgFrSceParameter(const std::string & name,
                 const std::string & stringVal,
                 paramType         type,
		 unsigned int methodSet = 0,
		 bool isMandatory = false);

  // float param  constructor
LgFrSceParameter(const std::string & name,
                 float             floatVal,
                 paramType         type,
		 unsigned int methodSet = 0,
		 bool isMandatory = false);  

  // int param  constructor
LgFrSceParameter(const std::string & name,
                 int               intVal ,
                 paramType         type,
		 unsigned int methodSet = 0,
		 bool isMandatory = false);  


  // preferred constructor
LgFrSceParameter(const std::string & name);


  // Set Parameter Value
  //** the whichOne arg refers to either DEFAULT_VAL or ACTUAL_VAL
  //   Note: the default for the set methods is DEFAULT_VAL 
int setStringVal(const std::string & stringVal, whichVal whichOne);
int setIntVal(int intVal, whichVal whichOne);
int setFloatVal(float floatVal, whichVal whichOne);
int setBoolVal(bool boolVal, whichVal whichOne);
void setMandatory(bool isMandatory); // only one

  // Get Parameter Value
  //** the whichOne arg refers to either DEFAULT_VAL or ACTUAL_VAL
  //   the default for the get methods is ACTUAL_VAL
std::string getStringVal(whichVal whichOne) const;
int       getIntVal(whichVal whichOne) const;
float     getFloatVal(whichVal whichOne) const;
bool      getBoolVal(whichVal whichOne) const;
bool      getMandatory() const;  // only one

  // Set/Get Current TYPE of parameter
void  setType(paramType);
paramType getType() const;

  // Get MethodSet_ flag value; Note: there is not explicit "SET" method
  //  intentionally, because the only time this should be set is when a
  //  parameter is created.
int getMethodSet() const;

  // get the parameter Name (used for operator ==)
std::string getName() const;


  
// copy constructor
LgFrSceParameter(const LgFrSceParameter& source);

// assignment operator
LgFrSceParameter&
operator=(const LgFrSceParameter& rhs);

// destructor
virtual
~LgFrSceParameter();

  // default constructor
LgFrSceParameter();

private:
  
  std::string      name_;
  // note: these are the default values
  std::string      defStringVal_;
  float          defFloatVal_;
  int            defIntVal_;
  bool      defBoolVal_;
  // note: these are the user-defined/used values
  std::string      actStringVal_;
  float          actFloatVal_;
  int            actIntVal_;
  bool      actBoolVal_;

  paramType      type_;
  // adding flag for method set {0=default;1=fromParamFile;maybe others}
  unsigned int   methodSet_;

  // another flag to indicate whether or not mandatory {true=Mandatory Param}
  bool      mandatory_;
};


// == operator
// IMPORTANT!! Two parameters are considered "==" if 
// lhs.name_.compareTo(rhs.name_, std::string::ignoreCase) == 0
bool
operator==(const LgFrSceParameter& lhs, const LgFrSceParameter& rhs);  




// RW2STL - declare (RWGVector, LgFrSceParameter)

// RW2STL - declare (RWGOrderedVector, LgFrSceParameter)

typedef std::vector<LgFrSceParameter> LgFrOrderedVectorParameter; // RW2STL - typedef RWGOrderedVector(LgFrSceParameter) LgFrOrderedVectorParameter;




// SCE Set of Parameters Class
class LgFrSceSetOfParameters {
public:

  // self-test
static void test();
  
// copy constructor
LgFrSceSetOfParameters(const LgFrSceSetOfParameters& source);

// assignment operator
LgFrSceSetOfParameters&
operator=(const LgFrSceSetOfParameters& rhs);

// destructor
virtual
~LgFrSceSetOfParameters();

  // default constructor
LgFrSceSetOfParameters();

// indexing operator
LgFrSceParameter&
operator()(size_t index);

// returns number of parameters in SOP
size_t
numParameters() const;


// Set Parameter
int
setString(std::string name,
          std::string stringVal,
	  unsigned int methodSet = 0,
	  bool isMandatory = false);

int
setInt(std::string name,
       int intVal,
       unsigned int methodSet = 0,
       bool isMandatory = false);

int
setFloat(std::string name,
         float floatVal,
	 unsigned int methodSet = 0,
	 bool isMandatory = false);

int
setBool(std::string name,
        bool boolVal,
	unsigned int methodSet = 0,
	bool isMandatory = false);
  
// Get Parameter
std::string
getString(std::string  name) const;

int
getInt(std::string name) const;

float
getFloat(std::string name) const;

bool 
getBool(std::string name) const;


// necessary for sharing error facility for messaging purposes
void
setErrFacility(MclFacility * existingFacility);

MclFacility*
getErrFacility();

// returns -1 if parameter of this type does not exist...
// otherwise returns int cast paramType {0,1,2,3}
int
exists(const std::string name) const;

// prints out status of all parameters -- param.log is default name
void printParamLog(std::string logFileName) const;


  // prints out a scrubbed param file
void
printScrubbedParamFile(std::string scrubParamFileName) const;


int getMethodSet(const std::string name) const;

private:
  LgFrOrderedVectorParameter    paramVec_;

  // necessary for error messaging by parameter class methods
  MclFacility * sceErrFacility_;
};


#endif
