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

#ifndef mcl_mcl_h
#define mcl_mcl_h

//-----------------------------------------------------------------------------
// File mcl.h
//
// Contains the #include statements for the header files that need to be
// included by a client of MCL.
//-----------------------------------------------------------------------------

#include <mcl/src/arg.h>
#include <mcl/src/facility.h>
#include <mcl/src/level.h>
#include <mcl/src/msg.h>
#include <mcl/src/output.h>

#endif


//-----------------------------------------------------------------------------
// Below are notes that are included in some of the header files within this
// directory. These are basically general notes on usage of the MCL.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Notes on Usage:    MCLFacility Class
//
// * The MclFacility serves basically as the public interface for the
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
//        Note: It is essential that arguments are of the correct TYPE and
//              ORDER in this list. Beware of accidentally passing octal or
//              other type values in the place of base-10 values. Also make
//              sure not to pass ambiguous values to << operator or you may
//              get compile-time errors. To be sure, maybe do a type cast.
//
//   *) Other Stuff:
//
//      There are a couple of problems with incorporating MclFacility objects
//      as data members of other classes.
//      - First, there is no explicit default constructor for the MclFacility 
//        class.
//      - The assignment operator = , is overloaded as a private method so
//        it cannot be used.
//
//      Possible solutions include:
//      - writing another class which serves as a wrapper for the MclFacility 
//        class, with the desired functionality and a default constructor.
//      - having the data member be of type MclFacility* , and dynamically
//        allocate memory for and instantiate a facility object.
//      - ??
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Notes on Implementation:  MclMsgUnit,MclMsg,MclMsgFrag Classes
//
// * The classes: MclMsg and MclMsgFrag are sub-classes of the MclMsgUnit
//   which are the type of objects that are held and issued by the facilities.
//
// * These message units must be created after the MclFacility object to which
//   it will "belong" has been instantiated. They can be created by allocating
//   memory dynamically as in the examples given in unitTest.C and in the
//   Notes on Implementation for the MclFacility class in facility.h.
//
//    new MclMsg(someMclFacil,"messageID1",someMclLevel,9999);
//    new MclMsg(someMclFacil,"messageID2",otherMclLevel,100);
//     .
//     .
//    etc.
//
// * Message units own attributes which effect how it is issued, but only
//   the MclMsg class has methods to manipulate them (from below):
//
//    MclMsg * maxTimesIssued        (int        theValue);
//    MclMsg * preceedingLineFeeds   (int        theValue);
//    MclMsg * trailingLineFeeds     (int        theValue);
//    MclMsg * displayExternalNumber (MclBoolean theValue);
//    MclMsg * allowedToChange       (MclBoolean theValue);
//
//   These, along with their respective "get" methods and the constructors, 
//   should be the only methods used externally.
//
//
// * When a message is issued by its facility, the facility takes care of
//   locating the proper message determined by its key (<STRING> message id),
//   but the actual final-formatting and printing of the message is done
//   within the MclMsg object.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Notes on Implementation:   MCLArgList Class
//
// * All the classes including MclArg and its derived classes, are used
//   internally within MCL. There is no intended public interface for these
//   classes.
//
// * Of the class declarations in this file, MclArgList is the only one that
//   should be used outside of MCL.
//
// * Objects of type MclArgList are required as arguments to the MclFacility
//   parentheses operator which is used when issuing messages. These
//   MclArgList objects are used to hold the data values that are then 
//   printed within the framework of the message being printed.
//
// * The most compact method of creating and passing one of these argument
//   lists to the facility is by a call such as:
//
//    MclArgList() << arg1 << arg2 << etc;
//
//   which invokes the default constructor, and appends to the list using
//   the overloaded << operator.
//
// * it is also possible to index members of the argument list using the 
//   [] operator.
//
// * implementation is left up to the user, but it is important to make
//   sure that the arguments are of the correct (and unambiguous) type,
//   and also that they are in the correct order (determined by the format
//   string associated with the message being issued.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Notes on Implementation: MclOutputDisplayer,MclPrintfOutputDisplayer Classes
//
// * This is the class that serves as an interface between the Message Class
//   Library and the output buffer -- {file,screen}; It writes all output.
//
// * Though there are public methods that allow for manual issuing of
//   of messages, this is not necessary if used with the MclFacility. This
//   is all done internally within MclFacility class.
//
// * Note: the base class MclOutputDisplayer does not have a public
//         constructor. you must either use the MclPrintfOutputDisplayer class
//         or create another derived class of MclOutputDisplayer.
//
// * Also: the MclPrintfOutputDisplayer is missing a default constructor
//         like the MclFacility. if you haven't read the Notes on Imp. for
//         the MclFacility class (in file: facility.h), read them.
//
//-----------------------------------------------------------------------------
