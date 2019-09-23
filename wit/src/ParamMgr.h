//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef ParamMgrH
#define ParamMgrH

//------------------------------------------------------------------------------
// Header file: "ParamMgr.h"
//
// Contains the declaration of class ParamMgr.
//------------------------------------------------------------------------------

#include <Str.h>
#include <List.h>

//------------------------------------------------------------------------------
// Class ParamMgr
//
// Responsible for owning all of the Params for an SaeMgr.
//------------------------------------------------------------------------------

class WitParamMgr
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitParamMgr (WitRun * theWitRun, const char * theFileName);
         //
         // theFileName is the parameter file name, if any.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitParamMgr ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitMsgFacility &,  myMsgFac)
      accessFunc (const WitString &, fileName)

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void procParams ();
         //
         // Builds, reads, applies, and prints the control paramaters.

      void store (WitParam * theParam);
         //
         // Stores theParam in the Param Lists and checks for duplication.

      const WitString & stringParamVal (const char *);
      int               intParamVal    (const char *);
      double            doubleParamVal (const char *);
      const WitString & choiceParamVal (const char *);
      bool              boolParamVal   (const char *);
         //
         // Each of these functions returns the value of the (typed) Param
         // whose name matches the argument and asserts that a match exists.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitParamMgr);

      const char * compParamIFName (const char * theFileName);
         //
         // Computes and returns the input file name.
         // theFileName is the parameter file name, if any.

      void buildNonDevParams ();
         //
         // Builds all the non-development mode Params for this ParamMgr.

      void buildDevParams ();
         //
         // Builds all the development mode Params for this ParamMgr.

      void buildActionParam ();
         //
         // Builds the action Param.

      void buildOptInitMethodParam ();
         //
         // Builds the optInitMethod Param.

      void buildStringParam (
            const char * theName,
            const char * theText,
            const char * initValue);
         //
         // Constructs a StringParam with the given arguments.

      void buildIntParam (
            const char * theName,
            const char * theText,
            int          theLB,
            int          theUB,
            int          initValue);
         //
         // Constructs an IntParam with the given arguments.

      void buildDoubleParam (
            const char * theName,
            const char * theText,
            double       theLB,
            double       initValue);
         //
         // Constructs an DoubleParam with the given arguments.

      WitChoiceParam * buildChoiceParam (
            const char * theName,
            const char * theText);
         //
         // Constructs a ChoiceParam with the given arguments.
         // Returns the contructed ChoiceParam so that valid and initial values
         // can be set.

      void buildBoolParam (
            const char * theName,
            const char * theText,
            bool         initValue);
         //
         // Constructs a Bool ChoiceParam with the given arguments.

      void readParams ();
         //
         // Reads the Params.

      void applyNonDevParams ();
         //
         // Takes whatever immediate action is implied by the values of the
         // Params.

      void applyDevParams ();
         //
         // Takes whatever immediate action is implied by the values of the
         // Params.

      void printParams ();
         //
         // Prints the Params.
         // Prints devParams only in development mode.

      void getTempParName (int theIdx, WitString & theString);
         //
         // Sets theString to be the name of tempPar #theIdx.

      void getTempParText (int theIdx, WitString & theString);
         //
         // Sets theString to be the text of tempPar #theIdx.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      inline WitGlobalComp * myGlobalComp ();
      inline WitOptComp *    myOptComp    ();
      inline WitProblem *    myProblem    ();

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitRun * const myWitRun_;
         //
         // The WitRun for this ParamMgr.

      WitMsgFacility & myMsgFac_;
         //
         // The MsgFacility for this ParamMgr.

      const WitString fileName_;
         //
         // Name of the parameter file.

      WitList <      WitParam>       allParams_;
      WitList <      WitParam> allNonDevParams_;
      WitList <      WitParam>    allDevParams_;

      WitList <WitStringParam> allStringParams_;
      WitList <   WitIntParam>    allIntParams_;
      WitList <WitDoubleParam> allDoubleParams_;
      WitList <WitChoiceParam> allChoiceParams_;
      WitList <WitChoiceParam>   allBoolParams_;
         //
         // The List of all                   Params for this ParamMgr.
         //                      non-dev-mode Params
         //                          dev-mode Params
         //
         //                             StringParams
         //                                IntParams
         //                             DoubleParams
         //                    non-Bool ChoiceParams
         //                        Bool ChoiceParams
         //
         // The Params are grouped into dev-mode Params and non-dev-mode Params
         // Dev-mode Params are only read and printed if devMode () is true.
         // When devMode () is false, they are frozen at their
         // default values (but still valid). Non-dev-mode Params are read and
         // printed unconditionally. Only the non-dev-mode Params are
         // documented in the WIT guide.
         //
         // Notice that the Choice Params that belong to this ParamMgr are
         // grouped into two Lists: Bool and non-Bool. The Bool
         // ChoiceParams are ChoiceParams with valid values "yes" and "no".
         // They are built using buildBoolParam () and accessed using
         // boolParamVal (), which returns a Bool. The non-Bool
         // ChoiceParams are just treated as ChoiceParams.

      WitList <WitParam> * pCurrentParams_;
         //
         // Pointer to:
         //    allNonDevParams_,
         //       if non-dev-mode Params are currently being built.
         //    allDevParams_,
         //       if dev-mode     Params are currently being built.
         //    NULL,
         //       else.
   };

#endif
