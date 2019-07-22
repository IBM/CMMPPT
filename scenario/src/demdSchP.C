// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.
#include <ostream>
#include <assert.h>
#include <scenario/src/demdSchP.h>
#include <scenario/src/demdSchd.h>
#include <scenario/src/sortSchP.h>


// Default Constructor
LgFrSortingDemandScheduleFloatSmartPointerToConst::LgFrSortingDemandScheduleFloatSmartPointerToConst() : pointer_(new LgFrSortingDemandScheduleFloat), count_(1)
{
};

// Preferred Constructor
LgFrSortingDemandScheduleFloatSmartPointerToConst::LgFrSortingDemandScheduleFloatSmartPointerToConst(LgFrSortingDemandScheduleFloat* newedPointer) 
: pointer_(newedPointer), count_(1)
{
}

// Constructor for cast operator
LgFrSortingDemandScheduleFloatSmartPointerToConst::LgFrSortingDemandScheduleFloatSmartPointerToConst(
    LgFrSortingDemandScheduleFloat* existingPointer,
    LgFrReferenceCount& count) : pointer_(existingPointer)
{
    count_ = count;
}

// Destructor
LgFrSortingDemandScheduleFloatSmartPointerToConst::~LgFrSortingDemandScheduleFloatSmartPointerToConst()
{
    if (pointer_ != 0)  {
	if (count_.reference() == 1)
	    delete pointer_;
    }
}

// Copy Constructor
LgFrSortingDemandScheduleFloatSmartPointerToConst::LgFrSortingDemandScheduleFloatSmartPointerToConst(
    const LgFrSortingDemandScheduleFloatSmartPointerToConst& rhs) 
: pointer_(rhs.pointer_)
{
    count_ = // cast away const because of LgFrReferenceCount::operator=()
      ((LgFrSortingDemandScheduleFloatSmartPointerToConst&)rhs).count_;
}

// Deep Copy
LgFrSortingDemandScheduleFloatSmartPointerToConst
LgFrSortingDemandScheduleFloatSmartPointerToConst::deepCopy() const
{
    // do SortingDemandScheduleFloat deep copy and create a new smart pointer
    //  we can cast as we know that we are cloning a derived class
    return LgFrSortingDemandScheduleFloatSmartPointerToConst((LgFrSortingDemandScheduleFloat *)pointer_->clone());
}

// Assignment Operator
LgFrSortingDemandScheduleFloatSmartPointerToConst& 
LgFrSortingDemandScheduleFloatSmartPointerToConst::operator=(const LgFrSortingDemandScheduleFloatSmartPointerToConst& rhs)
{
    // check to see if this smart pointer is the only one pointing to
    //  the actual object.  If it is, then we want to delete the
    //  pointer
    if (pointer_ != 0)  {
	if (count_.reference() == 1)
	    delete pointer_;
    }
    count_ = // cast away const because of LgFrReferenceCount::operator=()
      ((LgFrSortingDemandScheduleFloatSmartPointerToConst&)rhs).count_;
    pointer_ = rhs.pointer_;
    return *this;
}


bool
LgFrSortingDemandScheduleFloatSmartPointerToConst::operator==(const LgFrSortingDemandScheduleFloatSmartPointerToConst& rhs) const
{
    if ( (pointer_ == rhs.pointer_) && (pointer_ == 0 || count_ == rhs.count_) )
	return true;
    else
	return false;
}

bool
LgFrSortingDemandScheduleFloatSmartPointerToConst::operator!=(const LgFrSortingDemandScheduleFloatSmartPointerToConst& rhs) const
{
    return !( *this == rhs );
}

const LgFrSortingDemandScheduleFloat* const
LgFrSortingDemandScheduleFloatSmartPointerToConst::operator->() const
{
    // implicit cast to const
    return pointer_;
}

const LgFrSortingDemandScheduleFloat&
LgFrSortingDemandScheduleFloatSmartPointerToConst::operator*() const
{
    // implicit cast to const
    return *pointer_;
}

/*
LgFrSortingDemandScheduleFloatSmartPointerToConst::operator const LgFrSortingDemandScheduleFloat* const () const
{
    return &(*pointer_);
}
*/


LgFrSortingDemandScheduleFloatSmartPointerToConst::operator LgFrSortingScheduleFloatSmartPointerToConst () const
{
    return	// need to cast away const for count_
      LgFrSortingScheduleFloatSmartPointerToConst(pointer_,
						  (LgFrReferenceCount&)count_);
}

bool
LgFrSortingDemandScheduleFloatSmartPointerToConst::unique() const
{
    return (count_.reference() == 1);
}

bool 
LgFrSortingDemandScheduleFloatSmartPointerToConst::null() const
{
    return (pointer_ == 0);
}

