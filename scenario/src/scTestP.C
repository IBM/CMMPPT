// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.
#include <iostream>
#include <assert.h>
#include <scenario/src/scenP.h>
#include <scenario/src/scTestP.h>
#include <scenario/src/testScen.h>
#include <scenario/src/inPrFile.h>
#include <scenario/src/schFctry.h>

// Default Constructor
LgFrTestScenarioSmartPointerToConst::LgFrTestScenarioSmartPointerToConst() : pointer_(0)
{
};

// Preferred Constructor
LgFrTestScenarioSmartPointerToConst::LgFrTestScenarioSmartPointerToConst(LgFrTestScenario* newedPointer) 
: pointer_(newedPointer), count_(1)
{
}

// Constructor for cast operator
LgFrTestScenarioSmartPointerToConst::LgFrTestScenarioSmartPointerToConst(
    LgFrTestScenario* existingPointer,
    LgFrReferenceCount& count) : pointer_(existingPointer)
{
    count_ = count;
}

// Constructor 
LgFrTestScenarioSmartPointerToConst::LgFrTestScenarioSmartPointerToConst(
    LgFrScenarioSmartPointerToConst rhs)
{
    if (rhs->GetScenarioType() != __LGFRTESTSCENARIO)  {
	std::cerr << "Cannot construct a LgFrTestScenarioSmartPointerToConst from a non LgFrTestScenario object" << std::endl;
	assert(0);
    }
    else  {
	pointer_ = (LgFrTestScenario*)(rhs.pointer_);
	count_ = rhs.count_;
    }
}

// Destructor
LgFrTestScenarioSmartPointerToConst::~LgFrTestScenarioSmartPointerToConst()
{
    // If a scenario smart pointer is fully constructed, there are
    //  3 references to the actual scenario. 
    //    1 from the routine that owns the last Smart Pointer
    //    1 from the scenario's Initial Problem and Parameters
    //    1 from the scenario's Schedule Factory
    //  but we need to check to make sure that a full scenario is
    //    built up!

    if ( (pointer_->ippPtr_) && (pointer_->sfPtr_) 
	 && (count_.reference() == 3) ) {
	LgFrScenarioSmartPointerToConst::deletingScenario_ = 1;
	delete pointer_;
	pointer_ = 0;
	LgFrScenarioSmartPointerToConst::deletingScenario_ = 0;
    }
    else if ( (count_.reference() == 2) && 
	      ( (pointer_->ippPtr_) || (pointer_->sfPtr_) )  ) {
	LgFrScenarioSmartPointerToConst::deletingScenario_ = 1;
	delete pointer_;
	pointer_ = 0;
	LgFrScenarioSmartPointerToConst::deletingScenario_ = 0;
    }
    else if (count_.reference() == 1) {
	LgFrScenarioSmartPointerToConst::deletingScenario_ = 1;
	delete pointer_;
	pointer_ = 0;
	LgFrScenarioSmartPointerToConst::deletingScenario_ = 0;
    }
}

// Copy Constructor
LgFrTestScenarioSmartPointerToConst::LgFrTestScenarioSmartPointerToConst(const LgFrTestScenarioSmartPointerToConst& rhs) 
: pointer_(rhs.pointer_)
{
    count_ = ((LgFrTestScenarioSmartPointerToConst&)rhs).count_;
}

// Deep Copy
LgFrTestScenarioSmartPointerToConst
LgFrTestScenarioSmartPointerToConst::deepCopy() const
{
    // do scenario deep copy and create a new smart pointer
    LgFrTestScenarioSmartPointer temp(new LgFrTestScenario(*pointer_));

    // fix the ipp - since Scenario::initialProblemAndParameters() is non-const
    //  we need to cast away *pointer_
    temp->initialProblemAndParameters(((LgFrInitialProblemFromFile&)(((LgFrTestScenario&)(*pointer_)).initialProblemAndParameters())).clone(temp));

    // fix the schedule factory - since Scenario::scheduleFactory() 
    // is non-const we need to cast away *pointer_
    temp->scheduleFactory(((LgFrScheduleFactory*)(((LgFrTestScenario&)(*pointer_)).scheduleFactory()))->clone(temp));

    return temp;
}

