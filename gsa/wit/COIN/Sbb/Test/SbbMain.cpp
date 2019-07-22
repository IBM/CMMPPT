// copyright (C) 2002, International Business Machines
// Corporation and others.  All Rights Reserved.
#if defined(_MSC_VER)
// Turn off compiler warning about long names
#  pragma warning(disable:4786)
#endif

#include <cassert>
#define SBBVERSION "0.60"

#include "CoinMpsIO.hpp"
#include "CoinPackedMatrix.hpp"
#include "CoinPackedVector.hpp"
#include "CoinWarmStartBasis.hpp"
// For Branch and bound
#include "OsiSolverInterface.hpp"
#include "OsiCuts.hpp"
#include "OsiRowCut.hpp"
#include "OsiColCut.hpp"

#include "SbbModel.hpp"
#define CUTS
#ifdef CUTS
#include "CglCutGenerator.hpp"
#include "CglGomory.hpp"
#include "CglProbing.hpp"
#include "CglKnapsackCover.hpp"
#include "CglOddHole.hpp"
#endif
#include "SbbHeuristic.hpp"
#ifdef COIN_USE_CLP
#include "OsiClpSolverInterface.hpp"
#endif
#include "SbbCompareBase.hpp"

/* Before first solution do depth first,
   then it is computed to hit first solution less 2%
*/
class SbbCompareUser  : public SbbCompareBase {
public:
  // Weight for each infeasibility
  double weight_;
  // Number of solutions
  int numberSolutions_;
  // Default Constructor 
  SbbCompareUser () : weight_(-1.0), numberSolutions_(0) {test_=this;};

  ~SbbCompareUser() {};

  /* 
     Return true if y better than x
     Node y is better than node x if y has fewer unsatisfied (greater depth on tie) or
     after solution weighted value of y is less than weighted value of x
  */
  virtual bool test (SbbNode * x, SbbNode * y) {
    if (weight_<0.0) {
      // before solution
      /* printf("x %d %d %g, y %d %d %g\n",
	     x->numberUnsatisfied(),x->depth(),x->objectiveValue(),
	     y->numberUnsatisfied(),y->depth(),y->objectiveValue()); */
      if (x->numberUnsatisfied() > y->numberUnsatisfied())
	return true;
      else if (x->numberUnsatisfied() < y->numberUnsatisfied())
	return false;
      else
	return x->depth() < y->depth();
    } else {
      // after solution
      return x->objectiveValue()+ weight_*x->numberUnsatisfied() > 
	y->objectiveValue() + weight_*y->numberUnsatisfied();
    }
  }
  // This allows method to change behavior as it is called
  // after each solution
  virtual void newSolution(SbbModel * model,
			   double objectiveAtContinuous,
			   int numberInfeasibilitiesAtContinuous) 
  {
    if (model->getSolutionCount()==model->getNumberHeuristicSolutions())
      return; // solution was got by rounding
    // set to get close to this solution
    double costPerInteger = 
      (model->getObjValue()-objectiveAtContinuous)/
      ((double) numberInfeasibilitiesAtContinuous);
    weight_ = 0.98*costPerInteger;
    numberSolutions_++;
    if (numberSolutions_>5)
      weight_ =0.0; // this searches on objective
  }
  // This allows method to change behavior 
  virtual void every1000Nodes(SbbModel * model, int numberNodes)
  {
    if (numberNodes>10000)
      weight_ =0.0; // this searches on objective
  }
};

#include <stdio.h>
#include <cmath>
#include <cfloat>

#include <string>
#include <iostream>
#include  <time.h>
#if !defined(_MSC_VER)
#include <sys/times.h>
#include <sys/resource.h>
#include <unistd.h>
#endif
static double totalTime=0.0;
static double cpuTime()
{
  double cpu_temp;
#if defined(_MSC_VER)
  unsigned int ticksnow;        /* clock_t is same as int */
  
  ticksnow = (unsigned int)clock();
  
  cpu_temp = (double)((double)ticksnow/CLOCKS_PER_SEC);
#else
  struct rusage usage;
  getrusage(RUSAGE_SELF,&usage);
  cpu_temp = usage.ru_utime.tv_sec;
  cpu_temp += 1.0e-6*((double) usage.ru_utime.tv_usec);
#endif
  return cpu_temp;
}


//#############################################################################

#ifdef NDEBUG
#undef NDEBUG
#endif

enum SbbParameterType {
  GENERALQUERY=-100,
  
  PRIMALTOLERANCE=1,DUALTOLERANCE,CUTOFF,
  INFEASIBILITYWEIGHT=51,INTEGERTOLERANCE,INCREMENT,ALLOWABLEGAP,

  DJFIX=81,GAPRATIO,TIGHTENFACTOR,

  LOGLEVEL=151, SOLVERLOGLEVEL, MAXNODES,STRONGBRANCHING,
  
  DIRECTION=201,ERRORSALLOWED,KEEPNAMES,SCALING,
  NODESTRATEGY=251,BRANCHSTRATEGY,ADDCUTS,STRATEGY,
  GOMORYCUTS,PROBINGCUTS,KNAPSACKCUTS,ODDHOLECUTS,PRESOLVE,
  ROUNDING,
  
  DIRECTORY=301,IMPORT,EXPORT,RESTORE,SAVE,SOLVECONTINUOUS,BAB,
  MAXIMIZE,MINIMIZE,EXIT,STDIN,UNITTEST,MIPLIB,SOLUTION,CLEARCUTS,
  VERSION,

  OSLSTUFF=401,SBBSTUFF,

  INVALID=1000
};
/// Very simple class for setting parameters
class SbbItem {

public:

  /**@name Constructor and destructor */
  //@{
  /// Constructors
  SbbItem (  );
  SbbItem (std::string name, std::string help,
	   double lower, double upper, SbbParameterType type);
  SbbItem (std::string name, std::string help,
	   int lower, int upper, SbbParameterType type);
  // Other strings will be added by insert
  SbbItem (std::string name, std::string help, std::string defaultValue,
	   SbbParameterType type);
  // Action
  SbbItem (std::string name, std::string help,
	   SbbParameterType type);
  /// Copy constructor. 
  SbbItem(const SbbItem &);
  /// Assignment operator. This copies the data
    SbbItem & operator=(const SbbItem & rhs);
  /// Destructor
  ~SbbItem (  );
  //@}

