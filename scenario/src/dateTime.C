
#include <scenario/src/dateTime.h>
#include <scenario/src/RWToSTLHelpers.h>

#include <assert.h>
#include <math.h>

const unsigned int secsPerDay = 86400; // quit saying "sex per day", it's "secs per day"
// This is used to interpret the 2-digit values passed-in for years as being before or after 2000
const unsigned jan1st1970JulianDay = 2440587;
const unsigned int pivotYear = 70;     


LgFrDatetime::LgFrDatetime( time_t s )
 : time_(s) 
{
}  

LgFrDatetime::LgFrDatetime( const LgFrDatetime& t )
 : time_( t.time_ )
{
}

LgFrDatetime::LgFrDatetime( const std::string& date, const std::string& time )
: time_( 0 )
{ 
    setDate( date ); 
    if( !time.empty() )
        setTime( time );
}

const struct tm& LgFrDatetime::getAsStruct_tm() const
{
    // this function owns a private, static struct tm and the value it is good for.
    // It prevents it from having to reconvert from time_ to struct tm on every call.
    // It also allows it to appear as const because the struct tm is not a member.
    static time_t timeStructtmFor = 0; 
    static struct tm tm_;

    if( time_ != timeStructtmFor )
    {
        memcpy( &tm_, gmtime( &time_ ), sizeof( tm_ ) );
        timeStructtmFor = time_;
    }

    return tm_;
}

// accessors
std::string LgFrDatetime::getDateAsString( const std::string& format ) const
{
    const struct tm& z = getAsStruct_tm();

    char tmp[100];
    strftime( tmp, sizeof( tmp ), format.c_str(), &z );

    return tmp;
}
std::string LgFrDatetime::getTimeAsString( const std::string& format ) const
{
    const struct tm& z = getAsStruct_tm();

    char tmp[100];
    strftime( tmp, sizeof( tmp ), format.c_str(), &z );

    return tmp;
}

void LgFrDatetime::getDate( int& month, int& day, int& year ) const
{
    const struct tm& z = getAsStruct_tm();

    year =  z.tm_year + 1900; // tm_year is 1900-based
    month = z.tm_mon + 1; // tm_mon is 0-based
    day =   z.tm_mday;
}

void LgFrDatetime::getTime( int& hour, int& min, int& sec ) const
{
    const struct tm& z = getAsStruct_tm(); 

    hour =  z.tm_hour;
    min =   z.tm_min;
    sec =   z.tm_sec;
}

void LgFrDatetime::getDatetime( int& month, int& day, int& year, int& hour, int& min, int& sec ) const
{
    const struct tm& z = getAsStruct_tm();

    year =  z.tm_year + 1900; // tm_year is 1900-based
    month = z.tm_mon + 1; // tm_mon is 0-based 
    day =   z.tm_mday;
    hour =  z.tm_hour;
    min =   z.tm_min;
    sec =   z.tm_sec;
}

int  LgFrDatetime::getHour() const 
{
    return getAsStruct_tm().tm_hour;
}

int  LgFrDatetime::getMin() const
{
    return getAsStruct_tm().tm_min;
}

int  LgFrDatetime::getSec() const
{
    return getAsStruct_tm().tm_sec;
}

int  LgFrDatetime::getDayOfWeek() const
{
    return getAsStruct_tm().tm_wday;
}

int  LgFrDatetime::getDayOfMonth() const
{
    return getAsStruct_tm().tm_mday;
}

int  LgFrDatetime::getDayOfYear() const
{
    return getAsStruct_tm().tm_yday;
}

int  LgFrDatetime::getMonth() const
{
    return getAsStruct_tm().tm_mon + 1; // tm_mon is 0-based
}

int  LgFrDatetime::getYear() const
{
    int year = getAsStruct_tm().tm_year;
    if( year >= 100 )
        year -= 100;

    return year;
}

int  LgFrDatetime::getLongYear() const
{
    return getAsStruct_tm().tm_year + 1900; // tm_year is 1900-based
}

