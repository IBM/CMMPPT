//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef OrigMrpH
#define OrigMrpH

//------------------------------------------------------------------------------
// Header file: "OrigMrp.h"
//
// Contains the declaration of class OrigMrp.
//------------------------------------------------------------------------------

#include <MrpExp.h>

//------------------------------------------------------------------------------
// class OrigMrpExp.
//
// Responsible for performing MRP explosion on the original Problem.
//------------------------------------------------------------------------------

class WitOrigMrpExp: public WitMrpExp
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitOrigMrpExp (WitProblem * theProblem);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitOrigMrpExp ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (bool, exploding)
      accessFunc (bool, uploadingSoln)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitOrigMrpExp);

      //------------------------------------------------------------------------
      // Other private member functions.
      //------------------------------------------------------------------------

      virtual void initDerived ();

      virtual double   supplyVol (WitPart      *, WitPeriod);
      virtual double   demandVol (WitPart      *, WitPeriod);
      virtual double & execVol   (WitOperation *, WitPeriod);
      virtual double & subVol    (WitSubEntry  *, WitPeriod);

      virtual void finishDerived ();

      void uploadSoln ();
         //
         // Uploads the MRP solution.

      void compMrpConsVols ();
         //
         // Computes mrpConsVol for each Part.

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      bool exploding_;
         //
         // true, iff this OrigMrpExp is currently performing Mrp explosion.

      bool uploadingSoln_;
         //
         // True, iff the MRP subsystem is currently uploading the MRP solution.

      //------------------------------------------------------------------------
      // MRP solution Schedules.
      //------------------------------------------------------------------------

      WitSchedule <WitOperation, double> mrpExecVol_;
      WitSchedule <WitSubEntry,  double> mrpSubVol_;
   };

#endif
