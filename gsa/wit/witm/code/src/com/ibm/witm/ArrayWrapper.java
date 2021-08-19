
////////////////////////////////////////////////////////////////////////////////
// WIT-M Source File ArrayWrapper.java.
////////////////////////////////////////////////////////////////////////////////

package com.ibm.witm;

/*******************************************************************************
 * An ArrayWrapper {@literal<E>} provides generic access to an array of
 * primitives of type P, where E is the wrapper class for type P.
 *******************************************************************************
 */
abstract class ArrayWrapper <E>
{
////////////////////////////////////////////////////////////////////////////////
// Package Internal Non-Static Methods
////////////////////////////////////////////////////////////////////////////////
   
/*******************************************************************************
 * @return The length of the array.
 *******************************************************************************
 */
   abstract int length ();

/*******************************************************************************
 * @return the array as an Object.
 *******************************************************************************
 */
   abstract Object getArrayObject ();

/*******************************************************************************
 * Sets the element of the array indexed by theIdx to theValue.
 *******************************************************************************
 */
   abstract void setElement (int theIdx, E theValue);

/*******************************************************************************
 * @return The element of the array indexed by theIdx.
 *******************************************************************************
 */
   abstract E getElement (int theIdx);

////////////////////////////////////////////////////////////////////////////////
// Package Internal Static Methods
////////////////////////////////////////////////////////////////////////////////
   
/*******************************************************************************
 * Creates and returns a new ArrayWrapper for an array of doubles.
 *******************************************************************************
 */
   static ArrayWrapper <Double> newArrayWrapper (final double[] theDblArr)
      {
      return new ArrayWrapper <Double> ()
         {
         private final double[] myDblArray = theDblArr;
         
            @Override
         int length ()
            {
            return myDblArray.length;
            }

               @Override
         Object getArrayObject ()
            {
            return myDblArray;
            }

             @Override
         void setElement (int theIdx, Double theValue)
            {
            myDblArray[theIdx] = theValue;
            }
      
               @Override
         Double getElement (int theIdx)
            {
            return myDblArray[theIdx];
            }
         };
      }

/*******************************************************************************
 * Creates and returns a new ArrayWrapper for an array of ints.
 *******************************************************************************
 */
   static ArrayWrapper <Integer> newArrayWrapper (final int[] theIntArr)
      {
      return new ArrayWrapper <Integer> ()
         {
         private final int[] myIntArray = theIntArr;
         
            @Override
         int length ()
            {
            return myIntArray.length;
            }
         
               @Override
         Object getArrayObject ()
            {
            return myIntArray;
            }

             @Override
         void setElement (int theIdx, Integer theValue)
            {
            myIntArray[theIdx] = theValue;
            }
      
               @Override
         Integer getElement (int theIdx)
            {
            return myIntArray[theIdx];
            }
         };
      }

/*******************************************************************************
 * Creates and returns a new ArrayWrapper for an array of booleans.
 *******************************************************************************
 */
   static ArrayWrapper <Boolean> newArrayWrapper (final boolean[] theBoolArr)
      {
      return new ArrayWrapper <Boolean> ()
         {
         private final boolean[] myBoolArray = theBoolArr;
         
            @Override
         int length ()
            {
            return myBoolArray.length;
            }
         
               @Override
         Object getArrayObject ()
            {
            return myBoolArray;
            }

             @Override
         void setElement (int theIdx, Boolean theValue)
            {
            myBoolArray[theIdx] = theValue;
            }
      
               @Override
         Boolean getElement (int theIdx)
            {
            return myBoolArray[theIdx];
            }
         };
      }

////////////////////////////////////////////////////////////////////////////////
}
