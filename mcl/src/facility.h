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

#ifndef mcl_facility_h
#define mcl_facility_h

//-----------------------------------------------------------------------------
// File facility.h
//
// Contains the declaration of the following classes:
//
// Facility
// MsgUnitItr
//-----------------------------------------------------------------------------

#include <mcl/src/defs.h>
#include <string>
#include <map>

//-----------------------------------------------------------------------------

class MclArgList;
class MclMsgUnit;
class MclMsg;
class MclMsgFrag;
class MclLevel;
class MclOutputDisplayer;
class MclMsgUnitSet;

//-----------------------------------------------------------------------------
// class MclFacility
//
// Notes on Usage:
//
// * The MclFacility serves as the public interface for the
//   Message Class Library.  It serves as a container for messages and for
//   the methods which issue them.
//
// * For concrete examples of usage, see the file: unitTest.C
//
// * General Steps of Implementation:
//
//   1) First, create an instance of the a class derived from the 
//      MclOutputDisplayer class (ie: MclPrintfOutputDisplayer)
//
//       MclPrintfOutputDisplayer myMsgDisplayer("myMsgFileName","w+");
//                                                      |         |
//        <STRING> messages are printed to this file ----         |
//        {r(+),w(+),a(+)} open method for file - C convention ----
//
//   2) Second, create an instance of the MclFacility class with a call
//      that should resemble the following:
//
//       MclFacility myMsgFacility("XYZ", myMsgDisplayer);
//                                   |           |
//       <STRING> Three character prefix         |
//       <MclOutputDisplayer-DERIVED class> name of the displayer object
//
//   3) Now, once the facility has been created, messages can be created.
//      Note: they are "added" in the sense that the constructors for the
//            messages require an MclFacility object -- they belong to the
//            facility with which it is constructed.
//
//       new MclMsg(myMsgFacility,    <-- the MclFacility object to which
//                                        this message will belong
//                  "MESSAGE_1",      <-- <STRING> the "name" of this message
//                                        which is not printed, but is used
//                                        when issuing.
//                  MclLevel::info(), <-- this is returns a pointer to one
//                                        member of a set of "MclLevel"s
//                                        (see level.h, level.C for more)
//                  "This is Message number %1$d", <-- <STRING> this is the
//                                                  frame of the message using
//                                                  "printf"-type syntax.
//                  101);             <-- <INT> an external number associated
//                                        with this message. Can be printed 
//                                        when message is issued, or can be
//                                        "turned off" with method:
//                                   MclFacility::displayExternalNumber( . . )
//
//   4) After all the desired messages have been added, they can be issued
//      by invoking a method of an MclFacility object, using the parentheses
//      operator.
//
//       myMsgFacility("MESSAGE_1",MclArgList() << 1234);
//                         |       \__________________/
//        <STRING> a message id              |
//        <MclArgList object> this seems to be the most compact way to pass
//                            an argument list to the facility in this method
//                            but there are other ways to create such lists.
//-----------------------------------------------------------------------------

class MclFacility
{
 friend class MclMsgUnitItr;
 friend class MclTester;

public:

  MclFacility(const char* prefix, MclOutputDisplayer& theOutputDisplayer);

  ~MclFacility();

  void insert (MclMsgUnit * msgUnit);
     //
     // adds a message unit to myMsgUnits

  void operator()(const char * msgId, const MclArgList&);

  void operator()(const char * msgId);

  //---------------------------------------------------------------------------
  // Access functions.
  //---------------------------------------------------------------------------

  const char * prefix () const;
     //
     // Returns prefix_.

  const char * booleanText (MclBoolean theValue) const;
     //
     // Returns the text representing theValue.

  int lineLength () const;
     //
     // Returns lineLength.

  inline const MclLevel & minErrOutLevel () const
     {
     return * minErrOutLevelPtr_;
     }

  MclBoolean deletingMsgUnits () const;
     //
     // Returns TRUE, iff this Facility is currently deleting its MsgUnits.

  //---------------------------------------------------------------------------

  MclMsgUnit * findMsgUnit (const char * theId);
     //
     // Returns the MsgUnit in this Facility whose id_ matches theId, if one
     // exists; otherwise returns NULL.

  MclMsg * findMsg (const char * theId);
     //
     // Returns the Msg in this Facility whose id_ matches theId, if one
     // exists; otherwise returns NULL.

  MclMsg * findMsg (int theExtNumber);
     //
     // Returns the Msg in this Facility whose externalNumber_ matches
     // theExtNumber, if one exists; otherwise returns NULL.

  MclMsgFrag * findMsgFrag (const char * theId);
     //
     // Returns the MsgFrag in this Facility whose id_ matches theId, if one
     // exists; otherwise returns NULL.

  int maxSeverityOfMsgsIssued();
  //
  // find out the worst severity level of messages being isuued.
  