// Assignment Operator
LgFrTestScenarioSmartPointerToConst& 
LgFrTestScenarioSmartPointerToConst::operator=(const LgFrTestScenarioSmartPointerToConst& rhs)
{
    // If a scenario smart pointer is fully constructed, there are
    //  3 references to the actual scenario. 
    //    1 from the routine that owns the last Smart Pointer
    //    1 from the scenario's Initial Problem and Parameters
    //    1 from the scenario's Schedule Factory
    //  but we need to check to make sure that a full scenario is
    //    built up!

    if (pointer_ != 0)  {
	if ( (pointer_->ippPtr_) && (pointer_->sfPtr_) 
	     && (count_.reference() == 3) ) {
	    LgFrScenarioSmartPointerToConst::deletingScenario_ = 1;
	    std::cout << "deleting LgFrTestScenario (count_.reference() == 3)" << std::endl;
	    delete pointer_;
	    std::cout << "deleted LgFrTestScenario" << std::endl;
	    pointer_ = 0;
	    LgFrScenarioSmartPointerToConst::deletingScenario_ = 0;
	}
	else if ( (count_.reference() == 2) && 
		  ( (pointer_->ippPtr_) || (pointer_->sfPtr_) )  ) {
	    LgFrScenarioSmartPointerToConst::deletingScenario_ = 1;
	    std::cout << "deleting LgFrTestScenario (count_.reference() == 2)" << std::endl;
	    delete pointer_;
	    std::cout << "deleted LgFrTestScenario" << std::endl;
	    pointer_ = 0;
	    LgFrScenarioSmartPointerToConst::deletingScenario_ = 0;
	}
	else if (count_.reference() == 1) {
	    LgFrScenarioSmartPointerToConst::deletingScenario_ = 1;
	    std::cout << "deleting LgFrTestScenario (count_.reference() == 1)" << std::endl;
	    delete pointer_;
	    std::cout << "deleted LgFrTestScenario" << std::endl;
	    pointer_ = 0;
	    LgFrScenarioSmartPointerToConst::deletingScenario_ = 0;
	}
    }
    count_ = ((LgFrTestScenarioSmartPointerToConst&)rhs).count_;
    pointer_ = rhs.pointer_;
    return *this;
}


bool
LgFrTestScenarioSmartPointerToConst::operator==(const LgFrTestScenarioSmartPointerToConst& rhs) const
{
    if ( (pointer_ == rhs.pointer_) && (pointer_ == 0 || count_ == rhs.count_) )
	return true;
    else
	return false;
}

bool
LgFrTestScenarioSmartPointerToConst::operator!=(const LgFrTestScenarioSmartPointerToConst& rhs) const
{
    return !( *this == rhs );
}

const LgFrTestScenario* const
LgFrTestScenarioSmartPointerToConst::operator->() const
{
    // implicit cast to const
    return pointer_;
}

const LgFrTestScenario&
LgFrTestScenarioSmartPointerToConst::operator*() const
{
    // implicit cast to const
    return *pointer_;
}


LgFrTestScenarioSmartPointerToConst::operator LgFrScenarioSmartPointerToConst () const
{
    // need to cast away const for count_
    return LgFrScenarioSmartPointerToConst(pointer_, (LgFrReferenceCount&)count_);
}

bool
LgFrTestScenarioSmartPointerToConst::unique() const
{
    return (count_.reference() == 1);
}

bool 
LgFrTestScenarioSmartPointerToConst::null() const
{
    return (pointer_ == 0);
}

