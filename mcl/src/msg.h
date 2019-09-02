//==============================================================================
// Message Class Library
//
// (C) Copyright IBM Corp. 1997, 2005  All Rights Reserved
//==============================================================================

#ifndef mcl_msg_h
#define mcl_msg_h

//-----------------------------------------------------------------------------
// File msg.h
//
// Contains the declaration of the following classes:
//
// MsgUnit
//    Msg
//    MsgFrag
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Notes on Implementation:
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
//    MclMsg * vectorIndent          (int        theValue);
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

#include <stdlib.h>
#include <string>

#include <mcl/src/defs.h>

//-----------------------------------------------------------------------------

class MclArg;
class MclArgList;
class MclFacility;
class MclLevel;
class MclMsg;
class MclMsgFrag;

//-----------------------------------------------------------------------------
// class MsgUnit.
//
// Message unit: either a complete message or a fragment of one.
// Note: The classes Msg and MsgFrag directly derive from this class: No
// other classes should be derived from it, except by deriving fron Msg or
// MsgFrag.
//-----------------------------------------------------------------------------

class MclMsgUnit
   {
   public:

      virtual ~MclMsgUnit ();
         //
         // Destructor

      //-----------------------------------------------------------------------
      // Access functions.
      //-----------------------------------------------------------------------

      inline MclFacility & myFacility () const
         {
         return myFacility_;
         }

      const char * id () const;
         //
         // Returns id_;

      const char * text () const;
         //
         // Returns text_;

      inline MclBoolean isInserting () const
         {
         return isInserting_;
         }

      //-----------------------------------------------------------------------

      virtual MclMsg * asaMsg ();
         //
         // If this MsgUnit is actually a Msg, the Msg is returned;
         // otherwise NULL is returned.

      virtual MclMsgFrag * asaMsgFrag ();
         //
         // If this MsgUnit is actually a MsgFrag, the MsgFrag is returned.
         // Otherwise NULL is returned.

      void clientError (const char * text) const;
         //
         // Reports an error, printing text and aborting for a client error
         // associated with this MsgUnit.

   protected:

      MclMsgUnit (
            MclFacility & theFacility,
            const char *  idVal,
            const char *  textVal);
         //
         // myFacility_ <-- theFacility
         // id_         <-- idVal
         // text_       <-- textVal

      void insertSelf ();
         //
         // Inserts this MsgUnit into myFacility.

   private:

      MclMsgUnit             (const MclMsgUnit &);
      MclMsgUnit & operator= (const MclMsgUnit &);
         //
         // Copy ctor and assignment are declared private and not implemented
         // so as to prevent inadvertant copying and assignment.

      void forbidBlankId () const;
         //
         // Disallows id_ == all blanks.
         // (This includes the case of a null string.)

      void forbidDupId () const;
         //
         // Disallows id_ duplicated within myFacility ().

      //-----------------------------------------------------------------------
      // Data.
      //-----------------------------------------------------------------------

      MclFacility & myFacility_;
         //
         // The Facility that owns with this MsgUnit.

      std::string id_;
         //
         // String for identifying this MsgUnit.

      std::string text_;
         //
         // The (unprocessed) text of this MsgUnit.

      MclBoolean isInserting_;
         //
         // TRUE, iff this MsgUnit is currently inserting itself into
         // myFacility ().
   };

//-----------------------------------------------------------------------------

class MclMsg: public MclMsgUnit
{

public:

  MclMsg (
     MclFacility &    theFacility,
     const char *     id,
     const MclLevel & theLevel,
     const char *     textVal,
     int              externalNumber);

  // Destructor
  virtual ~MclMsg();

  //---------------------------------------------------------------------------
  // Access functions.
  //---------------------------------------------------------------------------

  inline int externalNumber () const
     {
     return externalNumber_;
     }

  inline int maxTimesIssued () const
     {
     return maxTimesIssued_;
     }

  inline int timesIssued () const
     {
     return timesIssued_;
     }

  inline int preceedingLineFeeds () const
     {
     return preceedingLineFeeds_;
     }

  inline int trailingLineFeeds () const
     {
     return trailingLineFeeds_;
     }

  inline int vectorIndent () const
     {
     return vectorIndent_;
     }

  inline MclBoolean displayExternalNumber () const
     {
     return displayExternalNumber_;
     }

  inline MclBoolean allowedToChange () const
     {
     return allowedToChange_;
     }

  inline const MclLevel & myLevel () const
     {
     return myLevel_;
     }
  
  inline MclBoolean mark() const
  {
    return mark_;
  }
  
  //---------------------------------------------------------------------------
  // Attribute setting functions.
  //---------------------------------------------------------------------------

