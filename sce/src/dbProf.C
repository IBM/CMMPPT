
// RW2STL - inserted:
#include <string>
// End of RW2STL - inserted includes.
#include <sce/src/dbProf.h>
#include <sce/src/dbObj.h>


class std::string;

std::string SceDBProfile::dbName_;
std::string SceDBProfile::server_;
std::string SceDBProfile::userID_;
std::string SceDBProfile::password_;
std::string SceDBProfile::dbInstance_;

SceDBProfile::SceDBProfile(const char * db)
{
  dbName_ = db;
}

SceDBProfile::SceDBProfile(const char * db,
                           const char* inst,
                           const char * userID,
                           const char * password)
{
  dbName_ = db;
  dbInstance_ = inst;
  userID_ = userID;
  password_ = password;
}

SceDBProfile::SceDBProfile(const char * db,
                           const char * userID,
                           const char * password)
{
  dbName_ = db;
  userID_ = userID;
  password_ = password;
}

SceDBProfile::~SceDBProfile()
{
  SceDBObj::shutdown();
}
void SceDBProfile::setDbName(const char * theDbName)
{ dbName_ = theDbName ;}

void SceDBProfile::setServer(const char * theServer)
{ server_ = theServer;}

void SceDBProfile::setUserID(const char * theUserID)
{ userID_ = theUserID;}

void SceDBProfile::setPassword(const char * thePassword)
{ password_ = thePassword;}

std::string& SceDBProfile::getDbName()
{ return dbName_;}

std::string& SceDBProfile::getServer()
{ return server_;}

std::string& SceDBProfile::getUserID()
{ return userID_;}

std::string& SceDBProfile::getPassword()
{return password_;}

std::string& SceDBProfile::getDbInstance()
{return dbInstance_;}

int SceDBProfile::setup()
{
  return SceDBObj::setup(dbName_, userID_, password_);
}