void
LgFrTestScenarioSmartPointerToConst::test()
{
    std::cout << "Testing default constructor" << std::endl;
    LgFrTestScenarioSmartPointerToConst dummy;
    assert(dummy.pointer_ == 0);
    std::cout << "dummy count is " << dummy.count_.reference() << std::endl;

    std::cout << "Testing preferred constructor (dummy)" << std::endl;
    LgFrTestScenarioSmartPointerToConst b(new LgFrTestScenario);
    std::cout << "b count is " << b.count_.reference() << std::endl;    

    std::cout << "Testing assignement operator dummy = b" << std::endl;
    dummy = b;
    std::cout << "b count is " << b.count_.reference() << std::endl;    

    std::cout << "Testing copy constructor (c = b)" << std::endl;
    LgFrTestScenarioSmartPointerToConst c(b);
    std::cout << "b count is " << b.count_.reference() << std::endl;    

    std::cout << "Testing backdoor constructor" << std::endl;
    LgFrTestScenarioSmartPointerToConst e(b.pointer_, b.count_);
    std::cout << "e count is " << e.count_.reference() << std::endl;    

    std::cout << "Testing equality operator (c == b)" << std::endl;
    assert(c == b);

    std::cout << "Testing inequality operator (c != d)" << std::endl;
    LgFrTestScenarioSmartPointerToConst d(new LgFrTestScenario);
    assert(c != d);

    std::cout << "Test destructor" << std::endl;
    c = d;
    std::cout << "b count is " << b.count_.reference() << std::endl;    
    c = b;
    d = b;

    // this should be a compiler error
//    LgFrTestScenarioSmartPointerToConst e(&(*d));
}



LgFrTestScenarioSmartPointer::LgFrTestScenarioSmartPointer() 
{
    // calls base class default constructor
}

LgFrTestScenarioSmartPointer::LgFrTestScenarioSmartPointer(LgFrTestScenario* newedPointer) 
: LgFrTestScenarioSmartPointerToConst(newedPointer)
{
}

LgFrTestScenarioSmartPointer::LgFrTestScenarioSmartPointer(
    LgFrTestScenario* existingPointer,
    LgFrReferenceCount& count) 
: LgFrTestScenarioSmartPointerToConst(existingPointer, count)
{
}


LgFrTestScenarioSmartPointer::~LgFrTestScenarioSmartPointer()
{
    // call base class destructor
}

LgFrTestScenarioSmartPointer::LgFrTestScenarioSmartPointer(const LgFrTestScenarioSmartPointer& rhs) 
: LgFrTestScenarioSmartPointerToConst(rhs.pointer_)
{
    count_ = ((LgFrTestScenarioSmartPointer&)rhs).count_;
}

// need to do some casting to a derived class so that we can access protected
//  members
LgFrTestScenarioSmartPointer::LgFrTestScenarioSmartPointer(LgFrTestScenarioSmartPointerToConst rhs) 
    : LgFrTestScenarioSmartPointerToConst(((LgFrTestScenarioSmartPointer&)rhs).pointer_)
{
    count_ = ((LgFrTestScenarioSmartPointer&)rhs).count_;
}

// Constructor 
LgFrTestScenarioSmartPointer::LgFrTestScenarioSmartPointer(
    LgFrScenarioSmartPointerToConst rhs)
{
    if (rhs->GetScenarioType() != __LGFRTESTSCENARIO)  {
	std::cerr << "Cannot construct a LgFrTestScenarioSmartPointerToConst from a non LgFrTestScenario object" << std::endl;
	assert(0);
    }
    else  {
	// silly casting to derived class (SmartPointerToConst -> SmartPointer)
	//   so that we can access protected members pointer_, count_
	pointer_ = (LgFrTestScenario*)(((LgFrScenarioSmartPointer&)rhs).pointer_);
	count_ = ((LgFrScenarioSmartPointer&)rhs).count_;
    }
}

