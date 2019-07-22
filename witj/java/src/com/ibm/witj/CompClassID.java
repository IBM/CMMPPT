
//==============================================================================
// WIT-J Java Source File CompClassID.java.
//==============================================================================

package       com.ibm.witj;

import                java.util.*;

import static com.ibm.witj.Utility.*;

//==============================================================================
// Enum CompClassID
//
// "Component Class Identification"
// Identifies a subclass of class Component.
//==============================================================================

enum CompClassID
{
//==============================================================================
// Enum Constants
//==============================================================================

   PROBLEM    (Problem   .class),
   PART       (Part      .class),
   DEMAND     (Demand    .class),
   OPERATION  (Operation .class),
   BOM_ENTRY  (BomEntry  .class),
   SUBSTITUTE (Substitute.class),
   BOP_ENTRY  (BopEntry  .class),
   ;

//==============================================================================
// Public Methods
//==============================================================================

//------------------------------------------------------------------------------
// toString ().
//
// Override from class Object.
//------------------------------------------------------------------------------

   public final String toString ()
      {
      return myCompClass.getSimpleName ();
      }

//==============================================================================
// Package Internal Methods
//==============================================================================

   static CompClassID getInstanceFor (Class <? extends Component> theCompClass)
      {
      assert (theCompClass != Component.class);

      return instForCompClass.get (theCompClass);
      }

//------------------------------------------------------------------------------
// Accessors.
//------------------------------------------------------------------------------

   Class <? extends Component> getCompClass ()
      {
      return myCompClass;
      }

//==============================================================================
// Private Constructor
//==============================================================================

   private CompClassID (Class <? extends Component> theCompClass)
      {     
      myCompClass = theCompClass;
      }

//==============================================================================
// Private Fields
//==============================================================================

//------------------------------------------------------------------------------
// myCompClass
//
// The Component class identified by this CompClassID.
//------------------------------------------------------------------------------

   private final Class <? extends Component> myCompClass;

//------------------------------------------------------------------------------
// instForCompClass
//
// instForCompClass.get (theCompClass) is the CompClassID that identifies
// theCompClass, if any; otherwise null.
//------------------------------------------------------------------------------

   private
      static
         final
            HashMap <Class <? extends Component>, CompClassID> instForCompClass;

//==============================================================================
// Static initialization block
//==============================================================================

   static
      {
      Class <? extends Component> theCompClass;

      instForCompClass =
         new HashMap <Class <? extends Component>, CompClassID> (50);

      for (CompClassID theCompClassID: CompClassID.values ())
         {
         theCompClass = theCompClassID.myCompClass;

         instForCompClass.put (theCompClass, theCompClassID);
         }
      }
}
