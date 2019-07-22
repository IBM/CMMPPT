#ifndef RWToSTLHelpers_h
#define RWToSTLHelpers_h

#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif

// RW2STL -- inserted:
#include <fstream>
#include <scenario/src/RWToSTLHelpers.h>
#include <string>
#include <vector>
// End of RW2STL -- inserted includes.

#include <sstream>

class ostream;
class ostream_withassign;

//std::ofstream& operator<<( std::ofstream& stream, const std::string& s );
//std::strstreambuf& operator<<( std::strstreambuf& stream, const std::string& s );
//ostream& operator<<( ostream& stream, std::string& );
//ostream& operator<<( ostream& stream, const std::string& );
//inline ostream_withassign& operator<<( ostream_withassign& stream, std::string& );

namespace sce 
{

const size_t badIndex = 29999;

size_t      hashString( const std::string& str );
int        compareStringsCaseIns( const std::string& l, const std::string& r );
size_t      find( const std::string& s, const std::string& sub );
size_t      findCaseIns( const std::string& s, const std::string& sub );
std::string extractQuotedPart( const std::string& s );
void		toLower( std::string &str );
void		toUpper( std::string &str );
std::string strip( std::string &str, const std::string& toRemove = " " );

std::string stringFrom( int i );
std::string stringFrom( long i );
std::string stringFrom( unsigned long i );
std::string stringFrom( float i, size_t precision = 6, bool showPoint = false );
std::string stringFrom( double i, size_t precision = 6, bool showPoint = false );

bool        fromString( const std::string& s, long& l );
bool        fromString( const std::string& s, unsigned long& l );
bool        fromString( const std::string& s, int& l );
bool        fromString( const std::string& s, unsigned int& l );
bool        fromString( const std::string& s, float& l );
bool        fromString( const std::string& s, double& l );

bool		matchRegexp( const std::string& s, const std::string& regExp );
bool		checkRegexp( const std::string& regExp );

template<class T>
int      findInVector( const T&, const std::vector<T>& v );

} // namespace sce

class SCETokenizer
{
public:
  SCETokenizer( const std::string& s );     

  std::string       operator()( const std::string& tokens = " \n\t\000" );
  //std::string       allRemaining( const std::string& tokens = " \n\t\000" );   
private:
  const std::string   myString_;
  size_t              cursor_;
};




#endif
