
////////////////////////////////////////////////////////////////////////////////
// WIT-M Source File TupleCategory.java.
////////////////////////////////////////////////////////////////////////////////

package com.ibm.witm;

import static com.ibm.witm.Utility.*;

import java.io  .PrintStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

/*******************************************************************************
 * A TupleCategory is an EntityCategory of Tuples.<br><br>
 * 
 * The factory method for creating instances of this class is
 * Scheme.newTupleCategory (theName, theDimens).
 *******************************************************************************
 */
public final class TupleCategory extends EntityCategory <Tuple>
{
////////////////////////////////////////////////////////////////////////////////
// Private Non-Static Fields
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * The "dimensions" of this TupleCategory<br>
 * For each Tuple in this TupleCategory, the number of coordinates of the
 * Tuple must match myDimensions.size () and
 * for i = 0, ... myDimensions.size () - 1, coordinate #i of the Tuple must
 * belong to myDimensions.get (i).<br>
 * This List is an unmodifiable view of an ArrayList.
 *******************************************************************************
 */
   private final List <EntityCategory <?>> myDimensions;

/*******************************************************************************
 * This field is null until the Scheme for this TupleCategory is frozen.
 * After the Scheme for this TupleCategory is frozen:
 *    uniqueDimenIndexArr[theEntCat.getCatIndex ()] =
 *       The dimension index of theEntCat in this TupleCategory,
 *           if there is exactly one.<br>
 *       -1, if there is no matching dimension.<br>
 *       -2, if there is more than one matching dimension.
 *******************************************************************************
 */
   private int[] uniqueDimenIndexArr = null;

/*******************************************************************************
 * An array of n nulls, where n is the # of dimensions in this TupleCategory.
 * To be used by buildAll () as the pattern array for buildSlice (...).
 *******************************************************************************
 */
   private final Entity[] nullPattern;

////////////////////////////////////////////////////////////////////////////////
// Private Non-Static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Verifies that the # of coordinates passed to a method matches the # of
 * dimensions in this TupleCategory.
 * 
 * @param signature The signature of the method called
 *******************************************************************************
 */
   private void checkNCoords (Entity[] theCoordArr, String signature)
      {
      if (theCoordArr.length != myDimensions.size ())
         throw
            new IllegalArgumentException (
               "\n\n"
             + "   Call to TupleCategory." + signature + ",\n"
             + "   where the number of coordinates passed does not match the\n"
             + "   number of dimensions of the TupleCategory.\n"
             + "\n"
             + "      TupleCategory:        " + getName ()           + "\n"
             + "      # Dimensions:         " + myDimensions.size () + "\n"
             + "      # Coordinates Passed: " + theCoordArr.length   + "\n");
        }
   
/*******************************************************************************
 * Verifies that the Categories of the coordinates passed to a method match the
 * dimensions of this TupleCategory.
 * 
 * @param signature The signature of the method called
 *******************************************************************************
 */
   private void checkCategories (Entity[] theCoordArr, String signature)
      {
      int                theIdx;
      EntityCategory <?> theEntCat;
      EntityCategory <?> theDimen;
      
      for (theIdx = 0; theIdx < theCoordArr.length; theIdx ++)
         {
         if (theCoordArr[theIdx] == null)
            continue;
         
         theEntCat = theCoordArr[theIdx].getEntityCategory ();
         
         theDimen  = myDimensions.get (theIdx);
         
         if (theEntCat != theDimen)
            throw
               new IllegalArgumentException (
                  "\n\n"
                + "   Call to TupleCategory." + signature + ",\n"
                + "   where the EntityCategory of at least one of the\n"
                + "   coordinates passed does not match the corresponding\n"
                + "   dimension of the TupleCategory.\n"
                + "\n"
                + "      TupleCategory:             " +    getName () + "\n"
                + "      Dimension Index:           " + theIdx        + "\n"
                + "      Dimension:                 " + theDimen.
                                                           getName () + "\n"
                + "      Coordinate EntityCategory: " + theEntCat.
                                                           getName () + "\n");
         }
      }
   
/*******************************************************************************
 * Verifies that the Entities passed to a method all belong to the current
 * Embodiment.
 *******************************************************************************
 */
   private void reqEmbsMatchCur (Entity[] theCoordArr, String signature)
      {
      int        theIdx;
      Embodiment theEmb;
      
      for (theIdx = 0; theIdx < theCoordArr.length; theIdx ++)
         {
         if (theCoordArr[theIdx] == null)
            continue;
         
         theEmb = theCoordArr[theIdx].getEmbodiment ();
         
         if (theEmb != curEmb ())
            throw
               new IllegalArgumentException (
                  "\n\n"
                + "   Call to TupleCategory." + signature + ",\n"
                + "   where at least one of the coordinate Entities does not\n"
                + "   belong to the current Embodiment.\n"
                + "\n"
                + "      TupleCategory:         " +   getName ()          + "\n"
                + "      Coordinate Index:      " +   theIdx              + "\n"
                + "      Coordinate:            " +   theCoordArr[theIdx] + "\n"
                + "      Coordinate Embodiment: \"" + theEmb.getName () + "\"\n"
                + "      Current    Embodiment: \"" + curEmb ().
                                                      getName ()        + "\"\n"
                  );
         }
      }
   
/*******************************************************************************
 * Verifies that theCoordArr does not duplicate the coordinate list of a
 * Tuple of this TupleCategory in the current Embodiment.
 *******************************************************************************
 */
   private void banDupCoords (Entity[] theCoordArr)
      {
      Tuple theTuple;
      
      theTuple = curEmb ().getTupleMgr (this).getTuple (theCoordArr);
      
      if (theTuple != null)
         throw
            new IllegalArgumentException (
               "\n\n"
             + "   Call to TupleCategory.newTuple (Entity...)\n"
             + "   with a coordinate list that duplicates coordinate list of\n"
             + "   an existing Tuple of the TupleCategory in the current\n"
             + "   Embodiment.\n"
             + "\n"
             + "      Tuple: " + theTuple + "\n");
      }
   
////////////////////////////////////////////////////////////////////////////////
// Package Internal Constructor
////////////////////////////////////////////////////////////////////////////////

