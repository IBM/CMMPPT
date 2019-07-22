#include <iostream.h>
#include <assert.h>
#include <scenario/src/scenP.h>
#include <scenario/src/scATPP.h>
#include <scenario/src/scenATP.h>
#include <scenario/src/ipATP.h>
#include <scenario/src/schFcATP.h>

// Default Constructor
LgFrScenarioForATPSmartPointerToConst::LgFrScenarioForATPSmartPointerToConst() : pointer_(NULL)
{
};

// Preferred Constructor
LgFrScenarioForATPSmartPointerToConst::LgFrScenarioForATPSmartPointerToConst(LgFrScenarioForATP* newedPointer) 
: pointer_(newedPointer), count_(1)
{
}

// Constructor for cast operator
LgFrScenarioForATPSmartPointerToConst::LgFrScenarioForATPSmartPointerToConst(
    LgFrScenarioForATP* existingPointer,
    LgFrReferenceCount& count) : pointer_(existingPointer)
{
    count_ = count;
}

// Constructor 
LgFrScenarioForATPSmartPointerToConst::LgFrScenarioForATPSmartPointerToConst(
    LgFrScenarioSmartPointerToConst rhs)
{
    if (rhs->isA() != __LGFRSCENARIOFORATP)  {
	cerr << "Cannot construct a LgFrScenarioForATPSmartPointerToConst from a non LgFrScenarioForATP object" << endl;
	assert(0);
    }
    else  {
	pointer_ = (LgFrScenarioForATP*)(rhs.pointer_);
	count_ = rhs.count_;
    }
}

// Destructor
LgFrScenarioForATPSmartPointerToConst::~LgFrScenarioForATPSmartPointerToConst()
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
	pointer_ = NULL;
	LgFrScenarioSmartPointerToConst::deletingScenario_ = 0;
    }
    else if ( (count_.reference() == 2) && 
	      ( (pointer_->ippPtr_) || (pointer_->sfPtr_) )  ) {
	LgFrScenarioSmartPointerToConst::deletingScenario_ = 1;
	delete pointer_;
	pointer_ = NULL;
	LgFrScenarioSmartPointerToConst::deletingScenario_ = 0;
    }
    else if (count_.reference() == 1) {
	LgFrScenarioSmartPointerToConst::deletingScenario_ = 1;
	delete pointer_;
	pointer_ = NULL;
	LgFrScenarioSmartPointerToConst::deletingScenario_ = 0;
    }
}

// Copy Constructor
LgFrScenarioForATPSmartPointerToConst::LgFrScenarioForATPSmartPointerToConst(const LgFrScenarioForATPSmartPointerToConst& rhs) 
: pointer_(rhs.pointer_)
{
    count_ = ((LgFrScenarioForATPSmartPointerToConst&)rhs).count_;
}

// Deep Copy
LgFrScenarioForATPSmartPointerToConst
LgFrScenarioForATPSmartPointerToConst::deepCopy() const
{
    // do scenario deep copy and create a new smart pointer
    LgFrScenarioForATPSmartPointer temp(new LgFrScenarioForATP(*pointer_));

    // fix the ipp - since Scenario::initialProblemAndParameters() is non-const
    //  we need to cast away *pointer_
    temp->initialProblemAndParameters(((LgFrInitialProblemForATP&)(((LgFrScenarioForATP&)(*pointer_)).initialProblemAndParameters())).clone(temp));

    // fix the schedule factory - since Scenario::scheduleFactory() 
    // is non-const we need to cast away *pointer_
    temp->scheduleFactory(((LgFrScheduleFactoryForATP*)(((LgFrScenarioForATP&)(*pointer_)).scheduleFactory()))->clone(temp));

    return temp;
}

// Assignment Operator
LgFrScenarioForATPSmartPointerToConst& 
LgFrScenarioForATPSmartPointerToConst::operator=(const LgFrScenarioForATPSmartPointerToConst& rhs)
{
    // If a scenario smart pointer is fully constructed, there are
    //  3 references to the actual scenario. 
    //    1 from the routine that owns the last Smart Pointer
    //    1 from the scenario's Initial Problem and Parameters
    //    1 from the scenario's Schedule Factory
    //  but we need to check to make sure that a full scenario is
    //    built up!

    if (pointer_ != NULL)  {
	if ( (pointer_->ippPtr_) && (pointer_->sfPtr_) 
	     && (count_.reference() == 3) ) {
	    LgFrScenarioSmartPointerToConst::deletingScenario_ = 1;
	    cout << "deleting LgFrTestScenario (count_.reference() == 3)" << endl;
	    delete pointer_;
	    cout << "deleted LgFrTestScenario" << endl;
	    pointer_ = NULL;
	    LgFrScenarioSmartPointerToConst::deletingScenario_ = 0;
	}
	else if ( (count_.reference() == 2) && 
		  ( (pointer_->ippPtr_) || (pointer_->sfPtr_) )  ) {
	    LgFrScenarioSmartPointerToConst::deletingScenario_ = 1;
	    cout << "deleting LgFrTestScenario (count_.reference() == 2)" << endl;
	    delete pointer_;
	    cout << "deleted LgFrTestScenario" << endl;
	    pointer_ = NULL;
	    LgFrScenarioSmartPointerToConst::deletingScenario_ = 0;
	}
	else if (count_.reference() == 1) {
	    LgFrScenarioSmartPointerToConst::deletingScenario_ = 1;
	    cout << "deleting LgFrTestScenario (count_.reference() == 1)" << endl;
	    delete pointer_;
	    cout << "deleted LgFrTestScenario" << endl;
	    pointer_ = NULL;
	    LgFrScenarioSmartPointerToConst::deletingScenario_ = 0;
	}
    }
    count_ = ((LgFrScenarioForATPSmartPointerToConst&)rhs).count_;
    pointer_ = rhs.pointer_;
    return *this;
}


