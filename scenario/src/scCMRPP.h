#ifndef LgFrScenarioForCMRPSMARTPTRH
#define LgFrScenarioForCMRPSMARTPTRH
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/refCount.h>

// forward declaration
class LgFrScenarioSmartPointerToConst;
class LgFrScenarioSmartPointer;
class LgFrScenarioForCMRP;


class LgFrScenarioForCMRPSmartPointerToConst
{

public:
    LgFrScenarioForCMRPSmartPointerToConst();
    LgFrScenarioForCMRPSmartPointerToConst(LgFrScenarioForCMRP*);
    LgFrScenarioForCMRPSmartPointerToConst(LgFrScenarioForCMRP*, LgFrReferenceCount&);
    LgFrScenarioForCMRPSmartPointerToConst(LgFrScenarioSmartPointerToConst);
    virtual ~LgFrScenarioForCMRPSmartPointerToConst();
    LgFrScenarioForCMRPSmartPointerToConst(const LgFrScenarioForCMRPSmartPointerToConst&);
    LgFrScenarioForCMRPSmartPointerToConst deepCopy() const;
    LgFrScenarioForCMRPSmartPointerToConst& operator=(const LgFrScenarioForCMRPSmartPointerToConst&);
    bool operator==(const LgFrScenarioForCMRPSmartPointerToConst&) const;
    bool operator!=(const LgFrScenarioForCMRPSmartPointerToConst&) const;
    const LgFrScenarioForCMRP& operator*() const;
    const LgFrScenarioForCMRP * const operator->() const;
    operator LgFrScenarioSmartPointerToConst () const;

    bool unique() const;
    bool null() const;
    static void test();

protected:
    LgFrScenarioForCMRP* pointer_; // should make this const but I want to use
                            //  it for the LgFrScenarioForCMRPSmartPointer class
			    //  (the non-const class) to inherit this member.
			    //  The way it's been set up only const stuff
                            //  is being returned so we're OK.
    LgFrReferenceCount count_;

};


class LgFrScenarioForCMRPSmartPointer : public LgFrScenarioForCMRPSmartPointerToConst
{

public:
    LgFrScenarioForCMRPSmartPointer();
    LgFrScenarioForCMRPSmartPointer(LgFrScenarioForCMRP*);
    LgFrScenarioForCMRPSmartPointer(LgFrScenarioForCMRP*, LgFrReferenceCount&);
    LgFrScenarioForCMRPSmartPointer(LgFrScenarioSmartPointerToConst);
    ~LgFrScenarioForCMRPSmartPointer();
    LgFrScenarioForCMRPSmartPointer(const LgFrScenarioForCMRPSmartPointer&);
    LgFrScenarioForCMRPSmartPointer(LgFrScenarioForCMRPSmartPointerToConst);
    LgFrScenarioForCMRPSmartPointer deepCopy();
    LgFrScenarioForCMRPSmartPointer& operator=(const LgFrScenarioForCMRPSmartPointer&);
    LgFrScenarioForCMRPSmartPointer& operator=(LgFrScenarioForCMRPSmartPointerToConst);
    bool operator==(const LgFrScenarioForCMRPSmartPointer&) const;
    bool operator!=(const LgFrScenarioForCMRPSmartPointer&) const;

    // these are really dangerous as they circumvent the notion
    //  of using smart pointers anyways.  you had better be using
    //  these just to call another method
    LgFrScenarioForCMRP& operator*();
    const LgFrScenarioForCMRP& operator*() const;
    LgFrScenarioForCMRP* const operator->();
    const LgFrScenarioForCMRP* const operator->() const;
    operator LgFrScenarioSmartPointerToConst () const;
    operator LgFrScenarioSmartPointer ();

    bool unique() const;
    bool null() const;
    static void test();
};

#endif

