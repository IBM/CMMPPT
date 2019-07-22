#ifndef LgFrSortingScheduleFloatSMARTPTRH
#define LgFrSortingScheduleFloatSMARTPTRH
// RW2STL -- inserted:
#include <vector>
// End of RW2STL -- inserted includes.

#include <scenario/src/generVc.h>
#include <scenario/src/refCount.h>


// forward declaration
class LgFrSortingScheduleFloat;


class LgFrSortingScheduleFloatSmartPointerToConst
{

    friend class LgFrSortingDemandScheduleSmartPointerToConst;
    friend class LgFrSortingPartScheduleSmartPointerToConst;

public:
    LgFrSortingScheduleFloatSmartPointerToConst();
    LgFrSortingScheduleFloatSmartPointerToConst(LgFrSortingScheduleFloat*);
    LgFrSortingScheduleFloatSmartPointerToConst(LgFrSortingScheduleFloat*, LgFrReferenceCount&);
    virtual ~LgFrSortingScheduleFloatSmartPointerToConst();
    LgFrSortingScheduleFloatSmartPointerToConst(const LgFrSortingScheduleFloatSmartPointerToConst&);
    LgFrSortingScheduleFloatSmartPointerToConst deepCopy() const;
    LgFrSortingScheduleFloatSmartPointerToConst& operator=(const LgFrSortingScheduleFloatSmartPointerToConst&);
    bool operator==(const LgFrSortingScheduleFloatSmartPointerToConst&) const;
    bool operator!=(const LgFrSortingScheduleFloatSmartPointerToConst&) const;
    const LgFrSortingScheduleFloat& operator*() const;
    const LgFrSortingScheduleFloat * const operator->() const;


    bool unique() const;
    bool null() const;
    static void test();

protected:
    LgFrSortingScheduleFloat* pointer_; // should make this const but I want to use
                            //  it for the LgFrSortingScheduleFloatSmartPointer class
			    //  (the non-const class) to inherit this member.
			    //  The way it's been set up only const stuff
                            //  is being returned so we're OK.
    LgFrReferenceCount count_;

};


class LgFrSortingScheduleFloatSmartPointer : public LgFrSortingScheduleFloatSmartPointerToConst
{

    friend class LgFrSortingDemandScheduleSmartPointer;
    friend class LgFrSortingPartScheduleSmartPointer;

public:
    LgFrSortingScheduleFloatSmartPointer();
    LgFrSortingScheduleFloatSmartPointer(LgFrSortingScheduleFloat*);
    LgFrSortingScheduleFloatSmartPointer(LgFrSortingScheduleFloat*, LgFrReferenceCount&);
    ~LgFrSortingScheduleFloatSmartPointer();
    LgFrSortingScheduleFloatSmartPointer(const LgFrSortingScheduleFloatSmartPointer&);
    LgFrSortingScheduleFloatSmartPointer(LgFrSortingScheduleFloatSmartPointerToConst);
    LgFrSortingScheduleFloatSmartPointer deepCopy() const;
    LgFrSortingScheduleFloatSmartPointer& operator=(const LgFrSortingScheduleFloatSmartPointer&);

    bool operator==(const LgFrSortingScheduleFloatSmartPointer&) const;
    bool operator!=(const LgFrSortingScheduleFloatSmartPointer&) const;


    // these are really dangerous as they circumvent the notion
    //  of using smart pointers anyways.  you had better be using
    //  these just to call another method
    LgFrSortingScheduleFloat& operator*();
    const LgFrSortingScheduleFloat& operator*() const;
    LgFrSortingScheduleFloat* const operator->();
    const LgFrSortingScheduleFloat* const operator->() const;

    bool unique() const;
    bool null() const;
    static void test();
};

// RW2STL - declare(RWGVector, LgFrSortingScheduleFloatSmartPointer)

// RW2STL - declare(RWGOrderedVector, LgFrSortingScheduleFloatSmartPointer)


typedef std::vector<LgFrSortingScheduleFloatSmartPointer> // RW2STL - typedef RWGOrderedVector(LgFrSortingScheduleFloatSmartPointer)

                 LgFrOrderedVectorSortingScheduleFloatSmartPointer;

#endif

