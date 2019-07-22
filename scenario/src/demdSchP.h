#ifndef LgFrSortingDemandScheduleFloatSMARTPTRH
#define LgFrSortingDemandScheduleFloatSMARTPTRH
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.
#include <scenario/src/refCount.h>

/*
class LgFrSortingDemandScheduleFloat
{
private:
    int i_;
public:
    LgFrSortingDemandScheduleFloat(int j = 0) : i_(j) {}
    const int i() const {return i_;}
    void i(const int j) {i_ = j;}
};
*/

// forward declaration
class LgFrSortingScheduleFloatSmartPointerToConst;
class LgFrSortingScheduleFloatSmartPointer;
class LgFrSortingDemandScheduleFloat;

class LgFrSortingDemandScheduleFloatSmartPointerToConst
{

public:
    LgFrSortingDemandScheduleFloatSmartPointerToConst();
    LgFrSortingDemandScheduleFloatSmartPointerToConst(LgFrSortingDemandScheduleFloat*);
    LgFrSortingDemandScheduleFloatSmartPointerToConst(LgFrSortingDemandScheduleFloat*, LgFrReferenceCount&);
    virtual ~LgFrSortingDemandScheduleFloatSmartPointerToConst();
    LgFrSortingDemandScheduleFloatSmartPointerToConst(const LgFrSortingDemandScheduleFloatSmartPointerToConst&);
    LgFrSortingDemandScheduleFloatSmartPointerToConst deepCopy() const;
    LgFrSortingDemandScheduleFloatSmartPointerToConst& operator=(const LgFrSortingDemandScheduleFloatSmartPointerToConst&);
    bool operator==(const LgFrSortingDemandScheduleFloatSmartPointerToConst&) const;
    bool operator!=(const LgFrSortingDemandScheduleFloatSmartPointerToConst&) const;
    const LgFrSortingDemandScheduleFloat& operator*() const;
    const LgFrSortingDemandScheduleFloat * const operator->() const;
//    operator const LgFrSortingDemandScheduleFloat* const () const;
    operator LgFrSortingScheduleFloatSmartPointerToConst () const;

    bool unique() const;
    bool null() const;
    static void test();

protected:
    LgFrSortingDemandScheduleFloat* pointer_; // should make this const but I want to use
                            //  it for the LgFrSortingDemandScheduleFloatSmartPointer class
			    //  (the non-const class) to inherit this member.
			    //  The way it's been set up only const stuff
                            //  is being returned so we're OK.
    LgFrReferenceCount count_;

};


class LgFrSortingDemandScheduleFloatSmartPointer : public LgFrSortingDemandScheduleFloatSmartPointerToConst
{
public:
    LgFrSortingDemandScheduleFloatSmartPointer();
    LgFrSortingDemandScheduleFloatSmartPointer(LgFrSortingDemandScheduleFloat*);
    LgFrSortingDemandScheduleFloatSmartPointer(LgFrSortingDemandScheduleFloat*, LgFrReferenceCount&);
    ~LgFrSortingDemandScheduleFloatSmartPointer();
    LgFrSortingDemandScheduleFloatSmartPointer(const LgFrSortingDemandScheduleFloatSmartPointer&);
    LgFrSortingDemandScheduleFloatSmartPointer(LgFrSortingDemandScheduleFloatSmartPointerToConst);
    LgFrSortingDemandScheduleFloatSmartPointer deepCopy();
    LgFrSortingDemandScheduleFloatSmartPointer& operator=(const LgFrSortingDemandScheduleFloatSmartPointer&);
//    LgFrSortingDemandScheduleFloatSmartPointer& operator=(LgFrSortingDemandScheduleFloatSmartPointerToConst);
    bool operator==(const LgFrSortingDemandScheduleFloatSmartPointer&) const;
//    bool operator==(LgFrSortingDemandScheduleFloatSmartPointerToConst) const;
    bool operator!=(const LgFrSortingDemandScheduleFloatSmartPointer&) const;
//    bool operator!=(LgFrSortingDemandScheduleFloatSmartPointerToConst) const;

    // these are really dangerous as they circumvent the notion
    //  of using smart pointers anyways.  you had better be using
    //  these just to call another method
    LgFrSortingDemandScheduleFloat& operator*();
    const LgFrSortingDemandScheduleFloat& operator*() const;
    LgFrSortingDemandScheduleFloat* const operator->();
    const LgFrSortingDemandScheduleFloat* const operator->() const;
//    operator const LgFrSortingDemandScheduleFloat* const () const;
//    operator LgFrSortingDemandScheduleFloat* ();
    operator LgFrSortingScheduleFloatSmartPointerToConst () const;
    operator LgFrSortingScheduleFloatSmartPointer ();

    bool unique() const;
    bool null() const;
    static void test();
};

#endif

