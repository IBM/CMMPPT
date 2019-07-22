//------------------------------------------------------------------------------
// WIT-J C++ Header File CTVecSupply.h.
//
// Contains the declaration of class CTVecSupply.
//------------------------------------------------------------------------------

#ifndef CTVecSupply_h
#define CTVecSupply_h

#include <Includes.h>

#include     <list>

//------------------------------------------------------------------------------
// Class CTVecSupply
//
// "C Time-Vector Supply"
// A source of supply of C time vectors to be used with a given Problem.
//------------------------------------------------------------------------------

namespace WitJ
{
class CTVecSupply
   {
   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   explicit CTVecSupply (int nPeriodsVal);
      //
      // Constructor.
      // nPeriodsVal is the # periods for the Problem associated with the
      // CTVecSupply being constructed.

   ~CTVecSupply ();
      //
      // Destructor.

   void provideCTVec (int    * &);
   void provideCTVec (double * &);
      //
      // Each of these functions:
      //    Sets its argument to a c-vector of length nPeriods.
      //    Transfers ownership of the c-vector to the caller.

   void reclaimCTVec (int    * &);
   void reclaimCTVec (double * &);
      //
      // Each of these functions:
      //    Requires its argument to be a c-vector of length nPeriods.
      //    Transfers ownership of the c-vector to this CTVecSupply.
      //    Sets its argument to NULL.

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   CTVecSupply              (const CTVecSupply &);
   CTVecSupply & operator = (const CTVecSupply &);
      //
      // Not implemented; prevents accidental copying and assignment.

   //---------------------------------------------------------------------------
   // Private member data.
   //---------------------------------------------------------------------------

   const int nPeriods_;
      //
      // The nPeriods value to which all the C time vectors will be allocated.

   std::list <int *>    myIntCTVecs_;
   std::list <double *> myDblCTVecs_;
      //
      // The C time vectors owned by this CTVecSupply.
   };
};
//
// End of namespace WitJ.

#endif
