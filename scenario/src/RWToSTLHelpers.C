// RW2STL -- inserted:
#include <fstream>
#include <scenario/src/RWToSTLHelpers.h>
#include <string>
// End of RW2STL -- inserted includes.

#include <scenario/src/machdep.h>

#include <iosfwd>
#include <ostream>
#include <assert.h>

// General std::string functions previously done in RWCString
// Note that these may not work for wide-character strings

bool sce::matchRegexp( const std::string& s, const std::string& regExp )
{
    if( regExp == ".*" )
        return true;

    if( regExp.find_first_of( "+*?.[]^$" ) == std::string::npos )
        return ( find( s, regExp ) == std::string::npos ) ? false : true;

    // Note that this logiv replaces the RWRegExp class. It tries to behave similarly,
    // and as a result the regexp string is not automatically anchored to the beginning
    // or end-of-line. Therefore, an occurence of ".*" at the end or beginning of the
    // regexp string has absolutely no consequence.
    if( regExp.length() > 2 && regExp.compare( regExp.length() - 2, 2, ".*" ) == 0 )
        return matchRegexp( s, regExp.substr( 0, regExp.length() - 2 ) );

    if( regExp.length() > 2 && regExp.compare( 0, 2, ".*" ) == 0 )
        return matchRegexp( s, regExp.substr( 2 ) );

    assert( 0 && "Unhandled regular expression!" ); // Add code to handle this unexpected form of regexp

	return false;
}

bool sce::checkRegexp( const std::string& regExp )
{
    if( regExp == ".*" )
        return true;

    if( regExp.find_first_of( "+*?.[]^$" ) == std::string::npos )
        return true; 

    if( regExp.compare( 0, 2, ".*" ) == 0 )
        return checkRegexp( regExp.substr( 2 ) );

    if( regExp.compare( regExp.length() - 2, 2, ".*" ) == 0 )
        return checkRegexp( regExp.substr( 0, regExp.length() - 2 ) );

    assert( 0 && "Unhandled regular expression!" ); // Add code to handle this unexpected form of regexp

	return false;
}

void sce::toLower( std::string &str )
{
	size_t i = 0;
    for( i = 0; i < str.size(); i++ )
		str[i] = tolower( str[i] );
}


void sce::toUpper( std::string &str )
{
	size_t i = 0;
    for( i = 0; i < str.size(); i++ )
		str[i] = toupper( str[i] );
}


size_t sce::hashString( const std::string& str )
{
    const char* s = str.c_str();
    size_t h = 0;
    size_t g;
    if( ! s )
        return 0;
    while( *s )
        if( g = ( h = ( h << 4 ) + *s++ ) & 0xf0000000UL )
            h = ( h ^ ( g >> 24 ) ) & 0x0fffffffUL;
    return h;
}

int sce::compareStringsCaseIns( const std::string& l, const std::string& r )
{
    return LgFrStrcmpcaseins( l.c_str(), r.c_str() );
}

size_t sce::findCaseIns( const std::string& s, const std::string& sub )
{
    std::string s_ = s;
    toLower( s_ );
    std::string sub_ = sub;
    toLower( sub_ );

    return find( s_, sub_ );
}

size_t sce::find( const std::string& s, const std::string& sub )
{
    if( s == sub )
        return 0;
    if( s.length() < sub.length() || sub.empty() )
        return std::string::npos;

    size_t i = 0;
    for( i = 0; i <= s.size() - sub.size(); i++ )
        if( strncmp( &s.c_str()[i], sub.c_str(), sub.length() ) == 0 )
            return i;

    return std::string::npos;
}

std::string sce::extractQuotedPart( const std::string& s )
{
    // At this time, cursor_ points to the first character following the last word
    // We need to skip the tokens
    int firstquote = s.find_first_of( '\"' );
    int secondquote = s.find_first_of( '\"', firstquote + 1 );

    return s.substr( firstquote + 1, secondquote - firstquote - 1 ); // also works if cursor_ == std::npos
}

std::string sce::strip( std::string& str, const std::string& toRemove )
{
  std::string::size_type first = str.find_first_not_of( toRemove );
  std::string::size_type last = str.find_last_not_of( toRemove );

  if( first == std::string::npos ) first = 0;

  return str.substr( first, last - first + 1 );
} 

std::string sce::stringFrom( int i )
{
    char str[100];
    sprintf( str, "%d", i );
    return str;
}

std::string sce::stringFrom( long i )
{
    char str[100];
    sprintf( str, "%ld", i );
    return str;
}

std::string sce::stringFrom( unsigned long i )
{
    char str[100];
    sprintf( str, "%ld", i );
    return str;
}

std::string sce::stringFrom( float i, size_t precision, bool showPoint )
{
    char str[100];

    sprintf( str, "%.*f", precision, i );
    if( showPoint && ( strchr( str, '.' ) == 0 ) )
        strcat( str, "." );
    return str;
}

std::string sce::stringFrom( double i, size_t precision, bool showPoint )
{
    char str[100];

    sprintf( str, "%.*f", precision, i );
    if( showPoint && ( strchr( str, '.' ) == 0 ) )
        strcat( str, "." );
    return str;
}

bool sce::fromString( const std::string& s, long& l )
{
    bool b = sscanf( s.c_str(), "%ld", &l) > 0 ? true : false;
    return b;
}

bool sce::fromString( const std::string& s, unsigned long& l )
{
    bool b = sscanf( s.c_str(), "%ld", &l) > 0 ? true : false;
    return b;
}

bool sce::fromString( const std::string& s, int& i )
{
    bool b = sscanf( s.c_str(), "%d", &i) > 0 ? true : false;
    return b;
}

bool sce::fromString( const std::string& s, unsigned int& i )
{
    bool b = sscanf( s.c_str(), "%d", &i) > 0 ? true : false;
    return b;
}

bool sce::fromString( const std::string& s, float& i )
{
    bool b = sscanf( s.c_str(), "%f", &i) > 0 ? true : false;
    return b;
}

bool sce::fromString( const std::string& s, double& i )
{
    bool b = sscanf( s.c_str(), "%lf", &i) > 0 ? true : false;
    return b;
}



// RWTokenizer replacement

SCETokenizer::SCETokenizer(const std::string& s) : myString_( s ), cursor_(0)
{
}

std::string SCETokenizer::operator()( const std::string& tokens )
{
    // At this time, cursor_ points to the first character following the last word
    // We need to skip the tokens
    cursor_ = myString_.find_first_not_of( tokens, cursor_ );
    if( cursor_ == std::string::npos )
        return "";
    size_t first = cursor_;
    cursor_ = myString_.find_first_of( tokens, first );

    return myString_.substr( first, cursor_ - first ); // also works if cursor_ == std::npos
}

/*std::string SCETokenizer::allRemaining( const std::string& tokens )
{
    cursor_ = myString_.find_first_not_of( tokens, cursor_ );

    return &myString_[ cursor_ ];
}*/


/*std::ofstream& operator<<( std::ofstream& stream, const std::string& s )
{

// stream << (char*) s.c_str();   This was replaced by the line below because AIX gets
// confused between the stream.operator<<(char*) and this global std::ostream<<( const std::string ) 
    stream.operator<<( (char*) s.c_str());
    return stream;
}*/

/*ostream& operator<<( ostream& stream, std::string& s )
{
    stream << s.c_str();
    return stream;
}*/

