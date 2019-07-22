#ifndef sceDateTime_h 
#define sceDateTime_h 

#include <string>
#include <iostream>
#include <time.h>

// As a general rule, the LgFrDatetime methods do not check for valid dates. 
// The class provides an IsValid() method to verify error conditions, and it
// should be called after every call that modifies the object

class LgFrDatetime
{
  time_t time_; // Always number of seconds since jan 1st 1970, same as time()

public:

  LgFrDatetime( time_t s = 0 ); // passing-in LgFrDatetime::currentTime() constructs it as now
  LgFrDatetime( const LgFrDatetime& t );
  LgFrDatetime( const std::string& date, const std::string& time = "" );

  // accessors - all const methods
  bool isValid() const { return time_ != 0; }

  // No assumption should be made about the units returned by getValue.
  // getValue() and setValue() are only provided so you can make date arithmetics
  // with no regards to seconds, months, years etc...
  time_t getValue() const {return time_;}

  // This formats with a default, but you can provide an alternate format.
  // see documentation on "strftime"
  std::string getDateAsString( const std::string& format = "%m/%d/%y" ) const;
  std::string getDateAsStraightString() const { return getDateAsString( "%Y%m%d" ); };
  std::string getTimeAsString( const std::string& format = "%H:%M:%S" ) const;

  void getDate( int& month, int& day, int& year ) const;
  void getTime( int& hour, int& min, int& sec ) const;
  void getDatetime( int& month, int& day, int& year, int& hour, int& min, int& sec ) const;

  int  getHour() const;      // 0-23
  int  getMin() const;       // 0-59
  int  getSec() const;       // 0-59
  int  getDayOfWeek() const; // Sunday = 0
  int  getDayOfMonth() const;// 1-31
  int  getDayOfYear() const; // Jan 1st = 0
  int  getDayJulian() const; // 1-?
  int  getMonth() const;     // 1-12
  int  getYear() const;      // 0 - 38 representing 2000-2038, 70 - 99 representing 1970 to 1999
  int  getLongYear() const;  // 1970-2038

  friend std::ostream& operator<<(std::ostream&, const LgFrDatetime&);

  // modifiers
  void clear() { time_ = 0; }

  void setTime( int h, int m, int s );
  void setTime( const std::string& hms ); // expected format: hh:mm:ss

  void setDate( int month, int day, int year );
  void setDate( const std::string& mdy ); // expected format: mm/dd/yy OR yyyymmdd

  void setValue( time_t secs ) { time_ = secs; }

  // operators
  LgFrDatetime& operator=(const LgFrDatetime& t) { time_ = t.time_; return *this; }

  bool   operator==( const LgFrDatetime& t ) const { return time_ == t.time_; }
  bool   operator< ( const LgFrDatetime& t ) const { return time_ < t.time_; }
  bool   operator> ( const LgFrDatetime& t ) const { return time_ > t.time_; }
  bool   operator>=( const LgFrDatetime& t ) const { return time_ >= t.time_; }
  bool   operator<=( const LgFrDatetime& t ) const { return time_ <= t.time_; }

  LgFrDatetime operator-(const LgFrDatetime& t) { return( time_ - t.time_ ); }
  LgFrDatetime operator+(const LgFrDatetime& t) { return( time_ + t.time_ ); }

  LgFrDatetime plusYears( int y ) const;
  LgFrDatetime plusQuarters( int q ) const;
  LgFrDatetime plusMonths( int m ) const;
  LgFrDatetime plusWeeks( int w ) const;
  LgFrDatetime plusDays( int d ) const;
  LgFrDatetime plusHours( int h ) const;
  LgFrDatetime plusMinutes( int m ) const;
  LgFrDatetime plusSeconds( int s ) const;

  // general services
  static  bool   validateDate( int month, int day, int year );
  static  bool   validateTime( int hours, int mins, int seconds );
  static  time_t currentTime() { return ::time(0); }

  static  bool   unitTest();

private:

  const struct tm& getAsStruct_tm() const;

  static bool isLeapYear( size_t y ) { return ( y % 400 == 0 || (y % 4 == 0 && y % 100 != 0 )); }
  static long julian_date(int day, int month, int year);
  static void calendar_date(long jdate, int& day, int& month, int& year);
  static bool valid_date(int day, int month, int year);

};


#endif