// Deep Copy
LgFrTestScenarioSmartPointer
LgFrTestScenarioSmartPointer::deepCopy()
{
    // do scenario deep copy and create a new smart pointer
    LgFrTestScenarioSmartPointer temp(new LgFrTestScenario(*pointer_));

    // fix the ipp 
    temp->initialProblemAndParameters(((LgFrInitialProblemFromFile&)(pointer_->initialProblemAndParameters())).clone(temp));

    // fix the schedule factory 
    temp->scheduleFactory(((LgFrScheduleFactory*)(pointer_->scheduleFactory()))->clone(temp));

    return temp;
}

LgFrTestScenarioSmartPointer& 
LgFrTestScenarioSmartPointer::operator=(const LgFrTestScenarioSmartPointer& rhs)
{
    // If a scenario smart pointer is fully constructed, there are
    //  3 references to the actual scenario. 
    //    1 from the routine that owns the last Smart Pointer
    //    1 from the scenario's Initial Problem and Parameters
    //    1 from the scenario's Schedule Factory
    //  but we need to check to make sure that a full scenario is
    //    built up!

    if (pointer_ != 0)  {
	if ( (pointer_->ippPtr_) && (pointer_->sfPtr_) 
	     && (count_.reference() == 3) ) {
	    LgFrScenarioSmartPointerToConst::deletingScenario_ = 1;
	    std::cout << "deleting LgFrTestScenario (count_.reference() == 3)" << std::endl;
	    delete pointer_;
	    std::cout << "deleted LgFrTestScenario" << std::endl;
	    pointer_ = 0;
	    LgFrScenarioSmartPointerToConst::deletingScenario_ = 0;
	}
	else if ( (count_.reference() == 2) && 
		  ( (pointer_->ippPtr_) || (pointer_->sfPtr_) )  ) {
	    LgFrScenarioSmartPointerToConst::deletingScenario_ = 1;
	    std::cout << "deleting LgFrTestScenario (count_.reference() == 2)" << std::endl;
	    delete pointer_;
	    std::cout << "deleted LgFrTestScenario" << std::endl;
	    pointer_ = 0;
	    LgFrScenarioSmartPointerToConst::deletingScenario_ = 0;
	}
	else if (count_.reference() == 1) {
	    LgFrScenarioSmartPointerToConst::deletingScenario_ = 1;
	    std::cout << "deleting LgFrTestScenario (count_.reference() == 1)" << std::endl;
	    delete pointer_;
	    std::cout << "deleted LgFrTestScenario" << std::endl;
	    pointer_ = 0;
	    LgFrScenarioSmartPointerToConst::deletingScenario_ = 0;
	}
    }
    count_ = ((LgFrTestScenarioSmartPointer&)rhs).count_;
    pointer_ = rhs.pointer_;
    return *this;
}

// need to do some casting to a derived class so that we can access protected
//  members
LgFrTestScenarioSmartPointer& 
LgFrTestScenarioSmartPointer::operator=(LgFrTestScenarioSmartPointerToConst rhs)
{
    // If a scenario smart pointer is fully constructed, there are
    //  3 references to the actual scenario. 
    //    1 from the routine that owns the last Smart Pointer
    //    1 from the scenario's Initial Problem and Parameters
    //    1 from the scenario's Schedule Factory
    //  but we need to check to make sure that a full scenario is
    //    built up!

    if (pointer_ != 0)  {
	if ( (pointer_->ippPtr_) && (pointer_->sfPtr_) 
	     && (count_.reference() == 3) ) {
	    LgFrScenarioSmartPointerToConst::deletingScenario_ = 1;
	    std::cout << "deleting LgFrTestScenario (count_.reference() == 3)" << std::endl;
	    delete pointer_;
	    std::cout << "deleted LgFrTestScenario" << std::endl;
	    pointer_ = 0;
	    LgFrScenarioSmartPointerToConst::deletingScenario_ = 0;
	}
	else if ( (count_.reference() == 2) && 
		  ( (pointer_->ippPtr_) || (pointer_->sfPtr_) )  ) {
	    LgFrScenarioSmartPointerToConst::deletingScenario_ = 1;
	    std::cout << "deleting LgFrTestScenario (count_.reference() == 2)" << std::endl;
	    delete pointer_;
	    std::cout << "deleted LgFrTestScenario" << std::endl;
	    pointer_ = 0;
	    LgFrScenarioSmartPointerToConst::deletingScenario_ = 0;
	}
	else if (count_.reference() == 1) {
	    LgFrScenarioSmartPointerToConst::deletingScenario_ = 1;
	    std::cout << "deleting LgFrTestScenario (count_.reference() == 1)" << std::endl;
	    delete pointer_;
	    std::cout << "deleted LgFrTestScenario" << std::endl;
	    pointer_ = 0;
	    LgFrScenarioSmartPointerToConst::deletingScenario_ = 0;
	}
    }
    count_ = ((LgFrTestScenarioSmartPointer&)rhs).count_;
    pointer_ = ((LgFrTestScenarioSmartPointer&)rhs).pointer_;
    return *this;
}

