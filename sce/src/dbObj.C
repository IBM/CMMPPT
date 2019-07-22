#include <sce/src/dbObj.h>

SQLHENV SceDBObj::sqlhenv_ = SQL_NULL_HENV;
SQLHDBC SceDBObj::sqlhdbc_ = SQL_NULL_HDBC;

SceDBObj::SceDBObj()
{
  SQLAllocStmt(sqlhdbc_, &sqlhstmt_); /* allocate a statement handle    */
}

SceDBObj::~SceDBObj()
{
  SQLTransact(sqlhenv_, sqlhdbc_, SQL_COMMIT);  /* commit the transaction       */
  SQLFreeStmt(sqlhstmt_, SQL_DROP);
}

int SceDBObj::setup(const char* db, const char* id, const char* password)
{
  
  SQLRETURN  rc;

    rc = SQLAllocEnv(&sqlhenv_);    /* allocate an environment handle    */
    if (rc != SQL_SUCCESS)
      {
        terminate(sqlhenv_, rc);
        return 0;
      }

    rc = SQLAllocConnect( sqlhenv_, &sqlhdbc_ );
    if( rc != SQL_SUCCESS )
      {
        terminate(sqlhenv_, rc);
        return 0;
      }
 
      rc = SQLConnect(
                      sqlhdbc_,
                      (SQLCHAR *)db,
                      SQL_NTS,
                      (SQLCHAR *)id,
                      SQL_NTS,
                      (SQLCHAR *)password,
                      SQL_NTS );
    
    if( rc != SQL_SUCCESS )
      {
        terminate(sqlhenv_, rc);
        return 0;
      }
    return 1;
}

void SceDBObj::shutdown()
{
  SQLDisconnect(sqlhdbc_);        /* disconnect from the database */

  SQLFreeConnect(sqlhdbc_);       /* free the connection handle   */
  SQLFreeEnv(sqlhenv_);           /* free the environment handle  */

}


SQLRETURN
SceDBObj::terminate(SQLHENV henv, SQLRETURN rc)
{
    SQLCHAR         buffer[255];
    SQLSMALLINT     outlen;

    printf(">Terminating ....\n");
    print_error(henv, SQL_NULL_HDBC, SQL_NULL_HENV, rc, __LINE__, __FILE__);

    /* Free environment handle */
    if (SQLFreeEnv(henv) != SQL_SUCCESS)
        print_error(henv, SQL_NULL_HDBC, SQL_NULL_HENV, rc, __LINE__, __FILE__);

    return (rc);
}


// print_error   - call SQLError(), display SQLSTATE and message
//               - called by check_error, see below


SQLRETURN
SceDBObj::print_error(SQLHENV henv,
                      SQLHDBC hdbc,
                      SQLHSTMT hstmt,
                      SQLRETURN frc,
                      /* Return code to be included with error msg  */
                      int line,
                      /* Used for output message, indcate where     */
                      char * file)  /* the error was reported from  */
{
    SQLCHAR         buffer[SQL_MAX_MESSAGE_LENGTH + 1];
    SQLCHAR         sqlstate[SQL_SQLSTATE_SIZE + 1];
    SQLINTEGER      sqlcode;
    SQLSMALLINT     length;


    //printf(">--- ERROR -- RC= %d Reported from %s, line %d ------------\n",
    //                                   frc, file, line);
    printf(">--- ERROR -- RC= %d Reported ------------\n",frc);
    while (SQLError(henv, hdbc, hstmt, sqlstate, &sqlcode, buffer,
                    SQL_MAX_MESSAGE_LENGTH + 1, &length) == SQL_SUCCESS) {
        printf("         SQLSTATE: %s\n", sqlstate);
        printf("Native Error Code: %ld\n", sqlcode);
        printf("%s \n", buffer);
    };
    printf(">--------------------------------------------------\n");
    return (SQL_ERROR);

}

//- check_error   - call print_error(), checks severity of return code

SQLRETURN
SceDBObj::check_error(SQLHENV henv,
                      SQLHDBC hdbc,
                      SQLHSTMT hstmt,
                      SQLRETURN frc,
                      int line,
                      char * file)
{
    SQLRETURN       rc;

    print_error(henv, hdbc, hstmt, frc, line, file);

    switch (frc) {
    case SQL_SUCCESS:
        break;
    case SQL_INVALID_HANDLE:
        printf("\n>------ ERROR Invalid Handle --------------------------\n");
    case SQL_ERROR:
        printf("\n>--- FATAL ERROR, Attempting to rollback transaction --\n");
        rc = SQLTransact(henv, hdbc, SQL_ROLLBACK);
        if (rc != SQL_SUCCESS)
            printf(">Rollback Failed, Exiting application\n");
        else
            printf(">Rollback Successful, Exiting application\n");
        exit(terminate(henv, frc));
        break;
    case SQL_SUCCESS_WITH_INFO:
        printf("\n> ----- Warning Message, application continuing --------\n");
        break;
    case SQL_NO_DATA_FOUND:
        printf("\n> ----- No Data Found, application continuing --------- \n");
        break;
    default:
        printf("\n> ----------- Invalid Return Code --------------------- \n");
        printf("> --------- Attempting to rollback transaction ---------- \n");
        SQLTransact(henv, hdbc, SQL_ROLLBACK);
        exit(terminate(henv, frc));
        break;
    }
    return (SQL_SUCCESS);

}                               

void SceDBObj::CHECK_ENV (SQLHENV henv, SQLRETURN rc)
{
  if (rc !=SQL_SUCCESS)
    check_error(henv, SQL_NULL_HDBC, SQL_NULL_HSTMT, rc, __LINE__, __FILE__);
}

void SceDBObj::CHECK_DBC( SQLHDBC hdbc, SQLRETURN rc)
{
  if (rc != SQL_SUCCESS) 
    check_error(SQL_NULL_HENV, hdbc, SQL_NULL_HSTMT, rc, __LINE__, __FILE__);
}
  
void SceDBObj::CHECK_STMT( SQLHSTMT hstmt, SQLRETURN rc)
{
  if (rc != SQL_SUCCESS)
    check_error(SQL_NULL_HENV, SQL_NULL_HDBC, hstmt, rc, __LINE__, __FILE__);
}
