
//==============================================================================
// WIT-J Java Source File Substitute.java.
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;

//==============================================================================
// Class Substitute
//
// Represents a substitute BOM entry in an implosion problem.
//
// Class Hierarchy:
//
// ThreadedObject
//    PreservedObject
//       Component
//          Substitute
//
// Part.Associate
//    Substitute
//==============================================================================

public final class Substitute extends Component implements Part.Associate
{
//==============================================================================
// Public Methods
//==============================================================================

//------------------------------------------------------------------------------
// newInstance (theBomEnt, thePart)
//
// Factory method: Constructs and returns a new Substitute for the arguments.
//------------------------------------------------------------------------------

   public static Substitute newInstance (BomEntry theBomEnt, Part thePart)
      {
      try
         {
         notAllowedWhenTerminated ("Substitute.newInstance");

         reqMatchingProblems (
            theBomEnt,
           "theBomEnt",
            thePart,
           "thePart",
           "Substitute.newInstance");

         return new Substitute (theBomEnt, thePart);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// Accessors.
//------------------------------------------------------------------------------

   public BomEntry getBomEntry ()
      {
      try
         {
         mustBeActive ("Substitute.getBomEntry");

         return myBomEntry;
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
         mustBeActive ("Substitute.getPart");

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
           "Substitute #"
         + getIndex ()
         + " for BomEntry #"
         + myBomEntry.getIndex ()
         + " for "
         + myBomEntry.getOperation ()
         + ", consuming "
         + myPart;
      }

//------------------------------------------------------------------------------
// deactivate ()
//
// Override from class PreservedObject.
//------------------------------------------------------------------------------

   void deactivate ()
      {
      mySubstituteCOR.deactivate ();
      super          .deactivate ();
      }

//------------------------------------------------------------------------------
// getIndex ()
//
// Override from class Component.
//------------------------------------------------------------------------------

   int getIndex ()
      {
      return mySubstituteCOR.getSubIndex ();
      }

//------------------------------------------------------------------------------
// getCompClassID ()
//
// Override from class Component.
//------------------------------------------------------------------------------

   CompClassID getCompClassID ()
      {
      return CompClassID.SUBSTITUTE;
      }

//------------------------------------------------------------------------------
// getComponentCOR ()
//
// Override from class Component.
//------------------------------------------------------------------------------

   ComponentCOR getComponentCOR ()
      {
      return mySubstituteCOR;
      }

//------------------------------------------------------------------------------
// Accessors.
//------------------------------------------------------------------------------

   SubstituteCOR getSubstituteCOR ()
      {
      return mySubstituteCOR;
      }

//==============================================================================
// Private Constructor
//==============================================================================

   private Substitute (BomEntry theBomEnt, Part thePart)
      {
      super (theBomEnt.getCoordinator ());

      int theSubIdx;

      myBomEntry      = theBomEnt;
      myPart          = thePart;

      theSubIdx       = theBomEnt.getSubstitutes ().size ();

      mySubstituteCOR =
         new SubstituteCOR (
            theBomEnt.getBomEntryCOR (),
            theSubIdx,
            thePart.getPartCOR ());

      getCoordinator ().recordConcComp   (this);
      myBomEntry       .recordSubstitute (this);
      myPart           .recordSubstitute (this);
      }

//==============================================================================
// Private Fields
//==============================================================================

//------------------------------------------------------------------------------
// myBomEntry
//
// The BomEntry for which this Substitute represents alternative consumption.
//------------------------------------------------------------------------------

   private final BomEntry myBomEntry;

//------------------------------------------------------------------------------
// myPart.
//
// The Part whose consumption is represented by this Substitute.
//------------------------------------------------------------------------------

   private final Part myPart;

//------------------------------------------------------------------------------
// mySubstituteCOR.
//
// The SubstituteCOR that belongs to this Substitute.
//------------------------------------------------------------------------------

   private final SubstituteCOR mySubstituteCOR;

//==============================================================================
}
