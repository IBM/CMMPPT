// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.
#include <iostream>
#include <assert.h>
#include <scenario/src/scenP.h>
#include <scenario/src/scenario.h>
#include <scenario/src/schFctry.h>


// initialize
const LgFrScenarioSmartPointer 
      LgFrScenarioSmartPointer::nullScenarioSmartPointer = 0;
int LgFrScenarioSmartPointerToConst::deletingScenario_ = 0;

// Default Constructor
LgFrScenarioSmartPointerToConst::LgFrScenarioSmartPointerToConst() : pointer_(0)
{
};

// Preferred Constructor
LgFrScenarioSmartPointerToConst::LgFrScenarioSmartPointerToConst(LgFrScenario* newedPointer) 
: pointer_(newedPointer), count_(1)
{
}

// Constructor for cast operator
LgFrScenarioSmartPointerToConst::LgFrScenarioSmartPointerToConst(
    LgFrScenario* existingPointer,
    LgFrReferenceCount& count) : pointer_(existingPointer)
{
    count_ = count;
}

// Destructor
LgFrScenarioSmartPointerToConst::~LgFrScenarioSmartPointerToConst()
{
    // If a scenario smart pointer is fully constructed, there are
    //  3 references to the actual scenario. 
    //    1 from the routine that owns the last Smart Pointer
    //    1 from the scenario's Initial Problem and Parameters
    //    1 from the scenario's Schedule Factory
    //  but we need to check to make sure that a full scenario is
    //    built up!

    if (pointer_ != 0)  {
	if ( (!deletingScenario_) && (pointer_->ippPtr_) && (pointer_->sfPtr_) 
	     && (count_.reference() == 3) ) {
	    deletingScenario_ = 1;
	    delete pointer_;
	    pointer_ = 0;
	    deletingScenario_ = 0;
	}
	else if ( (!deletingScenario_) && (count_.reference() == 2) && 
		  ( (pointer_->ippPtr_) || (pointer_->sfPtr_) )  ) {
	    deletingScenario_ = 1;
	    delete pointer_;
	    pointer_ = 0;
	    deletingScenario_ = 0;
	}
	else if ( (!deletingScenario_) && (count_.reference() == 1) ) {
	    deletingScenario_ = 1;
	    delete pointer_;
	    pointer_ = 0;
	    deletingScenario_ = 0;
	}
    }
}

// Copy Constructor
LgFrScenarioSmartPointerToConst::LgFrScenarioSmartPointerToConst(const LgFrScenarioSmartPointerToConst& rhs) 
: pointer_(rhs.pointer_)
{
    count_ = ((LgFrScenarioSmartPointerToConst&)rhs).count_;
}

// Deep Copy
LgFrScenarioSmartPointerToConst
LgFrScenarioSmartPointerToConst::deepCopy() const
{
    // do scenario deep copy and create a new smart pointer
    LgFrScenarioSmartPointer temp(new LgFrScenario(*pointer_));

    // fix the ipp - since Scenario::initialProblemAndParameters() is non-const
    //  we need to cast away *pointer_
    temp->initialProblemAndParameters((((LgFrScenario&)(*pointer_)).initialProblemAndParameters()).clone(temp));
    temp->scheduleFactory((((LgFrScenario&)(*pointer_)).scheduleFactory())->clone(temp));
    return temp;
}

// Assignment Operator
LgFrScenarioSmartPointerToConst& 
LgFrScenarioSmartPointerToConst::operator=(const LgFrScenarioSmartPointerToConst& rhs)
{
    // first of all check for a 0 pointer
    if (pointer_ != 0)  {
	// If a scenario smart pointer is fully constructed, there are
	//  3 references to the actual scenario. 
	//    1 from the routine that owns the last Smart Pointer
	//    1 from the scenario's Initial Problem and Parameters
	//    1 from the scenario's Schedule Factory
	//  but we need to check to make sure that a full scenario is
	//    built up!

	if ( (pointer_->ippPtr_) && (pointer_->sfPtr_) )  {
	    if (count_.reference() == 3)  {
		deletingScenario_ = 1;
		delete pointer_;
		pointer_ = 0;  
		deletingScenario_ = 0;
	    }
	}
	else if ( (pointer_->ippPtr_) || (pointer_->sfPtr_) )  {
	    if (count_.reference() == 2)  {
		deletingScenario_ = 1;
		delete pointer_;
		pointer_ = 0;
		deletingScenario_ = 0;
	    }
	}
	else {
	    if (count_.reference() == 1)  {
		deletingScenario_ = 1;
		delete pointer_;
		pointer_ = 0;
		deletingScenario_ = 0;
	    }
	}
    }
    count_ = ((LgFrScenarioSmartPointerToConst&)rhs).count_;
    pointer_ = rhs.pointer_;
    return *this;
}


