// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.
#include <iostream>
#include <assert.h>
#include <scenario/src/scenP.h>
#include <scenario/src/scCMRPP.h>
#include <scenario/src/scenCMRP.h>
#include <scenario/src/ipCMRP.h>
#include <scenario/src/schFCMRP.h>

// Default Constructor
LgFrScenarioForCMRPSmartPointerToConst::LgFrScenarioForCMRPSmartPointerToConst() : pointer_(0)
{
};

// Preferred Constructor
LgFrScenarioForCMRPSmartPointerToConst::LgFrScenarioForCMRPSmartPointerToConst(LgFrScenarioForCMRP* newedPointer) 
: pointer_(newedPointer), count_(1)
{
}

// Constructor for cast operator
LgFrScenarioForCMRPSmartPointerToConst::LgFrScenarioForCMRPSmartPointerToConst(
    LgFrScenarioForCMRP* existingPointer,
    LgFrReferenceCount& count) : pointer_(existingPointer)
{
    count_ = count;
}

// Constructor 
LgFrScenarioForCMRPSmartPointerToConst::LgFrScenarioForCMRPSmartPointerToConst(
    LgFrScenarioSmartPointerToConst rhs)
{
    if (rhs->GetScenarioType() != __LGFRSCENARIOFORCMRP)  {
	std::cerr << "Cannot construct a LgFrScenarioForCMRPSmartPointerToConst from a non LgFrScenarioForCMRP object" << std::endl;
	assert(0);
    }
    else  {
	pointer_ = (LgFrScenarioForCMRP*)(rhs.pointer_);
	count_ = rhs.count_;
    }
}

// Destructor
LgFrScenarioForCMRPSmartPointerToConst::~LgFrScenarioForCMRPSmartPointerToConst()
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
LgFrScenarioForCMRPSmartPointerToConst::LgFrScenarioForCMRPSmartPointerToConst(const LgFrScenarioForCMRPSmartPointerToConst& rhs) 
: pointer_(rhs.pointer_)
{
    count_ = ((LgFrScenarioForCMRPSmartPointerToConst&)rhs).count_;
}

// Deep Copy
LgFrScenarioForCMRPSmartPointerToConst
LgFrScenarioForCMRPSmartPointerToConst::deepCopy() const
{
    // do scenario deep copy and create a new smart pointer
    LgFrScenarioForCMRPSmartPointer temp(new LgFrScenarioForCMRP(*pointer_));

    // fix the ipp - since Scenario::initialProblemAndParameters() is non-const
    //  we need to cast away *pointer_
    temp->initialProblemAndParameters(((LgFrInitialProblemForCMRP&)(((LgFrScenarioForCMRP&)(*pointer_)).initialProblemAndParameters())).clone(temp));

    // fix the schedule factory - since Scenario::scheduleFactory() 
    // is non-const we need to cast away *pointer_
    temp->scheduleFactory(((LgFrScheduleFactoryForCMRP*)(((LgFrScenarioForCMRP&)(*pointer_)).scheduleFactory()))->clone(temp));

    return temp;
}

// Assignment Operator
LgFrScenarioForCMRPSmartPointerToConst& 
LgFrScenarioForCMRPSmartPointerToConst::operator=(const LgFrScenarioForCMRPSmartPointerToConst& rhs)
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
    count_ = ((LgFrScenarioForCMRPSmartPointerToConst&)rhs).count_;
    pointer_ = rhs.pointer_;
    return *this;
}


bool
LgFrScenarioForCMRPSmartPointerToConst::operator==(const LgFrScenarioForCMRPSmartPointerToConst& rhs) const
{
    if ( (pointer_ == rhs.pointer_) && (pointer_ == 0 || count_ == rhs.count_) )
	return true;
    else
	return false;
}

bool
LgFrScenarioForCMRPSmartPointerToConst::operator!=(const LgFrScenarioForCMRPSmartPointerToConst& rhs) const
{
    return !( *this == rhs );
}

const LgFrScenarioForCMRP* const
LgFrScenarioForCMRPSmartPointerToConst::operator->() const
{
    // implicit cast to const
    return pointer_;
}

const LgFrScenarioForCMRP&
LgFrScenarioForCMRPSmartPointerToConst::operator*() const
{
    // implicit cast to const
    return *pointer_;
}


bool
LgFrScenarioForCMRPSmartPointerToConst::unique() const
{
    return (count_.reference() == 1);
}

bool 
LgFrScenarioForCMRPSmartPointerToConst::null() const
{
    return (pointer_ == 0);
}

