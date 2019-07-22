
//==============================================================================
// WIT-J Java Source File PeggingTriple.java.
//==============================================================================

package       com.ibm.witj;

import static com.ibm.witj.Utility.*;

//==============================================================================
// Generic Class PeggingTriple <C extends Component>
//
// Represents a triple of type (C, int, double).
// Conceptually, a PeggingTriple <C> represents a portion of the value of some
// Attribute <double[]> associated with a particular instance of Component class
// C (called the root) in a particular period.
//==============================================================================

public final class PeggingTriple <C extends Component>
{
//==============================================================================
// Public Methods
//==============================================================================

//------------------------------------------------------------------------------
// toString
//
// Override from class Object.
//------------------------------------------------------------------------------

   public String toString ()
      {
      try
         {
         notAllowedWhenTerminated ("PeggingTriple.toString");

         return "(" + myRoot + ", " + myPeriod + ", " + myVolume + ")";
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getRoot ()
//
// Accessor
//------------------------------------------------------------------------------

   public C getRoot ()
      {
      try
         {
         notAllowedWhenTerminated ("PeggingTriple.getRoot");

         return myRoot;
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getPeriod ()
//
// Accessor
//------------------------------------------------------------------------------

   public int getPeriod ()
      {
      try
         {
         notAllowedWhenTerminated ("PeggingTriple.getPeriod");

         return myPeriod;
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//------------------------------------------------------------------------------
// getVolume ()
//
// Accessor
//------------------------------------------------------------------------------

   public double getVolume ()
      {
      try
         {
         notAllowedWhenTerminated ("PeggingTriple.getVolume");

         return myVolume;
         }

      catch (Throwable              theThrowable)
         {
         throw getExceptionToThrow (theThrowable);
         }
      }

//==============================================================================
// Package-Internal Constructor
//==============================================================================

   PeggingTriple (C theRoot, int thePeriod, double theVolume)
      {
      myRoot   = theRoot;
      myPeriod = thePeriod;
      myVolume = theVolume;
      }

//==============================================================================
// Private Fields
//==============================================================================

//------------------------------------------------------------------------------
// myRoot
//
// The root of this PeggingTriple.
//------------------------------------------------------------------------------

   private final C myRoot;

//------------------------------------------------------------------------------
// myPeriod
//
// The period for this PeggingTriple.
//------------------------------------------------------------------------------

   private final int myPeriod;

//------------------------------------------------------------------------------
// myVolume
//
// The "volume" for this PeggingTriple.
//------------------------------------------------------------------------------

   private final double myVolume;

//==============================================================================
}
