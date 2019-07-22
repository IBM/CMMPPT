//==============================================================================
// IBM Confidential
//
// OCO Source Materials
//
// Message Class Library
//
// (C) Copyright IBM Corp. 1997, 2005  All Rights Reserved
//
// The Source code for this program is not published or otherwise divested of
// its trade secrets, irrespective of what has been deposited with the U. S.
// Copyright office.
//==============================================================================

#ifndef mcl_unitTest_h
#define mcl_unitTest_h

//-----------------------------------------------------------------------------
// File unitTest.h
//
// Contains the declaration of the following classes:
//
// Tester
// TestOutputDisplayer
// TestMsg
//-----------------------------------------------------------------------------

#include <mcl/src/msg.h>
#include <mcl/src/output.h>

//-----------------------------------------------------------------------------
// class Tester.
//
// Responsible for performing a unit test of MCL.
//-----------------------------------------------------------------------------

class MclTester
   {
   public:

      MclTester ();

      ~MclTester ();

      void unitTest ();
         //
         // Performs a unitTest of MCL.

   private:

      MclTester             (const MclTester &);
      MclTester & operator= (const MclTester &);
         //
         // Copy ctor and assignment are declared private and not implemented
         // so as to prevent inadvertant copying and assignment.

      //-----------------------------------------------------------------------
      // Test functions.
      //-----------------------------------------------------------------------

      void testArg ();

      void testStringArg ();

      void testArgList ();

      void testMsgUnit ();

      void testMsg ();

      void testMsgFrag ();

      void testLevel ();

      void testDictEl ();

      void testMsgUnitSet ();

      void testOutputDisplayer ();

      void testMsgUnitItr ();

      void testFacility ();

      //-----------------------------------------------------------------------

      void testingMessage (const char * msg);
         //
         // Display message on stdout.

      std::string valueText (const MclArg & theArg, const MclMsg * theMsg) const;
         //
         // Returns a formatted string representing theArg for testing
         // purposes. Uses theMsg as a context.

      void match (const char * charStar1, const char * charStar2);
         //
         // Asserts that charStar1 and charStar2 match.

      //-----------------------------------------------------------------------
      // Data.
      //-----------------------------------------------------------------------
   };

//-----------------------------------------------------------------------------
// class TestOutputDisplayer
//
// An output displayer designed for testing MCL.
// Output sent to an instance of this class is sent to stdout and then stored
// for verification.
//-----------------------------------------------------------------------------

class MclTestOutputDisplayer: public MclOutputDisplayer
   {
   friend class MclTester;

   public:

      MclTestOutputDisplayer ();
     
         //
         // Default ctor.

      virtual ~MclTestOutputDisplayer ();
         //
         // Destructor.

      virtual void output (const char * finalText);
         //
         // Prints finalText to stdout and stores it.

      virtual void outputError (const char * finalText);
         //
         // Prints finalText to stdout and stores it.

      void reqText (const char * theText, MclBoolean isErrorText);
         //
         // Verifies the following:
         //
         //   outputText_     == theText.
         //   outputErrorText == theText, if isErrorText
         //                      "",      else

   private:

      MclTestOutputDisplayer             (const MclTestOutputDisplayer &);
      MclTestOutputDisplayer & operator= (const MclTestOutputDisplayer &);
         //
         // Copy ctor and assigment are is declared private and not implemented
         // so as to avoid inadvertant copying and assignment.

      void testText (
            const char *      textName,
            const std::string & actualText,
            const char *      expectedText);
         //
         // Verifies that actualText == expectedText and issues a fatal error
         // if not.

      //-----------------------------------------------------------------------
      // Data.
      //-----------------------------------------------------------------------

      std::string outputText_;
         //
         // The text most recently passed to output.

      std::string outputErrorText_;
         //
         // If outputError has been called more recently then output, this data
         // is the text that was passed to outputError; otherwise it is the
         // null string.
   };

//-----------------------------------------------------------------------------
// class TestMsg
//
// A Msg designed for testing MCL.
//-----------------------------------------------------------------------------

class MclTestMsg: public MclMsg
   {
   public:

      MclTestMsg (
            MclFacility &,
            const char *,
            const MclLevel &,
            const char *,
            int);
         //
         // The arguments are just passed to the Msg ctor.

      virtual ~MclTestMsg ();
         //
         // Destructor.

   private:

      MclTestMsg             (const MclTestMsg &);
      MclTestMsg & operator= (const MclTestMsg &);
         //
         // Copy ctor and assigment are is declared private and not implemented
         // so as to avoid inadvertant copying and assignment.

      virtual void postIssue ();
   };

#endif
