#ifndef LgFrScenarioSMARTPTRH
#define LgFrScenarioSMARTPTRH
// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL -- inserted includes.

#include <scenario/src/refCount.h>


// forward declaration
class LgFrScenario;

class LgFrScenarioSmartPointerToConst
{
    friend class LgFrScenarioForATPSmartPointerToConst;
    friend class LgFrScenarioForCMRPSmartPointerToConst;
    friend class LgFrScenarioForDifTestingSmartPointerToConst;
    friend class LgFrTestScenarioSmartPointerToConst;
    friend class LgFrScenarioForSceSmartPointerToConst;
    friend class LgFrScenarioForImanSmartPointerToConst;
    friend class LgFrScenarioForSmanSmartPointerToConst;

    friend class LgFrScenarioForATPSmartPointer;
    friend class LgFrScenarioForCMRPSmartPointer;
    friend class LgFrScenarioForDifTestingSmartPointer;
    friend class LgFrTestScenarioSmartPointer;
    friend class LgFrScenarioForSceSmartPointer;

public:
    LgFrScenarioSmartPointerToConst();
    LgFrScenarioSmartPointerToConst(LgFrScenario*);
    LgFrScenarioSmartPointerToConst(LgFrScenario*, LgFrReferenceCount&);
    virtual ~LgFrScenarioSmartPointerToConst();
    LgFrScenarioSmartPointerToConst(const LgFrScenarioSmartPointerToConst&);
    LgFrScenarioSmartPointerToConst deepCopy() const;
    LgFrScenarioSmartPointerToConst& operator=(const LgFrScenarioSmartPointerToConst&);
    bool operator==(const LgFrScenarioSmartPointerToConst&) const;
    bool operator!=(const LgFrScenarioSmartPointerToConst&) const;
    const LgFrScenario& operator*() const;
    const LgFrScenario * const operator->() const;

    bool unique() const;
    bool null() const;

    //-------------------------------------------------------------------------
    // This method was used for debugging.  Be warned that the reference
    // count can be changed by invoking this method.
    //
    // Example Usage:
    //   LgFrScenarioForATPSmartPointer scenarioPtr = builder.getScenario();
    //   cout <<((LgFrScenarioSmartPointerToConst)scenarioPtr).referenceCount();
    //-------------------------------------------------------------------------
    //unsigned int referenceCount() { return count_.reference(); } const

    static void test();

protected:
    LgFrScenario* pointer_; // should make this const but I want to use
                            //  it for the LgFrScenarioSmartPointer class
			    //  (the non-const class) to inherit this member.
			    //  The way it's been set up only const stuff
                            //  is being returned so we're OK.
    LgFrReferenceCount count_;

    static int deletingScenario_;

};


class LgFrScenarioSmartPointer : public LgFrScenarioSmartPointerToConst
{
    friend class LgFrScenarioForATPSmartPointer;
    friend class LgFrScenarioForCMRPSmartPointer;
    friend class LgFrScenarioForDifTestingSmartPointer;
    friend class LgFrTestScenarioSmartPointer;
    friend class LgFrScenarioForSceSmartPointer;
    friend class LgFrScenarioForImanSmartPointer;
    friend class LgFrScenarioForSmanSmartPointer;

public:
    LgFrScenarioSmartPointer();
    LgFrScenarioSmartPointer(LgFrScenario*);
    LgFrScenarioSmartPointer(LgFrScenario*, LgFrReferenceCount&);
    ~LgFrScenarioSmartPointer();
    LgFrScenarioSmartPointer(const LgFrScenarioSmartPointer&);
    LgFrScenarioSmartPointer(LgFrScenarioSmartPointerToConst);
    LgFrScenarioSmartPointer deepCopy();
    LgFrScenarioSmartPointer& operator=(const LgFrScenarioSmartPointer&);
    LgFrScenarioSmartPointer& operator=(LgFrScenarioSmartPointerToConst);
    bool operator==(const LgFrScenarioSmartPointer&) const;
    bool operator!=(const LgFrScenarioSmartPointer&) const;

    // these are really dangerous as they circumvent the notion
    //  of using smart pointers anyways.  you had better be using
    //  these just to call another method
    LgFrScenario& operator*();
    const LgFrScenario& operator*() const;
    LgFrScenario* const operator->();
    const LgFrScenario* const operator->() const;

    bool unique() const;
    bool null() const;
    static void test();

	static const LgFrScenarioSmartPointer nullScenarioSmartPointer;
};

#endif