void
LgFrSortingDemandScheduleFloatSmartPointerToConst::test()
{
    std::cout << "Testing default constructor" << std::endl;
    LgFrSortingDemandScheduleFloatSmartPointerToConst dummy;
    std::cout << "dummy count is " << dummy.count_.reference() << std::endl;

    std::cout << "Testing preferred constructor (dummy)" << std::endl;
    LgFrSortingDemandScheduleFloatSmartPointerToConst b(new LgFrSortingDemandScheduleFloat);
    std::cout << "b count is " << b.count_.reference() << std::endl;    

    std::cout << "Testing assignement operator dummy = b" << std::endl;
    dummy = b;
    std::cout << "b count is " << b.count_.reference() << std::endl;    


    std::cout << "Testing copy constructor (c = b)" << std::endl;
    LgFrSortingDemandScheduleFloatSmartPointerToConst c(b);
    std::cout << "b count is " << b.count_.reference() << std::endl;    


    std::cout << "Testing backdoor constructor" << std::endl;
    LgFrSortingDemandScheduleFloatSmartPointerToConst e(b.pointer_, b.count_);
    std::cout << "e count is " << e.count_.reference() << std::endl;    

    std::cout << "Testing equality operator (c == b)" << std::endl;
    assert(c == b);

    std::cout << "Testing inequality operator (c != d)" << std::endl;
    LgFrSortingDemandScheduleFloatSmartPointerToConst d(new LgFrSortingDemandScheduleFloat);
    assert(c != d);

    std::cout << "Test destructor" << std::endl;
    c = d;
    std::cout << "b count is " << b.count_.reference() << std::endl;    
    c = b;
    d = b;

    // this should be a compiler error
//    LgFrSortingDemandScheduleFloatSmartPointerToConst e(&(*d));
}



LgFrSortingDemandScheduleFloatSmartPointer::LgFrSortingDemandScheduleFloatSmartPointer() 
{
    // calls base class default constructor
}

LgFrSortingDemandScheduleFloatSmartPointer::LgFrSortingDemandScheduleFloatSmartPointer(LgFrSortingDemandScheduleFloat* newedPointer) 
: LgFrSortingDemandScheduleFloatSmartPointerToConst(newedPointer)
{
}

LgFrSortingDemandScheduleFloatSmartPointer::LgFrSortingDemandScheduleFloatSmartPointer(
    LgFrSortingDemandScheduleFloat* existingPointer,
    LgFrReferenceCount& count) 
: LgFrSortingDemandScheduleFloatSmartPointerToConst(existingPointer, count)
{
}


LgFrSortingDemandScheduleFloatSmartPointer::~LgFrSortingDemandScheduleFloatSmartPointer()
{
    // call base class destructor
}

LgFrSortingDemandScheduleFloatSmartPointer::LgFrSortingDemandScheduleFloatSmartPointer
    (const LgFrSortingDemandScheduleFloatSmartPointer& rhs) 
: LgFrSortingDemandScheduleFloatSmartPointerToConst(rhs.pointer_)
{
    count_ = // cast away const because of LgFrReferenceCount::operator=()
      ((LgFrSortingDemandScheduleFloatSmartPointer&)rhs).count_;
}

// need to do some casting to a derived class so that we can access protected
//  members
LgFrSortingDemandScheduleFloatSmartPointer::LgFrSortingDemandScheduleFloatSmartPointer(LgFrSortingDemandScheduleFloatSmartPointerToConst rhs) 
    : LgFrSortingDemandScheduleFloatSmartPointerToConst(((LgFrSortingDemandScheduleFloatSmartPointer&)rhs).pointer_)
{
    count_ = ((LgFrSortingDemandScheduleFloatSmartPointer&)rhs).count_;
}

LgFrSortingDemandScheduleFloatSmartPointer
LgFrSortingDemandScheduleFloatSmartPointer::deepCopy() 
{
    // do SortingDemandScheduleFloat deep copy and create a new smart pointer
    return LgFrSortingDemandScheduleFloatSmartPointer((LgFrSortingDemandScheduleFloat *)pointer_->clone());
}

LgFrSortingDemandScheduleFloatSmartPointer& 
LgFrSortingDemandScheduleFloatSmartPointer::operator=(const LgFrSortingDemandScheduleFloatSmartPointer& rhs)
{
    // check to see if this smart pointer is the only one pointing to
    //  the actual object.  If it is, then we want to delete the
    //  pointer
    if (pointer_ != 0)  {
	if (count_.reference() == 1)
	    delete pointer_;
    }
    count_ = // cast away const because of LgFrReferenceCount::operator=()
      ((LgFrSortingDemandScheduleFloatSmartPointer&)rhs).count_;
    pointer_ = rhs.pointer_;
    return *this;
}

