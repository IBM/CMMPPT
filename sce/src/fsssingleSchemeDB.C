
// RW2STL - inserted:
#include <string>
// End of RW2STL - inserted includes.
#include <sce/src/fsssingleSchemeDB.h>
#include <sce/src/dbLink.h>
#include <sce/src/dbProfile.h>
#include <iostream.h>
#include <string.h>
#include <ctype.h>

int SceFSSSingleSchemeDB::fDelete_ = 0;

SceFSSSingleSchemeDB::SceFSSSingleSchemeDB() : SceDBObj()
{
}
void SceFSSSingleSchemeDB::insert(const SceDBLink& fsss)
{
  // blank the table
  //delete();
  int i = 0; // Pulled out of the for below by RW2STL
  for (i=1; i<fsss.size() + 1; i++)
    {
      groupInsert(fsss.nPeriods(),
                  (const char*)(SceStringArg&)fsss[i][1],
                  (const char*)(SceStringArg&)fsss[i][2],
                  (const char*)(SceStringArg&)fsss[i][3],
                  (const char*)(SceStringArg&)fsss[i][4],
                  (float*)(SceFloatVectorArg&)fsss[i][5],
                  (float*)(SceFloatVectorArg&)fsss[i][6],
                  (float*)(SceFloatVectorArg&)fsss[i][7]);
    }
}

void SceFSSSingleSchemeDB::groupInsert(
                                       int            nPeriods,
                                       const char *   part1, 
                                       const char *   geo,
                                       const char *   part2,
                                       const char *   pdf,
                                       float *        FSSReq,
                                       float *        FSSCov,
                                       float *        FSSDelta)
{
  SQLRETURN       rc;
  SQLUINTEGER     pirow = 0;
  int             period;
  SQLCHAR         subgeo[] = " ";
  float           pFSSReq, pFSSCov, pFSSDelta;
  char *          thePart1 = (char*) part1;
  char *          theGeo = (char*) geo;
  char *          thePart2 = (char*) part2;
  char *          thePdf = (char*) pdf;
  std::string       insert;
 
 {
   std::string inst = SceDBProfile::getDbInstance();
   std::string tbl = (inst.empty() ? "FSSSINGLESCHEME "
                    : (const char*)(inst+".FSSSINGLESCHEME "));
   insert = "INSERT INTO " + tbl;
   insert = insert + "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?) ";
 }

 // Prepare the statement
 
 rc = SQLPrepare(sqlhstmt_, (SQLCHAR*)insert.c_str(), SQL_NTS);
 CHECK_STMT(sqlhstmt_, rc);

 rc = SQLBindParameter(sqlhstmt_, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                       LEN, 0, thePart1, LEN, 0);
 CHECK_STMT(sqlhstmt_, rc);
 
 rc = SQLBindParameter(sqlhstmt_, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                     LEN, 0, theGeo, LEN, 0);
 CHECK_STMT(sqlhstmt_, rc);
 
 rc = SQLBindParameter(sqlhstmt_, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                     LEN, 0, subgeo, LEN, 0);
 CHECK_STMT(sqlhstmt_, rc);
    
 rc = SQLBindParameter(sqlhstmt_, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                       LEN, 0, thePart2, LEN, 0);
 CHECK_STMT(sqlhstmt_, rc);
    
 rc = SQLBindParameter(sqlhstmt_, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR,
                       LEN, 0, thePdf, LEN, 0);
 CHECK_STMT(sqlhstmt_, rc);

 rc = SQLBindParameter(sqlhstmt_, 6, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
                       0, 0, &period, 0, 0);
 CHECK_STMT(sqlhstmt_, rc);
 
 rc = SQLBindParameter(sqlhstmt_, 7, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_DOUBLE,
                       10, 2, &pFSSReq, 0, 0);
 CHECK_STMT(sqlhstmt_, rc);
 
 rc = SQLBindParameter(sqlhstmt_, 8, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_DOUBLE,
                       10, 2, &pFSSCov, 0, 0);
 CHECK_STMT(sqlhstmt_, rc);
 
 rc = SQLBindParameter(sqlhstmt_, 9, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_DOUBLE,
                       10, 2, &pFSSDelta, 0, 0);
 CHECK_STMT(sqlhstmt_, rc);
 

 // prepare a record and execute insert 

 int i = 0; // Pulled out of the for below by RW2STL
 for (i=0; i< nPeriods; i++)
   {
     pFSSReq = FSSReq[i];
     pFSSCov = FSSCov[i];
     pFSSDelta = FSSDelta[i];
     period = i;
     rc = SQLExecute(sqlhstmt_);
     CHECK_STMT(sqlhstmt_, rc);
   }

 //return pirow;
}

void SceFSSSingleSchemeDB::deleteTbl()
{
  int rc;
  std::string       deleteTbl;
 {
   std::string inst = SceDBProfile::getDbInstance();
   std::string tbl = (inst.empty() ? "FSSSINGLESCHEME "
                    : (const char*)(inst+".FSSSINGLESCHEME "));
   deleteTbl = "DELETE FROM " + tbl;
 }

  if ((rc=SQLExecDirect(sqlhstmt_, (SQLCHAR *) deleteTbl.c_str(), SQL_NTS))
     != SQL_SUCCESS && rc != SQL_SUCCESS_WITH_INFO)
       {
          // print_err(sqlhdbc_, SQL_NULL_HSTMT);
         //CHECK_STMT(sqlhstmt_, rc);
       }
  else
    /* EXEC SQL COMMIT WORK;                                     */
    SQLTransact(sqlhenv_, sqlhdbc_, SQL_COMMIT);  /* commit delete table */
  fDelete_ = 1;
}