bool
LgFrScenarioSmartPointerToConst::operator==(const LgFrScenarioSmartPointerToConst& rhs) const
{
    if ( (pointer_ == rhs.pointer_) && (pointer_ == 0 || count_ == rhs.count_) )
	return true;
    else
	return false;
}

bool
LgFrScenarioSmartPointerToConst::operator!=(const LgFrScenarioSmartPointerToConst& rhs) const
{
    return !( *this == rhs );
}

const LgFrScenario* const
LgFrScenarioSmartPointerToConst::operator->() const
{
    // implicit cast to const
    return pointer_;
}

const LgFrScenario&
LgFrScenarioSmartPointerToConst::operator*() const
{
    // implicit cast to const
    return *pointer_;
}

bool
LgFrScenarioSmartPointerToConst::unique() const
{
    return (count_.reference() == 1);
}

bool 
LgFrScenarioSmartPointerToConst::null() const
{
    return (pointer_ == 0);
}

void
LgFrScenarioSmartPointerToConst::test()
{
    std::cout << "Testing default constructor" << std::endl;
    LgFrScenarioSmartPointerToConst dummy;
    assert(dummy.pointer_ == 0);
    std::cout << "dummy count is " << dummy.count_.reference() << std::endl;

    std::cout << "Testing preferred constructor (dummy)" << std::endl;
    LgFrScenarioSmartPointerToConst b(new LgFrScenario);
    std::cout << "b count is " << b.count_.reference() << std::endl;    

    std::cout << "Testing assignement operator dummy = b" << std::endl;
    dummy = b;
    std::cout << "b count is " << b.count_.reference() << std::endl;    

    std::cout << "Testing copy constructor (c = b)" << std::endl;
    LgFrScenarioSmartPointerToConst c(b);
    std::cout << "b count is " << b.count_.reference() << std::endl;    

    std::cout << "Testing backdoor constructor" << std::endl;
    LgFrScenarioSmartPointerToConst e(b.pointer_, b.count_);
    std::cout << "e count is " << e.count_.reference() << std::endl;    

    std::cout << "Testing equality operator (c == b)" << std::endl;
    assert(c == b);

    std::cout << "Testing inequality operator (c != d)" << std::endl;
    LgFrScenarioSmartPointerToConst d(new LgFrScenario);
    assert(c != d);

    std::cout << "Test destructor" << std::endl;
    c = d;
    std::cout << "b count is " << b.count_.reference() << std::endl;    
    c = b;
    d = b;

    // this should be a compiler error
//    LgFrScenarioSmartPointerToConst e(&(*d));
}



LgFrScenarioSmartPointer::LgFrScenarioSmartPointer() 
{
    // calls base class default constructor
}

LgFrScenarioSmartPointer::LgFrScenarioSmartPointer(LgFrScenario* newedPointer) 
: LgFrScenarioSmartPointerToConst(newedPointer)
{
}

LgFrScenarioSmartPointer::LgFrScenarioSmartPointer(
    LgFrScenario* existingPointer,
    LgFrReferenceCount& count) 
: LgFrScenarioSmartPointerToConst(existingPointer, count)
{
}


LgFrScenarioSmartPointer::~LgFrScenarioSmartPointer()
{
    // call base class destructor
}

LgFrScenarioSmartPointer::LgFrScenarioSmartPointer(const LgFrScenarioSmartPointer& rhs) 
: LgFrScenarioSmartPointerToConst(rhs.pointer_)
{
    count_ = ((LgFrScenarioSmartPointer&)rhs).count_;
}

// need to do some casting to a derived class so that we can access protected
//  members
LgFrScenarioSmartPointer::LgFrScenarioSmartPointer(LgFrScenarioSmartPointerToConst rhs) 
    : LgFrScenarioSmartPointerToConst(((LgFrScenarioSmartPointer&)rhs).pointer_)
{
    count_ = ((LgFrScenarioSmartPointer&)rhs).count_;
}

LgFrScenarioSmartPointer
LgFrScenarioSmartPointer::deepCopy() 
{
    // do scenario deep copy and create a new smart pointer
    LgFrScenarioSmartPointer temp(new LgFrScenario(*pointer_));

    // fix the ipp 
    temp->initialProblemAndParameters( (pointer_->initialProblemAndParameters()).clone(temp) );
    temp->scheduleFactory((pointer_->scheduleFactory())->clone(temp));
    return temp;
}

