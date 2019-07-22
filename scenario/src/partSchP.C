// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.
#include <iostream>
#include <assert.h>
#include <scenario/src/partSchP.h>
#include <scenario/src/partSchd.h>
#include <scenario/src/sortSchP.h>


// Default Constructor
LgFrSortingPartScheduleFloatSmartPointerToConst::LgFrSortingPartScheduleFloatSmartPointerToConst() : pointer_(new LgFrSortingPartScheduleFloat), count_(1)
{
};

// Preferred Constructor
LgFrSortingPartScheduleFloatSmartPointerToConst::LgFrSortingPartScheduleFloatSmartPointerToConst(LgFrSortingPartScheduleFloat* newedPointer) 
: pointer_(newedPointer), count_(1)
{
}

// Constructor for cast operator
LgFrSortingPartScheduleFloatSmartPointerToConst::LgFrSortingPartScheduleFloatSmartPointerToConst(
    LgFrSortingPartScheduleFloat* existingPointer,
    LgFrReferenceCount& count) : pointer_(existingPointer)
{
    count_ = count;
}

// Destructor
LgFrSortingPartScheduleFloatSmartPointerToConst::~LgFrSortingPartScheduleFloatSmartPointerToConst()
{
    if (pointer_ != 0)  {
	if (count_.reference() == 1)
	    delete pointer_;
    }
}

// Copy Constructor
LgFrSortingPartScheduleFloatSmartPointerToConst::LgFrSortingPartScheduleFloatSmartPointerToConst(const LgFrSortingPartScheduleFloatSmartPointerToConst& rhs) 
: pointer_(rhs.pointer_)
{
    count_ = ((LgFrSortingPartScheduleFloatSmartPointerToConst&)rhs).count_;
}

// Deep Copy
LgFrSortingPartScheduleFloatSmartPointerToConst
LgFrSortingPartScheduleFloatSmartPointerToConst::deepCopy() const
{
    // do SortingPartScheduleFloat deep copy and create a new smart pointer
    //  we can cast as we know that we are cloning a derived class
    return LgFrSortingPartScheduleFloatSmartPointerToConst((LgFrSortingPartScheduleFloat *)pointer_->clone());
}

// Assignment Operator
LgFrSortingPartScheduleFloatSmartPointerToConst& 
LgFrSortingPartScheduleFloatSmartPointerToConst::operator=(const LgFrSortingPartScheduleFloatSmartPointerToConst& rhs)
{
    // check to see if this smart pointer is the only one pointing to
    //  the actual object.  If it is, then we want to delete the
    //  pointer
    if (pointer_ != 0)  {
	if (count_.reference() == 1)
	    delete pointer_;
    }
    count_ = ((LgFrSortingPartScheduleFloatSmartPointerToConst&)rhs).count_;
    pointer_ = rhs.pointer_;
    return *this;
}


bool
LgFrSortingPartScheduleFloatSmartPointerToConst::operator==(const LgFrSortingPartScheduleFloatSmartPointerToConst& rhs) const
{
    if ( (pointer_ == rhs.pointer_) && (pointer_ == 0 || count_ == rhs.count_) )
	return true;
    else
	return false;
}

bool
LgFrSortingPartScheduleFloatSmartPointerToConst::operator!=(const LgFrSortingPartScheduleFloatSmartPointerToConst& rhs) const
{
    return !( *this == rhs );
}

const LgFrSortingPartScheduleFloat* const
LgFrSortingPartScheduleFloatSmartPointerToConst::operator->() const
{
    // implicit cast to const
    return pointer_;
}

const LgFrSortingPartScheduleFloat&
LgFrSortingPartScheduleFloatSmartPointerToConst::operator*() const
{
    // implicit cast to const
    return *pointer_;
}

/*
LgFrSortingPartScheduleFloatSmartPointerToConst::operator const LgFrSortingPartScheduleFloat* const () const
{
    return &(*pointer_);
}
*/


LgFrSortingPartScheduleFloatSmartPointerToConst::operator LgFrSortingScheduleFloatSmartPointerToConst () const
{
    // need to cast away const for count_
    return LgFrSortingScheduleFloatSmartPointerToConst(pointer_, (LgFrReferenceCount&)count_);
}

bool
LgFrSortingPartScheduleFloatSmartPointerToConst::unique() const
{
    return (count_.reference() == 1);
}

bool 
LgFrSortingPartScheduleFloatSmartPointerToConst::null() const
{
    return (pointer_ == 0);
}

