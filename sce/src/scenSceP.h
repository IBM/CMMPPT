#ifndef LgFrScenarioForSceSMARTPTRH
#define LgFrScenarioForSceSMARTPTRH

#include <scenario/src/refCount.h>

// forward declaration
class LgFrScenarioSmartPointerToConst;
class LgFrScenarioSmartPointer;
class LgFrScenarioForSce;


class LgFrScenarioForSceSmartPointerToConst
{

public:
    LgFrScenarioForSceSmartPointerToConst();
    LgFrScenarioForSceSmartPointerToConst(LgFrScenarioForSce*);
    LgFrScenarioForSceSmartPointerToConst(LgFrScenarioForSce*, LgFrReferenceCount&);
    LgFrScenarioForSceSmartPointerToConst(LgFrScenarioSmartPointerToConst);
    virtual ~LgFrScenarioForSceSmartPointerToConst();
    LgFrScenarioForSceSmartPointerToConst(const LgFrScenarioForSceSmartPointerToConst&);
    LgFrScenarioForSceSmartPointerToConst deepCopy() const;
    LgFrScenarioForSceSmartPointerToConst& operator=(const LgFrScenarioForSceSmartPointerToConst&);
    bool operator==(const LgFrScenarioForSceSmartPointerToConst&) const;
    bool operator!=(const LgFrScenarioForSceSmartPointerToConst&) const;
    const LgFrScenarioForSce& operator*() const;
    const LgFrScenarioForSce * const operator->() const;
    operator LgFrScenarioSmartPointerToConst () const;

    bool unique() const;
    bool null() const;
    static void test();

protected:
    LgFrScenarioForSce* pointer_; // should make this const but I want to use
                            //  it for the LgFrScenarioForSceSmartPointer class
			    //  (the non-const class) to inherit this member.
			    //  The way it's been set up only const stuff
                            //  is being returned so we're OK.
    LgFrReferenceCount count_;

};


class LgFrScenarioForSceSmartPointer : public LgFrScenarioForSceSmartPointerToConst
{

public:
    LgFrScenarioForSceSmartPointer();
    LgFrScenarioForSceSmartPointer(LgFrScenarioForSce*);
    LgFrScenarioForSceSmartPointer(LgFrScenarioForSce*, LgFrReferenceCount&);
    LgFrScenarioForSceSmartPointer(LgFrScenarioSmartPointerToConst);
    ~LgFrScenarioForSceSmartPointer();
    LgFrScenarioForSceSmartPointer(const LgFrScenarioForSceSmartPointer&);
    LgFrScenarioForSceSmartPointer(LgFrScenarioForSceSmartPointerToConst);
    LgFrScenarioForSceSmartPointer deepCopy();
    LgFrScenarioForSceSmartPointer& operator=(const LgFrScenarioForSceSmartPointer&);
    LgFrScenarioForSceSmartPointer& operator=(LgFrScenarioForSceSmartPointerToConst);
    bool operator==(const LgFrScenarioForSceSmartPointer&) const;
    bool operator!=(const LgFrScenarioForSceSmartPointer&) const;

    // these are really dangerous as they circumvent the notion
    //  of using smart pointers anyways.  you had better be using
    //  these just to call another method
    LgFrScenarioForSce& operator*();
    const LgFrScenarioForSce& operator*() const;
    LgFrScenarioForSce* const operator->();
    const LgFrScenarioForSce* const operator->() const;
    operator LgFrScenarioSmartPointerToConst () const;
    operator LgFrScenarioSmartPointer ();

    bool unique() const;
    bool null() const;
    static void test();
};

#endif