// NOTE: This function is trusted to be exact copies of what was once published 
// in a reliable publication. The only changes that were made are removing code
// dealing with the pre-gregorian calendar, and updating a few types;
// date management, computer language, dec. 1990, p 57 */
// dates are represented as Julian day numbers, stored as long integers (at least 32 bits )
int  LgFrDatetime::getDayJulian() const
{
    return time_ / secsPerDay;
}

std::ostream& operator<<(std::ostream& s, const LgFrDatetime& d)
{
    if( d.isValid() )
        return s << asctime( &d.getAsStruct_tm() );
    else
        return s << "Invalid date!";
}


// modifiers
void LgFrDatetime::setTime( int h, int m, int s )
{
    bool valid = validateTime( h, m, s );

    if( valid )
    {
        time_ = time_ - time_ % secsPerDay;
        time_ += s + 60 * ( m + 60 * h );
    }
    else
    {
        time_ = 0;
    }
}

void LgFrDatetime::setTime( const std::string& hms ) 
{
    if( hms.find_first_of( ':' ) == std::string::npos )
    {
        time_ = 0; 
        return;
    }

    // Must be specified in "HH:MM" or "HH:MM:SS" 24-hour format
    size_t firstSep = hms.find_first_of( ':' );
    size_t secondSep = hms.find_first_of( ':', firstSep + 1 );
    size_t thirdSep = hms.find_first_of( ':', secondSep + 1 );

    // Make sure we have one or two separators placed reasonably
    bool valid = ( firstSep != std::string::npos && thirdSep == std::string::npos );

    if( valid )
    {
        bool longForm = secondSep != std::string::npos;
        size_t h, m, s = 0;
        valid &= sce::fromString( hms.substr( 0, firstSep ), h );
        valid &= sce::fromString( hms.substr( firstSep + 1, secondSep - firstSep - 1 ), m );
        if( longForm )
            valid &= sce::fromString( hms.substr( secondSep + 1, hms.length() - secondSep + 1 ), s );

        valid &= ( h < 24 && m < 60 && s < 60 );

        if( valid )
            setTime( h, m, s );
    }
    if( !valid )
        time_ = 0;
}

void LgFrDatetime::setDate( int month, int day, int year )
{
    if( year < pivotYear )
        year += 100;
    if( year < 1900 )
        year += 1900;

    bool valid = validateDate( month, day, year ); 

    if( valid )
    {
        time_ = time_ % secsPerDay; 

        time_ += ( ( julian_date( day, month, year ) - jan1st1970JulianDay ) * secsPerDay );
    }
    else
        time_ = 0;

}

void LgFrDatetime::setDate( const std::string& mdy ) // format: yy/mm/dd OR yyyy/mm/dd (pivot year is 70)
{
    if( mdy.find_first_of( '/' ) == std::string::npos && mdy.length() != 8 )
    {
        time_ = 0; 
        return;
    }

    size_t firstSep = mdy.find_first_of( '/' );
    size_t secondSep = mdy.find_first_of( '/', firstSep + 1 );
    size_t thirdSep = mdy.find_first_of( '/', secondSep + 1 );

    // Make sure we have only two separators placed reasonably
    bool goodSeps = ( firstSep != std::string::npos && secondSep != std::string::npos && thirdSep == std::string::npos );
    goodSeps &= ( firstSep > 0 && secondSep > firstSep + 1 && secondSep < mdy.length() - 1 );

    bool straight = (firstSep == std::string::npos) && (secondSep == std::string::npos) && (thirdSep == std::string::npos);

    unsigned int m, d, y;

    bool valid = false;
    if( straight )
    {
        valid  = sce::fromString( mdy.substr( 0, 4 ), y );
        valid &= sce::fromString( mdy.substr( 4, 2 ), m );
        valid &= sce::fromString( mdy.substr( 6, 2 ), d );
    }
    else if( goodSeps )
    {
        valid  = sce::fromString( mdy.substr( 0, firstSep ), m );
        valid &= sce::fromString( mdy.substr( firstSep + 1, secondSep - firstSep - 1 ), d );
        valid &= sce::fromString( mdy.substr( secondSep + 1, mdy.length() - secondSep + 1 ), y );
    }

    setDate( m, d, y );
}

