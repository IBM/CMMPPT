
// RW2STL - inserted:
#include <ostream>
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL - inserted includes.
#include <iostream>
#include <assert.h>
#include <mcl/src/mcl.h>
#include <scenario/src/scenP.h>
#include <sce/src/scenSceP.h>
#include <sce/src/scenSce.h>
#include <sce/src/ipSce.h>
#include <sce/src/schFSce.h>

// Default Constructor
LgFrScenarioForSceSmartPointerToConst::LgFrScenarioForSceSmartPointerToConst() : pointer_(0)
{
};

// Preferred Constructor
LgFrScenarioForSceSmartPointerToConst::LgFrScenarioForSceSmartPointerToConst(LgFrScenarioForSce* newedPointer) 
: pointer_(newedPointer), count_(1)
{
}

// Constructor for cast operator
LgFrScenarioForSceSmartPointerToConst::LgFrScenarioForSceSmartPointerToConst(
    LgFrScenarioForSce* existingPointer,
    LgFrReferenceCount& count) : pointer_(existingPointer)
{
    count_ = count;
}

// Constructor 
LgFrScenarioForSceSmartPointerToConst::LgFrScenarioForSceSmartPointerToConst(
    LgFrScenarioSmartPointerToConst rhs)
{
    if (rhs->GetScenarioType() != __LGFRSCENARIOFORSCE)  {
      // cannot use the error msg facility here because it is not yet created 
	std::cerr << "Cannot construct a LgFrScenarioForSceSmartPointerToConst from a non LgFrScenarioForSce object" << std::endl;
	assert(0);
    }
    else  {
	pointer_ = (LgFrScenarioForSce*)(rhs.pointer_);
	count_ = rhs.count_;
    }
}

// Destructor
LgFrScenarioForSceSmartPointerToConst::~LgFrScenarioForSceSmartPointerToConst()
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
LgFrScenarioForSceSmartPointerToConst::LgFrScenarioForSceSmartPointerToConst(const LgFrScenarioForSceSmartPointerToConst& rhs) 
: pointer_(rhs.pointer_)
{
    count_ = ((LgFrScenarioForSceSmartPointerToConst&)rhs).count_;
}

// Deep Copy
LgFrScenarioForSceSmartPointerToConst
LgFrScenarioForSceSmartPointerToConst::deepCopy() const
{
    // do scenario deep copy and create a new smart pointer
    LgFrScenarioForSceSmartPointer temp(new LgFrScenarioForSce(*pointer_));

    // fix the ipp - since Scenario::initialProblemAndParameters() is non-const
    //  we need to cast away *pointer_
    temp->initialProblemAndParameters(((LgFrInitialProblemForSce&)(((LgFrScenarioForSce&)(*pointer_)).initialProblemAndParameters())).clone(temp));

    // fix the schedule factory - since Scenario::scheduleFactory() 
    // is non-const we need to cast away *pointer_
    temp->scheduleFactory(((LgFrScheduleFactoryForSce*)(((LgFrScenarioForSce&)(*pointer_)).scheduleFactory()))->clone(temp));

    return temp;
}

// Assignment Operator
LgFrScenarioForSceSmartPointerToConst& 
LgFrScenarioForSceSmartPointerToConst::operator=(const LgFrScenarioForSceSmartPointerToConst& rhs)
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
    count_ = ((LgFrScenarioForSceSmartPointerToConst&)rhs).count_;
    pointer_ = rhs.pointer_;
    return *this;
}


bool
LgFrScenarioForSceSmartPointerToConst::operator==(const LgFrScenarioForSceSmartPointerToConst& rhs) const
{
    if ( (pointer_ == rhs.pointer_) && (pointer_ == 0 || count_ == rhs.count_) )
	return true;
    else
	return false;
}

bool
LgFrScenarioForSceSmartPointerToConst::operator!=(const LgFrScenarioForSceSmartPointerToConst& rhs) const
{
    if ( (pointer_ != rhs.pointer_) || (count_ != rhs.count_) )
	return true;
    else
	return false;
}

const LgFrScenarioForSce* const
LgFrScenarioForSceSmartPointerToConst::operator->() const
{
    // implicit cast to const
    return pointer_;
}

const LgFrScenarioForSce&
LgFrScenarioForSceSmartPointerToConst::operator*() const
{
    // implicit cast to const
    return *pointer_;
}


