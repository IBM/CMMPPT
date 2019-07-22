
//==============================================================================
// WIT-J Java Source File BopEntry.java.
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;

//==============================================================================
// Class BopEntry
//
// Represents a BOP entry in an implosion problem.
//
// Class Hierarchy:
//
// ThreadedObject
//    PreservedObject
//       Component
//          BopEntry
//
// Part.Associate
//    BopEntry
//==============================================================================

public final class BopEntry extends Component implements Part.Associate
{
//==============================================================================
// Public Methods
//==============================================================================

//------------------------------------------------------------------------------
// newInstance (theOpn, thePart)
//
// Factory method: Constructs and returns a new BopEntry for the arguments.
//------------------------------------------------------------------------------

   public static BopEntry newInstance (Operation theOpn, Part thePart)
      {
      try
         {
         notAllowedWhenTerminated ("BopEntry.newInstance");

         reqMatchingProblems (
             theOpn,
            "theOpn",
             thePart,
            "thePart",
            "BopEntry.newInstance");

         return new BopEntry (theOpn, thePart);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// Accessors.
//------------------------------------------------------------------------------

   public Operation getOperation ()
      {
      try
         {
         mustBeActive ("BopEntry.getOperation");

         return myOperation;
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------

   public Part getPart ()
      {
      try
         {
         mustBeActive ("BopEntry.getPart");

         return myPart;
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
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
      return
           "BopEntry #"
         + getIndex ()
         + " for "
         + myOperation
         + ", producing "
         + myPart;
      }

//------------------------------------------------------------------------------
// deactivate ().
//
// Override from class PreservedObject.
//------------------------------------------------------------------------------

   void deactivate ()
      {
      myBopEntryCOR.deactivate ();
      super        .deactivate ();
      }

//------------------------------------------------------------------------------
// getIndex ()
//
// Override from class Component.
//------------------------------------------------------------------------------

   int getIndex ()
      {
      return myBopEntryCOR.getBopIndex ();
      }

//------------------------------------------------------------------------------
// getCompClassID ()
//
// Override from class Component.
//------------------------------------------------------------------------------

   CompClassID getCompClassID ()
      {
      return CompClassID.BOP_ENTRY;
      }

//------------------------------------------------------------------------------
// getComponentCOR ().
//
// Override from class Component.
//------------------------------------------------------------------------------

   ComponentCOR getComponentCOR ()
      {
      return myBopEntryCOR;
      }

//------------------------------------------------------------------------------
// Accessors.
//------------------------------------------------------------------------------

   BopEntryCOR getBopEntryCOR ()
      {
      return myBopEntryCOR;
      }

//==============================================================================
// Private Constructor
//==============================================================================

   private BopEntry (Operation theOpn, Part thePart)
      {
      super (theOpn.getCoordinator ());

      int theBopIdx;

      myOperation   = theOpn;
      myPart        = thePart;

      theBopIdx     = theOpn.getBopEntries ().size ();

      myBopEntryCOR    =
         new BopEntryCOR (
            theOpn. getOperationCOR (),
            theBopIdx,
            thePart.getPartCOR ());

      getCoordinator ().recordConcComp (this);
      myOperation      .recordBopEntry (this);
      myPart           .recordBopEntry (this);
      }

//==============================================================================
// Private Fields
//==============================================================================

//------------------------------------------------------------------------------
// myOperation.
//
// The Operation whose production of a Part is represented by this BopEntry.
//------------------------------------------------------------------------------

   private final Operation myOperation;

//------------------------------------------------------------------------------
// myPart.
//
// The Part whose production by an Operation is represented by this BopEntry.
//------------------------------------------------------------------------------

   private final Part myPart;

//------------------------------------------------------------------------------
// myBopEntryCOR.
//
// The BopEntryCOR that belongs to this BopEntry.
//------------------------------------------------------------------------------

   private final BopEntryCOR myBopEntryCOR;

//==============================================================================
}
