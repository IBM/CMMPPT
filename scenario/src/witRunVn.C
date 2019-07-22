// RW2STL -- inserted:
// End of RW2STL -- inserted includes.
#include <stdlib.h>

#if !defined (WIN32) && !defined (__OS2__) && !defined (__WINDOWS__)
#include <unistd.h>
#endif

#include <errno.h>
#include <assert.h>


#include <scenario/src/witRunVn.h>
#include <scenario/src/machdep.h>

// Return a pointer to a const version of my WitRun
// Note: implement the const part after there has been time to
// change witRun and the WIT API to handle const
/* const */ WitRun *
LgFrWitRunVeneer::constWitRun ()
const {
  assert (myRun_ != 0);
  return (/* (const WitRun *) */ myRun_);
}

// Return a pointer to a non-const version of my WitRun.
WitRun *
LgFrWitRunVeneer::mutableWitRun ()
{
  if (myRun_ == 0) 
    {
      // If the client wants to change it, better create
      // a real run, even if it is has no real data.
      witNewRun (&myRun_);
      witNoDependantWitRunVeneersIncr(myRun_);

      // If WIT Informational messages are desired, the following
      // line can me commented or LastProblemSolved::printMessage
      // can be used to control the printing of WIT messages.
      // Turn off WIT Informational messages
      witSetMesgTimesPrint(
          myRun_,
          WitTRUE,
          WitINFORMATIONAL_MESSAGES,
          0 );

      // Turn off WIT message warning that a part name is greater than
      // 12 characters long.
      witSetMesgTimesPrint( myRun_, WitTRUE, 338, 0 );

      witInitialize(myRun_);   // Initialize this wit run.

      witSetMesgTimesPrint( myRun_, WitTRUE, 538, 0 );
      witSetWit34Compatible( myRun_, WitTRUE );
    }
  else if (! (this -> noVeneersAroundMyWitRunIsOne()))
    {
      // Copy the old WitRun and decrement the number of references to it
      WitRun * oldWitRun;
      oldWitRun = myRun_;
      witNewRun (&myRun_);
      // If WIT Informational messages are desired, the following
      // line can me commented or LastProblemSolved::printMessage
      // can be used to control the printing of WIT messages.
      // Turn off WIT Informational messages
      witSetMesgTimesPrint(
               myRun_,
               WitTRUE,
               WitINFORMATIONAL_MESSAGES,
               0 );
      // Turn off WIT message warning that a part name is greater than
      // 12 characters long.
      witSetMesgTimesPrint( myRun_, WitTRUE, 338, 0 );

      witInitialize(myRun_);   // Initialize this wit run.

      // No need to set wit34Compatible attribute, because this is done
      // by witCopyData.

      witCopyData( myRun_, oldWitRun );

      witNoDependantWitRunVeneersDecr (oldWitRun);
      witNoDependantWitRunVeneersIncr (myRun_);
    }
  return myRun_;
}


int 
LgFrWitRunVeneer::noVeneersAroundMyWitRunIsOne ()
{
  assert (myRun_ != 0);
  int isOne;
  witNoDependantWitRunVeneersIsOne (myRun_, &isOne);
  return isOne;
}

LgFrWitRunVeneer::~LgFrWitRunVeneer()
{
  // Remove the reference to the old WitRun (and destroy the
  // old WitRun if this was the last reference to it)
  if (myRun_ != 0)
    {
      if (this -> noVeneersAroundMyWitRunIsOne())
	witDeleteRun (myRun_);
      else witNoDependantWitRunVeneersDecr(myRun_);
    }
}

// Assignment operator.
LgFrWitRunVeneer&
LgFrWitRunVeneer::operator=(
            const LgFrWitRunVeneer& rhs)
{
  if (&rhs != this) 
    {
      // Remove the reference to the old WitRun (and destroy the
      // old WitRun if this was the last reference to it)
      if (myRun_ != 0)
	{
	  if (this -> noVeneersAroundMyWitRunIsOne())
	    witDeleteRun (myRun_); // This line is causing PostProcessor::deleteConsVol() 
				   // [PostProc.o, pc=0xbbd80]  to read unitialized memory
	  else witNoDependantWitRunVeneersDecr (myRun_);
	}
      // Start refering to rhs's WitRun.
      myRun_ = rhs.myRun_;
      if (myRun_ != 0)
	witNoDependantWitRunVeneersIncr (myRun_);
    }
  return *this;
}

// Copy constructor
LgFrWitRunVeneer::LgFrWitRunVeneer(
            const LgFrWitRunVeneer& source)
: myRun_ (source.myRun_)
{
  if (myRun_ != 0)
    witNoDependantWitRunVeneersIncr (myRun_);
}

// Default Constructor
LgFrWitRunVeneer::LgFrWitRunVeneer()
: myRun_ (0)
{
}


// Another Constructor
LgFrWitRunVeneer::LgFrWitRunVeneer(
  WitRun * wr )
: myRun_ (wr)
{
 witNoDependantWitRunVeneersIncr(myRun_);
 assert( this->noVeneersAroundMyWitRunIsOne() );
}


#ifdef NDEBUG
#undef NDEBUG
#endif
// Test this class
void
LgFrWitRunVeneer::test()
{
  // Test constructors
  LgFrWitRunVeneer nullVeneer;
  LgFrWitRunVeneer veneerCopy (nullVeneer);

  // Test the assignment method.
  veneerCopy = nullVeneer;

  // Test the mutableWitRun and constWitRun methods
  char * realProb = "../../../../wit/wit-data/brenda/denise/wit.data";
  WitRun * veneerCopiesWitRun = veneerCopy.mutableWitRun();
  witReadData (veneerCopiesWitRun, realProb);
  int n = 0;
  witGetNPeriods (veneerCopy.constWitRun(), &n);
  assert (n == 5);

  LgFrWitRunVeneer walnut;	// A popular veneer
  char * realProb1 = "../../../../wit/wit-data/standard/obj1/wit.data";
  WitRun * walnutsWitRun = walnut.mutableWitRun();
  witReadData (walnutsWitRun, realProb1);
  int walnutNPeriods;
  witGetNPeriods (walnut.constWitRun(), &walnutNPeriods);
  assert (walnutNPeriods == 8);

  // Test the assignment method with non-null operands
  veneerCopy = walnut;
  int veneerCopyNPeriods;
  witGetNPeriods (walnut.constWitRun(), &veneerCopyNPeriods);
  assert (veneerCopyNPeriods == walnutNPeriods);
}

