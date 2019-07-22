// Copyright (C) 2002, International Business Machines
// Corporation and others.  All Rights Reserved.
#if defined(_MSC_VER)
// Turn off compiler warning about long names
#  pragma warning(disable:4786)
#endif
#include <cassert>

#include "OsiRowCut.hpp"
#include "SbbCountRowCut.hpp"
#include "SbbNode.hpp"
// Default Constructor 
SbbCountRowCut::SbbCountRowCut ()
  :
  OsiRowCut(),
  owner_(NULL),
  ownerCut_(-1),
  numberPointingToThis_(0),
  whichCutGenerator_(-1)
{
#ifdef CHECK_CUT_COUNTS
  printf("SbbCountRowCut default constructor %x\n",this);
#endif
}
   
// Copy Constructor 
SbbCountRowCut::SbbCountRowCut (const OsiRowCut & rhs)
  : OsiRowCut(rhs),
    owner_(NULL),
    ownerCut_(-1),
    numberPointingToThis_(0),
    whichCutGenerator_(-1)
{
#ifdef CHECK_CUT_COUNTS
  printf("SbbCountRowCut constructor %x from RowCut\n",this);
#endif
}
// Copy Constructor 
SbbCountRowCut::SbbCountRowCut (const OsiRowCut & rhs,
				SbbNodeInfo * info, int whichOne,
				int whichGenerator)
  : OsiRowCut(rhs),
  owner_(info),
  ownerCut_(whichOne),
    numberPointingToThis_(0),
  whichCutGenerator_(whichGenerator)
{
#ifdef CHECK_CUT_COUNTS
  printf("SbbCountRowCut constructor %x from RowCut and info\n",this);
#endif
}
SbbCountRowCut::~SbbCountRowCut()
{
#ifdef CHECK_CUT_COUNTS
  printf("SbbCountRowCut destructor %x - references %d\n",this,
	 numberPointingToThis_);
#endif
  // Look at owner and delete
  owner_->deleteCut(ownerCut_);
}
// Increment number of references
void 
SbbCountRowCut::increment(int change)
{
  numberPointingToThis_+=change;
}

// Decrement number of references and return number left
int 
SbbCountRowCut::decrement(int change)
{
  assert(numberPointingToThis_>=change);
  numberPointingToThis_-=change;
  return numberPointingToThis_;
}
// Set information
void 
SbbCountRowCut::setInfo(SbbNodeInfo * info, int whichOne)
{
  owner_=info;
  ownerCut_=whichOne;
}