bool
LgFrScenarioForSceSmartPointerToConst::unique() const
{
    return (count_.reference() == 1);
}

bool 
LgFrScenarioForSceSmartPointerToConst::null() const
{
    return (pointer_ == 0);
}

void
LgFrScenarioForSceSmartPointerToConst::test()
{
    std::cout << "Testing default constructor" << std::endl;
    LgFrScenarioForSceSmartPointerToConst dummy;
    assert(dummy.pointer_ == 0);
    std::cout << "dummy count is " << dummy.count_.reference() << std::endl;

    std::cout << "Testing preferred constructor (dummy)" << std::endl;
    LgFrScenarioForSceSmartPointerToConst b(new LgFrScenarioForSce);
    std::cout << "b count is " << b.count_.reference() << std::endl;    

    std::cout << "Testing assignement operator dummy = b" << std::endl;
    dummy = b;
    std::cout << "b count is " << b.count_.reference() << std::endl;    

    std::cout << "Testing copy constructor (c = b)" << std::endl;
    LgFrScenarioForSceSmartPointerToConst c(b);
    std::cout << "b count is " << b.count_.reference() << std::endl;    

    std::cout << "Testing backdoor constructor" << std::endl;
    LgFrScenarioForSceSmartPointerToConst e(b.pointer_, b.count_);
    std::cout << "e count is " << e.count_.reference() << std::endl;    

    std::cout << "Testing equality operator (c == b)" << std::endl;
    assert(c == b);

    std::cout << "Testing inequality operator (c != d)" << std::endl;
    LgFrScenarioForSceSmartPointerToConst d(new LgFrScenarioForSce);
    assert(c != d);

    std::cout << "Test destructor" << std::endl;
    c = d;
    std::cout << "b count is " << b.count_.reference() << std::endl;    
    c = b;
    d = b;

    // this should be a compiler error
//    LgFrScenarioForSceSmartPointerToConst e(&(*d));
}



LgFrScenarioForSceSmartPointer::LgFrScenarioForSceSmartPointer() 
{
    // calls base class default constructor
}

LgFrScenarioForSceSmartPointer::LgFrScenarioForSceSmartPointer(LgFrScenarioForSce* newedPointer) 
: LgFrScenarioForSceSmartPointerToConst(newedPointer)
{
}

LgFrScenarioForSceSmartPointer::LgFrScenarioForSceSmartPointer(
    LgFrScenarioForSce* existingPointer,
    LgFrReferenceCount& count) 
: LgFrScenarioForSceSmartPointerToConst(existingPointer, count)
{
}


LgFrScenarioForSceSmartPointer::~LgFrScenarioForSceSmartPointer()
{
    // call base class destructor
}

LgFrScenarioForSceSmartPointer::LgFrScenarioForSceSmartPointer(const LgFrScenarioForSceSmartPointer& rhs) 
: LgFrScenarioForSceSmartPointerToConst(rhs.pointer_)
{
    count_ = ((LgFrScenarioForSceSmartPointer&)rhs).count_;
}

// need to do some casting to a derived class so that we can access protected
//  members
LgFrScenarioForSceSmartPointer::LgFrScenarioForSceSmartPointer(LgFrScenarioForSceSmartPointerToConst rhs) 
    : LgFrScenarioForSceSmartPointerToConst(((LgFrScenarioForSceSmartPointer&)rhs).pointer_)
{
    count_ = ((LgFrScenarioForSceSmartPointer&)rhs).count_;
}

// Constructor 
LgFrScenarioForSceSmartPointer::LgFrScenarioForSceSmartPointer(
    LgFrScenarioSmartPointerToConst rhs)
{
    if (rhs->GetScenarioType() != __LGFRSCENARIOFORSCE)  {
	std::cerr << "Cannot construct a LgFrScenarioForSceSmartPointerToConst from a non LgFrScenarioForSce object" << std::endl;
	assert(0);
    }
    else  {
	// silly casting to derived class (SmartPointerToConst -> SmartPointer)
	//   so that we can access protected members pointer_, count_
	pointer_ = (LgFrScenarioForSce*)(((LgFrScenarioSmartPointer&)rhs).pointer_);
	count_ = ((LgFrScenarioSmartPointer&)rhs).count_;
    }
}