RWBoolean
LgFrScenarioForATPSmartPointerToConst::operator==(const LgFrScenarioForATPSmartPointerToConst& rhs) const
{
    if ( (pointer_ == rhs.pointer_) && (count_ == rhs.count_) )
	return TRUE;
    else
	return FALSE;
}

RWBoolean
LgFrScenarioForATPSmartPointerToConst::operator!=(const LgFrScenarioForATPSmartPointerToConst& rhs) const
{
    if ( (pointer_ != rhs.pointer_) || (count_ != rhs.count_) )
	return TRUE;
    else
	return FALSE;
}

const LgFrScenarioForATP* const
LgFrScenarioForATPSmartPointerToConst::operator->() const
{
    // implicit cast to const
    return pointer_;
}

const LgFrScenarioForATP&
LgFrScenarioForATPSmartPointerToConst::operator*() const
{
    // implicit cast to const
    return *pointer_;
}


LgFrScenarioForATPSmartPointerToConst::operator LgFrScenarioSmartPointerToConst () const
{
    // need to cast away const for count_
    return LgFrScenarioSmartPointerToConst(pointer_, (LgFrReferenceCount&)count_);
}

RWBoolean
LgFrScenarioForATPSmartPointerToConst::unique() const
{
    return (count_.reference() == 1);
}

RWBoolean 
LgFrScenarioForATPSmartPointerToConst::null() const
{
    return (pointer_ == NULL);
}

void
LgFrScenarioForATPSmartPointerToConst::test()
{
    cout << "Testing default constructor" << endl;
    LgFrScenarioForATPSmartPointerToConst dummy;
    assert(dummy.pointer_ == NULL);
    cout << "dummy count is " << dummy.count_.reference() << endl;

    cout << "Testing preferred constructor (dummy)" << endl;
    LgFrScenarioForATPSmartPointerToConst b(new LgFrScenarioForATP);
    cout << "b count is " << b.count_.reference() << endl;    

    cout << "Testing assignement operator dummy = b" << endl;
    dummy = b;
    cout << "b count is " << b.count_.reference() << endl;    

    cout << "Testing copy constructor (c = b)" << endl;
    LgFrScenarioForATPSmartPointerToConst c(b);
    cout << "b count is " << b.count_.reference() << endl;    

    cout << "Testing backdoor constructor" << endl;
    LgFrScenarioForATPSmartPointerToConst e(b.pointer_, b.count_);
    cout << "e count is " << e.count_.reference() << endl;    

    cout << "Testing equality operator (c == b)" << endl;
    assert(c == b);

    cout << "Testing inequality operator (c != d)" << endl;
    LgFrScenarioForATPSmartPointerToConst d(new LgFrScenarioForATP);
    assert(c != d);

    cout << "Test destructor" << endl;
    c = d;
    cout << "b count is " << b.count_.reference() << endl;    
    c = b;
    d = b;

    // this should be a compiler error
//    LgFrScenarioForATPSmartPointerToConst e(&(*d));
}



LgFrScenarioForATPSmartPointer::LgFrScenarioForATPSmartPointer() 
{
    // calls base class default constructor
}

LgFrScenarioForATPSmartPointer::LgFrScenarioForATPSmartPointer(LgFrScenarioForATP* newedPointer) 
: LgFrScenarioForATPSmartPointerToConst(newedPointer)
{
}

LgFrScenarioForATPSmartPointer::LgFrScenarioForATPSmartPointer(
    LgFrScenarioForATP* existingPointer,
    LgFrReferenceCount& count) 
: LgFrScenarioForATPSmartPointerToConst(existingPointer, count)
{
}


LgFrScenarioForATPSmartPointer::~LgFrScenarioForATPSmartPointer()
{
    // call base class destructor
}

