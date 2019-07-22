
//==============================================================================
// WIT-J Java Source File MessageMgr.java.
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;

//==============================================================================
// Class MessageMgr
//
// "Message Manager"
// Provides an interface to WIT's message functions for a Problem.
//
// Class Hierarchy:
//
// ThreadedObject
//    PreservedObject
//       MessageMgr
//==============================================================================

public final class MessageMgr extends PreservedObject
{
//==============================================================================
// Public Methods
//==============================================================================

//------------------------------------------------------------------------------
// flushFile ()
//
// Flushes WIT's message file.
//------------------------------------------------------------------------------

   public void flushFile ()
      {
      try
         {
         mustBeActive ("MessageMgr.flushFile");

         myMessageMgrCOR.flushFile ();
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// setQuiet (quietVal)
//
// Setter.
//------------------------------------------------------------------------------

   public void setQuiet (boolean quietVal)
      {
      try
         {
         mustBeActive ("MessageMgr.setQuiet");

         myMessageMgrCOR.setQuiet (quietVal);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// isQuiet ()
//
// Accessor.
//------------------------------------------------------------------------------

   public boolean isQuiet ()
      {
      try
         {
         mustBeActive ("MessageMgr.isQuiet");

         return myMessageMgrCOR.isQuiet ();
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// Wrappers for corresponding WIT API functions.
//------------------------------------------------------------------------------

   public void setMesgFileAccessMode (String theValue)
      {
      try
         {
         mustBeActive ("MessageMgr.setMesgFileAccessMode");

         myMessageMgrCOR.setMesgFileAccessMode (theValue);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public String getMesgFileAccessMode ()
      {
      try
         {
         mustBeActive ("MessageMgr.getMesgFileAccessMode");

         return myMessageMgrCOR.getMesgFileAccessMode ();
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public void setMesgFileName (String theValue)
      {
      try
         {
         mustBeActive ("MessageMgr.setMesgFileName");

         myMessageMgrCOR.setMesgFileName (theValue);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public String getMesgFileName ()
      {
      try
         {
         mustBeActive ("MessageMgr.getMesgFileName");

         return myMessageMgrCOR.getMesgFileName ();
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public void setMesgTimesPrint (int theMsgNo, int theCount)
      {
      try
         {
         mustBeActive ("MessageMgr.setMesgTimesPrint");

         myMessageMgrCOR.setMesgTimesPrint (theMsgNo, theCount);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public int getMesgTimesPrint (int theMsgNo)
      {
      try
         {
         mustBeActive ("MessageMgr.getMesgTimesPrint");

         return myMessageMgrCOR.getMesgTimesPrint (theMsgNo);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public void setMesgTimesPrint (MessageGroup theGroup, int theCount)
      {
      try
         {
         CaseTokenCOR theMsgGroupNoCTCOR;

         mustBeActive ("MessageMgr.setMesgTimesPrint");

         theMsgGroupNoCTCOR = getCoordinator ().getCaseTokenCOR (theGroup);

         myMessageMgrCOR.
            setMesgTimesPrintByGroup (theMsgGroupNoCTCOR, theCount);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//==============================================================================
// Package-Internal Constructor
//==============================================================================

   MessageMgr (Coordinator theCoord)
      {
      super (theCoord);

      myMessageMgrCOR = new MessageMgrCOR (theCoord.getCoordinatorCOR ());
      }

//==============================================================================
// Package-Internal Methods
//==============================================================================

//------------------------------------------------------------------------------
// toStringActive ().
//
// Override from class PreservedObject.
//------------------------------------------------------------------------------

   String toStringActive ()
      {
      return "MessageMgr";
      }

//------------------------------------------------------------------------------
// deactivate ()
//
// Override from class PreservedObject.
//------------------------------------------------------------------------------

   void deactivate ()
      {
      myMessageMgrCOR.deactivate ();
      super          .deactivate ();
      }

//==============================================================================
// Private Fields
//==============================================================================

//------------------------------------------------------------------------------
// myMessageMgrCOR.
//
// The C++ MsgMgr interface that belongs to this MessageMgr.
//------------------------------------------------------------------------------

   private final MessageMgrCOR myMessageMgrCOR;

//==============================================================================
}
