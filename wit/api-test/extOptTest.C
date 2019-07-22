//------------------------------------------------------------------------------
// Source File: "extOptTest.C".
//
// A WIT application program whose purpose is to test WIT's "external optimizing
// implosion" capability.
//------------------------------------------------------------------------------

#include <wit.h>

#include <cplex.h>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <float.h>

//------------------------------------------------------------------------------
// Class ExtOptTester
//------------------------------------------------------------------------------

class ExtOptTester
   {
   public:

   //---------------------------------------------------------------------------
   // Public Member Functions
   //---------------------------------------------------------------------------

    ExtOptTester ();
   ~ExtOptTester ();

   void testExtOpt ();

   private:

   //---------------------------------------------------------------------------
   // Private Member Functions
   //---------------------------------------------------------------------------

   void buildProblem      ();
   void getMipDataFromWit ();
   void compRhsSense      ();
   void compMatCnt        ();
   void compXctype        ();
   void makeCplexCalls    ();
   void prtRemFile        (const char *);
   void shutDown          ();

   //---------------------------------------------------------------------------
   // Private Member Data
   //---------------------------------------------------------------------------

   WitRun * myWitRun_;

   //------------------
   // MIP Data from WIT
   //------------------

   int      numcols_;
   int      numrows_;
   int *    start_;
   int *    index_;
   double * value_;
   double * collb_;
   double * colub_;
   double * obj_;
   double * rowlb_;
   double * rowub_;
   int *    intIndices_;
   int      nIntVars_;

   //------------------------
   // Data Computed for CPLEX
   //------------------------

   double * rhs_;
   char *   sense_;
   int *    matcnt_;
   char *   xctype_;

   //-------------------------
   // Solution Data from CPLEX
   //-------------------------

   double * x_;
   };

//------------------------------------------------------------------------------
// Main Program
//------------------------------------------------------------------------------

int main ()
   {
   ExtOptTester theTester;

   theTester.testExtOpt ();
   }

//------------------------------------------------------------------------------
// Implementation of class ExtOptTester
//------------------------------------------------------------------------------

ExtOptTester::ExtOptTester ()
   {
   }

//------------------------------------------------------------------------------

ExtOptTester::~ExtOptTester ()
   {
   }

//------------------------------------------------------------------------------

void ExtOptTester::testExtOpt ()
   {
   int scrapVarIndex;

   witNewRun (& myWitRun_);

   buildProblem ();

   witStartExtOpt (myWitRun_);

   getMipDataFromWit ();

   compRhsSense ();

   compMatCnt ();

   compXctype ();

   witGetPartScrapVarIndex (myWitRun_, "C", 0, & scrapVarIndex);

   collb_[scrapVarIndex] = 4.0;

   makeCplexCalls ();

   witSetExtOptSolnDbl (myWitRun_, x_);

   witFinishExtOpt     (myWitRun_);

   witWriteExecSched   (myWitRun_, "exec.out", WitBSV);
   prtRemFile                     ("exec.out");

   witWriteShipSched   (myWitRun_, "ship.out", WitBSV);
   prtRemFile                     ("ship.out");

   shutDown ();
   }

//------------------------------------------------------------------------------

void ExtOptTester::buildProblem ()
   {
   float consRateD0 [] =  {2.0};

   float supplyVolA [] = {79.0};

   float demandVolF [] = {30.0};
   float demandVolG [] = {60.0};

   float shipRewardF[] =  {3.0};
   float shipRewardG[] =  {2.0};

   witInitialize              (myWitRun_);

   witSetNPeriods             (myWitRun_, 1);
   witSetMipMode              (myWitRun_, WitTRUE);

   witAddPart                 (myWitRun_, "A",      WitMATERIAL);
   witAddPart                 (myWitRun_, "B",      WitMATERIAL);
   witAddPart                 (myWitRun_, "C",      WitMATERIAL);

   witAddOperation            (myWitRun_, "D");
   witAddOperation            (myWitRun_, "E");

   witAddBomEntry             (myWitRun_, "D", "A");
   witAddBomEntry             (myWitRun_, "E", "A");

   witAddBopEntry             (myWitRun_, "D", "B");
   witAddBopEntry             (myWitRun_, "E", "C");

   witAddDemand               (myWitRun_, "B", "F");
   witAddDemand               (myWitRun_, "C", "G");

   witSetBomEntryConsRate     (myWitRun_, "D",  0,  consRateD0);

   witSetOperationIntExecVols (myWitRun_, "D",      WitTRUE);
   witSetOperationIntExecVols (myWitRun_, "E",      WitTRUE);

   witSetPartSupplyVol        (myWitRun_, "A",      supplyVolA);

   witSetDemandDemandVol      (myWitRun_, "B", "F", demandVolF);
   witSetDemandDemandVol      (myWitRun_, "C", "G", demandVolG);

   witSetDemandShipReward     (myWitRun_, "B", "F", shipRewardF);
   witSetDemandShipReward     (myWitRun_, "C", "G", shipRewardG);
   }