bool
LgFrTestScenarioSmartPointer::operator==(const LgFrTestScenarioSmartPointer& rhs) const
{
    if ( (pointer_ == rhs.pointer_) && (pointer_ == 0 || count_ == rhs.count_) )
	return true;
    else
	return false;
}

bool
LgFrTestScenarioSmartPointer::operator!=(const LgFrTestScenarioSmartPointer& rhs) const
{
    return !( *this == rhs );
}

LgFrTestScenario* const
LgFrTestScenarioSmartPointer::operator->()
{
    return pointer_;
}

const LgFrTestScenario* const
LgFrTestScenarioSmartPointer::operator->() const
{
    return pointer_;
}

LgFrTestScenario &
LgFrTestScenarioSmartPointer::operator*()
{
    return *pointer_;
}

const LgFrTestScenario &
LgFrTestScenarioSmartPointer::operator*() const
{
    return *pointer_;
}


LgFrTestScenarioSmartPointer::operator LgFrScenarioSmartPointerToConst () const
{
    // need to cast away const for count_
    return LgFrScenarioSmartPointerToConst(pointer_, (LgFrReferenceCount&)count_);
}

LgFrTestScenarioSmartPointer::operator LgFrScenarioSmartPointer () 
{
    return LgFrScenarioSmartPointer(pointer_, count_);
}



bool
LgFrTestScenarioSmartPointer::unique() const
{
    return (count_.reference() == 1);
}

bool 
LgFrTestScenarioSmartPointer::null() const
{
    return (pointer_ == 0);
}

void
LgFrTestScenarioSmartPointer::test()
{
    std::cout << "Testing default constructor" << std::endl;
    LgFrTestScenarioSmartPointer dummy;
    assert(dummy.pointer_ == 0);
    std::cout << "dummy count is " << dummy.count_.reference() << std::endl;

    std::cout << "Testing preferred constructor (dummy)" << std::endl;
    LgFrTestScenarioSmartPointer b(new LgFrTestScenario);
    std::cout << "b count is " << b.count_.reference() << std::endl;    

    std::cout << "Testing assignement operator dummy = b" << std::endl;
    dummy = b;
    std::cout << "b count is " << b.count_.reference() << std::endl;    

    std::cout << "Testing copy constructor (c = b)" << std::endl;
    LgFrTestScenarioSmartPointer c(b);
    std::cout << "b count is " << b.count_.reference() << std::endl;    

    std::cout << "Testing backdoor constructor" << std::endl;
    LgFrTestScenarioSmartPointer e(b.pointer_, b.count_);
    std::cout << "e count is " << e.count_.reference() << std::endl;    

    std::cout << "Testing equality operator (c == b)" << std::endl;
    assert(c == b);

    std::cout << "Testing inequality operator (c != d)" << std::endl;
    LgFrTestScenarioSmartPointer d(new LgFrTestScenario);
    assert(c != d);

    std::cout << "Test destructor" << std::endl;
    c = d;
    std::cout << "b count is " << b.count_.reference() << std::endl;    
    c = b;
    d = b;
}
