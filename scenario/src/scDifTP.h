#ifndef LgFrScenarioForDifTestingSMARTPTRH
#define LgFrScenarioForDifTestingSMARTPTRH

#include <rw/defs.h>
#include <scenario/src/refCount.h>

// forward declaration
class LgFrScenarioSmartPointerToConst;
class LgFrScenarioSmartPointer;
class LgFrScenarioForDifTesting;

class LgFrScenarioForDifTestingSmartPointerToConst
{

public:
    LgFrScenarioForDifTestingSmartPointerToConst();
    LgFrScenarioForDifTestingSmartPointerToConst(LgFrScenarioForDifTesting*);
    LgFrScenarioForDifTestingSmartPointerToConst(LgFrScenarioForDifTesting*, LgFrReferenceCount&);
    LgFrScenarioForDifTestingSmartPointerToConst(LgFrScenarioSmartPointerToConst);
    virtual ~LgFrScenarioForDifTestingSmartPointerToConst();
    LgFrScenarioForDifTestingSmartPointerToConst(const LgFrScenarioForDifTestingSmartPointerToConst&);
    LgFrScenarioForDifTestingSmartPointerToConst deepCopy() const;
    LgFrScenarioForDifTestingSmartPointerToConst& operator=(const LgFrScenarioForDifTestingSmartPointerToConst&);
    RWBoolean operator==(const LgFrScenarioForDifTestingSmartPointerToConst&) const;
    RWBoolean operator!=(const LgFrScenarioForDifTestingSmartPointerToConst&) const;
    const LgFrScenarioForDifTesting& operator*() const;
    const LgFrScenarioForDifTesting * const operator->() const;
    operator LgFrScenarioSmartPointerToConst () const;

    RWBoolean unique() const;
    RWBoolean null() const;
    static void test();

protected:
    LgFrScenarioForDifTesting* pointer_; // should make this const but I want to use
                            //  it for the LgFrScenarioForDifTestingSmartPointer class
			    //  (the non-const class) to inherit this member.
			    //  The way it's been set up only const stuff
                            //  is being returned so we're OK.
    LgFrReferenceCount count_;

};


class LgFrScenarioForDifTestingSmartPointer : public LgFrScenarioForDifTestingSmartPointerToConst
{

public:
    LgFrScenarioForDifTestingSmartPointer();
    LgFrScenarioForDifTestingSmartPointer(LgFrScenarioForDifTesting*);
    LgFrScenarioForDifTestingSmartPointer(LgFrScenarioForDifTesting*, LgFrReferenceCount&);
    LgFrScenarioForDifTestingSmartPointer(LgFrScenarioSmartPointerToConst);
    ~LgFrScenarioForDifTestingSmartPointer();
    LgFrScenarioForDifTestingSmartPointer(const LgFrScenarioForDifTestingSmartPointer&);
    LgFrScenarioForDifTestingSmartPointer(LgFrScenarioForDifTestingSmartPointerToConst);
    LgFrScenarioForDifTestingSmartPointer deepCopy();
    LgFrScenarioForDifTestingSmartPointer& operator=(const LgFrScenarioForDifTestingSmartPointer&);
    LgFrScenarioForDifTestingSmartPointer& operator=(LgFrScenarioForDifTestingSmartPointerToConst);
    RWBoolean operator==(const LgFrScenarioForDifTestingSmartPointer&) const;
    RWBoolean operator!=(const LgFrScenarioForDifTestingSmartPointer&) const;

    // these are really dangerous as they circumvent the notion
    //  of using smart pointers anyways.  you had better be using
    //  these just to call another method
    LgFrScenarioForDifTesting& operator*();
    const LgFrScenarioForDifTesting& operator*() const;
    LgFrScenarioForDifTesting* const operator->();
    const LgFrScenarioForDifTesting* const operator->() const;
    operator LgFrScenarioSmartPointerToConst () const;
    operator LgFrScenarioSmartPointer ();

    RWBoolean unique() const;
    RWBoolean null() const;
    static void test();
};

#endif

