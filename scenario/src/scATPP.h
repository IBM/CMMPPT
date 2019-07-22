#ifndef LgFrScenarioForATPSMARTPTRH
#define LgFrScenarioForATPSMARTPTRH
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/refCount.h>


// forward declaration

class LgFrScenarioSmartPointerToConst;
class LgFrScenarioSmartPointer;
class LgFrScenarioForATP;

class LgFrScenarioForATPSmartPointerToConst
{

public:
    LgFrScenarioForATPSmartPointerToConst();
    LgFrScenarioForATPSmartPointerToConst(LgFrScenarioForATP*);
    LgFrScenarioForATPSmartPointerToConst(LgFrScenarioForATP*, LgFrReferenceCount&);
    LgFrScenarioForATPSmartPointerToConst(LgFrScenarioSmartPointerToConst);
    virtual ~LgFrScenarioForATPSmartPointerToConst();
    LgFrScenarioForATPSmartPointerToConst(const LgFrScenarioForATPSmartPointerToConst&);
    LgFrScenarioForATPSmartPointerToConst deepCopy() const;
    LgFrScenarioForATPSmartPointerToConst& operator=(const LgFrScenarioForATPSmartPointerToConst&);
    bool operator==(const LgFrScenarioForATPSmartPointerToConst&) const;
    bool operator!=(const LgFrScenarioForATPSmartPointerToConst&) const;
    const LgFrScenarioForATP& operator*() const;
    const LgFrScenarioForATP * const operator->() const;
    operator LgFrScenarioSmartPointerToConst () const;

    bool unique() const;
    bool null() const;
    static void test();

protected:
    LgFrScenarioForATP* pointer_; // should make this const but I want to use
                            //  it for the LgFrScenarioForATPSmartPointer class
			    //  (the non-const class) to inherit this member.
			    //  The way it's been set up only const stuff
                            //  is being returned so we're OK.
    LgFrReferenceCount count_;

};


class LgFrScenarioForATPSmartPointer : public LgFrScenarioForATPSmartPointerToConst
{

public:
    LgFrScenarioForATPSmartPointer();
    LgFrScenarioForATPSmartPointer(LgFrScenarioForATP*);
    LgFrScenarioForATPSmartPointer(LgFrScenarioForATP*, LgFrReferenceCount&);
    LgFrScenarioForATPSmartPointer(LgFrScenarioSmartPointerToConst);
    ~LgFrScenarioForATPSmartPointer();
    LgFrScenarioForATPSmartPointer(const LgFrScenarioForATPSmartPointer&);
    LgFrScenarioForATPSmartPointer(LgFrScenarioForATPSmartPointerToConst);
    LgFrScenarioForATPSmartPointer deepCopy();
    LgFrScenarioForATPSmartPointer& operator=(const LgFrScenarioForATPSmartPointer&);
    LgFrScenarioForATPSmartPointer& operator=(LgFrScenarioForATPSmartPointerToConst);
    bool operator==(const LgFrScenarioForATPSmartPointer&) const;
    bool operator!=(const LgFrScenarioForATPSmartPointer&) const;

    // these are really dangerous as they circumvent the notion
    //  of using smart pointers anyways.  you had better be using
    //  these just to call another method
    LgFrScenarioForATP& operator*();
    const LgFrScenarioForATP& operator*() const;
    LgFrScenarioForATP* const operator->();
    const LgFrScenarioForATP* const operator->() const;
    operator LgFrScenarioSmartPointerToConst () const;
    operator LgFrScenarioSmartPointer ();

    bool unique() const;
    bool null() const;
    static void test();
};

#endif