// general services
bool LgFrDatetime::validateDate( int month, int day, int year )
{
     if( year < 1970 || year >= 2038 ) // these are the limitations of the time library routines
         return false;

	/* validate the specified d/m/y */
	/* i.e., 2/29/73 and april 31 are not valid */

	int cal_day, cal_month, cal_year;
	/* convert date to julian day number and back */
	calendar_date(julian_date(day, month, year), cal_day, cal_month, cal_year);

	/* date is valid if d/m/y did not change */
	return ((day == cal_day) && (month == cal_month) && (year == cal_year));
}

bool LgFrDatetime::validateTime( int h, int m, int s )
{
	return ( h >= 0 && h < 24 && m >= 0 && m < 60 && s >= 0 && s < 60 );
}
  
LgFrDatetime LgFrDatetime::plusYears( int years ) const
{
    LgFrDatetime dt( *this );
    int m,d,y;
    dt.getDate( m, d, y );
    y += years;
    dt.setDate( m, d, y );

    return dt;
}

LgFrDatetime LgFrDatetime::plusQuarters( int quarters ) const
{
    return plusMonths( 3 * quarters );
}

LgFrDatetime LgFrDatetime::plusMonths( int months ) const
{
    LgFrDatetime dt( *this );
    int m,d,y;
    dt.getDate( m, d, y );

    bool lastDayOfMonth = !validateDate( m, d+1, y );

    m += months;
    while( m <= 0 ) // months may be more than 12, and negative
    {
        m += 12;
        y -= 1;
    }
    while( m > 12 )
    {
        m -= 12;
        y += 1;
    }

    // If we were the last day of the month, make the result also the last day of the month
    if( lastDayOfMonth )
    {
        while( validateDate( m, d+1, y ) ) 
            d += 1;
    }

    // we may have an invalid date if the day was near the upper limit for the month
    while( !validateDate( m, d, y ) ) 
        d -= 1;

    dt.setDate( m, d, y );

    return dt;
}

LgFrDatetime LgFrDatetime::plusWeeks( int weeks ) const
{
    LgFrDatetime dt( *this );

    dt.setValue( dt.getValue() + weeks * 7 * secsPerDay );

    return dt;
}

LgFrDatetime LgFrDatetime::plusDays( int days ) const
{
    LgFrDatetime dt( *this );

    dt.setValue( dt.getValue() + days * secsPerDay );

    return dt;
}

LgFrDatetime LgFrDatetime::plusHours( int hours ) const
{
    LgFrDatetime dt( *this );

    dt.setValue( dt.getValue() + hours * 3600 );

    return dt;
}

LgFrDatetime LgFrDatetime::plusMinutes( int minutes ) const
{
    LgFrDatetime dt( *this );

    dt.setValue( dt.getValue() + minutes * 60 );

    return dt;
}

LgFrDatetime LgFrDatetime::plusSeconds( int seconds ) const
{
    LgFrDatetime dt( *this );

    dt.setValue( dt.getValue() + seconds );

    return dt;
}

// NOTE: These three functions are trusted to be exact copies of what was once published 
// in a reliable publication. The only changes that were made are removing code
// dealing with the pre-gregorian calendar, and updating a few types;
// date management, computer language, dec. 1990, p 57 */
// dates are represented as Julian day numbers, stored as long integers (at least 32 bits )

long LgFrDatetime::julian_date(int day, int month, int year) // static
{

    long aa = (14L-(long)month)/12L;
    long yy = (long)year+4800L-aa;
    long mm = (long)month+12L*aa-3L;

    long retVal = day;
    retVal += (153L*(long)mm+2L)/5L;
    retVal += 365L*(long)yy;
    retVal += (long)yy/4L;

    retVal -= (long)yy/100L;
    retVal += (long)yy/400L;
    retVal -= 32045L;

    retVal -= 1L;

    return retVal;


    //    return jd2;
}