void
LgFrScenarioForCMRPSmartPointerToConst::test()
{
    std::cout << "Testing default constructor" << std::endl;
    LgFrScenarioForCMRPSmartPointerToConst dummy;
    assert(dummy.pointer_ == 0);
    std::cout << "dummy count is " << dummy.count_.reference() << std::endl;

    std::cout << "Testing preferred constructor (dummy)" << std::endl;
    LgFrScenarioForCMRPSmartPointerToConst b(new LgFrScenarioForCMRP);
    std::cout << "b count is " << b.count_.reference() << std::endl;    

    std::cout << "Testing assignement operator dummy = b" << std::endl;
    dummy = b;
    std::cout << "b count is " << b.count_.reference() << std::endl;    

    std::cout << "Testing copy constructor (c = b)" << std::endl;
    LgFrScenarioForCMRPSmartPointerToConst c(b);
    std::cout << "b count is " << b.count_.reference() << std::endl;    

    std::cout << "Testing backdoor constructor" << std::endl;
    LgFrScenarioForCMRPSmartPointerToConst e(b.pointer_, b.count_);
    std::cout << "e count is " << e.count_.reference() << std::endl;    

    std::cout << "Testing equality operator (c == b)" << std::endl;
    assert(c == b);

    std::cout << "Testing inequality operator (c != d)" << std::endl;
    LgFrScenarioForCMRPSmartPointerToConst d(new LgFrScenarioForCMRP);
    assert(c != d);

    std::cout << "Test destructor" << std::endl;
    c = d;
    std::cout << "b count is " << b.count_.reference() << std::endl;    
    c = b;
    d = b;

    // this should be a compiler error
//    LgFrScenarioForCMRPSmartPointerToConst e(&(*d));
}



LgFrScenarioForCMRPSmartPointer::LgFrScenarioForCMRPSmartPointer() 
{
    // calls base class default constructor
}

LgFrScenarioForCMRPSmartPointer::LgFrScenarioForCMRPSmartPointer(LgFrScenarioForCMRP* newedPointer) 
: LgFrScenarioForCMRPSmartPointerToConst(newedPointer)
{
}

LgFrScenarioForCMRPSmartPointer::LgFrScenarioForCMRPSmartPointer(
    LgFrScenarioForCMRP* existingPointer,
    LgFrReferenceCount& count) 
: LgFrScenarioForCMRPSmartPointerToConst(existingPointer, count)
{
}


LgFrScenarioForCMRPSmartPointer::~LgFrScenarioForCMRPSmartPointer()
{
    // call base class destructor
}

LgFrScenarioForCMRPSmartPointer::LgFrScenarioForCMRPSmartPointer(const LgFrScenarioForCMRPSmartPointer& rhs) 
: LgFrScenarioForCMRPSmartPointerToConst(rhs.pointer_)
{
    count_ = ((LgFrScenarioForCMRPSmartPointer&)rhs).count_;
}

// need to do some casting to a derived class so that we can access protected
//  members
LgFrScenarioForCMRPSmartPointer::LgFrScenarioForCMRPSmartPointer(LgFrScenarioForCMRPSmartPointerToConst rhs) 
    : LgFrScenarioForCMRPSmartPointerToConst(((LgFrScenarioForCMRPSmartPointer&)rhs).pointer_)
{
    count_ = ((LgFrScenarioForCMRPSmartPointer&)rhs).count_;
}

// Constructor 
LgFrScenarioForCMRPSmartPointer::LgFrScenarioForCMRPSmartPointer(
    LgFrScenarioSmartPointerToConst rhs)
{
    if (rhs->GetScenarioType() != __LGFRSCENARIOFORCMRP)  {
	std::cerr << "Cannot construct a LgFrScenarioForCMRPSmartPointerToConst from a non LgFrScenarioForCMRP object" << std::endl;
	assert(0);
    }
    else  {
	// silly casting to derived class (SmartPointerToConst -> SmartPointer)
	//   so that we can access protected members pointer_, count_
	pointer_ = (LgFrScenarioForCMRP*)(((LgFrScenarioSmartPointer&)rhs).pointer_);
	count_ = ((LgFrScenarioSmartPointer&)rhs).count_;
    }
}

