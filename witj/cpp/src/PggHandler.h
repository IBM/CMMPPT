//------------------------------------------------------------------------------
// WIT-J C++ Header File PggHandler.h.
//
// Contains the declaration of class PggHandler.
//------------------------------------------------------------------------------

#ifndef PggHandler_h
#define PggHandler_h

#include <Includes.h>

//------------------------------------------------------------------------------
// Class template PggHandler
//
// Responsible for retrieving a pegging.
//------------------------------------------------------------------------------

namespace WitJ
{
class PggHandler
   {
   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   explicit PggHandler (Coordinator *);
      //
      // Constructor.

   ~PggHandler ();
      //
      // Destructor.

   //---------------------------------------------------------------------------
   // Demand pegging retrieval functions.
   // Each of these functons retrieves from WIT the pegging indicated by the
   // function name for the Demand in the period, storing it in the
   // PeggingTripleALJOR.
   //---------------------------------------------------------------------------

   void getConsVolPip   (Demand *, int, PeggingTripleALJOR &);
   void getCoExecVolPip (Demand *, int, PeggingTripleALJOR &);
   void getExecVolPip   (Demand *, int, PeggingTripleALJOR &);
   void getProdVolPip   (Demand *, int, PeggingTripleALJOR &);
   void getSideVolPip   (Demand *, int, PeggingTripleALJOR &);
   void getSubVolPip    (Demand *, int, PeggingTripleALJOR &);
   void getSupplyVolPip (Demand *, int, PeggingTripleALJOR &);
   void getExecVolCp    (Demand *, int, PeggingTripleALJOR &);
   void getSubVolCp     (Demand *, int, PeggingTripleALJOR &);

   //---------------------------------------------------------------------------
   // Operation pegging retrieval functions.
   // Each of these functons retrieves from WIT the pegging indicated by the
   // function name for the Operation in the period, storing it in the
   // PeggingTripleALJOR.
   //---------------------------------------------------------------------------

   void getConsVolPip   (Operation *, int, PeggingTripleALJOR &);
   void getCoExecVolPip (Operation *, int, PeggingTripleALJOR &);
   void getExecVolPip   (Operation *, int, PeggingTripleALJOR &);
   void getProdVolPip   (Operation *, int, PeggingTripleALJOR &);
   void getSideVolPip   (Operation *, int, PeggingTripleALJOR &);
   void getSubVolPip    (Operation *, int, PeggingTripleALJOR &);
   void getSupplyVolPip (Operation *, int, PeggingTripleALJOR &);

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   //---------------------------------------------------------------------------
   // Each of the following functions takes the pegging that was retrieved from
   // WIT and stores it in the PeggingTripleALJOR.
   // The pegging attribute type to be stored is indicated by the function name.
   // The function then frees all c-vectors retrieved from WIT.
   //---------------------------------------------------------------------------

   void storePartPgg   (PeggingTripleALJOR &);
   void storeOpnPgg    (PeggingTripleALJOR &);
   void storeSubPgg    (PeggingTripleALJOR &);
   void storeBopEntPgg (PeggingTripleALJOR &);

   //---------------------------------------------------------------------------

   void freeWitCVectors ();
      //
      // Frees all c-vectors retrieved from WIT.

   PggHandler              (const PggHandler &);
   PggHandler & operator = (const PggHandler &);
      //
      // Not implemented; prevents accidental copying and assignment.

   //---------------------------------------------------------------------------
   // Private member data.
   //---------------------------------------------------------------------------

   Coordinator * const myCoordinator_;
      //
      // The Coordinator associated with this PggHandler.

   WitRun * const myWitRun_;
      //
      // The WitRun associated with this PggHandler.

   int nTriples_;
      //
      // # pegging triples retrieved by the most recently called WIT pegging
      // retrieval function, or -1.

   //---------------------------------------------------------------------------
   // Each of the following data members is a c-vector of length nTriples_
   // that was retrieved by the most recently called WIT pegging retrieval
   // function, or NULL.
   //---------------------------------------------------------------------------

   char * * myPartNameCVec_;
   char * * myOpnNameCVec_;
   int *    myBopIdxCVec_;
   int *    myBomIdxCVec_;
   int *    mySubIdxCVec_;
   int *    myPeriodCVec_;
   double * myVolumeCVec_;
   };
};
//
// End of namespace WitJ.

#endif
