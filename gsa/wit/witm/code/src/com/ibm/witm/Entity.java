
////////////////////////////////////////////////////////////////////////////////
// WIT-M Source File Entity.java.
////////////////////////////////////////////////////////////////////////////////

package com.ibm.witm;

import static com.ibm.witm.ArrayWrapper.*;
import static com.ibm.witm.Utility.*;

import com.ibm.witj.*;

/*******************************************************************************
 * An Entity represents a data object in a WIT application problem. Each
 * Entity belongs to a particular Embodiment and a particular EntityCategory.
 * The Entities that belong to a given Embodiment are the building blocks
 * that make up the Embodiment.
 *******************************************************************************
 */
public abstract class Entity
{
////////////////////////////////////////////////////////////////////////////////
// Private Non-Static Fields
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * The EntityCategory to which this Entity belongs.
 *******************************************************************************
 */
   private final EntityCategory <?> myEntityCategory;
   
/*******************************************************************************
 * The Embodiment that owns this Entity.
 *******************************************************************************
 */
   private final Embodiment myEmbodiment;
   
/*******************************************************************************
 * myPropValueArr[theProp.getIndex ()] is the value of theProp associated with
 * this Entity.
 *******************************************************************************
 */
   private final Object[] myPropValueArr;
   
////////////////////////////////////////////////////////////////////////////////
// Private Non-Static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * @return The WIT-J Problem associated with myEmbodiment.
 *******************************************************************************
 */
   private final Problem getProblem ()
      {
      return myEmbodiment.getProblem ();
      }
   
/*******************************************************************************
 * Verifies that the EntityCategory of theProp matches the EntityCategory of
 * this Entity.
 *******************************************************************************
 */
   private final void reqCatsMatch (Property <?> theProp, String signature)
      {
      if (theProp.getEntityCategory () != myEntityCategory)
         throw
            new IllegalArgumentException (
               "\n\n"
             + "   Call to Entity." + signature + ", where the EntityCategory\n"
             + "   of the Property does not match the EntityCategory of the\n"
             + "   Entity.\n"
             + "\n"
             + "      Property:          " + theProp                      + "\n"
             + "         EntityCategory: " + theProp.getEntityCategory () + "\n"
             + "      Entity:            " + this                         + "\n"
             + "         EntityCategory: " + myEntityCategory             + "\n"
             );
      }
   
/*******************************************************************************
 * Verifies that the EntityCategory of theProp matches the EntityCategory of
 * this Entity.
 *******************************************************************************
 */
   private final void reqTupleCat (Property <?> theProp, String signature)
      {
      if (! (theProp.getEntityCategory () instanceof TupleCategory))
         throw
            new IllegalArgumentException (
               "\n\n"
             + "   Call to Entity." + signature              + " with a\n"
             + "   Property whose EntityCategory is not a TupleCategory.\n"
             + "\n"
             + "      Property:          " + theProp                      + "\n"
             + "         EntityCategory: " + theProp.getEntityCategory () + "\n"
             );
      }
   
/*******************************************************************************
 * Verifies that the # of dimensions of theTupleCat is 2.
 *******************************************************************************
 */
   private final void req2Dims (
         Property <?>  theProp,
         TupleCategory theTupleCat,
         String        signature)
      {
      int nDims;
      
      nDims = theTupleCat.getDimensions ().size ();
      
      if (nDims != 2)
         throw
            new IllegalArgumentException (
               "\n\n"
             + "   Call to Entity." + signature              + " with a\n"
             + "   Property of a TupleCategory that does not have two\n"
             + "   dimensions.\n"
             + "\n"
             + "      Property:         " + theProp     + "\n"
             + "         TupleCategory: " + theTupleCat + "\n"
             + "         # Dimensions:  " + nDims       + "\n");
      }
   
/*******************************************************************************
 * Verifies that dimension #0 of theTupleCat is myEntityCategory.
 *******************************************************************************
 */
   private final void reqDim0IsMyCat (
         Property <?>  theProp,
         TupleCategory theTupleCat,
         String        signature)
      {
      EntityCategory <?> dim0;
      
      dim0 = theTupleCat.getDimensions ().get (0);
      
      if (dim0 != myEntityCategory)
         throw
            new IllegalArgumentException (
               "\n\n"
             + "   Call to Entity." + signature              + " with a\n"
             + "   Property of a TupleCategory whose dimension #0 is not the\n"
             + "   EntityCategory of the Entity.\n"
             + "\n"
             + "      Property:          " + theProp          + "\n"
             + "         TupleCategory:  " + theTupleCat      + "\n"
             + "         Dimension #0:   " + dim0             + "\n"
             + "      Entity:            " + this             + "\n"
             + "         EntityCategory: " + myEntityCategory + "\n");
      }
   
/*******************************************************************************
 * Verifies that dimension #1 of theTupleCat is the Period Category.
 *******************************************************************************
 */
   private final void reqDim1PeriodCat (
         Property <?>  theProp,
         TupleCategory theTupleCat,
         String        signature)
      {
      EntityCategory <?> dim1;
      
      dim1 = theTupleCat.getDimensions ().get (1);
      
      if (! dim1.isPeriodCategory ())
         throw
            new IllegalArgumentException (
               "\n\n"
             + "   Call to Entity." + signature              + " with a\n"
             + "   Property of a TupleCategory whose dimension #1 is not the\n"
             + "   Period Category.\n"
             + "\n"
             + "      Property:         " + theProp        + "\n"
             + "         TupleCategory: " + theTupleCat    + "\n"
             + "         Dimension #1:  " + dim1           + "\n");
      }
   
/*******************************************************************************
 * Verifies that the length of an array matches the # of Periods.
 * 
 * @param theArrWrap An ArrayWrapper for the array.
 *******************************************************************************
 */
   private final void reqArrLenMatchesNPerItems (
         String           signature,
         ArrayWrapper <?> theArrWrap)
      {
      int nPers;
      
      nPers = getEmbodiment ().getPeriodMgr ().getAll ().size ();
      
      if (theArrWrap.length () != nPers)
         throw
            new IllegalArgumentException (
               "\n\n"
             + "   Call to Entity." + signature + " with an\n"
             + "   array whose length does not match the number of Period\n"
             + "   Items in the Embodiment.\n"
             + "\n"
             + "      Array Length: " + theArrWrap.length () + "\n"
             + "      # Periods:    " + nPers                + "\n");
      }
   
/*******************************************************************************
 * Does error checking for the setByPeriod and getByPeriod methods.
 * 
 * @param signature The signature of the public method being executed.
 *******************************************************************************
 */
   private final void checkSetGetByPeriod (
         Property <?>     theProp,
         ArrayWrapper <?> theArrWrap,
         String           signature)
      {
      TupleCategory theTupleCat;
      
      banNull     (theProp,                      "theProp");
      banNull     (theArrWrap.getArrayObject (), "theArr");
      reqTupleCat (theProp,                      signature);
      
      theTupleCat = (TupleCategory) theProp.getEntityCategory ();
      
      req2Dims         (theProp, theTupleCat, signature);
      reqDim0IsMyCat   (theProp, theTupleCat, signature);
      reqDim1PeriodCat (theProp, theTupleCat, signature);
      
      reqArrLenMatchesNPerItems (signature, theArrWrap);
      }

/*******************************************************************************
 * Performs the actions of setByPeriod (...) for a specific Period.
 *******************************************************************************
 */
   private final <E> void setOnePeriod (
         Property <E>     theProp,
         ArrayWrapper <E> theArrWrap,
         int              theIdx,
         Tuple            theTuple)
      {
      E theValue;
      
      if (theTuple == null)
         return;

      theValue = theArrWrap.getElement (theIdx);
         
      theProp.setValueFor (theTuple, theValue);
      }

/*******************************************************************************
 * Performs the actions of getByPeriod (...) for a specific Period.
 *******************************************************************************
 */
   private final <E> void getOnePeriod (
         Property <E>     theProp,
         ArrayWrapper <E> theArrWrap,
         int              theIdx,
         Tuple            theTuple,
         E                defaultVal)
      {
      E theValue;
      
      if (theTuple == null)
         theValue = defaultVal;
      else
         {
         theValue = theTuple.getValue (theProp);

         if (theValue == null)
            theValue = defaultVal;
         }

      theArrWrap.setElement (theIdx, theValue);
      }

/*******************************************************************************
 * Internal, generic version of the setByPeriod and getByPeriod methods.
 * 
 * @param theArrWrap Wrapper the array parameter.
 * @param defaultVal Used for getByPeriod, but not setByPeriod.
 * @param forSet     True, if called for setByPeriod; false, if called for
 *                   getByPeriod.
 *******************************************************************************
 */
   private final <E> void setGetByPeriod (
         Property <E>     theProp,
         ArrayWrapper <E> theArrWrap,
         E                defaultVal,
         boolean          forSet)
      {
      TupleCategory theTupleCat;
      TupleMgr      theTupleMgr;
      Entity[]      theCoordArr;
      int           theIdx;
      Tuple         theTuple;

      theTupleCat = (TupleCategory) theProp.getEntityCategory ();
      
      theTupleMgr = getEmbodiment ().getTupleMgr (theTupleCat);
      
      theCoordArr = new Entity[]{this, null};
      
      for (Item thePerItem: getEmbodiment ().getPeriodMgr ().getAll ())
         {
         theIdx         = thePerItem.getPeriodIndex ();
         
         theCoordArr[1] = thePerItem;
         
         theTuple       = theTupleMgr.getTuple (theCoordArr);

         if (forSet)
            setOnePeriod (theProp, theArrWrap, theIdx, theTuple);
         else
            getOnePeriod (theProp, theArrWrap, theIdx, theTuple, defaultVal);
         }
      }

/*******************************************************************************
 * Internal, generic version of the setByPeriod methods.
 *******************************************************************************
 */
   private final <E> void setByPeriod (
         Property <E>     theProp,
         ArrayWrapper <E> theArrWrap)
      {
      checkSetGetByPeriod (theProp, theArrWrap, "setByPeriod (theProp, theArr)");
      
      setGetByPeriod (theProp, theArrWrap, null, true);
      }
   
/*******************************************************************************
 * Internal, generic version of the getByPeriod methods.
 *******************************************************************************
 */
   private final <E> void getByPeriod (
         Property <E>     theProp,
         ArrayWrapper <E> theArrWrap,
         E                defaultVal)
      {
      checkSetGetByPeriod (
         theProp,
         theArrWrap,
         "getByPeriod (theProp, theArr, defaultVal)");
      
      setGetByPeriod (theProp, theArrWrap, defaultVal, false);
      }

/*******************************************************************************
 * Verifies that myEmbodiment is associated with a WIT-J Problem.
 *******************************************************************************
 */
   private void reqProb (String methName)
      {
      if (getProblem () == null)
         {
         throw
            new IllegalStateException (
               "\n\n"
             + "   Call to Entity." + methName + " (...) on an Entity whose\n"
             + "   Embodiment is not associated with a WIT-J Problem.\n"
             + "\n"
             + "      Entity: " + this + "\n");
         }
      }

/*******************************************************************************
 * Computes and returns the name to be used used for a new Part, Operation, or
 * Demand.
 *******************************************************************************
 */
   private String makeName (Property <?> theProp, int theCount)
      {
      String theCountStr;
      
      theCountStr = Integer.toString (theCount);
      
      if (myEmbodiment.getUseShortNames ())
         return theCountStr;
      else
         return
            concatenation (theProp.getName (), ":", getID (), "#", theCountStr);
      }

////////////////////////////////////////////////////////////////////////////////
// Package-Internal Constructor
////////////////////////////////////////////////////////////////////////////////
   
