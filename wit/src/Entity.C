//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
//
//     Source file: WitEntit.C
//     Implementation of the following classes:
//
//        Entity
//           Attribute
//
//------------------------------------------------------------------------------

#include <Parlex.h>
#include <SymTable.h>
#include <VecBS.h>
#include <Entity.h>
#include <Parser.h>
#include <CompMgr.h>
#include <Operation.h>
#include <BomEntry.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
//
//     Implementation of class Entity
//
//------------------------------------------------------------------------------

extern WitParser::PS  commandState;   // defined in witParser.y

// Constructor and destructor

WitEntity::WitEntity (
         WitProblem * theProblem,
         const char * id,
         eType        type):

      argc_       (0),
      entityType_ (type),
      attrPtr_    (NULL),
      invalid_    (false),
      myProblem_  (theProblem)
   {
   entityID_ = new char[strlen (id) + 1];

   strcpy (entityID_, id);
   }

WitEntity::~WitEntity()
{ 
  WitDRParmType::Tag type;

  // delete all the strings that have been allocated
  // also, delete other composite objects
  for (int i = 0; i < argc_; i++) {
    type = argVector_[i].argType;

    switch (type) {
      case WitDRParmType::string:
        delete[] argVector_[i].argValue.sval;
        break;

      case WitDRParmType::boolVector:
      case WitDRParmType::intVector:
      case WitDRParmType::doubleVector:
         {
         delete argVector_[i].argValue.vval;

         break;
         }
  
      case WitDRParmType::boundSet:
        delete argVector_[i].argValue.bsval;
        break;

      default:                         
        break;
    };
  };

  delete[] entityID_;
}

// Operations

WitSymTabEntry *
WitEntity::lookupRule(WitSymTable *theSymTable, std::string theRule)
{
  WitSymTabEntry  *entryPtr = 0;

  // printf("Current rule is: %s\n", theRule);

  /* look up rule in hash dictionary */
  WitSymTable::WitSymTableDict::const_iterator it = theSymTable->theDictionary().find(theRule);
  if( it != theSymTable->theDictionary().end() )
    entryPtr = it->second;

  return entryPtr;
}

// Accessors

int WitEntity::argc() const
{ return argc_; }

WitEntity::eType WitEntity::entityType() const
{ return entityType_; }

const char * WitEntity::entityID() const
{ return entityID_; }

WitSymTabEntry *WitEntity::attrPtr() const
{ return attrPtr_; }

bool WitEntity::invalid() const
{ return invalid_; }

WitMsgFacility & WitEntity::myMsgFac () const
   {
   return myProblem ()->myMsgFac ();
   }

void WitEntity::setArgc(const int n)
{ argc_ = n; }

void WitEntity::setEntityType(const eType type)
{ entityType_ = type; }

void WitEntity::setEntityID(const char * const s)
{
  if (entityID_ != NULL)
    delete[] entityID_;
  entityID_ = new char[strlen(s)+1];  
  strcpy(entityID_, s);
}

void WitEntity::setAttrPtr(WitSymTabEntry * const pentry)
{ attrPtr_ = pentry; }
 
void WitEntity::setInvalid(bool flag)
{ invalid_ = flag; }

// get an argument value
int WitEntity::getIntArg(int index)
{
  return (argVector_[index].argValue.ival);
}

double WitEntity::getDoubleArg(int index)
{
  return argVector_[index].argValue.dval;
}

const char * WitEntity::getStringArg(int index)
{
  return (argVector_[index].argValue.sval);
}

bool WitEntity::getBoolArg(int index)
{
  return (argVector_[index].argValue.bval);
}

WitDRPartCat::Tag WitEntity::getCatArg(int index)
{
  return (argVector_[index].argValue.cval);
}

WitVectorSpec*  WitEntity::getVectorArg(int index)
{
  return (argVector_[index].argValue.vval);
}

WitBoundSetSpec*  WitEntity::getBoundSetArg(int index)
{
  return (argVector_[index].argValue.bsval);
}


// store the next argument value and set its type
WitEntity::argVecDef* WitEntity::setIntArg(int value)
{
  // cout << "Argument: " << value << endl;
  if (argc_ >= MAX_ARGS) return NULL; 
  argVector_[argc_].argValue.ival = value;
  argVector_[argc_++].argType = WitDRParmType::intScalar;
  return &argVector_[argc_-1];
} 

WitEntity::argVecDef* WitEntity::setDoubleArg(double value)
{
  // cout << "Argument: " << value << endl;
  if (argc_ >= MAX_ARGS) return NULL;
  argVector_[argc_].argValue.dval = value;
  argVector_[argc_++].argType = WitDRParmType::doubleScalar;
  return &argVector_[argc_-1];
}

WitEntity::argVecDef*  WitEntity::setBoolArg(bool value)
{

  // cout << "Argument: " << value << endl;
  if (argc_ >= MAX_ARGS) return NULL;
  argVector_[argc_].argValue.bval = value;
  argVector_[argc_++].argType = WitDRParmType::boolScalar;
  return &argVector_[argc_-1];
}

