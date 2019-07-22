#ifndef LgFrTestScenarioSMARTPTRH
#define LgFrTestScenarioSMARTPTRH
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/refCount.h>

// forward declaration
class LgFrScenarioSmartPointerToConst;
class LgFrScenarioSmartPointer;
class LgFrTestScenario;

class LgFrTestScenarioSmartPointerToConst
{

public:
    LgFrTestScenarioSmartPointerToConst();
    LgFrTestScenarioSmartPointerToConst(LgFrTestScenario*);
    LgFrTestScenarioSmartPointerToConst(LgFrTestScenario*, LgFrReferenceCount&);
    LgFrTestScenarioSmartPointerToConst(LgFrScenarioSmartPointerToConst);
    virtual ~LgFrTestScenarioSmartPointerToConst();
    LgFrTestScenarioSmartPointerToConst(const LgFrTestScenarioSmartPointerToConst&);
    LgFrTestScenarioSmartPointerToConst deepCopy() const;
    LgFrTestScenarioSmartPointerToConst& operator=(const LgFrTestScenarioSmartPointerToConst&);
    bool operator==(const LgFrTestScenarioSmartPointerToConst&) const;
    bool operator!=(const LgFrTestScenarioSmartPointerToConst&) const;
    const LgFrTestScenario& operator*() const;
    const LgFrTestScenario * const operator->() const;
    operator LgFrScenarioSmartPointerToConst () const;

    bool unique() const;
    bool null() const;
    static void test();

protected:
    LgFrTestScenario* pointer_; // should make this const but I want to use
                            //  it for the LgFrTestScenarioSmartPointer class
			    //  (the non-const class) to inherit this member.
			    //  The way it's been set up only const stuff
                            //  is being returned so we're OK.
    LgFrReferenceCount count_;

};


class LgFrTestScenarioSmartPointer : public LgFrTestScenarioSmartPointerToConst
{

public:
    LgFrTestScenarioSmartPointer();
    LgFrTestScenarioSmartPointer(LgFrTestScenario*);
    LgFrTestScenarioSmartPointer(LgFrTestScenario*, LgFrReferenceCount&);
    LgFrTestScenarioSmartPointer(LgFrScenarioSmartPointerToConst);
    ~LgFrTestScenarioSmartPointer();
    LgFrTestScenarioSmartPointer(const LgFrTestScenarioSmartPointer&);
    LgFrTestScenarioSmartPointer(LgFrTestScenarioSmartPointerToConst);
    LgFrTestScenarioSmartPointer deepCopy();
    LgFrTestScenarioSmartPointer& operator=(const LgFrTestScenarioSmartPointer&);
    LgFrTestScenarioSmartPointer& operator=(LgFrTestScenarioSmartPointerToConst);
    bool operator==(const LgFrTestScenarioSmartPointer&) const;
    bool operator!=(const LgFrTestScenarioSmartPointer&) const;

    // these are really dangerous as they circumvent the notion
    //  of using smart pointers anyways.  you had better be using
    //  these just to call another method
    LgFrTestScenario& operator*();
    const LgFrTestScenario& operator*() const;
    LgFrTestScenario* const operator->();
    const LgFrTestScenario* const operator->() const;
    operator LgFrScenarioSmartPointerToConst () const;
    operator LgFrScenarioSmartPointer ();

    bool unique() const;
    bool null() const;
    static void test();
};

#endif

