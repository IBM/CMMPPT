#ifndef LGFRREFCOUNTH
#define LGFRREFCOUNTH

#pragma warning(disable:4522)

// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

// This looks very similar to the Rogue Wave RWReference class
//  but since I couldn't find documentation on the class I will
//  assume that they don't want me to use it


class LgFrReferenceCount
{
public:
    LgFrReferenceCount(const int initCount = 0);
    ~LgFrReferenceCount();
    void addReference();
    unsigned int reference() const;
    void reference(unsigned int*);
    LgFrReferenceCount& operator=(LgFrReferenceCount&); 
    bool operator==(const LgFrReferenceCount&) const; 
    bool operator!=(const LgFrReferenceCount&) const; 

private:
    unsigned int* count_;
    LgFrReferenceCount(const LgFrReferenceCount&); // do not allow 
    LgFrReferenceCount& operator=(const LgFrReferenceCount&); // do not allow

};
#endif
