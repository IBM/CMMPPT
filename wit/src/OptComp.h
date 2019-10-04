//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef OptCompH
#define OptCompH

//------------------------------------------------------------------------------
// Header file: "OptComp.h"
//
// Contains the declaration of class OptComp.
//------------------------------------------------------------------------------

#include <Component.h>
#include <Str.h>

//------------------------------------------------------------------------------
// class OptComp
//
// Responsible for the global attributes of a Problem that are specific to
// optimizing implosion.
//
// Class Hierarchy:
//
// ProbAssoc
//    Component
//       OptComp
//------------------------------------------------------------------------------

class WitOptComp: public WitComponent
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitOptComp (WitCompMgr * theCompMgr);
         //
         // Component <<< theCompMgr.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitOptComp ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitOptStarter *,                heurOptStarter)
      accessFunc (WitOptStarter *,                 accOptStarter)
      accessFunc (WitOptStarter *,               schedOptStarter)
      accessFunc (WitOptStarter *,               crashOptStarter)
      accessFunc (const WitList <WitOptStarter> &, allOptStarters)
      accessFunc (WitCpxParSpecMgr *,            myCpxParSpecMgr)
      accessFunc (bool,                          compPrices)
      accessFunc (bool,                          accAfterOptImp)
      accessFunc (bool,                          accAfterSoftLB)
      accessFunc (bool,                          mipMode)
      accessFunc (bool,                          multiObjMode)
      accessFunc (int,                           objChoice)
      accessFunc (double,                        wbounds)
      accessFunc (WitOptStarter *,               optInitMethod)
      accessFunc (const WitString &,             solverLogFileName)
      accessFunc (double,                        boundsValue)
      accessFunc (bool,                          boundsValueValid)
      accessFunc (int,                           cplexStatusCode)
      accessFunc (const WitString &,             cplexStatusText)
      accessFunc (double,                        cplexMipBound)
      accessFunc (double,                        cplexMipRelGap)
      accessFunc (bool,                          printOptProblem)
      accessFunc (bool,                          printMatByCols)
      accessFunc (bool,                          printInitSoln)
      accessFunc (bool,                          printOptSoln)
      accessFunc (bool,                          printMps)
      accessFunc (bool,                          skipScreening)

      inline WitMultiObjMgr * myMultiObjMgr ()
         {
         witAssert (multiObjMode_);

         return myMultiObjMgr_;
         }

      //------------------------------------------------------------------------
      // Default value functions.
      //------------------------------------------------------------------------

      defaultValue (bool         defCompPrices,        false)
      defaultValue (bool         defAccAfterOptImp,    false)
      defaultValue (bool         defAccAfterSoftLB,    false)
      defaultValue (bool         defMipMode,           false)
      defaultValue (bool         defMultiObjMode,      false)
      defaultValue (double       defWbounds,           1.0e4)
      defaultValue (const char * defSolverLogFileName, "solver.log")
      defaultValue (int          defObjChoice,         1)

      WitOptStarter * defOptInitMethod () const;

      //------------------------------------------------------------------------
      // "Set" functions.
      //------------------------------------------------------------------------

      void setCompPrices        (bool);
      void setAccAfterOptImp    (bool);
      void setAccAfterSoftLB    (bool);
      void setMipMode           (bool);
      void setMultiObjMode      (bool);
      void setObjChoice         (int);
      void setWbounds           (double);
      void setSolverLogFileName (const char *);
      void setPrintOptProblem   (bool);
      void setPrintMatByCols    (bool);
      void setPrintInitSoln     (bool);
      void setPrintOptSoln      (bool);
      void setPrintMps          (bool);
      void setSkipScreening     (bool);

      //------------------------------------------------------------------------
      // store<Activity><Attr> functions.
      // Each of these functions sets the value of indicated attribute to the
      // value of the argument.
      // Each function is only valid when the indicated activity is being
      // performed.
      //------------------------------------------------------------------------

      void storeOptObjValue    (double);
      void storeOptBoundsValue (double);
         //
         // Valid only when the optimizing implosion subsystem is uploading the
         // solution.

      void storeStochObjValue    (double);
      void storeStochBoundsValue (double);
         //
         // Valid only when the stochastic implosion subsystem is uploading
         // data.

      void storeCplexStatusCode (int);
      void storeCplexStatusText (const char *);
      void storeCplexMipBound   (double);
      void storeCplexMipRelGap  (double);
         //
         // Valid only in cplexEmbedded mode.

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      virtual bool inserted       ();
      virtual void copySelfInto   (WitCompMgr * theCompMgr);
      virtual void recount        ();
      virtual void clearStochSoln ();
         //
         // Overrides from class Component.

      double objValue () const;
         //
         // In single objective mode:
         //    Returns the objective function value.
         // In multiple objectives mode:
         //    Returns the objective function value of the current Objective.

      void unpostprocess ();
         //
         // Does unpostprocessing for this OptComp.

      void display ();
         //
         // Displays the input and preprocessing data for this OptComp.

      void choose (WitOptStarter * theOptStarter);
         //
         // Sets optInitMethod_ to theOptStarter.
         // No-op (with warning), if an accOptStarter is requested and
         // myDetOptImpMgr () is not accelerated.

      void store (WitOptStarter * theOptStarter);
         //
         // Stores theOptStarter in this OptComp.

      bool negativeCostsExist ();
         //
         // Returns true, iff there is at least one cost coefficient < 0.

      void requireCoinOrCplex ();
         //
         // Verifies that COIN or CPLEX were embedded.

      void writeObjValue ();
         //
         // Writes the objValue attribute to the message file.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitOptComp);

      void setUpOptStarters ();
         //
         // Sets up the OptStarters for this OptComp.

      void checkAccAfter ();
         //
         // Issues a warning if accAfterSoftLB () is true and accAfterOptImp ()
         // is false.

      virtual void writeDataID    ();
      virtual void writeDataAttrs ();
         //
         // Overrides from class Component.

      void copyAttrsFrom (WitOptComp * theOptComp);
         //
         // Copies the input attributes of theOptComp into this OptComp.

      inline double & sglObjValue () const;
         //
         // Valid in single objective mode only.
         // Returns a reference to the main objective function value

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitOptStarter *  heurOptStarter_;
      WitOptStarter *   accOptStarter_;
      WitOptStarter * schedOptStarter_;
      WitOptStarter * crashOptStarter_;
         //
         // The OptStarters for this OptComp.

      WitList <WitOptStarter> allOptStarters_;
         //
         // The List of all possible OptStarters.
         // Specifically, this List contains one instance of each concrete
         // OptStarter class (stored abstractly as an OptStarter *).

      WitCpxParSpecMgr * myCpxParSpecMgr_;
         //
         // The CpxParSpecMgr owned by this OptComp.

      WitMultiObjMgr * myMultiObjMgr_;
         //
         // The MultiObjMgr owned by this OptComp in multiple objectives mode;
         // NULL, in single objective mode.

      //------------------------------------------------------------------------
      // Private input data.
      //------------------------------------------------------------------------

      bool compPrices_;
         //
         // true, iff shadow prices on parts are to be computed.

      bool accAfterOptImp_;
         //
         // true, iff opt implosion is to stay in an accelerated
         // state at the end of an implosion.
         // If false, opt implosion will shot down when it completes.

      bool accAfterSoftLB_;
         //
         // true, iff the Optimploder is in accelerated bounds mode.
         // If an DetOptImpMgr is in accelerated bounds mode, then arbitrary
         // changes can be made to a BoundSet without shutting down opt
         // implosion. If it is not in accelerated bounds mode, then whenever a
         // change is made to a BoundSet such that a lower bound that used to be
         // hard becomes soft, opt implosion is shut down. Other changes to a
         // BoundSet do not cause a shut down. The disadvantage of
         // accelerated bounds mode is that it causes a somewhat larger
         // optimization problem to be generated, which is somewhat slower to
         // solve.

      bool mipMode_;
         //
         // If true,  optimizing implosion will be solved as a MIP.
         // If false, optimizing implosion will be solved as an LP.

      bool multiObjMode_;
         //
         // True, iff WIT is in multiple objectives mode.

      int objChoice_;
         //
         // Obsolete attribute: has no effect.

      double wbounds_;
         //
         // Weight on the bounds objective.

      WitOptStarter * optInitMethod_;
         //
         // The OptStarter currently selected.

      WitString solverLogFileName_;
         //
         // Name of the solver log output file.

      //------------------------------------------------------------------------
      // Solution data.
      //------------------------------------------------------------------------

      double * sglObjValuePtr_;
         //
         // In single objective mode:
         //    Ptr to the main objective function value
         // In  multiple objectives mode:
         //    NULL

      double boundsValue_;
         //
         // Bounds objective function value.

      bool boundsValueValid_;
         //
         // true, iff the current value of boundsValue_ is valid.

      int cplexStatusCode_;
         //
         // If a CPLEX solve routine has been called, cplexStatusCode_ is the
         // solution status code resulting from the most recent call.
         // Otherwise cplexStatusCode_ is -1.

      WitString cplexStatusText_;
         //
         // If a CPLEX solve routine has been called, cplexStatusText_ is the
         // string indicating the solution status resulting from the most recent
         // call.

      double cplexMipBound_;
         //
         // If the CPLEX MIP solver has been invoked, cplexMipBound_ is the
         // tightest upper bound that it found on the optimal objective function
         // value.
         // If the CPLEX MIP solver has not been invoked, cplexMipBound_ is 0.0.

      double cplexMipRelGap_;
         //
         // If the CPLEX MIP solver has been invoked, cplexMipRelGap_ is the
         // relative objective function value gap for the solution:
         //    (cplexMipBound_ - objValue) / (1.e-10 + |cplexMipBound_|)
         // Otherwise cplexMipRelGap_ is -1.0.

      //------------------------------------------------------------------------
      // Development mode control parameters.
      //------------------------------------------------------------------------

      bool printOptProblem_;
         //
         // Print the optimization problem.

      bool printMatByCols_;
         //
         // Print the matrix by columns.

      bool printInitSoln_;
         //
         // Print the initial optimization solution.

      bool printOptSoln_;
         //
         // Print the final optimization solution.

      bool printMps_;
         //
         // Print an MPS file.

      bool skipScreening_;
         //
         // Skip screening of the opt problem.
   };

#endif