  /**@name stuff */
  //@{
  /// Insert string (only valid for keywords)
  void append(std::string keyWord);
  /// Adds one help line
  void addHelp(std::string keyWord);
  /// Returns name
  inline std::string  name(  ) const {
    return name_;
  };
  /// Returns short help
  inline std::string  shortHelp(  ) const {
    return shortHelp_;
  };
  /// Sets a double parameter (nonzero code if error)
  int setDoubleParameter(OsiSolverInterface * model, double value) const;
  /// Gets a double parameter
  double doubleParameter(OsiSolverInterface * model) const;
  /// Sets a int parameter (nonzero code if error)
  int setIntParameter(OsiSolverInterface * model, int value) const;
  /// Gets a int parameter
  int intParameter(OsiSolverInterface * model) const;
  /// Sets a double parameter (nonzero code if error)
  int setDoubleParameter(SbbModel &model, double value) const;
  /// Gets a double parameter
  double doubleParameter(SbbModel &model) const;
  /// Checks a double parameter (nonzero code if error)
  int checkDoubleParameter(double value) const;
  /// Sets a int parameter (nonzero code if error)
  int setIntParameter(SbbModel &model, int value) const;
  /// Gets a int parameter
  int intParameter(SbbModel &model) const;
  /// Returns name which could match
  std::string matchName (  ) const;
  /// Returns parameter option which matches (-1 if none)
  int parameterOption ( std::string check ) const;
  /// Prints parameter options
  void printOptions (  ) const;
  /// Returns current parameter option
  inline std::string currentOption (  ) const
  { return definedKeyWords_[currentKeyWord_]; };
  /// Sets current parameter option
  inline void setCurrentOption ( int value )
  { currentKeyWord_=value; };
  /// Returns 1 if matches minimum, 2 if matches less, 0 if not matched
  int matches (std::string input) const;
  /// type
  inline SbbParameterType type() const
  { return type_;};
private:
  /// gutsOfConstructor
  void gutsOfConstructor();
  //@}
////////////////// data //////////////////
private:

  /**@name data 
   We might as well throw all type data in - could derive?
  */
  //@{
  // Type see SbbParameterType
  SbbParameterType type_;
  /// If double == okay
  double lowerDoubleValue_;
  double upperDoubleValue_;
  /// If int == okay
  int lowerIntValue_;
  int upperIntValue_;
  // Length of name
  unsigned int lengthName_;
  // Minimum match
  unsigned int lengthMatch_;
  /// set of valid strings
  std::vector<std::string> definedKeyWords_;
  /// Name
  std::string name_;
  /// Short help
  std::string shortHelp_;
  /// Long help
  std::vector<std::string> longHelp_;
  /// Action
  SbbParameterType action_;
  /// Current keyWord (if a keyword parameter)
  int currentKeyWord_;
  //@}
};
//#############################################################################
// Constructors / Destructor / Assignment
//#############################################################################

//-------------------------------------------------------------------
// Default Constructor 
//-------------------------------------------------------------------
SbbItem::SbbItem () 
  : type_(INVALID),
    lowerDoubleValue_(0.0),
    upperDoubleValue_(0.0),
    lowerIntValue_(0),
    upperIntValue_(0),
    lengthName_(0),
    lengthMatch_(0),
    definedKeyWords_(),
    name_(),
    shortHelp_(),
    longHelp_(),
    action_(INVALID),
    currentKeyWord_(-1)
{
}
// Other constructors
SbbItem::SbbItem (std::string name, std::string help,
	   double lower, double upper, SbbParameterType type)
  : type_(type),
    lowerIntValue_(0),
    upperIntValue_(0),
    definedKeyWords_(),
    name_(name),
    shortHelp_(help),
    longHelp_(),
    action_(type),
    currentKeyWord_(-1)
{
  lowerDoubleValue_ = lower;
  upperDoubleValue_ = upper;
  gutsOfConstructor();
}
SbbItem::SbbItem (std::string name, std::string help,
	   int lower, int upper, SbbParameterType type)
  : type_(type),
    lowerDoubleValue_(0.0),
    upperDoubleValue_(0.0),
    definedKeyWords_(),
    name_(name),
    shortHelp_(help),
    longHelp_(),
    action_(type),
    currentKeyWord_(-1)
{
  gutsOfConstructor();
  lowerIntValue_ = lower;
  upperIntValue_ = upper;
}
// Other strings will be added by append
SbbItem::SbbItem (std::string name, std::string help, 
		  std::string defaultValue,
		  SbbParameterType type)
  : type_(type),
    lowerDoubleValue_(0.0),
    upperDoubleValue_(0.0),
    lowerIntValue_(0),
    upperIntValue_(0),
    definedKeyWords_(),
    name_(name),
    shortHelp_(help),
    longHelp_(),
    action_(type),
    currentKeyWord_(0)
{
  gutsOfConstructor();
  definedKeyWords_.push_back(defaultValue);
}
// Action
SbbItem::SbbItem (std::string name, std::string help,
	   SbbParameterType type)
  : type_(type),
    lowerDoubleValue_(0.0),
    upperDoubleValue_(0.0),
    lowerIntValue_(0),
    upperIntValue_(0),
    definedKeyWords_(),
    name_(name),
    shortHelp_(help),
    longHelp_(),
    action_(type),
    currentKeyWord_(-1)
{
  gutsOfConstructor();
}

//-------------------------------------------------------------------
// Copy constructor 
//-------------------------------------------------------------------
SbbItem::SbbItem (const SbbItem & rhs) 
{  
  type_ = rhs.type_;
  lowerDoubleValue_ = rhs.lowerDoubleValue_;
  upperDoubleValue_ = rhs.upperDoubleValue_;
  lowerIntValue_ = rhs.lowerIntValue_;
  upperIntValue_ = rhs.upperIntValue_;
  lengthName_ = rhs.lengthName_;
  lengthMatch_ = rhs.lengthMatch_;
  definedKeyWords_ = rhs.definedKeyWords_;
  name_ = rhs.name_;
  shortHelp_ = rhs.shortHelp_;
  longHelp_ = rhs.longHelp_;
  action_ = rhs.action_;
  currentKeyWord_ = rhs.currentKeyWord_;
  
}

//-------------------------------------------------------------------
// Destructor 
//-------------------------------------------------------------------
SbbItem::~SbbItem ()
{
}