void LgFrDatetime::calendar_date(long jdate, int& day, int& month, int& year) // static
{
	/* calculate the day, month, year corresponding to julian day number */
	/* the year will be negative if year is B.C. */

	long a, b, c, d, e, z, alpha;
	
	z = jdate + 1;
	/* cope with Gregorian calendar reform */
	if (z < 2299161L)
		a = z;
	else
	{
		alpha = (long) ((z - 1867216.25) / 36524.25);
		a = z + 1 + alpha - alpha / 4;
	}
	b = a + 1524;
	c = (long) ((b - 122.1) / 365.25);
	d = (long) (365.25 * c);
	e = (long) ((b - d) / 30.6001);
	day = (int) b - d - (long) (30.6001 * e);
	month = (int) (e < 13.5) ? e - 1 : e - 13;
	year = (int) (month > 2.5) ? (c - 4716) : c - 4715;
}


bool LgFrDatetime::valid_date( int day, int month, int year ) // static
{
	/* validate the specified d/m/y */
	/* i.e., 2/29/73 and april 31 are not valid */

	int cal_day, cal_month, cal_year;
	/* convert date to julian day number and back */
	calendar_date(julian_date(day, month, year), cal_day, cal_month, cal_year);

	/* date is valid if d/m/y did not change */
	return ((day == cal_day) && (month == cal_month) && (year == cal_year));
}




