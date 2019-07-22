#ifndef sce_dbobj_h
#define sce_dbobj_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sce/src/arg.h>
#include <sce/src/dbLink.h>
#include "sqlcli1.h"

#ifndef max
#define  max(a,b) (a > b ? a : b)
#endif

#define DEL "\",\n"
#define WHITE " \t"
#define LEN 128
#define NUM 256
#define PNUM 12

class SceDBLink;

class SceDBObj
{
public:
  virtual ~SceDBObj();
  virtual void update(const char* field, const SceDBLink& dataSet){};
  //insert new records from set
  virtual void insert(const SceDBLink& dataSet){}
  //insert a new field in a existing record
  virtual void insert(const char* field, const SceDBLink& dataSet){}
  virtual void export(const char* field, const SceDBLink& dataSet){}
  virtual void deleteTbl(){}
  static int setup(const char* db, const char* id, const char* password);
  static void shutdown();

protected:

  SceDBObj();
  
  //  virtual SceAttrValue& getAttribute(SceAttrId)=0
  //virtual void setAttribute(SceAttrId, SceAttrValue&)=0
 
  // SQL error handling
  
  static SQLRETURN terminate(SQLHENV henv, SQLRETURN rc);

  static SQLRETURN print_error(SQLHENV henv,
                        SQLHDBC hdbc,
                        SQLHSTMT hstmt,
                        SQLRETURN frc,
                        int line,
                        char *  file);

  static SQLRETURN check_error(SQLHENV henv,
                        SQLHDBC hdbc,
                        SQLHSTMT hstmt,
                        SQLRETURN frc,
                        int line,
                        char *  file);

  static void CHECK_ENV (SQLHENV henv, SQLRETURN rc);
  static void CHECK_DBC( SQLHDBC hdbc, SQLRETURN rc);
  static void CHECK_STMT( SQLHSTMT hstmt, SQLRETURN rc);
 
  static SQLHENV sqlhenv_;
  static SQLHDBC sqlhdbc_;
  SQLHSTMT sqlhstmt_;
};



#endif