void
LgFrSortingPartScheduleFloatSmartPointerToConst::test()
{
    std::cout << "Testing default constructor" << std::endl;
    LgFrSortingPartScheduleFloatSmartPointerToConst dummy;
    std::cout << "dummy count is " << dummy.count_.reference() << std::endl;

    std::cout << "Testing preferred constructor (dummy)" << std::endl;
    LgFrSortingPartScheduleFloatSmartPointerToConst b(new LgFrSortingPartScheduleFloat);
    std::cout << "b count is " << b.count_.reference() << std::endl;    

    std::cout << "Testing assignement operator dummy = b" << std::endl;
    dummy = b;
    std::cout << "b count is " << b.count_.reference() << std::endl;    


    std::cout << "Testing copy constructor (c = b)" << std::endl;
    LgFrSortingPartScheduleFloatSmartPointerToConst c(b);
    std::cout << "b count is " << b.count_.reference() << std::endl;    


    std::cout << "Testing backdoor constructor" << std::endl;
    LgFrSortingPartScheduleFloatSmartPointerToConst e(b.pointer_, b.count_);
    std::cout << "e count is " << e.count_.reference() << std::endl;    

    std::cout << "Testing equality operator (c == b)" << std::endl;
    assert(c == b);

    std::cout << "Testing inequality operator (c != d)" << std::endl;
    LgFrSortingPartScheduleFloatSmartPointerToConst d(new LgFrSortingPartScheduleFloat);
    assert(c != d);

    std::cout << "Test destructor" << std::endl;
    c = d;
    std::cout << "b count is " << b.count_.reference() << std::endl;    
    c = b;
    d = b;

    // this should be a compiler error
//    LgFrSortingPartScheduleFloatSmartPointerToConst e(&(*d));
}



LgFrSortingPartScheduleFloatSmartPointer::LgFrSortingPartScheduleFloatSmartPointer() 
{
    // calls base class default constructor
}

LgFrSortingPartScheduleFloatSmartPointer::LgFrSortingPartScheduleFloatSmartPointer(LgFrSortingPartScheduleFloat* newedPointer) 
: LgFrSortingPartScheduleFloatSmartPointerToConst(newedPointer)
{
}

LgFrSortingPartScheduleFloatSmartPointer::LgFrSortingPartScheduleFloatSmartPointer(
    LgFrSortingPartScheduleFloat* existingPointer,
    LgFrReferenceCount& count) 
: LgFrSortingPartScheduleFloatSmartPointerToConst(existingPointer, count)
{
}


LgFrSortingPartScheduleFloatSmartPointer::~LgFrSortingPartScheduleFloatSmartPointer()
{
    // call base class destructor
}

LgFrSortingPartScheduleFloatSmartPointer::LgFrSortingPartScheduleFloatSmartPointer(const LgFrSortingPartScheduleFloatSmartPointer& rhs) 
: LgFrSortingPartScheduleFloatSmartPointerToConst(rhs.pointer_)
{
    count_ = ((LgFrSortingPartScheduleFloatSmartPointer&)rhs).count_;
}

// need to do some casting to a derived class so that we can access protected
//  members
LgFrSortingPartScheduleFloatSmartPointer::LgFrSortingPartScheduleFloatSmartPointer(LgFrSortingPartScheduleFloatSmartPointerToConst rhs) 
    : LgFrSortingPartScheduleFloatSmartPointerToConst(((LgFrSortingPartScheduleFloatSmartPointer&)rhs).pointer_)
{
    count_ = ((LgFrSortingPartScheduleFloatSmartPointer&)rhs).count_;
}

LgFrSortingPartScheduleFloatSmartPointer
LgFrSortingPartScheduleFloatSmartPointer::deepCopy() 
{
    // do SortingPartScheduleFloat deep copy and create a new smart pointer
    return LgFrSortingPartScheduleFloatSmartPointer((LgFrSortingPartScheduleFloat *)pointer_->clone());
}

LgFrSortingPartScheduleFloatSmartPointer& 
LgFrSortingPartScheduleFloatSmartPointer::operator=(const LgFrSortingPartScheduleFloatSmartPointer& rhs)
{
    // check to see if this smart pointer is the only one pointing to
    //  the actual object.  If it is, then we want to delete the
    //  pointer
    if (pointer_ != 0)  {
	if (count_.reference() == 1)
	    delete pointer_;
    }
    count_ = ((LgFrSortingPartScheduleFloatSmartPointer&)rhs).count_;
    pointer_ = rhs.pointer_;
    return *this;
}

// need to do some casting to a derived class so that we can access protected
//  members
/*
LgFrSortingPartScheduleFloatSmartPointer& 
LgFrSortingPartScheduleFloatSmartPointer::operator=(LgFrSortingPartScheduleFloatSmartPointerToConst rhs)
{
    // check to see if this smart pointer is the only one pointing to
    //  the actual object.  If it is, then we want to delete the
    //  pointer
    if (pointer_ != 0)  {
	if (count_.reference() == 1)
	    delete pointer_;
    }
    count_ = ((LgFrSortingPartScheduleFloatSmartPointer&)rhs).count_;
    pointer_ = ((LgFrSortingPartScheduleFloatSmartPointer&)rhs).pointer_;
    return *this;
}
*/