   Entity (EntityCategory <?> theEntCat, Embodiment theEmb)
      {
      assert (theEmb.getScheme () == theEntCat.getScheme ());
      
      myEntityCategory = theEntCat;
      
      myEmbodiment     = theEmb;
      
      myPropValueArr   = new Object[theEntCat.getProperties ().size ()];
      }

////////////////////////////////////////////////////////////////////////////////
// Package-Internal Non-Static Methods
////////////////////////////////////////////////////////////////////////////////
   
/*******************************************************************************
 * Returns a String that identifies this Entity among Entities of its
 * EntityCategory. Note that the String returned might not be unique.
 *******************************************************************************
 */
   abstract String getID ();

/*******************************************************************************
 * Sets the value of a Property for this Entity to theValue.
 * (Internal Version)
 *******************************************************************************
 */
   final <V> void setValue (Property <V> theProp, V theValue)
      {
      myPropValueArr[theProp.getIndex ()] = theValue;
      }

/*******************************************************************************
 * Retrieves the value of a Property for this Entity.
 * (Internal Version)
 *******************************************************************************
 */           @SuppressWarnings("unchecked")
   final <V> V getValue (Property <V> theProp)
      {
      return (V) myPropValueArr[theProp.getIndex ()];
      }

////////////////////////////////////////////////////////////////////////////////
// Public Non-Static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * @return The EntityCategory to which this Entity belongs
 *******************************************************************************
 */
   public final EntityCategory <?> getEntityCategory ()
      {
      return myEntityCategory;
      }

/*******************************************************************************
 * @return The Scheme for this Entity.
 *******************************************************************************
 */
   public final Scheme getScheme ()
      {
      return myEmbodiment.getScheme ();
      }

/*******************************************************************************
 * @return The Embodiment that owns this Entity
 *******************************************************************************
 */
   public final Embodiment getEmbodiment ()
      {
      return myEmbodiment;
      }

/*******************************************************************************
 * {@inheritDoc}
 *******************************************************************************
 */                   @Override
   public final String toString ()
      {
      return concatenation (myEntityCategory.getName (), " ", getID ());
      }
   
/*******************************************************************************
 * Sets the value of a Property for this Entity to a specified value.
 * Specifically, a reference to the specified object is stored, rather than a
 * copy of it.
 * 
 * @param <V>      The value type of the Property
 * @param theProp  The Property whose value is to be set
 * @param theValue The V instance to which the Property value is to be set.
 *
 * @throws NullPointerException     If theProp is null
 * @throws IllegalArgumentException If the EntityCategory of theProp does not
 *                                  match the EntityCategory of this Entity.
 *******************************************************************************
 */
   public final <V> void set (Property <V> theProp, V theValue)
      {
      banNull      (theProp, "theProp");
      reqCatsMatch (theProp, "set (theProp, theValue)");
      
      theProp.setValueFor (this, theValue);
      }

/*******************************************************************************
 * Retrieves the value of a Property for this Entity.
 * 
 * @param <V>     The value type of the Property
 * @param theProp The Property whose value is to be retrieved
 * @return        A reference to the current value of the Property for this
 *                Entity
 * 
 * @throws NullPointerException     If theProp is null
 * @throws IllegalArgumentException If the EntityCategory of theProp does not
 *                                  match the EntityCategory of this Entity.
 *******************************************************************************
 */
   public final <V> V get (Property <V> theProp)
      {
      banNull      (theProp, "theProp");
      reqCatsMatch (theProp, "get (theProp)");
      
      return getValue (theProp);
      }

/*******************************************************************************
 * Sets the values of a Property for a 1-dimensional slice of a 2-dimensional
 * TupleCategory to the elements of an array of doubles. The slice is defined by
 * this Entity and the set of all Periods in the Embodiment.<br>
 * <br>
 * Preconditions:<br>
 * The EntityCategory of the Property must be a two-dimensional TupleCategory
 * whose dimension #0 is the EntityCategory of this Entity and whose dimension
 * #1 is the Period Category. The length of the array must match the number of
 * of Periods in the Embodiment of this Entity.<br>
 * <br>
 * Actions:<br>
 * For each Period in the Embodiment, this method performs the following
 * actions:
 * <ul>
 * <li>
 * Finds the Tuple whose coordinates are this Entity and the Period, if such a
 * Tuple exists.
 * <li>
 * If the Tuple exists, the value of the Property for the Tuple is set to the
 * value of the array element indexed by the period index of the Period.
 * <li>
 * If the Tuple does not exist, the Period is skipped.
 * </ul>
 * @param  theProp                  The Property whose values are to be set
 * @param  theArr                   The array that holds the values to be
 *                                  stored
 * 
 * @throws NullPointerException     If any argument is null
 * @throws IllegalArgumentException If the EntityCategory of theProp is not a
 *                                  TupleCategory
 * @throws IllegalArgumentException If the TupleCategory of theProp does not
 *                                  have exactly two dimensions
 * @throws IllegalArgumentException If dimension #0 of the TupleCategory of
 *                                  theProp is not the EntityCategory of this
 *                                  Entity
 * @throws IllegalArgumentException If dimension #1 of the TupleCategory of
 *                                  theProp is not the Period Category
 * @throws IllegalArgumentException If the length of the array does not match
 *                                  the number of Periods
 *******************************************************************************
 */
   public final void setByPeriod (Property <Double> theProp, double[] theArr)
      {
      setByPeriod (theProp, newArrayWrapper (theArr));
      }
   
/*******************************************************************************
 * Sets the values of a Property for a 1-dimensional slice of a 2-dimensional
 * TupleCategory to the elements of an array of ints.
 * @see {@literal setByPeriod (Property <Double>, ...)}
 *******************************************************************************
 */
   public final void setByPeriod (Property <Integer> theProp, int[] theArr)
      {
      setByPeriod (theProp, newArrayWrapper (theArr));
      }
   
/*******************************************************************************
 * Sets the values of a Property for a 1-dimensional slice of a 2-dimensional
 * TupleCategory to the elements of an array of booleans.
 * @see {@literal setByPeriod (Property <Double>, ...)}
 *******************************************************************************
 */
   public final void setByPeriod (Property <Boolean> theProp, boolean[] theArr)
      {
      setByPeriod (theProp, newArrayWrapper (theArr));
      }
   
/*******************************************************************************
 * Retrieves the values of a Property for a 1-dimensional slice of a
 * 2-dimensional TupleCategory, storing them in an array of doubles. The slice
 * is defined by this Entity and the set of all Periods in the
 * Embodiment.<br>
 * <br>
 * Preconditions:<br>
 * The EntityCategory of the Property must be a two-dimensional TupleCategory
 * whose dimension #0 is the EntityCategory of this Entity and whose dimension
 * #1 is the Period Category. The length of the array must match the number of
 * Periods in the Embodiment of this Entity.<br>
 * <br>
 * Actions:<br>
 * For each Period in the Embodiment, this method performs the following
 * actions:
 * <ul>
 * <li>
 * Finds the Tuple whose coordinates are this Entity and the Period, if such a
 * Tuple exists.
 * <li>
 * If the Tuple exists and has a non-null value for the Property, this method
 * sets the value of the array element indexed by the period index of the Period
 * Item to the Property value of the Tuple.
 * <li>
 * If the Tuple does not exist or has a null value for the Property, this method
 * sets the value of the array element to defaultVal.
 * </ul>
 * 
 * @see    setByPeriod
 * @param  theProp    The Property whose values are to be retrieved
 * @param  theArr     The array in which the retrieved values are to be stored
 * @param  defaultVal The default value to be stored in the array elements.
 * @throws -          Same as setByPeriod
 *******************************************************************************
 */
   public final void getByPeriod (
         Property <Double> theProp,
         double[]          theArr,
         double            defaultVal)
      {
      getByPeriod (theProp, newArrayWrapper (theArr), defaultVal);
      }

/*******************************************************************************
 * Retrieves the values of a Property for a 1-dimensional slice of a
 * 2-dimensional TupleCategory, storing them in an array of ints.
 * @see {@literal getByPeriod (Property <Double>, ...)}
 *******************************************************************************
 */
   public final void getByPeriod (
         Property <Integer> theProp,
         int[]              theArr,
         int                defaultVal)
      {
      getByPeriod (theProp, newArrayWrapper (theArr), defaultVal);
      }

/*******************************************************************************
 * Retrieves the values of a Property for a 1-dimensional slice of a
 * 2-dimensional TupleCategory, storing them in an array of booleans.
 * @see {@literal getByPeriod (Property <Double>, ...)}
 *******************************************************************************
 */
   public final void getByPeriod (
         Property <Boolean> theProp,
         boolean[]          theArr,
         boolean            defaultVal)
      {
      getByPeriod (theProp, newArrayWrapper (theArr), defaultVal);
      }

/*******************************************************************************
 * Creates a new WIT-J Part and stores a reference to it as the value of a
 * Property of this Entity. The Part will belong to the WIT-J Problem associated
 * with the Embodiment for this Entity.<br><br>
 * 
 * The name of the Part is computed by the following naming algorithm:
 * <ul>
 * <li>
 * If useShortNames is true for the Embodiment, then the name will be the number
 * of Parts already created, converted to a String.
 * <li>
 * If useShortNames is false for the Embodiment, then the name will be the name
 * of the Property, followed by ":", followed by a String identifying this
 * Entity, followed by "#", followed by the number of Parts already created,
 * converted to a String.
 * </ul>
 * 
 * @param theProp    The Property in which the reference to the Part is to be
 *                   stored.
 * @param thePartCat The Part EntityCategory of the new Part.
 * @return           The new Part
 * 
 * @throws NullPointerException     If any argument is null
 * @throws IllegalArgumentException If the EntityCategory of theProp does not
 *                                  match the EntityCategory of this Entity.
 * @throws IllegalStateException    If the Embodiment for this Entity is not
 *                                  associated with a WIT-J Problem.
 * @throws witj.StatusQuoxception   If thrown by Part.newInstance (...).
 *******************************************************************************
 */
   public final Part newPart (Property <Part> theProp, Part.Category thePartCat)
      {
      int    nParts;
      String thePartName;
      Part   thePart;
      
      banNull      (theProp,    "theProp");
      banNull      (thePartCat, "thePartCat");
      reqCatsMatch (theProp,    "newPart (theProp, thePartCat)");
      reqProb      ("newPart");
      
      nParts      = getProblem ().getNPartsCreated ();
      
      thePartName = makeName (theProp, nParts);
      
      thePart     = Part.newInstance (getProblem (), thePartName, thePartCat);

      theProp.setValueFor (this, thePart);
      
      return thePart;
      }
   
/*******************************************************************************
 * Creates a new WIT-J Operation and stores a reference to it as the value of a
 * Property of this Entity. The Operation will belong to the WIT-J Problem
 * associated with the Embodiment for this Entity. The name of the Operation is
 * computed by the naming algorithm used by newPart, except that the number of
 * Operations created is used in the name.
 * 
 * @param theProp The Property in which the reference to the Operation is to be
 *                stored.
 * @return        The new Operation
 * 
 * @throws NullPointerException     If any argument is null
 * @throws IllegalArgumentException If the EntityCategory of theProp does not
 *                                  match the EntityCategory of this Entity.
 * @throws IllegalStateException    If the Embodiment for this Entity is not
 *                                  associated with a WIT-J Problem.
 * @throws witj.StatusQuoxception   If thrown by Operation.newInstance (...).
 *******************************************************************************
 */
   public final Operation newOperation (Property <Operation> theProp)
      {
      int       nOpns;
      String    theOpnName;
      Operation theOpn;
      
      banNull      (theProp, "theProp");
      reqCatsMatch (theProp, "newOperation (theProp)");
      reqProb      ("newOperation");

      
      nOpns      = getProblem ().getNOperationsCreated ();
      
      theOpnName = makeName (theProp, nOpns);

      theOpn     = Operation.newInstance (getProblem (), theOpnName);

      theProp.setValueFor (this, theOpn);

      return theOpn;
      }

/*******************************************************************************
 * Creates a new WIT-J Demand and stores a reference to it as the value of a
 * Property of this Entity. The name of the Demand is computed by the naming
 * algorithm used by newPart, except that the number of Demands created is used
 * in the name.
 * 
 * @param theProp The Property in which the reference to the Demand is to be
 *                stored.
 * @param thePart The Part with which the Demand is to be associated
 * @return        The new Demand
 * 
 * @throws NullPointerException     If any argument is null
 * @throws IllegalArgumentException If the EntityCategory of theProp does not
 *                                  match the EntityCategory of this Entity.
 * @throws witj.StatusQuoxception   If thrown by Demand.newInstance (...).
 *******************************************************************************
 */
   public final Demand newDemand (Property <Demand> theProp, Part thePart)
      {
      int    nDemands;
      String theDemandName;
      Demand theDemand;
      
      banNull      (theProp, "theProp");
      banNull      (thePart, "thePart");
      reqCatsMatch (theProp, "newDemand (theProp, thePart)");
      
      nDemands      = thePart.getProblem ().getNDemandsCreated ();
      
      theDemandName = makeName (theProp, nDemands);
      
      theDemand     = Demand.newInstance (thePart, theDemandName);

      theProp.setValueFor (this, theDemand);
      
      return theDemand;
      }

/*******************************************************************************
 * Creates a new BomEntry in the current Problem and stores a reference to it as
 * the value of a Property of this Entity.
 * 
 * @param theProp The Property in which the reference to the BomEntry is to be
 *                stored.
 * @param theOpn  The Operation with which the BomEntry is to be associated
 * @param thePart The Part with which the BomEntry is to be associated
 * @return        The new BomEntry
 * 
 * @throws NullPointerException     If any argument is null
 * @throws IllegalArgumentException If the EntityCategory of theProp does not
 *                                  match the EntityCategory of this Entity.
 * @throws witj.StatusQuoxception   If thrown by BomEntry.newInstance (...).
 *******************************************************************************
 */
   public final BomEntry newBomEntry (
         Property <BomEntry> theProp,
         Operation           theOpn,
         Part                thePart)
      {
      BomEntry theBomEnt;
      
      banNull      (theProp, "theProp");
      banNull      (theOpn,  "theOpn");
      banNull      (thePart, "thePart");
      reqCatsMatch (theProp, "newBomEntry (theProp, theOpn, thePart)");
      
      theBomEnt = BomEntry.newInstance (theOpn, thePart);
      
      theProp.setValueFor (this, theBomEnt);
      
      return theBomEnt;
      }

/*******************************************************************************
 * Creates a new BopEntry in the current Problem and stores a reference to it as
 * the value of a Property of this Entity.
 * 
 * @param theProp The Property in which the reference to the BopEntry is to be
 *                stored.
 * @param theOpn  The Operation with which the BopEntry is to be associated
 * @param thePart The Part with which the BopEntry is to be associated
 * @return        The new BopEntry
 * 
 * @throws NullPointerException     If any argument is null
 * @throws IllegalArgumentException If the EntityCategory of theProp does not
 *                                  match the EntityCategory of this Entity.
 * @throws witj.StatusQuoxception   If thrown by BopEntry.newInstance (...).
 *******************************************************************************
 */
   public final BopEntry newBopEntry (
         Property <BopEntry> theProp,
         Operation           theOpn,
         Part                thePart)
      {
      BopEntry theBopEnt;
      
      banNull      (theProp, "theProp");
      banNull      (theOpn,  "theOpn");
      banNull      (thePart, "thePart");
      reqCatsMatch (theProp, "newBopEntry (theProp, theOpn, thePart)");
      
      theBopEnt = BopEntry.newInstance (theOpn, thePart);
      
      theProp.setValueFor (this, theBopEnt);
      
      return theBopEnt;
      }

/*******************************************************************************
 * Creates a new Substitute in the current Problem and stores a reference to it
 * as the value of a Property of this Entity.
 * 
 * @param theProp   The Property in which the reference to the Substitute is to
 *                  be stored.
 * @param theBomEnt The BomEntry with which the Substitute is to be associated
 * @param thePart   The Part with which the Substitute is to be associated
 * @return          The new Substitute
 * 
 * @throws NullPointerException     If any argument is null
 * @throws IllegalArgumentException If the EntityCategory of theProp does not
 *                                  match the EntityCategory of this Entity.
 * @throws witj.StatusQuoxception   If thrown by Substitue.newInstance (...).
 *******************************************************************************
 */
   public final Substitute newSubstitute (
         Property <Substitute> theProp,
         BomEntry              theBomEnt,
         Part                  thePart)
      {
      Substitute theSub;
      
      banNull      (theProp,   "theProp");
      banNull      (theBomEnt, "theBomEnt");
      banNull      (thePart,   "thePart");
      reqCatsMatch (theProp,   "newSubstitute (theProp, theBomEnt, thePart)");
      
      theSub = Substitute.newInstance (theBomEnt, thePart);
      
      theProp.setValueFor (this, theSub);
      
      return theSub;
      }

////////////////////////////////////////////////////////////////////////////////
}
