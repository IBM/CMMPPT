//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef WitEntitH
#define WitEntitH

//------------------------------------------------------------------------------
//
//     Source file: Entity.h
//
//     Definition of classes Entity and Attribute
//
//------------------------------------------------------------------------------

#include <string.h>
#include <string>

#include <Problem.h>

//------------------------------------------------------------------------------
//
//     Definition of class WitEntity -- this class represents a Wit Entity.
//     A Wit Entity may be a part, an operation, a bomEntry, a subEntry, a
//     bopEntry, or a demand.  For the purposes of the parser, there may
//     be two additional special entities: a problem and a partWithOperation.
//     A "problem" entity is used to specify global attributes only.
//
//------------------------------------------------------------------------------

#define MAX_ARGS   10

class WitEntity
{

public:
  typedef enum eType {undefined, problem, part, operation, bomEntry, subEntry,
    bopEntry, demand, partWithOperation} eType;

  typedef struct argVecDef {
    WitDRParmType::Tag   argType; // type of argument, 
                                  //   as defined in Parlex.h
    union {
      int               ival;     // type is Integer
      double            dval;     // type is Double
      const char *      sval;     // type is String
      bool              bval;     // type is Boolean
      WitDRPartCat::Tag cval;     // type is Category
      WitBoundSetSpec * bsval;    // type is a bound set
      WitVectorSpec *   vval;     // type is a vector

    } argValue;
  } argVecDef; 

  // Constructors and destructors

  WitEntity (
        WitProblem *,
        const char * id = "unnamed",
        eType type      = undefined);

  virtual ~WitEntity();

  // Operations
  virtual WitSymTabEntry *lookupRule(WitSymTable *, std::string);
  virtual WitParseRC     checkArgs(WitSymTabEntry * entryPtr);

  // Accessors
  virtual int      argc() const;
  eType            entityType() const;
  const char *     entityID() const;
  WitSymTabEntry * attrPtr() const;
  bool             invalid() const;

  accessFunc (WitProblem *, myProblem)

  WitMsgFacility & myMsgFac () const;

  accessWrapper (WitCompMgr *, myCompMgr, myProblem ())

  void           setEntityType(const eType);
  void           setEntityID(const char * const);
  void           setAttrPtr(WitSymTabEntry * const);
  void           setInvalid(bool);
  virtual void   setArgc(const int);

  // get an argument value
  virtual int          getIntArg(int i);     // where i= argument number
  virtual double       getDoubleArg(int i);   //   0, 1, ..., MAX_ARGS-1
  virtual const char * getStringArg(int i);
  virtual bool         getBoolArg(int i);
  WitDRPartCat::Tag    getCatArg(int i);
  WitVectorSpec *      getVectorArg(int i);
  WitBoundSetSpec *    getBoundSetArg(int i);

  // store the next argument value
  virtual argVecDef*  setIntArg(int);      
  virtual argVecDef*  setDoubleArg(double);
  virtual argVecDef*  setBoolArg(bool);
  virtual argVecDef*  setStringArg(const char *);
  argVecDef*          setCatArg(WitDRPartCat::Tag); 
  argVecDef*          setVectorArg(WitVectorSpec *);
  argVecDef*          setBoundSetArg(WitBoundSetSpec *);

  // The following functions are used to obtain various values
  // needed as arguments by the "set attribute" functions	
  const char * getPartName();
  const char * getDemandName();      
  const char * getOperationName();
  int          getBomEntryIndex();
  int          getSubEntryIndex();
  int          getBopEntryIndex();

protected:

  int            argc_;             // number of arguments found
  argVecDef      argVector_[MAX_ARGS];  // arguments for entity

private:

  eType            entityType_;    // code for type of entity
  char *           entityID_;      // name of entity
  WitSymTabEntry * attrPtr_;       // pointer to entry in symbol table
                                   // for "set" or add" rule
  bool             invalid_;       // true: means entity is invalid
                                   // (set if entity error occurs)

  WitProblem * const myProblem_;

  void           prtInvalidMsg(int arg, const char *s);
};

//------------------------------------------------------------------------------
//
//     Definition of class WitAttribute -- this class represents a Wit
//     attribute.  Wit attributes are associated with entities and describe
//     characteristics of the entity.  There may also be "global attributes"
//     associated with the pseudo-entity "problem."  A "set" or "add"
//     command consists of two parts: an entity part and an attribute part.
//     The attribute part contains zero or more attribute specifications
//     for the associated entity.
//
//------------------------------------------------------------------------------

class WitAttribute: public WitEntity {

public:

  typedef enum aType {undefined, simple, boundSet} aType;

  // Constructors and destructors
  WitAttribute();

  WitAttribute (
     WitProblem * theProblem,
     const char * id   = "unnamed",
     aType        type = simple);

  ~WitAttribute();

  // Operations

  // Accessors
  aType            attrType() const;
  const char *     attrID() const;
  WitSymTabEntry * attrPtr() const;
  bool             invalid() const;

  void           setAttrType(const aType);
  void           setAttrID(const char * const);
  void           setAttrPtr(WitSymTabEntry * const);
  void           setInvalid(bool);

private:

  aType            attrType_;        // code for type of attribute
  char *           attrID_;          // name of attribute
  WitSymTabEntry * attrPtr_;         // pointer to entry in symbol table
  bool             invalid_;         // true: means entity is invalid

};

#endif     // WitEntityH