bool LgFrDatetime::unitTest()
{
    // Formats required by Calendar Unittest: SCETime(SCEDate("12/30/94"), "08:00"),
   
    LgFrDatetime tmz ( "1/11/3", "12:02:32" );
    time_t tt0 = tmz.getValue();
    assert( tmz.getTimeAsString() == "12:02:32" );
    tmz.setTime( "12:02:32" );
    assert( tmz.getTimeAsString() == "12:02:32" );
    tmz.setDate( "1/11/3" );
    assert( tmz.getDateAsString() == "01/11/03" );

    LgFrDatetime t0;
    assert( t0.getValue() == 0 );
    assert( !t0.isValid() );
    t0.setValue( 1000 );
    assert( t0.getValue() == 1000 );

    LgFrDatetime t1 ( "1/11/3", "21:05:56" );

    std::cout << t1;

    LgFrDatetime t2( t1 ); 

    assert( t2.getYear() == 3 );
    assert( t2.getLongYear() == 2003 );
    assert( t2.getMonth() == 1 );
    assert( t2.getDayJulian() == 12063 );
    assert( t2.getDayOfYear() == 10 ); 
    assert( t2.getDayOfMonth() == 11 );
    assert( t2.getDayOfWeek() == 6 );
    assert( t2.getHour() == 21 );
    assert( t2.getMin() == 5 );
    assert( t2.getSec() == 56 );

    assert( t1 == t2 );
    assert( t1.getValue() > 1041948451 && t1.getValue() < 1141948451 );

    t2 = t1 + 3;

    assert( t2 > t1 && t1 < t2 );
    assert( t2 >= t1 && t1 <= t2 );
    
    //Check that the following is correct on AIX
    assert( t1.getDateAsString() == "01/11/03" );
    assert( t1.getTimeAsString() == "21:05:56" );

    t1.setDate( "01/15/03" );
    assert( t1.getDateAsString() == "01/15/03" );
    assert( t1.getDateAsStraightString() == "20030115" );

    LgFrDatetime t3( "20030111" );
    assert( t3.getDateAsString() == "01/11/03" );

    LgFrDatetime t4( "20030131" );
    t4 = t4.plusMonths( 1 );
    assert( t4.getDateAsStraightString() == "20030228" );
    t4 = t4.plusMonths( 1 );
    assert( t4.getDateAsStraightString() == "20030331" );

    assert( LgFrDatetime::validateTime( -1, 0, 0 ) == false );
    assert( LgFrDatetime::validateTime( 0, -1, 0 ) == false );
    assert( LgFrDatetime::validateTime( 0, 0, -1 ) == false );
    assert( LgFrDatetime::validateTime( 24, 0, 0 ) == false );
    assert( LgFrDatetime::validateTime( 0, 60, 0 ) == false );
    assert( LgFrDatetime::validateTime( 0, 0, 60 ) == false );
    assert( LgFrDatetime::validateTime( 0, 0, 0 ) == true );
    assert( LgFrDatetime::validateTime( 23, 59, 59 ) == true );

    assert( LgFrDatetime::validateDate( -1, 0, 0 ) == false );
    assert( LgFrDatetime::validateDate( 0, -1, 0 ) == false );
    assert( LgFrDatetime::validateDate( 0, 0, -1 ) == false );
    assert( LgFrDatetime::validateDate( 13, 0, 0 ) == false );
    assert( LgFrDatetime::validateDate( 0, 32, 0 ) == false );

    // Boundary testing for month upper dates, considering leap years
    assert( LgFrDatetime::validateDate( 1, 1, 1969 ) == false );
    assert( LgFrDatetime::validateDate( 1, 1, 2038 ) == false );
    for( int y = 1970; y < 2038; y++ )
    {
        assert( LgFrDatetime::validateDate( 1, 31, y ) == true );
        assert( LgFrDatetime::validateDate( 2, 28, y ) == true );
        if( LgFrDatetime::isLeapYear( y ) )
            assert( LgFrDatetime::validateDate( 2, 29, y ) == true );
        else
            assert( LgFrDatetime::validateDate( 2, 29, y ) == false );
        assert( LgFrDatetime::validateDate( 2, 30, y ) == false );
        assert( LgFrDatetime::validateDate( 3, 31, y ) == true );
        assert( LgFrDatetime::validateDate( 4, 30, y ) == true );
        assert( LgFrDatetime::validateDate( 4, 31, y ) == false );
        assert( LgFrDatetime::validateDate( 5, 31, y ) == true );
        assert( LgFrDatetime::validateDate( 6, 30, y ) == true );
        assert( LgFrDatetime::validateDate( 6, 31, y ) == false );
        assert( LgFrDatetime::validateDate( 7, 31, y ) == true );
        assert( LgFrDatetime::validateDate( 8, 31, y ) == true );
        assert( LgFrDatetime::validateDate( 9, 30, y ) == true );
        assert( LgFrDatetime::validateDate( 9, 31, y ) == false );
        assert( LgFrDatetime::validateDate( 10, 31, y ) == true );
        assert( LgFrDatetime::validateDate( 11, 30, y ) == true );
        assert( LgFrDatetime::validateDate( 11, 31, y ) == false );
        assert( LgFrDatetime::validateDate( 12, 31, y ) == true );
        assert( LgFrDatetime::validateDate( 13, 1, y ) == false );
        assert( LgFrDatetime::validateDate( 1, 0, y ) == false );
        assert( LgFrDatetime::validateDate( 0, 1, y ) == false );
    }

    LgFrDatetime dt( "11/30/02", "22:58:58" );
    assert( dt.getTimeAsString() == "22:58:58" ); 

    assert( dt.plusYears( 3 ).getDateAsString() == "11/30/05" );

    assert( dt.plusQuarters( 5 ).getDateAsString() == "02/29/04" ); // 04 is leap

    assert( dt.plusMonths( 3 ).getDateAsString() == "02/28/03" ); // 03 is not

    assert( dt.plusWeeks( 8 ).getDateAsString() == "01/25/03" );

    assert( dt.plusDays( 45 ).getDateAsString() == "01/14/03" );
    assert( dt.plusDays( -3 ).getDateAsString() == "11/27/02" );

    assert( dt.plusHours( 1 ).getTimeAsString() == "23:58:58" ); 
    assert( dt.plusHours( 1 ).getDateAsString() == "11/30/02" ); 

    assert( dt.plusHours( 2 ).getTimeAsString() == "00:58:58" ); 
    assert( dt.plusHours( 2 ).getDateAsString() == "12/01/02" ); 

    assert( dt.plusMinutes( 1 ).getTimeAsString() == "22:59:58" ); 
    assert( dt.plusMinutes( 2 ).getDateAsString() == "11/30/02" ); 

    assert( dt.plusMinutes( 62 ).getTimeAsString() == "00:00:58" ); 
    assert( dt.plusMinutes( 62 ).getDateAsString() == "12/01/02" ); 

    assert( dt.plusSeconds( 3 ).getTimeAsString() == "22:59:01" ); 
    assert( dt.plusSeconds( -3 ).getTimeAsString() == "22:58:55" );

    assert( dt.plusSeconds( secsPerDay ).getTimeAsString() == "22:58:58" ); 
    assert( dt.plusSeconds( secsPerDay ).getDateAsString() == "12/01/02" ); 

    return true;
}

