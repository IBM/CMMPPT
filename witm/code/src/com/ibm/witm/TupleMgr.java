
////////////////////////////////////////////////////////////////////////////////
// WIT-M Source File TupleMgr.java.
////////////////////////////////////////////////////////////////////////////////

package com.ibm.witm;

import static com.ibm.witm.Utility.*;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;

/*******************************************************************************
 * "Tuple Manager"<br>
 * A TupleMgr is responsible for maintaining the Tuples of a particular
 * TupleCategory that belong to a particular Embodiment.
 *******************************************************************************
 */
final class TupleMgr extends EntityMgr <Tuple>
{
////////////////////////////////////////////////////////////////////////////////
// Private Non-Static Fields
////////////////////////////////////////////////////////////////////////////////
   
/*******************************************************************************
 * The TupleCategory for this TupleMgr.
 *******************************************************************************
 */
   private final TupleCategory myTupleCategory;
   
/*******************************************************************************
 * The dimensions of the TupleCategory for this TupleMgr.
 *******************************************************************************
 */
   private final List <EntityCategory <?>> myDimensions;
   
/*******************************************************************************
 * The Embodiment to which this TupleMgr belongs.
 *******************************************************************************
 */
   private final Embodiment myEmbodiment;
   
/*******************************************************************************
 * myTupleMap.get (theCoordList) is the Tuple stored by this TupleMgr
 * whose coordinate list matches theCoordList, if any; otherwise null.
 *******************************************************************************
 */
   private final HashMap <List <Entity>, Tuple> myTupleMap;
   
/*******************************************************************************
 * During the execution of getSlice (), this is the coordinate pattern that the
 * Tuples in the slice are to match; otherwise null.
 *******************************************************************************
 */
   private Entity[] curPattern;
   
/*******************************************************************************
 * During the execution of getSlice (), this is the Slice currently being
 * built-up; otherwise null.
 *******************************************************************************
 */
   private ArrayList <Tuple> curSlice;
   
/*******************************************************************************
 * During the execution of getSlice ...(), this is the coordinate
 * list that will be used to look up the next Tuple in the slice.
 *******************************************************************************
 */
   private final ArrayList <Entity> curCoordList;
   
////////////////////////////////////////////////////////////////////////////////
// Private Non-Static Methods
////////////////////////////////////////////////////////////////////////////////
   
/*******************************************************************************
 * Populates the slice for getSlice with Tuples whose coordinates for
 * indices < coordIdx match those in curCoordList and whose coordinate at
 * coordIdx is theEnt and whose coordinates for indices > coordIdx match
 * curPattern.
 *******************************************************************************
 */
   private void popSliceWithEntity (int coordIdx, Entity theEnt)
      {
      Tuple theTuple;
      
      curCoordList.set (coordIdx, theEnt);
         
      if (coordIdx == myDimensions.size () - 1)
         {
         theTuple = myTupleMap.get (curCoordList);
            
         if (theTuple != null)
            curSlice.add (theTuple);
         }
      else
         popSliceFromIdx (coordIdx + 1);
         
      curCoordList.set (coordIdx, null);
      }
   
/*******************************************************************************
 * Populates the slice for getSlice with all Tuples whose coordinates for
 * indices < coordIdx match those in curCoordList and whose coordinates for
 * indices >= coordIdx match curPattern.
 *******************************************************************************
 */
   private void popSliceFromIdx (int coordIdx)
      {
      EntityCategory <?> theEntCat;
      EntityMgr <?>      theEntMgr;
      
      if (curPattern[coordIdx] == null)
         {
         theEntCat = myDimensions.get (coordIdx);
      
         theEntMgr = myEmbodiment.getEntityMgr (theEntCat);
      
         for (Entity theEnt: theEntMgr.getAll ())
            {
            popSliceWithEntity (coordIdx, theEnt);
            }
         }
      else
         popSliceWithEntity (coordIdx, curPattern[coordIdx]);
      }
   
/*******************************************************************************
 * For buildSlice, creates all non-existing Tuples whose coordinates for
 * indices < coordIdx match those in curCoordList and whose coordinate at
 * coordIdx is theEnt and whose coordinates for indices > coordIdx match
 * curPattern.
 *******************************************************************************
 */
   private void buildSliceWithEntity (int coordIdx, Entity theEnt)
      {
      Tuple theTuple;
      
      curCoordList.set (coordIdx, theEnt);
      
      if (coordIdx == myDimensions.size () - 1)
         {
         theTuple = myTupleMap.get (curCoordList);
            
         if (theTuple == null)
            new Tuple (myTupleCategory, myEmbodiment, curCoordList);
         }
      else
         buildSliceFromIdx (coordIdx + 1);
         
      curCoordList.set (coordIdx, null);
      }
   
/*******************************************************************************
 * For buildSlice, creates all non-existing Tuples whose coordinates for
 * indices < coordIdx match those in curCoordList and whose coordinates for
 * indices >= coordIdx match curPattern.
 *******************************************************************************
 */
   private void buildSliceFromIdx (int coordIdx)
      {
      EntityCategory <?> theEntCat;
      EntityMgr <?>      theEntMgr;
      
      if (curPattern[coordIdx] == null)
         {
         theEntCat = myDimensions.get (coordIdx);
      
         theEntMgr = myEmbodiment.getEntityMgr (theEntCat);
      
         for (Entity theEnt: theEntMgr.getAll ())
            {
            buildSliceWithEntity (coordIdx, theEnt);
            }
         }
      else
         buildSliceWithEntity (coordIdx, curPattern[coordIdx]);
      }
   
////////////////////////////////////////////////////////////////////////////////
// Package Internal Constructor
////////////////////////////////////////////////////////////////////////////////
   
