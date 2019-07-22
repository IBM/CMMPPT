
// RW2STL - inserted:
#include <string>
// End of RW2STL - inserted includes.
#include <sce/src/capacityAttrDB.h>
#include <sce/src/dbProfile.h>

class SceDBProfile;

SceCapacityAttrDB::SceCapacityAttrDB() : SceDBObj()
{
}

void SceCapacityAttrDB::update(const char* field, const SceDBLink& capAttr)
{
  int i = 0; // Pulled out of the for below by RW2STL
  for (i=1; i<capAttr.size()+1; i++)
    {
      groupUpdate(field,capAttr.nPeriods(),
                  (const char*) (SceStringArg&)capAttr[i][1],
                  (const char*) (SceStringArg&)capAttr[i][2],
                  (float*) (SceFloatVectorArg&)capAttr[i][3]);
    }
}

void SceCapacityAttrDB::groupUpdate(                 
                   const char *    field,
                   int             nPeriods,
                   const char*     part,
                   const char *    geo,
                   float *   timeVec)
{

  SQLRETURN       rc;
  SQLUINTEGER     pirow = 0;
  float           value;
  int             period;
  char *          thePart = (char*) part;
  char *          theGeo = (char*) geo;
  std::string       update;
 
 {
   std::string inst = SceDBProfile::getDbInstance();
   std::string tbl = (inst.empty() ? "CAPACITYATTR " : (const char*)(inst+".CAPACITYATTR "));
   update = "UPDATE " + tbl;
   update = update + "SET ";
   update = update + field;
   update = update + " = ? where capacityname = ? and pdf = ? and periodnum = ?";
 
 }

 // Prepare the statement
 
 rc = SQLPrepare(sqlhstmt_, (SQLCHAR*) update.c_str(), SQL_NTS);
 CHECK_STMT(sqlhstmt_, rc);

 rc = SQLBindParameter(sqlhstmt_, 1, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_DOUBLE,
                       10, 2, &value, 0, 0);
 CHECK_STMT(sqlhstmt_, rc);
 
 rc = SQLBindParameter(sqlhstmt_, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                       LEN, 0, thePart, LEN, 0);
 CHECK_STMT(sqlhstmt_, rc);
 
 rc = SQLBindParameter(sqlhstmt_, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR,
                       LEN, 0, theGeo, LEN, 0);
 CHECK_STMT(sqlhstmt_, rc);

 rc = SQLBindParameter(sqlhstmt_, 4, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
                       0, 0, &period, 0, 0);

 CHECK_STMT(sqlhstmt_, rc);
 

 // prepare a record and execute update 

 int i = 0; // Pulled out of the for below by RW2STL
 for (i=0; i< nPeriods; i++)
   {
     value = timeVec[i];
     period = i;
     rc = SQLExecute(sqlhstmt_);
     CHECK_STMT(sqlhstmt_, rc);
   }

 //return pirow;
}

 
