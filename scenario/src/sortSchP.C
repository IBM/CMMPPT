// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.
#include <iostream>
#include <assert.h>
#include <scenario/src/sortSchP.h>
#include <scenario/src/sortSchd.h>

// Default Constructor
LgFrSortingScheduleFloatSmartPointerToConst::LgFrSortingScheduleFloatSmartPointerToConst() : pointer_(new LgFrSortingScheduleFloat), count_(1)
{
};

// Preferred Constructor
LgFrSortingScheduleFloatSmartPointerToConst::LgFrSortingScheduleFloatSmartPointerToConst(LgFrSortingScheduleFloat* newedPointer) 
: pointer_(newedPointer), count_(1)
{
}

// Constructor for cast operator
LgFrSortingScheduleFloatSmartPointerToConst::LgFrSortingScheduleFloatSmartPointerToConst(
    LgFrSortingScheduleFloat* existingPointer,
    LgFrReferenceCount& count) : pointer_(existingPointer)
{
    count_ = count;
}

// Destructor
LgFrSortingScheduleFloatSmartPointerToConst::~LgFrSortingScheduleFloatSmartPointerToConst()
{
    if (pointer_ != 0)  {
	if (count_.reference() == 1)  {
	    delete pointer_;
	}
    }
}

// Copy Constructor
LgFrSortingScheduleFloatSmartPointerToConst::LgFrSortingScheduleFloatSmartPointerToConst(const LgFrSortingScheduleFloatSmartPointerToConst& rhs) 
: pointer_(rhs.pointer_)
{
    count_ = ((LgFrSortingScheduleFloatSmartPointerToConst&)rhs).count_;
}

// Deep Copy
LgFrSortingScheduleFloatSmartPointerToConst
LgFrSortingScheduleFloatSmartPointerToConst::deepCopy() const
{
    // do SortingScheduleFloat deep copy and create a new smart pointer
    return LgFrSortingScheduleFloatSmartPointerToConst(pointer_->clone());
}

// Assignment Operator
LgFrSortingScheduleFloatSmartPointerToConst& 
LgFrSortingScheduleFloatSmartPointerToConst::operator=(const LgFrSortingScheduleFloatSmartPointerToConst& rhs)
{
    // check to see if this smart pointer is the only one pointing to
    //  the actual object.  If it is, then we want to delete the
    //  pointer
    if (pointer_ != 0)  {
	if (count_.reference() == 1)
	    delete pointer_;
    }
    count_ = ((LgFrSortingScheduleFloatSmartPointerToConst&)rhs).count_;
    pointer_ = rhs.pointer_;
    return *this;
}

bool
LgFrSortingScheduleFloatSmartPointerToConst::operator==(const LgFrSortingScheduleFloatSmartPointerToConst& rhs) const
{
    if ( (pointer_ == rhs.pointer_) && (pointer_ == 0 || count_ == rhs.count_) )
	return true;
    else
	return false;
}

bool
LgFrSortingScheduleFloatSmartPointerToConst::operator!=(const LgFrSortingScheduleFloatSmartPointerToConst& rhs) const
{
    return !( *this == rhs );
}

const LgFrSortingScheduleFloat* const
LgFrSortingScheduleFloatSmartPointerToConst::operator->() const
{
    // implicit cast to const
    return pointer_;
}

const LgFrSortingScheduleFloat&
LgFrSortingScheduleFloatSmartPointerToConst::operator*() const
{
    // implicit cast to const
    return *pointer_;
}

/*
LgFrSortingScheduleFloatSmartPointerToConst::operator const LgFrSortingScheduleFloat* const () const
{
    return &(*pointer_);
}
*/

bool
LgFrSortingScheduleFloatSmartPointerToConst::unique() const
{
    return (count_.reference() == 1);
}

bool 
LgFrSortingScheduleFloatSmartPointerToConst::null() const
{
    return (pointer_ == 0);
}

