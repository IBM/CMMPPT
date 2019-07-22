#ifndef LgFrSortingPartScheduleFloatSMARTPTRH
#define LgFrSortingPartScheduleFloatSMARTPTRH
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/refCount.h>

/*
class LgFrSortingPartScheduleFloat
{
private:
    int i_;
public:
    LgFrSortingPartScheduleFloat(int j = 0) : i_(j) {}
    const int i() const {return i_;}
    void i(const int j) {i_ = j;}
};
*/

// forward declaration
class LgFrSortingScheduleFloatSmartPointerToConst;
class LgFrSortingScheduleFloatSmartPointer;
class LgFrSortingPartScheduleFloat;

class LgFrSortingPartScheduleFloatSmartPointerToConst
{

public:
    LgFrSortingPartScheduleFloatSmartPointerToConst();
    LgFrSortingPartScheduleFloatSmartPointerToConst(LgFrSortingPartScheduleFloat*);
    LgFrSortingPartScheduleFloatSmartPointerToConst(LgFrSortingPartScheduleFloat*, LgFrReferenceCount&);
    virtual ~LgFrSortingPartScheduleFloatSmartPointerToConst();
    LgFrSortingPartScheduleFloatSmartPointerToConst(const LgFrSortingPartScheduleFloatSmartPointerToConst&);
    LgFrSortingPartScheduleFloatSmartPointerToConst deepCopy() const;
    LgFrSortingPartScheduleFloatSmartPointerToConst& operator=(const LgFrSortingPartScheduleFloatSmartPointerToConst&);
    bool operator==(const LgFrSortingPartScheduleFloatSmartPointerToConst&) const;
    bool operator!=(const LgFrSortingPartScheduleFloatSmartPointerToConst&) const;
    const LgFrSortingPartScheduleFloat& operator*() const;
    const LgFrSortingPartScheduleFloat * const operator->() const;
//    operator const LgFrSortingPartScheduleFloat* const () const;
    operator LgFrSortingScheduleFloatSmartPointerToConst () const;

    bool unique() const;
    bool null() const;
    static void test();

protected:
    LgFrSortingPartScheduleFloat* pointer_; // should make this const but I want to use
                            //  it for the LgFrSortingPartScheduleFloatSmartPointer class
			    //  (the non-const class) to inherit this member.
			    //  The way it's been set up only const stuff
                            //  is being returned so we're OK.
    LgFrReferenceCount count_;

};


class LgFrSortingPartScheduleFloatSmartPointer : public LgFrSortingPartScheduleFloatSmartPointerToConst
{
public:
    LgFrSortingPartScheduleFloatSmartPointer();
    LgFrSortingPartScheduleFloatSmartPointer(LgFrSortingPartScheduleFloat*);
    LgFrSortingPartScheduleFloatSmartPointer(LgFrSortingPartScheduleFloat*, LgFrReferenceCount&);
    ~LgFrSortingPartScheduleFloatSmartPointer();
    LgFrSortingPartScheduleFloatSmartPointer(const LgFrSortingPartScheduleFloatSmartPointer&);
    LgFrSortingPartScheduleFloatSmartPointer(LgFrSortingPartScheduleFloatSmartPointerToConst);
    LgFrSortingPartScheduleFloatSmartPointer deepCopy();
    LgFrSortingPartScheduleFloatSmartPointer& operator=(const LgFrSortingPartScheduleFloatSmartPointer&);
//    LgFrSortingPartScheduleFloatSmartPointer& operator=(LgFrSortingPartScheduleFloatSmartPointerToConst);
    bool operator==(const LgFrSortingPartScheduleFloatSmartPointer&) const;
//    bool operator==(LgFrSortingPartScheduleFloatSmartPointerToConst) const;
    bool operator!=(const LgFrSortingPartScheduleFloatSmartPointer&) const;
//    bool operator!=(LgFrSortingPartScheduleFloatSmartPointerToConst) const;

    // these are really dangerous as they circumvent the notion
    //  of using smart pointers anyways.  you had better be using
    //  these just to call another method
    LgFrSortingPartScheduleFloat& operator*();
    const LgFrSortingPartScheduleFloat& operator*() const;
    LgFrSortingPartScheduleFloat* const operator->();
    const LgFrSortingPartScheduleFloat* const operator->() const;
//    operator const LgFrSortingPartScheduleFloat* const () const;
//    operator LgFrSortingPartScheduleFloat* ();
    operator LgFrSortingScheduleFloatSmartPointerToConst () const;
    operator LgFrSortingScheduleFloatSmartPointer ();

    bool unique() const;
    bool null() const;
    static void test();
};

#endif

