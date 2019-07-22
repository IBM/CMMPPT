
////////////////////////////////////////////////////////////////////////////////
// WIT-M Source File Tuple.java.
////////////////////////////////////////////////////////////////////////////////

package com.ibm.witm;

import static com.ibm.witm.Utility.*;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/*******************************************************************************
 * A Tuple is an Entity that represents a data object that's defined in terms
 * of other data objects. More precisely, a Tuple represents an ordered list
 * of other Entities. These Entities are called the coordinates of the Tuple and
 * the Tuple stores this coordinate list. Each Tuple belongs to a
 * particular TupleCategory and a particular Embodiment.<br><br>
 * 
 * The factory method for creating instances of this class is
 * TupleCategory.newTuple (theCoords)
 *******************************************************************************
 */
public final class Tuple extends Entity
{
////////////////////////////////////////////////////////////////////////////////
// Private Non-static Fields
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * The coordinate List of this Tuple.<br>
 * (An unmodifiable ArrayList)
 *******************************************************************************
 */
   private final List <Entity> myCoordinateList;
   
////////////////////////////////////////////////////////////////////////////////
// Private Non-static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Verifies that the Scheme of theEntCat matches the Scheme of this
 * Tuple.<br>
 * To be called by get (EntityCategory <E>).
 *******************************************************************************
 */
   private void reqMatchingScheme (EntityCategory <?> theEntCat)
      {
      if (theEntCat.getScheme () != getScheme ())
         throw
            new IllegalArgumentException (
               "\n\n"
             + "   Call to Tuple.get (EntityCategory <E>) with an\n"
             + "   EntityCategory whose Scheme does not match the\n"
             + "   Scheme of theTuple.\n"
             + "\n"
             + "      EntityCategory: " + theEntCat        .getName () + "\n"
             + "         Scheme:      " + theEntCat.getScheme ()
                                                           .getName () + "\n"
             + "      Tuple:          " + this                         + "\n"
             + "         Scheme:      " + getScheme ()     .getName () + "\n");
      }

/*******************************************************************************
 * Verifies that theEntCat is a dimension of the TupleCategory of this Tuple.
 * <br>
 * To be called by get (EntityCategory <E>).
 * 
 * @param dimIdx The valued returned by
 *               getTupleCategory ().uniqueDimenIndex (theEntCat)
 *******************************************************************************
 */
   private void reqDimen (EntityCategory <?> theEntCat, int dimIdx)
      {
      if (dimIdx == -1)
         throw
            new IllegalArgumentException (
               "\n\n"
             + "   Call to Tuple.get (theEntCat) with a EntityCategory that\n"
             + "   is not a dimension of the TupleCategory of the Tuple.\n"
             + "\n"
             + "      EntityCategory: " + theEntCat.getName () + "\n"
             + "      Tuple:          " + this                 + "\n");
      }

/*******************************************************************************
 * Verifies that theEntCat does not occur more than once as a dimension of the
 * TupleCategory of this Tuple.<br>
 * To be called by get (EntityCategory <E>).
 * 
 * @param dimIdx The valued returned by
 *               getTupleCategory ().uniqueDimenIndex (theEntCat)
 *******************************************************************************
 */
   private void banDupDimen (EntityCategory <?> theEntCat, int dimIdx)
      {
      if (dimIdx == -2)
         throw
            new IllegalArgumentException (
               "\n\n"
             + "   Call to Tuple.get (theEntCat) with a EntityCategory that\n"
             + "   occurs more than once in the dimensions of the\n"
             + "   TupleCategory of the Tuple.\n"
             + "\n"
             + "      EntityCategory: " + theEntCat.getName () + "\n"
             + "      Tuple:          " + this                 + "\n");
      }

/*******************************************************************************
 * Verifies that dimIdx is within the range of valid dimension indices for this
 * Tuple.
 *******************************************************************************
 */
   private void reqDimIdxInRange (int dimIdx)
      {
      int nDimens;
      
      nDimens = getTupleCategory ().getDimensions ().size ();
      
      if ((dimIdx < 0) || (dimIdx >= nDimens))
         throw
            new IndexOutOfBoundsException (
               "\n\n"
             + "   Call to Tuple.get (theEntCat, dimIdx), where dimIdx is\n"
             + "   outside of the range 0 <= dimIdx < #dimensions.\n"
             + "\n"
             + "      Tuple:        " + this    + "\n"
             + "      dimIdx:       " + dimIdx  + "\n"
             + "      # dimensions: " + nDimens + "\n");
      }

/*******************************************************************************
 * Verifies that theEntCat matches the dimension of the TupleCategory indexed
 * by dimIdx.
 *******************************************************************************
 */
   private void reqIndexedDimMatch (EntityCategory <?> theEntCat, int dimIdx)
      {
      EntityCategory<?> indexedDim;
      
      indexedDim = getTupleCategory ().getDimensions ().get (dimIdx);
      
      if (theEntCat != indexedDim)
         throw
            new IllegalArgumentException (
               "\n\n"
             + "   Call to Tuple.get (theEntCat, dimIdx), where theEntCat\n"
             + "   does not match the TupleCategory's dimension indexed by\n"
             + "   dimIdx.\n"
             + "\n"
             + "      Tuple:             " + this       + "\n"
             + "      theEntCat:         " + theEntCat  + "\n"
             + "      dimIdx:            " + dimIdx     + "\n"
             + "      Indexed Dimension: " + indexedDim + "\n");
      }

////////////////////////////////////////////////////////////////////////////////
// Package Internal Constructor
////////////////////////////////////////////////////////////////////////////////
   