// Deep Copy
LgFrScenarioForSceSmartPointer
LgFrScenarioForSceSmartPointer::deepCopy()
{
    // do scenario deep copy and create a new smart pointer
    LgFrScenarioForSceSmartPointer temp(new LgFrScenarioForSce(*pointer_));

    // fix the ipp 
    temp->initialProblemAndParameters(((LgFrInitialProblemForSce&)(pointer_->initialProblemAndParameters())).clone(temp));

    // fix the schedule factory 
    temp->scheduleFactory(((LgFrScheduleFactoryForSce*)(pointer_->scheduleFactory()))->clone(temp));

    return temp;
}

LgFrScenarioForSceSmartPointer& 
LgFrScenarioForSceSmartPointer::operator=(const LgFrScenarioForSceSmartPointer& rhs)
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
    count_ = ((LgFrScenarioForSceSmartPointer&)rhs).count_;
    pointer_ = rhs.pointer_;
    return *this;
}

// need to do some casting to a derived class so that we can access protected
//  members
LgFrScenarioForSceSmartPointer& 
LgFrScenarioForSceSmartPointer::operator=(LgFrScenarioForSceSmartPointerToConst rhs)
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
    count_ = ((LgFrScenarioForSceSmartPointer&)rhs).count_;
    pointer_ = ((LgFrScenarioForSceSmartPointer&)rhs).pointer_;
    return *this;
}

bool
LgFrScenarioForSceSmartPointer::operator==(const LgFrScenarioForSceSmartPointer& rhs) const
{
    if ( (pointer_ == rhs.pointer_) && (pointer_ == 0 || count_ == rhs.count_) )
	return true;
    else
	return false;
}

bool
LgFrScenarioForSceSmartPointer::operator!=(const LgFrScenarioForSceSmartPointer& rhs) const
{
    return !( *this == rhs );
}

LgFrScenarioForSce* const
LgFrScenarioForSceSmartPointer::operator->()
{
    return pointer_;
}

const LgFrScenarioForSce* const
LgFrScenarioForSceSmartPointer::operator->() const
{
    return pointer_;
}

LgFrScenarioForSce &
LgFrScenarioForSceSmartPointer::operator*()
{
    return *pointer_;
}

const LgFrScenarioForSce &
LgFrScenarioForSceSmartPointer::operator*() const
{
    return *pointer_;
}


LgFrScenarioForSceSmartPointer::operator LgFrScenarioSmartPointerToConst () const
{
    // need to cast away const for count_
    return LgFrScenarioSmartPointerToConst(pointer_, (LgFrReferenceCount&)count_);
}

LgFrScenarioForSceSmartPointer::operator LgFrScenarioSmartPointer () 
{
    return LgFrScenarioSmartPointer(pointer_, count_);
}



bool
LgFrScenarioForSceSmartPointer::unique() const
{
    return (count_.reference() == 1);
}

bool 
LgFrScenarioForSceSmartPointer::null() const
{
    return (pointer_ == 0);
}

void
LgFrScenarioForSceSmartPointer::test()
{
    std::cout << "Testing default constructor" << std::endl;
    LgFrScenarioForSceSmartPointer dummy;
    assert(dummy.pointer_ == 0);
    std::cout << "dummy count is " << dummy.count_.reference() << std::endl;

    std::cout << "Testing preferred constructor (dummy)" << std::endl;
    LgFrScenarioForSceSmartPointer b(new LgFrScenarioForSce);
    std::cout << "b count is " << b.count_.reference() << std::endl;    

    std::cout << "Testing assignement operator dummy = b" << std::endl;
    dummy = b;
    std::cout << "b count is " << b.count_.reference() << std::endl;    

    std::cout << "Testing copy constructor (c = b)" << std::endl;
    LgFrScenarioForSceSmartPointer c(b);
    std::cout << "b count is " << b.count_.reference() << std::endl;    

    std::cout << "Testing backdoor constructor" << std::endl;
    LgFrScenarioForSceSmartPointer e(b.pointer_, b.count_);
    std::cout << "e count is " << e.count_.reference() << std::endl;    

    std::cout << "Testing equality operator (c == b)" << std::endl;
    assert(c == b);

    std::cout << "Testing inequality operator (c != d)" << std::endl;
    LgFrScenarioForSceSmartPointer d(new LgFrScenarioForSce);
    assert(c != d);

    std::cout << "Test destructor" << std::endl;
    c = d;
    std::cout << "b count is " << b.count_.reference() << std::endl;    
    c = b;
    d = b;
}