LgFrScenarioForATPSmartPointer::LgFrScenarioForATPSmartPointer(const LgFrScenarioForATPSmartPointer& rhs) 
: LgFrScenarioForATPSmartPointerToConst(rhs.pointer_)
{
    count_ = ((LgFrScenarioForATPSmartPointer&)rhs).count_;
}

// need to do some casting to a derived class so that we can access protected
//  members
LgFrScenarioForATPSmartPointer::LgFrScenarioForATPSmartPointer(LgFrScenarioForATPSmartPointerToConst rhs) 
    : LgFrScenarioForATPSmartPointerToConst(((LgFrScenarioForATPSmartPointer&)rhs).pointer_)
{
    count_ = ((LgFrScenarioForATPSmartPointer&)rhs).count_;
}

// Constructor 
LgFrScenarioForATPSmartPointer::LgFrScenarioForATPSmartPointer(
    LgFrScenarioSmartPointerToConst rhs)
{
    if (rhs->isA() != __LGFRSCENARIOFORATP)  {
	cerr << "Cannot construct a LgFrScenarioForATPSmartPointerToConst from a non LgFrScenarioForATP object" << endl;
	assert(0);
    }
    else  {
	// silly casting to derived class (SmartPointerToConst -> SmartPointer)
	//   so that we can access protected members pointer_, count_
	pointer_ = (LgFrScenarioForATP*)(((LgFrScenarioSmartPointer&)rhs).pointer_);
	count_ = ((LgFrScenarioSmartPointer&)rhs).count_;
    }
}

LgFrScenarioForATPSmartPointer
LgFrScenarioForATPSmartPointer::deepCopy() 
{
    // do scenario deep copy and create a new smart pointer
    LgFrScenarioForATPSmartPointer temp(new LgFrScenarioForATP(*pointer_));

    // fix the ipp 
    temp->initialProblemAndParameters(((LgFrInitialProblemForATP&)(pointer_->initialProblemAndParameters())).clone(temp));

    // fix the schedule factory 
    temp->scheduleFactory(((LgFrScheduleFactoryForATP*)(pointer_->scheduleFactory()))->clone(temp));


    return temp;
}

LgFrScenarioForATPSmartPointer& 
LgFrScenarioForATPSmartPointer::operator=(const LgFrScenarioForATPSmartPointer& rhs)
{
    // If a scenario smart pointer is fully constructed, there are
    //  3 references to the actual scenario. 
    //    1 from the routine that owns the last Smart Pointer
    //    1 from the scenario's Initial Problem and Parameters
    //    1 from the scenario's Schedule Factory
    //  but we need to check to make sure that a full scenario is
    //    built up!

    if (pointer_ != NULL)  {
	if ( (pointer_->ippPtr_) && (pointer_->sfPtr_) 
	     && (count_.reference() == 3) ) {
	    LgFrScenarioSmartPointerToConst::deletingScenario_ = 1;
	    cout << "deleting LgFrTestScenario (count_.reference() == 3)" << endl;
	    delete pointer_;
	    cout << "deleted LgFrTestScenario" << endl;
	    pointer_ = NULL;
	    LgFrScenarioSmartPointerToConst::deletingScenario_ = 0;
	}
	else if ( (count_.reference() == 2) && 
		  ( (pointer_->ippPtr_) || (pointer_->sfPtr_) )  ) {
	    LgFrScenarioSmartPointerToConst::deletingScenario_ = 1;
	    cout << "deleting LgFrTestScenario (count_.reference() == 2)" << endl;
	    delete pointer_;
	    cout << "deleted LgFrTestScenario" << endl;
	    pointer_ = NULL;
	    LgFrScenarioSmartPointerToConst::deletingScenario_ = 0;
	}
	else if (count_.reference() == 1) {
	    LgFrScenarioSmartPointerToConst::deletingScenario_ = 1;
	    cout << "deleting LgFrTestScenario (count_.reference() == 1)" << endl;
	    delete pointer_;
	    cout << "deleted LgFrTestScenario" << endl;
	    pointer_ = NULL;
	    LgFrScenarioSmartPointerToConst::deletingScenario_ = 0;
	}
    }
    count_ = ((LgFrScenarioForATPSmartPointer&)rhs).count_;
    pointer_ = rhs.pointer_;
    return *this;
}