//------------------------------------------------------------------------------

void ExtOptTester::getMipDataFromWit ()
   {
   witGetExtOptLpProbDbl (
        myWitRun_,
      & numcols_,
      & numrows_,
      & start_,
      & index_,
      & value_,
      & collb_,
      & colub_,
      & obj_,
      & rowlb_,
      & rowub_);

   witGetExtOptIntVarIndices (
        myWitRun_,
      & intIndices_,
      & nIntVars_);
   }

//------------------------------------------------------------------------------

void ExtOptTester::compRhsSense ()
   {
   int theIdx;

   rhs_   = new double[numrows_];
   sense_ = new char  [numrows_];

   for (theIdx = 0; theIdx < numrows_; theIdx ++)
      {
      if (rowlb_[theIdx] == rowub_[theIdx])
         {
         rhs_  [theIdx] = rowlb_[theIdx];
         sense_[theIdx] = 'E';
         }
      else if (rowub_[theIdx] == DBL_MAX)
         {
         rhs_  [theIdx] = rowlb_[theIdx];
         sense_[theIdx] = 'G';
         }
      else if (rowlb_[theIdx] == DBL_MAX)
         {
         rhs_  [theIdx] = rowub_[theIdx];
         sense_[theIdx] = 'L';
         }
      else
         {
         assert (false);
         }
      }
   }

//------------------------------------------------------------------------------

void ExtOptTester::compMatCnt ()
   {
   int theIdx;

   matcnt_ = new int[numcols_];

   for (theIdx = 0; theIdx < numcols_; theIdx ++)
      {
      matcnt_[theIdx] = start_[theIdx + 1] - start_[theIdx];
      }
   }

//------------------------------------------------------------------------------

void ExtOptTester::compXctype ()
   {
   int theIdx;

   xctype_ = new char[nIntVars_];

   for (theIdx = 0; theIdx < nIntVars_; theIdx ++)
      {
      xctype_[theIdx] = 'I';
      }
   }

//------------------------------------------------------------------------------

void ExtOptTester::makeCplexCalls ()
   {
   CPXENVptr theCpxEnv;
   CPXLPptr  theCpxLp;
   int       theErrCode;
   int       theStatCode;

   theCpxEnv = CPXopenCPLEX (& theErrCode);

   assert (theErrCode == 0);

   assert (theCpxEnv != NULL);

   theCpxLp = CPXcreateprob (theCpxEnv, & theErrCode, "Ext-Opt-Test");

   assert (theErrCode == 0);

   assert (theCpxLp != NULL);

   theErrCode =
      CPXcopylp (
         theCpxEnv,
         theCpxLp,
         numcols_,
         numrows_,
         -1,
         obj_,
         rhs_,
         sense_,
         start_,
         matcnt_,
         index_,
         value_,
         collb_,
         colub_,
         NULL);
         
   assert (theErrCode == 0);

   theErrCode =
      CPXchgctype (theCpxEnv, theCpxLp, nIntVars_, intIndices_, xctype_);

   assert (theErrCode == 0);

   theErrCode = CPXwriteprob (theCpxEnv, theCpxLp, "app-opt.mps", NULL);

   assert (theErrCode == 0);

   prtRemFile ("app-opt.mps");

   theErrCode = CPXmipopt (theCpxEnv, theCpxLp);

   assert (theErrCode == 0);

   theStatCode = CPXgetstat (theCpxEnv, theCpxLp);

   assert (theStatCode == CPXMIP_OPTIMAL);

   x_ = new double[numcols_];

   theErrCode = CPXgetx (theCpxEnv, theCpxLp, x_, 0, numcols_ - 1);

   assert (theErrCode == 0);

   theErrCode = CPXfreeprob (theCpxEnv, & theCpxLp);

   assert (theErrCode == 0);

   theErrCode = CPXcloseCPLEX (& theCpxEnv);

   assert (theErrCode == 0);
   }

//------------------------------------------------------------------------------

void ExtOptTester::prtRemFile (const char * fileName)
   {
   char command[101] = "";

   printf (
      "\n"
      "------------------------------\n"
      "File \"%s\":\n"
      "------------------------------\n"
      "\n",
      fileName);

   fflush (stdout);

   sprintf (command, "cat %s", fileName);

   system (command);

   printf (
      "\n"
      "------------------------------\n"
      "End of file \"%s\".\n"
      "------------------------------\n"
      "\n",
      fileName);

   remove (fileName);
   }

//------------------------------------------------------------------------------

void ExtOptTester::shutDown ()
   {
   free (start_);
   free (index_);
   free (value_);
   free (collb_);
   free (colub_);
   free (obj_);
   free (rowlb_);
   free (rowub_);
   free (intIndices_);

   delete [] rhs_;
   delete [] sense_;
   delete [] matcnt_;
   delete [] xctype_;
   delete [] x_;

   witDeleteRun (myWitRun_);
   }