LgFrScenarioSmartPointer& 
LgFrScenarioSmartPointer::operator=(const LgFrScenarioSmartPointer& rhs)
{
    // first of all check for a 0 pointer
    if (pointer_ != 0)  {
	// If a scenario smart pointer is fully constructed, there are
	//  3 references to the actual scenario. 
	//    1 from the routine that owns the last Smart Pointer
	//    1 from the scenario's Initial Problem and Parameters
	//    1 from the scenario's Schedule Factory
	//  but we need to check to make sure that a full scenario is
	//    built up!

	if ( (pointer_->ippPtr_) && (pointer_->sfPtr_) )  {
	    if (count_.reference() == 3)  {
		deletingScenario_ = 1;
		delete pointer_;
		pointer_ = 0;
		deletingScenario_ = 0;
	    }
	}
	else if ( (pointer_->ippPtr_) || (pointer_->sfPtr_) )  {
	    if (count_.reference() == 2)  {
		deletingScenario_ = 1;
		delete pointer_;
		pointer_ = 0;
		deletingScenario_ = 0;
	    }
	}
	else {
	    if (count_.reference() == 1)  {
		deletingScenario_ = 1;
		delete pointer_;
		pointer_ = 0;
		deletingScenario_ = 0;
	    }
	}
    }
    count_ = ((LgFrScenarioSmartPointer&)rhs).count_;
    pointer_ = rhs.pointer_;
    return *this;
}

// need to do some casting to a derived class so that we can access protected
//  members
LgFrScenarioSmartPointer& 
LgFrScenarioSmartPointer::operator=(LgFrScenarioSmartPointerToConst rhs)
{
    // first of all check for a 0 pointer
    if (pointer_ != 0)  {
	// If a scenario smart pointer is fully constructed, there are
	//  3 references to the actual scenario. 
	//    1 from the routine that owns the last Smart Pointer
	//    1 from the scenario's Initial Problem and Parameters
	//    1 from the scenario's Schedule Factory
	//  but we need to check to make sure that a full scenario is
	//    built up!

	if ( (pointer_->ippPtr_) && (pointer_->sfPtr_) )  {
	    if (count_.reference() == 3)  {
		deletingScenario_ = 1;
		delete pointer_;
		pointer_ = 0;
		deletingScenario_ = 0;
	    }
	}
	else if ( (pointer_->ippPtr_) || (pointer_->sfPtr_) )  {
	    if (count_.reference() == 2)  {
		deletingScenario_ = 1;
		delete pointer_;
		pointer_ = 0;
		deletingScenario_ = 0;
	    }
	}
	else {
	    if (count_.reference() == 1)  {
		deletingScenario_ = 1;
		delete pointer_;
		pointer_ = 0;
		deletingScenario_ = 0;
	    }
	}
    }
    count_ = ((LgFrScenarioSmartPointer&)rhs).count_;
    pointer_ = ((LgFrScenarioSmartPointer&)rhs).pointer_;
    return *this;
}

bool
LgFrScenarioSmartPointer::operator==(const LgFrScenarioSmartPointer& rhs) const
{
    if ( (pointer_ == rhs.pointer_) && (pointer_ == 0 || count_ == rhs.count_) )
	return true;
    else
	return false;
}

bool
LgFrScenarioSmartPointer::operator!=(const LgFrScenarioSmartPointer& rhs) const
{
    return !( *this == rhs );
}

const LgFrScenario* const
LgFrScenarioSmartPointer::operator->() const
{
    return pointer_;
}

LgFrScenario* const
LgFrScenarioSmartPointer::operator->()
{
    return pointer_;
}

LgFrScenario &
LgFrScenarioSmartPointer::operator*()
{
    return *pointer_;
}

const LgFrScenario &
LgFrScenarioSmartPointer::operator*() const
{
    return *pointer_;
}

bool
LgFrScenarioSmartPointer::unique() const
{
    return (count_.reference() == 1);
}

bool 
LgFrScenarioSmartPointer::null() const
{
    return (pointer_ == 0);
}

void
LgFrScenarioSmartPointer::test()
{
    std::cout << "Testing default constructor" << std::endl;
    LgFrScenarioSmartPointer dummy;
    assert(dummy.pointer_ == 0);
    std::cout << "dummy count is " << dummy.count_.reference() << std::endl;

    std::cout << "Testing preferred constructor (dummy)" << std::endl;
    LgFrScenarioSmartPointer b(new LgFrScenario);
    std::cout << "b count is " << b.count_.reference() << std::endl;    

    std::cout << "Testing assignement operator dummy = b" << std::endl;
    dummy = b;
    std::cout << "b count is " << b.count_.reference() << std::endl;    

    std::cout << "Testing copy constructor (c = b)" << std::endl;
    LgFrScenarioSmartPointer c(b);
    std::cout << "b count is " << b.count_.reference() << std::endl;    

    std::cout << "Testing backdoor constructor" << std::endl;
    LgFrScenarioSmartPointer e(b.pointer_, b.count_);
    std::cout << "e count is " << e.count_.reference() << std::endl;    

    std::cout << "Testing equality operator (c == b)" << std::endl;
    assert(c == b);

    std::cout << "Testing inequality operator (c != d)" << std::endl;
    LgFrScenarioSmartPointer d(new LgFrScenario);
    assert(c != d);

    std::cout << "Test destructor" << std::endl;
    c = d;
    std::cout << "b count is " << b.count_.reference() << std::endl;    
    c = b;
    d = b;
}
