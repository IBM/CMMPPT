//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef CpxParSpecMgrH
#define CpxParSpecMgrH

//------------------------------------------------------------------------------
// Header file: "CpxParSpecMgr.h"
//
// Contains the declaration of class CpxParSpecMgr.
//------------------------------------------------------------------------------

#include <Assoc.h>
#include <Str.h>
#include <List.h>

//------------------------------------------------------------------------------
// Class CpxParSpecMgr
//
// "CPLEX Parameter Specification Manager"
// Responsible for owning and generally managing the CpxParSpecs of a Problem.
//
// Implemented in CpxParSpec.C.
//------------------------------------------------------------------------------

class WitCpxParSpecMgr: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitCpxParSpecMgr (WitProblem *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitCpxParSpecMgr ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (const WitList <WitCpxParSpec> &, myCpxParSpecs)
      accessFunc (const WitString &,               cplexParSpecName)
      accessFunc (int,                             cplexParSpecIntVal)
      accessFunc (double,                          cplexParSpecDblVal)

      //------------------------------------------------------------------------
      // Default value functions.
      //------------------------------------------------------------------------

      defaultValue (const char * defCplexParSpecName,   "NO_PARAM")
      defaultValue (int          defCplexParSpecIntVal, 0)
      defaultValue (double       defCplexParSpecDblVal, 0)

      //------------------------------------------------------------------------
      // "Set" functions.
      //------------------------------------------------------------------------

      void setCplexParSpecName   (const char *);
      void setCplexParSpecIntVal (int);
      void setCplexParSpecDblVal (double);

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void addIntCpxParSpec (const char * theName, int    theValue);
      void addDblCpxParSpec (const char * theName, double theValue);
         //
         // Each of these functions:
         //    Creates a CpxParSpec with the indicated type, name and value.
         //    Stores it at the end of myCpxParSpecs_.
         //    Deletes any existing CpxParSpec whose name matches theName.

      WitCpxParSpec * findCpxParSpec (const char * theName);
         //
         // Finds and returns the CpxParSpec owned by this CpxParSpecMgr whose
         // name matches theName, if any; otherwise returns NULL.

      void clearCpxParSpecs ();
         //
         // Deletes all of the CpxParSpecs owned by this CpxParSpecMgr.

      void display ();
         //
         // Displays the input data for this CpxParSpecMgr.

      void copyAttrsFrom (WitCpxParSpecMgr * theCpxParSpecMgr);
         //
         // Copies the input attributes of theCpxParSpecMgr into this
         // CpxParSpecMgr.

      void writeData ();
         //
         // Writes the section of the input data file for this CpxParSpecMgr.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      void deleteMatchingCpxParSpec (const char * theName);
         //
         // If this CpxParSpecMgr owns a CpxParSpec whose name matches theName,
         // it is deleted.

      noCopyCtorAssign (WitCpxParSpecMgr);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitList <WitCpxParSpec> myCpxParSpecs_;
         //
         // The CpxParSpecs owned by this CpxParSpecMgr.

      WitString cplexParSpecName_;
         //
         // The name of a CPLEX parameter specification to be created.

      int cplexParSpecIntVal_;
         //
         // The value of a CPLEX parameter specification of type int to be
         // created.

      double cplexParSpecDblVal_;
         //
         // The value of a CPLEX parameter specification of type double to be
         // created.

      bool addingSpecsByVal_;
         //
         // Setting the value of cplexParSpecIntVal_ or cplexParSpecDblVal_ will
         // cause of new CpxParSpec to be added, iff addingSpecsByVal_ is true.
   };

#endif