WitEntity::argVecDef* WitEntity::setCatArg(WitDRPartCat::Tag value)
{

  // cout << "Argument: " << value << endl;
  if (argc_ >= MAX_ARGS) return NULL;
  argVector_[argc_].argValue.cval = value;
  argVector_[argc_++].argType = WitDRParmType::partCat;
  return &argVector_[argc_-1];
}

WitEntity::argVecDef* WitEntity::setStringArg(const char * value)
{
  char * s;

  // cout << "Argument: " << value << endl;
  if (argc_ >= MAX_ARGS) return NULL;
  s = new char[strlen(value)];
  strcpy(s, value+1);                   // get rid of leading quotes
  if (s[strlen(s)-1] == '\"')
    s[strlen(s)-1] = '\0';              // get rid of trailing quotes      
  argVector_[argc_].argValue.sval = s;
  argVector_[argc_++].argType = WitDRParmType::string;
  return &argVector_[argc_-1];
}

WitEntity::argVecDef*  WitEntity::setVectorArg(WitVectorSpec* vptr)
{
    // cout << "Argument: " << vptr->formatType() << endl;
    if (argc_ >= MAX_ARGS) return NULL;
    argVector_[argc_].argValue.vval = vptr;
    argVector_[argc_++].argType = vptr->dataType();
    return &argVector_[argc_-1];
}

WitEntity::argVecDef* WitEntity::setBoundSetArg(WitBoundSetSpec* bsptr)
{
  // cout << "Argument is a bound set " << endl;
  if (argc_ >= MAX_ARGS) return NULL;
  argVector_[argc_].argValue.bsval = bsptr;
  argVector_[argc_++].argType = WitDRParmType::boundSet;
  return &argVector_[argc_-1];
}


// The following functions are used to obtain various values
// needed as arguments by the "set attribute" functions

const char * WitEntity::getPartName()
{
  switch (entityType_) {
    case part:
      return this->getStringArg(0);
    case bomEntry:
    case bopEntry:
      if (commandState == WitParser::PS_add)
        return this->getStringArg(1);
      else
        return NULL;
    case subEntry:
      if (commandState == WitParser::PS_add)
        return this->getStringArg(2);
      else
        return NULL;
    case demand:
      return this->getStringArg(0);
    case partWithOperation:
      if (commandState == WitParser::PS_add)
        return this->getStringArg(0);
      else
        return NULL;
    default:
      return NULL;
  }
}

const char * WitEntity::getDemandName()
{
  switch (entityType_) {
    case demand:
      return this->getStringArg(1);
    default:
      return NULL;
  }
}

const char * WitEntity::getOperationName()
{
  switch (entityType_) {
    case operation:
    case bomEntry:
    case subEntry:
    case bopEntry:
      return this->getStringArg(0);
    case partWithOperation:
      if (commandState == WitParser::PS_add)
        return this->getStringArg(0);
      else
        return NULL;
    default:
      return NULL;              
  }
}

int WitEntity::getBomEntryIndex()
{
  switch (entityType_) {
    case bomEntry:
      if (commandState == WitParser::PS_add)
        {
        WitOperation * theOpn;

        theOpn = myCompMgr ()->findOperation (getOperationName ());

        witAssert (theOpn != NULL);

        return theOpn->bom ().nElements () - 1;
        }
      else
        return this->getIntArg(1);
    case subEntry:
        return this->getIntArg(1);
    default:
      return 0;              
  }
}

int WitEntity::getSubEntryIndex()
{
  switch (entityType_) {
    case subEntry:
      if (commandState == WitParser::PS_add)
        {
        WitOperation * theOpn;
        WitBomEntry *  theBomEnt;

        theOpn = myCompMgr ()->findOperation (getOperationName ());

        witAssert (theOpn != NULL);

        theBomEnt = myCompMgr ()->findBomEntry (theOpn, getBomEntryIndex ());

        witAssert (theBomEnt != NULL);

        return theBomEnt->mySubEntries ().nElements () - 1;
        }
      else
        return this->getIntArg(2);
    default:
      return 0;
  }
}

int WitEntity::getBopEntryIndex()
{
  switch (entityType_) {
    case bopEntry:
      if (commandState == WitParser::PS_add)
        {
        WitOperation * theOpn;

        theOpn = myCompMgr ()->findOperation (getOperationName ());

        witAssert (theOpn != NULL);

        return theOpn->bop ().nElements () - 1;
        }
      else
        return this->getIntArg(1);
    default:
      return 0;
  }
}