  void output      (const char * finalText);
     //
     // Performs a normal output of the finalTest of a Msg.

  void outputError (const char * finalText);
     //
     // Performs an error output of the finalTest of a Msg.

  static void forbidNullArg (
        const void * thePtr,
        const char * argName,
        const char * funcName);
     //
     // If thePtr is NULL, aborts, indicating funcName as the function name
     // and argName as the NULL pointer argument name.

  static void calledByClientError (const char * funcName);
     //
     // Aborts, indicating that an inappropriate function was called by the
     // client.
     // funcName: function name

  static void abortMcl ();
     //
     // Aborts the program.

  void booleanMsgFrags (
        const MclMsgFrag *  trueFrag,
        const MclMsgFrag * falseFrag);
     //
     // Sets booleanText values to the text of the arguments.

  void lineLength (int theValue);

  void minErrOutLevel (const MclLevel & theLevel);
     //
     // Sets minErrOutLevel () to theLevel.

  //---------------------------------------------------------------------------
  // Attribute set-by-Level functions.
  //
  // Each of these functions sets the named message attribute to theValue for
  // each Msg in this Facility associated with theLevel and for which
  // allowedToChange () is TRUE.
  //---------------------------------------------------------------------------

  void maxTimesIssued        (const MclLevel & theLevel, int        theValue);
  void displayExternalNumber (const MclLevel & theLevel, MclBoolean theValue);

private:

  MclFacility             (const MclFacility & );
  MclFacility & operator= (const MclFacility &);
     //
     // Copy ctor and assignment are declared private and not implemented so as
     // to avoid inadvertant copying and assignment.

  //---------------------------------------------------------------------------
  
   static const char * const proprietaryStmt_;
      //
      // The proprietary statement for MCL.
      // The purpose of this data member is to cause the MCL object library file
      // (e.g., libmcl.a) to contain MCL's propretary statement as a sequence of
      // text characters.

  int lineLength_;

  std::string prefix_;
  
  MclMsgUnitSet & myMsgUnitSet_;

  MclOutputDisplayer & myOutDisp_;

  std::string trueText_;
  std::string falseText_;
     //
     // Text used to implement the 'b' format.

  const MclLevel * minErrOutLevelPtr_;
     //
     // A Msg will use errorOutput, iff
     // theMsg->myLevel () >= * minErrOutLevelPtr_.

  MclBoolean deletingMsgUnits_;
     //
     // TRUE, iff this Facility is currently deleting its MsgUnits.
};

//-----------------------------------------------------------------------------
// Class MsgUnitItr.
//
// Allows iteration through the set of all Msgs owned by a Facility.
//-----------------------------------------------------------------------------

class MclMsgUnitItr
   {
   public:

      //-----------------------------------------------------------------------
      // Constructors
      //-----------------------------------------------------------------------

      MclMsgUnitItr (MclFacility & theFacility);

      //-----------------------------------------------------------------------

      ~MclMsgUnitItr ();
         //
         // Destructor

      //-----------------------------------------------------------------------

      MclMsg * nextMsgForChange (const MclLevel & theLevel);
         //
         // Advances this MsgUnitItr to the next Msg with theLevel whose
         // allowedToChange attribute is TRUE. Returns the resulting Msg or
         // NULL, if this MsgUnitItr gets to the end of the MsgUnits for the
         // Facility.

      MclMsg * nextMsg ();
         //
         // Advances this MsgUnitItr to the next Msg and returns it.
         // Returns NULL, if the MsgUnitItr gets to the end of the MsgUnits for
         // the Facility.

      MclMsgFrag * nextMsgFrag ();
         //
         // Advances this MsgUnitItr to the next MsgFrag and returns it.
         // Returns NULL, if the MsgUnitItr gets to the end of the MsgUnits for
         // the Facility.

      MclMsgUnit * nextMsgUnit ();
         //
         // Advances this MsgUnitItr to the next MsgUnit and returns it.
         // Returns NULL, if the MsgUnitItr gets to the end of the MsgUnits for
         // the Facility.

      void reset ();
         //
         // Resets this MsgUnitItr to its initial state.

   private:

      MclMsgUnitItr             (const MclMsgUnitItr &);
      MclMsgUnitItr & operator= (const MclMsgUnitItr &);
         //
         // Copy ctor and assigment are declared private and not implemented
         // so as to prevent inadvertant copying and assignment.

      //-----------------------------------------------------------------------
      // Data.
      //-----------------------------------------------------------------------

      std::multimap <int, MclMsgUnit*>::iterator myItr_;
         //
         // Iterator for the actual MclMsgUnit, the one ordered by
         // externalMsgNumber.

      bool iteratorValid_;
         //
         // Flag to invalidate the iterator so it starts fresh next time it's
         // called

      MclMsgUnitSet & myMsgUnitSet_;
         //
         // We need to rememebr this in order to restart the above iterator
   };

#endif