bool
LgFrSortingPartScheduleFloatSmartPointer::operator==(const LgFrSortingPartScheduleFloatSmartPointer& rhs) const
{
    if ( (pointer_ == rhs.pointer_) && (pointer_ == 0 || count_ == rhs.count_) )
	return true;
    else
	return false;
}
/*
bool
LgFrSortingPartScheduleFloatSmartPointer::operator==(LgFrSortingPartScheduleFloatSmartPointerToConst rhs) const
{
    if ( (pointer_ == rhs.pointer_) && (count_ == rhs.count_) )
	return true;
    else
	return false;
}
*/
bool
LgFrSortingPartScheduleFloatSmartPointer::operator!=(const LgFrSortingPartScheduleFloatSmartPointer& rhs) const
{
    return !( *this == rhs );
}
/*
bool
LgFrSortingPartScheduleFloatSmartPointer::operator!=(LgFrSortingPartScheduleFloatSmartPointerToConst rhs) const
{
    if ( (pointer_ != rhs.pointer_) || (count_ != rhs.count_) )
	return true;
    else
	return false;
}
*/
const LgFrSortingPartScheduleFloat* const
LgFrSortingPartScheduleFloatSmartPointer::operator->() const
{
    return pointer_;
}

LgFrSortingPartScheduleFloat* const
LgFrSortingPartScheduleFloatSmartPointer::operator->()
{
    return pointer_;
}

LgFrSortingPartScheduleFloat &
LgFrSortingPartScheduleFloatSmartPointer::operator*()
{
    return *pointer_;
}

const LgFrSortingPartScheduleFloat &
LgFrSortingPartScheduleFloatSmartPointer::operator*() const
{
    return *pointer_;
}

/*
LgFrSortingPartScheduleFloatSmartPointer::operator const LgFrSortingPartScheduleFloat* const () const
{
    return &(*pointer_);
}

LgFrSortingPartScheduleFloatSmartPointer::operator LgFrSortingPartScheduleFloat* ()
{
    return &(*pointer_);
}
*/


LgFrSortingPartScheduleFloatSmartPointer::operator LgFrSortingScheduleFloatSmartPointerToConst () const
{
    // need to cast away const for count_
    return LgFrSortingScheduleFloatSmartPointerToConst(pointer_, (LgFrReferenceCount&)count_);
}

LgFrSortingPartScheduleFloatSmartPointer::operator LgFrSortingScheduleFloatSmartPointer () 
{
    return LgFrSortingScheduleFloatSmartPointer(pointer_, count_);
}

bool
LgFrSortingPartScheduleFloatSmartPointer::unique() const
{
    return (count_.reference() == 1);
}

bool 
LgFrSortingPartScheduleFloatSmartPointer::null() const
{
    return (pointer_ == 0);
}


void
LgFrSortingPartScheduleFloatSmartPointer::test()
{
    std::cout << "Testing default constructor" << std::endl;
    LgFrSortingPartScheduleFloatSmartPointer dummy;
    std::cout << "dummy count is " << dummy.count_.reference() << std::endl;

    std::cout << "Testing preferred constructor (dummy)" << std::endl;
    LgFrSortingPartScheduleFloatSmartPointer b(new LgFrSortingPartScheduleFloat);
    std::cout << "b count is " << b.count_.reference() << std::endl;    

    std::cout << "Testing assignement operator dummy = b" << std::endl;
    dummy = b;
    std::cout << "b count is " << b.count_.reference() << std::endl;    


    std::cout << "Testing copy constructor (c = b)" << std::endl;
    LgFrSortingPartScheduleFloatSmartPointer c(b);
    std::cout << "b count is " << b.count_.reference() << std::endl;    


    std::cout << "Testing backdoor constructor" << std::endl;
    LgFrSortingPartScheduleFloatSmartPointer e(b.pointer_, b.count_);
    std::cout << "e count is " << e.count_.reference() << std::endl;    

    std::cout << "Testing equality operator (c == b)" << std::endl;
    assert(c == b);

    std::cout << "Testing inequality operator (c != d)" << std::endl;
    LgFrSortingPartScheduleFloatSmartPointer d(new LgFrSortingPartScheduleFloat);
    assert(c != d);

    std::cout << "Test destructor" << std::endl;
    c = d;
    std::cout << "b count is " << b.count_.reference() << std::endl;    
    c = b;
    d = b;
}

