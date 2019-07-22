
//==============================================================================
// WIT-J Java Source File BomEntry.java.
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;

import           java.util.*;

//==============================================================================
// Class BomEntry
//
// Represents a BOM entry in an implosion problem.
//
// Class Hierarchy:
//
// ThreadedObject
//    PreservedObject
//       Component
//          BomEntry
//
// Part.Associate
//    BomEntry
//==============================================================================

public final class BomEntry extends Component implements Part.Associate
{
//==============================================================================
// Public Methods
//==============================================================================

//------------------------------------------------------------------------------
// newInstance (theOpn, thePart)
//
// Factory method: Constructs and returns a new BomEntry for the arguments.
//------------------------------------------------------------------------------

   public static BomEntry newInstance (Operation theOpn, Part thePart)
      {
      try
         {
         notAllowedWhenTerminated ("BomEntry.newInstance");

         reqMatchingProblems (
            theOpn,
           "theOpn",
            thePart,
           "thePart",
           "BomEntry.newInstance");

         return new BomEntry (theOpn, thePart);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getSubstitutes ()
//
// Returns an unmodifiable List of all the Substitutes associated with this
// BomEntry, listed in the order of creation.
//------------------------------------------------------------------------------

   public List <Substitute> getSubstitutes ()
      {
      try
         {
         mustBeActive ("BomEntry.getSubstitutes");

         return unmodSubstitutes;
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getSubstitutesTo (thePart)
//
// Creates and returns a List of all Substitutes from this BomEntry to thePart,
// listed in order of creation.
//------------------------------------------------------------------------------

   public List <Substitute> getSubstitutesTo (Part thePart)
      {
      try
         {
         mustBeActive ("BomEntry.getSubstitutesTo");

         return thePart.findAssociatesIn (mySubstitutes);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getUniqueSubstituteTo (thePart)
//
// If there is exactly one Substitute from this BomEntry to thePart, it is
//    returned.
// If there are none, null is returned.
// If there are more than one, a StatusQuoExeption is thrown.
//------------------------------------------------------------------------------

   public Substitute getUniqueSubstituteTo (Part thePart)
      {
      try
         {
         mustBeActive ("BomEntry.getUniqueSubstituteTo");

         return
            thePart.
               findUniqueAssociateIn (
                  mySubstitutes,
                  "   theBomEntry.getUniqueSubstituteTo (thePart) was\n"
                + "   called where theBomEntry and thePart are connected\n"
                + "   by more than one Substitute:");
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
         mustBeActive ("BomEntry.getOperation");

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
         mustBeActive ("BomEntry.getPart");

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
           "BomEntry #"
         + getIndex ()
         + " for "
         + myOperation
         + ", consuming "
         + myPart;
      }

//------------------------------------------------------------------------------
// deactivate ().
//
// Override from class PreservedObject.
//------------------------------------------------------------------------------

   void deactivate ()
      {
      myBomEntryCOR.deactivate ();
      super        .deactivate ();
      }

//------------------------------------------------------------------------------
// recordSubstitute (theSub)
//
// Records the fact that theSub has been created.
//------------------------------------------------------------------------------

   void recordSubstitute (Substitute theSub)
      {
      assert (theSub.getBomEntry () == this);

      mySubstitutes.add (theSub);
      }

//------------------------------------------------------------------------------
// restoreAfterPurge ()
//
// Override from class Component.
//------------------------------------------------------------------------------

   void restoreAfterPurge ()
      {
      removeInactive (mySubstitutes);
      }

//------------------------------------------------------------------------------
// getIndex ()
//
// Override from class Component.
//------------------------------------------------------------------------------

   int getIndex ()
      {
      return myBomEntryCOR.getBomIndex ();
      }

//------------------------------------------------------------------------------
// getCompClassID ()
//
// Override from class Component.
//------------------------------------------------------------------------------

   CompClassID getCompClassID ()
      {
      return CompClassID.BOM_ENTRY;
      }

//------------------------------------------------------------------------------
// getComponentCOR ().
//
// Override from class Component.
//------------------------------------------------------------------------------

   ComponentCOR getComponentCOR ()
      {
      return myBomEntryCOR;
      }

//------------------------------------------------------------------------------
// Accessors.
//------------------------------------------------------------------------------

   BomEntryCOR getBomEntryCOR ()
      {
      return myBomEntryCOR;
      }

//==============================================================================
// Private Constructor
//==============================================================================

   private BomEntry (Operation theOpn, Part thePart)
      {
      super (theOpn.getCoordinator ());

      int theBomIdx;

      myOperation      = theOpn;
      myPart           = thePart;

      theBomIdx        = theOpn. getBomEntries ().size ();

      myBomEntryCOR    =
         new BomEntryCOR (
            theOpn. getOperationCOR (),
            theBomIdx,
            thePart.getPartCOR ());

      mySubstitutes    = new ArrayList <Substitute> (0);

      unmodSubstitutes = Collections.unmodifiableList (mySubstitutes);

      getCoordinator ().recordConcComp (this);
      myOperation      .recordBomEntry (this);
      myPart           .recordBomEntry (this);
      }

//==============================================================================
// Private Fields
//==============================================================================

//------------------------------------------------------------------------------
// myOperation.
//
// The Operation whose consumption of a Part is represented by this BomEntry.
//------------------------------------------------------------------------------

   private final Operation myOperation;

//------------------------------------------------------------------------------
// myPart.
//
// The Part whose consumption by an Operation is represented by this BomEntry.
//------------------------------------------------------------------------------

   private final Part myPart;

//------------------------------------------------------------------------------
// myBomEntryCOR.
//
// The BomEntryCOR that belongs to this BomEntry.
//------------------------------------------------------------------------------

   private final BomEntryCOR myBomEntryCOR;

//------------------------------------------------------------------------------
// mySubstitutes
//
// The list of all Substitutes associated with this BomEntry, listed in the
// order of creation.
//------------------------------------------------------------------------------

   private final ArrayList <Substitute> mySubstitutes;

//------------------------------------------------------------------------------
// unmodSubstitutes
//
// Unmodifiable view of mySubstitutes.
//------------------------------------------------------------------------------

   private final List <Substitute> unmodSubstitutes;

//==============================================================================
}