// Deep Copy
LgFrScenarioForCMRPSmartPointer
LgFrScenarioForCMRPSmartPointer::deepCopy()
{
    // do scenario deep copy and create a new smart pointer
    LgFrScenarioForCMRPSmartPointer temp(new LgFrScenarioForCMRP(*pointer_));

    // fix the ipp 
    temp->initialProblemAndParameters(((LgFrInitialProblemForCMRP&)(pointer_->initialProblemAndParameters())).clone(temp));

    // fix the schedule factory 
    temp->scheduleFactory(((LgFrScheduleFactoryForCMRP*)(pointer_->scheduleFactory()))->clone(temp));

    return temp;
}

LgFrScenarioForCMRPSmartPointer& 
LgFrScenarioForCMRPSmartPointer::operator=(const LgFrScenarioForCMRPSmartPointer& rhs)
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
    count_ = ((LgFrScenarioForCMRPSmartPointer&)rhs).count_;
    pointer_ = rhs.pointer_;
    return *this;
}

// need to do some casting to a derived class so that we can access protected
//  members
LgFrScenarioForCMRPSmartPointer& 
LgFrScenarioForCMRPSmartPointer::operator=(LgFrScenarioForCMRPSmartPointerToConst rhs)
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
    count_ = ((LgFrScenarioForCMRPSmartPointer&)rhs).count_;
    pointer_ = ((LgFrScenarioForCMRPSmartPointer&)rhs).pointer_;
    return *this;
}

bool
LgFrScenarioForCMRPSmartPointer::operator==(const LgFrScenarioForCMRPSmartPointer& rhs) const
{
    if ( (pointer_ == rhs.pointer_) && (pointer_ == 0 || count_ == rhs.count_) )
	return true;
    else
	return false;
}

bool
LgFrScenarioForCMRPSmartPointer::operator!=(const LgFrScenarioForCMRPSmartPointer& rhs) const
{
    return !( *this == rhs );
}

LgFrScenarioForCMRP* const
LgFrScenarioForCMRPSmartPointer::operator->()
{
    return pointer_;
}

const LgFrScenarioForCMRP* const
LgFrScenarioForCMRPSmartPointer::operator->() const
{
    return pointer_;
}

LgFrScenarioForCMRP &
LgFrScenarioForCMRPSmartPointer::operator*()
{
    return *pointer_;
}

const LgFrScenarioForCMRP &
LgFrScenarioForCMRPSmartPointer::operator*() const
{
    return *pointer_;
}


LgFrScenarioForCMRPSmartPointer::operator LgFrScenarioSmartPointerToConst () const
{
    // need to cast away const for count_
    return LgFrScenarioSmartPointerToConst(pointer_, (LgFrReferenceCount&)count_);
}

LgFrScenarioForCMRPSmartPointer::operator LgFrScenarioSmartPointer () 
{
    return LgFrScenarioSmartPointer(pointer_, count_);
}



bool
LgFrScenarioForCMRPSmartPointer::unique() const
{
    return (count_.reference() == 1);
}

bool 
LgFrScenarioForCMRPSmartPointer::null() const
{
    return (pointer_ == 0);
}

void
LgFrScenarioForCMRPSmartPointer::test()
{
    std::cout << "Testing default constructor" << std::endl;
    LgFrScenarioForCMRPSmartPointer dummy;
    assert(dummy.pointer_ == 0);
    std::cout << "dummy count is " << dummy.count_.reference() << std::endl;

    std::cout << "Testing preferred constructor (dummy)" << std::endl;
    LgFrScenarioForCMRPSmartPointer b(new LgFrScenarioForCMRP);
    std::cout << "b count is " << b.count_.reference() << std::endl;    

    std::cout << "Testing assignement operator dummy = b" << std::endl;
    dummy = b;
    std::cout << "b count is " << b.count_.reference() << std::endl;    

    std::cout << "Testing copy constructor (c = b)" << std::endl;
    LgFrScenarioForCMRPSmartPointer c(b);
    std::cout << "b count is " << b.count_.reference() << std::endl;    

    std::cout << "Testing backdoor constructor" << std::endl;
    LgFrScenarioForCMRPSmartPointer e(b.pointer_, b.count_);
    std::cout << "e count is " << e.count_.reference() << std::endl;    

    std::cout << "Testing equality operator (c == b)" << std::endl;
    assert(c == b);

    std::cout << "Testing inequality operator (c != d)" << std::endl;
    LgFrScenarioForCMRPSmartPointer d(new LgFrScenarioForCMRP);
    assert(c != d);

    std::cout << "Test destructor" << std::endl;
    c = d;
    std::cout << "b count is " << b.count_.reference() << std::endl;    
    c = b;
    d = b;
}