//----------------------------------------------------------------
// Assignment operator 
//-------------------------------------------------------------------
SbbItem &
SbbItem::operator=(const SbbItem& rhs)
{
  if (this != &rhs) {
    type_ = rhs.type_;
    lowerDoubleValue_ = rhs.lowerDoubleValue_;
    upperDoubleValue_ = rhs.upperDoubleValue_;
    lowerIntValue_ = rhs.lowerIntValue_;
    upperIntValue_ = rhs.upperIntValue_;
    lengthName_ = rhs.lengthName_;
    lengthMatch_ = rhs.lengthMatch_;
    definedKeyWords_ = rhs.definedKeyWords_;
    name_ = rhs.name_;
    shortHelp_ = rhs.shortHelp_;
    longHelp_ = rhs.longHelp_;
    action_ = rhs.action_;
    currentKeyWord_ = rhs.currentKeyWord_;
  }
  return *this;
}
void 
SbbItem::gutsOfConstructor()
{
  unsigned int  shriekPos = name_.find('!');
  lengthName_ = name_.length();
  if ( shriekPos==std::string::npos ) {
    //does not contain '!'
    lengthMatch_= lengthName_;
  } else {
    lengthMatch_=shriekPos;
    name_ = name_.substr(0,shriekPos)+name_.substr(shriekPos+1);
    lengthName_--;
  }
}
// Insert string (only valid for keywords)
void 
SbbItem::append(std::string keyWord)
{
  definedKeyWords_.push_back(keyWord);
}

int 
SbbItem::matches (std::string input) const
{
  // look up strings to do more elegantly
  if (input.length()>lengthName_) {
    return 0;
  } else {
    unsigned int i;
    for (i=0;i<input.length();i++) {
      if (tolower(name_[i])!=tolower(input[i])) 
	break;
    }
    if (i<input.length()) {
      return 0;
    } else if (i>=lengthMatch_) {
      return 1;
    } else {
      // matched but too short
      return 2;
    }
  }
}
// Returns name which could match
std::string 
SbbItem::matchName (  ) const
{ 
  if (lengthMatch_==lengthName_) 
    return name_;
  else
    return name_.substr(0,lengthMatch_)+"("+name_.substr(lengthMatch_)+")";
}

// Returns parameter option which matches (-1 if none)
int 
SbbItem::parameterOption ( std::string check ) const
{
  int numberItems = definedKeyWords_.size();
  if (!numberItems) {
    return -1;
  } else {
    int whichItem=0;
    unsigned int it;
    for (it=0;it<definedKeyWords_.size();it++) {
      std::string thisOne = definedKeyWords_[it];
      unsigned int  shriekPos = thisOne.find('!');
      unsigned int length1 = thisOne.length();
      unsigned int length2 = length1;
      if ( shriekPos!=std::string::npos ) {
	//contains '!'
	length2 = shriekPos;
	thisOne = thisOne.substr(0,shriekPos)+
	  thisOne.substr(shriekPos+1);
	length1 = thisOne.length();
      }
      if (check.length()<=length1) {
	unsigned int i;
	for (i=0;i<check.length();i++) {
	  if (tolower(thisOne[i])!=tolower(check[i])) 
	    break;
	}
	if (i<check.length()) {
	  whichItem++;
	} else if (i>=length2) {
	  break;
	} 
      } else {
	whichItem++;
      }
    }
    if (whichItem<numberItems)
      return whichItem;
    else
      return -1;
  }
}
// Prints parameter options
void 
SbbItem::printOptions (  ) const
{
  std::cout<<"Possible options for "<<name_<<" are:"<<std::endl;
  unsigned int it;
  for (it=0;it<definedKeyWords_.size();it++) {
    std::string thisOne = definedKeyWords_[it];
    unsigned int  shriekPos = thisOne.find('!');
    if ( shriekPos!=std::string::npos ) {
      //contains '!'
      thisOne = thisOne.substr(0,shriekPos)+
	"("+thisOne.substr(shriekPos+1)+")";
    }
    std::cout<<thisOne<<std::endl;
  }
}
int
SbbItem::setDoubleParameter (OsiSolverInterface * model,double value) const
{
  if (value<lowerDoubleValue_||value>upperDoubleValue_) {
    std::cout<<value<<" was provided for "<<name_<<
      " - valid range is "<<lowerDoubleValue_<<" to "<<
      upperDoubleValue_<<std::endl;
    return 1;
  } else {
    double oldValue;
    switch(type_) {
    case DUALTOLERANCE:
      model->getDblParam(OsiDualTolerance,oldValue);
      model->setDblParam(OsiDualTolerance,value);
      break;
    case PRIMALTOLERANCE:
      model->getDblParam(OsiPrimalTolerance,oldValue);
      model->setDblParam(OsiPrimalTolerance,value);
      break;
    default:
      oldValue=0.0; // to avoid compiler message
      abort();
    }
    std::cout<<name_<<" was changed from "<<oldValue<<" to "
	     <<value<<std::endl;
    return 0;
  }
}
int
SbbItem::checkDoubleParameter (double value) const
{
  if (value<lowerDoubleValue_||value>upperDoubleValue_) {
    std::cout<<value<<" was provided for "<<name_<<
      " - valid range is "<<lowerDoubleValue_<<" to "<<
      upperDoubleValue_<<std::endl;
    return 1;
  } else {
    return 0;
  }
}
double 
SbbItem::doubleParameter (OsiSolverInterface * model) const
{
  double value;
  switch(type_) {
  case DUALTOLERANCE:
    assert(model->getDblParam(OsiDualTolerance,value));
    break;
  case PRIMALTOLERANCE:
    assert(model->getDblParam(OsiPrimalTolerance,value));
    break;
  default:
    abort();
  }
  return value;
}
int 
SbbItem::setIntParameter (OsiSolverInterface * model,int value) const
{
  if (value<lowerIntValue_||value>upperIntValue_) {
    std::cout<<value<<" was provided for "<<name_<<
      " - valid range is "<<lowerIntValue_<<" to "<<
      upperIntValue_<<std::endl;
    return 1;
  } else {
    int oldValue;
    switch(type_) {
    case LOGLEVEL:
      model->messageHandler()->setLogLevel(value);
      break;
    default:
      oldValue=0; // to avoid compiler message
      abort();
    }
    std::cout<<name_<<" was changed from "<<oldValue<<" to "
	     <<value<<std::endl;
    return 0;
  }
}
int 
SbbItem::intParameter (OsiSolverInterface * model) const
{
  int value=0;
  switch(type_) {
  case LOGLEVEL:
    //value=model->logLevel();
    break;
  default:
    abort();
  }
  return value;
}
int
SbbItem::setDoubleParameter (SbbModel &model,double value) const
{
  if (value<lowerDoubleValue_||value>upperDoubleValue_) {
    std::cout<<value<<" was provided for "<<name_<<
      " - valid range is "<<lowerDoubleValue_<<" to "<<
      upperDoubleValue_<<std::endl;
    return 1;
  } else {
    double oldValue;
    switch(type_) {
    case INFEASIBILITYWEIGHT:
      oldValue=model.getDblParam(SbbModel::SbbInfeasibilityWeight);
      model.setDblParam(SbbModel::SbbInfeasibilityWeight,value);
      break;
    case INTEGERTOLERANCE:
      oldValue=model.getDblParam(SbbModel::SbbIntegerTolerance);
      model.setDblParam(SbbModel::SbbIntegerTolerance,value);
      break;
    case INCREMENT:
      oldValue=model.getDblParam(SbbModel::SbbCutoffIncrement);
      model.setDblParam(SbbModel::SbbCutoffIncrement,value);
    case ALLOWABLEGAP:
      oldValue=model.getDblParam(SbbModel::SbbAllowableGap);
      model.setDblParam(SbbModel::SbbAllowableGap,value);
      break;
    default:
      oldValue=0.0; // to avoid compiler message
      abort();
    }
    std::cout<<name_<<" was changed from "<<oldValue<<" to "
	     <<value<<std::endl;
    return 0;
  }
}
double 
SbbItem::doubleParameter (SbbModel &model) const
{
  double value;
  switch(type_) {
  case INFEASIBILITYWEIGHT:
    value=model.getDblParam(SbbModel::SbbInfeasibilityWeight);
    break;
  case INTEGERTOLERANCE:
    value=model.getDblParam(SbbModel::SbbIntegerTolerance);
    break;
  case INCREMENT:
    value=model.getDblParam(SbbModel::SbbCutoffIncrement);
  case ALLOWABLEGAP:
    value=model.getDblParam(SbbModel::SbbAllowableGap);
    break;
  default:
    abort();
  }
  return value;
}
int 
SbbItem::setIntParameter (SbbModel &model,int value) const
{
  if (value<lowerIntValue_||value>upperIntValue_) {
    std::cout<<value<<" was provided for "<<name_<<
      " - valid range is "<<lowerIntValue_<<" to "<<
      upperIntValue_<<std::endl;
    return 1;
  } else {
    int oldValue;
    switch(type_) {
    case LOGLEVEL:
      oldValue = model.messageHandler()->logLevel();
      model.messageHandler()->setLogLevel(value);
      break;
    case SOLVERLOGLEVEL:
      oldValue = model.solver()->messageHandler()->logLevel();
      model.solver()->messageHandler()->setLogLevel(value);
      break;
    case MAXNODES:
      oldValue=model.getIntParam(SbbModel::SbbMaxNumNode);
      model.setIntParam(SbbModel::SbbMaxNumNode,value);
      break;
    case STRONGBRANCHING:
      oldValue=model.numberStrong();
      model.setNumberStrong(value);
      break;
    default:
      oldValue=0; // to avoid compiler message
      abort();
    }
    std::cout<<name_<<" was changed from "<<oldValue<<" to "
	     <<value<<std::endl;
    return 0;
  }
}
int 
SbbItem::intParameter (SbbModel &model) const
{
  int value;
  switch(type_) {
  case LOGLEVEL:
    value = model.messageHandler()->logLevel();
      break;
  case SOLVERLOGLEVEL:
    value = model.solver()->messageHandler()->logLevel();
      break;
  case MAXNODES:
    value = model.getIntParam(SbbModel::SbbMaxNumNode);
    break;
  case STRONGBRANCHING:
    value=model.numberStrong();
    break;
  default:
    abort();
  }
  return value;
}
#ifdef READLINE     
#include <readline/readline.h>
#include <readline/history.h>
#endif
// Returns next valid field
static int read_mode=1;
static char line[1000];
static char * where=NULL;