   TupleCategory (
         Scheme               theScheme,
         String               theName,
         EntityCategory <?>[] theDimenArr)
      {
      super (theScheme, theName);
      
      ArrayList <EntityCategory <?>> theDimens;
      
      theDimens = new ArrayList <EntityCategory <?>> (theDimenArr.length);
      
      Collections.addAll (theDimens, theDimenArr);
      
      myDimensions = Collections.unmodifiableList (theDimens);
      
      nullPattern  = new Entity[myDimensions.size ()];
      }

////////////////////////////////////////////////////////////////////////////////
// Package Internal Non-Static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Sets up uniqueDimenIndexArr.
 *******************************************************************************
 */    @Override 
   void freeze ()
      {
      int nCats;
      int theIdx;
      int theEntCatIdx;
      
      nCats = getScheme ().getEntityCategories ().size ();

      uniqueDimenIndexArr = new int[nCats];
      
      Arrays.fill (uniqueDimenIndexArr, 0, nCats, -1);
      
      for (theIdx = 0; theIdx < myDimensions.size (); theIdx ++)
         {
         theEntCatIdx = myDimensions.get (theIdx).getIndex ();
         
         if (uniqueDimenIndexArr[theEntCatIdx] == -1)
            {
            uniqueDimenIndexArr[theEntCatIdx] = theIdx;
            }
         else
            {
            uniqueDimenIndexArr[theEntCatIdx] = -2;
            }
         }
      }
   
/*******************************************************************************
 * {@inheritDoc}
 *******************************************************************************
 */    @Override
   void printConcrete (PrintStream theStream)
      {
      int theIdx;
      
      theStream.printf (
         "%n"
      +  "TupleCategory %s:%n%n",
         getName ());

      for (theIdx = 0; theIdx < myDimensions.size (); theIdx ++)
         {
         theStream.printf (
            "   Dimension #%d: %s%n",
            theIdx,
            myDimensions.get (theIdx).getName ());
         }
      }

/*******************************************************************************
 * {@inheritDoc}
********************************************************************************
 */                 @Override 
   EntityMgr <Tuple> newEntityMgr (Embodiment theEmb)
      {
      return new TupleMgr (this, theEmb);
      }

/*******************************************************************************
 * @return The "dimensions" of this TupleCategory, stored as an unmodifiable
 *         view of an ArrayList.
 *******************************************************************************
 */
   List <EntityCategory <?>> getDimensions ()
      {
      return myDimensions;
      }

/*******************************************************************************
 * @return The unique dimension index of theEntCat in this TupleCategory, if
 *         there is exactly one<br>
 *         -1, if there is no matching dimension<br>
 *         -2, if there is more than one matching dimension
 * 
 * @throws NullPointerException If the Scheme for this TupleCategory is not
 *                              frozen.
 *******************************************************************************
 */
   int uniqueDimenIndex (EntityCategory <?> theEntCat)
      {
      return uniqueDimenIndexArr[theEntCat.getIndex ()];
      }

////////////////////////////////////////////////////////////////////////////////
// Public Non-Static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Factory method:<br>
 * Constructs a new Tuple for this TupleCategory in the current Embodiment.
 *
 * @param theCoords                 The Entities that are to be the coordinates
 *                                  of the Tuple
 * @return                          The new Tuple
 * 
 * @throws NullPointerException     If any argument is null
 * @throws IllegalStateException    If the current Embodiment is null
 * @throws IllegalStateException    If the Scheme of this TupleCategory
 *                                  does not match the Scheme of the current
 *                                  Embodiment
 * @throws IllegalArgumentException If the Categories of the Entities do not
 *                                  match the corresponding dimension Categories
 *                                  of this TupleCategory
 * @throws IllegalArgumentException If the Entities do not all belong to the 
 *                                  current Embodiment
 * @throws IllegalArgumentException If the Entities match the coordinate list of
 *                                  an existing Tuple of this TupleCategory
 *                                  in the current Embodiment
 *******************************************************************************
 */
   public Tuple newTuple (Entity... theCoords)
      {
      List <Entity> theCoordList;
      
      banNullArray           (theCoords, "theCoords");
      reqCurEmb              ();
      reqSchemeMatchesCurEmb ();
      checkNCoords           (theCoords, "newTuple (theCoords)");
      checkCategories        (theCoords, "newTuple (theCoords)");
      reqEmbsMatchCur        (theCoords, "newTuple (theCoords)");
      banDupCoords           (theCoords);

      theCoordList = Arrays.asList (theCoords);
      
      return new Tuple (this, curEmb (), theCoordList);
      }
   
/*******************************************************************************
 * Tuple look-up method.
 * 
 * @param theCoords                 The list of coordinates for the Tuple to
 *                                  be retrieved
 * @return                          The Tuple of this TupleCategory in the
 *                                  current Embodiment whose coordinates
 *                                  match theCoords, if any; otherwise null.
 *                                  
 * @throws NullPointerException     If any argument is null
 * @throws IllegalStateException    If the current Embodiment is null
 * @throws IllegalStateException    If the Scheme of this TupleCategory
 *                                  does not match the Scheme of the current
 *                                  Embodiment
 * @throws IllegalArgumentException If the Categories of the Entities do not match
 *                                  the corresponding dimension Categories of
 *                                  this TupleCategory
 * @throws IllegalArgumentException If the Entities do not all belong to the 
 *                                  current Embodiment
 *******************************************************************************
 */
   public Tuple get (Entity... theCoords)
      {
      banNullArray           (theCoords,  "theCoords");
      reqCurEmb              ();
      reqSchemeMatchesCurEmb ();
      checkNCoords           (theCoords, "get (theCoords)");
      checkCategories        (theCoords, "get (theCoords)");
      reqEmbsMatchCur        (theCoords, "get (theCoords)");
      
      return curEmb ().getTupleMgr (this).getTuple (theCoords);
      }
   
/*******************************************************************************
 * Computes and returns a "slice": the set of all Tuples of this TupleCategory
 * in the current Embodiment whose coordinates match a "pattern". The pattern is
 * a sequence of n Entity references, where n is the number of dimensions in
 * this TupleCategory. Some of the Entity references may be null. A Tuple will
 * be in the slice, iff it belongs to this TupleCategory and to the current
 * Embodiment and for each coordinate of the Tuple, the corresponding Entity
 * reference in the pattern is either the same Entity as the coordinate or it's
 * a null pointer. Thus the nulls in the pattern function as "wild cards" that
 * match all Entities.<br>
 * <br>
 * The run time is proportional to the product of the number of Entities of each
 * unspecified (i.e. null) dimension in the current Embodiment.<br>
 * The Tuples are listed in lexicographic order.<br>
 * 
 * @param  thePattern The pattern to which the Tuples in the slice are to be
 *                    matched
 * @return            The slice
 * 
 * @throws NullPointerException     If thePattern is a null references
 * @throws IllegalStateException    If the current Embodiment is null
 * @throws IllegalStateException    If the Scheme of this TupleCategory
 *                                  does not match the Scheme of the current
 *                                  Embodiment
 * @throws IllegalArgumentException If the Categories of the Entities in
 *                                  thePattern do not match the corresponding
 *                                  dimension Categories of this TupleCategory
 * @throws IllegalArgumentException If the Entities in thePattern do not all
 *                                  belong to the current Embodiment
 *******************************************************************************
 */
   public List <Tuple> getSlice (Entity... thePattern)
      {
      banNull                (thePattern, "thePattern");
      reqCurEmb              ();
      reqSchemeMatchesCurEmb ();
      checkNCoords           (thePattern, "getSlice (thePattern)");
      checkCategories        (thePattern, "getSlice (thePattern)");
      reqEmbsMatchCur        (thePattern, "getSlice (thePattern)");
      
      return curEmb ().getTupleMgr (this).getSlice (thePattern);
      }
   
/*******************************************************************************
 * Builds a "slice" of Tuples: Creates every possible Tuple of this
 * TupleCategory in the current Embodiment whose coordinates match a pattern.
 * See getSlice for how the pattern matching is defined. Note that for any
 * Tuple that already exists whose coordinates match the pattern, no new Tuple
 * is created.<br>
 * <br>
 * The run time is proportional to the product of the number of Entities of each
 * unspecified (i.e. null) dimension in the current Embodiment.
 * The new Tuples are created in lexicographic order.<br>
 * 
 * @see    getSlice
 * 
 * @param  thePattern The pattern that the newly created Tuples will match
 * 
 * @throws NullPointerException     If thePattern is a null references
 * @throws IllegalStateException    If the current Embodiment is null
 * @throws IllegalStateException    If the Scheme of this TupleCategory
 *                                  does not match the Scheme of the current
 *                                  Embodiment
 * @throws IllegalArgumentException If the Categories of the Entities in
 *                                  thePattern do not match the corresponding
 *                                  dimension Categories of this TupleCategory
 * @throws IllegalArgumentException If the Entities in thePattern do not all
 *                                  belong to the current Embodiment
 *******************************************************************************
 */
   public void buildSlice (Entity... thePattern)
      {
      banNull                (thePattern, "thePattern");
      reqCurEmb              ();
      reqSchemeMatchesCurEmb ();
      checkNCoords           (thePattern, "buildSlice (thePattern)");
      checkCategories        (thePattern, "buildSlice (thePattern)");
      reqEmbsMatchCur        (thePattern, "buildSlice (thePattern)");
      
      curEmb ().getTupleMgr (this).buildSlice (thePattern);
      }
   
/*******************************************************************************
 * Creates every possible Tuple of this TupleCategory in the current Embodiment
 * that doesn't already exist.<br>
 * <br>
 * The run time is proportional to the product of the number of Entities of each
 * dimension of this TupleCategory in the current Embodiment.
 * The new Tuples are created in lexicographic order.
 * 
 * @throws IllegalStateException If the current Embodiment is null
 * @throws IllegalStateException If the Scheme of this TupleCategory
 *                               does not match the Scheme of the current
 *                               Embodiment
 *******************************************************************************
 */
   public void buildAll ()
      {
      reqCurEmb              ();
      reqSchemeMatchesCurEmb ();
      
      curEmb ().getTupleMgr (this).buildSlice (nullPattern);
      }
   
////////////////////////////////////////////////////////////////////////////////
}
