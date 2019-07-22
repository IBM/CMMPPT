
// RW2STL - inserted:
#include <string>
// End of RW2STL - inserted includes.
#include <sce/src/supplyDB.h>
#include <sce/src/dbLink.h>
#include <sce/src/dbProf.h>
#include <iostream.h>
#include <string.h>
#include <ctype.h>

int SceSupplyResponseDB::fDelete_ = 0;

SceSupplyResponseDB::SceSupplyResponseDB() : SceDBObj()
{
}
void SceSupplyResponseDB::insert(const SceDBLink& supply)
{
  int i = 0; // Pulled out of the for below by RW2STL
  for (i=1; i<supply.size() + 1; i++)
    {
      groupInsert(supply.nPeriods(),
                  (const char*)(SceStringArg&)supply[i][1],
                  (const char*)(SceStringArg&)supply[i][2],
                  (const char*)(SceStringArg&)supply[i][3],
                  (const char*)(SceStringArg&)supply[i][4],
                  (float*)(SceFloatVectorArg&)supply[i][5]);
    }
}

void SceSupplyResponseDB::groupInsert(
                                int            nPeriods,
                                const char *   part1, 
                                const char *   pdf1,
                                const char *   part2,
                                const char *   pdf2,
                                float *        supplyVol)
{
  SQLRETURN       rc;
  SQLUINTEGER     pirow = 0;
  int             period;
  float           supply;
  SQLCHAR         subgeo[] = " ";
  char *          thePart1 = (char*) part1;
  char *          thePdf1 = (char*) pdf1;
  char *          thePart2 = (char*) part2;
  char *          thePdf2 = (char*) pdf2;
  std::string       insert;
 
 {
   std::string inst = SceDBProfile::getDbInstance();
   std::string tbl = (inst.empty() ? "SUPPLYRESPONSE "
                    : (const char*)(inst+".SUPPLYRESPONSE "));
   insert = "INSERT INTO " + tbl;
   insert = insert + "VALUES (?, ?, ?, ?, ?, ?, ?) ";
 }

 // Prepare the statement
 
 rc = SQLPrepare(sqlhstmt_, (SQLCHAR*)insert.c_str(), SQL_NTS);
 CHECK_STMT(sqlhstmt_, rc);

 rc = SQLBindParameter(sqlhstmt_, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                       LEN, 0, thePart1, LEN, 0);
 CHECK_STMT(sqlhstmt_, rc);
 
 rc = SQLBindParameter(sqlhstmt_, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                     LEN, 0, thePdf1, LEN, 0);
 CHECK_STMT(sqlhstmt_, rc);
 
 rc = SQLBindParameter(sqlhstmt_, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                       LEN, 0, subgeo, LEN, 0);
 CHECK_STMT(sqlhstmt_, rc);

  rc = SQLBindParameter(sqlhstmt_, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                       LEN, 0, thePart2, LEN, 0);
 CHECK_STMT(sqlhstmt_, rc);
    
 rc = SQLBindParameter(sqlhstmt_, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR,
                       LEN, 0, thePdf2, LEN, 0);
 CHECK_STMT(sqlhstmt_, rc);

 rc = SQLBindParameter(sqlhstmt_, 6, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
                       0, 0, &period, 0, 0);
 CHECK_STMT(sqlhstmt_, rc);
 
 rc = SQLBindParameter(sqlhstmt_, 7, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_DOUBLE,
                       10, 2, &supply, 0, 0);
 CHECK_STMT(sqlhstmt_, rc);


 // prepare a record and execute insert 

 int i = 0; // Pulled out of the for below by RW2STL
 for (i=0; i< nPeriods; i++)
   {
     supply = supplyVol[i];
     period = i;
     rc = SQLExecute(sqlhstmt_);
     CHECK_STMT(sqlhstmt_, rc);
   }
}

void SceSupplyResponseDB::deleteTbl()
{
  int rc;
  std::string       deleteTbl;
 {
   std::string inst = SceDBProfile::getDbInstance();
   std::string tbl = (inst.empty() ? "SUPPLYRESPONSE "
                    : (const char*)(inst+".SUPPLYRESPONSE "));
   deleteTbl = "DELETE FROM " + tbl;
 }

  if ((rc=SQLExecDirect(sqlhstmt_, (SQLCHAR *) deleteTbl.c_str(), SQL_NTS))
     != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO)
       {
         //CHECK_STMT(sqlhstmt_, rc);
       }
  else
    /* EXEC SQL COMMIT WORK;                                     */
    SQLTransact(sqlhenv_, sqlhdbc_, SQL_COMMIT);  /* commit delete table */
  fDelete_ = 1;
}