void
LgFrSortingScheduleFloatSmartPointerToConst::test()
{
    std::cout << "Testing default constructor" << std::endl;
    LgFrSortingScheduleFloatSmartPointerToConst dummy;
    std::cout << "dummy count is " << dummy.count_.reference() << std::endl;

    std::cout << "Testing preferred constructor (dummy)" << std::endl;
    LgFrSortingScheduleFloatSmartPointerToConst b(new LgFrSortingScheduleFloat);
    std::cout << "b count is " << b.count_.reference() << std::endl;    

    std::cout << "Testing assignement operator dummy = b" << std::endl;
    dummy = b;
    std::cout << "b count is " << b.count_.reference() << std::endl;    

    std::cout << "Testing copy constructor (c = b)" << std::endl;
    LgFrSortingScheduleFloatSmartPointerToConst c(b);
    std::cout << "b count is " << b.count_.reference() << std::endl;    

    std::cout << "Testing backdoor constructor" << std::endl;
    LgFrSortingScheduleFloatSmartPointerToConst e(b.pointer_, b.count_);
    std::cout << "e count is " << e.count_.reference() << std::endl;    

    std::cout << "Testing equality operator (c == b)" << std::endl;
    assert(c == b);

    std::cout << "Testing inequality operator (c != d)" << std::endl;
    LgFrSortingScheduleFloatSmartPointerToConst d(new LgFrSortingScheduleFloat);
    assert(c != d);

    std::cout << "Test destructor" << std::endl;
    c = d;
    std::cout << "b count is " << b.count_.reference() << std::endl;    
    c = b;
    d = b;

    // this should be a compiler error
//    LgFrSortingScheduleFloatSmartPointerToConst e(&(*d));
}



LgFrSortingScheduleFloatSmartPointer::LgFrSortingScheduleFloatSmartPointer() 
{
    // calls base class default constructor
}

LgFrSortingScheduleFloatSmartPointer::LgFrSortingScheduleFloatSmartPointer(LgFrSortingScheduleFloat* newedPointer) 
: LgFrSortingScheduleFloatSmartPointerToConst(newedPointer)
{
}

LgFrSortingScheduleFloatSmartPointer::LgFrSortingScheduleFloatSmartPointer(
    LgFrSortingScheduleFloat* existingPointer,
    LgFrReferenceCount& count) 
: LgFrSortingScheduleFloatSmartPointerToConst(existingPointer, count)
{
}


LgFrSortingScheduleFloatSmartPointer::~LgFrSortingScheduleFloatSmartPointer()
{
    // call base class destructor
}

LgFrSortingScheduleFloatSmartPointer::LgFrSortingScheduleFloatSmartPointer(const LgFrSortingScheduleFloatSmartPointer& rhs) 
: LgFrSortingScheduleFloatSmartPointerToConst(rhs.pointer_)
{
    count_ = ((LgFrSortingScheduleFloatSmartPointer&)rhs).count_;
}

// need to do some casting to a derived class so that we can access protected
//  members
LgFrSortingScheduleFloatSmartPointer::LgFrSortingScheduleFloatSmartPointer(LgFrSortingScheduleFloatSmartPointerToConst rhs) 
    : LgFrSortingScheduleFloatSmartPointerToConst(((LgFrSortingScheduleFloatSmartPointer&)rhs).pointer_)
{
    count_ = ((LgFrSortingScheduleFloatSmartPointer&)rhs).count_;
}

LgFrSortingScheduleFloatSmartPointer
LgFrSortingScheduleFloatSmartPointer::deepCopy() const
{
    // do SortingScheduleFloat deep copy and create a new smart pointer
    return LgFrSortingScheduleFloatSmartPointer(pointer_->clone());
}

LgFrSortingScheduleFloatSmartPointer& 
LgFrSortingScheduleFloatSmartPointer::operator=(const LgFrSortingScheduleFloatSmartPointer& rhs)
{
    // check to see if this smart pointer is the only one pointing to
    //  the actual object.  If it is, then we want to delete the
    //  pointer
    if (pointer_ != 0)  {
	if (count_.reference() == 1)
	    delete pointer_;
    }
    count_ = ((LgFrSortingScheduleFloatSmartPointer&)rhs).count_;
    pointer_ = rhs.pointer_;
    return *this;
}

