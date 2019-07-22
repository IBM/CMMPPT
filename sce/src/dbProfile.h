#ifndef sce_dbProfile_h
#define sce_dbProfile_h

// RW2STL - inserted:
#include <string>
// End of RW2STL - inserted includes.


class std::string;

class SceDBProfile
{
public:

  SceDBProfile(const char* db);
  
  SceDBProfile(const char* db,
               const char* inst,
               const char* id,
               const char* password);
  
  SceDBProfile(const char* db,
               const char* id,
               const char* password);
  
  ~SceDBProfile();
  void setDbName(const char * theDbName);
  void setServer(const char * theServer);
  void setUserID(const char * theUserID);
  void setPassword(const char * thePassword);
  static std::string& getDbName();
  static std::string& getServer();
  static std::string& getUserID();
  static std::string& getPassword();
  static std::string& getDbInstance();
  int setup();

private:

  static std::string dbName_;
  static std::string server_;
  static std::string userID_;
  static std::string password_;
  static std::string dbInstance_;
};

#endif