// check entity arguments for validity
WitParseRC WitEntity::checkArgs(WitSymTabEntry * entryPtr)
{
  WitDRParmType::Tag aType;
  WitParseRC retcode = 0;
  int        n, np;

  // compare each argument type against types in symbol table entry
  np = entryPtr->nparms();                // number of params in table
  n = (argc_ < np) ? argc_ : np;          // number of args to be checked
  for (int i = 0; i < n; i++) {
    aType = argVector_[i].argType;        // the actual type 
    
    switch (entryPtr->parmTypes()[i]) {   // what it should be
      case WitDRParmType::intScalar:
        if (aType == WitDRParmType::intScalar) break;      // it matches
        prtInvalidMsg(i+1, "must be an integer");
        retcode = BAD_ARG;
        break;
      case WitDRParmType::doubleScalar:                      
        if (aType == WitDRParmType::doubleScalar) break;        // it matches
        if (aType == WitDRParmType::intScalar) {
          argVector_[i].argValue.dval = 
             static_cast <double> (argVector_[i].argValue.ival);
          argVector_[i].argType = WitDRParmType::doubleScalar;
          break;
        };
        prtInvalidMsg(i+1, "must be a double");
        retcode =  BAD_ARG;
        break;
      case WitDRParmType::string:
        if (aType == WitDRParmType::string) break;       // it matches
        prtInvalidMsg(i+1, "must be a string");
        retcode = BAD_ARG;
        break;
      case WitDRParmType::boolScalar:
        if (aType == WitDRParmType::boolScalar) break;      // it matches
        prtInvalidMsg(i+1, "must be a boolean");
        retcode = BAD_ARG;
        break;
      case WitDRParmType::partCat:
        if (aType == WitDRParmType::partCat) break;     // it matches
        prtInvalidMsg(i+1, "must be a \"category\"");
        retcode = BAD_ARG;
        break;

      case WitDRParmType::boolVector:
         {
         if (aType != WitDRParmType::boolVector)
            {
            prtInvalidMsg (i + 1, "must be a \"boolean vector\"");

            retcode = BAD_ARG;
            };

         break;
         }

      case WitDRParmType::intVector:
         {
         if (aType != WitDRParmType::intVector)
            {
            prtInvalidMsg (i + 1, "must be an \"integer vector\"");

            retcode = BAD_ARG;
            };

         break;  
         }

      case WitDRParmType::doubleVector:
        if (aType != WitDRParmType::doubleVector) {      // if it doesn't match	
          prtInvalidMsg(i+1, "must be a \"double vector\"");
          retcode = BAD_ARG;
        };
        break;

      case WitDRParmType::boundSet:
        if (aType == WitDRParmType::boundSet) break;       // it matches
        prtInvalidMsg(i+1, "must be a \"bound set\"");
        retcode = BAD_ARG;
        break;
      default:
        if (aType != entryPtr->parmTypes()[i]) {
          prtInvalidMsg(i+1, "is not of the proper type");
          retcode = BAD_ARG;
          break;
        };
    }   
  }

  // number of arguments must equal number of parameters
  if (argc_ != np)
    retcode = BAD_NUM_ARGS;

  return retcode;
}

// Print error message -- invalid entity argument or attribute value
void  WitEntity::prtInvalidMsg(int arg, const char * s)
{
  // Check whether we are parsing an entity or an attribute
  if (WitParser::instance ()->currentAttr() == NULL) {
    
    if (commandState == WitParser::PS_add)
      myMsgFac () ("invalidArgTypeMsg", arg, "add",
        WitParser::instance ()->currentEntity()->entityID(), s);
    else if (commandState == WitParser::PS_set)
      myMsgFac () ("invalidArgTypeMsg", arg, "set",
        WitParser::instance ()->currentEntity()->entityID(), s);
  }
  else
      myMsgFac () ("invalidValueTypeMsg",
        WitParser::instance ()->currentAttr()->attrID(), s);
  return;

}
//------------------------------------------------------------------------------
//
//     Implementation of class WitAttribute
//
//------------------------------------------------------------------------------

// Constructor and destructor

WitAttribute::WitAttribute (
         WitProblem * theProblem,
         const char * id,
         aType        type):

      WitEntity (theProblem, "unnamed"),

      attrType_ (type),
      attrPtr_  (NULL),
      invalid_  (false)
   {
   attrID_ = new char[strlen (id) + 1];

   strcpy (attrID_, id);
   }

WitAttribute::~WitAttribute()
{
  delete[] attrID_;
}


// Accessors

WitAttribute::aType WitAttribute::attrType() const
{ return attrType_; }

const char * WitAttribute::attrID() const
{ return attrID_; }

WitSymTabEntry *WitAttribute::attrPtr() const
{ return attrPtr_; }

bool WitAttribute::invalid() const
{ return invalid_; }

void WitAttribute::setAttrType(const aType type)
{ attrType_ = type; }

void WitAttribute::setAttrID(const char * const s)
{
  if (attrID_ != NULL)
    delete[] attrID_;
  attrID_ = new char[strlen(s)+1];
  strcpy(attrID_, s);
}

void WitAttribute::setAttrPtr(WitSymTabEntry * const pentry)
{ attrPtr_ = pentry; }

void WitAttribute::setInvalid(bool flag)
{ invalid_ = flag; }