// need to do some casting to a derived class so that we can access protected
//  members
/*
LgFrSortingScheduleFloatSmartPointer& 
LgFrSortingScheduleFloatSmartPointer::operator=(LgFrSortingScheduleFloatSmartPointerToConst rhs)
{
    // check to see if this smart pointer is the only one pointing to
    //  the actual object.  If it is, then we want to delete the
    //  pointer
    if (pointer_ != 0)  {
	if (count_.reference() == 1)
	    delete pointer_;
    }
    count_ = ((LgFrSortingScheduleFloatSmartPointer&)rhs).count_;
    pointer_ = ((LgFrSortingScheduleFloatSmartPointer&)rhs).pointer_;
    return *this;
}
*/

bool
LgFrSortingScheduleFloatSmartPointer::operator==(const LgFrSortingScheduleFloatSmartPointer& rhs) const
{
    if ( (pointer_ == rhs.pointer_) && (pointer_ == 0 || count_ == rhs.count_) )
	return true;
    else
	return false;
}
/*
bool
LgFrSortingScheduleFloatSmartPointer::operator==(LgFrSortingScheduleFloatSmartPointerToConst rhs) const
{
    if ( (pointer_ == rhs.pointer_) && (count_ == rhs.count_) )
	return true;
    else
	return false;
}
*/
bool
LgFrSortingScheduleFloatSmartPointer::operator!=(const LgFrSortingScheduleFloatSmartPointer& rhs) const
{
    return !( *this == rhs );
}
/*
bool
LgFrSortingScheduleFloatSmartPointer::operator!=(LgFrSortingScheduleFloatSmartPointerToConst rhs) const
{
    if ( (pointer_ != rhs.pointer_) || (count_ != rhs.count_) )
	return true;
    else
	return false;
}
*/
const LgFrSortingScheduleFloat* const
LgFrSortingScheduleFloatSmartPointer::operator->() const
{
    return pointer_;
}

LgFrSortingScheduleFloat* const
LgFrSortingScheduleFloatSmartPointer::operator->()
{
    return pointer_;
}

LgFrSortingScheduleFloat &
LgFrSortingScheduleFloatSmartPointer::operator*()
{
    return *pointer_;
}

const LgFrSortingScheduleFloat &
LgFrSortingScheduleFloatSmartPointer::operator*() const
{
    return *pointer_;
}

/*
LgFrSortingScheduleFloatSmartPointer::operator const LgFrSortingScheduleFloat* const () const
{
    return &(*pointer_);
}

LgFrSortingScheduleFloatSmartPointer::operator LgFrSortingScheduleFloat* ()
{
    return &(*pointer_);
}
*/

bool
LgFrSortingScheduleFloatSmartPointer::unique() const
{
    return (count_.reference() == 1);
}

bool 
LgFrSortingScheduleFloatSmartPointer::null() const
{
    return (pointer_ == 0);
}


void
LgFrSortingScheduleFloatSmartPointer::test()
{
    std::cout << "Testing default constructor" << std::endl;
    LgFrSortingScheduleFloatSmartPointer dummy;
    std::cout << "dummy count is " << dummy.count_.reference() << std::endl;

    std::cout << "Testing preferred constructor (dummy)" << std::endl;
    LgFrSortingScheduleFloatSmartPointer b(new LgFrSortingScheduleFloat);
    std::cout << "b count is " << b.count_.reference() << std::endl;    

    std::cout << "Testing assignement operator dummy = b" << std::endl;
    dummy = b;
    std::cout << "b count is " << b.count_.reference() << std::endl;    

    std::cout << "Testing copy constructor (c = b)" << std::endl;
    LgFrSortingScheduleFloatSmartPointer c(b);
    std::cout << "b count is " << b.count_.reference() << std::endl;    

    std::cout << "Testing backdoor constructor" << std::endl;
    LgFrSortingScheduleFloatSmartPointer e(b.pointer_, b.count_);
    std::cout << "e count is " << e.count_.reference() << std::endl;    

    std::cout << "Testing equality operator (c == b)" << std::endl;
    assert(c == b);

    std::cout << "Testing inequality operator (c != d)" << std::endl;
    LgFrSortingScheduleFloatSmartPointer d(new LgFrSortingScheduleFloat);
    assert(c != d);

    std::cout << "Test destructor" << std::endl;
    c = d;
    std::cout << "b count is " << b.count_.reference() << std::endl;    
    c = b;
    d = b;
}

