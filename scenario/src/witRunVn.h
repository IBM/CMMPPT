#ifndef LgFrWitRunVeneer_h
#define LgFrWitRunVeneer_h

// RW2STL -- inserted:
#include <string>
// End of RW2STL -- inserted includes.
// This class is a veneer around WitRun.  It's purpose is to allow
// constrolled use of one WitRun by multiple clients.  In particular,
// it allows multiple clients to use the same WitRun read-only and
// makes a copy of the WitRun for any client that wants to modify
// its copy.  It is similart to the std::string class in that a copy of
// the WitRun is only made when a client wants to modify it.

// The definition and implementation are not particularly good.  Ideally,
// this class would completely encapsulate WitRun and only allow you
// to access it through methods that knew whether to copy the WitRun
// or not.  For the sake of expediency though, it's not done that way.
// Instead, it returns a pointer to it's WitRun that you can use
// to access the WitRun.  Clients can ask for two versions of the
// pointer.  One points to a const WitRun, the other points to a
// (non-const) WitRun.  The method that returns a pointer to a const
// allows multiple clients to use the same WitRun, the one that returns
// a pointer to a non-const WitRun makes a copy of the old WitRun
// if it is being shared and returns a pointer to the copy.

#include <wit/src/wit.h>

class LgFrWitRunVeneer {
public:
  // Return a pointer to a const version of my WitRun
  virtual
    /* const */ WitRun *
      constWitRun ()
	const;

  // Return a pointer to a non-const version of my WitRun.
  virtual
    WitRun *
      mutableWitRun ();
  
  // Destructor
  ~LgFrWitRunVeneer();

  // Assignment operator.
  LgFrWitRunVeneer&
    operator=(
	      const LgFrWitRunVeneer& rhs);

  // Copy constructor
  LgFrWitRunVeneer(
		   const LgFrWitRunVeneer& source);

  // Default Constructor
  LgFrWitRunVeneer();

  // Another Constructor
  // When using this constructor the WitRunVeneer assumes ownership
  // of the witRun.  The client should no longer reference the witRun.
  LgFrWitRunVeneer(
    WitRun * wr);

  // Tests this class.  Only returns if all tests passed
  static
    void
      test ();

private:
  // Returns the number of veneers around my WitRun
  int noVeneersAroundMyWitRunIsOne ();

  WitRun *myRun_;	// This is the WitRun I'm veneering

};

#endif