// need to do some casting to a derived class so that we can access protected
//  members
LgFrScenarioForATPSmartPointer& 
LgFrScenarioForATPSmartPointer::operator=(LgFrScenarioForATPSmartPointerToConst rhs)
{
    // If a scenario smart pointer is fully constructed, there are
    //  3 references to the actual scenario. 
    //    1 from the routine that owns the last Smart Pointer
    //    1 from the scenario's Initial Problem and Parameters
    //    1 from the scenario's Schedule Factory
    //  but we need to check to make sure that a full scenario is
    //    built up!

    if (pointer_ != NULL)  {
	if ( (pointer_->ippPtr_) && (pointer_->sfPtr_) 
	     && (count_.reference() == 3) ) {
	    LgFrScenarioSmartPointerToConst::deletingScenario_ = 1;
	    cout << "deleting LgFrTestScenario (count_.reference() == 3)" << endl;
	    delete pointer_;
	    cout << "deleted LgFrTestScenario" << endl;
	    pointer_ = NULL;
	    LgFrScenarioSmartPointerToConst::deletingScenario_ = 0;
	}
	else if ( (count_.reference() == 2) && 
		  ( (pointer_->ippPtr_) || (pointer_->sfPtr_) )  ) {
	    LgFrScenarioSmartPointerToConst::deletingScenario_ = 1;
	    cout << "deleting LgFrTestScenario (count_.reference() == 2)" << endl;
	    delete pointer_;
	    cout << "deleted LgFrTestScenario" << endl;
	    pointer_ = NULL;
	    LgFrScenarioSmartPointerToConst::deletingScenario_ = 0;
	}
	else if (count_.reference() == 1) {
	    LgFrScenarioSmartPointerToConst::deletingScenario_ = 1;
	    cout << "deleting LgFrTestScenario (count_.reference() == 1)" << endl;
	    delete pointer_;
	    cout << "deleted LgFrTestScenario" << endl;
	    pointer_ = NULL;
	    LgFrScenarioSmartPointerToConst::deletingScenario_ = 0;
	}
    }
    count_ = ((LgFrScenarioForATPSmartPointer&)rhs).count_;
    pointer_ = ((LgFrScenarioForATPSmartPointer&)rhs).pointer_;
    return *this;
}

RWBoolean
LgFrScenarioForATPSmartPointer::operator==(const LgFrScenarioForATPSmartPointer& rhs) const
{
    if ( (pointer_ == rhs.pointer_) && (count_ == rhs.count_) )
	return TRUE;
    else
	return FALSE;
}

RWBoolean
LgFrScenarioForATPSmartPointer::operator!=(const LgFrScenarioForATPSmartPointer& rhs) const
{
    if ( (pointer_ != rhs.pointer_) || (count_ != rhs.count_) )
	return TRUE;
    else
	return FALSE;
}

LgFrScenarioForATP* const
LgFrScenarioForATPSmartPointer::operator->()
{
    return pointer_;
}

const LgFrScenarioForATP* const
LgFrScenarioForATPSmartPointer::operator->() const
{
    return pointer_;
}

LgFrScenarioForATP &
LgFrScenarioForATPSmartPointer::operator*()
{
    return *pointer_;
}

const LgFrScenarioForATP &
LgFrScenarioForATPSmartPointer::operator*() const
{
    return *pointer_;
}


LgFrScenarioForATPSmartPointer::operator LgFrScenarioSmartPointerToConst () const
{
    // need to cast away const for count_
    return LgFrScenarioSmartPointerToConst(pointer_, (LgFrReferenceCount&)count_);
}

LgFrScenarioForATPSmartPointer::operator LgFrScenarioSmartPointer () 
{
    return LgFrScenarioSmartPointer(pointer_, count_);
}



RWBoolean
LgFrScenarioForATPSmartPointer::unique() const
{
    return (count_.reference() == 1);
}

RWBoolean 
LgFrScenarioForATPSmartPointer::null() const
{
    return (pointer_ == NULL);
}

void
LgFrScenarioForATPSmartPointer::test()
{
    cout << "Testing default constructor" << endl;
    LgFrScenarioForATPSmartPointer dummy;
    assert(dummy.pointer_ == NULL);
    cout << "dummy count is " << dummy.count_.reference() << endl;

    cout << "Testing preferred constructor (dummy)" << endl;
    LgFrScenarioForATPSmartPointer b(new LgFrScenarioForATP);
    cout << "b count is " << b.count_.reference() << endl;    

    cout << "Testing assignement operator dummy = b" << endl;
    dummy = b;
    cout << "b count is " << b.count_.reference() << endl;    

    cout << "Testing copy constructor (c = b)" << endl;
    LgFrScenarioForATPSmartPointer c(b);
    cout << "b count is " << b.count_.reference() << endl;    

    cout << "Testing backdoor constructor" << endl;
    LgFrScenarioForATPSmartPointer e(b.pointer_, b.count_);
    cout << "e count is " << e.count_.reference() << endl;    

    cout << "Testing equality operator (c == b)" << endl;
    assert(c == b);

    cout << "Testing inequality operator (c != d)" << endl;
    LgFrScenarioForATPSmartPointer d(new LgFrScenarioForATP);
    assert(c != d);

    cout << "Test destructor" << endl;
    c = d;
    cout << "b count is " << b.count_.reference() << endl;    
    c = b;
    d = b;
}
