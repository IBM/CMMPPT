
//==============================================================================
// WIT-J Java Source File Part.java.
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;

import           java.util.*;

//==============================================================================
// Class Part
//
// Represents a part in an implosion problem.
//
// Class Hierarchy:
//
// ThreadedObject
//    PreservedObject
//       Component
//          Part
//==============================================================================

public final class Part extends Component
{
//==============================================================================
// Public Static Nested Enum Category
//
// Indicates the part category of a part.
//==============================================================================

   public static enum Category implements CaseToken
   {
      MATERIAL,
      CAPACITY,
   }

//==============================================================================
// Public Methods
//==============================================================================

//------------------------------------------------------------------------------
// newInstance (theProblem, thePartName, theCategory)
//
// Factory method: Constructs and returns a new Part for the arguments.
//------------------------------------------------------------------------------

   public static Part newInstance (
         Problem  theProblem,
         String   thePartName,
         Category theCategory)
      {
      try
         {
         notAllowedWhenTerminated                  ("Part.newInstance");
         mustBeActive  (theProblem,  "theProblem",  "Part.newInstance");
         mustBeNonNull (thePartName, "thePartName", "Part.newInstance");
         mustBeNonNull (theCategory, "theCategory", "Part.newInstance");

         return
            new Part (theProblem.getCoordinator (), thePartName, theCategory);
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getCategory ()
//
// Returns the Part Category for this Part.
//------------------------------------------------------------------------------

   public Category getCategory ()
      {
      try
         {
         mustBeActive ("Part.getCategory");

         return myCategory;
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getDemands ()
//
// Returns an unmodifiable List of all the Demands associated with this Part,
// listed in the order of creation.
//------------------------------------------------------------------------------

   public List <Demand> getDemands ()
      {
      try
         {
         mustBeActive ("Part.getDemands");

         return unmodDemands;
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getBomEntries ()
//
// Returns an unmodifiable List of all the BomEntries associated with this Part,
// listed in the order of creation.
//------------------------------------------------------------------------------

   public List <BomEntry> getBomEntries ()
      {
      try
         {
         mustBeActive ("Part.getBomEntries");

         return unmodBomEntries;
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
// Part, listed in the order of creation.
//------------------------------------------------------------------------------

   public List <Substitute> getSubstitutes ()
      {
      try
         {
         mustBeActive ("Part.getSubstitutes");

         return unmodSubstitutes;
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getBopEntries ()
//
// Returns an unmodifiable List of all the BopEntries associated with this Part,
// listed in the order of creation.
//------------------------------------------------------------------------------

   public List <BopEntry> getBopEntries ()
      {
      try
         {
         mustBeActive ("Part.getBopEntries");

         return unmodBopEntries;
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getDemand (theDemandName)
//
// Returns the Demand that's associated with this Part and whose DemandName
// matches theDemandName, if any; otherwise null.
//------------------------------------------------------------------------------

   public Demand getDemand (String theDemandName)
      {
      try
         {
         mustBeActive ("Part.getDemand");

         for (Demand theDemand: myDemands)
            if (theDemand.getName ().equals (theDemandName))
               return theDemand;

         return null;
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getBelowList ()
//
// Returns the below list for this Part.
//------------------------------------------------------------------------------

   public ArrayList <Part> getBelowList ()
      {
      try
         {
         ArrayList <Part> theBelowList;

         mustBeActive ("Part.getBelowList");

         theBelowList = new ArrayList <Part> (0);

         myPartCOR.getBelowList (theBelowList);

         return theBelowList;
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
      String theCategoryName;

      theCategoryName =
         (myCategory == Category.MATERIAL)?
            "Material":
            "Capacity";

      return theCategoryName + " Part \"" + myName + "\"";
      }

//------------------------------------------------------------------------------
// deactivate
//
// Override from class PreservedObject.
//------------------------------------------------------------------------------

   void deactivate ()
      {
      myPartCOR.deactivate ();
      super    .deactivate ();
      }

//------------------------------------------------------------------------------
// recordDemand     (theDemand)
// recordBomEntry   (theBomEnt)
// recordSubstitute (theSub)
// recordBopEntry   (theBopEnt)
//
// Each of these methods records the fact that the argument has been created.
//------------------------------------------------------------------------------

   void recordDemand (Demand theDemand)
      {
      assert (theDemand.getPart () == this);

      myDemands.add (theDemand);
      }

   void recordBomEntry (BomEntry theBomEnt)
      {
      assert (theBomEnt.getPart () == this);

      myBomEntries.add (theBomEnt);
      }

   void recordSubstitute (Substitute theSub)
      {
      assert (theSub.getPart () == this);

      mySubstitutes.add (theSub);
      }

   void recordBopEntry (BopEntry theBopEnt)
      {
      assert (theBopEnt.getPart () == this);

      myBopEntries.add (theBopEnt);
      }

//------------------------------------------------------------------------------
// restoreAfterPurge ()
//
// Override from class Component.
//------------------------------------------------------------------------------

   void restoreAfterPurge ()
      {
      removeInactive (myDemands);
      removeInactive (myBomEntries);
      removeInactive (mySubstitutes);
      removeInactive (myBopEntries);
      }

//------------------------------------------------------------------------------
// findAssociatesIn (theList)
//
// Creates and returns a List of the A's in theList that are associated with
// this Part. The returned List retains the order of theList.
//------------------------------------------------------------------------------

   <A extends Associate>
         List <A> findAssociatesIn (List <A> theList)
      {
      ArrayList <A> theAssocs;

      theAssocs = new ArrayList <A> (theList.size ());

      for (A theA: theList)
         if (theA.getPart () == this)
            theAssocs.add (theA);

      theAssocs.trimToSize ();

      return theAssocs;
      }

//------------------------------------------------------------------------------
// findUniqueAssociateIn (theList, errText)
//
// If there is exactly one A in theList that's associated with this Part, it is
//    returned.
// If there are none, null is returned.
// If there are more than one, a StatusQuoExeption is thrown whose message
//    with errText.
//------------------------------------------------------------------------------

   <A extends Associate>
         A findUniqueAssociateIn (List <A> theList, String errText)
      {
      A uniqueA;

      uniqueA = null;

      for (A theA: theList)
         if (theA.getPart () == this)
            {
            if (uniqueA == null)
               {
               uniqueA = theA;
               }
            else
               {
               throw
                  new StatusQuoException (
                     errText + "\n"
                   + "\n"
                   + "      " + uniqueA + "\n"
                   + "      " + theA);
               }
            }
      
      return uniqueA;
      }

//------------------------------------------------------------------------------
// setName ()
//
// Override from class Component.
//------------------------------------------------------------------------------

   void setName (String theName)
      {
      myPartCOR.setPartName (theName);

      myName = theName;
      }

//------------------------------------------------------------------------------
// getName ()
//
// Override from class Component.
//------------------------------------------------------------------------------

   String getName ()
      {
      return myName;
      }

//------------------------------------------------------------------------------
// getCompClassID
//
// Override from class Component.
//------------------------------------------------------------------------------

   CompClassID getCompClassID ()
      {
      return CompClassID.PART;
      }

//------------------------------------------------------------------------------
// getComponentCOR
//
// Override from class Component.
//------------------------------------------------------------------------------

   ComponentCOR getComponentCOR ()
      {
      return myPartCOR;
      }

//------------------------------------------------------------------------------
// Accessors.
//------------------------------------------------------------------------------

   PartCOR getPartCOR ()
      {
      return myPartCOR;
      }

//==============================================================================
// Package-Internal Static Nested Interface Part.Associate.
//
// An interface to an Object that's associated with a particular Part.
//
// Class Hierarchy:
//
// Part.Associate
//    BomEntry
//    Substitute
//    BopEntry
//==============================================================================

   static interface Associate
   {
   //---------------------------------------------------------------------------
   // getPart ()
   //
   // Returns the Part associated with this Part.Associate.
   //---------------------------------------------------------------------------

      Part getPart ();
   }

//==============================================================================
// Private Constructor.
//
// Arguments:
//    theCoord:    The Coordinator  for the Part
//    thePartName: The PartName      of the Part
//    theCategory: The Part Category of the Part
//==============================================================================

   private Part (
         Coordinator theCoord,
         String      thePartName,
         Category    theCategory)
      {
      super (theCoord);

      CaseTokenCOR thePartCatCTCOR;

      myName           = thePartName;

      myCategory       = theCategory;

      thePartCatCTCOR  = theCoord.getCaseTokenCOR (theCategory);

      myPartCOR        =
         new PartCOR (
            theCoord.getCoordinatorCOR (),
            thePartName,
            thePartCatCTCOR);

      myDemands        = new ArrayList <Demand>     (0);
      myBomEntries     = new ArrayList <BomEntry>   (0);
      mySubstitutes    = new ArrayList <Substitute> (0);
      myBopEntries     = new ArrayList <BopEntry>   (0);

      unmodDemands     = Collections.unmodifiableList (myDemands);
      unmodBomEntries  = Collections.unmodifiableList (myBomEntries);
      unmodSubstitutes = Collections.unmodifiableList (mySubstitutes);
      unmodBopEntries  = Collections.unmodifiableList (myBopEntries);

      getCoordinator ().recordPart (this);
      }

//==============================================================================
// Private Methods
//==============================================================================

//------------------------------------------------------------------------------
// makeInstance (theProblem, thePartName, isMaterial)  
//
// Constructs a new Part for the arguments.
// To be called from WIT-J/C++.
//------------------------------------------------------------------------------

   private static void makeInstance (
         Problem theProblem,
         String  thePartName,
         boolean isMaterial)
      {
      Part.Category theCategory;

      theCategory =
         isMaterial?
            Part.Category.MATERIAL:
            Part.Category.CAPACITY;

      Part.newInstance (theProblem, thePartName, theCategory);
      }

//==============================================================================
// Private Fields
//==============================================================================

//------------------------------------------------------------------------------
// myName
//
// The PartName of this Part.
//------------------------------------------------------------------------------

   private String myName;

//------------------------------------------------------------------------------
// myCategory
//
// The Category of this Part.
//------------------------------------------------------------------------------

   private final Category myCategory;

//------------------------------------------------------------------------------
// myPartCOR
//
// The PartCOR that belongs to this Part.
//------------------------------------------------------------------------------

   private final PartCOR myPartCOR;

//------------------------------------------------------------------------------
// myDemands
// myBomEntries
// mySubstitutes
// myBopEntries
//
// A List of all Demands     associated with this Part.
// A List of all BomEntries  associated with this Part.
// A List of all Substitutes associated with this Part.
// A List of all BopEntries  associated with this Part.
//
// The Components are listed in the order of creation.
//------------------------------------------------------------------------------
                                              
   private final ArrayList <Demand>     myDemands;
   private final ArrayList <BomEntry>   myBomEntries;
   private final ArrayList <Substitute> mySubstitutes;
   private final ArrayList <BopEntry>   myBopEntries;

//------------------------------------------------------------------------------
// unmodDemands
// unmodBomEntries
// unmodSubstitutes
// unmodBopEntries
//
// Unmodifiable views of:
//    myDemands
//    myBomEntries
//    mySubstitutes
//    myBopEntries
//------------------------------------------------------------------------------

   private final List <Demand>     unmodDemands;
   private final List <BomEntry>   unmodBomEntries;
   private final List <Substitute> unmodSubstitutes;
   private final List <BopEntry>   unmodBopEntries;

//==============================================================================
}
