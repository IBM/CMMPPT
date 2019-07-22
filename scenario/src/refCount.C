// RW2STL -- inserted:
// End of RW2STL -- inserted includes.
#include <scenario/src/refCount.h>

LgFrReferenceCount::LgFrReferenceCount(const int initCount) 
: count_(new unsigned int ((unsigned int)initCount))
{
}

LgFrReferenceCount::~LgFrReferenceCount()
{
    if (*count_ == 1)  {
	delete count_;
	count_ = 0;
    }
    else
	(*count_)--;
}

void
LgFrReferenceCount::addReference()
{
    (*count_)++;
}


unsigned int
LgFrReferenceCount::reference() const
{
    return *count_;
}


void
LgFrReferenceCount::reference(unsigned int* count) 
{
    count_ = count;
}

LgFrReferenceCount&
LgFrReferenceCount::operator=(LgFrReferenceCount& rhs)
{
    rhs.addReference();
    if (*count_ <= 1)  {
	delete count_;
	count_ = 0;
    }
    else
	(*count_)--;
    count_ = rhs.count_;
    return *this;
}

bool
LgFrReferenceCount::operator==(const LgFrReferenceCount& rhs) const
{
    return (count_ == rhs.count_);
}


bool
LgFrReferenceCount::operator!=(const LgFrReferenceCount& rhs) const
{
    return (count_ != rhs.count_);
}
