//==============================================================================
// IBM Confidential
//
// OCO Source Materials
//
// Watson Implosion Technology
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//
// The Source code for this program is not published or otherwise divested of
// its trade secrets, irrespective of what has been deposited with the U. S.
// Copyright office.
//==============================================================================

#ifndef DataWritH
#define DataWritH

//------------------------------------------------------------------------------
// Header file: "DataWrit.h"
//
// Contains the declaration of class DataWriter.
//------------------------------------------------------------------------------

#include <Component.h>
#include <Str.h>

//------------------------------------------------------------------------------
// class DataWriter
//
// Responsible for writing the input data file.
//------------------------------------------------------------------------------

class WitDataWriter: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitDataWriter (WitProblem * theProblem);
         //
         // ProbAssoc <<< theProblem.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitDataWriter ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void writeData (const char * fileName);
         //
         // Writes an input data file for myProblem ().
         // fileName is the name of the file to be written.

      void writeEndCommand ();
         //
         // Writes the semicolon at the end of a command in the input data
         // output file.

      void writeFirstKeyWord (const char * theKeyWord);
         //
         // Writes theKeyWord on a new line.

      void writeKeyWord (const char * theKeyWord);
         //
         // Writes theKeyWord on the current line.

      void writeIntValue (int theInt);
         //
         // Writes theInt.

      void writeStringValue (const WitString & theString);
         //
         // Writes theString.

      void writeCstringValue (const char * theCstring);
         //
         // Writes theCstring.

      //------------------------------------------------------------------------
      // writeVector (attrName, theVector, defaultVal) functions:
      //
      // Each of these functions writes the value of theVector to the input data
      // output file, or no-op, if the attribute is at its default value.
      // attrName is the name of the attribute.
      // defaultVal is the default value of the attribute.
      //------------------------------------------------------------------------

      void writeVector (
         const char *              attrName,
         const WitFlexVec <bool> & theVector,
         bool                      defaultVal);

      void writeVector (
         const char *              attrName,
         const WitTVec    <int> &  theVector,
         int                       defaultVal);

      void writeVector (
         const char *              attrName,
         const WitFlexVec <int> &  theVector,
         int                       defaultVal);

      void writeVector (
         const char *              attrName,
         const WitDblFlexVec &     theVector,
         double                    defaultVal);

      void writeVector (
         const char *              attrName,
         const WitTVec <double> &  theVector,
         double                    defaultVal);

      //------------------------------------------------------------------------
      // Each of these functions writes an attribute iff its current value is
      // different from its default value.
      //------------------------------------------------------------------------

      void writeDouble (
            const char *      attrName,
            double            theValue,
            double            defaultVal);

      void writeInt (
            const char *      attrName,
            int               theValue,
            int               defaultVal);

      void writeBool (
            const char *      attrName,
            bool              theValue, 
            bool              defaultVal);

      void writeString (
            const char *      attrName,
            const WitString & theValue,
            const char *      defaultVal);

      //------------------------------------------------------------------------
      // Each of these functions writes an attribute unconditionally.
      //------------------------------------------------------------------------

      void writeDouble (const char * attrName, double            theValue);
      void writeInt    (const char * attrName, int               theValue);
      void writeBool   (const char * attrName, bool              theValue);
      void writeString (const char * attrName, const WitString & theValue);

      //------------------------------------------------------------------------

      void writeObjVec (
            const char *      attrName,
            const WitObjVec & theObjVec,
            double            defaultVal);
         //
         // Write for an ObjVec.
         // No-op in Multi-Objective mode.

      void writeSolnData (const char * fileName);
         //
         // Writes the solution data file for myProblem ().
         // fileName is the name of the file to be written.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitDataWriter);

      void writeData (const char * fileName, bool solnData);
         //
         // Writes an input data file for myProblem ().
         // fileName is the name of the file to be written.
         // If solnData is true,  the solution data will be written.
         // If solnData is false, the problem  data will be written.

      const char * withSlashes (const char * theCstring);
         //
         // Returns a c-string whose content is theCstring with a "\" in front
         // of each special char.

      inline static bool isSpecial (char theChar)
         {
         return ((theChar == '\"') or (theChar == '\\'));
         }
         //
         // Returns true, iff theChar is "special", i.e., one of the characters
         // that must be preceeded by a "\" in the file format.

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitComponent * curComp_;
         //
         // The Component currently being written, if any; else NULL.

      WitString buffer_;
         //
         // A buffer for expanding Strings with "\"s.

      bool keyWordWritten_;
         //
         // True, iff at least one key word has been written into the file.
   };

#endif
