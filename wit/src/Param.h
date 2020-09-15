//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef ParamH
#define ParamH

//------------------------------------------------------------------------------
// Header file: "Param.h"
//
// Contains the declaration of the following classes:
//
//    Param
//    StringParam
//    IntParam
//    DoubleParam
//    ChoiceParam
//------------------------------------------------------------------------------

#include <Str.h>
#include <List.h>

class WitParamMgr;

//------------------------------------------------------------------------------
// class Param
//
// A control parameter to stand-alone WIT.
//
// Class Hierarchy:
//
// Param
//    StringParam
//    IntParam
//    DoubleParam
//    ChoiceParam
//------------------------------------------------------------------------------

class WitParam
   {
   public:

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitParam ();

      //------------------------------------------------------------------------
      // Public pure virtual member functions.
      //------------------------------------------------------------------------

      virtual void parse (const WitString & theToken) = 0;
         //
         // Parses theToken and sets the value of this Param accordingly.

      virtual void print () = 0;
         //
         // Prints myName_, myText_, and the value of this Param.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (const WitString &, myName)
      accessFunc (WitMsgFacility &,  myMsgFac)

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      static const WitString & itsName (const WitParam * theParam);
         //
         // Return theParam->myName ();

   protected:

      //------------------------------------------------------------------------
      // Protected member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitParam (
            const char *  theName,
            const char *  theText,
            WitParamMgr * theParamMgr);
         //
         // myName_     <-- theName.
         // myText_     <-- theText.
         // myParamMgr_ <-- theParamMgr.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (const WitString &, myText)
      accessFunc (WitParamMgr *,     myParamMgr)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitParam);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      const WitString myName_;
         //
         // The name of this Param.
         // See ctor for length limit.

      const WitString myText_;
         //
         // Text describing the meaning of this Param.
         // See ctor for length limit.

      WitParamMgr * const myParamMgr_;
         //
         // The ParamMgr that owns this Param.

      WitMsgFacility & myMsgFac_;
         //
         // The MsgFacility for this Param.
   };

//------------------------------------------------------------------------------
// class StringParam
//
// A String-valued control parameter.
//
// Class Hierarchy:
//
// Param
//    StringParam
//------------------------------------------------------------------------------

class WitStringParam: public WitParam
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitStringParam (
            const char *  theName,
            const char *  theText,
            const char *  initValue,
            WitParamMgr * theParamMgr);
         //
         // myValue_ <-- initValue

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitStringParam ();

      //------------------------------------------------------------------------
      // Public overiding virtual member functions.
      //------------------------------------------------------------------------

      virtual void parse (const WitString & theToken);

      virtual void print ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (const WitString &, myValue)

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      static const WitString & itsName (const WitStringParam * theStringParam);
         //
         // Return theStringParam->myName ();

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitStringParam);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitString myValue_;
         //
         // The "value" of this StringParam.
   };

//------------------------------------------------------------------------------
// class IntParam
//
// An int-valued control parameter.
//
// Class Hierarchy:
//
// Param
//    IntParam
//------------------------------------------------------------------------------

class WitIntParam: public WitParam
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitIntParam (
            const char *  theName,
            const char *  theText,
            int           theLB,
            int           theUB,
            int           initValue,
            WitParamMgr * theParamMgr);
         //
         // lb_      <-- theLB.
         // ub_      <-- theUB.
         // myValue_ <-- initValue

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitIntParam ();

      //------------------------------------------------------------------------
      // Public overiding virtual member functions.
      //------------------------------------------------------------------------

      virtual void parse (const WitString & theToken);

      virtual void print ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (int, myValue)

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      static const WitString & itsName (const WitIntParam * theIntParam);
         //
         // Return theIntParam->myName ();

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitIntParam);

      bool lbSatisfied (int theValue);
         //
         // Returns true, iff theValue satisfies the lower bound on this
         // IntParam.

      bool ubSatisfied (int theValue);
         //
         // Returns true, iff theValue satisfies the upper bound on this
         // IntParam.

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      const int lb_;
      const int ub_;
         //
         // Lower and upper bounds on the valid values.
         // In both cases, a negative number means no bound.

      int myValue_;
         //
         // The "value" of this IntParam.
   };

//------------------------------------------------------------------------------
// class DoubleParam
//
// A double-valued control parameter.
//
// Class Hierarchy:
//
// Param
//    DoubleParam
//------------------------------------------------------------------------------

class WitDoubleParam: public WitParam
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitDoubleParam (
            const char *  theName,
            const char *  theText,
            double        theLB,
            double        initValue,
            WitParamMgr * theParamMgr);
         //
         // lb_      <-- theLB.
         // myValue_ <-- initValue

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitDoubleParam ();

      //------------------------------------------------------------------------
      // Public overiding virtual member functions.
      //------------------------------------------------------------------------

      virtual void parse (const WitString & theToken);

      virtual void print ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (double, myValue)

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      static const WitString & itsName (const WitDoubleParam * theDoubleParam);
         //
         // Return theDoubleParam->myName ();

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitDoubleParam);

      bool lbSatisfied (double theValue);
         //
         // Returns true, iff theValue satisfies the lower bound on this
         // DoubleParam.

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      const double lb_;
         //
         // Lower bound on the valid values.
         // A negative number means no bound.

      double myValue_;
         //
         // The "value" of this DoubleParam.
   };

//------------------------------------------------------------------------------
// class ChoiceParam
//
// A multiple-choice-valued control parameter.
// I.e., the value is a String that is an element of a List of valid values.
//
// Class Hierarchy:
//
// Param
//    ChoiceParam
//------------------------------------------------------------------------------

class WitChoiceParam: public WitParam
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitChoiceParam (
            const char *  theName,
            const char *  theText,
            WitParamMgr * theParamMgr);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitChoiceParam ();

      //------------------------------------------------------------------------
      // Public overiding virtual member functions.
      //------------------------------------------------------------------------

      virtual void parse (const WitString & theToken);

      virtual void print ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      inline const WitString & myValue ()
         {
         witAssert (myValue_ != NULL);

         return * myValue_;
         }
            //
            // Returns the value of this ChoiceParam.
            // An initial value must be set prior to invoking this function.

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void addChoice (const char * theChoice);
         //
         // Adds theChoice to allChoices_.

      void choose (const char * theChoice);
         //
         // Sets the value of this ChoiceParam to theChoice.
         // Should only be invoked after addChoice has been used.

      static const WitString & itsName (const WitChoiceParam * theChoiceParam);
         //
         // Return theChoiceParam->myName ();

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitChoiceParam);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitList <WitString> allChoices_;
         //
         // The List of all valid choices for this ChoiceParam.

      const WitString * myValue_;
         //
         // The "value" of this ChoiceParam, an element of allChoices_.
   };

#endif