std::string
nextField()
{
  std::string field;
  if (!where) {
    // need new line
#ifdef READLINE     
    // Get a line from the user. 
    where = readline ("Sbb:");
     
    // If the line has any text in it, save it on the history.
    if (where) {
      if ( *where)
	add_history (where);
      strcpy(line,where);
    }
#else
    fprintf(stdout,"Sbb:");
    fflush(stdout);
    where = fgets(line,1000,stdin);
#endif
    if (!where)
      return field; // EOF
    where = line;
    // clean image
    char * lastNonBlank = line-1;
    while ( *where != '\0' ) {
      if ( *where != '\t' && *where < ' ' ) {
	break;
      } else if ( *where != '\t' && *where != ' ') {
	lastNonBlank = where;
      }
      where++;
    }
    where=line;
    *(lastNonBlank+1)='\0';
  }
  // munch white space
  while(*where==' '||*where=='\t')
    where++;
  char * saveWhere = where;
  while (*where!=' '&&*where!='\t'&&*where!='\0')
    where++;
  if (where!=saveWhere) {
    char save = *where;
    *where='\0';
    //convert to string
    field=saveWhere;
    *where=save;
  } else {
    where=NULL;
    field="EOL";
  }
  return field;
}

std::string
getCommand(int argc, const char *argv[])
{
  std::string field="EOL";
  while (field=="EOL") {
    if (read_mode>0) {
      if (read_mode<argc) {
	field = argv[read_mode++];
	if (field=="-") {
	  std::cout<<"Switching to line mode"<<std::endl;
	  read_mode=-1;
	  field=nextField();
	} else if (field[0]!='-') {
	  if (read_mode!=2) {
	    std::cout<<"skipping non-command "<<field<<std::endl;
	    field="EOL"; // skip
	  } else {
	    // special dispensation - taken as -import name
	    read_mode--;
	    field="import";
	  }
	} else {
	  if (field!="--") {
	    // take off -
	    field = field.substr(1);
	  } else {
	    // special dispensation - taken as -import --
	    read_mode--;
	    field="import";
	  }
	}
      } else {
	field="";
      }
    } else {
      field=nextField();
    }
  }
  //std::cout<<field<<std::endl;
  return field;
}
std::string
getString(int argc, const char *argv[])
{
  std::string field="EOL";
  if (read_mode>0) {
    if (read_mode<argc) {
      if (argv[read_mode][0]!='-') { 
	field = argv[read_mode++];
      } else if (!strcmp(argv[read_mode],"--")) {
	field = argv[read_mode++];
	// -- means import from stdin
	field = "-";
      }
    }
  } else {
    field=nextField();
  }
  //std::cout<<field<<std::endl;
  return field;
}
// valid 0 - okay, 1 bad, 2 not there
int
getIntField(int argc, const char *argv[],int * valid)
{
  std::string field="EOL";
  if (read_mode>0) {
    if (read_mode<argc) {
      // may be negative value so do not check for -
      field = argv[read_mode++];
    }
  } else {
    field=nextField();
  }
  int value=0;
  //std::cout<<field<<std::endl;
  if (field!="EOL") {
    // how do I check valid
    value =  atoi(field.c_str());
    *valid=0;
  } else {
    *valid=2;
  }
  return value;
}
double
getDoubleField(int argc, const char *argv[],int * valid)
{
  std::string field="EOL";
  if (read_mode>0) {
    if (read_mode<argc) {
      // may be negative value so do not check for -
      field = argv[read_mode++];
    }
  } else {
    field=nextField();
  }
  double value=0.0;
  //std::cout<<field<<std::endl;
  if (field!="EOL") {
    // how do I check valid
    value = atof(field.c_str());
    *valid=0;
  } else {
    *valid=2;
  }
  return value;
}
int main (int argc, const char *argv[])
{
  // next {} is just to make sure all memory should be freed - for debug
  {
    double time1 = cpuTime(),time2;
#define MAXPARAMETERS 100
    SbbItem parameters[MAXPARAMETERS];
    int numberParameters=0;
    parameters[numberParameters++]=
      SbbItem("?","For help",GENERALQUERY);
    parameters[numberParameters++]=
      SbbItem("dualT!olerance","For an optimal solution \
no dual infeasibility may exceed this value",
	      1.0e-20,1.0e12,DUALTOLERANCE);
    parameters[numberParameters++]=
      SbbItem("primalT!olerance","For an optimal solution \
no primal infeasibility may exceed this value",
	      1.0e-20,1.0e12,PRIMALTOLERANCE);
    parameters[numberParameters++]=
      SbbItem("inf!easibilityWeight","Each integer infeasibility is expected \
to cost this much",
	      0.0,1.0e20,INFEASIBILITYWEIGHT);
    parameters[numberParameters++]=
      SbbItem("integerT!olerance","For an optimal solution \
no integer variable may be this away from an integer value",
	      1.0e-20,0.5,INTEGERTOLERANCE);
    parameters[numberParameters++]=
      SbbItem("inc!rement","A valid solution must be at least this \
much better than last integer solution",
	      -1.0e20,1.0e20,INCREMENT);
    parameters[numberParameters++]=
      SbbItem("allow!ableGap","Stop when gap between best possible and \
best less than this",
	      0.0,1.0e20,ALLOWABLEGAP);
    parameters[numberParameters++]=
      SbbItem("ratio!Gap","Stop when gap between best possible and \
best less than this fraction of continuous optimum",
	      0.0,1.0e20,GAPRATIO);
    parameters[numberParameters++]=
      SbbItem("fix!OnDj","Try heuristic based on fixing variables with \
reduced costs greater than this",
	      -1.0e20,1.0e20,DJFIX);
    parameters[numberParameters++]=
      SbbItem("tighten!Factor","Tighten bounds using this times largest \
activity at continuous solution",
	      1.0,1.0e20,TIGHTENFACTOR);
    parameters[numberParameters++]=
      SbbItem("log!Level","Level of detail in BAB output",
	      0,63,LOGLEVEL);
    parameters[numberParameters++]=
      SbbItem("slog!Level","Level of detail in Solver output",
	      0,63,SOLVERLOGLEVEL);
    parameters[numberParameters++]=
      SbbItem("maxN!odes","Maximum number of nodes to do",
	      1,999999,MAXNODES);
    parameters[numberParameters++]=
      SbbItem("strong!Branching","Number of variables to look at in strong branching",
	      0,999999,STRONGBRANCHING);
    parameters[numberParameters++]=
      SbbItem("direction","Minimize or Maximize",
	      "min!imize",DIRECTION);
    parameters[numberParameters-1].append("max!imize");
    parameters[numberParameters++]=
      SbbItem("error!sAllowed","Whether to allow import errors",
	      "off",ERRORSALLOWED);
    parameters[numberParameters-1].append("on");
    parameters[numberParameters++]=
      SbbItem("gomory!Cuts","Whether to use Gomory cuts",
	      "off",GOMORYCUTS);
    parameters[numberParameters-1].append("on");
    parameters[numberParameters++]=
      SbbItem("probing!Cuts","Whether to use Probing cuts",
	      "off",PROBINGCUTS);
    parameters[numberParameters-1].append("on");
    parameters[numberParameters++]=
      SbbItem("knapsack!Cuts","Whether to use Knapsack cuts",
	      "off",KNAPSACKCUTS);
    parameters[numberParameters-1].append("on");
    parameters[numberParameters++]=
      SbbItem("oddhole!Cuts","Whether to use Oddhole cuts",
	      "off",ODDHOLECUTS);
    parameters[numberParameters-1].append("on");
    parameters[numberParameters++]=
      SbbItem("round!ingHeuristic","Whether to use Rounding heuristic",
	      "off",ROUNDING);
    parameters[numberParameters-1].append("on");
    parameters[numberParameters++]=
      SbbItem("keepN!ames","Whether to keep names from import",
	      "on",KEEPNAMES);
    parameters[numberParameters-1].append("off");
    parameters[numberParameters++]=
      SbbItem("scaling","Whether to do scaling",
	      "on",SCALING);
    parameters[numberParameters-1].append("off");
    parameters[numberParameters++]=
      SbbItem("directory","Set Default import directory",
	      DIRECTORY);
    parameters[numberParameters++]=
      SbbItem("import","Import model from mps file",
	      IMPORT);
    parameters[numberParameters++]=
      SbbItem("export","Export model as mps file",
	      EXPORT);
    parameters[numberParameters++]=
      SbbItem("save!Model","Save model to binary file",
	      SAVE);
    parameters[numberParameters++]=
      SbbItem("restore!Model","Restore model from binary file",
	      RESTORE);
    parameters[numberParameters++]=
      SbbItem("presolve","Whether to use integer presolve - be careful",
	      "off",PRESOLVE);
    parameters[numberParameters-1].append("on");
    parameters[numberParameters++]=
      SbbItem("initialS!olve","Solve to continuous",
	      SOLVECONTINUOUS);
    parameters[numberParameters++]=
      SbbItem("branch!Andbound","Do Branch and Bound",
	      BAB);
    parameters[numberParameters++]=
      SbbItem("sol!ution","Prints solution to file",
	      SOLUTION);
    parameters[numberParameters++]=
      SbbItem("max!imize","Set optimization direction to maximize",
	      MAXIMIZE);
    parameters[numberParameters++]=
      SbbItem("min!imize","Set optimization direction to minimize",
	      MINIMIZE);
    parameters[numberParameters++]=
      SbbItem("exit","Stops sbb execution",
	      EXIT);
    parameters[numberParameters++]=
      SbbItem("stop","Stops sbb execution",
	      EXIT);
    parameters[numberParameters++]=
      SbbItem("quit","Stops sbb execution",
	      EXIT);
    parameters[numberParameters++]=
      SbbItem("-","From stdin",
	      STDIN);
    parameters[numberParameters++]=
      SbbItem("stdin","From stdin",
	      STDIN);
    parameters[numberParameters++]=
      SbbItem("unitTest","Do unit test",
	      UNITTEST);
    parameters[numberParameters++]=
      SbbItem("miplib","Do some of miplib test set",
	      MIPLIB);
    parameters[numberParameters++]=
      SbbItem("ver!sion","Print out version",
	      VERSION);
    assert(numberParameters<MAXPARAMETERS);
    
    // total number of commands read
    int numberGoodCommands=0;
    //int numberModels=1;
#ifdef COIN_USE_CLP
    OsiClpSolverInterface solver1;
    solver1.messageHandler()->setLogLevel(0);
    SbbModel * model = new SbbModel(solver1);
    model->messageHandler()->setLogLevel(1);
#endif
    bool goodModel=false;
    
    
    // default action on import
    int allowImportErrors=0;
    int keepImportNames=1;
    int doScaling=1;
    int preSolve=0;
    double djFix=1.0e100;
    double gapRatio=1.0e100;
    double tightenFactor=0.0;
    // Set up likely cut generators and defaults
    CglGomory try1;
    CglProbing try2;
    try2.setUsingObjective(true);
    try2.setMaxPass(3);
    try2.setMaxProbe(100);
    try2.setMaxLook(50);
    try2.setRowCuts(3);
    CglKnapsackCover try3;
    CglOddHole try4;
    try4.setMinimumViolation(0.005);
    try4.setMinimumViolationPer(0.0002);
    try4.setMaximumEntries(100);
    bool useRounding=false;
   
    std::string directory ="./";
    std::string field;
    
    while (1) {
      // next command
      field=getCommand(argc,argv);
      
      // exit if null or similar
      if (!field.length()) {
	if (numberGoodCommands==1&&goodModel) {
	  // we just had file name
	  model->initialSolve();
	  model->solver()->messageHandler()->setLogLevel(0);
	  model->branchAndBound();
	  time2 = cpuTime();
	  totalTime += time2-time1;
	  std::cout<<"Result "<<model->getObjValue()<<
	    " iterations "<<model->getIterationCount()<<
	    " nodes "<<model->getNodeCount()<<
	    " took "<<time2-time1<<" seconds - total "<<totalTime<<std::endl;
	} else if (!numberGoodCommands) {
	  // let's give the sucker a hint
	  std::cout
	    <<"Sbb takes input from arguments ( - switches to stdin)"
	    <<std::endl
	    <<"Enter ? for list of commands or (-)unitTest or -miplib"
	    <<" for tests"<<std::endl;
	}
	break;
      }
      
      // see if ? at end
      int numberQuery=0;
      if (field!="?") {
	int length = field.length();
	int i;
	for (i=length-1;i>0;i--) {
	  if (field[i]=='?') 
	    numberQuery++;
	  else
	    break;
	}
	field=field.substr(0,length-numberQuery);
      }
      // find out if valid command
      int iParam;
      int numberMatches=0;
      for ( iParam=0; iParam<numberParameters; iParam++ ) {
	int match = parameters[iParam].matches(field);
	if (match==1) {
	  numberMatches = 1;
	  break;
	} else {
	  numberMatches += match>>1;
	}
      }
      if (iParam<numberParameters&&!numberQuery) {
	// found
	SbbItem found = parameters[iParam];
	SbbParameterType type = found.type();
	int valid;
	numberGoodCommands++;
	if (type==GENERALQUERY) {
	  std::cout<<"In argument list keywords have leading - "
	    ", -stdin or just - switches to stdin"<<std::endl;
	  std::cout<<"One command per line (and no -)"<<std::endl;
	  std::cout<<"abcd? gives list of possibilities, if only one + explanation"<<std::endl;
	  std::cout<<"abcd?? adds explanation, if only one fuller help(LATER)"<<std::endl;
	  std::cout<<"abcd without value (where expected) gives current value"<<std::endl;
	  std::cout<<"abcd value or abcd = value sets value"<<std::endl;
	  std::cout<<"Commands are:"<<std::endl;
	  for ( iParam=0; iParam<numberParameters; iParam+=4 ) {
	    int i;
	    for (i=iParam;i<min(numberParameters,iParam+4);i++) 
	      std::cout<<parameters[i].matchName()<<"  ";
	    std::cout<<std::endl;
	  }
	} else if (type<81) {
	  // get next field as double
	  double value = getDoubleField(argc,argv,&valid);
	  if (!valid) {
	    parameters[iParam].setDoubleParameter(*model,value);
	  } else if (valid==1) {
	    abort();
	  } else {
	    std::cout<<parameters[iParam].name()<<" has value "<<
	      parameters[iParam].doubleParameter(*model)<<std::endl;
	  }
	} else if (type<101) {
	  // get next field as double for local use
	  double value = getDoubleField(argc,argv,&valid);
	  if (!valid) {
	    if (!parameters[iParam].checkDoubleParameter(value)) {
	      switch(type) {
	      case DJFIX:
		djFix=value;
		preSolve=5;
		break;
	      case GAPRATIO:
		gapRatio=value;
		break;
	      case TIGHTENFACTOR:
		tightenFactor=value;
		break;
	      default:
		abort();
	      }
	    }
	  } else if (valid==1) {
	    abort();
	  } else {
	    switch(type) {
	    case DJFIX:
	      djFix=value;
	      preSolve=5;
	      break;
	    case GAPRATIO:
	      gapRatio=value;
	      break;
	    default:
	      abort();
	    }
	    std::cout<<parameters[iParam].name()<<" has value "<<
	      parameters[iParam].doubleParameter(*model)<<std::endl;
	  }
	} else if (type<201) {
	  // get next field as int
	  int value = getIntField(argc,argv,&valid);
	  if (!valid) {
	    parameters[iParam].setIntParameter(*model,value);
	  } else if (valid==1) {
	    abort();
	  } else {
	    std::cout<<parameters[iParam].name()<<" has value "<<
	      parameters[iParam].intParameter(*model)<<std::endl;
	  }
	} else if (type<301) {
	  // one of several strings
	  std::string value = getString(argc,argv);
	  int action = parameters[iParam].parameterOption(value);
	  if (action<0) {
	    if (value!="EOL") {
	      // no match
	      parameters[iParam].printOptions();
	    } else {
	      // print current value
	      std::cout<<parameters[iParam].name()<<" has value "<<
		parameters[iParam].currentOption()<<std::endl;
	    }
	  } else {
	    parameters[iParam].setCurrentOption(action);
	    // for now hard wired
	    switch (type) {
	    case DIRECTION:
	      if (action==0)
		model->solver()->setObjSense(1);
	      else
		model->solver()->setObjSense(-1);
	      break;
	    case ERRORSALLOWED:
	      allowImportErrors = action;
	      break;
	    case KEEPNAMES:
	      keepImportNames = 1-action;
	      break;
	    case SCALING:
	      doScaling = 1-action;
	      break;
	    case GOMORYCUTS:
	      model->addCutGenerator(&try1,-1,"Gomory");
	      break;
	    case PROBINGCUTS:
	      model->addCutGenerator(&try2,-1,"Probing");
	      break;
	    case KNAPSACKCUTS:
	      model->addCutGenerator(&try3,-1,"Knapsack");
	      break;
	    case ODDHOLECUTS:
	      model->addCutGenerator(&try4,-1,"OddHole");
	      break;
	    case ROUNDING:
	      useRounding = action;
	      break;
	    case PRESOLVE:
	      preSolve = action*5;
	      break;
	    default:
	      abort();
	    }
	  }
	} else {
	  // action
	  if (type==EXIT)
	    break; // stop all
	  switch (type) {
	  case SOLVECONTINUOUS:
	    if (goodModel) {
	      model->initialSolve();
	      time2 = cpuTime();
	      totalTime += time2-time1;
	      std::cout<<"Result "<<model->solver()->getObjValue()<<
		" iterations "<<model->solver()->getIterationCount()<<
		" took "<<time2-time1<<" seconds - total "<<totalTime<<std::endl;
	      time1=time2;
	    } else {
	      std::cout<<"** Current model not valid"<<std::endl;
	    }
	    break;
	  case BAB:
	    if (goodModel) {
	      // Definition of node choice
	      SbbCompareUser compare;
	      model->setNodeComparison(compare);
	      OsiSolverInterface * solver = model->solver();
	      if (!doScaling)
		solver->setHintParam(OsiDoScale,false,OsiHintTry);
	      OsiClpSolverInterface * si =
		dynamic_cast<OsiClpSolverInterface *>(solver) ;
	      if (preSolve&&si != NULL) {
		// get clp itself
		ClpSimplex * modelC = si->getModelPtr();
		if (si->messageHandler()->logLevel())
		  si->messageHandler()->setLogLevel(1);
		assert (modelC->tightenPrimalBounds()==0);
		model->initialSolve();
		// bounds based on continuous
		if (tightenFactor)
		  assert (modelC->tightenPrimalBounds(tightenFactor)==0);
		if (gapRatio<1.0e100) {
		  double value = si->getObjValue();
		  double value2 = gapRatio*(1.0e-5+fabs(value));
		  model->setAllowableGap(value2);
		  std::cout<<"Continuous "<<value
			   <<", so allowable gap set to "<<value2<<std::endl;
		}
		if (djFix<1.0e20) {
		  // do some fixing
		  int numberColumns = modelC->numberColumns();
		  int i;
		  const char * type = modelC->integerInformation();
		  double * lower = modelC->columnLower();
		  double * upper = modelC->columnUpper();
		  double * solution = modelC->primalColumnSolution();
		  double * dj = modelC->dualColumnSolution();
		  for (i=0;i<numberColumns;i++) {
		    if (type[i]) {
		      double value = solution[i];
		      if (value<lower[i]+1.0e-5&&dj[i]>djFix) {
			solution[i]=lower[i];
			upper[i]=lower[i];
		      } else if (value>upper[i]-1.0e-5&&dj[i]<-djFix) {
			solution[i]=upper[i];
			lower[i]=upper[i];
		      }
		    }
		  }
		}
		{
		  // integer presolve
		  SbbModel * model2 = model->integerPresolve();
		  if (model2) {
		    // Do complete search
		    
		    SbbRounding heuristic1(*model2);
		    if (useRounding)
		      model2->addHeuristic(&heuristic1);
		    model2->branchAndBound();
		    // get back solution
		    model->originalModel(model2);
		  } else {
		    // infeasible
		    exit(1);
		  }
		}
	      } else {
		if (model->solver()->messageHandler()->logLevel())
		  model->solver()->messageHandler()->setLogLevel(1);
		model->initialSolve();
		if (gapRatio<1.0e100) {
		  double value = model->solver()->getObjValue();
		  double value2 = gapRatio*(1.0e-5+fabs(value));
		  model->setAllowableGap(value2);
		  std::cout<<"Continuous "<<value
			   <<", so allowable gap set to "<<value2<<std::endl;
		}
		SbbRounding heuristic1(*model);
		if (useRounding)
		  model->addHeuristic(&heuristic1);
		model->branchAndBound();
	      }
	      model->solver()->resolve();
	      std::cout<<"Optimal solution "<<model->solver()->getObjValue()<<std::endl;
	      time2 = cpuTime();
	      totalTime += time2-time1;
	      std::cout<<"Result "<<model->solver()->getObjValue()<<
		" took "<<time2-time1<<" seconds - total "<<totalTime<<std::endl;
	      time1=time2;
	    } else {
	      std::cout<<"** Current model not valid"<<std::endl;
	    }
	    break;
	  case IMPORT:
	    {
	      // get next field
	      field = getString(argc,argv);
	      std::string fileName;
	      bool canOpen=false;
	      if (field=="-") {
		// stdin
		canOpen=true;
		fileName = "-";
	      } else {
		if (field[0]=='/'||field[0]=='~')
		  fileName = field;
		else
		  fileName = directory+field;
		FILE *fp=fopen(fileName.c_str(),"r");
		if (fp) {
		  // can open - lets go for it
		  fclose(fp);
		  canOpen=true;
		} else {
		  std::cout<<"Unable to open file "<<fileName<<std::endl;
		}
	      }
	      if (canOpen) {
		int status =model->solver()->readMps(fileName.c_str(),"");
		if (!status||(status>0&&allowImportErrors)) {
		  // I don't think there is any need for this but ..
		  //OsiWarmStartBasis allSlack;
		  goodModel=true;
		  //model->setBasis(allSlack);
		  time2 = cpuTime();
		  totalTime += time2-time1;
		  time1=time2;
		} else {
		  // errors
		  std::cout<<"There were "<<status<<
		    " errors on input"<<std::endl;
		}
	      }
	    }
	    break;
	  case EXPORT:
	    {
	      // get next field
	      field = getString(argc,argv);
	      std::string fileName;
	      bool canOpen=false;
	      if (field[0]=='/'||field[0]=='~')
		fileName = field;
	      else
		fileName = directory+field;
	      FILE *fp=fopen(fileName.c_str(),"w");
	      if (fp) {
		// can open - lets go for it
		fclose(fp);
		canOpen=true;
	      } else {
		std::cout<<"Unable to open file "<<fileName<<std::endl;
	      }
	      if (canOpen) {
		model->solver()->writeMps(fileName.c_str(),"");
		time2 = cpuTime();
		totalTime += time2-time1;
		time1=time2;
	      }
	    }
	    break;
	  case MAXIMIZE:
	    model->solver()->setObjSense(-1);
	    break;
	  case MINIMIZE:
	    model->solver()->setObjSense(1);
	    break;
	  case DIRECTORY:
	    directory = getString(argc,argv);
	    break;
	  case STDIN:
	    read_mode=-1;
	    break;
	  case VERSION:
	    std::cout<<"Coin LP version "<<SBBVERSION
		     <<", build "<<__DATE__<<std::endl;
	    break;
	  case UNITTEST:
	    {
	      // okay so there is not a real unit test

	      int status =model->solver()->readMps("../Mps/Sample/p0033.mps",
						   "");
	      assert(!status);
	      model->branchAndBound();
	      model->solver()->resolve();
	      std::cout<<"Optimal solution "<<model->solver()->getObjValue()<<std::endl;
	      assert(fabs(model->solver()->getObjValue()-3089.0)<1.0e-5);
	      fprintf(stderr,"Test was okay\n");
	      status =model->solver()->readMps("../Mps/Sample/p0033.mps",
						   "");
	      assert(!status);
	      model->setCutoff(1.0e20);
	      model->setMaximumSolutions(1);
	      model->setSolutionCount(0);
	      model->branchAndBound();
	      model->solver()->resolve();
	      std::cout<<"partial solution "<<model->solver()->getObjValue()<<std::endl;
	      assert(fabs(model->solver()->getObjValue()-3457.0)<1.0e-5);
	    }
	    break;
	  case MIPLIB:
	    {
	      int mainTest (int argc, const char *argv[]);
	      // create fields for test
	      const char * fields[3];
	      int nFields=1;
	      fields[0]="fake main for miplib";
	      if (directory!="./") {
		fields[1]=("-miplibDir="+directory).c_str();
		nFields=2;
	      }
	      mainTest(nFields,fields);
	    }
	    break;
	  case SOLUTION:
	    if (goodModel) {
	      // get next field
	      field = getString(argc,argv);
	      std::string fileName;
	      FILE *fp=NULL;
	      if (field=="-"||field=="EOL") {
		// stdout
		fp=stdout;
	      } else {
		if (field[0]=='/'||field[0]=='~')
		  fileName = field;
		else
		  fileName = directory+field;
		fp=fopen(fileName.c_str(),"w");
	      }
	      if (fp) {
		// make fancy later on
		int iRow;
		int numberRows=model->solver()->getNumRows();
		const double * dualRowSolution = model->getRowPrice();
		const double * primalRowSolution =  model->getRowActivity();
		for (iRow=0;iRow<numberRows;iRow++) {
		  fprintf(fp,"%7d ",iRow);
		  fprintf(fp,"%15.8g        %15.8g\n",primalRowSolution[iRow],
			  dualRowSolution[iRow]);
		}
		int iColumn;
		int numberColumns=model->solver()->getNumCols();
		const double * dualColumnSolution = 
		  model->getReducedCost();
		const double * primalColumnSolution = 
		  model->getColSolution();
		for (iColumn=0;iColumn<numberColumns;iColumn++) {
		  fprintf(fp,"%7d ",iColumn);
		  fprintf(fp,"%15.8g        %15.8g\n",
			  primalColumnSolution[iColumn],
			  dualColumnSolution[iColumn]);
		}
		if (fp!=stdout)
		  fclose(fp);
	      } else {
		std::cout<<"Unable to open file "<<fileName<<std::endl;
	      }
	    } else {
	      std::cout<<"** Current model not valid"<<std::endl;
	      
	    }
	    break;
	  default:
	    abort();
	  }
	} 
      } else if (!numberMatches) {
	std::cout<<"No match for "<<field<<" - ? for list of commands"
		 <<std::endl;
      } else if (numberMatches==1) {
	if (!numberQuery) {
	  std::cout<<"Short match for "<<field<<" possible completion:"
		   <<std::endl;
	  for ( iParam=0; iParam<numberParameters; iParam++ ) {
	    int match = parameters[iParam].matches(field);
	    if (match) 
	      std::cout<<parameters[iParam].matchName()<<std::endl;
	  }
	} else if (numberQuery) {
	  std::cout<<"Short match for "<<field<<" completion:"
		   <<std::endl;
	  for ( iParam=0; iParam<numberParameters; iParam++ ) {
	    int match = parameters[iParam].matches(field);
	    if (match) {
	      std::cout<<parameters[iParam].matchName()<<" : ";
	      std::cout<<parameters[iParam].shortHelp()<<std::endl;
	    }
	  }
	}
      } else {
	if (!numberQuery) 
	  std::cout<<"Multiple matches for "<<field<<" - possible completions:"
		   <<std::endl;
	else
	  std::cout<<"Completions of "<<field<<":"<<std::endl;
	for ( iParam=0; iParam<numberParameters; iParam++ ) {
	  int match = parameters[iParam].matches(field);
	  if (match) {
	    std::cout<<parameters[iParam].matchName();
	    if (numberQuery>=2) 
	      std::cout<<" : "<<parameters[iParam].shortHelp();
	    std::cout<<std::endl;
	  }
	}
      }
    }
    delete model;
  }
  return 0;
}    