  MclMsg * maxTimesIssued        (int        theValue);
  MclMsg * preceedingLineFeeds   (int        theValue);
  MclMsg * trailingLineFeeds     (int        theValue);
  MclMsg * vectorIndent          (int        theValue);
  MclMsg * displayExternalNumber (MclBoolean theValue);
  MclMsg * allowedToChange       (MclBoolean theValue);

  inline void mark(MclBoolean theValue) { mark_ = theValue; }
 
  //---------------------------------------------------------------------------

  virtual MclMsg * asaMsg ();
     //
     // Override from class MsgUnit;

  //Prints message text with arguments

  void issue(const MclArgList &);

protected:

   virtual void postIssue ();
      //
      // Called by issue () after it completes its activities.
      // Called even if timesIssued > maxTimesIssued.
      // The main purpose of this function is to allow client specific error
      // handling behavior.

  MclMsg * set (
        int &        theAttr,
        const char * attrName,
        int          theValue);
     //
     // Sets attribute theAttr, whose name is given by attrName to theValue.
     // Returns this Msg.
     // Issues a client error if allowedToChange is FALSE.

private:

  void checkExtNumRange () const;
     //
     // Disallows externalNumber_ out of range.

  void forbidDupExtNum () const;
     //
     // Disallows externalNumber_ duplicated with myFacility ().

  MclMsg * setNonNeg (
        int &        theAttr,
        const char * attrName,
        int          theValue);
     //
     // Sets attribute theAttr, whose name is given by attrName to theValue,
     // after verifying that theValue >= 0. Returns this Msg.

  void getFinalText (
        std::string &        finalText,
        const MclArgList & theArgList)
        const;
     //
     // Sets finalText to text_, processed for issuing, applying the Args in
     // theArgList.

  void validateArgNum (
        size_t             argNum,
        const MclArgList & theArgList)
        const;
     //
     // Verifies that theArgList specifies argument # argNum.

  void validateMaxArgNum (
        int                maxArgNum,
        const MclArgList & theArgList)
        const;
     //
     // Verifies that theArgList specifies no more arguments than maxArgNum.

  void missingFormatFlag (const char * argSpec) const;
     //
     // Reports an error indicating that the argument specification argSpec
     // is missing a format flag.

  void procForAttrs (std::string & finalText) const;
     //
     // Processes finalText according to the attributes of this Msg.
  
  void validateCurArgFmt (
        const std::string & finalText,
        size_t              start,
        size_t &            posPercent,
        int&                number,
        size_t &            posDollar,
        size_t &            posv,
        size_t &            posFmt)
        const;
     //
     // Checks that current arg output format begins with '%', followed by a
     // sequence of digits, by an optional 'v' and then followed by the '$'

  /* Obsoleted by the new version of the above
  size_t indexInCurArgFmt(const char* c,
                          const std::string & finalText,
                          size_t start) const;*/
  
  void procLine (std::string & finalText) const;
  //
  // if any line is longer than lineLength, break it
  //
  
  MclMsg             (const MclMsg &);
  MclMsg & operator= (const MclMsg &);
     //
     // Copy ctor and assignment are declared private and not implemented
     // so as to prevent inadvertant copying and assignment.

  //---------------------------------------------------------------------------
  // Data.
  //---------------------------------------------------------------------------

  const int  externalNumber_;
  int        maxTimesIssued_;
  int        timesIssued_;
  int        preceedingLineFeeds_;
  int        trailingLineFeeds_;

  int        vectorIndent_;
     //
     // The # of chars to indent each new line when formatting a vector.

  MclBoolean displayExternalNumber_;
  MclBoolean allowedToChange_;
     //
     // True if client can change attribute values.
  
  MclBoolean mark_;
  //
  // True if this message is issued.
  
  const MclLevel & myLevel_;
     //
     // The Level for this Msg.
};

//-----------------------------------------------------------------------------
// class MsgFrag.
//
// Message fragment.
//-----------------------------------------------------------------------------

class MclMsgFrag: public MclMsgUnit
   {
   public:

      MclMsgFrag (
            MclFacility & theFacility,
            const char *  idVal,
            const char *  textVal);
         //
         // Constructor.
         // See MsgUnit ctor for meaning of args.

      virtual ~MclMsgFrag ();
         //
         // Destructor

      virtual MclMsgFrag * asaMsgFrag ();
         //
         // Override from class MsgUnit;

   private:

      MclMsgFrag             (const MclMsgFrag &);
      MclMsgFrag & operator= (const MclMsgFrag &);
         //
         // Copy ctor and assignment are declared private and not implemented
         // so as to prevent inadvertant copying and assignment.
   };

#endif