   TupleMgr (TupleCategory theTupleCat, Embodiment theEmb)
      {
      int nDimens;
      int theIdx;
      
      myTupleCategory = theTupleCat;
      
      myDimensions    = theTupleCat.getDimensions ();
   
      myEmbodiment    = theEmb;
   
      myTupleMap      = new HashMap <List <Entity>, Tuple> ();
      
      curPattern      = null;
      curSlice        = null;
      
      nDimens         = myDimensions.size ();
      
      curCoordList    = new ArrayList <Entity> (nDimens);
      
      for (theIdx = 0; theIdx < nDimens; theIdx ++)
         curCoordList.add (null);
      }
   
////////////////////////////////////////////////////////////////////////////////
// Package Internal Non-Static Methods
////////////////////////////////////////////////////////////////////////////////
   
/*******************************************************************************
 * Stores theTuple in this TupleMgr.
 *******************************************************************************
 */
   void storeTuple (Tuple theTuple)
      {
      List <Entity> theCoordList;
      Tuple         oldTuple;
      
      storeEntity (theTuple);
      
      theCoordList = theTuple.getCoordinateList ();
      
      oldTuple     = myTupleMap.put (theCoordList, theTuple);
      
      assert (oldTuple == null);
      }
   
/*******************************************************************************
 * Builds and returns the String to be used by theTuple.getID ().
 *******************************************************************************
 */
   String makeIDFor (Tuple theTuple)
      {
      int      nStrings;
      String[] theStringArr;
      int      theIdx;
      
      nStrings     = (2 * theTuple.getCoordinateList ().size ()) + 1;
      
      theStringArr = new String[nStrings];
      
      theIdx       = 0;
      
      theStringArr[theIdx ++] = "(";
      
      for (Entity theEnt: theTuple.getCoordinateList ())
         {
         theStringArr[theIdx ++] = theEnt.toString ();
         
         theStringArr[theIdx ++] = ", ";
         }
      
      theStringArr[theIdx - 1] = ")";
      
      return concatenation (theStringArr);
      }
   
/*******************************************************************************
 * @return The Tuple stored by this TupleMgr whose coordinate list matches
 *         theCoordArr, if any; otherwise null.
 *******************************************************************************
 */
   Tuple getTuple (Entity[] theCoordArr)
      {
      List <Entity> theCoordList;
      
      theCoordList = Arrays.asList (theCoordArr);
      
      return myTupleMap.get (theCoordList);
      }
   
/*******************************************************************************
 * Computes and returns a "slice": the set of all Tuples maintained by this
 * TupleMgr whose coordinates match thePattern.
 *******************************************************************************
 */
   ArrayList <Tuple> getSlice (Entity[] thePattern)
      {
      ArrayList <Tuple> theSlice;
      
      curPattern = thePattern;
      
      curSlice   = new ArrayList <Tuple> ();
      
      popSliceFromIdx (0);
      
      theSlice   = curSlice;
      curPattern = null;
      curSlice   = null;
      
      return theSlice;
      }
   
/*******************************************************************************
 * Builds all non-existing Tuples that match thePattern.
 *******************************************************************************
 */
   void buildSlice (Entity[] thePattern)
      {
      curPattern = thePattern;
      
      buildSliceFromIdx (0);

      curPattern = null;
      }
   
////////////////////////////////////////////////////////////////////////////////
}