   Tuple (
         TupleCategory theTupleCat,
         Embodiment    theEmb,
         List <Entity> theCoordList)
      {
      super (theTupleCat, theEmb);
      
      ArrayList <Entity> theArrayList;
      
      theArrayList     = new ArrayList <Entity> (theCoordList.size ());
      
      theArrayList.addAll (theCoordList);
      
      myCoordinateList = Collections.unmodifiableList (theArrayList);
      
      theEmb.getTupleMgr (theTupleCat).storeTuple (this);
      }
   
////////////////////////////////////////////////////////////////////////////////
// Package Internal Non-static Methods
////////////////////////////////////////////////////////////////////////////////
   
/*******************************************************************************
 * @return The coordinate list of this Tuple as an unmodifiable ArrayList
 *******************************************************************************
 */
   List <Entity> getCoordinateList ()
      {
      return myCoordinateList;
      }

/*******************************************************************************
 * {@inheritDoc}
 *******************************************************************************
 */      @Override
   String getID ()
      {
      TupleMgr theTupleMgr;
      
      theTupleMgr = getEmbodiment ().getTupleMgr (getTupleCategory ());
      
      return theTupleMgr.makeIDFor (this);
      }

////////////////////////////////////////////////////////////////////////////////
// Public Non-static Methods
////////////////////////////////////////////////////////////////////////////////
   
/*******************************************************************************
 * @return The TupleCategory of this Tuple
 *******************************************************************************
 */
   public TupleCategory getTupleCategory ()
      {
      return (TupleCategory) getEntityCategory ();
      }

/*******************************************************************************
 * If there is exactly one dimension in the TupleCategory for this Tuple
 * that matches theEntCat, the corresponding coordinate for this Tuple is
 * returned.
 * 
 * @param  <E>       The type of Entity to be retrieved (either Item or Tuple)
 * @param  theEntCat The EntityCategory <E> of the dimension being specified.
 * @return           The coordinate of this Tuple for the specified dimension
 * 
 * @throws NullPointerException     If any argument is null
 * @throws IllegalArgumentException If the Scheme of theEntCat does not
 *                                  match the Scheme of this Tuple.
 * @throws IllegalArgumentException If theEntCat is not a dimension of the
 *                                  TupleCategory of this Tuple
 * @throws IllegalArgumentException If theEntCat occurs more than once as a
 *                                  dimension of the TupleCategory of this Tuple
 *******************************************************************************
 */                           @SuppressWarnings("unchecked")
   public <E extends Entity> E get (EntityCategory <E> theEntCat)
      {
      int dimIdx;
      
      banNull           (theEntCat, "theEntCat");
      reqMatchingScheme (theEntCat);

      dimIdx = getTupleCategory ().uniqueDimenIndex (theEntCat);
      
      reqDimen    (theEntCat, dimIdx);
      banDupDimen (theEntCat, dimIdx);
      
      return (E) myCoordinateList.get (dimIdx);
      }

/*******************************************************************************
 * Returns the coordinate of this Tuple for a dimension specified by its index.
 * 
 * @param  <E>       The type of Entity to be retrieved (either Item or Tuple)
 * @param  theEntCat The EntityCategory <E> of the dimension being specified.
 * @param  dimIdx    The index of the dimension being specified.
 * @return           The coordinate of this Tuple for the specified dimension
 * 
 * @throws NullPointerException      If any argument is null
 * @throws IndexOutOfBoundsException If dimIdx is outside of the range
 *                                   0 <= dimIdx < #dimensions
 * @throws IllegalArgumentException  If theEntCat does not match the
 *                                   TupleCategory's dimension indexed by dimIdx
 *******************************************************************************
 */                           @SuppressWarnings("unchecked")
   public <E extends Entity> E get (EntityCategory <E> theEntCat, int dimIdx)
      {
      banNull            (theEntCat, "theEntCat");
      reqDimIdxInRange   (dimIdx);
      reqIndexedDimMatch (theEntCat, dimIdx);

      return (E) myCoordinateList.get (dimIdx);
      }

////////////////////////////////////////////////////////////////////////////////
}
