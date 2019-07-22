
//==============================================================================
// WIT-J Java Source File SpecAttImp.java.
//==============================================================================

package       com.ibm.witj;
import static com.ibm.witj.Attribute.*;

//==============================================================================
// Class SpecAttImp <V>
//
// "Special Attribute Implementation."
// An AttImp <V> that operates in a special way.
//
// Class Hierarchy:
//
// PreservedInternal
//    AttImp <V>
//       SpecAttImp <V>
//          SpecAttImp.NPeriodsImp
//          SpecAttImp.NameImp
//          SpecAttImp.IndexImp
//          SpecAttImp.BoundSetImp
//==============================================================================

abstract class SpecAttImp <V> extends AttImp <V>
{
//==============================================================================
// Package-Internal Methods
//==============================================================================

//------------------------------------------------------------------------------
// buildInstances (theCoord)
//
// Constructs all of the SpecAttImps that are to belong to theCoord.
//------------------------------------------------------------------------------

   static void buildInstances (Coordinator theCoord)
      {
      new NPeriodsImp (                 theCoord);

      new     NameImp (      PART_NAME, theCoord);
      new     NameImp (    DEMAND_NAME, theCoord);
      new     NameImp ( OPERATION_NAME, theCoord);

      new    IndexImp (      BOM_INDEX, theCoord);
      new    IndexImp (      SUB_INDEX, theCoord);
      new    IndexImp (      BOP_INDEX, theCoord);

      new BoundSetImp (CUM_SHIP_BOUNDS, theCoord);
      new BoundSetImp (    EXEC_BOUNDS, theCoord);
      new BoundSetImp (   STOCK_BOUNDS, theCoord);
      }

//==============================================================================
// Private Constructor
//==============================================================================

   private SpecAttImp (Attribute <V> theAtt, Coordinator theCoord)
      {
      super (theAtt, theCoord);
      }

//==============================================================================
// Private Static Nested Class SpecAttImp.NPeriodsImp.
//
// The AttImp for N_PERIODS
//==============================================================================

   private static final class NPeriodsImp extends SpecAttImp <Integer>
   {
   NPeriodsImp (Coordinator theCoord)
      {
      super (N_PERIODS, theCoord);
      }

   void set (Integer theValue)
      {
      if (getCurComponent ().getCoordinator ().hasPartsOrOperations ())
         {
         throw
            new StatusQuoException (
               "   The set method was called for Attribute \"N_PERIODS\"\n"
             + "   in a Problem that has one or more Parts or Operations."
                );
         }

      getCurComponent ().asProblem ().setNPeriods (theValue);
      }

   Integer get ()
      {
      return getCurComponent ().asProblem ().getNPeriods ();
      }
   }

//==============================================================================
// Private Static Nested Class SpecAttImp.NameImp.
//
// The AttImp for PART_NAME, DEMAND_NAME, or OPERATION_NAME.
//==============================================================================

   private static final class NameImp extends SpecAttImp <String>
   {
   NameImp (Attribute <String> theStrAtt, Coordinator theCoord)
      {
      super (theStrAtt, theCoord);
      }

   void set (String theName)
      {
      getCurComponent ().setName (theName);
      }

   String get ()
      {
      return getCurComponent ().getName ();
      }
   }

//==============================================================================
// Private Static Nested Class SpecAttImp.IndexImp.
//
// The AttImp for BOP_INDEX, SUB_INDEX, or BOP_INDEX.
//==============================================================================

   private static final class IndexImp extends SpecAttImp <Integer>
   {
   IndexImp (Attribute <Integer> theIntAtt, Coordinator theCoord)
      {
      super (theIntAtt, theCoord);
      }

   Integer get ()
      {
      return getCurComponent ().getIndex ();
      }
   }

//==============================================================================
// Private Static Nested Class SpecAttImp.BoundSetImp.
//
// The AttImp for any Attribute <BoundSet>.
//==============================================================================

   private static final class BoundSetImp extends SpecAttImp <BoundSet>
   {
   BoundSetImp (Attribute <BoundSet> theBSetAtt, Coordinator theCoord)
      {
      super (theBSetAtt, theCoord);
      }
   }

//==============================================================================
}
