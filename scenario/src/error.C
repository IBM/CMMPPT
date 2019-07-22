#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif

// RW2STL -- inserted:
// End of RW2STL -- inserted includes.
#include <scenario/src/error.h>

void
LgFrError::setError(int e) { errorNum_ = e; }

// Equality and inequality
int
LgFrError::operator==(const LgFrError& e) const
{
    return errorNum_ == e.errorNum_; 
}

int 
LgFrError::operator!=(const LgFrError& e) const
{
    return !(*this == e);
}

// Assignment
LgFrError&
LgFrError::operator=(const LgFrError& e)
{
    if (this != &e) {           // Check for assignment to self
	errorNum_ = e.errorNum_;
    }
    return *this;
}

// Constructor
LgFrError::LgFrError(){}
LgFrError::LgFrError(int e) 
{
    errorNum_ = e; 
}

// Destructor
LgFrError::~LgFrError(){}