// need to do some casting to a derived class so that we can access protected
//  members
/*
LgFrSortingDemandScheduleFloatSmartPointer& 
LgFrSortingDemandScheduleFloatSmartPointer::operator=(LgFrSortingDemandScheduleFloatSmartPointerToConst rhs)
{
    // check to see if this smart pointer is the only one pointing to
    //  the actual object.  If it is, then we want to delete the
    //  pointer
    if (pointer_ != 0)  {
	if (count_.reference() == 1)
	    delete pointer_;
    }
    count_ = ((LgFrSortingDemandScheduleFloatSmartPointer&)rhs).count_;
    pointer_ = ((LgFrSortingDemandScheduleFloatSmartPointer&)rhs).pointer_;
    return *this;
}
*/

bool
LgFrSortingDemandScheduleFloatSmartPointer::operator==(const LgFrSortingDemandScheduleFloatSmartPointer& rhs) const
{
    if ( (pointer_ == rhs.pointer_) && (pointer_ == 0 || count_ == rhs.count_) )
	return true;
    else
	return false;
}
/*
bool
LgFrSortingDemandScheduleFloatSmartPointer::operator==(LgFrSortingDemandScheduleFloatSmartPointerToConst rhs) const
{
    if ( (pointer_ == rhs.pointer_) && (count_ == rhs.count_) )
	return true;
    else
	return false;
}
*/
bool
LgFrSortingDemandScheduleFloatSmartPointer::operator!=(const LgFrSortingDemandScheduleFloatSmartPointer& rhs) const
{
    return !( *this == rhs );
}
/*
bool
LgFrSortingDemandScheduleFloatSmartPointer::operator!=(LgFrSortingDemandScheduleFloatSmartPointerToConst rhs) const
{
    if ( (pointer_ != rhs.pointer_) || (count_ != rhs.count_) )
	return true;
    else
	return false;
}
*/
const LgFrSortingDemandScheduleFloat* const
LgFrSortingDemandScheduleFloatSmartPointer::operator->() const
{
    return pointer_;
}

LgFrSortingDemandScheduleFloat* const
LgFrSortingDemandScheduleFloatSmartPointer::operator->()
{
    return pointer_;
}

LgFrSortingDemandScheduleFloat &
LgFrSortingDemandScheduleFloatSmartPointer::operator*()
{
    return *pointer_;
}

const LgFrSortingDemandScheduleFloat &
LgFrSortingDemandScheduleFloatSmartPointer::operator*() const
{
    return *pointer_;
}

/*
LgFrSortingDemandScheduleFloatSmartPointer::operator const LgFrSortingDemandScheduleFloat* const () const
{
    return &(*pointer_);
}

LgFrSortingDemandScheduleFloatSmartPointer::operator LgFrSortingDemandScheduleFloat* ()
{
    return &(*pointer_);
}
*/


LgFrSortingDemandScheduleFloatSmartPointer::operator LgFrSortingScheduleFloatSmartPointerToConst () const
{
    return	// need to cast away const for count_
      LgFrSortingScheduleFloatSmartPointerToConst(pointer_,
						  (LgFrReferenceCount&)count_);
}

LgFrSortingDemandScheduleFloatSmartPointer::operator LgFrSortingScheduleFloatSmartPointer () 
{
    return LgFrSortingScheduleFloatSmartPointer(pointer_, count_);
}

bool
LgFrSortingDemandScheduleFloatSmartPointer::unique() const
{
    return (count_.reference() == 1);
}

bool 
LgFrSortingDemandScheduleFloatSmartPointer::null() const
{
    return (pointer_ == 0);
}


void
LgFrSortingDemandScheduleFloatSmartPointer::test()
{
    std::cout << "Testing default constructor" << std::endl;
    LgFrSortingDemandScheduleFloatSmartPointer dummy;
    std::cout << "dummy count is " << dummy.count_.reference() << std::endl;

    std::cout << "Testing preferred constructor (dummy)" << std::endl;
    LgFrSortingDemandScheduleFloatSmartPointer b(new LgFrSortingDemandScheduleFloat);
    std::cout << "b count is " << b.count_.reference() << std::endl;    

    std::cout << "Testing assignement operator dummy = b" << std::endl;
    dummy = b;
    std::cout << "b count is " << b.count_.reference() << std::endl;    


    std::cout << "Testing copy constructor (c = b)" << std::endl;
    LgFrSortingDemandScheduleFloatSmartPointer c(b);
    std::cout << "b count is " << b.count_.reference() << std::endl;    

    std::cout << "Testing backdoor constructor" << std::endl;
    LgFrSortingDemandScheduleFloatSmartPointer e(b.pointer_, b.count_);
    std::cout << "e count is " << e.count_.reference() << std::endl;    

    std::cout << "Testing equality operator (c == b)" << std::endl;
    assert(c == b);

    std::cout << "Testing inequality operator (c != d)" << std::endl;
    LgFrSortingDemandScheduleFloatSmartPointer d(new LgFrSortingDemandScheduleFloat);
    assert(c != d);

    std::cout << "Test destructor" << std::endl;
    c = d;
    std::cout << "b count is " << b.count_.reference() << std::endl;    
    c = b;
    d = b;
}

